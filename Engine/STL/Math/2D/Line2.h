// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Rectangle.h"

namespace GX_STL
{
namespace GXMath
{


	//
	// Line
	//

	template <typename T, ulong U>
	struct Line2 : public CompileTime::CopyQualifiers< T >
	{
	// types
	public:
		using Self		= Line2< T, U >;
		using Vec_t		= Vec<T,2,U>;
		using Value_t	= T;
		using Rect_t	= Rectangle<T,U>;
		using Rad_t		= Radians<T>;


	// variables
	private:
		Vec_t	_p0;
		Vec_t	_p1;


	// methods
	public:
		Line2 (GX_DEFCTOR) {}

		explicit
		Line2 (const Vec_t &end) : _p0(), _p1(end) {}

		Line2 (const Vec_t &begin, const Vec_t &end) : _p0(begin), _p1(end) {}

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

		Self Invert ()					const	{ return Self( _p1, _p0 ); }

		Self Move (const Vec_t &delta)	const	{ return Self( _p0 + delta, _p1 + delta ); }

		void GetEquation (OUT T &k, OUT T &b) const;
		void GetEquation (OUT T &a, OUT T &b, OUT T &c) const;
		void GetBoundingRect (OUT Rect_t &boundRect) const;

		T	GetXfromY (T y) const;
		T	GetYfromX (T x) const;

		Vec_t	GetFromY (T y) const;
		Vec_t	GetFromX (T x) const;

		bool IsInnerPoint (const Vec_t &point) const;
		bool IsInnerLine (const Self &line) const;

		bool IntersectLine (const Self &other) const;
		bool IntersectRect (const Rect_t &other) const;

		bool GetVecIntersection (const Self &other, OUT Vec_t &result) const;
		bool GetLineIntersection (const Self &other, OUT Vec_t &result) const;
		bool GetRectIntersection (const Rect_t &other, OUT Vec_t &result0, OUT Vec_t &result1) const;

		bool GetRayIntersection (const Self &other, OUT Vec_t &res) const;

		bool IsPointInBoundingRects (const Self &other, const Vec_t &point) const;

		Rad_t	AngleBetweenLines (const Self &other) const;	// angle of other line rotation by this line in a clockwise 
		T		CosBetweenLines (const Self &other) const;
		Self	Reflection (const Vec_t &normal) const;

		bool	PerpendicularBase (const Vec_t &point, OUT Vec_t &base)	const;
		bool	Perpendicular (const Vec_t &point, OUT Self &result) const;
		T		MinDistance (const Vec_t &point) const;
		
		Vec_t	Normal () const;
		Vec_t	LeftNormal () const;
		Vec_t	RightNormal () const;
		Vec_t	GetInterpolationPoint (T value)	const	{ return Lerp( _p0, _p1, value ); }


		static bool _CropLine (const Self &src, Self &dst);

	private:
		bool _IntersectRect (const Rect_t &other, OUT Vec_t &res0, OUT Vec_t &res1) const;
	};


	
	
		
/*
=================================================
	operator ==
=================================================
*/
	template <typename T, ulong U>
	inline bool	Line2<T,U>::operator == (const Self &right) const
	{
		return All( Begin() == right.Begin() ) and All( End() == right.End() );
	}
	
