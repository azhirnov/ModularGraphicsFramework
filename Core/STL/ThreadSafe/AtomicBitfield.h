// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Types.h"

#ifdef GX_ATOMIC_SUPPORTED

namespace GX_STL
{
namespace GXTypes
{

	//
	// Atomic Bitfield
	//

	template <usize B, typename IndexType = usize>
	struct AtomicBitfield final
	{
	// types
	public:
		using Self		= AtomicBitfield< B, IndexType >;
		using T			= typename CompileTime::NearUInt::FromBits< CompileTime::Max< uint, B, 32 > >;
		using Value_t	= T;
		using Index_t	= IndexType;
		using Int_t		= CompileTime::NearUInt::FromSize< sizeof(IndexType) >;


	// variables
	private:
		std::atomic<T>		_atomic;


	// methods
	public:
		AtomicBitfield (GX_DEFCTOR) {}

		AtomicBitfield (Self &&) = delete;
		AtomicBitfield (const Self &) = delete;

		explicit AtomicBitfield (const T &value) : _atomic{ value } {}

		Self& operator = (Self &&) = delete;
		Self& operator = (const Self &) = delete;

		void		Set (Index_t i)					{ Or( i ); }
		void		Invert (Index_t i)				{ Xor( i ); }

		void		operator |= (Index_t i)			{ Or( i ); }
		void		operator &= (Index_t i)			{ And( i ); }
		void		operator ^= (Index_t i)			{ Xor( i ); }
		
		ND_ bool	operator [] (Index_t i)	 const	{ ASSUME(Int_t(i) < B);  return !!( Load() & (T(1) << Int_t(i)) ); }

		ND_ static constexpr usize Count ()			{ return B; }


#	if 1			// optimized
		void		And (Index_t i)					{ ASSUME(Int_t(i) < B);  _atomic.fetch_and( T(1) << Int_t(i), std::memory_order_relaxed ); }
		void		Or (Index_t i)					{ ASSUME(Int_t(i) < B);  _atomic.fetch_or(  T(1) << Int_t(i), std::memory_order_relaxed ); }
		void		Xor (Index_t i)					{ ASSUME(Int_t(i) < B);  _atomic.fetch_xor( T(1) << Int_t(i), std::memory_order_relaxed ); }

		ND_ const T	Load ()					const	{ return _atomic.load( std::memory_order_acquire ); }
		void		Store (const T &value)			{ _atomic.store( value, std::memory_order_release ); }

		ND_ const T	CmpExch (const T& val, const T& compare)
		{
			T	tmp = compare;
			_atomic.compare_exchange_strong( INOUT tmp, val, std::memory_order_acq_rel );
			return tmp;
		}

#	else			// for debugging
		void		And (Index_t i)					{ ASSUME(Int_t(i) < B);  _atomic.fetch_and( 1 ); }
		void		Or (Index_t i)					{ ASSUME(Int_t(i) < B);  _atomic.fetch_or(  1 ); }
		void		Xor (Index_t i)					{ ASSUME(Int_t(i) < B);  _atomic.fetch_xor( 1 ); }

		ND_ const T	Load ()					const	{ return _atomic.load(); }
		void		Store (const T &value)			{ _atomic.store( value ); }

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
