// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
		Self &	operator _op_##= (const Vec2_t& other)			{ left _op_##= other.x;  bottom _op_##= other.y;  right _op_##= other.x;  top _op_##= other.y;  return *this; } \
		Self	operator _op_    (const Vec2_t& other) const	{ return Self( left _op_ other.x, bottom _op_ other.y, right _op_ other.x, top _op_ other.y ); } \
		\
		friend Self & operator _op_ (const T& lvalue, const Self& rvalue)		{ return Self( lvalue, lvalue, lvalue, lvalue ) _op_ rvalue; } \
		friend Self & operator _op_ (const Vec2_t& lvalue, const Self& rvalue)	{ return Self( lvalue.x, lvalue.y, lvalue.x, lvalue.y ) _op_ rvalue; } \
		

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

	template <typename T, ulong U>
	struct Rectangle : public CompileTime::CopyQualifiers< T >
	{
	// types
	public:
		using _is_rect		= bool;
		using Self			= Rectangle<T,U>;
		using Value_t		= T;
		using Vec2_t		= Vec<T,2,U>;
		using CVec2_t		= Vec2_t const;
		using Vec4_t		= Vec<T,4,U>;
		using Line_t		= Line2<T,U>;
		using Arr_t			= T[4];

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
		Rectangle (const Vec2_t &size) :					left(0), bottom(0), right(size.x), top(size.y) {}

		Rectangle (const Vec2_t &leftBottom, const Vec2_t &rightTop) :
			left(leftBottom.x), bottom(leftBottom.y),
			right(rightTop.x), top(rightTop.y)
		{}

		Rectangle (ArrayCRef<Vec2_t> points);

		// unary operators
		Self		operator - () const;
		Self		operator ~ () const;
		Self		operator ! () const;


		// binary operators
		Vec2_t		operator () (usize i)	const;

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
		T		*	ptr ()							{ _CheckAlign();  return &left; }
		const T *	ptr () const					{ _CheckAlign();  return &left; }

		CVec2_t		LeftBottom () const				{ return Vec2_t( left, bottom ); }
		Vec2_t &	LeftBottom ();

		CVec2_t		RightTop () const				{ return Vec2_t( right, top ); }
		Vec2_t &	RightTop ();

		CVec2_t		LeftTop () const				{ return Vec2_t( left, top ); }
		CVec2_t		RightBottom () const			{ return Vec2_t( right, bottom ); }

		Vec4_t		ToVec4 () const					{ return Vec4_t( left, bottom, right, top ); }

		Line_t		Left () const;
		Line_t		Right () const;
		Line_t		Bottom () const;
		Line_t		Top () const;
		//Line_t	ToLine (EEdge e) const;

		T			Width () const					{ return right - left; }
		T			Height () const					{ return top - bottom; }

		Self &		SetCenter (const Vec2_t &center);
		Self &		SetSize (const Vec2_t &size);

		Self &		Scale (T scale)					{ return Scale( Vec2_t(scale) ); }
		Self &		Scale (const Vec2_t &scale);

		Self &		Join (const Self &other);
		Self &		Join (const Vec2_t &point);

		Self &		Stretch (const Vec2_t &size);
		Self &		Stretch (T size)				{ return Stretch( Vec2_t( size ) ); }

		Vec2_t		Size ()		const;
		Vec2_t		Center ()	const;

		bool		IsNormalized () const;
		Self &		Repair ();

		// IsInner*		// TODO: rename
		bool  IsInnerLine (const Line_t &line) const;
		bool  IsInnerPoint (const Vec2_t &point) const;
		bool  IsInnerRect (const Self &other) const;

		// Intersect*
		bool  IntersectLine (const Line_t &line) const;
		bool  IntersectRect (const Self &other) const;

		// GetIntersection*
		bool  GetLineIntersection (const Line_t &line, OUT Vec2_t &result0, OUT Vec2_t &result1) const;

		// Crop*
		bool  CropRect (INOUT Self &other) const;
		bool  CropLine (INOUT Line_t &line) const;

		bool  Sub (const Self &other, OUT Self &result0, OUT Self &result1) const;

		//EBorder::type PointInBorder (const Vec2_t &point, T border) const;
		
		template <typename T2>
		Rectangle<T2,U>	Convert () const;

		template <typename B>
		const B		To () const;


	private:
		static void _CheckAlign ();
	};

	
	
	
