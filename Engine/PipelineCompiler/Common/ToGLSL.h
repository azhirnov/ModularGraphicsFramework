// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/PipelineCompiler/Common/ShaderEnums.h"
#include "Engine/Platforms/Shared/GPU/Enums.ToString.h"

namespace PipelineCompiler
{
	
/*
=================================================
	ToStringGLSL (EGpuMemoryModel)
=================================================
*/
	inline StringCRef  ToStringGLSL (EGpuMemoryModel::type value)
	{
		for (uint i = 0; i < CompileTime::SizeOf< EGpuMemoryModel::type >::bits; ++i)
		{
			const auto	t = EGpuMemoryModel::type(1 << i);

			if ( not EnumEq( value, t ) )
				continue;

			switch (t)
			{
				case EGpuMemoryModel::Coherent :	return "coherent";
				case EGpuMemoryModel::Volatile :	return "volatile";
				case EGpuMemoryModel::Restrict :	return "restrict";
				case EGpuMemoryModel::ReadOnly :	return "readonly";
				case EGpuMemoryModel::WriteOnly :	return "writeonly";
			}
			break;
		}
		RETURN_ERR( "unknown memory model type!" );
	}

/*
=================================================
	ToStringGLSL (EPrecision)
=================================================
*/
	inline StringCRef  ToStringGLSL (EPrecision::type value)
	{
		switch ( value )
		{
			case EPrecision::Default :	return "";
			case EPrecision::Low :		return "lowp";
			case EPrecision::Medium :	return "mediump";
			case EPrecision::High :		return "highp";
		}
		RETURN_ERR( "unsupported precision type!" );
	}
	
/*
=================================================
	ToStringGLSL (EVariableQualifier)
=================================================
*/
	inline String  ToStringGLSL (EVariableQualifier::bits value)
	{
		String str;

		FOR( i, value )
		{
			const auto	t = EVariableQualifier::type(i);

			if ( not value[t] )
				continue;

			switch ( t )
			{
				case EVariableQualifier::Patch :			str << "patch ";			break;
				case EVariableQualifier::Flat :				str << "flat ";				break;
				case EVariableQualifier::NoPerspective :	str << "noPerspective ";	break;
				case EVariableQualifier::Smooth :			str << "smooth ";			break;
				case EVariableQualifier::Centroid :			str << "centroid ";			break;
				case EVariableQualifier::Sample :			str << "sample ";			break;
				case EVariableQualifier::Shared :			str << "shared ";			break;
				case EVariableQualifier::Precise :			str << "precise ";			break;
				case EVariableQualifier::Invariant :		str << "invariant ";		break;
				case EVariableQualifier::In :				str << "in ";				break;
				case EVariableQualifier::Out :				str << "out ";				break;
				default :									RETURN_ERR( "unknown qualifier type!" );
			}
		}
		return str;
	}

/*
=================================================
	ToStringGLSL (EShaderVariable)
=================================================
*/
	inline StringCRef  ToStringGLSL (EShaderVariable::type value)
	{
		switch ( value )
		{
			case EShaderVariable::Void :		return "void";
			case EShaderVariable::Bool :		return "bool";
			case EShaderVariable::Bool2 :		return "bvec2";
			case EShaderVariable::Bool3 :		return "bvec3";
			case EShaderVariable::Bool4 :		return "bvec4";
			case EShaderVariable::Int :			return "int";
			case EShaderVariable::Int2 :		return "ivec2";
			case EShaderVariable::Int3 :		return "ivec3";
			case EShaderVariable::Int4 :		return "ivec4";
			case EShaderVariable::UInt :		return "uint";
			case EShaderVariable::UInt2 :		return "uvec2";
			case EShaderVariable::UInt3 :		return "uvec3";
			case EShaderVariable::UInt4 :		return "uvec4";
			case EShaderVariable::Long :		return "int64_t";
			case EShaderVariable::Long2 :		return "i64vec2";
			case EShaderVariable::Long3 :		return "i64vec3";
			case EShaderVariable::Long4 :		return "i64vec4";
			case EShaderVariable::ULong :		return "uint64_t";
			case EShaderVariable::ULong2 :		return "u64vec2";
			case EShaderVariable::ULong3 :		return "u64vec3";
			case EShaderVariable::ULong4 :		return "u64vec4";
			case EShaderVariable::Float :		return "float";
			case EShaderVariable::Float2 :		return "vec2";
			case EShaderVariable::Float3 :		return "vec3";
			case EShaderVariable::Float4 :		return "vec4";
			case EShaderVariable::Float2x2 :	return "mat2x2";
			case EShaderVariable::Float2x3 :	return "mat2x3";
			case EShaderVariable::Float2x4 :	return "mat2x4";
			case EShaderVariable::Float3x2 :	return "mat3x2";
			case EShaderVariable::Float3x3 :	return "mat3x3";
			case EShaderVariable::Float3x4 :	return "mat3x4";
			case EShaderVariable::Float4x2 :	return "mat4x2";
			case EShaderVariable::Float4x3 :	return "mat4x3";
			case EShaderVariable::Float4x4 :	return "mat4x4";
			case EShaderVariable::Double :		return "double";
			case EShaderVariable::Double2 :		return "dvec2";
			case EShaderVariable::Double3 :		return "dvec3";
			case EShaderVariable::Double4 :		return "dvec4";
			case EShaderVariable::Double2x2 :	return "dmat2x2";
			case EShaderVariable::Double2x3 :	return "dmat2x3";
			case EShaderVariable::Double2x4 :	return "dmat2x4";
			case EShaderVariable::Double3x2 :	return "dmat3x2";
			case EShaderVariable::Double3x3 :	return "dmat3x3";
			case EShaderVariable::Double3x4 :	return "dmat3x4";
			case EShaderVariable::Double4x2 :	return "dmat4x2";
			case EShaderVariable::Double4x3 :	return "dmat4x3";
			case EShaderVariable::Double4x4 :	return "dmat4x4";

			case EShaderVariable::FloatSampler1D :				return "sampler1D";
			case EShaderVariable::FloatSampler1DShadow :		return "sampler1DShadow";
			case EShaderVariable::FloatSampler1DArray :			return "sampler1DArray";
			case EShaderVariable::FloatSampler1DArrayShadow :	return "sampler1DArrayShadow";
			case EShaderVariable::FloatSampler2D :				return "sampler2D";
			case EShaderVariable::FloatSampler2DShadow :		return "sampler2DShadow";
			case EShaderVariable::FloatSampler2DMS :			return "sampler2DMS";
			case EShaderVariable::FloatSampler2DArray :			return "sampler2DArray";
			case EShaderVariable::FloatSampler2DArrayShadow :	return "sampler2DArrayShadow";
			case EShaderVariable::FloatSampler2DMSArray :		return "sampler2DMSArray";
			case EShaderVariable::FloatSamplerCube :			return "samplerCube";
			case EShaderVariable::FloatSamplerCubeShadow :		return "samplerCubeShadow";
			case EShaderVariable::FloatSamplerCubeArray :		return "samplerCubeArray";
			case EShaderVariable::FloatSampler3D :				return "sampler3D";
			case EShaderVariable::FloatSamplerBuffer :			return "samplerBuffer";
			case EShaderVariable::IntSampler1D :				return "isampler1D";
			case EShaderVariable::IntSampler1DArray :			return "isampler1DArray";
			case EShaderVariable::IntSampler2D :				return "isampler2D";
			case EShaderVariable::IntSampler2DMS :				return "isampler2DMS";
			case EShaderVariable::IntSampler2DArray :			return "isampler2DArray";
			case EShaderVariable::IntSampler2DMSArray :			return "isampler2DMSArray";
			case EShaderVariable::IntSamplerCube :				return "isamplerCube";
			case EShaderVariable::IntSamplerCubeArray :			return "isamplerCubeArray";
			case EShaderVariable::IntSampler3D :				return "isampler3D";
			case EShaderVariable::IntSamplerBuffer :			return "isamplerBuffer";
			case EShaderVariable::UIntSampler1D :				return "uisampler1D";
			case EShaderVariable::UIntSampler1DArray :			return "uisampler1DArray";
			case EShaderVariable::UIntSampler2D :				return "uisampler2D";
			case EShaderVariable::UIntSampler2DMS :				return "uisampler2DMS";
			case EShaderVariable::UIntSampler2DArray :			return "uisampler2DArray";
			case EShaderVariable::UIntSampler2DMSArray :		return "uisampler2DMSArray";
			case EShaderVariable::UIntSamplerCube :				return "uisamplerCube";
			case EShaderVariable::UIntSamplerCubeArray :		return "uisamplerCubeArray";
			case EShaderVariable::UIntSampler3D :				return "uisampler3D";
			case EShaderVariable::UIntSamplerBuffer :			return "uisamplerBuffer";

			case EShaderVariable::FloatImage1D :		return "image1D";
			case EShaderVariable::FloatImage1DArray :	return "image1DArray";
			case EShaderVariable::FloatImage2D :		return "image2D";
			case EShaderVariable::FloatImage2DMS :		return "image2DMS";
			case EShaderVariable::FloatImage2DArray :	return "image2DArray";
			case EShaderVariable::FloatImage2DMSArray :	return "image2DMSArray";
			case EShaderVariable::FloatImageCube :		return "imageCube";
			case EShaderVariable::FloatImageCubeArray :	return "imageCubeArray";
			case EShaderVariable::FloatImage3D :		return "image3D";
			case EShaderVariable::FloatImageBuffer :	return "imageBuffer";
			case EShaderVariable::IntImage1D :			return "iimage1D";
			case EShaderVariable::IntImage1DArray :		return "iimage1DArray";
			case EShaderVariable::IntImage2D :			return "iimage2D";
			case EShaderVariable::IntImage2DMS :		return "iimage2DMS";
			case EShaderVariable::IntImage2DArray :		return "iimage2DArray";
			case EShaderVariable::IntImage2DMSArray :	return "iimage2DMSArray";
			case EShaderVariable::IntImageCube :		return "iimageCube";
			case EShaderVariable::IntImageCubeArray :	return "iimageCubeArray";
			case EShaderVariable::IntImage3D :			return "iimage3D";
			case EShaderVariable::IntImageBuffer :		return "iimageBuffer";
			case EShaderVariable::UIntImage1D :			return "uimage1D";
			case EShaderVariable::UIntImage1DArray :	return "uimage1DArray";
			case EShaderVariable::UIntImage2D :			return "uimage2D";
			case EShaderVariable::UIntImage2DMS :		return "uimage2DMS";
			case EShaderVariable::UIntImage2DArray :	return "uimage2DArray";
			case EShaderVariable::UIntImage2DMSArray :	return "uimage2DMSArray";
			case EShaderVariable::UIntImageCube :		return "uimageCube";
			case EShaderVariable::UIntImageCubeArray :	return "uimageCubeArray";
			case EShaderVariable::UIntImage3D :			return "uimage3D";
			case EShaderVariable::UIntImageBuffer :		return "uimageBuffer";

			//case Struct :						return "struct";
			//case Array :						return "array";
		}

		RETURN_ERR( "invalid variable type", "unknown" );
	}
	
/*
=================================================
	ToStringGLSL (VertexAttribs)
=================================================
*/
	inline String  ToStringGLSL (const VertexAttribs &attribs)
	{
		String	str;
		FOR( i, attribs )
		{
			const auto&		attr = attribs[i];

			str << "in " << ToStringGLSL( EShaderVariable::type( attr.second.type ) ) << " " << attr.first << ";\n";
		}
		return str;
	}
	
/*
=================================================
	ToStringGLSL (FragmentOutputState)
=================================================
*/
	inline String  ToStringGLSL (const FragmentOutputState &state)
	{
		String	str;

		FOR( i, state )
		{
			const auto&		st = state[i];

			str << "layout(location=" << uint(st.second.index) << ", index=0) out "
				<< ToStringGLSL( EShaderVariable::type( st.second.type ) ) << " "
				<< st.first << ";\n";
		}
		return str;
	}
	
/*
=================================================
	ToStringGLSL (EPixelFormat)
=================================================
*/
	inline StringCRef  ToStringGLSL (const EPixelFormat::type &value)
	{
		return EPixelFormat::ToString( value );
	}
	
/*
=================================================
	ToStringGLSL (EVariablePacking)
=================================================
*/
	inline StringCRef  ToStringGLSL (const EVariablePacking::type &value)
	{
		return EVariablePacking::ToString( value );
	}
	

}	// PipelineCompiler
