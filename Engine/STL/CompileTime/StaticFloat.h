// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "StaticInteger.h"
#include "NewTypeInfo.h"
#include "Engine/STL/Containers/String.h"

#ifdef GX_PHYSICS_DIMENSIONS_ENABLED

namespace GX_STL
{
namespace CompileTime
{
	
	namespace _ctime_hidden_
	{
		template <bool Condition, typename T, T Left, T Right, T Default = T(0)>
		struct AddIf {
			static const T	value = Left + Right;
		};
	
		template <typename T, T Left, T Right, T Default>
		struct AddIf< false, T, Left, Right, Default > {
			static const T	value = Default;
		};


		template <bool Condition, typename T, T Left, T Right, T Default = T(0)>
		struct SubIf {
			static const T	value = Left - Right;
		};
	
		template <typename T, T Left, T Right, T Default>
		struct SubIf< false, T, Left, Right, Default > {
			static const T	value = Default;
		};
	

		template <bool Condition, typename T, T Left, T Right, T Default = T(0)>
		struct DivIf {
			static const T	value = Left / Right;
		};
	
		template <typename T, T Left, T Right, T Default>
		struct DivIf< false, T, Left, Right, Default > {
			static const T	value = Default;
		};


		template <typename MT, typename ET, MT M, ET E>
		struct StaticFloat_RoundToInt {
			// if Exp < 0 then calc rounding correction
			static const ET		_rounding_exp = Max< ET, -E-1, 0 >;
			static const bool	_rounding	= E < 0 ? (Abs< MT, M > / Pow< ET, 10, _rounding_exp >) % 10 >= MT(5) : 0;
			static const MT		_pow		= Pow< MT, 10, (Max< ET, E, 0 >) >;
			static const MT		value		= (M / Pow< ET, 10, (Max< ET, -E, 0 >) >) * _pow + ( MT(_rounding) * Sign< MT, M > );
		};
   
		template <typename T, T Value, int Power>
		struct StaticFloat_CanPow10 {
			static const bool	value = StaticInteger_CanPowRecursive< T, (Abs< T, Value >), 10, (Max< int, Power-1, 0 >) >::value;
		};
		

		template <typename T, int Power, bool Overflow>
		struct StaticFloat_SafePow10_Impl {
			static const T	value = Pow< T, 10, Power >;
		};
			
		template <typename T, int Power>
		struct StaticFloat_SafePow10_Impl< T, Power, true > {
			static const T	value = 1;
		};
		
		template <typename T, int Power>
		struct StaticFloat_SafePow10 {
			static const bool	overflow = not StaticFloat_CanPow10< T, 10, Power >::value;
			static const T		value	 = StaticFloat_SafePow10_Impl< T, Power, overflow >::value;
		};


		template <typename MT, MT M, typename ET, ET EDiff>
		struct StaticFloat_CalcBaseExponent_DiffOverflow
		{
			typedef StaticFloat_SafePow10< MT, EDiff >	pow10;

			static const bool	value =	not ( StaticFloat_CanPow10< MT, M, EDiff >::value and
											  not pow10::overflow and
											  StaticInteger_CanMulWithSign< MT, M, pow10::value >::value );
		};

		template <typename MT, MT M0, MT M1, typename ET, ET EDiff>
		struct StaticFloat_CalcBaseExponent_Overflow
		{
			static const bool	value = typename SwitchType< EDiff >= 0,
													StaticFloat_CalcBaseExponent_DiffOverflow< MT, M0, ET,  EDiff >,
													StaticFloat_CalcBaseExponent_DiffOverflow< MT, M1, ET, -EDiff > >::value;
		};


		template <typename MT, MT M0, MT M1, typename ET, ET E0, ET E1>
		struct StaticFloat_OpData
		{
			typedef MT	mantissa_t;
			typedef ET	exponent_t;

			static const ET		e0	= E0;
			static const ET		e1	= E1;
			static const MT		m0	= M0;
			static const MT		m1	= M1;
		};


		template <typename OpData>
		struct StaticFloat_BaseExponentData
		{
			typedef typename OpData::exponent_t		ET;
			typedef typename OpData::mantissa_t		MT;

			static const ET		e	= Min< ET, OpData::e0, OpData::e1 >;
			static const MT		m0	= OpData::m0 * Pow< ET, 10, OpData::e0-e >;
			static const MT		m1	= OpData::m1 * Pow< ET, 10, OpData::e1-e >;

			typedef StaticFloat_OpData< MT, m0, m1, ET, e, e >	type;
		};


