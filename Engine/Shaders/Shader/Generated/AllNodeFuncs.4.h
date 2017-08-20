// This is generated file, don't change anything!

#pragma once

#include "Engine/Shaders/Shader/Helpers.h"

namespace ShaderEditor
{
namespace ShaderNodes
{


	// Pow, Ln, Log, Log2, Log10, Exp, Exp2, Exp10, RoundTo
	template <typename T, uint I>
	inline NVec<T,I> Ln (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Ln", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> Log2 (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Log2", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> Log10 (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Log10", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> Exp (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Exp", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> Exp2 (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Exp2", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> Exp10 (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Exp10", x );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> Pow (const NVec<T,I> &x, const NVec<T,J> &base) {
		STATIC_ASSERT( I == J or J == 1 );
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "Pow", x, base );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> Log (const NVec<T,I> &x, const NVec<T,J> &base) {
		STATIC_ASSERT( I == J or J == 1 );
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "Log", x, base );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> RoundTo (const NVec<T,I> &x, const NVec<T,J> &base) {
		STATIC_ASSERT( I == J or J == 1 );
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "RoundTo", x, base );
	}


	// IntLog2, CeilPowerOfTwo, FloorPowerOfTwo, NearPowerOfTwo, IsPowerOfTwo
	template <typename T, uint I>
	inline NVec<T,I> CeilPowerOfTwo (const NVec<T,I> &x) {
		Helpers::IntegerOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "CeilPowerOfTwo", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> FloorPowerOfTwo (const NVec<T,I> &x) {
		Helpers::IntegerOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "FloorPowerOfTwo", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> NearPowerOfTwo (const NVec<T,I> &x) {
		Helpers::IntegerOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "NearPowerOfTwo", x );
	}

	template <typename T, uint I>
	inline NVec<bool,I> IsPowerOfTwo (const NVec<T,I> &x) {
		Helpers::IntegerOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<bool,I> >( "IsPowerOfTwo", x );
	}


	// Sqrt, InvSqrt
	template <typename T, uint I>
	inline NVec<T,I> Sqrt (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Sqrt", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> InvSqrt (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "InvSqrt", x );
	}



}	// ShaderNodes
}	// ShaderEditor
