// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Physics/Common/Common.h"

namespace GXPhysics
{

	//
	// Physics Dimension enum
	//

	struct EPhysicsDimension
	{
		enum type
		{
			// SI
			Seconds,		// time
			Kilograms,		// mass
			Meters,			// length
			Amperes,		// electric current
			Kelvins,		// temperature
			Moles,			// amount of substance
			Candelas,		// luminous intensity

			// non SI
			Currency,		// monetary unit
			Bits,			// unit of information

			// TODO:
			// Pixels,		// or untyped

			_Count
		};
	};



	//
	// Physics Dimension
	//

	template <EPhysicsDimension::type Dim, typename FracPower = CompileTime::Fractional32<0> >
	struct TPhysicsDimension : public GXTypes::Noninstancable
	{
	// types
	public:
		typedef TPhysicsDimension< Dim, FracPower >	Self;
		typedef FracPower							POWER;
	
		static const EPhysicsDimension::type		DIMENSION	= Dim;


		template <typename T>
		struct IsSame1 {
			static const bool	value = (DIMENSION == T::DIMENSION);
		};

		template <typename T>
		struct Mul1 {
			typedef TPhysicsDimension< DIMENSION, typename POWER::template Add< typename T::POWER > >	type;
		};

		template <typename T>
		struct MulIfSame1 {
			typedef typename CompileTime::SwitchType< IsSame1<T>::value, typename Mul1<T>::type, Self >	type;
		};

		struct Inverse1 {
			typedef TPhysicsDimension< DIMENSION, typename POWER::Neg >	type;
		};

		template <typename ToFracPower>
		struct Power1 {
			typedef TPhysicsDimension< DIMENSION, typename POWER::template Mul< ToFracPower > >		type;
		};

		template <typename T>
		struct Equal {
			static const bool	value = IsSame1<T>::value and POWER::template Equal<T::POWER>::value;
		};


	// methods
	public:
		static String ToString ()
		{
			static const char *	shortTypeNames[] = {
				"s", "kg", "m", "A", "K", "mol", "cd", "$", "bit"
			};

			STATIC_ASSERT( CountOf(shortTypeNames) == EPhysicsDimension::_Count );

			if ( POWER::IsZero::value )
				return String();

			return ( String( shortTypeNames[ DIMENSION ] ) << '^' << POWER::ToString() );
		}
	};



	//
	// Physics Dimensions List
	//
	
	template <typename DimensionsTypeList>
	struct TPhysicsDimensionsList : public GXTypes::Noninstancable
	{
	// checks
		STATIC_ASSERT( DimensionsTypeList::Count == EPhysicsDimension::_Count );
		STATIC_ASSERT( DimensionsTypeList::template Get< EPhysicsDimension::Seconds   >::DIMENSION == EPhysicsDimension::Seconds   );
		STATIC_ASSERT( DimensionsTypeList::template Get< EPhysicsDimension::Kilograms >::DIMENSION == EPhysicsDimension::Kilograms );
		STATIC_ASSERT( DimensionsTypeList::template Get< EPhysicsDimension::Meters    >::DIMENSION == EPhysicsDimension::Meters    );
		STATIC_ASSERT( DimensionsTypeList::template Get< EPhysicsDimension::Amperes   >::DIMENSION == EPhysicsDimension::Amperes   );
		STATIC_ASSERT( DimensionsTypeList::template Get< EPhysicsDimension::Kelvins   >::DIMENSION == EPhysicsDimension::Kelvins   );
		STATIC_ASSERT( DimensionsTypeList::template Get< EPhysicsDimension::Moles     >::DIMENSION == EPhysicsDimension::Moles     );
		STATIC_ASSERT( DimensionsTypeList::template Get< EPhysicsDimension::Candelas  >::DIMENSION == EPhysicsDimension::Candelas  );
		STATIC_ASSERT( DimensionsTypeList::template Get< EPhysicsDimension::Currency  >::DIMENSION == EPhysicsDimension::Currency  );
		STATIC_ASSERT( DimensionsTypeList::template Get< EPhysicsDimension::Bits      >::DIMENSION == EPhysicsDimension::Bits      );


	// types
	protected:
		template <typename Dim>
		struct _MulDimFunc
		{
			template <usize Index, typename Type, typename PrevFuncResult, typename Typelist>
			struct Func
			{
				typedef typename Type::template MulIfSame1< Dim >::type	type;
				typedef CompileTime::TypeList< type, PrevFuncResult >	result;
			};
		};
		
