// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "MathTypes3D.h"

namespace GX_STL
{
namespace GXMath
{


	//
	// Line
	//

	template <typename T>
	struct Line3 : public CompileTime::CopyQualifiers< T >
	{
	// types
	public:
		typedef Line3< T >		Self;
		typedef Vec<T,3>		vec_t;
		typedef T				value_t;


	// variables
	private:
		vec_t	_p0;
		vec_t	_p1;


	// methods
	public:
		Line3 (GX_DEFCTOR) {}

		explicit
		Line3 (const vec_t &end) : _p0(), _p1(end) {}

		Line3 (const vec_t &begin, const vec_t &end) : _p0(begin), _p1(end) {}

		bool	operator == (const Self &right) const;
		bool	operator != (const Self &right) const;

		vec_t &			Begin ()				{ return _p0; }
		vec_t const &	Begin ()		const	{ return _p0; }

		vec_t &			End ()					{ return _p1; }
		vec_t const &	End ()			const	{ return _p1; }

		T				Length ()		const	{ return _p1.Distance( _p0 ); }
		vec_t			Vector ()		const	{ return (_p1 - _p0); }
		vec_t			Direction ()	const	{ return Vector().Normalized(); }
		vec_t			Center ()		const	{ return (Begin() + End()) * T(0.5); }
		
		vec_t	GetInterpolationPoint (T value)	const	{ return Lerp( _p0, _p1, value ); }
		
		T		MinDistance (const vec_t &point) const;

		Self	Invert ()					const	{ return Self( _p1, _p0 ); }

		Self	Move (const vec_t &delta)	const	{ return Self( _p0 + delta, _p1 + delta ); }

		Self	Reflection (const vec_t &normal) const;
	};

	
/*
=================================================
	operator ==
=================================================
*/
	template <typename T>
	inline bool	Line3<T>::operator == (const Self &right) const
	{
		return All( Begin() == right.Begin() ) and All( End() == right.End() );
	}
	
/*
=================================================
	operator !=
=================================================
*/
	template <typename T>
	inline bool	Line3<T>::operator != (const Self &right) const
	{
		return not ( (*this) == right );
	}
	
/*
=================================================
	Reflection
=================================================
*/
	template <typename T>
	inline Line3<T>  Line3<T>::Reflection (const vec_t &normal) const
	{
		/*      this
			|  /
			| /
			|*---------* normal
			| \
			|  \
		        * result
		*/

		return Self( End(), End() + Vector().Reflect( normal ).SetLength( Length() ) );
	}
	
/*
=================================================
	MinDistance
=================================================
*/
	template <typename T>
	inline T  Line3<T>::MinDistance (const vec_t &point) const
	{
		// a, b, c
		const vec_t	d = End() - Begin();

		// (c*(p.y - b.y) - b*(p.z - b.z))^2 + (a*(p.z - b.z) - c*(p.x - b.x))^2 + (b*(p.x - b.x) - a*(p.y - b.y))^2
		const T		a = Square( d.z * (point.y - Begin().y) - d.y * (point.z - Begin().z) ) +
						Square( d.x * (point.z - Begin().z) - d.z * (point.x - Begin().x) ) +
						Square( d.y * (point.x - Begin().x) - d.x * (point.y - Begin().y) );

		// a^2 + b^2 + c^2
		const T		b = Dot( d, d );

		return Min( Sqrt( a / b ), Min( point.Distance( Begin() ), point.Distance( End() ) ) );
	}


}	// GXMath

namespace GXTypes
{
	
	template <typename T>
	struct Hash< GXMath::Line3<T> > :
		private Hash< typename GXMath::Line3<T>::vec_t >
	{
		typedef GXMath::Line3<T>							key_t;
		typedef Hash< typename GXMath::Line3<T>::vec_t >	base_t;
		typedef typename base_t::result_t					result_t;

		result_t operator () (const key_t &x) const
		{
			return	base_t::operator ()( x.Begin() ) +
					base_t::operator ()( x.End() );
		}
	};

}	// GXTypes
}	// GX_STL
