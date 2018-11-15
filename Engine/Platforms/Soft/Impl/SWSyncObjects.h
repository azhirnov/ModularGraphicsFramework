// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_SOFT

#include "Engine/Platforms/Public/GPU/Sync.h"
#include "Engine/Platforms/Soft/Impl/SWBaseModule.h"

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
		StaticString<64>	_name;
		bool				_signaled;
		bool				_enqueued;

	// methods
	public:
		explicit SWFence (StringCRef name) :
			_name{name}, _signaled{false}, _enqueued{false}
		{}

		void Enqueue ()
		{
			_enqueued = true;
			_signaled = false;
		}

		bool IsEnqueued () const
		{
			return _enqueued;
		}

		void Signal ()
		{
			ASSERT( not _signaled );
			_signaled = true;
		}

		bool Wait () const
		{
			return _enqueued and _signaled;
		}
	};
//-----------------------------------------------------------------------------



	//
	// Event
	//
	class SWEvent final : public RefCountedObject<>
	{
	// variables
	private:
		StaticString<64>	_name;
		// TODO

	// methods
	public:
		explicit SWEvent (StringCRef name) : _name{name}
		{}
	};
//-----------------------------------------------------------------------------



	//
	// Semaphore
	//
	class SWSemaphore final : public RefCountedObject<>
	{
	// variables
	private:
		StaticString<64>	_name;
		bool				_locked;	// in vulkan this is a mutex
		//bool				_enqueued;

	// methods
	public:
		explicit SWSemaphore (StringCRef name) :
			_name{name}, _locked{false} //, _enqueued{false}
		{}
		
		bool TryLock (EPipelineStage::bits)	// TODO: use 'stage'
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
