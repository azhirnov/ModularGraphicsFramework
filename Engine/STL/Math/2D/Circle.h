// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Rectangle.h"
#include "Line2.h"

namespace GX_STL
{
namespace GXMath
{

	//
	// Circle
	//

	template <typename T, ulong U>
	struct Circle : public CompileTime::CopyQualifiers< T >
	{
	// types
	public:
		typedef Circle< T, U >		Self;
		typedef Vec< T, 2, U >		Vec_t;
		typedef Rectangle< T, U >	Rect_t;
		typedef Line2< T, U >		Line_t;


	// variables
	private:
		Vec_t	_center;
		T		_radius;


	// methods
	public:
		Circle (GX_DEFCTOR) : _radius(0) {}

		explicit
		Circle (T radius) : _radius(radius) {}

		Circle (const Vec_t &center, T radius) : _radius(radius), _center(center) {}

		Self &	Move (const Vec_t &delta);
		Self &	SetCenter (const Vec_t &center);

		Self &	AddToRadius (T radius);
		Self &	SetRadius (T radius);

		Self &	FromRect (const Rect_t &rect);
		//Self &	FromTriangle (const Triangle<T> &tr);
		//Self &	FromPoints (ArrayCRef<Vec_t> points);

		Rect_t			GetAxisAlignedRectangle ()	const;

		Vec_t const &	Center ()					const	{ return _center; }
		Vec_t &			Center ()							{ return _center; }

		T const &		Radius ()					const	{ return _radius; }
		T &				Radius ()							{ return _radius; }
		//T				RadiusSqr ()				const	{ return _radius * _radius; }

		T			Length ()						const	{ return Pi<T> * T(2) * Radius(); }
		T			ArcLength (Radians<T> angle)	const	{ return (T)angle * Radius(); }
		T			Square ()						const	{ return Pi<T> * Square( Radius() ); }

		Radians<T>	GetAngle (const Vec_t &point)	const;
		Radians<T>	AngleFromArcLength (T len)		const	{ return len / Radius(); }
		Radians<T>	AngleFromChordLength (T len)	const	{ return T(2) * ASin( len / ( T(2) * Radius() ) ); }

		Line_t		GetChord (Radians<T> startAngle, Radians<T> endAngle) const;
		Vec_t		GetPoint (Radians<T> angle) const;
		
		Vec_t		GetXfromY (T y) const;
		Vec_t		GetYfromX (T x) const;

		bool IsInnerPoint (const Vec_t &point) const;
		bool IsInnerLine (const Line_t &line) const;
		bool IsInnerRect (const Rect_t &rect) const;
		bool IsInnerCircle (const Self &circle)	const;

		bool IntersectLine (const Line_t &line) const;
		bool IntersectRect (const Rect_t &rect) const;
		bool IntersectCircle (const Self &circle) const;
	};



	
	template <typename T, ulong U>
	inline Circle<T,U> &  Circle<T,U>::Move (const Vec_t &delta)
	{
		_center += delta;
		return *this;
	}
	

	template <typename T, ulong U>
	inline Circle<T,U> &  Circle<T,U>::SetCenter (const Vec_t &center)
	{
		_center = center;
		return *this;
	}
	

	template <typename T, ulong U>
	inline Circle<T,U> &  Circle<T,U>::AddToRadius (T radius)
	{
		_radius += radius;
		return *this;
	}
	

	template <typename T, ulong U>
	inline Circle<T,U> &  Circle<T,U>::SetRadius (T radius)
	{
		_radius = radius;
		return *this;
	}
	

	// Circumscribed circle
	template <typename T, ulong U>
	inline Circle<T,U> &  Circle<T,U>::FromRect (const Rect_t &rect)
	{
		return SetCenter( rect.Center() ).SetRadius( rect.LeftBottom().Distance( rect.RightTop() ) / T(2) );
	}


	template <typename T, ulong U>
	inline CHECKRES(Rectangle<T,U>)  Circle<T,U>::GetAxisAlignedRectangle () const
	{
		Rect_t	r( _center, _center );
		r.LeftBottom() -= _radius;
		r.RightTop()   += _radius;
	}
	

