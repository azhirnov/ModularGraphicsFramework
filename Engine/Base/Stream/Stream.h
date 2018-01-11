// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Common/IDs.h"
#include "Engine/Base/Common/Enums.h"
#include "Engine/Base/Modules/ModuleMessages.h"
#include "Engine/Base/Modules/CreateInfo.h"

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
		File::RFilePtr				file;
	};

	struct OutStreamFromFile
	{
		File::WFilePtr				file;
	};

	struct InStreamFromUri
	{
		ReadOnce< String >			uri;	// local file name or internet address
	};

	struct OutStreamFromUri
	{
		ReadOnce< String >			uri;	// local file name or internet address
	};

	struct InStreamFromMemory
	{
		ReadOnce< BinaryArray >		memory;
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
	// variables
		BytesUL				offset;
		BinArrayCRef		data;
		Out< BytesUL >		wasWritten;

	// methods
		WriteToStream () {}
		explicit WriteToStream (BinArrayCRef data, Bytes<uint> offset = Uninitialized) : offset(offset), data(data) {}
		explicit WriteToStream (BinArrayCRef data, Bytes<ulong> offset) : offset(offset), data(data) {}
		template <typename T>			explicit WriteToStream (ArrayCRef<T> arr, BytesU offset = Uninitialized) : offset(offset), data(BinArrayCRef::From(arr)) {}
		template <typename B, usize I>	explicit WriteToStream (const B (&arr)[I], BytesU offset = Uninitialized) : offset(offset), data(BinArrayCRef::From(arr)) {}
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
