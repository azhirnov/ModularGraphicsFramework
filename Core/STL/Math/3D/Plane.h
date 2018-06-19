// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Math/3D/AxisAlignedBox.h"

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
		using Self		= Plane<T>;
		using Value_t	= T;
		using Vec3_t	= Vec<T,3>;
		using Rad_t		= Radians<T>;
		
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
		using _ESide_t	= typename ESide::type;


	// variables
	private:
		Vec3_t		_normal;
		T			_dist;


	// methods
	public:
		Plane ();
		Plane (const Vec3_t &normal, const T& dist);
		Plane (const Vec3_t &p0, const Vec3_t &p1, const Vec3_t &p2);

		void Set (const Vec3_t &normal, const T& dist);
		void Set (const Vec3_t &p0, const Vec3_t &p1, const Vec3_t &p2);

		void GetPerpendiculars (OUT Vec3_t &n0, OUT Vec3_t &n1, OUT Vec3_t &n2) const;

		_ESide_t		Intersect (const Vec3_t &point) const;
		_ESide_t		Intersect (const AABBox<T> &box) const;
		_ESide_t		Intersect (const Vec3_t &center, const Vec3_t &halfextent) const;
		
		Vec3_t &		Normal ()				{ return _normal; }
		Vec3_t const&	Normal ()	const		{ return _normal; }
		T &				Distance ()				{ return _dist; }
		T const &		Distance ()	const		{ return _dist; }

		Self &			Normalize ();
		T				Distance (const Vec3_t &point) const;
		Vec3_t			Project (const Vec3_t &point) const;

		Rad_t			AngleBetweenPlanes (const Self &other) const;

		bool			GetIntersection (const Self &other, OUT Vec3_t &result) const;
		bool			GetIntersection (const Vec3_t &rayDir, const Vec3_t &rayOrigin, OUT Vec3_t &result) const;

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
	inline Plane<T>::Plane (const Vec3_t &normal, const T& dist) : _normal(normal), _dist(dist)
	{}

	template <typename T>
	inline Plane<T>::Plane (const Vec3_t &p0, const Vec3_t &p1, const Vec3_t &p2) : _normal(), _dist(0)
	{
		Set( p0, p1, p2 );
	}
	
/*
=================================================
	Set
=================================================
*/
	template <typename T>
	inline void Plane<T>::Set (const Vec3_t &normal, const T& dist)
	{
		_normal	= normal.Normalized();
		_dist	= Abs( dist );
	}
	
/*
=================================================
	Set
=================================================
*/
	template <typename T>
	inline void Plane<T>::Set (const Vec3_t &p0, const Vec3_t &p1, const Vec3_t &p2)
	{
		Vec3_t	edge1 = p1 - p0,
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
	ND_ inline typename Plane<T>::ESide::type  Plane<T>::Intersect (const Vec3_t &point) const
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
	ND_ inline typename Plane<T>::ESide::type  Plane<T>::Intersect (const AABBox<T> &box) const
	{
		return Intersect( box.Center(), box.HalfExtent() );
	}
	
	template <typename T>
	ND_ inline typename Plane<T>::ESide::type  Plane<T>::Intersect (const Vec3_t &center, const Vec3_t &halfextent) const
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
	ND_ inline Plane<T> &  Plane<T>::Normalize ()
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
	ND_ inline T  Plane<T>::Distance (const Vec3_t &point) const
	{
		return _normal.Dot( point ) + _dist;
	}

/*
=================================================
	Project
=================================================
*/
	template <typename T>
	ND_ inline Vec<T,3>  Plane<T>::Project (const Vec3_t &point) const
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
	AngleBetweenPlanes
=================================================
*/
	template <typename T>
	ND_ inline Radians<T>  Plane<T>::AngleBetweenPlanes (const Self &other) const
	{
		return ACos( Dot( _normal, other._normal ) );
	}
	
/*
=================================================
	GetIntersection
=================================================
*/
	template <typename T>
	inline bool  Plane<T>::GetIntersection (const Self &other, OUT Vec3_t &result) const
	{
		const Vec3_t	n = Cross( _normal, other._normal ).Normalize();
		const T			d = n.LengthSqr();

		if ( IsZero( d ) )
			return false;

		result = n;
		return true;
	}
	
/*
=================================================
	GetIntersection
=================================================
*/
	template <typename T>
	ND_ inline bool  Plane<T>::GetIntersection (const Vec3_t &rayDir, const Vec3_t &rayOrigin, OUT Vec3_t &result) const
	{
		const Vec3_t dir = rayDir.Normalized();
		const T ndr = Dot( _normal, dir );

		if ( IsZero( ndr ) )
			return false;	// no intersection, line is parallel to the plane

		const T x = (_dist - Dot(_normal, rayOrigin)) / ndr;

		result = rayOrigin + dir * x;
		return true;
	}

/*
=================================================
	operator ==
=================================================
*/
	template <typename T>
	ND_ inline bool  Plane<T>::operator == (const Plane<T> &right) const
	{
		return Equals( _dist, right._dist ) and All( _normal == right._normal );
	}

/*
=================================================
	operator !=
=================================================
*/
	template <typename T>
	ND_ inline bool  Plane<T>::operator != (const Plane<T> &right) const
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
	ND_ inline Plane<T2>  Plane<T>::Convert () const
	{
		return Plane<T2>( _normal.template Convert<T2>(), T2(_dist) );
	}
	
/*
=================================================
	GetPerpendiculars
=================================================
*/
	template <typename T>
	inline void Plane<T>::GetPerpendiculars (OUT Vec3_t &n0, OUT Vec3_t &n1, OUT Vec3_t &n2) const
	{
		n0 = Cross( -Vec3_t(T(0), T(0), T(1)), _normal ).Normalize();
		n1 = Cross( -Vec3_t(T(0), T(1), T(0)), _normal ).Normalize();
		n2 = Cross( -Vec3_t(T(1), T(0), T(0)), _normal ).Normalize();
	}


}	// GXMath
}	// GX_STL
