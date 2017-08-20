// This is generated file, don't change anything!

#pragma once

#include "Engine/Shaders/Shader/Helpers.h"

namespace ShaderEditor
{
namespace ShaderNodes
{


	// Type casts
	template <typename T> struct _VecConstructors;

	template <typename T>
	inline Bool BoolCtor (const NVec<T,1> &x) {
		return Helpers::BuildNode< Bool >( "BoolCtor", x );
	}

	template <>  struct _VecConstructors< Bool >  {template <typename ...Args> static inline Bool  Ctor (const Args& ...args) { return BoolCtor( args... ); } };

	template <typename T>
	inline Bool2 Bool2Ctor (const NVec<T,1> &x) {
		return Helpers::BuildNode< Bool2 >( "Bool2Ctor", x );
	}

	template <typename T>
	inline Bool2 Bool2Ctor (const NVec<T,2> &xy) {
		return Helpers::BuildNode< Bool2 >( "Bool2Ctor", xy );
	}

	template <typename T>
	inline Bool2 Bool2Ctor (const NVec<T,3> &xyz) {
		return Helpers::BuildNode< Bool2 >( "Bool2Ctor", xyz );
	}

	template <typename T>
	inline Bool2 Bool2Ctor (const NVec<T,4> &xyzw) {
		return Helpers::BuildNode< Bool2 >( "Bool2Ctor", xyzw );
	}

	template <typename T>
	inline Bool2 Bool2Ctor (const NVec<T,1> &x, const NVec<T,1> &y) {
		return Helpers::BuildNode< Bool2 >( "Bool2Ctor", x, y );
	}

	template <>  struct _VecConstructors< Bool2 >  {template <typename ...Args> static inline Bool2  Ctor (const Args& ...args) { return Bool2Ctor( args... ); } };

	template <typename T>
	inline Bool3 Bool3Ctor (const NVec<T,1> &x) {
		return Helpers::BuildNode< Bool3 >( "Bool3Ctor", x );
	}

	template <typename T>
	inline Bool3 Bool3Ctor (const NVec<T,2> &xy) {
		return Helpers::BuildNode< Bool3 >( "Bool3Ctor", xy );
	}

	template <typename T>
	inline Bool3 Bool3Ctor (const NVec<T,3> &xyz) {
		return Helpers::BuildNode< Bool3 >( "Bool3Ctor", xyz );
	}

	template <typename T>
	inline Bool3 Bool3Ctor (const NVec<T,4> &xyzw) {
		return Helpers::BuildNode< Bool3 >( "Bool3Ctor", xyzw );
	}

	template <typename T>
	inline Bool3 Bool3Ctor (const NVec<T,2> &xy, const NVec<T,1> &z) {
		return Helpers::BuildNode< Bool3 >( "Bool3Ctor", xy, z );
	}

	template <typename T>
	inline Bool3 Bool3Ctor (const NVec<T,1> &x, const NVec<T,2> &yz) {
		return Helpers::BuildNode< Bool3 >( "Bool3Ctor", x, yz );
	}

	template <typename T>
	inline Bool3 Bool3Ctor (const NVec<T,1> &x, const NVec<T,1> &y, const NVec<T,1> &z) {
		return Helpers::BuildNode< Bool3 >( "Bool3Ctor", x, y, z );
	}

	template <>  struct _VecConstructors< Bool3 >  {template <typename ...Args> static inline Bool3  Ctor (const Args& ...args) { return Bool3Ctor( args... ); } };

	template <typename T>
	inline Bool4 Bool4Ctor (const NVec<T,1> &x) {
		return Helpers::BuildNode< Bool4 >( "Bool4Ctor", x );
	}

	template <typename T>
	inline Bool4 Bool4Ctor (const NVec<T,2> &xy) {
		return Helpers::BuildNode< Bool4 >( "Bool4Ctor", xy );
	}

	template <typename T>
	inline Bool4 Bool4Ctor (const NVec<T,3> &xyz) {
		return Helpers::BuildNode< Bool4 >( "Bool4Ctor", xyz );
	}

	template <typename T>
	inline Bool4 Bool4Ctor (const NVec<T,4> &xyzw) {
		return Helpers::BuildNode< Bool4 >( "Bool4Ctor", xyzw );
	}

	template <typename T>
	inline Bool4 Bool4Ctor (const NVec<T,3> &xyz, const NVec<T,1> &w) {
		return Helpers::BuildNode< Bool4 >( "Bool4Ctor", xyz, w );
	}

	template <typename T>
	inline Bool4 Bool4Ctor (const NVec<T,2> &xy, const NVec<T,2> &zw) {
		return Helpers::BuildNode< Bool4 >( "Bool4Ctor", xy, zw );
	}

	template <typename T>
	inline Bool4 Bool4Ctor (const NVec<T,1> &x, const NVec<T,3> &yzw) {
		return Helpers::BuildNode< Bool4 >( "Bool4Ctor", x, yzw );
	}

	template <typename T>
	inline Bool4 Bool4Ctor (const NVec<T,2> &xy, const NVec<T,1> &z, const NVec<T,1> &w) {
		return Helpers::BuildNode< Bool4 >( "Bool4Ctor", xy, z, w );
	}

