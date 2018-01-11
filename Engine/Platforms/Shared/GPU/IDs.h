// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"

namespace Engine
{
namespace Platforms
{

	// Shared
	static constexpr OModID::type  PipelineTemplateModuleID			= "pp-templ"_OModID;

	// TODO: remove platform-dependent ids

	// Vulkan
	static constexpr GModID::type  VkContextModuleID				= "vk.ctx"_GModID;
	static constexpr TModID::type  VkThreadModuleID					= "vk.thread"_TModID;
	static constexpr OModID::type  VkBufferModuleID					= "vk.buffer"_OModID;
	static constexpr OModID::type  VkCommandBufferModuleID			= "vk.cmdbuf"_OModID;
	static constexpr OModID::type  VkCommandBuilderModuleID			= "vk.cmdbuild"_OModID;
	static constexpr OModID::type  VkFramebufferModuleID			= "vk.fbuffer"_OModID;
	static constexpr OModID::type  VkImageModuleID					= "vk.image"_OModID;
	//static constexpr OModID::type  VkMemoryModuleID					= "vk.memory"_OModID;
	static constexpr OModID::type  VkManagedMemoryModuleID			= "vk.mngd-mem"_OModID;	// part of memory manager
	static constexpr OModID::type  VkMemoryManagerModuleID			= "vk.mem-mngr"_OModID;
	static constexpr OModID::type  VkGraphicsPipelineModuleID		= "vk.g-ppln"_OModID;
	static constexpr OModID::type  VkComputePipelineModuleID		= "vk.c-ppln"_OModID;
	static constexpr OModID::type  VkPipelineResourceTableModuleID	= "vk.restable"_OModID;
	static constexpr OModID::type  VkRenderPassModuleID				= "vk.renpass"_OModID;
	static constexpr OModID::type  VkSamplerModuleID				= "vk.sampler"_OModID;
	static constexpr OModID::type  VkUniformsModuleID				= "vk.uniforms"_OModID;	// push constants
	static constexpr OModID::type  VkSyncManagerModuleID			= "vk.sync"_OModID;
	
	// OpenGL
	static constexpr GModID::type  GLContextModuleID				= "gl.ctx"_GModID;
	static constexpr TModID::type  GLThreadModuleID					= "gl.thread"_TModID;
	static constexpr OModID::type  GLBufferModuleID					= "gl.buffer"_OModID;
	static constexpr OModID::type  GLCommandBufferModuleID			= "gl.cmdbuf"_OModID;
	static constexpr OModID::type  GLCommandBuilderModuleID			= "gl.cmdbuild"_OModID;
	static constexpr OModID::type  GLFramebufferModuleID			= "gl.fbuffer"_OModID;
	static constexpr OModID::type  GLImageModuleID					= "gl.image"_OModID;
	static constexpr OModID::type  GLMemoryModuleID					= "gl.memory"_OModID;
	static constexpr OModID::type  GLGraphicsPipelineModuleID		= "gl.g-ppln"_OModID;
	static constexpr OModID::type  GLComputePipelineModuleID		= "gl.c-ppln"_OModID;
	static constexpr OModID::type  GLPipelineResourceTableModuleID	= "gl.restable"_OModID;
	static constexpr OModID::type  GLRenderPassModuleID				= "gl.renpass"_OModID;
	static constexpr OModID::type  GLSamplerModuleID				= "gl.sampler"_OModID;
	static constexpr OModID::type  GLUniformsModuleID				= "gl.uniforms"_OModID;
	static constexpr OModID::type  GLSyncManagerModuleID			= "gl.sync"_OModID;
	
	// DirectX
	static constexpr GModID::type  DXContextModuleID				= "dx.ctx"_GModID;
	static constexpr TModID::type  DXThreadModuleID					= "dx.thread"_TModID;
	static constexpr OModID::type  DXBufferModuleID					= "dx.buffer"_OModID;
	static constexpr OModID::type  DXCommandBufferModuleID			= "dx.cmdbuf"_OModID;
	static constexpr OModID::type  DXCommandBuilderModuleID			= "dx.cmdbuild"_OModID;
	static constexpr OModID::type  DXFramebufferModuleID			= "dx.fbuffer"_OModID;
	static constexpr OModID::type  DXImageModuleID					= "dx.image"_OModID;
	static constexpr OModID::type  DXMemoryModuleID					= "dx.memory"_OModID;
	static constexpr OModID::type  DXGraphicsPipelineModuleID		= "dx.g-ppln"_OModID;
	static constexpr OModID::type  DXComputePipelineModuleID		= "dx.c-ppln"_OModID;
	static constexpr OModID::type  DXPipelineResourceTableModuleID	= "dx.restable"_OModID;
	static constexpr OModID::type  DXRenderPassModuleID				= "dx.renpass"_OModID;
	static constexpr OModID::type  DXSamplerModuleID				= "dx.sampler"_OModID;
	static constexpr OModID::type  DXUniformsModuleID				= "dx.uniforms"_OModID;
	static constexpr OModID::type  DXSyncManagerModuleID			= "dx.sync"_OModID;

