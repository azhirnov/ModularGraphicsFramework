// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Types.h"
#include "Core/STL/CompileTime/NewTypeInfo.h"
#include "Core/STL/Dimensions/ByteAndBit.h"

namespace GX_STL
{
namespace GXTypes
{

	template <typename T>
	struct ArrayRef;

/*
=================================================
	CheckPointersAliasing
----
	Returns true if memory regions are intersected
=================================================
*/
	template <typename T0, typename T1>
	ND_ forceinline bool CheckPointersAliasing (const T0 *begin0, const T0 *end0, const T1 *begin1, const T1 *end1) noexcept
	{
		ASSERT( begin0 <= end0 );
		ASSERT( begin1 <= end1 );
		return ( begin0 < end1 ) & ( end0 > begin1 );
	}


	namespace UnsafeMem
	{
/*
=================================================
	MemCopy
----
	memory blocks must not intersects
=================================================
*/
		forceinline void * MemCopy (void *dst, const void *src, BytesU size) noexcept
		{
			// TODO: add checks
			ASSERT( not CheckPointersAliasing( Cast<ubyte *>(dst), Cast<ubyte *>(dst) + usize(size),
											   Cast<const ubyte *>(src), Cast<const ubyte *>(src) + usize(size) ) );

            return ::memcpy( dst, src, usize(size) );
		}
		
/*
=================================================
	MemMove
----
	memory blocks may intersects
=================================================
*/
		forceinline void * MemMove (void *dst, const void *src, BytesU size) noexcept
		{
			// TODO: add checks
            return ::memmove( dst, src, usize(size) );
		}

/*
=================================================
	MemCmp
=================================================
*/
		ND_ forceinline int MemCmp (const void *left, const void *right, BytesU size) noexcept
		{
			// TODO: add checks
            return ::memcmp( left, right, usize(size) );
		}

/*
=================================================
	ZeroMem
=================================================
*/
		forceinline void ZeroMem (void *dst, BytesU size) noexcept
		{
            ::memset( dst, 0, usize(size) );
		}

	}	// UnsafeMem
	

/*
=================================================
	AddressOf
=================================================
*/
	template <typename T>
	ND_ forceinline constexpr T*  AddressOf (T &ref)
	{
		return std::addressof( ref );
	}

/*
=================================================
	MemCopy
----
	memory blocks must not intersects
=================================================
*/
	template <typename T0, typename T1>
	inline void MemCopy (T0 &dst, const T1 &src) noexcept
	{
		STATIC_ASSERT( CompileTime::IsPOD<T0> );
		STATIC_ASSERT( CompileTime::IsPOD<T1> );
		STATIC_ASSERT( sizeof(dst) == sizeof(src) );

		UnsafeMem::MemCopy( AddressOf(dst), AddressOf(src), SizeOf<T0> );
	}
	
	template <typename T0, typename T1>
	inline void MemCopy (ArrayRef<T0> dst, ArrayRef<const T1> src);
	
/*
=================================================
	MemMove
----
	memory blocks may intersects
=================================================
*/
	template <typename T0, typename T1>
	inline void MemMove (T0 &dst, const T1 &src) noexcept
	{
		STATIC_ASSERT( CompileTime::IsPOD<T0> );
		STATIC_ASSERT( CompileTime::IsPOD<T1> );
		STATIC_ASSERT( sizeof(dst) == sizeof(src) );

		UnsafeMem::MemMove( AddressOf(dst), AddressOf(src), SizeOf<T0> );
	}

	template <typename T0, typename T1>
	inline void MemMove (ArrayRef<T0> dst, ArrayRef<const T1> src);
	
/*
=================================================
	MemCmp
=================================================
*/
	template <typename T0, typename T1>
	ND_ inline int  MemCmp (const T0 &left, const T1 &right) noexcept
	{
		STATIC_ASSERT( sizeof(left) == sizeof(right) );

		return UnsafeMem::MemCmp( AddressOf(left), AddressOf(right), SizeOf<T0> );
	}

	template <typename T0, typename T1>
	ND_ inline int  MemCmp (ArrayRef<const T0> left, ArrayRef<const T1> right);

/*
=================================================
	ZeroMem
=================================================
*/
	template <typename T, usize I>
	inline void ZeroMem (T (&arr)[I]) noexcept
	{
		STATIC_ASSERT( CompileTime::IsZeroMemAvailable<T> );
		UnsafeMem::ZeroMem( arr, I * SizeOf<T> );
	}

	template <typename T>
	inline void ZeroMem (T &val) noexcept
	{
		STATIC_ASSERT( CompileTime::IsZeroMemAvailable<T> );
		UnsafeMem::ZeroMem( AddressOf(val), SizeOf<T> );
	}

	template <typename T>
	inline void ZeroMem (ArrayRef<T> buf);

}	// GXTypes
}	// GX_STL

