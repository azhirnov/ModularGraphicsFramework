// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
			::_ReadWriteBarrier();
			return left;
		}

#		undef  type_cast
#		undef  itype_cast
#		undef  ctype_cast
#		undef  rtype_cast


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
	
	// Warning: all functions except 'CmpExch' returns the operation result!
	// 'CmpExch' returns origin value.

	// byte //
	forceinline byte  AtomicOp::Inc (volatile byte * ptr)						{ return Add( ptr, 1 ); }
	forceinline byte  AtomicOp::Dec (volatile byte * ptr)						{ return Add( ptr, -1 ); }
	forceinline byte  AtomicOp::Add (volatile byte * ptr, byte val)				{ return ::_InterlockedExchangeAdd8( (volatile char *)ptr, val ) + val; }
	forceinline byte  AtomicOp::Sub (volatile byte * ptr, byte val)				{ return ::_InterlockedExchangeAdd8( (volatile char *)ptr, -val ) - val; }
	forceinline byte  AtomicOp::Set (volatile byte * ptr, byte val)				{ ::_InterlockedExchange8( (volatile char *)ptr, val );  return val; }
	forceinline byte  AtomicOp::CmpExch (volatile byte * ptr, byte val, byte cmp){ return ::_InterlockedCompareExchange8( (volatile char *)ptr, val, cmp ); }
	forceinline byte  AtomicOp::Or  (volatile byte * ptr, byte val)				{ return ::_InterlockedOr8( (volatile char *)ptr, val ) | val; }
	forceinline byte  AtomicOp::Xor (volatile byte * ptr, byte val)				{ return ::_InterlockedXor8( (volatile char *)ptr, val ) ^ val; }
	forceinline byte  AtomicOp::And (volatile byte * ptr, byte val)				{ return ::_InterlockedAnd8( (volatile char *)ptr, val ) & val; }
		
	// short //
	forceinline short AtomicOp::Inc (volatile short * ptr)							{ return ::_InterlockedIncrement16( (volatile short *)ptr ); }
	forceinline short AtomicOp::Dec (volatile short * ptr)							{ return ::_InterlockedDecrement16( (volatile short *)ptr ); }
	forceinline short AtomicOp::Add (volatile short * ptr, short val)				{ return ::_InterlockedExchangeAdd16( (volatile short *)ptr, val ) + val; }
	forceinline short AtomicOp::Sub (volatile short * ptr, short val)				{ return ::_InterlockedExchangeAdd16( (volatile short *)ptr, -val ) - val; }
	forceinline short AtomicOp::Set (volatile short * ptr, short val)				{ ::_InterlockedExchange16( (volatile short *)ptr, val );  return val; }
	forceinline short AtomicOp::CmpExch (volatile short * ptr, short val, short cmp){ return ::_InterlockedCompareExchange16( (volatile short *)ptr, val, cmp ); }
	forceinline short AtomicOp::Or  (volatile short * ptr, short val)				{ return ::_InterlockedOr16( (volatile short *)ptr, val ) | val; }
	forceinline short AtomicOp::Xor (volatile short * ptr, short val)				{ return ::_InterlockedXor16( (volatile short *)ptr, val ) ^ val; }
	forceinline short AtomicOp::And (volatile short * ptr, short val)				{ return ::_InterlockedAnd16( (volatile short *)ptr, val ) & val; }

	// int //
	forceinline int AtomicOp::Inc (volatile int * ptr)							{ return ::_InterlockedIncrement( (volatile long *)ptr ); }
	forceinline int AtomicOp::Dec (volatile int * ptr)							{ return ::_InterlockedDecrement( (volatile long *)ptr ); }
	forceinline int AtomicOp::Add (volatile int * ptr, int val)					{ return ::_InterlockedExchangeAdd( (volatile long *)ptr, val ) + val; }
	forceinline int AtomicOp::Sub (volatile int * ptr, int val)					{ return ::_InterlockedExchangeAdd( (volatile long *)ptr, -val ) - val; }
	forceinline int AtomicOp::Set (volatile int * ptr, int val)					{ ::_InterlockedExchange( (volatile long *)ptr, val );  return val; }
	forceinline int AtomicOp::CmpExch (volatile int * ptr, int val, int cmp)	{ return ::_InterlockedCompareExchange( (volatile long *)ptr, val, cmp ); }
	forceinline int AtomicOp::Or  (volatile int * ptr, int val)					{ return ::_InterlockedOr( (volatile long *)ptr, val ) | val; }
	forceinline int AtomicOp::Xor (volatile int * ptr, int val)					{ return ::_InterlockedXor( (volatile long *)ptr, val ) ^ val; }
	forceinline int AtomicOp::And (volatile int * ptr, int val)					{ return ::_InterlockedAnd( (volatile long *)ptr, val ) & val; }
		
#	if PLATFORM_BITS == 32
		
	// pointers //
	forceinline void * AtomicOp::SetPtr (volatile void ** ptr, void * val)				{ ::_InterlockedExchange( (volatile long *)ptr, (long)val );  return val; }
	forceinline void * AtomicOp::CmpExchP (volatile void **ptr, void *val, void *cmp)	{ return (void *)::_InterlockedCompareExchange( (volatile long *)ptr, (long)val, (long)cmp ); }

#	endif	// 32
	
#	if PLATFORM_BITS == 64

	// pointers //
	forceinline void * AtomicOp::SetPtr (void* volatile* ptr, void * val)			{ ::_InterlockedExchangePointer( ptr, val );  return val; }
	forceinline void * AtomicOp::CmpExP (void* volatile* ptr, void *val, void *cmp)	{ return ::_InterlockedCompareExchangePointer( ptr, val, cmp ); }
		
	// ilong //
	forceinline ilong AtomicOp::Inc (volatile ilong * ptr)							{ return ::_InterlockedIncrement64( ptr ); }
	forceinline ilong AtomicOp::Dec (volatile ilong * ptr)							{ return ::_InterlockedDecrement64( ptr ); }
	forceinline ilong AtomicOp::Add (volatile ilong * ptr, ilong val)				{ return ::_InterlockedExchangeAdd64( ptr, val ) + val; }
	forceinline ilong AtomicOp::Sub (volatile ilong * ptr, ilong val)				{ return ::_InterlockedExchangeAdd64( ptr, -val ) - val; }
	forceinline ilong AtomicOp::Set (volatile ilong * ptr, ilong val)				{ ::_InterlockedExchange64( ptr, val );  return val; }
	forceinline ilong AtomicOp::CmpExch (volatile ilong * ptr, ilong val, ilong cmp){ return ::_InterlockedCompareExchange64( ptr, val, cmp ); }
	forceinline ilong AtomicOp::Or  (volatile ilong * ptr, ilong val)				{ return ::_InterlockedOr64( ptr, val ) | val; }
	forceinline ilong AtomicOp::Xor (volatile ilong * ptr, ilong val)				{ return ::_InterlockedXor64( ptr, val ) ^ val; }
	forceinline ilong AtomicOp::And (volatile ilong * ptr, ilong val)				{ return ::_InterlockedAnd64( ptr, val ) & val; }

#	endif	// 64


}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS