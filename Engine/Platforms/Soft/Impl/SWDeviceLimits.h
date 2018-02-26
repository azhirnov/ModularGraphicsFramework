// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Soft/Impl/SWEnums.h"

#ifdef GRAPHICS_API_SOFT

namespace Engine
{
namespace PlatformSW
{

	//
	// Device Limits
	//
	struct SWDeviceLimits final : Noninstancable
	{
		static constexpr uint	maxPushConstantsSize	= 256u;			// 256 bytes
		static constexpr uint	maxUpdateBufferSize		= 65536u;		// bytes
		static constexpr uint	maxUniformBufferRange	= 64u << 10;	// 64Kb
		static constexpr uint	maxStorageBufferRange	= 2u << 30;		// 2 Gb

		static constexpr uint	maxComputeSharedMemorySize		= 48 << 10;	// 48 Kb
		static constexpr uint	maxComputeWorkGroupInvocations	= 1536;
		static constexpr uint3	maxComputeWorkGroupSize			= uint3( 1536, 1024, 64 );		// group local size
		static constexpr uint3	maxComputeWorkGroupCount		= uint3( 2147483647, 65535, 65535 );
	};


}	// PlatformSW
}	// Engine

#endif	// GRAPHICS_API_SOFT
