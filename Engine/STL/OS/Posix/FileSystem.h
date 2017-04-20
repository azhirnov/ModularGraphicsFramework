// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "OSPosix.h"
#include "Engine/STL/OS/Base/BaseFileSystem.h"

#ifdef PLATFORM_BASE_POSIX

namespace GX_STL
{
namespace posix
{
#	include <dirent.h>
#	include <sys/vfs.h>
#	include <unistd.h>
#	include <sys/stat.h>
#	include <unistd.h>
#	include <stdio.h>
}
namespace OS
{

	
	//
	// Posix File System
	//

	struct PosixFileSystem : public Noninstancable
	{
	public:
		static bool DeleteFile (StringCRef filename);

		static bool SetCurrentDirectory (StringCRef dir);
		static bool GetCurrentDirectory (OUT String &dir);

		static bool IsFileExist (StringCRef filename);
		static bool IsDirectoryExist (StringCRef dirname);

		static bool NewDirectory (StringCRef dir);			// create directory in current directory
		//static bool CreateDirectories (StringCRef path);	// create directories for path

		static bool DeleteEmptyDirectory (StringCRef dir);
		static bool DeleteDirectory (StringCRef dir);
		/*
		static bool GetAllFilesInPath (StringCRef path, OUT Array<String> &fileNames);
		static bool GetAllDirsInPath (StringCRef path, OUT Array<String> &directories);
		
		static bool CopyFile (StringCRef fromFile, StringCRef toFile);
		static bool CopyDirectory (StringCRef fromDir, StringCRef toDir);*/
	};

	
	typedef BaseFileSystem< PosixFileSystem >		FileSystem;


/*
=================================================
	DeleteFile
=================================================
*/
	inline bool PosixFileSystem::DeleteFile (StringCRef filename)
	{
		ASSERT( filename.IsNullTerminated() );

		return posix::remove( filename.cstr() ) == 0;
	}
	
/*
=================================================
	SetCurrentDirectory
=================================================
*/
	inline bool PosixFileSystem::SetCurrentDirectory (StringCRef dir)
	{
		ASSERT( dir.IsNullTerminated() );

		return posix::chdir( dir.cstr() ) == 0;
	}
	
/*
=================================================
	GetCurrentDirectory
=================================================
*/
	inline bool PosixFileSystem::GetCurrentDirectory (OUT String &dir)
	{
		char	buf[ 0xFF ];
		const char * ptr = posix::getcwd( buf, CountOf(buf) );

		if ( ptr != null )
		{
			dir = ptr;
			return true;
		}
		return false;
	}
	
/*
=================================================
	IsFileExist
=================================================
*/
	inline bool PosixFileSystem::IsFileExist (StringCRef filename)
	{
		if ( filename.Empty() )
			return false;
		
		ASSERT( dir.IsNullTerminated() );

		return access( filename.cstr(), F_OK ) != -1;
	}
	
/*
=================================================
	IsDirectoryExist
=================================================
*/
	inline bool PosixFileSystem::IsDirectoryExist (StringCRef dirname)
	{
		if ( dirname.Empty() )
			return true;
		
		ASSERT( dirname.IsNullTerminated() );

		struct stat	st;
		return ( stat( dirname.cstr(), &st ) == 0 and S_ISDIR(st.st_mode) );
	}
	
/*
=================================================
	NewDirectory
=================================================
*/
	inline bool PosixFileSystem::NewDirectory (StringCRef dir)
	{
		if ( dir.Empty() )
			return true;
		
		ASSERT( dir.IsNullTerminated() );

		mode_t process_mask = umask(0);

		bool	res = mkdir( dir.cstr(), S_IRUSR | S_IWUSR ) == 0;

		umask(process_mask);

		return res;
	}
	
/*
=================================================
	DeleteEmptyDirectory
=================================================
*/
	inline bool PosixFileSystem::DeleteEmptyDirectory (StringCRef dir)
	{
		ASSERT( dir.IsNullTerminated() );
		ASSERT( IsDirectoryExist( dir ) );

		return ( rmdir( dir.cstr() ) == 0 );
	}
	
/*
=================================================
	DeleteDirectory
=================================================
*/
	inline bool PosixFileSystem::DeleteDirectory (StringCRef dir)
	{
		ASSERT( dir.IsNullTerminated() );
		ASSERT( IsDirectoryExist( dir ) );

		DIR *			dp = null;
		struct dirent *	ep = null;
		String			cur_dir;

		dp = opendir( dir.cstr() );

		if ( dp == null )
			return false;

		while ( (ep = readdir(dp)) != null )
		{
			StringCRef	name( ep->d_name );

			if ( name == "." or name == ".." )
				continue;

			cur_dir = dir;
			cur_dir << '/' << name;

			if ( ep->d_type == DT_DIR )
				DeleteDirectory( cur_dir );
			else
				unlink( cur_dir.cstr() );
		}

		closedir( dp );
		
		return DeleteEmptyDirectory( dir );
	}

	
}	// OS
}	// GX_STL

#endif	// PLATFORM_BASE_POSIX