// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "MathTypes3D.h"

namespace GX_STL
{
namespace GXMath
{

	//
	// Triangle
	//

	template <typename VecType>
	struct Triangle : public CompileTime::CopyQualifiers< VecType >
	{
	// types
	public:
		typedef Triangle< VecType >			Self;
		typedef typename VecType::value_t	value_t;
		typedef VecType						vec_t;
		typedef Vec<value_t,3>				vec3_t;


	// variables
	private:
		vec_t	_p0, _p1, _p2;


	// methods
	public:
		Triangle (GX_DEFCTOR)
		{}

		Triangle (const vec_t &p0, const vec_t &p1, const vec_t &p2) :
			_p0(p0), _p1(p1), _p2(p2)
		{}

		vec_t const &	operator [] (uint i) const	{ ASSERT(i<3);  return ptr()[i]; }
		vec_t &			operator [] (uint i)		{ ASSERT(i<3);  return ptr()[i]; }

		vec_t			Center ()			const	{ return (_p0 + _p1 + _p2) / value_t(3); }

		bool	InnerPoint (const vec_t &p) const;
		vec3_t	PointToBarycentric (const vec_t &p) const;
		vec_t	PointFromBarycentric (const vec3_t &b) const;
	};
	

	
/*
=================================================
	InnerPoint
=================================================
*/
	template <typename VecType>
	inline bool  Triangle<VecType>::InnerPoint (const VecType &p) const
	{
		struct Util {
			static bool Sign (const vec_t &a, const vec_t &b, const vec_t &c) {
				return (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y);
			}
		};

		const bool b1 = Util::Sign( p, _p0, _p1 ) < value_t(0);
		const bool b2 = Util::Sign( p, _p1, _p2 ) < value_t(0);
		const bool b3 = Util::Sign( p, _p2, _p0 ) < value_t(0);

		return ( (b1 == b2) and (b2 == b3) );
	}
	
/*
=================================================
	PointToBarycentric
=================================================
*/
	template <typename VecType>
	inline typename Triangle<VecType>::vec3_t  Triangle<VecType>::PointToBarycentric (const vec_t &p) const
	{
		vec_t const		v0 = _p1 - _p0;
		vec_t const		v1 = _p2 - _p0;
		vec_t const		v2 = p - _p0;

		value_t const	d00 = Dot( v0, v0 );
		value_t const	d01 = Dot( v0, v1 );
		value_t const	d11 = Dot( v1, v1 );
		value_t const	d20 = Dot( v2, v0 );
		value_t const	d21 = Dot( v2, v1 );
		value_t	const	denom = d00 * d11 - d01 * d01;

		vec3_t	uvw;
		uvw[1] = (d11 * d20 - d01 * d21) / denom;
		uvw[2] = (d00 * d21 - d01 * d20) / denom;
		uvw[0] = value_t(1) - uvw[1] - uvw[2];

		return uvw;
	}
	
/*
=================================================
	PointFromBarycentric
=================================================
*/
	template <typename VecType>
	inline VecType  Triangle<VecType>::PointFromBarycentric (const vec3_t &b) const
	{
		return _p0 * b.x + _p1 * b.y + _p2 * b.z;
	}


}	// GXMath

namespace GXTypes
{
	
	template <typename VecType>
	struct Hash< GXMath::Triangle<VecType> > :
		private Hash< VecType >
	{
		typedef GXMath::Triangle<VecType>		key_t;
		typedef Hash< VecType >					base_t;
		typedef typename base_t::result_t		result_t;

		result_t operator () (const key_t &x) const
		{
			return	base_t::operator ()( x[0] ) +
					base_t::operator ()( x[1] ) +
					base_t::operator ()( x[2] );
		}
	};

}	// GXTypes
}	// GX_STL
