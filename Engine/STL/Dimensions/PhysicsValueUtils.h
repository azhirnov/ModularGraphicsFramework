// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "PhysicsDimension.h"
#include "Engine/STL/CompileTime/StaticFloat.h"

#ifdef GX_PHYSICS_DIMENSIONS_ENABLED

namespace GX_STL
{
namespace GXMath
{

	namespace _math_hidden_
	{

		template <typename ValueType, typename Scale, typename Power>
		struct Dimensions_ConvertValue_ScalePower {
			static ValueType Get (const ValueType &value) {
				return value * Pow( Scale::ToFloat<ValueType>(), Power::ToFloat<ValueType>() );
			}
		};
		
		template <typename ValueType, typename Scale, typename Power>
		struct Dimensions_ConvertValue_ScaleStaticPower
		{
			STATIC_ASSERT( Power::IsInteger::value );

			typedef typename Scale::template Pow< Power::N >::type	scale_t;

			static ValueType Get (const ValueType &value) {
				return value * scale_t::ToFloat<ValueType>();
			}
		};

		template <typename ValueType, typename Scale>
		struct Dimensions_ConvertValue_Scale {
			static ValueType Get (const ValueType &value) {
				return value * Scale::ToFloat<ValueType>();
			}
		};
		
		template <typename ValueType>
		struct Dimensions_ConvertValue_Zero {
			static ValueType Get (const ValueType &value) {
				return ValueType(0);
			}
		};
		
		template <typename ValueType>
		struct Dimensions_ConvertValue_ValueOnly {
			static ValueType const & Get (const ValueType &value) {
				return value;
			}
		};

		template <typename ValueType, typename Scale, typename Power>
		struct Dimensions_ConvertValue
		{
			static const bool	ScaleIsZero	= Scale::IsZero::value;
			static const bool	ScaleIsOne	= Scale::template FastEqual< typename CompileTime::StaticFloat< 1 > >::value;

			static const bool	PowerIsZero	= Power::IsZero::value;
			static const bool	PowerIsOne	= Power::EqualInt< 1 >::value;

			static const int	Index		=	ScaleIsZero ? 0 :
												PowerIsZero ? 1 :
												ScaleIsOne  ? 2 :
												PowerIsOne  ? 3 :
												Power::IsInteger::value ? 4 :
												5;

			typedef typename CompileTime::TypeListFrom<
						Dimensions_ConvertValue_Zero< ValueType >,							// [0]  value = value * 0 ^ pow
						Dimensions_ConvertValue_ValueOnly< ValueType >,						// [1]  value = value * scale ^ 0
						Dimensions_ConvertValue_ValueOnly< ValueType >,						// [2]  value = value * 1 ^ pow
						Dimensions_ConvertValue_Scale< ValueType, Scale >,					// [3]  value = value * scale ^ 1
						Dimensions_ConvertValue_ScaleStaticPower< ValueType, Scale, Power >,// [4]  value = value * scale (^ pow)
						Dimensions_ConvertValue_ScalePower< ValueType, Scale, Power >		// [5]  value = value * scale ^ pow
					>													ConvesionsList_t;
						
			typedef typename ConvesionsList_t::template Get< Index >	Conversion;
		};


		template <typename ValueType, typename Scale, typename Power>
		struct DimensionConversion_AddEq
		{
			typedef Scale	scale_t;
			typedef Power	power_t;
			
			static ValueType Get (const ValueType &left, const ValueType &right) {
				return left + right;
			}
		};

		template <typename ValueType, typename LeftScale, typename LeftPower, typename RightScale, typename RightPower>
		struct DimensionConversion_Add
		{
			typedef typename CompileTime::StaticFloatPoint< typename LeftScale::mantissa_t, 
								typename LeftScale::exponent_t, 1, 0 >::Simplify							scale_t;

			typedef typename CompileTime::Fractional< typename LeftPower::value_t, 1, 1 >::Simplified		power_t;
			
			static ValueType Get (const ValueType &left, const ValueType &right)
			{
				typedef typename Dimensions_ConvertValue< ValueType, LeftScale, LeftPower >::Conversion		LFunc;
				typedef typename Dimensions_ConvertValue< ValueType, RightScale, RightPower >::Conversion	RFunc;

				return LFunc::Get( left ) + RFunc::Get( right );
			}
		};


		template <typename ValueType, typename Scale, typename Power>
		struct DimensionConversion_SubEq
		{
			typedef Scale	scale_t;
			typedef Power	power_t;
			
			static ValueType Get (const ValueType &left, const ValueType &right) {
				return left - right;
			}
		};

		template <typename ValueType, typename LeftScale, typename LeftPower, typename RightScale, typename RightPower>
		struct DimensionConversion_Sub
		{
			typedef typename CompileTime::StaticFloatPoint< typename LeftScale::mantissa_t, 
								typename LeftScale::exponent_t, 1, 0 >::Simplify							scale_t;

			typedef typename CompileTime::Fractional< typename LeftPower::value_t, 1, 1 >::Simplified		power_t;
			
			static ValueType Get (const ValueType &left, const ValueType &right)
			{
				typedef typename Dimensions_ConvertValue< ValueType, LeftScale, LeftPower >::Conversion		LFunc;
				typedef typename Dimensions_ConvertValue< ValueType, RightScale, RightPower >::Conversion	RFunc;

				return LFunc::Get( left ) - RFunc::Get( right );
			}
		};
		

		template <typename ValueType, typename LeftScale, typename LeftPower, typename RightScale, typename RightPower>
		struct DimensionConversion_MulEqScale
		{
			STATIC_ASSERT( LeftScale::template Equal< RightScale >::value );

			typedef LeftScale											scale_t;
			typedef typename LeftPower::template Add< RightPower >		power_t;

			static ValueType Get (const ValueType &left, const ValueType &right) {
				return left * right;
			}
		};

		template <typename ValueType, typename LeftScale, typename LeftPower, typename RightScale, typename RightPower>
		struct DimensionConversion_MulIntPow
		{
			STATIC_ASSERT( LeftPower::IsInteger::value and RightPower::IsInteger::value );

			typedef typename LeftScale::template Pow< LeftPower::Round::value >		lscale_t;
			typedef typename RightScale::template Pow< RightPower::Round::value >	rscale_t;

			typedef typename lscale_t::template Mul< rscale_t >											scale_t;
			typedef typename CompileTime::Fractional< typename LeftPower::value_t, 1, 1 >::Simplified	power_t;
			
			static ValueType Get (const ValueType &left, const ValueType &right) {
				return left * right;
			}
		};

		template <typename ValueType, typename LeftScale, typename LeftPower, typename RightScale, typename RightPower>
		struct DimensionConversion_Mul
		{
			typedef typename CompileTime::StaticFloatPoint< typename LeftScale::mantissa_t, 
								typename LeftScale::exponent_t, 1, 0 >::Simplify							scale_t;

			typedef typename CompileTime::Fractional< typename LeftPower::value_t, 1, 1 >::Simplified		power_t;

			static ValueType Get (const ValueType &left, const ValueType &right)
			{
				typedef typename Dimensions_ConvertValue< ValueType, LeftScale, LeftPower >::Conversion		LFunc;
				typedef typename Dimensions_ConvertValue< ValueType, RightScale, RightPower >::Conversion	RFunc;

				return LFunc::Get( left ) * RFunc::Get( right );
			}
		};
		

		template <typename ValueType, typename LeftScale, typename LeftPower, typename RightScale, typename RightPower>
		struct DimensionConversion_DivEqScale
		{
			STATIC_ASSERT( LeftScale::template FastEqual< RightScale >::value );

			typedef LeftScale										scale_t;
			typedef typename LeftPower::template Sub< RightPower >	power_t;

			static ValueType Get (const ValueType &left, const ValueType &right) {
				return left / right;
			}
		};

		template <typename ValueType, typename LeftScale, typename LeftPower, typename RightScale, typename RightPower>
		struct DimensionConversion_DivIntPow
		{
			STATIC_ASSERT( LeftPower::IsInteger::value and RightPower::IsInteger::value );

			typedef typename LeftScale::template Pow< LeftPower::Round::value >		lscale_t;
			typedef typename RightScale::template Pow< RightPower::Round::value >	rscale_t;

			typedef typename lscale_t::template Div< rscale_t >											scale_t;
			typedef typename CompileTime::Fractional< typename LeftPower::value_t, 1, 1 >::Simplified	power_t;
			
			static ValueType Get (const ValueType &left, const ValueType &right) {
				return left / right;
			}
		};

		template <typename ValueType, typename LeftScale, typename LeftPower, typename RightScale, typename RightPower>
		struct DimensionConversion_Div
		{
			typedef typename CompileTime::StaticFloatPoint< typename LeftScale::mantissa_t, 
								typename LeftScale::exponent_t, 1, 0 >::Simplify							scale_t;

			typedef typename CompileTime::Fractional< typename LeftPower::value_t, 1, 1 >::Simplified		power_t;

			static ValueType Get (const ValueType &left, const ValueType &right)
			{
				typedef typename Dimensions_ConvertValue< ValueType, LeftScale, LeftPower >::Conversion		LFunc;
				typedef typename Dimensions_ConvertValue< ValueType, RightScale, RightPower >::Conversion	RFunc;

				return LFunc::Get( left ) / RFunc::Get( right );
			}
		};
		

