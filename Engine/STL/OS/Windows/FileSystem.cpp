// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "FileSystem.h"
#include "Engine/STL/Math/BinaryMath.h"

#ifdef PLATFORM_WINDOWS

#include "Engine/STL/OS/Base/BaseFileSystem.h"
#include "WinHeader.h"

namespace GX_STL
{
namespace OS
{

/*
=================================================
	DeleteFile
=================================================
*/
	bool OSFileSystem::DeleteFile (StringCRef filename)
	{
		ASSERT( filename.IsNullTerminated() );

		return ::DeleteFileA( filename.cstr() ) == TRUE;
	}
	
/*
=================================================
	SetCurrentDirectory
=================================================
*/
	bool OSFileSystem::SetCurrentDirectory (StringCRef dir)
	{
		if ( dir.Empty() )
			return false;
		
		ASSERT( dir.IsNullTerminated() );

		return ::SetCurrentDirectoryA( dir.cstr() ) == TRUE;
	}
	
/*
=================================================
	GetCurrentDirectory
=================================================
*/
	bool OSFileSystem::GetCurrentDirectory (OUT String &dir)
	{
		dir.Reserve( MAX_PATH+1 );

		if ( ::GetCurrentDirectoryA( (DWORD) dir.Capacity(), dir.ptr() ) != 0 )
		{
			dir.CalculateLength();
			return true;
		}
		dir.Clear();
		return false;
	}
	
/*
=================================================
	IsFileExist
=================================================
*/
	bool OSFileSystem::IsFileExist (StringCRef filename)
	{
		if ( filename.Empty() )
			return false;
		
		ASSERT( filename.IsNullTerminated() );

		int i_code = ::GetFileAttributes( filename.cstr() );
		return (i_code != -1) and not (FILE_ATTRIBUTE_DIRECTORY & i_code);
	}
	
/*
=================================================
	IsDirectoryExist
=================================================
*/
	bool OSFileSystem::IsDirectoryExist (StringCRef dirname)
	{
		if ( dirname.Empty() )
			return true;
		
		ASSERT( dirname.IsNullTerminated() );

		int i_code = ::GetFileAttributes( dirname.cstr() );
		return (i_code != -1) and (FILE_ATTRIBUTE_DIRECTORY & i_code);
	}
	
/*
=================================================
	NewDirectory
=================================================
*/
	bool OSFileSystem::NewDirectory (StringCRef dir)
	{
		if ( dir.Empty() )
			return true;
		
		ASSERT( dir.IsNullTerminated() );
		ASSERT( not IsDirectoryExist( dir ) );

		return ::CreateDirectoryA( dir.cstr(), null ) == TRUE;
	}
	
/*
=================================================
	DeleteEmptyDirectory
=================================================
*/
	bool OSFileSystem::DeleteEmptyDirectory (StringCRef dir)
	{
		ASSERT( dir.IsNullTerminated() );
		ASSERT( IsDirectoryExist( dir ) );

		return ::RemoveDirectoryA( dir.cstr() ) == TRUE;
	}
	
/*
=================================================
	DeleteDirectory
=================================================
*/
	bool OSFileSystem::DeleteDirectory (StringCRef dir)
	{
		ASSERT( dir.IsNullTerminated() );
		ASSERT( IsDirectoryExist( dir ) );

		SHFILEOPSTRUCTA file_op = {
			null,
			FO_DELETE,
			dir.cstr(),
			"",
			FOF_NOCONFIRMATION |
			FOF_NOERRORUI |
			FOF_SILENT,
			false,
			0,
			"" };

		return SHFileOperation( &file_op ) == 0;
	}
	
/*
=================================================
	GetAllFilesInPath
=================================================
*/
	bool OSFileSystem::GetAllFilesInPath (StringCRef path, Array<String> &fileNames)
	{
		ASSERT( IsDirectoryExist( path ) );

		fileNames.Clear();

		String	dir( path );
		
		if ( dir.Empty() or dir.Back() == '\\' or dir.Back() == '/' )	dir << '*';
		else															dir << "\\*";

		WIN32_FIND_DATAA	s_wfd;
		HANDLE				h_search = ::FindFirstFileA( dir.cstr(), &s_wfd );
		
		if ( h_search == INVALID_HANDLE_VALUE )
			return true;

		do
		{
			if ( not (s_wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
			{
				fileNames.PushBack( FileAddress::GetNameAndExt( StringCRef( s_wfd.cFileName ) ) );
			}
		}
		while ( ::FindNextFileA( h_search, &s_wfd ) );
		
		::FindClose( h_search );
		return true;
	}

/*
=================================================
	GetAllDirsInPath
=================================================
*/
	bool OSFileSystem::GetAllDirsInPath (StringCRef path, Array<String> &directories)
	{
		ASSERT( IsDirectoryExist( path ) );

		directories.Clear();

		String	dir( path );
		
		if ( dir.Empty() or dir.Back() == '\\' or dir.Back() == '/' )	dir << '*';
		else															dir << "\\*";

		WIN32_FIND_DATAA	s_wfd;
		HANDLE				h_search   = ::FindFirstFileA( dir.cstr(), &s_wfd );
		
		if ( h_search == INVALID_HANDLE_VALUE )
			return true;

		do
		{
			if  ( s_wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				if ( s_wfd.cFileName[0] == '.' and s_wfd.cFileName[1] == '\0' )
					continue;
				
				if ( s_wfd.cFileName[0] == '.' and s_wfd.cFileName[1] == '.' and s_wfd.cFileName[2] == '\0' )
					continue;
				
				directories.PushBack( FileAddress::GetNameAndExt( StringCRef( s_wfd.cFileName ) ) );
			}
		}
		while ( ::FindNextFileA( h_search, &s_wfd ) );
		
		::FindClose( h_search );
		return true;
	}
	
/*
=================================================
	CopyFile
=================================================
*/
	bool OSFileSystem::CopyFile (StringCRef fromFile, StringCRef toFile)
	{
		ASSERT( fromFile.IsNullTerminated() );
		ASSERT( toFile.IsNullTerminated() );

		return ::CopyFileA( fromFile.cstr(), toFile.cstr(), FALSE ) == TRUE;
	}
	
/*
=================================================
	CopyDirectory
=================================================
*/
	bool OSFileSystem::CopyDirectory (StringCRef fromDir, StringCRef toDir)
	{
		SHFILEOPSTRUCTA s;
		UnsafeMem::ZeroMem( &s, SizeOf(s) );

		String	from_dir	= fromDir;
		String	to_dir		= toDir;

		// can't use << '\0' - it's have no effect
		from_dir << ' ';		from_dir.Back() = '\0';
		to_dir   << ' ';		from_dir.Back() = '\0';

		s.wFunc		= FO_COPY;
		s.fFlags	= FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;
		s.pTo		= to_dir.cstr();
		s.pFrom		= from_dir.cstr();

		int res = SHFileOperation( &s );

		ASSERT( res == 0 );
		return res == 0;
	}
	
/*
=================================================
	GetFileLastModificationTime
=================================================
*/
	bool OSFileSystem::MoveFile (StringCRef oldName, StringCRef newName, bool async)
	{
		ASSERT( oldName.IsNullTerminated() );
		ASSERT( newName.IsNullTerminated() );

		const uint	flags = MOVEFILE_COPY_ALLOWED | (async ? 0 : MOVEFILE_WRITE_THROUGH);

		return ::MoveFileEx( oldName.cstr(), newName.cstr(), flags ) == TRUE;
	}

/*
=================================================
	GetFileLastModificationTime
=================================================
*/
	ulong OSFileSystem::GetFileLastModificationTime (StringCRef filename)
	{
		ASSERT( filename.IsNullTerminated() );

		FILETIME	time = {};
		HANDLE		file = ::CreateFileA( filename.cstr(), GENERIC_READ,
							FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
							null, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, null );

		if ( file == null or ::GetFileTime( file, null, &time, null ) != TRUE )
			return 0;

		return (ulong(time.dwHighDateTime) << 32) | ulong(time.dwLowDateTime);
	}
	
/*
=================================================
	GetFileCreationTime
=================================================
*/
	ulong OSFileSystem::GetFileCreationTime (StringCRef filename)
	{
		ASSERT( filename.IsNullTerminated() );

		FILETIME	time = {};
		HANDLE		file = ::CreateFileA( filename.cstr(), GENERIC_READ,
							FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
							null, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, null );

		if ( file == null or ::GetFileTime( file, &time, null, null ) != TRUE )
			return 0;

		return (ulong(time.dwHighDateTime) << 32) | ulong(time.dwLowDateTime);
	}
	
/*
=================================================
	GetFileSize
=================================================
*/
	ulong OSFileSystem::GetFileSize (StringCRef filename)
	{
		ASSERT( filename.IsNullTerminated() );

		HANDLE		file = ::CreateFileA( filename.cstr(), GENERIC_READ,
							FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
							null, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, null );

		if ( file == null )
			return 0;

		LARGE_INTEGER	size;
		::GetFileSizeEx( file, &size );

		return ulong(size.QuadPart);
	}


}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS
