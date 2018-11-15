// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Vulkan style synchronization primitives.
	For more information see vulkan docs.

	Fence - used to sync device with client.

	Semaphore (mutex) - used to sync between command buffers in queue.
	https://www.khronos.org/registry/vulkan/specs/1.0/html/vkspec.html#synchronization-semaphores-waiting

	Event - used to sync between command buffer in different queues.
			Client may check and change state of event, but can't wait the event.
			Limited support in OpenGL and DirectX11.
*/

#pragma once

#include "Engine/Platforms/Public/GPU/IDs.h"

namespace Engine
{
namespace Platforms
{

	enum class GpuFenceId : ulong
	{
		Unknown = 0
	};

	enum class GpuEventId : ulong
	{
		Unknown = 0
	};

	enum class GpuSemaphoreId : ulong
	{
		Unknown = 0
	};

}	// Platforms


namespace CreateInfo
{

	//
	// Sync Manager Create Info
	//
	struct GpuSyncManager
	{
		ModulePtr	gpuThread;
	};

}	// CreateInfo


namespace GpuMsg
{

	//
	// Wait Device Idle (sync with GPU in client side)
	//
	struct ClientWaitDeviceIdle : _MsgBase_
	{};


	//
	// Create / Destroy Fence
	//
	struct CreateFence : _MsgBase_
	{
	// variables
		StaticString<64>				name;
		Out< Platforms::GpuFenceId >	result;

	// methods
		CreateFence () {}
		explicit CreateFence (StringCRef name) : name{name} {}
	};

	struct DestroyFence : _MsgBase_
	{
	// variables
		Platforms::GpuFenceId			id;
		
	// methods
		explicit DestroyFence (Platforms::GpuFenceId id) : id{id} {}
	};


	//
	// Wait Fence
	//
	struct ClientWaitFence : _MsgBase_
	{
	// types
		using Time_t	= Platforms::TimeL;
		using Fence_t	= Platforms::GpuFenceId;
		using Fences_t	= FixedSizeArray< Fence_t, GlobalConst::GAPI_MaxWaitFence >;

		static constexpr Time_t		MAX_TIME = Time_t(ulong(UMax));

	// variables
		Fences_t	fences;
		Time_t		timeout;

	// methods
		explicit ClientWaitFence (Fence_t fence, Time_t timeout = MAX_TIME) : fences{fence}, timeout{timeout} {}
		explicit ClientWaitFence (ArrayCRef<Fence_t> fences, Time_t timeout = MAX_TIME) : fences{fences}, timeout{timeout} {}
	};


	//
	// Create / Destroy Event
	//
	struct CreateEvent : _MsgBase_
	{
	// variables
		StaticString<64>				name;
		Out< Platforms::GpuEventId >	result;

	// methods
		CreateEvent () {}
		explicit CreateEvent (StringCRef name) : name{name} {}
	};


	struct DestroyEvent : _MsgBase_
	{
	// variables
		Platforms::GpuEventId			id;
		
	// methods
		explicit DestroyEvent (Platforms::GpuEventId id) : id{id} {}
	};
	

	//
	// Set Event State (signaled / non-signaled)
	//
	struct SetEvent : _MsgBase_
	{
	// variables
		Platforms::GpuEventId			id;
		
	// methods
		explicit SetEvent (Platforms::GpuEventId id) : id{id} {}
	};


	struct ResetEvent : _MsgBase_
	{
	// variables
		Platforms::GpuEventId			id;
		
	// methods
		explicit ResetEvent (Platforms::GpuEventId id) : id{id} {}
	};


	//
	// Create / Destroy Semaphore
	//
	struct CreateSemaphore : _MsgBase_
	{
	// variables
		StaticString<64>					name;
		Out< Platforms::GpuSemaphoreId >	result;

	// methods
		CreateSemaphore () {}
		explicit CreateSemaphore (StringCRef name) : name{name} {}
	};


	struct DestroySemaphore : _MsgBase_
	{
	// variables
		Platforms::GpuSemaphoreId		id;

	// methods
		explicit DestroySemaphore (Platforms::GpuSemaphoreId id) : id{id} {}
	};


}	// GpuMsg
}	// Engine
