// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "MathTypes2D.h"

namespace GX_STL
{
namespace GXMath
{

#	define RECT_OPERATOR( _op_ ) \
		\
		Self &	operator _op_##= (const T& other)				{ left _op_##= other;  bottom _op_##= other;  right _op_##= other;  top _op_##= other;  return *this; } \
		Self	operator _op_    (const T& other) const			{ return Self( left _op_ other, bottom _op_ other, right _op_ other, top _op_ other ); } \
		\
		Self &	operator _op_##= (const Self& other)			{ left _op_##= other.left;  bottom _op_##= other.bottom;  right _op_##= other.right;  top _op_##= other.top;  return *this; } \
		Self	operator _op_    (const Self& other) const		{ return Self( left _op_ other.left, bottom _op_ other.bottom, right _op_ other.right, top _op_ other.top ); } \
		\
		Self &	operator _op_##= (const vec2_t& other)			{ left _op_##= other.x;  bottom _op_##= other.y;  right _op_##= other.x;  top _op_##= other.y;  return *this; } \
		Self	operator _op_    (const vec2_t& other) const	{ return Self( left _op_ other.x, bottom _op_ other.y, right _op_ other.x, top _op_ other.y ); } \
		\
		friend Self & operator _op_ (const T& lvalue, const Self& rvalue)		{ return Self( lvalue, lvalue, lvalue, lvalue ) _op_ rvalue; } \
		friend Self & operator _op_ (const vec2_t& lvalue, const Self& rvalue)	{ return Self( lvalue.x, lvalue.y, lvalue.x, lvalue.y ) _op_ rvalue; } \
		

	//
	// Border
	//
	/*
	struct EBorder
	{
		enum	type
		{
			OUTER		= 0x00,
			INNER		= 0x11,		// in other
			LEFT		= 0x02,
			RIGHT		= 0x04,
			TOP			= 0x20,
			BOTTOM		= 0x40,
			LB_ANGLE	= LEFT  | BOTTOM,
			LT_ANGLE	= LEFT  | TOP,
			RB_ANGLE	= RIGHT | BOTTOM,
			RT_ANGLE	= RIGHT | TOP
		};

		GX_ENUM_BIT_OPERATIONS( type )
	};*/



	//
	// Rect
	//

	template <typename T>
	struct Rectangle : public CompileTime::CopyQualifiers< T >
	{
	// types
	public:
		typedef bool			_is_rect;
		typedef Rectangle<T>	Self;
		typedef T				value_t;
		typedef Vec<T,2>		vec2_t;
		typedef Vec<T,4>		vec4_t;
		typedef Line2<T>		line_t;

		//enum EEdge : ubyte
		//{
		//	LEFT = 0,
		//	TOP,
		//	RIGHT,
		//	BOTTOM,
		//};


	// variables
	public:
		T	left,
			bottom,
			right,
			top;
		

	public:
		// constructors
		Rectangle (GX_DEFCTOR) :							left(0), bottom(0), right(0), top(0) {}

		Rectangle (T left, T bottom, T right, T top) :		left(left), bottom(bottom), right(right), top(top) {}

		explicit
		Rectangle (const vec2_t &size) :					left(0), bottom(0), right(size.x), top(size.y) {}

		Rectangle (const vec2_t &leftBottom, const vec2_t &rightTop) :
			left(leftBottom.x), bottom(leftBottom.y),
			right(rightTop.x), top(rightTop.y)
		{}

		Rectangle (ArrayCRef<vec2_t> points);

		// unary operators
		Self		operator - () const;
		Self		operator ~ () const;
		Self		operator ! () const;


		// binary operators
		vec2_t		operator () (usize i)	const;

		bool		operator == (const Self &right) const;
		bool		operator != (const Self &right) const;

		RECT_OPERATOR( +  );
		RECT_OPERATOR( -  );
		RECT_OPERATOR( *  );
		RECT_OPERATOR( /  );
		RECT_OPERATOR( %  );
		RECT_OPERATOR( &  );
		RECT_OPERATOR( |  );
		RECT_OPERATOR( ^  );
		RECT_OPERATOR( >> );
		RECT_OPERATOR( << );
		

		// methods
		T		*	ptr ()							{ return PointerCast< T >( this ); }
		const T *	ptr () const					{ return PointerCast< T >( this ); }

		vec2_t		LeftBottom () const				{ return vec2_t( left, bottom ); }
		vec2_t &	LeftBottom ()					{ return *PointerCast< vec2_t >( &left ); }

		vec2_t		RightTop () const				{ return vec2_t( right, top ); }
		vec2_t &	RightTop ()						{ return *PointerCast< vec2_t >( &right ); }