	template <typename T>
	inline Bool4 Bool4Ctor (const NVec<T,1> &x, const NVec<T,2> &yz, const NVec<T,1> &w) {
		return Helpers::BuildNode< Bool4 >( "Bool4Ctor", x, yz, w );
	}

	template <typename T>
	inline Bool4 Bool4Ctor (const NVec<T,1> &x, const NVec<T,1> &y, const NVec<T,2> &zw) {
		return Helpers::BuildNode< Bool4 >( "Bool4Ctor", x, y, zw );
	}

	template <typename T>
	inline Bool4 Bool4Ctor (const NVec<T,1> &x, const NVec<T,1> &y, const NVec<T,1> &z, const NVec<T,1> &w) {
		return Helpers::BuildNode< Bool4 >( "Bool4Ctor", x, y, z, w );
	}

	template <>  struct _VecConstructors< Bool4 >  {template <typename ...Args> static inline Bool4  Ctor (const Args& ...args) { return Bool4Ctor( args... ); } };

	template <typename T>
	inline Int IntCtor (const NVec<T,1> &x) {
		return Helpers::BuildNode< Int >( "IntCtor", x );
	}

	template <>  struct _VecConstructors< Int >  {template <typename ...Args> static inline Int  Ctor (const Args& ...args) { return IntCtor( args... ); } };

	template <typename T>
	inline Int2 Int2Ctor (const NVec<T,1> &x) {
		return Helpers::BuildNode< Int2 >( "Int2Ctor", x );
	}

	template <typename T>
	inline Int2 Int2Ctor (const NVec<T,2> &xy) {
		return Helpers::BuildNode< Int2 >( "Int2Ctor", xy );
	}

	template <typename T>
	inline Int2 Int2Ctor (const NVec<T,3> &xyz) {
		return Helpers::BuildNode< Int2 >( "Int2Ctor", xyz );
	}

	template <typename T>
	inline Int2 Int2Ctor (const NVec<T,4> &xyzw) {
		return Helpers::BuildNode< Int2 >( "Int2Ctor", xyzw );
	}

	template <typename T>
	inline Int2 Int2Ctor (const NVec<T,1> &x, const NVec<T,1> &y) {
		return Helpers::BuildNode< Int2 >( "Int2Ctor", x, y );
	}

	template <>  struct _VecConstructors< Int2 >  {template <typename ...Args> static inline Int2  Ctor (const Args& ...args) { return Int2Ctor( args... ); } };

	template <typename T>
	inline Int3 Int3Ctor (const NVec<T,1> &x) {
		return Helpers::BuildNode< Int3 >( "Int3Ctor", x );
	}

	template <typename T>
	inline Int3 Int3Ctor (const NVec<T,2> &xy) {
		return Helpers::BuildNode< Int3 >( "Int3Ctor", xy );
	}

	template <typename T>
	inline Int3 Int3Ctor (const NVec<T,3> &xyz) {
		return Helpers::BuildNode< Int3 >( "Int3Ctor", xyz );
	}

	template <typename T>
	inline Int3 Int3Ctor (const NVec<T,4> &xyzw) {
		return Helpers::BuildNode< Int3 >( "Int3Ctor", xyzw );
	}

	template <typename T>
	inline Int3 Int3Ctor (const NVec<T,2> &xy, const NVec<T,1> &z) {
		return Helpers::BuildNode< Int3 >( "Int3Ctor", xy, z );
	}

	template <typename T>
	inline Int3 Int3Ctor (const NVec<T,1> &x, const NVec<T,2> &yz) {
		return Helpers::BuildNode< Int3 >( "Int3Ctor", x, yz );
	}

	template <typename T>
	inline Int3 Int3Ctor (const NVec<T,1> &x, const NVec<T,1> &y, const NVec<T,1> &z) {
		return Helpers::BuildNode< Int3 >( "Int3Ctor", x, y, z );
	}

	template <>  struct _VecConstructors< Int3 >  {template <typename ...Args> static inline Int3  Ctor (const Args& ...args) { return Int3Ctor( args... ); } };

	template <typename T>
	inline Int4 Int4Ctor (const NVec<T,1> &x) {
		return Helpers::BuildNode< Int4 >( "Int4Ctor", x );
	}

	template <typename T>
	inline Int4 Int4Ctor (const NVec<T,2> &xy) {
		return Helpers::BuildNode< Int4 >( "Int4Ctor", xy );
	}

	template <typename T>
	inline Int4 Int4Ctor (const NVec<T,3> &xyz) {
		return Helpers::BuildNode< Int4 >( "Int4Ctor", xyz );
	}

	template <typename T>
	inline Int4 Int4Ctor (const NVec<T,4> &xyzw) {
		return Helpers::BuildNode< Int4 >( "Int4Ctor", xyzw );
	}

	template <typename T>
	inline Int4 Int4Ctor (const NVec<T,3> &xyz, const NVec<T,1> &w) {
		return Helpers::BuildNode< Int4 >( "Int4Ctor", xyz, w );
	}

