// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Compression/Compression.h"

#ifdef GX_ENABLE_LZ4

#include "lz4/lib/lz4.h"

namespace GX_STL
{
namespace GXCompression
{

	//
	// LZ4 Compressor
	//

	class LZ4Compressor : public ICompressor
	{
	public:
		BytesU GetPrefferedSize (BytesU uncompressedSize) const
		{
			return BytesU(LZ4_compressBound( int(uncompressedSize) ));
		}

		bool Compress (BinArrayCRef src, INOUT BinArrayRef &dst)
		{
			const int	size = LZ4_compress_default( Cast<const char *>(src.RawPtr()), OUT Cast<char *>(dst.RawPtr()), int(src.Size()), int(dst.Size()) );

			if ( size < 0 ) {
				dst = BinArrayRef();
				return false;
			}

			dst = dst.SubArray( 0, size );
			return true;
		}
	};



	//
	// LZ4 Decompressor
	//

	class LZ4Decompressor : public IDecompressor
	{
	public:
		BytesU GetPrefferedSize (BytesU compressedSize) const
		{
			return compressedSize * 3;
		}

		bool Decompress (BinArrayCRef src, INOUT BinArrayRef &dst)
		{
			const int	size = LZ4_decompress_safe( Cast<const char *>(src.RawPtr()), OUT Cast<char *>(dst.RawPtr()), int(src.Size()), int(dst.Size()) );
			
			if ( size < 0 ) {
				dst = BinArrayRef();
				return false;
			}

			dst = dst.SubArray( 0, size );
			return true;
		}
	};

	
}	// GXCompression
}	// GX_STL

#endif	// GX_ENABLE_LZ4
