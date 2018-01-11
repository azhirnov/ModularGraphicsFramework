// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Math/Mathematics.h"

namespace GX_STL
{
namespace GXMath
{
namespace Spline
{



	//
	// Cubic Hermite
	//
	/*
	template <typename VecType>
	struct CubicHermite
	{
	// types
	public:
		typedef VecType						Vec_t;
		typedef typename VecType::Value_t	Value_t;
		
	// variables
	private:
		Vec_t	_p0, _p1, _p2, _p3;
		
	// methods
	public:
		CubicHermite () {}

		void Create ()
		{
			// Cubic Hermite Spline
			template <typename T, usize I>
			static Vec<T,I,U>  CubicHermite(const Vec<T,I,U> &p0, const Vec<T,I,U> &tangent0,
										   const Vec<T,I,U> &p1, const Vec<T,I,U> &tangent1,
										   const T& a)
			{
				const T	a2 = a * a;
				const T	a3 = a2 * a;

				return	( T(2.0) * a3 - T(3.0) * a2 + T(1.0) ) * p0 +
						( a3 - T(2.0) * a2 + a ) * tangent0 +
						( -T(2.0) * a3 + T(3.0) * a2 ) * p1 +
						( a3 - a2 ) * tangent1;
			}
		}

		Vec_t Interpolate (T a) const
		{
			return Vec_t();
		}
	};
	*/


	//
	// Centripetal Catmull-Rom (best quality)
	//
	
	template <typename VecType>
	struct CentripetalCatmullRom
	{
	// types
	public:
		typedef VecType						Vec_t;
		typedef typename VecType::Value_t	Value_t;

	// variables
	private:
		Vec_t	_c0, _c1, _c2, _c3;

	// methods
	public:
		CentripetalCatmullRom ()
		{}
		
		CentripetalCatmullRom (const Vec_t &p0, const Vec_t &p1, const Vec_t &p2, const Vec_t &p3)
		{
			Create( p0, p1, p2, p3 );
		}

		void Create (const Vec_t &p0, const Vec_t &p1, const Vec_t &p2, const Vec_t &p3)
		{
			typedef Value_t	T;

			T const	p	= T(0.25);
			T const e	= T(1e-4);

			T 		dt0	= Pow( p0.DistanceSqr( p1 ), p );
			T 		dt1	= Pow( p1.DistanceSqr( p2 ), p );
			T 		dt2	= Pow( p2.DistanceSqr( p3 ), p );

			if ( dt1 < e )	dt1 = T(1);
			if ( dt0 < e )	dt0 = dt1;
			if ( dt2 < e )	dt2 = dt1;

			Vec_t const	t1 = ( (p1 - p0) / dt0 - (p2 - p0) / (dt0 + dt1) + (p2 - p1) / dt1 ) * dt1;
			Vec_t const	t2 = ( (p2 - p1) / dt1 - (p3 - p1) / (dt1 + dt2) + (p3 - p2) / dt2 ) * dt1;

			_c0 = p1;
			_c1 = t1;
			_c2 = T(-3) * p1 + T(3) * p2 - T(2) * t1 - t2;
			_c3 = T(2) * p1 - T(2) * p2 + t1 + t2;
		}

		Vec_t Interpolate (Value_t a) const
		{
			return (_c0) + (_c1 * a) + (_c2 * a * a) + (_c3 * a * a * a);
		}
	};



	//
	// Chordal Catmull-Rom
	//
	
	template <typename VecType>
	struct ChordalCatmullRom
	{
	// types
	public:
		typedef VecType						Vec_t;
		typedef typename VecType::Value_t	Value_t;
		
	// variables
	private:
		Vec_t	_c0, _c1, _c2, _c3;

	// methods
	public:
		ChordalCatmullRom ()
		{}

		ChordalCatmullRom (const Vec_t &p0, const Vec_t &p1, const Vec_t &p2, const Vec_t &p3)
		{
			Create( p0, p1, p2, p3 );
		}

		void Create (const Vec_t &p0, const Vec_t &p1, const Vec_t &p2, const Vec_t &p3)
		{
			typedef Value_t	T;

			_c0	= ( T(2.0) * p1 );
			_c1 = ( -p0 + p2 );
			_c2 = ( T(2.0) * p0 - T(5.0) * p1 + T(4.0) * p2 - p3 );
			_c3 = ( -p0 + T(3.0) * p1 - T(3.0) * p2 + p3 );
		}

