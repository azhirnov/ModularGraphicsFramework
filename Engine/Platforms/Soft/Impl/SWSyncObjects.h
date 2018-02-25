// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/Sync.h"
#include "Engine/Platforms/Soft/Impl/SWBaseModule.h"

#ifdef GRAPHICS_API_SOFT

namespace Engine
{
namespace PlatformSW
{


	//
	// Fence
	//
	class SWFence final : public RefCountedObject<>
	{
	// variables
	private:
		bool		_signaled;

	// methods
	public:
		SWFence () : _signaled{false}
		{}

		void Signal ()
		{
			ASSERT( not _signaled );
			_signaled = true;
		}

		bool Wait () const
		{
			return _signaled;
		}
	};
//-----------------------------------------------------------------------------



	//
	// Event
	//
	class SWEvent final : public RefCountedObject<>
	{
		// TODO
	};
//-----------------------------------------------------------------------------



	//
	// Semaphore
	//
	class SWSemaphore final : public RefCountedObject<>
	{
	// variables
	private:
		bool		_locked;	// in vulkan this is a mutex

	// methods
	public:
		SWSemaphore () : _locked{false}
		{}
		
		bool TryLock (EPipelineStage::bits stage)	// TODO: use 'stage'
		{
			if ( not _locked )
			{
				_locked = true;
				return true;
			}
			return false;
		}

		void Unlock ()
		{
			ASSERT( _locked );
			_locked = false;
		}
	};
//-----------------------------------------------------------------------------

	

}	// PlatformSW
}	// Engine

#endif	// GRAPHICS_API_SOFT
