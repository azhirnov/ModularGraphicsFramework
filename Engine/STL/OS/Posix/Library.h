// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/OS/Posix/OSPosix.h"
#include "Engine/STL/OS/Posix/FileSystem.h"

#if defined( PLATFORM_BASE_POSIX ) and not defined( PLATFORM_SDL )

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
		Library ();
		explicit Library (StringCRef name, bool canFree = true);

		~Library ();
		
		void * Handle () const
		{
			return _library;
		}
		
		const String & GetName () const
		{
			return _name;
		}
		
		bool IsValid () const
		{
			return _library != null;
		}

		bool FindAndLoad (StringCRef name, GXTypes::uint searchDepth, bool canFree = true);
		
		bool Load (StringCRef name, bool canFree = true);
		
		bool Load (void * lib, bool canFree = false);
		
		bool LoadSelf ();
		
		void Unload ();

		template <typename T>
		bool GetProc (OUT T &proc, StringCRef procName) const
		{
			ASSERT( IsValid() );
			ASSERT( not procName.Empty() );

			T * tmp = ReferenceCast<T>( GetProc( procName ) );
			return tmp != null;
		}

		CHECKRES Func_t  GetProc (StringCRef procName, void *defProc = null) const;
	};
	

}	// OS
}	// GX_STL

#endif	// PLATFORM_BASE_POSIX
