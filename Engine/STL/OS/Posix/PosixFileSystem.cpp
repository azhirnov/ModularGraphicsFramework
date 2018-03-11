// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/STL/Common/Platforms.h"

#ifdef PLATFORM_BASE_POSIX

#include "Engine/STL/OS/Posix/FileSystem.h"
#include "Engine/STL/Math/BinaryMath.h"
#include "Engine/STL/OS/Posix/PosixHeader.h"

namespace GX_STL
{
namespace OS
{

/*
=================================================
	DeleteFile
=================================================
*/
	bool PosixFileSystem::DeleteFile (StringCRef filename)
	{
		return ::remove( filename.cstr() ) == 0;
	}
	
/*
=================================================
	SetCurrentDirectory
=================================================
*/
	bool PosixFileSystem::SetCurrentDirectory (StringCRef dir)
	{
		return ::chdir( dir.cstr() ) == 0;
	}
	
/*
=================================================
	GetCurrentDirectory
=================================================
*/
	bool PosixFileSystem::GetCurrentDirectory (OUT String &dir)
	{
		char	buf[ 0xFF ];
		const char * ptr = ::getcwd( buf, CountOf(buf) );

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
	bool PosixFileSystem::IsFileExist (StringCRef filename)
	{
		if ( filename.Empty() )
			return false;

		return ::access( filename.cstr(), F_OK ) != -1;
	}
	
/*
=================================================
	IsDirectoryExist
=================================================
*/
	bool PosixFileSystem::IsDirectoryExist (StringCRef dirname)
	{
		if ( dirname.Empty() )
			return true;

		struct stat	st;
		return ( ::stat( dirname.cstr(), &st ) == 0 and S_ISDIR(st.st_mode) );
	}
	
/*
=================================================
	NewDirectory
=================================================
*/
	bool PosixFileSystem::NewDirectory (StringCRef dir)
	{
		if ( dir.Empty() )
			return true;

		mode_t process_mask = ::umask(0);

		bool	res = ::mkdir( dir.cstr(), S_IRUSR | S_IWUSR ) == 0;

		::umask(process_mask);

		return res;
	}
	
/*
=================================================
	DeleteEmptyDirectory
=================================================
*/
	bool PosixFileSystem::DeleteEmptyDirectory (StringCRef dir)
	{
		ASSERT( IsDirectoryExist( dir ) );

		return ( ::rmdir( dir.cstr() ) == 0 );
	}
	
/*
=================================================
	DeleteDirectory
=================================================
*/
	bool PosixFileSystem::DeleteDirectory (StringCRef dir)
	{
		ASSERT( IsDirectoryExist( dir ) );

		DIR *			dp = null;
		struct dirent *	ep = null;
		String			cur_dir;

		dp = ::opendir( dir.cstr() );

		if ( dp == null )
			return false;

		while ( (ep = ::readdir(dp)) != null )
		{
			StringCRef	name( ep->d_name );

			if ( name == "." or name == ".." )
				continue;

			cur_dir = dir;
			cur_dir << '/' << name;

			if ( ep->d_type == DT_DIR )
				DeleteDirectory( cur_dir );
			else
				::unlink( cur_dir.cstr() );
		}

		::closedir( dp );
		
		return DeleteEmptyDirectory( dir );
	}
	
/*
=================================================
	GetAllFilesInPath
=================================================
*/
	bool PosixFileSystem::GetAllFilesInPath (StringCRef path, OUT Array<String> &fileNames)
	{
		TODO("GetAllFilesInPath");
		return false;
	}
	
/*
=================================================
	GetAllDirsInPath
=================================================
*/
	bool PosixFileSystem::GetAllDirsInPath (StringCRef path, OUT Array<String> &directories)
	{
		TODO("GetAllDirsInPath");
		return false;
	}
	
/*
=================================================
	CopyFile
=================================================
*/
	bool PosixFileSystem::CopyFile (StringCRef fromFile, StringCRef toFile)
	{
		TODO("CopyFile");
		return false;
	}

/*
=================================================
	CopyDirectory
=================================================
*/
	bool PosixFileSystem::CopyDirectory (StringCRef fromDir, StringCRef toDir)
	{
		TODO("CopyDirectory");
		return false;
	}
	
/*
=================================================
	MoveFile
=================================================
*/
	bool PosixFileSystem::MoveFile (StringCRef oldName, StringCRef newName, bool async)
	{
		TODO("MoveFile");
		return false;
	}
	
/*
=================================================
	GetFileLastModificationTime
=================================================
*/
	Date PosixFileSystem::GetFileLastModificationTime (StringCRef filename)
	{
		TODO("GetFileLastModificationTime");
		return Uninitialized;
	}

/*
=================================================
	GetFileCreationTime
=================================================
*/
	Date PosixFileSystem::GetFileCreationTime (StringCRef filename)
	{
		TODO("GetFileCreationTime");
		return Uninitialized;
	}

/*
=================================================
	GetFileSize
=================================================
*/
	BytesUL PosixFileSystem::GetFileSize (StringCRef filename)
	{
		TODO("GetFileSize");
		return Uninitialized;
	}

}	// OS
}	// GX_STL

#endif	// PLATFORM_BASE_POSIX
