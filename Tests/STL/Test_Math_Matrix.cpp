// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


template <typename T, usize C, usize R, usize Q>
static Matrix<T,Q,R> Mul (const Matrix<T,C,R> &left, const Matrix<T,Q,C> &right)
{
	Matrix<T,Q,R>	result;

	for (uint r = 0; r < R; ++r)
	{
		for (uint q = 0; q < Q; ++q)
		{
			result(q,r) = 0.0f;

			for (uint c = 0; c < C; ++c)
			{
				result(q,r) += left(c,r) * right(q,c);
			}
		}
	}

	return result;
}


extern void Test_Math_Matrix ()
{
	float4x4	mat = float4x4::Identity();
	float4		v( 1.0f, 2.0f, 3.0f, 4.0f );

	float4	a = mat * v;
	float4	b = v * mat;

	float3	c = (v * mat).xyz();


	float4x4	mat1( 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f );
	float4x4	mat2( 5.0f, 4.0f, 3.0f, 1.0f, 16.0f, 15.0f, 14.0f, 13.0f, 9.0f, 8.0f, 7.0f, 6.0f, 12.0f, 11.0f, 10.0f, 9.0f );
	float4x4	mat3 = mat1 * mat2;
	float4x4	mat4 = Mul( mat1, mat2 );

	ASSERT( mat3 == mat4 );
}