		template <typename Dims>
		struct _MulFunc
		{
			template <usize Index, typename Type, typename PrevFuncResult, typename Typelist>
			struct Func
			{
				typedef typename Dims::template Get2< (EPhysicsDimension::type)Index >::type	right;

				STATIC_ASSERT( Type::template IsSame1< right >::value );

				typedef typename Type::template Mul1< right >::type		type;

				typedef CompileTime::TypeList< type, PrevFuncResult >	result;
			};
		};
		
		template <typename ToFracPower>
		struct _PowerFunc
		{
			template <usize Index, typename Type, typename PrevFuncResult, typename Typelist>
			struct Func
			{
				 typedef typename Type::template Power1< ToFracPower >::type	type;
				 typedef CompileTime::TypeList< type, PrevFuncResult >			result;
			};
		};

		template <usize Index, typename Type, typename PrevFuncResult, typename Typelist>
		struct _IsNonDimFunc
		{
			typedef CompileTime::ValueToType< bool, (Type::POWER::IsZero::value and PrevFuncResult::value) >	result;
		};

		struct _ToStringFunc
		{
			const StringCRef	divider;
			String				result;

			_ToStringFunc (StringCRef divider) : divider(divider)
			{}

			template <typename T, usize Index>
			void Process ()
			{
				String s = T::ToString();
				
				if ( s.Empty() )
					return;
				
				if ( not result.Empty() )
					result << divider;

				result << s;
			}
		};


	public:
		typedef bool											_is_physics_dimensions_list;
		typedef DimensionsTypeList								DimensionsTypeList_t;
		typedef TPhysicsDimensionsList< DimensionsTypeList >	Self;

		template <typename Dim>
		struct _MulDim {
			typedef TPhysicsDimensionsList< typename DimensionsTypeList::template ReverseForEach< _MulDimFunc< Dim >::template Func > >	type;
		};

		template <typename Dims>
		struct Equal {
			static const bool	value = DimensionsTypeList::template Equal< typename Dims::DimensionsTypeList_t >;
		};

		template <typename Dims>
		struct Add2 {
			//STATIC_ASSERT( typename Dims::_is_physics_dimensions_list(true) );
			STATIC_ASSERT( Self::template Equal< Dims >::value );
			typedef Self	type;
		};

		template <typename Dims>
		struct Sub2 {
			//STATIC_ASSERT( typename Dims::_is_physics_dimensions_list(true) );
			STATIC_ASSERT( Self::template Equal< Dims >::value );
			typedef Self	type;
		};
	
		template <typename Dims>
		struct Mul2 {
			//STATIC_ASSERT( typename Dims::_is_physics_dimensions_list(true) );
			typedef TPhysicsDimensionsList< typename DimensionsTypeList::template ReverseForEach< _MulFunc< Dims >::template Func > >		type;
		};

		template <typename Dims>
		struct Div2 {
			//STATIC_ASSERT( typename Dims::_is_physics_dimensions_list(true) );
			typedef typename Mul2< typename Dims::Inverse2::type >::type		type;
		};
	
		template <typename ToFracPower>
		struct Power2 {
			typedef TPhysicsDimensionsList< typename DimensionsTypeList::template ReverseForEach< _PowerFunc< ToFracPower >::template Func > >		type;
		};

		struct Inverse2 {
			typedef typename Power2< CompileTime::Fractional32<-1> >::type	type;
		};

		template <EPhysicsDimension::type Index>
		struct Get2 {
			typedef typename DimensionsTypeList::template Get< Index >	type;
		};

		struct IsNonDimensional {
			static const bool	value = DimensionsTypeList::template ForEach< _IsNonDimFunc, CompileTime::ValueToType<bool,true> >::value;
		};


	// methods
	public:
		CHECKRES static String ToString (StringCRef divider = "\n")
		{
			_ToStringFunc	func( divider );

			DimensionsTypeList::RuntimeForEach( func );

			return func.result;
		}
	};



	//
	// Simplified Physics Dimensions List
	//
	
	namespace _phys_hidden_
	{
		template <	int SecondsNom = 0,		uint SecondsDenom = 1,
					int KilogramsNom = 0,	uint KilogramsDenom = 1,
					int MetersNom = 0,		uint MetersDenom = 1,
					int AmpersNom = 0,		uint AmpersDenom = 1,
					int KelvinsNom = 0,		uint KelvinsDenom = 1,
					int MolesNom = 0,		uint MolesDenom = 1,
					int CandelasNom = 0,	uint CandelsDenom = 1,
					int CurrencyNom = 0,	uint CurrencyDenom = 1,
					int BitsNom = 0,		uint BitsDenom = 1
				 >
		struct PhysDimList : public GXTypes::Noninstancable
		{
		// types
		private:
			template <typename Dims>
			struct _From {
				typedef typename Dims::DimensionsTypeList_t	tl;

