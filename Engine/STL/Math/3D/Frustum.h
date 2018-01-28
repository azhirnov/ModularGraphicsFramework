// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Math/3D/Plane.h"

namespace GX_STL
{
namespace GXMath
{
	
	//
	// Frustum
	//

	template <typename T>
	struct Frustum
	{
	// types
	public:
		using Self		= Frustum<T>;
		using Value_t	= T;
		using Vec3_t	= Vec<T,3>;
		using Plane_t	= Plane<T>;

		struct EPlane
		{
			enum type
			{
				Near,
				Far,
				Left,
				Right,
				Top,
				Bottom,
				_Count
			};
		};

	private:
		using _EPlane_t		= typename EPlane::type;
		using _PlaneArr_t	= StaticArray< Plane_t, EPlane::_Count >;


	// variables
	private:
		_PlaneArr_t		_planes;


	// methods
	public:
		void Setup (const Matrix<T,4,4> &viewProjMat);

		bool IsVisible (const AABBox<T> &box) const;
		bool IsVisible (const Vec3_t &point) const;
		//bool IsVisible (const Frustum<T> &frustum) const;
		bool IsVisible (const Vec3_t &center, T radius) const;
		bool IsVisible (const Vec3_t &center, const Vec3_t &halfextents) const;

		Plane_t const&	GetPlane (_EPlane_t loc) const	{ return _planes[loc]; }

		void GetRays (OUT Vec3_t &leftTop, OUT Vec3_t &leftBottom, OUT Vec3_t &rightTop, OUT Vec3_t &rightBottom) const;

		template <typename T2>
		Frustum<T2> Convert () const;
	};


	
/*
=================================================
	Setup
=================================================
*/
	template <typename T>
	inline void Frustum<T>::Setup (const Matrix<T,4,4> &mat)
	{
		_planes[EPlane::Top].Set(    Vec3_t( mat(0,3) - mat(0,1), mat(1,3) - mat(1,1), mat(2,3) - mat(2,1) ), -mat(3,3) + mat(3,1) );
		_planes[EPlane::Bottom].Set( Vec3_t( mat(0,3) + mat(0,1), mat(1,3) + mat(1,1), mat(2,3) + mat(2,1) ), -mat(3,3) - mat(3,1) );
		_planes[EPlane::Left].Set(   Vec3_t( mat(0,3) + mat(0,0), mat(1,3) + mat(1,0), mat(2,3) + mat(2,0) ), -mat(3,3) - mat(3,0) );
		_planes[EPlane::Right].Set(  Vec3_t( mat(0,3) - mat(0,0), mat(1,3) - mat(1,0), mat(2,3) - mat(2,0) ), -mat(3,3) + mat(3,0) );
		_planes[EPlane::Near].Set(   Vec3_t( mat(0,3) + mat(0,2), mat(1,3) + mat(1,2), mat(2,3) + mat(2,2) ), -mat(3,3) - mat(3,2) );
		_planes[EPlane::Far].Set(    Vec3_t( mat(0,3) - mat(0,2), mat(1,3) - mat(1,2), mat(2,3) - mat(2,2) ), -mat(3,3) + mat(3,2) );
	}
	
/*
=================================================
	IsVisible
=================================================
*/	
	template <typename T>
	inline CHECKRES(bool)  Frustum<T>::IsVisible (const Vec<T,3> &point) const
	{
		typedef typename Plane<T>::ESide		PSide;

		return ( _planes[EPlane::Left  ].Intersect( point ) != PSide::Negative and
				 _planes[EPlane::Right ].Intersect( point ) != PSide::Negative and
				 _planes[EPlane::Top   ].Intersect( point ) != PSide::Negative and
				 _planes[EPlane::Bottom].Intersect( point ) != PSide::Negative and
				 _planes[EPlane::Near  ].Intersect( point ) != PSide::Negative and
				 _planes[EPlane::Far   ].Intersect( point ) != PSide::Negative );
	}
	
/*
=================================================
	IsVisible
=================================================
*/
	template <typename T>
	inline CHECKRES(bool)  Frustum<T>::IsVisible (const AABBox<T> &box) const
	{
		return IsVisible( box.Center(), box.HalfExtent() );
	}
	
/*
=================================================
	IsVisible
=================================================
*/
	template <typename T>
	inline CHECKRES(bool)  Frustum<T>::IsVisible (const Vec<T,3> &center, const T radius) const
	{
		return ( _planes[EPlane::Left  ].Distance( center ) >= -radius and
				 _planes[EPlane::Right ].Distance( center ) >= -radius and
				 _planes[EPlane::Top   ].Distance( center ) >= -radius and
				 _planes[EPlane::Bottom].Distance( center ) >= -radius and
				 _planes[EPlane::Near  ].Distance( center ) >= -radius and
				 _planes[EPlane::Far   ].Distance( center ) >= -radius );
	}
	
/*
=================================================
	IsVisible
=================================================
*/
	template <typename T>
	inline CHECKRES(bool)  Frustum<T>::IsVisible (const Vec<T,3> &center, const Vec<T,3> &halfextents) const
	{
		typedef typename Plane<T>::ESide		PSide;

		return ( _planes[EPlane::Left  ].Intersect( center, halfextents ) != PSide::Negative and
				 _planes[EPlane::Right ].Intersect( center, halfextents ) != PSide::Negative and
				 _planes[EPlane::Top   ].Intersect( center, halfextents ) != PSide::Negative and
				 _planes[EPlane::Bottom].Intersect( center, halfextents ) != PSide::Negative and
				 _planes[EPlane::Near  ].Intersect( center, halfextents ) != PSide::Negative and
				 _planes[EPlane::Far   ].Intersect( center, halfextents ) != PSide::Negative );
	}
	
/*
=================================================
	IsVisible
=================================================
*/
	/*template <typename T>
	inline CHECKRES(bool)  Frustum<T>::IsVisible (const Frustum<T> &frustum) const
	{
		TODO( "intersect plane" );
		return false;
	}*/
	
/*
=================================================
	GetRays
=================================================
*/
	template <typename T>
	void Frustum<T>::GetRays (OUT Vec3_t &leftTop, OUT Vec3_t &leftBottom, OUT Vec3_t &rightTop, OUT Vec3_t &rightBottom) const
	{
		auto const&	p0 = GetPlane( EPlane::Left );
		auto const&	p1 = GetPlane( EPlane::Right );
		auto const&	p2 = GetPlane( EPlane::Top );
		auto const&	p3 = GetPlane( EPlane::Bottom );

		p3.GetIntersection( p0, OUT leftBottom );
		p0.GetIntersection( p2, OUT leftTop );
		p1.GetIntersection( p3, OUT rightBottom );
		p2.GetIntersection( p1, OUT rightTop );
	}

/*
=================================================
	Convert
=================================================
*/
	template <typename T>
	template <typename T2>
	inline CHECKRES(Frustum<T2>)  Frustum<T>::Convert () const
	{
		Frustum<T2>	ret;

		ret._planes[0] = _planes[0].template Convert<T2>();
		ret._planes[1] = _planes[1].template Convert<T2>();
		ret._planes[2] = _planes[2].template Convert<T2>();
		ret._planes[3] = _planes[3].template Convert<T2>();
		ret._planes[4] = _planes[4].template Convert<T2>();
		ret._planes[5] = _planes[5].template Convert<T2>();

		return ret;
	}


}	// GXMath
}	// GX_STL
