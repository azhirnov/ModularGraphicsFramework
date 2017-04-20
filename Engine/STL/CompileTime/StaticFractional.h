// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "TemplateMath.h"
#include "NewTypeInfo.h"
#include "Engine/STL/Containers/String.h"
#include "StaticFloat.h"
#include "Engine/STL/Types/Noninstancable.h"

#ifdef GX_PHYSICS_DIMENSIONS_ENABLED

namespace GX_STL
{
namespace CompileTime
{

	//
	// Fractional
	//

	template <typename T, T Numerator, T Denominator = 1>
	struct Fractional : public Noninstancable
	{
	// checks
		STATIC_ASSERT( Denominator > 0, "denominator must be positive no-zero value!" );


	// types
	private:
		static const T	_GCD = Abs< T, GreatestCommonDivisor< T, Numerator, Denominator > >;
		STATIC_ASSERT( _GCD > 0 );

	public:
		static const T	N	= Numerator / _GCD;
		static const T	D	= Denominator / _GCD;
		
		typedef T												value_t;
		typedef typename NearFloat::FromSize< sizeof(T)*2 >		float_t;
		typedef Fractional< T, Numerator, Denominator >			Self;
		typedef Fractional< T, N, D >							Simplified;


	// static values
	public:
		template <typename Frac>
		using Add = typename Fractional< T, N * Frac::D + Frac::N * D, D * Frac::D >::Simplified;
		
		template <typename Frac>
		using Sub = typename Fractional< T, N * Frac::D - Frac::N * D, D * Frac::D >::Simplified;

		using Neg = typename Fractional< T, 0-N, D >::Simplified;
		
		template <typename Frac>
		using Mul = typename Fractional< T, N * Frac::N, D * Frac::D >::Simplified;
		
		template <typename Frac>
		using Div = typename Fractional< T, N * Frac::D, D * Frac::N >::Simplified;

		template <uint ToPower>
		using Pow = typename Fractional< T, CompileTime::Pow< T, N, ToPower >, CompileTime::Pow< T, D, ToPower > >::Simplified;

		struct IsZero {
			static const bool	value = (N == 0);
		};

		struct IsInteger {
			static const bool	value = (D == 1);
		};

		struct Round {
			static const T		rounding = ((Abs< T, N > * 10) / D) % 10 >= 5;
			static const T		value	 = (N / D) + (rounding * Sign< T, N >);
		};

		template <typename Frac>
		struct Equal {
			static const bool	value = (N == 0 and Frac::N == 0) or (N == Frac::N and D == Frac::D);
		};

		template <T Value>
		struct EqualInt {
			static const bool	value = (N == Value and D == 1);
		};

		template <typename MT, typename ET>
		struct _ToStaticFloat {
			typedef StaticFloatPoint< MT, ET, N >	n;
			typedef StaticFloatPoint< MT, ET, D >	d;
			typedef typename n::template Div< d >	type;
		};

		template <typename MT, typename ET>
		using ToStaticFloat = typename _ToStaticFloat<MT, ET>::type;

		template <typename StaticFloatType>
		using ToStaticFloatType = typename ToStaticFloat< typename StaticFloatType::mantissa_t, typename StaticFloatType::exponent_t >;

		template <typename B>
		using To = Fractional< B, N, D >;


	// static methods
	public:
		template <typename FT>
		static FT ToFloat ()
		{
			return FT( N ) / FT( D );
		}

		static String ToString ()
		{
			String	s;
			s << String().FormatI( N, 10 );

			if ( N != 0 and D > 1 )
				s << '/' << String().FormatI( D, 10 );
			
			return s;
		}

	// methods
	public:
		operator float_t () const
		{
			return ToFloat< float_t >();
		}
	};


	template <short Numerator, short Denominator = 1>	using Fractional16 = Fractional< short, Numerator, Denominator >;
	template <int Numerator, int Denominator = 1>		using Fractional32 = Fractional< int, Numerator, Denominator >;
	template <ilong Numerator, ilong Denominator = 1>	using Fractional64 = Fractional< ilong, Numerator, Denominator >;


}	// CompileTime
}	// GX_STL

#endif	// GX_PHYSICS_DIMENSIONS_ENABLED