	template <typename T>
	inline Int4 Int4Ctor (const NVec<T,2> &xy, const NVec<T,2> &zw) {
		return Helpers::BuildNode< Int4 >( "Int4Ctor", xy, zw );
	}

	template <typename T>
	inline Int4 Int4Ctor (const NVec<T,1> &x, const NVec<T,3> &yzw) {
		return Helpers::BuildNode< Int4 >( "Int4Ctor", x, yzw );
	}

	template <typename T>
	inline Int4 Int4Ctor (const NVec<T,2> &xy, const NVec<T,1> &z, const NVec<T,1> &w) {
		return Helpers::BuildNode< Int4 >( "Int4Ctor", xy, z, w );
	}

	template <typename T>
	inline Int4 Int4Ctor (const NVec<T,1> &x, const NVec<T,2> &yz, const NVec<T,1> &w) {
		return Helpers::BuildNode< Int4 >( "Int4Ctor", x, yz, w );
	}

	template <typename T>
	inline Int4 Int4Ctor (const NVec<T,1> &x, const NVec<T,1> &y, const NVec<T,2> &zw) {
		return Helpers::BuildNode< Int4 >( "Int4Ctor", x, y, zw );
	}

	template <typename T>
	inline Int4 Int4Ctor (const NVec<T,1> &x, const NVec<T,1> &y, const NVec<T,1> &z, const NVec<T,1> &w) {
		return Helpers::BuildNode< Int4 >( "Int4Ctor", x, y, z, w );
	}

	template <>  struct _VecConstructors< Int4 >  {template <typename ...Args> static inline Int4  Ctor (const Args& ...args) { return Int4Ctor( args... ); } };

	template <typename T>
	inline UInt UIntCtor (const NVec<T,1> &x) {
		return Helpers::BuildNode< UInt >( "UIntCtor", x );
	}

	template <>  struct _VecConstructors< UInt >  {template <typename ...Args> static inline UInt  Ctor (const Args& ...args) { return UIntCtor( args... ); } };

	template <typename T>
	inline UInt2 UInt2Ctor (const NVec<T,1> &x) {
		return Helpers::BuildNode< UInt2 >( "UInt2Ctor", x );
	}

	template <typename T>
	inline UInt2 UInt2Ctor (const NVec<T,2> &xy) {
		return Helpers::BuildNode< UInt2 >( "UInt2Ctor", xy );
	}

	template <typename T>
	inline UInt2 UInt2Ctor (const NVec<T,3> &xyz) {
		return Helpers::BuildNode< UInt2 >( "UInt2Ctor", xyz );
	}

	template <typename T>
	inline UInt2 UInt2Ctor (const NVec<T,4> &xyzw) {
		return Helpers::BuildNode< UInt2 >( "UInt2Ctor", xyzw );
	}

	template <typename T>
	inline UInt2 UInt2Ctor (const NVec<T,1> &x, const NVec<T,1> &y) {
		return Helpers::BuildNode< UInt2 >( "UInt2Ctor", x, y );
	}

	template <>  struct _VecConstructors< UInt2 >  {template <typename ...Args> static inline UInt2  Ctor (const Args& ...args) { return UInt2Ctor( args... ); } };

	template <typename T>
	inline UInt3 UInt3Ctor (const NVec<T,1> &x) {
		return Helpers::BuildNode< UInt3 >( "UInt3Ctor", x );
	}

	template <typename T>
	inline UInt3 UInt3Ctor (const NVec<T,2> &xy) {
		return Helpers::BuildNode< UInt3 >( "UInt3Ctor", xy );
	}

	template <typename T>
	inline UInt3 UInt3Ctor (const NVec<T,3> &xyz) {
		return Helpers::BuildNode< UInt3 >( "UInt3Ctor", xyz );
	}

	template <typename T>
	inline UInt3 UInt3Ctor (const NVec<T,4> &xyzw) {
		return Helpers::BuildNode< UInt3 >( "UInt3Ctor", xyzw );
	}

	template <typename T>
	inline UInt3 UInt3Ctor (const NVec<T,2> &xy, const NVec<T,1> &z) {
		return Helpers::BuildNode< UInt3 >( "UInt3Ctor", xy, z );
	}

	template <typename T>
	inline UInt3 UInt3Ctor (const NVec<T,1> &x, const NVec<T,2> &yz) {
		return Helpers::BuildNode< UInt3 >( "UInt3Ctor", x, yz );
	}

	template <typename T>
	inline UInt3 UInt3Ctor (const NVec<T,1> &x, const NVec<T,1> &y, const NVec<T,1> &z) {
		return Helpers::BuildNode< UInt3 >( "UInt3Ctor", x, y, z );
	}

	template <>  struct _VecConstructors< UInt3 >  {template <typename ...Args> static inline UInt3  Ctor (const Args& ...args) { return UInt3Ctor( args... ); } };

	template <typename T>
	inline UInt4 UInt4Ctor (const NVec<T,1> &x) {
		return Helpers::BuildNode< UInt4 >( "UInt4Ctor", x );
	}

	template <typename T>
	inline UInt4 UInt4Ctor (const NVec<T,2> &xy) {
		return Helpers::BuildNode< UInt4 >( "UInt4Ctor", xy );
	}