	// OpenCL
	static constexpr GModID::type  CLContextModuleID				= "cl.ctx"_GModID;
	static constexpr TModID::type  CLThreadModuleID					= "cl.thread"_TModID;
	static constexpr OModID::type  CLBufferModuleID					= "cl.buffer"_OModID;
	static constexpr OModID::type  CLCommandBufferModuleID			= "cl.cmdbuf"_OModID;
	static constexpr OModID::type  CLCommandBuilderModuleID			= "cl.cmdbuild"_OModID;
	static constexpr OModID::type  CLImageModuleID					= "cl.image"_OModID;
	static constexpr OModID::type  CLMemoryModuleID					= "cl.memory"_OModID;
	static constexpr OModID::type  CLComputePipelineModuleID		= "cl.c-ppln"_OModID;
	static constexpr OModID::type  CLPipelineResourceTableModuleID	= "cl.restable"_OModID;
	static constexpr OModID::type  CLSamplerModuleID				= "cl.sampler"_OModID;
	static constexpr OModID::type  CLUniformsModuleID				= "cl.uniforms"_OModID;
	static constexpr OModID::type  CLSyncManagerModuleID			= "cl.sync"_OModID;

	// Software
	static constexpr GModID::type  SWContextModuleID				= "sw.ctx"_GModID;
	static constexpr TModID::type  SWThreadModuleID					= "sw.thread"_TModID;
	static constexpr OModID::type  SWBufferModuleID					= "sw.buffer"_OModID;
	static constexpr OModID::type  SWCommandBufferModuleID			= "sw.cmdbuf"_OModID;
	static constexpr OModID::type  SWCommandBuilderModuleID			= "sw.cmdbuild"_OModID;
	static constexpr OModID::type  SWImageModuleID					= "sw.image"_OModID;
	static constexpr OModID::type  SWMemoryModuleID					= "sw.memory"_OModID;
	static constexpr OModID::type  SWComputePipelineModuleID		= "sw.c-ppln"_OModID;
	static constexpr OModID::type  SWPipelineResourceTableModuleID	= "sw.restable"_OModID;
	static constexpr OModID::type  SwSyncManagerModuleID			= "sw.sync"_OModID;



	struct ComputeModuleIDs
	{
		GModID::type	context			= GModID::type(0);
		TModID::type	thread			= TModID::type(0);
		OModID::type	buffer			= OModID::type(0);
		OModID::type	image			= OModID::type(0);
		OModID::type	memory			= OModID::type(0);
		OModID::type	memoryManager	= OModID::type(0);
		OModID::type	pipeline		= OModID::type(0);
		OModID::type	resourceTable	= OModID::type(0);
		OModID::type	uniforms		= OModID::type(0);
		OModID::type	commandBuffer	= OModID::type(0);
		OModID::type	commandBuilder	= OModID::type(0);
	};

	struct GraphicsModuleIDs
	{
		GModID::type	context			= GModID::type(0);
		TModID::type	thread			= TModID::type(0);
		OModID::type	buffer			= OModID::type(0);
		OModID::type	image			= OModID::type(0);
		OModID::type	sampler			= OModID::type(0);
		OModID::type	memory			= OModID::type(0);
		OModID::type	memoryManager	= OModID::type(0);
		OModID::type	pipeline		= OModID::type(0);
		OModID::type	resourceTable	= OModID::type(0);
		OModID::type	uniforms		= OModID::type(0);
		OModID::type	commandBuffer	= OModID::type(0);
		OModID::type	commandBuilder	= OModID::type(0);
		OModID::type	framebuffer		= OModID::type(0);
		OModID::type	renderPass		= OModID::type(0);
	};


}	// Platforms
}	// Engine
