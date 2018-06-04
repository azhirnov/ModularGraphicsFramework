// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/STL/Algorithms/FileAddress.h"
#include "Engine/STL/Math/Vec.h"
#include "Engine/STL/Math/BinaryMath.h"
#include "Engine/STL/OS/Base/BaseFileSystem.h"

namespace GX_STL
{
namespace GXTypes
{
	
/*
=================================================
	_GetExtensionPos
=================================================
*/
	usize  FileAddress::_GetExtensionPos (StringCRef filename)
	{
		for (usize i = filename.Length()-1; i < filename.Length(); --i)
		{
			if ( filename[i] == '.' )
				return i;
		}
		return filename.Length();
	}
	
/*
=================================================
	_GetNamePos
=================================================
*/
	usize  FileAddress::_GetNamePos (StringCRef filename)
	{
		for (usize i = filename.Length()-1; i < filename.Length(); --i)
		{
			const char c = filename[i];

			if ( c == '/' or c == '\\' )
				return i;
		}
		return 0;
	}
	
/*
=================================================
	_SafeInc
=================================================
*/
	usize  FileAddress::_SafeInc (usize value, StringCRef str)
	{
		return	value == 0 ? 0 :
				(value+1 < str.Length() ? value+1 : str.Length());
	}

/*
=================================================
	GetPath
=================================================
*/
	StringCRef  FileAddress::GetPath (StringCRef filename)
	{
		return StringCRef( filename.cstr(), _GetNamePos( filename ) );
	}
	
/*
=================================================
	GetName
=================================================
*/
	StringCRef  FileAddress::GetName (StringCRef filename)
	{
		const isize	name_pos = _SafeInc( _GetNamePos( filename ), filename );
		const isize ext_pos  = _GetExtensionPos( filename );
		const usize len		 = (ext_pos >= name_pos ? ext_pos - name_pos : filename.Length() - name_pos);
		return StringCRef( filename.cstr() + name_pos, len );
	}
	
/*
=================================================
	GetExtension
=================================================
*/
	StringCRef  FileAddress::GetExtension (StringCRef filename)
	{
		const usize	name_pos = _SafeInc( _GetNamePos( filename ), filename );
		const usize ext_pos  = _GetExtensionPos( filename ) + 1;

		if ( ext_pos < filename.Length() )
			return StringCRef( filename.ptr() + ext_pos, filename.Length() - ext_pos );

		return StringCRef();
	}
	
/*
=================================================
	GetExtensions
=================================================
*/
	StringCRef  FileAddress::GetExtensions (StringCRef filename)
	{
		const usize	name_pos = _SafeInc( _GetNamePos( filename ), filename );
		usize		ext_pos  = name_pos;

		for (; ext_pos < filename.Length(); ++ext_pos)
		{
			if ( filename[ext_pos] == '.' and ext_pos+1 < filename.Length() )
			{
				++ext_pos;
				return StringCRef( filename.ptr() + ext_pos, filename.Length() - ext_pos );
			}
		}

		return StringCRef();
	}

/*
=================================================
	GetNameAndExt
=================================================
*/
	StringCRef  FileAddress::GetNameAndExt (StringCRef filename)
	{
		const usize name_pos = _SafeInc( _GetNamePos( filename ), filename );
		return StringCRef( filename.ptr() + name_pos, filename.Length() - name_pos );
	}
	
/*
=================================================
	GetPathAndName
=================================================
*/
	StringCRef  FileAddress::GetPathAndName (StringCRef filename)
	{
		const usize	ext_pos = _GetExtensionPos( filename );
		return filename.SubString( 0, ext_pos );
	}

/*
=================================================
	ToShortPath
=================================================
*/
	StringCRef  FileAddress::ToShortPath (StringCRef filename, const uint maxFolders)
	{
		uint num_folders = 0;

		FOR_rev( i, filename )
		{
			const char c = filename[i];
			
			if ( c == '/' or c == '\\')
			{
				++num_folders;

				if ( num_folders >= maxFolders )
					return filename.SubString( i+1 );
			}
		}

		return filename;
	}

/*
=================================================
	DividePath
=================================================
*/
	void  FileAddress::DividePath (StringCRef filename, OUT Array<StringCRef> &path)
	{
		usize	prev_pos = 0;

		path.Clear();
		path.Reserve(64);

		FOR( i, filename )
		{
			const char c = filename[i];

			if ( c == '/' or c == '\\' )
			{
				const isize len = i - prev_pos;

				if ( len > 0 )
					path.PushBack( StringCRef( filename.ptr() + prev_pos, len ) );

				prev_pos = i+1;
			}
		}

		const isize len = filename.Length() - prev_pos;

		if ( len > 0 )
			path.PushBack( StringCRef( filename.ptr() + prev_pos, len ) );
	}
	
/*
=================================================
	FormatPath
=================================================
*/
	void  FileAddress::FormatPath (INOUT String &filename)
	{
		if ( filename.Empty() )
			return;

		StaticArray<GXMath::usize2, 128>	pos_size_arr;
		usize								arr_len		= 0;
		usize								last_pos	= 0;
		String								temp;

		temp.Reserve( filename.Length() );

		for (usize i = 0; i < filename.Length(); ++i)
		{
			const char c = filename[i];

			if ( c == '/' or c == '\\' )
			{
				pos_size_arr[arr_len++] = GXMath::usize2( last_pos, i-last_pos );
				last_pos = i+1;
			}
		}

		temp = filename.cstr() + last_pos;

		uint	back = 0;

		for (isize i = arr_len-1; i >= 0; --i)
		{
			if ( pos_size_arr[i][1] == 2 and
				 filename[ pos_size_arr[i][0]   ] == '.' and
				 filename[ pos_size_arr[i][0]+1 ] == '.' )
			{
				 ++back;
			}
			else
			{
				if ( back != 0 ) {
					--back;
					continue;
				}
				else
				{
					'/' >> temp;
					temp.Insert( StringCRef( filename.cstr() + pos_size_arr[i][0], pos_size_arr[i][1] ), 0 );
				}
			}
		}

		for (usize i = 0; i < back; ++i) {
			"../" >> temp;
		}

		filename = RVREF( temp );
	}

/*
=================================================
	RemoveName
=================================================
*/
	void  FileAddress::RemoveName (INOUT String &filename)
	{
		const usize name_pos = _GetNamePos( filename );
		filename.Erase( name_pos, filename.Length() - name_pos );
	}
	
/*
=================================================
	RemoveExtension
=================================================
*/
	void  FileAddress::RemoveExtension (INOUT String &filename)
	{
		const usize ext_pos = _GetExtensionPos( filename );
		filename.Erase( ext_pos, filename.Length() - ext_pos );
	}
	
/*
=================================================
	PathMoveBack
=================================================
*/
	bool  FileAddress::PathMoveBack (INOUT String &path)
	{
		for (usize i = path.Length()-1; i < path.Length(); --i)
		{
			const char c = path[i];

			if ( c == '/' or c == '\\' )
			{
				path.SetLength( i );
				return true;
			}
		}
		return false;
	}
	
/*
=================================================
	RemoveDirectoriesFromLeft
=================================================
*/
	bool  FileAddress::RemoveDirectoriesFromLeft (INOUT String &path, const uint numDirectories)
	{
		if ( numDirectories == 0 )
			return true;

		int dirs = numDirectories;
		
		usize i = 0;

		for (; i < path.Length(); ++i)
		{
			const char c = path[i];

			if ( c == '/' or c == '\\' )
			{
				if ( --dirs <= 0 )
					break;
			}
		}
		
		path.Erase( i, path.Length() - i );

		return (dirs <= 0);
	}
	
/*
=================================================
	RemoveDirectoriesFromRight
=================================================
*/
	bool  FileAddress::RemoveDirectoriesFromRight (INOUT String &path, const uint numDirectories)
	{
		if ( numDirectories == 0 )
			return true;

		int dirs = numDirectories;

		for (usize i = path.Length()-1; i < path.Length(); --i)
		{
			const char c = path[i];

			if ( c == '/' or c == '\\' )
			{
				path.SetLength( i );

				if ( --dirs <= 0 )
					return true;
			}
		}
		return false;
	}

/*
=================================================
	AddDirectoryToPath
=================================================
*/
	void  FileAddress::AddDirectoryToPath (INOUT String &path, StringCRef dir)
	{
		if ( dir.Empty() )
			return;

		if ( path.Empty() )
		{
			path = dir;
			return;
		}

		const char p = path.Back();
		const char d = dir.Front();

		if ( p != '/' and p != '\\' )
			path << '/';

		if ( d == '/' or d == '\\' )
			path.PopBack();
		
		path << dir;
	}
	
/*
=================================================
	AddBaseDirectoryToPath
=================================================
*/
	void  FileAddress::AddBaseDirectoryToPath (INOUT String &path, StringCRef dir)
	{
		if ( dir.Empty() )
			return;

		if ( path.Empty() )
		{
			path = dir;
			return;
		}

		const char p = path.Front();
		const char d = dir.Back();

		if ( p != '/' and p != '\\' )
			path.Insert( '/', 0 );

		if ( d == '/' or d == '\\' )
			path.Erase(0);
		
		path.Insert( dir, 0 );
	}

/*
=================================================
	AddNameToPath
=================================================
*/
	void  FileAddress::AddNameToPath (INOUT String &path, StringCRef name)
	{
		return AddDirectoryToPath( INOUT path, name );
	}
	
/*
=================================================
	AddExtensionToName
=================================================
*/
	void  FileAddress::AddExtensionToName (INOUT String &filename, StringCRef ext)
	{
		if ( ext.Empty() )
			return;

		if ( filename.Empty() or filename.Back() != '.' )
			filename << '.';

		if ( ext.Front() == '.' )
			filename.PopBack();

		filename << ext;
	}

/*
=================================================
	BuildPath
=================================================
*/
	String  FileAddress::BuildPath (StringCRef path, StringCRef nameWithExt)
	{
		ASSERT( not OS::FileSystem::IsAbsolutePath( nameWithExt ) );

		String	result = path;
		AddNameToPath( INOUT result, nameWithExt );
		return result;
	}

/*
=================================================
	BuildPath
=================================================
*/
	String  FileAddress::BuildPath (StringCRef path, StringCRef name, StringCRef ext)
	{
		ASSERT( not OS::FileSystem::IsAbsolutePath( name ) );

		String	result = path;
		AddNameToPath( INOUT result, name );
		AddExtensionToName( INOUT result, ext );
		return result;
	}
	
/*
=================================================
	AbsoluteToRelativePath
=================================================
*/
	bool  FileAddress::AbsoluteToRelativePath (StringCRef source, StringCRef base, OUT String &result)
	{
		Array< StringCRef >	path1;
		Array< StringCRef >	path2;

		DividePath( source, OUT path1 );
		DividePath( base, OUT path2 );

		if ( path1.Empty() or path2.Empty() )
		{
			result = source;
			return true;
		}

		CHECK_ERR( path1.Front() == path2.Front() );

		usize	i = 0;

		for (; i < path1.Count() and i < path2.Count(); ++i)
		{
			if ( path1[i] != path2[i] )
				break;

			//AddDirectoryToPath( INOUT result, path1[i] );
		}

		for (usize j = i; j < path2.Count(); ++j)
		{
			AddDirectoryToPath( INOUT result, ".." );
		}

		for (; i < path1.Count(); ++i)
		{
			AddDirectoryToPath( INOUT result, path1[i] );
		}

		return true;
	}


}	// GXTypes
}	// GX_STL
