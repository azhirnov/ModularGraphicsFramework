// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Vulkan/110/Vk1Messages.h"
#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/Public/GPU/Framebuffer.h"
#include "Engine/Platforms/Public/GPU/RenderPass.h"
#include "Engine/Base/Modules/MessageCache.h"
#include "Engine/STL/CompileTime/CompileTimeDebug.h"

namespace Engine
{
namespace PlatformVK
{

	//
	// Resource Cache
	//

	class Vk1ResourceCache final
	{
	// types
	private:
		using ImageIDCache_t		= MessageCache< GpuMsg::GetVkImageID, GpuMsg::GetImageDescriptor >;
		using BufferIDCache_t		= MessageCache< GpuMsg::GetVkBufferID, GpuMsg::GetBufferDescriptor >;
		using FramebufferCache_t	= MessageCache< GpuMsg::GetVkFramebufferID, GpuMsg::GetFramebufferDescriptor >;
		using GPipelineCache_t		= MessageCache< GpuMsg::GetVkGraphicsPipelineID, GpuMsg::GetGraphicsPipelineDescriptor >;
		using CPipelineCache_t		= MessageCache< GpuMsg::GetVkComputePipelineID, GpuMsg::GetComputePipelineDescriptor >;
		using CmdBufferCache_t		= MessageCache< GpuMsg::GetVkCommandBufferID >;
		using ResTableCache_t		= MessageCache< GpuMsg::GetVkPipelineLayoutID, GpuMsg::GetVkPipelineResourceTableID >;
		using RenderPassCache_t		= MessageCache< GpuMsg::GetVkRenderPassID, GpuMsg::GetRenderPassDescriptor >;


	// variables
	private:
		ImageIDCache_t		_imageCache;
		BufferIDCache_t		_bufferCache;
		FramebufferCache_t	_framebufferCache;
		RenderPassCache_t	_renderPassCache;
		GPipelineCache_t	_gpipelineCache;
		CPipelineCache_t	_cpipelineCache;
		CmdBufferCache_t	_cmdBufferCache;
		ResTableCache_t		_resTableCache;


	// methods
	public:
		Vk1ResourceCache () {}

		ND_ ImageIDCache_t::CacheData_t const&  GetImageID (const ModulePtr &mod) const
		{
			return _imageCache.Get( mod );
		}
		
		ND_ BufferIDCache_t::CacheData_t const&  GetBufferID (const ModulePtr &mod) const
		{
			return _bufferCache.Get( mod );
		}
		
		ND_ FramebufferCache_t::CacheData_t const&  GetFramebufferID (const ModulePtr &mod) const
		{
			return _framebufferCache.Get( mod );
		}
		
		ND_ GPipelineCache_t::CacheData_t const&  GetGraphicsPipelineID (const ModulePtr &mod) const
		{
			return _gpipelineCache.Get( mod );
		}
		
		ND_ CPipelineCache_t::CacheData_t const&  GetComputePipelineID (const ModulePtr &mod) const
		{
			return _cpipelineCache.Get( mod );
		}
		
		ND_ CmdBufferCache_t::CacheData_t const&  GetCommandBufferID (const ModulePtr &mod) const
		{
			return _cmdBufferCache.Get( mod );
		}

		ND_ ResTableCache_t::CacheData_t const&  GetResourceTableID (const ModulePtr &mod) const
		{
			return _resTableCache.Get( mod );
		}

		ND_ RenderPassCache_t::CacheData_t const&  GetRenderPassID (const ModulePtr &mod) const
		{
			return _renderPassCache.Get( mod );
		}

		void Erase (const ModulePtr &mod)
		{
			_imageCache.Erase( mod );
			_bufferCache.Erase( mod );
			_framebufferCache.Erase( mod );
			_renderPassCache.Erase( mod );
			_gpipelineCache.Erase( mod );
			_cpipelineCache.Erase( mod );
			_cmdBufferCache.Erase( mod );
			_resTableCache.Erase( mod );
		}
	};


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
