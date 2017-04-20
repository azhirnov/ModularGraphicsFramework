// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Posix/OSPosix.h"

#ifdef PLATFORM_ANDROID

#if defined( PLATFORM_SDL )
	// disable Android Shells
#else
	// enable Android Shells
#	define PLATFORM_ANDROID_SHELL
#endif


namespace GX_STL
{
namespace android
{
	using namespace GXTypes;
	using namespace posix;
	
#	include <jni.h>
	
//	<stdint.h>
	typedef byte	int8_t;
	typedef ubyte	uint8_t;

	typedef int	int32_t;
	typedef uint	uint32_t;

	typedef ilong	int64_t;
	typedef ulong	uint64_t;

#	include <android/api-level.h>
#	include <android/asset_manager.h>
#	include <android/asset_manager_jni.h>
#	include <android/configuration.h>
#	include <android/input.h>
#	include <android/keycodes.h>
#	include <android/log.h>
//#	include <android/looper.h>
//#	include <android/native_activity.h>
#	include <android/native_window.h>
#	include <android/native_window_jni.h>
//#	include <android/obb.h>
#	include <android/sensor.h>
//#	include <android/storage_manager.h>
#	include <android/window.h>


}	// android
}	// GX_STL

#endif	// PLATFORM_ANDROID