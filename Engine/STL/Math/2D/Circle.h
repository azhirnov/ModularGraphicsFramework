// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

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

	template <typename T>
	struct Circle : public CompileTime::CopyQualifiers< T >
	{
	// types
	public:
		typedef Circle< T >			Self;
		typedef Vec< T, 2 >			vec_t;
		typedef Rectangle< T >		rect_t;
		typedef Line2< T >			line_t;


	// variables
	private:
		vec_t	_center;
		T		_radius;


	// methods
	public:
		Circle (GX_DEFCTOR) : _radius(0) {}

		explicit
		Circle (T radius) : _radius(radius) {}

		Circle (const vec_t &center, T radius) : _radius(radius), _center(center) {}

		Self &	Move (const vec_t &delta);
		Self &	SetCenter (const vec_t &center);

		Self &	AddToRadius (T radius);
		Self &	SetRadius (T radius);

		Self &	FromRect (const rect_t &rect);
		//Self &	FromTriangle (const Triangle<T> &tr);
		//Self &	FromPoints (ArrayCRef<vec_t> points);

		rect_t			GetAxisAlignedRectangle ()	const;

		vec_t const &	Center ()					const	{ return _center; }
		vec_t &			Center ()							{ return _center; }

		T const &		Radius ()					const	{ return _radius; }
		T &				Radius ()							{ return _radius; }
		//T				RadiusSqr ()				const	{ return _radius * _radius; }

		T			Length ()						const	{ return Pi<T> * T(2) * Radius(); }
		T			ArcLength (Radians<T> angle)	const	{ return (T)angle * Radius(); }
		T			Square ()						const	{ return Pi<T> * Square( Radius() ); }

		Radians<T>	GetAngle (const vec_t &point)	const;
		Radians<T>	AngleFromArcLength (T len)		const	{ return len / Radius(); }
		Radians<T>	AngleFromChordLength (T len)	const	{ return T(2) * ASin( len / ( T(2) * Radius() ) ); }

		line_t		GetChord (Radians<T> startAngle, Radians<T> endAngle) const;
		vec_t		GetPoint (Radians<T> angle) const;
		
		vec_t		GetXfromY (T y) const;
		vec_t		GetYfromX (T x) const;

		bool IsInnerPoint (const vec_t &point) const;
		bool IsInnerLine (const line_t &line) const;
		bool IsInnerRect (const rect_t &rect) const;
		bool IsInnerCircle (const Self &circle)	const;

		bool IntersectLine (const line_t &line) const;
		bool IntersectRect (const rect_t &rect) const;
		bool IntersectCircle (const Self &circle) const;
	};



	
	template <typename T>
	inline Circle<T> &  Circle<T>::Move (const vec_t &delta)
	{
		_center += delta;
		return *this;
	}
	

	template <typename T>
	inline Circle<T> &  Circle<T>::SetCenter (const vec_t &center)
	{
		_center = center;
		return *this;
	}
	

	template <typename T>
	inline Circle<T> &  Circle<T>::AddToRadius (T radius)
	{
		_radius += radius;
		return *this;
	}
	

	template <typename T>
	inline Circle<T> &  Circle<T>::SetRadius (T radius)
	{
		_radius = radius;
		return *this;
	}
	

	// Circumscribed circle
	template <typename T>
	inline Circle<T> &  Circle<T>::FromRect (const rect_t &rect)
	{
		return SetCenter( rect.Center() ).SetRadius( rect.LeftBottom().Distance( rect.RightTop() ) / T(2) );
	}


	template <typename T>
	inline Rectangle<T>  Circle<T>::GetAxisAlignedRectangle () const
	{
		rect_t	r( _center, _center );
		r.LeftBottom() -= _radius;
		r.RightTop()   += _radius;
	}
	

	template <typename T>
	inline Vec<T,2>  Circle<T>::GetPoint (Radians<T> angle) const
	{
		return _center + _radius * SinCos( (T)angle ).yx();
	}
	

	template <typename T>
	inline Radians<T>  Circle<T>::GetAngle (const vec_t &point) const
	{
		//const vec_t	v = point - _center;
		//const T		d = v.Length();
		const vec_t	sc = (point - _center).Normalized(); //v / d;
		return ATan( sc.y, sc.x );
	}
	

	template <typename T>
	inline Vec<T,2>  Circle<T>::GetXfromY (const T y) const
	{
		// (x-cx)^2 + (y-cy)^2 = R^2
		const T	x = Sqrt( Square( _radius ) - Square( y - _center.y ) );
		return _center.x + vec_t( x, -x );
	}

	
	template <typename T>
	inline Vec<T,2>  Circle<T>::GetYfromX (const T x) const
	{
		const T	y = Sqrt( Square( _radius ) - Square( x - _center.x ) );
		return _center.y + vec_t( y, -y );
	}


	template <typename T>
	inline Line2<T>  Circle<T>::GetChord (Radians<T> startAngle, Radians<T> endAngle) const
	{
		return line_t( GetPoint( startAngle ), GetPoint( endAngle ) );
	}


	template <typename T>
	inline bool Circle<T>::IsInnerPoint (const vec_t &point) const
	{
		return _center.Distance( point ) <= Radius();
	}
	

	template <typename T>
	inline bool Circle<T>::IsInnerLine (const line_t &line) const
	{
		return IsInnerPoint( line.Begin() ) and IsInnerPoint( line.End() );
	}
	

	template <typename T>
	inline bool Circle<T>::IsInnerRect (const rect_t &rect) const
	{
		return IsInnerPoint( rect.LeftBottom() ) and IsInnerPoint( rect.RightTop() );
	}
	

	template <typename T>
	inline bool Circle<T>::IsInnerCircle (const Self &circle) const
	{
		return	_radius >= circle.Radius() and
				_center.Distance( circle.Center() ) + circle.Radius() <= Radius();
	}
	

	template <typename T>
	inline bool Circle<T>::IntersectLine (const line_t &line) const
	{
		TODO( "IntersectLine" );
		return line.IntersectRect( ToRect() );
	}
		
	
	template <typename T>
	inline bool Circle<T>::IntersectRect (const rect_t &rect) const
	{
		return rect.IntersectRect( ToRect() );
	}
		
	
	template <typename T>
	inline bool Circle<T>::IntersectCircle (const Self &circle) const
	{
		return this->Center().Distance( circle.Center() ) - circle.Radius() <= this->Radius();
	}


}	// GXMath

namespace GXTypes
{
	
	template <typename T>
	struct Hash< GXMath::Circle<T> > :
		private Hash< typename GXMath::Circle<T>::vec_t >, private Hash< T >
	{
		typedef GXMath::Circle<T>			key_t;
		typedef typename Hash<T>::result_t	result_t;

		result_t operator () (const key_t &x) const
		{
			return	Hash< typename GXMath::Circle<T>::vec_t >::operator ()( x.Center() ) +
					Hash< T >::operator ()( x.Radius() );
		}
	};

}	// GXTypes
}	// GX_STL
