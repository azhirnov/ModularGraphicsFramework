// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "vulkan1.h"

#if defined( GRAPHICS_API_VULKAN )

namespace vk
{


	//
	// Vulkan Library
	//

	struct Vk1Library
	{
	// types
	private:
		using FuncPtr_t		= PFN_vkVoidFunction;
		using StringCRef	= GX_STL::GXTypes::StringCRef;
		using Library		= GX_STL::OS::Library;
		using VoidFunc_t	= void (CALLBACK *) ();


	// variables
	private:
		Library						_lib;
		PFN_vkGetDeviceProcAddr		_getDeviceProc;
		PFN_vkGetInstanceProcAddr	_getInstanceProc;


	// methods
	public:
		Vk1Library ();

		bool Load (StringCRef name = StringCRef());

		void Unload ();

		VoidFunc_t GetProc (vk::VkInstance instance, vk::VkDevice device, StringCRef address) const;

		static StringCRef  GetDefaultName ();

	private:
		bool _OnInit ();
	};


	
/*
=================================================
	constructor
=================================================
*/
	inline Vk1Library::Vk1Library () :
		_getDeviceProc( null ),	_getInstanceProc( null )
	{
	}
	
/*
=================================================
	Load
=================================================
*/
	inline bool Vk1Library::Load (StringCRef name)
	{
		if ( _lib.IsValid() )
			return true;

		//Unload();

		if ( not name.Empty() )
		{
			if ( _lib.Load( name, true ) )
				return _OnInit();
		}

		if ( _lib.Load( GetDefaultName(), true ) )
			return _OnInit();

		return false;
	}
	
/*
=================================================
	Unload
=================================================
*/
	inline void Vk1Library::Unload ()
	{
		_getDeviceProc		= null;
		_getInstanceProc	= null;

		_lib.Unload();
	}
	
/*
=================================================
	GetProc
=================================================
*/
	inline Vk1Library::VoidFunc_t  Vk1Library::GetProc (vk::VkInstance instance, vk::VkDevice device, StringCRef address) const
	{
		using namespace GX_STL::GXTypes;

		vk::PFN_vkVoidFunction	res = null;

		if ( device != VK_NULL_HANDLE		and
			 _getDeviceProc != null			and
			 (res = _getDeviceProc( device, address.cstr() )) != null )
		{
			return res;
		}
		
		if ( instance != VK_NULL_HANDLE		and 
			 _getInstanceProc != null		and
			 (res = _getInstanceProc( instance, address.cstr() )) != null )
		{
			return res;
		}
		
		if ( (res = ReferenceCast<VoidFunc_t>(_lib.GetProc( address ))) != null )
			return res;

		return null;
	}
	
/*
=================================================
	GetDefaultName
=================================================
*/
	inline GX_STL::GXTypes::StringCRef  Vk1Library::GetDefaultName ()
	{
		return "vulkan-1.dll";
	}
	
/*
=================================================
	_OnInit
=================================================
*/
	inline bool Vk1Library::_OnInit ()
	{
		using namespace GX_STL::GXTypes;

		_getDeviceProc		= ReferenceCast<PFN_vkGetDeviceProcAddr>(   _lib.GetProc( "vkGetDeviceProcAddr" ) );
		_getInstanceProc	= ReferenceCast<PFN_vkGetInstanceProcAddr>( _lib.GetProc( "vkGetInstanceProcAddr" ) );

		return	_getDeviceProc   != null and
				_getInstanceProc != null;
	}

}	// vk

#endif	// GRAPHICS_API_VULKAN