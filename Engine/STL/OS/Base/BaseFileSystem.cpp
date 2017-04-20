// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "BaseFileSystem.h"

namespace GX_STL
{
namespace OS
{

/*
=================================================
	SearchFile
=================================================
*/
	bool FileSystem::SearchFile (StringCRef file, uint depth, OUT String &result)
	{
		return _Search< TFileSearch >( file, depth, result );
	}

/*
=================================================
	SearchFileForward
=================================================
*/
	bool FileSystem::SearchFileForward (StringCRef file, uint depth, OUT String &result)
	{
		return _SearchForward< TFileSearch >( "", file, depth, result );
	}
	
/*
=================================================
	SearchFileBackward
=================================================
*/
	bool FileSystem::SearchFileBackward (StringCRef file, uint depth, OUT String &result)
	{
		return _SearchBackward< TFileSearch >( file, depth, result );
	}
	
/*
=================================================
	SearchDir
=================================================
*/
	bool FileSystem::SearchDir (StringCRef dir, uint depth, OUT String &result)
	{
		return _Search< TDirSearch >( dir, depth, result );
	}

/*
=================================================
	SearchDirForward
=================================================
*/
	bool FileSystem::SearchDirForward (StringCRef dir, uint depth, OUT String &result)
	{
		return _SearchForward< TDirSearch >( dir.cstr(), dir, depth, result );
	}
	
/*
=================================================
	SearchDirBackward
=================================================
*/
	bool FileSystem::SearchDirBackward (StringCRef dir, uint depth, OUT String &result)
	{
		return _SearchBackward< TDirSearch >( dir, depth, result );
	}
	
/*
=================================================
	_Search
=================================================
*/
	template <typename B>
	bool FileSystem::_Search (StringCRef name, uint depth, OUT String &result)
	{
		String	tmp;
		String	fname;	  

		if ( _SearchForward<B>( "", name, depth, result ) )
			return true;

		for (uint i = 0; i < depth; ++i)
		{
			FileAddress::BuildPath( fname, tmp, name );

			if ( B::IsExist( fname ) ) {
				result = RVREF( fname );
				return true;
			}
			
			"../" >> tmp;

			if ( _SearchForward<B>( tmp, name, depth-i, result ) )
				return true;
		}
		return false;
	}
	
/*
=================================================
	_SearchForward
=================================================
*/
	template <typename B>
	bool FileSystem::_SearchForward (StringCRef dir, StringCRef name, uint depth, OUT String &result)
	{
		if ( depth == 0 )
			return false;

		Array<String>	dirs;
		CHECK_ERR( GetAllDirsInPath( dir, OUT dirs ) );

		String	tmp;
		
		// search in current directory
		FOR( i, dirs )
		{
			tmp = dir;
			FileAddress::AddDirectoryToPath( INOUT tmp, dirs[i] );
			FileAddress::AddNameToPath( INOUT tmp, name );

			if ( B::IsExist( tmp ) )
			{
				result = RVREF( tmp );
				return true;
			}
		}

		// recursive search
		FOR( i, dirs )
		{
			tmp = dir;
			FileAddress::AddDirectoryToPath( INOUT tmp, dirs[i] );

			if ( _SearchForward<B>( tmp, name, depth-1, OUT result ) )
				return true;
		}

		return false;
	}

/*
=================================================
	_SearchBackward
=================================================
*/
	template <typename B>
	bool FileSystem::_SearchBackward (StringCRef name, uint depth, OUT String &result)
	{
		result = name;
		
		for (uint i = 0; i < depth; ++i)
		{
			if ( B::IsExist( result ) ) {
				return true;
			}
			"../" >> result;
		}
		result.Clear();
		return false;
	}
	
/*
=================================================
	CreateDirectories
=================================================
*/
	bool FileSystem::CreateDirectories (StringCRef path)
	{
		if ( path.Empty() or IsDirectoryExist( path ) )
			return true;

		String	tmp		= path;
		int		depth	= 0;

		while ( FileAddress::PathMoveBack( tmp ) )
		{
			if ( IsDirectoryExist( tmp ) )
				break;

			++depth;
		}

		while ( depth >= 0 )
		{
			tmp = path;
			CHECK( FileAddress::RemoveDirectoriesFromRight( tmp, depth-- ) );
			CHECK_ERR( NewDirectory( tmp ) );
		}
		
		return IsDirectoryExist( path );
	}
	
/*
=================================================
	FindAndSetCurrentDir
=================================================
*/
	bool FileSystem::FindAndSetCurrentDir (StringCRef dirname, uint searchDepth)
	{
		String	dir;
		CHECK_ERR( SearchDirBackward( dirname, searchDepth, dir ) or SearchDir( dirname, searchDepth, dir ) );
		
		CHECK_ERR( SetCurrentDirectory( dir.cstr() ) );
		return true;
	}


}	// OS
}	// GX_STL
