// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Translate from GLSL/GXSL to HLSL code.
*/

#include "Engine/PipelineCompiler/Shaders/ShaderCompiler_Translator.h"
#include "Engine/PipelineCompiler/Shaders/ShaderCompiler_NameValidator.h"
#include "Engine/PipelineCompiler/hlsl/hlsl_source_vfs.h"

namespace PipelineCompiler
{

	//
	// HLSL Language
	//

	class HLSL_DstLanguage : public Translator::IDstLanguage
	{
	// types
	private:
		using StringMap_t		= HashMap< String, String >;
		using SamplerMap_t		= HashMap< String, String >;	// { texture name, sampler name }
		using StReaderMap_t		= HashMap< String, String >;	// { typename, reader }
		using NamedExternals_t	= Map< SymbolID, String >;		// used for unnameed buffers

		struct StructInfo
		{
			String		reader;
			String		writer;
		};
		using StructRWMap_t		= HashMap< String, StructInfo >;	// { typename, reader/writer }


	// variables
	private:
		StringMap_t						_builtinMap;
		SamplerMap_t					_samplerMap;
		NamedExternals_t				_externalNames;
		ShaderVarNameValidator			_nameValidator;
		const EShader::type				_shaderType;
		const glslang::TIntermediate*	_intermediate;
		StructRWMap_t					_structBufferRW;


	// methods
	public:
		HLSL_DstLanguage (EShader::type shaderType, const glslang::TIntermediate* intermediate);

		~HLSL_DstLanguage ()
		{}

		bool TranslateLocalVar (const Symbol &, INOUT String &src) override;
		bool TranslateStruct (const TypeInfo &, INOUT String &src) override;
		bool TranslateType (const TypeInfo &, INOUT String &src) override;
		bool TranslateName (const Symbol &, INOUT String &src) override;
		
		bool ValidateSymbolName (INOUT Symbol &, uint fnScopeId) override;
		bool ValidateFieldName (INOUT TypeInfo &) override;

		bool TranslateFunctionDecl (StringCRef sign, const TypeInfo &resultType, ArrayCRef<Symbol> args, INOUT String &src) override;
		bool TranslateFunctionCall (StringCRef sign, const TypeInfo &resultType, ArrayCRef<String> args, ArrayCRef<TypeInfo const*> argTypes, INOUT String &src) override;
		bool MustBeInlined (StringCRef sign, ArrayCRef<TypeInfo const*> argTypes) override;
		
		bool TranslateConstant (const glslang::TConstUnionArray &, const TypeInfo &, INOUT String &src) override;
		bool TranslateExternal (glslang::TIntermTyped*, const Symbol &, INOUT String &src) override;
		bool TranslateOperator (glslang::TOperator op, const TypeInfo &resultType, ArrayCRef<String> args, ArrayCRef<TypeInfo const*> argTypes, INOUT String &src) override;
		bool TranslateSwizzle (const TypeInfo &result, const TypeInfo &valType, StringCRef val, ArrayCRef<uint> swizzle, INOUT String &src) override;
		
		bool TranslateEntry (const TypeInfo &ret, StringCRef sign, ArrayCRef<Symbol> args, StringCRef body, OUT String &entryPoint, INOUT String &src) override;
		bool TranslateStructAccess (SymbolID id, const TypeInfo &stType, StringCRef objName, const TypeInfo &fieldType, INOUT String &src) override;

		bool TranslateBufferLoad (SymbolID id, StringCRef objName, StringCRef fieldOffset, const TypeInfo &fieldType, INOUT String &src) override;
		bool TranslateBufferStore (SymbolID id, StringCRef objName, StringCRef fieldOffset, const TypeInfo &fieldType,
								   StringCRef dataStr, const TypeInfo &dataType, INOUT String &src) override;
		
		bool TranslateValue (VariantCRef value, INOUT String &src) const override;
		
		bool DeclExternalTypes () const override		{ return false; }
		bool ReplaceStructByBuffer () const override	{ return true; }
		
		bool AddSource (const glslang::TIntermediate* intermediate, Translator &translator) const;

	private:
		String _TranslateFunctionName (StringCRef sign);
		bool _TranslateArg (const Symbol &t, INOUT String &res);
		bool _TranslateField (const TypeInfo &t, INOUT String &res);
		
		bool _CreateStructRW (TypeInfo const& info);

		bool _TranslateBuffer (Symbol const& info, OUT String &str);
		bool _TranslateImage (Symbol const& info, OUT String &str);
		bool _TranslateVarying (glslang::TType const& type, Symbol const& info, OUT String &str);
		bool _TranslateConst (glslang::TIntermTyped* typed, Symbol const& info, OUT String &str);
		bool _TranslateGlobal (glslang::TIntermTyped* typed, Symbol const& info, OUT String &str);
		bool _TranslateShared (Symbol const& info, OUT String &str);
		
		bool _TranslateOperator0 (glslang::TOperator op, const TypeInfo &resultType, INOUT String &src);
		bool _TranslateOperator1 (glslang::TOperator op, const TypeInfo &resultType, ArrayCRef<String> args, ArrayCRef<TypeInfo const*> argTypes, StringCRef allArgs, INOUT String &src);
		bool _TranslateOperator2 (glslang::TOperator op, const TypeInfo &resultType, ArrayCRef<String> args, ArrayCRef<TypeInfo const*> argTypes, StringCRef allArgs, INOUT String &src);
		bool _TranslateOperatorN (glslang::TOperator op, const TypeInfo &resultType, ArrayCRef<String> args, ArrayCRef<TypeInfo const*> argTypes, StringCRef allArgs, INOUT String &src);

		bool _RecursiveInitConstStruct (const Array<TypeInfo> &fields, const glslang::TConstUnionArray& cu_arr, INOUT int &index, OUT String &src);

