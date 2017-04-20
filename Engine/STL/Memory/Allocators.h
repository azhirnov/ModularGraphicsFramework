// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Memory/MemoryViewer.h"
#include "Engine/STL/Containers/CopyStrategy.h"

namespace GX_STL
{
namespace GXTypes
{
	
	//
	// Default Allocator
	//

	template <typename T>
	struct TDefaultAllocator : public Noninstancable
	{
		static bool Allocate (INOUT T *&ptr, INOUT usize &size) noexcept
		{
			ASSUME( size > 0 );

			ptr = (T*) ::operator new( sizeof(T) * size );
			ASSERT( ptr != null and "can't allocate memory!" );

			return ptr != null;
		}

		static void Deallocate (T *&ptr) noexcept
		{
			::operator delete( ptr );
			ptr = null;
		}
	};


	template <>
	struct TDefaultAllocator<void> : public Noninstancable
	{
		static bool Allocate (INOUT void *&ptr, INOUT usize &size) noexcept
		{
			return TDefaultAllocator<ubyte>::Allocate( reinterpret_cast< ubyte *&>(ptr), size );
		}

		static void Deallocate (void *&ptr) noexcept
		{
			return TDefaultAllocator<ubyte>::Deallocate( reinterpret_cast< ubyte *&>( ptr ) );
		}
	};



	//
	// Aligned Allocator
	//
	
# ifdef PLATFORM_ANDROID
#	pragma message ("aligned allocation changed to default")
	
	template <typename T, usize AlignInBytes = 16>
	struct TAlignedAllocator : TDefaultAllocator<T>
	{
		static const usize	ALIGN = AlignInBytes;
	};

# else

	template <typename T, usize AlignInBytes = 16>
	struct TAlignedAllocator : public Noninstancable
	{
		static const usize	ALIGN = AlignInBytes;

		static bool Allocate (INOUT T *&ptr, INOUT usize &size) noexcept
		{
			STATIC_ASSERT( (CompileTime::IsPowerOfTwo< usize, AlignInBytes >), "Align must be power of 2" );

			ASSUME( size > 0 );

			ptr = (T*) _mm_malloc( sizeof(T) * size, AlignInBytes );
			ASSERT( ptr != null and "can't allocate aligned memory!" );

			return ptr != null;
		}

		static void Deallocate (T *&ptr) noexcept
		{
			_mm_free( ptr );
			ptr = null;
		}
	};


	template <usize AlignInBytes>
	struct TAlignedAllocator<void, AlignInBytes> : public Noninstancable
	{
		static const usize	ALIGN = AlignInBytes;

		static bool Allocate (INOUT void *&ptr, INOUT usize &size) noexcept
		{
			return TAlignedAllocator<ubyte, AlignInBytes>::Allocate( reinterpret_cast< ubyte *&>(ptr), size );
		}

		static void Deallocate (void *&ptr) noexcept
		{
			return TAlignedAllocator<ubyte, AlignInBytes>::Deallocate( reinterpret_cast< ubyte *&>( ptr ) );
		}
	};

# endif

}	// GXTypes
}	// GX_STL
