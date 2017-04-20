// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "MathTypes.h"

namespace GX_STL
{
namespace GXMath
{

	//
	// Trigonometry
	//
	
	template <typename T>				inline T			Sin (const Radians<T>& x);
	template <typename T>				inline T			Cos (const Radians<T>& x);

	template <typename T>				inline Radians<T>	ASin (const T& x);
	template <typename T>				inline Radians<T>	ACos (const T& x);

	template <typename T>				inline T			SinH (const Radians<T>& x);
	template <typename T>				inline T			CosH (const Radians<T>& x);

	template <typename T>				inline Radians<T>	ASinH (const T& x);
	template <typename T>				inline Radians<T>	ACosH (const T& x);

	template <typename T>				inline T			Tan (const Radians<T>& x);
	template <typename T>				inline T			CoTan (const Radians<T>& x);
	
	template <typename T>				inline T			TanH (const Radians<T>& x);
	template <typename T>				inline T			CoTanH (const Radians<T>& x);

	template <typename T>				inline Radians<T>	ATan (const T& y_over_x);
	template <typename T>				inline Radians<T>	ATan (const T& y, const T& x);
	template <typename T>				inline Radians<T>	ACoTan (const T& x);

	template <typename T>				inline Radians<T>	ATanH (const T& x);
	template <typename T>				inline Radians<T>	ACoTanH (const T& x);
	


	//
	// Compare operations
	//
	
	template <typename T>				inline bool		All (const T& x);
	template <typename T>				inline bool		Any (const T& x);
	//template <typename T>				inline bool		Most (const T& x);
	template <typename T>				inline bool		IsZero (const T& x);
	template <typename T>				inline bool		IsNotZero (const T& x);
	template <typename T>				inline bool		Equals (const T& a, const T& b);
	


	//
	// Operation with sign
	//

	template <typename T>				inline T		Abs (const T& x);
	template <typename T>				inline T		SetSign (const T &x, bool bSign = true);
	template <typename T>				inline T		CopySign (const T& from, const T& to);
	template <typename T>				inline T		Sign (const T& x);
	template <typename T>				inline T		SignOrZero (const T& x);



	//
	// Pow, Exp
	//

	template <typename T, typename B>	inline T		Pow (const T&x, const B& base);
	template <typename T>				inline T		Ln (const T& x);
	template <typename T>				inline T		Log (const T& x, const T& base);
	template <typename T>				inline T		Log2 (const T& x);
	template <typename T>				inline T		Log10 (const T& x);
	template <typename T>				inline T		Exp (const T& x);
	template <typename T>				inline T		Exp2 (const T& x);
	template <typename T>				inline T		Exp10 (const T& x);
	template <typename T>				inline uint		IntLog2 (const T &x);
	template <typename T>				inline T		CeilPowerOfTwo (const T& x);
	template <typename T>				inline T		FloorPowerOfTwo (const T& x);
	template <typename T>				inline T		NearPowerOfTwo (const T& x);
	template <typename T>				inline bool		IsPowerOfTwo (const T& x);
	template <typename T>				inline T		Square (const T& x);
	template <typename T>				inline T		Sqrt (const T& x);
	template <typename T>				inline T		SquareSign (const T& x);
	template <typename T>				inline T		InvSqrt (const T& x);



	//
	// Interpolation
	//

	template <typename T, typename B>	inline T		Lerp (const T& x, const T& y, const B& factor);
	template <typename T>				inline T		InvLerp (const T& x, const T& y, const T& interpolated);
	template <typename T, typename B>	inline T		BiLerp (const T& v0, const T& v1, const T& v2, const T& v3, const B& factorX, const B& factorY);


	
	//
	// Other
	//

	template <typename T>				inline T		Floor (const T& x);
	template <typename T>				inline T		Ceil (const T& x);
	template <typename T>				inline T		Fract (const T& x);
	template <typename T>				inline T		Trunc (const T& x);

	template <typename T>				inline T		Round (const T& x);
	template <typename R, typename T>	inline R		RoundTo (const T& x);
	template <typename T>				inline T		RoundTo (const T& x, const T& base);

	template <typename T>				inline bool		IsOdd (const T& x);
	template <typename T>				inline bool		IsEven (const T& x);

	template <typename T>				inline T		SafeDiv (const T& left, const T& right, const T& defVal);

	template <typename T>				inline T		IntFactorial (const T& x);
	template <typename T>				inline T		IntSuperFactorial (const T& x);
	template <typename T>				inline T		IntHyperFactorial (const T& x);
	template <typename T>				inline T		IntDoubleFactorial (const T& x);
	template <typename T>				inline T		IntGammaFunction (const T& x);
	template <typename T>				inline T		IntLnGammaFunction (const T& x);

	template <typename T>				inline T		Factorial (const T& x);
	template <typename T>				inline T		Gamma (const T& x);
	template <typename T>				inline T		LnGamma (const T& x);

	template <typename T>				inline T		Step (const T& x, const T& edge);
	template <typename T>				inline T		LinearStep (const T& x, const T& edge0, const T& edge1);		//	/ 
	template <typename T>				inline T		SmoothStep (const T& x, const T& edge0, const T& edge1);		//	/ smooth
	template <typename T>				inline T		BumpStep (const T& x, const T& edge0, const T& edge1);			//  /\/\/\ 
	template <typename T>				inline T		SmoothBumpStep (const T& x, const T& edge0, const T& edge1);	//	/\/\/\ smooth
	


	//
	// Bit Operators
	//

	template <typename T>				inline T		ToBit (Bits<usize> index);
	template <typename T>				inline T		ToMask (Bits<usize> index);
	template <typename T>				inline T		ToMask (Bits<usize> first, Bits<usize> last);
	template <typename T>				inline T		GetMaskForType (const T &type = T());

	template <typename T>				inline T		SafeLeftBitShift (const T& x, Bits<usize> shift);	// <<
	template <typename T>				inline T		SafeRightBitShift (const T& x, Bits<usize> shift);	// >>
	
	template <typename T>				inline T		BitRotateLeft (const T& x, Bits<usize> shift);
	template <typename T>				inline T		BitRotateRight (const T& x, Bits<usize> shift);
	
	template <typename T>				inline T		ReverseBitOrder (const T& x);

	template <typename T>				inline Bits<usize>	BitScanReverse (const T& x);	// clz, findMSB
	template <typename T>				inline Bits<usize>	BitScanForward (const T& x);	// ctz, findLSB



	//
	// AlignToLarge
	//
	
	template <typename T>			constexpr T AlignToLarge (const T& value, usize align);


}	// GXMath
}	// GX_STL
