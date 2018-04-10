// This is generated file, don't change anything!

#include "cl_source_vfs.h"

namespace cl_vfs
{
	extern void VFS_matrix (OUT String &src)
	{
		src << R"#(/*
	Matrix functions.

	This is generated file, dont change anything!

	Templates:
	Matrix type                  realCxR where C - number of columns, R - number of rows.
	Column access                realR realCxR.colX where X - index of column (0..C-1)
	Row access                   realC MatCxR_RowX (realCxR x) where X - index of row (0..R-1)
	Column vector constructor    realCxR MatCxR_Ctor (realR, ...[C])
	Row vector constructor       realCxR MatCxR_RowCtor (realC, ...[R])
	Scalar constructor           realCxR MatCxR_ScalarCtor (real ... [C*R])
	Vector constructor           realCxR MatCxR_VecCtor (real4/real8/real16 x)
	Identity matrix              realCxR MatCxR_Identity ()
	Transpose matrix             realRxC MatCxR_Transpose (realCxR x)
	Cast to other matrix         realQxW MatCxR_ToQxW (realCxR x) where Q and W is columns and rows number of other matrix
	Multiplicate matrices        realQxR MatCxR_MulQxC (realCxR x, realQxC y)
	Transform vector             realC MatCxR_Mul (realCxR, realR)
	Transform vector             realR Mul_MatCxR (realC, realCxR)
*/

#include "math.cl"

// Matrix types
typedef struct _Mat2x2
{
	real2	col0;
	real2	col1;

} real2x2;

typedef struct _Mat2x3
{
	real3	col0;
	real3	col1;

} real2x3;

typedef struct _Mat2x4
{
	real4	col0;
	real4	col1;

} real2x4;

typedef struct _Mat3x2
{
	real2	col0;
	real2	col1;
	real2	col2;

} real3x2;

typedef struct _Mat3x3
{
	real3	col0;
	real3	col1;
	real3	col2;

} real3x3;

typedef struct _Mat3x4
{
	real4	col0;
	real4	col1;
	real4	col2;

} real3x4;

typedef struct _Mat4x2
{
	real2	col0;
	real2	col1;
	real2	col2;
	real2	col3;

} real4x2;

typedef struct _Mat4x3
{
	real3	col0;
	real3	col1;
	real3	col2;
	real3	col3;

} real4x3;

typedef struct _Mat4x4
{
	real4	col0;
	real4	col1;
	real4	col2;
	real4	col3;

} real4x4;


// Matrix row access
force_inline real2 Mat2x2_Row0 (const real2x2 x)
{
	return (real2)(x.col0.s0, x.col1.s0);
}

force_inline real2 Mat2x2_Row1 (const real2x2 x)
{
	return (real2)(x.col0.s1, x.col1.s1);
}

force_inline real2 Mat2x3_Row0 (const real2x3 x)
{
	return (real2)(x.col0.s0, x.col1.s0);
}

force_inline real2 Mat2x3_Row1 (const real2x3 x)
{
	return (real2)(x.col0.s1, x.col1.s1);
}

force_inline real2 Mat2x3_Row2 (const real2x3 x)
{
	return (real2)(x.col0.s2, x.col1.s2);
}

force_inline real2 Mat2x4_Row0 (const real2x4 x)
{
	return (real2)(x.col0.s0, x.col1.s0);
}

force_inline real2 Mat2x4_Row1 (const real2x4 x)
{
	return (real2)(x.col0.s1, x.col1.s1);
}

force_inline real2 Mat2x4_Row2 (const real2x4 x)
{
	return (real2)(x.col0.s2, x.col1.s2);
}

force_inline real2 Mat2x4_Row3 (const real2x4 x)
{
	return (real2)(x.col0.s3, x.col1.s3);
}

force_inline real3 Mat3x2_Row0 (const real3x2 x)
{
	return (real3)(x.col0.s0, x.col1.s0, x.col2.s0);
}

force_inline real3 Mat3x2_Row1 (const real3x2 x)
{
	return (real3)(x.col0.s1, x.col1.s1, x.col2.s1);
}

force_inline real3 Mat3x3_Row0 (const real3x3 x)
{
	return (real3)(x.col0.s0, x.col1.s0, x.col2.s0);
}

force_inline real3 Mat3x3_Row1 (const real3x3 x)
{
	return (real3)(x.col0.s1, x.col1.s1, x.col2.s1);
}

force_inline real3 Mat3x3_Row2 (const real3x3 x)
{
	return (real3)(x.col0.s2, x.col1.s2, x.col2.s2);
}

force_inline real3 Mat3x4_Row0 (const real3x4 x)
{
	return (real3)(x.col0.s0, x.col1.s0, x.col2.s0);
}

force_inline real3 Mat3x4_Row1 (const real3x4 x)
{
	return (real3)(x.col0.s1, x.col1.s1, x.col2.s1);
}

force_inline real3 Mat3x4_Row2 (const real3x4 x)
{
	return (real3)(x.col0.s2, x.col1.s2, x.col2.s2);
}

force_inline real3 Mat3x4_Row3 (const real3x4 x)
{
	return (real3)(x.col0.s3, x.col1.s3, x.col2.s3);
}

force_inline real4 Mat4x2_Row0 (const real4x2 x)
{
	return (real4)(x.col0.s0, x.col1.s0, x.col2.s0, x.col3.s0);
}

force_inline real4 Mat4x2_Row1 (const real4x2 x)
{
	return (real4)(x.col0.s1, x.col1.s1, x.col2.s1, x.col3.s1);
}

force_inline real4 Mat4x3_Row0 (const real4x3 x)
{
	return (real4)(x.col0.s0, x.col1.s0, x.col2.s0, x.col3.s0);
}

force_inline real4 Mat4x3_Row1 (const real4x3 x)
{
	return (real4)(x.col0.s1, x.col1.s1, x.col2.s1, x.col3.s1);
}

force_inline real4 Mat4x3_Row2 (const real4x3 x)
{
	return (real4)(x.col0.s2, x.col1.s2, x.col2.s2, x.col3.s2);
}

force_inline real4 Mat4x4_Row0 (const real4x4 x)
{
	return (real4)(x.col0.s0, x.col1.s0, x.col2.s0, x.col3.s0);
}

force_inline real4 Mat4x4_Row1 (const real4x4 x)
{
	return (real4)(x.col0.s1, x.col1.s1, x.col2.s1, x.col3.s1);
}

force_inline real4 Mat4x4_Row2 (const real4x4 x)
{
	return (real4)(x.col0.s2, x.col1.s2, x.col2.s2, x.col3.s2);
}

force_inline real4 Mat4x4_Row3 (const real4x4 x)
{
	return (real4)(x.col0.s3, x.col1.s3, x.col2.s3, x.col3.s3);
}


// Matrix column vector constructor
force_inline real2x2 Mat2x2_Ctor (const real2 col0, const real2 col1)
{
	real2x2 res;
	res.col0 = col0;
	res.col1 = col1;
	return res;
}

force_inline real2x3 Mat2x3_Ctor (const real3 col0, const real3 col1)
{
	real2x3 res;
	res.col0 = col0;
	res.col1 = col1;
	return res;
}

force_inline real2x4 Mat2x4_Ctor (const real4 col0, const real4 col1)
{
	real2x4 res;
	res.col0 = col0;
	res.col1 = col1;
	return res;
}

force_inline real3x2 Mat3x2_Ctor (const real2 col0, const real2 col1, const real2 col2)
{
	real3x2 res;
	res.col0 = col0;
	res.col1 = col1;
	res.col2 = col2;
	return res;
}

force_inline real3x3 Mat3x3_Ctor (const real3 col0, const real3 col1, const real3 col2)
{
	real3x3 res;
	res.col0 = col0;
	res.col1 = col1;
	res.col2 = col2;
	return res;
}

force_inline real3x4 Mat3x4_Ctor (const real4 col0, const real4 col1, const real4 col2)
{
	real3x4 res;
	res.col0 = col0;
	res.col1 = col1;
	res.col2 = col2;
	return res;
}

force_inline real4x2 Mat4x2_Ctor (const real2 col0, const real2 col1, const real2 col2, const real2 col3)
{
	real4x2 res;
	res.col0 = col0;
	res.col1 = col1;
	res.col2 = col2;
	res.col3 = col3;
	return res;
}

force_inline real4x3 Mat4x3_Ctor (const real3 col0, const real3 col1, const real3 col2, const real3 col3)
{
	real4x3 res;
	res.col0 = col0;
	res.col1 = col1;
	res.col2 = col2;
	res.col3 = col3;
	return res;
}

force_inline real4x4 Mat4x4_Ctor (const real4 col0, const real4 col1, const real4 col2, const real4 col3)
{
	real4x4 res;
	res.col0 = col0;
	res.col1 = col1;
	res.col2 = col2;
	res.col3 = col3;
	return res;
}


// Matrix row vector constructor
force_inline real2x2 Mat2x2_RowCtor (const real2 row0, const real2 row1)
{
	real2x2 res;
	res.col0 = (real2)(row0.s0, row1.s0);
	res.col1 = (real2)(row0.s1, row1.s1);
	return res;
}

force_inline real2x3 Mat2x3_RowCtor (const real2 row0, const real2 row1, const real2 row2)
{
	real2x3 res;
	res.col0 = (real3)(row0.s0, row1.s0, row2.s0);
	res.col1 = (real3)(row0.s1, row1.s1, row2.s1);
	return res;
}

force_inline real2x4 Mat2x4_RowCtor (const real2 row0, const real2 row1, const real2 row2, const real2 row3)
{
	real2x4 res;
	res.col0 = (real4)(row0.s0, row1.s0, row2.s0, row3.s0);
	res.col1 = (real4)(row0.s1, row1.s1, row2.s1, row3.s1);
	return res;
}

force_inline real3x2 Mat3x2_RowCtor (const real3 row0, const real3 row1)
{
	real3x2 res;
	res.col0 = (real2)(row0.s0, row1.s0);
	res.col1 = (real2)(row0.s1, row1.s1);
	res.col2 = (real2)(row0.s2, row1.s2);
	return res;
}

force_inline real3x3 Mat3x3_RowCtor (const real3 row0, const real3 row1, const real3 row2)
{
	real3x3 res;
	res.col0 = (real3)(row0.s0, row1.s0, row2.s0);
	res.col1 = (real3)(row0.s1, row1.s1, row2.s1);
	res.col2 = (real3)(row0.s2, row1.s2, row2.s2);
	return res;
}

force_inline real3x4 Mat3x4_RowCtor (const real3 row0, const real3 row1, const real3 row2, const real3 row3)
{
	real3x4 res;
	res.col0 = (real4)(row0.s0, row1.s0, row2.s0, row3.s0);
	res.col1 = (real4)(row0.s1, row1.s1, row2.s1, row3.s1);
	res.col2 = (real4)(row0.s2, row1.s2, row2.s2, row3.s2);
	return res;
}

force_inline real4x2 Mat4x2_RowCtor (const real4 row0, const real4 row1)
{
	real4x2 res;
	res.col0 = (real2)(row0.s0, row1.s0);
	res.col1 = (real2)(row0.s1, row1.s1);
	res.col2 = (real2)(row0.s2, row1.s2);
	res.col3 = (real2)(row0.s3, row1.s3);
	return res;
}

force_inline real4x3 Mat4x3_RowCtor (const real4 row0, const real4 row1, const real4 row2)
{
	real4x3 res;
	res.col0 = (real3)(row0.s0, row1.s0, row2.s0);
	res.col1 = (real3)(row0.s1, row1.s1, row2.s1);
	res.col2 = (real3)(row0.s2, row1.s2, row2.s2);
	res.col3 = (real3)(row0.s3, row1.s3, row2.s3);
	return res;
}

force_inline real4x4 Mat4x4_RowCtor (const real4 row0, const real4 row1, const real4 row2, const real4 row3)
{
	real4x4 res;
	res.col0 = (real4)(row0.s0, row1.s0, row2.s0, row3.s0);
	res.col1 = (real4)(row0.s1, row1.s1, row2.s1, row3.s1);
	res.col2 = (real4)(row0.s2, row1.s2, row2.s2, row3.s2);
	res.col3 = (real4)(row0.s3, row1.s3, row2.s3, row3.s3);
	return res;
}


// Matrix scalar constructor
force_inline real2x2 Mat2x2_ScalarCtor (
		real scalar00, real scalar10,
		real scalar01, real scalar11)
{
	real2x2 res;
	res.col0 = (real2)(scalar00, scalar01);
	res.col1 = (real2)(scalar10, scalar11);
	return res;
}

force_inline real2x3 Mat2x3_ScalarCtor (
		real scalar00, real scalar10,
		real scalar01, real scalar11,
		real scalar02, real scalar12)
{
	real2x3 res;
	res.col0 = (real3)(scalar00, scalar01, scalar02);
	res.col1 = (real3)(scalar10, scalar11, scalar12);
	return res;
}

force_inline real2x4 Mat2x4_ScalarCtor (
		real scalar00, real scalar10,
		real scalar01, real scalar11,
		real scalar02, real scalar12,
		real scalar03, real scalar13)
{
	real2x4 res;
	res.col0 = (real4)(scalar00, scalar01, scalar02, scalar03);
	res.col1 = (real4)(scalar10, scalar11, scalar12, scalar13);
	return res;
}

force_inline real3x2 Mat3x2_ScalarCtor (
		real scalar00, real scalar10, real scalar20,
		real scalar01, real scalar11, real scalar21)
{
	real3x2 res;
	res.col0 = (real2)(scalar00, scalar01);
	res.col1 = (real2)(scalar10, scalar11);
	res.col2 = (real2)(scalar20, scalar21);
	return res;
}

force_inline real3x3 Mat3x3_ScalarCtor (
		real scalar00, real scalar10, real scalar20,
		real scalar01, real scalar11, real scalar21,
		real scalar02, real scalar12, real scalar22)
{
	real3x3 res;
	res.col0 = (real3)(scalar00, scalar01, scalar02);
	res.col1 = (real3)(scalar10, scalar11, scalar12);
	res.col2 = (real3)(scalar20, scalar21, scalar22);
	return res;
}

force_inline real3x4 Mat3x4_ScalarCtor (
		real scalar00, real scalar10, real scalar20,
		real scalar01, real scalar11, real scalar21,
		real scalar02, real scalar12, real scalar22,
		real scalar03, real scalar13, real scalar23)
{
	real3x4 res;
	res.col0 = (real4)(scalar00, scalar01, scalar02, scalar03);
	res.col1 = (real4)(scalar10, scalar11, scalar12, scalar13);
	res.col2 = (real4)(scalar20, scalar21, scalar22, scalar23);
	return res;
}

force_inline real4x2 Mat4x2_ScalarCtor (
		real scalar00, real scalar10, real scalar20, real scalar30,
		real scalar01, real scalar11, real scalar21, real scalar31)
{
	real4x2 res;
	res.col0 = (real2)(scalar00, scalar01);
	res.col1 = (real2)(scalar10, scalar11);
	res.col2 = (real2)(scalar20, scalar21);
	res.col3 = (real2)(scalar30, scalar31);
	return res;
}

force_inline real4x3 Mat4x3_ScalarCtor (
		real scalar00, real scalar10, real scalar20, real scalar30,
		real scalar01, real scalar11, real scalar21, real scalar31,
		real scalar02, real scalar12, real scalar22, real scalar32)
{
	real4x3 res;
	res.col0 = (real3)(scalar00, scalar01, scalar02);
	res.col1 = (real3)(scalar10, scalar11, scalar12);
	res.col2 = (real3)(scalar20, scalar21, scalar22);
	res.col3 = (real3)(scalar30, scalar31, scalar32);
	return res;
}

force_inline real4x4 Mat4x4_ScalarCtor (
		real scalar00, real scalar10, real scalar20, real scalar30,
		real scalar01, real scalar11, real scalar21, real scalar31,
		real scalar02, real scalar12, real scalar22, real scalar32,
		real scalar03, real scalar13, real scalar23, real scalar33)
{
	real4x4 res;
	res.col0 = (real4)(scalar00, scalar01, scalar02, scalar03);
	res.col1 = (real4)(scalar10, scalar11, scalar12, scalar13);
	res.col2 = (real4)(scalar20, scalar21, scalar22, scalar23);
	res.col3 = (real4)(scalar30, scalar31, scalar32, scalar33);
	return res;
}


// Matrix vector constructor
force_inline real2x2 Mat2x2_VecCtor (const real4 x)
{
	real2x2 res;
	res.col0 = x.s01;
	res.col1 = x.s23;
	return res;
}

force_inline real2x3 Mat2x3_VecCtor (const real8 x)
{
	real2x3 res;
	res.col0 = x.s012;
	res.col1 = x.s456;
	return res;
}

force_inline real2x4 Mat2x4_VecCtor (const real8 x)
{
	real2x4 res;
	res.col0 = x.s0123;
	res.col1 = x.s4567;
	return res;
}

force_inline real3x2 Mat3x2_VecCtor (const real8 x)
{
	real3x2 res;
	res.col0 = x.s01;
	res.col1 = x.s23;
	res.col2 = x.s45;
	return res;
}

force_inline real3x3 Mat3x3_VecCtor (const real16 x)
{
	real3x3 res;
	res.col0 = x.s012;
	res.col1 = x.s456;
	res.col2 = x.s89A;
	return res;
}

force_inline real3x4 Mat3x4_VecCtor (const real16 x)
{
	real3x4 res;
	res.col0 = x.s0123;
	res.col1 = x.s4567;
	res.col2 = x.s89AB;
	return res;
}

force_inline real4x2 Mat4x2_VecCtor (const real8 x)
{
	real4x2 res;
	res.col0 = x.s01;
	res.col1 = x.s23;
	res.col2 = x.s45;
	res.col3 = x.s67;
	return res;
}

force_inline real4x3 Mat4x3_VecCtor (const real16 x)
{
	real4x3 res;
	res.col0 = x.s012;
	res.col1 = x.s456;
	res.col2 = x.s89A;
	res.col3 = x.sCDE;
	return res;
}

force_inline real4x4 Mat4x4_VecCtor (const real16 x)
{
	real4x4 res;
	res.col0 = x.s0123;
	res.col1 = x.s4567;
	res.col2 = x.s89AB;
	res.col3 = x.sCDEF;
	return res;
}


// Create identity matrix
force_inline real2x2 Mat2x2_Identity ()
{
	real2x2 res;
	res.col0 = (real2)(1.0, 0.0);
	res.col1 = (real2)(0.0, 1.0);
	return res;
}

force_inline real2x3 Mat2x3_Identity ()
{
	real2x3 res;
	res.col0 = (real3)(1.0, 0.0, 0.0);
	res.col1 = (real3)(0.0, 1.0, 0.0);
	return res;
}

force_inline real2x4 Mat2x4_Identity ()
{
	real2x4 res;
	res.col0 = (real4)(1.0, 0.0, 0.0, 0.0);
	res.col1 = (real4)(0.0, 1.0, 0.0, 0.0);
	return res;
}

force_inline real3x2 Mat3x2_Identity ()
{
	real3x2 res;
	res.col0 = (real2)(1.0, 0.0);
	res.col1 = (real2)(0.0, 1.0);
	res.col2 = (real2)(0.0, 0.0);
	return res;
}

force_inline real3x3 Mat3x3_Identity ()
{
	real3x3 res;
	res.col0 = (real3)(1.0, 0.0, 0.0);
	res.col1 = (real3)(0.0, 1.0, 0.0);
	res.col2 = (real3)(0.0, 0.0, 1.0);
	return res;
}

force_inline real3x4 Mat3x4_Identity ()
{
	real3x4 res;
	res.col0 = (real4)(1.0, 0.0, 0.0, 0.0);
	res.col1 = (real4)(0.0, 1.0, 0.0, 0.0);
	res.col2 = (real4)(0.0, 0.0, 1.0, 0.0);
	return res;
}

force_inline real4x2 Mat4x2_Identity ()
{
	real4x2 res;
	res.col0 = (real2)(1.0, 0.0);
	res.col1 = (real2)(0.0, 1.0);
	res.col2 = (real2)(0.0, 0.0);
	res.col3 = (real2)(0.0, 0.0);
	return res;
}

force_inline real4x3 Mat4x3_Identity ()
{
	real4x3 res;
	res.col0 = (real3)(1.0, 0.0, 0.0);
	res.col1 = (real3)(0.0, 1.0, 0.0);
	res.col2 = (real3)(0.0, 0.0, 1.0);
	res.col3 = (real3)(0.0, 0.0, 0.0);
	return res;
}

force_inline real4x4 Mat4x4_Identity ()
{
	real4x4 res;
	res.col0 = (real4)(1.0, 0.0, 0.0, 0.0);
	res.col1 = (real4)(0.0, 1.0, 0.0, 0.0);
	res.col2 = (real4)(0.0, 0.0, 1.0, 0.0);
	res.col3 = (real4)(0.0, 0.0, 0.0, 1.0);
	return res;
}


// Get transpose matrix
force_inline real2x2 Mat2x2_Transpose (const real2x2 x)
{
	real2x2 res;
	res.col0 = (real2)(x.col0.s0, x.col1.s0);
	res.col1 = (real2)(x.col0.s1, x.col1.s1);
	return res;
}

force_inline real3x2 Mat2x3_Transpose (const real2x3 x)
{
	real3x2 res;
	res.col0 = (real2)(x.col0.s0, x.col1.s0);
	res.col1 = (real2)(x.col0.s1, x.col1.s1);
	res.col2 = (real2)(x.col0.s2, x.col1.s2);
	return res;
}

force_inline real4x2 Mat2x4_Transpose (const real2x4 x)
{
	real4x2 res;
	res.col0 = (real2)(x.col0.s0, x.col1.s0);
	res.col1 = (real2)(x.col0.s1, x.col1.s1);
	res.col2 = (real2)(x.col0.s2, x.col1.s2);
	res.col3 = (real2)(x.col0.s3, x.col1.s3);
	return res;
}

force_inline real2x3 Mat3x2_Transpose (const real3x2 x)
{
	real2x3 res;
	res.col0 = (real3)(x.col0.s0, x.col1.s0, x.col2.s0);
	res.col1 = (real3)(x.col0.s1, x.col1.s1, x.col2.s1);
	return res;
}

force_inline real3x3 Mat3x3_Transpose (const real3x3 x)
{
	real3x3 res;
	res.col0 = (real3)(x.col0.s0, x.col1.s0, x.col2.s0);
	res.col1 = )#";

		src << R"#((real3)(x.col0.s1, x.col1.s1, x.col2.s1);
	res.col2 = (real3)(x.col0.s2, x.col1.s2, x.col2.s2);
	return res;
}

force_inline real4x3 Mat3x4_Transpose (const real3x4 x)
{
	real4x3 res;
	res.col0 = (real3)(x.col0.s0, x.col1.s0, x.col2.s0);
	res.col1 = (real3)(x.col0.s1, x.col1.s1, x.col2.s1);
	res.col2 = (real3)(x.col0.s2, x.col1.s2, x.col2.s2);
	res.col3 = (real3)(x.col0.s3, x.col1.s3, x.col2.s3);
	return res;
}

force_inline real2x4 Mat4x2_Transpose (const real4x2 x)
{
	real2x4 res;
	res.col0 = (real4)(x.col0.s0, x.col1.s0, x.col2.s0, x.col3.s0);
	res.col1 = (real4)(x.col0.s1, x.col1.s1, x.col2.s1, x.col3.s1);
	return res;
}

force_inline real3x4 Mat4x3_Transpose (const real4x3 x)
{
	real3x4 res;
	res.col0 = (real4)(x.col0.s0, x.col1.s0, x.col2.s0, x.col3.s0);
	res.col1 = (real4)(x.col0.s1, x.col1.s1, x.col2.s1, x.col3.s1);
	res.col2 = (real4)(x.col0.s2, x.col1.s2, x.col2.s2, x.col3.s2);
	return res;
}

force_inline real4x4 Mat4x4_Transpose (const real4x4 x)
{
	real4x4 res;
	res.col0 = (real4)(x.col0.s0, x.col1.s0, x.col2.s0, x.col3.s0);
	res.col1 = (real4)(x.col0.s1, x.col1.s1, x.col2.s1, x.col3.s1);
	res.col2 = (real4)(x.col0.s2, x.col1.s2, x.col2.s2, x.col3.s2);
	res.col3 = (real4)(x.col0.s3, x.col1.s3, x.col2.s3, x.col3.s3);
	return res;
}


// Cast matrix to other matrix type
force_inline real2x3 Mat2x2_To2x3 (const real2x2 x)
{
	real2x3 res;
	res.col0 = (real3)(x.col0.s0, x.col0.s1, 0.0);
	res.col1 = (real3)(x.col1.s0, x.col1.s1, 0.0);
	return res;
}

force_inline real2x4 Mat2x2_To2x4 (const real2x2 x)
{
	real2x4 res;
	res.col0 = (real4)(x.col0.s0, x.col0.s1, 0.0, 0.0);
	res.col1 = (real4)(x.col1.s0, x.col1.s1, 0.0, 0.0);
	return res;
}

force_inline real3x2 Mat2x2_To3x2 (const real2x2 x)
{
	real3x2 res;
	res.col0 = (real2)(x.col0.s0, x.col0.s1);
	res.col1 = (real2)(x.col1.s0, x.col1.s1);
	res.col2 = (real2)(0.0, 0.0);
	return res;
}

force_inline real3x3 Mat2x2_To3x3 (const real2x2 x)
{
	real3x3 res;
	res.col0 = (real3)(x.col0.s0, x.col0.s1, 0.0);
	res.col1 = (real3)(x.col1.s0, x.col1.s1, 0.0);
	res.col2 = (real3)(0.0, 0.0, 1.0);
	return res;
}

force_inline real3x4 Mat2x2_To3x4 (const real2x2 x)
{
	real3x4 res;
	res.col0 = (real4)(x.col0.s0, x.col0.s1, 0.0, 0.0);
	res.col1 = (real4)(x.col1.s0, x.col1.s1, 0.0, 0.0);
	res.col2 = (real4)(0.0, 0.0, 1.0, 0.0);
	return res;
}

force_inline real4x2 Mat2x2_To4x2 (const real2x2 x)
{
	real4x2 res;
	res.col0 = (real2)(x.col0.s0, x.col0.s1);
	res.col1 = (real2)(x.col1.s0, x.col1.s1);
	res.col2 = (real2)(0.0, 0.0);
	res.col3 = (real2)(0.0, 0.0);
	return res;
}

force_inline real4x3 Mat2x2_To4x3 (const real2x2 x)
{
	real4x3 res;
	res.col0 = (real3)(x.col0.s0, x.col0.s1, 0.0);
	res.col1 = (real3)(x.col1.s0, x.col1.s1, 0.0);
	res.col2 = (real3)(0.0, 0.0, 1.0);
	res.col3 = (real3)(0.0, 0.0, 0.0);
	return res;
}

force_inline real4x4 Mat2x2_To4x4 (const real2x2 x)
{
	real4x4 res;
	res.col0 = (real4)(x.col0.s0, x.col0.s1, 0.0, 0.0);
	res.col1 = (real4)(x.col1.s0, x.col1.s1, 0.0, 0.0);
	res.col2 = (real4)(0.0, 0.0, 1.0, 0.0);
	res.col3 = (real4)(0.0, 0.0, 0.0, 1.0);
	return res;
}

force_inline real2x2 Mat2x3_To2x2 (const real2x3 x)
{
	real2x2 res;
	res.col0 = (real2)(x.col0.s0, x.col0.s1);
	res.col1 = (real2)(x.col1.s0, x.col1.s1);
	return res;
}

force_inline real2x4 Mat2x3_To2x4 (const real2x3 x)
{
	real2x4 res;
	res.col0 = (real4)(x.col0.s0, x.col0.s1, x.col0.s2, 0.0);
	res.col1 = (real4)(x.col1.s0, x.col1.s1, x.col1.s2, 0.0);
	return res;
}

force_inline real3x2 Mat2x3_To3x2 (const real2x3 x)
{
	real3x2 res;
	res.col0 = (real2)(x.col0.s0, x.col0.s1);
	res.col1 = (real2)(x.col1.s0, x.col1.s1);
	res.col2 = (real2)(0.0, 0.0);
	return res;
}

force_inline real3x3 Mat2x3_To3x3 (const real2x3 x)
{
	real3x3 res;
	res.col0 = (real3)(x.col0.s0, x.col0.s1, x.col0.s2);
	res.col1 = (real3)(x.col1.s0, x.col1.s1, x.col1.s2);
	res.col2 = (real3)(0.0, 0.0, 1.0);
	return res;
}

force_inline real3x4 Mat2x3_To3x4 (const real2x3 x)
{
	real3x4 res;
	res.col0 = (real4)(x.col0.s0, x.col0.s1, x.col0.s2, 0.0);
	res.col1 = (real4)(x.col1.s0, x.col1.s1, x.col1.s2, 0.0);
	res.col2 = (real4)(0.0, 0.0, 1.0, 0.0);
	return res;
}

force_inline real4x2 Mat2x3_To4x2 (const real2x3 x)
{
	real4x2 res;
	res.col0 = (real2)(x.col0.s0, x.col0.s1);
	res.col1 = (real2)(x.col1.s0, x.col1.s1);
	res.col2 = (real2)(0.0, 0.0);
	res.col3 = (real2)(0.0, 0.0);
	return res;
}

force_inline real4x3 Mat2x3_To4x3 (const real2x3 x)
{
	real4x3 res;
	res.col0 = (real3)(x.col0.s0, x.col0.s1, x.col0.s2);
	res.col1 = (real3)(x.col1.s0, x.col1.s1, x.col1.s2);
	res.col2 = (real3)(0.0, 0.0, 1.0);
	res.col3 = (real3)(0.0, 0.0, 0.0);
	return res;
}

force_inline real4x4 Mat2x3_To4x4 (const real2x3 x)
{
	real4x4 res;
	res.col0 = (real4)(x.col0.s0, x.col0.s1, x.col0.s2, 0.0);
	res.col1 = (real4)(x.col1.s0, x.col1.s1, x.col1.s2, 0.0);
	res.col2 = (real4)(0.0, 0.0, 1.0, 0.0);
	res.col3 = (real4)(0.0, 0.0, 0.0, 1.0);
	return res;
}

force_inline real2x2 Mat2x4_To2x2 (const real2x4 x)
{
	real2x2 res;
	res.col0 = (real2)(x.col0.s0, x.col0.s1);
	res.col1 = (real2)(x.col1.s0, x.col1.s1);
	return res;
}

force_inline real2x3 Mat2x4_To2x3 (const real2x4 x)
{
	real2x3 res;
	res.col0 = (real3)(x.col0.s0, x.col0.s1, x.col0.s2);
	res.col1 = (real3)(x.col1.s0, x.col1.s1, x.col1.s2);
	return res;
}

force_inline real3x2 Mat2x4_To3x2 (const real2x4 x)
{
	real3x2 res;
	res.col0 = (real2)(x.col0.s0, x.col0.s1);
	res.col1 = (real2)(x.col1.s0, x.col1.s1);
	res.col2 = (real2)(0.0, 0.0);
	return res;
}

force_inline real3x3 Mat2x4_To3x3 (const real2x4 x)
{
	real3x3 res;
	res.col0 = (real3)(x.col0.s0, x.col0.s1, x.col0.s2);
	res.col1 = (real3)(x.col1.s0, x.col1.s1, x.col1.s2);
	res.col2 = (real3)(0.0, 0.0, 1.0);
	return res;
}

force_inline real3x4 Mat2x4_To3x4 (const real2x4 x)
{
	real3x4 res;
	res.col0 = (real4)(x.col0.s0, x.col0.s1, x.col0.s2, x.col0.s3);
	res.col1 = (real4)(x.col1.s0, x.col1.s1, x.col1.s2, x.col1.s3);
	res.col2 = (real4)(0.0, 0.0, 1.0, 0.0);
	return res;
}

force_inline real4x2 Mat2x4_To4x2 (const real2x4 x)
{
	real4x2 res;
	res.col0 = (real2)(x.col0.s0, x.col0.s1);
	res.col1 = (real2)(x.col1.s0, x.col1.s1);
	res.col2 = (real2)(0.0, 0.0);
	res.col3 = (real2)(0.0, 0.0);
	return res;
}

force_inline real4x3 Mat2x4_To4x3 (const real2x4 x)
{
	real4x3 res;
	res.col0 = (real3)(x.col0.s0, x.col0.s1, x.col0.s2);
	res.col1 = (real3)(x.col1.s0, x.col1.s1, x.col1.s2);
	res.col2 = (real3)(0.0, 0.0, 1.0);
	res.col3 = (real3)(0.0, 0.0, 0.0);
	return res;
}

force_inline real4x4 Mat2x4_To4x4 (const real2x4 x)
{
	real4x4 res;
	res.col0 = (real4)(x.col0.s0, x.col0.s1, x.col0.s2, x.col0.s3);
	res.col1 = (real4)(x.col1.s0, x.col1.s1, x.col1.s2, x.col1.s3);
	res.col2 = (real4)(0.0, 0.0, 1.0, 0.0);
	res.col3 = (real4)(0.0, 0.0, 0.0, 1.0);
	return res;
}

force_inline real2x2 Mat3x2_To2x2 (const real3x2 x)
{
	real2x2 res;
	res.col0 = (real2)(x.col0.s0, x.col0.s1);
	res.col1 = (real2)(x.col1.s0, x.col1.s1);
	return res;
}

force_inline real2x3 Mat3x2_To2x3 (const real3x2 x)
{
	real2x3 res;
	res.col0 = (real3)(x.col0.s0, x.col0.s1, 0.0);
	res.col1 = (real3)(x.col1.s0, x.col1.s1, 0.0);
	return res;
}

force_inline real2x4 Mat3x2_To2x4 (const real3x2 x)
{
	real2x4 res;
	res.col0 = (real4)(x.col0.s0, x.col0.s1, 0.0, 0.0);
	res.col1 = (real4)(x.col1.s0, x.col1.s1, 0.0, 0.0);
	return res;
}

force_inline real3x3 Mat3x2_To3x3 (const real3x2 x)
{
	real3x3 res;
	res.col0 = (real3)(x.col0.s0, x.col0.s1, 0.0);
	res.col1 = (real3)(x.col1.s0, x.col1.s1, 0.0);
	res.col2 = (real3)(x.col2.s0, x.col2.s1, 1.0);
	return res;
}

force_inline real3x4 Mat3x2_To3x4 (const real3x2 x)
{
	real3x4 res;
	res.col0 = (real4)(x.col0.s0, x.col0.s1, 0.0, 0.0);
	res.col1 = (real4)(x.col1.s0, x.col1.s1, 0.0, 0.0);
	res.col2 = (real4)(x.col2.s0, x.col2.s1, 1.0, 0.0);
	return res;
}

force_inline real4x2 Mat3x2_To4x2 (const real3x2 x)
{
	real4x2 res;
	res.col0 = (real2)(x.col0.s0, x.col0.s1);
	res.col1 = (real2)(x.col1.s0, x.col1.s1);
	res.col2 = (real2)(x.col2.s0, x.col2.s1);
	res.col3 = (real2)(0.0, 0.0);
	return res;
}

force_inline real4x3 Mat3x2_To4x3 (const real3x2 x)
{
	real4x3 res;
	res.col0 = (real3)(x.col0.s0, x.col0.s1, 0.0);
	res.col1 = (real3)(x.col1.s0, x.col1.s1, 0.0);
	res.col2 = (real3)(x.col2.s0, x.col2.s1, 1.0);
	res.col3 = (real3)(0.0, 0.0, 0.0);
	return res;
}

force_inline real4x4 Mat3x2_To4x4 (const real3x2 x)
{
	real4x4 res;
	res.col0 = (real4)(x.col0.s0, x.col0.s1, 0.0, 0.0);
	res.col1 = (real4)(x.col1.s0, x.col1.s1, 0.0, 0.0);
	res.col2 = (real4)(x.col2.s0, x.col2.s1, 1.0, 0.0);
	res.col3 = (real4)(0.0, 0.0, 0.0, 1.0);
	return res;
}

force_inline real2x2 Mat3x3_To2x2 (const real3x3 x)
{
	real2x2 res;
	res.col0 = (real2)(x.col0.s0, x.col0.s1);
	res.col1 = (real2)(x.col1.s0, x.col1.s1);
	return res;
}

force_inline real2x3 Mat3x3_To2x3 (const real3x3 x)
{
	real2x3 res;
	res.col0 = (real3)(x.col0.s0, x.col0.s1, x.col0.s2);
	res.col1 = (real3)(x.col1.s0, x.col1.s1, x.col1.s2);
	return res;
}

force_inline real2x4 Mat3x3_To2x4 (const real3x3 x)
{
	real2x4 res;
	res.col0 = (real4)(x.col0.s0, x.col0.s1, x.col0.s2, 0.0);
	res.col1 = (real4)(x.col1.s0, x.col1.s1, x.col1.s2, 0.0);
	return res;
}

force_inline real3x2 Mat3x3_To3x2 (const real3x3 x)
{
	real3x2 res;
	res.col0 = (real2)(x.col0.s0, x.col0.s1);
	res.col1 = (real2)(x.col1.s0, x.col1.s1);
	res.col2 = (real2)(x.col2.s0, x.col2.s1);
	return res;
}

force_inline real3x4 Mat3x3_To3x4 (const real3x3 x)
{
	real3x4 res;
	res.col0 = (real4)(x.col0.s0, x.col0.s1, x.col0.s2, 0.0);
	res.col1 = (real4)(x.col1.s0, x.col1.s1, x.col1.s2, 0.0);
	res.col2 = (real4)(x.col2.s0, x.col2.s1, x.col2.s2, 0.0);
	return res;
}

force_inline real4x2 Mat3x3_To4x2 (const real3x3 x)
{
	real4x2 res;
	res.col0 = (real2)(x.col0.s0, x.col0.s1);
	res.col1 = (real2)(x.col1.s0, x.col1.s1);
	res.col2 = (real2)(x.col2.s0, x.col2.s1);
	res.col3 = (real2)(0.0, 0.0);
	return res;
}

force_inline real4x3 Mat3x3_To4x3 (const real3x3 x)
{
	real4x3 res;
	res.col0 = (real3)(x.col0.s0, x.col0.s1, x.col0.s2);
	res.col1 = (real3)(x.col1.s0, x.col1.s1, x.col1.s2);
	res.col2 = (real3)(x.col2.s0, x.col2.s1, x.col2.s2);
	res.col3 = (real3)(0.0, 0.0, 0.0);
	return res;
}

force_inline real4x4 Mat3x3_To4x4 (const real3x3 x)
{
	real4x4 res;
	res.col0 = (real4)(x.col0.s0, x.col0.s1, x.col0.s2, 0.0);
	res.col1 = (real4)(x.col1.s0, x.col1.s1, x.col1.s2, 0.0);
	res.col2 = (real4)(x.col2.s0, x.col2.s1, x.col2.s2, 0.0);
	res.col3 = (real4)(0.0, 0.0, 0.0, 1.0);
	return res;
}

force_inline real2x2 Mat3x4_To2x2 (const real3x4 x)
{
	real2x2 res;
	res.col0 = (real2)(x.col0.s0, x.col0.s1);
	res.col1 = (real2)(x.col1.s0, x.col1.s1);
	return res;
}

force_inline real2x3 Mat3x4_To2x3 (const real3x4 x)
{
	real2x3 res;
	res.col0 = (real3)(x.col0.s0, x.col0.s1, x.col0.s2);
	res.col1 = (real3)(x.col1.s0, x.col1.s1, x.col1.s2);
	return res;
}

force_inline real2x4 Mat3x4_To2x4 (const real3x4 x)
{
	real2x4 res;
	res.col0 = (real4)(x.col0.s0, x.col0.s1, x.col0.s2, x.col0.s3);
	res.col1 = (real4)(x.col1.s0, x.col1.s1, x.col1.s2, x.col1.s3);
	return res;
}

force_inline real3x2 Mat3x4_To3x2 (const real3x4 x)
{
	real3x2 res;
	res.col0 = (real2)(x.col0.s0, x.col0.s1);
	res.col1 = (real2)(x.col1.s0, x.col1.s1);
	res.col2 = (real2)(x.col2.s0, x.col2.s1);
	return res;
}

force_inline real3x3 Mat3x4_To3x3 (const real3x4 x)
{
	real3x3 res;
	res.col0 = (real3)(x.col0.s0, x.col0.s1, x.col0.s2);
	res.col1 = (real3)(x.col1.s0, x.col1.s1, x.col1.s2);
	res.col2 = (real3)(x.col2.s0, x.col2.s1, x.col2.s2);
	return res;
}

force_inline real4x2 Mat3x4_To4x2 (const real3x4 x)
{
	real4x2 res;
	res.col0 = (real2)(x.col0.s0, x.col0.s1);
	res.col1 = (real2)(x.col1.s0, x.col1.s1);
	res.col2 = (real2)(x.col2.s0, x.col2.s1);
	res.col3 = (real2)(0.0, 0.0);
	return res;
}

force_inline real4x3 Mat3x4_To4x3 (const real3x4 x)
{
	real4x3 res;
	res.col0 = (real3)(x.col0.s0, x.col0.s1, x.col0.s2);
	res.col1 = (real3)(x.col1.s0, x.col1.s1, x.col1.s2);
	res.col2 = (real3)(x.col2.s0, x.col2.s1, x.col2.s2);
	res.col3 = (real3)(0.0, 0.0, 0.0);
	return res;
}

force_inline real4x4 Mat3x4_To4x4 (const real3x4 x)
{
	real4x4 res;
	res.col0 = (real4)(x.col0.s0, x.col0.s1, x.col0.s2, x.col0.s3);
	res.col1 = (real4)(x.col1.s0, x.col1.s1, x.col1.s2, x.col1.s3);
	res.col2 = (real4)(x.col2.s0, x.col2.s1, x.col2.s2, x.col2.s3);
	res.col3 = (real4)(0.0, 0.0, 0.0, 1.0);
	return res;
}

force_inline real2x2 Mat4x2_To2x2 (const real4x2 x)
{
	real2x2 res;
	res.col0 = (real2)(x.col0.s0, x.col0.s1);
	res.col1 = (real2)(x.col1.s0, x.col1.s1);
	return res;
}

force_inline real2x3 Mat4x2_To2x3 (const real4x2 x)
{
	real2x3 res;
	res.col0 = (real3)(x.col0.s0, x.col0.s1, 0.0);
	res.col1 = (real3)(x.col1.s0, x.col1.s1, 0.0);
	return res;
}

force_inline real2x4 Mat4x2_To2x4 (const real4x2 x)
{
	real2x4 res;
	res.col0 = (real4)(x.col0.s0, x.col0.s1, 0.0, 0.0);
	res.col1 = (real4)(x.col1.s0, x.col1.s1, 0.0, 0.0);
	return res;
}

force_inline real3x2 Mat4x2_To3x2 (const real4x2 x)
{
	real3x2 res;
	res.col0 = (real2)(x.col0.s0, x.col0.s1);
	res.col1 = (real2)(x.col1.s0, x.col1.s1);
	res.col2 = (real2)(x.col2.s0, x.col2.s1);
	return res;
}

force_inline real3x3 Mat4x2_To3x3 (const real4x2 x)
{
	real3x3 res;
	res.col0 = (real3)(x.col0.s0, x.col0.s1, 0.0);
	res.col1 = (real3)(x.col1.s0, x.col1.s1, 0.0);
	res.col2 = (real3)(x.col2.s0, x.col2.s1, 1.0);
	return res;
}

force_inline real3x4 Mat4x2_To3x4 (const real4x2 x)
{
	real3x4 res;
	res.col0 = (real4)(x.col0.s0, x.col0.s1, 0.0, 0.0);
	res.col1 = (real4)(x.col1.s0, x.col1.s1, 0.0, 0.0);
	res.col2 = (real4)(x.col2.s0, x.col2.s1, 1.0, 0.0);
	return res;
}

force_inline real4x3 Mat4x2_To4x3 (const real4x2 x)
{
	real4x3 res;
	res.col0 = (real3)(x.col0.s0, x.col0.s1, 0.0);
	res.col1 = (real3)(x.col1.s0, x.col1.s1, 0.0);
	res.col2 = (real3)(x.col2.s0, x.col2.s1, 1.0);
	res.col3 = (real3)(x.col3.s0, x.col3.s1, 0.0);
	return res;
}

force_inline real4x4 Mat4x2_To4x4 (const real4x2 x)
{
	real4x4 res;
	res.col0 = (real4)(x.col0.s0, x.col0.s1, 0.0, 0.0);
	res.col1 = (real4)(x.col1.s0, x.col1.s1, 0.0, 0.0);
	res.col2 = (real4)(x.col2.s0, x.col2.s1, 1.0, 0.0);
	res.col3 = (real4)(x.col3.s0, x.col3.s1, 0.0, 1.0);
	return res;
}

force_inline real2x2 Mat4x3_To2x2 (const real4x3 x)
{
	real2x2 res;
	res.col0 = (real2)(x.col0.s0, x.col0.s1);
	res.col1 = (real2)(x.col1.s0, x.col1.s1);
	return res;
}

force_inline real2x3 Mat4x3_To2x3 (const real4x3 x)
{
	real2x3 res;
	res.col0 = (real3)(x.col0.s0, x.col0.s1, x.col0.s2);
	res.col1 = (real3)(x.col1.s0, x.col1.s1, x.col1.s2);
	return res;
}

force_inline real2x4 Mat4x3_To2x4 (const real4x3 x)
{
	real2x4 res;
	res.col0 = (real4)(x.col0.s0, x.col0.s1, x.col0.s2, 0.0);
	res.col1 = (real4)(x.col1.s0, x.col1.s1, x.col1.s2, 0.0);
	return res;
}

force_inline real3x2 Mat4x3_To3x2 (const real4x3 x)
{
	real3x2 res;
	res.col0 = (real2)(x.col0.s0, x.col0.s1);
	res.col1 = (real2)(x.col1.s0, x.col1.s1);
	res.col2 = (real2)(x.col2.s0, x.col2.s1);
	return res;
}

force_inline real3x3 Mat4x3_To3x3 (const real4x3 x)
{
	real3x3 res;
	res.col0 = (real3)(x.col0.s0, x.col0.s1, x.col0.s2);
	res.col1 = (real3)(x.col1.s0, x.col1.s1, x.col1.s2);
	res.col2 = (real3)(x.col2.s0, x.col2.s1, x.col2.s2);
	return res;
}

force_inline real3x4 Mat4x3_To3x4 (const real4x3 x)
{
	real3x4 res;
	res.col0 = (real4)(x.col0.s0, x.col0.s1, x.col0.s2, 0.0);
	res.col1 = (real4)(x.col1.s0, x.col1.s1, x.col1.s2, 0.0);
	res.col2 = (real4)(x.col2.s0, x.col2.s1, x.col2.s2, 0.0);
	return res;
}

force_inline real4x2 Mat4x3_To4x2 (const real4x3 x)
{
	real4x2 res;
	res.col0 = (real2)(x.col0.s0, x.col0.s1);
	res.col1 = (real2)(x.col1.s0, x.col1.s1);
	res.col2 = (real2)(x.col2.s0, x.col2.s1);
	res.col3 = (real2)(x.col3.s0, x.col3.s1);
	return res;
}

force_inline real4x4 Mat4x3_To4x4 (const real4x3 x)
{
	real4x4 res;
	res.col0 = (real4)(x.col0.s0, x.col0.s1, x.col0.s2, 0.0);
	res.col1 = (real4)(x.col1.s0, x.col1.s1, x.col1.s2, 0.0);
	res.col2 = (real4)(x.col2.s0, x.col2.s1, x.col2.s2, 0.0);
	res.col3 = (real4)(x.col3.s0, x.col3.s1, x.col3.s2, 1.0);
	return res;
}

force_inline real2x2 Mat4x4_To2x2 (const real4x4 x)
{
	r)#";

		src << R"#(eal2x2 res;
	res.col0 = (real2)(x.col0.s0, x.col0.s1);
	res.col1 = (real2)(x.col1.s0, x.col1.s1);
	return res;
}

force_inline real2x3 Mat4x4_To2x3 (const real4x4 x)
{
	real2x3 res;
	res.col0 = (real3)(x.col0.s0, x.col0.s1, x.col0.s2);
	res.col1 = (real3)(x.col1.s0, x.col1.s1, x.col1.s2);
	return res;
}

force_inline real2x4 Mat4x4_To2x4 (const real4x4 x)
{
	real2x4 res;
	res.col0 = (real4)(x.col0.s0, x.col0.s1, x.col0.s2, x.col0.s3);
	res.col1 = (real4)(x.col1.s0, x.col1.s1, x.col1.s2, x.col1.s3);
	return res;
}

force_inline real3x2 Mat4x4_To3x2 (const real4x4 x)
{
	real3x2 res;
	res.col0 = (real2)(x.col0.s0, x.col0.s1);
	res.col1 = (real2)(x.col1.s0, x.col1.s1);
	res.col2 = (real2)(x.col2.s0, x.col2.s1);
	return res;
}

force_inline real3x3 Mat4x4_To3x3 (const real4x4 x)
{
	real3x3 res;
	res.col0 = (real3)(x.col0.s0, x.col0.s1, x.col0.s2);
	res.col1 = (real3)(x.col1.s0, x.col1.s1, x.col1.s2);
	res.col2 = (real3)(x.col2.s0, x.col2.s1, x.col2.s2);
	return res;
}

