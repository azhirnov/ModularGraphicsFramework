// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/OpenGL/Windows/GLWinLibrary.h"

#if defined( GRAPHICS_API_OPENGL ) and defined( PLATFORM_WINDOWS )

#include "Engine/STL/OS/Windows/WinHeader.h"
#include "External/opengl/wglext.h"

namespace Engine
{
namespace PlatformGL
{
	
	using wglGetProcAddressProc_t = decltype(&wglGetProcAddress);
	
/*
=================================================
	constructor
=================================================
*/
	GLWinLibrary::GLWinLibrary () :
		_getProc( UninitializedT<wglGetProcAddressProc_t>() )
	{}
	
/*
=================================================
	Load
=================================================
*/
	bool GLWinLibrary::Load (StringCRef name)
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
	void GLWinLibrary::Unload ()
	{
		_getProc = null;
		_lib.Unload();
	}
	
/*
=================================================
	GetProc
=================================================
*/
	void * GLWinLibrary::GetProc (StringCRef address) const
	{
		void * res = null;

		if ( _getProc.IsNotNull<wglGetProcAddressProc_t>() and
			 (res = _getProc.Get<wglGetProcAddressProc_t>()( address.cstr() )) != null )
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
	StringCRef  GLWinLibrary::GetDefaultName ()
	{
		return "opengl32.dll";
	}

/*
=================================================
	_OnInit
=================================================
*/
	bool GLWinLibrary::_OnInit ()
	{
		_getProc = (wglGetProcAddressProc_t) _lib.GetProc( "wglGetProcAddress" );
		return _getProc.IsNotNull<wglGetProcAddressProc_t>();
	}


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL and PLATFORM_WINDOWS
