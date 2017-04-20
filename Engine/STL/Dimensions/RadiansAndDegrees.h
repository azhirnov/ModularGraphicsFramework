// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/CompileTime/TemplateMath.h"
#include "Engine/STL/Math/MathTypes.h"

namespace GX_STL
{
namespace GXMath
{

	//template <typename T>	struct Radians;		// liternals:	rad
	//template <typename T>	struct Degrees;		// literals:	deg

	//template <typename T, usize I>	using RadiansVec = Vec< Radians<T>, I >;
	//template <typename T, usize I>	using DegreesVec = Vec< Degrees<T>, I >;


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
	// types
	public:
		typedef Radians<T>		Self;
		typedef T				value_t;


	// variables
	private:
		T	_value;


	// methods
	public:
		Radians (GX_DEFCTOR) : _value(0) {}

		explicit
		constexpr Radians (T value) : _value(value) {}

		explicit operator T ()			const		{ return _value; }
		explicit operator Degrees<T> ()	const		{ return ToDegrees(); }

		T &			ref ()							{ return _value; }
		T const &	ref ()		const				{ return _value; }

		Degrees<T> ToDegrees () const;
		
		template <typename B>
		Degrees<B> ToDegrees () const;

		template <typename B>
		Radians<B> To () const;

		Self	ToInterval (Self minValue, Self maxValue) const;

		_GX_DIM_ALL_FLOAT_OPERATORS_SELF( _value )
		_GX_DIM_ALL_FLOAT_OPERATORS_TYPE( _value, value_t, )

		static constexpr Self	Pi (T scale)		{ return Self( GXMath::Pi<T> * scale ); }	// Pi * x
		static constexpr Self	Pi ()				{ return Self( GXMath::Pi<T> ); }			// Pi
		static constexpr Self	ReciporalPi ()		{ return Self( GXMath::ReciporalPi<T> ); }	// 1/Pi
		static constexpr Self	DoublePi ()			{ return Self( Pi( T(2) ) ); }				// 2*Pi
		static constexpr Self	HalfPi ()			{ return Self( Pi( T(0.5) ) ); }			// Pi/2

		static Self FromArcminutes (T value);
		static Self FromArcseconds (T value);
		
		static Self From (T deg, T arcmin = T(0), T arcsec = T(0));
	};



	//
	// Degrees
	//

	template <typename T>
	struct Degrees : public CompileTime::CopyQualifiers< T >
	{
	// types
	public:
		typedef Degrees<T>		Self;
		typedef T				value_t;


	// variables
	private:
		T	_value;


	// methods
	public:
		Degrees (GX_DEFCTOR) : _value(0) {}

		explicit
		constexpr Degrees (T value) : _value(value) {}

		explicit operator T ()			const		{ return _value; }
		explicit operator Radians<T> ()	const		{ return ToRadians(); }

		T &			ref ()							{ return _value; }
		T const &	ref ()		const				{ return _value; }

		Radians<T> ToRadians () const;

		template <typename B>
		Radians<B> ToRadians () const;

		template <typename B>
		Degrees<B> To () const;
		
		Self	ToInterval (Self minValue, Self maxValue) const;

		_GX_DIM_ALL_FLOAT_OPERATORS_SELF( _value );
		
		static constexpr Self	Pi (T scale)				{ return Self( T(180) * scale ); }				// Pi * x
		static constexpr Self	Pi ()						{ return Self( T(180) ); }						// Pi
		static constexpr Self	ReciporalPi ()				{ return Self( T(0.0055555555555555558) ); }	// 1/Pi
		static constexpr Self	DoublePi ()					{ return Self( Pi( T(2) ) ); }					// 2*Pi
		static constexpr Self	HalfPi ()					{ return Self( Pi( T(0.5) ) ); }				// Pi/2
		
		static constexpr Self	FromArcminutes (T value)	{ return Self( value / T(60.0) ); }				// T(0.01666667)
		static constexpr Self	FromArcseconds (T value)	{ return Self( value / T(3600.0) ); }			// T(0.0002777778)

		static Self From (T deg, T arcmin = T(0), T arcsec = T(0));
	};

	

	constexpr Rad operator "" _rad (long double value)			{ return Rad( real(value) ); }
	constexpr Rad operator "" _rad (unsigned long long value)	{ return Rad( real(value) ); }
	
	constexpr Deg operator "" _deg (long double value)			{ return Deg( real(value) ); }
	constexpr Deg operator "" _deg (unsigned long long value)	{ return Deg( real(value) ); }

	

//--------------------------------- Radians --------------------------------//
	
	template <typename T>
	inline Degrees<T> Radians<T>::ToDegrees () const
	{
		return ToDegrees<T>();
	}
		
	template <typename T>
	template <typename B>
	inline Degrees<B> Radians<T>::ToDegrees () const
	{
		return Degrees<B>( B(_value) * (B)Degrees<B>::Pi() * (B)Radians<B>::ReciporalPi() );
	}
	
	template <typename T>
	template <typename B>
	inline Radians<B> Radians<T>::To () const
	{
		return Radians<B>( B(_value) );
	}
	
	template <typename T>
	inline Radians<T>  Radians<T>::ToInterval (Self minValue, Self maxValue) const
	{
		// 0..+2Pi, 0..+Pi, -Pi..0, -2Pi..0, and other
		return (Self) GXMath::Wrap( _value, (T)minValue, (T)maxValue );
	}
	
	template <typename T>
	inline Radians<T>  Radians<T>::FromArcminutes (T value)
	{
		return Degrees<T>::FromArcminutes( value ).ToRadians();
	}
	
	template <typename T>
	inline Radians<T>  Radians<T>::FromArcseconds (T value)
	{
		return Degrees<T>::FromArcseconds( value ).ToRadians();
	}
	
	template <typename T>
	inline Radians<T>  Radians<T>::From (T deg, T arcmin, T arcsec)
	{
		return Degrees<T>()::From( degm arcmin, arcsec ).ToRadians();
	}
	


//--------------------------------- Degrees --------------------------------//

	template <typename T>
	inline Radians<T> Degrees<T>::ToRadians () const
	{
		return ToRadians<T>();
	}
	
	template <typename T>
	template <typename B>
	inline Radians<B> Degrees<T>::ToRadians () const
	{
		return Radians<B>( B(_value) * (B)Radians<B>::Pi() * (B)Degrees<B>::ReciporalPi() );
	}
	
	template <typename T>
	template <typename B>
	inline Degrees<B> Degrees<T>::To () const
	{
		return Degrees<B>( B(_value) );
	}
	
	template <typename T>
	inline Degrees<T>  Degrees<T>::ToInterval (Self minValue, Self maxValue) const
	{
		// 0..+2Pi, 0..+Pi, -Pi..0, -2Pi..0, and other
		return (Self) GXMath::Wrap( _value, (T)minValue, (T)maxValue );
	}
	
	template <typename T>
	inline Degrees<T>  Degrees<T>::From (T deg, T arcmin, T arcsec)
	{
		return Degrees<T>( deg + (arcmin / T(60.0)) + (arcsec / T(3600.0)) );
	}

	
/*
=================================================
	TypeInfo
=================================================
*/
	template <typename T>
	struct ::GX_STL::CompileTime::TypeInfo< GXMath::Radians<T> >
	{
		typedef GXMath::Radians<T>	type;
		typedef T					inner_type;
		
		template <typename OtherType>
		using CreateWith = GXMath::Radians< OtherType >;

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

	template <typename T>
	struct ::GX_STL::CompileTime::TypeInfo< GXMath::Degrees<T> >
	{
		typedef GXMath::Degrees<T>	type;
		typedef T					inner_type;
		
		template <typename OtherType>
		using CreateWith = GXMath::Degrees< OtherType >;

		enum {
			FLAGS	= (int)TypeInfo< inner_type >::FLAGS | int(_ctime_hidden_::WRAPPER),
		};
		
		static constexpr type	Max()		{ return type( TypeInfo< inner_type >::Max() ); }
		static constexpr type	Min()		{ return type( TypeInfo< inner_type >::Min() ); }
		static constexpr type	Inf()		{ return type(); }
		static constexpr type	NaN()		{ return type(); }
		
		static constexpr type	Epsilon()	{ return type( TypeInfo< inner_type >::Epsilon() ); }
		static constexpr uint	SignBit()	{ return TypeInfo< inner_type >::SignBit(); }
		static constexpr uint	Count()		{ return TypeInfo< inner_type >::Count(); }
	};


/*
=================================================
	Hash
=================================================
*/
	template <typename T>
	struct ::GX_STL::GXTypes::Hash< GXMath::Radians<T> > : private Hash<T>
	{
		typedef GXMath::Radians<T>			key_t;
		typedef Hash<T>						base_t;
		typedef typename base_t::result_t	result_t;

		result_t operator () (const key_t &x) const
		{
			return base_t::operator ()( x.ref() );
		}
	};
	
	
	template <typename T>
	struct ::GX_STL::GXTypes::Hash< GXMath::Degrees<T> > : private Hash<T>
	{
		typedef GXMath::Degrees<T>			key_t;
		typedef Hash<T>						base_t;
		typedef typename base_t::result_t	result_t;

		result_t operator () (const key_t &x) const
		{
			return base_t::operator ()( x.ref() );
		}
	};

}	// GXTypes
}	// GX_STL