		template <typename Scale, typename Power>
		struct DimensionConversion_Simplify_Zero {
			typedef typename CompileTime::StaticFloatPoint< typename Scale::mantissa_t, typename Scale::exponent_t, 0, 0 >	scale_t;
			typedef typename CompileTime::Fractional< typename Power::value_t, 1, 1 >										power_t;
		};

		template <typename Scale, typename Power>
		struct DimensionConversion_Simplify_One {
			typedef typename CompileTime::StaticFloatPoint< typename Scale::mantissa_t, typename Scale::exponent_t, 1, 0 >	scale_t;
			typedef typename CompileTime::Fractional< typename Power::value_t, 1, 1 >										power_t;
		};

		template <typename Scale, typename Power>
		struct DimensionConversion_Simplify_None {
			typedef Scale	scale_t;
			typedef Power	power_t;
		};

		template <typename Scale, typename Power>
		struct DimensionConversion_Simplify
		{
			typedef typename CompileTime::StaticFloatPoint< typename Scale::mantissa_t, typename Scale::exponent_t, 1, 0 >	one_t;

			typedef typename CompileTime::SwitchType< (Scale::IsZero::value),
						DimensionConversion_Simplify_Zero< Scale, Power >,
						typename CompileTime::SwitchType< (Power::IsZero::value or Scale::template FastEqual< one_t >::value),
							DimensionConversion_Simplify_One< Scale, Power >,
							DimensionConversion_Simplify_None< Scale, Power >
						>
					>			type;
		};

	}	// _math_hidden_



	//
	// Physics Value Conversions
	//
	
	template <typename ValueType>
	struct PhysicsDimensionScale : public Noninstancable
	{
		static const int EXPONENT_TYPE_SIZEOF = CompileTime::Max< int, (sizeof(ValueType)>>2), 1 >;

		typedef typename CompileTime::NearInt::FromType< ValueType >			mantissa_t;
		typedef typename CompileTime::NearInt::FromSize< EXPONENT_TYPE_SIZEOF >	exponent_t;
		typedef mantissa_t														fractional_int_t;

		template <mantissa_t Mantissa, exponent_t Exponent = 0>
		struct Float : CompileTime::StaticFloatPoint< mantissa_t, exponent_t, Mantissa, Exponent >
		{};

		template <fractional_int_t Numerator, fractional_int_t Denominator = 1>
		struct TFractional : CompileTime::Fractional< fractional_int_t, Numerator, Denominator >
		{};


		// value * scale ^ pow
		template <mantissa_t ScaleMantissa,
				  exponent_t ScaleExponent = 0,
				  fractional_int_t PowNumerator = 1,
				  fractional_int_t PowDenominator = 1
				 >
		struct BaseConversion
		{
			typedef typename Float< ScaleMantissa, ScaleExponent >::Simplify									Scale;
			typedef typename TFractional< PowNumerator, PowDenominator >::Simplified							Power;
			typedef typename _math_hidden_::Dimensions_ConvertValue< ValueType, Scale, Power >::Conversion		Func;
			typedef BaseConversion< ScaleMantissa, ScaleExponent, PowNumerator, PowDenominator >				Self;

			template <typename Conv>
			struct Equal {
				static const bool	value = Scale::Equal< Conv::Scale >::value and
											Power::Equal< Conv::Power >::value;
			};

			struct _Simplify4 {
				typedef typename _math_hidden_::DimensionConversion_Simplify< Scale, Power >::type	_simp_t;
				typedef BaseConversion< _simp_t::scale_t::MANTISSA, _simp_t::scale_t::EXPONENT,
										_simp_t::power_t::N, _simp_t::power_t::D >					type;
			};

			using Simplify = typename _Simplify4::type;

			template <typename Conv>
			struct _Add4 {
				typedef typename Conv::Scale			rscale_t;
				typedef typename Conv::Power			rpower_t;

				typedef typename CompileTime::SwitchType< (Equal< Conv >::value),
							_math_hidden_::DimensionConversion_AddEq< ValueType, Scale, Power >,
							_math_hidden_::DimensionConversion_Add< ValueType, Scale, Power, rscale_t, rpower_t >
						>		add_op;
					
				typedef typename BaseConversion< add_op::scale_t::MANTISSA, add_op::scale_t::EXPONENT,
									add_op::power_t::N, add_op::power_t::D >::Simplify					type;

				static ValueType Get (const ValueType &left, const ValueType &right) {
					return add_op::Get( left, right );
				}
			};
			
			template <typename Conv>
			using Add = typename _Add4<Conv>::type;

			template <typename Conv>
			struct _Sub4 {
				typedef typename Conv::Scale			rscale_t;
				typedef typename Conv::Power			rpower_t;

				typedef typename CompileTime::SwitchType< (Equal< Conv >::value),
							_math_hidden_::DimensionConversion_SubEq< ValueType, Scale, Power >,
							_math_hidden_::DimensionConversion_Sub< ValueType, Scale, Power, rscale_t, rpower_t >
						>		sub_op;
					
