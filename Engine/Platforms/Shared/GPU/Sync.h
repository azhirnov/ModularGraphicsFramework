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

#include "Engine/Platforms/Shared/GPU/IDs.h"

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
	struct ClientWaitDeviceIdle
	{};


	//
	// Create / Destroy Fence
	//
	struct CreateFence
	{
		Out< Platforms::GpuFenceId >	result;
	};

	struct DestroyFence
	{
		Platforms::GpuFenceId			id	= Uninitialized;
	};


	//
	// Wait Fence
	//
	struct ClientWaitFence
	{
	// types
		using Time_t	= Platforms::TimeL;
		using Fence_t	= Platforms::GpuFenceId;
		using Fences_t	= FixedSizeArray< Fence_t, 16 >;

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
	struct CreateEvent
	{
		Out< Platforms::GpuEventId >	result;
	};

	struct DestroyEvent
	{
		Platforms::GpuEventId			id	= Uninitialized;
	};
	

	//
	// Set Event State (signaled / non-signaled)
	//
	struct SetEvent
	{
		Platforms::GpuEventId			id	= Uninitialized;
	};

	struct ResetEvent
	{
		Platforms::GpuEventId			id	= Uninitialized;
	};


	//
	// Create / Destroy Semaphore
	//
	struct CreateSemaphore
	{
		Out< Platforms::GpuSemaphoreId >	result;
	};

	struct DestroySemaphore
	{
		Platforms::GpuSemaphoreId			id	= Uninitialized;
	};


}	// GpuMsg
}	// Engine