	template <typename T>
	inline UInt4 UInt4Ctor (const NVec<T,3> &xyz) {
		return Helpers::BuildNode< UInt4 >( "UInt4Ctor", xyz );
	}

	template <typename T>
	inline UInt4 UInt4Ctor (const NVec<T,4> &xyzw) {
		return Helpers::BuildNode< UInt4 >( "UInt4Ctor", xyzw );
	}

	template <typename T>
	inline UInt4 UInt4Ctor (const NVec<T,3> &xyz, const NVec<T,1> &w) {
		return Helpers::BuildNode< UInt4 >( "UInt4Ctor", xyz, w );
	}

	template <typename T>
	inline UInt4 UInt4Ctor (const NVec<T,2> &xy, const NVec<T,2> &zw) {
		return Helpers::BuildNode< UInt4 >( "UInt4Ctor", xy, zw );
	}

	template <typename T>
	inline UInt4 UInt4Ctor (const NVec<T,1> &x, const NVec<T,3> &yzw) {
		return Helpers::BuildNode< UInt4 >( "UInt4Ctor", x, yzw );
	}

	template <typename T>
	inline UInt4 UInt4Ctor (const NVec<T,2> &xy, const NVec<T,1> &z, const NVec<T,1> &w) {
		return Helpers::BuildNode< UInt4 >( "UInt4Ctor", xy, z, w );
	}

	template <typename T>
	inline UInt4 UInt4Ctor (const NVec<T,1> &x, const NVec<T,2> &yz, const NVec<T,1> &w) {
		return Helpers::BuildNode< UInt4 >( "UInt4Ctor", x, yz, w );
	}

	template <typename T>
	inline UInt4 UInt4Ctor (const NVec<T,1> &x, const NVec<T,1> &y, const NVec<T,2> &zw) {
		return Helpers::BuildNode< UInt4 >( "UInt4Ctor", x, y, zw );
	}

	template <typename T>
	inline UInt4 UInt4Ctor (const NVec<T,1> &x, const NVec<T,1> &y, const NVec<T,1> &z, const NVec<T,1> &w) {
		return Helpers::BuildNode< UInt4 >( "UInt4Ctor", x, y, z, w );
	}

	template <>  struct _VecConstructors< UInt4 >  {template <typename ...Args> static inline UInt4  Ctor (const Args& ...args) { return UInt4Ctor( args... ); } };

	template <typename T>
	inline ILong ILongCtor (const NVec<T,1> &x) {
		return Helpers::BuildNode< ILong >( "ILongCtor", x );
	}

	template <>  struct _VecConstructors< ILong >  {template <typename ...Args> static inline ILong  Ctor (const Args& ...args) { return ILongCtor( args... ); } };

	template <typename T>
	inline ILong2 ILong2Ctor (const NVec<T,1> &x) {
		return Helpers::BuildNode< ILong2 >( "ILong2Ctor", x );
	}

	template <typename T>
	inline ILong2 ILong2Ctor (const NVec<T,2> &xy) {
		return Helpers::BuildNode< ILong2 >( "ILong2Ctor", xy );
	}

	template <typename T>
	inline ILong2 ILong2Ctor (const NVec<T,3> &xyz) {
		return Helpers::BuildNode< ILong2 >( "ILong2Ctor", xyz );
	}

	template <typename T>
	inline ILong2 ILong2Ctor (const NVec<T,4> &xyzw) {
		return Helpers::BuildNode< ILong2 >( "ILong2Ctor", xyzw );
	}

	template <typename T>
	inline ILong2 ILong2Ctor (const NVec<T,1> &x, const NVec<T,1> &y) {
		return Helpers::BuildNode< ILong2 >( "ILong2Ctor", x, y );
	}

	template <>  struct _VecConstructors< ILong2 >  {template <typename ...Args> static inline ILong2  Ctor (const Args& ...args) { return ILong2Ctor( args... ); } };

	template <typename T>
	inline ILong3 ILong3Ctor (const NVec<T,1> &x) {
		return Helpers::BuildNode< ILong3 >( "ILong3Ctor", x );
	}

	template <typename T>
	inline ILong3 ILong3Ctor (const NVec<T,2> &xy) {
		return Helpers::BuildNode< ILong3 >( "ILong3Ctor", xy );
	}

	template <typename T>
	inline ILong3 ILong3Ctor (const NVec<T,3> &xyz) {
		return Helpers::BuildNode< ILong3 >( "ILong3Ctor", xyz );
	}

	template <typename T>
	inline ILong3 ILong3Ctor (const NVec<T,4> &xyzw) {
		return Helpers::BuildNode< ILong3 >( "ILong3Ctor", xyzw );
	}

	template <typename T>
	inline ILong3 ILong3Ctor (const NVec<T,2> &xy, const NVec<T,1> &z) {
		return Helpers::BuildNode< ILong3 >( "ILong3Ctor", xy, z );
	}

	template <typename T>
	inline ILong3 ILong3Ctor (const NVec<T,1> &x, const NVec<T,2> &yz) {
		return Helpers::BuildNode< ILong3 >( "ILong3Ctor", x, yz );
	}