				typedef typename BaseConversion< sub_op::scale_t::MANTISSA, sub_op::scale_t::EXPONENT,
									sub_op::power_t::N, sub_op::power_t::D >::Simplify					type;

				static ValueType Get (const ValueType &left, const ValueType &right) {
					return sub_op::Get( left, right );
				}
			};
			
			template <typename Conv>
			using Sub = typename _Sub4<Conv>::type;

			template <typename Conv>
			struct _Mul4 {
				typedef typename Conv::Scale			rscale_t;
				typedef typename Conv::Power			rpower_t;

				typedef typename CompileTime::SwitchType< (Scale::template FastEqual< rscale_t >::value),
							_math_hidden_::DimensionConversion_MulEqScale< ValueType, Scale, Power, rscale_t, rpower_t >,
							typename CompileTime::SwitchType< (Power::IsInteger::value and Conv::Power::IsInteger::value),
								_math_hidden_::DimensionConversion_MulIntPow< ValueType, Scale, Power, rscale_t, rpower_t >,
								_math_hidden_::DimensionConversion_Mul< ValueType, Scale, Power, rscale_t, rpower_t >
							>
						>		mul_op;

				typedef typename BaseConversion< mul_op::scale_t::MANTISSA, mul_op::scale_t::EXPONENT,
									mul_op::power_t::N, mul_op::power_t::D >::Simplify					type;

				static ValueType Get (const ValueType &left, const ValueType &right) {
					return mul_op::Get( left, right );
				}
			};
			
			template <typename Conv>
			using Mul = typename _Mul4<Conv>::type;

			template <typename Conv>
			struct _Div4 {
				typedef typename Conv::Scale			rscale_t;
				typedef typename Conv::Power			rpower_t;

				typedef typename CompileTime::SwitchType< (Scale::template FastEqual< rscale_t >::value),
							_math_hidden_::DimensionConversion_DivEqScale< ValueType, Scale, Power, rscale_t, rpower_t >,
							typename CompileTime::SwitchType< (Power::IsInteger::value and Conv::Power::IsInteger::value),
								_math_hidden_::DimensionConversion_DivIntPow< ValueType, Scale, Power, rscale_t, rpower_t >,
								_math_hidden_::DimensionConversion_Div< ValueType, Scale, Power, rscale_t, rpower_t >
							>
						>		div_op;
					
				typedef typename BaseConversion< div_op::scale_t::MANTISSA, div_op::scale_t::EXPONENT,
									div_op::power_t::N, div_op::power_t::D >::Simplify					type;

				static ValueType Get (const ValueType &left, const ValueType &right) {
					return div_op::Get( left, right );
				}
			};
			
			template <typename Conv>
			using Div = typename _Div4<Conv>::type;

			template <typename Conv>
			struct _UnitMul4 {
				STATIC_ASSERT( Power::IsInteger::value and Conv::Power::IsInteger::value );
				typedef typename _Mul4< Conv >::type	type;
			};
			
			template <typename Conv>
			using UnitMul = typename _UnitMul4<Conv>::type;

			template <typename Conv>
			struct _UnitDiv4 {
				STATIC_ASSERT( Power::IsInteger::value and Conv::Power::IsInteger::value );
				typedef typename _Div4< Conv >::type	type;
			};
			
			template <typename Conv>
			using UnitDiv = typename _UnitDiv4<Conv>::type;

			template <typename FracPow>
			struct _Pow4 {
				typedef typename Power::template Mul< FracPow >							pow_t;
				typedef typename BaseConversion< Scale::MANTISSA, Scale::EXPONENT,
									pow_t::N, pow_t::D >::Simplify						type;
			};
			
			template <typename FracPow>
			using Pow = typename _Pow4<FracPow>::type;

			using Inverse = typename _Pow4< TFractional< -1 > >::type;

			template <typename NewValueType>
			struct _To4 {
				typedef PhysicsDimensionScale< NewValueType >	_dim_t;
				typedef typename _dim_t::mantissa_t				_mantissa_t;
				typedef typename _dim_t::exponent_t				_exponent_t;
				typedef typename _dim_t::fractional_int_t		_fract_int_t;

				typedef typename _dim_t::template BaseConversion<
										_mantissa_t( Scale::MANTISSA ), _exponent_t( Scale::EXPONENT ),
										_fract_int_t( Power::N ), _fract_int_t( Power::D ) >				type;
			};
			
			template <typename NewValueType>
			using To = typename _To4<NewValueType>::type;

			static inline ValueType Get (const ValueType &value) {
				return Func::Get( value );
			}

			static inline ValueType ToFloat () {
				return GXMath::Pow( Scale::ToFloat< ValueType >(), Power::ToFloat< ValueType >() );
			}

