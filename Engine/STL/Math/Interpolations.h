// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Trigonometry.h"

namespace GX_STL
{
namespace GXMath
{

/*
=================================================
	InvLerp
----
	inverse linear interpolation
=================================================
*/
	template <typename T>
	inline CHECKRES(T)  InvLerp (const T& x, const T& y, const T& interpolated)
	{
		return (interpolated - x) / (y - x);
	}
	
/*
=================================================
	Lerp
----
	linear interpolation
=================================================
*/
	template <typename T, typename B>
	inline CHECKRES(T)  Lerp (const T& x, const T& y, const B& factor)
	{
		//return T(factor) * (y - x) + x;
		return x * (T(1) - T(factor)) + y * T(factor);
	}
	
/*
=================================================
	IncLerpToEnd
----
	incremental linear interpolation from current to end value.
=================================================
*/
	template <typename T, typename B>
	inline CHECKRES(T)  IncLerp (const T& x1, const T& endX, const B& factor1, const B& factor)
	{
		/*
		x1 = Lerp( startX, endX, factor1 ) = startX * (1 - factor1) + endX * factor1;
		x2 = Lerp( startX, endX, factor ) = startX * (1 - factor) + endX * factor;
		startX = (x1 - factor1 * endX) / (1 - factor1);
		x2 = (x1 - factor1 * endX) / (1 - factor1) * (1 - factor) + endX * factor;
		*/
		ASSERT( Equals( factor1, B(1) ) );
		const T k = T( (B(1) - factor) / (B(1) - factor1) );
		return (x1 - T(factor1) * endX) + endX * T(factor);
	}

/*
=================================================
	BiLerp
----
	bilinear interpolation
=================================================
*/
	template <typename T, typename B>
	inline CHECKRES(T)  BiLerp (const T& v0, const T& v1, const T& v2, const T& v3, const B& factorX, const B& factorY)
	{
		return Lerp( Lerp( v0, v3, factorX ), Lerp( v1, v2, factorX ), factorY );
	}

	template <typename T, typename B>
	inline CHECKRES(T)  BiLerp (const T& v0, const T& v1, const T& v2, const T& v3, const Vec<B,2> &factor)
	{
		return BiLerp( v0, v1, v2, v3, factor.x, factor.y );
	}
	
/*
=================================================
	InvBiLerp
----
	inverse bilinear interpolation
=================================================
*
	template <typename T, usize I, ulong U>
	inline Vec<T,2>  InvBiLerp (const Vec<T,I,U>& v0, const Vec<T,I,U>& v1, const Vec<T,I,U>& v2, const Vec<T,I,U>& v3, const Vec<T,I,U>& interpolated)
	{
	}
	*/

}	// GXMath
}	// GX_STL