		template <typename MT, MT M0, MT M1, typename ET, ET E0, ET E1, bool Overflow>
		struct StaticFloat_CalcBaseExponentImpl
		{
			static const ET		eDiff	= E0 - E1;
			static const ET		diff	= ET( eDiff == 0 );

			static const ET		e0		= E0 + (eDiff < 0 ? 1 : diff);
			static const ET		e1		= E1 + (eDiff > 0 ? 1 : diff);

			static const MT		m0		= StaticFloat_RoundToInt< MT, ET, M0, E0-e0 >::value;
			static const MT		m1		= StaticFloat_RoundToInt< MT, ET, M1, E1-e1 >::value;

			typedef typename StaticFloat_CalcBaseExponentImpl<
						MT, m0, m1,  ET, e0, e1,
						StaticFloat_CalcBaseExponent_Overflow< MT, m0, m1, ET, e0 - e1 >::value
					>::type		type;
		};

		template <typename MT, MT M0, MT M1, typename ET, ET E0, ET E1>
		struct StaticFloat_CalcBaseExponentImpl< MT, M0, M1, ET, E0, E1, false > {
			typedef StaticFloat_OpData< MT, M0, M1, ET, E0, E1 >	type;
		};

		template <typename MT, MT M0, MT M1, typename ET, ET E0, ET E1>
		struct StaticFloat_CalcBaseExponent {
			typedef typename StaticFloat_CalcBaseExponentImpl< MT, M0, M1, ET, E0, E1,
						StaticFloat_CalcBaseExponent_Overflow< MT, M0, M1, ET, E0 - E1 >::value >::type		type;
		};


		template <typename MT, MT M0, MT M1, typename ET, ET E, bool Overflow>
		struct StaticFloat_CalcBaseExponentForAddImpl
		{
			static const MT	m0 = StaticFloat_RoundToInt< MT, ET, M0, -1 >::value;
			static const MT	m1 = StaticFloat_RoundToInt< MT, ET, M1, -1 >::value;

			typedef typename StaticFloat_CalcBaseExponentForAddImpl<
						MT, m0, m1, ET, E+1,
						not StaticInteger_CanAddWithSign< MT, m0, m1 >::value >::type	type;
		};
		
		template <typename MT, MT M0, MT M1, typename ET, ET E>
		struct StaticFloat_CalcBaseExponentForAddImpl< MT, M0, M1, ET, E, false > {
			typedef StaticFloat_OpData< MT, M0, M1, ET, E, E >	type;
		};
		
		template <typename MT, MT M0, MT M1, typename ET, ET E0, ET E1>
		struct StaticFloat_CalcBaseExponentForAdd {
			typedef StaticFloat_BaseExponentData<
				typename StaticFloat_CalcBaseExponent< MT, M0, M1, ET, E0, E1 >::type >				B;

			typedef typename StaticFloat_CalcBaseExponentForAddImpl< MT, B::m0, B::m1, ET, B::e,
						not StaticInteger_CanAddWithSign< MT, B::m0, B::m1 >::value >::type			type;
		};


		template <typename MT, MT M0, MT M1, typename ET, ET E0, ET E1, bool Overflow>
		struct StaticFloat_CalcBaseForMulImpl
		{
			static const MT	m0 = StaticFloat_RoundToInt< MT, ET, M0, -1 >::value;
			static const MT	m1 = StaticFloat_RoundToInt< MT, ET, M1, -1 >::value;

			typedef typename StaticFloat_CalcBaseForMulImpl<
						MT, m0, m1, ET, E0+1, E1+1,
						not StaticInteger_CanMulWithSign< MT, m0, m1 >::value >::type	type;
		};
		
		template <typename MT, MT M0, MT M1, typename ET, ET E0, ET E1>
		struct StaticFloat_CalcBaseForMulImpl< MT, M0, M1, ET, E0, E1, false > {
			typedef StaticFloat_OpData< MT, M0, M1, ET, E0, E1 >	type;
		};
		
