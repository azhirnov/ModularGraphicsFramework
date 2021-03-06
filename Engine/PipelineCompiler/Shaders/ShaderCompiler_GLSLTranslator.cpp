// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Translate from GXSL to GLSL.
	This is temporary solution for dynamic arrays problem,
	all functions will be inlined to entry point, then new code
	may be optimized as valid GLSL or compiled to SPIRV.
*/

#include "Engine/PipelineCompiler/Shaders/ShaderCompiler_Translator.h"
#include "Engine/PipelineCompiler/Shaders/ShaderCompiler_NameValidator.h"
#include "Engine/PipelineCompiler/Common/ToGLSL.h"

namespace PipelineCompiler
{

	//
	// GLSL Language
	//

	class GLSL_DstLanguage : public Translator::IDstLanguage
	{
	// variables
	private:
		ShaderVarNameValidator	_nameValidator;


	// methods
	public:
		GLSL_DstLanguage () : _nameValidator{EShaderFormat::GLSL_450}
		{}

		~GLSL_DstLanguage ()
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

		bool TranslateBufferLoad (SymbolID, StringCRef, StringCRef, const TypeInfo &, INOUT String &) override { return false; }
		bool TranslateBufferStore (SymbolID, StringCRef, StringCRef, const TypeInfo &, StringCRef, const TypeInfo &, INOUT String &) override { return false; }
		
		bool TranslateValue (VariantCRef value, INOUT String &src) const override;
		
		bool DeclExternalTypes () const override		{ return false; }
		bool ReplaceStructByBuffer () const override	{ return false; }

	private:
		String _TranslateFunctionName (StringCRef sign);
		bool _TranslateArg (const Symbol &t, INOUT String &res);
		bool _TranslateField (const TypeInfo &t, INOUT String &res);

		bool _TranslateBuffer (glslang::TType const& type, Symbol const& info, OUT String &str);
		bool _TranslateImage (Symbol const& info, OUT String &str);
		bool _TranslateVarying (Symbol const& info, OUT String &str);
		bool _TranslateConst (glslang::TIntermTyped* typed, Symbol const& info, OUT String &str);
		bool _TranslateGlobal (glslang::TIntermTyped* typed, Symbol const& info, OUT String &str);
		bool _TranslateShared (Symbol const& info, OUT String &str);
		bool _TranslateSubpassInput (glslang::TType const& type, Symbol const& info, OUT String &str);
		
		bool _TranslateOperator0 (glslang::TOperator op, const TypeInfo &resultType, INOUT String &src);
		bool _TranslateOperator1 (glslang::TOperator op, const TypeInfo &resultType, ArrayCRef<String> args, ArrayCRef<TypeInfo const*> argTypes, StringCRef allArgs, INOUT String &src);
		bool _TranslateOperator2 (glslang::TOperator op, const TypeInfo &resultType, ArrayCRef<String> args, ArrayCRef<TypeInfo const*> argTypes, StringCRef allArgs, INOUT String &src);
		bool _TranslateOperatorN (glslang::TOperator op, const TypeInfo &resultType, ArrayCRef<String> args, ArrayCRef<TypeInfo const*> argTypes, StringCRef allArgs, INOUT String &src);

		bool _RecursiveInitConstStruct (const Array<TypeInfo> &fields, const glslang::TConstUnionArray& cu_arr, INOUT int &index, OUT String &src);
	};

	

/*
=================================================
	_TranslateGXSLtoGLSL
=================================================
*/
	static bool TranslateShaderInfo (const glslang::TIntermediate* intermediate, bool skipExternals, Translator &translator);

	bool ShaderCompiler::_TranslateGXSLtoGLSL (const Config &cfg, const _GLSLangResult &glslangData, OUT String &log, OUT BinaryArray &result) const
	{
		CHECK_ERR(	EShaderFormat::GetApiFormat( cfg.source ) == EShaderFormat::GXSL or
					EShaderFormat::GetApiFormat( cfg.source ) == EShaderFormat::GLSL or
					EShaderFormat::GetApiFormat( cfg.source ) == EShaderFormat::VKSL );

		CHECK_ERR(	EShaderFormat::GetApiFormat( cfg.target ) == EShaderFormat::GXSL or
					EShaderFormat::GetApiFormat( cfg.target ) == EShaderFormat::GLSL or
					EShaderFormat::GetApiFormat( cfg.target ) == EShaderFormat::VKSL );
	
		// not supported here
		ASSERT( not cfg.optimize );

		const glslang::TIntermediate* intermediate = glslangData.prog.getIntermediate( glslangData.shader->getStage() );
		CHECK_ERR( intermediate );

		TIntermNode*	root	= intermediate->getTreeRoot();
		Translator		translator{ log };

		translator.states.useGXrules	= intermediate->getSource() == glslang::EShSourceGxsl;
		translator.states.inlineAll		= cfg.inlineAll;
		translator.entryPoint			= intermediate->getEntryPointName().c_str();
		translator.language				= new GLSL_DstLanguage();

		CHECK_ERR( TranslateShaderInfo( intermediate, cfg.skipExternals, translator ) );
		
		CHECK_ERR( translator.Main( root, cfg.skipExternals ) );

		result	= BinArrayCRef::From( translator.src );
		return true;
	}
//-----------------------------------------------------------------------------



/*
=================================================
	TranslateShaderInfo
=================================================
*/
	static StringCRef ConvertGeometryInputPrimitive (glslang::TLayoutGeometry value);
	static StringCRef ConvertGeometryOutputPrimitive (glslang::TLayoutGeometry value);

	static bool TranslateShaderInfo (const glslang::TIntermediate* intermediate, bool skipExternals, Translator &translator)
	{
		String &	src = translator.src;

		// add version and profile
		if ( not skipExternals )
		{
			src << "#version " << intermediate->getVersion();

			switch ( intermediate->getProfile() ) {
				case EProfile::ECompatibilityProfile :	src << " compatibility\n";	break;
				case EProfile::ECoreProfile :			src << " core\n";			break;
				case EProfile::EEsProfile :				src << " es\n";				break;
				default :								src << "\n";
			}
		}

		switch ( intermediate->getStage() )
		{
			case EShLanguage::EShLangVertex :
			{
				break;
			}
			
			case EShLanguage::EShLangFragment :
			{
				// early fragment test
				if ( intermediate->getEarlyFragmentTests() )
					src << "layout(early_fragment_tests) in;\n";

				// frag coord
				{
					if ( intermediate->getOriginUpperLeft() and intermediate->getPixelCenterInteger() )
						src << "layout(origin_upper_left, pixel_center_integer) in vec4 gl_FragCoord;\n";
					else
					if ( intermediate->getOriginUpperLeft() )
						src << "layout(origin_upper_left) in vec4 gl_FragCoord;\n";
					else
					if ( intermediate->getPixelCenterInteger() )
						src << "layout(pixel_center_integer) in vec4 gl_FragCoord;\n";
				}

				// depth coverage
				if ( intermediate->getPostDepthCoverage() )
				{
					// see https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_post_depth_coverage.txt
					src << "layout(post_depth_coverage) in;\n";
				}

				// depth export
				if ( intermediate->isDepthReplacing() )
				{
					// see https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_conservative_depth.txt
					switch ( intermediate->getDepth() )
					{
						case glslang::TLayoutDepth::EldAny :		src << "layout (depth_any) out float gl_FragDepth;\n";			break;
						case glslang::TLayoutDepth::EldGreater :	src << "layout (depth_greater) out float gl_FragDepth;\n";		break;
						case glslang::TLayoutDepth::EldLess :		src << "layout (depth_less) out float gl_FragDepth;\n";			break;
						case glslang::TLayoutDepth::EldUnchanged :	src << "layout (depth_unchanged) out float gl_FragDepth;\n";	break;
						default :									src << "out float gl_FragDepth;\n";
					}
				}
				break;
			}
			/*
			case EShLanguage::EShLangTessControl :
			{
				result._tessControl.patchSize		= intermediate->getVertices();
				break;
			}

			case EShLanguage::EShLangTessEvaluation :
			{
				result._tessEvaluation.spacing		= ConvertTessellationSpacing( intermediate->getVertexSpacing() );
				result._tessEvaluation.ccw			= not (intermediate->getVertexOrder() == glslang::TVertexOrder::EvoCw);
				result._tessEvaluation.primitive	= ConvertTessellationInputPrimitive( intermediate->getInputPrimitive(), intermediate->getPointMode() );
				break;
			}*/

			case EShLanguage::EShLangGeometry :
			{
				if ( intermediate->getInvocations() > 1 ) {
					src << "layout (invocations = " << intermediate->getInvocations() << ") in;\n";
				}

				src << "layout (" << ConvertGeometryInputPrimitive( intermediate->getInputPrimitive() ) << ") in;\n";
				src << "layout (" << ConvertGeometryOutputPrimitive( intermediate->getOutputPrimitive() ) << ", max_vertices = " << intermediate->getVertices() << ") out;\n";
				break;
			}
			
			case EShLanguage::EShLangCompute :
			{
				src	<< "layout (local_size_x=" << intermediate->getLocalSize(0)
					<< ", local_size_y=" << intermediate->getLocalSize(1)
					<< ", local_size_z=" << intermediate->getLocalSize(2) << ") in;\n";
				break;
			}
		}

		src << '\n';
		return true;
	}
	
/*
=================================================
	ConvertGeometryInputPrimitive
=================================================
*/
	static StringCRef ConvertGeometryInputPrimitive (glslang::TLayoutGeometry value)
	{
		switch ( value )
		{
			case glslang::TLayoutGeometry::ElgPoints :				return "points";
			case glslang::TLayoutGeometry::ElgLines :				return "lines";
			case glslang::TLayoutGeometry::ElgLinesAdjacency :		return "lines_adjacency";
			case glslang::TLayoutGeometry::ElgTriangles :			return "triangles";
			case glslang::TLayoutGeometry::ElgTrianglesAdjacency :	return "triangles_adjacency";
		}
		RETURN_ERR( "invalid geometry shader input primitive type!" );
	}
	
/*
=================================================
	ConvertGeometryOutputPrimitive
=================================================
*/
	static StringCRef ConvertGeometryOutputPrimitive (glslang::TLayoutGeometry value)
	{
		switch ( value )
		{
			case glslang::TLayoutGeometry::ElgPoints :			return "points";
			case glslang::TLayoutGeometry::ElgLineStrip :		return "line_strip";
			case glslang::TLayoutGeometry::ElgTriangleStrip :	return "triangle_strip";
		}
		RETURN_ERR( "invalid geometry shader output primitive type!" );
	}

/*
=================================================
	TranslateLocalVar
=================================================
*/
	bool GLSL_DstLanguage::TranslateLocalVar (const Symbol &t, INOUT String &res)
	{
		CHECK_ERR( not t.name.Empty() );

		// access
		//if ( memoryModel != EShaderMemoryModel::Default )
		//	res << ToStringGLSL( memoryModel ) << ' ';

		// read-only
		if ( t.qualifier[ EVariableQualifier::Constant ] )
			res << "const ";

		// layout
		if ( t.format != EPixelFormat::Unknown )
			res << "layout(" << ToStringGLSL( t.format ) << ") ";	// TODO: for writeonly access format you may skip explicit format

		// precision
		//if ( t.precision != EPrecision::Default )
		//	res << ToStringGLSL( t.precision ) << ' ';

		// type
		if ( not t.typeName.Empty() ) {
			res << t.typeName;
		} else {
			res << ToStringGLSL( t.type );
		}

		res << " " << t.name << (t.arraySize.IsNotArray() ? "" : (t.arraySize.IsDynamicArray() ? "[]" : "["_str << t.arraySize.Size() << "]"));
		return true;
	}
	
/*
=================================================
	_TranslateField
=================================================
*/
	bool GLSL_DstLanguage::_TranslateField (const TypeInfo &t, INOUT String &src)
	{
		if ( not t.typeName.Empty() ) {
			src << t.typeName;
		} else {
			src << ToStringGLSL( t.type );
		}

		src << " " << t.name << (t.arraySize.IsNotArray() ? "" : (t.arraySize.IsDynamicArray() ? "[]" : "["_str << t.arraySize.Size() << "]"));
		return true;
	}

/*
=================================================
	TranslateStruct
=================================================
*/
	bool GLSL_DstLanguage::TranslateStruct (const TypeInfo &info, INOUT String &src)
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
	bool GLSL_DstLanguage::TranslateType (const TypeInfo &t, INOUT String &res)
	{
		// access
		//if ( t.memoryModel != EShaderMemoryModel::Default )
		//	res << ToStringGLSL( t.memoryModel ) << ' ';

		// layout
		if ( t.format != EPixelFormat::Unknown )
			res << "layout(" << ToStringGLSL( t.format ) << ") ";	// TODO: for writeonly access format you may skip explicit format

		// precision
		//if ( t.precision != EPrecision::Default )
		//	res << ToStringGLSL( t.precision ) << ' ';

		// type
		if ( not t.typeName.Empty() ) {
			res << t.typeName;
		} else {
			res << ToStringGLSL( t.type );
		}

		res << (t.arraySize.IsNotArray() ? "" : (t.arraySize.IsDynamicArray() ? "[]" : "["_str << t.arraySize.Size() << "]"));
		return true;
	}
	
/*
=================================================
	TranslateName
=================================================
*/
	bool GLSL_DstLanguage::TranslateName (const Symbol &t, INOUT String &src)
	{
		src << t.name;
		return true;
	}
	
/*
=================================================
	ValidateSymbolName
=================================================
*/
	bool GLSL_DstLanguage::ValidateSymbolName (INOUT Symbol &symb, uint fnScopeId)
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
	bool GLSL_DstLanguage::ValidateFieldName (INOUT TypeInfo &info)
	{
		return _nameValidator.ValidateField( INOUT info.name );
	}

/*
=================================================
	_TranslateFunctionName
=================================================
*/
	String GLSL_DstLanguage::_TranslateFunctionName (StringCRef signature)
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
	bool GLSL_DstLanguage::TranslateExternal (glslang::TIntermTyped* typed, const Symbol &info, INOUT String &str)
	{
		if ( EShaderVariable::IsBuffer( info.type ) ) {
			CHECK_ERR( _TranslateBuffer( typed->getType(), info, INOUT str ) );
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
		if ( info.qualifier[ EVariableQualifier::In ] or info.qualifier[ EVariableQualifier::Out ] ) {
			CHECK_ERR( _TranslateVarying( info, INOUT str ) );
		}
		else
		if ( info.qualifier[ EVariableQualifier::Constant ] ) {
			CHECK_ERR( _TranslateConst( typed, info, INOUT str ) );
		}
		else
		if ( typed->getQualifier().storage == glslang::TStorageQualifier::EvqGlobal ) {
			CHECK_ERR( _TranslateGlobal( typed, info, INOUT str ) );
		}
		else
		if ( info.type == EShaderVariable::SubpassInput ) {
			CHECK_ERR( _TranslateSubpassInput( typed->getType(), info, INOUT str ) );
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
	bool GLSL_DstLanguage::TranslateOperator (glslang::TOperator op, const TypeInfo &resultType, ArrayCRef<String> args, ArrayCRef<TypeInfo const*> argTypes, INOUT String &src)
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
			CHECK_ERR( TranslateType( resultType, INOUT src ) );
			src << all_args;
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
	bool GLSL_DstLanguage::_TranslateOperator0 (glslang::TOperator op, const TypeInfo &, INOUT String &src)
	{
		switch ( op )
		{
			case glslang::TOperator::EOpEmitVertex :				src << "EmitVertex()";			break;
			case glslang::TOperator::EOpEndPrimitive :				src << "EndPrimitive()";		break;
			case glslang::TOperator::EOpBarrier :					src << "barrier()";				break;
			case glslang::TOperator::EOpMemoryBarrier :				src << "memoryBarrier()";		break;
			case glslang::TOperator::EOpMemoryBarrierAtomicCounter:	src << "memoryBarrierAtomicCounter()";break;
			case glslang::TOperator::EOpMemoryBarrierBuffer :		src << "memoryBarrierBuffer()";	break;
			case glslang::TOperator::EOpMemoryBarrierImage :		src << "memoryBarrierImage()";	break;
			case glslang::TOperator::EOpMemoryBarrierShared :		src << "memoryBarrierShared()";	break;
			case glslang::TOperator::EOpGroupMemoryBarrier :		src << "groupMemoryBarrier()";	break;
			default :												RETURN_ERR( "unknown operator!" );
		}
		return true;
	}
	
/*
=================================================
	_TranslateOperator1
=================================================
*/
	bool GLSL_DstLanguage::_TranslateOperator1 (glslang::TOperator op, const TypeInfo &, ArrayCRef<String>,
												ArrayCRef<TypeInfo const*>, StringCRef all_args, INOUT String &src)
	{
		switch ( op )
		{
			case glslang::TOperator::EOpNegative :				src << "-" << all_args;				break;
			case glslang::TOperator::EOpLogicalNot :			src << "!" << all_args;				break;
			case glslang::TOperator::EOpVectorLogicalNot :		src << "not" << all_args;			break;
			case glslang::TOperator::EOpBitwiseNot :			src << "~" << all_args;				break;
			case glslang::TOperator::EOpPostIncrement :			src << all_args << "++";			break;
			case glslang::TOperator::EOpPostDecrement :			src << all_args << "--";			break;
			case glslang::TOperator::EOpPreIncrement :			src << "++" << all_args;			break;
			case glslang::TOperator::EOpPreDecrement :			src << "--" << all_args;			break;
			case glslang::TOperator::EOpArrayLength :			src << all_args << ".length()";		break;
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
			case glslang::TOperator::EOpInverseSqrt :			src << "inversesqrt" << all_args;	break;
			case glslang::TOperator::EOpAbs :					src << "abs" << all_args;			break;
			case glslang::TOperator::EOpSign :					src << "sign" << all_args;			break;
			case glslang::TOperator::EOpFloor :					src << "floor" << all_args;			break;
			case glslang::TOperator::EOpTrunc :					src << "trunc" << all_args;			break;
			case glslang::TOperator::EOpRound :					src << "round" << all_args;			break;
			case glslang::TOperator::EOpRoundEven :				src << "roundEven" << all_args;		break;
			case glslang::TOperator::EOpCeil :					src << "ceil" << all_args;			break;
			case glslang::TOperator::EOpFract :					src << "fract" << all_args;			break;
			case glslang::TOperator::EOpIsNan :					src << "isnan" << all_args;			break;
			case glslang::TOperator::EOpIsInf :					src << "isinf" << all_args;			break;
			case glslang::TOperator::EOpFloatBitsToInt :		src << "floatBitsToInt" << all_args;			break;
			case glslang::TOperator::EOpFloatBitsToUint :		src << "floatBitsToUint" << all_args;			break;
			case glslang::TOperator::EOpIntBitsToFloat :		src << "intBitsToFloat" << all_args;			break;
			case glslang::TOperator::EOpUintBitsToFloat :		src << "uintBitsToFloat" << all_args;			break;
			case glslang::TOperator::EOpDoubleBitsToInt64 :		src << "doubleBitsToInt64" << all_args;			break;
			case glslang::TOperator::EOpDoubleBitsToUint64 :	src << "doubleBitsToUint64" << all_args;		break;
			case glslang::TOperator::EOpInt64BitsToDouble :		src << "int64BitsToDouble" << all_args;			break;
			case glslang::TOperator::EOpUint64BitsToDouble :	src << "uint64BitsToDouble" << all_args;		break;
			#ifdef AMD_EXTENSIONS
			case glslang::TOperator::EOpFloat16BitsToInt16 :	src << "float16BitsToInt16" << all_args;		break;
			case glslang::TOperator::EOpFloat16BitsToUint16 :	src << "float16BitsToUint16" << all_args;		break;
			case glslang::TOperator::EOpInt16BitsToFloat16 :	src << "int16BitsToFloat16" << all_args;		break;
			case glslang::TOperator::EOpUint16BitsToFloat16 :	src << "uint16BitsToFloat16" << all_args;		break;
			#endif
			case glslang::TOperator::EOpPackSnorm2x16 :			src << "packSnorm2x16" << all_args;				break;
			case glslang::TOperator::EOpUnpackSnorm2x16 :		src << "unpackSnorm2x16" << all_args;			break;
			case glslang::TOperator::EOpPackUnorm2x16 :			src << "packUnorm2x16" << all_args;				break;
			case glslang::TOperator::EOpUnpackUnorm2x16 :		src << "unpackUnorm2x16" << all_args;			break;
			case glslang::TOperator::EOpPackSnorm4x8 :			src << "packSnorm4x8" << all_args;				break;
			case glslang::TOperator::EOpUnpackSnorm4x8 :		src << "unpackSnorm4x8" << all_args;			break;
			case glslang::TOperator::EOpPackUnorm4x8 :			src << "packUnorm4x8" << all_args;				break;
			case glslang::TOperator::EOpUnpackUnorm4x8 :		src << "unpackUnorm4x8" << all_args;			break;
			case glslang::TOperator::EOpPackHalf2x16 :			src << "packHalf2x16" << all_args;				break;
			case glslang::TOperator::EOpUnpackHalf2x16 :		src << "unpackHalf2x16" << all_args;			break;
			case glslang::TOperator::EOpPackDouble2x32 :		src << "packDouble2x32" << all_args;			break;
			case glslang::TOperator::EOpUnpackDouble2x32 :		src << "unpackDouble2x32" << all_args;			break;
			case glslang::TOperator::EOpPackInt2x32 :			src << "packInt2x32" << all_args;				break;
			case glslang::TOperator::EOpUnpackInt2x32 :			src << "unpackInt2x32" << all_args;				break;
			case glslang::TOperator::EOpPackUint2x32 :			src << "packUint2x32" << all_args;				break;
			case glslang::TOperator::EOpUnpackUint2x32 :		src << "unpackUint2x32" << all_args;			break;
			#ifdef AMD_EXTENSIONS
			case glslang::TOperator::EOpPackFloat2x16 :			src << "packFloat2x16" << all_args;				break;
			case glslang::TOperator::EOpUnpackFloat2x16 :		src << "unpackFloat2x16" << all_args;			break;
			case glslang::TOperator::EOpPackInt2x16 :			src << "packInt2x16" << all_args;				break;
			case glslang::TOperator::EOpUnpackInt2x16 :			src << "unpackInt2x16" << all_args;				break;
			case glslang::TOperator::EOpPackUint2x16 :			src << "packUint2x16" << all_args;				break;
			case glslang::TOperator::EOpUnpackUint2x16 :		src << "unpackUint2x16" << all_args;			break;
			case glslang::TOperator::EOpPackInt4x16 :			src << "packInt4x16" << all_args;				break;
			case glslang::TOperator::EOpUnpackInt4x16 :			src << "unpackInt4x16" << all_args;				break;
			case glslang::TOperator::EOpPackUint4x16 :			src << "packUint4x16" << all_args;				break;
			case glslang::TOperator::EOpUnpackUint4x16 :		src << "unpackUint4x16" << all_args;			break;
			#endif
			case glslang::TOperator::EOpLength :				src << "length" << all_args;					break;
			case glslang::TOperator::EOpNormalize :				src << "normalize" << all_args;					break;
			case glslang::TOperator::EOpDPdx :					src << "dFdx" << all_args;						break;
			case glslang::TOperator::EOpDPdy :					src << "dFdy" << all_args;						break;
			case glslang::TOperator::EOpFwidth :				src << "fwidth" << all_args;					break;
			case glslang::TOperator::EOpDPdxFine :				src << "dFdxFine" << all_args;					break;
			case glslang::TOperator::EOpDPdyFine :				src << "dFdyFine" << all_args;					break;
			case glslang::TOperator::EOpFwidthFine :			src << "fwidthFine" << all_args;				break;
			case glslang::TOperator::EOpDPdxCoarse :			src << "dFdxCoarse" << all_args;				break;
			case glslang::TOperator::EOpDPdyCoarse :			src << "dFdyCoarse" << all_args;				break;
			case glslang::TOperator::EOpFwidthCoarse :			src << "fwidthCoarse" << all_args;				break;
			case glslang::TOperator::EOpInterpolateAtCentroid :	src << "interpolateAtCentroid" << all_args;		break;
			case glslang::TOperator::EOpBitFieldReverse :		src << "bitfieldReverse" << all_args;			break;
			case glslang::TOperator::EOpBitCount :				src << "bitCount" << all_args;					break;
			case glslang::TOperator::EOpFindLSB :				src << "findLSB" << all_args;					break;
			case glslang::TOperator::EOpFindMSB :				src << "findMSB" << all_args;					break;
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpInterpolateAtVertex :	src << "atanh" << all_args;						break;
			#endif
			case glslang::TOperator::EOpDeterminant :			src << "determinant" << all_args;				break;
			case glslang::TOperator::EOpMatrixInverse :			src << "inverse" << all_args;					break;
			case glslang::TOperator::EOpTranspose :				src << "transpose" << all_args;					break;
			case glslang::TOperator::EOpEmitStreamVertex :		src << "EmitStreamVertex" << all_args;			break;
			case glslang::TOperator::EOpEndStreamPrimitive :	src << "EndStreamPrimitive" << all_args;		break;

			case glslang::TOperator::EOpImageQuerySize :		src << "imageSize" << all_args;					break;
			case glslang::TOperator::EOpImageQuerySamples :		src << "imageSamples" << all_args;				break;
			case glslang::TOperator::EOpNoise :					src << "noise" << all_args;						break;	// deprecated
					
			case glslang::TOperator::EOpAtomicCounter :			src << "atomicCounter" << all_args;				break;
			case glslang::TOperator::EOpAtomicCounterIncrement:	src << "atomicCounterIncrement" << all_args;	break;
			case glslang::TOperator::EOpAtomicCounterDecrement:	src << "atomicCounterDecrement" << all_args;	break;

			case glslang::TOperator::EOpTextureQuerySize :		src << "textureSize" << all_args;				break;
			case glslang::TOperator::EOpTextureQueryLevels :	src << "textureQueryLevels" << all_args;		break;
			case glslang::TOperator::EOpTextureQuerySamples :	src << "textureSamples" << all_args;			break;
				
			case glslang::TOperator::EOpSubpassLoad :			src << "subpassLoad" << all_args;				break;
			//case glslang::TOperator::EOpSubpassLoadMS :		src << "subpassLoadMs" << all_args;				break;

			default :											RETURN_ERR( "unknown operator!" );
		}
		return true;
	}
	
/*
=================================================
	_TranslateOperator2
=================================================
*/
	bool GLSL_DstLanguage::_TranslateOperator2 (glslang::TOperator op, const TypeInfo &resultType, ArrayCRef<String> args,
												ArrayCRef<TypeInfo const*> argTypes, StringCRef all_args, INOUT String &src)
	{
		const bool	is_vec	=	(resultType.arraySize.IsNotArray() and EShaderVariable::VecSize( resultType.type ) > 1)		and
								(argTypes[0]->arraySize.IsNotArray() and EShaderVariable::VecSize( argTypes[0]->type ) > 1)	and
								(argTypes[1]->arraySize.IsNotArray() and EShaderVariable::VecSize( argTypes[1]->type ) > 1);
			
		const bool	is_float = (EShaderVariable::IsFloat( argTypes[0]->type ) or EShaderVariable::IsFloat( argTypes[1]->type ));

		if ( is_vec )
		{
			const auto &	lhs_type = *argTypes[0];
			const auto &	rhs_type = *argTypes[1];
			const uint		vec_size = Max( EShaderVariable::VecSize( resultType.type ), EShaderVariable::VecSize( lhs_type.type ), EShaderVariable::VecSize( rhs_type.type ) );
			const auto		sh_type  = EShaderVariable::ToVec( lhs_type.type, vec_size );
			String			temp;
			
			if ( lhs_type.type != sh_type )		temp << ToStringGLSL( sh_type ) << '(' << args[0] << ')';
			else								temp << args[0];
			temp << ", ";
			if ( rhs_type.type != sh_type )		temp << ToStringGLSL( sh_type ) << '(' << args[1] << ')';
			else								temp << args[1];
			
			switch ( op )
			{
				case glslang::TOperator::EOpVectorEqual :		src<<"equal("<<temp<<')';				return true;
				case glslang::TOperator::EOpVectorNotEqual :	src<<"notEqual("<<temp<<')';			return true;
				case glslang::TOperator::EOpLessThan :			src<<"lessThan("<<temp<<')';			return true;
				case glslang::TOperator::EOpGreaterThan :		src<<"greaterThan("<<temp<<')';			return true;
				case glslang::TOperator::EOpLessThanEqual :		src<<"lessThanEqual("<<temp<<')';		return true;
				case glslang::TOperator::EOpGreaterThanEqual :	src<<"greaterThanEqual("<<temp<<')';	return true;
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
			case glslang::TOperator::EOpVectorTimesMatrix :			src << '('<<args[0] <<" * " << args[1]<<')';	break;
			case glslang::TOperator::EOpMatrixTimesVector :			src << '('<<args[0] <<" * " << args[1]<<')';	break;
			case glslang::TOperator::EOpMatrixTimesScalar :			src << '('<<args[0] <<" * " << args[1]<<')';	break;
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
			case glslang::TOperator::EOpVectorTimesMatrixAssign :	src << '('<<args[0] <<" *= "<< args[1]<<')';	break;
			case glslang::TOperator::EOpVectorTimesScalarAssign :	src << '('<<args[0] <<" *= "<< args[1]<<')';	break;
			case glslang::TOperator::EOpMatrixTimesScalarAssign :	src << '('<<args[0] <<" *= "<< args[1]<<')';	break;
			case glslang::TOperator::EOpMatrixTimesMatrixAssign :	src << '('<<args[0] <<" *= "<< args[1]<<')';	break;
			case glslang::TOperator::EOpDivAssign :					src << '('<<args[0] <<" /= "<< args[1]<<')';	break;
			case glslang::TOperator::EOpModAssign :					src << '('<<args[0] <<" %= "<< args[1]<<')';	break;
			case glslang::TOperator::EOpAndAssign :					src << '('<<args[0] <<" &= "<< args[1]<<')';	break;
			case glslang::TOperator::EOpInclusiveOrAssign :			src << '('<<args[0] <<" |= "<< args[1]<<')';	break;
			case glslang::TOperator::EOpExclusiveOrAssign :			src << '('<<args[0] <<" ^= "<< args[1]<<')';	break;
			case glslang::TOperator::EOpLeftShiftAssign :			src << '('<<args[0] <<" <<= "<< args[1]<<')';	break;
			case glslang::TOperator::EOpRightShiftAssign :			src << '('<<args[0] <<" >>= "<< args[1]<<')';	break;

			case glslang::TOperator::EOpAtan :						src << "atan" << all_args;						break;
			case glslang::TOperator::EOpImageLoad :					src << "imageLoad" << all_args;					break;
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
			case glslang::TOperator::EOpInterpolateAtSample :		src << "interpolateAtSample" << all_args;		break;
			case glslang::TOperator::EOpInterpolateAtOffset :		src << "interpolateAtOffset" << all_args;		break;
			case glslang::TOperator::EOpOuterProduct :				src << "outerProduct" << all_args;				break;
			case glslang::TOperator::EOpMatrixTimesMatrix :			src << "matrixCompMult" << all_args;			break;

			case glslang::TOperator::EOpAtomicAdd :					src << "atomicAdd" << all_args;					break;
			case glslang::TOperator::EOpAtomicMin :					src << "atomicMin" << all_args;					break;
			case glslang::TOperator::EOpAtomicMax :					src << "atomicMax" << all_args;					break;
			case glslang::TOperator::EOpAtomicAnd :					src << "atomicAnd" << all_args;					break;
			case glslang::TOperator::EOpAtomicOr :					src << "atomicOr" << all_args;					break;
			case glslang::TOperator::EOpAtomicXor :					src << "atomicXor" << all_args;					break;
			case glslang::TOperator::EOpAtomicExchange :			src << "atomicExchange" << all_args;			break;
			case glslang::TOperator::EOpAtomicCounterAdd :			src << "atomicCounterAdd" << all_args;			break;
			case glslang::TOperator::EOpAtomicCounterSubtract :		src << "atomicCounterSub" << all_args;			break;
			case glslang::TOperator::EOpAtomicCounterMin :			src << "atomicCounterMin" << all_args;			break;
			case glslang::TOperator::EOpAtomicCounterMax :			src << "atomicCounterMax" << all_args;			break;
			case glslang::TOperator::EOpAtomicCounterAnd :			src << "atomicCounterAnd" << all_args;			break;
			case glslang::TOperator::EOpAtomicCounterOr :			src << "atomicCounterOr" << all_args;			break;
			case glslang::TOperator::EOpAtomicCounterXor :			src << "atomicCounterXor" << all_args;			break;
			case glslang::TOperator::EOpAtomicCounterExchange :		src << "atomicCounterExchange" << all_args;		break;

			case glslang::TOperator::EOpTextureQuerySize :			src << "textureSize" << all_args;				break;
			case glslang::TOperator::EOpTextureQueryLod :			src << "textureQueryLod" << all_args;			break;
			case glslang::TOperator::EOpTexture :					src << "texture" << all_args;					break;
			case glslang::TOperator::EOpTextureProj :				src << "textureProj" << all_args;				break;
			case glslang::TOperator::EOpTextureFetch :				src << "texelFetch" << all_args;				break;
			case glslang::TOperator::EOpTextureGather :				src << "textureGather" << all_args;				break;
			default :												RETURN_ERR( "unknown operator!" );
		}
		return true;
	}
	
/*
=================================================
	_TranslateOperatorN
=================================================
*/
	bool GLSL_DstLanguage::_TranslateOperatorN (glslang::TOperator op, const TypeInfo &, ArrayCRef<String>,
												ArrayCRef<TypeInfo const*>, StringCRef all_args, INOUT String &src)
	{
		switch ( op )
		{
			case glslang::TOperator::EOpImageLoad :					src << "imageLoad" << all_args;				break;	// 3 args
			case glslang::TOperator::EOpImageStore :				src << "imageStore" << all_args;			break;	// 3,4 args
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpImageLoadLod :			src << "imageLoadLod" << all_args;			break;
			//case glslang::TOperator::EOpImageStoreLod :			src << "imageStoreLod" << all_args;			break;
			#endif
			case glslang::TOperator::EOpImageAtomicAdd :			src << "imageAtomicAdd" << all_args;		break;	// 3,4 args
			case glslang::TOperator::EOpImageAtomicMin :			src << "imageAtomicMin" << all_args;		break;	// 3,4 args
			case glslang::TOperator::EOpImageAtomicMax :			src << "imageAtomicMax" << all_args;		break;	// 3,4 args
			case glslang::TOperator::EOpImageAtomicAnd :			src << "imageAtomicAnd" << all_args;		break;	// 3,4 args
			case glslang::TOperator::EOpImageAtomicOr :				src << "imageAtomicOr" << all_args;			break;	// 3,4 args
			case glslang::TOperator::EOpImageAtomicXor :			src << "imageAtomicXor" << all_args;		break;	// 3,4 args
			case glslang::TOperator::EOpImageAtomicExchange :		src << "imageAtomicExchange" << all_args;	break;	// 3,4 args
			case glslang::TOperator::EOpImageAtomicCompSwap :		src << "imageAtomicCompSwap" << all_args;	break;	// 4,5 args
			//case glslang::TOperator::EOpSubpassLoad :				src << "image(";	break;
			//case glslang::TOperator::EOpSubpassLoadMS :			src << "image(";	break;
			//case glslang::TOperator::EOpSparseImageLoad :			src << "image(";	break;
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpSparseImageLoadLod :		src << "image(";	break;
			#endif
			case glslang::TOperator::EOpClamp :						src << "clamp" << all_args;					break;	// 3 args
			case glslang::TOperator::EOpMix :						src << "mix" << all_args;					break;	// 3 args
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
			case glslang::TOperator::EOpFtransform :				src << "ftransform" << all_args;			break;	// WTF?
			case glslang::TOperator::EOpBallot :					src << "ballotARB" << all_args;				break;	// WTF?
			case glslang::TOperator::EOpReadInvocation :			src << "readInvocation" << all_args;		break;	// WTF?
			case glslang::TOperator::EOpReadFirstInvocation :		src << "readFirstInvocatin" << all_args;	break;	// WTF?
			case glslang::TOperator::EOpAnyInvocation :				src << "anyInvocation" << all_args;			break;	// WTF?
			case glslang::TOperator::EOpAllInvocations :			src << "allInvocations" << all_args;		break;	// WTF?
			case glslang::TOperator::EOpAllInvocationsEqual :		src << "allInvocationsEqual" << all_args;	break;	// WTF?
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
			case glslang::TOperator::EOpAtomicCompSwap :			src << "atomicCompSwap" << all_args;			break;	// 3 args
			case glslang::TOperator::EOpAtomicCounterCompSwap :		src << "atomicCounterCompSwap" << all_args;		break;	// 3 args

			case glslang::TOperator::EOpAddCarry :					src << "uaddCarry" << all_args;					break;	// 3 args
			case glslang::TOperator::EOpSubBorrow :					src << "usubBorrow" << all_args;				break;	// 3 args
			case glslang::TOperator::EOpUMulExtended :				src << "umulExtended" << all_args;				break;	// 4 args
			case glslang::TOperator::EOpIMulExtended :				src << "imulExtended" << all_args;				break;	// 4 args
			case glslang::TOperator::EOpBitfieldExtract :			src << "bitfieldExtract" << all_args;			break;	// 3 args
			case glslang::TOperator::EOpBitfieldInsert :			src << "bitfieldInsert" << all_args;			break;	// 4 args

			case glslang::TOperator::EOpTexture :					src << "texture" << all_args;					break;	// 3 args
			case glslang::TOperator::EOpTextureProj :				src << "textureProj" << all_args;				break;	// 3 args
			case glslang::TOperator::EOpTextureLod :				src << "textureLod" << all_args;				break;	// 3 args
			case glslang::TOperator::EOpTextureOffset :				src << "textureOffset" << all_args;				break;	// 3,4 args
			case glslang::TOperator::EOpTextureFetch :				src << "texelFetch" << all_args;				break;	// 3 args
			case glslang::TOperator::EOpTextureFetchOffset :		src << "texelFetchOffset" << all_args;			break;	// 3,4 args
			case glslang::TOperator::EOpTextureProjOffset :			src << "textureProjOffset" << all_args;			break;	// 3,4 args
			case glslang::TOperator::EOpTextureLodOffset :			src << "textureLodOffset" << all_args;			break;	// 4 args
			case glslang::TOperator::EOpTextureProjLod :			src << "textureProjLod" << all_args;			break;	// 3 args
			case glslang::TOperator::EOpTextureProjLodOffset :		src << "textureProjLodOffset" << all_args;		break;	// 4 args
			case glslang::TOperator::EOpTextureGrad :				src << "textureGrad" << all_args;				break;	// 4 args
			case glslang::TOperator::EOpTextureGradOffset :			src << "textureGradOffset" << all_args;			break;	// 5 args
			case glslang::TOperator::EOpTextureProjGrad :			src << "textureProjGrad" << all_args;			break;	// 4 args
			case glslang::TOperator::EOpTextureProjGradOffset :		src << "textureProjGradOffset" << all_args;		break;	// 5 args
			case glslang::TOperator::EOpTextureGather :				src << "textureGather" << all_args;				break;	// 3 args
			case glslang::TOperator::EOpTextureGatherOffset :		src << "textureGatherOffset" << all_args;		break;	// 3,4 args
			case glslang::TOperator::EOpTextureGatherOffsets :		src << "textureGatherOffsets" << all_args;		break;	// 3,4 args
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
	bool GLSL_DstLanguage::TranslateFunctionCall (StringCRef signature, const TypeInfo &, ArrayCRef<String> args, ArrayCRef<TypeInfo const*>, INOUT String &src)
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
	bool GLSL_DstLanguage::MustBeInlined (StringCRef, ArrayCRef<TypeInfo const*>)
	{
		return false;
	}

/*
=================================================
	_TranslateArg
=================================================
*/
	bool GLSL_DstLanguage::_TranslateArg (const Symbol &t, INOUT String &res)
	{
		// qualifies
		if ( t.qualifier[ EVariableQualifier::Constant ] )
		{}	//res << "const ";
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
	bool GLSL_DstLanguage::TranslateFunctionDecl (StringCRef signature, const TypeInfo &resultType, ArrayCRef<Symbol> args, INOUT String &src)
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
	bool GLSL_DstLanguage::TranslateSwizzle (const TypeInfo &, const TypeInfo &, StringCRef val, ArrayCRef<uint> swizzle, INOUT String &src)
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
	bool GLSL_DstLanguage::TranslateEntry (const TypeInfo &, StringCRef signature, ArrayCRef<Symbol>, StringCRef body, OUT String &entryPoint, INOUT String &src)
	{
		entryPoint = _TranslateFunctionName( signature );

		String	body_src = body;
		StringParser::IncreaceIndent( INOUT body_src );
		src << "\n{\n" << body_src << "}\n\n";

		return true;
	}
		
/*
=================================================
	TranslateStructAccess
=================================================
*/
	bool GLSL_DstLanguage::TranslateStructAccess (SymbolID, const TypeInfo &, StringCRef objName, const TypeInfo &fieldType, INOUT String &src)
	{
		if ( not objName.Empty() )
			src << objName << ".";

		src << fieldType.name;
		return true;
	}
//-----------------------------------------------------------------------------


	
/*
=================================================
	_TranslateBuffer
=================================================
*/
	bool GLSL_DstLanguage::_TranslateBuffer (glslang::TType const& type, Symbol const& info, OUT String &str)
	{
		if ( info.binding != UMax )
			str << "layout(binding=" << info.binding << ") ";

		if ( type.getQualifier().hasPacking() )
		{
			switch ( type.getQualifier().layoutPacking ) {
				case glslang::TLayoutPacking::ElpStd140 :	str << "layout(std140) ";	break;
				case glslang::TLayoutPacking::ElpStd430 :	str << "layout(std430) ";	break;
				default :									RETURN_ERR( "unsupported packing" );
			}
		}
		
		if ( info.type == EShaderVariable::StorageBlock )
		{
			if ( info.memoryModel != EShaderMemoryModel::Default )
				str << ToStringGLSL( info.memoryModel ) << ' ';

			str << "buffer " << info.typeName;
		}
		else
			str << "uniform " << info.typeName;

		CHECK_ERR( EShaderVariable::IsBuffer( info.type ) and not info.fields.Empty() );
		
		str << "{\n";
		for (auto& fld : info.fields)
		{
			str << "\t";

			if ( fld.memoryModel != info.memoryModel and fld.memoryModel != EShaderMemoryModel::Default )
				str << ToStringGLSL( fld.memoryModel ) << ' ';
			
			if ( not fld.typeName.Empty() ) {
				str << fld.typeName;
			} else {
				str << ToStringGLSL( fld.type );
			}

			str << " " << fld.name << (fld.arraySize.IsNotArray() ? "" : (fld.arraySize.IsDynamicArray() ? "[]" : "["_str << fld.arraySize.Size() << "]")) << ";\n";
		}
		str << "} " << info.name << ";\n";
		return true;
	}

/*
=================================================
	_TranslateImage
=================================================
*/
	bool GLSL_DstLanguage::_TranslateImage (Symbol const& info, OUT String &str)
	{
		CHECK_ERR( not info.name.Empty() );

		if ( info.binding != UMax )
			str << "layout(binding=" << info.binding << ") ";

		if ( EShaderVariable::IsImage( info.type ) )
		{
			str << "layout(" << ToStringGLSL( info.format ) << ") "	// TODO: for writeonly access format you may skip explicit format
				<< ToStringGLSL( info.memoryModel )
				<< " uniform ";
		}
		else
		{
			str << "uniform ";
		}

		// precision
		//if ( info.precision != EPrecision::Default )
		//	str << ToStringGLSL( info.precision ) << ' ';

		// type
		if ( not info.typeName.Empty() ) {
			str << info.typeName;
		} else {
			str << ToStringGLSL( info.type );
		}

		str << " " << info.name << (info.arraySize.IsNotArray() ? "" : (info.arraySize.IsDynamicArray() ? "[]" : "["_str << info.arraySize.Size() << "]")) << ";\n";
		return true;
	}

/*
=================================================
	_TranslateShared
=================================================
*/
	bool GLSL_DstLanguage::_TranslateShared (Symbol const& info, OUT String &str)
	{
		str << "shared ";
		CHECK_ERR( TranslateLocalVar( info, INOUT str ) );
		str << ";\n";
		return true;
	}
	
/*
=================================================
	_TranslateSubpassInput
=================================================
*/
	bool GLSL_DstLanguage::_TranslateSubpassInput (glslang::TType const& type, Symbol const& info, OUT String &str)
	{
		if ( type.getQualifier().hasAttachment() )
			str << "layout(input_attachment_index=" << type.getQualifier().layoutAttachment << ") ";
		
		if ( info.binding != UMax )
			str << "layout(binding=" << info.binding << ") ";

		str << "uniform subpassInput " << info.name << ";\n";
		return true;
	}

/*
=================================================
	_TranslateVarying
=================================================
*/
	bool GLSL_DstLanguage::_TranslateVarying (Symbol const& info, OUT String &str)
	{
		if ( info.qualifier[ EVariableQualifier::In ] )
			str << "in ";
		else
			str << "out ";

		if ( info.type == EShaderVariable::VaryingsBlock )
		{
			str << info.typeName << " {\n";

			for (auto& fld : info.fields)
			{
				str << "\t";
				CHECK_ERR( _TranslateField( fld, INOUT str ) );
				str << ";\n";
			}
			str << "} " << info.name << (info.arraySize.IsNotArray() ? "" : (info.arraySize.IsDynamicArray() ? "[]" : "["_str << info.arraySize.Size() << "]"));
		}
		else
		{
			ASSERT( info.fields.Empty() );	// TODO

			CHECK_ERR( TranslateLocalVar( info, INOUT str ) );
		}

		str << ";\n";
		return true;
	}
	
/*
=================================================
	_RecursiveInitConstStruct
=================================================
*/
	bool GLSL_DstLanguage::_RecursiveInitConstStruct (const Array<TypeInfo> &fields, const glslang::TConstUnionArray& cu_arr, INOUT int &index, OUT String &src)
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

					CU_ToArray_Func	func{ this };
					
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
	bool GLSL_DstLanguage::TranslateConstant (const glslang::TConstUnionArray &cu_arr, const TypeInfo &info, INOUT String &str)
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
				CU_ToArray_Func	func{ this };

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
				CU_ToArray_Func	func{ this };

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
	bool GLSL_DstLanguage::_TranslateConst (glslang::TIntermTyped* typed, Symbol const& info, OUT String &str)
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
	bool GLSL_DstLanguage::_TranslateGlobal (glslang::TIntermTyped* typed, Symbol const& info, OUT String &str)
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
	bool GLSL_DstLanguage::TranslateValue (VariantCRef value, INOUT String &src) const
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

			if ( val != MinValue(val) )
				src << ToString( val ) << "L";
			else
				src << "(" << (MinValue(val)+1) << "L - 1)";
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
			src << ToString( value.Get<ulong>() ) << "UL";
		}
		else
		// half
		if ( value.GetValueTypeId() == TypeIdOf<half>() )
		{
			src << String().FormatF( float(value.Get<half>()), StringFormatF().Fmt(0,8).CutZeros() ) << "HF";
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
			src << String().FormatF( value.Get<double>(), StringFormatF().Fmt(0,16).CutZeros() ) << "LF";
		}
		else
			RETURN_ERR( "unsupported scalar type" );

		return true;
	}


}	// PipelineCompiler
