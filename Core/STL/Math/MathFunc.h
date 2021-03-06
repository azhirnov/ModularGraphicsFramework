// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Math/MathTypes.h"
#include "Core/STL/CompileTime/TypeListHelpers.h"

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
	
	template <typename T>			constexpr bool		All (const T& x);
	template <typename T>			constexpr bool		Any (const T& x);
	template <typename T>			constexpr bool		IsZero (const T& x);
	template <typename T>			constexpr bool		IsNotZero (const T& x);
	template <typename T>			constexpr bool		Equals (const T& a, const T& b);
	


	//
	// Operation with sign
	//

	template <typename T>				constexpr T		Abs (const T& x);
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
	template <typename T>				constexpr int	IntLog2 (const T &x);
	template <typename T>				constexpr T		CeilPowerOfTwo (const T& x);
	template <typename T>				constexpr T		FloorPowerOfTwo (const T& x);
	template <typename T>				constexpr T		NearPowerOfTwo (const T& x);
	template <typename T>				constexpr bool	IsPowerOfTwo (const T& x);
	template <typename T>				constexpr T		Square (const T& x);
	template <typename T>				inline T		Sqrt (const T& x);
	template <typename T>				inline T		SquareSign (const T& x);
	template <typename T>				inline T		InvSqrt (const T& x);



	//
	// Interpolation
	//

	template <typename T, typename B>	inline T		Lerp (const T& x, const T& y, const B& factor);
	template <typename T, typename B>	inline T		IncLerp (const T& x1, const T& endX, const B& factor1, const B& factor);
	template <typename T>				inline T		InvLerp (const T& x, const T& y, const T& interpolated);
	template <typename T, typename B>	inline T		BiLerp (const T& v0, const T& v1, const T& v2, const T& v3, const B& factorX, const B& factorY);


	
	//
	// Other
	//

	template <typename T>				constexpr T		Floor (const T& x);
	template <typename T>				constexpr T		Ceil (const T& x);
	template <typename T>				constexpr T		Fract (const T& x);
	template <typename T>				constexpr T		Trunc (const T& x);

	template <typename T>				constexpr T		Round (const T& x);
	template <typename R, typename T>	constexpr R		RoundTo (const T& x);
	template <typename T>				constexpr T		RoundTo (const T& x, const T& base);

	template <typename T>				constexpr bool	IsOdd (const T& x);
	template <typename T>				constexpr bool	IsEven (const T& x);

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
	template <typename T>				inline T		BumpStep (const T& x, const T& edge0, const T& edge1);			//  /\/\/\ - bump step
	template <typename T>				inline T		SmoothBumpStep (const T& x, const T& edge0, const T& edge1);	//	/\/\/\ - smooth bump step
	


	//
	// Bit Operators
	//

	template <typename T>				constexpr T		ToBit (BitsU index);
	template <typename T>				constexpr T		ToMask (BitsU index);
	template <typename T>				constexpr T		ToMask (BitsU first, BitsU last);
	template <typename T>				constexpr T		GetMaskForType (const T &type = T());

	template <typename T>				constexpr T		SafeLeftBitShift (const T& x, BitsU shift);	// <<
	template <typename T>				constexpr T		SafeRightBitShift (const T& x, BitsU shift);	// >>
	
	template <typename T>				constexpr T		BitRotateLeft (const T& x, BitsU shift);
	template <typename T>				constexpr T		BitRotateRight (const T& x, BitsU shift);
	
	template <typename T>				constexpr T		ReverseBitOrder (const T& x);

	template <typename T>		constexpr BitsU	BitScanReverse (const T& x);	// findMSB
	template <typename T>		constexpr BitsU	BitScanForward (const T& x);	// findLSB



	//
	// AlignToLarge
	//
	
	template <typename T, typename S>	constexpr T		AlignToLarge (const T& value, const S& align);

	
	template <typename A, typename B>				auto	Min (const A& a, const B& b);
	template <typename A, typename B>				auto	Max (const A& a, const B& b);
	template <typename A, typename B, typename C>	auto	Wrap (const A& value, const B& minValue, const C& maxValue);
	template <typename A, typename B, typename C>	auto	MirroredWrap (const A& value, const B& minValue, const C& maxValue);
	template <typename A, typename B>				auto	Mod (const A& left, const B& right);


}	// GXMath
}	// GX_STL