	template <typename T>
	inline ILong3 ILong3Ctor (const NVec<T,1> &x, const NVec<T,1> &y, const NVec<T,1> &z) {
		return Helpers::BuildNode< ILong3 >( "ILong3Ctor", x, y, z );
	}

	template <>  struct _VecConstructors< ILong3 >  {template <typename ...Args> static inline ILong3  Ctor (const Args& ...args) { return ILong3Ctor( args... ); } };

	template <typename T>
	inline ILong4 ILong4Ctor (const NVec<T,1> &x) {
		return Helpers::BuildNode< ILong4 >( "ILong4Ctor", x );
	}

	template <typename T>
	inline ILong4 ILong4Ctor (const NVec<T,2> &xy) {
		return Helpers::BuildNode< ILong4 >( "ILong4Ctor", xy );
	}

	template <typename T>
	inline ILong4 ILong4Ctor (const NVec<T,3> &xyz) {
		return Helpers::BuildNode< ILong4 >( "ILong4Ctor", xyz );
	}

	template <typename T>
	inline ILong4 ILong4Ctor (const NVec<T,4> &xyzw) {
		return Helpers::BuildNode< ILong4 >( "ILong4Ctor", xyzw );
	}

	template <typename T>
	inline ILong4 ILong4Ctor (const NVec<T,3> &xyz, const NVec<T,1> &w) {
		return Helpers::BuildNode< ILong4 >( "ILong4Ctor", xyz, w );
	}

	template <typename T>
	inline ILong4 ILong4Ctor (const NVec<T,2> &xy, const NVec<T,2> &zw) {
		return Helpers::BuildNode< ILong4 >( "ILong4Ctor", xy, zw );
	}

	template <typename T>
	inline ILong4 ILong4Ctor (const NVec<T,1> &x, const NVec<T,3> &yzw) {
		return Helpers::BuildNode< ILong4 >( "ILong4Ctor", x, yzw );
	}

	template <typename T>
	inline ILong4 ILong4Ctor (const NVec<T,2> &xy, const NVec<T,1> &z, const NVec<T,1> &w) {
		return Helpers::BuildNode< ILong4 >( "ILong4Ctor", xy, z, w );
	}

	template <typename T>
	inline ILong4 ILong4Ctor (const NVec<T,1> &x, const NVec<T,2> &yz, const NVec<T,1> &w) {
		return Helpers::BuildNode< ILong4 >( "ILong4Ctor", x, yz, w );
	}

	template <typename T>
	inline ILong4 ILong4Ctor (const NVec<T,1> &x, const NVec<T,1> &y, const NVec<T,2> &zw) {
		return Helpers::BuildNode< ILong4 >( "ILong4Ctor", x, y, zw );
	}

	template <typename T>
	inline ILong4 ILong4Ctor (const NVec<T,1> &x, const NVec<T,1> &y, const NVec<T,1> &z, const NVec<T,1> &w) {
		return Helpers::BuildNode< ILong4 >( "ILong4Ctor", x, y, z, w );
	}

	template <>  struct _VecConstructors< ILong4 >  {template <typename ...Args> static inline ILong4  Ctor (const Args& ...args) { return ILong4Ctor( args... ); } };

	template <typename T>
	inline ULong ULongCtor (const NVec<T,1> &x) {
		return Helpers::BuildNode< ULong >( "ULongCtor", x );
	}

	template <>  struct _VecConstructors< ULong >  {template <typename ...Args> static inline ULong  Ctor (const Args& ...args) { return ULongCtor( args... ); } };

	template <typename T>
	inline ULong2 ULong2Ctor (const NVec<T,1> &x) {
		return Helpers::BuildNode< ULong2 >( "ULong2Ctor", x );
	}

	template <typename T>
	inline ULong2 ULong2Ctor (const NVec<T,2> &xy) {
		return Helpers::BuildNode< ULong2 >( "ULong2Ctor", xy );
	}

	template <typename T>
	inline ULong2 ULong2Ctor (const NVec<T,3> &xyz) {
		return Helpers::BuildNode< ULong2 >( "ULong2Ctor", xyz );
	}

	template <typename T>
	inline ULong2 ULong2Ctor (const NVec<T,4> &xyzw) {
		return Helpers::BuildNode< ULong2 >( "ULong2Ctor", xyzw );
	}

	template <typename T>
	inline ULong2 ULong2Ctor (const NVec<T,1> &x, const NVec<T,1> &y) {
		return Helpers::BuildNode< ULong2 >( "ULong2Ctor", x, y );
	}

	template <>  struct _VecConstructors< ULong2 >  {template <typename ...Args> static inline ULong2  Ctor (const Args& ...args) { return ULong2Ctor( args... ); } };

	template <typename T>
	inline ULong3 ULong3Ctor (const NVec<T,1> &x) {
		return Helpers::BuildNode< ULong3 >( "ULong3Ctor", x );
	}

	template <typename T>
	inline ULong3 ULong3Ctor (const NVec<T,2> &xy) {
		return Helpers::BuildNode< ULong3 >( "ULong3Ctor", xy );
	}