			static inline String ToString () {
				return String() << '(' << Scale::ToString() << ")^" << Power::ToString();
			}
		};

#		define CONVERSION( _scaleM_, _scaleE_, _name_ ) \
			typedef typename BaseConversion< _scaleM_, _scaleE_, 1, 1 >::Simplify		_name_

		typedef BaseConversion< 1, 0, 1, 1 >	None;

		CONVERSION( 1,		 1,			Deca	);		// da
		CONVERSION( 1,		 2,			Hecto	);		// h
		CONVERSION( 1,		 3,			Kilo	);		// k
		CONVERSION( 1,		 6,			Mega	);		// M
		CONVERSION( 1,		 9,			Giga	);		// G
		CONVERSION( 1,		12,			Tera	);		// T
		CONVERSION( 1,		15,			Peta	);		// P
		CONVERSION( 1,		18,			Exa		);		// E
		CONVERSION( 1,		21,			Zetta	);		// Z
		CONVERSION( 1,		24,			Yotta	);		// Y

		CONVERSION( 1,		 -1,		Deci	);		// d
		CONVERSION( 1,		 -2,		Centi	);		// c
		CONVERSION( 1,		 -3,		Milli	);		// m
		CONVERSION( 1,		 -6,		Micro	);		// u
		CONVERSION( 1,		 -9,		Nano	);		// n
		CONVERSION( 1,		-12,		Pico	);		// p
		CONVERSION( 1,		-15,		Femto	);		// f
		CONVERSION( 1,		-18,		Atto	);		// a
		CONVERSION( 1,		-21,		Zepto	);		// z
		CONVERSION( 1,		-24,		Yocto	);		// y

		CONVERSION( 6,			   1,	Minute	);		// s	min
		CONVERSION( 6*6,		   2,	Hour	);		// s	h
		CONVERSION( 6*6*24,		   2,	Day		);		// s	d
		CONVERSION( 6*6*24*36525,  0,	Year	);		// s	y

		CONVERSION( 1,			  -3,	Litre				);		// m^3	L
		CONVERSION( 378541178,	-3-8,	USGallon			);		// m^3	gal

		CONVERSION( 160217733,	-19-8,	ElectronVolt		);		// J	eV
		CONVERSION( 1660538922, -27-9,	Dalton				);		// kg	Da	amu
		CONVERSION( 1495978707,	    2,	AstronomicalUnit	);		// m	au

		CONVERSION( 1852,		   0,	NauticalMile		);		// m	
		CONVERSION( 1609344,	  -3,	StatuteMile			);		// m	
		CONVERSION(	254,		-2-2,	Inch				);		// m	in
		CONVERSION( 3048,		  -4,	Foot				);		// m	ft
		CONVERSION( 4828032,	  -3,	League				);		// m	
		CONVERSION( 9144,		  -4,	Yard				);		// m

		CONVERSION( 6479891,	-6-5,	Grain				);		// kg	gr
		CONVERSION(	45359237,	  -8,	Pound				);		// kg	lb

		CONVERSION( 1,			 -10,	Angstrom			);		// m	A
		CONVERSION( 1,			 -28,	Barn				);		// m^2	b

		CONVERSION( 1,			   5,	Bar					);		// Pa	bar
		CONVERSION( 1013,		 3-1,	Atmosphere			);		// Pa

		CONVERSION( 299792458,	   0,	SpeedOfLight		);		// m/s	c
		CONVERSION( 30856776,	16-7,	Parsec				);		// m	pc

		CONVERSION( 667384808,	-11-8,	GravitationalConstant		);	// m^3 / (s^2 * kg)
		CONVERSION( 980665,		   -5,	GravitationalAcceleration	);	// m / s^2

		CONVERSION( 602214078,  23-8,	AvogadroConstant			);	// 1/mol
		CONVERSION( 1256637061, -6-9,	VacuumPermeabilityConst		);	// kg * m / (s * A)^2
		CONVERSION( 83144621,	  -7,	GasConstant					);	// R = J / (K * mol)

		//CONVERSION( 6371,		   3,	EarthRadius		);
		CONVERSION( 59736,		24-4,	EarthMass		);
		CONVERSION( 198892,		30-5,	SolarMass		);
		CONVERSION( 3846,		26-3,	SolarLuminosity	);
		CONVERSION( 6955,		 8-3,	SolarRadius		);
		
		CONVERSION( 8,			   0,	Byte			);		// byte

		CONVERSION( 1024,		   0,	Kilobit			);		// bit	1 << 10
		CONVERSION( 1048576,	   0,	Megabit			);		// bit	1 << 20
		CONVERSION( 1073741824,	   0,	Gigabit			);		// bit	1 << 30
		CONVERSION( 1099511628,	   3,	Terabit			);		// bit	1 << 40
		CONVERSION( 1125899907,	   6,	Petabit			);		// bit	1 << 50
		CONVERSION( 1152921505,	   9,	Exabit			);		// bit	1 << 60
		CONVERSION( 1180591621,	  12,	Zettabit		);		// bit	1 << 70
		CONVERSION( 1208925820,	  15,	Yottabit		);		// bit	1 << 80

