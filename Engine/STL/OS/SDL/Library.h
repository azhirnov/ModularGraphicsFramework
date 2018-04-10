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
		explicit Library (StringCRef name, bool canFree = true);

		Library ();
		~Library ();

		bool FindAndLoad (StringCRef name, uint searchDepth, bool canFree = true);
		bool Load (StringCRef name, bool canFree = true);
		void Unload ();

		bool		IsValid () const	{ return _handle != null; }

		StringCRef	GetName () const	{ return _name; }

		CHECKRES Func_t GetProc (StringCRef name, Func_t defProc = null) const;

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
