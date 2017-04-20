// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "OS_SDL.h"
#include "FileSystem.h"

#ifdef PLATFORM_SDL

namespace GX_STL
{
namespace OS
{

	//
	// Library
	//

	struct Library
	{
	// variables
	private:
		void *		_handle;
		String		_name;
		bool		_freeWhenDelete;


	// methods
	public:
		Library () : _handle(null), _freeWhenDelete(true)
		{}

		explicit
		Library (StringCRef name, bool canFree = true) :
			_handle(null), _freeWhenDelete(true)
		{
			Load( name, canFree );
		}

		~Library ()
		{
			if ( _freeWhenDelete )
				Unload();
		}

		bool FindAndLoad (StringCRef name, uint searchDepth, bool canFree = true)
		{
			String	fname;

			if ( not FileSystem::SearchFile( name, searchDepth, OUT fname ) )
				return false;

			return Load( fname, canFree );
		}

		bool Load (StringCRef name, bool canFree = true)
		{
			ASSERT( name.IsNullTerminated() );

			Unload();

			_handle = ::SDL_LoadObject( name.cstr() );
			_name	= name;

			return IsValid();
		}

		void Unload ()
		{
			if ( _handle != null )
			{
				::SDL_UnloadObject( _handle );
				_handle = null;
			}
			_name.Clear();
		}

		bool IsValid () const
		{
			return _handle != null;
		}

		const String & GetName () const
		{
			return _name;
		}

		void * GetProc (StringCRef name, void *defProc = null) const
		{
			ASSERT( name.IsNullTerminated() );

			void *	tmp = ::SDL_LoadFunction( _handle, name.cstr() );
			return tmp != null ? tmp : defProc;
		}

		template <typename T>
		bool GetProc (T &func, StringCRef name)
		{
			func = (T) GetProc( name );
			return func != null;
		}
	};

}	// OS
}	// GX_STL

#endif	// PLATFORM_SDL