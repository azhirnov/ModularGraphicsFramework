// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Rectangle.h"

namespace GX_STL
{
namespace GXMath
{


	//
	// Line
	//

	template <typename T>
	struct Line2 : public CompileTime::CopyQualifiers< T >
	{
	// types
	public:
		typedef Line2< T >		Self;
		typedef Vec<T,2>		vec_t;
		typedef T				value_t;


	// variables
	private:
		vec_t	_p0;
		vec_t	_p1;


	// methods
	public:
		Line2 (GX_DEFCTOR) {}

		explicit
		Line2 (const vec_t &end) : _p0(), _p1(end) {}

		Line2 (const vec_t &begin, const vec_t &end) : _p0(begin), _p1(end) {}

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

		Self Invert ()					const	{ return Self( _p1, _p0 ); }

		Self Move (const vec_t &delta)	const	{ return Self( _p0 + delta, _p1 + delta ); }

		void GetEquation (OUT T &k, OUT T &b) const;
		void GetEquation (OUT T &a, OUT T &b, OUT T &c) const;
		void GetBoundingRect (OUT Rectangle<T> &boundRect) const;

		T	GetXfromY (T y) const;
		T	GetYfromX (T x) const;

		vec_t	GetFromY (T y) const;
		vec_t	GetFromX (T x) const;

		bool IsInnerPoint (const vec_t &point) const;
		bool IsInnerLine (const Self &line) const;

		bool IntersectLine (const Self &other) const;
		bool IntersectRect (const Rectangle<T> &other) const;

		bool GetVecIntersection (const Self &other, OUT vec_t &result) const;
		bool GetLineIntersection (const Self &other, OUT vec_t &result) const;
		bool GetRectIntersection (const Rectangle<T> &other, OUT vec_t &result0, OUT vec_t &result1) const;

		bool GetRayIntersection (const Self &other, OUT vec_t &res) const;

		bool IsPointInBoundingRects (const Self &other, const vec_t &point) const;

		Radians<T>	AngleBetweenLines (const Self &other) const;	// angle of other line rotation by this line in a clockwise 
		T			CosBetweenLines (const Self &other) const;
		Self		Reflection (const vec_t &normal) const;

		bool	PerpendicularBase (const vec_t &point, OUT vec_t &base)	const;
		bool	Perpendicular (const vec_t &point, OUT Self &result) const;
		T		MinDistance (const vec_t &point) const;
		
		vec_t	Normal () const;
		vec_t	LeftNormal () const;
		vec_t	RightNormal () const;
		vec_t	GetInterpolationPoint (T value)	const	{ return Lerp( _p0, _p1, value ); }


		static bool _CropLine (const Self &src, Self &dst);

	private:
		bool _IntersectRect (const Rectangle<T> &other, OUT vec_t &res0, OUT vec_t &res1) const;
	};


	
	
		
/*
=================================================
	operator ==
=================================================
*/
	template <typename T>
	inline bool	Line2<T>::operator == (const Self &right) const
	{
		return All( Begin() == right.Begin() ) and All( End() == right.End() );
	}
	
