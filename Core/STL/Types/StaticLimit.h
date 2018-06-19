// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Math/Mathematics.h"
#include "Limit.h"

namespace GX_STL
{
namespace GXTypes
{

	namespace _types_hidden_
	{
		template <typename T>
		using StaticLimitValueType = typename CompileTime::SwitchType< (CompileTime::IsUnsigned<T>),
																		typename CompileTime::NearUInt::FromType<T>,
																		typename CompileTime::NearInt::FromType<T> >;
	}
	
#	define _LIMIT_OPERATOR( _op_, _ret1_, _func1_, _ret2_, _func2_, _ret3_, _func3_, _ret4_, _func4_ ) \
		template <typename B>		 _ret1_ operator _op_##= (const B& right)					 _func1_ \
									 _ret2_ operator _op_    (const Self& right) const			 _func2_ \
		template <typename B>		 _ret3_ operator _op_    (const B& right) const				 _func3_ \
		template <typename B> friend _ret4_ operator _op_    (const B& left, const Self& right)	 _func4_ \

#	define _LIMIT_OPERATOR2( _op_ ) \
		_LIMIT_OPERATOR( _op_, \
				Self&,	{ _value _op_##= T(right);  _Validate();  return *this; }, \
				Self,	{ return Self(*this) _op_##= right.Value(); }, \
				Self,	{ return Self(*this) _op_##= T(right); }, \
				B,		{ return left _op_ B(right.Value()); } \
		)



	//
	// Static Limit
	//
	
	template <	typename T,
				_types_hidden_::StaticLimitValueType<T> MinLimit,
				_types_hidden_::StaticLimitValueType<T> MaxLimit,
				template <typename> class S = LimitStrategy::Wrap
			 >
	struct StaticLimit : public CompileTime::CopyQualifiers< T >
	{
	// types
	public:
		typedef StaticLimit< T, MinLimit, MaxLimit, S >	Self;
		typedef S< T >										Strategy_t;


	// variables
	private:
		T	_value;


	// methods
	private:
		void _Validate ()
		{
			Strategy_t::Validate( OUT _value, Min(), Max() );
		}

		STATIC_ASSERT( MinLimit < MaxLimit, "incorrect values" );


	public:
		StaticLimit (GX_DEFCTOR) : _value( MinLimit )				{}

		explicit
		StaticLimit (const T &val) : _value( val )					{ _Validate(); }

		StaticLimit (const Self &other) : _value( other._value )	{}
		StaticLimit (Self &&other) : _value( other._value )		{}

		template <	_types_hidden_::StaticLimitValueType<T> OtherMinValue,
					_types_hidden_::StaticLimitValueType<T> OtherMaxValue
				 >
		StaticLimit (const StaticLimit< T, OtherMinValue, OtherMaxValue > &other) : _value(other._value)	{ _Validate(); }

		// get
		ND_ constexpr static T	Max ()			{ return T(MaxLimit); }
		ND_ constexpr static T	Min ()			{ return T(MinLimit); }
		ND_ const			  T	Value () const	{ return _value; }
		

		// type cast
		ND_ operator const T ()	const			{ return _value; }


		// unary operators
			Self&	operator ++ ()				{ ++_value;  _Validate();  return *this; }
			Self&	operator -- ()				{ --_value;  _Validate();  return *this; }
			Self	operator ++ (int) 			{ Self ret(*this);  ++(*this);  return ret; }
			Self	operator -- (int)			{ Self ret(*this);  --(*this);  return ret; }
		ND_ Self	operator +  ()	const		{ return Self( +_value ); }
		ND_ Self	operator -  ()	const		{ return Self( -_value ); }
		ND_ bool	operator ! ()	const		{ return not _value; }
		ND_ Self	operator ~ ()	const		{ return Self( ~_value ); }


		// binary operators
		Self &	operator = (const Self &right)	{ _value = right._value;  return *this; }
		Self &	operator = (const T &right)		{ _value = right;  _Validate();  return *this; }

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
				Self&,	{ return _value = Mod( Value(), T(right) ); },
				Self,	{ return Self(*this) %= right.Value(); },
				Self,	{ return Self(*this) %= T(right); },
				B,		{ return Mod( left, B(right.Value()) ); }
		);
	};

#	undef _LIMIT_OPERATOR2
#	undef _LIMIT_OPERATOR


	template <typename T>
	using UNormClamped	= StaticLimit< T, 0, 1, LimitStrategy::Clamp >;
	
	template <typename T>
	using UNormWraped	= StaticLimit< T, 0, 1, LimitStrategy::Wrap >;

	template <typename T>
	using SNormClamped	= StaticLimit< T, -1, 1, LimitStrategy::Clamp >;
	
	template <typename T>
	using SNormWraped	= StaticLimit< T, -1, 1, LimitStrategy::Wrap >;

	
	template <typename T, T MinLimit, T MaxLimit, template <typename> class S>
	struct Hash< StaticLimit< T, MinLimit, MaxLimit, S > >
	{
		ND_ HashResult  operator () (const StaticLimit< T, MinLimit, MaxLimit, S > &x) const noexcept
		{
			// TODO: is min and max limits needed here?
			return HashOf( x.Value() );
		}
	};

}	// GXTypes
}	// GX_STL