		Vec_t Interpolate (Value_t a) const
		{
			return Value_t(0.5) * ( (_c0) + (_c1 * a) + (_c2 * a * a) + (_c3 * a * a * a) );
		}
	};



	//
	// 
	//
	/*
	template <typename VecType>
	struct Unknown
	{
	// types
	public:
		typedef VecType						Vec_t;
		typedef typename VecType::Value_t	Value_t;
		
	// variables
	private:
		Vec_t	_c0, _c1, _c2, _c3;

	// methods
	public:
		Unknown ()
		{}

		Unknown (const Vec_t &p0, const Vec_t &p1, const Vec_t &p2, const Vec_t &p3)
		{
			Create( p0, p1, p2, p3 );
		}

		void Create (const Vec_t &p0, const Vec_t &p1, const Vec_t &p2, const Vec_t &p3)
		{
			_c0 = (p3 - p2) - (p0 - p1);
			_c1 = (p0 - p1) - _c0;
			_c2 = p2 - p0;
			_c3 = p1;
		}

		Vec_t Interpolate (Value_t a) const
		{
			return (_c0 * a * a * a) + (_c1 * a * a) + (_c2 * a) + _c3;
		}
	};

	
	template <typename VecType>
	struct Unknown
	{
	// types
	public:
		typedef VecType						Vec_t;
		typedef typename VecType::Value_t	Value_t;
		
	// variables
	private:
		Vec_t	_c0, _c1, _c2, _c3;

	// methods
	public:
		Unknown ()
		{}

		Unknown (const Vec_t &p0, const Vec_t &p1, const Vec_t &p2, const Vec_t &p3)
		{
			Create( p0, p1, p2, p3 );
		}

		void Create (const Vec_t &p0, const Vec_t &p1, const Vec_t &p2, const Vec_t &p3)
		{
			_c0 = (p1 - p2) * T(1.5) + (p3 - p0) * T(0.5);
			_c1 = p1 * T(-2.5) + p2 * T(2) - p3 * T(0.5) + p0;
			_c2 = (p3 - p0) * T(0.5);
			_c3 = p1;
		}

		Vec_t Interpolate (Value_t a) const
		{
			return (_c0 * a * a * a) + (_c1 * a * a) + (_c2 * a) + _c3;
		}
	};
	*/
	

	//
	// B-Spline
	//

	template <typename VecType>
	struct BSpline
	{
	// types
	public:
		typedef VecType						Vec_t;
		typedef typename VecType::Value_t	Value_t;

	// variables
	private:
		Vec_t	_c0, _c1, _c2, _c3;

	// methods
	public:
		BSpline ()
		{}
		
		BSpline (const Vec_t &p0, const Vec_t &p1, const Vec_t &p2, const Vec_t &p3)
		{
			Create( p0, p1, p2, p3 );
		}

		void Create (const Vec_t &p0, const Vec_t &p1, const Vec_t &p2, const Vec_t &p3)
		{
			typedef Value_t	T;

			_c3 = (p3 - p0) / T(6) + (p1 - p2) / T(2);
			_c2 = (p0 + p2) / T(2) - p1;
			_c1 = (p2 - p0) / T(2);
			_c0 = (p0 + T(4) * p1 + p2) / T(6);
		}

		Vec_t Interpolate (Value_t a) const
		{
			return (_c3 * a * a * a) + (_c2 * a * a) + (_c1 * a) + _c0;
		}
	};

	
	/*
	//
	// Cubic Spline
	//

	template <typename VecType>
	struct CubicSpline
	{
	// types
	public:
		typedef VecType						Vec_t;
		typedef typename VecType::Value_t	Value_t;

	// variables
	private:
		Vec_t	_c0, _c1, _c2, _c3;

	// methods
	public:
		CubicSpline ()
		{}
		
		CubicSpline (const Vec_t &p0, const Vec_t &p1, const Vec_t &p2, const Vec_t &p3)
		{
			Create( p0, p1, p2, p3 );
		}

		void Create (const Vec_t &p0, const Vec_t &p1, const Vec_t &p2, const Vec_t &p3)
		{
			_c0 = p0;
			_c1 = p1;
			_c2 = p2;
			_c3 = p3;
		}

		Vec_t Interpolate (Value_t a) const
		{
			return	((_c3 - _c2) - (_c0 - _c1)) * a * a * a +
					(_c3 - _c2) * a * a +
					(_c2 - _c0) * a +
					_c1;
		}
	};*/

}	// Spline
}	// GXMath
}	// GX_STL
