// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Init.h"
#include "Engine/STL/Common/Uninitialized.h"

namespace GX_STL
{
namespace GXTypes
{

	// integer types
	using wchar		= wchar_t;
	using byte		= signed char;
	//using short	= signed short;
	//using int		= signed int;
	using ilong		= signed long long;

	using ubyte		= unsigned char;
	using ushort	= unsigned short;
	using uint		= unsigned int;
	using ulong		= unsigned long long;


	// pointer & size type
#if PLATFORM_BITS == 16
//	using usize		= unsigned short;
	using isize		= signed short;
#elif PLATFORM_BITS == 32
//	using usize		= unsigned int;
	using isize		= signed int;
#elif PLATFORM_BITS == 64
//	using usize		= unsigned long long;
	using isize		= signed long long;
#endif

	using usize		= std::size_t;

	// check size types
	STATIC_ASSERT( sizeof(void *) == sizeof(usize),	"unsigned size type is incorrect" );
	STATIC_ASSERT( sizeof(void *) == sizeof(isize),	"signed size type is incorrect" );


	// null pointer
	using NullPtr_t			= std::nullptr_t;
	

	// initializer list
#	define InitializerList	std::initializer_list


	// Bytes and Bits
	template <typename T> struct Bytes;
	template <typename T> struct Bits;


}	// GXTypes
}	// GX_STL
