// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Platforms.h"

#if defined( PLATFORM_BASE_POSIX ) and not defined( PLATFORM_SDL )

#include "Engine/STL/OS/Posix/OSPosix.h"
#include "Engine/STL/OS/Posix/FileSystem.h"

namespace GX_STL
{
	using SharedLibFunction_t 	= void (*) ();

namespace OS
{

	//
	// Library
	//

	struct Library final : public Noncopyable
	{
	// types
	public:
		using Func_t	= SharedLibFunction_t;


	// variables
	private:
		void *		_library;
		String		_name;
		bool		_freeWhenDelete;


	// methods
	public:
		explicit Library (StringCRef name, bool canFree = true);

		Library ();
		~Library ();

		bool FindAndLoad (StringCRef name, GXTypes::uint searchDepth, bool canFree = true);
		bool Load (StringCRef name, bool canFree = true);
		bool LoadSelf ();
		void Unload ();
		
		ND_ StringCRef	GetName () const		{ return _name; }
		
		ND_ bool		IsValid () const		{ return _library != null; }

		ND_ Func_t		GetProc (StringCRef procName, Func_t defProc = null) const;

		template <typename T>
		bool GetProc (OUT T &proc, StringCRef procName) const
		{
			ASSERT( IsValid() );
			ASSERT( not procName.Empty() );

			T * tmp = ReferenceCast<T>( GetProc( procName ) );
			return tmp != null;
		}

	private:
		bool _LoadFromHandle (void * lib, bool canFree = false);
	};
	

}	// OS
}	// GX_STL

#endif	// PLATFORM_BASE_POSIX
