// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/CompileTime/TemplateMath.h"
#include "Engine/STL/Math/MathFunc.h"

namespace GX_STL
{
namespace GXMath
{

	typedef Radians<real>			Rad;
	typedef RadiansVec< real, 2 >	Rad2;
	typedef RadiansVec< real, 3 >	Rad3;
	typedef RadiansVec< real, 4 >	Rad4;

	typedef Degrees<real>			Deg;
	typedef DegreesVec< real, 2 >	Deg2;
	typedef DegreesVec< real, 3 >	Deg3;
	typedef DegreesVec< real, 4 >	Deg4;



	//
	// Radians
	//

	template <typename T>
	struct Radians : public CompileTime::CopyQualifiers< T >
	{
		STATIC_ASSERT( CompileTime::IsScalar<T> );

	// types
	public:
		typedef Radians<T>		Self;
		typedef T				Value_t;


	// variables
	private:
		T	_value;


	// methods
	public:
		Radians (GX_DEFCTOR) : _value(0) {}

		explicit constexpr Radians (T value) : _value(value) {}

		ND_ explicit operator T ()			const	{ return _value; }
		ND_ explicit operator Degrees<T> ()	const	{ return ToDegrees(); }

		ND_ T &			ref ()							{ return _value; }
		ND_ T const &	ref ()		const				{ return _value; }

		ND_ Degrees<T> ToDegrees () const;
		
		template <typename B>
		ND_ Degrees<B> ToDegrees () const;

		template <typename B>
		ND_ Radians<B> To () const;

		ND_ Self		ToInterval (Self minValue, Self maxValue) const;

		_GX_DIM_ALL_FLOAT_OPERATORS_SELF( _value );
		_GX_DIM_ALL_FLOAT_OPERATORS_TYPE( _value, const Value_t&, );

		ND_ static constexpr Self	Pi (T scale)		{ return Self( GXMath::Pi<T> * scale ); }	// Pi * x
		ND_ static constexpr Self	Pi ()				{ return Self( GXMath::Pi<T> ); }			// Pi
		ND_ static constexpr Self	ReciporalPi ()		{ return Self( GXMath::ReciporalPi<T> ); }	// 1/Pi
		ND_ static constexpr Self	DoublePi ()			{ return Self( Pi( T(2) ) ); }				// 2*Pi
		ND_ static constexpr Self	HalfPi ()			{ return Self( Pi( T(0.5) ) ); }			// Pi/2

		ND_ static Self FromArcminutes (T value);
		ND_ static Self FromArcseconds (T value);
		
		ND_ static Self From (T deg, T arcmin = T(0), T arcsec = T(0));
	};



	//
	// Degrees
	//

	template <typename T>
	struct Degrees : public CompileTime::CopyQualifiers< T >
	{
		STATIC_ASSERT( CompileTime::IsScalar<T> );

	// types
	public:
		typedef Degrees<T>		Self;
		typedef T				Value_t;


	// variables
	private:
		T	_value;


	// methods
	public:
		Degrees (GX_DEFCTOR) : _value(0) {}

		explicit constexpr Degrees (T value) : _value(value) {}

		ND_ explicit operator T ()			const	{ return _value; }
		ND_ explicit operator Radians<T> ()	const	{ return ToRadians(); }

		ND_ T &			ref ()						{ return _value; }
		ND_ T const &	ref ()				const	{ return _value; }

		ND_ Radians<T> ToRadians () const;

		template <typename B>
		ND_ Radians<B> ToRadians () const;

		template <typename B>
		ND_ Degrees<B> To () const;
		
		ND_ Self		ToInterval (Self minValue, Self maxValue) const;

		_GX_DIM_ALL_FLOAT_OPERATORS_SELF( _value );
		_GX_DIM_ALL_FLOAT_OPERATORS_TYPE( _value, const Value_t&, );
		
		ND_ static constexpr Self	Pi (T scale)				{ return Self( T(180) * scale ); }				// Pi * x
		ND_ static constexpr Self	Pi ()						{ return Self( T(180) ); }						// Pi
		ND_ static constexpr Self	ReciporalPi ()				{ return Self( T(0.0055555555555555558) ); }	// 1/Pi
		ND_ static constexpr Self	DoublePi ()					{ return Self( Pi( T(2) ) ); }					// 2*Pi
		ND_ static constexpr Self	HalfPi ()					{ return Self( Pi( T(0.5) ) ); }				// Pi/2
		
