// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "AtomicOp.h"

#if defined( PLATFORM_WINDOWS ) and not defined( GX_USE_STD )

#include "WinHeader.h"

namespace GX_STL
{
namespace OS
{


#	if PLATFORM_BITS == 32
		
	// ilong //
	ilong AtomicOp::Inc (volatile ilong * ptr)							{ return InterlockedIncrement64( ptr ); }
	ilong AtomicOp::Dec (volatile ilong * ptr)							{ return InterlockedDecrement64( ptr ); }
	ilong AtomicOp::Add (volatile ilong * ptr, ilong val)				{ return InterlockedExchangeAdd64( ptr, val ); }
	ilong AtomicOp::Sub (volatile ilong * ptr, ilong val)				{ return InterlockedExchangeAdd64( ptr, -val ); }
	ilong AtomicOp::Set (volatile ilong * ptr, ilong val)				{ return InterlockedExchange64( ptr, val ); }
	ilong AtomicOp::CmpEx (volatile ilong * ptr, ilong val, ilong cmp)	{ return InterlockedCompareExchange64( ptr, val, cmp ); }
	ilong AtomicOp::Or  (volatile ilong * ptr, ilong val)				{ return InterlockedOr64( ptr, val ); }
	ilong AtomicOp::Xor (volatile ilong * ptr, ilong val)				{ return InterlockedXor64( ptr, val ); }
	ilong AtomicOp::And (volatile ilong * ptr, ilong val)				{ return InterlockedAnd64( ptr, val ); }

#	endif	// 32

}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS
