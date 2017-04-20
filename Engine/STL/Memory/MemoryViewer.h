// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Common/Types.h"

namespace GX_STL
{
namespace GXTypes
{
	
	//
	// Memory Viewer
	//

	template <typename T, usize I = 400>
	struct TMemoryViewer
	{
		// types
		typedef T					(*p_static_array_t)[I];
		typedef TMemoryViewer<T,I>	Self;

		// value
		p_static_array_t	ptr;

		// cast
		static const Self Cast (const void *ptr)	{ return *PointerCast< Self >( &ptr ); }

		// check
		STATIC_ASSERT( sizeof(T*) == sizeof(p_static_array_t), "size mismatch" );
	};


}	// GXTypes
}	// GX_STL
