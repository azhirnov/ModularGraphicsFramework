// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "OSPosix.h"
#include "FileSystem.h"

#ifdef PLATFORM_BASE_POSIX_SHELL

namespace GX_STL
{
namespace OS
{
	using namespace posix;

	
	//
	// Library
	//

	struct Library : public Noncopyable
	{
	// variables
	private:
		void *				_library;
		GXTypes::String		_name;
		bool				_freeWhenDelete;


	// methods
	public:
		Library (): _library(null), _freeWhenDelete(true)
		{}

		explicit
		Library (StringCRef name, bool canFree = true):
			_library(null), _freeWhenDelete(true)
		{
			Load( name, canFree );
		}

		~Library ()
		{
			if ( _freeWhenDelete )
				Unload();
		}
		
		void * Handle () const
		{
			return _library;
		}
		
		const GXTypes::String & GetName () const
		{
			return _name;
		}
		
		bool IsValid () const
		{
			return _library != null;
		}

		bool FindAndLoad (StringCRef name, GXTypes::uint searchDepth, bool canFree = true)
		{
			String	fname;

			if ( not FileSystem::SearchFile( name, searchDepth, OUT fname ) )
				return false;

			return Load( fname, canFree );
		}
		
		bool Load (StringCRef name, bool canFree = true)
		{
			if ( name.Empty() )
				return false;

			Unload();
			
			ASSERT( name.IsNullTerminated() );

			_library		= dlopen( name.cstr(), RTLD_GLOBAL );
			_name			= name;
			_freeWhenDelete = canFree;
			
			return IsValid();
		}
		
		bool Load (void * lib, bool canFree = false)
		{
			Unload();

			_freeWhenDelete		= canFree;
			_library			= lib;

			Dl_info	info;

			if ( dladdr( _library, &info ) != 0 )
				_name = info.dli_fname;
			
			return IsValid();
		}
		
		bool LoadSelf ()
		{
			return Load( dlopen( null, RTLD_GLOBAL ) );
		}
		
		void Unload ()
		{
			_name.Clear();

			if ( IsValid() )
			{
				dlclose( _library );
				_library = null;
			}
		}

		template <typename T>
		bool GetProc (T *&proc, StringCRef procName) const
		{
			ASSERT( IsValid() );
			ASSERT( not procName.Empty() );
			ASSERT( procName.IsNullTerminated() );

			T * tmp = reinterpret_cast< T *>( dlsym( _library, procName.cstr() ) );

			if ( tmp != null )
			{
				proc = tmp;
				return true;
			}
			return false;
		}

		void * GetProc (StringCRef procName, void *defProc = null) const
		{
			ASSERT( IsValid() );
			ASSERT( not procName.Empty() );
			ASSERT( procName.IsNullTerminated() );

			void * tmp = dlsym( _library, procName.cstr() );
			return tmp != null ? tmp : defProc;
		}
	};
	
}	// OS
}	// GX_STL

#endif	// PLATFORM_BASE_POSIX_SHELL