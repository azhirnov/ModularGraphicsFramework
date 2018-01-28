// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
		typedef Vec<T,3>		Vec_t;
		typedef T				Value_t;


	// variables
	private:
		Vec_t	_p0;
		Vec_t	_p1;


	// methods
	public:
		Line3 (GX_DEFCTOR) {}

		explicit
		Line3 (const Vec_t &end) : _p0(), _p1(end) {}

		Line3 (const Vec_t &begin, const Vec_t &end) : _p0(begin), _p1(end) {}

		bool	operator == (const Self &right) const;
		bool	operator != (const Self &right) const;

		Vec_t &			Begin ()				{ return _p0; }
		Vec_t const &	Begin ()		const	{ return _p0; }

		Vec_t &			End ()					{ return _p1; }
		Vec_t const &	End ()			const	{ return _p1; }

		T				Length ()		const	{ return _p1.Distance( _p0 ); }
		Vec_t			Vector ()		const	{ return (_p1 - _p0); }
		Vec_t			Direction ()	const	{ return Vector().Normalized(); }
		Vec_t			Center ()		const	{ return (Begin() + End()) * T(0.5); }
		
		Vec_t	GetInterpolationPoint (T value)	const	{ return Lerp( _p0, _p1, value ); }
		
		T		MinDistance (const Vec_t &point) const;

		Self	Invert ()					const	{ return Self( _p1, _p0 ); }

		Self	Move (const Vec_t &delta)	const	{ return Self( _p0 + delta, _p1 + delta ); }

		Self	Reflection (const Vec_t &normal) const;
	};

	
/*
=================================================
	operator ==
=================================================
*/
	template <typename T>
	inline CHECKRES(bool)  Line3<T>::operator == (const Self &right) const
	{
		return All( Begin() == right.Begin() ) and All( End() == right.End() );
	}
	
/*
=================================================
	operator !=
=================================================
*/
	template <typename T>
	inline CHECKRES(bool)  Line3<T>::operator != (const Self &right) const
	{
		return not ( (*this) == right );
	}
	
/*
=================================================
	Reflection
=================================================
*/
	template <typename T>
	inline CHECKRES(Line3<T>)  Line3<T>::Reflection (const Vec_t &normal) const
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
	inline CHECKRES(T)  Line3<T>::MinDistance (const Vec_t &point) const
	{
		// a, b, c
		const Vec_t	d = End() - Begin();

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
		private Hash< typename GXMath::Line3<T>::Vec_t >
	{
		typedef GXMath::Line3<T>							Key_t;
		typedef Hash< typename GXMath::Line3<T>::Vec_t >	Base_t;
		typedef typename Base_t::Result_t					Result_t;

		CHECKRES(Result_t)  operator () (const Key_t &x) const noexcept
		{
			return	Base_t::operator ()( x.Begin() ) +
					Base_t::operator ()( x.End() );
		}
	};

}	// GXTypes
}	// GX_STL
