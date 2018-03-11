// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/STL/Common/Platforms.h"
#include "Engine/Config/STL.Config.h"

#if defined( PLATFORM_WINDOWS ) and not defined( GX_USE_STD )

#include "Engine/STL/OS/Windows/AtomicOp.h"
#include "Engine/STL/Math/BinaryMath.h"
#include "Engine/STL/Math/Interpolations.h"
#include "Engine/STL/OS/Windows/WinHeader.h"

#ifdef COMPILER_GCC
# include <winbase.h>
#endif

namespace GX_STL
{
namespace OS
{

	
	// Warning: all functions except 'CmpExch' returns the operation result!
	// 'CmpExch' returns origin value.

	void AtomicOp::_Barrier ()											{ _ReadWriteBarrier(); }
	
# ifdef COMPILER_MSVC
	// byte //
	byte  AtomicOp::Inc (volatile byte * ptr)							{ return Add( ptr, 1 ); }
	byte  AtomicOp::Dec (volatile byte * ptr)							{ return Add( ptr, -1 ); }
	byte  AtomicOp::Add (volatile byte * ptr, byte val)					{ return ::_InterlockedExchangeAdd8( (volatile char *)ptr, val ) + val; }
	byte  AtomicOp::Sub (volatile byte * ptr, byte val)					{ return ::_InterlockedExchangeAdd8( (volatile char *)ptr, -val ) - val; }
	byte  AtomicOp::Set (volatile byte * ptr, byte val)					{ ::_InterlockedExchange8( (volatile char *)ptr, val );  return val; }
	byte  AtomicOp::CmpExch (volatile byte * ptr, byte val, byte cmp)	{ return ::_InterlockedCompareExchange8( (volatile char *)ptr, val, cmp ); }
	byte  AtomicOp::Or  (volatile byte * ptr, byte val)					{ return byte(::_InterlockedOr8( (volatile char *)ptr, val ) | val); }
	byte  AtomicOp::Xor (volatile byte * ptr, byte val)					{ return byte(::_InterlockedXor8( (volatile char *)ptr, val ) ^ val); }
	byte  AtomicOp::And (volatile byte * ptr, byte val)					{ return byte(::_InterlockedAnd8( (volatile char *)ptr, val ) & val); }
		
	// short //
	short AtomicOp::Inc (volatile short * ptr)							{ return ::_InterlockedIncrement16( (volatile short *)ptr ); }
	short AtomicOp::Dec (volatile short * ptr)							{ return ::_InterlockedDecrement16( (volatile short *)ptr ); }
	short AtomicOp::Add (volatile short * ptr, short val)				{ return ::_InterlockedExchangeAdd16( (volatile short *)ptr, val ) + val; }
	short AtomicOp::Sub (volatile short * ptr, short val)				{ return ::_InterlockedExchangeAdd16( (volatile short *)ptr, -val ) - val; }
	short AtomicOp::Set (volatile short * ptr, short val)				{ ::_InterlockedExchange16( (volatile short *)ptr, val );  return val; }
	short AtomicOp::CmpExch (volatile short * ptr, short val, short cmp){ return ::_InterlockedCompareExchange16( (volatile short *)ptr, val, cmp ); }
	short AtomicOp::Or  (volatile short * ptr, short val)				{ return ::_InterlockedOr16( (volatile short *)ptr, val ) | val; }
	short AtomicOp::Xor (volatile short * ptr, short val)				{ return ::_InterlockedXor16( (volatile short *)ptr, val ) ^ val; }
	short AtomicOp::And (volatile short * ptr, short val)				{ return ::_InterlockedAnd16( (volatile short *)ptr, val ) & val; }

# endif	// COMPILER_MSVC

