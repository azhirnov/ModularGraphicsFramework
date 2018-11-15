// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Types.h"

#ifdef GX_ATOMIC_SUPPORTED

namespace GX_STL
{
namespace GXTypes
{

	//
	// Atomic Counter
	//

	template <typename T>
	struct AtomicCounter final
	{
	// types
	public:
		using Self		= AtomicCounter<T>;
		using Value_t	= T;


	// variables
	private:
		std::atomic<T>		_atomic;


	// methods
	public:
		AtomicCounter (GX_DEFCTOR) {}

		AtomicCounter (Self &&) = delete;
		AtomicCounter (const Self &) = delete;

		explicit AtomicCounter (const T &value) : _atomic{ value } {}

		Self&		operator = (Self &&) = delete;
		Self&		operator = (const Self &) = delete;

		void		operator ++ ()				{ Inc(); }
		void		operator -- ()				{ Dec(); }


#	if 1			// optimized
		void		Inc ()						{ _atomic.fetch_add( 1, std::memory_order_relaxed ); }
		void		Dec ()						{ _atomic.fetch_sub( 1, std::memory_order_relaxed ); }

		ND_ const T	Load ()				const	{ return _atomic.load( std::memory_order_acquire ); }
		void		Store (const T &value)		{ _atomic.store( value, std::memory_order_release ); }
		
		ND_ bool	DecAndTest ()
		{
			const T  res = _atomic.fetch_sub( 1, std::memory_order_release );
			ASSERT( res > 0 );

			if ( res == 1 ) {
				std::atomic_thread_fence( std::memory_order_acquire );
				return true;
			}
			return false;
		}

		ND_ const T	CmpExch (const T& val, const T& compare)
		{
			T	tmp = compare;
			_atomic.compare_exchange_strong( INOUT tmp, val, std::memory_order_acq_rel );
			return tmp;
		}

#	else			// for debugging
		void		Inc ()						{ _atomic.fetch_add( 1 ); }
		void		Dec ()						{ _atomic.fetch_sub( 1 ); }

		ND_ const T	Load ()				const	{ return _atomic.load(); }
		void		Store (const T &value)		{ _atomic.store( value ); }

		ND_ bool	DecAndTest ()				{ return _atomic.fetch_sub( 1 ) == 1; }

		ND_ const T	CmpExch (const T& val, const T& compare)
		{
			T	tmp = compare;
			_atomic.compare_exchange_strong( INOUT tmp, val );
			return tmp;
		}
#	endif
	};


}	// GXTypes
}	// GX_STL

#endif	// GX_ATOMIC_SUPPORTED
