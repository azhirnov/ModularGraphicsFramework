// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "Engine/Platforms/OpenCL/120/CL1Messages.h"
#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Base/Modules/MessageCache.h"
#include "Core/STL/CompileTime/CompileTimeDebug.h"

namespace Engine
{
namespace PlatformCL
{

	//
	// Resource Cache
	//

	class CL1ResourceCache final
	{
	// types
	private:
		using ImageCache_t		= MessageCache< GpuMsg::GetCLImageID, GpuMsg::GetImageDescription >;
		using BufferCache_t		= MessageCache< GpuMsg::GetCLBufferID, GpuMsg::GetBufferDescription >;
		using PipelineCache_t	= MessageCache< GpuMsg::GetCLComputePipelineID, GpuMsg::GetComputePipelineDescription >;


	// variables
	private:
		ImageCache_t		_imageCache;
		BufferCache_t		_bufferCache;
		PipelineCache_t		_pipelineCache;


	// methods
	public:
		CL1ResourceCache () {}

		ND_ ImageCache_t::CacheData_t const&  GetImageID (const ModulePtr &mod) const
		{
			return _imageCache.Get( mod );
		}
		
		ND_ BufferCache_t::CacheData_t const&  GetBufferID (const ModulePtr &mod) const
		{
			return _bufferCache.Get( mod );
		}
		
		ND_ PipelineCache_t::CacheData_t const&  GetPipelineID (const ModulePtr &mod) const
		{
			return _pipelineCache.Get( mod );
		}

		void Erase (const ModulePtr &mod)
		{
			_imageCache.Erase( mod );
			_bufferCache.Erase( mod );
			_pipelineCache.Erase( mod );
		}
	};


}	// PlatformCL
}	// Engine

#endif	// COMPUTE_API_OPENCL
