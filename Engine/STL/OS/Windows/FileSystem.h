// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "OSWindows.h"
#include "PlatformUtils.h"
#include "Engine/STL/Algorithms/FileAddress.h"

#ifdef PLATFORM_WINDOWS

namespace GX_STL
{
namespace OS
{

	//
	// Windows File System
	//

	struct OSFileSystem
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
		
		static bool GetAllFilesInPath (StringCRef path, OUT Array<String> &fileNames);
		static bool GetAllDirsInPath (StringCRef path, OUT Array<String> &directories);
		
		static bool CopyFile (StringCRef fromFile, StringCRef toFile);
		static bool CopyDirectory (StringCRef fromDir, StringCRef toDir);

		// TODO: MoveFile
		// TODO: MoveDirectory

		static bool MoveFile (StringCRef oldName, StringCRef newName, bool async = false);

		static ulong GetFileLastModificationTime (StringCRef filename);
		static ulong GetFileCreationTime (StringCRef filename);
		static ulong GetFileSize (StringCRef filename);
	};

	
}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS
