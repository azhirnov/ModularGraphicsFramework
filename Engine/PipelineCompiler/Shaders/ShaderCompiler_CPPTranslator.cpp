// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Translate from GLSL/GXSL to C++ code.
*/

#include "Engine/PipelineCompiler/Shaders/ShaderCompiler_Translator.h"
#include "Engine/PipelineCompiler/Shaders/ShaderCompiler_NameValidator.h"
#include "Engine/PipelineCompiler/Common/ToGLSL.h"

namespace PipelineCompiler
{

	//
	// C++ Language
	//

	class CPP_DstLanguage : public Translator::IDstLanguage
	{
	// types
	private:
		using Externals_t		= Array< Symbol >;
		using GlobalVars_t		= Array<Pair< glslang::TIntermTyped*, Symbol >>;
		using StringSet_t		= HashSet< String >;
		using BuiltinMap_t		= HashMap< String, Pair< String, EShader::bits > >;
		using NamedExternals_t	= Map< SymbolID, String >;	// used for unnameed buffers


	// variables
	private:
		Externals_t				_externals;
		GlobalVars_t			_globalVars;
		StringSet_t				_builtinList;
		NamedExternals_t		_externalNames;
		ShaderVarNameValidator	_nameValidator;
		const EShader::type		_shaderType;

		uint					_sharedCounter = 0;
		uint					_barrierCounter = 0;


	// methods
	public:
		explicit CPP_DstLanguage (EShader::type shaderType) :
			_nameValidator{EShaderFormat::Soft_100_Exe}, _shaderType{shaderType}
		{}

		~CPP_DstLanguage ()
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

		bool DeclExternalTypes () const	override		{ return true; }
		bool ReplaceStructByBuffer () const override	{ return false; }

	private:
		String _TranslateFunctionName (StringCRef sign);
		bool _TranslateArg (const Symbol &, INOUT String &res);
		bool _TranslateField (const TypeInfo &, INOUT String &src);

		bool _TranslateBuffer (Symbol const& info, OUT String &str);
		bool _TranslateImage (Symbol const& info, OUT String &str);
		bool _TranslateConst (glslang::TIntermTyped* typed, Symbol const& info, OUT String &str);
		bool _TranslateShared (Symbol const& info, OUT String &str);
		bool _TranslateBuiltin (StringCRef name, OUT String &str) const;
		bool _TranslateBarrier (uint index, OUT String &str) const;
		bool _TranslateGlobal (glslang::TIntermTyped* typed, Symbol const& info, OUT String &str);
		
		bool _TranslateOperator0 (glslang::TOperator op, const TypeInfo &resultType, INOUT String &src);
		bool _TranslateOperator1 (glslang::TOperator op, const TypeInfo &resultType, ArrayCRef<String> args, ArrayCRef<TypeInfo const*> argTypes, StringCRef allArgs, INOUT String &src);
		bool _TranslateOperator2 (glslang::TOperator op, const TypeInfo &resultType, ArrayCRef<String> args, ArrayCRef<TypeInfo const*> argTypes, StringCRef allArgs, INOUT String &src);
		bool _TranslateOperatorN (glslang::TOperator op, const TypeInfo &resultType, ArrayCRef<String> args, ArrayCRef<TypeInfo const*> argTypes, StringCRef allArgs, INOUT String &src);

		bool _RecursiveInitConstStruct (const Array<TypeInfo> &fields, const glslang::TConstUnionArray& cu_arr, INOUT int &index, OUT String &src);

		static String _ToString (EShaderMemoryModel::type value);
		static String _ToString (EShaderVariable::type value);
		static String _ToImageType (EShaderVariable::type value);
		static String _ToTextureType (EShaderVariable::type value);

		static BuiltinMap_t const& _GetBuiltinMapping ();
	};

	

/*
=================================================
	_TranslateGXSLtoCPP
=================================================
*/
	static bool TranslateShaderInfo (const glslang::TIntermediate* intermediate, bool skipExternals, Translator &translator);

