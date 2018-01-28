// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Library.h"
#include "Engine/STL/Math/BinaryMath.h"
#include "Engine/STL/Math/Interpolations.h"

#if defined( PLATFORM_WINDOWS ) and \
	not defined( PLATFORM_SDL )

#include "Engine/STL/OS/Base/BaseFileSystem.h"
#include "WinHeader.h"

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
		_library( UninitializedT<HMODULE>() ),
		_freeWhenDelete(true)
	{}
	
/*
=================================================
	constructor
=================================================
*/
	Library::Library (StringCRef name, bool canFree):
		_library( UninitializedT<HMODULE>() ),
		_freeWhenDelete(true)
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
	IsValid
=================================================
*/
	CHECKRES(bool)  Library::IsValid () const
	{
		return _library.IsNotNull<HMODULE>();
	}
	
/*
=================================================
	FindAndLoad
=================================================
*/
	bool Library::FindAndLoad (StringCRef name, uint searchDepth, bool canFree)
	{
		String	fname;

		if ( not FileSystem::SearchFile( name, searchDepth, fname ) )
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
		if ( name == null )
			return false;

		Unload();

		_library		= ::LoadLibraryA( name.cstr() );
		_name			= name;
		_freeWhenDelete	= canFree;
			
		return IsValid();
	}
	
/*
=================================================
	Load
=================================================
*/
	bool Library::LoadFromHandle (const Handle_t &lib, bool canFree)
	{
		Unload();

		char	a_buf[MAX_PATH] = {0};
			
		_freeWhenDelete	 = canFree;
		_library		 = lib;
		uint	size	 = ::GetModuleFileNameA( _library.Get<HMODULE>(), a_buf, (DWORD) CountOf(a_buf) );

		if ( size == 0 ) {
			_library = null;
			return false;
		}

		_name.Copy( StringCRef( a_buf, size ) );
		return IsValid();
	}
	
/*
=================================================
	LoadSelf
=================================================
*/
	bool Library::LoadSelf ()
	{
		return LoadFromHandle( Handle_t( ::GetModuleHandle( (LPCSTR)null ) ) );
	}
	
/*
=================================================
	Unload
=================================================
*/
	bool Library::Unload ()
	{
		bool	ret = true;
			
		_name.Clear();

		if ( IsValid() )
		{
			ret		 = ( ::FreeLibrary( _library.Get<HMODULE>() ) != FALSE );
			_library = null;
		}
		return ret;
	}
	
/*
=================================================
	GetProc
=================================================
*/
	CHECKRES(Library::Func_t)  Library::GetProc (StringCRef procName, Func_t defProc) const
	{
		ASSERT( IsValid() );
		ASSERT( not procName.Empty() );

		Func_t tmp = ReferenceCast<Func_t>( GetProcAddress( _library.Get<HMODULE>(), procName.cstr() ) );
		return tmp != null ? tmp : defProc;
	}

	
}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS
