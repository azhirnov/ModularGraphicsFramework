// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/ShaderEnums.h"
#include "Engine/Platforms/Shared/GPU/IDs.h"

namespace Engine
{
namespace Platforms
{

	//
	// Uniforms Descriptor
	//

	struct UniformsDescriptor : CompileTime::PODStruct
	{
	// types
		using Name_t		= StaticString<32>;
		using Value_t		= Union< int, int2, int3, int4,
									 uint, uint2, uint3, uint4,
									 ilong, ilong2, ilong3, ilong4,
									 ulong, ulong2, ulong3, ulong4,
									 float, float2, float3, float4,
									 float2x2, float3x3, float4x4,
									 double, double2, double3, double4,
									 double2x2, double3x3, double4x4 >;
		using ValueMap_t	= HashMap< Name_t, Value_t >;

	// variables
		ValueMap_t		values;
		EShader::bits	stageFlags;

	// methods
		UniformsDescriptor (GX_DEFCTOR) {}
	};

}	// Platforms


namespace CreateInfo
{
	//
	// GPU Uniforms Create Info
	//

	struct GpuUniforms
	{
		ModulePtr						gpuThread;			// can be null
		Platforms::UniformsDescriptor	descr;
	};

}	// CreateInfo


namespace GpuMsg
{

	//
	// Get Uniforms Descriptor
	//
	struct GetUniformsDescriptor
	{
		Out< Platforms::UniformsDescriptor >	result;
	};


}	// GpuMsg
}	// Engine
