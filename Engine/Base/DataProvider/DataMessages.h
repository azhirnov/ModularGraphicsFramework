// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Public/DataProvider.h"

namespace Engine
{
namespace DSMsg
{

	//
	// Open File
	//
	struct OpenFileForRead
	{
	// variables
		StringCRef					filename;
		Out< GXFile::RFilePtr >		result;

	// methods
		explicit OpenFileForRead (StringCRef filename) : filename{filename} {}
	};
	
	struct OpenFileForWrite
	{
	// variables
		StringCRef					filename;
		Out< GXFile::WFilePtr >		result;

	// methods
		explicit OpenFileForWrite (StringCRef filename) : filename{filename} {}
	};


}	// DSMsg
}	// Engine