		CONVERSION( 795774715,	  -7,	Oersted			);		// A/m	Oe
		
#		undef CONVERSION
	};
	
	
	template <typename ValueType,
			  typename Dimensions,
			  typename ValueScale = typename PhysicsDimensionScale< ValueType >::None
			 >
	struct PhysicsValue;


	//
	// Default Physics Values
	//

	template <typename T>
	struct DefaultPhysicsValues : public Noninstancable
	{
		typedef DefaultPhysicsDimensions	_def;

		typedef PhysicsValue< T, _def::NonDimensional >			NonDimensional;

		// base //
		typedef PhysicsValue< T, _def::Second >					Seconds;		// s
		typedef PhysicsValue< T, _def::Kilogram >				Kilograms;		// kg
		typedef PhysicsValue< T, _def::Meter >					Meters;			// m
		typedef PhysicsValue< T, _def::Amper >					Ampers;			// A
		typedef PhysicsValue< T, _def::Kelvin >					Kelvins;		// K
		typedef PhysicsValue< T, _def::Mole >					Moles;			// mol
		typedef PhysicsValue< T, _def::Candela >				Candelas;		// cd
		typedef PhysicsValue< T, _def::Currency >				Currencies;		// $
		typedef PhysicsValue< T, _def::Bit >					Bits;			// bit


		// compound //
		typedef PhysicsValue< T, _def::SquareMeter >			SquareMeters;				// m^2
		typedef PhysicsValue< T, _def::CubicMeter >				CubicMeters;				// m^3
		typedef PhysicsValue< T, _def::MeterPerSecond >			MetersPerSeconds;			// m/s
		typedef PhysicsValue< T, _def::MeterPerSquareSecond >	MetersPerSquareSeconds;		// m/s^2
		typedef PhysicsValue< T, _def::KilogramPerSecond >		KilogramsPerSeconds;		// kg / s
		typedef PhysicsValue< T, _def::KilogramMeterPerSecond >	KilogramMetersPerSeconds;	// kg * m/s
		typedef PhysicsValue< T, _def::KilogramPerCubicMeter >	KilogramsPerCubicMeters;	// p = kg / m^3
		typedef PhysicsValue< T, _def::Newton >					Newtons;					// N
		typedef PhysicsValue< T, _def::Joule >					NewtonMeters;				// N*m
		typedef PhysicsValue< T, _def::Joule >					Joules;						// J
		typedef PhysicsValue< T, _def::Pascal >					Pascals;					// Pa
		typedef PhysicsValue< T, _def::Hertz >					Hertz;						// Hz
		typedef PhysicsValue< T, _def::Watt >					Watts;						// W
		typedef PhysicsValue< T, _def::Coulomb >				Coulombs;					// C
		typedef PhysicsValue< T, _def::Volt >					Volts;						// V
		typedef PhysicsValue< T, _def::Ohm >					Ohms;						// Ohm
		typedef PhysicsValue< T, _def::Farad >					Farads;						// F
		typedef PhysicsValue< T, _def::Weber >					Webers;						// Wb
		typedef PhysicsValue< T, _def::Henry >					Henries;					// H
		typedef PhysicsValue< T, _def::Tesla >					Teslas;						// T
		typedef PhysicsValue< T, _def::Siemens >				Siemens;					// S
		typedef PhysicsValue< T, _def::Lumen >					Lumens;						// lm
		typedef PhysicsValue< T, _def::Lux >					Lux;						// lx
		typedef PhysicsValue< T, _def::AmperPerMeter >			AmpersPerMeters;			// A/m
		typedef PhysicsValue< T, _def::KilogramPerMole >		KilogramsPerMoles;			// kg/mol


		// with scale //
		typedef PhysicsDimensionScale<T>													_scale;
		typedef typename _scale::SpeedOfLight::template UnitMul< typename _scale::Minute >	_lightMinute;
		typedef typename _scale::SpeedOfLight::template UnitMul< typename _scale::Hour >	_lightHour;
		typedef typename _scale::SpeedOfLight::template UnitMul< typename _scale::Day >		_lightDay;
		typedef typename _scale::SpeedOfLight::template UnitMul< typename _scale::Year >	_lightYear;
		typedef typename _scale::NauticalMile::template UnitDiv< typename _scale::Hour >	_knot;
		typedef typename _scale::GravitationalAcceleration									_gravAccel;
		typedef typename _scale::GravitationalConstant										_gravConst;
		typedef typename _scale::Year::template UnitMul< typename _scale::Kilo >			_thousandYears;
		typedef typename _scale::Year::template UnitMul< typename _scale::Mega >			_millionYears;
		typedef typename _scale::Milli::template UnitMul< typename _scale::Deci >			_gaussToTesla;

