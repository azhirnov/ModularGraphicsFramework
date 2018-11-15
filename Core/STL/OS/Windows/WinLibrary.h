// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Platforms.h"

#if defined( PLATFORM_WINDOWS ) and defined( GX_USE_NATIVE_API )

#include "Core/STL/OS/Windows/OSWindows.h"

namespace GX_STL
{
	using SharedLibFunction_t 	= void (CALLBACK *) ();

namespace OS
{

	//
	// Library
	//

	struct Library final : public Noncopyable
	{
	// types
	public:
		using Handle_t	= DeferredTypeFrom<void *>;	// HMODULE
		using Self		= Library;
		using Func_t	= SharedLibFunction_t;


	// variables
	private:
		Handle_t	_library;
		String		_name;
		bool		_freeWhenDelete;


	// methods
	public:
		explicit Library (StringCRef name, bool canFree = true);

		Library ();
		~Library ();

		bool FindAndLoad (StringCRef name, uint searchDepth, bool canFree = true);
		bool Load (StringCRef name, bool canFree = true);
		bool LoadSelf ();
		bool Unload ();
		
		ND_ bool		IsValid () const;

		ND_ StringCRef	GetName () const		{ return _name; }

		ND_ Func_t		GetProc (StringCRef procName, Func_t defProc = null) const;

		template <typename T>
		bool GetProc (OUT T &proc, StringCRef procName) const
		{
			ASSERT( IsValid() );
			ASSERT( not procName.Empty() );

			T tmp = ReferenceCast<T>( GetProc( procName.cstr() ) );

			if ( tmp != null )
			{
				proc = tmp;
				return true;
			}
			return false;
		}

	private:
		bool _LoadFromHandle (const Handle_t &lib, bool canFree = false);
	};

	
}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS and GX_USE_NATIVE_API
