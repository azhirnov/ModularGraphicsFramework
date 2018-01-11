// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
		typedef typename VecType::Value_t	Value_t;
		typedef VecType						Vec_t;
		typedef Vec<Value_t,3>				Vec3_t;


	// variables
	private:
		StaticArray< Vec_t, 3 >		_points;


	// methods
	public:
		Triangle (GX_DEFCTOR)
		{}

		Triangle (const Vec_t &p0, const Vec_t &p1, const Vec_t &p2) :
			_points{ p0, p1, p2 }
		{}

		Vec_t const &	operator [] (uint i) const	{ return _points[i]; }
		Vec_t &			operator [] (uint i)		{ return _points[i]; }

		Vec_t			Center ()			const	{ return (_points[0] + _points[1] + _points[2]) / Value_t(3); }

		bool	InnerPoint (const Vec_t &p) const;
		Vec3_t	PointToBarycentric (const Vec_t &p) const;
		Vec_t	PointFromBarycentric (const Vec3_t &b) const;
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
			static bool Sign (const Vec_t &a, const Vec_t &b, const Vec_t &c) {
				return (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y);
			}
		};

		const bool b1 = Util::Sign( p, _points[0], _points[1] ) < Value_t(0);
		const bool b2 = Util::Sign( p, _points[1], _points[2] ) < Value_t(0);
		const bool b3 = Util::Sign( p, _points[2], _points[0] ) < Value_t(0);

		return ( (b1 == b2) and (b2 == b3) );
	}
	
/*
=================================================
	PointToBarycentric
=================================================
*/
	template <typename VecType>
	inline typename Triangle<VecType>::Vec3_t  Triangle<VecType>::PointToBarycentric (const Vec_t &p) const
	{
		Vec_t const		v0 = _points[1] - _points[0];
		Vec_t const		v1 = _points[2] - _points[0];
		Vec_t const		v2 = p - _points[0];

		Value_t const	d00 = Dot( v0, v0 );
		Value_t const	d01 = Dot( v0, v1 );
		Value_t const	d11 = Dot( v1, v1 );
		Value_t const	d20 = Dot( v2, v0 );
		Value_t const	d21 = Dot( v2, v1 );
		Value_t	const	denom = d00 * d11 - d01 * d01;

		Vec3_t	uvw;
		uvw[1] = (d11 * d20 - d01 * d21) / denom;
		uvw[2] = (d00 * d21 - d01 * d20) / denom;
		uvw[0] = Value_t(1) - uvw[1] - uvw[2];

		return uvw;
	}
	
/*
=================================================
	PointFromBarycentric
=================================================
*/
	template <typename VecType>
	inline VecType  Triangle<VecType>::PointFromBarycentric (const Vec3_t &b) const
	{
		return _points[0] * b.x + _points[1] * b.y + _points[2] * b.z;
	}


}	// GXMath

namespace GXTypes
{
	
	template <typename VecType>
	struct Hash< GXMath::Triangle<VecType> > :
		private Hash< VecType >
	{
		typedef GXMath::Triangle<VecType>		Key_t;
		typedef Hash< VecType >					Base_t;
		typedef typename Base_t::Result_t		Result_t;

		Result_t operator () (const Key_t &x) const noexcept
		{
			return	Base_t::operator ()( x[0] ) +
					Base_t::operator ()( x[1] ) +
					Base_t::operator ()( x[2] );
		}
	};

}	// GXTypes
}	// GX_STL
