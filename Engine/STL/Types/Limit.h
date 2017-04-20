// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Math/Mathematics.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Limit Strategy
	//

	struct LimitStrategy
	{
		template <typename T>
		struct Clamp
		{
			forceinline static void Validate (OUT T &value, const T& min, const T& max)
			{
				value = GXMath::Clamp( value, min, max );
			}
		};

		template <typename T>
		struct Wrap
		{
			forceinline static void Validate (OUT T &value, const T& min, const T& max)
			{
				value = GXMath::Wrap( value, min, max );
			}
		};
	};

	
#	define _LIMIT_OPERATOR( _op_, _ret1_, _func1_, _ret2_, _func2_, _ret3_, _func3_, _ret4_, _func4_ ) \
		template <typename B>		 _ret1_ operator _op_##= (const B& right)					 _func1_ \
									 _ret2_ operator _op_    (const Self& right) const			 _func2_ \
		template <typename B>		 _ret3_ operator _op_    (const B& right) const				 _func3_ \
		template <typename B> friend _ret4_ operator _op_    (const B& left, const Self& right)	 _func4_ \

#	define _LIMIT_OPERATOR2( _op_ ) \
		_LIMIT_OPERATOR( _op_, \
				Self&,	{ _value _op_##= (T)right;  _Validate();  return *this; }, \
				Self,	{ return Self(*this) _op_##= right.Value(); }, \
				Self,	{ return Self(*this) _op_##= (T)right; }, \
				B,		{ return left _op_ (B)right.Value(); } \
		)


	//
	// Dynamic Limit
	//

	template <	typename T,
				template <typename> class S = LimitStrategy::Wrap
			 >
	struct Limit : public CompileTime::CopyQualifiers< T >
	{
	// types
	public:
		typedef Limit< T, S >		Self;
		typedef S< T >				Strategy;
		typedef T					value_t;


	// variables
	private:
		T		_value,
				_min,
				_max;


	// methods
	private:
		void _Validate ()
		{
			Strategy::Validate( _value, _min, _max );
		}

		void _Check () const
		{
			ASSUME( _min < _max and "incorrect values" );
		}

	public:
		Limit (GX_DEFCTOR) : _value(0), _min(0), _max(1)
		{}

		Limit (const T &value, const T &min, const T &max) :
			_value(value), _min(min), _max(max)
		{
			_Check();
			_Validate();
		}

		Limit (const Limit<T> &other) :
			_value(other._value), _min(other._min), _max(other._max)
		{
			_Check();
			_Validate();
		}

		Limit (Limit<T> &&other) :
			_value(other._value), _min(other._min), _max(other._max)
		{
			_Check();
			_Validate();
		}

		Self & SetLimits (const T min, const T max)
		{
			_min = min;
			_max = max;

			_Check();
			_Validate();

			return *this;
		}


		// type cast
		operator const T ()	const					{ return _value; }


		// get
		const T	Value ()	const					{ return _value; }
		const T	Min ()		const					{ return _min; }
		const T	Max ()		const					{ return _max; }
		

		// unary operators
		Self&	operator ++ ()						{ ++_value;  _Validate();  return *this; }
		Self&	operator -- ()						{ --_value;  _Validate();  return *this; }
		Self	operator ++ (int) 					{ Self ret(*this);  ++(*this);  return ret; }
		Self	operator -- (int)					{ Self ret(*this);  --(*this);  return ret; }
		Self	operator +  () const				{ return Self( +_value ); }
		Self	operator -  () const				{ return Self( -_value ); }
		bool	operator ! () const					{ return not _value; }
		Self	operator ~ () const					{ return Self( ~_value ); }

		
		// binary operators
		Self & operator = (const Self &right)		{ _value = right._value;  _Validate();  return *this; }
		Self & operator = (const T &right)			{ _value = right;  _Validate();  return *this; }

		_LIMIT_OPERATOR2( + );
		_LIMIT_OPERATOR2( - );
		_LIMIT_OPERATOR2( * );
		_LIMIT_OPERATOR2( / );
		_LIMIT_OPERATOR2( | );
		_LIMIT_OPERATOR2( & );
		_LIMIT_OPERATOR2( ^ );
		_LIMIT_OPERATOR2( >> );
		_LIMIT_OPERATOR2( << );

		_LIMIT_OPERATOR( %,
				Self&,	{ return _value = Mod( Value(), (T)right ); },
				Self,	{ return Self(*this) %= right.Value(); },
				Self,	{ return Self(*this) %= (T)right; },
				B,		{ return Mod( left, (B)right.Value() ); }
		);
	};


#	undef _LIMIT_OPERATOR2
#	undef _LIMIT_OPERATOR

	
	template <typename T, template <typename> class S>
	struct Hash< Limit< T, S > > :
		private Hash< typename Limit<T,S>::value_t >
	{
		typedef Limit< T, S >						key_t;
		typedef Hash< typename key_t::value_t >		base_t;
		typedef typename base_t::result_t			result_t;

		result_t operator () (const key_t &x) const
		{
			// TODO: is min and max limits needed here?
			return base_t::operator ()( x.Value() );
		}
	};

}	// GXTypes
}	// GX_STL
