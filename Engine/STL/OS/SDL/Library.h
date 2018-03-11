// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Platforms.h"

#ifdef PLATFORM_SDL

#include "Engine/STL/OS/SDL/OS_SDL.h"
#include "Engine/STL/OS/Base/BaseFileSystem.h"

namespace GX_STL
{
	using SharedLibFunction_t 	= void (*) ();

namespace OS
{

	//
	// Library
	//

	struct Library
	{
	// types
	public:
		using Func_t	= SharedLibFunction_t;


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
			Unload();

			_handle			= ::SDL_LoadObject( name.cstr() );
			_name			= name;
			_freeWhenDelete	= canFree;

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

		CHECKRES Func_t GetProc (StringCRef name, Func_t defProc = null) const
		{
			Func_t	tmp = reinterpret_cast<Func_t>(::SDL_LoadFunction( _handle, name.cstr() ));
			return tmp != null ? tmp : defProc;
		}

		template <typename T>
		bool GetProc (OUT T &func, StringCRef name)
		{
			func = ReferenceCast<T>( GetProc( name ) );
			return func != null;
		}
	};

}	// OS
}	// GX_STL

#endif	// PLATFORM_SDL