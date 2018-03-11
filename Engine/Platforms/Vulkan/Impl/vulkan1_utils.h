// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "vulkan1.h"
#include "Vk1Library.h"
#include "Engine/STL/ThreadSafe/Singleton.h"

namespace vk
{
	
	// get function address from driver //
#	define VK1_GET_PROC( _type_, _fp_, _def_, _instance_, _device_ ) \
		++counter; \
		if ( (_fp_ == _def_ or _fp_ == null) and \
			 (_fp_ = (_type_)Vk1_GetProcAddress( _instance_, _device_, TOSTRING( _fp_ ) )) == null ) \
		{ \
			_fp_ = _def_; \
			missing++; \
		}

	// default Functions //
#	define VULKAN1_DEFFUNC( _retType_, _funcName_, _funcParams_, _retValue_ ) \
		_retType_ VKAPI_CALL Default##_funcName_ _funcParams_ \
		{ \
			using namespace Engine; \
			LOG( "Used dummy function " TOSTRING( _funcName_ ), ELog::Debug ); \
			return _retValue_; \
		}
	
	// pointers to functions //
#	define VULKAN1_INITFUNC( _retType_, _funcName_, _funcParams_, _retValue_ ) \
		PFN_##_funcName_	_funcName_ = &Default##_funcName_;
		

	// set pointers to functions //
#	define VULKAN1_GETFUNC( _retType_, _funcName_, _funcParams_, _retValue_ ) \
		VK1_GET_PROC( PFN_##_funcName_, _funcName_, &Default##_funcName_, instance, null );


	// set pointers to default functions //
#	define VULKAN1_ZEROFUNC( _retType_, _funcName_, _funcParams_, _retValue_ ) \
		_funcName_ = &Default##_funcName_;
		

/*
=================================================
	Vk1_GetProcAddress
=================================================
*/
	inline PFN_vkVoidFunction Vk1_GetProcAddress (VkInstance instance, VkDevice device, const char *name)
	{
		using namespace Engine;

		PFN_vkVoidFunction	ptr = SingletonSingleThread::Instance< Vk1Library >()->GetProc( instance, device, name );
		
		DEBUG_ONLY(
		if ( ptr == null ) {
			LOG( ("Can't get address of "_str << name).cstr(), ELog::Debug );
		})
		return ptr;
	}
	
/*
=================================================
	Vk1_LoadLibrary
=================================================
*/
	inline bool Vk1_LoadLibrary ()
	{
		return Engine::SingletonSingleThread::Instance< Vk1Library >()->Load();
	}

/*
=================================================
	Vk1_UnloadLibrary
=================================================
*/
	inline void Vk1_UnloadLibrary ()
	{
		Engine::SingletonSingleThread::Instance< Vk1Library >()->Unload();
	}

}	// vk

#endif	// GRAPHICS_API_VULKAN