	bool ShaderCompiler::_TranslateGXSLtoCPP (const Config &cfg, const _GLSLangResult &glslangData, OUT String &log, OUT BinaryArray &result) const
	{
		CHECK_ERR(	EShaderFormat::GetApiFormat( cfg.source ) == EShaderFormat::GXSL or
					EShaderFormat::GetApiFormat( cfg.source ) == EShaderFormat::GLSL or
					EShaderFormat::GetApiFormat( cfg.source ) == EShaderFormat::VKSL );
		CHECK_ERR(	cfg.target == EShaderFormat::Soft_100_Exe );
	
		// not supported here
		ASSERT( not cfg.optimize );

		const glslang::TIntermediate* intermediate = glslangData.prog.getIntermediate( glslangData.shader->getStage() );
		CHECK_ERR( intermediate );

		TIntermNode*	root	= intermediate->getTreeRoot();
		Translator		translator{ log };

		translator.states.useGXrules	= intermediate->getSource() == glslang::EShSourceGxsl;
		translator.states.inlineAll		= cfg.inlineAll or (not translator.states.useGXrules);
		translator.entryPoint			= intermediate->getEntryPointName().c_str();
		translator.language				= new CPP_DstLanguage( ConvertShaderType( glslangData.shader->getStage() ) );

		CHECK_ERR( TranslateShaderInfo( intermediate, cfg.skipExternals, translator ) );
		
		CHECK_ERR( translator.Main( root, cfg.skipExternals ) );
		
		StringParser::IncreaceIndent( translator.src );
		
		"#ifdef GRAPHICS_API_SOFT\n"
		"namespace SWShaderLang {\n"
		"namespace {\n\n"		// anonymous namespace to avoid ODR problem

		"#	define INOUT\n"
		"#	define IN\n"
		"#	define OUT\n\n"
			>> translator.src;

		translator.src <<
			"}		// anonymous namespace\n"
			"}		// SWShaderLang\n"
			"#endif	// GRAPHICS_API_SOFT\n\n";

		result	= BinArrayCRef::From( translator.src );
		return true;
	}
//-----------------------------------------------------------------------------



/*
=================================================
	TranslateShaderInfo
=================================================
*/
	static bool TranslateShaderInfo (const glslang::TIntermediate*, bool, Translator &translator)
	{
		String &	src = translator.src;

		// TODO
		return true;
	}

/*
=================================================
	TranslateLocalVar
=================================================
*/
	bool CPP_DstLanguage::TranslateLocalVar (const Symbol &t, INOUT String &res)
	{
		CHECK_ERR( not t.name.Empty() );

		// access
		//if ( memoryModel != EShaderMemoryModel::Default )
		//	res << _ToString( memoryModel ) << ' ';

		// read-only
		if ( t.qualifier[ EVariableQualifier::Constant ] )
			res << "const ";

		// layout
		//if ( t.format != EPixelFormat::Unknown )
		//	res << "layout(" << _ToString( t.format ) << ") ";

		// precision
		//if ( t.precision != EPrecision::Default )
		//	res << _ToString( t.precision ) << ' ';
		
		_TranslateField( t, res );
		return true;
	}
	
/*
=================================================
	_TranslateField
=================================================
*/
	bool CPP_DstLanguage::_TranslateField (const TypeInfo &t, INOUT String &res)
	{
		const bool	is_atomic = t.qualifier[ EVariableQualifier::Volatile ];

		if ( t.arraySize.IsStaticArray() )
			res << "SArr<";

		if ( is_atomic )
			res << "Atomic<";

		// type
		if ( not t.typeName.Empty() ) {
			res << t.typeName;
		} else {
			res << _ToString( t.type );
		}

		if ( is_atomic )
			res << ">";
		
		if ( t.arraySize.IsStaticArray() )
			res << "," << t.arraySize.Size() << ">";

		res << " " << t.name << (t.arraySize.IsDynamicArray() ? "[]" : "");
		return true;
	}

/*
=================================================
	RecursiveFindVolatile
=================================================
*/
	static bool RecursiveFindVolatile (const Translator::TypeInfo &info)
	{
		if ( info.qualifier[ EVariableQualifier::Volatile ] )
			return true;

		for (auto& fld : info.fields)
		{
			if ( RecursiveFindVolatile( fld ) )
				return true;
		}
		return false;
	}
	
/*
=================================================
	TranslateStruct
=================================================
*/
	bool CPP_DstLanguage::TranslateStruct (const TypeInfo &info, INOUT String &src)
	{
		bool	skip_operators	= false;

		src << "struct " << info.typeName << "\n{\n";
		for (auto& fld : info.fields)
		{
			src << "\t";
			CHECK_ERR( _TranslateField( fld, INOUT src ) );
			src << ";\n";

			skip_operators |= RecursiveFindVolatile( fld );
			skip_operators |= fld.arraySize.IsDynamicArray();
		}

		if ( not skip_operators )
		{
			// create constructors
			src << "\n\t" << info.typeName << " () {}\n";
			src << "\t" << info.typeName << " (" << info.typeName << " &&) = default;\n";
			src << "\t" << info.typeName << " (const " << info.typeName << " &) = default;\n";

			src << "\texplicit " << info.typeName << "(";
			FOR( j, info.fields )
			{
				src << (j ? ", " : "") << "const ";
				TranslateType( info.fields[j], INOUT src );
				src << " &" << info.fields[j].name;
			}

			src << ") : ";
			FOR( j, info.fields ) {
				src << (j ? ", " : "") << info.fields[j].name << '{' << info.fields[j].name << '}';
			}
			src << " {}\n\n";

			// create compare operators
			src << "\t" << info.typeName << "& operator = (" << info.typeName << " &&) = default;\n";
			src << "\t" << info.typeName << "& operator = (const " << info.typeName << " &) = default;\n";

			src << "\tBool operator == (const " << info.typeName << " &right) const {\n\t\treturn";
			FOR( j, info.fields ) {
				src << (j ? " &&\n\t\t\t\t" : "\t") << "all( " << info.fields[j].name << " == right." << info.fields[j].name << " )";
			}
			src << ";\n\t}\n";

			src << "\tBool operator != (const " << info.typeName << " &right) const { return !(*this == right); }\n\n";
		}

		src << "};\n\n";
		return true;
	}
	
/*
=================================================
	TranslateType
=================================================
*/
	bool CPP_DstLanguage::TranslateType (const TypeInfo &t, INOUT String &res)
	{
		// access
		//if ( t.memoryModel != EShaderMemoryModel::Default )
		//	res << _ToString( t.memoryModel ) << ' ';

		// layout
		//if ( t.format != EPixelFormat::Unknown )
		//	res << "layout(" << _ToString( t.format ) << ") ";

		// precision
		//if ( t.precision != EPrecision::Default )
		//	res << _ToString( t.precision ) << ' ';

		const bool	is_atomic = t.qualifier[ EVariableQualifier::Volatile ];
		
		if ( t.arraySize.IsStaticArray() )
			res << "SArr<";

		if ( is_atomic )
			res << "Atomic<";

		// type
		if ( not t.typeName.Empty() ) {
			res << t.typeName;
		} else {
			res << _ToString( t.type );
		}

		if ( is_atomic )
			res << ">";
		
		if ( t.arraySize.IsDynamicArray() )
			res << "[]";
		else if ( t.arraySize.IsStaticArray() )
			res << "," << t.arraySize.Size() << ">";

		return true;
	}
	
/*
=================================================
	TranslateName
=================================================
*/
	bool CPP_DstLanguage::TranslateName (const Symbol &t, INOUT String &src)
	{
		if ( t.name.StartsWithIC( "gl_" ) )
		{
			_builtinList.Add( t.name );
		}

		src << t.name;
		return true;
	}
	
/*
=================================================
	ValidateSymbolName
=================================================
*/
	bool CPP_DstLanguage::ValidateSymbolName (INOUT Symbol &symb, uint fnScopeId)
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
	bool CPP_DstLanguage::ValidateFieldName (INOUT TypeInfo &info)
	{
		return _nameValidator.ValidateField( INOUT info.name );
	}

/*
=================================================
	_TranslateFunctionName
=================================================
*/
	String CPP_DstLanguage::_TranslateFunctionName (StringCRef signature)
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
	bool CPP_DstLanguage::TranslateExternal (glslang::TIntermTyped* typed, const Symbol &info, INOUT String &src)
	{
		if ( info.qualifier[ EVariableQualifier::Constant ] ) {
			CHECK_ERR( _TranslateConst( typed, info, INOUT src ) );
		}
		else
		if ( not info.isGlobal ) {
			_globalVars.PushBack({ typed, info });
		}
		else
		{
			_externals.PushBack( info );

			if ( info.name.Empty() )
			{
				// create unique name
				String		name	= "external_";
				const usize	len		= name.Length();
				
				for (usize i = 0; i < 1000; ++i)
				{
					name << (_externalNames.Count() + i);

					bool	found = true;

					FOR( j, _externalNames ) {
						if ( _externalNames[i].second == name ) {
							found = false;
							break;
						}
					}

					if ( found )
						break;
				}

				_externalNames.Add( info.id, name );
				_externals.Back().name = name;
			}
		}
		return true;
	}
	
/*
=================================================
	TranslateOperator
=================================================
*/
	bool CPP_DstLanguage::TranslateOperator (glslang::TOperator op, const TypeInfo &resultType, ArrayCRef<String> args, ArrayCRef<TypeInfo const*> argTypes, INOUT String &src)
	{
		CHECK_ERR( args.Count() == argTypes.Count() );

		String	all_args;

		FOR( i, args ) {
			all_args << (i ? ", " : "(") << args[i];
		}
		all_args << ")";
		
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
	bool CPP_DstLanguage::_TranslateOperator0 (glslang::TOperator op, const TypeInfo &, INOUT String &src)
	{
		switch ( op )
		{
			case glslang::TOperator::EOpEmitVertex :				src << "EmitVertex()";			break;
			case glslang::TOperator::EOpEndPrimitive :				src << "EndPrimitive()";		break;

			case glslang::TOperator::EOpBarrier :
				src << "__barrier_obj" << _barrierCounter << ".Wait()";
				++_barrierCounter;
				break;

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
	bool CPP_DstLanguage::_TranslateOperator1 (glslang::TOperator op, const TypeInfo &, ArrayCRef<String>,
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
			case glslang::TOperator::EOpAny :					src << "any" << all_args;				break;
			case glslang::TOperator::EOpAll :					src << "all" << all_args;				break;
			case glslang::TOperator::EOpRadians :				src << "radians" << all_args;			break;
			case glslang::TOperator::EOpDegrees :				src << "degrees" << all_args;			break;
			case glslang::TOperator::EOpSin :					src << "glm::sin" << all_args;			break;
			case glslang::TOperator::EOpCos :					src << "glm::cos" << all_args;			break;
			case glslang::TOperator::EOpTan :					src << "glm::tan" << all_args;			break;
			case glslang::TOperator::EOpAsin :					src << "glm::asin" << all_args;			break;
			case glslang::TOperator::EOpAcos :					src << "glm::acos" << all_args;			break;
			case glslang::TOperator::EOpAtan :					src << "glm::atan" << all_args;			break;
			case glslang::TOperator::EOpSinh :					src << "glm::sinh" << all_args;			break;
			case glslang::TOperator::EOpCosh :					src << "glm::cosh" << all_args;			break;
			case glslang::TOperator::EOpTanh :					src << "glm::tanh" << all_args;			break;
			case glslang::TOperator::EOpAsinh :					src << "glm::asinh" << all_args;		break;
			case glslang::TOperator::EOpAcosh :					src << "glm::acosh" << all_args;		break;
			case glslang::TOperator::EOpAtanh :					src << "glm::atanh" << all_args;		break;
			case glslang::TOperator::EOpExp :					src << "glm::exp" << all_args;			break;
			case glslang::TOperator::EOpLog :					src << "glm::log" << all_args;			break;
			case glslang::TOperator::EOpExp2 :					src << "glm::exp2" << all_args;			break;
			case glslang::TOperator::EOpLog2 :					src << "glm::log2" << all_args;			break;
			case glslang::TOperator::EOpSqrt :					src << "glm::sqrt" << all_args;			break;
			case glslang::TOperator::EOpInverseSqrt :			src << "inversesqrt" << all_args;		break;
			case glslang::TOperator::EOpAbs :					src << "glm::abs" << all_args;			break;
			case glslang::TOperator::EOpSign :					src << "glm::sign" << all_args;			break;
			case glslang::TOperator::EOpFloor :					src << "glm::floor" << all_args;		break;
			case glslang::TOperator::EOpTrunc :					src << "glm::trunc" << all_args;		break;
			case glslang::TOperator::EOpRound :					src << "glm::round" << all_args;		break;
			case glslang::TOperator::EOpRoundEven :				src << "roundEven" << all_args;			break;
			case glslang::TOperator::EOpCeil :					src << "glm::ceil" << all_args;			break;
			case glslang::TOperator::EOpFract :					src << "glm::fract" << all_args;		break;
			case glslang::TOperator::EOpIsNan :					src << "glm::isnan" << all_args;		break;
			case glslang::TOperator::EOpIsInf :					src << "glm::isinf" << all_args;		break;
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
			//case glslang::TOperator::EOpNoise :				src << "noise" << all_args;						break;	// deprecated
					
			case glslang::TOperator::EOpAtomicCounter :			src << "atomicCounter" << all_args;				break;
			case glslang::TOperator::EOpAtomicCounterIncrement:	src << "atomicCounterIncrement" << all_args;	break;
			case glslang::TOperator::EOpAtomicCounterDecrement:	src << "atomicCounterDecrement" << all_args;	break;

			case glslang::TOperator::EOpTextureQuerySize :		src << "textureSize" << all_args;				break;
			case glslang::TOperator::EOpTextureQueryLevels :	src << "textureQueryLevels" << all_args;		break;
			case glslang::TOperator::EOpTextureQuerySamples :	src << "textureSamples" << all_args;			break;
			default :											RETURN_ERR( "unknown operator!" );
		}
		return true;
	}
	
/*
=================================================
	_TranslateOperator2
=================================================
*/
	bool CPP_DstLanguage::_TranslateOperator2 (glslang::TOperator op, const TypeInfo &resultType, ArrayCRef<String> args,
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
			
			if ( lhs_type.type != sh_type )		temp << _ToString( sh_type ) << '(' << args[0] << ')';
			else								temp << args[0];
			temp << ", ";
			if ( rhs_type.type != sh_type )		temp << _ToString( sh_type ) << '(' << args[1] << ')';
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

			case glslang::TOperator::EOpAtan :						src << "glm::atan" << all_args;					break;
			case glslang::TOperator::EOpImageLoad :					src << "imageLoad" << all_args;					break;
			case glslang::TOperator::EOpPow :						src << "glm::pow" << all_args;					break;
			case glslang::TOperator::EOpModf :						src << "glm::modf" << all_args;					break;
			case glslang::TOperator::EOpMin :						src << "glm::min" << all_args;					break;
			case glslang::TOperator::EOpMax :						src << "glm::max" << all_args;					break;
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
	bool CPP_DstLanguage::_TranslateOperatorN (glslang::TOperator op, const TypeInfo &, ArrayCRef<String>,
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
			case glslang::TOperator::EOpClamp :						src << "glm::clamp" << all_args;			break;	// 3 args
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
	bool CPP_DstLanguage::TranslateFunctionCall (StringCRef signature, const TypeInfo &, ArrayCRef<String> args, ArrayCRef<TypeInfo const*>, INOUT String &src)
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
	bool CPP_DstLanguage::MustBeInlined (StringCRef, ArrayCRef<TypeInfo const*>)
	{
		return false;
	}

/*
=================================================
	_TranslateArg
=================================================
*/
	bool CPP_DstLanguage::_TranslateArg (const Symbol &t, INOUT String &res)
	{
		// qualifies
		if ( t.qualifier[ EVariableQualifier::Constant ] )
			res << "const ";
		else
		if ( t.qualifier[ EVariableQualifier::InArg ] and t.qualifier[ EVariableQualifier::OutArg ] )
			res << "INOUT ";
		else
		if ( t.qualifier[ EVariableQualifier::InArg ] )
			res << "IN ";
		else
		if ( t.qualifier[ EVariableQualifier::OutArg ] )
			res << "OUT ";
		
		const bool	is_atomic	= t.qualifier[ EVariableQualifier::Volatile ];

		if ( t.arraySize.IsStaticArray() )
			res << "SArr<";

		if ( is_atomic )
			res << "Atomic<";

		// type
		if ( not t.typeName.Empty() ) {
			res << t.typeName;
		} else {
			res << _ToString( t.type );
		}
		
		if ( is_atomic )
			res << ">";

		if ( t.arraySize.IsStaticArray() )
			res << "," << t.arraySize.Size() << ">";

		if ( t.qualifier[ EVariableQualifier::InArg ] and t.qualifier[ EVariableQualifier::OutArg ] )
			res << " &";
		else
		if ( t.qualifier[ EVariableQualifier::InArg ] )
			res << " ";
		else
		if ( t.qualifier[ EVariableQualifier::OutArg ] )
			res << " &";
		else
			res << " ";

		res << t.name;
		
		if ( t.arraySize.IsDynamicArray() )
			res << "[]";

		return true;
	}
	
/*
=================================================
	TranslateFunctionDecl
=================================================
*/
	bool CPP_DstLanguage::TranslateFunctionDecl (StringCRef signature, const TypeInfo &resultType, ArrayCRef<Symbol> args, INOUT String &src)
	{
		src << "static ";

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
	bool CPP_DstLanguage::TranslateSwizzle (const TypeInfo &, const TypeInfo &, StringCRef val, ArrayCRef<uint> swizzle, INOUT String &src)
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
	bool CPP_DstLanguage::TranslateEntry (const TypeInfo &ret, StringCRef, ArrayCRef<Symbol> args, StringCRef body, OUT String &entryPoint, INOUT String &src)
	{
		CHECK_ERR( args.Empty() );
		CHECK_ERR( ret.type == EShaderVariable::Void );

		entryPoint = "##main##";	// TODO

		src.Clear();
		src << "static void " << entryPoint << " (const Impl::SWShaderHelper &_helper_)\n"
			<< "{\n"
			<< "	// prepare externals\n";

		// externals
		FOR( i, _externals )
		{
			Symbol const&	info = _externals[i];
			
			if ( EShaderVariable::IsBuffer( info.type ) ) {
				CHECK_ERR( _TranslateBuffer( info, INOUT src ) );
			}
			else
			if ( EShaderVariable::IsImage( info.type ) or EShaderVariable::IsTexture( info.type ) ) {
				CHECK_ERR( _TranslateImage( info, INOUT src ) );
			}
			else
			if ( info.qualifier[ EVariableQualifier::Shared ] ) {
				CHECK_ERR( _TranslateShared( info, INOUT src ) );
			}
		}

		// barriers
		for (uint i = 0; i < _barrierCounter; ++i)
		{
			CHECK_ERR( _TranslateBarrier( i, INOUT src ) );
		}

		// builtins
		FOR( i, _builtinList )
		{
			CHECK_ERR( _TranslateBuiltin( _builtinList[i], INOUT src ) );
		}

		// global variables
		FOR( i, _globalVars )
		{
			CHECK_ERR( _TranslateGlobal( _globalVars[i].first, _globalVars[i].second, INOUT src ) );
		}

		String	body_src = body;
		StringParser::IncreaceIndent( INOUT body_src, "\t\t" );
		src << "\n	// shader\n\t{\n" << body_src << "\t}\n}\n";

		return true;
	}
		
/*
=================================================
	TranslateStructAccess
=================================================
*/
	bool CPP_DstLanguage::TranslateStructAccess (SymbolID id, const TypeInfo &stType, StringCRef objName, const TypeInfo &fieldType, INOUT String &src)
	{
		if ( not objName.Empty() )
		{
			if ( EShaderVariable::IsBuffer( stType.type ) )
				src << objName << "->";
			else
				src << objName << '.';
		}
		else
		if ( EShaderVariable::IsBuffer( stType.type ) )
		{
			NamedExternals_t::iterator	iter;
			CHECK_ERR( _externalNames.Find( id, OUT iter ) );

			src << iter->second << "->";
		}

		src << fieldType.name;
		return true;
	}
//-----------------------------------------------------------------------------


	
/*
=================================================
	_ToString
=================================================
*/
	String CPP_DstLanguage::_ToString (EShaderMemoryModel::type value)
	{
		switch ( value )
		{
			case EShaderMemoryModel::Default :
			case EShaderMemoryModel::Coherent :		return "Impl::EStorageAccess::Coherent";
			case EShaderMemoryModel::Volatile :		return "Impl::EStorageAccess::Volatile";
			case EShaderMemoryModel::Restrict :		return "Impl::EStorageAccess::Restrict";
			case EShaderMemoryModel::ReadOnly :		return "Impl::EStorageAccess::ReadOnly";
			case EShaderMemoryModel::WriteOnly :	return "Impl::EStorageAccess::WriteOnly";
		}
		RETURN_ERR( "unknown memory model type!" );
	}
	
/*
=================================================
	_ToString
=================================================
*/
	String CPP_DstLanguage::_ToString (EShaderVariable::type value)
	{
		switch ( value )
		{
			// glm types
			case EShaderVariable::Void :		return "void";
			case EShaderVariable::Bool :		return "Bool";
			case EShaderVariable::Bool2 :		return "Bool2";
			case EShaderVariable::Bool3 :		return "Bool3";
			case EShaderVariable::Bool4 :		return "Bool4";
			case EShaderVariable::Int :			return "Int";
			case EShaderVariable::Int2 :		return "Int2";
			case EShaderVariable::Int3 :		return "Int3";
			case EShaderVariable::Int4 :		return "Int4";
			case EShaderVariable::UInt :		return "UInt";
			case EShaderVariable::UInt2 :		return "UInt2";
			case EShaderVariable::UInt3 :		return "UInt3";
			case EShaderVariable::UInt4 :		return "UInt4";
			case EShaderVariable::Long :		return "Long";
			case EShaderVariable::Long2 :		return "Long2";
			case EShaderVariable::Long3 :		return "Long3";
			case EShaderVariable::Long4 :		return "Long4";
			case EShaderVariable::ULong :		return "ULongt";
			case EShaderVariable::ULong2 :		return "ULong2";
			case EShaderVariable::ULong3 :		return "ULong3";
			case EShaderVariable::ULong4 :		return "ULong4";
			case EShaderVariable::Float :		return "Float";
			case EShaderVariable::Float2 :		return "Float2";
			case EShaderVariable::Float3 :		return "Float3";
			case EShaderVariable::Float4 :		return "Float4";
			case EShaderVariable::Float2x2 :	return "Float2x2";
			case EShaderVariable::Float2x3 :	return "Float2x3";
			case EShaderVariable::Float2x4 :	return "Float2x4";
			case EShaderVariable::Float3x2 :	return "Float3x2";
			case EShaderVariable::Float3x3 :	return "Float3x3";
			case EShaderVariable::Float3x4 :	return "Float3x4";
			case EShaderVariable::Float4x2 :	return "Float4x2";
			case EShaderVariable::Float4x3 :	return "Float4x3";
			case EShaderVariable::Float4x4 :	return "Float4x4";
			case EShaderVariable::Double :		return "Double";
			case EShaderVariable::Double2 :		return "Double2";
			case EShaderVariable::Double3 :		return "Double3";
			case EShaderVariable::Double4 :		return "Double4";
			case EShaderVariable::Double2x2 :	return "Double2x2";
			case EShaderVariable::Double2x3 :	return "Double2x3";
			case EShaderVariable::Double2x4 :	return "Double2x4";
			case EShaderVariable::Double3x2 :	return "Double3x2";
			case EShaderVariable::Double3x3 :	return "Double3x3";
			case EShaderVariable::Double3x4 :	return "Double3x4";
			case EShaderVariable::Double4x2 :	return "Double4x2";
			case EShaderVariable::Double4x3 :	return "Double4x3";
			case EShaderVariable::Double4x4 :	return "Double4x4";
		}
		RETURN_ERR( "unsupported type!" );
	}

/*
=================================================
	_ToImageType
=================================================
*/
	String CPP_DstLanguage::_ToImageType (EShaderVariable::type value)
	{
		using _vtypeinfo	= _platforms_hidden_::EValueTypeInfo;

		String	str;

		switch ( value & _vtypeinfo::_SAMP_MASK )
		{
			case _vtypeinfo::_SAMP_1D :		str << "Impl::Image1D";				break;
			case _vtypeinfo::_SAMP_1DS :	str << "Impl::Image1DShadow";		break;
			case _vtypeinfo::_SAMP_1DA :	str << "Impl::Image1DArray";		break;
			case _vtypeinfo::_SAMP_1DAS :	str << "Impl::Image1DArrayShadow";	break;
			case _vtypeinfo::_SAMP_2D :		str << "Impl::Image2D";				break;
			case _vtypeinfo::_SAMP_2DS :	str << "Impl::Image2DShadow";		break;
			case _vtypeinfo::_SAMP_2DMS :	str << "Impl::Image2DMS";			break;
			case _vtypeinfo::_SAMP_2DA :	str << "Impl::Image2DArray";		break;
			case _vtypeinfo::_SAMP_2DAS :	str << "Impl::Image2DArrayShadow";	break;
			case _vtypeinfo::_SAMP_2DAMS :	str << "Impl::Image2DMSArray";		break;
			case _vtypeinfo::_SAMP_3D :		str << "Impl::Image3D";				break;
			case _vtypeinfo::_SAMP_CUBE :	str << "Impl::ImageCube";			break;
			case _vtypeinfo::_SAMP_CUBES :	str << "Impl::ImageCubeShadow";		break;
			case _vtypeinfo::_SAMP_CUBEA :	str << "Impl::ImageCubeArray";		break;
			case _vtypeinfo::_SAMP_BUF :	str << "Impl::ImageBuffer";			break;
			default :						RETURN_ERR( "unsupported sampler type!" );
		}

		str << "< ";

		switch ( value & _vtypeinfo::_TYPE_FLAG_MASK )
		{
			case _vtypeinfo::_INT :		str << "ivec4";		break;
			case _vtypeinfo::_UINT :	str << "uvec4";		break;
			case _vtypeinfo::_FLOAT :	str << "vec4";		break;
		}

		return str;
	}
	
/*
=================================================
	_ToTextureType
=================================================
*/
	String CPP_DstLanguage::_ToTextureType (EShaderVariable::type value)
	{
		using _vtypeinfo	= _platforms_hidden_::EValueTypeInfo;

		String	str;

		switch ( value & _vtypeinfo::_SAMP_MASK )
		{
			case _vtypeinfo::_SAMP_1D :		str << "Impl::Texture1D";				break;
			case _vtypeinfo::_SAMP_1DS :	str << "Impl::Texture1DShadow";			break;
			case _vtypeinfo::_SAMP_1DA :	str << "Impl::Texture1DArray";			break;
			case _vtypeinfo::_SAMP_1DAS :	str << "Impl::Texture1DArrayShadow";	break;
			case _vtypeinfo::_SAMP_2D :		str << "Impl::Texture2D";				break;
			case _vtypeinfo::_SAMP_2DS :	str << "Impl::Texture2DShadow";			break;
			case _vtypeinfo::_SAMP_2DMS :	str << "Impl::Texture2DMS";				break;
			case _vtypeinfo::_SAMP_2DA :	str << "Impl::Texture2DArray";			break;
			case _vtypeinfo::_SAMP_2DAS :	str << "Impl::Texture2DArrayShadow";	break;
			case _vtypeinfo::_SAMP_2DAMS :	str << "Impl::Texture2DMSArray";		break;
			case _vtypeinfo::_SAMP_3D :		str << "Impl::Texture3D";				break;
			case _vtypeinfo::_SAMP_CUBE :	str << "Impl::TextureCube";				break;
			case _vtypeinfo::_SAMP_CUBES :	str << "Impl::TextureCubeShadow";		break;
			case _vtypeinfo::_SAMP_CUBEA :	str << "Impl::TextureCubeArray";		break;
			case _vtypeinfo::_SAMP_BUF :	str << "Impl::TextureBuffer";			break;
			default :						RETURN_ERR( "unsupported sampler type!" );
		}

		str << "< ";

		switch ( value & _vtypeinfo::_TYPE_FLAG_MASK )
		{
			case _vtypeinfo::_INT :		str << "ivec4";		break;
			case _vtypeinfo::_UINT :	str << "uvec4";		break;
			case _vtypeinfo::_FLOAT :	str << "vec4";		break;
		}

		return str;
	}

/*
=================================================
	_TranslateBuffer
=================================================
*/
	bool CPP_DstLanguage::_TranslateBuffer (Symbol const& info, OUT String &str)
	{
		CHECK_ERR( info.arraySize.IsNotArray() );
		CHECK_ERR( not info.name.Empty() );
		CHECK_ERR( not info.typeName.Empty() );

		if ( info.type == EShaderVariable::StorageBlock )
		{
			str << "\tImpl::StorageBuffer< " << info.typeName << ", "
				<< _ToString( info.memoryModel ) << " >  " << info.name
				<< ";    _helper_.GetStorageBuffer( " << info.binding
				<< ", " << info.name << " );\n";
		}
		else
		if ( info.type == EShaderVariable::UniformBlock )
		{
			str << "\tImpl::UniformBuffer< " << info.typeName << " >  " << info.name
				<< ";    _helper_.GetUniformBuffer( " << info.binding
				<< ", " << info.name << " );\n";
		}
		else
		{
			RETURN_ERR( "unknown buffer type!" );
		}
		return true;
	}

/*
=================================================
	_TranslateImage
=================================================
*/
	bool CPP_DstLanguage::_TranslateImage (Symbol const& info, OUT String &str)
	{
		CHECK_ERR( info.arraySize.IsNotArray() );
		CHECK_ERR( not info.name.Empty() );
		
		if ( EShaderVariable::IsImage( info.type ) )
		{
			str << "\t" << _ToImageType( info.type ) << ", " << _ToString( info.memoryModel )
				<< " >  " << info.name << ";    _helper_.GetImage( " << info.binding
				<< ", " << info.name << " );\n";
		}
		else
		{
			str << "\t" << _ToTextureType( info.type ) 
				<< " >  " << info.name << ";    _helper_.GetTexture( " << info.binding
				<< ", " << info.name << " );\n";
		}
		return true;
	}

/*
=================================================
	_TranslateShared
=================================================
*/
	bool CPP_DstLanguage::_TranslateShared (Symbol const& info, OUT String &str)
	{
		CHECK_ERR( not info.name.Empty() );

		str << "\tImpl::SharedMemory< ";
		
		// type
		if ( not info.typeName.Empty() ) {
			str << info.typeName;
		} else {
			str << _ToString( info.type );
		}
		
		str << " >	" << info.name << ";	_helper_.GetShared( "
			<< _sharedCounter << ", ";

		if ( info.arraySize.IsStaticArray() )
			str << info.arraySize.Size();
		else
			str << "1";

		++_sharedCounter;

		str << ", " << info.name << " );\n";
		return true;
	}
	
/*
=================================================
	_TranslateBarrier
=================================================
*/
	bool CPP_DstLanguage::_TranslateBarrier (uint index, OUT String &str) const
	{
		str << "\tImpl::Barrier __barrier_obj" << index << ";"
			<< "\t_helper_.InitBarrier( " << index << ", " << "__barrier_obj" << index << " );\n";
		return true;
	}
	
/*
=================================================
	_TranslateGlobal
=================================================
*/
	bool CPP_DstLanguage::_TranslateGlobal (glslang::TIntermTyped* typed, Symbol const& info, OUT String &str)
	{
		CHECK_ERR( typed->getAsSymbolNode() );

		str << "\t";
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
	_RecursiveInitConstStruct
=================================================
*/
	bool CPP_DstLanguage::_RecursiveInitConstStruct (const Array<TypeInfo> &fields, const glslang::TConstUnionArray& cu_arr, INOUT int &index, OUT String &src)
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
	bool CPP_DstLanguage::TranslateConstant (const glslang::TConstUnionArray &cu_arr, const TypeInfo &info, INOUT String &str)
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
	bool CPP_DstLanguage::_TranslateConst (glslang::TIntermTyped* typed, Symbol const& info, OUT String &str)
	{
		CHECK_ERR( typed->getAsSymbolNode() );
		CHECK_ERR( not info.name.Empty() );

		glslang::TConstUnionArray const&	cu_arr	= typed->getAsSymbolNode()->getConstArray();
		CHECK_ERR( not cu_arr.empty() );

		str << "\t";
		CHECK_ERR( TranslateLocalVar( info, INOUT str ) );
		str << " = ";
		
		CHECK_ERR( TranslateConstant( cu_arr, info, INOUT str ) );
			
		str << ";\n";
		return true;
	}
	
/*
=================================================
	TranslateValue
=================================================
*/
	bool CPP_DstLanguage::TranslateValue (VariantCRef value, INOUT String &src) const
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
				src << ToString( val ) << "LL";
			else
				src << "(" << (MinValue(val)+1) << "LL - 1)";
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
			src << ToString( value.Get<ulong>() ) << "ULL";
		}
		else
		// half
		if ( value.GetValueTypeId() == TypeIdOf<half>() )
		{
			src << String().FormatF( float(value.Get<half>()), StringFormatF().Fmt(0,8).CutZeros() ) << "_h";
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
			src << String().FormatF( value.Get<double>(), StringFormatF().Fmt(0,16).CutZeros() );
		}
		else
			RETURN_ERR( "unsupported scalar type" );

		return true;
	}
	
/*
=================================================
	_TranslateBuiltin
=================================================
*/
	bool CPP_DstLanguage::_TranslateBuiltin (StringCRef name, OUT String &str) const
	{
		BuiltinMap_t::const_iterator	iter;

		CHECK_ERR( _GetBuiltinMapping().Find( name, OUT iter ) );
		CHECK_ERR( iter->second.second[ _shaderType ] );

		str << iter->second.first;
		return true;
	}
	
/*
=================================================
	_GetBuiltinMapping
=================================================
*/
	CPP_DstLanguage::BuiltinMap_t const&  CPP_DstLanguage::_GetBuiltinMapping ()
	{
		static const BuiltinMap_t	mapping = {
			{ "gl_GlobalInvocationID",		{"\tauto& gl_GlobalInvocationID = _helper_.GetComputeShaderState().inGlobalInvocationID;\n", EShader::Compute} },
			{ "gl_LocalInvocationID",		{"\tauto& gl_LocalInvocationID = _helper_.GetComputeShaderState().inLocalInvocationID;\n", EShader::Compute} },
			{ "gl_LocalInvocationIndex",	{"\tauto& gl_LocalInvocationIndex = _helper_.GetComputeShaderState().inLocalInvocationIndex;\n", EShader::Compute} },
			{ "gl_NumWorkGroups",			{"\tauto& gl_NumWorkGroups = _helper_.GetComputeShaderState().inNumWorkGroups;\n", EShader::Compute} },
			{ "gl_WorkGroupID",				{"\tauto& gl_WorkGroupID = _helper_.GetComputeShaderState().inWorkGroupID;\n", EShader::Compute} },
			{ "gl_WorkGroupSize",			{"\tauto& gl_WorkGroupSize = _helper_.GetComputeShaderState().constWorkGroupSize;\n", EShader::Compute} },
			{ "gl_VertexID",				{"\tauto& gl_VertexID = _helper_.GetVertexShaderState().constVertexID;\n", EShader::Vertex} }
		};
		return mapping;
	}

}	// PipelineCompiler