		vec2_t		LeftTop () const				{ return vec2_t( left, top ); }
		vec2_t		RightBottom () const			{ return vec2_t( right, bottom ); }

		vec4_t		ToVec4 () const					{ return vec4_t( left, bottom, right, top ); }

		line_t		Left () const;
		line_t		Right () const;
		line_t		Bottom () const;
		line_t		Top () const;
		//line_t		ToLine (EEdge e) const;

		T			Width () const					{ return right - left; }
		T			Height () const					{ return top - bottom; }

		Self &		SetCenter (const vec2_t &center);
		Self &		SetSize (const vec2_t &size);

		Self &		Scale (T scale)					{ return Scale( vec2_t(scale) ); }
		Self &		Scale (const vec2_t &scale);

		Self &		Join (const Self &other);
		Self &		Join (const vec2_t &point);

		Self &		Stretch (const vec2_t &size);
		Self &		Stretch (T size)				{ return Stretch( vec2_t( size ) ); }

		vec2_t		Size ()		const;
		vec2_t		Center ()	const;

		// IsInner*
		bool  IsInnerLine (const line_t &line) const;
		bool  IsInnerPoint (const vec2_t &point) const;
		bool  IsInnerRect (const Self &other) const;

		// Intersect*
		bool  IntersectLine (const line_t &line) const;
		bool  IntersectRect (const Self &other) const;

		// GetIntersection*
		bool  GetLineIntersection (const line_t &line, OUT vec2_t &result0, OUT vec2_t &result1) const;

		bool  CropRect (OUT Self &other) const;
		bool  CropLine (OUT line_t &line) const;

		bool  Sub (const Self &other, OUT Self &result0, OUT Self &result1) const;

		//EBorder::type PointInBorder (const vec2_t &point, T border) const;
		
		template <typename T2>
		Rectangle<T2>	Convert () const;

