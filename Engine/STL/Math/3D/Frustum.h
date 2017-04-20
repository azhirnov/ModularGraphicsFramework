// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Plane.h"

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
		typedef Frustum<T>		Self;
		typedef T				value_t;
		typedef Vec<T,3>		vec3_t;

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

		typedef StaticArray< Plane<T>, EPlane::_Count >		Planes_t;


	// variables
	private:
		Planes_t	_planes;


	// methods
	public:
		void Setup (const Matrix<T,4,4> &viewProjMat);

		bool IsVisible (const AABBox<T> &box) const;
		bool IsVisible (const vec3_t &point) const;
		//bool IsVisible (const Frustum<T> &frustum) const;
		bool IsVisible (const vec3_t &center, T radius) const;
		bool IsVisible (const vec3_t &center, const vec3_t &halfextents) const;

		template <typename T2>
		Frustum<T2> Convert () const;
	};


	
/*
=================================================
	Setup
=================================================
*/
	template <typename T>
	inline void Frustum<T>::Setup (const Matrix<T,4,4> &vp)
	{
		_planes[EPlane::Left].Normal().x = vp(3,0) + vp(0,0);
		_planes[EPlane::Left].Normal().y = vp(3,1) + vp(0,1);
		_planes[EPlane::Left].Normal().z = vp(3,2) + vp(0,2);
		_planes[EPlane::Left].Distance() = vp(3,3) + vp(0,3);
		_planes[EPlane::Left].Normalize();
		
		_planes[EPlane::Right].Normal().x = vp(3,0) - vp(0,0);
		_planes[EPlane::Right].Normal().y = vp(3,1) - vp(0,1);
		_planes[EPlane::Right].Normal().z = vp(3,2) - vp(0,2);
		_planes[EPlane::Right].Distance() = vp(3,3) - vp(0,3);
		_planes[EPlane::Right].Normalize();

		_planes[EPlane::Top].Normal().x = vp(3,0) - vp(1,0);
		_planes[EPlane::Top].Normal().x = vp(3,1) - vp(1,1);
		_planes[EPlane::Top].Normal().x = vp(3,2) - vp(1,2);
		_planes[EPlane::Top].Distance() = vp(3,3) - vp(1,3);
		_planes[EPlane::Top].Normalize();
		
		_planes[EPlane::Bottom].Normal().x = vp(3,0) + vp(1,0);
		_planes[EPlane::Bottom].Normal().x = vp(3,1) + vp(1,1);
		_planes[EPlane::Bottom].Normal().x = vp(3,2) + vp(1,2);
		_planes[EPlane::Bottom].Distance() = vp(3,3) + vp(1,3);
		_planes[EPlane::Bottom].Normalize();
		
		_planes[EPlane::Near].Normal().x = vp(3,0) + vp(2,0);
		_planes[EPlane::Near].Normal().x = vp(3,1) + vp(2,1);
		_planes[EPlane::Near].Normal().x = vp(3,2) + vp(2,2);
		_planes[EPlane::Near].Distance() = vp(3,3) + vp(2,3);
		_planes[EPlane::Near].Normalize();
		
		_planes[EPlane::Far].Normal().x = vp(3,0) - vp(2,0);
		_planes[EPlane::Far].Normal().x = vp(3,1) - vp(2,1);
		_planes[EPlane::Far].Normal().x = vp(3,2) - vp(2,2);
		_planes[EPlane::Far].Distance() = vp(3,3) - vp(2,3);
		_planes[EPlane::Far].Normalize();
	}
	
/*
=================================================
	IsVisible
=================================================
*/	
	template <typename T>
	inline bool Frustum<T>::IsVisible (const Vec<T,3> &point) const
	{
		typedef Plane<T>::ESide		PSide;

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
	inline bool Frustum<T>::IsVisible (const AABBox<T> &box) const
	{
		return IsVisible( box.Center(), box.HalfExtent() );
	}
	
/*
=================================================
	IsVisible
=================================================
*/
	template <typename T>
	inline bool Frustum<T>::IsVisible (const Vec<T,3> &center, const T radius) const
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
	inline bool Frustum<T>::IsVisible (const Vec<T,3> &center, const Vec<T,3> &halfextents) const
	{
		typedef Plane<T>::ESide		PSide;

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
	inline bool Frustum<T>::IsVisible (const Frustum<T> &frustum) const
	{
		TODO( "intersect plane" );
		return false;
	}*/
	
/*
=================================================
	Convert
=================================================
*/
	template <typename T>
	template <typename T2>
	inline Frustum<T2> Frustum<T>::Convert () const
	{
		Frustum<T2>	ret;

		ret._planes[0] = _planes[0].Convert<T2>();
		ret._planes[1] = _planes[1].Convert<T2>();
		ret._planes[2] = _planes[2].Convert<T2>();
		ret._planes[3] = _planes[3].Convert<T2>();
		ret._planes[4] = _planes[4].Convert<T2>();
		ret._planes[5] = _planes[5].Convert<T2>();

		return ret;
	}


}	// GXMath
}	// GX_STL
