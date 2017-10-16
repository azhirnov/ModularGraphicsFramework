// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Construct	- call contructor
	Destruct	- call destructor
	Create		- call default constructors for elements
	Destroy		- call destructors for elements
	Copy		- copy elements (copy-ctor)
	Move		- move external objects (move-ctor)
	Replace		- move internal elements (move-ctor + dtor)
*/

#pragma once

#include "Engine/STL/Memory/MemFunc.h"
#include "Engine/STL/Memory/PlacementNew.h"
#include "Engine/STL/Dimensions/ByteAndBit.h"

namespace GX_STL
{
namespace GXTypes
{

	struct CopyStrategy
	{
	
		//
		// Copy Strategy with MemMove
		//

		template <typename T>
		struct MemCopyWithCtor : public Noninstancable
		{
			STATIC_ASSERT(	CompileTime::IsMemCopyAvailable<T> and
							not CompileTime::IsNoncopyable<T> );

			// create default elements
			static void Create (T *ptr, usize count)
			{
				for (usize i = 0; i < count; ++i) {
					UnsafeMem::PlacementNew<T>( ptr + i );
				}
			}

			static void Destroy (T *ptr, usize count)
			{
				for (usize i = 0; i < count; ++i) {
					PlacementDelete( ptr[i] );
				}
			}

			// copy elements from one memblock to other memblock
			static void Copy (T *to, const T * const from, const usize count)
			{
				for (usize i = 0; i < count; ++i) {
					UnsafeMem::PlacementNew<T>( to+i, from[i] );
				}
			}

			// move elements from one memblock to other memblock
			static void Move (T *to, T *from, const usize count)
			{
				for (usize i = 0; i < count; ++i) {
					UnsafeMem::PlacementNew<T>( to+i, RVREF( from[i] ) );
				}
			}
			
			// replace elements inside memory block
			static void Replace (T *to, T *from, const usize count)
			{
				UnsafeMem::MemMove( to, from, SizeOf<T>() * count );

				DEBUG_ONLY( 
				for (T* t = from; t+1 < from + count; ++t) {
					if (t < to or t >= to + count) {
						UnsafeMem::ZeroMem( t, SizeOf<T>() );
					}
				})
			}
		};



		//
		// Copy Strategy with Copy Ctor and Move Ctor
		//

		template <typename T>
		struct CopyAndMoveCtor : public Noninstancable
		{
			STATIC_ASSERT( (CompileTime::IsCtorAvailable<T> or
							CompileTime::IsDtorAvailable<T>) and
							not CompileTime::IsNoncopyable<T> );
			
			// create default elements
			static void Create (T *ptr, usize count)
			{
				for (usize i = 0; i < count; ++i) {
					UnsafeMem::PlacementNew<T>( ptr + i );
				}
			}

			static void Destroy (T *ptr, usize count)
			{
				for (usize i = 0; i < count; ++i) {
					PlacementDelete( ptr[i] );
				}
			}
			
			// copy elements from one memblock to other memblock
			static void Copy (T *to, const T * const from, const usize count)
			{
				for (usize i = 0; i < count; ++i) {
					UnsafeMem::PlacementNew<T>( to+i, from[i] );
				}
			}
			
			// move elements from one memblock to other memblock
			static void Move (T *to, T *from, const usize count)
			{
				for (usize i = 0; i < count; ++i) {
					UnsafeMem::PlacementNew<T>( to+i, RVREF( from[i] ) );
				}
			}
			
			// replace elements inside memory block
			static void Replace (T *to, T *from, const usize count)
			{
				for (usize i = 0; i < count; ++i) {
					UnsafeMem::PlacementNew<T>( to+i, RVREF( from[i] ) );
					PlacementDelete( from[i] );	// TODO: is it needed?
				}
			}
		};



		//
		// Copy Strategy with MemCopy & MemMove without Ctors
		//

		template <typename T>
		struct MemCopyWithoutCtor : public Noninstancable
		{
			STATIC_ASSERT(	CompileTime::IsMemCopyAvailable<T>	 and
							CompileTime::IsZeroMemAvailable<T>	 and
							not CompileTime::IsCtorAvailable<T>  and
							not CompileTime::IsDtorAvailable<T>  and
							not CompileTime::IsNoncopyable<T> );
			
			// create default elements
			static void Create (T *ptr, usize count)
			{
				UnsafeMem::ZeroMem( ptr, SizeOf<T>() * count );
			}

			static void Destroy (T *ptr, usize count)
			{
				DEBUG_ONLY( UnsafeMem::ZeroMem( ptr, SizeOf<T>() * count ) );
			}
			
			// copy elements from one memblock to other memblock
			static void Copy (T *to, const T * const from, const usize count)
			{
				UnsafeMem::MemCopy( to, from, SizeOf<T>() * count );
			}
			
			// move elements from one memblock to other memblock
			static void Move (T *to, T * from, const usize count)
			{
				UnsafeMem::MemMove( to, from, SizeOf<T>() * count );
			}
			
			// replace elements inside memory block
			static void Replace (T *to, T *from, const usize count)
			{
				Move( to, from, count );
				
				DEBUG_ONLY( 
				for (T* t = from; t+1 < from + count; ++t) {
					if (t < to or t >= to + count) {
						UnsafeMem::ZeroMem( t, SizeOf<T>() );
					}
				})
			}
		};

	};

	
	//
	// Auto Detect Copy Strategy
	//

	template <typename T>
	struct AutoDetectCopyStrategy : public Noninstancable
	{
		STATIC_ASSERT( not CompileTime::IsNoncopyable<T> );

		static const bool _is_fastcopy	= CompileTime::IsMemCopyAvailable<T>;
		static const bool _is_pod		= _is_fastcopy and not CompileTime::IsCtorAvailable<T> and not CompileTime::IsDtorAvailable<T>;

		typedef typename CompileTime::SwitchType< _is_pod, CopyStrategy::MemCopyWithoutCtor<T>,
							typename CompileTime::SwitchType< _is_fastcopy, CopyStrategy::MemCopyWithCtor<T>,
																			CopyStrategy::CopyAndMoveCtor<T> > >		type;
	};


}	// GXTypes
}	// GX_STL
