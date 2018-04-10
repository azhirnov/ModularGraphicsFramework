// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/STL/OS/SDL/Library.h"

#ifdef PLATFORM_SDL

namespace GX_STL
{
namespace OS
{

/*
=================================================
	constructor
=================================================
*/
	Library::Library () :
		_handle{ null },
		_freeWhenDelete{ true }
	{}

	Library::Library (StringCRef name, bool canFree) :
		_handle{ null },
		_freeWhenDelete{ true }
	{
		Load( name, canFree );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Library::~Library ()
	{
		if ( _freeWhenDelete )
			Unload();
	}
	
/*
=================================================
	FindAndLoad
=================================================
*/
	bool Library::FindAndLoad (StringCRef name, uint searchDepth, bool canFree)
	{
		String	fname;

		if ( not FileSystem::SearchFile( name, searchDepth, OUT fname ) )
			return false;

		return Load( fname, canFree );
	}

/*
=================================================
	Load
=================================================
*/
	bool Library::Load (StringCRef name, bool canFree)
	{
		Unload();

		_handle			= ::SDL_LoadObject( name.cstr() );
		_name			= name;
		_freeWhenDelete	= canFree;

		return IsValid();
	}
	
/*
=================================================
	Unload
=================================================
*/
	void Library::Unload ()
	{
		if ( _handle != null )
		{
			::SDL_UnloadObject( _handle );
			_handle = null;
		}
		_name.Clear();
	}
	
/*
=================================================
	GetProc
=================================================
*/
	Library::Func_t  Library::GetProc (StringCRef name, Func_t defProc) const
	{
		Func_t	tmp = reinterpret_cast<Func_t>(::SDL_LoadFunction( _handle, name.cstr() ));
		return tmp != null ? tmp : defProc;
	}

}	// OS
}	// GX_STL

#endif	// PLATFORM_SDL
