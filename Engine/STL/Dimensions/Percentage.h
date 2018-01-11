// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/CompileTime/TemplateMath.h"
#include "Engine/STL/Math/MathTypes.h"

namespace GX_STL
{
namespace GXMath
{

	template <typename T> struct Percentage;	// literals:	pct

	typedef Percentage<real>	Percents;



	//
	// Percentage
	//

	template <typename T>
	struct Percentage :	public CompileTime::CopyQualifiers< T >
	{
		STATIC_ASSERT( CompileTime::IsFloat<T> and
					   CompileTime::IsScalar<T> );

	// types
	public:
		typedef Percentage<T>		Self;
		typedef T					Value_t;


	// variables
	private:
		T	_value;


	// methods
	public:
		Percentage (GX_DEFCTOR) : _value(T(0)) {}

		explicit
		constexpr Percentage (T value) : _value(value)	{ /*ASSERT( value >= T(0) );*/ }

		T const&	ref ()				const			{ return _value; }
		T &			ref ()								{ return _value; }

		_GX_DIM_ALL_FLOAT_OPERATORS_SELF( _value )

		T			GetPercents ()		const			{ return _value; }					// 0..100%
		T			GetFraction ()		const			{ return _value * T(0.01); }		// 0..1

		template <typename B>
		B			Of (const B &value)	const			{ return B( value * GetFraction() ); }

		static constexpr Self	FromPercents (T value)	{ return Self( value ); }
		static constexpr Self	FromFraction (T value)	{ return Self( value * T(100.0) ); }

		static constexpr Self	Max ()					{ return Self( T(100.0) ); }
		static constexpr Self	Min ()					{ return Self( T(0.0) ); }
	};
	
	
	constexpr Percents operator "" _pct (long double value)			{ return Percents::FromPercents( real(value) ); }
	constexpr Percents operator "" _pct (unsigned long long value)	{ return Percents::FromPercents( real(value) ); }

}	// GXMath


namespace CompileTime
{
/*
=================================================
	TypeInfo
=================================================
*/
	template <typename T>
	struct TypeInfo< GXMath::Percentage<T> >
	{
		typedef GXMath::Percentage<T>	type;
		typedef T						inner_type;

		template <typename OtherType>
		using CreateWith = GXMath::Percentage< OtherType >;

		enum {
			FLAGS	= (int)TypeInfo< inner_type >::FLAGS | int(_ctime_hidden_::WRAPPER),
		};

		static constexpr type	Max()		{ return type( TypeInfo< inner_type >::Max() ); }
		static constexpr type	Min()		{ return type( TypeInfo< inner_type >::Min() ); }
		static constexpr type	Inf()		{ return type( TypeInfo< inner_type >::Inf() ); }
		static constexpr type	NaN()		{ return type( TypeInfo< inner_type >::NaN() ); }

		static constexpr type	Epsilon()	{ return type( TypeInfo< inner_type >::Epsilon() ); }
		static constexpr uint	SignBit()	{ return TypeInfo< inner_type >::SignBit(); }
		static constexpr uint	Count()		{ return TypeInfo< inner_type >::Count(); }
	};

}	// CompileTime


namespace GXTypes
{
/*
=================================================
	Hash
=================================================
*/
	template <typename T>
	struct Hash< GXMath::Percentage<T> > : private Hash<T>
	{
		typedef GXMath::Percentage<T>		Key_t;
		typedef Hash<T>						Base_t;
		typedef typename Base_t::Result_t	Result_t;

		Result_t operator () (const Key_t &x) const
		{
			return Base_t::operator ()( x.GetPercents() );
		}
	};

}	// GXTypes
}	// GX_STL
