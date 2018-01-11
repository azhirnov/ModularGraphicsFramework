// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Description:	2x2, 2x3, 2x4 matrices
*/

#pragma once

#include "MatrixUtils.h"

namespace GX_STL
{
namespace GXMath
{
	

	//
	// Matrix 2x2
	//
	
#define C	2
#define R	2
#define MATRIX_METHODS() \
		Matrix (T _00, T _10, \
				T _01, T _11); \
		Matrix (const Col_t &c0, \
				const Col_t &c1); \
		\
		Self		Inverse () const; \


#include "MatrixCR.h"

/*
=================================================
	constructor
=================================================
*/
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>::Matrix (T _00, T _10,
								   T _01, T _11)
	{
		_v[0][0] = _00;		_v[1][0] = _10;
		_v[0][1] = _01;		_v[1][1] = _11;
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>::Matrix (const Col_t &c0, const Col_t &c1)
	{
		UnsafeMem::MemCopy( _v[0], c0.ptr(), sizeof(T)*R );
		UnsafeMem::MemCopy( _v[1], c1.ptr(), sizeof(T)*R );
	}
	
/*
=================================================
	Inverse
=================================================
*/
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::Inverse () const
	{
		const T	det = _v[0][0] * _v[1][1] - _v[1][0] * _v[0][1];

		return Self(  _v[1][1] / det,	- _v[1][0] / det,
					- _v[0][1] / det,	  _v[0][0] / det );
	}

#undef	R
#undef	C
#undef	MATRIX_METHODS


	
	

	//
	// Matrix 2x3
	//
	
#define C	2
#define R	3
#define MATRIX_METHODS() \
		Matrix (T _00, T _10, \
				T _01, T _11, \
				T _02, T _12); \
		Matrix (const Col_t &c0, \
				const Col_t &c1); \


#include "MatrixCR.h"
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>::Matrix (T _00, T _10,
								   T _01, T _11,
								   T _02, T _12)
	{
		_v[0][0] = _00;		_v[1][0] = _10;
		_v[0][1] = _01;		_v[1][1] = _11;
		_v[0][2] = _02;		_v[1][2] = _12;
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>::Matrix (const Col_t &c0, const Col_t &c1)
	{
		UnsafeMem::MemCopy( _v[0], c0.ptr(), sizeof(T)*R );
		UnsafeMem::MemCopy( _v[1], c1.ptr(), sizeof(T)*R );
	}

#undef	R
#undef	C
#undef	MATRIX_METHODS


	

	//
	// Matrix 2x4
	//
	
#define C	2
#define R	4
#define MATRIX_METHODS() \
		Matrix (T _00, T _10, \
				T _01, T _11, \
				T _02, T _12, \
				T _03, T _13); \
		Matrix (const Col_t &c0, const Col_t &c1); \


#include "MatrixCR.h"
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>::Matrix (T _00, T _10,
								   T _01, T _11,
								   T _02, T _12,
								   T _03, T _13)
	{
		_v[0][0] = _00;		_v[1][0] = _10;
		_v[0][1] = _01;		_v[1][1] = _11;
		_v[0][2] = _02;		_v[1][2] = _12;
		_v[0][3] = _03;		_v[1][3] = _13;
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>::Matrix (const Col_t &c0, const Col_t &c1)
	{
		UnsafeMem::MemCopy( _v[0], c0.ptr(), sizeof(T)*R );
		UnsafeMem::MemCopy( _v[1], c1.ptr(), sizeof(T)*R );
	}

#undef	R
#undef	C
#undef	MATRIX_METHODS


}	// GXMath
}	// GX_STL