		ND_ static constexpr Self	FromArcminutes (T value)	{ return Self( value / T(60.0) ); }				// T(0.01666667)
		ND_ static constexpr Self	FromArcseconds (T value)	{ return Self( value / T(3600.0) ); }			// T(0.0002777778)

		ND_ static Self From (T deg, T arcmin = T(0), T arcsec = T(0));
	};

	

	ND_ constexpr Rad operator "" _rad (long double value)			{ return Rad( real(value) ); }
	ND_ constexpr Rad operator "" _rad (unsigned long long value)	{ return Rad( real(value) ); }
	
	ND_ constexpr Deg operator "" _deg (long double value)			{ return Deg( real(value) ); }
	ND_ constexpr Deg operator "" _deg (unsigned long long value)	{ return Deg( real(value) ); }

	

//--------------------------------- Radians --------------------------------//
	
	template <typename T>
	ND_ inline Degrees<T> Radians<T>::ToDegrees () const
	{
		return ToDegrees<T>();
	}
		
	template <typename T>
	template <typename B>
	ND_ inline Degrees<B> Radians<T>::ToDegrees () const
	{
		return Degrees<B>( B(_value) * B(Degrees<B>::Pi()) * B(Radians<B>::ReciporalPi()) );
	}
	
	template <typename T>
	template <typename B>
	ND_ inline Radians<B> Radians<T>::To () const
	{
		return Radians<B>( B(_value) );
	}
	
	template <typename T>
	ND_ inline Radians<T>  Radians<T>::ToInterval (Self minValue, Self maxValue) const
	{
		// 0..+2Pi, 0..+Pi, -Pi..0, -2Pi..0, and other
		return Self{GXMath::Wrap( _value, T(minValue), T(maxValue) )};
	}
	
	template <typename T>
	ND_ inline Radians<T>  Radians<T>::FromArcminutes (T value)
	{
		return Degrees<T>::FromArcminutes( value ).ToRadians();
	}
	
	template <typename T>
	ND_ inline Radians<T>  Radians<T>::FromArcseconds (T value)
	{
		return Degrees<T>::FromArcseconds( value ).ToRadians();
	}
	
	template <typename T>
	ND_ inline Radians<T>  Radians<T>::From (T deg, T arcmin, T arcsec)
	{
		return Degrees<T>::From( deg, arcmin, arcsec ).ToRadians();
	}
	


//--------------------------------- Degrees --------------------------------//

	template <typename T>
	ND_ inline Radians<T> Degrees<T>::ToRadians () const
	{
		return ToRadians<T>();
	}
	
	template <typename T>
	template <typename B>
	ND_ inline Radians<B> Degrees<T>::ToRadians () const
	{
		return Radians<B>( B(_value) * B(Radians<B>::Pi()) * B(Degrees<B>::ReciporalPi()) );
	}
	
	template <typename T>
	template <typename B>
	ND_ inline Degrees<B> Degrees<T>::To () const
	{
		return Degrees<B>( B(_value) );
	}
	
	template <typename T>
	ND_ inline Degrees<T>  Degrees<T>::ToInterval (Self minValue, Self maxValue) const
	{
		// 0..+2Pi, 0..+Pi, -Pi..0, -2Pi..0, and other
		return Self{GXMath::Wrap( _value, T(minValue), T(maxValue) )};
	}
	
	template <typename T>
	ND_ inline Degrees<T>  Degrees<T>::From (T deg, T arcmin, T arcsec)
	{
		return Degrees<T>( deg + (arcmin / T(60.0)) + (arcsec / T(3600.0)) );
	}

}	// GXMath
	

namespace CompileTime {
/*
=================================================
	TypeInfo
=================================================
*/
	template <typename T>
	struct TypeInfo< GXMath::Radians<T> >
	{
		typedef GXMath::Radians<T>	type;
		typedef T					inner_type;
		
		template <typename OtherType>
		using CreateWith = GXMath::Radians< OtherType >;

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

	template <typename T>
	struct TypeInfo< GXMath::Degrees<T> >
	{
		typedef GXMath::Degrees<T>	type;
		typedef T					inner_type;
		
		template <typename OtherType>
		using CreateWith = GXMath::Degrees< OtherType >;

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
	struct Hash< GXMath::Radians<T> >
	{
		ND_ HashResult  operator () (const GXMath::Radians<T> &x) const
		{
			return HashOf( x.ref() );
		}
	};
	
	
	template <typename T>
	struct Hash< GXMath::Degrees<T> >
	{
		ND_ HashResult  operator () (const GXMath::Degrees<T> &x) const
		{
			return HashOf( x.ref() );
		}
	};

}	// GXTypes
}	// GX_STL
