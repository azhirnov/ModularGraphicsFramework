// This is generated file, don't change anything!

#pragma once

#include "Engine/Shaders/Shader/Helpers.h"

namespace ShaderEditor
{
namespace ShaderNodes
{


	// Lerp, InvLerp, BiLerp
	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> Lerp (const NVec<T,I> &x, const NVec<T,I> &y, const NVec<T,J> &factor) {
		STATIC_ASSERT( I == J or J == 1 );
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "Lerp", x, y, factor );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> InvLerp (const NVec<T,I> &x, const NVec<T,I> &y, const NVec<T,J> &interpolated) {
		STATIC_ASSERT( I == J or J == 1 );
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "InvLerp", x, y, interpolated );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> BiLerp (const NVec<T,I> &v0, const NVec<T,I> &v1, const NVec<T,I> &v2, const NVec<T,I> &v3, const NVec<T,J> &factorX, const NVec<T,J> &factorY) {
		STATIC_ASSERT( I == J or J == 1 );
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "BiLerp", v0, v1, v2, v3, factorX, factorY );
	}


	// AlignToLarge
	template <typename T, uint I>
	inline NVec<T,I> AlignToLarge (const NVec<T,I> &x, const NVec<int,1> &align) {
		return Helpers::BuildNode< NVec<T,I> >( "AlignToLarge", x, align );
	}


	// Step, LinearStep, SmoothStep, BumpStep, SmoothBumpStep
	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> Step (const NVec<T,I> &x, const NVec<T,J> &edge0, const NVec<T,J> &edge1) {
		STATIC_ASSERT( I == J or J == 1 );
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "Step", x, edge0, edge1 );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> LinearStep (const NVec<T,I> &x, const NVec<T,J> &edge0, const NVec<T,J> &edge1) {
		STATIC_ASSERT( I == J or J == 1 );
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "LinearStep", x, edge0, edge1 );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> SmoothStep (const NVec<T,I> &x, const NVec<T,J> &edge0, const NVec<T,J> &edge1) {
		STATIC_ASSERT( I == J or J == 1 );
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "SmoothStep", x, edge0, edge1 );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> BumpStep (const NVec<T,I> &x, const NVec<T,J> &edge0, const NVec<T,J> &edge1) {
		STATIC_ASSERT( I == J or J == 1 );
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "BumpStep", x, edge0, edge1 );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> SmoothBumpStep (const NVec<T,I> &x, const NVec<T,J> &edge0, const NVec<T,J> &edge1) {
		STATIC_ASSERT( I == J or J == 1 );
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "SmoothBumpStep", x, edge0, edge1 );
	}



}	// ShaderNodes
}	// ShaderEditor