				typedef PhysDimList<
							tl::template Get< EPhysicsDimension::Seconds   >::POWER::N,	tl::template Get< EPhysicsDimension::Seconds   >::POWER::D,
							tl::template Get< EPhysicsDimension::Kilograms >::POWER::N,	tl::template Get< EPhysicsDimension::Kilograms >::POWER::D,
							tl::template Get< EPhysicsDimension::Meters    >::POWER::N,	tl::template Get< EPhysicsDimension::Meters    >::POWER::D,
							tl::template Get< EPhysicsDimension::Amperes   >::POWER::N,	tl::template Get< EPhysicsDimension::Amperes   >::POWER::D,
							tl::template Get< EPhysicsDimension::Kelvins   >::POWER::N,	tl::template Get< EPhysicsDimension::Kelvins   >::POWER::D,
							tl::template Get< EPhysicsDimension::Moles     >::POWER::N,	tl::template Get< EPhysicsDimension::Moles     >::POWER::D,
							tl::template Get< EPhysicsDimension::Candelas  >::POWER::N,	tl::template Get< EPhysicsDimension::Candelas  >::POWER::D,
							tl::template Get< EPhysicsDimension::Currency  >::POWER::N,	tl::template Get< EPhysicsDimension::Currency  >::POWER::D,
							tl::template Get< EPhysicsDimension::Bits      >::POWER::N,	tl::template Get< EPhysicsDimension::Bits      >::POWER::D >	type;
			};

		public:
			typedef PhysDimList< SecondsNom,	SecondsDenom,
								 KilogramsNom,	KilogramsDenom,
								 MetersNom,		MetersDenom,
								 AmpersNom,		AmpersDenom,
								 KelvinsNom,	KelvinsDenom,
								 MolesNom,		MolesDenom,
								 CandelasNom,	CandelsDenom,
								 CurrencyNom,	CurrencyDenom,
								 BitsNom,		BitsDenom		>	Self;

			typedef TPhysicsDimensionsList< CompileTime::TypeListFrom<
						TPhysicsDimension< EPhysicsDimension::Seconds,		typename CompileTime::Fractional32< SecondsNom,		SecondsDenom > >,
						TPhysicsDimension< EPhysicsDimension::Kilograms,	typename CompileTime::Fractional32< KilogramsNom,	KilogramsDenom > >,
						TPhysicsDimension< EPhysicsDimension::Meters,		typename CompileTime::Fractional32< MetersNom,		MetersDenom > >,
						TPhysicsDimension< EPhysicsDimension::Amperes,		typename CompileTime::Fractional32< AmpersNom,		AmpersDenom > >,
						TPhysicsDimension< EPhysicsDimension::Kelvins,		typename CompileTime::Fractional32< KelvinsNom,		KelvinsDenom > >,
						TPhysicsDimension< EPhysicsDimension::Moles,		typename CompileTime::Fractional32< MolesNom,		MolesDenom > >,
						TPhysicsDimension< EPhysicsDimension::Candelas,		typename CompileTime::Fractional32< CandelasNom,	CandelsDenom > >,
						TPhysicsDimension< EPhysicsDimension::Currency,		typename CompileTime::Fractional32< CurrencyNom,	CurrencyDenom > >,
						TPhysicsDimension< EPhysicsDimension::Bits,			typename CompileTime::Fractional32< BitsNom,		BitsDenom > >
					> >		dim_t;

			template <typename Dims>
			using Add = typename _From< typename dim_t::template Add2< typename Dims::dim_t >::type >::type;

			template <typename Dims>
			using Sub = typename _From< typename dim_t::template Sub2< typename Dims::dim_t >::type >::type;
	
			template <typename Dims>
			using Mul = typename _From< typename dim_t::template Mul2< typename Dims::dim_t >::type >::type;

			template <typename Dims>
			using Div = typename _From< typename dim_t::template Div2< typename Dims::dim_t >::type >::type;
	
			template <typename ToFracPower>
			using Power = typename _From< typename dim_t::template Power2< ToFracPower >::type >::type;

			template <EPhysicsDimension::type Index>
			using Get = typename dim_t::template Get2< Index >;

			using Inverse = typename _From< typename dim_t::Inverse2::type >::type;

