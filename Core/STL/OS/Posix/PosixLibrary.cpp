// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/STL/Common/Platforms.h"

#if defined( PLATFORM_BASE_POSIX ) and defined( GX_USE_NATIVE_API )

#include "Core/STL/OS/Posix/PosixHeader.h"
#include "Core/STL/OS/Posix/Library.h"
#include "Core/STL/OS/Base/BaseFileSystem.h"

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
		_library(null), _freeWhenDelete(true)
	{}

	Library::Library (StringCRef name, bool canFree) :
		_library(null), _freeWhenDelete(true)
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
	bool Library::FindAndLoad (StringCRef name, GXTypes::uint searchDepth, bool canFree)
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
		if ( name.Empty() )
			return false;

		Unload();

		_library		= ::dlopen( name.cstr(), RTLD_GLOBAL );
		_name			= name;
		_freeWhenDelete = canFree;
			
		return IsValid();
	}

/*
=================================================
	LoadSelf
=================================================
*/
	bool Library::LoadSelf ()
	{
		Unload();

		_freeWhenDelete		= true;
		_library			= ::dlopen( null, RTLD_GLOBAL );

		Dl_info	info;

		if ( ::dladdr( _library, OUT &info ) != 0 )
			_name = info.dli_fname;
			
		return IsValid();
	}
		
/*
=================================================
	Unload
=================================================
*/
	void Library::Unload ()
	{
		_name.Clear();

		if ( IsValid() )
		{
			::dlclose( _library );
			_library = null;
		}
	}
		
/*
=================================================
	GetProc
=================================================
*/
	Library::Func_t  Library::GetProc (StringCRef procName, Func_t defProc) const
	{
		ASSERT( IsValid() );
		ASSERT( not procName.Empty() );

		Func_t tmp = reinterpret_cast<Func_t>(::dlsym( _library, procName.cstr() ));
		return tmp != null ? tmp : defProc;
	}
	

}	// OS
}	// GX_STL

#endif	// PLATFORM_BASE_POSIX and GX_USE_NATIVE_API