		template <typename MT, MT M0, MT M1, typename ET, ET E0, ET E1>
		struct StaticFloat_CalcBaseForMul {
			typedef typename StaticFloat_CalcBaseForMulImpl< MT, M0, M1, ET, E0, E1,
						not StaticInteger_CanMulWithSign< MT, M0, M1 >::value >::type		type;
		};

		
		template <typename MT, MT Num, MT Den, MT Div, typename ET, ET E, bool Overflow = false>
		struct StaticFloat_CalcBaseForDivImpl
		{
			static const bool	_cm  = StaticInteger_CanMulWithSign< MT, Den, 10 >::value;
			static const MT		_d10 = MulIf< _cm, MT, Den, 10 >::value;
			static const MT		_n10 = Num * 10;
			static const MT		_dd  = _d10 / Div;
			static const bool	_b	 = ( (Den == 0) or
										 not StaticInteger_CanAddWithSign< MT, _n10, _dd >::value or
										 not StaticInteger_CanMulWithSign< MT, _n10, 10 >::value );
			static const MT		_n	 = _n10 + _dd;
			static const MT		_d	 = _d10 % Div;
			
			STATIC_ASSERT( _dd < 10 );

			typedef typename StaticFloat_CalcBaseForDivImpl< MT, _n, _d, Div, ET, E-1, _b >::type		type;

			STATIC_ASSERT( type::m0 != 0 );
		};
		
		template <typename MT, MT Num, MT Den, MT Div, typename ET, ET E>
		struct StaticFloat_CalcBaseForDivImpl< MT, Num, Den, Div, ET, E, true > {
			typedef StaticFloat_OpData< MT, Num, 0, ET, E, 0 >	type;	// TODO: round Den to 1 or 0
		};
		
		template <typename MT, MT Num, MT Den, MT Div, typename ET, ET E>
		struct StaticFloat_CalcBaseForDivImpl2 {
			static const bool	_b = (Den == 0) or not StaticInteger_CanMulWithSign< MT, Num, 10 >::value;
			typedef typename StaticFloat_CalcBaseForDivImpl< MT, Num, Den, Div, ET, E, _b >::type	type;
		};

		template <typename MT, MT M0, MT M1, typename ET, ET E0, ET E1, bool Over = false>
		struct StaticFloat_CalcBaseForDivImpl3 {
			static const bool	b0 = StaticInteger_CanMulWithSign< MT, M0, 10 >::value;
			static const bool	b1 = not b0;
			static const MT		m0 = MulIf< b0, MT, M0, 10, M0 >::value;
			static const ET		e0 = SubIf< b0, ET, E0,  1, E0 >::value;
			static const MT		m1 = DivIf< b1, MT, M1, 10, M1 >::value;
			static const ET		e1 = AddIf< b1, ET, E1,  1, E1 >::value;
			typedef typename StaticFloat_CalcBaseForDivImpl3< MT, m0, m1, ET, e0, e1, ((m0 / m1) > 0) >::type	type;
		};

		template <typename MT, MT M0, MT M1, typename ET, ET E0, ET E1>
		struct StaticFloat_CalcBaseForDivImpl3 <MT, M0, M1, ET, E0, E1, true> {
			typedef typename StaticFloat_CalcBaseForDivImpl2< MT, M0 / M1, M0 % M1, M1, ET, E0 - E1 >::type		type;
		};

		template <typename MT, MT M0, MT M1, typename ET, ET E0, ET E1>
		struct StaticFloat_CalcBaseForDiv
		{
			static const MT		m0 = Abs< MT, M0 >;
			static const MT		m1 = Abs< MT, M1 >;
			static const MT		s  = Sign< MT, M0 > * Sign< MT, M1 >;
			
			//typedef typename StaticFloat_CalcBaseForDivImpl2< MT, m0/m1, m0%m1, m1, ET, E0 - E1 >::type	B;
			typedef typename StaticFloat_CalcBaseForDivImpl3< MT, m0, m1, ET, E0, E1, (m0 / m1 > 0) >::type	B;
			typedef StaticFloat_OpData< MT, B::m0 * s, 0, ET, B::e0, 0 >									type;
		};


		template <typename MT, typename ET, MT M, ET E, bool B>
		struct StaticFloat_SimplifyImpl {
			static const MT	m = M / 10;
			typedef typename StaticFloat_SimplifyImpl< MT, ET, m, E+1, ((Abs< MT, m >) > 0 and m % 10 == 0) >::type	type;
		};

		template <typename MT, typename ET, MT M, ET E>
		struct StaticFloat_SimplifyImpl< MT, ET, M, E, false > {
			typedef StaticFloat_OpData< MT, M, M, ET, E, E >	type;
		};
			
		template <typename MT, typename ET, MT M, ET E>
		struct StaticFloat_Simplify {
			typedef typename StaticFloat_SimplifyImpl< MT, ET, M, E, ((Abs< MT, M >) > 0 and M % 10 == 0) >::type		type;
		};


