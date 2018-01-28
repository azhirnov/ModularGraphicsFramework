// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Description:	3x2, 3x3, 3x4 matrices
*/

#pragma once

#include "MatrixUtils.h"

namespace GX_STL
{
namespace GXMath
{
	

	//
	// Matrix 3x2
	//
	
#define C	3
#define R	2
#define MATRIX_METHODS() \
		Matrix (T _00, T _10, T _20, \
				T _01, T _11, T _21); \
		Matrix (const Col_t &c0, \
				const Col_t &c1, \
				const Col_t &c2); \
		\
		static Self	BuildTextureMatrix (const T& a, const Vec<T,2,U> &center, \
										   const Vec<T,2,U> &pos, const Vec<T,2,U> &scale); \


#include "MatrixCR.h"
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>::Matrix (T _00, T _10, T _20,
								   T _01, T _11, T _21)
	{
		_v[0][0] = _00;		_v[1][0] = _10;		_v[2][0] = _20;
		_v[0][1] = _01;		_v[1][1] = _11;		_v[2][1] = _21;
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>::Matrix (const Col_t &c0,
								   const Col_t &c1,
								   const Col_t &c2)
	{
		UnsafeMem::MemCopy( _v[0], c0.ptr(), sizeof(T)*R );
		UnsafeMem::MemCopy( _v[1], c1.ptr(), sizeof(T)*R );
		UnsafeMem::MemCopy( _v[2], c2.ptr(), sizeof(T)*R );
	}
	
/*
=================================================
	BuildTextureMatrix
=================================================
*/
	template <typename T, ulong U>
	inline CHECKRES(Matrix<T,C,R,U>)  Matrix<T,C,R,U>::BuildTextureMatrix (const T& a, const Vec2_t &center,
																			const Vec2_t &pos, const Vec2_t &scale)
	{
		Self			m;
		const Vec2_t	sc = SinCos( a );

		m(0,0) = sc[1] * scale.x;
		m(0,1) = sc[0] * scale.y;

		m(1,0) = -sc[0] * scale.x;
		m(1,1) =  sc[1] * scale.y;

		m(2,0) = sc[1] * scale.x * center.x + -sc[0] * center.y + pos.x;
		m(2,1) = sc[0] * scale.y * center.x +  sc[1] * center.y + pos.y;

		return m;
	}

#undef	R
#undef	C
#undef	MATRIX_METHODS

	
	

	//
	// Matrix 3x3
	//
	
#define C	3
#define R	3
#define MATRIX_METHODS() \
		Matrix (T _00, T _10, T _20, \
				T _01, T _11, T _21, \
				T _02, T _12, T _22); \
		Matrix (const Col_t &c0, \
				const Col_t &c1, \
				const Col_t &c2); \
		\
		Self		Inverse() const; \
		\
		static Self	BuildCubeFace(ubyte uFace); \


#include "MatrixCR.h"
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>::Matrix (T _00, T _10, T _20,
								   T _01, T _11, T _21,
								   T _02, T _12, T _22)
	{
		_v[0][0] = _00;		_v[1][0] = _10;		_v[2][0] = _20;
		_v[0][1] = _01;		_v[1][1] = _11;		_v[2][1] = _21;
		_v[0][2] = _02;		_v[1][2] = _12;		_v[2][2] = _22;
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>::Matrix (const Col_t &c0,
								   const Col_t &c1,
								   const Col_t &c2)
	{
		UnsafeMem::MemCopy( _v[0], c0.ptr(), sizeof(T)*R );
		UnsafeMem::MemCopy( _v[1], c1.ptr(), sizeof(T)*R );
		UnsafeMem::MemCopy( _v[2], c2.ptr(), sizeof(T)*R );
	}
	
/*
=================================================
	Inverse
=================================================
*/
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::Inverse () const
	{
		const Self	inv( _v[1][1] * _v[2][2] - _v[2][1] * _v[1][2],
						 _v[1][2] * _v[2][0] - _v[2][2] * _v[1][0],
						 _v[1][0] * _v[2][1] - _v[2][0] * _v[1][1],
						 _v[0][2] * _v[2][1] - _v[0][1] * _v[2][2],
						 _v[0][0] * _v[2][2] - _v[0][2] * _v[2][0],
						 _v[0][1] * _v[2][0] - _v[0][0] * _v[2][1],
						 _v[1][2] * _v[0][1] - _v[1][1] * _v[0][2],
						 _v[1][0] * _v[0][2] - _v[1][2] * _v[0][0],
						 _v[1][1] * _v[0][0] - _v[1][0] * _v[0][1]);

		const T det =	_v[0][0] * ( _v[1][1] * _v[2][2] - _v[2][1] * _v[1][2] ) -
						_v[1][0] * ( _v[0][1] * _v[2][2] - _v[2][1] * _v[0][2] ) +
						_v[2][0] * ( _v[0][1] * _v[1][2] - _v[1][1] * _v[0][2] );

		return ( inv / det );
	}
	
/*
=================================================
	BuildCubeFace
=================================================
*/
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::BuildCubeFace (ubyte uFace)
	{
		// real3( -0.5..0.5, -0.5...0.5, 0.5 ) * BuildCubeFace( ... )

		ASSUME( uFace < 6 );

		const int	idx		 = uFace < 6 ? uFace : 5;		// pos: 0, 2, 4; neg: 1, 3, 5
		const int	norm	 = idx >> 1;					// norm: 0, 1, 2
		const bool	negative = idx & 1;
		
		Self	m = Zero();

		m[ norm==0 ] = T(1);
		m[ 6-norm  ] = T(1);
		m[ norm+6  ] = T(1);

		negative ? m[ norm+6 ] = -m[ norm+6 ] : 0;
		
		return m.Transpose();
	}

#undef	R
#undef	C
#undef	MATRIX_METHODS

	
	

	//
	// Matrix 3x4
	//
	
#define C	3
#define R	4
#define MATRIX_METHODS() \
		Matrix (T _00, T _10, T _20, \
				T _01, T _11, T _21, \
				T _02, T _12, T _22, \
				T _03, T _13, T _23); \
		Matrix (const Col_t &c0, \
				const Col_t &c1, \
				const Col_t &c2); \


#include "MatrixCR.h"
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>::Matrix (T _00, T _10, T _20,
								   T _01, T _11, T _21,
								   T _02, T _12, T _22,
								   T _03, T _13, T _23)
	{
		_v[0][0] = _00;		_v[1][0] = _10;		_v[2][0] = _20;
		_v[0][1] = _01;		_v[1][1] = _11;		_v[2][1] = _21;
		_v[0][2] = _02;		_v[1][2] = _12;		_v[2][2] = _22;
		_v[0][3] = _03;		_v[1][3] = _13;		_v[2][3] = _23;
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>::Matrix (const Col_t &c0,
								   const Col_t &c1,
								   const Col_t &c2)
	{
		UnsafeMem::MemCopy( _v[0], c0.ptr(), sizeof(T)*R );
		UnsafeMem::MemCopy( _v[1], c1.ptr(), sizeof(T)*R );
		UnsafeMem::MemCopy( _v[2], c2.ptr(), sizeof(T)*R );
	}

#undef	R
#undef	C
#undef	MATRIX_METHODS


}	// GXMath
}	// GX_STL
