// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"

namespace Engine
{
namespace Platforms
{

	// Shared
	static constexpr OModID::type  PipelineTemplateModuleID			= "pp-templ"_OModID;

	// Vulkan
	static constexpr GModID::type  VkContextModuleID				= "vk.ctx"_GModID;
	static constexpr TModID::type  VkThreadModuleID					= "vk.thread"_TModID;
	static constexpr OModID::type  VkBufferModuleID					= "vk.buffer"_OModID;
	static constexpr OModID::type  VkCommandBufferModuleID			= "vk.cmdbuf"_OModID;
	static constexpr OModID::type  VkCommandBuilderModuleID			= "vk.cmdbuild"_OModID;
	static constexpr OModID::type  VkFramebufferModuleID			= "vk.fbuffer"_OModID;
	static constexpr OModID::type  VkSystemFramebufferModuleID		= "vk.sys-fb"_OModID;
	static constexpr OModID::type  VkImageModuleID					= "vk.image"_OModID;
	static constexpr OModID::type  VkMemoryModuleID					= "vk.memory"_OModID;
	static constexpr OModID::type  VkGraphicsPipelineModuleID		= "vk.g-ppln"_OModID;
	static constexpr OModID::type  VkComputePipelineModuleID		= "vk.c-ppln"_OModID;
	static constexpr OModID::type  VkPipelineResourceTableModuleID	= "vk.restabl"_OModID;
	static constexpr OModID::type  VkRenderPassModuleID				= "vk.renpass"_OModID;
	static constexpr OModID::type  VkSamplerModuleID				= "vk.sampler"_OModID;
	
	// OpenGL
	static constexpr GModID::type  GLContextModuleID				= "gl.ctx"_GModID;
	static constexpr TModID::type  GLThreadModuleID					= "gl.thread"_TModID;
	static constexpr OModID::type  GLBufferModuleID					= "gl.buffer"_OModID;
	static constexpr OModID::type  GLCommandBufferModuleID			= "gl.cmdbuf"_OModID;
	static constexpr OModID::type  GLCommandBuilderModuleID			= "gl.cmdbuild"_OModID;
	static constexpr OModID::type  GLFramebufferModuleID			= "gl.fbuffer"_OModID;
	static constexpr OModID::type  GLSystemFramebufferModuleID		= "gl.sys-fb"_OModID;
	static constexpr OModID::type  GLImageModuleID					= "gl.image"_OModID;
	static constexpr OModID::type  GLMemoryModuleID					= "gl.memory"_OModID;
	static constexpr OModID::type  GLGraphicsPipelineModuleID		= "gl.g-ppln"_OModID;
	static constexpr OModID::type  GLComputePipelineModuleID		= "gl.c-ppln"_OModID;
	static constexpr OModID::type  GLPipelineResourceTableModuleID	= "gl.restabl"_OModID;
	static constexpr OModID::type  GLRenderPassModuleID				= "gl.renpass"_OModID;
	static constexpr OModID::type  GLSamplerModuleID				= "gl.sampler"_OModID;

}	// Platforms
}	// Engine