			template <typename Dims>
			struct Equal {
				static const bool	value = dim_t::template Equal< typename Dims::dim_t >::value;
			};

			struct IsNonDimensional {
				static const bool	value = dim_t::IsNonDimensional::value;
			};


		protected:
			template <typename Dim>
			struct _MulDim {
				typedef typename _From< typename dim_t::template _MulDim< Dim >::type >::type	type;
			};


		// methods
		public:
			static String ToString (StringCRef divider = "\n")
			{
				return dim_t::ToString( divider );
			}
		};

	}	// _phys_hidden_
	


	//
	// Default Physics Dimensions List
	//
	
	struct DefaultPhysicsDimensionsList : _phys_hidden_::PhysDimList<>
	{
		template <EPhysicsDimension::type DimType, isize PowNum = 1, isize PowDenom = 1>
		using Create = typename Self::_MulDim< TPhysicsDimension< DimType,
						typename CompileTime::Fractional32< PowNum, PowDenom >::Simplified > >::type;

		using CreateNonDimensional = Self;
	};


	//
	// Default Physics Dimensions
	//
	
	struct DefaultPhysicsDimensions : public GXTypes::Noninstancable
	{
		// base //
		typedef DefaultPhysicsDimensionsList::CreateNonDimensional						NonDimensional;	// 
		typedef DefaultPhysicsDimensionsList::Create< EPhysicsDimension::Seconds >		Second;			// s
		typedef DefaultPhysicsDimensionsList::Create< EPhysicsDimension::Kilograms >	Kilogram;		// kg
		typedef DefaultPhysicsDimensionsList::Create< EPhysicsDimension::Meters >		Meter;			// m
		typedef DefaultPhysicsDimensionsList::Create< EPhysicsDimension::Amperes >		Amper;			// A
		typedef DefaultPhysicsDimensionsList::Create< EPhysicsDimension::Kelvins >		Kelvin;			// K
		typedef DefaultPhysicsDimensionsList::Create< EPhysicsDimension::Moles >		Mole;			// mol
		typedef DefaultPhysicsDimensionsList::Create< EPhysicsDimension::Candelas >		Candela;		// cd
		typedef DefaultPhysicsDimensionsList::Create< EPhysicsDimension::Currency >		Currency;		// $
		typedef DefaultPhysicsDimensionsList::Create< EPhysicsDimension::Bits >			Bit;			// bit
		
		// compound //
		typedef Meter::Power< CompileTime::Fractional32< 2 > >			SquareMeter;			// m^2
		typedef Meter::Power< CompileTime::Fractional32< 3 > >			CubicMeter;				// m^3
		typedef Meter::Div< Second >									MeterPerSecond;			// m / s
		typedef MeterPerSecond::Div< Second >							MeterPerSquareSecond;	// m / s^2
		typedef Kilogram::Div< Second >									KilogramPerSecond;		// kg / s
		typedef MeterPerSecond::Mul< Kilogram >							KilogramMeterPerSecond;	// kg * m / s
		typedef Kilogram::Div< CubicMeter >								KilogramPerCubicMeter;	// kg / m^3
		typedef	Kilogram::Mul< MeterPerSquareSecond >					Newton;					// N = kg * m / s^2
		typedef Newton::Mul< Meter >									Joule;					// J = kg * (m / s)^2
		typedef Kilogram::Div< Meter::Mul< Second::Mul< Second > > >	Pascal;					// Pa = kg / (m * s^2)
		typedef Second::Inverse											Hertz;					// Hz = 1 / s
		typedef Joule::Div< Second >									Watt;					// W = J / s
		typedef Amper::Mul< Second >									Coulomb;				// C = A * s
		typedef Joule::Div< Coulomb >									Volt;					// V = J / C
		typedef Volt::Div< Amper >										Ohm;					// Ohm = V / A
		typedef Coulomb::Div< Volt >									Farad;					// F = C / V
		typedef Volt::Mul< Second >										Weber;					// Wb = V * s
		typedef Weber::Div< Amper >										Henry;					// H = Wb / A
		typedef Weber::Div< SquareMeter >								Tesla;					// T = Wb / m^2
		typedef Ohm::Inverse											Siemens;				// S = 1 / Ohm
		typedef Candela													Lumen;					// lm = cd * (sr)
		typedef	Lumen::Div< SquareMeter >								Lux;					// lx = lm / m^2
		typedef Amper::Div< Meter >										AmperPerMeter;			// A/m
		typedef Kilogram::Div< Mole >									KilogramPerMole;		// kg/mol
	};
	

}	// GXPhysics
