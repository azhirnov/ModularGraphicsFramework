// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Types.h"

#ifdef GX_ATOMIC_SUPPORTED

namespace GX_STL
{
namespace GXTypes
{

	//
	// Atomic Flag
	//

	struct AtomicFlag final
	{
	// types
	public:
		using Self		= AtomicFlag;
		using Value_t	= bool;


	// variables
	private:
		std::atomic<Value_t>	_flag;


	// methods
	public:
		AtomicFlag () = default;
		AtomicFlag (const Value_t& value) : _flag{ value } {}

		AtomicFlag (AtomicFlag &&) = delete;
		AtomicFlag (const AtomicFlag &) = delete;

		AtomicFlag& operator = (AtomicFlag &&) = delete;
		AtomicFlag& operator = (const AtomicFlag &) = delete;


		// fast
		void operator = (Value_t value)				{ _flag.store( value, std::memory_order_relaxed ); }

		ND_ explicit operator Value_t () const		{ return _flag.load( std::memory_order_relaxed ); }


		// slower
		void		StoreAndSync (Value_t value)	{ _flag.store( value, std::memory_order_release ); }

		ND_ Value_t	SyncAndLoad () const			{ return _flag.load( std::memory_order_acquire ); }
	};


}	// GXTypes
}	// GX_STL

#endif	// GX_ATOMIC_SUPPORTED
