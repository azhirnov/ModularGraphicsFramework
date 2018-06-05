// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Compression/Compression.h"

#ifdef GX_ENABLE_MINIZ

#include "miniz/miniz.h"

namespace GX_STL
{
namespace GXCompression
{

	//
	// MiniZ Compressor
	//

	class MiniZCompressor : public ICompressor
	{
	public:
		BytesU GetPrefferedSize (BytesU uncompressedSize) const
		{
			return BytesU(compressBound( mz_ulong(usize(uncompressedSize)) ));
		}

		bool Compress (BinArrayCRef src, INOUT BinArrayRef &dst)
		{
			mz_ulong	dst_size = mz_ulong(usize(dst.Size()));

			if ( mz_compress2( OUT dst.RawPtr(), INOUT &dst_size, src.RawPtr(), mz_ulong(usize(src.Size())), MZ_BEST_COMPRESSION ) != MZ_OK ) {
				dst = BinArrayRef();
				return false;
			}

			dst = dst.SubArray( 0, dst_size );
			return true;
		}
	};



	//
	// MiniZ Decompressor
	//

	class MiniZDecompressor : public IDecompressor
	{
	public:
		BytesU GetPrefferedSize (BytesU compressedSize) const
		{
			return compressedSize * 3;
		}

		bool Decompress (BinArrayCRef src, INOUT BinArrayRef &dst)
		{
			mz_ulong	dst_size = mz_ulong(usize(dst.Size()));

			if ( uncompress( OUT dst.RawPtr(), INOUT &dst_size, src.RawPtr(), mz_ulong(usize(src.Size())) ) != MZ_OK )
			{
				dst = BinArrayRef();
				return false;
			}

			dst = dst.SubArray( 0, dst_size );
			return true;
		}
	};

	
}	// GXCompression
}	// GX_STL

#endif	// GX_ENABLE_MINIZ
