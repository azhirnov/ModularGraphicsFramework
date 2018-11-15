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
	struct OpenFileForRead : _MsgBase_
	{
	// variables
		StringCRef					filename;
		Out< GXFile::RFilePtr >		result;

	// methods
		explicit OpenFileForRead (StringCRef filename) : filename{filename} {}
	};
	

	struct OpenFileForWrite : _MsgBase_
	{
	// variables
		StringCRef					filename;
		Out< GXFile::WFilePtr >		result;

	// methods
		explicit OpenFileForWrite (StringCRef filename) : filename{filename} {}
	};


}	// DSMsg
}	// Engine