		template <typename B>
		const B		To () const;
	};

	
	
	
//--------------------------- Math Func ---------------------------//
/*
=================================================
	Abs
=================================================
*/
	template <typename T>
	inline Rectangle<T> Abs (const Rectangle<T> &val)
	{
		return Rectangle<T>( Abs(val.left), Abs(val.bottom), Abs(val.right), Abs(val.top) );
	}
	
/*
=================================================
	IsZero
=================================================
*/
	template <typename T>
	inline bool IsZero (const Rectangle<T>& val)
	{
		return IsZero(val.left) and IsZero(val.bottom) and IsZero(val.right) and IsZero(val.top);
	}
	
/*
=================================================
	SetSign
=================================================
*/
	template <typename T>
	inline Rectangle<T> SetSign (const Rectangle<T> &val, bool bSign)
	{
		return Rectangle<T>(	SetSign( val.left,  bSign ),	SetSign( val.bottom, bSign ),
								SetSign( val.right, bSign ),	SetSign( val.top,	 bSign ) );
	}
	
/*
=================================================
	Mod
=================================================
*/
	template <typename T>
	inline Rectangle<T> Mod (const Rectangle<T>& left, const Rectangle<T>& right)
	{
		return Rectangle<T>(	Mod( left.left,  right.left ),	Mod( left.bottom, right.bottom ),
								Mod( left.right, right.right ), Mod( left.top,	  right.top ) );
	}
	
/*
=================================================
	Round
=================================================
*/
	template <typename T>
	inline Rectangle<T> Round (const Rectangle<T>& val)
	{
		return Rectangle<T>(	Round( val.left ),	Round( val.bottom ),
								Round( val.right ),	Round( val.top ) );
	}
	
/*
=================================================
	RoundToInt
=================================================
*/
	template <typename T>
	inline Rectangle< typename CompileTime::NearInt::FromType<T> > RoundToInt (const Rectangle<T>& val)
	{
		return Rectangle< typename CompileTime::NearInt::FromType<T> >(
					RoundToInt( val.left ),		RoundToInt( val.bottom ),
					RoundToInt( val.right ),	RoundToInt( val.top ) );
	}
	
/*
=================================================
	RoundToUInt
=================================================
*/
	template <typename T>
	inline Rectangle< typename CompileTime::NearUInt::FromType<T> > RoundToUInt (const Rectangle<T>& val)
	{
		return Rectangle< typename CompileTime::NearUInt::FromType<T> >(
					RoundToUInt( val.left ),	RoundToUInt( val.bottom ),
					RoundToUInt( val.right ),	RoundToUInt( val.top ) );
	}
	
/*
=================================================
	SafeDiv
=================================================
*/
	template <typename T>
	inline Rectangle<T> SafeDiv (const Rectangle<T>& left, const Rectangle<T>& right, const Rectangle<T>& defVal)
	{
		return Rectangle<T>(	SafeDiv( left.left,		right.left,		defVal.left ),
								SafeDiv( left.bottom,	right.bottom,	defVal.bottom ),
								SafeDiv( left.right,	right.right,	defVal.right ),
								SafeDiv( left.top,		right.top,		defVal.top ) );
	}



//--------------------------- Rectangle -------------------------------//
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	inline Rectangle<T>::Rectangle (ArrayCRef<vec2_t> points) :
		left(0), bottom(0), right(0), top(0)
	{
		if ( points.Empty() )
			return;

		left = right = points.Front().x;
		bottom = top = points.Front().y;

		FORv( i, 1, points )
		{
			vec2_t const&	p = points[i];

			left   = Min( left,		p.x );
			right  = Max( right,	p.x );
			bottom = Min( bottom,	p.y );
			top    = Max( top,		p.y );
		}
	}
	
/*
=================================================
	operator ==
=================================================
*/
	template <typename T>
	inline bool Rectangle<T>::operator == (const Self &other) const
	{
		return ( (left  == other.left)  & (bottom == other.bottom) &
				 (right == other.right) & (top    == other.top) );
	}
	
/*
=================================================
	operator !=
=================================================
*/
	template <typename T>
	inline bool Rectangle<T>::operator != (const Self &other) const
	{
		return not (*this == other);
	}

/*
=================================================
	operator ()
=================================================
*/
	template <typename T>
	inline Vec<T,2> Rectangle<T>::operator () (usize i) const
	{
		ASSUME( i < 4 );
		return vec2_t( (*this)[ i & 2 ], (*this)[ ((( (i & 1) + (i >> 1) ) & 1) << 1) + 1 ] );
	}
	
/*
=================================================
	Convert
=================================================
*/
	template <typename T>
	template <typename T2>
	inline Rectangle<T2> Rectangle<T>::Convert () const
	{
		return Rectangle<T2>( T2(left), T2(bottom), T2(right), T2(top) );
	}
	
/*
=================================================
	IsInnerPoint
=================================================
*/
	template <typename T>
	inline bool Rectangle<T>::IsInnerPoint (const vec2_t &point) const
	{
		return ( (point.x >= left)	 & (point.x <= right) &
				 (point.y >= bottom) & (point.y <= top) );
	}

/*
=================================================
	IsInnerRect
=================================================
*/
	template <typename T>
	inline bool Rectangle<T>::IsInnerRect (const Self &other) const
	{
		// this rect in other rect
		return ( (left >= other.left) & (right <= other.right) & (bottom >= other.bottom) & (top <= other.top) );
	}
	
/*
=================================================
	IntersectRect
=================================================
*/
	template <typename T>
	inline bool Rectangle<T>::IntersectRect (const Self &other) const
	{
		return !!( ( (left < other.right) & (right > other.left) & (bottom < other.top) & (top > other.bottom) ) |
				   ( (other.right < left) & (other.left > right) & (other.top < bottom) & (other.bottom > top) ) );
	}

/*
=================================================
	SetCenter
=================================================
*/
	template <typename T>
	inline Rectangle<T> & Rectangle<T>::SetCenter (const vec2_t &center)
	{
		const vec2_t	half_size = Size() / T(2);

		left	= center.x - half_size.x;
		bottom	= center.y - half_size.y;
		right	= center.x + half_size.x;
		top		= center.y + half_size.y;

		return *this;
	}
	
/*
=================================================
	SetSize
=================================================
*/
	template <typename T>
	inline Rectangle<T> & Rectangle<T>::SetSize (const vec2_t &size)
	{
		const vec2_t	center		= Center();
		const vec2_t	half_size	= size / T(2);
		
		left	= center.x - half_size.x;
		bottom	= center.y - half_size.y;
		right	= center.x + half_size.x;
		top		= center.y + half_size.y;

		return *this;
	}
	
/*
=================================================
	Join
=================================================
*/
	template <typename T>
	inline Rectangle<T> & Rectangle<T>::Join (const Self &other)
	{
		left	= GXMath::Min( left,	other.left );
		bottom	= GXMath::Min( bottom,	other.bottom );
		right	= GXMath::Max( right,	other.right );
		top		= GXMath::Max( top,		other.top );
		return *this;
	}
	
