// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Common/Init.h"
#include "Uninitialized.h"

namespace GX_STL
{
namespace GXTypes
{

	// integer types
	typedef	wchar_t				wchar;
	typedef signed char			byte;
	//typedef signed short		short;
	//typedef signed int		int;
	typedef	signed long long	ilong;

	typedef unsigned char		ubyte;
	typedef	unsigned short		ushort;
	typedef unsigned int		uint;
	typedef unsigned long long	ulong;


	// pointer & size type
#if PLATFORM_BITS == 16
	typedef unsigned short		usize;
	typedef signed short		isize;
#elif PLATFORM_BITS == 32
	typedef unsigned int		usize;
	typedef signed int			isize;
#elif PLATFORM_BITS == 64
	typedef unsigned long long	usize;
	typedef signed long long	isize;
#endif

	// check size types
	STATIC_ASSERT( sizeof(void *) == sizeof(usize),	"unsigned size type is incorrect" );
	STATIC_ASSERT( sizeof(void *) == sizeof(isize),	"signed size type is incorrect" );


	// null pointer
	typedef std::nullptr_t		NullPtr_t;
	

	// Bytes and Bits
	template <typename T> struct Bytes;
	template <typename T> struct Bits;


}	// GXTypes
}	// GX_STL
