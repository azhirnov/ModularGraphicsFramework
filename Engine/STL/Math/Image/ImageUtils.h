// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Math/Color/ColorFormats.h"
#include "Engine/STL/Memory/MemPointer.h"

namespace GX_STL
{
namespace GXMath
{

struct GXImageUtils : public Noninstancable
{
	
/*
=================================================
	ResCorrectionAspect
----
	resolution correction for normalized coords
=================================================
*/
	template <typename T>
	ND_ static inline Vec<T,2>  ResCorrectionAspect (const Vec<T,2> &dim)
	{
		// proportional_coords = normalized_coords * aspect
		return	dim.x < dim.y ?
				Vec<T,2>( T(1), dim.x / dim.y ) :
				Vec<T,2>( dim.y / dim.x, T(1) );
	}

/*
=================================================
	AlignDimension
=================================================
*/
	template <typename T, typename B, usize I, ulong U>
	ND_ static inline Vec<T,I,U>  AlignDimension (const Vec<T,I,U> &size, const B& align)
	{
		STATIC_ASSERT( CompileTime::IsInteger<T> );
		ASSERT( IsPowerOfTwo( align ) and "align must be a power of 2" );

		Vec<T,I,U>	res;
		uint		align_po2 = IntLog2( align );

		FOR( i, res ) {
			res[i] = ( size[i] + (align-1) ) >> align_po2;
		}
		return res;
	}
	
/*
=================================================
	GetNumberOfMipmaps
----
	returns the number of mipmaps for current dimension,
	to calculate full number of images add 1 (top level)
=================================================
*/
	template <typename T>
	ND_ static inline uint  GetNumberOfMipmaps (const T len)
	{
		return IntLog2( len );
	}

/*
=================================================
	GetFrameCoord
=================================================
*/
	template <typename T, typename B>
	static inline void GetFrameCoord (OUT Rectangle<T> &sTexcoord, uint uFrameIdx, const Vec<B,2> &vDim,
									  const Vec<B,2> &vNumFrames, const Vec<B,2> &border = Vec<B,2>())
	{
		STATIC_ASSERT( CompileTime::IsInteger<B> );

		ASSERT( uFrameIdx < vNumFrames.Area() );

		const Vec<B,2>	v_frame_size	= vDim / vNumFrames;
		const Vec<B,2>	v_frame_pos		= Vec<B,2>( uFrameIdx % vNumFrames.x, uFrameIdx / vNumFrames.x );
		Rectangle<B>	s_frame_coord	= Rectangle<B>( Vec<B,2>(0), v_frame_size ) + (v_frame_pos * v_frame_size);

		s_frame_coord.LeftBottom() += border;
		s_frame_coord.RightTop()   -= border;

		sTexcoord = s_frame_coord.template To< Rectangle<T> >() / vDim.template To< Vec<T,2> >();
	}

/*
=================================================
	AlignedDataSize
=================================================
*/
	template <typename T, typename B>
	ND_ static inline Bytes<B>  AlignedDataSize (const Vec<T,3> &dimension, Bytes<B> bytePerPixel, Bytes<B> rowAlign, Bytes<B> sliceAlign)
	{
		using S = CompileTime::GenType<T, typename Bytes<B>::Value_t>;
		using S3 = Vec<S,3>;
		
		STATIC_ASSERT( CompileTime::IsInteger<S> );

		const S3	dim			= S3(Max( dimension, Vec<T,3>(1) ));
		const S		row_size	= (S) AlignedRowSize( dim.x, bytePerPixel, rowAlign );
		const S		slice_size	= (S) AlignedRowSize( dim.y * row_size, Bytes<B>(1), sliceAlign );
		const S		size		= slice_size * dim.z;
		return Bytes<B>( size );
	}

	template <typename ColorType, typename T, typename B>
	ND_ static inline Bytes<B> AlignedDataSize (const Vec<T,3> &dimension, Bytes<B> rowAlign, Bytes<B> sliceAlign)
	{
		return AlignedDataSize( dimension, Bytes<B>::template SizeOf<ColorType>(), rowAlign, sliceAlign );
	}
	
/*
=================================================
	AlignedRowSize
=================================================
*/
	template <typename T, typename B>
	ND_ static inline Bytes<B>  AlignedRowSize (const T rowPixels, Bytes<B> bytePerPixel, Bytes<B> rowAlign)
	{
		using S = CompileTime::GenType<T, typename Bytes<B>::Value_t>;

		STATIC_ASSERT( CompileTime::IsInteger<S> and CompileTime::IsScalar<T> );

		return (Bytes<B>) GXMath::AlignToLarge( Max( S(rowPixels), S(1) ) * S(bytePerPixel), rowAlign );
	}
	
/*
=================================================
	AlignedRowSize
=================================================
*/
	template <typename T, typename B>
	ND_ static inline Bytes<B>  AlignedRowSize (const T rowPixels, Bits<B> bitPerPixel, Bytes<B> rowAlign)
	{
		using S = CompileTime::GenType<T, typename Bytes<B>::Value_t>;

		STATIC_ASSERT( CompileTime::IsInteger<S> and CompileTime::IsScalar<T> );

		return (Bytes<B>) GXMath::AlignToLarge( Max( (S(rowPixels) * S(bitPerPixel)) >> 3, S(1) ), rowAlign );
	}
	
/*
=================================================
	AlignedSliceSize
=================================================
*/
	template <typename T, typename B>
	ND_ static inline Bytes<B>  AlignedSliceSize (const Vec<T,2> dim, Bytes<B> bytePerPixel, Bytes<B> rowAlign, Bytes<B> sliceAlign)
	{
		using S = CompileTime::GenType<T, typename Bytes<B>::Value_t>;

		const S	row_size	= (S) AlignedRowSize( dim.x, bytePerPixel, rowAlign );
		const S slice_size	= (S) AlignedRowSize( dim.y * row_size, Bytes<B>(1), sliceAlign );
		return Bytes<B>( slice_size );
	}

/*
=================================================
	GetPixelOffset
=================================================
*/
	template <typename T, typename B>
	ND_ static inline Bytes<B>  GetPixelOffset (const Vec<T,3> &coord, const Vec<T,3> &dimension, Bytes<B> bytePerPixel,
													 Bytes<B> rowAlign = Bytes<B>(1), Bytes<B> sliceAlign = Bytes<B>(1))
	{
		using S = CompileTime::GenType<T, typename Bytes<B>::Value_t>;
		using S3 = Vec<S,3>;
		
		STATIC_ASSERT( CompileTime::IsInteger<S> );

		const S3		dim			= S3(Max( dimension, Vec<T,3>(1) ));
		const S			row_size	= (S) AlignedRowSize( dim.x, bytePerPixel, rowAlign );
		const S			slice_size	= (S) AlignedRowSize( dim.y * row_size, Bytes<B>(1), sliceAlign );
		const S3		point		= S3(Clamp( S3(coord), S3(0), dim-1 ));
		const S			z_off		= slice_size * point.z;
		const S			y_off		= z_off + row_size * point.y;
		const Bytes<B>	i			= y_off + bytePerPixel * point.x;
		return i;
	}

/*
=================================================
	Copy
=================================================
*/
	template <typename T, typename SrcType, typename DstType, typename B>
	static inline bool Copy (const Vec<T,3> &dimension, const SrcType *srcImage, DstType *dstImage,
							 Bytes<B> srcRowAlignInBytes, Bytes<B> dstRowAlignInBytes)
	{
		return CopyPart( dimension, Vec<T,3>(0), srcImage, dimension, Vec<T,3>(0), dstImage, srcRowAlignInBytes, dstRowAlignInBytes );
	}

/*
=================================================
	CopyPart
=================================================
*/
	template <typename T, typename SrcType, typename DstType, typename B>
	static bool CopyPart (const Vec<T,3> &srcDim, const Vec<T,3> &srcOff, const SrcType *srcImage,
							const Vec<T,3> &dstDim, const Vec<T,3> &dstOff, DstType *dstImage,
							Bytes<B> srcRowAlignInBytes, Bytes<B> dstRowAlignInBytes)
	{
		CHECK_ERR( srcImage != null and dstImage != null );
		
		using S = CompileTime::GenType<T, typename Bytes<B>::Value_t>;
		using S3 = Vec<S,3>;
		
		STATIC_ASSERT( CompileTime::IsInteger<S> );

		const S3		src_dim	= S3(Max( srcDim, Vec<T,3>(1) ));
		const S3		src_off = S3(srcOff);
		const S3		dst_dim	= S3(Max( dstDim, Vec<T,3>(1) ));
		const S3		dst_off	= S3(dstOff);
		const S			src_row	= (S) AlignedRowSize( src_dim.x, Bytes<B>::template SizeOf<SrcType>(), srcRowAlignInBytes );
		const S			dst_row	= (S) AlignedRowSize( dst_dim.x, Bytes<B>::template SizeOf<DstType>(), dstRowAlignInBytes );
		const void *	src		= (const void *) srcImage;
		void *			dst		= (void *) dstImage;

		const S3		src_size = src_dim - src_off;
		const S3		dst_size = dst_dim - dst_off;
			
		//CHECK_ERR( All( src_size == dst_size ) );
		CHECK_ERR( All( src_off <= src_dim ) );
		CHECK_ERR( All( dst_off <= dst_dim ) );

		for (S z = 0; z < dst_size.z and z < src_size.z; ++z)
		{
			const S		src_z_off = src_row * src_dim.y * (z + src_off.z);
			const S		dst_z_off = dst_row * dst_dim.y * (z + dst_off.z);

			for (S y = 0; y < dst_size.y and y < src_size.y; ++y)
			{
				const S		src_y_off = src_z_off + src_row * (y + src_off.y);
				const S		dst_y_off = dst_z_off + dst_row * (y + dst_off.y);

				for (S x = 0; x < dst_size.x and x < src_size.x; ++x)
				{
					const Bytes<S>	i = Bytes<S>( src_y_off + sizeof(SrcType) * (x + src_off.x) );
					const Bytes<S>	j = Bytes<S>( dst_y_off + sizeof(DstType) * (x + dst_off.x) );

					const SrcType *	s = UnsafeMem::MovePointer< SrcType >( src, i );
					DstType *		d = UnsafeMem::MovePointer< DstType >( dst, j );
						
					ColorFormatUtils::ColorFormatConverter::Convert( *d, *s );
				}
			}
		}
		return true;
	}
		
/*
=================================================
	Fill
=================================================
*/
	template <typename T, typename SrcType, typename DstType>
	static bool Fill (const Vec<T,3> &regionDim, const SrcType &color,
						const Vec<T,3> &dstOff, const Vec<T,3> &dstDim, DstType *dstImage,
						BytesU dstRowAlignInBytes = 1_b)
	{
		CHECK_ERR( dstImage != null );

		const uint3		src_dim	= Max( regionDim, Vec<T,3>(1) ).template To<uint3>();
		const uint3		dst_dim	= Max( dstDim, Vec<T,3>(1) ).template To<uint3>();
		const uint3		dst_off	= dstOff.template To<uint3>();
		const BytesU	dst_row	= AlignedRowSize( dst_dim.x, BytesU::SizeOf<DstType>(), dstRowAlignInBytes );
		void *			dst		= (void *) dstImage;
			
		CHECK_ERR( All( src_dim <= dst_dim - dst_off ) );

		for (uint z = 0; z < src_dim.z; ++z)
		{
			const BytesU	dst_z_off = dst_row * dst_dim.y * (z + dst_off.z);

			for (uint y = 0; y < src_dim.y; ++y)
			{
				const BytesU	dst_y_off = dst_z_off + dst_row * (y + dst_off.y);

				for (uint x = 0; x < src_dim.x; ++x)
				{
					const BytesU	j = dst_y_off + SizeOf<DstType> * (x + dst_off.x);
					DstType *		d = UnsafeMem::MovePointer< DstType >( dst, j );
						
					ColorFormatUtils::ColorFormatConverter::Convert( *d, color );
				}
			}
		}
		return true;
	}
		
/*
=================================================
	ForEach
=================================================
*/
	template <typename Iterator, typename T, typename ColorType>
	static bool ForEach (Iterator &iter, const Vec<T,3> &regionDim, ColorType *image,
							const Vec<T,3> &offset, const Vec<T,3> &dim,
							BytesU dstRowAlignInBytes = 1_b)
	{
		CHECK_ERR( image != null );

		const uint3		src_dim	= Max( regionDim, Vec<T,3>(1) ).template To<uint3>();
		const uint3		dst_dim	= Max( dim, Vec<T,3>(1) ).template To<uint3>();
		const uint3		dst_off	= offset.template To<uint3>();
		const BytesU	dst_row	= AlignedRowSize( dst_dim.x, BytesU::SizeOf<ColorType>(), dstRowAlignInBytes );
		void *			dst		= (void *) image;
			
		CHECK_ERR( All( src_dim <= dst_dim - dst_off ) );

		for (uint z = 0; z < src_dim.z; ++z)
		{
			const BytesU	dst_z_off = dst_row * dst_dim.y * (z + dst_off.z);

			for (uint y = 0; y < src_dim.y; ++y)
			{
				const BytesU	dst_y_off = dst_z_off + dst_row * (y + dst_off.y);

				for (uint x = 0; x < src_dim.x; ++x)
				{
					const BytesU	j = dst_y_off + SizeOf<ColorType> * (x + dst_off.x);
					ColorType *		d = UnsafeMem::MovePointer< ColorType >( dst, j );
						
					iter( *d, uint3(x,y,z) );
				}
			}
		}
		return true;
	}

/*
=================================================
	GetPixel
=================================================
*/
	template <typename T, typename ColorType>
	ND_ inline static ColorType &  GetPixel (const Vec<T,3> &imageDim, ColorType *image, const Vec<T,3> &pixelCoord, uint rowAlignInBytes = 1)
	{
		ASSUME( image != null );
			
		const uint3		dim		= Max( imageDim, Vec<T,3>(1) ).template To<uint3>();
		const uint3		coord	= pixelCoord.template To<uint3>();
		const BytesU	row		= AlignedRowSize( dim.x, BytesU::SizeOf<ColorType>(), rowAlignInBytes );
		const BytesU	i		= row * (dim.y * coord.z + coord.y) + coord.x * sizeof(ColorType);
			
		ASSERT( All( coord < dim ) );

		return *UnsafeMem::MovePointer< ColorType *>( image, i );
	}
	

};	// GXImageUtils
}	// GXMath
}	// GX_STL