force_inline real3x4 Mat4x4_To3x4 (const real4x4 x)
{
	real3x4 res;
	res.col0 = (real4)(x.col0.s0, x.col0.s1, x.col0.s2, x.col0.s3);
	res.col1 = (real4)(x.col1.s0, x.col1.s1, x.col1.s2, x.col1.s3);
	res.col2 = (real4)(x.col2.s0, x.col2.s1, x.col2.s2, x.col2.s3);
	return res;
}

force_inline real4x2 Mat4x4_To4x2 (const real4x4 x)
{
	real4x2 res;
	res.col0 = (real2)(x.col0.s0, x.col0.s1);
	res.col1 = (real2)(x.col1.s0, x.col1.s1);
	res.col2 = (real2)(x.col2.s0, x.col2.s1);
	res.col3 = (real2)(x.col3.s0, x.col3.s1);
	return res;
}

force_inline real4x3 Mat4x4_To4x3 (const real4x4 x)
{
	real4x3 res;
	res.col0 = (real3)(x.col0.s0, x.col0.s1, x.col0.s2);
	res.col1 = (real3)(x.col1.s0, x.col1.s1, x.col1.s2);
	res.col2 = (real3)(x.col2.s0, x.col2.s1, x.col2.s2);
	res.col3 = (real3)(x.col3.s0, x.col3.s1, x.col3.s2);
	return res;
}


// Matrix multiplication
force_inline real2x2 Mat2x2_Mul2x2 (const real2x2 left, const real2x2 right)
{
	real2x2 res;
	res.col0.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col0 );
	res.col1.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col1 );
	res.col0.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col0 );
	res.col1.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col1 );
	return res;
}

force_inline real3x2 Mat2x2_Mul3x2 (const real2x2 left, const real3x2 right)
{
	real3x2 res;
	res.col0.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col0 );
	res.col1.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col1 );
	res.col2.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col2 );
	res.col0.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col0 );
	res.col1.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col1 );
	res.col2.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col2 );
	return res;
}

force_inline real4x2 Mat2x2_Mul4x2 (const real2x2 left, const real4x2 right)
{
	real4x2 res;
	res.col0.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col0 );
	res.col1.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col1 );
	res.col2.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col2 );
	res.col3.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col3 );
	res.col0.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col0 );
	res.col1.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col1 );
	res.col2.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col2 );
	res.col3.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col3 );
	return res;
}

force_inline real2x3 Mat2x3_Mul2x2 (const real2x3 left, const real2x2 right)
{
	real2x3 res;
	res.col0.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col0 );
	res.col1.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col1 );
	res.col0.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col0 );
	res.col1.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col1 );
	res.col0.s2 = dot( (real2)(left.col0.s2, left.col1.s2), right.col0 );
	res.col1.s2 = dot( (real2)(left.col0.s2, left.col1.s2), right.col1 );
	return res;
}

force_inline real3x3 Mat2x3_Mul3x2 (const real2x3 left, const real3x2 right)
{
	real3x3 res;
	res.col0.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col0 );
	res.col1.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col1 );
	res.col2.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col2 );
	res.col0.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col0 );
	res.col1.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col1 );
	res.col2.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col2 );
	res.col0.s2 = dot( (real2)(left.col0.s2, left.col1.s2), right.col0 );
	res.col1.s2 = dot( (real2)(left.col0.s2, left.col1.s2), right.col1 );
	res.col2.s2 = dot( (real2)(left.col0.s2, left.col1.s2), right.col2 );
	return res;
}

force_inline real4x3 Mat2x3_Mul4x2 (const real2x3 left, const real4x2 right)
{
	real4x3 res;
	res.col0.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col0 );
	res.col1.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col1 );
	res.col2.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col2 );
	res.col3.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col3 );
	res.col0.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col0 );
	res.col1.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col1 );
	res.col2.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col2 );
	res.col3.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col3 );
	res.col0.s2 = dot( (real2)(left.col0.s2, left.col1.s2), right.col0 );
	res.col1.s2 = dot( (real2)(left.col0.s2, left.col1.s2), right.col1 );
	res.col2.s2 = dot( (real2)(left.col0.s2, left.col1.s2), right.col2 );
	res.col3.s2 = dot( (real2)(left.col0.s2, left.col1.s2), right.col3 );
	return res;
}

force_inline real2x4 Mat2x4_Mul2x2 (const real2x4 left, const real2x2 right)
{
	real2x4 res;
	res.col0.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col0 );
	res.col1.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col1 );
	res.col0.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col0 );
	res.col1.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col1 );
	res.col0.s2 = dot( (real2)(left.col0.s2, left.col1.s2), right.col0 );
	res.col1.s2 = dot( (real2)(left.col0.s2, left.col1.s2), right.col1 );
	res.col0.s3 = dot( (real2)(left.col0.s3, left.col1.s3), right.col0 );
	res.col1.s3 = dot( (real2)(left.col0.s3, left.col1.s3), right.col1 );
	return res;
}

force_inline real3x4 Mat2x4_Mul3x2 (const real2x4 left, const real3x2 right)
{
	real3x4 res;
	res.col0.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col0 );
	res.col1.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col1 );
	res.col2.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col2 );
	res.col0.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col0 );
	res.col1.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col1 );
	res.col2.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col2 );
	res.col0.s2 = dot( (real2)(left.col0.s2, left.col1.s2), right.col0 );
	res.col1.s2 = dot( (real2)(left.col0.s2, left.col1.s2), right.col1 );
	res.col2.s2 = dot( (real2)(left.col0.s2, left.col1.s2), right.col2 );
	res.col0.s3 = dot( (real2)(left.col0.s3, left.col1.s3), right.col0 );
	res.col1.s3 = dot( (real2)(left.col0.s3, left.col1.s3), right.col1 );
	res.col2.s3 = dot( (real2)(left.col0.s3, left.col1.s3), right.col2 );
	return res;
}

force_inline real4x4 Mat2x4_Mul4x2 (const real2x4 left, const real4x2 right)
{
	real4x4 res;
	res.col0.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col0 );
	res.col1.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col1 );
	res.col2.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col2 );
	res.col3.s0 = dot( (real2)(left.col0.s0, left.col1.s0), right.col3 );
	res.col0.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col0 );
	res.col1.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col1 );
	res.col2.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col2 );
	res.col3.s1 = dot( (real2)(left.col0.s1, left.col1.s1), right.col3 );
	res.col0.s2 = dot( (real2)(left.col0.s2, left.col1.s2), right.col0 );
	res.col1.s2 = dot( (real2)(left.col0.s2, left.col1.s2), right.col1 );
	res.col2.s2 = dot( (real2)(left.col0.s2, left.col1.s2), right.col2 );
	res.col3.s2 = dot( (real2)(left.col0.s2, left.col1.s2), right.col3 );
	res.col0.s3 = dot( (real2)(left.col0.s3, left.col1.s3), right.col0 );
	res.col1.s3 = dot( (real2)(left.col0.s3, left.col1.s3), right.col1 );
	res.col2.s3 = dot( (real2)(left.col0.s3, left.col1.s3), right.col2 );
	res.col3.s3 = dot( (real2)(left.col0.s3, left.col1.s3), right.col3 );
	return res;
}

force_inline real2x2 Mat3x2_Mul2x3 (const real3x2 left, const real2x3 right)
{
	real2x2 res;
	res.col0.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col0 );
	res.col1.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col1 );
	res.col0.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col0 );
	res.col1.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col1 );
	return res;
}

force_inline real3x2 Mat3x2_Mul3x3 (const real3x2 left, const real3x3 right)
{
	real3x2 res;
	res.col0.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col0 );
	res.col1.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col1 );
	res.col2.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col2 );
	res.col0.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col0 );
	res.col1.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col1 );
	res.col2.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col2 );
	return res;
}

force_inline real4x2 Mat3x2_Mul4x3 (const real3x2 left, const real4x3 right)
{
	real4x2 res;
	res.col0.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col0 );
	res.col1.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col1 );
	res.col2.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col2 );
	res.col3.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col3 );
	res.col0.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col0 );
	res.col1.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col1 );
	res.col2.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col2 );
	res.col3.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col3 );
	return res;
}

force_inline real2x3 Mat3x3_Mul2x3 (const real3x3 left, const real2x3 right)
{
	real2x3 res;
	res.col0.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col0 );
	res.col1.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col1 );
	res.col0.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col0 );
	res.col1.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col1 );
	res.col0.s2 = dot( (real3)(left.col0.s2, left.col1.s2, left.col2.s2), right.col0 );
	res.col1.s2 = dot( (real3)(left.col0.s2, left.col1.s2, left.col2.s2), right.col1 );
	return res;
}

force_inline real3x3 Mat3x3_Mul3x3 (const real3x3 left, const real3x3 right)
{
	real3x3 res;
	res.col0.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col0 );
	res.col1.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col1 );
	res.col2.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col2 );
	res.col0.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col0 );
	res.col1.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col1 );
	res.col2.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col2 );
	res.col0.s2 = dot( (real3)(left.col0.s2, left.col1.s2, left.col2.s2), right.col0 );
	res.col1.s2 = dot( (real3)(left.col0.s2, left.col1.s2, left.col2.s2), right.col1 );
	res.col2.s2 = dot( (real3)(left.col0.s2, left.col1.s2, left.col2.s2), right.col2 );
	return res;
}

force_inline real4x3 Mat3x3_Mul4x3 (const real3x3 left, const real4x3 right)
{
	real4x3 res;
	res.col0.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col0 );
	res.col1.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col1 );
	res.col2.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col2 );
	res.col3.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col3 );
	res.col0.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col0 );
	res.col1.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col1 );
	res.col2.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col2 );
	res.col3.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col3 );
	res.col0.s2 = dot( (real3)(left.col0.s2, left.col1.s2, left.col2.s2), right.col0 );
	res.col1.s2 = dot( (real3)(left.col0.s2, left.col1.s2, left.col2.s2), right.col1 );
	res.col2.s2 = dot( (real3)(left.col0.s2, left.col1.s2, left.col2.s2), right.col2 );
	res.col3.s2 = dot( (real3)(left.col0.s2, left.col1.s2, left.col2.s2), right.col3 );
	return res;
}

force_inline real2x4 Mat3x4_Mul2x3 (const real3x4 left, const real2x3 right)
{
	real2x4 res;
	res.col0.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col0 );
	res.col1.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col1 );
	res.col0.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col0 );
	res.col1.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col1 );
	res.col0.s2 = dot( (real3)(left.col0.s2, left.col1.s2, left.col2.s2), right.col0 );
	res.col1.s2 = dot( (real3)(left.col0.s2, left.col1.s2, left.col2.s2), right.col1 );
	res.col0.s3 = dot( (real3)(left.col0.s3, left.col1.s3, left.col2.s3), right.col0 );
	res.col1.s3 = dot( (real3)(left.col0.s3, left.col1.s3, left.col2.s3), right.col1 );
	return res;
}

force_inline real3x4 Mat3x4_Mul3x3 (const real3x4 left, const real3x3 right)
{
	real3x4 res;
	res.col0.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col0 );
	res.col1.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col1 );
	res.col2.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col2 );
	res.col0.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col0 );
	res.col1.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col1 );
	res.col2.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col2 );
	res.col0.s2 = dot( (real3)(left.col0.s2, left.col1.s2, left.col2.s2), right.col0 );
	res.col1.s2 = dot( (real3)(left.col0.s2, left.col1.s2, left.col2.s2), right.col1 );
	res.col2.s2 = dot( (real3)(left.col0.s2, left.col1.s2, left.col2.s2), right.col2 );
	res.col0.s3 = dot( (real3)(left.col0.s3, left.col1.s3, left.col2.s3), right.col0 );
	res.col1.s3 = dot( (real3)(left.col0.s3, left.col1.s3, left.col2.s3), right.col1 );
	res.col2.s3 = dot( (real3)(left.col0.s3, left.col1.s3, left.col2.s3), right.col2 );
	return res;
}

force_inline real4x4 Mat3x4_Mul4x3 (const real3x4 left, const real4x3 right)
{
	real4x4 res;
	res.col0.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col0 );
	res.col1.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col1 );
	res.col2.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col2 );
	res.col3.s0 = dot( (real3)(left.col0.s0, left.col1.s0, left.col2.s0), right.col3 );
	res.col0.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col0 );
	res.col1.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col1 );
	res.col2.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col2 );
	res.col3.s1 = dot( (real3)(left.col0.s1, left.col1.s1, left.col2.s1), right.col3 );
	res.col0.s2 = dot( (real3)(left.col0.s2, left.col1.s2, left.col2.s2), right.col0 );
	res.col1.s2 = dot( (real3)(left.col0.s2, left.col1.s2, left.col2.s2), right.col1 );
	res.col2.s2 = dot( (real3)(left.col0.s2, left.col1.s2, left.col2.s2), right.col2 );
	re)#";

		src << R"#(s.col3.s2 = dot( (real3)(left.col0.s2, left.col1.s2, left.col2.s2), right.col3 );
	res.col0.s3 = dot( (real3)(left.col0.s3, left.col1.s3, left.col2.s3), right.col0 );
	res.col1.s3 = dot( (real3)(left.col0.s3, left.col1.s3, left.col2.s3), right.col1 );
	res.col2.s3 = dot( (real3)(left.col0.s3, left.col1.s3, left.col2.s3), right.col2 );
	res.col3.s3 = dot( (real3)(left.col0.s3, left.col1.s3, left.col2.s3), right.col3 );
	return res;
}

force_inline real2x2 Mat4x2_Mul2x4 (const real4x2 left, const real2x4 right)
{
	real2x2 res;
	res.col0.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col0 );
	res.col1.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col1 );
	res.col0.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col0 );
	res.col1.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col1 );
	return res;
}

force_inline real3x2 Mat4x2_Mul3x4 (const real4x2 left, const real3x4 right)
{
	real3x2 res;
	res.col0.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col0 );
	res.col1.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col1 );
	res.col2.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col2 );
	res.col0.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col0 );
	res.col1.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col1 );
	res.col2.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col2 );
	return res;
}

force_inline real4x2 Mat4x2_Mul4x4 (const real4x2 left, const real4x4 right)
{
	real4x2 res;
	res.col0.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col0 );
	res.col1.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col1 );
	res.col2.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col2 );
	res.col3.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col3 );
	res.col0.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col0 );
	res.col1.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col1 );
	res.col2.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col2 );
	res.col3.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col3 );
	return res;
}

force_inline real2x3 Mat4x3_Mul2x4 (const real4x3 left, const real2x4 right)
{
	real2x3 res;
	res.col0.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col0 );
	res.col1.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col1 );
	res.col0.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col0 );
	res.col1.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col1 );
	res.col0.s2 = dot( (real4)(left.col0.s2, left.col1.s2, left.col2.s2, left.col3.s2), right.col0 );
	res.col1.s2 = dot( (real4)(left.col0.s2, left.col1.s2, left.col2.s2, left.col3.s2), right.col1 );
	return res;
}

force_inline real3x3 Mat4x3_Mul3x4 (const real4x3 left, const real3x4 right)
{
	real3x3 res;
	res.col0.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col0 );
	res.col1.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col1 );
	res.col2.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col2 );
	res.col0.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col0 );
	res.col1.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col1 );
	res.col2.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col2 );
	res.col0.s2 = dot( (real4)(left.col0.s2, left.col1.s2, left.col2.s2, left.col3.s2), right.col0 );
	res.col1.s2 = dot( (real4)(left.col0.s2, left.col1.s2, left.col2.s2, left.col3.s2), right.col1 );
	res.col2.s2 = dot( (real4)(left.col0.s2, left.col1.s2, left.col2.s2, left.col3.s2), right.col2 );
	return res;
}

force_inline real4x3 Mat4x3_Mul4x4 (const real4x3 left, const real4x4 right)
{
	real4x3 res;
	res.col0.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col0 );
	res.col1.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col1 );
	res.col2.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col2 );
	res.col3.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col3 );
	res.col0.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col0 );
	res.col1.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col1 );
	res.col2.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col2 );
	res.col3.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col3 );
	res.col0.s2 = dot( (real4)(left.col0.s2, left.col1.s2, left.col2.s2, left.col3.s2), right.col0 );
	res.col1.s2 = dot( (real4)(left.col0.s2, left.col1.s2, left.col2.s2, left.col3.s2), right.col1 );
	res.col2.s2 = dot( (real4)(left.col0.s2, left.col1.s2, left.col2.s2, left.col3.s2), right.col2 );
	res.col3.s2 = dot( (real4)(left.col0.s2, left.col1.s2, left.col2.s2, left.col3.s2), right.col3 );
	return res;
}

force_inline real2x4 Mat4x4_Mul2x4 (const real4x4 left, const real2x4 right)
{
	real2x4 res;
	res.col0.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col0 );
	res.col1.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col1 );
	res.col0.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col0 );
	res.col1.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col1 );
	res.col0.s2 = dot( (real4)(left.col0.s2, left.col1.s2, left.col2.s2, left.col3.s2), right.col0 );
	res.col1.s2 = dot( (real4)(left.col0.s2, left.col1.s2, left.col2.s2, left.col3.s2), right.col1 );
	res.col0.s3 = dot( (real4)(left.col0.s3, left.col1.s3, left.col2.s3, left.col3.s3), right.col0 );
	res.col1.s3 = dot( (real4)(left.col0.s3, left.col1.s3, left.col2.s3, left.col3.s3), right.col1 );
	return res;
}

force_inline real3x4 Mat4x4_Mul3x4 (const real4x4 left, const real3x4 right)
{
	real3x4 res;
	res.col0.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col0 );
	res.col1.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col1 );
	res.col2.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col2 );
	res.col0.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col0 );
	res.col1.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col1 );
	res.col2.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col2 );
	res.col0.s2 = dot( (real4)(left.col0.s2, left.col1.s2, left.col2.s2, left.col3.s2), right.col0 );
	res.col1.s2 = dot( (real4)(left.col0.s2, left.col1.s2, left.col2.s2, left.col3.s2), right.col1 );
	res.col2.s2 = dot( (real4)(left.col0.s2, left.col1.s2, left.col2.s2, left.col3.s2), right.col2 );
	res.col0.s3 = dot( (real4)(left.col0.s3, left.col1.s3, left.col2.s3, left.col3.s3), right.col0 );
	res.col1.s3 = dot( (real4)(left.col0.s3, left.col1.s3, left.col2.s3, left.col3.s3), right.col1 );
	res.col2.s3 = dot( (real4)(left.col0.s3, left.col1.s3, left.col2.s3, left.col3.s3), right.col2 );
	return res;
}

force_inline real4x4 Mat4x4_Mul4x4 (const real4x4 left, const real4x4 right)
{
	real4x4 res;
	res.col0.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col0 );
	res.col1.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col1 );
	res.col2.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col2 );
	res.col3.s0 = dot( (real4)(left.col0.s0, left.col1.s0, left.col2.s0, left.col3.s0), right.col3 );
	res.col0.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col0 );
	res.col1.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col1 );
	res.col2.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col2 );
	res.col3.s1 = dot( (real4)(left.col0.s1, left.col1.s1, left.col2.s1, left.col3.s1), right.col3 );
	res.col0.s2 = dot( (real4)(left.col0.s2, left.col1.s2, left.col2.s2, left.col3.s2), right.col0 );
	res.col1.s2 = dot( (real4)(left.col0.s2, left.col1.s2, left.col2.s2, left.col3.s2), right.col1 );
	res.col2.s2 = dot( (real4)(left.col0.s2, left.col1.s2, left.col2.s2, left.col3.s2), right.col2 );
	res.col3.s2 = dot( (real4)(left.col0.s2, left.col1.s2, left.col2.s2, left.col3.s2), right.col3 );
	res.col0.s3 = dot( (real4)(left.col0.s3, left.col1.s3, left.col2.s3, left.col3.s3), right.col0 );
	res.col1.s3 = dot( (real4)(left.col0.s3, left.col1.s3, left.col2.s3, left.col3.s3), right.col1 );
	res.col2.s3 = dot( (real4)(left.col0.s3, left.col1.s3, left.col2.s3, left.col3.s3), right.col2 );
	res.col3.s3 = dot( (real4)(left.col0.s3, left.col1.s3, left.col2.s3, left.col3.s3), right.col3 );
	return res;
}


// Transform vector
force_inline real2 Mat2x2_Mul (const real2x2 left, const real2 right)
{
	real2 res;
	res.s0 = dot( left.col0, right.s01 );
	res.s1 = dot( left.col1, right.s01 );
	return res;
}

force_inline real2 Mat2x3_Mul (const real2x3 left, const real3 right)
{
	real2 res;
	res.s0 = dot( left.col0, right.s012 );
	res.s1 = dot( left.col1, right.s012 );
	return res;
}

force_inline real2 Mat2x4_Mul (const real2x4 left, const real4 right)
{
	real2 res;
	res.s0 = dot( left.col0, right.s0123 );
	res.s1 = dot( left.col1, right.s0123 );
	return res;
}

force_inline real3 Mat3x2_Mul (const real3x2 left, const real2 right)
{
	real3 res;
	res.s0 = dot( left.col0, right.s01 );
	res.s1 = dot( left.col1, right.s01 );
	res.s2 = dot( left.col2, right.s01 );
	return res;
}

force_inline real3 Mat3x3_Mul (const real3x3 left, const real3 right)
{
	real3 res;
	res.s0 = dot( left.col0, right.s012 );
	res.s1 = dot( left.col1, right.s012 );
	res.s2 = dot( left.col2, right.s012 );
	return res;
}

force_inline real3 Mat3x4_Mul (const real3x4 left, const real4 right)
{
	real3 res;
	res.s0 = dot( left.col0, right.s0123 );
	res.s1 = dot( left.col1, right.s0123 );
	res.s2 = dot( left.col2, right.s0123 );
	return res;
}

force_inline real4 Mat4x2_Mul (const real4x2 left, const real2 right)
{
	real4 res;
	res.s0 = dot( left.col0, right.s01 );
	res.s1 = dot( left.col1, right.s01 );
	res.s2 = dot( left.col2, right.s01 );
	res.s3 = dot( left.col3, right.s01 );
	return res;
}

force_inline real4 Mat4x3_Mul (const real4x3 left, const real3 right)
{
	real4 res;
	res.s0 = dot( left.col0, right.s012 );
	res.s1 = dot( left.col1, right.s012 );
	res.s2 = dot( left.col2, right.s012 );
	res.s3 = dot( left.col3, right.s012 );
	return res;
}

force_inline real4 Mat4x4_Mul (const real4x4 left, const real4 right)
{
	real4 res;
	res.s0 = dot( left.col0, right.s0123 );
	res.s1 = dot( left.col1, right.s0123 );
	res.s2 = dot( left.col2, right.s0123 );
	res.s3 = dot( left.col3, right.s0123 );
	return res;
}


// Transform vector
force_inline real2 Mul_Mat2x2 (const real2 left, const real2x2 right)
{
	real2 res;
	res.s0 = dot( (real2)(right.col0.s0, right.col1.s0), left.s01 );
	res.s1 = dot( (real2)(right.col0.s1, right.col1.s1), left.s01 );
	return res;
}

force_inline real3 Mul_Mat2x3 (const real2 left, const real2x3 right)
{
	real3 res;
	res.s0 = dot( (real2)(right.col0.s0, right.col1.s0), left.s01 );
	res.s1 = dot( (real2)(right.col0.s1, right.col1.s1), left.s01 );
	res.s2 = dot( (real2)(right.col0.s2, right.col1.s2), left.s01 );
	return res;
}

force_inline real4 Mul_Mat2x4 (const real2 left, const real2x4 right)
{
	real4 res;
	res.s0 = dot( (real2)(right.col0.s0, right.col1.s0), left.s01 );
	res.s1 = dot( (real2)(right.col0.s1, right.col1.s1), left.s01 );
	res.s2 = dot( (real2)(right.col0.s2, right.col1.s2), left.s01 );
	res.s3 = dot( (real2)(right.col0.s3, right.col1.s3), left.s01 );
	return res;
}

force_inline real2 Mul_Mat3x2 (const real3 left, const real3x2 right)
{
	real2 res;
	res.s0 = dot( (real3)(right.col0.s0, right.col1.s0, right.col2.s0), left.s012 );
	res.s1 = dot( (real3)(right.col0.s1, right.col1.s1, right.col2.s1), left.s012 );
	return res;
}

force_inline real3 Mul_Mat3x3 (const real3 left, const real3x3 right)
{
	real3 res;
	res.s0 = dot( (real3)(right.col0.s0, right.col1.s0, right.col2.s0), left.s012 );
	res.s1 = dot( (real3)(right.col0.s1, right.col1.s1, right.col2.s1), left.s012 );
	res.s2 = dot( (real3)(right.col0.s2, right.col1.s2, right.col2.s2), left.s012 );
	return res;
}

force_inline real4 Mul_Mat3x4 (const real3 left, const real3x4 right)
{
	real4 res;
	res.s0 = dot( (real3)(right.col0.s0, right.col1.s0, right.col2.s0), left.s012 );
	res.s1 = dot( (real3)(right.col0.s1, right.col1.s1, right.col2.s1), left.s012 );
	res.s2 = dot( (real3)(right.col0.s2, right.col1.s2, right.col2.s2), left.s012 );
	res.s3 = dot( (real3)(right.col0.s3, right.col1.s3, right.col2.s3), left.s012 );
	return res;
}

force_inline real2 Mul_Mat4x2 (const real4 left, const real4x2 right)
{
	real2 res;
	res.s0 = dot( (real4)(right.col0.s0, right.col1.s0, right.col2.s0, right.col3.s0), left.s0123 );
	res.s1 = dot( (real4)(right.col0.s1, right.col1.s1, right.col2.s1, right.col3.s1), left.s0123 );
	return res;
}

force_inline real3 Mul_Mat4x3 (const real4 left, const real4x3 right)
{
	real3 res;
	res.s0 = dot( (real4)(right.col0.s0, right.col1.s0, right.col2.s0, right.col3.s0), left.s0123 );
	res.s1 = dot( (real4)(right.col0.s1, right.col1.s1, right.col2.s1, right.col3.s1), left.s0123 );
	res.s2 = dot( (real4)(right.col0.s2, right.col1.s2, right.col2.s2, right.col3.s2), left.s0123 );
	return res;
}

force_inline real4 Mul_Mat4x4 (const real4 left, const real4x4 right)
{
	real4 res;
	res.s0 = dot( (real4)(right.col0.s0, right.col1.s0, right.col2.s0, right.col3.s0), left.s0123 );
	res.s1 = dot( (real4)(right.col0.s1, right.col1.s1, right.col2.s1, right.col3.s1), left.s0123 );
	res.s2 = dot( (real4)(right.col0.s2, right.col1.s2, right.col2.s2, right.col3.s2), left.s0123 );
	res.s3 = dot( (real4)(right.col0.s3, right.col1.s3, right.col2.s3, right.col3.s3), left.s0123 );
	return res;
}

)#";

	}
}	// cl_vfs