	template <typename T>
	inline bool	Line2<T>::operator != (const Self &right) const
	{
		return not ( (*this) == right );
	}
	
/*
=================================================
	GetEquation
=================================================
*/
	template <typename T>
	inline void Line2<T>::GetEquation (OUT T &outK, OUT T &outB) const
	{
		// y = Kx + B

		T	a, b, c;
		GetEquation( a, b, c );
		
		if ( IsNotZero( b ) )
		{
			outK = -a / b;
			outB = -c / b;
		}
		else
		{
			outK = 0;
			outB = 0;
			WARNING( "!!!" );
		}
	}
		
/*
=================================================
	GetEquation
=================================================
*/
	template <typename T>
	inline void Line2<T>::GetEquation (OUT T &a, OUT T &b, OUT T &c) const
	{
		// Ax + By + C = 0

		a = Begin().y - End().y;
		b = End().x - Begin().x;
		c = Begin().x * End().y - End().x * Begin().y;
	}
	
/*
=================================================
	GetXfromY
=================================================
*/
	template <typename T>
	inline T Line2<T>::GetXfromY (T y) const
	{
		// ax + by + c = 0 -> x = -(by + c) / a

		T	a, b, c;
		GetEquation( a, b, c );
		return SafeDiv( - (b * y + c ), a, T(0) );
	}
	
/*
=================================================
	GetYfromX
=================================================
*/
	template <typename T>
	inline T  Line2<T>::GetYfromX (T x) const
	{
		// y = kx + b
		T	k, b;
		GetEquation( k, b );
		return k * x + b;
	}
	
/*
=================================================
	GetFromY
=================================================
*/
	template <typename T>
	inline typename Line2<T>::vec_t  Line2<T>::GetFromY (T y) const
	{
		return vec_t( GetXfromY( y ), y );
	}
	
/*
=================================================
	GetFromX
=================================================
*/
	template <typename T>
	inline typename Line2<T>::vec_t  Line2<T>::GetFromX (T x) const
	{
		return vec_t( x, GetYfromX( x ) );
	}
		
/*
=================================================
	PerpendicularBase
=================================================
*/
	template <typename T>
	inline bool Line2<T>::PerpendicularBase (const vec_t &p, OUT vec_t &base) const
	{
		vec_t	vec		= End() - Begin();
		vec_t	vecSqr	= vec * vec;

		base.x = ( Begin().x * vecSqr.y + p.x * vecSqr.x + vec.x * vec.y * (p.y - Begin().y) ) / (vecSqr.y + vecSqr.x);
		base.y = vec.x * (p.x - base.x) / vec.y + p.y;

		return IsInnerPoint( base );
	}
	
/*
=================================================
	Perpendicular
=================================================
*/
	template <typename T>
	inline bool Line2<T>::Perpendicular (const vec_t &point, OUT Self &result) const
	{
		vec_t	base;

		if ( not PerpendicularBase( point, base ) )
		{
			result = Self();
			return false;
		}

		result = Self( point, base );
		return true;
	}
	
/*
=================================================
	MinDistance
=================================================
*/
	template <typename T>
	inline T  Line2<T>::MinDistance (const vec_t &point) const
	{
		T	a, b, c;
		GetEquation( a, b, c );

		T	dist = ( a * point.x + b * point.y + c ) / Sqrt( a*a + b*b );

		return Min( dist, Min( point.Distance( Begin() ), point.Distance( End() ) ) );
	}
	
/*
=================================================
	Normal
=================================================
*/
	template <typename T>
	inline Vec<T,2>  Line2<T>::Normal () const
	{
		const vec_t	v = Vector();
		return ( v.x >= T(0) ? vec_t( -v.y, v.x ) : vec_t( v.y, -v.x ) ).Normalized();
	}
	
/*
=================================================
	LeftNormal
=================================================
*/
	template <typename T>
	inline Vec<T,2>  Line2<T>::LeftNormal () const
	{
		const vec_t	v = Vector();
		return vec_t( -v.y, v.x ).Normalized();
	}
	
/*
=================================================
	RightNormal
=================================================
*/
	template <typename T>
	inline Vec<T,2>  Line2<T>::RightNormal () const
	{
		const vec_t	v = Vector();
		return vec_t( v.y, -v.x ).Normalized();
	}
	
/*
=================================================
	GetBoundingRect
=================================================
*/
	template <typename T>
	inline void Line2<T>::GetBoundingRect (Rectangle<T> &boundRect) const
	{
		boundRect = Rectangle<T>( StaticArrayBuilder::Create( Begin(), End() ) );
		boundRect.Stretch( Vector().Max() /* T(0.01)*/ );
	}
	
/*
=================================================
	IsInnerPoint
=================================================
*/
	template <typename T>
	inline bool Line2<T>::IsInnerPoint (const vec_t &point) const
	{
		Rectangle<T>	r;
		GetBoundingRect( r );
	
		if ( not r.IsInnerPoint( point ) )
			return false;

		T	a, b, c;
		GetEquation( a, b, c );
		return IsZero( a * point.x + b * point.y + c );
	}
	
/*
=================================================
	IsInnerLine
=================================================
*/
	template <typename T>
	inline bool Line2<T>::IsInnerLine (const Self &line) const
	{
		Rectangle<T>	r;
		GetBoundingRect( r );
	
		if ( not r.IsInnerPoint( line.Begin() ) or
			 not r.IsInnerPoint( line.End() ) )
		{
			return false;
		}
		
		T	a, b, c;
		GetEquation( a, b, c );

		return	IsZero( a * line.Begin().x + b * line.Begin().y + c ) and
				isZero(	a * line.End().x + b * line.End().y + c );
	}
	
/*
=================================================
	IntersectLine
=================================================
*/
	template <typename T>
	inline bool Line2<T>::IntersectLine (const Self &other) const
	{
		vec_t	res;
		return GetLineIntersection( other, res );
	}
	
/*
=================================================
	IntersectRect
=================================================
*/
	template <typename T>
	inline bool Line2<T>::IntersectRect (const Rectangle<T> &other) const
	{
		vec_t	res0, res1;
		return _IntersectRect( other, res0, res1 );
	}
	
/*
=================================================
	IsPointInBoundingRects
=================================================
*/
	template <typename T>
	inline bool Line2<T>::IsPointInBoundingRects (const Self &other, const vec_t &point) const
	{
		Rectangle<T>	r0;
		Rectangle<T>	r1;
		this->GetBoundingRect( r0 );
		other.GetBoundingRect( r1 );
		return r0.IsInnerPoint( point ) and r1.IsInnerPoint( point );
	}
	
/*
=================================================
	GetVecIntersection
=================================================
*/
	template <typename T>
	inline bool Line2<T>::GetVecIntersection (const Self &other, OUT vec_t &result) const
	{
		result = vec_t();
		return GetRayIntersection( other, result );
	}
	
/*
=================================================
	GetLineIntersection
=================================================
*/
	template <typename T>
	inline bool Line2<T>::GetLineIntersection (const Self &other, OUT vec_t &result) const
	{
		result = vec_t();
		return GetRayIntersection( other, result ) and IsPointInBoundingRects( other, result );
	}
	
/*
=================================================
	GetRectIntersection
=================================================
*/
	template <typename T>
	inline bool Line2<T>::GetRectIntersection (const Rectangle<T> &other, OUT vec_t &res0, OUT vec_t &res1) const
	{
		return _IntersectRect( other, res0, res1 );
	}
	
/*
=================================================
	GetRayIntersection
=================================================
*/
	template <typename T>
	inline bool Line2<T>::GetRayIntersection (const Self &other, OUT vec_t &res) const
	{
		const T	x12 = this->Begin().x - this->End().x;
		const T	y12 = this->Begin().y - this->End().y;
		
		const T	x34 = other.Begin().x - other.End().x;
		const T	y34 = other.Begin().y - other.End().y;

		const T	c = x12 * y34 - y12 * x34;

		if ( IsZero( c ) )
		{
			return false;
		}
		else
		{
			const T	a = this->Begin().x * this->End().y - this->Begin().y * this->End().x;
			const T	b = other.Begin().x * other.End().y - other.Begin().y * other.End().x;

			res.x = ( a * x34 - b * x12 ) / c;
			res.y = ( a * y34 - b * y12 ) / c;
			return true;
		}
	}
	
/*
=================================================
	_IntersectRect
=================================================
*/
	template <typename T>
	inline bool Line2<T>::_IntersectRect (const Rectangle<T> &other, OUT vec_t &res0, OUT vec_t &res1) const
	{
		Rectangle<T>	r;
		GetBoundingRect( r );
		
		if ( not other.IntersectRect( r ) )
			return false;

		vec_t *		results[4]	= { &res0, &res1, /* for safety: */ &res0, &res1 };
		int			i			= 0;
		bool		res			= false;
		vec_t		tmp;

		if ( GetLineIntersection( Self( other.LeftBottom(), other.LeftTop() ), tmp ) and other.IsInnerPoint( tmp ) )
		{
			*(results[i++]) = tmp;
			res = true;
		}

		if ( GetLineIntersection( Self( other.LeftBottom(), other.RightBottom() ), tmp ) and other.IsInnerPoint( tmp ) )
		{
			*(results[i++]) = tmp;
			res = true;
		}

		if ( GetLineIntersection( Self( other.LeftTop(), other.RightTop() ), tmp ) and other.IsInnerPoint( tmp ) )
		{
			*(results[i++]) = tmp;
			res = true;
		}

		if ( GetLineIntersection( Self( other.RightBottom(), other.RightTop() ), tmp ) and other.IsInnerPoint( tmp ) )
		{
			*(results[i++]) = tmp;
			res = true;
		}
		
		return res;
	}
	
/*
=================================================
	_CropLine
=================================================
*/
	template <typename T>
	inline bool Line2<T>::_CropLine (const Self &src, Self &dst)
	{
		vec_t	i;

		if ( src.GetLineIntersection( dst, i ) )
		{
			dst.Begin() = i;
			return true;
		}
		return false;
	}
	
/*
=================================================
	AngleBetweenLines
=================================================
*/
	template <typename T>
	inline Radians<T>  Line2<T>::AngleBetweenLines (const Self &other) const
	{
		const vec_t	v0 = Vector();
		const vec_t	v1 = other.Vector();

		if ( All( v0 == v1 ) )
			return T(0);

		if ( All( v0 == -v1 ) )
			return Pi<T>;

		const T	a0 = ATan( v0.x, v0.y );
		const T	a1 = ATan( v1.x, v1.y );
		const T	a  = (a1 - a0);

		return Radians<T>(a).ToInterval( -Radians<T>::Pi(), Radians<T>::Pi() );
	}
	
/*
=================================================
	CosBetweenLines
=================================================
*/
	template <typename T>
	inline T Line2<T>::CosBetweenLines (const Self &other) const
	{
		const vec_t	v0 = Vector();
		const vec_t	v1 = other.Vector();

		if ( All( v0 == v1 ) )
			return T(1);	// cos(0)

		if ( All( v0 == -v1 ) )
			return T(-1);	// cos(Pi)

		return Dot( v0, v1 ) / ( v0.Length() * v1.Length() );
	}
	
/*
=================================================
	Reflection
=================================================
*/
	template <typename T>
	inline Line2<T>  Line2<T>::Reflection (const vec_t &normal) const
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



//--------------------------- Rectangle -------------------------------//
/*
=================================================
	IsInnerLine
=================================================
*/
	template <typename T>
	inline bool Rectangle<T>::IsInnerLine (const Line2<T> &other) const
	{
		return IsInnerPoint( other.Begin() ) & IsInnerPoint( other.End() );
	}
	
/*
=================================================
	IntersectLine
=================================================
*/
	template <typename T>
	inline bool Rectangle<T>::IntersectLine (const Line2<T> &other) const
	{
		return other.IntersectRect( *this );
	}
	
/*
=================================================
	GetLineIntersection
=================================================
*/
	template <typename T>
	inline bool Rectangle<T>::GetLineIntersection (const Line2<T> &other, OUT Vec<T,2> &result0, OUT Vec<T,2> &result1) const
	{
		return other.GetRectIntersection( *this, result0, result1 );
	}
	
/*
=================================================
	CropLine
=================================================
*/
	template <typename T>
	inline bool Rectangle<T>::CropLine (OUT Line2<T> &other) const
	{
		if ( IntersectLine( other ) )
		{
			TODO("CropLine");
			return true;
		}
		return false;
	}
	
/*
=================================================
	Left
=================================================
*/
	template <typename T>
	inline Line2<T>  Rectangle<T>::Left () const
	{
		return line_t( LeftBottom(), LeftTop() );
	}
	
/*
=================================================
	Right
=================================================
*/
	template <typename T>
	inline Line2<T>  Rectangle<T>::Right () const
	{
		return line_t( RightTop(), RightBottom() );
	}
	
/*
=================================================
	Bottom
=================================================
*/
	template <typename T>
	inline Line2<T>  Rectangle<T>::Bottom () const
	{
		return line_t( RightBottom(), LeftBottom() );
	}
		
/*
=================================================
	Top
=================================================
*/
	template <typename T>
	inline Line2<T>  Rectangle<T>::Top () const
	{
		return line_t( LeftTop(), RightTop() );
	}
	
/*
=================================================
	ToLine
=================================================
*
	template <typename T>
	inline Line2<T>  Rectangle<T>::ToLine (EEdge i) const
	{
		return line_t( (*this)(i & 3), (*this)((i+1) & 3) );
	}
	*/

}	// GXMath

namespace GXTypes
{
	
	template <typename T>
	struct Hash< GXMath::Line2<T> > :
		private Hash< typename GXMath::Line2<T>::vec_t >
	{
		typedef GXMath::Circle<T>							key_t;
		typedef Hash< typename GXMath::Line2<T>::vec_t >	base_t;
		typedef typename base_t::result_t					result_t;

		result_t operator () (const key_t &x) const
		{
			return	base_t::operator ()( x.Begin() ) +
					base_t::operator ()( x.End() );
		}
	};

}	// GXTypes
}	// GX_STL
