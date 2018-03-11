// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Platforms.h"

#ifdef PLATFORM_WINDOWS

#include "Engine/STL/OS/Windows/OSWindows.h"
#include "Engine/STL/OS/Windows/PlatformUtils.h"
#include "Engine/STL/OS/Base/Date.h"

namespace GX_STL
{
namespace OS
{

	//
	// Windows File System
	//

	struct _STL_EXPORT_ WindowsFileSystem : public Noninstancable
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

		static Date GetFileLastModificationTime (StringCRef filename);
		static Date GetFileCreationTime (StringCRef filename);
		static BytesUL GetFileSize (StringCRef filename);
	};
	
	using OSFileSystem = WindowsFileSystem;
	
}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS
