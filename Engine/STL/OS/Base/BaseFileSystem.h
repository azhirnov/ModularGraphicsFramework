// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Common.h"
#include "Engine/STL/Algorithms/StringUtils.h"

#include "Engine/STL/OS/Windows/FileSystem.h"

namespace GX_STL
{
namespace OS
{
	
	//
	// Base File System
	//

	struct FileSystem : OSFileSystem
	{
	// methods
	public:
		static bool SearchFile (StringCRef file, uint depth, OUT String &result);
		static bool SearchFileForward (StringCRef file, uint depth, OUT String &result);
		static bool SearchFileBackward (StringCRef file, uint depth, OUT String &result);

		static bool SearchDir (StringCRef dir, uint depth, OUT String &result);
		static bool SearchDirForward (StringCRef dir, uint depth, OUT String &result);
		static bool SearchDirBackward (StringCRef dir, uint depth, OUT String &result);
		
		static bool CreateDirectories (StringCRef path);	// create directories for path
		static bool FindAndSetCurrentDir (StringCRef dirname, uint searchDepth);
		
		template <typename Filter>
		static void RecursiveFindFiles (StringCRef path, Filter filter, uint depth, INOUT Array<String> &fileNames);

	private:
		template <typename B>
		static bool _Search (StringCRef name, uint depth, OUT String &result);
		
		template <typename B>
		static bool _SearchForward (StringCRef dir, StringCRef name, uint depth, OUT String &result);
		
		template <typename B>
		static bool _SearchBackward (StringCRef name, uint depth, OUT String &result);


	// types
	private:
		struct TFileSearch {
			static bool IsExist (StringCRef name) {
				return OSFileSystem::IsFileExist( name );
			}
		};

		struct TDirSearch {
			static bool IsExist (StringCRef name) {
				return OSFileSystem::IsDirectoryExist( name );
			}
		};
	};
	
	
/*
=================================================
	RecursiveFindFiles
=================================================
*/
	template <typename Filter>
	inline void FileSystem::RecursiveFindFiles (StringCRef path, Filter filter, uint depth, INOUT Array<String> &fileNames)
	{
		if ( depth == 0 )
			return;

		String			tmp;
		Array<String>	files;
		CHECK( GetAllFilesInPath( path, OUT files ) );

		// search files in current path
		FOR( i, files )
		{
			if ( filter( files[i] ) )
			{
				tmp = path;
				FileAddress::AddNameToPath( INOUT tmp, files[i] );

				fileNames.PushBack( tmp );
			}
		}

		Array<String>&	dirs = files;
		CHECK( GetAllDirsInPath( path, OUT dirs ) );

		// recursive search
		FOR( i, dirs )
		{
			tmp = path;
			FileAddress::AddDirectoryToPath( INOUT tmp, dirs[i] );

			RecursiveFindFiles( tmp, filter, depth-1, INOUT fileNames );
		}
	}


}	// OS
}	// GX_STL
