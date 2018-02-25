// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/PipelineCompiler/Shaders/LunarGLASS_Include.h"
#include "Engine/PipelineCompiler/Shaders/ShaderCompiler.h"
	
namespace PipelineCompiler
{

/*
=================================================
	ConvertBasicType
=================================================
*/
	inline EShaderVariable::type  ConvertBasicType (const glslang::TBasicType &basicType, int vectorSize = 0, int matrixCols = 0, int matrixRows = 0)
	{
		EShaderVariable::type	var_type = EShaderVariable::Void;
		
		switch ( basicType )
		{
			case glslang::TBasicType::EbtVoid :			return EShaderVariable::Void;

			case glslang::TBasicType::EbtFloat :		var_type = EShaderVariable::Float;	break;
			case glslang::TBasicType::EbtDouble :		var_type = EShaderVariable::Double;	break;
			#ifdef AMD_EXTENSIONS
			//case glslang::TBasicType::EbtFloat16 :	var_type = EShaderVariable::Half;	break;
			#endif
			case glslang::TBasicType::EbtInt :			var_type = EShaderVariable::Int;	break;
			case glslang::TBasicType::EbtUint :			var_type = EShaderVariable::UInt;	break;
			case glslang::TBasicType::EbtInt64 :		var_type = EShaderVariable::Long;	break;
			case glslang::TBasicType::EbtUint64 :		var_type = EShaderVariable::ULong;	break;
			#ifdef AMD_EXTENSIONS
			//case glslang::TBasicType::EbtInt16 :		var_type = EShaderVariable::Short;	break;
			//case glslang::TBasicType::EbtUint16 :		var_type = EShaderVariable::UShort;	break;
			#endif
			case glslang::TBasicType::EbtBool :			var_type = EShaderVariable::Bool;	break;
			default :									RETURN_ERR( "unknown basic type!" );
		}

		if ( vectorSize > 0 )
			var_type = EShaderVariable::ToVec( var_type, vectorSize );
		else
		if ( matrixCols > 0 and matrixRows > 0 )
			var_type = EShaderVariable::ToMat( var_type, matrixCols, matrixRows );
		
		return var_type;
	}
	
/*
=================================================
	ConvertImageLayoutFormat
=================================================
*/
	inline EPixelFormat::type  ConvertImageLayoutFormat (glslang::TLayoutFormat format)
	{
		switch ( format )
		{
			case glslang::TLayoutFormat::ElfRgba32f :		return EPixelFormat::RGBA32F;
			case glslang::TLayoutFormat::ElfRgba16f :		return EPixelFormat::RGBA16F;
			case glslang::TLayoutFormat::ElfR32f :			return EPixelFormat::R32F;
			case glslang::TLayoutFormat::ElfRgba8 :			return EPixelFormat::RGBA8_UNorm;
			case glslang::TLayoutFormat::ElfRgba8Snorm :	return EPixelFormat::RGBA8_SNorm;
			case glslang::TLayoutFormat::ElfRg32f :			return EPixelFormat::RG32F;
			case glslang::TLayoutFormat::ElfRg16f :			return EPixelFormat::RG16F;
			case glslang::TLayoutFormat::ElfR11fG11fB10f :	return EPixelFormat::RGB_11_11_10F;
			case glslang::TLayoutFormat::ElfR16f :			return EPixelFormat::R16F;
			case glslang::TLayoutFormat::ElfRgba16 :		return EPixelFormat::RGBA16_UNorm;
			case glslang::TLayoutFormat::ElfRgb10A2 :		return EPixelFormat::RGB10_A2_UNorm;
			case glslang::TLayoutFormat::ElfRg16 :			return EPixelFormat::RG16_UNorm;
			case glslang::TLayoutFormat::ElfRg8 :			return EPixelFormat::RG8_UNorm;
			case glslang::TLayoutFormat::ElfR16 :			return EPixelFormat::R16_UNorm;
			case glslang::TLayoutFormat::ElfR8 :			return EPixelFormat::R8_UNorm;
			case glslang::TLayoutFormat::ElfRgba16Snorm :	return EPixelFormat::RGBA16_SNorm;
			case glslang::TLayoutFormat::ElfRg16Snorm :		return EPixelFormat::RG16_SNorm;
			case glslang::TLayoutFormat::ElfRg8Snorm :		return EPixelFormat::RG8_SNorm;
			case glslang::TLayoutFormat::ElfR16Snorm :		return EPixelFormat::RG16_SNorm;
			case glslang::TLayoutFormat::ElfR8Snorm :		return EPixelFormat::R8_SNorm;
			case glslang::TLayoutFormat::ElfRgba32i :		return EPixelFormat::RGBA32I;
			case glslang::TLayoutFormat::ElfRgba16i :		return EPixelFormat::RGBA16I;
			case glslang::TLayoutFormat::ElfRgba8i :		return EPixelFormat::RGBA8I;
			case glslang::TLayoutFormat::ElfR32i :			return EPixelFormat::R32I;
			case glslang::TLayoutFormat::ElfRg32i :			return EPixelFormat::RG32I;
			case glslang::TLayoutFormat::ElfRg16i :			return EPixelFormat::RG16I;
			case glslang::TLayoutFormat::ElfRg8i :			return EPixelFormat::RG8I;
			case glslang::TLayoutFormat::ElfR16i :			return EPixelFormat::R16I;
			case glslang::TLayoutFormat::ElfR8i :			return EPixelFormat::R8I;
			case glslang::TLayoutFormat::ElfRgba32ui :		return EPixelFormat::RGBA32U;
			case glslang::TLayoutFormat::ElfRgba16ui :		return EPixelFormat::RGBA16U;
			case glslang::TLayoutFormat::ElfRgba8ui :		return EPixelFormat::RGBA8U;
			case glslang::TLayoutFormat::ElfR32ui :			return EPixelFormat::R32U;
			case glslang::TLayoutFormat::ElfRg32ui :		return EPixelFormat::RG32U;
			case glslang::TLayoutFormat::ElfRg16ui :		return EPixelFormat::RG16U;
			case glslang::TLayoutFormat::ElfRgb10a2ui :		return EPixelFormat::RGB10_A2U;
			case glslang::TLayoutFormat::ElfRg8ui :			return EPixelFormat::RG8U;
			case glslang::TLayoutFormat::ElfR16ui :			return EPixelFormat::R16U;
			case glslang::TLayoutFormat::ElfR8ui :			return EPixelFormat::R8U;
		}
		RETURN_ERR( "Unsupported image format!" );
	}

/*
=================================================
	DeserializeConstant
=================================================
*/
	struct DeserializeConstant
	{
		template <typename T>
		static bool _Run (const glslang::TConstUnionArray& src, bool single, INOUT int &index, OUT DeserializedShader::Constant::ValueArray_t &result) {
			return _Run2( src, single, INOUT index, result, T() );
		}