	template <typename T>
	inline Rectangle<T> & Rectangle<T>::Join (const vec2_t &point)
	{
		left	= GXMath::Min( left,	point.x );
		bottom	= GXMath::Min( bottom,	point.y );
		right	= GXMath::Max( right,	point.x );
		top		= GXMath::Max( top,		point.y );
		return *this;
	}
	
/*
=================================================
	Stretch
=================================================
*/
	template <typename T>
	inline Rectangle<T> & Rectangle<T>::Stretch (const vec2_t &size)
	{
		const vec2_t half_size = size / T(2);

		LeftBottom() -= half_size;
		RightTop()   += half_size;
		return *this;
	}
	
/*
=================================================
	Center
=================================================
*/
	template <typename T>
	inline Vec<T,2> Rectangle<T>::Center () const
	{
		return vec2_t( (right + left) / T(2), (top + bottom) / T(2) );
	}
	
/*
=================================================
	CropRect
=================================================
*/
	template <typename T>
	inline bool Rectangle<T>::CropRect (Self &other) const
	{
		// crop input rectangle
		//	 ----=====--
		//	|	|#####| | <-this
		//	|	|#####| |
		//	|	|#####| |
		//	 ----=====\-
		//   ^-other   ` result

		if ( IntersectRect( other ) )
		{
			if ( other.left   < left   )	other.left   = left;
			if ( other.bottom < bottom )	other.bottom = bottom;
			if ( other.right  > right  )	other.right  = right;
			if ( other.top    > top    )	other.top    = top;
			return true;
		}
		return false;
	}
	
/*
=================================================
	Sub
=================================================
*/
	template <typename T>
	inline bool Rectangle<T>::Sub (const Self &other, OUT Self &result0, OUT Self &result1) const
	{
		// this = this - other
		//			 -----===-
		//			|####|   || <- other
		//	this ->	|####|   ||
		//			|####|   ||
		//			/-----===-
		//		result

		TODO("Rect::Sub()");
		return false;
	}
	
/*
=================================================
	PointInBorder
=================================================
*
	template <typename T>
	inline EBorder::type Rectangle<T>::PointInBorder (const vec2_t &point, const T border) const
	{
		EBorder::type	ret = EBorder::OUTER;

		if ( IsInnerPoint( point ) )
		{
			ret  = EBorder::INNER;
			ret |= ( (point.x >= left)		& (point.x <= left   + border) ) ? EBorder::LEFT	: 0;	// left
			ret |= ( (point.y >= bottom)	& (point.y <= bottom + border) ) ? EBorder::BOTTOM	: 0;	// bottom
			ret |= ( (point.x <= right)		& (point.x >= right  - border) ) ? EBorder::RIGHT	: 0;	// right
			ret |= ( (point.y <= top)		& (point.y >= top    - border) ) ? EBorder::TOP		: 0;	// top
		}
		return ret;
	}
	
/*
=================================================
	Scale
=================================================
*/
	template <typename T>
	inline Rectangle<T> & Rectangle<T>::Scale (const vec2_t &scale)
	{
		const vec2_t	center	= Center();
		const vec2_t	size	= Size() * Abs(scale) / T(2);

		left	= center.x - size.x;
		bottom	= center.y - size.y;
		right	= center.x + size.x;
		top		= center.y + size.y;

		return *this;
	}
	
/*
=================================================
	Size
=================================================
*/
	template <typename T>
	inline Vec<T,2> Rectangle<T>::Size () const
	{
		return vec2_t( right - left, top - bottom );
	}
	
/*
=================================================
	RotateSinCos
=================================================
*
	template <typename T>
	inline void  Rectangle<T>::RotateSinCos (const vec2_t &sc, OUT vec2_t &p0, vec2_t &p1, vec2_t &p2, vec2_t &p3) const
	{
		const Matrix<T,2,2>	m	= Matrix<T,2,2>( sc[1], -sc[0], sc[0], sc[1] );

		p0 = m * (*this)(0);
		p1 = m * (*this)(1);
		p2 = m * (*this)(2);
		p3 = m * (*this)(3);
	}
	
/*
=================================================
	To
=================================================
*/
	template <typename T>
	template <typename B>
	inline const B  Rectangle<T>::To () const
	{
		STATIC_ASSERT( typename B::_is_rect(true), "type is not other" );

		return Convert< typename B::value_t >();
	}


#	undef	RECT_OPERATOR


}	// GXMath

namespace GXTypes
{
	
	template <typename T>
	struct Hash< GXMath::Rectangle<T> > :
		private Hash< typename GXMath::Rectangle<T>::vec4_t >
	{
		typedef GXMath::Rectangle<T>							key_t;
		typedef Hash< typename GXMath::Rectangle<T>::vec4_t >	base_t;
		typedef typename base_t::result_t						result_t;

		result_t operator () (const key_t &x) const noexcept
		{
			return base_t::operator ()( x.ToVec4() );
		}
	};

}	// GXTypes
}	// GX_STL
