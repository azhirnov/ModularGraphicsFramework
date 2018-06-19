// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_OPENGL

#include "Engine/Platforms/OpenGL/450/GL4Messages.h"
#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/Public/GPU/Framebuffer.h"
#include "Engine/Base/Modules/MessageCache.h"
#include "Core/STL/CompileTime/CompileTimeDebug.h"

namespace Engine
{
namespace PlatformGL
{

	//
	// Resource Cache
	//

	class GL4ResourceCache final
	{
	// types
	private:
		using ImageCache_t			= MessageCache< GpuMsg::GetGLImageID, GpuMsg::GetImageDescription >;
		using BufferCache_t			= MessageCache< GpuMsg::GetGLBufferID, GpuMsg::GetBufferDescription >;
		using FramebufferCache_t	= MessageCache< GpuMsg::GetGLFramebufferID, GpuMsg::GetFramebufferDescription >;
		using GPipelineCache_t		= MessageCache< GpuMsg::GetGLGraphicsPipelineID, GpuMsg::GetGraphicsPipelineDescription >;
		using CPipelineCache_t		= MessageCache< GpuMsg::GetGLComputePipelineID, GpuMsg::GetComputePipelineDescription >;


	// variables
	private:
		ImageCache_t		_imageCache;
		BufferCache_t		_bufferCache;
		FramebufferCache_t	_framebufferCache;
		GPipelineCache_t	_gpipelineCache;
		CPipelineCache_t	_cpipelineCache;


	// methods
	public:
		GL4ResourceCache () {}

		ND_ ImageCache_t::CacheData_t const&  GetImageID (const ModulePtr &mod) const
		{
			return _imageCache.Get( mod );
		}
		
		ND_ BufferCache_t::CacheData_t const&  GetBufferID (const ModulePtr &mod) const
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

		void Erase (const ModulePtr &mod)
		{
			_imageCache.Erase( mod );
			_bufferCache.Erase( mod );
			_framebufferCache.Erase( mod );
			_gpipelineCache.Erase( mod );
			_cpipelineCache.Erase( mod );
		}
	};


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
