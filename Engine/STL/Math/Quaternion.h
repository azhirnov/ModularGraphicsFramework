// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.
/*
	based on code from OpenGL Mathematics (GLM) from http://glm.g-truc.net/
*/

#pragma once

#include "Vec.h"
#include "Matrix.h"
#include "Trigonometry.h"

namespace GX_STL
{
namespace GXMath
{
	
#	define _QUAT_OPERATOR1( _op_, _right_, _func_ ) \
		Self &	operator _op_##= (const _right_& right)			_func_ \
		Self	operator _op_    (const _right_& right) const	{ return Self(*this) _op_##= right; }

#	define _QUAT_OPERATOR2( _op_ ) \
		_QUAT_OPERATOR1( _op_, T, { x _op_##= right;  y _op_##= right;  z _op_##= right;  w _op_##= right;  return *this; } )

#	define _QUAT_OPERATOR3( _op_ ) \
		_QUAT_OPERATOR1( _op_, Self, { x _op_##= right.x;  y _op_##= right.y;  z _op_##= right.z;  w _op_##= right.w;  return *this; } )

#	define _QUAT_OPERATOR( _op_ ) \
		_QUAT_OPERATOR2( _op_ ); \
		_QUAT_OPERATOR3( _op_ )



	//
	// Quaternion
	//

	template <typename T>
	struct Quaternion : public CompileTime::CopyQualifiers< T >
	{
		// types
		typedef Quaternion<T>		Self;
		typedef T					value_t;
		typedef Vec<T,3>			vec3_t;
		typedef Matrix<T,3,3>		mat3_t;
		typedef Matrix<T,4,4>		mat4_t;
		typedef bool				_is_quaternion;


		// variables
		T	x, y, z, w;
		

		// constructors
		Quaternion (GX_DEFCTOR): x(0), y(0), z(0), w(1) {}
		Quaternion (const mat3_t &matrix);
		Quaternion (const mat4_t &matrix);
		Quaternion (T x, T y, T z, T w): x(x), y(y), z(z), w(w) {}
		Quaternion (const vec3_t &vec, T w): x(vec.x), y(vec.y), z(vec.z), w(w) {}

		// unary operators
		Self		operator - () const;
		bool		operator ! () const;

		// binary operators
		T		&	operator [] (usize index);
		const T &	operator [] (usize index) const;

		bool		operator == (const Self &right)	const;
		bool		operator != (const Self &right)	const;

		Self		operator *  (const Self &right)	const;
		vec3_t		operator *  (const vec3_t &right) const;
		Self &		operator *= (const Self &right);
		
		_QUAT_OPERATOR( +  );
		_QUAT_OPERATOR( -  );
		_QUAT_OPERATOR2( * );
		_QUAT_OPERATOR( /  );
		_QUAT_OPERATOR( %  );
		_QUAT_OPERATOR( &  );
		_QUAT_OPERATOR( |  );
		_QUAT_OPERATOR( ^  );
		_QUAT_OPERATOR( >> );
		_QUAT_OPERATOR( << );

		
		// methods
		T		*	 ptr ()							{ return PointerCast< T >( this ); }
		const T	*	 ptr () const					{ return PointerCast< T >( this ); }

		static usize Count ()						{ return 4; }

		Self &	Set (Radians<T> x, Radians<T> y, Radians<T> z);
		Self &	Set (const RadiansVec<T,3> &angle);
		Self &	Set (const DegreesVec<T,3> &angle);
		
		T		Dot (const Self &right) const;

		bool	IsZero () const;

		Self	Normalized () const;
		Self &	Normalize ();
		
		vec3_t	GetDirection () const;

		Self &	Inverse ();
		Self	Inversed () const;

		Self &	FromAngleAxis (Radians<T> angle, const vec3_t &axis);

		void	ToAngleAxis (OUT Radians<T> &angle, OUT vec3_t &axis) const;
		void	ToEuler (OUT RadiansVec<T,3> &euler) const;

		//Self & FromSpherical (const Vec<T,2> &sAngles);
		//void	ToSpherical (Vec<T,2> &sAngles) const;

		Self &	MakeIdentity ();
		Self &	RotationFromTo (const vec3_t &fromDir, const vec3_t &toDir);

		Self &	RotationX (Radians<T> angle);	// pitch
		Self &	RotationY (Radians<T> angle);	// yaw
		Self &	RotationZ (Radians<T> angle);	// roll

		Self &	RotationX (Degrees<T> angle)		{ return RotationX( angle.ToRadians() ); }
		Self &	RotationY (Degrees<T> angle)		{ return RotationY( angle.ToRadians() ); }
		Self &	RotationZ (Degrees<T> angle)		{ return RotationZ( angle.ToRadians() ); }

		Self &	Rotation (const RadiansVec<T,3> &angle);
		Self &	Rotation (const DegreesVec<T,3> &angle);

		template <typename T2>
		const Quaternion<T2> Convert () const;

		template <typename B>
		const B	To () const;

		Vec<T,4>	xyzw () const		{ return Vec<T,4>( x, y, z, w ); }
		Vec<T,2>	xy ()	const		{ return Vec<T,2>( x, y ); }
		Vec<T,3>	xyz ()	const		{ return Vec<T,3>( x, y, z ); }
	};

	

//--------------------------- Math Func ---------------------------//
	
	template <typename T>
	inline bool IsZero (const Quaternion<T>& val)
	{
		return val.IsZero();
	}



//----------------------------- Quaternion -----------------------//
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	inline Quaternion<T>::Quaternion (const mat3_t &matrix): x(0), y(0), z(0), w(1)
	{
		T			s		= T(0);
		const T		trace	= right(0,0) + right(1,1) + right(2,2);
		Self &		q		= *this;

		if ( trace > T(0) )
		{
			s = Sqrt( trace + T(1) );
			w = s * T(0.5);
			s = T(0.5) / s;

			x = ( right(1,2) - right(2,1) ) * s;
			y = ( right(2,0) - right(0,2) ) * s;
			z = ( right(0,1) - right(1,0) ) * s;
		}
		else
		{
			const int	nxt[3] = {1,2,0};
			int			i = 0,
						j = 0,
						k = 0;

			if ( right(1,1) > right(0,0) )
				i = 1;

			if ( right(2,2) > right(1,1) )
				i = 2;

			j = nxt[i];
			k = nxt[j];
			s = Sqrt( ( right(i,i) - ( right(j,j) + right(k,k) ) ) + T(1) );

			q[i] = s * T(0.5);
			s = T(0.5) / s;

			q[3] = ( right(j,k) - right(k,j) ) * s;
			q[j] = ( right(i,j) + right(j,i) ) * s;
			q[k] = ( right(i,k) + right(k,i) ) * s;
		}
		Normalize();
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	inline Quaternion<T>::Quaternion (const mat4_t &matrix): x(0), y(0), z(0), w(1)
	{
		T			s		= T(0);
		const T		trace	= right(0,0) + right(1,1) + right(2,2);
		Self &		q		= *this;

		if ( trace > T(0) )
		{
			s = Sqrt( trace + T(1) );
			w = s * T(0.5);
			s = T(0.5) / s;

			x = ( right(1,2) - right(2,1) ) * s;
			y = ( right(2,0) - right(0,2) ) * s;
			z = ( right(0,1) - right(1,0) ) * s;
		}
		else
		{
			const int	nxt[3] = { 1, 2, 0 };
			int			i = 0,
						j = 0,
						k = 0;

			if ( right(1,1) > right(0,0) )
				i = 1;

			if ( right(2,2) > right(1,1) )
				i = 2;

			j = nxt[i];
			k = nxt[j];
			s = Sqrt( ( right(i,i) - ( right(j,j) + right(k,k) ) ) + T(1) );

			q[i] = s * T(0.5);
			s = T(0.5) / s;

			q[3] = ( right(j,k) - right(k,j) ) * s;
			q[j] = ( right(i,j) + right(j,i) ) * s;
			q[k] = ( right(i,k) + right(k,i) ) * s;
		}
		Normalize();
	}
	
/*
=================================================
	operator []
=================================================
*/
	template <typename T>
	inline T & Quaternion<T>::operator [] (usize index)
	{
		ASSUME( index < 4 );
		return ptr()[index];
	}
	
/*
=================================================
	operator []
=================================================
*/
	template <typename T>
	inline const T & Quaternion<T>::operator [] (usize index) const
	{
		ASSUME( index < 4 );
		return ptr()[index];
	}
	
/*
=================================================
	operator ==
=================================================
*/
	template <typename T>
	inline bool Quaternion<T>::operator == (const Self &right) const
	{
		return ( Equals( x, right.x ) and Equals( y, right.y ) and
				 Equals( z, right.z ) and Equals( w, right.w ) );
	}
	
/*
=================================================
	operator !=
=================================================
*/
	template <typename T>
	inline bool Quaternion<T>::operator != (const Self &right) const
	{
		return not ( *this == right );
	}

/*
=================================================
	operator !
=================================================
*/
	template <typename T>
	inline bool Quaternion<T>::operator ! () const
	{
		// TODO: WTF?
		return not x and not y and not z and not w;
	}

/*
=================================================
	operator *
=================================================
*/
	template <typename T>
	inline Quaternion<T> Quaternion<T>::operator * (const Self &right) const
	{
		return Self(*this) *= right;
	}

/*
=================================================
	operator *
=================================================
*/
	template <typename T>
	inline Vec<T,3> Quaternion<T>::operator * (const vec3_t &right) const
	{
		vec3_t const	qvec(x, y, z);
		vec3_t const	uv  = Cross( qvec, right );
		vec3_t const	uuv = Cross( qvec, uv );

		return right + ((uv * w) + uuv) * T(2);
	}

/*
=================================================
	operator *=
=================================================
*/
	template <typename T>
	inline Quaternion<T> & Quaternion<T>::operator *= (const Self &right)
	{
		Self const	p(*this);
		Self const&	q = right;

		this->w = p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z;
		this->x = p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y;
		this->y = p.w * q.y + p.y * q.w + p.z * q.x - p.x * q.z;
		this->z = p.w * q.z + p.z * q.w + p.x * q.y - p.y * q.x;
		return *this;
	}

/*
=================================================
	Dot
=================================================
*/
	template <typename T>
	inline T Quaternion<T>::Dot (const Self &right) const
	{
		return (x * right.x) + (y * right.y) + (z * right.z) + (w * right.w);
	}
	
/*
=================================================
	Set
=================================================
*/
	template <typename T>
	inline Quaternion<T> &  Quaternion<T>::Set (Radians<T> xAngle, Radians<T> yAngle, Radians<T> zAngle)
	{
		const Vec<T,2>	scr = SinCos( xAngle * T(0.5) );
		const Vec<T,2>	scp = SinCos( yAngle * T(0.5) );
		const Vec<T,2>	scy = SinCos( zAngle * T(0.5) );

		const T	cpcy = scp[1] * scy[1];
		const T	spcy = scp[0] * scy[1];
		const T	cpsy = scp[1] * scy[0];
		const T	spsy = scp[0] * scy[0];
		
		x = scr[0] * cpcy - scr[1] * spsy;
		y = scr[1] * spcy + scr[0] * cpsy;
		z = scr[1] * cpsy - scr[0] * spcy;
		w = scr[1] * cpcy + scr[0] * spsy;

		return Normalize();
	}
	
	template <typename T>
	inline Quaternion<T> &  Quaternion<T>::Set (const RadiansVec<T,3> &angle)
	{
		return Set( angle.x, angle.y, angle.z );
	}
	

	template <typename T>
	inline Quaternion<T> &	Quaternion<T>::Set (const DegreesVec<T,3> &angle)
	{
		return Set( angle.To< RadiansVec<T,3> >() );
	}

/*
=================================================
	IsZero
=================================================
*/
	template <typename T>
	inline bool Quaternion<T>::IsZero () const
	{
		return ( GXMath::IsZero(x) and GXMath::IsZero(y) and GXMath::IsZero(z) and GXMath::Equals( w, T(1) ) );
	}

/*
=================================================
	Normalize
=================================================
*/
	template <typename T>
	inline Quaternion<T> & Quaternion<T>::Normalize ()
	{
		const T	n = x*x + y*y + z*z + w*w;

		if ( n == T(1) )
			return *this;

		return ( *this *= T(1) / Sqrt(n) );
	}

	template <typename T>
	inline Quaternion<T> Quaternion<T>::Normalized () const
	{
		return Self(*this).Normalize();
	}
	
/*
=================================================
	GetDirection
=================================================
*/
	template <typename T>
	inline Vec<T,3> Quaternion<T>::GetDirection () const
	{
		return vec3_t(	T(2) * x * z + T(2) * y * w,
						T(2) * z * y - T(2) * x * w,
						T(1) - T(2) * x * x - T(2) * y * y );
	}
	
/*
=================================================
	Inverse
=================================================
*/
	template <typename T>
	inline Quaternion<T> & Quaternion<T>::Inverse ()
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}
	
	template <typename T>
	inline Quaternion<T> Quaternion<T>::Inversed () const
	{
		return Self( *this ).Inverse();
	}

	template <typename T>
	inline Quaternion<T> Quaternion<T>::operator - () const
	{
		return Inversed();
	}

/*
=================================================
	Slerp
=================================================
*/
	template <typename T>
	inline Quaternion<T>  Slerp (const Quaternion<T> &qx, const Quaternion<T> &qy, T factor)
	{
		Quaternion<T>	qz			= qy;
		T				cos_theta	= Dot( qx, qy );

		if ( cos_theta < T(0) )
		{
			qz			= -qy;
			cos_theta	= -cos_theta;
		}

		if ( cos_theta > T(1) - Epsilon<T>() )
		{
			return Quaternion<T>( Lerp( qx.x, qy.x, factor ),
								  Lerp( qx.y, qy.y, factor ),
								  Lerp( qx.z, qy.z, factor ),
								  Lerp( qx.w, qy.w, factor ) );
		}
		else
		{
			T	angle = ACos( cos_theta );

			return	( Sin( (T(1) - factor) * angle ) * qx +
					  Sin( factor * angle ) * qz ) / Sin( angle );
		}
	}
	
/*
=================================================
	FromAngleAxis
=================================================
*/
	template <typename T>
	inline Quaternion<T> & Quaternion<T>::FromAngleAxis (Radians<T> angle, const vec3_t &axis)
	{
		const T	half_angle	= T(0.5) * angle;
		const T	sin			= Sin( half_angle );

		x = sin * axis.x;
		y = sin * axis.y;
		z = sin * axis.z;
		w = Cos( half_angle );

		return *this;
	}
	
/*
=================================================
	ToAngleAxis
=================================================
*/
	template <typename T>
	inline void Quaternion<T>::ToAngleAxis (OUT Radians<T> &angle, OUT vec3_t &axis) const
	{
		const T scale = Sqrt( x*x + y*y + z*z );

		if ( GXMath::IsZero( scale ) or w > T(1) or w < T(-1) )
		{
			angle  = T(0);
			axis.x = T(0);
			axis.y = T(1);
			axis.z = T(0);
		}
		else
		{
			const T invscale = T(1) / scale;

			angle  = T(2) * ACos( w );
			axis.x = x * invscale;
			axis.y = y * invscale;
			axis.z = z * invscale;
		}
	}
	
/*
=================================================
	ToEuler
=================================================
*/
	template <typename T>
	inline void Quaternion<T>::ToEuler (OUT RadiansVec<T,3> &euler) const
	{
		const T	sqw = w*w;
		const T	sqx = x*x;
		const T	sqy = y*y;
		const T	sqz = z*z;

		euler.x = Radians<T>( ATan( T(2) * (y*z +x*w), -sqx - sqy + sqz + sqw ) );
		euler.y = Radians<T>( ASin( Clamp( T(-2) * (x*z - y*w), T(-1), T(1) ) ) );
		euler.z = Radians<T>( ATan( T(2) * (x*y +z*w), sqx - sqy - sqz + sqw ) );
	}
	
/*
=================================================
	MakeIdentity
=================================================
*/
	template <typename T>
	inline Quaternion<T> & Quaternion<T>::MakeIdentity ()
	{
		w = T(1);
		x = T(0);
		y = T(0);
		z = T(0);
		return *this;
	}
	
/*
=================================================
	RotationFromTo
=================================================
*/
	template <typename T>
	inline Quaternion<T> & Quaternion<T>::RotationFromTo (const vec3_t &fromDir, const vec3_t &toDir)
	{
		const vec3_t	v0 = fromDir.Normalized();
		const vec3_t	v1 = toDir.Normalized();
		const T			d  = Dot( v0, v1 );

		if ( d >= T(1) )
		{
			return MakeIdentity();
		}
		else
		if ( d <= T(-1) )
		{
			vec3_t	axis( T(1), T(0), T(0) );

			axis = Cross( axis, vec3_t( x, y, z ) );

			if ( IsZero( axis.Length() ) )
			{
				axis.Set( Radians<T>(0), Radians<T>(1), Radians<T>(0) );
				axis.Cross( vec3_t( x, y, z ) );
			}
			return FromAngleAxis( Pi<T>, axis );
		}

		const T			s	 = Sqrt( (1 + d) * T(2) );
		const T			invs = T(1) / s;
		const vec3_t	c	 = v0.Cross(v1) * invs;
		
		x = c.x;
		y = c.y;
		z = c.z;
		w = s * T(0.5);

		return *this;
	}
	
/*
=================================================
	RotationX
=================================================
*/
	template <typename T>
	inline Quaternion<T> & Quaternion<T>::RotationX (Radians<T> angle)
	{
		const Radians<T>	half_angle = angle * T(0.5);

		x = Sin( half_angle );
		y = T(0);
		z = T(0);
		w = Cos( half_angle );

		return *this;
	}
	
/*
=================================================
	RotationY
=================================================
*/
	template <typename T>
	inline Quaternion<T> & Quaternion<T>::RotationY (Radians<T> angle)
	{
		const Radians<T>	half_angle = angle * T(0.5);

		x = T(0);
		y = Sin( half_angle );
		z = T(0);
		w = Cos( half_angle );

		return *this;
	}
	
/*
=================================================
	RotationZ
=================================================
*/
	template <typename T>
	inline Quaternion<T> & Quaternion<T>::RotationZ (Radians<T> angle)
	{
		const Radians<T>	half_angle = angle * T(0.5);

		x = T(0);
		y = T(0);
		z = Sin( half_angle );
		w = Cos( half_angle );

		return *this;
	}
	
/*
=================================================
	Rotation
=================================================
*/
	template <typename T>
	inline Quaternion<T> & Quaternion<T>::Rotation (const RadiansVec<T,3> &angle)
	{
		*this = Self().RotationX( angle.x ) * Self().RotationY( angle.y ) * Self().RotationZ( angle.z );
		return *this;
	}
	
	template <typename T>
	inline Quaternion<T> & Quaternion<T>::Rotation (const DegreesVec<T,3> &angle)
	{
		return Rotation( angle.To< RadiansVec<T,3> >() );
	}

/*
=================================================
	Convert
=================================================
*/
	template <typename T>
	template <typename T2>
	inline const Quaternion<T2>  Quaternion<T>::Convert () const
	{
		return Quaternion<T2>( (T2)x, (T2)y, (T2)z, (T2)w );
	}
	
/*
=================================================
	To
=================================================
*/
	template <typename T>
	template <typename B>
	inline const B  Quaternion<T>::To () const
	{
		STATIC_ASSERT( B::_is_quaternion(true), "type is not quaternion" );

		return Convert< B::value_t >();
	}


#	undef _QUAT_OPERATOR
#	undef _QUAT_OPERATOR1
#	undef _QUAT_OPERATOR2
#	undef _QUAT_OPERATOR3


}	// GXMath

namespace GXTypes
{
	
	template <typename T>
	struct Hash< GXMath::Quaternion<T> > :
		private Hash< GXMath::Vec<T,4> >
	{
		typedef GXMath::Quaternion<T>		key_t;
		typedef Hash< GXMath::Vec<T,4> >	base_t;
		typedef typename base_t::result_t	result_t;

		result_t operator () (const key_t &x) const
		{
			return base_t::operator ()( x.xyzw() );
		}
	};

}	// GXTypes
}	// GX_STL
