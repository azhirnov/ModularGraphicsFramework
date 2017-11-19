// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/OS/Posix/AtomicOp.h"
#include "Engine/STL/OS/Windows/AtomicOp.h"

#ifdef GX_USE_STD
#	include <atomic>
#endif

namespace GX_STL
{
namespace GXTypes
{

# ifndef GX_USE_STD
	

	//
	// Atomic
	//

	template <typename T>
	struct Atomic
	{
	// types
	public:
		typedef T				Value_t;
		typedef Atomic<T>		Self;

	private:
		typedef OS::AtomicOp	Op;

		typedef CompileTime::TypeListFrom< bool,
					byte, short, int, ilong, isize,
					ubyte, ushort, uint, ulong, usize >		ValidTypeList_t;

		STATIC_ASSERT( ValidTypeList_t::HasType<T> );


	// variables
	private:
		volatile T			_value;


	// methods
	public:
		forceinline Atomic (GX_DEFCTOR): _value(0) {}

		forceinline explicit Atomic (const T &val) : _value(val) {}
		forceinline explicit Atomic (const Self &other) : _value(other.Get()) {}
		forceinline explicit Atomic (Self &&other) : _value(other.Get()) {}

		forceinline explicit operator T () const			{ return Get(); }

		forceinline const T	operator ++ ()					{ return Op::Inc<T>( _value ); }
		forceinline const T	operator -- ()					{ return Op::Dec<T>( _value ); }
		forceinline const T	operator += (const T& right)	{ return Op::Add<T>( _value, right ); }
		forceinline const T	operator -= (const T& right)	{ return Op::Sub<T>( _value, right ); }
		forceinline const T	operator =  (const T& right)	{ return Op::Set<T>( _value, right ); }
		forceinline const T	operator |= (const T& right)	{ return Op::Or<T> ( _value, right ); }
		forceinline const T	operator &= (const T& right)	{ return Op::And<T>( _value, right ); }
		forceinline const T	operator ^= (const T& right)	{ return Op::Xor<T>( _value, right ); }
		
		forceinline const T	Inc ()							{ return Op::Inc<T>( _value ); }
		forceinline const T	Dec ()							{ return Op::Dec<T>( _value ); }
		forceinline const T	Get ()	const					{ return Op::Get<T>( _value ); }
		forceinline const T	Add (const T& right)			{ return Op::Add<T>( _value, right ); }
		forceinline const T	Sub (const T& right)			{ return Op::Sub<T>( _value, right ); }
		forceinline const T	Set (const T& right)			{ return Op::Set<T>( _value, right ); }
		forceinline const T	Or  (const T& right)			{ return Op::Or<T> ( _value, right ); }
		forceinline const T	And (const T& right)			{ return Op::And<T>( _value, right ); }
		forceinline const T	Xor (const T& right)			{ return Op::Xor<T>( _value, right ); }

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
		forceinline T CompareEx (const T& val, const T& compare)
		{
			return Op::CmpExch<T>( _value, val, compare );
		}

		_GX_DIM_CMP_OPERATORS_SELF( Get() );
		_GX_DIM_CMP_OPERATORS_TYPE( Get(), const T&, );
	};

# else
	

	//
	// Atomic
	//

	template <typename T>
	struct Atomic
	{
	// types
	public:
		typedef T				Value_t;
		typedef Atomic<T>		Self;

	private:
		typedef CompileTime::TypeListFrom< bool,
					byte, short, int, ilong, isize,
					ubyte, ushort, uint, ulong, usize >		ValidTypeList_t;

		STATIC_ASSERT( ValidTypeList_t::HasType<T> );


	// variables
	private:
		volatile std::atomic<T>			_atomic;

		//static const std::memory_order	_memoryOrder = std::memory_order_seq_cst;


	// methods
	public:
		Atomic (GX_DEFCTOR) : _atomic(0) {}

		explicit Atomic (T val) : _atomic(val) {}
		explicit Atomic (const Self &sObj) : _atomic(sObj.Get()) {}

		operator const volatile T () const		{ return Get(); }

		Self &	operator ++ ()					{ ++_atomic;				return *this; }
		Self &	operator -- ()					{ --_atomic;				return *this; }
		Self &	operator += (const T& right)	{ Add( right );				return *this; }
		Self &	operator -= (const T& right)	{ Sub( right );				return *this; }
		Self &	operator =  (const T& right)	{ _atomic.store( right );	return *this; }
		Self &	operator |= (const T& right)	{ Or(  right );				return *this; }
		Self &	operator &= (const T& right)	{ And( right );				return *this; }
		Self &	operator ^= (const T& right)	{ Xor( right );				return *this; }
		
		T		Inc ()							{ return Add( 1 ); }
		T		Dec ()							{ return Sub( 1 ); }
		T		Get ()	const					{ return _atomic.load(); }
		T		Add (const T& right)			{ return _atomic.fetch_add( right ); }
		T		Sub (const T& right)			{ return _atomic.fetch_sub( right ); }
		T		Set (const T& right)			{ return _atomic.exchange( right ); }
		T		Or  (const T& right)			{ return _atomic.fetch_or( right ); }
		T		And (const T& right)			{ return _atomic.fetch_and( right ); }
		T		Xor (const T& right)			{ return _atomic.fetch_xor( right ); }

		Self	operator ++ (int)
		{
			const Self  ret( _atomic );
			++(*this);
			return ret;
		}

		Self	operator -- (int)
		{
			const Self  ret( _atomic );
			--(*this);
			return ret;
		}


		// if *this == compare then *this = val return old *this
		//T CompareEx (const T& val, const T& compare)
		//{
		//	return _atomic.compare_exchange_strong( val, compare );
		//}
	};

# endif	// GX_USE_STD

}	// GXTypes
}	// GX_STL
