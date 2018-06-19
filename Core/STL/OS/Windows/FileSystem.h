// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Platforms.h"

#ifdef PLATFORM_WINDOWS

#include "Core/STL/OS/Windows/OSWindows.h"
#include "Core/STL/OS/Windows/PlatformUtils.h"
#include "Core/STL/OS/Base/Date.h"

namespace GX_STL
{
namespace OS
{

	//
	// Windows File System
	//

	struct WindowsFileSystem : public Noninstancable
	{
	public:
		static bool DeleteFile (StringCRef filename);

		static bool SetCurrentDirectory (StringCRef dir);
		static bool GetCurrentDirectory (OUT String &dir);

		ND_ static bool IsFileExist (StringCRef filename);
		ND_ static bool IsDirectoryExist (StringCRef folder);
		ND_ static bool IsAbsolutePath (StringCRef path);
		ND_ static bool IsReadOnly (StringCRef path);			// directory or file

		static bool NewDirectory (StringCRef dir);			// create directory in current directory
		//static bool CreateDirectories (StringCRef path);	// create directories for path

		static bool DeleteEmptyDirectory (StringCRef dir);
		static bool DeleteDirectory (StringCRef dir);
		
		static bool GetAllFilesInPath (StringCRef path, OUT Array<String> &fileNames);
		static bool GetAllDirsInPath (StringCRef path, OUT Array<String> &directories);
		
		static bool CopyFile (StringCRef fromFile, StringCRef toFile);
		static bool CopyDirectory (StringCRef fromDir, StringCRef toDir);

		// TODO: MoveDirectory
		static bool MoveFile (StringCRef oldName, StringCRef newName, bool async = false);

		ND_ static Date GetFileLastModificationTime (StringCRef filename);
		ND_ static Date GetFileCreationTime (StringCRef filename);
		ND_ static BytesUL GetFileSize (StringCRef filename);
	};
	
	using OSFileSystem = WindowsFileSystem;
	
}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS
