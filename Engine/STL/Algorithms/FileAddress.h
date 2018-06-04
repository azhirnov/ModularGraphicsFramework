// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Containers/String.h"
#include "Engine/STL/Containers/ArrayRef.h"
#include "Engine/STL/Containers/Array.h"
#include "Engine/STL/Containers/StaticArray.h"
#include "Engine/STL/Algorithms/ArrayUtils.h"
#include "Engine/STL/Math/Mathematics.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// File Address Utils
	//

	struct FileAddress final : Noninstancable
	{
	// methods
	public:
		ND_ static StringCRef	GetPath (StringCRef filename);
		ND_ static StringCRef	GetName (StringCRef filename);
		ND_ static StringCRef	GetExtension (StringCRef filename);
		ND_ static StringCRef	GetExtensions (StringCRef filename);
		ND_ static StringCRef	GetPathAndName (StringCRef filename);
		ND_ static StringCRef	GetNameAndExt (StringCRef filename);

		ND_ static StringCRef	ToShortPath (StringCRef filename, uint maxFolders = 1);

			static void		DividePath (StringCRef filename, OUT Array<StringCRef> &path);

			static void		FormatPath (INOUT String &filename);
			//static bool	IsNeedToFormatPath (StringCRef filename);	// TODO

			static void		RemoveName (INOUT String &filename);
			static void		RemoveExtension (INOUT String &filename);
			static bool		PathMoveBack (INOUT String &path);

			static bool		RemoveDirectoriesFromLeft (INOUT String &path, uint numDirectories);
			static bool		RemoveDirectoriesFromRight (INOUT String &path, uint numDirectories);

			static void		AddDirectoryToPath (INOUT String &path, StringCRef dir);
			static void		AddBaseDirectoryToPath (INOUT String &path, StringCRef dir);
			static void		AddNameToPath (INOUT String &path, StringCRef name);
			static void		AddExtensionToName (INOUT String &filename, StringCRef ext);

		ND_ static String	BuildPath (StringCRef path, StringCRef nameWithExt);
		ND_ static String	BuildPath (StringCRef path, StringCRef name, StringCRef ext);

			static bool		AbsoluteToRelativePath (StringCRef source, StringCRef base, OUT String &result);
			//static String		ReleativeToRelativePath (StringCRef path1, StringCRef path2);	// TODO: find first match in folder and build relative path

	private:
		static usize	_GetExtensionPos (StringCRef filename);
		static usize	_GetNamePos (StringCRef filename);
		static usize	_SafeInc (usize value, StringCRef str);
	};


}	// GXTypes
}	// GX_STL
