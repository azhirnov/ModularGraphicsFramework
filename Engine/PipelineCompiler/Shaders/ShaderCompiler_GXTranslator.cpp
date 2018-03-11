// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Translate from GXSL to GLSL.
	This is temporary solution for dynamic arrays problem,
	all functions will be inlined to entry point, then new code
	may be optimized as valid GLSL or compiled to SPIRV.
*/

#include "Engine/PipelineCompiler/Shaders/ShaderCompiler_Translator.h"
#include "Engine/PipelineCompiler/Common/ToGLSL.h"

namespace PipelineCompiler
{

	//
	// GLSL Language
	//

	class GLSL_DstLanguage : public Translator::IDstLanguage
	{
	// types
	private:
		using TypeInfo	= Translator::TypeInfo;


	// variables
	private:
		Translator&		_translator;


	// methods
	public:
		explicit GLSL_DstLanguage (Translator &translator) : _translator{translator}
		{}

		~GLSL_DstLanguage ()
		{}

		bool TranslateLocalVar (const TypeInfo &, INOUT String &src) override;
		bool TranslateStruct (const TypeInfo &, INOUT String &src) override;
		bool TranslateArg (const TypeInfo &, INOUT String &src) override;
		bool TranslateType (const TypeInfo &, INOUT String &src) override;
		bool TranslateName (const TypeInfo &, INOUT String &src) override;

		bool TranslateExternal (glslang::TIntermTyped*, const TypeInfo &, INOUT String &src) override;
		bool TranslateOperator (glslang::TOperator op, const TypeInfo &resultType, ArrayCRef<String> args, ArrayCRef<TypeInfo const*> argTypes, INOUT String &src) override;
		bool TranslateSwizzle (const TypeInfo &type, StringCRef val, StringCRef swizzle, INOUT String &src) override;
		
		bool TranslateEntry (const TypeInfo &ret, StringCRef name, ArrayCRef<TypeInfo> args, INOUT String &src) override;
		bool TranslateStructAccess (const TypeInfo &stType, StringCRef objName, const TypeInfo &fieldType, INOUT String &src) override;

	private:
		bool _TranslateBuffer (glslang::TType const& type, Translator::TypeInfo const& info, OUT String &str);
		bool _TranslateImage (glslang::TType const& type, Translator::TypeInfo const& info, OUT String &str);
		bool _TranslateVarying (glslang::TType const& type, Translator::TypeInfo const& info, OUT String &str);
		bool _TranslateConst (glslang::TIntermTyped* typed, Translator::TypeInfo const& info, OUT String &str);
		bool _TranslateGlobal (glslang::TIntermTyped* typed, Translator::TypeInfo const& info, OUT String &str);
		bool _TranslateShared (Translator::TypeInfo const& info, OUT String &str);
	};

	

/*
=================================================
	_TranslateGXSLtoGLSL
=================================================
*/
	static bool TranslateShaderInfo (const glslang::TIntermediate* intermediate, bool skipExternals, Translator &translator);

	bool ShaderCompiler::_TranslateGXSLtoGLSL (const Config &cfg, const _GLSLangResult &glslangData, OUT String &log, OUT BinaryArray &result) const
	{
		CHECK_ERR(	cfg.source == EShaderSrcFormat::GXSL or
					cfg.source == EShaderSrcFormat::GLSL or
					cfg.source == EShaderSrcFormat::GXSL_Vulkan or
					cfg.source == EShaderSrcFormat::GLSL_Vulkan );
		CHECK_ERR(	cfg.target == EShaderDstFormat::GLSL_Source );
	
		// not supported here
		//ASSERT( not cfg.filterInactive );
		ASSERT( not cfg.optimize );

		const glslang::TIntermediate* intermediate = glslangData.prog.getIntermediate( glslangData.shader->getStage() );
		CHECK_ERR( intermediate );

		TIntermNode*	root	= intermediate->getTreeRoot();
		Translator		translator;

		translator.useGXrules	= intermediate->getSource() == glslang::EShSourceGxsl;
		translator.entryPoint	= intermediate->getEntryPointName().c_str();
		translator.language		= new GLSL_DstLanguage( translator );

		CHECK_ERR( TranslateShaderInfo( intermediate, cfg.skipExternals, translator ) );
		
		CHECK_ERR( translator.Main( root, cfg.skipExternals ) );

		log		<< translator.log;
		result	= BinArrayCRef::From( translator.src );
		return true;
	}
//-----------------------------------------------------------------------------



/*
=================================================
	TranslateShaderInfo
=================================================
*/
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
					switch ( intermediate->getDepth() )
					{
						case glslang::TLayoutDepth::EldAny :		src << "layout (depth_any) out float gl_FragDepth;";		break;
						case glslang::TLayoutDepth::EldGreater :	src << "layout (depth_greater) out float gl_FragDepth;";	break;
						case glslang::TLayoutDepth::EldLess :		src << "layout (depth_less) out float gl_FragDepth;";		break;
						case glslang::TLayoutDepth::EldUnchanged :	src << "layout (depth_unchanged) out float gl_FragDepth;";	break;
						default :									WARNING( "unknown depth layout" );
					}
				}

				// depth export
				if ( intermediate->isDepthReplacing() )
				{
					TODO( "" );
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
			}

			case EShLanguage::EShLangGeometry :
			{
				result._geometry.invocations		= intermediate->getInvocations();
				result._geometry.maxOutputVertices	= intermediate->getVertices();
				result._geometry.inputPrimitive		= ConvertGeometryInputPrimitive( intermediate->getInputPrimitive() );
				result._geometry.outputPrimitive	= ConvertGeometryOutputPrimitive( intermediate->getOutputPrimitive() );
				break;
			}
			*/
			case EShLanguage::EShLangCompute :
			{
				translator.src
					<< "layout (local_size_x=" << intermediate->getLocalSize(0)
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
	TranslateLocalVar
=================================================
*/
	bool GLSL_DstLanguage::TranslateLocalVar (const TypeInfo &t, INOUT String &res)
	{
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
		if ( t.precision != EPrecision::Default )
			res << ToStringGLSL( t.precision ) << ' ';

		// type
		if ( not t.typeName.Empty() ) {
			res << t.typeName;
		} else {
			res << ToStringGLSL( t.type );
		}

		res << " " << t.name << (t.arraySize == 0 ? "" : (t.arraySize == UMax ? "[]" : "["_str << t.arraySize << "]"));
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

		FOR( j, info.fields )
		{
			src << "\t";
			CHECK_ERR( TranslateLocalVar( info.fields[j], INOUT src ) );
			src << ";\n";
		}

		src << "};\n\n";
		return true;
	}
	
/*
=================================================
	TranslateArg
=================================================
*/
	bool GLSL_DstLanguage::TranslateArg (const TypeInfo &t, INOUT String &res)
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
		if ( t.precision != EPrecision::Default )
			res << ToStringGLSL( t.precision ) << ' ';

		// type
		if ( not t.typeName.Empty() ) {
			res << t.typeName;
		} else {
			res << ToStringGLSL( t.type );
		}

		res << (t.arraySize == 0 ? "" : (t.arraySize == UMax ? "[]" : "["_str << t.arraySize << "]"));
		return true;
	}
	
/*
=================================================
	TranslateName
=================================================
*/
	bool GLSL_DstLanguage::TranslateName (const TypeInfo &t, INOUT String &src)
	{
		src << t.name;
		return true;
	}

/*
=================================================
	TranslateExternal
=================================================
*/
	bool GLSL_DstLanguage::TranslateExternal (glslang::TIntermTyped* typed, const TypeInfo &info, INOUT String &str)
	{
		glslang::TType const&	type	= typed->getType();
		auto const&				qual	= type.getQualifier();

		if ( type.getBasicType() == glslang::TBasicType::EbtBlock and
			(qual.storage == glslang::TStorageQualifier::EvqBuffer or qual.storage == glslang::TStorageQualifier::EvqUniform) )
		{
			CHECK_ERR( _TranslateBuffer( type, info, INOUT str ) );
		}
		else
		if ( type.isImage() or type.getSampler().isCombined() ) {
			CHECK_ERR( _TranslateImage( type, info, INOUT str ) );
		}
		else
		if ( qual.storage == glslang::TStorageQualifier::EvqShared ) {
			CHECK_ERR( _TranslateShared( info, INOUT str ) );
		}
		else
		if ( qual.storage == glslang::TStorageQualifier::EvqVaryingIn or qual.storage == glslang::TStorageQualifier::EvqVaryingOut ) {
			CHECK_ERR( _TranslateVarying( type, info, INOUT str ) );
		}
		else
		if ( qual.storage == glslang::TStorageQualifier::EvqConst ) {
			CHECK_ERR( _TranslateConst( typed, info, INOUT str ) );
		}
		else
		if ( qual.storage == glslang::TStorageQualifier::EvqGlobal ) {
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
	bool GLSL_DstLanguage::TranslateOperator (glslang::TOperator op, const TypeInfo &resultType, ArrayCRef<String> args, ArrayCRef<TypeInfo const*> argTypes, INOUT String &src)
	{
		CHECK_ERR( args.Count() == argTypes.Count() );

		String	all_args;

		FOR( i, args ) {
			all_args << (i ? ", " : "( ") << args[i];
		}
		all_args << " )";

		if ( op >= glslang::TOperator::EOpConvIntToBool and op <= glslang::TOperator::EOpConvUint16ToUint64 )
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
		

		if ( args.Count() == 1 )
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
				default :											RETURN_ERR( "unknown operator!" );
			}
			return true;
		}
		
		if ( args.Count() == 2 )
		{
			const bool	is_vec	=	(resultType.arraySize == 0 and EShaderVariable::VecSize( resultType.type ) > 1)		and
									(argTypes[0]->arraySize == 0 and EShaderVariable::VecSize( argTypes[0]->type ) > 1)	and
									(argTypes[1]->arraySize == 0 and EShaderVariable::VecSize( argTypes[1]->type ) > 1);
			
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
				case glslang::TOperator::EOpLeftShiftAssign :			src << '('<<args[0] <<" << ="<< args[1]<<')';	break;
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
	TranslateSwizzle
=================================================
*/
	bool GLSL_DstLanguage::TranslateSwizzle (const TypeInfo &type, StringCRef val, StringCRef swizzle, INOUT String &src)
	{
		src << val << '.' << swizzle;
		return true;
	}
	
/*
=================================================
	TranslateEntry
=================================================
*/
	bool GLSL_DstLanguage::TranslateEntry (const TypeInfo &ret, StringCRef name, ArrayCRef<TypeInfo> args, INOUT String &src)
	{
		return true;
	}
		
/*
=================================================
	TranslateStructAccess
=================================================
*/
	bool GLSL_DstLanguage::TranslateStructAccess (const TypeInfo &stType, StringCRef objName, const TypeInfo &fieldType, INOUT String &src)
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
	bool GLSL_DstLanguage::_TranslateBuffer (glslang::TType const& type, Translator::TypeInfo const& info, OUT String &str)
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
		
		if ( type.getQualifier().storage == glslang::TStorageQualifier::EvqBuffer )
		{
			if ( info.memoryModel != EShaderMemoryModel::Default )
				str << ToStringGLSL( info.memoryModel ) << ' ';

			str << "buffer " << info.typeName;
		}
		else
			str << "uniform " << info.typeName;

		CHECK_ERR( EShaderVariable::IsBuffer( info.type ) and not info.fields.Empty() );
		
		str << "{\n";
		FOR( j, info.fields )
		{
			str << "\t";

			if ( info.fields[j].memoryModel != info.memoryModel and info.fields[j].memoryModel != EShaderMemoryModel::Default )
				str << ToStringGLSL( info.fields[j].memoryModel ) << ' ';

			CHECK_ERR( TranslateLocalVar( info.fields[j], INOUT str ) );
			str << ";\n";
		}
		str << "} " << info.name << ";\n";
		return true;
	}

/*
=================================================
	_TranslateImage
=================================================
*/
	bool GLSL_DstLanguage::_TranslateImage (glslang::TType const& type, Translator::TypeInfo const& info, OUT String &str)
	{
		if ( info.binding != UMax )
			str << "layout(binding=" << info.binding << ") ";

		if ( type.isImage() )
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
		if ( info.precision != EPrecision::Default )
			str << ToStringGLSL( info.precision ) << ' ';

		// type
		if ( not info.typeName.Empty() ) {
			str << info.typeName;
		} else {
			str << ToStringGLSL( info.type );
		}

		str << " " << info.name << (info.arraySize == 0 ? "" : (info.arraySize == UMax ? "[]" : "["_str << info.arraySize << "]")) << ";\n";
		return true;
	}

/*
=================================================
	_TranslateShared
=================================================
*/
	bool GLSL_DstLanguage::_TranslateShared (Translator::TypeInfo const& info, OUT String &str)
	{
		str << "shared ";
		CHECK_ERR( TranslateLocalVar( info, INOUT str ) );
		str << ";\n";
		return true;
	}

/*
=================================================
	_TranslateVarying
=================================================
*/
	bool GLSL_DstLanguage::_TranslateVarying (glslang::TType const& type, Translator::TypeInfo const& info, OUT String &str)
	{
		if ( type.getQualifier().storage == glslang::TStorageQualifier::EvqVaryingIn )
			str << "in ";
		else
			str << "out ";
		
		ASSERT( info.fields.Empty() );	// TODO

		CHECK_ERR( TranslateLocalVar( info, INOUT str ) );
		str << ";\n";
		return true;
	}
	
/*
=================================================
	_TranslateConst
=================================================
*/
	bool GLSL_DstLanguage::_TranslateConst (glslang::TIntermTyped* typed, Translator::TypeInfo const& info, OUT String &str)
	{
		CHECK_ERR( typed->getAsSymbolNode() );

		glslang::TType const&				type	= typed->getType();
		glslang::TConstUnionArray const&	cu_arr	= typed->getAsSymbolNode()->getConstArray();

		CHECK_ERR( TranslateLocalVar( info, INOUT str ) );
		str << " = ";

		if ( type.isArray() )
		{
			Translator::TypeInfo	scalar_info = info;		scalar_info.arraySize = 0;

			str << "{ ";
			
			DeserializedShader::Constant::ValueArray_t	values;
			CHECK_ERR( DeserializeConstant::Process( scalar_info.type, cu_arr, OUT values ) );
		
			FOR( i, values )
			{
				CU_ToArray_Func	func;

				str << (i ? ", " : "");
				values[i].Apply( func );

				CHECK_ERR( TranslateOperator( glslang::TOperator::EOpConstructGuardStart,
											  scalar_info, func.GetStrings(), func.GetTypes(), INOUT str ) );
			}

			str << " };\n";
		}
		else
		if ( type.isStruct() )
		{
			TODO( "" );
		}
		else
		{
			DeserializedShader::Constant::ValueArray_t	values;
			CHECK_ERR( DeserializeConstant::Process( info.type, cu_arr, OUT values ) );

			FOR( i, values )
			{
				CU_ToArray_Func	func;

				str << (i ? ", " : "");
				values[i].Apply( func );

				CHECK_ERR( TranslateOperator( glslang::TOperator::EOpConstructGuardStart,
											  info, func.GetStrings(), func.GetTypes(), INOUT str ) );
			}
			str << ";\n";
		}
		return true;
	}
	
/*
=================================================
	_TranslateGlobal
=================================================
*/
	bool GLSL_DstLanguage::_TranslateGlobal (glslang::TIntermTyped* typed, Translator::TypeInfo const& info, OUT String &str)
	{
		CHECK_ERR( typed->getAsSymbolNode() );

		glslang::TType const&				type	= typed->getType();
		glslang::TQualifier const&			qual	= type.getQualifier();
		glslang::TConstUnionArray const&	cu_arr	= typed->getAsSymbolNode()->getConstArray();

		CHECK_ERR( TranslateLocalVar( info, INOUT str ) );

		if ( type.isArray() )
		{
			Translator::TypeInfo	scalar_info = info;		scalar_info.arraySize = 0;

			DeserializedShader::Constant::ValueArray_t	values;
			CHECK_ERR( DeserializeConstant::Process( scalar_info.type, cu_arr, OUT values ) );
		
			if ( not values.Empty() )
			{
				str << " = { ";

				FOR( i, values )
				{
					CU_ToArray_Func	func;

					str << (i ? ", " : "");
					values[i].Apply( func );

					CHECK_ERR( TranslateOperator( glslang::TOperator::EOpConstructGuardStart,
												  scalar_info, func.GetStrings(), func.GetTypes(), INOUT str ) );
				}
				str << " };\n";
			}
			else
				str << ";\n";
		}
		else
		if ( type.isStruct() )
		{
			TODO( "" );
		}
		else
		{
			DeserializedShader::Constant::ValueArray_t	values;
			CHECK_ERR( DeserializeConstant::Process( info.type, cu_arr, OUT values ) );
		
			if ( not values.Empty() )
			{
				str << " = ";

				FOR( i, values )
				{
					CU_ToArray_Func	func;

					str << (i ? ", " : "");
					values[i].Apply( func );

					CHECK_ERR( TranslateOperator( glslang::TOperator::EOpConstructGuardStart,
												  info, func.GetStrings(), func.GetTypes(), INOUT str ) );
				}
				str << ";\n";
			}
			else
				str << ";\n";
		}
		return true;
	}


}	// PipelineCompiler