	template <typename T>
	inline ULong3 ULong3Ctor (const NVec<T,3> &xyz) {
		return Helpers::BuildNode< ULong3 >( "ULong3Ctor", xyz );
	}

	template <typename T>
	inline ULong3 ULong3Ctor (const NVec<T,4> &xyzw) {
		return Helpers::BuildNode< ULong3 >( "ULong3Ctor", xyzw );
	}

	template <typename T>
	inline ULong3 ULong3Ctor (const NVec<T,2> &xy, const NVec<T,1> &z) {
		return Helpers::BuildNode< ULong3 >( "ULong3Ctor", xy, z );
	}

	template <typename T>
	inline ULong3 ULong3Ctor (const NVec<T,1> &x, const NVec<T,2> &yz) {
		return Helpers::BuildNode< ULong3 >( "ULong3Ctor", x, yz );
	}

	template <typename T>
	inline ULong3 ULong3Ctor (const NVec<T,1> &x, const NVec<T,1> &y, const NVec<T,1> &z) {
		return Helpers::BuildNode< ULong3 >( "ULong3Ctor", x, y, z );
	}

	template <>  struct _VecConstructors< ULong3 >  {template <typename ...Args> static inline ULong3  Ctor (const Args& ...args) { return ULong3Ctor( args... ); } };

	template <typename T>
	inline ULong4 ULong4Ctor (const NVec<T,1> &x) {
		return Helpers::BuildNode< ULong4 >( "ULong4Ctor", x );
	}

	template <typename T>
	inline ULong4 ULong4Ctor (const NVec<T,2> &xy) {
		return Helpers::BuildNode< ULong4 >( "ULong4Ctor", xy );
	}

	template <typename T>
	inline ULong4 ULong4Ctor (const NVec<T,3> &xyz) {
		return Helpers::BuildNode< ULong4 >( "ULong4Ctor", xyz );
	}

	template <typename T>
	inline ULong4 ULong4Ctor (const NVec<T,4> &xyzw) {
		return Helpers::BuildNode< ULong4 >( "ULong4Ctor", xyzw );
	}

	template <typename T>
	inline ULong4 ULong4Ctor (const NVec<T,3> &xyz, const NVec<T,1> &w) {
		return Helpers::BuildNode< ULong4 >( "ULong4Ctor", xyz, w );
	}

	template <typename T>
	inline ULong4 ULong4Ctor (const NVec<T,2> &xy, const NVec<T,2> &zw) {
		return Helpers::BuildNode< ULong4 >( "ULong4Ctor", xy, zw );
	}

	template <typename T>
	inline ULong4 ULong4Ctor (const NVec<T,1> &x, const NVec<T,3> &yzw) {
		return Helpers::BuildNode< ULong4 >( "ULong4Ctor", x, yzw );
	}

	template <typename T>
	inline ULong4 ULong4Ctor (const NVec<T,2> &xy, const NVec<T,1> &z, const NVec<T,1> &w) {
		return Helpers::BuildNode< ULong4 >( "ULong4Ctor", xy, z, w );
	}

	template <typename T>
	inline ULong4 ULong4Ctor (const NVec<T,1> &x, const NVec<T,2> &yz, const NVec<T,1> &w) {
		return Helpers::BuildNode< ULong4 >( "ULong4Ctor", x, yz, w );
	}

	template <typename T>
	inline ULong4 ULong4Ctor (const NVec<T,1> &x, const NVec<T,1> &y, const NVec<T,2> &zw) {
		return Helpers::BuildNode< ULong4 >( "ULong4Ctor", x, y, zw );
	}

	template <typename T>
	inline ULong4 ULong4Ctor (const NVec<T,1> &x, const NVec<T,1> &y, const NVec<T,1> &z, const NVec<T,1> &w) {
		return Helpers::BuildNode< ULong4 >( "ULong4Ctor", x, y, z, w );
	}

	template <>  struct _VecConstructors< ULong4 >  {template <typename ...Args> static inline ULong4  Ctor (const Args& ...args) { return ULong4Ctor( args... ); } };

	template <typename T>
	inline Float FloatCtor (const NVec<T,1> &x) {
		return Helpers::BuildNode< Float >( "FloatCtor", x );
	}

	template <>  struct _VecConstructors< Float >  {template <typename ...Args> static inline Float  Ctor (const Args& ...args) { return FloatCtor( args... ); } };

	template <typename T>
	inline Float2 Float2Ctor (const NVec<T,1> &x) {
		return Helpers::BuildNode< Float2 >( "Float2Ctor", x );
	}

	template <typename T>
	inline Float2 Float2Ctor (const NVec<T,2> &xy) {
		return Helpers::BuildNode< Float2 >( "Float2Ctor", xy );
	}

	template <typename T>
	inline Float2 Float2Ctor (const NVec<T,3> &xyz) {
		return Helpers::BuildNode< Float2 >( "Float2Ctor", xyz );
	}

	template <typename T>
	inline Float2 Float2Ctor (const NVec<T,4> &xyzw) {
		return Helpers::BuildNode< Float2 >( "Float2Ctor", xyzw );
	}