	template <typename T, ulong U>
	inline bool	Line2<T,U>::operator != (const Self &right) const
	{
		return not ( (*this) == right );
	}
	
/*
=================================================
	GetEquation
=================================================
*/
	template <typename T, ulong U>
	inline void Line2<T,U>::GetEquation (OUT T &outK, OUT T &outB) const
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
	template <typename T, ulong U>
	inline void Line2<T,U>::GetEquation (OUT T &a, OUT T &b, OUT T &c) const
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
	template <typename T, ulong U>
	inline T Line2<T,U>::GetXfromY (T y) const
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
	template <typename T, ulong U>
	inline T  Line2<T,U>::GetYfromX (T x) const
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
	template <typename T, ulong U>
	inline typename Line2<T,U>::Vec_t  Line2<T,U>::GetFromY (T y) const
	{
		return Vec_t( GetXfromY( y ), y );
	}
	
/*
=================================================
	GetFromX
=================================================
*/
	template <typename T, ulong U>
	inline typename Line2<T,U>::Vec_t  Line2<T,U>::GetFromX (T x) const
	{
		return Vec_t( x, GetYfromX( x ) );
	}
		
/*
=================================================
	PerpendicularBase
=================================================
*/
	template <typename T, ulong U>
	inline bool Line2<T,U>::PerpendicularBase (const Vec_t &p, OUT Vec_t &base) const
	{
		Vec_t	vec		= End() - Begin();
		Vec_t	vecSqr	= vec * vec;

		base.x = ( Begin().x * vecSqr.y + p.x * vecSqr.x + vec.x * vec.y * (p.y - Begin().y) ) / (vecSqr.y + vecSqr.x);
		base.y = vec.x * (p.x - base.x) / vec.y + p.y;

		return IsInnerPoint( base );
	}
	
/*
=================================================
	Perpendicular
=================================================
*/
	template <typename T, ulong U>
	inline bool Line2<T,U>::Perpendicular (const Vec_t &point, OUT Self &result) const
	{
		Vec_t	base;

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
	template <typename T, ulong U>
	inline T  Line2<T,U>::MinDistance (const Vec_t &point) const
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
	template <typename T, ulong U>
	inline Vec<T,2,U>  Line2<T,U>::Normal () const
	{
		const Vec_t	v = Vector();
		return ( v.x >= T(0) ? Vec_t( -v.y, v.x ) : Vec_t( v.y, -v.x ) ).Normalized();
	}
	
/*
=================================================
	LeftNormal
=================================================
*/
	template <typename T, ulong U>
	inline Vec<T,2,U>  Line2<T,U>::LeftNormal () const
	{
		const Vec_t	v = Vector();
		return Vec_t( -v.y, v.x ).Normalized();
	}
	
/*
=================================================
	RightNormal
=================================================
*/
	template <typename T, ulong U>
	inline Vec<T,2,U>  Line2<T,U>::RightNormal () const
	{
		const Vec_t	v = Vector();
		return Vec_t( v.y, -v.x ).Normalized();
	}
	
/*
=================================================
	GetBoundingRect
=================================================
*/
	template <typename T, ulong U>
	inline void Line2<T,U>::GetBoundingRect (OUT Rect_t &boundRect) const
	{
		boundRect = Rect_t( MakeStaticArray( Begin(), End() ) );
		boundRect.Stretch( Vector().Max() /* T(0.01)*/ );
	}
	
/*
=================================================
	IsInnerPoint
=================================================
*/
	template <typename T, ulong U>
	inline bool Line2<T,U>::IsInnerPoint (const Vec_t &point) const
	{
		Rect_t	r;
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
	template <typename T, ulong U>
	inline bool Line2<T,U>::IsInnerLine (const Self &line) const
	{
		Rect_t	r;
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
	template <typename T, ulong U>
	inline bool Line2<T,U>::IntersectLine (const Self &other) const
	{
		Vec_t	res;
		return GetLineIntersection( other, res );
	}
	
/*
=================================================
	IntersectRect
=================================================
*/
	template <typename T, ulong U>
	inline bool Line2<T,U>::IntersectRect (const Rect_t &other) const
	{
		Vec_t	res0, res1;
		return _IntersectRect( other, res0, res1 );
	}
	
/*
=================================================
	IsPointInBoundingRects
=================================================
*/
	template <typename T, ulong U>
	inline bool Line2<T,U>::IsPointInBoundingRects (const Self &other, const Vec_t &point) const
	{
		Rect_t	r0;
		Rect_t	r1;
		this->GetBoundingRect( r0 );
		other.GetBoundingRect( r1 );
		return r0.IsInnerPoint( point ) and r1.IsInnerPoint( point );
	}
	
/*
=================================================
	GetVecIntersection
=================================================
*/
	template <typename T, ulong U>
	inline bool Line2<T,U>::GetVecIntersection (const Self &other, OUT Vec_t &result) const
	{
		result = Vec_t();
		return GetRayIntersection( other, result );
	}
	
/*
=================================================
	GetLineIntersection
=================================================
*/
	template <typename T, ulong U>
	inline bool Line2<T,U>::GetLineIntersection (const Self &other, OUT Vec_t &result) const
	{
		result = Vec_t();
		return GetRayIntersection( other, result ) and IsPointInBoundingRects( other, result );
	}
	
/*
=================================================
	GetRectIntersection
=================================================
*/
	template <typename T, ulong U>
	inline bool Line2<T,U>::GetRectIntersection (const Rect_t &other, OUT Vec_t &res0, OUT Vec_t &res1) const
	{
		return _IntersectRect( other, res0, res1 );
	}
	
/*
=================================================
	GetRayIntersection
=================================================
*/
	template <typename T, ulong U>
	inline bool Line2<T,U>::GetRayIntersection (const Self &other, OUT Vec_t &res) const
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
	template <typename T, ulong U>
	inline bool Line2<T,U>::_IntersectRect (const Rect_t &other, OUT Vec_t &res0, OUT Vec_t &res1) const
	{
		Rect_t	r;
		GetBoundingRect( r );
		
		if ( not other.IntersectRect( r ) )
			return false;

		Vec_t *		results[4]	= { &res0, &res1, /* for safety: */ &res0, &res1 };
		int			i			= 0;
		bool		res			= false;
		Vec_t		tmp;

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
	template <typename T, ulong U>
	inline bool Line2<T,U>::_CropLine (const Self &src, Self &dst)
	{
		Vec_t	i;

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
	template <typename T, ulong U>
	inline Radians<T>  Line2<T,U>::AngleBetweenLines (const Self &other) const
	{
		const Vec_t	v0 = Vector();
		const Vec_t	v1 = other.Vector();

		if ( All( v0 == v1 ) )
			return Rad_t(T(0));

		if ( All( v0 == -v1 ) )
			return Rad_t::Pi();

		const Rad_t	a0 = ATan( v0.x, v0.y );
		const Rad_t	a1 = ATan( v1.x, v1.y );
		const Rad_t	a  = (a1 - a0);

		return a.ToInterval( -Rad_t::Pi(), Rad_t::Pi() );
	}
	
/*
=================================================
	CosBetweenLines
=================================================
*/
	template <typename T, ulong U>
	inline T Line2<T,U>::CosBetweenLines (const Self &other) const
	{
		const Vec_t	v0 = Vector();
		const Vec_t	v1 = other.Vector();

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
	template <typename T, ulong U>
	inline Line2<T,U>  Line2<T,U>::Reflection (const Vec_t &normal) const
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
	template <typename T, ulong U>
	inline bool Rectangle<T,U>::IsInnerLine (const Line2<T,U> &other) const
	{
		return IsInnerPoint( other.Begin() ) & IsInnerPoint( other.End() );
	}
	
/*
=================================================
	IntersectLine
=================================================
*/
	template <typename T, ulong U>
	inline bool Rectangle<T,U>::IntersectLine (const Line2<T,U> &other) const
	{
		return other.IntersectRect( *this );
	}
	
/*
=================================================
	GetLineIntersection
=================================================
*/
	template <typename T, ulong U>
	inline bool Rectangle<T,U>::GetLineIntersection (const Line2<T,U> &other, OUT Vec<T,2,U> &result0, OUT Vec<T,2,U> &result1) const
	{
		return other.GetRectIntersection( *this, result0, result1 );
	}
	
/*
=================================================
	CropLine
=================================================
*/
	template <typename T, ulong U>
	inline bool Rectangle<T,U>::CropLine (OUT Line2<T,U> &other) const
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
	template <typename T, ulong U>
	inline Line2<T,U>  Rectangle<T,U>::Left () const
	{
		return Line_t( LeftBottom(), LeftTop() );
	}
	
/*
=================================================
	Right
=================================================
*/
	template <typename T, ulong U>
	inline Line2<T,U>  Rectangle<T,U>::Right () const
	{
		return Line_t( RightTop(), RightBottom() );
	}
	
/*
=================================================
	Bottom
=================================================
*/
	template <typename T, ulong U>
	inline Line2<T,U>  Rectangle<T,U>::Bottom () const
	{
		return Line_t( RightBottom(), LeftBottom() );
	}
		
/*
=================================================
	Top
=================================================
*/
	template <typename T, ulong U>
	inline Line2<T,U>  Rectangle<T,U>::Top () const
	{
		return Line_t( LeftTop(), RightTop() );
	}
	
/*
=================================================
	ToLine
=================================================
*
	template <typename T, ulong U>
	inline Line2<T,U>  Rectangle<T,U>::ToLine (EEdge i) const
	{
		return Line_t( (*this)(i & 3), (*this)((i+1) & 3) );
	}
	*/

}	// GXMath

namespace GXTypes
{
	
	template <typename T, ulong U>
	struct Hash< GXMath::Line2<T,U> > :
		private Hash< typename GXMath::Line2<T,U>::Vec_t >
	{
		using Key_t		= GXMath::Line2<T,U>;
		using Base_t	= Hash< typename Key_t::Vec_t >;
		using Result_t	= typename Base_t::Result_t;

		Result_t operator () (const Key_t &x) const noexcept
		{
			return	Base_t::operator ()( x.Begin() ) + Base_t::operator ()( x.End() );
		}
	};

}	// GXTypes
}	// GX_STL
