// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Engine.Base.h"

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
	
	void RegisterPlatforms ();
	void UnregisterPlatforms ();

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
