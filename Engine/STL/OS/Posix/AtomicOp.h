// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Platforms.h"
#include "Engine/Config/STL.Config.h"

#if defined( PLATFORM_BASE_POSIX ) and not defined( GX_USE_STD )

#include "Engine/STL/OS/Posix/OSPosix.h"

namespace GX_STL
{
namespace OS
{

	//
	// Atomic Operations
	//

	struct AtomicOp final : public Noninstancable
	{
		// type cast //
#		define type_cast( _val_ )	& ReferenceCast< volatile CompileTime::NearInt::FromType<T>, volatile T >( _val_ )
#		define itype_cast( _val_ )  & ReferenceCast< volatile CompileTime::NearInt::FromType<T>, volatile T >( _val_ )
#		define ctype_cast( _val_ )	ReferenceCast< const CompileTime::NearInt::FromType<T>, const T >( _val_ )
#		define rtype_cast( _val_ )	ReferenceCast< const T, const CompileTime::NearInt::FromType<T> >( _val_ )


		// T //
		template <typename T>
		forceinline static T  Inc (volatile T & left) {
			return rtype_cast( __sync_add_and_fetch( type_cast( left ), 1 ) );
		}

		template <typename T>
		forceinline static T  Dec (volatile T & left) {
			return rtype_cast( __sync_sub_and_fetch( type_cast( left ), 1 ) );
		}

		template <typename T>
		forceinline static T  Add (volatile T & left, const T& right) {
			return rtype_cast( __sync_add_and_fetch( type_cast( left ), ctype_cast( right ) ) );
		}

		template <typename T>
		forceinline static T  Sub (volatile T & left, const T& right) {
			return rtype_cast( __sync_sub_and_fetch( type_cast( left ), ctype_cast( right ) ) );
		}

		template <typename T>
		forceinline static void  _Set (volatile T * left, const T& right)
		{
			T prev;
			do {
				prev = *left;
			}
			while ( not __sync_bool_compare_and_swap( left, prev, right ) );
		}

		template <typename T>
		forceinline static T  Set (volatile T & left, const T& right) {
			_Set( itype_cast( left ), ctype_cast( right ) );
			return right;
		}

		template <typename T>
		ND_ forceinline static T Get (volatile T const & left) {
			T	value = left;
			__sync_synchronize();
			return value;
		}

		template <typename T>
		forceinline static T  CmpExch (volatile T & left, const T& right, const T& cmp) {
			return rtype_cast( __sync_val_compare_and_swap( itype_cast( left ), ctype_cast( cmp ), ctype_cast( right ) ) );
		}

		template <typename T>
		forceinline static T  Or (volatile T & left, const T& right) {
			return rtype_cast( __sync_or_and_fetch( type_cast( left ), ctype_cast( right ) ) );
		}

		template <typename T>
		forceinline static T  Xor (volatile T & left, const T& right) {
			return rtype_cast( __sync_xor_and_fetch( type_cast( left ), ctype_cast( right ) ) );
		}

		template <typename T>
		forceinline static T  And (volatile T & left, const T& right) {
			return rtype_cast( __sync_and_and_fetch( type_cast( left ), ctype_cast( right ) ) );
		}

#		undef  type_cast
#		undef  itype_cast
#		undef  ctype_cast
#		undef  rtype_cast
	};

}	// OS
}	// GX_STL

#endif	// PLATFORM_BASE_POSIX