//--------------------------- Math Func ---------------------------//
/*
=================================================
	Abs
=================================================
*/
	template <typename T, ulong U>
	ND_ inline Rectangle<T,U>  Abs (const Rectangle<T,U> &val)
	{
		return Rectangle<T,U>( Abs(val.left), Abs(val.bottom), Abs(val.right), Abs(val.top) );
	}
	
/*
=================================================
	IsZero
=================================================
*/
	template <typename T, ulong U>
	ND_ inline bool  IsZero (const Rectangle<T,U>& val)
	{
		return IsZero( val.Size() );
	}
	
/*
=================================================
	SetSign
=================================================
*/
	template <typename T, ulong U>
	ND_ inline Rectangle<T,U>  SetSign (const Rectangle<T,U> &val, bool bSign)
	{
		return Rectangle<T,U>(	SetSign( val.left,  bSign ),	SetSign( val.bottom, bSign ),
								SetSign( val.right, bSign ),	SetSign( val.top,	 bSign ) );
	}
	
/*
=================================================
	Mod
=================================================
*/
	template <typename T, ulong U>
	ND_ inline Rectangle<T,U>  Mod (const Rectangle<T,U>& left, const Rectangle<T,U>& right)
	{
		return Rectangle<T,U>(	Mod( left.left,  right.left ),	Mod( left.bottom, right.bottom ),
								Mod( left.right, right.right ), Mod( left.top,	  right.top ) );
	}
	
/*
=================================================
	Round
=================================================
*/
	template <typename T, ulong U>
	ND_ inline Rectangle<T,U>  Round (const Rectangle<T,U>& val)
	{
		return Rectangle<T,U>(	Round( val.left ),	Round( val.bottom ),
								Round( val.right ),	Round( val.top ) );
	}
	
/*
=================================================
	RoundToInt
=================================================
*/
	template <typename T, ulong U>
	ND_ inline Rectangle< typename CompileTime::NearInt::FromType<T> >  RoundToInt (const Rectangle<T,U>& val)
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
	template <typename T, ulong U>
	ND_ inline Rectangle< typename CompileTime::NearUInt::FromType<T> >  RoundToUInt (const Rectangle<T,U>& val)
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
	template <typename T, ulong U>
	ND_ inline Rectangle<T,U>  SafeDiv (const Rectangle<T,U>& left, const Rectangle<T,U>& right, const Rectangle<T,U>& defVal)
	{
		return Rectangle<T,U>(	SafeDiv( left.left,		right.left,		defVal.left ),
								SafeDiv( left.bottom,	right.bottom,	defVal.bottom ),
								SafeDiv( left.right,	right.right,	defVal.right ),
								SafeDiv( left.top,		right.top,		defVal.top ) );
	}

/*
=================================================
	Equals
=================================================
*/
	template <typename T, ulong U>
	ND_ inline constexpr Vec<bool,4,U>  Equals (const Rectangle<T,U> &a, const Rectangle<T,U> &b)
	{
		Vec<bool,4,U>	res;
		res[0] = Equals( a.left,   b.left   );
		res[1] = Equals( a.bottom, b.bottom );
		res[2] = Equals( a.right,  b.right  );
		res[3] = Equals( a.top,    b.top    );
		return res;
	}



//--------------------------- Rectangle -------------------------------//
/*
=================================================
	constructor
=================================================
*/
	template <typename T, ulong U>
	inline Rectangle<T,U>::Rectangle (ArrayCRef<Vec2_t> points) :
		left(0), bottom(0), right(0), top(0)
	{
		if ( points.Empty() )
			return;

		left = right = points.Front().x;
		bottom = top = points.Front().y;

		FORv( i, 1, points )
		{
			Vec2_t const&	p = points[i];

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
	template <typename T, ulong U>
	ND_ inline bool  Rectangle<T,U>::operator == (const Self &other) const
	{
		return ( (left  == other.left)  & (bottom == other.bottom) &
				 (right == other.right) & (top    == other.top) );
	}
	
/*
=================================================
	operator !=
=================================================
*/
	template <typename T, ulong U>
	ND_ inline bool  Rectangle<T,U>::operator != (const Self &other) const
	{
		return not (*this == other);
	}

/*
=================================================
	operator ()
=================================================
*/
	template <typename T, ulong U>
	ND_ inline Vec<T,2,U>  Rectangle<T,U>::operator () (usize i) const
	{
		ASSUME( i < 4 );
		return Vec2_t( (*this)[ i & 2 ], (*this)[ ((( (i & 1) + (i >> 1) ) & 1) << 1) + 1 ] );
	}
	
/*
=================================================
	LeftBottom
=================================================
*/
	template <typename T, ulong U>
	ND_ inline Vec<T,2,U> &  Rectangle<T,U>::LeftBottom ()
	{
		STATIC_ASSERT( sizeof(left) == (offsetof(Self, bottom) - offsetof(Self, left)) );
		return *PointerCast< Vec2_t >( &left );
	}
	
/*
=================================================
	RightTop
=================================================
*/	
	template <typename T, ulong U>
	ND_ inline Vec<T,2,U> &  Rectangle<T,U>::RightTop ()
	{
		STATIC_ASSERT( sizeof(right) == (offsetof(Self, top) - offsetof(Self, right)) );
		return *PointerCast< Vec2_t >( &right );
	}

/*
=================================================
	Convert
=================================================
*/
	template <typename T, ulong U>
	template <typename T2>
	ND_ inline Rectangle<T2,U>  Rectangle<T,U>::Convert () const
	{
		return Rectangle<T2,U>( T2(left), T2(bottom), T2(right), T2(top) );
	}
	
/*
=================================================
	IsInnerPoint
=================================================
*/
	template <typename T, ulong U>
	ND_ inline bool  Rectangle<T,U>::IsInnerPoint (const Vec2_t &point) const
	{
		return ( (point.x >= left)	 & (point.x <= right) &
				 (point.y >= bottom) & (point.y <= top) );
	}

/*
=================================================
	IsInnerRect
=================================================
*/
	template <typename T, ulong U>
	ND_ inline bool  Rectangle<T,U>::IsInnerRect (const Self &other) const
	{
		// this rect in other rect
		return ( (left >= other.left) & (right <= other.right) & (bottom >= other.bottom) & (top <= other.top) );
	}
	
/*
=================================================
	IntersectRect
=================================================
*/
	template <typename T, ulong U>
	ND_ inline bool  Rectangle<T,U>::IntersectRect (const Self &other) const
	{
		return !!( ( (left < other.right) & (right > other.left) & (bottom < other.top) & (top > other.bottom) ) |
				   ( (other.right < left) & (other.left > right) & (other.top < bottom) & (other.bottom > top) ) );
	}

/*
=================================================
	SetCenter
=================================================
*/
	template <typename T, ulong U>
	inline Rectangle<T,U> &  Rectangle<T,U>::SetCenter (const Vec2_t &center)
	{
		const Vec2_t	half_size = Size() / T(2);

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
	template <typename T, ulong U>
	inline Rectangle<T,U> &  Rectangle<T,U>::SetSize (const Vec2_t &size)
	{
		const Vec2_t	center		= Center();
		const Vec2_t	half_size	= size / T(2);
		
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
	template <typename T, ulong U>
	inline Rectangle<T,U> &  Rectangle<T,U>::Join (const Self &other)
	{
		left	= GXMath::Min( left,	other.left );
		bottom	= GXMath::Min( bottom,	other.bottom );
		right	= GXMath::Max( right,	other.right );
		top		= GXMath::Max( top,		other.top );
		return *this;
	}
	
	template <typename T, ulong U>
	inline Rectangle<T,U> &  Rectangle<T,U>::Join (const Vec2_t &point)
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
	template <typename T, ulong U>
	inline Rectangle<T,U> &  Rectangle<T,U>::Stretch (const Vec2_t &size)
	{
		const Vec2_t half_size = size / T(2);

		LeftBottom() -= half_size;
		RightTop()   += half_size;
		return *this;
	}
	
/*
=================================================
	Center
=================================================
*/
	template <typename T, ulong U>
	ND_ inline Vec<T,2,U>  Rectangle<T,U>::Center () const
	{
		return Vec2_t( (right + left) / T(2), (top + bottom) / T(2) );
	}
	
/*
=================================================
	CropRect
=================================================
*/
	template <typename T, ulong U>
	inline bool Rectangle<T,U>::CropRect (INOUT Self &other) const
	{
		// crop input rectangle
		//   --------- <---other
		//	|    -----|-
		//	|	|#####| | <-this
		//	|	|#####| |
		//	|	|#####|<|--result
		//	 ---|-----  |
		//      |_______|
		
		ASSERT( this->IsNormalized() and other.IsNormalized() );

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
	IsNormalized
=================================================
*/
	template <typename T, ulong U>
	inline bool  Rectangle<T,U>::IsNormalized () const
	{
		return (left <= right) & (bottom <= top);
	}
	
/*
=================================================
	Repair
=================================================
*/
	template <typename T, ulong U>
	inline Rectangle<T,U>&  Rectangle<T,U>::Repair ()
	{
		const auto	x = MinMax( left, right );
		const auto	y = MinMax( bottom, top );
		
		left	= x.min;
		right	= x.max;
		bottom	= y.min;
		top		= y.max;
		return *this;
	}

/*
=================================================
	Sub
=================================================
*/
	template <typename T, ulong U>
	inline bool Rectangle<T,U>::Sub (const Self &other, OUT Self &result0, OUT Self &result1) const
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
	template <typename T, ulong U>
	inline EBorder::type Rectangle<T,U>::PointInBorder (const Vec2_t &point, const T border) const
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
	template <typename T, ulong U>
	inline Rectangle<T,U> &  Rectangle<T,U>::Scale (const Vec2_t &scale)
	{
		const Vec2_t	center	= Center();
		const Vec2_t	size	= Size() * Abs(scale) / T(2);

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
	template <typename T, ulong U>
	ND_ inline Vec<T,2,U>  Rectangle<T,U>::Size () const
	{
		return Vec2_t( right - left, top - bottom );
	}
	
/*
=================================================
	RotateSinCos
=================================================
*
	template <typename T, ulong U>
	inline void  Rectangle<T,U>::RotateSinCos (const Vec2_t &sc, OUT Vec2_t &p0, Vec2_t &p1, Vec2_t &p2, Vec2_t &p3) const
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
	template <typename T, ulong U>
	template <typename B>
	ND_ inline const B  Rectangle<T,U>::To () const
	{
		STATIC_ASSERT( typename B::_is_rect(true), "type is not Rectangle" );
		using T2 = typename B::Value_t;
		return B( T2(left), T2(bottom), T2(right), T2(top) );
	}
	
/*
=================================================
	_CheckAlign
=================================================
*/
	template <typename T, ulong U>
	inline void Rectangle<T,U>::_CheckAlign ()
	{
		STATIC_ASSERT( offsetof(Self, left)  + sizeof(T) == offsetof(Self, top) );
		STATIC_ASSERT( offsetof(Self, top)   + sizeof(T) == offsetof(Self, right) );
		STATIC_ASSERT( offsetof(Self, right) + sizeof(T) == offsetof(Self, bottom) );
		STATIC_ASSERT( sizeof(T[3]) == (offsetof(Self, bottom) - offsetof(Self, left)) );
	}

#	undef	RECT_OPERATOR


}	// GXMath

namespace GXTypes
{
	
	template <typename T, ulong U>
	struct Hash< GXMath::Rectangle<T,U> >
	{
		ND_ HashResult  operator () (const GXMath::Rectangle<T,U> &x) const noexcept
		{
			return HashOf( x.ToVec4() );
		}
	};

}	// GXTypes
}	// GX_STL