		template <typename T, typename Base, int Power>
		struct StaticFloat_PowPositive {
			STATIC_ASSERT( Power > 0 );
			typedef typename StaticFloat_PowPositive< typename T::template Mul< Base >, Base, Power - 1 >::type		type;
		};
	
		template <typename T, typename Base>
		struct StaticFloat_PowPositive< T, Base, 0 > {
			typedef T	type;
		};
		
		
		template <typename T, typename One, int Power>
		struct StaticFloat_PowNegative {
			STATIC_ASSERT( Power > 0 );
			typedef typename One::template Div< typename StaticFloat_PowPositive< T, T, Power >::type >		type;
		};


		template <typename T, typename One, int Power>
		struct StaticFloat_Pow {
			typedef typename SwitchType< (Power > 0),
						StaticFloat_PowPositive< T, T, (Power-1) >,
						StaticFloat_PowNegative< T, One, (-Power-1) > >::type	type;
		};
		

		template <int I>
		struct _TPowerStructUnsigned {
			template <typename T>	forceinline static constexpr T power (const T& x)		{ return _TPowerStructUnsigned<I-1>::power( x ) * x; }
		};
			
		template <>
		struct _TPowerStructUnsigned<0> {
			template <typename T>	forceinline static constexpr T power (const T&)			{ return 1; }
		};

		template <bool S>
		struct _TPowerStruct {
			template <typename T, int I> forceinline static constexpr T power (const T& x)	{ return _TPowerStructUnsigned<I>::power( x ); }
		};

		template <>
		struct _TPowerStruct<true> {
			template <typename T, int I> forceinline static constexpr T power (const T& x)	{ return T(1) / _TPowerStructUnsigned<-I>::power( x ); }
		};

		template <int I, typename T>
		forceinline constexpr T StaticPow (const T& x)
		{
			return _TPowerStruct< (I<0) >::template power<T,I>( x );
		}

	}	// _ctime_hidden_



	//
	// Static Float
	//

	template <typename MantissaType, typename ExponentType, MantissaType Mantissa, ExponentType Exponent10 = 0>
	struct StaticFloatPoint : public Noninstancable
	{
	// types
	public:
		typedef StaticFloatPoint< MantissaType, ExponentType, Mantissa, Exponent10 >	Self;

		typedef typename NearFloat::FromType< MantissaType >	float_t;

		typedef MantissaType		mantissa_t;
		typedef ExponentType		exponent_t;

		static const mantissa_t		MANTISSA	= Mantissa;
		static const exponent_t		EXPONENT	= Exponent10;


	// types (methods)
	public:
		template <typename StaticFloatValue>
		struct _Add {
			typedef typename _ctime_hidden_::StaticFloat_CalcBaseExponentForAdd<
								mantissa_t, MANTISSA, StaticFloatValue::MANTISSA,
								exponent_t, EXPONENT, StaticFloatValue::EXPONENT >::type	_B;
			STATIC_ASSERT( _B::e0 == _B::e1 );
			
			typedef StaticFloatPoint< mantissa_t, exponent_t, _B::m0 + _B::m1, _B::e0 >			type;
		};

		template <typename StaticFloatValue>
		struct _Mul {
			typedef typename _ctime_hidden_::StaticFloat_CalcBaseForMul<
								mantissa_t, MANTISSA, StaticFloatValue::MANTISSA,
								exponent_t, EXPONENT, StaticFloatValue::EXPONENT >::type	_B;

			typedef StaticFloatPoint< mantissa_t, exponent_t, _B::m0 * _B::m1, _B::e0 + _B::e1 >	type;
		};

		template <typename StaticFloatValue>
		struct _Div {
			typedef typename StaticFloatValue::_Simplify::type					SF;
			STATIC_ASSERT( SF::MANTISSA != 0 );

			typedef typename _ctime_hidden_::StaticFloat_CalcBaseForDiv<
								mantissa_t, MANTISSA, SF::MANTISSA,
								exponent_t, EXPONENT, SF::EXPONENT >::type	_B;
			STATIC_ASSERT( (_B::m0 != 0) == (MANTISSA != 0) );

			typedef typename StaticFloatPoint< mantissa_t, exponent_t, _B::m0, _B::e0 >::_Simplify::type	type;
		};

		template <exponent_t ToPower>
		struct _Pow {
			typedef StaticFloatPoint< mantissa_t, exponent_t, 1, 0 >	one_t;

			typedef typename SwitchType< (ToPower == 0),
						typename one_t::_Simplify,
						typename _ctime_hidden_::StaticFloat_Pow< Self, one_t, ToPower > >::type	type;
		};

		struct _Simplify {
			typedef typename _ctime_hidden_::StaticFloat_Simplify< mantissa_t, exponent_t, MANTISSA, EXPONENT >::type	_B;
			typedef StaticFloatPoint< mantissa_t, exponent_t, _B::m0, _B::e0 >		type;
		};


	public:
		struct IsZero {
			static const bool	value	= ( MANTISSA == 0 );
		};

		struct IsOne {
			static const bool	value	= ( MANTISSA == 1 and Abs< exponent_t, EXPONENT > <= 1 );
		};

		template <typename StaticFloatValue>
		using Add = typename _Add< StaticFloatValue >::type;

		template <typename StaticFloatValue>
		using Sub = typename Add< typename StaticFloatValue::Neg >;

		template <typename StaticFloatValue>
		using Mul = typename _Mul< StaticFloatValue >::type;
		
		template <typename StaticFloatValue>
		using Div = typename _Div< StaticFloatValue >::type;

		using Neg = StaticFloatPoint< mantissa_t, exponent_t, 0-MANTISSA, EXPONENT >;

		struct Inv {
			typedef typename StaticFloatPoint< mantissa_t, exponent_t, 1, 0 >::template Div< Self >		type;
		};

		template <exponent_t ToPower>
		using Pow = typename _Pow<ToPower>::type;

		template <exponent_t ToPower>
		using Pow10 = StaticFloatPoint< mantissa_t, exponent_t, MANTISSA, EXPONENT + ToPower >;

		using Simplify = typename _Simplify::type;

		template <typename IntType>
		struct Round {
			static const IntType	value = _ctime_hidden_::StaticFloat_RoundToInt< IntType, exponent_t, MANTISSA, EXPONENT >::value;
		};

		template <typename StaticFloatValue>
		struct Equal {
			static const bool	value = Sub< StaticFloatValue >::IsZero::value;
		};

		template <typename StaticFloatValue>
		struct FastEqual {
			typedef typename Simplify						t0;
			typedef typename StaticFloatValue::Simplify		t1;
			static const bool	value = (t0::MANTISSA == t1::MANTISSA and t0::EXPONENT == t1::EXPONENT);
		};

		template <typename StaticFloatValue>
		struct NotEqual {
			static const bool	value = not Equal< StaticFloatValue >::value;
		};

		template <typename StaticFloatValue>
		struct LessThan {
			static const bool	value = Sub< StaticFloatValue >::MANTISSA < 0;
		};

		template <typename StaticFloatValue>
		struct GreaterThan {
			static const bool	value = Sub< StaticFloatValue >::MANTISSA > 0;
		};

		template <typename StaticFloatValue>
		struct LessThanEqual {
			typedef typename Sub< StaticFloatValue >	sub;
			static const bool	value = sub::IsZero::value or sub::MANTISSA < 0;
		};

		template <typename StaticFloatValue>
		struct GreaterThanEqual {
			typedef typename Sub< StaticFloatValue >	sub;
			static const bool	value = sub::IsZero::value or sub::MANTISSA > 0;
		};

		template <typename MT, typename ET>
		using To = StaticFloatPoint< MT, ET, MANTISSA, EXPONENT >;


	// static methods
	public:
		template <typename FT>
		static FT ToFloat ()
		{
			return FT(MANTISSA) * _ctime_hidden_::StaticPow<EXPONENT>( FT(10) );
		};

		static String ToString ()
		{
			return String().FormatI( MANTISSA ) << "*10^" << String().FormatI( EXPONENT );
		}

		
	// methods
	public:
		operator float_t () const
		{
			return ToFloat< float_t >();
		}
	};

	
	template <int Mantissa, byte Exponent10 = 0>
	using StaticFloat = StaticFloatPoint< int, byte, Mantissa, Exponent10 >;

	template <ilong Mantissa, short Exponent10 = 0>
	using StaticDouble = StaticFloatPoint< ilong, short, Mantissa, Exponent10 >;

	template <typename T, typename NearInt::FromType< T > Mantissa, typename NearInt::FromSize< sizeof(T)/2 > Exponent10 = 10>
	using StaticFloatFromType = StaticFloatPoint< typename NearInt::FromType< T >, typename NearInt::FromSize< sizeof(T)/2 >, Mantissa, Exponent10 >;


}	// CompileTime
}	// GX_STL

#endif	// GX_PHYSICS_DIMENSIONS_ENABLED