		template <typename T>
		static bool _Run2 (const glslang::TConstUnionArray& src, bool single, INOUT int &index, OUT DeserializedShader::Constant::ValueArray_t &result, const T&)
		{
			const int first = index;

			for (index; index < src.size(); ++index)
			{
				if ( single and first != index )
					break;

				T	value = _Get<T>( src[index] );

				result.PushBack( DeserializedShader::Constant::Value_t( value ) );
			}
			return true;
		}
		

		template <typename T, usize I, ulong U>
		static bool _Run2 (const glslang::TConstUnionArray& src, bool single, INOUT int &index, OUT DeserializedShader::Constant::ValueArray_t &result, const Vec<T,I,U>&)
		{
			const int first = index;

			for (index; index < src.size(); index += I)
			{
				if ( single and first != index )
					break;

				CHECK_ERR( src.size() - index >= int(I) );

				Vec<T,I,U>	value;

				for (usize j = 0; j < I; ++j)
				{
					value[j] = _Get<T>( src[index+j] );
				}

				result.PushBack( DeserializedShader::Constant::Value_t( value ) );
			}
			return true;
		}
		

		template <typename T, usize C, usize R, ulong U>
		static bool _Run2 (const glslang::TConstUnionArray& src, bool single, INOUT int &index, OUT DeserializedShader::Constant::ValueArray_t &result, const Matrix<T,C,R,U>&)
		{
			const int first = index;

			for (index; index < src.size(); index += C*R)
			{
				if ( single and first != index )
					break;

				CHECK_ERR( src.size() - index >= int(C*R) );

				Matrix<T,C,R,U>	value;

				for (usize c = 0; c < C; ++c)
				for (usize r = 0; r < R; ++r)
				{
					value(c,r) = _Get<T>( src[index + c*R + r] );
				}

				result.PushBack( DeserializedShader::Constant::Value_t( value ) );
			}
			return true;
		}


		template <typename T>
		static T _Get (const glslang::TConstUnion& un);


		static bool Process (EShaderVariable::type valueType, const glslang::TConstUnionArray& src,
							 OUT DeserializedShader::Constant::ValueArray_t &result)
		{
			int index;
			return Process( valueType, src, 0, false, OUT result, OUT index );
		}

		static bool Process (EShaderVariable::type valueType, const glslang::TConstUnionArray& src, int first, bool single, 
							 OUT DeserializedShader::Constant::ValueArray_t &result, OUT int &index)
		{
			index = first;

			switch ( valueType )
			{
				case EShaderVariable::Bool :		return _Run<bool>( src, single, INOUT index, OUT result );
				case EShaderVariable::Bool2 :		return _Run<bool2>( src, single, INOUT index, OUT result );
				case EShaderVariable::Bool3 :		return _Run<bool3>( src, single, INOUT index, OUT result );
				case EShaderVariable::Bool4 :		return _Run<bool4>( src, single, INOUT index, OUT result );
				case EShaderVariable::Int :			return _Run<int>( src, single, INOUT index, OUT result );
				case EShaderVariable::Int2 :		return _Run<int2>( src, single, INOUT index, OUT result );
				case EShaderVariable::Int3 :		return _Run<int3>( src, single, INOUT index, OUT result );
				case EShaderVariable::Int4 :		return _Run<int4>( src, single, INOUT index, OUT result );
				case EShaderVariable::UInt :		return _Run<uint>( src, single, INOUT index, OUT result );
				case EShaderVariable::UInt2 :		return _Run<uint2>( src, single, INOUT index, OUT result );
				case EShaderVariable::UInt3 :		return _Run<uint3>( src, single, INOUT index, OUT result );
				case EShaderVariable::UInt4 :		return _Run<uint4>( src, single, INOUT index, OUT result );
				case EShaderVariable::Long :		return _Run<ilong>( src, single, INOUT index, OUT result );
				case EShaderVariable::Long2 :		return _Run<ilong2>( src, single, INOUT index, OUT result );
				case EShaderVariable::Long3 :		return _Run<ilong3>( src, single, INOUT index, OUT result );
				case EShaderVariable::Long4 :		return _Run<ilong4>( src, single, INOUT index, OUT result );
				case EShaderVariable::ULong :		return _Run<ulong>( src, single, INOUT index, OUT result );
				case EShaderVariable::ULong2 :		return _Run<ulong2>( src, single, INOUT index, OUT result );
				case EShaderVariable::ULong3 :		return _Run<ulong3>( src, single, INOUT index, OUT result );
				case EShaderVariable::ULong4 :		return _Run<ulong4>( src, single, INOUT index, OUT result );
				case EShaderVariable::Float :		return _Run<float>( src, single, INOUT index, OUT result );
				case EShaderVariable::Float2 :		return _Run<float2>( src, single, INOUT index, OUT result );
				case EShaderVariable::Float3 :		return _Run<float3>( src, single, INOUT index, OUT result );
				case EShaderVariable::Float4 :		return _Run<float4>( src, single, INOUT index, OUT result );
				case EShaderVariable::Double :		return _Run<double>( src, single, INOUT index, OUT result );
				case EShaderVariable::Double2 :		return _Run<double2>( src, single, INOUT index, OUT result );
				case EShaderVariable::Double3 :		return _Run<double3>( src, single, INOUT index, OUT result );
				case EShaderVariable::Double4 :		return _Run<double4>( src, single, INOUT index, OUT result );
				case EShaderVariable::Float2x2 :	return _Run<float2x2>( src, single, INOUT index, OUT result );
				case EShaderVariable::Float2x3 :	return _Run<float2x3>( src, single, INOUT index, OUT result );
				case EShaderVariable::Float2x4 :	return _Run<float2x4>( src, single, INOUT index, OUT result );
				case EShaderVariable::Float3x2 :	return _Run<float3x2>( src, single, INOUT index, OUT result );
				case EShaderVariable::Float3x3 :	return _Run<float3x3>( src, single, INOUT index, OUT result );
				case EShaderVariable::Float3x4 :	return _Run<float3x4>( src, single, INOUT index, OUT result );
				case EShaderVariable::Float4x2 :	return _Run<float4x2>( src, single, INOUT index, OUT result );
				case EShaderVariable::Float4x3 :	return _Run<float4x3>( src, single, INOUT index, OUT result );
				case EShaderVariable::Float4x4 :	return _Run<float4x4>( src, single, INOUT index, OUT result );
				case EShaderVariable::Double2x2 :	return _Run<double2x2>( src, single, INOUT index, OUT result );
				case EShaderVariable::Double2x3 :	return _Run<double2x3>( src, single, INOUT index, OUT result );
				case EShaderVariable::Double2x4 :	return _Run<double2x4>( src, single, INOUT index, OUT result );
				case EShaderVariable::Double3x2 :	return _Run<double3x2>( src, single, INOUT index, OUT result );
				case EShaderVariable::Double3x3 :	return _Run<double3x3>( src, single, INOUT index, OUT result );
				case EShaderVariable::Double3x4 :	return _Run<double3x4>( src, single, INOUT index, OUT result );
				case EShaderVariable::Double4x2 :	return _Run<double4x2>( src, single, INOUT index, OUT result );
				case EShaderVariable::Double4x3 :	return _Run<double4x3>( src, single, INOUT index, OUT result );
				case EShaderVariable::Double4x4 :	return _Run<double4x4>( src, single, INOUT index, OUT result );
				default :	RETURN_ERR( "unsupported type!" );
			}
		}
	};
	
/*
=================================================
	DeserializeConstant::_Get
=================================================
*/
	template <> inline bool		DeserializeConstant::_Get (const glslang::TConstUnion& un)	{ ASSERT( un.getType() == glslang::TBasicType::EbtBool );    return un.getBConst(); }
	template <> inline int		DeserializeConstant::_Get (const glslang::TConstUnion& un)	{ ASSERT( un.getType() == glslang::TBasicType::EbtInt );     return un.getIConst(); }
	template <> inline uint		DeserializeConstant::_Get (const glslang::TConstUnion& un)	{ ASSERT( un.getType() == glslang::TBasicType::EbtUint );    return un.getUConst(); }
	template <> inline ilong	DeserializeConstant::_Get (const glslang::TConstUnion& un)	{ ASSERT( un.getType() == glslang::TBasicType::EbtInt64 );   return un.getI64Const(); }
	template <> inline ulong	DeserializeConstant::_Get (const glslang::TConstUnion& un)	{ ASSERT( un.getType() == glslang::TBasicType::EbtUint64 );  return un.getU64Const(); }
	template <> inline float	DeserializeConstant::_Get (const glslang::TConstUnion& un)	{ ASSERT( un.getType() == glslang::TBasicType::EbtDouble );  return (float)un.getDConst(); }
	template <> inline double	DeserializeConstant::_Get (const glslang::TConstUnion& un)	{ ASSERT( un.getType() == glslang::TBasicType::EbtDouble );  return un.getDConst(); }


}	// PipelineCompiler
