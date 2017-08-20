// This is generated file, don't change anything!

#pragma once

#include "Engine/Shaders/Shader/Helpers.h"

namespace ShaderEditor
{
namespace ShaderNodes
{


	// Abs, Square, SquareSign, Sign, SignOrZero
	template <typename T, uint I>
	inline NVec<T,I> Abs (const NVec<T,I> &x) {
		Helpers::IntegerOrFloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Abs", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> Square (const NVec<T,I> &x) {
		Helpers::IntegerOrFloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Square", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> Sign (const NVec<T,I> &x) {
		Helpers::IntegerOrFloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Sign", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> SignOrZero (const NVec<T,I> &x) {
		Helpers::IntegerOrFloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "SignOrZero", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> SquareSign (const NVec<T,I> &x) {
		Helpers::IntegerOrFloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "SquareSign", x );
	}


	// SafeDiv, SafeMod
	template <typename T, uint I>
	inline NVec<T,I> SafeDiv (const NVec<T,I> &x, const NVec<T,I> &y, const NVec<T,I> &def) {
		Helpers::IntegerOrFloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "SafeDiv", x, y, def );
	}

	template <typename T, uint I>
	inline NVec<T,I> SafeMod (const NVec<T,I> &x, const NVec<T,I> &y, const NVec<T,I> &def) {
		Helpers::IntegerOrFloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "SafeMod", x, y, def );
	}


	// IsZero, IsNotZero, IsOdd, IsEven
	template <typename T, uint I>
	inline NVec<bool,I> IsZero (const NVec<T,I> &x) {
		Helpers::IntegerOrFloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<bool,I> >( "IsZero", x );
	}

	template <typename T, uint I>
	inline NVec<bool,I> IsNotZero (const NVec<T,I> &x) {
		Helpers::IntegerOrFloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<bool,I> >( "IsNotZero", x );
	}

	template <typename T, uint I>
	inline NVec<bool,I> IsOdd (const NVec<T,I> &x) {
		Helpers::IntegerOrFloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<bool,I> >( "IsOdd", x );
	}

	template <typename T, uint I>
	inline NVec<bool,I> IsEven (const NVec<T,I> &x) {
		Helpers::IntegerOrFloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<bool,I> >( "IsEven", x );
	}


	// Floor, Ceil, Trunc, Fract, Round, RoundEven, RoundToInt, RoundEvenToInt
	template <typename T, uint I>
	inline NVec<T,I> Floor (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Floor", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> Ceil (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Ceil", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> Trunc (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Trunc", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> Round (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Round", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> RoundEven (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "RoundEven", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> RoundToInt (const NVec<T,I> &x) {
		Helpers::IntegerOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "RoundToInt", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> RoundEvenToInt (const NVec<T,I> &x) {
		Helpers::IntegerOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "RoundEvenToInt", x );
	}



}	// ShaderNodes
}	// ShaderEditor
