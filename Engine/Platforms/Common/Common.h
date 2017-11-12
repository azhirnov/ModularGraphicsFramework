// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Engine.Base.h"

#ifdef __GX_BUILD_LIB__
#	define _ENGINE_PLATFORMS_EXPORT_
#elif defined( __GX_ENGINE_PLATFORMS_BUILD__ )
#	define _ENGINE_PLATFORMS_EXPORT_		GX_DLL_EXPORT
#else
#	define _ENGINE_PLATFORMS_EXPORT_		GX_DLL_IMPORT
#endif

namespace Engine
{
namespace Platforms
{
	using namespace Engine::Base;
	using namespace GX_STL::GXMath;
	
	struct FramebufferDescriptor;
	struct GpuMemoryDescriptor;
	struct GraphicsPipelineDescriptor;
	struct RenderPassDescriptor;
	struct RenderState;
	struct SamplerDescriptor;
	struct ImageDescriptor;
	
	_ENGINE_PLATFORMS_EXPORT_ void RegisterPlatforms ();
	_ENGINE_PLATFORMS_EXPORT_ void UnregisterPlatforms ();

}	// Platforms

namespace GpuMsg {
	using namespace ModuleMsg;
}

namespace OSMsg {
	using namespace ModuleMsg;
}

namespace CreateInfo
{
	struct InputManager;
	struct InputThread;
	struct RawInputHandler;
	
	struct Platform;
	struct Window;

	struct GpuBuffer;
	struct GpuCommandBuffer;
	struct GpuCommandBuilder;
	struct GpuContext;
	struct GpuFramebuffer;
	struct GpuMemory;
	struct GraphicsPipeline;
	struct ComputePipeline;
	struct GpuShaderModulesFromFile;
	struct GpuShaderModulesFromBuffer;
	struct GpuRenderPass;
	struct GpuSampler;
	struct GpuImage;
	struct GpuThread;

}	// CreateInfo
}	// Engine
