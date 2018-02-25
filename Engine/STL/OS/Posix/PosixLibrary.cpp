// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/STL/OS/Posix/Library.h"
#include "Engine/STL/Math/BinaryMath.h"

#if defined( PLATFORM_BASE_POSIX ) and not defined( PLATFORM_SDL )

#include "Engine/STL/OS/Posix/PosixHeader.h"

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
	Load
=================================================
*/
	bool Library::Load (void * lib, bool canFree)
	{
		Unload();

		_freeWhenDelete		= canFree;
		_library			= lib;

		Dl_info	info;

		if ( ::dladdr( _library, OUT &info ) != 0 )
			_name = info.dli_fname;
			
		return IsValid();
	}
		
/*
=================================================
	LoadSelf
=================================================
*/
	bool Library::LoadSelf ()
	{
		return Load( ::dlopen( null, RTLD_GLOBAL ) );
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

#endif	// PLATFORM_BASE_POSIX
