// This is generated file, don't change anything!

#pragma once

#include "Engine/Shaders/Shader/Helpers.h"

namespace ShaderEditor
{
namespace ShaderNodes
{


	// Type reinterpret cast
	template <typename T, uint I>
	inline Int AsInt (const NVec<T,I> &x) {
		STATIC_ASSERT( (sizeof(typeName::Value_t) * typeName::COUNT) == (sizeof(T) * I) );
		return Helpers::BuildNode< Int >( "AsInt", x );
	}

	template <typename T, uint I>
	inline Int2 AsInt2 (const NVec<T,I> &x) {
		STATIC_ASSERT( (sizeof(typeName::Value_t) * typeName::COUNT) == (sizeof(T) * I) );
		return Helpers::BuildNode< Int2 >( "AsInt2", x );
	}

	template <typename T, uint I>
	inline Int3 AsInt3 (const NVec<T,I> &x) {
		STATIC_ASSERT( (sizeof(typeName::Value_t) * typeName::COUNT) == (sizeof(T) * I) );
		return Helpers::BuildNode< Int3 >( "AsInt3", x );
	}

	template <typename T, uint I>
	inline Int4 AsInt4 (const NVec<T,I> &x) {
		STATIC_ASSERT( (sizeof(typeName::Value_t) * typeName::COUNT) == (sizeof(T) * I) );
		return Helpers::BuildNode< Int4 >( "AsInt4", x );
	}

	template <typename T, uint I>
	inline UInt AsUInt (const NVec<T,I> &x) {
		STATIC_ASSERT( (sizeof(typeName::Value_t) * typeName::COUNT) == (sizeof(T) * I) );
		return Helpers::BuildNode< UInt >( "AsUInt", x );
	}

	template <typename T, uint I>
	inline UInt2 AsUInt2 (const NVec<T,I> &x) {
		STATIC_ASSERT( (sizeof(typeName::Value_t) * typeName::COUNT) == (sizeof(T) * I) );
		return Helpers::BuildNode< UInt2 >( "AsUInt2", x );
	}

	template <typename T, uint I>
	inline UInt3 AsUInt3 (const NVec<T,I> &x) {
		STATIC_ASSERT( (sizeof(typeName::Value_t) * typeName::COUNT) == (sizeof(T) * I) );
		return Helpers::BuildNode< UInt3 >( "AsUInt3", x );
	}

	template <typename T, uint I>
	inline UInt4 AsUInt4 (const NVec<T,I> &x) {
		STATIC_ASSERT( (sizeof(typeName::Value_t) * typeName::COUNT) == (sizeof(T) * I) );
		return Helpers::BuildNode< UInt4 >( "AsUInt4", x );
	}

	template <typename T, uint I>
	inline ILong AsILong (const NVec<T,I> &x) {
		STATIC_ASSERT( (sizeof(typeName::Value_t) * typeName::COUNT) == (sizeof(T) * I) );
		return Helpers::BuildNode< ILong >( "AsILong", x );
	}

	template <typename T, uint I>
	inline ILong2 AsILong2 (const NVec<T,I> &x) {
		STATIC_ASSERT( (sizeof(typeName::Value_t) * typeName::COUNT) == (sizeof(T) * I) );
		return Helpers::BuildNode< ILong2 >( "AsILong2", x );
	}

	template <typename T, uint I>
	inline ILong3 AsILong3 (const NVec<T,I> &x) {
		STATIC_ASSERT( (sizeof(typeName::Value_t) * typeName::COUNT) == (sizeof(T) * I) );
		return Helpers::BuildNode< ILong3 >( "AsILong3", x );
	}

	template <typename T, uint I>
	inline ILong4 AsILong4 (const NVec<T,I> &x) {
		STATIC_ASSERT( (sizeof(typeName::Value_t) * typeName::COUNT) == (sizeof(T) * I) );
		return Helpers::BuildNode< ILong4 >( "AsILong4", x );
	}

	template <typename T, uint I>
	inline ULong AsULong (const NVec<T,I> &x) {
		STATIC_ASSERT( (sizeof(typeName::Value_t) * typeName::COUNT) == (sizeof(T) * I) );
		return Helpers::BuildNode< ULong >( "AsULong", x );
	}

	template <typename T, uint I>
	inline ULong2 AsULong2 (const NVec<T,I> &x) {
		STATIC_ASSERT( (sizeof(typeName::Value_t) * typeName::COUNT) == (sizeof(T) * I) );
		return Helpers::BuildNode< ULong2 >( "AsULong2", x );
	}

	template <typename T, uint I>
	inline ULong3 AsULong3 (const NVec<T,I> &x) {
		STATIC_ASSERT( (sizeof(typeName::Value_t) * typeName::COUNT) == (sizeof(T) * I) );
		return Helpers::BuildNode< ULong3 >( "AsULong3", x );
	}

