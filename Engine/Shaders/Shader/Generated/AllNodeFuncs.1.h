// This is generated file, don't change anything!

#pragma once

#include "Engine/Shaders/Shader/Helpers.h"

namespace ShaderEditor
{
namespace ShaderNodes
{

	// Math operators
	template <typename T, uint I>
	inline NVec<T,I> operator ! (const NVec<T,I> &right) {
		Helpers::BooleanOnly<NVec<T,I>>();
		return Helpers::BuildNode< NVec<T,I> >( "Not", right );
	}

	template <typename T, uint I>
	inline NVec<T,I> operator ~ (const NVec<T,I> &right) {
		Helpers::IntegerOnly<NVec<T,I>>();
		return Helpers::BuildNode< NVec<T,I> >( "BitInverse", right );
	}

	template <typename T, uint I>
	inline NVec<T,I> operator + (const NVec<T,I> &right) {
		Helpers::IntegerOrFloatOnly<NVec<T,I>>();
		return Helpers::BuildNode< NVec<T,I> >( "Plus", right );
	}

	template <typename T, uint I>
	inline NVec<T,I> operator - (const NVec<T,I> &right) {
		Helpers::IntegerOrFloatOnly<NVec<T,I>>();
		return Helpers::BuildNode< NVec<T,I> >( "Minus", right );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> operator + (const NVec<T,I> &left, const NVec<T,J> &right) {
		Helpers::IntegerOrFloatOnly<Helpers::MaxNVec<T,I,J>>();
		STATIC_ASSERT( I == J or I == 1 or J == 1 );
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "Add", left, right );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> operator - (const NVec<T,I> &left, const NVec<T,J> &right) {
		Helpers::IntegerOrFloatOnly<Helpers::MaxNVec<T,I,J>>();
		STATIC_ASSERT( I == J or I == 1 or J == 1 );
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "Sub", left, right );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> operator * (const NVec<T,I> &left, const NVec<T,J> &right) {
		Helpers::IntegerOrFloatOnly<Helpers::MaxNVec<T,I,J>>();
		STATIC_ASSERT( I == J or I == 1 or J == 1 );
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "Mul", left, right );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> operator / (const NVec<T,I> &left, const NVec<T,J> &right) {
		Helpers::IntegerOrFloatOnly<Helpers::MaxNVec<T,I,J>>();
		STATIC_ASSERT( I == J or I == 1 or J == 1 );
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "Div", left, right );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> operator % (const NVec<T,I> &left, const NVec<T,J> &right) {
		Helpers::IntegerOrFloatOnly<Helpers::MaxNVec<T,I,J>>();
		STATIC_ASSERT( I == J or I == 1 or J == 1 );
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "Mod", left, right );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> operator & (const NVec<T,I> &left, const NVec<T,J> &right) {
		Helpers::IntegerOnly<Helpers::MaxNVec<T,I,J>>();
		STATIC_ASSERT( I == J or I == 1 or J == 1 );
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "And", left, right );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> operator | (const NVec<T,I> &left, const NVec<T,J> &right) {
		Helpers::IntegerOnly<Helpers::MaxNVec<T,I,J>>();
		STATIC_ASSERT( I == J or I == 1 or J == 1 );
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "Or", left, right );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> operator ^ (const NVec<T,I> &left, const NVec<T,J> &right) {
		Helpers::IntegerOnly<Helpers::MaxNVec<T,I,J>>();
		STATIC_ASSERT( I == J or I == 1 or J == 1 );
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "Xor", left, right );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> operator << (const NVec<T,I> &left, const NVec<T,J> &right) {
		Helpers::IntegerOnly<Helpers::MaxNVec<T,I,J>>();
		STATIC_ASSERT( I == J or I == 1 or J == 1 );
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "BitLShift", left, right );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> operator >> (const NVec<T,I> &left, const NVec<T,J> &right) {
		Helpers::IntegerOnly<Helpers::MaxNVec<T,I,J>>();
		STATIC_ASSERT( I == J or I == 1 or J == 1 );
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "BitRShift", left, right );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> operator && (const NVec<T,I> &left, const NVec<T,J> &right) {
		Helpers::BooleanOnly<Helpers::MaxNVec<T,I,J>>();
		STATIC_ASSERT( I == J or I == 1 or J == 1 );
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "BoolAnd", left, right );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<T,I,J> operator || (const NVec<T,I> &left, const NVec<T,J> &right) {
		Helpers::BooleanOnly<Helpers::MaxNVec<T,I,J>>();
		STATIC_ASSERT( I == J or I == 1 or J == 1 );
		return Helpers::BuildNode< Helpers::MaxNVec<T,I,J> >( "BoolOr", left, right );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<bool,I,J> operator == (const NVec<T,I> &left, const NVec<T,J> &right) {
		STATIC_ASSERT( I == J or I == 1 or J == 1 );
		return Helpers::BuildNode< Helpers::MaxNVec<bool,I,J> >( "Equal", left, right );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<bool,I,J> operator != (const NVec<T,I> &left, const NVec<T,J> &right) {
		STATIC_ASSERT( I == J or I == 1 or J == 1 );
		return Helpers::BuildNode< Helpers::MaxNVec<bool,I,J> >( "NotEqual", left, right );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<bool,I,J> operator > (const NVec<T,I> &left, const NVec<T,J> &right) {
		Helpers::IntegerOrFloatOnly<Helpers::MaxNVec<bool,I,J>>();
		STATIC_ASSERT( I == J or I == 1 or J == 1 );
		return Helpers::BuildNode< Helpers::MaxNVec<bool,I,J> >( "GreaterThan", left, right );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<bool,I,J> operator < (const NVec<T,I> &left, const NVec<T,J> &right) {
		Helpers::IntegerOrFloatOnly<Helpers::MaxNVec<bool,I,J>>();
		STATIC_ASSERT( I == J or I == 1 or J == 1 );
		return Helpers::BuildNode< Helpers::MaxNVec<bool,I,J> >( "LessThan", left, right );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<bool,I,J> operator >= (const NVec<T,I> &left, const NVec<T,J> &right) {
		Helpers::IntegerOrFloatOnly<Helpers::MaxNVec<bool,I,J>>();
		STATIC_ASSERT( I == J or I == 1 or J == 1 );
		return Helpers::BuildNode< Helpers::MaxNVec<bool,I,J> >( "GreaterThanEqual", left, right );
	}

	template <typename T, uint I, uint J>
	inline Helpers::MaxNVec<bool,I,J> operator <= (const NVec<T,I> &left, const NVec<T,J> &right) {
		Helpers::IntegerOrFloatOnly<Helpers::MaxNVec<bool,I,J>>();
		STATIC_ASSERT( I == J or I == 1 or J == 1 );
		return Helpers::BuildNode< Helpers::MaxNVec<bool,I,J> >( "LessThanEqual", left, right );
	}



}	// ShaderNodes
}	// ShaderEditor
