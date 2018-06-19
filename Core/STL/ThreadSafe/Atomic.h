// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/OS/Posix/AtomicOp.h"
#include "Core/STL/OS/Windows/AtomicOp.h"

# if not (defined( GX_ATOMIC_SUPPORTED ) and defined( GX_USE_STD ))

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

	private:
		using Op		= OS::AtomicOp;

		using ValidTypeList_t	= CompileTime::TypeListFrom< bool,
										byte, short, int, ilong, isize,
										ubyte, ushort, uint, ulong, usize >;
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

		ND_ forceinline explicit	operator const T ()	const		{ return Get(); }
		ND_ forceinline const T		Get ()				const		{ return Op::Get<T>( _value ); }

		forceinline const T	operator ++ ()							{ return Op::Inc<T>( _value ); }
		forceinline const T	operator -- ()							{ return Op::Dec<T>( _value ); }
		forceinline const T	operator += (const T& right)			{ return Op::Add<T>( _value, right ); }
		forceinline const T	operator -= (const T& right)			{ return Op::Sub<T>( _value, right ); }
		forceinline const T	operator =  (const T& right)			{ return Op::Set<T>( _value, right ); }
		forceinline const T	operator |= (const T& right)			{ return Op::Or<T> ( _value, right ); }
		forceinline const T	operator &= (const T& right)			{ return Op::And<T>( _value, right ); }
		forceinline const T	operator ^= (const T& right)			{ return Op::Xor<T>( _value, right ); }
		
		forceinline const T	Inc ()									{ return Op::Inc<T>( _value ); }
		forceinline const T	Dec ()									{ return Op::Dec<T>( _value ); }
		forceinline const T	Add (const T& right)					{ return Op::Add<T>( _value, right ); }
		forceinline const T	Sub (const T& right)					{ return Op::Sub<T>( _value, right ); }
		forceinline const T	Set (const T& right)					{ return Op::Set<T>( _value, right ); }
		forceinline const T	Or  (const T& right)					{ return Op::Or<T> ( _value, right ); }
		forceinline const T	And (const T& right)					{ return Op::And<T>( _value, right ); }
		forceinline const T	Xor (const T& right)					{ return Op::Xor<T>( _value, right ); }

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
			return Op::CmpExch<T>( _value, val, compare );
		}

		_GX_DIM_CMP_OPERATORS_SELF( Get() );
		_GX_DIM_CMP_OPERATORS_TYPE( Get(), const T&, );
	};

}	// GXTypes
}	// GX_STL
//-----------------------------------------------------------------------------


# elif 1
# include <atomic>

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
		volatile std::atomic<T>			_atomic;


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
//-----------------------------------------------------------------------------


# else

# include "Core/STL/OS/OSLowLevel.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Atomic (emulated)
	//

	template <typename T>
	struct Atomic final
	{
	// types
	public:
		using Value_t	= T;
		using Self		= Atomic<T>;

	private:
		using ValidTypeList_t	= CompileTime::TypeListFrom< bool,
										byte, short, int, ilong, isize,
										ubyte, ushort, uint, ulong, usize >;
		STATIC_ASSERT( ValidTypeList_t::HasType<T> );


	// variables
	private:
		volatile T				_value;
		mutable OS::Mutex		_mutex;


	// methods
	public:
		forceinline Atomic (GX_DEFCTOR): _value(0) {}

		forceinline explicit Atomic (const T &val) : _value(val) {}
		forceinline explicit Atomic (const Self &other) : _value(other.Get()) {}
		forceinline explicit Atomic (Self &&other) : _value(other.Get()) {}

		ND_ forceinline explicit	operator const T ()	const		{ return Get(); }
		ND_ forceinline const T		Get ()				const		{ _mutex.Lock();  T ret = _value;  _mutex.Unlock();  return ret; }

		forceinline const T	operator ++ ()							{ _mutex.Lock();  T ret = (++_value);         _mutex.Unlock();  return ret; }
		forceinline const T	operator -- ()							{ _mutex.Lock();  T ret = (--_value);         _mutex.Unlock();  return ret; }
		forceinline const T	operator ++ (int)						{ _mutex.Lock();  T ret = (_value++);         _mutex.Unlock();  return ret; }
		forceinline const T	operator -- (int)						{ _mutex.Lock();  T ret = (_value--);         _mutex.Unlock();  return ret; }
		forceinline const T	operator += (const T& right)			{ _mutex.Lock();  T ret = (_value += right);  _mutex.Unlock();  return ret; }
		forceinline const T	operator -= (const T& right)			{ _mutex.Lock();  T ret = (_value -= right);  _mutex.Unlock();  return ret; }
		forceinline const T	operator =  (const T& right)			{ _mutex.Lock();  T ret = (_value  = right);  _mutex.Unlock();  return ret; }
		forceinline const T	operator |= (const T& right)			{ _mutex.Lock();  T ret = (_value |= right);  _mutex.Unlock();  return ret; }
		forceinline const T	operator &= (const T& right)			{ _mutex.Lock();  T ret = (_value &= right);  _mutex.Unlock();  return ret; }
		forceinline const T	operator ^= (const T& right)			{ _mutex.Lock();  T ret = (_value ^= right);  _mutex.Unlock();  return ret; }
		
		forceinline const T	Inc ()									{ return ++(*this); }
		forceinline const T	Dec ()									{ return --(*this); }
		forceinline const T	Add (const T& right)					{ return (*this += right); }
		forceinline const T	Sub (const T& right)					{ return (*this -= right); }
		forceinline const T	Set (const T& right)					{ return (*this  = right); }
		forceinline const T	Or  (const T& right)					{ return (*this |= right); }
		forceinline const T	And (const T& right)					{ return (*this &= right); }
		forceinline const T	Xor (const T& right)					{ return (*this ^= right); }

		// if *this == compare then *this = val returns old *this
		forceinline T CompareEx (const T& val, const T& compare)
		{
			_mutex.Lock();
			T ret = _value;

			if ( _value == compare )
				_value = val;

			_mutex.Unlock();
			return ret;
		}

		_GX_DIM_CMP_OPERATORS_SELF( Get() );
		_GX_DIM_CMP_OPERATORS_TYPE( Get(), const T&, );
	};
	
}	// GXTypes
}	// GX_STL
//-----------------------------------------------------------------------------

# endif	// GX_USE_STD