	template <typename T, uint I>
	inline ULong4 AsULong4 (const NVec<T,I> &x) {
		STATIC_ASSERT( (sizeof(typeName::Value_t) * typeName::COUNT) == (sizeof(T) * I) );
		return Helpers::BuildNode< ULong4 >( "AsULong4", x );
	}

	template <typename T, uint I>
	inline Float AsFloat (const NVec<T,I> &x) {
		STATIC_ASSERT( (sizeof(typeName::Value_t) * typeName::COUNT) == (sizeof(T) * I) );
		return Helpers::BuildNode< Float >( "AsFloat", x );
	}

	template <typename T, uint I>
	inline Float2 AsFloat2 (const NVec<T,I> &x) {
		STATIC_ASSERT( (sizeof(typeName::Value_t) * typeName::COUNT) == (sizeof(T) * I) );
		return Helpers::BuildNode< Float2 >( "AsFloat2", x );
	}

	template <typename T, uint I>
	inline Float3 AsFloat3 (const NVec<T,I> &x) {
		STATIC_ASSERT( (sizeof(typeName::Value_t) * typeName::COUNT) == (sizeof(T) * I) );
		return Helpers::BuildNode< Float3 >( "AsFloat3", x );
	}

	template <typename T, uint I>
	inline Float4 AsFloat4 (const NVec<T,I> &x) {
		STATIC_ASSERT( (sizeof(typeName::Value_t) * typeName::COUNT) == (sizeof(T) * I) );
		return Helpers::BuildNode< Float4 >( "AsFloat4", x );
	}

	template <typename T, uint I>
	inline Double AsDouble (const NVec<T,I> &x) {
		STATIC_ASSERT( (sizeof(typeName::Value_t) * typeName::COUNT) == (sizeof(T) * I) );
		return Helpers::BuildNode< Double >( "AsDouble", x );
	}

	template <typename T, uint I>
	inline Double2 AsDouble2 (const NVec<T,I> &x) {
		STATIC_ASSERT( (sizeof(typeName::Value_t) * typeName::COUNT) == (sizeof(T) * I) );
		return Helpers::BuildNode< Double2 >( "AsDouble2", x );
	}

	template <typename T, uint I>
	inline Double3 AsDouble3 (const NVec<T,I> &x) {
		STATIC_ASSERT( (sizeof(typeName::Value_t) * typeName::COUNT) == (sizeof(T) * I) );
		return Helpers::BuildNode< Double3 >( "AsDouble3", x );
	}

	template <typename T, uint I>
	inline Double4 AsDouble4 (const NVec<T,I> &x) {
		STATIC_ASSERT( (sizeof(typeName::Value_t) * typeName::COUNT) == (sizeof(T) * I) );
		return Helpers::BuildNode< Double4 >( "AsDouble4", x );
	}


	// All, Any
	template <uint I>
	inline NVec<bool,1> All (const NVec<bool,I> &x) {
		return Helpers::BuildNode< NVec<bool,1> >( "All", x );
	}

	template <uint I>
	inline NVec<bool,1> Any (const NVec<bool,I> &x) {
		return Helpers::BuildNode< NVec<bool,1> >( "Any", x );
	}


	// Sin, Cos, Tan, CoTan
	template <typename T, uint I>
	inline NVec<T,I> Sin (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Sin", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> Cos (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Cos", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> Tan (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "Tan", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> CoTan (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "CoTan", x );
	}


	// ASin, ACos, ATan, ACoTan
	template <typename T, uint I>
	inline NVec<T,I> ASin (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "ASin", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> ACos (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "ACos", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> ATan (const NVec<T,I> &y, const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "ATan", y, x );
	}

	template <typename T, uint I>
	inline NVec<T,I> ATan (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "ATan", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> ACoTan (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "ACoTan", x );
	}


	// SinH, CosH, TanH, CoTanH
	template <typename T, uint I>
	inline NVec<T,I> SinH (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "SinH", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> CosH (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "CosH", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> TanH (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "TanH", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> CoTanH (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "CoTanH", x );
	}


	// ASinH, ACosH, ATanH, ACoTanH
	template <typename T, uint I>
	inline NVec<T,I> ASinH (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "ASinH", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> ACosH (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "ACosH", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> ATanH (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "ATanH", x );
	}

	template <typename T, uint I>
	inline NVec<T,I> ACoTanH (const NVec<T,I> &x) {
		Helpers::FloatOnly< NVec<T,I> >();
		return Helpers::BuildNode< NVec<T,I> >( "ACoTanH", x );
	}



}	// ShaderNodes
}	// ShaderEditor
