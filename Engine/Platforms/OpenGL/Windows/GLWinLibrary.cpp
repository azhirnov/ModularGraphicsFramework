// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/STL/Common/Platforms.h"
#include "Core/Config/Engine.Config.h"

#if defined( GRAPHICS_API_OPENGL ) and defined( PLATFORM_WINDOWS )

#include "Engine/Platforms/OpenGL/Windows/GLWinLibrary.h"
#include "Core/STL/OS/Windows/WinHeader.h"
#include "External/opengl/wglext.h"

namespace Engine
{
namespace PlatformGL
{
	
	using wglGetProcAddress_t = decltype(&wglGetProcAddress);
	
/*
=================================================
	constructor
=================================================
*/
	GLWinLibrary::GLWinLibrary () :
		_getProc( UninitializedT<wglGetProcAddress_t>() )
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
	SharedLibFunction_t  GLWinLibrary::GetProc (StringCRef address) const
	{
		SharedLibFunction_t  res = null;

		if ( _getProc.IsNotNull< wglGetProcAddress_t >() and
			 (res = ReferenceCast< SharedLibFunction_t >(_getProc.Get< wglGetProcAddress_t >()( address.cstr() ))) != null )
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
		_getProc = ReferenceCast< wglGetProcAddress_t >( _lib.GetProc( "wglGetProcAddress" ) );
		return _getProc.IsNotNull< wglGetProcAddress_t> ();
	}


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL and PLATFORM_WINDOWS
