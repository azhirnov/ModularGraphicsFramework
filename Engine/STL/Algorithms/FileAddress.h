// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Containers/String.h"
#include "Engine/STL/Containers/ArrayRef.h"
#include "Engine/STL/Containers/Array.h"
#include "Engine/STL/Containers/StaticArray.h"
#include "Engine/STL/Math/Mathematics.h"
#include "Engine/STL/Math/Vec.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// File Address Utils
	//

	// TODO: rename to FileAddress
	struct FileAddress
	{
	// methods
	public:
		static StringCRef	GetPath (StringCRef filename);
		static StringCRef	GetName (StringCRef filename);
		static StringCRef	GetExtension (StringCRef filename);
		static StringCRef	GetNameAndExt (StringCRef filename);

		static StringCRef	ToShortPath (StringCRef filename, uint maxFolders = 1);

		static void		DividePath (StringCRef filename, OUT Array<StringCRef> &path);

		static void		FormatPath (INOUT String &filename);
		//static bool		IsNeedToFormatPath (StringCRef filename);	// TODO

		static void		RemoveName (INOUT String &filename);
		static void		RemoveExtension (INOUT String &filename);
		static bool		PathMoveBack (INOUT String &path);

		static bool		RemoveDirectoriesFromLeft (INOUT String &path, uint numDirectories);
		static bool		RemoveDirectoriesFromRight (INOUT String &path, uint numDirectories);

		static void		AddDirectoryToPath (INOUT String &path, StringCRef dir);
		static void		AddBaseDirectoryToPath (INOUT String &path, StringCRef dir);
		static void		AddNameToPath (INOUT String &path, StringCRef name);
		static void		AddExtensionToName (INOUT String &filename, StringCRef ext);

		static String &	BuildPath (OUT String &result, StringCRef path, StringCRef nameWithExt);
		static String &	BuildPath (OUT String &result, StringCRef path, StringCRef name, StringCRef ext);

	private:
		static usize	_GetExtensionPos (StringCRef filename);
		static usize	_GetNamePos (StringCRef filename);
		static usize	_SafeInc (usize value, StringCRef str);
	};


}	// GXTypes
}	// GX_STL