	// int //
	int AtomicOp::Inc (volatile int * ptr)								{ return ::_InterlockedIncrement( (volatile long *)ptr ); }
	int AtomicOp::Dec (volatile int * ptr)								{ return ::_InterlockedDecrement( (volatile long *)ptr ); }
	int AtomicOp::Add (volatile int * ptr, int val)						{ return ::_InterlockedExchangeAdd( (volatile long *)ptr, val ) + val; }
	int AtomicOp::Sub (volatile int * ptr, int val)						{ return ::_InterlockedExchangeAdd( (volatile long *)ptr, -val ) - val; }
	int AtomicOp::Set (volatile int * ptr, int val)						{ ::_InterlockedExchange( (volatile long *)ptr, val );  return val; }
	int AtomicOp::CmpExch (volatile int * ptr, int val, int cmp)		{ return ::_InterlockedCompareExchange( (volatile long *)ptr, val, cmp ); }
	int AtomicOp::Or  (volatile int * ptr, int val)						{ return ::_InterlockedOr( (volatile long *)ptr, val ) | val; }
	int AtomicOp::Xor (volatile int * ptr, int val)						{ return ::_InterlockedXor( (volatile long *)ptr, val ) ^ val; }
	int AtomicOp::And (volatile int * ptr, int val)						{ return ::_InterlockedAnd( (volatile long *)ptr, val ) & val; }
		
#	if PLATFORM_BITS == 32
		
	// pointers //
	void * AtomicOp::SetPtr (volatile void ** ptr, void * val)				{ ::_InterlockedExchange( (volatile long *)ptr, (long)val );  return val; }
	void * AtomicOp::CmpExchP (volatile void **ptr, void *val, void *cmp)	{ return (void *)::_InterlockedCompareExchange( (volatile long *)ptr, (long)val, (long)cmp ); }

#	endif	// 32

	
#	if PLATFORM_BITS == 64

	// pointers //
	void * AtomicOp::SetPtr (void* volatile* ptr, void * val)			{ ::_InterlockedExchangePointer( ptr, val );  return val; }
	void * AtomicOp::CmpExP (void* volatile* ptr, void *val, void *cmp)	{ return ::_InterlockedCompareExchangePointer( ptr, val, cmp ); }
		
	// ilong //
	ilong AtomicOp::Inc (volatile ilong * ptr)							{ return ::_InterlockedIncrement64( ptr ); }
	ilong AtomicOp::Dec (volatile ilong * ptr)							{ return ::_InterlockedDecrement64( ptr ); }
	ilong AtomicOp::Add (volatile ilong * ptr, ilong val)				{ return ::_InterlockedExchangeAdd64( ptr, val ) + val; }
	ilong AtomicOp::Sub (volatile ilong * ptr, ilong val)				{ return ::_InterlockedExchangeAdd64( ptr, -val ) - val; }
	ilong AtomicOp::Set (volatile ilong * ptr, ilong val)				{ ::_InterlockedExchange64( ptr, val );  return val; }
	ilong AtomicOp::CmpExch (volatile ilong * ptr, ilong val, ilong cmp){ return ::_InterlockedCompareExchange64( ptr, val, cmp ); }
	ilong AtomicOp::Or  (volatile ilong * ptr, ilong val)				{ return ::_InterlockedOr64( ptr, val ) | val; }
	ilong AtomicOp::Xor (volatile ilong * ptr, ilong val)				{ return ::_InterlockedXor64( ptr, val ) ^ val; }
	ilong AtomicOp::And (volatile ilong * ptr, ilong val)				{ return ::_InterlockedAnd64( ptr, val ) & val; }

#	endif	// 64

#	if PLATFORM_BITS == 32
		
	// ilong //
	ilong AtomicOp::Inc (volatile ilong * ptr)							{ return InterlockedIncrement64( ptr ); }
	ilong AtomicOp::Dec (volatile ilong * ptr)							{ return InterlockedDecrement64( ptr ); }
	ilong AtomicOp::Add (volatile ilong * ptr, ilong val)				{ return InterlockedExchangeAdd64( ptr, val ); }
	ilong AtomicOp::Sub (volatile ilong * ptr, ilong val)				{ return InterlockedExchangeAdd64( ptr, -val ); }
	ilong AtomicOp::Set (volatile ilong * ptr, ilong val)				{ return InterlockedExchange64( ptr, val ); }
	ilong AtomicOp::CmpExch (volatile ilong * ptr, ilong val, ilong cmp){ return InterlockedCompareExchange64( ptr, val, cmp ); }
	ilong AtomicOp::Or  (volatile ilong * ptr, ilong val)				{ return InterlockedOr64( ptr, val ); }
	ilong AtomicOp::Xor (volatile ilong * ptr, ilong val)				{ return InterlockedXor64( ptr, val ); }
	ilong AtomicOp::And (volatile ilong * ptr, ilong val)				{ return InterlockedAnd64( ptr, val ); }

#	endif	// 32

}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS and not GX_USE_STD
