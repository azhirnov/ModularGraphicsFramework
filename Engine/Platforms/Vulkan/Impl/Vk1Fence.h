// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1Device.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1BaseObject.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{

	//
	// Vulan Fence
	//
/*
	class Vk1Fence final : public Vk1BaseObject
	{
	// variables
	private:
		vk::VkFence		_fenceId;

	// emthods
	public:
		explicit Vk1Fence (Ptr<Vk1Device> dev) :
			Vk1BaseObject( dev ), _fenceId( VK_NULL_HANDLE )
		{}

		~Vk1Fence ()
		{
			Destroy();
		}


		bool Create (bool signaled = false)
		{
			using namespace vk;
			Destroy();

			VkFenceCreateInfo	fence_info	= {};
			fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fence_info.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

			VK_CHECK( vkCreateFence( GetVkDevice(), &fence_info, null, OUT &_fenceId ) );
			return true;
		}


		void Destroy ()
		{
			if ( IsCreated() )
			{
				using namespace vk;
				vkDestroyFence( GetVkDevice(), _fenceId, null );
				_fenceId = VK_NULL_HANDLE;
			}
		}


		bool IsCreated () const
		{
			return _fenceId != VK_NULL_HANDLE;
		}


		vk::VkFence GetFenceID () const
		{
			return _fenceId;
		}


		bool Wait (TimeL timeout = 100000000000_nanoSec)
		{
			using namespace vk;
			CHECK_ERR( IsCreated() );
			VK_CHECK( vkWaitForFences( GetVkDevice(), 1, &_fenceId, VK_TRUE, timeout.NanoSeconds() ) );
			return true;
		}


		bool Reset ()
		{
			using namespace vk;
			CHECK_ERR( IsCreated() );
			VK_CHECK( vkResetFences( GetVkDevice(), 1, &_fenceId ) );
			return true;
		}


		bool IsSignaled ()
		{
			using namespace vk;
			CHECK_ERR( IsCreated() );

			VkResult res = vkGetFenceStatus( GetVkDevice(), _fenceId );

			switch ( res ) {
				case VK_SUCCESS :		return true;	// signaled
				case VK_NOT_READY :		return false;	// unsignaled
				//case VK_DEVICE_LOST :	WARNING("device lost");		return false;
				default :				WARNING("unknown error");	return false;
			}
		}
	};

	SHARED_POINTER( Vk1Fence );
	*/

}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
