// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/STL/Common/Platforms.h"
#include "Core/Config/STL.Config.h"

#if defined( PLATFORM_WINDOWS ) and defined( GX_USE_NATIVE_API )

#include "Core/STL/OS/Windows/WinLibrary.h"
#include "Core/STL/Math/Interpolations.h"
#include "Core/STL/OS/Base/BaseFileSystem.h"
#include "Core/STL/OS/Windows/WinHeader.h"

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
	bool Library::IsValid () const
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
	_LoadFromHandle
=================================================
*/
	bool Library::_LoadFromHandle (const Handle_t &lib, bool canFree)
	{
		Unload();

		char	a_buf[MAX_PATH] = {0};
			
		_freeWhenDelete	 = canFree;
		_library		 = lib;
		uint	size	 = ::GetModuleFileNameA( _library.Get<HMODULE>(), OUT a_buf, (DWORD) CountOf(a_buf) );

		if ( size == 0 ) {
			_library = null;
			return false;
		}

		_name = StringCRef( a_buf, size );
		return IsValid();
	}
	
/*
=================================================
	LoadSelf
=================================================
*/
	bool Library::LoadSelf ()
	{
		return _LoadFromHandle( Handle_t( ::GetModuleHandle( (LPCSTR)null ) ) );
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
	Library::Func_t  Library::GetProc (StringCRef procName, Func_t defProc) const
	{
		ASSERT( IsValid() );
		ASSERT( not procName.Empty() );

		Func_t tmp = ReferenceCast<Func_t>( ::GetProcAddress( _library.Get<HMODULE>(), procName.cstr() ) );
		return tmp != null ? tmp : defProc;
	}
	
/*
=================================================
	GetFullPath
=================================================
*
	String   Library::GetFullPath () const
	{
		char buf[MAX_PATH] = "";
		::GetModuleFileNameA( _library.Get<HMODULE>(), buf, MAX_PATH );

		return buf;
	}
*/
	
}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS and GX_USE_NATIVE_API