		typedef _def::CubicMeter::template Div< _def::Kilogram::Mul< _def::Second::Power<
					CompileTime::Fractional32< 2 > > > >									_gravConstType;

		typedef _def::Newton::template Div< _def::Amper::Power<
					CompileTime::Fractional32< 2 > > >										_newtonPerSquareAmper;

		typedef typename _scale::VacuumPermeabilityConst									_vacuumPermeability;

		typedef _def::Mole::Inverse															_invMole;

		typedef _def::Joule::template Div< _def::Kelvin::Mul< _def::Mole > >				_jouleKelvinMole;

		typedef PhysicsValue< T, _def::Meter, typename _scale::Nano >						Nanometers;			// nm
		typedef PhysicsValue< T, _def::Meter, typename _scale::Micro >						Micrometers;		// um
		typedef PhysicsValue< T, _def::Meter, typename _scale::Milli >						Millimeters;		// mm
		typedef PhysicsValue< T, _def::Meter, typename _scale::Centi >						Centimeters;		// cm
		typedef PhysicsValue< T, _def::Meter, typename _scale::Kilo >						Kilometers;			// km
		typedef PhysicsValue< T, _def::Meter, typename _scale::Mega >						Megameters;			// Mm
		typedef PhysicsValue< T, _def::Meter, typename _scale::NauticalMile >				NauticalMiles;		// 
		typedef PhysicsValue< T, _def::Meter, typename _scale::StatuteMile >				StatuteMile;		// 
		typedef PhysicsValue< T, _def::Meter, typename _scale::Inch >						Inch;				// in
		typedef PhysicsValue< T, _def::Meter, typename _scale::Foot >						Foot;				// ft
		typedef PhysicsValue< T, _def::Meter, typename _scale::League >						League;				// 
		typedef PhysicsValue< T, _def::Meter, typename _scale::Yard >						Yard;				// 
		typedef PhysicsValue< T, _def::Meter, typename _scale::Angstrom >					Angstroms;			// A
		typedef PhysicsValue< T, _def::Meter, typename _scale::SpeedOfLight >				LightSeconds;		// c*s
		typedef PhysicsValue< T, _def::Meter, _lightMinute >								LightMinutes;		// c*min
		typedef PhysicsValue< T, _def::Meter, _lightHour >									LightHours;			// c*h
		typedef PhysicsValue< T, _def::Meter, _lightDay >									LightDays;			// c*d
		typedef PhysicsValue< T, _def::Meter, _lightYear >									LightYears;			// c*y
		typedef PhysicsValue< T, _def::Meter, typename _scale::Parsec >						Parsecs;			// pc
		typedef PhysicsValue< T, _def::Meter, typename _scale::AstronomicalUnit >			AstronomicalUnits;	// au
		typedef PhysicsValue< T, _def::Meter, typename _scale::SolarRadius >				SolarRadius;		// 
		
		typedef PhysicsValue< T, _def::SquareMeter, typename _scale::Barn >					Barns;				// b
		
		typedef PhysicsValue< T, _def::CubicMeter, typename _scale::Litre >					Liters;				// L
		typedef PhysicsValue< T, _def::CubicMeter, typename _scale::USGallon >				Gallon;				// gal
		
		typedef PhysicsValue< T, _def::MeterPerSecond, typename _scale::SpeedOfLight >		LightSpeeds;		// c
		typedef PhysicsValue< T, _def::MeterPerSecond, _knot >								Knots;				// kn
		typedef PhysicsValue< T, _def::MeterPerSecond, typename _scale::AstronomicalUnit >	AstronomicalUnitsPerSeconds;	// AU/s

		typedef PhysicsValue< T, _def::Kilogram, typename _scale::Dalton >					Daltons;			// Da, u
		typedef PhysicsValue< T, _def::Kilogram, typename _scale::Pico >					Nanograms;			// ng
		typedef PhysicsValue< T, _def::Kilogram, typename _scale::Nano >					Micrograms;			// ug
		typedef PhysicsValue< T, _def::Kilogram, typename _scale::Micro >					Milligrams;			// mg
		typedef PhysicsValue< T, _def::Kilogram, typename _scale::Milli >					Grams;				// g
		typedef PhysicsValue< T, _def::Kilogram, typename _scale::Kilo >					Tones;				// t
		typedef PhysicsValue< T, _def::Kilogram, typename _scale::Mega >					Kilotones;			// kt
		typedef PhysicsValue< T, _def::Kilogram, typename _scale::Giga >					Megatones;			// Mt
		typedef PhysicsValue< T, _def::Kilogram, typename _scale::Grain >					Grains;				// gr
		typedef PhysicsValue< T, _def::Kilogram, typename _scale::Pound >					Pound;				// lb
		typedef PhysicsValue< T, _def::Kilogram, typename _scale::EarthMass >				EarthMass;			// EM
		typedef PhysicsValue< T, _def::Kilogram, typename _scale::SolarMass >				SolarMass;			// SM

		typedef PhysicsValue< T, _def::Second, typename _scale::Nano >						Nanoseconds;		// ns
		typedef PhysicsValue< T, _def::Second, typename _scale::Micro >						Microseconds;		// us
		typedef PhysicsValue< T, _def::Second, typename _scale::Milli >						Milliseconds;		// ms
		typedef PhysicsValue< T, _def::Second, typename _scale::Minute >					Minutes;			// min
		typedef PhysicsValue< T, _def::Second, typename _scale::Hour >						Hours;				// h
		typedef PhysicsValue< T, _def::Second, typename _scale::Day >						Days;				// d
		typedef PhysicsValue< T, _def::Second, typename _scale::Year >						Years;				// y
		typedef PhysicsValue< T, _def::Second, _thousandYears >								ThousandYears;		// 1 000 * y
		typedef PhysicsValue< T, _def::Second, _millionYears >								MillionYears;		// 1 000 000 * y

		typedef PhysicsValue< T, _def::Joule, typename _scale::ElectronVolt >				ElectronVolts;		// eV
		typedef PhysicsValue< T, _def::Joule, typename _scale::Nano >						Nanojoules;			// nJ
		typedef PhysicsValue< T, _def::Joule, typename _scale::Micro >						Microjoules;		// uJ
		typedef PhysicsValue< T, _def::Joule, typename _scale::Milli >						Millijoules;		// mJ
		typedef PhysicsValue< T, _def::Joule, typename _scale::Kilo >						Kilojoules;			// kJ
		typedef PhysicsValue< T, _def::Joule, typename _scale::Mega >						Megajoules;			// MJ
		typedef PhysicsValue< T, _def::Joule, typename _scale::Giga >						Gigajoules;			// GJ
		
		typedef PhysicsValue< T, _def::Pascal, typename _scale::Bar >						Bars;				// bar
		typedef PhysicsValue< T, _def::Pascal, typename _scale::Atmosphere >				Atmospheres;		// atm
		
		typedef PhysicsValue< T, _def::Pascal, _gravAccel >									GAcceleration;		// g
		typedef PhysicsValue< T, _gravConstType, _gravConst >								GConstant;			// G
		
		typedef PhysicsValue< T, _def::Watt, typename _scale::SolarLuminosity >				SolarLuminosity;	// SL
		
		typedef PhysicsValue< T, _def::Bit, typename _scale::Kilobit >						Kilobit;			// Kbit
		typedef PhysicsValue< T, _def::Bit, typename _scale::Megabit >						Megabit;			// Mbit
		typedef PhysicsValue< T, _def::Bit, typename _scale::Gigabit >						Gigabit;			// Gbit
		typedef PhysicsValue< T, _def::Bit, typename _scale::Terabit >						Terabit;			// Tbit
		typedef PhysicsValue< T, _def::Bit, typename _scale::Petabit >						Petabit;			// Pbit
		typedef PhysicsValue< T, _def::Bit, typename _scale::Exabit  >						Exabit;				// Ebit
		typedef PhysicsValue< T, _def::Bit, typename _scale::Zettabit >						Zettabit;			// Zbit
		typedef PhysicsValue< T, _def::Bit, typename _scale::Yottabit >						Yottabit;			// Ybit

		typedef PhysicsValue< T, _def::Bit, typename _scale::Byte >							Bytes;				// b
		
		typedef PhysicsValue< T, _def::AmperPerMeter, typename _scale::Oersted >			Oersteds;			// Oe

		typedef PhysicsValue< T, _def::Tesla, _gaussToTesla >								Gauss;				// G, Gs

		typedef PhysicsValue< T, _def::KilogramPerCubicMeter, typename _scale::Milli >		GramsPerCubicCentimeters;	// gr / cm^3
		
		typedef PhysicsValue< T, _def::KilogramPerMole, typename _scale::Milli >			GramsPerMoles;			// gr/mol

		typedef PhysicsValue< T, _invMole, typename _scale::AvogadroConstant >				AvogadroConstant;		// Na
		typedef PhysicsValue< T, _newtonPerSquareAmper, _vacuumPermeability >				VacuumPermeabilityConst;// mu0
		typedef PhysicsValue< T, _jouleKelvinMole, typename _scale::GasConstant >			GasConstant;			// R
	};


}	// GXMath
}	// GX_STL

#endif	// GX_PHYSICS_DIMENSIONS_ENABLED
