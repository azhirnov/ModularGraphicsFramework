// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Description:	4x2, 4x3, 4x4 matrices
*/

#pragma once

#include "Core/STL/Math/MatrixUtils.h"
#include "Core/STL/Math/2D/Rectangle.h"

namespace GX_STL
{
namespace GXMath
{



	//
	// Matrix 4x2
	//
	
#define C	4
#define R	2
#define MATRIX_METHODS() \
		Matrix (T _00, T _10, T _20, T _30, \
				T _01, T _11, T _21, T _31); \
		Matrix (const Col_t &c0, \
				const Col_t &c1, \
				const Col_t &c2, \
				const Col_t &c3);


#include "MatrixCR.h"
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>::Matrix (T _00, T _10, T _20, T _30,
								   T _01, T _11, T _21, T _31)
	{
		_v[0][0] = _00;		_v[1][0] = _10;		_v[2][0] = _20;		_v[3][0] = _30;
		_v[0][1] = _01;		_v[1][1] = _11;		_v[2][1] = _21;		_v[3][1] = _31;
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>::Matrix (const Col_t &c0,
								   const Col_t &c1,
								   const Col_t &c2,
								   const Col_t &c3)
	{
		UnsafeMem::MemCopy( _v[0], c0.ptr(), sizeof(T)*R );
		UnsafeMem::MemCopy( _v[1], c1.ptr(), sizeof(T)*R );
		UnsafeMem::MemCopy( _v[2], c2.ptr(), sizeof(T)*R );
		UnsafeMem::MemCopy( _v[3], c3.ptr(), sizeof(T)*R );
	}

#undef	R
#undef	C
#undef	MATRIX_METHODS

	
	

	//
	// Matrix 4x3
	//
	
#define C	4
#define R	3
#define MATRIX_METHODS() \
		Matrix (T _00, T _10, T _20, T _30, \
				T _01, T _11, T _21, T _31, \
				T _02, T _12, T _22, T _32); \
		Matrix (const Col_t &c0, \
				const Col_t &c1, \
				const Col_t &c2, \
				const Col_t &c3);


#include "MatrixCR.h"
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>::Matrix (T _00, T _10, T _20, T _30,
								   T _01, T _11, T _21, T _31,
								   T _02, T _12, T _22, T _32)
	{
		_v[0][0] = _00;		_v[1][0] = _10;		_v[2][0] = _20;		_v[3][0] = _30;
		_v[0][1] = _01;		_v[1][1] = _11;		_v[2][1] = _21;		_v[3][1] = _31;
		_v[0][2] = _02;		_v[1][2] = _12;		_v[2][2] = _22;		_v[3][2] = _32;
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>::Matrix (const Col_t &c0,
								   const Col_t &c1,
								   const Col_t &c2,
								   const Col_t &c3)
	{
		UnsafeMem::MemCopy( _v[0], c0.ptr(), sizeof(T)*R );
		UnsafeMem::MemCopy( _v[1], c1.ptr(), sizeof(T)*R );
		UnsafeMem::MemCopy( _v[2], c2.ptr(), sizeof(T)*R );
		UnsafeMem::MemCopy( _v[3], c3.ptr(), sizeof(T)*R );
	}

#undef	R
#undef	C
#undef	MATRIX_METHODS

	
	

	//
	// Matrix 4x4
	//
	
#define C	4
#define R	4
#define MATRIX_METHODS() \
		Matrix (T _00, T _10, T _20, T _30, \
				T _01, T _11, T _21, T _31, \
				T _02, T _12, T _22, T _32, \
				T _03, T _13, T _23, T _33); \
		Matrix (const Col_t &c0, \
				const Col_t &c1, \
				const Col_t &c2, \
				const Col_t &c3); \
		\
		Col3_t const &	Translation () const	{ return *Cast< Col3_t const*>( AddressOf((*this)(C-1)) ); } \
		Col3_t		 &	Translation ()			{ return *Cast< Col3_t *>( AddressOf((*this)(C-1)) ); } \
		\
		Self		Inverse () const; \
		\
		Self		ToBillboard () const; \
		static Self	SphericalBillboard (const Vec3_t &cameraPos, const Vec3_t &pos); \
		\
		static Self	BuildPerspective (const Radians<T>& fovY, const T& screenAspect, const Vec2_t &clipPlanes); \
		static Self	BuildRealPerspective (const Vec2_t &screenSize, const Vec2_t &clipPlanes, const T& dist); \
		static Self	BuildOrtho (const Rectangle<T> &rect, const Vec2_t &clipPlanes); \
		static Self	BuildOrtho2D (const Rectangle<T> &rect); \
		static Self	BuildFrustum (const Rectangle<T> &rect, const Vec2_t &clipPlanes); \
		static Self	BuildLookAt (const Vec3_t &pos, const Vec3_t &center, const Vec3_t &upVec); \
		/*Self		GetTextureMatrix () const;*/ \
		\
		static Vec3_t Project (const Vec3_t &pos, const Self &mvp, const Rectangle<T> &viewport); \
		static Vec3_t UnProject (const Vec3_t &pos, const Self &mvpInverse, const Rectangle<T> &viewport); \


#include "MatrixCR.h"
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>::Matrix (T _00, T _10, T _20, T _30,
								   T _01, T _11, T _21, T _31,
								   T _02, T _12, T _22, T _32,
								   T _03, T _13, T _23, T _33)
	{
		_v[0][0] = _00;		_v[1][0] = _10;		_v[2][0] = _20;		_v[3][0] = _30;
		_v[0][1] = _01;		_v[1][1] = _11;		_v[2][1] = _21;		_v[3][1] = _31;
		_v[0][2] = _02;		_v[1][2] = _12;		_v[2][2] = _22;		_v[3][2] = _32;
		_v[0][3] = _03;		_v[1][3] = _13;		_v[2][3] = _23;		_v[3][3] = _33;
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>::Matrix (const Col_t &c0,
								   const Col_t &c1,
								   const Col_t &c2,
								   const Col_t &c3)
	{
		UnsafeMem::MemCopy( _v[0], c0.ptr(), sizeof(T)*R );
		UnsafeMem::MemCopy( _v[1], c1.ptr(), sizeof(T)*R );
		UnsafeMem::MemCopy( _v[2], c2.ptr(), sizeof(T)*R );
		UnsafeMem::MemCopy( _v[3], c3.ptr(), sizeof(T)*R );
	}
	
/*
=================================================
	Inverse
=================================================
*/
	template <typename T, ulong U>
	ND_ inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::Inverse () const
	{
		const T s00 = _v[2][2] * _v[3][3] - _v[3][2] * _v[2][3];
		const T s01 = _v[2][1] * _v[3][3] - _v[3][1] * _v[2][3];
		const T s02 = _v[2][1] * _v[3][2] - _v[3][1] * _v[2][2];
		const T s03 = _v[2][0] * _v[3][3] - _v[3][0] * _v[2][3];
		const T s04 = _v[2][0] * _v[3][2] - _v[3][0] * _v[2][2];
		const T s05 = _v[2][0] * _v[3][1] - _v[3][0] * _v[2][1];
		const T s06 = _v[1][2] * _v[3][3] - _v[3][2] * _v[1][3];
		const T s07 = _v[1][1] * _v[3][3] - _v[3][1] * _v[1][3];
		const T s08 = _v[1][1] * _v[3][2] - _v[3][1] * _v[1][2];
		const T s09 = _v[1][0] * _v[3][3] - _v[3][0] * _v[1][3];
		const T s10 = _v[1][0] * _v[3][2] - _v[3][0] * _v[1][2];
		const T s11 = _v[1][1] * _v[3][3] - _v[3][1] * _v[1][3];
		const T s12 = _v[1][0] * _v[3][1] - _v[3][0] * _v[1][1];
		const T s13 = _v[1][2] * _v[2][3] - _v[2][2] * _v[1][3];
		const T s14 = _v[1][1] * _v[2][3] - _v[2][1] * _v[1][3];
		const T s15 = _v[1][1] * _v[2][2] - _v[2][1] * _v[1][2];
		const T s16 = _v[1][0] * _v[2][3] - _v[2][0] * _v[1][3];
		const T s17 = _v[1][0] * _v[2][2] - _v[2][0] * _v[1][2];
		const T s18 = _v[1][0] * _v[2][1] - _v[2][0] * _v[1][1];

		const Self	inv(  _v[1][1] * s00 - _v[1][2] * s01 + _v[1][3] * s02,
						- _v[1][0] * s00 + _v[1][2] * s03 - _v[1][3] * s04,
						  _v[1][0] * s01 - _v[1][1] * s03 + _v[1][3] * s05,
						- _v[1][0] * s02 + _v[1][1] * s04 - _v[1][2] * s05,

						- _v[0][1] * s00 + _v[0][2] * s01 - _v[0][3] * s02,
						  _v[0][0] * s00 - _v[0][2] * s03 + _v[0][3] * s04,
						- _v[0][0] * s01 + _v[0][1] * s03 - _v[0][3] * s05,
						  _v[0][0] * s02 - _v[0][1] * s04 + _v[0][2] * s05,

						  _v[0][1] * s06 - _v[0][2] * s07 + _v[0][3] * s08,
						- _v[0][0] * s06 + _v[0][2] * s09 - _v[0][3] * s10,
						  _v[0][0] * s11 - _v[0][1] * s09 + _v[0][3] * s12,
						- _v[0][0] * s08 + _v[0][1] * s10 - _v[0][2] * s12,

						- _v[0][1] * s13 + _v[0][2] * s14 - _v[0][3] * s15,
						  _v[0][0] * s13 - _v[0][2] * s16 + _v[0][3] * s17,
						- _v[0][0] * s14 + _v[0][1] * s16 - _v[0][3] * s18,
						  _v[0][0] * s15 - _v[0][1] * s17 + _v[0][2] * s18 );

		const T	det =	_v[0][0] * inv(0,0) + _v[0][1] * inv(1,0) +
						_v[0][2] * inv(2,0) + _v[0][3] * inv(3,0);

		return ( inv / det );
	}
	
/*
=================================================
	ToBillboard
=================================================
*/
	template <typename T, ulong U>
	ND_ inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::ToBillboard () const
	{
		return Self(
			1,				0,				0,				(*this)(3,0),
			0,				1,				0,				(*this)(3,1),
			0,				0,				1,				(*this)(3,2),
			(*this)(0,3),	(*this)(1,3),	(*this)(2,3),	(*this)(3,3) );
	}
	
/*
=================================================
	SphericalBillboard
=================================================
*/
	template <typename T, ulong U>
	ND_ inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::SphericalBillboard (const Vec3_t &cameraPos, const Vec3_t &pos)
	{
		Vec3_t	v_look_at( 0, 0, 1 ),
				v_obj_to_cam_proj( cameraPos.x - pos.x, 0, cameraPos.z - pos.z ),
				v_obj_to_cam,
				v_up_aux;
		Self	s_mv;
		T		t_angle_cosine;


		v_obj_to_cam_proj.Normalize();
		v_up_aux		= Cross( v_look_at, v_obj_to_cam_proj );
		t_angle_cosine	= Dot( v_look_at, v_obj_to_cam_proj );

		if ( t_angle_cosine < T(0.9999) and t_angle_cosine > T(-0.9999) )
			s_mv *= Rotation( acos(t_angle_cosine), v_up_aux );


		v_obj_to_cam	= (cameraPos - pos).Normalized();
		t_angle_cosine	= Dot( v_obj_to_cam_proj, v_obj_to_cam );

		if ( t_angle_cosine < T(0.9999) and t_angle_cosine > T(-0.9999) )
		{
			if ( v_obj_to_cam.y < T(0) )
				s_mv *= Rotation( acos(t_angle_cosine), Vec3_t( 1, 0, 0 ) );
			else
				s_mv *= Rotation( acos(t_angle_cosine), Vec3_t( -1, 0, 0 ) );
		}

		return s_mv;
	}
	
/*
=================================================
	BuildPerspective
=================================================
*/
	template <typename T, ulong U>
	ND_ inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::BuildPerspective (const Radians<T>& fovY, const T& screenAspect, const Vec2_t &clipPlanes)
	{
		const T	f = T(1) / Tan( fovY * T(0.5) ),
				A = ( clipPlanes.y + clipPlanes.x ) / ( clipPlanes.x - clipPlanes.y ),
				B = ( T(2) * clipPlanes.x * clipPlanes.y ) / ( clipPlanes.x - clipPlanes.y );

		return Self(	f / screenAspect,	0,	0,	0,
						0,					f,	0,	0,
						0,					0,	A,	B,
						0,					0,	-1,	0 );
	}
	
/*
=================================================
	BuildRealPerspective
----
	screenSize	- size of window in meters
	dist		- distance from eyes to screen in meters
=================================================
*/
	template <typename T, ulong U>
	ND_ inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::BuildRealPerspective (const Vec2_t &screenSize, const Vec2_t &clipPlanes, const T& dist)
	{

		const T	asp	= screenSize.x / screenSize.y,
				f	= (2 * dist) / screenSize.y,
				A	= ( clipPlanes.y + clipPlanes.x ) / ( clipPlanes.x - clipPlanes.y ),
				B	= ( T(2) * clipPlanes.x * clipPlanes.y ) / ( clipPlanes.x - clipPlanes.y );
		
		return Self(	f / asp,	0,	0,	0,
						0,			f,	0,	0,
						0,			0,	A,	B,
						0,			0,	-1,	0 );
	}
	
/*
=================================================
	BuildOrtho
=================================================
*/
	template <typename T, ulong U>
	ND_ inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::BuildOrtho (const Rectangle<T> &rect, const Vec2_t &clipPlanes)
	{
		const T	tx = - ( rect.right + rect.left ) / ( rect.right - rect.left ),
				ty = - ( rect.top + rect.bottom ) / ( rect.top - rect.bottom ),
				tz = - ( clipPlanes.y + clipPlanes.x ) / ( clipPlanes.y - clipPlanes.x ),
				sx = T(2) / ( rect.right - rect.left ),
				sy = T(2) / ( rect.top - rect.bottom ),
				sz = T(2) / ( clipPlanes.x - clipPlanes.y );

		return Self(	sx,	0,	0,	tx,
						0,	sy,	0,	ty,
						0,	0,	sz,	tz,
						0,	0,	0,	1 );
	}
	
/*
=================================================
	BuildOrtho2D
=================================================
*/
	template <typename T, ulong U>
	ND_ inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::BuildOrtho2D (const Rectangle<T> &rect)
	{
		return BuildOrtho( rect, Vec2_t( -1, 1 ) );
	}
	
/*
=================================================
	BuildFrustum
=================================================
*/
	template <typename T, ulong U>
	ND_ inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::BuildFrustum (const Rectangle<T> &rect, const Vec2_t &clipPlanes)
	{
		const T	sx = ( T(2) * clipPlanes.x ) / ( rect.left - rect.right ),
				sy = ( T(2) * clipPlanes.x ) / ( rect.top - rect.bottom ),
				sz = ( clipPlanes.y + clipPlanes.x ) / ( clipPlanes.x - clipPlanes.y ),
				rx = ( rect.left + rect.right ) / ( rect.left - rect.right ),
				ry = ( rect.top + rect.bottom ) / ( rect.top - rect.bottom ),
				tz = ( T(2) * clipPlanes.x * clipPlanes.y ) / ( clipPlanes.x - clipPlanes.y );

		return Self(	sx,	0,	rx,	0,
						0,	sy,	ry,	0,
						0,	0,	sz,	tz,
						0,	0,	-1,	0 );
	}
	
/*
=================================================
	BuildLookAt
=================================================
*/
	template <typename T, ulong U>
	ND_ inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::BuildLookAt (const Vec3_t &pos, const Vec3_t &center, const Vec3_t &upVec)
	{
		const Vec3_t	f = ( pos - center ).Normalized(),
						s = Cross( upVec, f ).Normalized(),
						u = Cross( f, s ).Normalized();

		return Self(	s.x,	s.y,	s.z,	-Dot( s, pos ),
						u.x,	u.y,	u.z,	-Dot( u, pos ),
						f.x,	f.y,	f.z,	-Dot( f, pos ),
						0,		0,		0,		1 );
	}
	
/*
=================================================
	GetTextureMatrix
=================================================
*
	template <typename T, ulong U>
	ND_ inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::GetTextureMatrix () const
	{
		return Self(	(*this)(0,0),	(*this)(1,0),	0,	(*this)(3,0),
						(*this)(0,1),	(*this)(1,1),	0,	(*this)(3,1),
						0,				0,				1,	(*this)(3,2),
						(*this)(0,3),	(*this)(1,3),	0,	(*this)(3,3) );
	}
	
/*
=================================================
	Project
=================================================
*/
	template <typename T, ulong U>
	ND_ inline Vec<T,3,U>  Matrix<T,C,R,U>::Project (const Vec3_t &pos, const Self &mvp, const Rectangle<T> &viewport)
	{
		Vec4_t			temp	= mvp * Vec4_t( pos, T(1) );
		Vec2_t const	size	= viewport.Size();

		temp  /= temp.w;
		temp   = temp * T(0.5) + T(0.5);
		temp.x = temp.x * size.x + viewport.left;
		temp.y = temp.y * size.y + viewport.bottom;

		return temp.xyz();
	}
	
/*
=================================================
	UnProject
=================================================
*/
	template <typename T, ulong U>
	ND_ inline Vec<T,3,U>  Matrix<T,C,R,U>::UnProject (const Vec3_t &pos, const Self &mvpInverse, const Rectangle<T> &viewport)
	{
		Vec4_t			temp	= Vec4_t( pos, T(1) );
		Vec2_t const	size	= viewport.Size();

		temp.x	= (temp.x - viewport.left) / size.x;
		temp.y	= (temp.y - viewport.bottom) / size.y;
		temp	= temp * T(2) - T(1);

		temp	 = mvpInverse * temp;
		temp	/= temp.w;

		return temp.xyz();
	}


#undef	R
#undef	C
#undef	MATRIX_METHODS


}	// GXMath
}	// GX_STL
