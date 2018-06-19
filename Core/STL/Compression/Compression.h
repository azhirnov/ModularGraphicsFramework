// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Containers/ArrayRef.h"

namespace GX_STL
{
namespace GXCompression
{
	using namespace GX_STL::GXTypes;



	//
	// Compressor interface
	//

	class ICompressor
	{
	public:
		virtual ~ICompressor () {}
		virtual BytesU GetPrefferedSize (BytesU uncompressedSize) const = 0;
		virtual bool   Compress (BinArrayCRef src, INOUT BinArrayRef &dst) = 0;
	};



	//
	// Stream Compressor interface
	//

	class IStreamCompressor
	{
	};



	//
	// Decompressor interface
	//

	class IDecompressor
	{
	public:
		virtual ~IDecompressor () {}
		virtual BytesU GetPrefferedSize (BytesU compressedSize) const = 0;
		virtual bool   Decompress (BinArrayCRef src, INOUT BinArrayRef &dst) = 0;
	};



	//
	// Stream Decompressor interface
	//

	class IStreamDecompressor
	{
	};

	
}	// GXCompression
}	// GX_STL
