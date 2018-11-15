// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/STL/Common/Platforms.h"
#include "Core/Config/Engine.Config.h"

#if defined( GRAPHICS_API_OPENGL ) and defined( PLATFORM_LINUX )

#include "Engine/Platforms/OpenGL/Linux/GLX11Library.h"
#include "Core/STL/OS/Posix/PosixHeader.h"
#include "External/opengl/glxext.h"

namespace Engine
{
namespace PlatformGL
{
	
	using glxGetProcAddress_t		= decltype(&glXGetProcAddress);
	using glxGetProcAddressARB_t	= decltype(&glXGetProcAddressARB);
	
/*
=================================================
	constructor
=================================================
*/
	GLX11Library::GLX11Library () :
		_getProc{ UninitializedT<glxGetProcAddress_t>() },
		_getProcARB{ UninitializedT<glxGetProcAddressARB_t>() }
	{}
	
/*
=================================================
	Load
=================================================
*/
	bool GLX11Library::Load (StringCRef name)
	{
		Unload();

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
	void GLX11Library::Unload ()
	{
		_getProc	= null;
		_getProcARB	= null;
		_lib.Unload();
	}
	
/*
=================================================
	GetProc
=================================================
*/
	SharedLibFunction_t  GLX11Library::GetProc (StringCRef address) const
	{
		SharedLibFunction_t  res = null;

		if ( _getProc.IsNotNull< glxGetProcAddress_t >() and
			 (res = ReferenceCast< SharedLibFunction_t >(_getProc.Get< glxGetProcAddress_t >()( address.cstr() ))) != null )
		{
			return res;
		}

		if ( _getProcARB.IsNotNull< glxGetProcAddressARB_t >() and
			 (res = ReferenceCast< SharedLibFunction_t >(_getProcARB.Get< glxGetProcAddressARB_t >()( address.cstr() ))) != null )
		{
			return res;
		}
			
		if ( (res = _lib.GetProc( address )) != null )
			return res;

		return null;
	}

/*
=================================================
	GetDefaultName
=================================================
*/
	StringCRef  GLX11Library::GetDefaultName ()
	{
		return "libGL.so";
	}

/*
=================================================
	_OnInit
=================================================
*/
	bool GLX11Library::_OnInit ()
	{
		_getProc	= ReferenceCast< glxGetProcAddress_t    >( _lib.GetProc( "glXGetProcAddress" ) );
		_getProcARB	= ReferenceCast< glxGetProcAddressARB_t >( _lib.GetProc( "glXGetProcAddressARB" ) );

		return	_getProc.IsNotNull< glxGetProcAddress_t >()		and
				_getProcARB.IsNotNull< glxGetProcAddressARB_t >();
	}


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL and PLATFORM_LINUX
