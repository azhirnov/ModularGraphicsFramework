// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "OSWindows.h"

#if defined( PLATFORM_WINDOWS ) and not defined( GX_USE_STD )

namespace GX_STL
{
namespace OS
{
	
	//
	// Atomic Operations
	//

	struct _STL_EXPORT_ AtomicOp : public Noninstancable
	{
		// type cast //
#		define type_cast( _val_ )	& ReferenceCast< volatile CompileTime::NearInt::FromType<T>, volatile T >( _val_ )
#		define itype_cast( _val_ )  & ReferenceCast< volatile CompileTime::NearInt::FromType<T>, volatile T >( _val_ )
#		define ctype_cast( _val_ )	ReferenceCast< const CompileTime::NearInt::FromType<T>, const T >( _val_ )
#		define rtype_cast( _val_ )	ReferenceCast< const T, const CompileTime::NearInt::FromType<T> >( _val_ )


		// T //
		template <typename T>
		forceinline static T  Inc (volatile T & left) {
			return Inc( type_cast( left ) );
		}

		template <typename T>
		forceinline static T  Dec (volatile T & left) {
			return Dec( type_cast( left ) );
		}

		template <typename T>
		forceinline static T  Add (volatile T & left, const T& right) {
			return Add( type_cast( left ), ctype_cast( right ) );
		}

		template <typename T>
		forceinline static T  Sub (volatile T & left, const T& right) {
			return Sub( type_cast( left ), ctype_cast( right ) );
		}

		template <typename T>
		forceinline static T  Set (volatile T & left, const T& right) {
			return rtype_cast( Set( itype_cast( left ), ctype_cast( right ) ) );
		}

		template <typename T>
		forceinline static T  CmpExch (volatile T & left, const T& right, const T& cmp) {
			return rtype_cast( CmpExch( itype_cast( left ), ctype_cast( right ), ctype_cast( cmp ) ) );
		}

		template <typename T>
		forceinline static T  Or (volatile T & left, const T& right) {
			return Or( type_cast( left ), ctype_cast( right ) );
		}

		template <typename T>
		forceinline static T  Xor (volatile T & left, const T& right) {
			return Xor( type_cast( left ), ctype_cast( right ) );
		}

		template <typename T>
		forceinline static T  And (volatile T & left, const T& right) {
			return And( type_cast( left ), ctype_cast( right ) );
		}
		
		template <typename T>
		forceinline static T  Get (volatile T const & left) {
			_Barrier();
			return left;
		}

		static void _Barrier ();

#		undef  type_cast
#		undef  itype_cast
#		undef  ctype_cast
#		undef  rtype_cast

#	ifdef COMPILER_MSVC
		// byte //
		static byte  Inc (volatile byte * ptr);
		static byte  Dec (volatile byte * ptr);
		static byte  Add (volatile byte * ptr, byte val);
		static byte  Sub (volatile byte * ptr, byte val);
		static byte  Set (volatile byte * ptr, byte val);
		static byte  CmpExch (volatile byte * ptr, byte val, byte cmp);
		static byte  Or  (volatile byte * ptr, byte val);
		static byte  Xor (volatile byte * ptr, byte val);
		static byte  And (volatile byte * ptr, byte val);
		
		// short //
		static short Inc (volatile short * ptr);
		static short Dec (volatile short * ptr);
		static short Add (volatile short * ptr, short val);
		static short Sub (volatile short * ptr, short val);
		static short Set (volatile short * ptr, short val);
		static short CmpExch (volatile short * ptr, short val, short cmp);
		static short Or  (volatile short * ptr, short val);
		static short Xor (volatile short * ptr, short val);
		static short And (volatile short * ptr, short val);
#	endif	// COMPILER_MSVC

		// int //
		static int Inc (volatile int * ptr);
		static int Dec (volatile int * ptr);
		static int Add (volatile int * ptr, int val);
		static int Sub (volatile int * ptr, int val);
		static int Set (volatile int * ptr, int val);
		static int CmpExch (volatile int * ptr, int val, int cmp);
		static int Or  (volatile int * ptr, int val);
		static int Xor (volatile int * ptr, int val);
		static int And (volatile int * ptr, int val);
		

#	if PLATFORM_BITS == 32
		
		// pointers //
		static void * SetPtr (volatile void ** ptr, void * val);
		static void * CmpExchP (volatile void **ptr, void *val, void *cmp);

		// ilong //
		static ilong Inc (volatile ilong * ptr);
		static ilong Dec (volatile ilong * ptr);
		static ilong Add (volatile ilong * ptr, ilong val);
		static ilong Sub (volatile ilong * ptr, ilong val);
		static ilong Set (volatile ilong * ptr, ilong val);
		static ilong CmpExch (volatile ilong * ptr, ilong val, ilong cmp);
		static ilong Or  (volatile ilong * ptr, ilong val);
		static ilong Xor (volatile ilong * ptr, ilong val);
		static ilong And (volatile ilong * ptr, ilong val);

#	endif	// 32

#	if PLATFORM_BITS == 64

		// pointers //
		static void * SetPtr (void* volatile* ptr, void * val);
		static void * CmpExP (void* volatile* ptr, void *val, void *cmp);
		
		// ilong //
		static ilong Inc (volatile ilong * ptr);
		static ilong Dec (volatile ilong * ptr);
		static ilong Add (volatile ilong * ptr, ilong val);
		static ilong Sub (volatile ilong * ptr, ilong val);
		static ilong Set (volatile ilong * ptr, ilong val);
		static ilong CmpExch (volatile ilong * ptr, ilong val, ilong cmp);
		static ilong Or  (volatile ilong * ptr, ilong val);
		static ilong Xor (volatile ilong * ptr, ilong val);
		static ilong And (volatile ilong * ptr, ilong val);
		
#	endif	// 64

	};


}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS