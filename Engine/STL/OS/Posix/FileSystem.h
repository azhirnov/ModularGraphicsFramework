// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Platforms.h"

#ifdef PLATFORM_BASE_POSIX

#include "Engine/STL/OS/Posix/OSPosix.h"
#include "Engine/STL/OS/Base/Date.h"

namespace GX_STL
{
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
		static bool IsDirectoryExist (StringCRef folder);
		static bool IsAbsolutePath (StringCRef path);

		static bool NewDirectory (StringCRef dir);			// create directory in current directory
		//static bool CreateDirectories (StringCRef path);	// create directories for path

		static bool DeleteEmptyDirectory (StringCRef dir);
		static bool DeleteDirectory (StringCRef dir);
		
		static bool GetAllFilesInPath (StringCRef path, OUT Array<String> &fileNames);
		static bool GetAllDirsInPath (StringCRef path, OUT Array<String> &directories);
		
		static bool CopyFile (StringCRef fromFile, StringCRef toFile);
		static bool CopyDirectory (StringCRef fromDir, StringCRef toDir);

		// TODO: MoveFile
		// TODO: MoveDirectory

		static bool MoveFile (StringCRef oldName, StringCRef newName, bool async = false);

		static Date GetFileLastModificationTime (StringCRef filename);
		static Date GetFileCreationTime (StringCRef filename);
		static BytesUL GetFileSize (StringCRef filename);
	};

	using OSFileSystem = PosixFileSystem;

	
}	// OS
}	// GX_STL

#endif	// PLATFORM_BASE_POSIX