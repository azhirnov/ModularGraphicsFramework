// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Common/BaseObject.h"

namespace Engine
{
namespace Base
{
	using RFilePtr	= GX_STL::File::RFilePtr;
	using WFilePtr	= GX_STL::File::WFilePtr;



	//
	// File System
	//

	class FileManager final : public BaseObject
	{
	// methods
	public:
		explicit
		FileManager (GlobalSystemsRef gs);
		~FileManager ();

		bool OpenForRead (StringCRef filename, OUT RFilePtr &file) const;
		bool OpenForWrite (StringCRef filename, OUT WFilePtr &file) const;
		bool OpenForAppend (StringCRef filename, OUT WFilePtr &file) const;
		bool CreateFile (StringCRef filename, OUT WFilePtr &file) const;

		//bool OpenForDecrypt (StringCRef filename, RFilePtr &file, StringCRef password) const;
		//bool CreateEncryptFile (StringCRef filename, WFilePtr &file, StringCRef password) const;

		bool CreateMemFile (OUT WFilePtr &file, BytesU reserve = BytesU()) const;
		bool SaveMemFile (StringCRef filename, const WFilePtr &file) const;
		bool ReadToMem (StringCRef filename, RFilePtr &file) const;
		bool ReadToMem (const RFilePtr &fromFile, OUT RFilePtr &toFile) const;
		bool ReadToMemIfSmall (const RFilePtr &fromFile, OUT RFilePtr &toFile) const;

		bool CopyFile (const RFilePtr &fromFile, const WFilePtr &toFile) const;
		bool CopyFile (StringCRef fromFilename, StringCRef toFilename) const;


		bool DeleteFile (StringCRef filename);

		bool FindAndSetCurrentDir (StringCRef dir, uint searchDepth = 4) const;
		bool SetCurrentDirectory (StringCRef dir) const;
		bool GetCurrentDirectory (OUT String &dir) const;

		bool OpenResource (StringCRef resname, OUT RFilePtr &file) const;

		bool IsFileExist (StringCRef filename) const;
		bool IsResourceExist (StringCRef filename) const;
		bool IsDirectoryExist (StringCRef dirname) const;

		bool NewDirectory (StringCRef dir) const;			// create directory in current directory
		bool CreateDirectories (StringCRef path) const;		// create directories for path

		bool DeleteEmptyDirectory (StringCRef dir) const;
		bool DeleteDirectory (StringCRef dir) const;

		ulong GetFileSize (StringCRef filename) const;
		ulong GetFreeSpace (StringCRef path) const;
		ulong GetTotalSpace (StringCRef path) const;
		
		bool SearchFile (StringCRef file, uint depth, OUT String &result) const;
		bool SearchFileForward (StringCRef file, uint depth, OUT String &result) const;
		bool SearchFileBackward (StringCRef file, uint depth, OUT String &result) const;

		bool SearchDir (StringCRef dir, uint depth, OUT String &result) const;
		bool SearchDirForward (StringCRef dir, uint depth, OUT String &result) const;
		bool SearchDirBackward (StringCRef dir, uint depth, OUT String &result) const;


		bool MakeUniqueName (StringCRef filename, OUT String &result) const;
	};


}	// Base
}	// Engine
