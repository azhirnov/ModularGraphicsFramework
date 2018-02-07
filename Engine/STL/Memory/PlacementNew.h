// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Cast.h"

namespace GX_STL
{
namespace GXTypes
{

	template <typename T>
	struct ArrayRef;
	
/*
=================================================
	IsAlignedMem
=================================================
*/
	namespace _types_hidden_
	{
		template <typename T>
		forceinline bool IsAlignedMem (void *ptr) noexcept
		{
			enum { ALIGN = alignof(T) /*sizeof(void *)*/ };
			return (sizeof(T) < ALIGN) or IsAlignedPointer< ALIGN >( ptr );
		}
	}
	
/*
=================================================
	PlacementNew
=================================================
*/
	namespace UnsafeMem
	{
		template <typename T>
		forceinline T *  PlacementNew (OUT void *ptr) noexcept
		{
			ASSERT( _types_hidden_::IsAlignedMem<T>( ptr ) );
			return ( new(ptr) T() );
		}

		template <typename T, typename ...Types>
		forceinline T *  PlacementNew (OUT void *ptr, Types&&... args) noexcept
		{
			ASSERT( _types_hidden_::IsAlignedMem<T>( ptr ) );
			return ( new(ptr) T( FW<Types>(args)... ) );
		}

	}	// UnsafeMem
	
/*
=================================================
	PlacementNew
=================================================
*/
	template <typename T, typename C>
	forceinline T * PlacementNew (OUT ArrayRef<C> buf) noexcept;

	template <typename T, typename C, typename ...Types>
	forceinline T * PlacementNew (OUT ArrayRef<C> buf, Types&&... args) noexcept;

/*
=================================================
	PlacementDelete
=================================================
*/
	template <typename T>
	forceinline void PlacementDelete (INOUT T &value) noexcept
	{
		value.~T();
		DEBUG_ONLY( ::memset( &value, 0, sizeof(value) ) );
	}


}	// GXTypes
}	// GX_STL