	template <typename T>
	inline Float2 Float2Ctor (const NVec<T,1> &x, const NVec<T,1> &y) {
		return Helpers::BuildNode< Float2 >( "Float2Ctor", x, y );
	}

	template <>  struct _VecConstructors< Float2 >  {template <typename ...Args> static inline Float2  Ctor (const Args& ...args) { return Float2Ctor( args... ); } };

	template <typename T>
	inline Float3 Float3Ctor (const NVec<T,1> &x) {
		return Helpers::BuildNode< Float3 >( "Float3Ctor", x );
	}

	template <typename T>
	inline Float3 Float3Ctor (const NVec<T,2> &xy) {
		return Helpers::BuildNode< Float3 >( "Float3Ctor", xy );
	}

	template <typename T>
	inline Float3 Float3Ctor (const NVec<T,3> &xyz) {
		return Helpers::BuildNode< Float3 >( "Float3Ctor", xyz );
	}

	template <typename T>
	inline Float3 Float3Ctor (const NVec<T,4> &xyzw) {
		return Helpers::BuildNode< Float3 >( "Float3Ctor", xyzw );
	}

	template <typename T>
	inline Float3 Float3Ctor (const NVec<T,2> &xy, const NVec<T,1> &z) {
		return Helpers::BuildNode< Float3 >( "Float3Ctor", xy, z );
	}

	template <typename T>
	inline Float3 Float3Ctor (const NVec<T,1> &x, const NVec<T,2> &yz) {
		return Helpers::BuildNode< Float3 >( "Float3Ctor", x, yz );
	}

	template <typename T>
	inline Float3 Float3Ctor (const NVec<T,1> &x, const NVec<T,1> &y, const NVec<T,1> &z) {
		return Helpers::BuildNode< Float3 >( "Float3Ctor", x, y, z );
	}

	template <>  struct _VecConstructors< Float3 >  {template <typename ...Args> static inline Float3  Ctor (const Args& ...args) { return Float3Ctor( args... ); } };

	template <typename T>
	inline Float4 Float4Ctor (const NVec<T,1> &x) {
		return Helpers::BuildNode< Float4 >( "Float4Ctor", x );
	}

	template <typename T>
	inline Float4 Float4Ctor (const NVec<T,2> &xy) {
		return Helpers::BuildNode< Float4 >( "Float4Ctor", xy );
	}

	template <typename T>
	inline Float4 Float4Ctor (const NVec<T,3> &xyz) {
		return Helpers::BuildNode< Float4 >( "Float4Ctor", xyz );
	}

	template <typename T>
	inline Float4 Float4Ctor (const NVec<T,4> &xyzw) {
		return Helpers::BuildNode< Float4 >( "Float4Ctor", xyzw );
	}

	template <typename T>
	inline Float4 Float4Ctor (const NVec<T,3> &xyz, const NVec<T,1> &w) {
		return Helpers::BuildNode< Float4 >( "Float4Ctor", xyz, w );
	}

	template <typename T>
	inline Float4 Float4Ctor (const NVec<T,2> &xy, const NVec<T,2> &zw) {
		return Helpers::BuildNode< Float4 >( "Float4Ctor", xy, zw );
	}

	template <typename T>
	inline Float4 Float4Ctor (const NVec<T,1> &x, const NVec<T,3> &yzw) {
		return Helpers::BuildNode< Float4 >( "Float4Ctor", x, yzw );
	}

	template <typename T>
	inline Float4 Float4Ctor (const NVec<T,2> &xy, const NVec<T,1> &z, const NVec<T,1> &w) {
		return Helpers::BuildNode< Float4 >( "Float4Ctor", xy, z, w );
	}

	template <typename T>
	inline Float4 Float4Ctor (const NVec<T,1> &x, const NVec<T,2> &yz, const NVec<T,1> &w) {
		return Helpers::BuildNode< Float4 >( "Float4Ctor", x, yz, w );
	}

	template <typename T>
	inline Float4 Float4Ctor (const NVec<T,1> &x, const NVec<T,1> &y, const NVec<T,2> &zw) {
		return Helpers::BuildNode< Float4 >( "Float4Ctor", x, y, zw );
	}

	template <typename T>
	inline Float4 Float4Ctor (const NVec<T,1> &x, const NVec<T,1> &y, const NVec<T,1> &z, const NVec<T,1> &w) {
		return Helpers::BuildNode< Float4 >( "Float4Ctor", x, y, z, w );
	}

	template <>  struct _VecConstructors< Float4 >  {template <typename ...Args> static inline Float4  Ctor (const Args& ...args) { return Float4Ctor( args... ); } };

	template <typename T>
	inline Double DoubleCtor (const NVec<T,1> &x) {
		return Helpers::BuildNode< Double >( "DoubleCtor", x );
	}

	template <>  struct _VecConstructors< Double >  {template <typename ...Args> static inline Double  Ctor (const Args& ...args) { return DoubleCtor( args... ); } };

	template <typename T>
	inline Double2 Double2Ctor (const NVec<T,1> &x) {
		return Helpers::BuildNode< Double2 >( "Double2Ctor", x );
	}