	template <typename T, ulong U>
	inline CHECKRES(Vec<T,2,U>)  Circle<T,U>::GetPoint (Radians<T> angle) const
	{
		return _center + _radius * SinCos( (T)angle ).yx();
	}
	

	template <typename T, ulong U>
	inline CHECKRES(Radians<T>)  Circle<T,U>::GetAngle (const Vec_t &point) const
	{
		//const Vec_t	v = point - _center;
		//const T		d = v.Length();
		const Vec_t	sc = (point - _center).Normalized(); //v / d;
		return ATan( sc.y, sc.x );
	}
	

	template <typename T, ulong U>
	inline CHECKRES(Vec<T,2,U>)  Circle<T,U>::GetXfromY (const T y) const
	{
		// (x-cx)^2 + (y-cy)^2 = R^2
		const T	x = Sqrt( Square( _radius ) - Square( y - _center.y ) );
		return _center.x + Vec_t( x, -x );
	}

	
	template <typename T, ulong U>
	inline CHECKRES(Vec<T,2,U>)  Circle<T,U>::GetYfromX (const T x) const
	{
		const T	y = Sqrt( Square( _radius ) - Square( x - _center.x ) );
		return _center.y + Vec_t( y, -y );
	}


	template <typename T, ulong U>
	inline CHECKRES(Line2<T,U>)  Circle<T,U>::GetChord (Radians<T> startAngle, Radians<T> endAngle) const
	{
		return Line_t( GetPoint( startAngle ), GetPoint( endAngle ) );
	}


	template <typename T, ulong U>
	inline CHECKRES(bool)  Circle<T,U>::IsInnerPoint (const Vec_t &point) const
	{
		return _center.Distance( point ) <= Radius();
	}
	

	template <typename T, ulong U>
	inline CHECKRES(bool)  Circle<T,U>::IsInnerLine (const Line_t &line) const
	{
		return IsInnerPoint( line.Begin() ) and IsInnerPoint( line.End() );
	}
	

	template <typename T, ulong U>
	inline CHECKRES(bool)  Circle<T,U>::IsInnerRect (const Rect_t &rect) const
	{
		return IsInnerPoint( rect.LeftBottom() ) and IsInnerPoint( rect.RightTop() );
	}
	

	template <typename T, ulong U>
	inline CHECKRES(bool)  Circle<T,U>::IsInnerCircle (const Self &circle) const
	{
		return	_radius >= circle.Radius() and
				_center.Distance( circle.Center() ) + circle.Radius() <= Radius();
	}
	

	template <typename T, ulong U>
	inline CHECKRES(bool)  Circle<T,U>::IntersectLine (const Line_t &line) const
	{
		TODO( "IntersectLine" );
		return line.IntersectRect( GetAxisAlignedRectangle() );
	}
		
	
	template <typename T, ulong U>
	inline CHECKRES(bool)  Circle<T,U>::IntersectRect (const Rect_t &rect) const
	{
		return rect.IntersectRect( GetAxisAlignedRectangle() );
	}
		
	
	template <typename T, ulong U>
	inline CHECKRES(bool)  Circle<T,U>::IntersectCircle (const Self &circle) const
	{
		return this->Center().Distance( circle.Center() ) - circle.Radius() <= this->Radius();
	}


}	// GXMath

namespace GXTypes
{
	
	template <typename T, ulong U>
	struct Hash< GXMath::Circle<T,U> > :
		private Hash< typename GXMath::Circle<T,U>::Vec_t >, private Hash< T >
	{
		typedef GXMath::Circle<T,U>			Key_t;
		typedef typename Hash<T>::Result_t	Result_t;

		CHECKRES(Result_t)  operator () (const Key_t &x) const noexcept
		{
			return	HashOf( x.Center() ) + HashOf( x.Radius() );
		}
	};

}	// GXTypes
}	// GX_STL