		static String ToStringHLSL (EShaderVariable::type value);
		static String ImageToStringHLSL (EShaderVariable::type value, EShaderMemoryModel::type memModel);
		static String ToStringHLSL (EShaderMemoryModel::type value);
	};

	

/*
=================================================
	_TranslateGXSLtoHLSL
=================================================
*/
	bool ShaderCompiler::_TranslateGXSLtoHLSL (const Config &cfg, const _GLSLangResult &glslangData, OUT String &log, OUT BinaryArray &result) const
	{
		CHECK_ERR(	EShaderFormat::GetApiFormat( cfg.source ) == EShaderFormat::GXSL or
					EShaderFormat::GetApiFormat( cfg.source ) == EShaderFormat::GLSL or
					EShaderFormat::GetApiFormat( cfg.source ) == EShaderFormat::VKSL );

		CHECK_ERR(	cfg.target == EShaderFormat::HLSL_11 );
	
		// not supported here
		ASSERT( not cfg.optimize );

		const glslang::TIntermediate* intermediate = glslangData.prog.getIntermediate( glslangData.shader->getStage() );
		CHECK_ERR( intermediate );

		Translator		translator{ log };
		TIntermNode*	root		= intermediate->getTreeRoot();
		EShader::type	shader_type	= ConvertShaderType( glslangData.shader->getStage() );
		auto			lang		= new HLSL_DstLanguage( shader_type, intermediate );

		translator.states.useGXrules	= intermediate->getSource() == glslang::EShSourceGxsl;
		translator.states.inlineAll		= true;		// access to builtins allowed only in entry point and it is not compatible with other shader languages, so inline all to avoid some problems
		translator.entryPoint			= intermediate->getEntryPointName().c_str();
		translator.language				= lang;
		
		CHECK_ERR( translator.Main( root, cfg.skipExternals ) );

		CHECK_ERR( lang->AddSource( intermediate, translator ) );

		result	= BinArrayCRef::From( translator.src );
		return true;
	}
//-----------------------------------------------------------------------------

	
/*
=================================================
	constructor
=================================================
*/
	HLSL_DstLanguage::HLSL_DstLanguage (EShader::type shaderType, const glslang::TIntermediate* intermediate) :
		_nameValidator{EShaderFormat::HLSL_11}, _shaderType{shaderType}, _intermediate{intermediate}
	{
		switch ( _shaderType )
		{
			case EShader::Vertex :
			{
				break;
			}

			case EShader::TessControl :
			{
				break;
			}

			case EShader::TessEvaluation :
			{
				break;
			}

			case EShader::Geometry :
			{
				break;
			}

			case EShader::Fragment :
			{
				break;
			}

			case EShader::Compute :
			{
				const uint3	local_size { _intermediate->getLocalSize(0), _intermediate->getLocalSize(1), _intermediate->getLocalSize(2) };

				_builtinMap.Add( "gl_LocalInvocationIndex",	"(dx_GroupThreadID.x + dx_GroupThreadID.y * "_str << local_size.x << " + dx_GroupThreadID.z * " << (local_size.x * local_size.y) << ')' );
				_builtinMap.Add( "gl_GlobalInvocationID",	"dx_DispatchThreadID" );
				_builtinMap.Add( "gl_LocalInvocationID",	"dx_GroupThreadID" );
				_builtinMap.Add( "gl_NumWorkGroups",		"dx_NumWorkGroups" );	// part of builtin cbuffer
				_builtinMap.Add( "gl_WorkGroupID",			"dx_GroupID" );
				_builtinMap.Add( "gl_WorkGroupSize",		"uint3("_str << local_size.x << ", " << local_size.y << ", " << local_size.z << ")" );
				break;
			}
		}
	}
	
/*
=================================================
	AddSource
=================================================
*/
	bool HLSL_DstLanguage::AddSource (const glslang::TIntermediate* intermediate, Translator &translator) const
	{
		String	src;

		CHECK_ERR( hlsl_vfs::LoadFile( "vload.hl", INOUT src ) );
		src << "\n";

		switch ( _shaderType )
		{
			case EShader::Vertex :
			{
				break;
			}

			case EShader::TessControl :
			{
				break;
			}

			case EShader::TessEvaluation :
			{
				break;
			}

			case EShader::Geometry :
			{
				break;
			}

			case EShader::Fragment :
			{
				break;
			}

			case EShader::Compute :
			{
				src << "cbuffer ComputeBuiltins : register(b0)\n{\n"	// TODO: unique register index
					<< "\tuint3		dx_NumWorkGroups;\n"
					<< "};\n\n";
				break;
			}
		}

		src >> translator.src;
		return true;
	}

/*
=================================================
	TranslateLocalVar
=================================================
*/
	bool HLSL_DstLanguage::TranslateLocalVar (const Symbol &t, INOUT String &res)
	{
		CHECK_ERR( not t.name.Empty() );

		// read-only
		if ( t.qualifier[ EVariableQualifier::Constant ] )
			res << "const ";

		// type
		if ( not t.typeName.Empty() ) {
			res << t.typeName;
		} else {
			res << ToStringHLSL( t.type );
		}

		res << " " << t.name << (t.arraySize.IsNotArray() ? "" : (t.arraySize.IsDynamicArray() ? "[]" : "["_str << t.arraySize.Size() << "]"));
		return true;
	}
	
/*
=================================================
	_TranslateField
=================================================
*/
	bool HLSL_DstLanguage::_TranslateField (const TypeInfo &t, INOUT String &res)
	{
		// type
		if ( not t.typeName.Empty() ) {
			res << t.typeName;
		} else {
			res << ToStringHLSL( t.type );
		}

		res << " " << t.name << (t.arraySize.IsNotArray() ? "" : (t.arraySize.IsDynamicArray() ? "[]" : "["_str << t.arraySize.Size() << "]"));
		return true;
	}

/*
=================================================
	TranslateStruct
=================================================
*/
	bool HLSL_DstLanguage::TranslateStruct (const TypeInfo &info, INOUT String &src)
	{
		src << "struct " << info.typeName << "\n{\n";

		for (auto& fld : info.fields)
		{
			src << "\t";
			CHECK_ERR( _TranslateField( fld, INOUT src ) );
			src << ";\n";
		}

		src << "};\n\n";
		return true;
	}
	
/*
=================================================
	TranslateType
=================================================
*/
	bool HLSL_DstLanguage::TranslateType (const TypeInfo &t, INOUT String &res)
	{
		// type
		if ( not t.typeName.Empty() ) {
			res << t.typeName;
		} else {
			res << ToStringHLSL( t.type );
		}

		res << (t.arraySize.IsNotArray() ? "" : (t.arraySize.IsDynamicArray() ? "[]" : "["_str << t.arraySize.Size() << "]"));
		return true;
	}
	
/*
=================================================
	TranslateName
=================================================
*/
	bool HLSL_DstLanguage::TranslateName (const Symbol &t, INOUT String &src)
	{
		// convert builtin names
		StringMap_t::iterator	iter;

		if ( _builtinMap.Find( t.name, OUT iter ) )
		{
			src << iter->second;
			return true;
		}

		src << t.name;
		return true;
	}
	
/*
=================================================
	ValidateSymbolName
=================================================
*/
	bool HLSL_DstLanguage::ValidateSymbolName (INOUT Symbol &symb, uint fnScopeId)
	{
		// builtin
		if ( symb.name.StartsWithIC( "gl_" ) )
			return true;

		if ( symb.qualifier[ EVariableQualifier::Local ] or
			 symb.qualifier[ EVariableQualifier::InArg ] or
			 symb.qualifier[ EVariableQualifier::OutArg ] )
		{
			return _nameValidator.ValidateLocalVar( INOUT symb.name, uint(symb.id), fnScopeId );
		}

		if ( symb.qualifier[ EVariableQualifier::In ]		 or
			 symb.qualifier[ EVariableQualifier::Out ]		 or
			 not symb.qualifier[ EVariableQualifier::Local ] or	// there is not 'global' flag, so if it is not a local or argument then it is global
			 symb.isGlobal )
		{
			return _nameValidator.ValidateGlobalVar( INOUT symb.name, uint(symb.id) );
		}

		return true;
	}
	
/*
=================================================
	ValidateFieldName
=================================================
*/
	bool HLSL_DstLanguage::ValidateFieldName (INOUT TypeInfo &info)
	{
		return _nameValidator.ValidateField( INOUT info.name );
	}
	
/*
=================================================
	_TranslateFunctionName
=================================================
*/
	String HLSL_DstLanguage::_TranslateFunctionName (StringCRef signature)
	{
		String	name;
		CHECK( _nameValidator.ValidateFunction( signature, OUT name ) );

		return name;
	}

/*
=================================================
	TranslateExternal
=================================================
*/
	bool HLSL_DstLanguage::TranslateExternal (glslang::TIntermTyped* typed, const Symbol &info, INOUT String &str)
	{
		glslang::TType const&	type	= typed->getType();
		
		if ( EShaderVariable::IsBuffer( info.type ) ) {
			CHECK_ERR( _TranslateBuffer( info, INOUT str ) );
			CHECK_ERR( _CreateStructRW( info ) );
		}
		else
		if ( EShaderVariable::IsImage( info.type ) or EShaderVariable::IsTexture( info.type ) ) {
			CHECK_ERR( _TranslateImage( info, INOUT str ) );
		}
		else
		if ( info.qualifier[ EVariableQualifier::Shared ] ) {
			CHECK_ERR( _TranslateShared( info, INOUT str ) );
		}
		else
		if ( info.type == EShaderVariable::SubpassInput ) {
			Symbol	image = info;
			image.type = EShaderVariable::Image2D;
			CHECK_ERR( _TranslateImage( image, INOUT str ) );
		}
		else
		if ( info.qualifier[ EVariableQualifier::In ] or info.qualifier[ EVariableQualifier::Out ] ) {
			CHECK_ERR( _TranslateVarying( type, info, INOUT str ) );
		}
		else
		if ( info.qualifier[ EVariableQualifier::Constant ] ) {
			CHECK_ERR( _TranslateConst( typed, info, INOUT str ) );
		}
		else
		if ( type.getQualifier().storage == glslang::TStorageQualifier::EvqGlobal ) {
			CHECK_ERR( _TranslateGlobal( typed, info, INOUT str ) );
		}
		else {
			RETURN_ERR( "unknown type" );
		}
		return true;
	}
	
/*
=================================================
	TranslateOperator
=================================================
*/
	bool HLSL_DstLanguage::TranslateOperator (glslang::TOperator op, const TypeInfo &resultType, ArrayCRef<String> args, ArrayCRef<TypeInfo const*> argTypes, INOUT String &src)
	{
		CHECK_ERR( args.Count() == argTypes.Count() );

		String	all_args;

		FOR( i, args ) {
			all_args << (i ? ", " : "( ") << args[i];
		}
		all_args << " )";

		if ( op >= glslang::TOperator::EOpConvInt8ToBool and op <= glslang::TOperator::EOpConvDoubleToFloat )
		{
			CHECK_ERR( args.Count() == 1 );
			CHECK_ERR( TranslateType( resultType, INOUT src ) );
			src << all_args;
			return true;
		}

		if ( op >= glslang::TOperator::EOpConstructGuardStart and op < glslang::TOperator::EOpConstructGuardEnd )
		{
			CHECK_ERR( resultType.arraySize.IsNotArray() );
			CHECK_ERR( TranslateType( resultType, INOUT src ) );

			const uint	vec_size = EShaderVariable::VecSize( resultType.type );
			if ( vec_size > 0 )
			{
				uint	arg_size = 0;
				for (auto& t : argTypes) {
					arg_size += EShaderVariable::VecSize( t->type );
				}

				if ( vec_size == arg_size )
					src << all_args;
				else
				if ( vec_size == 2 )
				{
					RETURN_ERR( "not supported" );
				}
				else
				if ( vec_size == 3 )
				{
					RETURN_ERR( "not supported" );
				}
				else
				if ( vec_size == 4 )
				{
					if ( arg_size == 1 )	src << '(' << all_args << ".xxxx)";		else
					RETURN_ERR( "not supported" );
				}
				else
					RETURN_ERR( "not supported" );
			}
			else
				RETURN_ERR( "not supported" );

			return true;
		}
		
		if ( args.Count() == 0 )
		{
			CHECK_ERR( _TranslateOperator0( op, resultType, INOUT src ) );
			return true;
		}
		
		if ( args.Count() == 1 )
		{
			CHECK_ERR( _TranslateOperator1( op, resultType, args, argTypes, all_args, INOUT src ) );
			return true;
		}
		
		if ( args.Count() == 2 )
		{
			CHECK_ERR( _TranslateOperator2( op, resultType, args, argTypes, all_args, INOUT src ) );
			return true;
		}
			
		CHECK_ERR( _TranslateOperatorN( op, resultType, args, argTypes, all_args, INOUT src ) );
		return true;
	}
	
/*
=================================================
	_TranslateOperator0
=================================================
*/
	bool HLSL_DstLanguage::_TranslateOperator0 (glslang::TOperator op, const TypeInfo &, INOUT String &src)
	{
		switch ( op )
		{
			//case glslang::TOperator::EOpEmitVertex :				src << "EmitVertex()";			break;
			//case glslang::TOperator::EOpEndPrimitive :				src << "EndPrimitive()";		break;
			case glslang::TOperator::EOpBarrier :					src << "GroupMemoryBarrierWithGroupSync()";		break;	// TODO: check
			//case glslang::TOperator::EOpMemoryBarrier :				src << "memoryBarrier()";		break;
			//case glslang::TOperator::EOpMemoryBarrierAtomicCounter:	src << "memoryBarrierAtomicCounter()";break;
			//case glslang::TOperator::EOpMemoryBarrierBuffer :		src << "memoryBarrierBuffer()";	break;
			//case glslang::TOperator::EOpMemoryBarrierImage :		src << "memoryBarrierImage()";	break;
			//case glslang::TOperator::EOpMemoryBarrierShared :		src << "memoryBarrierShared()";	break;
			//case glslang::TOperator::EOpGroupMemoryBarrier :		src << "groupMemoryBarrier()";	break;
			default :												RETURN_ERR( "unknown operator!" );
		}
		return true;
	}
	
/*
=================================================
	_TranslateOperator1
=================================================
*/
	bool HLSL_DstLanguage::_TranslateOperator1 (glslang::TOperator op, const TypeInfo &, ArrayCRef<String>,
												ArrayCRef<TypeInfo const*>, StringCRef all_args, INOUT String &src)
	{
		switch ( op )
		{
			case glslang::TOperator::EOpNegative :				src << "-" << all_args;				break;
			case glslang::TOperator::EOpLogicalNot :			src << "!" << all_args;				break;
			case glslang::TOperator::EOpVectorLogicalNot :		src << "! " << all_args;			break;
			case glslang::TOperator::EOpBitwiseNot :			src << "~" << all_args;				break;
			case glslang::TOperator::EOpPostIncrement :			src << all_args << "++";			break;
			case glslang::TOperator::EOpPostDecrement :			src << all_args << "--";			break;
			case glslang::TOperator::EOpPreIncrement :			src << "++" << all_args;			break;
			case glslang::TOperator::EOpPreDecrement :			src << "--" << all_args;			break;
			//case glslang::TOperator::EOpArrayLength :			src << all_args << ".length()";		break;
			case glslang::TOperator::EOpAny :					src << "any" << all_args;			break;
			case glslang::TOperator::EOpAll :					src << "all" << all_args;			break;
			case glslang::TOperator::EOpRadians :				src << "radians" << all_args;		break;
			case glslang::TOperator::EOpDegrees :				src << "degrees" << all_args;		break;
			case glslang::TOperator::EOpSin :					src << "sin" << all_args;			break;
			case glslang::TOperator::EOpCos :					src << "cos" << all_args;			break;
			case glslang::TOperator::EOpTan :					src << "tan" << all_args;			break;
			case glslang::TOperator::EOpAsin :					src << "asin" << all_args;			break;
			case glslang::TOperator::EOpAcos :					src << "acos" << all_args;			break;
			case glslang::TOperator::EOpAtan :					src << "atan" << all_args;			break;
			case glslang::TOperator::EOpSinh :					src << "sinh" << all_args;			break;
			case glslang::TOperator::EOpCosh :					src << "cosh" << all_args;			break;
			case glslang::TOperator::EOpTanh :					src << "tanh" << all_args;			break;
			case glslang::TOperator::EOpAsinh :					src << "asinh" << all_args;			break;
			case glslang::TOperator::EOpAcosh :					src << "acosh" << all_args;			break;
			case glslang::TOperator::EOpAtanh :					src << "atanh" << all_args;			break;
			case glslang::TOperator::EOpExp :					src << "exp" << all_args;			break;
			case glslang::TOperator::EOpLog :					src << "log" << all_args;			break;
			case glslang::TOperator::EOpExp2 :					src << "exp2" << all_args;			break;
			case glslang::TOperator::EOpLog2 :					src << "log2" << all_args;			break;
			case glslang::TOperator::EOpSqrt :					src << "sqrt" << all_args;			break;
			case glslang::TOperator::EOpInverseSqrt :			src << "rsqrt" << all_args;			break;
			case glslang::TOperator::EOpAbs :					src << "abs" << all_args;			break;
			case glslang::TOperator::EOpSign :					src << "sign" << all_args;			break;
			case glslang::TOperator::EOpFloor :					src << "floor" << all_args;			break;
			case glslang::TOperator::EOpTrunc :					src << "trunc" << all_args;			break;
			case glslang::TOperator::EOpRound :					src << "round" << all_args;			break;
			//case glslang::TOperator::EOpRoundEven :				src << "roundEven" << all_args;		break;
			case glslang::TOperator::EOpCeil :					src << "ceil" << all_args;			break;
			case glslang::TOperator::EOpFract :					src << "frac" << all_args;			break;
			case glslang::TOperator::EOpIsNan :					src << "isnan" << all_args;			break;
			case glslang::TOperator::EOpIsInf :					src << "isinf" << all_args;			break;
			case glslang::TOperator::EOpFloatBitsToInt :		src << "asint" << all_args;			break;
			case glslang::TOperator::EOpFloatBitsToUint :		src << "asuint" << all_args;			break;
			case glslang::TOperator::EOpIntBitsToFloat :		src << "asfloat" << all_args;			break;
			case glslang::TOperator::EOpUintBitsToFloat :		src << "asfloat" << all_args;			break;
			//case glslang::TOperator::EOpDoubleBitsToInt64 :		src << "doubleBitsToInt64" << all_args;			break;
			//case glslang::TOperator::EOpDoubleBitsToUint64 :	src << "doubleBitsToUint64" << all_args;		break;
			//case glslang::TOperator::EOpInt64BitsToDouble :		src << "int64BitsToDouble" << all_args;			break;
			//case glslang::TOperator::EOpUint64BitsToDouble :	src << "uint64BitsToDouble" << all_args;		break;
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpFloat16BitsToInt16 :	src << "float16BitsToInt16" << all_args;		break;
			//case glslang::TOperator::EOpFloat16BitsToUint16 :	src << "float16BitsToUint16" << all_args;		break;
			//case glslang::TOperator::EOpInt16BitsToFloat16 :	src << "int16BitsToFloat16" << all_args;		break;
			//case glslang::TOperator::EOpUint16BitsToFloat16 :	src << "uint16BitsToFloat16" << all_args;		break;
			#endif
			//case glslang::TOperator::EOpPackSnorm2x16 :			src << "packSnorm2x16" << all_args;				break;
			//case glslang::TOperator::EOpUnpackSnorm2x16 :		src << "unpackSnorm2x16" << all_args;			break;
			//case glslang::TOperator::EOpPackUnorm2x16 :			src << "packUnorm2x16" << all_args;				break;
			//case glslang::TOperator::EOpUnpackUnorm2x16 :		src << "unpackUnorm2x16" << all_args;			break;
			//case glslang::TOperator::EOpPackSnorm4x8 :			src << "packSnorm4x8" << all_args;				break;
			//case glslang::TOperator::EOpUnpackSnorm4x8 :		src << "unpackSnorm4x8" << all_args;			break;
			//case glslang::TOperator::EOpPackUnorm4x8 :			src << "packUnorm4x8" << all_args;				break;
			//case glslang::TOperator::EOpUnpackUnorm4x8 :		src << "unpackUnorm4x8" << all_args;			break;
			//case glslang::TOperator::EOpPackHalf2x16 :			src << "packHalf2x16" << all_args;				break;
			//case glslang::TOperator::EOpUnpackHalf2x16 :		src << "unpackHalf2x16" << all_args;			break;
			//case glslang::TOperator::EOpPackDouble2x32 :		src << "packDouble2x32" << all_args;			break;
			//case glslang::TOperator::EOpUnpackDouble2x32 :		src << "unpackDouble2x32" << all_args;			break;
			//case glslang::TOperator::EOpPackInt2x32 :			src << "packInt2x32" << all_args;				break;
			//case glslang::TOperator::EOpUnpackInt2x32 :			src << "unpackInt2x32" << all_args;				break;
			//case glslang::TOperator::EOpPackUint2x32 :			src << "packUint2x32" << all_args;				break;
			//case glslang::TOperator::EOpUnpackUint2x32 :		src << "unpackUint2x32" << all_args;			break;
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpPackFloat2x16 :			src << "packFloat2x16" << all_args;				break;
			//case glslang::TOperator::EOpUnpackFloat2x16 :		src << "unpackFloat2x16" << all_args;			break;
			//case glslang::TOperator::EOpPackInt2x16 :			src << "packInt2x16" << all_args;				break;
			//case glslang::TOperator::EOpUnpackInt2x16 :			src << "unpackInt2x16" << all_args;				break;
			//case glslang::TOperator::EOpPackUint2x16 :			src << "packUint2x16" << all_args;				break;
			//case glslang::TOperator::EOpUnpackUint2x16 :		src << "unpackUint2x16" << all_args;			break;
			//case glslang::TOperator::EOpPackInt4x16 :			src << "packInt4x16" << all_args;				break;
			//case glslang::TOperator::EOpUnpackInt4x16 :			src << "unpackInt4x16" << all_args;				break;
			//case glslang::TOperator::EOpPackUint4x16 :			src << "packUint4x16" << all_args;				break;
			//case glslang::TOperator::EOpUnpackUint4x16 :		src << "unpackUint4x16" << all_args;			break;
			#endif
			case glslang::TOperator::EOpLength :				src << "length" << all_args;					break;
			case glslang::TOperator::EOpNormalize :				src << "normalize" << all_args;					break;
			case glslang::TOperator::EOpDPdx :					src << "ddx" << all_args;						break;
			case glslang::TOperator::EOpDPdy :					src << "ddy" << all_args;						break;
			case glslang::TOperator::EOpFwidth :				src << "fwidth" << all_args;					break;
			case glslang::TOperator::EOpDPdxFine :				src << "ddx_fine" << all_args;					break;
			case glslang::TOperator::EOpDPdyFine :				src << "ddy_fine" << all_args;					break;
			//case glslang::TOperator::EOpFwidthFine :			src << "fwidthFine" << all_args;				break;
			case glslang::TOperator::EOpDPdxCoarse :			src << "ddx_coarse" << all_args;				break;
			case glslang::TOperator::EOpDPdyCoarse :			src << "ddy_coarse" << all_args;				break;
			//case glslang::TOperator::EOpFwidthCoarse :			src << "fwidthCoarse" << all_args;				break;
			//case glslang::TOperator::EOpInterpolateAtCentroid :	src << "interpolateAtCentroid" << all_args;		break;
			case glslang::TOperator::EOpBitFieldReverse :		src << "reversebits" << all_args;			break;
			case glslang::TOperator::EOpBitCount :				src << "countbits" << all_args;					break;
			case glslang::TOperator::EOpFindLSB :				src << "firstbitlow" << all_args;					break;
			case glslang::TOperator::EOpFindMSB :				src << "firstbithigh" << all_args;					break;
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpInterpolateAtVertex :	src << "atanh" << all_args;						break;
			#endif
			case glslang::TOperator::EOpDeterminant :			src << "determinant" << all_args;				break;
			case glslang::TOperator::EOpMatrixInverse :			src << "inverse" << all_args;					break;
			case glslang::TOperator::EOpTranspose :				src << "transpose" << all_args;					break;
			//case glslang::TOperator::EOpEmitStreamVertex :		src << "EmitStreamVertex" << all_args;			break;
			//case glslang::TOperator::EOpEndStreamPrimitive :	src << "EndStreamPrimitive" << all_args;		break;

			//case glslang::TOperator::EOpImageQuerySize :		src << "imageSize" << all_args;					break;
			//case glslang::TOperator::EOpImageQuerySamples :		src << "imageSamples" << all_args;				break;
			//case glslang::TOperator::EOpNoise :					src << "noise" << all_args;						break;	// deprecated
					
			//case glslang::TOperator::EOpAtomicCounter :			src << "atomicCounter" << all_args;				break;
			//case glslang::TOperator::EOpAtomicCounterIncrement:	src << "atomicCounterIncrement" << all_args;	break;
			//case glslang::TOperator::EOpAtomicCounterDecrement:	src << "atomicCounterDecrement" << all_args;	break;

			//case glslang::TOperator::EOpTextureQuerySize :		src << "textureSize" << all_args;				break;
			//case glslang::TOperator::EOpTextureQueryLevels :	src << "textureQueryLevels" << all_args;		break;
			//case glslang::TOperator::EOpTextureQuerySamples :	src << "textureSamples" << all_args;			break;
			default :											RETURN_ERR( "unknown operator!" );
		}
		return true;
	}
	
/*
=================================================
	_TranslateOperator2
=================================================
*/
	bool HLSL_DstLanguage::_TranslateOperator2 (glslang::TOperator op, const TypeInfo &resultType, ArrayCRef<String> args,
												ArrayCRef<TypeInfo const*> argTypes, StringCRef all_args, INOUT String &src)
	{
		using vinfo = _platforms_hidden_::EValueTypeInfo;
		
		const bool	is_vec	=	(resultType.arraySize.IsNotArray() and EShaderVariable::VecSize( resultType.type ) > 1)			and
								(argTypes[0]->arraySize.IsNotArray() and EShaderVariable::VecSize( argTypes[0]->type ) >= 1)	and
								(argTypes[1]->arraySize.IsNotArray() and EShaderVariable::VecSize( argTypes[1]->type ) >= 1)	/*and
								(EShaderVariable::VecSize( argTypes[0]->type ) > 1 or EShaderVariable::VecSize( argTypes[1]->type ) > 1)*/;

		const bool	is_float = (EShaderVariable::IsFloat( argTypes[0]->type ) or EShaderVariable::IsFloat( argTypes[1]->type ));

		if ( is_vec )
		{
			const auto &	lhs_type = *argTypes[0];
			const auto &	rhs_type = *argTypes[1];
			const uint		lhs_vec	 = EShaderVariable::VecSize( lhs_type.type );
			const uint		rhs_vec	 = EShaderVariable::VecSize( rhs_type.type );
			const uint		vec_size = Max( EShaderVariable::VecSize( resultType.type ), lhs_vec, rhs_vec );
			const auto		sh_type  = EShaderVariable::ToVec( lhs_type.type, vec_size );
			String			lhs, rhs;
			
			const auto		MakeSwizzleMask = LAMBDA( vec_size ) (uint size)
			{{
				String	mask = ".";
				for (uint i = 0; i < vec_size; ++i) { mask << ("xyzw"[ i % size ]); }
				return mask;
			}};
			
			if ( lhs_type.type != sh_type )		lhs << '(' << args[0] << ')' << MakeSwizzleMask( lhs_vec );
			else								lhs << args[0];

			if ( rhs_type.type != sh_type )		rhs << '(' << args[1] << ')' << MakeSwizzleMask( rhs_vec );
			else								rhs << args[1];
			
			switch ( op )
			{
				case glslang::TOperator::EOpVectorEqual :				src << '('<<lhs <<" == "<< rhs<<')';	return true;
				case glslang::TOperator::EOpVectorNotEqual :			src << '('<<lhs <<" != "<< rhs<<')';	return true;
				case glslang::TOperator::EOpAdd :						src << '('<<lhs <<" + " << rhs<<')';	return true;
				case glslang::TOperator::EOpSub :						src << '('<<lhs <<" - " << rhs<<')';	return true;
				case glslang::TOperator::EOpMul :						src << '('<<lhs <<" * " << rhs<<')';	return true;
				case glslang::TOperator::EOpDiv :						src << '('<<lhs <<" / " << rhs<<')';	return true;
				case glslang::TOperator::EOpMod :						src << '('<<lhs <<" % " << rhs<<')';	return true;
				case glslang::TOperator::EOpRightShift :				src << '('<<lhs <<" >> "<< rhs<<')';	return true;
				case glslang::TOperator::EOpLeftShift :					src << '('<<lhs <<" << "<< rhs<<')';	return true;
				case glslang::TOperator::EOpAnd :						src << '('<<lhs <<" & " << rhs<<')';	return true;
				case glslang::TOperator::EOpInclusiveOr :				src << '('<<lhs <<" | " << rhs<<')';	return true;
				case glslang::TOperator::EOpExclusiveOr :				src << '('<<lhs <<" ^ " << rhs<<')';	return true;
				case glslang::TOperator::EOpEqual :						src << '('<<lhs <<" == "<< rhs<<')';	return true;
				case glslang::TOperator::EOpNotEqual :					src << '('<<lhs <<" != "<< rhs<<')';	return true;
				case glslang::TOperator::EOpLessThan :					src << '('<<lhs <<" < " << rhs<<')';	return true;
				case glslang::TOperator::EOpGreaterThan :				src << '('<<lhs <<" > " << rhs<<')';	return true;
				case glslang::TOperator::EOpLessThanEqual :				src << '('<<lhs <<" <= "<< rhs<<')';	return true;
				case glslang::TOperator::EOpGreaterThanEqual :			src << '('<<lhs <<" >= "<< rhs<<')';	return true;
				case glslang::TOperator::EOpVectorTimesScalar :			src << '('<<lhs <<" * " << rhs<<')';	return true;
				case glslang::TOperator::EOpAssign :					src <<      lhs <<" = " << rhs;			return true;
				case glslang::TOperator::EOpAddAssign :					src << '('<<lhs <<" += "<< rhs<<')';	return true;
				case glslang::TOperator::EOpSubAssign :					src << '('<<lhs <<" -= "<< rhs<<')';	return true;
				case glslang::TOperator::EOpMulAssign :					src << '('<<lhs <<" *= "<< rhs<<')';	return true;
				case glslang::TOperator::EOpVectorTimesScalarAssign :	src << '('<<lhs <<" *= "<< rhs<<')';	return true;
				case glslang::TOperator::EOpDivAssign :					src << '('<<lhs <<" /= "<< rhs<<')';	return true;
				case glslang::TOperator::EOpModAssign :					src << '('<<lhs <<" %= "<< rhs<<')';	return true;
				case glslang::TOperator::EOpAndAssign :					src << '('<<lhs <<" &= "<< rhs<<')';	return true;
				case glslang::TOperator::EOpInclusiveOrAssign :			src << '('<<lhs <<" |= "<< rhs<<')';	return true;
				case glslang::TOperator::EOpExclusiveOrAssign :			src << '('<<lhs <<" ^= "<< rhs<<')';	return true;
				case glslang::TOperator::EOpLeftShiftAssign :			src << '('<<lhs <<" <<= "<< rhs<<')';	return true;
				case glslang::TOperator::EOpRightShiftAssign :			src << '('<<lhs <<" >>= "<< rhs<<')';	return true;
			}
		}

		if ( is_float )
		{
			switch ( op )
			{
				case glslang::TOperator::EOpMod :					src << "mod("<<args[0]<<", "<<args[1]<<')';		return true;
			}
		}

		switch ( op )
		{
			case glslang::TOperator::EOpAdd :						src << '('<<args[0] <<" + " << args[1]<<')';	break;
			case glslang::TOperator::EOpSub :						src << '('<<args[0] <<" - " << args[1]<<')';	break;
			case glslang::TOperator::EOpMul :						src << '('<<args[0] <<" * " << args[1]<<')';	break;
			case glslang::TOperator::EOpDiv :						src << '('<<args[0] <<" / " << args[1]<<')';	break;
			case glslang::TOperator::EOpMod :						src << '('<<args[0] <<" % " << args[1]<<')';	break;
			case glslang::TOperator::EOpRightShift :				src << '('<<args[0] <<" >> "<< args[1]<<')';	break;
			case glslang::TOperator::EOpLeftShift :					src << '('<<args[0] <<" << "<< args[1]<<')';	break;
			case glslang::TOperator::EOpAnd :						src << '('<<args[0] <<" & " << args[1]<<')';	break;
			case glslang::TOperator::EOpInclusiveOr :				src << '('<<args[0] <<" | " << args[1]<<')';	break;
			case glslang::TOperator::EOpExclusiveOr :				src << '('<<args[0] <<" ^ " << args[1]<<')';	break;
			case glslang::TOperator::EOpEqual :						src << '('<<args[0] <<" == "<< args[1]<<')';	break;
			case glslang::TOperator::EOpNotEqual :					src << '('<<args[0] <<" != "<< args[1]<<')';	break;
			case glslang::TOperator::EOpLessThan :					src << '('<<args[0] <<" < " << args[1]<<')';	break;
			case glslang::TOperator::EOpGreaterThan :				src << '('<<args[0] <<" > " << args[1]<<')';	break;
			case glslang::TOperator::EOpLessThanEqual :				src << '('<<args[0] <<" <= "<< args[1]<<')';	break;
			case glslang::TOperator::EOpGreaterThanEqual :			src << '('<<args[0] <<" >= "<< args[1]<<')';	break;
			case glslang::TOperator::EOpVectorTimesScalar :			src << '('<<args[0] <<" * " << args[1]<<')';	break;
			case glslang::TOperator::EOpVectorTimesMatrix :			src << "mul("<<args[0] <<", " << args[1]<<')';	break;
			case glslang::TOperator::EOpMatrixTimesVector :			src << "mul("<<args[0] <<", " << args[1]<<')';	break;
			case glslang::TOperator::EOpMatrixTimesScalar :			src << "mul("<<args[0] <<", " << args[1]<<')';	break;
			case glslang::TOperator::EOpLogicalOr :					src << '('<<args[0] <<" || "<< args[1]<<')';	break;
			case glslang::TOperator::EOpLogicalXor :				src << '('<<args[0] <<" ^^ "<< args[1]<<')';	break;
			case glslang::TOperator::EOpLogicalAnd :				src << '('<<args[0] <<" && "<< args[1]<<')';	break;
			case glslang::TOperator::EOpIndexDirect :				src << '('<<args[0] <<'[' << args[1]<<"])";		break;
			case glslang::TOperator::EOpIndexIndirect :				src << '('<<args[0] <<'[' << args[1]<<"])";		break;
			//case glslang::TOperator::EOpMethod :					break;
			//case glslang::TOperator::EOpScoping :					break;
			case glslang::TOperator::EOpAssign :					src <<      args[0] <<" = " << args[1];			break;
			case glslang::TOperator::EOpAddAssign :					src << '('<<args[0] <<" += "<< args[1]<<')';	break;
			case glslang::TOperator::EOpSubAssign :					src << '('<<args[0] <<" -= "<< args[1]<<')';	break;
			case glslang::TOperator::EOpMulAssign :					src << '('<<args[0] <<" *= "<< args[1]<<')';	break;
			//case glslang::TOperator::EOpVectorTimesMatrixAssign :	src << '('<<args[0] <<" *= "<< args[1]<<')';	break;
			case glslang::TOperator::EOpVectorTimesScalarAssign :	src << '('<<args[0] <<" *= "<< args[1]<<')';	break;
			//case glslang::TOperator::EOpMatrixTimesScalarAssign :	src << '('<<args[0] <<" *= "<< args[1]<<')';	break;
			//case glslang::TOperator::EOpMatrixTimesMatrixAssign :	src << '('<<args[0] <<" *= "<< args[1]<<')';	break;
			case glslang::TOperator::EOpDivAssign :					src << '('<<args[0] <<" /= "<< args[1]<<')';	break;
			case glslang::TOperator::EOpModAssign :					src << '('<<args[0] <<" %= "<< args[1]<<')';	break;
			case glslang::TOperator::EOpAndAssign :					src << '('<<args[0] <<" &= "<< args[1]<<')';	break;
			case glslang::TOperator::EOpInclusiveOrAssign :			src << '('<<args[0] <<" |= "<< args[1]<<')';	break;
			case glslang::TOperator::EOpExclusiveOrAssign :			src << '('<<args[0] <<" ^= "<< args[1]<<')';	break;
			case glslang::TOperator::EOpLeftShiftAssign :			src << '('<<args[0] <<" <<= "<< args[1]<<')';	break;
			case glslang::TOperator::EOpRightShiftAssign :			src << '('<<args[0] <<" >>= "<< args[1]<<')';	break;

			case glslang::TOperator::EOpAtan :						src << "atan" << all_args;						break;
			case glslang::TOperator::EOpImageLoad : {
				switch ( argTypes[0]->type & vinfo::_SAMP_MASK )
				{
					case vinfo::_SAMP_BUF :
					case vinfo::_SAMP_1D :
						src << args[0] << ".Load(" << args[1] << ')';	// int
						break;
					case vinfo::_SAMP_1DA :
						src << args[0] << ".Load(int3(" << args[1] << ", 0).xzy)";	// int3
						break;
					case vinfo::_SAMP_2D :
						src << args[0] << ".Load(int3(" << args[1] << ", 0))";	// int3
						break;
					case vinfo::_SAMP_3D :
					case vinfo::_SAMP_2DA :
						src << args[0] << ".Load(" << args[1] << ')';	// int4
						break;
					default :
						RETURN_ERR( "not supported!" );
				}
				break;
			}
			case glslang::TOperator::EOpPow :						src << "pow" << all_args;						break;
			case glslang::TOperator::EOpModf :						src << "modf" << all_args;						break;
			case glslang::TOperator::EOpMin :						src << "min" << all_args;						break;
			case glslang::TOperator::EOpMax :						src << "max" << all_args;						break;
			case glslang::TOperator::EOpStep :						src << "step" << all_args;						break;
			case glslang::TOperator::EOpFrexp :						src << "frexp" << all_args;						break;
			case glslang::TOperator::EOpLdexp :						src << "ldexp" << all_args;						break;
			case glslang::TOperator::EOpDistance :					src << "distance" << all_args;					break;
			case glslang::TOperator::EOpDot :						src << "dot" << all_args;						break;
			case glslang::TOperator::EOpCross :						src << "cross" << all_args;						break;
			case glslang::TOperator::EOpReflect :					src << "reflect" << all_args;					break;
			//case glslang::TOperator::EOpInterpolateAtSample :		src << "interpolateAtSample" << all_args;		break;
			//case glslang::TOperator::EOpInterpolateAtOffset :		src << "interpolateAtOffset" << all_args;		break;
			//case glslang::TOperator::EOpOuterProduct :				src << "outerProduct" << all_args;				break;
			//case glslang::TOperator::EOpMatrixTimesMatrix :			src << "matrixCompMult" << all_args;			break;

			case glslang::TOperator::EOpAtomicAdd :					src << "atomicAdd" << all_args;					break;
			//case glslang::TOperator::EOpAtomicMin :					src << "atomicMin" << all_args;					break;
			//case glslang::TOperator::EOpAtomicMax :					src << "atomicMax" << all_args;					break;
			//case glslang::TOperator::EOpAtomicAnd :					src << "atomicAnd" << all_args;					break;
			//case glslang::TOperator::EOpAtomicOr :					src << "atomicOr" << all_args;					break;
			//case glslang::TOperator::EOpAtomicXor :					src << "atomicXor" << all_args;					break;
			//case glslang::TOperator::EOpAtomicExchange :			src << "atomicExchange" << all_args;			break;
			//case glslang::TOperator::EOpAtomicCounterAdd :			src << "atomicCounterAdd" << all_args;			break;
			//case glslang::TOperator::EOpAtomicCounterSubtract :		src << "atomicCounterSub" << all_args;			break;
			//case glslang::TOperator::EOpAtomicCounterMin :			src << "atomicCounterMin" << all_args;			break;
			//case glslang::TOperator::EOpAtomicCounterMax :			src << "atomicCounterMax" << all_args;			break;
			//case glslang::TOperator::EOpAtomicCounterAnd :			src << "atomicCounterAnd" << all_args;			break;
			//case glslang::TOperator::EOpAtomicCounterOr :			src << "atomicCounterOr" << all_args;			break;
			//case glslang::TOperator::EOpAtomicCounterXor :			src << "atomicCounterXor" << all_args;			break;
			//case glslang::TOperator::EOpAtomicCounterExchange :		src << "atomicCounterExchange" << all_args;		break;

			//case glslang::TOperator::EOpTextureQuerySize :			src << "textureSize" << all_args;				break;
			//case glslang::TOperator::EOpTextureQueryLod :			src << "textureQueryLod" << all_args;			break;

			case glslang::TOperator::EOpTexture :{
				SamplerMap_t::iterator	iter;
				CHECK_ERR( _samplerMap.Find( args[0], OUT iter ) );

				if ( _shaderType == EShader::Fragment )
					src << args[0] << ".Sample(" << iter->second << ", " << args[1] << ')';
				else
					src << args[0] << ".SampleLevel(" << iter->second << ", " << args[1] << ", 0.0f)";
				break;
			}
			//case glslang::TOperator::EOpTextureProj :				src << "textureProj" << all_args;				break;
			//case glslang::TOperator::EOpTextureFetch :				src << "texelFetch" << all_args;				break;
			//case glslang::TOperator::EOpTextureGather :				src << "textureGather" << all_args;				break;
			default :												RETURN_ERR( "unknown operator!" );
		}
		return true;
	}
	
/*
=================================================
	_TranslateOperatorN
=================================================
*/
	bool HLSL_DstLanguage::_TranslateOperatorN (glslang::TOperator op, const TypeInfo &, ArrayCRef<String> args,
												ArrayCRef<TypeInfo const*>, StringCRef all_args, INOUT String &src)
	{
		switch ( op )
		{
			//case glslang::TOperator::EOpImageLoad :					src << "imageLoad" << all_args;				break;	// 3 args (multisample)
			case glslang::TOperator::EOpImageStore :				src << '('<<args[0]<<")["<<args[1]<<"] = "<<args[2];	break;	// 3,4 args (multisample)
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpImageLoadLod :			src << "imageLoadLod" << all_args;			break;
			//case glslang::TOperator::EOpImageStoreLod :			src << "imageStoreLod" << all_args;			break;
			#endif
			//case glslang::TOperator::EOpImageAtomicAdd :			src << "imageAtomicAdd" << all_args;		break;	// 3,4 args
			//case glslang::TOperator::EOpImageAtomicMin :			src << "imageAtomicMin" << all_args;		break;	// 3,4 args
			//case glslang::TOperator::EOpImageAtomicMax :			src << "imageAtomicMax" << all_args;		break;	// 3,4 args
			//case glslang::TOperator::EOpImageAtomicAnd :			src << "imageAtomicAnd" << all_args;		break;	// 3,4 args
			//case glslang::TOperator::EOpImageAtomicOr :				src << "imageAtomicOr" << all_args;			break;	// 3,4 args
			//case glslang::TOperator::EOpImageAtomicXor :			src << "imageAtomicXor" << all_args;		break;	// 3,4 args
			//case glslang::TOperator::EOpImageAtomicExchange :		src << "imageAtomicExchange" << all_args;	break;	// 3,4 args
			//case glslang::TOperator::EOpImageAtomicCompSwap :		src << "imageAtomicCompSwap" << all_args;	break;	// 4,5 args
			//case glslang::TOperator::EOpSubpassLoad :				src << "image(";	break;
			//case glslang::TOperator::EOpSubpassLoadMS :			src << "image(";	break;
			//case glslang::TOperator::EOpSparseImageLoad :			src << "image(";	break;
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpSparseImageLoadLod :		src << "image(";	break;
			#endif
			case glslang::TOperator::EOpClamp :						src << "clamp" << all_args;					break;	// 3 args
			case glslang::TOperator::EOpMix :						src << "lerp" << all_args;					break;	// 3 args
			case glslang::TOperator::EOpSmoothStep :				src << "smoothstep" << all_args;			break;	// 3 args
			case glslang::TOperator::EOpFma :						src << "fma" << all_args;					break;	// 3 args
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpFloat16BitsToInt16,
			//case glslang::TOperator::EOpFloat16BitsToUint16,
			//case glslang::TOperator::EOpInt16BitsToFloat16,
			//case glslang::TOperator::EOpUint16BitsToFloat16,
			#endif
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpPackFloat2x16,
			//case glslang::TOperator::EOpUnpackFloat2x16,
			//case glslang::TOperator::EOpPackInt2x16,
			//case glslang::TOperator::EOpUnpackInt2x16,
			//case glslang::TOperator::EOpPackUint2x16,
			//case glslang::TOperator::EOpUnpackUint2x16,
			//case glslang::TOperator::EOpPackInt4x16,
			//case glslang::TOperator::EOpUnpackInt4x16,
			//case glslang::TOperator::EOpPackUint4x16,
			//case glslang::TOperator::EOpUnpackUint4x16,
			#endif
			case glslang::TOperator::EOpFaceForward :				src << "faceForward" << all_args;		break;	// 3 args
			case glslang::TOperator::EOpRefract :					src << "refract" << all_args;			break;	// 3 args
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpMin3,
			//case glslang::TOperator::EOpMax3,
			//case glslang::TOperator::EOpMid3,
			#endif
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpInterpolateAtVertex,
			#endif
			//case glslang::TOperator::EOpFtransform :				src << "ftransform" << all_args;			break;	// WTF?
			//case glslang::TOperator::EOpBallot :					src << "ballotARB" << all_args;				break;	// WTF?
			//case glslang::TOperator::EOpReadInvocation :			src << "readInvocation" << all_args;		break;	// WTF?
			//case glslang::TOperator::EOpReadFirstInvocation :		src << "readFirstInvocatin" << all_args;	break;	// WTF?
			//case glslang::TOperator::EOpAnyInvocation :				src << "anyInvocation" << all_args;			break;	// WTF?
			//case glslang::TOperator::EOpAllInvocations :			src << "allInvocations" << all_args;		break;	// WTF?
			//case glslang::TOperator::EOpAllInvocationsEqual :		src << "allInvocationsEqual" << all_args;	break;	// WTF?
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpMinInvocations,
			//case glslang::TOperator::EOpMaxInvocations,
			//case glslang::TOperator::EOpAddInvocations,
			//case glslang::TOperator::EOpMinInvocationsNonUniform,
			//case glslang::TOperator::EOpMaxInvocationsNonUniform,
			//case glslang::TOperator::EOpAddInvocationsNonUniform,
			//case glslang::TOperator::EOpMinInvocationsInclusiveScan,
			//case glslang::TOperator::EOpMaxInvocationsInclusiveScan,
			//case glslang::TOperator::EOpAddInvocationsInclusiveScan,
			//case glslang::TOperator::EOpMinInvocationsInclusiveScanNonUniform,
			//case glslang::TOperator::EOpMaxInvocationsInclusiveScanNonUniform,
			//case glslang::TOperator::EOpAddInvocationsInclusiveScanNonUniform,
			//case glslang::TOperator::EOpMinInvocationsExclusiveScan,
			//case glslang::TOperator::EOpMaxInvocationsExclusiveScan,
			//case glslang::TOperator::EOpAddInvocationsExclusiveScan,
			//case glslang::TOperator::EOpMinInvocationsExclusiveScanNonUniform,
			//case glslang::TOperator::EOpMaxInvocationsExclusiveScanNonUniform,
			//case glslang::TOperator::EOpAddInvocationsExclusiveScanNonUniform,
			//case glslang::TOperator::EOpSwizzleInvocations,
			//case glslang::TOperator::EOpSwizzleInvocationsMasked,
			//case glslang::TOperator::EOpWriteInvocation,
			//case glslang::TOperator::EOpMbcnt,
			//case glslang::TOperator::EOpCubeFaceIndex,
			//case glslang::TOperator::EOpCubeFaceCoord,
			//case glslang::TOperator::EOpTime,
			#endif
			//case glslang::TOperator::EOpAtomicCompSwap :			src << "atomicCompSwap" << all_args;			break;	// 3 args
			//case glslang::TOperator::EOpAtomicCounterCompSwap :		src << "atomicCounterCompSwap" << all_args;		break;	// 3 args

			//case glslang::TOperator::EOpAddCarry :					src << "uaddCarry" << all_args;					break;	// 3 args
			//case glslang::TOperator::EOpSubBorrow :					src << "usubBorrow" << all_args;				break;	// 3 args
			//case glslang::TOperator::EOpUMulExtended :				src << "umulExtended" << all_args;				break;	// 4 args
			//case glslang::TOperator::EOpIMulExtended :				src << "imulExtended" << all_args;				break;	// 4 args
			//case glslang::TOperator::EOpBitfieldExtract :			src << "bitfieldExtract" << all_args;			break;	// 3 args
			//case glslang::TOperator::EOpBitfieldInsert :			src << "bitfieldInsert" << all_args;			break;	// 4 args

			//case glslang::TOperator::EOpTexture :					src << "texture" << all_args;					break;	// 3 args
			//case glslang::TOperator::EOpTextureProj :				src << "textureProj" << all_args;				break;	// 3 args
			//case glslang::TOperator::EOpTextureLod :				src << "textureLod" << all_args;				break;	// 3 args
			//case glslang::TOperator::EOpTextureOffset :				src << "textureOffset" << all_args;				break;	// 3,4 args
			//case glslang::TOperator::EOpTextureFetch :				src << "texelFetch" << all_args;				break;	// 3 args
			//case glslang::TOperator::EOpTextureFetchOffset :		src << "texelFetchOffset" << all_args;			break;	// 3,4 args
			//case glslang::TOperator::EOpTextureProjOffset :			src << "textureProjOffset" << all_args;			break;	// 3,4 args
			//case glslang::TOperator::EOpTextureLodOffset :			src << "textureLodOffset" << all_args;			break;	// 4 args
			//case glslang::TOperator::EOpTextureProjLod :			src << "textureProjLod" << all_args;			break;	// 3 args
			//case glslang::TOperator::EOpTextureProjLodOffset :		src << "textureProjLodOffset" << all_args;		break;	// 4 args
			//case glslang::TOperator::EOpTextureGrad :				src << "textureGrad" << all_args;				break;	// 4 args
			//case glslang::TOperator::EOpTextureGradOffset :			src << "textureGradOffset" << all_args;			break;	// 5 args
			//case glslang::TOperator::EOpTextureProjGrad :			src << "textureProjGrad" << all_args;			break;	// 4 args
			//case glslang::TOperator::EOpTextureProjGradOffset :		src << "textureProjGradOffset" << all_args;		break;	// 5 args
			//case glslang::TOperator::EOpTextureGather :				src << "textureGather" << all_args;				break;	// 3 args
			//case glslang::TOperator::EOpTextureGatherOffset :		src << "textureGatherOffset" << all_args;		break;	// 3,4 args
			//case glslang::TOperator::EOpTextureGatherOffsets :		src << "textureGatherOffsets" << all_args;		break;	// 3,4 args
			//case glslang::TOperator::EOpTextureClamp :			break;
			//case glslang::TOperator::EOpTextureOffsetClamp :		break;
			//case glslang::TOperator::EOpTextureGradClamp :		break;
			//case glslang::TOperator::EOpTextureGradOffsetClamp :	break;
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpTextureGatherLod :			break;
			//case glslang::TOperator::EOpTextureGatherLodOffset :		break;
			//case glslang::TOperator::EOpTextureGatherLodOffsets :		break;
			#endif
			//case glslang::TOperator::EOpSparseTexture :				break;
			//case glslang::TOperator::EOpSparseTextureLod :			break;
			//case glslang::TOperator::EOpSparseTextureOffset :			break;
			//case glslang::TOperator::EOpSparseTextureFetch :			break;
			//case glslang::TOperator::EOpSparseTextureFetchOffset :	break;
			//case glslang::TOperator::EOpSparseTextureLodOffset :		break;
			//case glslang::TOperator::EOpSparseTextureGrad :			break;
			//case glslang::TOperator::EOpSparseTextureGradOffset :		break;
			//case glslang::TOperator::EOpSparseTextureGather :			break;
			//case glslang::TOperator::EOpSparseTextureGatherOffset :	break;
			//case glslang::TOperator::EOpSparseTextureGatherOffsets :	break;
			//case glslang::TOperator::EOpSparseTexelsResident :		break;
			//case glslang::TOperator::EOpSparseTextureClamp :			break;
			//case glslang::TOperator::EOpSparseTextureOffsetClamp :	break;
			//case glslang::TOperator::EOpSparseTextureGradClamp :		break;
			//case glslang::TOperator::EOpSparseTextureGradOffsetClamp :	break;
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpSparseTextureGatherLod :			break;
			//case glslang::TOperator::EOpSparseTextureGatherLodOffset :	break;
			//case glslang::TOperator::EOpSparseTextureGatherLodOffsets :	break;
			#endif

			default :												RETURN_ERR( "unknown builtin function!" );
		}
		return true;
	}

/*
=================================================
	TranslateFunctionCall
=================================================
*/
	bool HLSL_DstLanguage::TranslateFunctionCall (StringCRef signature, const TypeInfo &, ArrayCRef<String> args, ArrayCRef<TypeInfo const*>, INOUT String &src)
	{
		src << _TranslateFunctionName( signature ) << '(';

		FOR( i, args ) {
			src << (i ? ", " : "") << args[i];
		}
		src << ')';
		return true;
	}
	
/*
=================================================
	MustBeInlined
=================================================
*/
	bool HLSL_DstLanguage::MustBeInlined (StringCRef, ArrayCRef<TypeInfo const*>)
	{
		return false;
	}

/*
=================================================
	_TranslateArg
=================================================
*/
	bool HLSL_DstLanguage::_TranslateArg (const Symbol &t, INOUT String &res)
	{
		// qualifies
		if ( t.qualifier[ EVariableQualifier::Constant ] )
		{}
		else
		if ( t.qualifier[ EVariableQualifier::InArg ] and t.qualifier[ EVariableQualifier::OutArg ] )
			res << "inout ";
		else
		if ( t.qualifier[ EVariableQualifier::InArg ] )
			res << "in ";
		else
		if ( t.qualifier[ EVariableQualifier::OutArg ] )
			res << "out ";

		return TranslateLocalVar( t, INOUT res );
	}
	
/*
=================================================
	TranslateFunctionDecl
=================================================
*/
	bool HLSL_DstLanguage::TranslateFunctionDecl (StringCRef signature, const TypeInfo &resultType, ArrayCRef<Symbol> args, INOUT String &src)
	{
		CHECK_ERR( TranslateType( resultType, INOUT src ) );

		src << ' ' << _TranslateFunctionName( signature ) << " (";
		
		FOR( i, args )
		{
			src << (i ? ", " : "");
			CHECK_ERR( _TranslateArg( args[i], OUT src ) );
		}

		src << ')';
		return true;
	}
	
/*
=================================================
	TranslateSwizzle
=================================================
*/
	bool HLSL_DstLanguage::TranslateSwizzle (const TypeInfo &, const TypeInfo &, StringCRef val, ArrayCRef<uint> swizzle, INOUT String &src)
	{
		src << val << '.';
		
		for (auto i : swizzle) {
			src << ("xyzw"[i]);
		}
		return true;
	}
	
/*
=================================================
	TranslateEntry
=================================================
*/
	bool HLSL_DstLanguage::TranslateEntry (const TypeInfo &, StringCRef signature, ArrayCRef<Symbol>, StringCRef body, OUT String &entryPoint, INOUT String &src)
	{
		String	attribs;
		String	kernel_args;

		entryPoint = _TranslateFunctionName( signature );

		src.Clear();
		
		for (auto& rw : _structBufferRW) {
			src << rw.second.reader << '\n' << rw.second.writer << '\n';
		}

		switch ( _shaderType )
		{
			case EShader::Vertex :
			{
				break;
			}
			case EShader::TessControl :
			{
				break;
			}
			case EShader::TessEvaluation :
			{
				break;
			}
			case EShader::Geometry :
			{
				break;
			}
			case EShader::Fragment :
			{
				break;
			}
			case EShader::Compute :
			{
				attribs << "[numthreads(" << _intermediate->getLocalSize(0) << ", " << _intermediate->getLocalSize(1) << ", " << _intermediate->getLocalSize(2) << ")]\n";
				kernel_args << "uint3 dx_DispatchThreadID : SV_DispatchThreadID, uint3 dx_GroupThreadID : SV_GroupThreadID, uint3 dx_GroupID : SV_GroupID";
				break;
			}
			default :
				RETURN_ERR( "unsupported shader type!" );
		}
		
		String	body_src = body;
		StringParser::IncreaceIndent( INOUT body_src );

		src << attribs << "void " << entryPoint << " (" << kernel_args << ")\n{\n" << body_src << "}\n\n";

		return true;
	}
	
/*
=================================================
	TranslateStructAccess
=================================================
*/
	bool HLSL_DstLanguage::TranslateStructAccess (SymbolID, const TypeInfo &stType, StringCRef objName, const TypeInfo &fieldType, INOUT String &src)
	{
		if ( EShaderVariable::IsBuffer( stType.type ) )
		{
			RETURN_ERR( "structured buffer is not supported" );
		}

		if ( not objName.Empty() )
			src << objName << ".";

		src << fieldType.name;
		return true;
	}
	
/*
=================================================
	TranslateBufferLoad
=================================================
*/
	bool HLSL_DstLanguage::TranslateBufferLoad (SymbolID id, StringCRef objName, StringCRef fieldOffset, const TypeInfo &fieldType, INOUT String &src)
	{
		if ( objName.Empty() )
		{
			NamedExternals_t::iterator	iter;
			CHECK_ERR( _externalNames.Find( id, OUT iter ) );

			objName = iter->second;
		}

		const uint	vec_size	= EShaderVariable::VecSize( fieldType.type );
		const uint2	mat_size	= EShaderVariable::MatSize( fieldType.type );
		const auto	scalar_type	= EShaderVariable::ToScalar( fieldType.type );

		if ( fieldType.arraySize.IsArray() )
		{
			src << "<not supported>";
			return true;
		}
		else
		if ( vec_size > 0 )
		{
			src << "vload_" << ToStringHLSL( scalar_type ) << vec_size << '(' << fieldOffset << ", " << objName << ')';
			return true;
		}
		else
		if ( All( mat_size > 0 ) )
		{
			src << "vload_" << ToStringHLSL( scalar_type ) << mat_size.x << 'x' << mat_size.y << '(' << fieldOffset << ", " << objName << ')';
			return true;
		}
		else
		if ( not fieldType.typeName.Empty() )
		{
			src << "vload_" << fieldType.typeName << '(' << fieldOffset << ", " << objName << ')';
			return true;
		}
		return false;
	}
		
/*
=================================================
	TranslateBufferStore
=================================================
*/
	bool HLSL_DstLanguage::TranslateBufferStore (SymbolID id, StringCRef objName, StringCRef fieldOffset, const TypeInfo &fieldType,
												 StringCRef dataStr, const TypeInfo &dataType, INOUT String &src)
	{
		if ( objName.Empty() )
		{
			NamedExternals_t::iterator	iter;
			CHECK_ERR( _externalNames.Find( id, OUT iter ) );

			objName = iter->second;
		}

		const uint	vec_size	= EShaderVariable::VecSize( fieldType.type );
		const uint2	mat_size	= EShaderVariable::MatSize( fieldType.type );
		const auto	scalar_type	= EShaderVariable::ToScalar( fieldType.type );

		if ( fieldType.arraySize.IsArray() )
		{
			src << "<not supported>";
			return true;
		}
		else
		if ( vec_size > 0 )
		{
			src << "vstore_" << ToStringHLSL( scalar_type ) << vec_size << '(' << dataStr << ", " << fieldOffset << ", " << objName << ')';
			return true;
		}
		else
		if ( All( mat_size > 0 ) )
		{
			src << "vstore_" << ToStringHLSL( scalar_type ) << mat_size.x << 'x' << mat_size.y << '(' << dataStr << ", " << fieldOffset << ", " << objName << ')';
			return true;
		}
		else
		if ( not fieldType.typeName.Empty() )
		{
			src << "vstore_" << fieldType.typeName << "(" << dataStr << ", " << fieldOffset << ", " << objName << ')';
			return true;
		}
		return false;
	}
	
/*
=================================================
	_CreateStructRW
=================================================
*/
	bool HLSL_DstLanguage::_CreateStructRW (TypeInfo const& info)
	{
		const auto	CreateRader = LAMBDA( this ) (const TypeInfo &info, OUT String &src) -> bool
		{{
			usize	offset = 0;
			src << info.typeName << " vload_" << info.typeName << " (uint offset, RWByteAddressBuffer ptr)\n{\n"
				<< '\t' << info.typeName << " data;\n";
			for (auto& fld : info.fields)
			{
				src << "\tdata." << fld.name << " = ";
				CHECK_ERR( TranslateBufferLoad( SymbolID(0), "ptr", "offset + "_str << offset, fld, INOUT src ) );
				src << ";\n";
				offset += usize(fld.sizeOf);
			}
			src << "\treturn data;\n}\n";
			return true;
		}};

		const auto	CreateWriter = LAMBDA( this ) (const TypeInfo &info, OUT String &src) -> bool
		{{
			usize	offset = 0;
			src << "void vstore_" << info.typeName << " (const struct " << info.typeName << " data, uint offset, RWByteAddressBuffer ptr)\n{\n";
			for (auto& fld : info.fields)
			{
				src << '\t';
				CHECK_ERR( TranslateBufferStore( SymbolID(0), "ptr", "offset + "_str << offset, fld, "data." + fld.name, fld, INOUT src ) );
				src << ";\n";
				offset += usize(fld.sizeOf);
			}
			src << "}\n";
			return true;
		}};
		

		for (auto& fld : info.fields)
		{
			if ( not fld.typeName.Empty() )
			{
				if ( _structBufferRW.IsExist( fld.typeName ) )
					continue;

				StructInfo	st;
				CHECK_ERR( CreateRader( fld, st.reader ) );
				CHECK_ERR( CreateWriter( fld, st.writer ) );

				_structBufferRW.Add( info.typeName, st );
			}
		}
		return true;
	}
	
/*
=================================================
	_TranslateBuffer
=================================================
*/
	bool HLSL_DstLanguage::_TranslateBuffer (Symbol const& info, OUT String &str)
	{
		String	buf_name = info.name;

		if ( buf_name.Empty() )
		{
			// create unique name
						buf_name	= "external_";
			const usize	len			= buf_name.Length();
				
			for (usize i = 0; i < 1000; ++i)
			{
				buf_name << (_externalNames.Count() + i);

				bool	found = true;

				for (auto& ext : _externalNames) {
					if ( ext.second == buf_name ) {
						found = false;
						break;
					}
				}

				if ( found )
					break;
			}

			_externalNames.Add( info.id, buf_name );
		}

		const bool	is_storage	= (info.type == EShaderVariable::StorageBlock);
		String		fld_str;

		CHECK_ERR( EShaderVariable::IsBuffer( info.type ) and not info.fields.Empty() );

		fld_str << "{\n";
		for (auto& fld : info.fields)
		{
			fld_str << "\t";
			CHECK_ERR( _TranslateField( fld, INOUT fld_str ) );
			fld_str << ";\n";
		}
		fld_str << "}";


		if ( is_storage )
		{
			//str << "struct " << info.typeName << fld_str << ";\n";

			if ( EShaderMemoryModel::HasWriteAccess( info.memoryModel ) )
				str << "RWByteAddressBuffer  " << buf_name;
			else
				str << "ByteAddressBuffer  " << buf_name;
			
			if ( info.binding != UMax )
				str << " : register(u" << info.binding << ")";

			str << ";\n";
		}
		else
		{
			str << "cbuffer " << info.typeName;
			
			if ( info.binding != UMax )
				str << " : register(b" << info.binding << ")";

			str << fld_str << "  " << buf_name << ";\n";
		}
		return true;
	}

/*
=================================================
	_TranslateImage
=================================================
*/
	bool HLSL_DstLanguage::_TranslateImage (Symbol const& info, OUT String &str)
	{
		CHECK_ERR( info.arraySize.IsNotArray() );
		CHECK_ERR( not info.name.Empty() );
		
		if ( EShaderVariable::IsImage( info.type ) )
		{
			if ( EShaderMemoryModel::HasWriteAccess( info.memoryModel ) )
			{
				str << "globallycoherent " << ImageToStringHLSL( info.type, info.memoryModel ) << " " << info.name;
				
				if ( info.binding != UMax )
					str << " : register(u" << info.binding << ")";
			}
			else
			{
				str << ImageToStringHLSL( info.type, info.memoryModel ) << " " << info.name;
				
				if ( info.binding != UMax )
					str << " : register(t" << info.binding << ")";
			}

			str << ";\n";
		}
		else
		{
			str << ImageToStringHLSL( info.type, EShaderMemoryModel::ReadOnly ) << " " << info.name;
			
			if ( info.binding != UMax )
				str << " : register(t" << info.binding << ")";
			
			str << ";\n";

			if ( EShaderVariable::IsShadowTexture( info.type ) )
				str << "SamplerComparisonState";
			else
				str << "SamplerState";

			const String	sampler_name = String(info.name) << "Sampler";
			
			str << " " << sampler_name;
			
			_samplerMap.Add( info.name, sampler_name );

			if ( info.binding != UMax )
				str << " : register(s" << info.binding << ")";

			str << ";\n";
		}
		return true;
	}

/*
=================================================
	_TranslateShared
=================================================
*/
	bool HLSL_DstLanguage::_TranslateShared (Symbol const& info, OUT String &str)
	{
		str << "groupshared ";
		CHECK_ERR( TranslateLocalVar( info, INOUT str ) );
		str << ";\n";
		return true;
	}

/*
=================================================
	_TranslateVarying
=================================================
*/
	bool HLSL_DstLanguage::_TranslateVarying (glslang::TType const&, Symbol const&, OUT String &)
	{
		//TODO( "" );
		return true;
	}
	
/*
=================================================
	_RecursiveInitConstStruct
=================================================
*/
	bool HLSL_DstLanguage::_RecursiveInitConstStruct (const Array<TypeInfo> &fields, const glslang::TConstUnionArray& cu_arr, INOUT int &index, OUT String &src)
	{
		DeserializedShader::Constant::ValueArray_t	values;

		src << "( ";

		FOR( i, fields )
		{
			const auto&	field	= fields[i];
			const uint	count	= field.arraySize.Size();
			const bool	is_arr	= field.arraySize.IsArray();

			CHECK_ERR( not field.arraySize.IsDynamicArray() );
			
			src << (i ? ", " : "");

			if ( is_arr )
				src << "{ ";

			if ( EShaderVariable::IsStruct( field.type ) )
			{
				for (uint j = 0; j < count; ++j) {
					CHECK_ERR( _RecursiveInitConstStruct( field.fields, cu_arr, INOUT index, INOUT src ) );
				}
			}
			else
			{
				for (uint j = 0; j < count; ++j)
				{
					values.Clear();
					CHECK_ERR( DeserializeConstant::Process( field.type, cu_arr, index, true, OUT values, OUT index ) );

					CU_ToArray_Func	func{ this, false };
					
					src << (j ? ", " : "");
					values.Front().Accept( func );

					CHECK_ERR( TranslateOperator( glslang::TOperator::EOpConstructGuardStart,
												  field, func.GetStrings(), func.GetTypes(), INOUT src ) );
				}
			}
			
			if ( is_arr )
				src << " }";
		}
		src << " )";
		return true;
	}
	
/*
=================================================
	TranslateConstant
=================================================
*/
	bool HLSL_DstLanguage::TranslateConstant (const glslang::TConstUnionArray &cu_arr, const TypeInfo &info, INOUT String &str)
	{
		// array
		if ( info.arraySize.IsArray() )
		{
			TypeInfo	scalar_info = info;		scalar_info.arraySize.MakeNonArray();

			str << "{ ";
			
			DeserializedShader::Constant::ValueArray_t	values;
			CHECK_ERR( DeserializeConstant::Process( scalar_info.type, cu_arr, OUT values ) );

			FOR( i, values )
			{
				CU_ToArray_Func	func{ this, false };

				str << (i ? ", " : "");
				values[i].Accept( func );

				CHECK_ERR( TranslateOperator( glslang::TOperator::EOpConstructGuardStart,
												scalar_info, func.GetStrings(), func.GetTypes(), INOUT str ) );
			}

			str << " }";
		}
		else
		// struct
		if ( EShaderVariable::IsStruct( info.type ) )
		{
			str << info.typeName;

			int index = 0;
			CHECK_ERR( _RecursiveInitConstStruct( info.fields, cu_arr, INOUT index, INOUT str ) );
		}
		else
		// scalar
		{
			DeserializedShader::Constant::ValueArray_t	values;
			CHECK_ERR( DeserializeConstant::Process( info.type, cu_arr, OUT values ) );

			FOR( i, values )
			{
				CU_ToArray_Func	func{ this, false };

				str << (i ? ", " : "");
				values[i].Accept( func );
				
				CHECK_ERR( TranslateOperator( glslang::TOperator::EOpConstructGuardStart,
												info, func.GetStrings(), func.GetTypes(), INOUT str ) );
			}
		}
		return true;
	}
	
/*
=================================================
	_TranslateConst
=================================================
*/
	bool HLSL_DstLanguage::_TranslateConst (glslang::TIntermTyped* typed, Symbol const& info, OUT String &str)
	{
		CHECK_ERR( typed->getAsSymbolNode() );

		glslang::TConstUnionArray const&	cu_arr	= typed->getAsSymbolNode()->getConstArray();
		CHECK_ERR( not cu_arr.empty() );

		CHECK_ERR( TranslateLocalVar( info, INOUT str ) );
		str << " = ";
		
		CHECK_ERR( TranslateConstant( cu_arr, info, INOUT str ) );
		
		str << ";\n";
		return true;
	}
	
/*
=================================================
	_TranslateGlobal
=================================================
*/
	bool HLSL_DstLanguage::_TranslateGlobal (glslang::TIntermTyped* typed, Symbol const& info, OUT String &str)
	{
		CHECK_ERR( typed->getAsSymbolNode() );

		CHECK_ERR( TranslateLocalVar( info, INOUT str ) );
		
		glslang::TConstUnionArray const&	cu_arr	= typed->getAsSymbolNode()->getConstArray();

		if ( not cu_arr.empty() )
		{
			str << " = ";
			CHECK_ERR( TranslateConstant( cu_arr, info, INOUT str ) );
		}
		
		str << ";\n";
		return true;
	}
	
/*
=================================================
	TranslateValue
=================================================
*/
	bool HLSL_DstLanguage::TranslateValue (VariantCRef value, INOUT String &src) const
	{
		src.Clear();
		
		// bool
		if ( value.GetValueTypeId() == TypeIdOf<bool>() )
		{
			src << ToString( value.Get<bool>() );
		}
		else
		// int
		if ( value.GetValueTypeId() == TypeIdOf<int>() )
		{
			const int	val = value.Get<int>();

			if ( val != MinValue(val) )
				src << ToString( val );
			else
				src << "(" << (MinValue(val)+1) << " - 1)";
		}
		else
		// long
		if ( value.GetValueTypeId() == TypeIdOf<ilong>() )
		{
			const ilong	val = value.Get<ilong>();

			TODO( "int64 literal" );

			/*if ( val != MinValue(val) )
				src << ToString( val ) << "L";
			else
				src << "(" << (MinValue(val)+1) << "L - 1)";*/
		}
		else
		// uint
		if ( value.GetValueTypeId() == TypeIdOf<uint>() )
		{
			src << ToString( value.Get<uint>() ) << "u";
		}
		else
		// ulong
		if ( value.GetValueTypeId() == TypeIdOf<ulong>() )
		{
			TODO( "uint64 literal" );

			//src << ToString( value.Get<ulong>() ) << "UL";
		}
		else
		// float
		if ( value.GetValueTypeId() == TypeIdOf<float>() )
		{
			src << String().FormatF( value.Get<float>(), StringFormatF().Fmt(0,8).CutZeros() ) << "f";
		}
		else
		// double
		if ( value.GetValueTypeId() == TypeIdOf<double>() )
		{
			src << String().FormatF( value.Get<double>(), StringFormatF().Fmt(0,16).CutZeros() ) << "L";
		}
		else
			RETURN_ERR( "unsupported scalar type" );

		return true;
	}
	
/*
=================================================
	ToStringHLSL
=================================================
*/
	String HLSL_DstLanguage::ToStringHLSL (EShaderVariable::type value)
	{
		switch ( value )
		{
			case EShaderVariable::Void :		return "void";
			case EShaderVariable::Bool :		return "bool";
			case EShaderVariable::Bool2 :		return "bool2";
			case EShaderVariable::Bool3 :		return "bool3";
			case EShaderVariable::Bool4 :		return "bool4";
			case EShaderVariable::Int :			return "int";
			case EShaderVariable::Int2 :		return "int2";
			case EShaderVariable::Int3 :		return "int3";
			case EShaderVariable::Int4 :		return "int4";
			case EShaderVariable::UInt :		return "uint";
			case EShaderVariable::UInt2 :		return "uint2";
			case EShaderVariable::UInt3 :		return "uint3";
			case EShaderVariable::UInt4 :		return "uint4";
			case EShaderVariable::Long :		return "int64_t";
			case EShaderVariable::Long2 :		return "int64_t2";
			case EShaderVariable::Long3 :		return "int64_t3";
			case EShaderVariable::Long4 :		return "int64_t4";
			case EShaderVariable::ULong :		return "uint64_t";
			case EShaderVariable::ULong2 :		return "uint64_t2";
			case EShaderVariable::ULong3 :		return "uint64_t3";
			case EShaderVariable::ULong4 :		return "uint64_t4";
			case EShaderVariable::Float :		return "float";
			case EShaderVariable::Float2 :		return "float2";
			case EShaderVariable::Float3 :		return "float3";
			case EShaderVariable::Float4 :		return "float4";
			case EShaderVariable::Double :		return "double";
			case EShaderVariable::Double2 :		return "double2";
			case EShaderVariable::Double3 :		return "double3";
			case EShaderVariable::Double4 :		return "double4";

			// software matrix types
			case EShaderVariable::Float2x2 :	return "float2x2";
			case EShaderVariable::Float2x3 :	return "float2x3";
			case EShaderVariable::Float2x4 :	return "float2x4";
			case EShaderVariable::Float3x2 :	return "float3x2";
			case EShaderVariable::Float3x3 :	return "float3x3";
			case EShaderVariable::Float3x4 :	return "float3x4";
			case EShaderVariable::Float4x2 :	return "float4x2";
			case EShaderVariable::Float4x3 :	return "float4x3";
			case EShaderVariable::Float4x4 :	return "float4x4";
			case EShaderVariable::Double2x2 :	return "double2x2";
			case EShaderVariable::Double2x3 :	return "double2x3";
			case EShaderVariable::Double2x4 :	return "double2x4";
			case EShaderVariable::Double3x2 :	return "double3x2";
			case EShaderVariable::Double3x3 :	return "double3x3";
			case EShaderVariable::Double3x4 :	return "double3x4";
			case EShaderVariable::Double4x2 :	return "double4x2";
			case EShaderVariable::Double4x3 :	return "double4x3";
			case EShaderVariable::Double4x4 :	return "double4x4";
		}
		RETURN_ERR( "invalid variable type", "unknown" );
	}
		
/*
=================================================
	ImageToStringHLSL
=================================================
*/
	String HLSL_DstLanguage::ImageToStringHLSL (EShaderVariable::type value, EShaderMemoryModel::type memModel)
	{
		using VT = _platforms_hidden_::EValueTypeInfo;

		const bool	readonly	= not EShaderMemoryModel::HasWriteAccess( memModel );
		uint		img_type	= uint(value) & ~(VT::_SAMPLER | VT::_IMAGE);

		if ( readonly )
			img_type |= VT::_SAMPLER;
		else
			img_type |= VT::_IMAGE;

		switch ( img_type )
		{
			case EShaderVariable::FloatSampler1DShadow :
			case EShaderVariable::FloatSampler1D :			return "Texture1D<float4>";
			case EShaderVariable::FloatSampler1DArrayShadow:
			case EShaderVariable::FloatSampler1DArray :		return "Texture1DArray<float4>";
			case EShaderVariable::FloatSampler2DShadow :
			case EShaderVariable::FloatSampler2D :			return "Texture2D<float4>";
			case EShaderVariable::FloatSampler2DMS :		return "Texture2DMS<float4>";
			case EShaderVariable::FloatSampler2DArrayShadow:
			case EShaderVariable::FloatSampler2DArray :		return "Texture2DArray<float4>";
			case EShaderVariable::FloatSampler2DMSArray :	return "Texture2DMSArray<float4>";
			case EShaderVariable::FloatSamplerCubeShadow :
			case EShaderVariable::FloatSamplerCube :		return "TextureCube<float4>";
			case EShaderVariable::FloatSamplerCubeArray :	return "TextureCubeArray<float4>";
			case EShaderVariable::FloatSampler3D :			return "Texture3D<float4>";
			case EShaderVariable::FloatSamplerBuffer :		return "Buffer<float4>";
			case EShaderVariable::IntSampler1D :			return "Texture1D<int4>";
			case EShaderVariable::IntSampler1DArray :		return "Texture1DArray<int4>";
			case EShaderVariable::IntSampler2D :			return "Texture2D<int4>";
			case EShaderVariable::IntSampler2DMS :			return "Texture2DMS<int4>";
			case EShaderVariable::IntSampler2DArray :		return "Texture2DArray<int4>";
			case EShaderVariable::IntSampler2DMSArray :		return "Texture2DMSArray<int4>";
			case EShaderVariable::IntSamplerCube :			return "TextureCube<int4>";
			case EShaderVariable::IntSamplerCubeArray :		return "TextureCubeArray<int4>";
			case EShaderVariable::IntSampler3D :			return "Texture3D<int4>";
			case EShaderVariable::IntSamplerBuffer :		return "Buffer<int4>";
			case EShaderVariable::UIntSampler1D :			return "Texture1D<uint4>";
			case EShaderVariable::UIntSampler1DArray :		return "Texture1DArray<uint4>";
			case EShaderVariable::UIntSampler2D :			return "Texture2D<uint4>";
			case EShaderVariable::UIntSampler2DMS :			return "Texture2DMS<uint4>";
			case EShaderVariable::UIntSampler2DArray :		return "Texture2DArray<uint4>";
			case EShaderVariable::UIntSampler2DMSArray :	return "Texture2DMSArray<uint4>";
			case EShaderVariable::UIntSamplerCube :			return "TextureCube<uint4>";
			case EShaderVariable::UIntSamplerCubeArray :	return "TextureCubeArray<uint4>";
			case EShaderVariable::UIntSampler3D :			return "Texture3D<uint4>";
			case EShaderVariable::UIntSamplerBuffer :		return "Buffer<uint4>";
			
			case EShaderVariable::FloatImage1D :			return "RWTexture1D<float4>";
			case EShaderVariable::IntImage1D :				return "RWTexture1D<int4>";
			case EShaderVariable::UIntImage1D :				return "RWTexture1D<uint4>";
			case EShaderVariable::FloatImage1DArray :		return "RWTexture1DArray<float4>";
			case EShaderVariable::IntImage1DArray :			return "RWTexture1DArray<int4>";
			case EShaderVariable::UIntImage1DArray :		return "RWTexture1DArray<uint4>";
			case EShaderVariable::FloatImage2D :			return "RWTexture2D<float4>";
			case EShaderVariable::IntImage2D :				return "RWTexture2D<int4>";
			case EShaderVariable::UIntImage2D :				return "RWTexture2D<uint4>";
			//case EShaderVariable::FloatImage2DMS :		return "RWTexture2DMS<float4>";
			//case EShaderVariable::IntImage2DMS :			return "RWTexture2DMS<int4>";
			//case EShaderVariable::UIntImage2DMS :			return "RWTexture2DMS<uint4>";
			case EShaderVariable::FloatImage2DArray :		return "RWTexture2DArray<float4>";
			case EShaderVariable::IntImage2DArray :			return "RWTexture2DArray<int4>";
			case EShaderVariable::UIntImage2DArray :		return "RWTexture2DArray<uint4>";
			case EShaderVariable::FloatImage2DMSArray :		return "RWTexture2DArrayMS<float4>";
			case EShaderVariable::IntImage2DMSArray :		return "RWTexture2DArrayMS<int4>";
			case EShaderVariable::UIntImage2DMSArray :		return "RWTexture2DArrayMS<uint4>";
			//case EShaderVariable::FloatImageCube :		return "RWTextureCube<float4>";
			//case EShaderVariable::IntImageCube :			return "RWTextureCube<int4>";
			//case EShaderVariable::UIntImageCube :			return "RWTextureCube<uint4>";
			//case EShaderVariable::FloatImageCubeArray :	return "RWTextureCubeArray<float4>";
			//case EShaderVariable::IntImageCubeArray :		return "RWTextureCubeArray<int4>";
			//case EShaderVariable::UIntImageCubeArray :	return "RWTextureCubeArray<uint4>";
			case EShaderVariable::FloatImage3D :			return "RWTexture3D<float4>";
			case EShaderVariable::IntImage3D :				return "RWTexture3D<int4>";
			case EShaderVariable::UIntImage3D :				return "RWTexture3D<uint4>";
			case EShaderVariable::FloatImageBuffer :		return "RWBuffer<float4>";
			case EShaderVariable::IntImageBuffer :			return "RWBuffer<int4>";
			case EShaderVariable::UIntImageBuffer :			return "RWBuffer<uint4>";
		}
		RETURN_ERR( "invalid variable type", "unknown" );
	}
	
/*
=================================================
	ToStringHLSL
=================================================
*/
	String HLSL_DstLanguage::ToStringHLSL (EShaderMemoryModel::type)
	{
		return "";
	}

}	// PipelineCompiler