	template <typename T>
	inline Double2 Double2Ctor (const NVec<T,2> &xy) {
		return Helpers::BuildNode< Double2 >( "Double2Ctor", xy );
	}

	template <typename T>
	inline Double2 Double2Ctor (const NVec<T,3> &xyz) {
		return Helpers::BuildNode< Double2 >( "Double2Ctor", xyz );
	}

	template <typename T>
	inline Double2 Double2Ctor (const NVec<T,4> &xyzw) {
		return Helpers::BuildNode< Double2 >( "Double2Ctor", xyzw );
	}

	template <typename T>
	inline Double2 Double2Ctor (const NVec<T,1> &x, const NVec<T,1> &y) {
		return Helpers::BuildNode< Double2 >( "Double2Ctor", x, y );
	}

	template <>  struct _VecConstructors< Double2 >  {template <typename ...Args> static inline Double2  Ctor (const Args& ...args) { return Double2Ctor( args... ); } };

	template <typename T>
	inline Double3 Double3Ctor (const NVec<T,1> &x) {
		return Helpers::BuildNode< Double3 >( "Double3Ctor", x );
	}

	template <typename T>
	inline Double3 Double3Ctor (const NVec<T,2> &xy) {
		return Helpers::BuildNode< Double3 >( "Double3Ctor", xy );
	}

	template <typename T>
	inline Double3 Double3Ctor (const NVec<T,3> &xyz) {
		return Helpers::BuildNode< Double3 >( "Double3Ctor", xyz );
	}

	template <typename T>
	inline Double3 Double3Ctor (const NVec<T,4> &xyzw) {
		return Helpers::BuildNode< Double3 >( "Double3Ctor", xyzw );
	}

	template <typename T>
	inline Double3 Double3Ctor (const NVec<T,2> &xy, const NVec<T,1> &z) {
		return Helpers::BuildNode< Double3 >( "Double3Ctor", xy, z );
	}

	template <typename T>
	inline Double3 Double3Ctor (const NVec<T,1> &x, const NVec<T,2> &yz) {
		return Helpers::BuildNode< Double3 >( "Double3Ctor", x, yz );
	}

	template <typename T>
	inline Double3 Double3Ctor (const NVec<T,1> &x, const NVec<T,1> &y, const NVec<T,1> &z) {
		return Helpers::BuildNode< Double3 >( "Double3Ctor", x, y, z );
	}

	template <>  struct _VecConstructors< Double3 >  {template <typename ...Args> static inline Double3  Ctor (const Args& ...args) { return Double3Ctor( args... ); } };

	template <typename T>
	inline Double4 Double4Ctor (const NVec<T,1> &x) {
		return Helpers::BuildNode< Double4 >( "Double4Ctor", x );
	}

	template <typename T>
	inline Double4 Double4Ctor (const NVec<T,2> &xy) {
		return Helpers::BuildNode< Double4 >( "Double4Ctor", xy );
	}

	template <typename T>
	inline Double4 Double4Ctor (const NVec<T,3> &xyz) {
		return Helpers::BuildNode< Double4 >( "Double4Ctor", xyz );
	}

	template <typename T>
	inline Double4 Double4Ctor (const NVec<T,4> &xyzw) {
		return Helpers::BuildNode< Double4 >( "Double4Ctor", xyzw );
	}

	template <typename T>
	inline Double4 Double4Ctor (const NVec<T,3> &xyz, const NVec<T,1> &w) {
		return Helpers::BuildNode< Double4 >( "Double4Ctor", xyz, w );
	}

	template <typename T>
	inline Double4 Double4Ctor (const NVec<T,2> &xy, const NVec<T,2> &zw) {
		return Helpers::BuildNode< Double4 >( "Double4Ctor", xy, zw );
	}

	template <typename T>
	inline Double4 Double4Ctor (const NVec<T,1> &x, const NVec<T,3> &yzw) {
		return Helpers::BuildNode< Double4 >( "Double4Ctor", x, yzw );
	}

	template <typename T>
	inline Double4 Double4Ctor (const NVec<T,2> &xy, const NVec<T,1> &z, const NVec<T,1> &w) {
		return Helpers::BuildNode< Double4 >( "Double4Ctor", xy, z, w );
	}

	template <typename T>
	inline Double4 Double4Ctor (const NVec<T,1> &x, const NVec<T,2> &yz, const NVec<T,1> &w) {
		return Helpers::BuildNode< Double4 >( "Double4Ctor", x, yz, w );
	}

	template <typename T>
	inline Double4 Double4Ctor (const NVec<T,1> &x, const NVec<T,1> &y, const NVec<T,2> &zw) {
		return Helpers::BuildNode< Double4 >( "Double4Ctor", x, y, zw );
	}

	template <typename T>
	inline Double4 Double4Ctor (const NVec<T,1> &x, const NVec<T,1> &y, const NVec<T,1> &z, const NVec<T,1> &w) {
		return Helpers::BuildNode< Double4 >( "Double4Ctor", x, y, z, w );
	}

	template <>  struct _VecConstructors< Double4 >  {template <typename ...Args> static inline Double4  Ctor (const Args& ...args) { return Double4Ctor( args... ); } };



}	// ShaderNodes
}	// ShaderEditor
