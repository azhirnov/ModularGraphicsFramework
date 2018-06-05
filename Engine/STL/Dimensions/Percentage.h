// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/CompileTime/TemplateMath.h"
#include "Engine/STL/Math/MathTypes.h"

namespace GX_STL
{
namespace GXMath
{

	template <typename T> struct Percentage;	// literals:	pct
	
	template <typename T, usize I, ulong UID = 0>
	using PercentageVec = Vec< Percentage<T>, I, UID >;

	using Percents	= Percentage< real >;
	using Percents2	= PercentageVec< real, 2 >;
	using Percents3	= PercentageVec< real, 3 >;
	using Percents4	= PercentageVec< real, 4 >;



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
		using Self		= Percentage<T>;
		using Value_t	= T;


	// variables
	private:
		T	_value;


	// methods
	public:
		Percentage (GX_DEFCTOR) : _value(T(0)) {}

		explicit
		constexpr Percentage (T value) : _value(value)		{ /*ASSERT( value >= T(0) );*/ }

		ND_ T const&	ref ()		const					{ return _value; }
		ND_ T &			ref ()								{ return _value; }

		_GX_DIM_OPERATORS_SELF( +,	_value );
		_GX_DIM_OPERATORS_SELF( -,	_value );
		_GX_DIM_OPERATORS_SELF( *,	_value );
		_GX_DIM_OPERATORS_SELF( /,	_value );
		_GX_DIM_CMP_OPERATORS_SELF( _value );

		ND_ T	GetPercents ()		const					{ return _value; }					// 0..100%
		ND_ T	GetFraction ()		const					{ return _value * T(0.01); }		// 0..1

		template <typename B>
		ND_ B	Of (const B &value)	const					{ return B( value * GetFraction() ); }

		ND_ static constexpr Self	FromPercents (T value)	{ return Self( value ); }
		ND_ static constexpr Self	FromFraction (T value)	{ return Self( value * T(100.0) ); }

		ND_ static constexpr Self	Max ()					{ return Self( T(100.0) ); }
		ND_ static constexpr Self	Min ()					{ return Self( T(0.0) ); }
	};
	
	
	ND_ constexpr Percents operator "" _pct (long double value)			{ return Percents::FromPercents( real(value) ); }
	ND_ constexpr Percents operator "" _pct (unsigned long long value)	{ return Percents::FromPercents( real(value) ); }

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
			FLAGS	= int(TypeInfo< inner_type >::FLAGS) | int(_ctime_hidden_::WRAPPER),
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
	struct Hash< GXMath::Percentage<T> >
	{
		ND_ HashResult  operator () (const GXMath::Percentage<T> &x) const
		{
			return HashOf( x.GetPercents() );
		}
	};

}	// GXTypes
}	// GX_STL
