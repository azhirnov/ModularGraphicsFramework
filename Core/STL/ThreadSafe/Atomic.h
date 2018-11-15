// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Types.h"

#ifdef GX_ATOMIC_SUPPORTED

namespace GX_STL
{
namespace GXTypes
{

	//
	// Atomic
	//

	template <typename T>
	struct Atomic final
	{
	// types
	public:
		using Value_t	= T;
		using Self		= Atomic<T>;


	// variables
	private:
		std::atomic<T>		_atomic;


	// methods
	public:
		forceinline Atomic (GX_DEFCTOR) : _atomic(0) {}

		forceinline explicit Atomic (const T &val) : _atomic(val) {}
		forceinline explicit Atomic (const Self &other) : _atomic(other.Get()) {}
		forceinline explicit Atomic (Self &&other) : _atomic(other.Get()) {}

		forceinline ND_ explicit	operator const T ()	const		{ return Get(); }
		forceinline ND_ const T		Get ()				const		{ return _atomic.load(); }

		forceinline const T	operator ++ ()							{ return ++_atomic; }
		forceinline const T	operator -- ()							{ return --_atomic; }
		forceinline const T	operator += (const T& right)			{ return _atomic += right; }
		forceinline const T	operator -= (const T& right)			{ return _atomic += right; }
		forceinline const T	operator =  (const T& right)			{ _atomic.exchange( right );  return right; }
		forceinline const T	operator |= (const T& right)			{ return _atomic |= right; }
		forceinline const T	operator &= (const T& right)			{ return _atomic &= right; }
		forceinline const T	operator ^= (const T& right)			{ return _atomic ^= right; }
		
		forceinline const T	Inc ()									{ return ++_atomic; }
		forceinline const T	Dec ()									{ return --_atomic; }
		forceinline const T	Add (const T& right)					{ return _atomic += right; }
		forceinline const T	Sub (const T& right)					{ return _atomic -= right; }
		forceinline const T	Set (const T& right)					{ _atomic.exchange( right );  return right; }
		forceinline const T	Or  (const T& right)					{ return _atomic |= right; }
		forceinline const T	And (const T& right)					{ return _atomic &= right; }
		forceinline const T	Xor (const T& right)					{ return _atomic ^= right; }

		forceinline const T	operator ++ (int)
		{
			const T  ret = Get();
			++(*this);
			return ret;
		}

		forceinline const T	operator -- (int)
		{
			const T  ret = Get();
			--(*this);
			return ret;
		}

		
		// if *this == compare then *this = val returns old *this
		forceinline const T CompareEx (const T& val, const T& compare)
		{
			T	tmp = compare;
			_atomic.compare_exchange_strong( tmp, val );
			return tmp;
		}

		_GX_DIM_CMP_OPERATORS_SELF( Get() );
		_GX_DIM_CMP_OPERATORS_TYPE( Get(), const T&, );
	};


}	// GXTypes
}	// GX_STL

#endif	// GX_ATOMIC_SUPPORTED
