// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Common/Enums.h"
#include "Engine/Base/Common/CreateInfo.h"
#include "Engine/Base/Common/ModuleMessages.h"
#include "Engine/Base/Modules/IDs.h"

namespace Engine
{
namespace Base
{

	//
	// Stream Descriptor
	//

	struct StreamDescriptor
	{
		BytesUL					totalSize;		// can be 0 if total size is unknown
		BytesUL					available;		
		EMemoryAccess::bits		memoryFlags;
	};


}	// Base


namespace CreateInfo
{

	//
	// Stream Manager Create Info
	//
	struct StreamManager
	{
	};


	//
	// Stream Create Info
	//

	struct InStreamFromFile
	{
		File::RFilePtr					file;
	};

	struct OutStreamFromFile
	{
		File::WFilePtr					file;
	};

	struct InStreamFromUri
	{
		SingleRead< String >			uri;	// local file name or internet address
	};

	struct OutStreamFromUri
	{
		SingleRead< String >			uri;	// local file name or internet address
	};

	struct InStreamFromMemory
	{
		SingleRead< BinaryArray >		memory;
	};

	struct OutStreamToMemory
	{
		// TODO
	};

}	// CreateInfo


namespace ModuleMsg
{

	//
	// Get Stream Descriptor
	//
	struct GetStreamDescriptor
	{
		Out< Base::StreamDescriptor >		result;
	};


	//
	// Read CPU Visible Memory Data
	//
	struct ReadFromStream
	{
		BytesUL					offset;
		Optional< BytesUL >		size;
		Out< BinArrayCRef >		result;
	};


	//
	// Write Data to CPU Visible Memory
	//
	struct WriteToStream
	{
		BytesUL				offset;
		BinArrayCRef		data;
		Out< BytesUL >		wasWritten;
	};


	//
	// Data Region Changed Event
	//
	struct DataRegionChanged
	{
	// types
		using EMemoryAccess		= Base::EMemoryAccess;

	// variables
		EMemoryAccess::type		access	= EMemoryAccess::Unknown;
		BytesUL					offset;
		BytesUL					size;

	// methods
		DataRegionChanged (EMemoryAccess::type access, Bytes<uint> offset, Bytes<uint> size) :
			access(access), offset(BytesUL(offset)), size(BytesUL(size))
		{}

		DataRegionChanged (EMemoryAccess::type access, Bytes<ulong> offset, Bytes<ulong> size) :
			access(access), offset(offset), size(size)
		{}
	};


}	// ModuleMsg
}	// Engine
