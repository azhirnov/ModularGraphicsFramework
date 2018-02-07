// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/STL/OS/Windows/FileSystem.h"
#include "Engine/STL/Math/BinaryMath.h"
#include "Engine/STL/Algorithms/FileAddress.h"

#ifdef PLATFORM_WINDOWS

#include "Engine/STL/OS/Base/BaseFileSystem.h"
#include "Engine/STL/OS/Windows/WinHeader.h"

namespace GX_STL
{
namespace OS
{

/*
=================================================
	DeleteFile
=================================================
*/
	bool WindowsFileSystem::DeleteFile (StringCRef filename)
	{
		return ::DeleteFileA( filename.cstr() ) != FALSE;
	}
	
/*
=================================================
	SetCurrentDirectory
=================================================
*/
	bool WindowsFileSystem::SetCurrentDirectory (StringCRef dir)
	{
		if ( dir.Empty() )
			return false;
		
		return ::SetCurrentDirectoryA( dir.cstr() ) != FALSE;
	}
	
/*
=================================================
	GetCurrentDirectory
=================================================
*/
	bool WindowsFileSystem::GetCurrentDirectory (OUT String &dir)
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
	bool WindowsFileSystem::IsFileExist (StringCRef filename)
	{
		if ( filename.Empty() )
			return false;

		int i_code = ::GetFileAttributes( filename.cstr() );
		return (i_code != -1) and not (FILE_ATTRIBUTE_DIRECTORY & i_code);
	}
	
/*
=================================================
	IsDirectoryExist
=================================================
*/
	bool WindowsFileSystem::IsDirectoryExist (StringCRef dirname)
	{
		if ( dirname.Empty() )
			return true;

		int i_code = ::GetFileAttributes( dirname.cstr() );
		return (i_code != -1) and (FILE_ATTRIBUTE_DIRECTORY & i_code);
	}
	
/*
=================================================
	NewDirectory
=================================================
*/
	bool WindowsFileSystem::NewDirectory (StringCRef dir)
	{
		if ( dir.Empty() )
			return true;

		ASSERT( not IsDirectoryExist( dir ) );

		return ::CreateDirectoryA( dir.cstr(), null ) != FALSE;
	}
	
/*
=================================================
	DeleteEmptyDirectory
=================================================
*/
	bool WindowsFileSystem::DeleteEmptyDirectory (StringCRef dir)
	{
		ASSERT( IsDirectoryExist( dir ) );

		return ::RemoveDirectoryA( dir.cstr() ) != FALSE;
	}
	
/*
=================================================
	DeleteDirectory
=================================================
*/
	bool WindowsFileSystem::DeleteDirectory (StringCRef dir)
	{
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
	bool WindowsFileSystem::GetAllFilesInPath (StringCRef path, Array<String> &fileNames)
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
	bool WindowsFileSystem::GetAllDirsInPath (StringCRef path, Array<String> &directories)
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
	bool WindowsFileSystem::CopyFile (StringCRef fromFile, StringCRef toFile)
	{
		return ::CopyFileA( fromFile.cstr(), toFile.cstr(), FALSE ) != FALSE;
	}
	
/*
=================================================
	CopyDirectory
=================================================
*/
	bool WindowsFileSystem::CopyDirectory (StringCRef fromDir, StringCRef toDir)
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
	MoveFile
=================================================
*/
	bool WindowsFileSystem::MoveFile (StringCRef oldName, StringCRef newName, bool async)
	{
		const uint	flags = MOVEFILE_COPY_ALLOWED | (async ? 0 : MOVEFILE_WRITE_THROUGH);

		return ::MoveFileEx( oldName.cstr(), newName.cstr(), flags ) != FALSE;
	}

/*
=================================================
	SystemTimeToDate
=================================================
*/
	static Date SystemTimeToDate (const SYSTEMTIME &sysTime)
	{
		Date	result;
		result.SetYear( sysTime.wYear );
		result.SetMonth( sysTime.wMonth );
		result.SetDay( sysTime.wDay );
		result.SetHour( sysTime.wHour );
		result.SetMinute( sysTime.wMinute );
		result.SetSecond( sysTime.wSecond );
		result.SetMillis( sysTime.wMilliseconds );
		return result;
	}
	
/*
=================================================
	GetFileLastModificationTime
=================================================
*/
	Date WindowsFileSystem::GetFileLastModificationTime (StringCRef filename)
	{
		FILETIME	atime = {};
		FILETIME	wtime = {};
		HANDLE		file = ::CreateFileA( filename.cstr(), GENERIC_READ,
							FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
							null, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, null );

		if ( file == null or ::GetFileTime( file, null, OUT &atime, OUT &wtime ) != TRUE )
			return Uninitialized;

		ulong	a = (ulong(atime.dwHighDateTime) << 32) | ulong(atime.dwLowDateTime);
		ulong	w = (ulong(wtime.dwHighDateTime) << 32) | ulong(wtime.dwLowDateTime);

		SYSTEMTIME	sys_time = {};

		if ( ::FileTimeToSystemTime( a > w ? &atime : &wtime, OUT &sys_time ) == FALSE )
			return Uninitialized;

		return SystemTimeToDate( sys_time );
	}
	
/*
=================================================
	GetFileCreationTime
=================================================
*/
	Date WindowsFileSystem::GetFileCreationTime (StringCRef filename)
	{
		FILETIME	time = {};
		HANDLE		file = ::CreateFileA( filename.cstr(), GENERIC_READ,
							FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
							null, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, null );

		if ( file == null or ::GetFileTime( file, OUT &time, null, null ) != TRUE )
			return Uninitialized;
		
		SYSTEMTIME	sys_time = {};

		if ( ::FileTimeToSystemTime( &time, OUT &sys_time ) == FALSE )
			return Uninitialized;

		return SystemTimeToDate( sys_time );
	}
	
/*
=================================================
	GetFileSize
=================================================
*/
	BytesUL WindowsFileSystem::GetFileSize (StringCRef filename)
	{
		HANDLE	file = ::CreateFileA( filename.cstr(), GENERIC_READ,
							FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
							null, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, null );

		if ( file == null )
			return Uninitialized;

		LARGE_INTEGER	size;
		::GetFileSizeEx( file, OUT &size );

		return BytesUL(size.QuadPart);
	}


}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS
