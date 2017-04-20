// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "AxisAlignedBox.h"

namespace GX_STL
{
namespace GXMath
{

	//
	// Plane
	//

	template <typename T>
	struct Plane : public CompileTime::CopyQualifiers< T >
	{
	// types
	public:
		typedef Plane<T>	Self;
		typedef T			value_t;
		typedef Vec<T,3>	vec3_t;
		
		struct ESide
		{
			enum type
			{
				None	= 0,
				Positive,
				Negative,
				Both,
			};
		};
		typedef typename ESide::type		_ESide_t;


	// variables
	private:
		Vec<T,3>	_normal;
		T			_dist;


	// methods
	public:
		Plane ();
		Plane (const vec3_t &normal, const T& dist);
		Plane (const vec3_t &p0, const vec3_t &p1, const vec3_t &p2);

		void Set (const vec3_t &normal, const T& dist);
		void Set (const vec3_t &p0, const vec3_t &p1, const vec3_t &p2);

		void GetPerpendiculars (OUT vec3_t &n0, OUT vec3_t &n1, OUT vec3_t &n2) const;

		_ESide_t		Intersect (const vec3_t &point) const;
		_ESide_t		Intersect (const AABBox<T> &box) const;
		_ESide_t		Intersect (const vec3_t &center, const vec3_t &halfextent) const;
		
		Vec<T,3> &		Normal ()				{ return _normal; }
		Vec<T,3> const&	Normal ()	const		{ return _normal; }
		T &				Distance ()				{ return _dist; }
		T const &		Distance ()	const		{ return _dist; }

		Self &			Normalize ();
		T				Distance (const vec3_t &point) const;
		vec3_t			Project (const vec3_t &point) const;

		bool			operator == (const Self &right) const;
		bool			operator != (const Self &right) const;

		template <typename T2>
		Plane<T2>		Convert () const;
	};


	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	inline Plane<T>::Plane () : _normal(), _dist(0)
	{}

	template <typename T>
	inline Plane<T>::Plane (const Vec<T,3> &normal, const T& dist) : _normal(normal), _dist(dist)
	{}

	template <typename T>
	inline Plane<T>::Plane (const Vec<T,3> &p0, const Vec<T,3> &p1, const Vec<T,3> &p2) : _normal(), _dist(0)
	{
		Set( p0, p1, p2 );
	}
	
/*
=================================================
	Set
=================================================
*/
	template <typename T>
	inline void Plane<T>::Set (const Vec<T,3> &normal, const T& dist)
	{
		_normal	= normal;
		_dist	= dist;
	}
	
/*
=================================================
	Set
=================================================
*/
	template <typename T>
	inline void Plane<T>::Set (const Vec<T,3> &p0, const Vec<T,3> &p1, const Vec<T,3> &p2)
	{
		Vec<T,3>	edge1 = p1 - p0,
					edge2 = p2 - p0;

		_normal = Cross( edge1, edge2 ).Normalize();
		_dist   = -_normal.Dot( p0 );
	}
	
/*
=================================================
	Intersect
=================================================
*/
	template <typename T>
	inline typename Plane<T>::ESide::type  Plane<T>::Intersect (const Vec<T,3> &point) const
	{
		const T	d = Distance( point );

		if ( d < T(0) )	return ESide::Negative;
		if ( d > T(0) )	return ESide::Positive;
		return ESide::None;
	}
	
/*
=================================================
	Intersect
=================================================
*/
	template <typename T>
	inline typename Plane<T>::ESide::type  Plane<T>::Intersect (const AABBox<T> &box) const
	{
		return Intersect( box.Center(), box.HalfExtent() );
	}
	
	template <typename T>
	inline typename Plane<T>::ESide::type  Plane<T>::Intersect (const Vec<T,3> &center, const Vec<T,3> &halfextent) const
	{
		const T	d			= Distance( center );
		const T	max_abs_d	= _normal.DotAbs( halfextent );

		if ( d < -max_abs_d )	return ESide::Negative;
		if ( d >  max_abs_d )	return ESide::Positive;
		return ESide::Both;
	}

/*
=================================================
	Normalize
=================================================
*/
	template <typename T>
	inline Plane<T> & Plane<T>::Normalize ()
	{
		const T	len = _normal.Length();

		if ( IsNotZero( len ) )
		{
			const T	inv_len = T(1) / len;
			_normal *= inv_len;
			_dist   *= inv_len;
		}

		return *this;
	}

/*
=================================================
	Distance
=================================================
*/
	template <typename T>
	inline T Plane<T>::Distance (const Vec<T,3> &point) const
	{
		return _normal.Dot( point ) + _dist;
	}

/*
=================================================
	Project
=================================================
*/
	template <typename T>
	inline Vec<T,3> Plane<T>::Project (const Vec<T,3> &point) const
	{
		Matrix<T,3,3>	mat;

		mat(0,0) = T(1) - _normal.x * _normal.x;
		mat(0,1) =		- _normal.x * _normal.y;
		mat(0,2) =		- _normal.x * _normal.z;
		mat(1,0) =		- _normal.y * _normal.x;
		mat(1,1) = T(1) - _normal.y * _normal.y;
		mat(1,2) =		- _normal.y * _normal.z;
		mat(2,0) =		- _normal.z * _normal.x;
		mat(2,1) =		- _normal.z * _normal.y;
		mat(2,2) = T(1) - _normal.z * _normal.z;

		return mat * point;
	}

/*
=================================================
	operator ==
=================================================
*/
	template <typename T>
	inline bool Plane<T>::operator == (const Plane<T> &right) const
	{
		return Equals( _dist, right._dist ) and _normal == right._normal;
	}

/*
=================================================
	operator !=
=================================================
*/
	template <typename T>
	inline bool Plane<T>::operator != (const Plane<T> &right) const
	{
		return not ( *this == right );
	}

/*
=================================================
	Convert
=================================================
*/
	template <typename T>
	template <typename T2>
	inline Plane<T2>  Plane<T>::Convert () const
	{
		return TPlane<T2>( _normal.Convert<T2>(), T2(_dist) );
	}
	
/*
=================================================
	GetPerpendiculars
=================================================
*/
	template <typename T>
	inline void Plane<T>::GetPerpendiculars (OUT vec3_t &n0, OUT vec3_t &n1, OUT vec3_t &n2) const
	{
		n0 = Cross( -vec3_t(T(0), T(0), T(1)), _normal ).Normalized();
		n1 = Cross( -vec3_t(T(0), T(1), T(0)), _normal ).Normalized();
		n2 = Cross( -vec3_t(T(1), T(0), T(0)), _normal ).Normalized();
	}


}	// GXMath
}	// GX_STL
