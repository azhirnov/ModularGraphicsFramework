// This is generated file, don't change anything!

#pragma once

#include "Engine/Shaders/Shader/Helpers.h"

namespace ShaderEditor
{
namespace ShaderNodes
{


	// SafeLeftBitShift, SafeRightBitShift, BitRotateLeft, BitRotateRight
	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> SafeLeftBitShift (const NVec<T,I> &x, const NVec<T,J> &shift) {
		STATIC_ASSERT( I == J or J == 1 );
		Helpers::IntegerOnly< NVec<T,I> >();
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "SafeLeftBitShift", x, shift );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> SafeRightBitShift (const NVec<T,I> &x, const NVec<T,J> &shift) {
		STATIC_ASSERT( I == J or J == 1 );
		Helpers::IntegerOnly< NVec<T,I> >();
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "SafeRightBitShift", x, shift );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> BitRotateLeft (const NVec<T,I> &x, const NVec<T,J> &shift) {
		STATIC_ASSERT( I == J or J == 1 );
		Helpers::IntegerOnly< NVec<T,I> >();
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "BitRotateLeft", x, shift );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> BitRotateRight (const NVec<T,I> &x, const NVec<T,J> &shift) {
		STATIC_ASSERT( I == J or J == 1 );
		Helpers::IntegerOnly< NVec<T,I> >();
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "BitRotateRight", x, shift );
	}


	// ReverseBitOrder, BitScanReverse, BitScanForward
	template <typename T, uint I>
	inline NVec<T,I> ReverseBitOrder (const NVec<T,I> &x) {
		Helpers::IntegerOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "ReverseBitOrder", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> BitScanReverse (const NVec<T,I> &x) {
		Helpers::IntegerOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "BitScanReverse", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> BitScanForward (const NVec<T,I> &x) {
		Helpers::IntegerOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "BitScanForward", x );
	}


	// Clamp, ClampOut, Wrap
	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> Clamp (const NVec<T,I> &x, const NVec<T,J> &min, const NVec<T,J> &max) {
		STATIC_ASSERT( I == J or J == 1 );
		Helpers::IntegerOnly< NVec<T,I> >();
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "Clamp", x, min, max );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> ClampOut (const NVec<T,I> &x, const NVec<T,J> &min, const NVec<T,J> &max) {
		STATIC_ASSERT( I == J or J == 1 );
		Helpers::IntegerOnly< NVec<T,I> >();
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "ClampOut", x, min, max );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> Wrap (const NVec<T,I> &x, const NVec<T,J> &min, const NVec<T,J> &max) {
		STATIC_ASSERT( I == J or J == 1 );
		Helpers::IntegerOnly< NVec<T,I> >();
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "Wrap", x, min, max );
	}


	// Min, Max, MinAbs, MaxAbs, MinMag, MaxMag, Mid
	template <typename T, uint I>
	inline NVec<T,I> Min (const NVec<T,I> &x, const NVec<T,I> &y) {
		Helpers::IntegerOrFloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Min", x, y );
	}

	template <typename T, uint I>
	inline NVec<T,I> Max (const NVec<T,I> &x, const NVec<T,I> &y) {
		Helpers::IntegerOrFloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Max", x, y );
	}

	template <typename T, uint I>
	inline NVec<T,I> MinAbs (const NVec<T,I> &x, const NVec<T,I> &y) {
		Helpers::IntegerOrFloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "MinAbs", x, y );
	}

	template <typename T, uint I>
	inline NVec<T,I> MaxAbs (const NVec<T,I> &x, const NVec<T,I> &y) {
		Helpers::IntegerOrFloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "MaxAbs", x, y );
	}

	template <typename T, uint I>
	inline NVec<T,I> MinMag (const NVec<T,I> &x, const NVec<T,I> &y) {
		Helpers::IntegerOrFloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "MinMag", x, y );
	}

	template <typename T, uint I>
	inline NVec<T,I> MaxMag (const NVec<T,I> &x, const NVec<T,I> &y) {
		Helpers::IntegerOrFloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "MaxMag", x, y );
	}

	template <typename T, uint I>
	inline NVec<T,I> Mid (const NVec<T,I> &x, const NVec<T,I> &y, const NVec<T,I> &z) {
		Helpers::IntegerOrFloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Mid", x, y, z );
	}


	// Dot, Cross, Length, Distance, Normalize, FaceForward, OuterProduct, Reflect, Refract
	template <typename T, uint I>
	inline NVec<T,I> Length (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Length", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> Normalize (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Normalize", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> Dot (const NVec<T,I> &x, const NVec<T,I> &y) {
		Helpers::FloatOnly< NVec<T,I> >();
		STATIC_ASSERT( I >= 2 );
		return Helpers::BuildNode< NVec<T,I> >( "Dot", x, y );
	}

	template <typename T, uint I>
	inline NVec<T,I> Cross (const NVec<T,I> &x, const NVec<T,I> &y) {
		Helpers::FloatOnly< NVec<T,I> >();
		STATIC_ASSERT( I == 3 );
		return Helpers::BuildNode< NVec<T,I> >( "Cross", x, y );
	}

	template <typename T, uint I>
	inline NVec<T,I> Distance (const NVec<T,I> &x, const NVec<T,I> &y) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Distance", x, y );
	}

	template <typename T, uint I>
	inline NVec<T,I> Reflect (const NVec<T,I> &x, const NVec<T,I> &y) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Reflect", x, y );
	}

	template <typename T, uint I>
	inline NVec<T,I> Refract (const NVec<T,I> &vI, const NVec<T,I> &vN, const NVec<T,1> &eta) {
		STATIC_ASSERT( I == 2 or I == 3 );
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Refract", vI, vN, eta );
	}

	template <typename T, uint I>
	inline NVec<T,I> FaceForward (const NVec<T,I> &vN, const NVec<T,I> &vI, const NVec<T,I> &Nref) {
		return Helpers::BuildNode< NVec<T,I> >( "FaceForward", vN, vI, Nref );
	}



}	// ShaderNodes
}	// ShaderEditor
