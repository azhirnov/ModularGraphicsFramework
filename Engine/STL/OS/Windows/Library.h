// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "OSWindows.h"

#if defined( PLATFORM_WINDOWS ) and \
	not defined( PLATFORM_SDL )

namespace GX_STL
{
namespace OS
{

	//
	// Library
	//

	struct Library
	{
	// types
	private:
		typedef HiddenOSTypeFrom<void *>	Handle_t;	// HMODULE
		typedef Library						Self;


	// variables
	private:
		Handle_t	_library;
		String		_name;
		bool		_freeWhenDelete;


	// methods
	public:
		Library ();

		explicit
		Library (StringCRef name, bool canFree = true);

		~Library ();
		
		//void * Handle () const;
		bool IsValid () const;

		const String & GetName () const		{ return _name; }

		bool FindAndLoad (StringCRef name, uint searchDepth, bool canFree = true);
		bool LoadFromHandle (const Handle_t &lib, bool canFree = false);
		bool Load (StringCRef name, bool canFree = true);
		bool LoadSelf ();
		bool Unload ();

		template <typename T>
		bool GetProc (T *&proc, StringCRef procName) const
		{
			ASSERT( IsValid() );
			ASSERT( not procName.Empty() );

			T * tmp = PointerCast< T >( GetProc( procName.cstr() ) );

			if ( tmp != null )
			{
				proc = tmp;
				return true;
			}
			return false;
		}

		void * GetProc (StringCRef procName, void *defProc = null) const;
	};

	
}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS