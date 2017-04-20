// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Vec.h"

namespace GX_STL
{
namespace GXMath
{

#	define MAT_BIN_OPERATOR_SCALAR( _op_ ) \
		\
		Self &	operator _op_##= (const T& right) \
		{ \
			FOR( i, *this )	 (*this)[i] _op_##= right; \
			return *this; \
		} \
		\
		Self	operator _op_  (const T& right) const \
		{ \
			Self	ret; \
			FOR( i, *this )	 ret[i] = (*this)[i] _op_ right; \
			return ret; \
		} \
		\
		friend Self operator _op_ (const T& left, const Self& right) \
		{ \
			return Self(left) _op_ right; \
		}


#	define MAT_BIN_OPERATOR_MAT( _op_ ) \
		\
		Self &	operator _op_##= (const Self& right) \
		{ \
			FOR( i, *this )	 (*this)[i] _op_##= right[i]; \
			return *this; \
		} \
		\
		Self	operator _op_  (const Self& right) const \
		{ \
			Self	ret; \
			FOR( i, *this )	 ret[i] = (*this)[i] _op_ right[i]; \
			return *this; \
		}


#	define MAT_BIN_OPERATOR( _op_ ) \
		MAT_BIN_OPERATOR_SCALAR( _op_ ) \
		MAT_BIN_OPERATOR_MAT( _op_ )
	

	
	template <typename T, usize C, usize R, usize Q>
	struct TMatType
	{
		typedef T	_lmat_t[C][R];
		typedef T	_rmat_t[Q][C];
		typedef T	_dmat_t[Q][R];
	};

#	define lmat_t	typename TMatType<T,C,R,Q>::_lmat_t
#	define rmat_t	typename TMatType<T,C,R,Q>::_rmat_t
#	define dmat_t	typename TMatType<T,C,R,Q>::_dmat_t



	//
	// Matrix Multiplication
	//

	namespace _math_hidden_
	{
		template <usize I>
		struct TMatMul
		{
			template <typename T, usize C, usize R, usize Q>
			static T  _Mul3(lmat_t const &left, rmat_t const &right, usize i, usize j)
			{
				return left[I][i] * right[j][I] + TMatMul<I-1>::template _Mul3<T,C,R,Q>( left, right, i, j );
			}
		
			template <typename T, usize C, usize R, usize Q>
			static void _Mul2(dmat_t &dst, lmat_t const &left, rmat_t const &right, usize i)
			{
				dst[I][i] = TMatMul<C-1>::template _Mul3<T,C,R,Q>( left, right, i, I );
				TMatMul<I-1>::template _Mul2<T,C,R,Q>( dst, left, right, i );
			}
		
			template <typename T, usize C, usize R, usize Q>
			static void _Mul(dmat_t &dst, lmat_t const &left, rmat_t const &right)
			{
				TMatMul<Q-1>::template _Mul2<T,C,R,Q>( dst, left, right, I );
				TMatMul<I-1>::template _Mul<T,C,R,Q>( dst, left, right );
			}
		};


		template <>
		struct TMatMul<0>
		{
			template <typename T, usize C, usize R, usize Q>
			static T  _Mul3(lmat_t const &left, rmat_t const &right, usize i, usize j)
			{
				return left[0][i] * right[j][0];
			}
		
			template <typename T, usize C, usize R, usize Q>
			static void _Mul2(dmat_t &dst, lmat_t const &left, rmat_t const &right, usize i)
			{
				dst[0][i] = TMatMul<C-1>::template _Mul3<T,C,R,Q>( left, right, i, 0 );
			}

			template <typename T, usize C, usize R, usize Q>
			static void _Mul(dmat_t &dst, lmat_t const &left, rmat_t const &right)
			{
				TMatMul<Q-1>::template _Mul2<T,C,R,Q>( dst, left, right, 0 );
			}
		};

	}	// _math_hidden_
	

	template <typename T, usize C, usize R, usize Q>
	struct TMatMul
	{
		TMatMul(dmat_t &dst, lmat_t const &left, rmat_t const &right)
		{
			_math_hidden_::TMatMul<R-1>::template _Mul<T,C,R,Q>( dst, left, right );
		}
	};



	//
	// Matrix For Each
	//

	namespace _math_hidden_
	{

		template <usize iR>
		struct ForRows
		{
			template <usize C, usize R, typename T, typename D, typename M, template <usize nC, usize nR> class G>
			static void  Step(M &m, const D &d)
			{
				m[C][iR] = G<C,iR>::template Elem<T>( d );
				ForRows<iR-1>::template Step< C, R, T, D, M, G >( m, d );
			}
		};

		template <>
		struct ForRows<0>
		{
			template <usize C, usize R, typename T, typename D, typename M, template <usize nC, usize nR> class G>
			static void  Step(M &m, const D &d)
			{
				m[C][0] = G<C,0>::template Elem<T>( d );
			}
		};


		template <usize iC>
		struct ForColumns
		{
			template <usize C, usize R, typename T, typename D, typename M, template <usize nC, usize nR> class G>
			static void  Step(M &m, const D &d)
			{
				ForRows<R-1>::template Step< iC, R, T, D, M, G >( m, d );
				ForColumns<iC-1>::template Step< C, R, T, D, M, G >( m, d );
			}
		};

		template <>
		struct ForColumns<0>
		{
			template <usize C, usize R, typename T, typename D, typename M, template <usize nC, usize nR> class G>
			static void  Step(M &m, const D &d)
			{
				ForRows<R-1>::template Step< 0, R, T, D, M, G >( m, d );
			}
		};

	}	// _math_hidden_


	template <typename T, template <usize nC, usize nR> class G, typename D, usize MC, usize MR>
	struct MatForEach
	{
		typedef T	mat_t[MC][MR];


		template <usize C, usize R>
		static void  Do(mat_t &m, const D &data)
		{
			enum {
				Columns = C < MC ? C : MC,
				Rows	= R < MR ? R : MR,
			};

			_math_hidden_::ForColumns< Columns-1 >::template Step< Columns, Rows, T, D, mat_t, G >( m, data );
		}

		
		static void  Do(mat_t &m, const D &data)
		{
			_math_hidden_::ForColumns< MC-1 >::template Step< MC, MR, T, D, mat_t, G >( m, data );
		}
	};



	//
	// Matrix From Euler
	//

	namespace _math_hidden_
	{
		template <typename T>
		struct TSinCos
		{
			Vec<T,2>	x;
			Vec<T,2>	y;
			Vec<T,2>	z;
		};


		template <usize C, usize R>
		struct MatFromEuler_GetElem
		{
			template <typename T>	static T  Elem(const TSinCos<T> &d)	{ return T(0); }
		};
			
		template <>
		struct MatFromEuler_GetElem<0,0>
		{
			template <typename T>	static T  Elem(const TSinCos<T> &d)	{ return d.y[1] * d.z[1]; }
		};
		
		template <>
		struct MatFromEuler_GetElem<1,0>
		{
			template <typename T>	static T  Elem(const TSinCos<T> &d)	{ return -d.y[1] * d.z[0]; }
		};

		template <>
		struct MatFromEuler_GetElem<2,0>
		{
			template <typename T>	static T  Elem(const TSinCos<T> &d)	{ return d.y[0]; }
		};

		template <>
		struct MatFromEuler_GetElem<0,1>
		{
			template <typename T>	static T  Elem(const TSinCos<T> &d)	{ return d.x[1] * d.z[0] + d.x[0] * d.z[1] * d.y[0]; }
		};

		template <>
		struct MatFromEuler_GetElem<1,1>
		{
			template <typename T>	static T  Elem(const TSinCos<T> &d)	{ return d.x[1] * d.z[1] - d.x[0] * d.y[0] * d.z[0]; }
		};

		template <>
		struct MatFromEuler_GetElem<2,1>
		{
			template <typename T>	static T  Elem(const TSinCos<T> &d)	{ return -d.y[1] * d.x[0]; }
		};

		template <>
		struct MatFromEuler_GetElem<0,2>
		{
			template <typename T>	static T  Elem(const TSinCos<T> &d)	{ return d.x[0] * d.z[0] - d.x[1] * d.z[1] * d.y[1]; }
		};

		template <>
		struct MatFromEuler_GetElem<1,2>
		{
			template <typename T>	static T  Elem(const TSinCos<T> &d)	{ return d.z[1] * d.x[0] + d.x[1] * d.y[0] * d.z[0]; }
		};

		template <>
		struct MatFromEuler_GetElem<2,2>
		{
			template <typename T>	static T  Elem(const TSinCos<T> &d)	{ return d.x[1] * d.y[1]; }
		};

		template <>
		struct MatFromEuler_GetElem<3,3>
		{
			template <typename T>	static T  Elem(const TSinCos<T> &d)	{ return T(1); }
		};

	}	// _math_hiddden_


	struct MatFromEuler
	{
		template <usize nC, usize nR, typename T, usize C, usize R>
		static void  Get(T (&m)[C][R], const Vec<T,3> &a)
		{
			using namespace _math_hidden_;

			TSinCos<T>	data;

			data.x = SinCos( a.x );
			data.y = SinCos( a.y );
			data.z = SinCos( a.z );

			MatForEach< T, MatFromEuler_GetElem, TSinCos<T>, C, R >::template Do< nC, nR >( m, data );
		}

		
		template <usize nC, usize nR, typename T, usize C, usize R>
		static void  Get(T (&m)[C][R], const Vec<T,2> &a)
		{
			using namespace _math_hidden_;

			TSinCos<T>	data;

			data.x = SinCos( a.x );
			data.y = SinCos( a.y );
			data.z = Vec<T,2>( 0, 1 );
			
			MatForEach< T, MatFromEuler_GetElem, TSinCos<T>, C, R >::template Do< nC, nR >( m, data );
		}

		
		template <usize nC, usize nR, typename T, usize C, usize R>
		static void  Get(T (&m)[C][R], const Vec<T,1> &a)
		{
			using namespace _math_hidden_;

			TSinCos<T>	data;

			data.x = SinCos( a.x );
			data.y = Vec<T,2>( 0, 1 );
			data.z = Vec<T,2>( 0, 1 );
			
			MatForEach< T, MatFromEuler_GetElem, TSinCos<T>, C, R >::template Do< nC, nR >( m, data );
		}
	};



	//
	// Matrix Rotation X
	//

	namespace _math_hidden_
	{
		template <usize C, usize R>
		struct MatRotationX_GetElem
		{
			template <typename T> static T  Elem(const Vec<T,2> &)		{ return T(0); }
		};

		template <>
		struct MatRotationX_GetElem<0,0>
		{
			template <typename T> static T  Elem(const Vec<T,2> &)		{ return T(1); }
		};

		template <>
		struct MatRotationX_GetElem<3,3>
		{	
			template <typename T> static T  Elem(const Vec<T,2> &)		{ return T(1); }
		};

		template <>
		struct MatRotationX_GetElem<1,1>
		{	
			template <typename T> static T  Elem(const Vec<T,2> &sc)	{ return sc[1]; }
		};

		template <>
		struct MatRotationX_GetElem<2,1>
		{	
			template <typename T> static T  Elem(const Vec<T,2> &sc)	{ return -sc[0]; }
		};

		template <>
		struct MatRotationX_GetElem<1,2>
		{	
			template <typename T> static T  Elem(const Vec<T,2> &sc)	{ return sc[0]; }
		};

		template <>
		struct MatRotationX_GetElem<2,2>
		{	
			template <typename T> static T  Elem(const Vec<T,2> &sc)	{ return sc[1]; }
		};

	}	// _math_hidden_


	struct MatRotationX
	{
		template <usize nC, usize nR, typename T, usize C, usize R>
		static void  Get(T (&m)[C][R], const T &a)
		{
			using namespace _math_hidden_;

			const Vec<T,2>	x = SinCos( a );

			MatForEach< T, MatRotationX_GetElem, Vec<T,2>, C, R >::template Do< nC, nR >( m, x );
		}
	};



	//
	// Matrix Rotation Y
	//

	namespace _math_hidden_
	{
		template <usize C, usize R>
		struct MatRotationY_GetElem
		{
			template <typename T> static T  Elem(const Vec<T,2> &)		{ return T(0); }
		};
			
		template <>
		struct MatRotationY_GetElem<1,1>
		{
			template <typename T> static T  Elem(const Vec<T,2> &)		{ return T(1); }
		};
			
		template <>
		struct MatRotationY_GetElem<3,3>
		{	
			template <typename T> static T  Elem(const Vec<T,2> &)		{ return T(1); }
		};
			
		template <>
		struct MatRotationY_GetElem<0,0>
		{	
			template <typename T> static T  Elem(const Vec<T,2> &sc)	{ return sc[1]; }
		};
			
		template <>
		struct MatRotationY_GetElem<2,0>
		{	
			template <typename T> static T  Elem(const Vec<T,2> &sc)	{ return sc[0]; }
		};
			
		template <>
		struct MatRotationY_GetElem<0,2>
		{	
			template <typename T> static T  Elem(const Vec<T,2> &sc)	{ return -sc[0]; }
		};
			
		template <>
		struct MatRotationY_GetElem<2,2>
		{	
			template <typename T> static T  Elem(const Vec<T,2> &sc)	{ return sc[1]; }
		};


	}	// _math_hidden_

	struct MatRotationY
	{
		template <usize nC, usize nR, typename T, usize C, usize R>
		static void  Get(T (&m)[C][R], const T &a)
		{
			using namespace _math_hidden_;

			const Vec<T,2>	x = SinCos( a );

			MatForEach< T, MatRotationY_GetElem, Vec<T,2>, C, R >::template Do< nC, nR >( m, x );
		}
	};



	//
	// Matrix Rotation Z
	//

	namespace _math_hidden_
	{
		template <usize C, usize R>
		struct MatRotationZ_GetElem
		{
			template <typename T> static T  Elem(const Vec<T,2> &)		{ return T(0); }
		};
			
		template <>
		struct MatRotationZ_GetElem<2,2>
		{
			template <typename T> static T  Elem(const Vec<T,2> &)		{ return T(1); }
		};
			
		template <>
		struct MatRotationZ_GetElem<3,3>
		{
			template <typename T> static T  Elem(const Vec<T,2> &)		{ return T(1); }
		};
			
		template <>
		struct MatRotationZ_GetElem<0,0>
		{
			template <typename T> static T  Elem(const Vec<T,2> &sc)	{ return sc[1]; }
		};
			
		template <>
		struct MatRotationZ_GetElem<1,0>
		{	
			template <typename T> static T  Elem(const Vec<T,2> &sc)	{ return -sc[0]; }
		};
			
		template <>
		struct MatRotationZ_GetElem<1,1>
		{	
			template <typename T> static T  Elem(const Vec<T,2> &sc)	{ return sc[1]; }
		};
			
		template <>
		struct MatRotationZ_GetElem<0,1>
		{	
			template <typename T> static T  Elem(const Vec<T,2> &sc)	{ return sc[0]; }
		};

	}	// _math_hidden_


	struct MatRotationZ
	{
		template <usize nC, usize nR, typename T, usize C, usize R>
		static void  Get(T (&m)[C][R], const T &a)
		{
			using namespace _math_hidden_;

			const Vec<T,2>	x = SinCos( a );

			MatForEach< T, MatRotationZ_GetElem, Vec<T,2>, C, R >::template Do< nC, nR >( m, x );
		}
	};



	//
	// Matrix From Quaternion
	//

	namespace _math_hidden_
	{
		template <typename T>
		struct TQuatSqr
		{
			T	xx,	xy,	xz,	xw,
				yy,	yz,	yw,
				zz,	zw;
			///
			TQuatSqr(const Quaternion<T> &q)
			{
				xx = q.x * q.x;		xy = q.x * q.y;		xz = q.x * q.z;
				xw = q.x * q.w;		yy = q.y * q.y;		yz = q.y * q.z;
				yw = q.y * q.w;		zz = q.z * q.z;		zw = q.z * q.w;
			}
		};

		template <usize C, usize R>
		struct MatFromQuat_GetElem
		{
			template <typename T>	static T  Elem(const TQuatSqr<T> &)		{ return C == R ? T(1) : T(0); }
		};
			
		template <>
		struct MatFromQuat_GetElem<0,0>
		{
			template <typename T>	static T  Elem(const TQuatSqr<T> &d)	{ return T(1) - T(2) * (d.yy + d.zz); }
		};
			
		template <>
		struct MatFromQuat_GetElem<0,1>
		{
			template <typename T>	static T  Elem(const TQuatSqr<T> &d)	{ return T(2) * (d.xy + d.zw); }
		};
			
		template <>
		struct MatFromQuat_GetElem<0,2>
		{	
			template <typename T>	static T  Elem(const TQuatSqr<T> &d)	{ return T(2) * (d.xz - d.yw); }
		};
			
		template <>
		struct MatFromQuat_GetElem<1,0>
		{	
			template <typename T>	static T  Elem(const TQuatSqr<T> &d)	{ return T(2) * (d.xy - d.zw); }
		};
			
		template <>
		struct MatFromQuat_GetElem<1,1>
		{	
			template <typename T>	static T  Elem(const TQuatSqr<T> &d)	{ return T(1) - T(2) * (d.xx + d.zz); }
		};
			
		template <>
		struct MatFromQuat_GetElem<1,2>
		{	
			template <typename T>	static T  Elem(const TQuatSqr<T> &d)	{ return T(2) * (d.yz + d.xw); }
		};
			
		template <>
		struct MatFromQuat_GetElem<2,0>
		{	
			template <typename T>	static T  Elem(const TQuatSqr<T> &d)	{ return T(2) * (d.xz + d.yw); }
		};
			
		template <>
		struct MatFromQuat_GetElem<2,1>
		{	
			template <typename T>	static T  Elem(const TQuatSqr<T> &d)	{ return T(2) * (d.yz - d.xw); }
		};
			
		template <>
		struct MatFromQuat_GetElem<2,2>
		{	
			template <typename T>	static T  Elem(const TQuatSqr<T> &d)	{ return T(1) - T(2) * (d.xx + d.yy); }
		};

	}	// _math_hidden_


	struct MatFromQuat
	{
		template <usize nC, usize nR, typename T, usize C, usize R>
		static void  Get(T (&m)[C][R], const Quaternion<T> &q)
		{
			using namespace _math_hidden_;

			const TQuatSqr<T>	qs( q );

			MatForEach< T, MatFromQuat_GetElem, TQuatSqr<T>, C, R >::template Do< nC, nR >( m, qs );
		}
	};



	//
	// Matrix Identity
	//

	struct MatIdentity
	{
		struct TVoid {};


		template <usize C, usize R>
		struct GetElem
		{
			template <typename T>
			static T  Elem(const TVoid &)
			{
				return C == R ? T(1) : T(0);
			}
		};

		
		template <typename T, usize C, usize R>
		static void  Get(T (&m)[C][R])
		{
			TVoid	d;
			MatForEach< T, GetElem, TVoid, C, R >::template Do< C, R >( m, d );
		}
	};



	//
	// Matrix Scale
	//

	struct MatScale
	{
		template <usize C, usize R>
		struct GetElem
		{
			template <typename T, usize V>
			static T  Elem(const Vec<T,V> &s)
			{
				return C == R ? ( C < V ? s[C] : T(1) ) : T(0);
			}
		};

		
		template <typename T, usize C, usize R, usize V>
		static void  Get(T (&m)[C][R], const Vec<T,V> &s)
		{
			MatForEach< T, GetElem, Vec<T,V>, C, R >::template Do< C, R >( m, s );
		}
	};



	//
	// Matrix Translate
	//

	struct MatTranslate
	{
		template <typename T, usize V, usize MC>
		struct VecData
		{
			const Vec<T,V>	&	v;
			///
			VecData(const Vec<T,V> &t): v(t) {}
		};

		template <usize C, usize R>
		struct GetElem
		{
			template <typename T, usize V, usize MC>
			static T  Elem(const VecData<T,V,MC> &t)
			{
				return C == R ? T(1) : ( C == MC-1 ? t.v[R] : T(0) );
			};
		};

		
		template <typename T, usize C, usize R, usize V>
		static void  Get(T (&m)[C][R], const Vec<T,V> &t)
		{
			VecData<T,V,C>	data(t);
			MatForEach< T, GetElem, VecData<T,V,C>, C, R >::template Do< C, R >( m, data );
		}
	};



	//
	// Matrix Transpose
	//

	struct MatTranspose
	{
		template <usize C, usize R>
		struct GetElem
		{
			template <typename T, typename M>
			static T  Elem(const M &m)
			{
				return m[R][C];
			}
		};
		

		template <typename T, usize C, usize R>
		static void  Get(T (&t)[R][C], const T (&m)[C][R])
		{
			typedef T mat_t[C][R];

			MatForEach< T, GetElem, mat_t, R, C >::template Do< R, C >( t, m );
		}
	};
	
#	undef lmat_t
#	undef rmat_t
#	undef dmat_t


}	// GXMath
}	// GX_STL
