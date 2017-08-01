// This is generated file, don't change anything!

#include "Engine/Shaders/Shader/NodeTypes.h"
#include "Engine/Shaders/Shader/ImageType.hpp"
#include "Engine/Shaders/Shader/AllNodeFuncs.h"

namespace ShaderEditor
{
namespace _ShaderNodesHidden_
{
	struct NodeGetter
	{
		static ISrcNodePtr Get (const Node &node)	{ return node._self; }
	};
}

namespace ShaderNodes
{

	using NodeFunctions = _ShaderNodesHidden_::NodeFunctions;
	using ShaderOutputNode = _ShaderNodesHidden_::ShaderOutputNode;
	using NodeGetter = _ShaderNodesHidden_::NodeGetter;


	// Type casts
	Bool2 Bool2Ctor (const Bool &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Bool2Ctor", x );
	}

	Bool2 Bool2Ctor (const Bool3 &xyz) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Bool2Ctor", xyz );
	}

	Bool2 Bool2Ctor (const Bool4 &xyzw) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Bool2Ctor", xyzw );
	}

	Bool2 Bool2Ctor (const Bool &x, const Bool &y) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Bool2Ctor", x, y );
	}

	Bool3 Bool3Ctor (const Bool &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Bool3Ctor", x );
	}

	Bool3 Bool3Ctor (const Bool2 &xy) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Bool3Ctor", xy );
	}

	Bool3 Bool3Ctor (const Bool4 &xyzw) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Bool3Ctor", xyzw );
	}

	Bool3 Bool3Ctor (const Bool2 &xy, const Bool &z) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Bool3Ctor", xy, z );
	}

	Bool3 Bool3Ctor (const Bool &x, const Bool2 &yz) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Bool3Ctor", x, yz );
	}

	Bool3 Bool3Ctor (const Bool &x, const Bool &y, const Bool &z) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Bool3Ctor", x, y, z );
	}

	Bool4 Bool4Ctor (const Bool &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Bool4Ctor", x );
	}

	Bool4 Bool4Ctor (const Bool2 &xy) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Bool4Ctor", xy );
	}

	Bool4 Bool4Ctor (const Bool3 &xyz) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Bool4Ctor", xyz );
	}

	Bool4 Bool4Ctor (const Bool3 &xyz, const Bool &w) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Bool4Ctor", xyz, w );
	}

	Bool4 Bool4Ctor (const Bool2 &xy, const Bool2 &zw) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Bool4Ctor", xy, zw );
	}

	Bool4 Bool4Ctor (const Bool &x, const Bool3 &yzw) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Bool4Ctor", x, yzw );
	}

	Bool4 Bool4Ctor (const Bool2 &xy, const Bool &z, const Bool &w) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Bool4Ctor", xy, z, w );
	}

	Bool4 Bool4Ctor (const Bool &x, const Bool2 &yz, const Bool &w) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Bool4Ctor", x, yz, w );
	}

	Bool4 Bool4Ctor (const Bool &x, const Bool &y, const Bool2 &zw) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Bool4Ctor", x, y, zw );
	}

	Bool4 Bool4Ctor (const Bool &x, const Bool &y, const Bool &z, const Bool &w) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Bool4Ctor", x, y, z, w );
	}

	Int IntCtor (const Bool &x) {
		return NodeFunctions::Instance()->Build< Int >( "IntCtor", x );
	}

	Int IntCtor (const UInt &x) {
		return NodeFunctions::Instance()->Build< Int >( "IntCtor", x );
	}

	Int IntCtor (const Long &x) {
		return NodeFunctions::Instance()->Build< Int >( "IntCtor", x );
	}

	Int IntCtor (const ULong &x) {
		return NodeFunctions::Instance()->Build< Int >( "IntCtor", x );
	}

	Int IntCtor (const Float &x) {
		return NodeFunctions::Instance()->Build< Int >( "IntCtor", x );
	}

	Int IntCtor (const Double &x) {
		return NodeFunctions::Instance()->Build< Int >( "IntCtor", x );
	}

	Int2 Int2Ctor (const Bool &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", x );
	}

	Int2 Int2Ctor (const Bool2 &xy) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", xy );
	}

	Int2 Int2Ctor (const Bool3 &xyz) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", xyz );
	}

	Int2 Int2Ctor (const Bool4 &xyzw) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", xyzw );
	}

	Int2 Int2Ctor (const Bool &x, const Bool &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", x, y );
	}

	Int2 Int2Ctor (const Int &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", x );
	}

	Int2 Int2Ctor (const Int3 &xyz) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", xyz );
	}

	Int2 Int2Ctor (const Int4 &xyzw) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", xyzw );
	}

	Int2 Int2Ctor (const Int &x, const Int &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", x, y );
	}

	Int2 Int2Ctor (const UInt &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", x );
	}

	Int2 Int2Ctor (const UInt2 &xy) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", xy );
	}

	Int2 Int2Ctor (const UInt3 &xyz) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", xyz );
	}

	Int2 Int2Ctor (const UInt4 &xyzw) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", xyzw );
	}

	Int2 Int2Ctor (const UInt &x, const UInt &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", x, y );
	}

	Int2 Int2Ctor (const Long &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", x );
	}

	Int2 Int2Ctor (const Long2 &xy) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", xy );
	}

	Int2 Int2Ctor (const Long3 &xyz) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", xyz );
	}

	Int2 Int2Ctor (const Long4 &xyzw) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", xyzw );
	}

	Int2 Int2Ctor (const Long &x, const Long &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", x, y );
	}

	Int2 Int2Ctor (const ULong &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", x );
	}

	Int2 Int2Ctor (const ULong2 &xy) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", xy );
	}

	Int2 Int2Ctor (const ULong3 &xyz) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", xyz );
	}

	Int2 Int2Ctor (const ULong4 &xyzw) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", xyzw );
	}

	Int2 Int2Ctor (const ULong &x, const ULong &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", x, y );
	}

	Int2 Int2Ctor (const Float &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", x );
	}

	Int2 Int2Ctor (const Float2 &xy) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", xy );
	}

	Int2 Int2Ctor (const Float3 &xyz) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", xyz );
	}

	Int2 Int2Ctor (const Float4 &xyzw) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", xyzw );
	}

	Int2 Int2Ctor (const Float &x, const Float &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", x, y );
	}

	Int2 Int2Ctor (const Double &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", x );
	}

	Int2 Int2Ctor (const Double2 &xy) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", xy );
	}

	Int2 Int2Ctor (const Double3 &xyz) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", xyz );
	}

	Int2 Int2Ctor (const Double4 &xyzw) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", xyzw );
	}

	Int2 Int2Ctor (const Double &x, const Double &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "Int2Ctor", x, y );
	}

	Int3 Int3Ctor (const Bool &x) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", x );
	}

	Int3 Int3Ctor (const Bool2 &xy) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xy );
	}

	Int3 Int3Ctor (const Bool3 &xyz) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xyz );
	}

	Int3 Int3Ctor (const Bool4 &xyzw) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xyzw );
	}

	Int3 Int3Ctor (const Bool2 &xy, const Bool &z) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xy, z );
	}

	Int3 Int3Ctor (const Bool &x, const Bool2 &yz) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", x, yz );
	}

	Int3 Int3Ctor (const Bool &x, const Bool &y, const Bool &z) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", x, y, z );
	}

	Int3 Int3Ctor (const Int &x) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", x );
	}

	Int3 Int3Ctor (const Int2 &xy) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xy );
	}

	Int3 Int3Ctor (const Int4 &xyzw) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xyzw );
	}

	Int3 Int3Ctor (const Int2 &xy, const Int &z) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xy, z );
	}

	Int3 Int3Ctor (const Int &x, const Int2 &yz) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", x, yz );
	}

	Int3 Int3Ctor (const Int &x, const Int &y, const Int &z) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", x, y, z );
	}

	Int3 Int3Ctor (const UInt &x) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", x );
	}

	Int3 Int3Ctor (const UInt2 &xy) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xy );
	}

	Int3 Int3Ctor (const UInt3 &xyz) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xyz );
	}

	Int3 Int3Ctor (const UInt4 &xyzw) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xyzw );
	}

	Int3 Int3Ctor (const UInt2 &xy, const UInt &z) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xy, z );
	}

	Int3 Int3Ctor (const UInt &x, const UInt2 &yz) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", x, yz );
	}

	Int3 Int3Ctor (const UInt &x, const UInt &y, const UInt &z) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", x, y, z );
	}

	Int3 Int3Ctor (const Long &x) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", x );
	}

	Int3 Int3Ctor (const Long2 &xy) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xy );
	}

	Int3 Int3Ctor (const Long3 &xyz) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xyz );
	}

	Int3 Int3Ctor (const Long4 &xyzw) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xyzw );
	}

	Int3 Int3Ctor (const Long2 &xy, const Long &z) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xy, z );
	}

	Int3 Int3Ctor (const Long &x, const Long2 &yz) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", x, yz );
	}

	Int3 Int3Ctor (const Long &x, const Long &y, const Long &z) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", x, y, z );
	}

	Int3 Int3Ctor (const ULong &x) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", x );
	}

	Int3 Int3Ctor (const ULong2 &xy) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xy );
	}

	Int3 Int3Ctor (const ULong3 &xyz) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xyz );
	}

	Int3 Int3Ctor (const ULong4 &xyzw) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xyzw );
	}

	Int3 Int3Ctor (const ULong2 &xy, const ULong &z) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xy, z );
	}

	Int3 Int3Ctor (const ULong &x, const ULong2 &yz) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", x, yz );
	}

	Int3 Int3Ctor (const ULong &x, const ULong &y, const ULong &z) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", x, y, z );
	}

	Int3 Int3Ctor (const Float &x) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", x );
	}

	Int3 Int3Ctor (const Float2 &xy) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xy );
	}

	Int3 Int3Ctor (const Float3 &xyz) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xyz );
	}

	Int3 Int3Ctor (const Float4 &xyzw) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xyzw );
	}

	Int3 Int3Ctor (const Float2 &xy, const Float &z) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xy, z );
	}

	Int3 Int3Ctor (const Float &x, const Float2 &yz) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", x, yz );
	}

	Int3 Int3Ctor (const Float &x, const Float &y, const Float &z) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", x, y, z );
	}

	Int3 Int3Ctor (const Double &x) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", x );
	}

	Int3 Int3Ctor (const Double2 &xy) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xy );
	}

	Int3 Int3Ctor (const Double3 &xyz) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xyz );
	}

	Int3 Int3Ctor (const Double4 &xyzw) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xyzw );
	}

	Int3 Int3Ctor (const Double2 &xy, const Double &z) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", xy, z );
	}

	Int3 Int3Ctor (const Double &x, const Double2 &yz) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", x, yz );
	}

	Int3 Int3Ctor (const Double &x, const Double &y, const Double &z) {
		return NodeFunctions::Instance()->Build< Int3 >( "Int3Ctor", x, y, z );
	}

	Int4 Int4Ctor (const Bool &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x );
	}

	Int4 Int4Ctor (const Bool2 &xy) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xy );
	}

	Int4 Int4Ctor (const Bool3 &xyz) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xyz );
	}

	Int4 Int4Ctor (const Bool4 &xyzw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xyzw );
	}

	Int4 Int4Ctor (const Bool3 &xyz, const Bool &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xyz, w );
	}

	Int4 Int4Ctor (const Bool2 &xy, const Bool2 &zw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xy, zw );
	}

	Int4 Int4Ctor (const Bool &x, const Bool3 &yzw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, yzw );
	}

	Int4 Int4Ctor (const Bool2 &xy, const Bool &z, const Bool &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xy, z, w );
	}

	Int4 Int4Ctor (const Bool &x, const Bool2 &yz, const Bool &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, yz, w );
	}

	Int4 Int4Ctor (const Bool &x, const Bool &y, const Bool2 &zw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, y, zw );
	}

	Int4 Int4Ctor (const Bool &x, const Bool &y, const Bool &z, const Bool &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, y, z, w );
	}

	Int4 Int4Ctor (const Int &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x );
	}

	Int4 Int4Ctor (const Int2 &xy) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xy );
	}

	Int4 Int4Ctor (const Int3 &xyz) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xyz );
	}

	Int4 Int4Ctor (const Int3 &xyz, const Int &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xyz, w );
	}

	Int4 Int4Ctor (const Int2 &xy, const Int2 &zw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xy, zw );
	}

	Int4 Int4Ctor (const Int &x, const Int3 &yzw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, yzw );
	}

	Int4 Int4Ctor (const Int2 &xy, const Int &z, const Int &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xy, z, w );
	}

	Int4 Int4Ctor (const Int &x, const Int2 &yz, const Int &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, yz, w );
	}

	Int4 Int4Ctor (const Int &x, const Int &y, const Int2 &zw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, y, zw );
	}

	Int4 Int4Ctor (const Int &x, const Int &y, const Int &z, const Int &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, y, z, w );
	}

	Int4 Int4Ctor (const UInt &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x );
	}

	Int4 Int4Ctor (const UInt2 &xy) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xy );
	}

	Int4 Int4Ctor (const UInt3 &xyz) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xyz );
	}

	Int4 Int4Ctor (const UInt4 &xyzw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xyzw );
	}

	Int4 Int4Ctor (const UInt3 &xyz, const UInt &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xyz, w );
	}

	Int4 Int4Ctor (const UInt2 &xy, const UInt2 &zw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xy, zw );
	}

	Int4 Int4Ctor (const UInt &x, const UInt3 &yzw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, yzw );
	}

	Int4 Int4Ctor (const UInt2 &xy, const UInt &z, const UInt &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xy, z, w );
	}

	Int4 Int4Ctor (const UInt &x, const UInt2 &yz, const UInt &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, yz, w );
	}

	Int4 Int4Ctor (const UInt &x, const UInt &y, const UInt2 &zw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, y, zw );
	}

	Int4 Int4Ctor (const UInt &x, const UInt &y, const UInt &z, const UInt &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, y, z, w );
	}

	Int4 Int4Ctor (const Long &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x );
	}

	Int4 Int4Ctor (const Long2 &xy) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xy );
	}

	Int4 Int4Ctor (const Long3 &xyz) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xyz );
	}

	Int4 Int4Ctor (const Long4 &xyzw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xyzw );
	}

	Int4 Int4Ctor (const Long3 &xyz, const Long &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xyz, w );
	}

	Int4 Int4Ctor (const Long2 &xy, const Long2 &zw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xy, zw );
	}

	Int4 Int4Ctor (const Long &x, const Long3 &yzw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, yzw );
	}

	Int4 Int4Ctor (const Long2 &xy, const Long &z, const Long &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xy, z, w );
	}

	Int4 Int4Ctor (const Long &x, const Long2 &yz, const Long &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, yz, w );
	}

	Int4 Int4Ctor (const Long &x, const Long &y, const Long2 &zw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, y, zw );
	}

	Int4 Int4Ctor (const Long &x, const Long &y, const Long &z, const Long &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, y, z, w );
	}

	Int4 Int4Ctor (const ULong &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x );
	}

	Int4 Int4Ctor (const ULong2 &xy) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xy );
	}

	Int4 Int4Ctor (const ULong3 &xyz) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xyz );
	}

	Int4 Int4Ctor (const ULong4 &xyzw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xyzw );
	}

	Int4 Int4Ctor (const ULong3 &xyz, const ULong &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xyz, w );
	}

	Int4 Int4Ctor (const ULong2 &xy, const ULong2 &zw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xy, zw );
	}

	Int4 Int4Ctor (const ULong &x, const ULong3 &yzw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, yzw );
	}

	Int4 Int4Ctor (const ULong2 &xy, const ULong &z, const ULong &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xy, z, w );
	}

	Int4 Int4Ctor (const ULong &x, const ULong2 &yz, const ULong &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, yz, w );
	}

	Int4 Int4Ctor (const ULong &x, const ULong &y, const ULong2 &zw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, y, zw );
	}

	Int4 Int4Ctor (const ULong &x, const ULong &y, const ULong &z, const ULong &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, y, z, w );
	}

	Int4 Int4Ctor (const Float &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x );
	}

	Int4 Int4Ctor (const Float2 &xy) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xy );
	}

	Int4 Int4Ctor (const Float3 &xyz) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xyz );
	}

	Int4 Int4Ctor (const Float4 &xyzw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xyzw );
	}

	Int4 Int4Ctor (const Float3 &xyz, const Float &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xyz, w );
	}

	Int4 Int4Ctor (const Float2 &xy, const Float2 &zw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xy, zw );
	}

	Int4 Int4Ctor (const Float &x, const Float3 &yzw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, yzw );
	}

	Int4 Int4Ctor (const Float2 &xy, const Float &z, const Float &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xy, z, w );
	}

	Int4 Int4Ctor (const Float &x, const Float2 &yz, const Float &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, yz, w );
	}

	Int4 Int4Ctor (const Float &x, const Float &y, const Float2 &zw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, y, zw );
	}

	Int4 Int4Ctor (const Float &x, const Float &y, const Float &z, const Float &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, y, z, w );
	}

	Int4 Int4Ctor (const Double &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x );
	}

	Int4 Int4Ctor (const Double2 &xy) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xy );
	}

	Int4 Int4Ctor (const Double3 &xyz) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xyz );
	}

	Int4 Int4Ctor (const Double4 &xyzw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xyzw );
	}

	Int4 Int4Ctor (const Double3 &xyz, const Double &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xyz, w );
	}

	Int4 Int4Ctor (const Double2 &xy, const Double2 &zw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xy, zw );
	}

	Int4 Int4Ctor (const Double &x, const Double3 &yzw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, yzw );
	}

	Int4 Int4Ctor (const Double2 &xy, const Double &z, const Double &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", xy, z, w );
	}

	Int4 Int4Ctor (const Double &x, const Double2 &yz, const Double &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, yz, w );
	}

	Int4 Int4Ctor (const Double &x, const Double &y, const Double2 &zw) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, y, zw );
	}

	Int4 Int4Ctor (const Double &x, const Double &y, const Double &z, const Double &w) {
		return NodeFunctions::Instance()->Build< Int4 >( "Int4Ctor", x, y, z, w );
	}

	UInt UIntCtor (const Bool &x) {
		return NodeFunctions::Instance()->Build< UInt >( "UIntCtor", x );
	}

	UInt UIntCtor (const Int &x) {
		return NodeFunctions::Instance()->Build< UInt >( "UIntCtor", x );
	}

	UInt UIntCtor (const Long &x) {
		return NodeFunctions::Instance()->Build< UInt >( "UIntCtor", x );
	}

	UInt UIntCtor (const ULong &x) {
		return NodeFunctions::Instance()->Build< UInt >( "UIntCtor", x );
	}

	UInt UIntCtor (const Float &x) {
		return NodeFunctions::Instance()->Build< UInt >( "UIntCtor", x );
	}

	UInt UIntCtor (const Double &x) {
		return NodeFunctions::Instance()->Build< UInt >( "UIntCtor", x );
	}

	UInt2 UInt2Ctor (const Bool &x) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", x );
	}

	UInt2 UInt2Ctor (const Bool2 &xy) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", xy );
	}

	UInt2 UInt2Ctor (const Bool3 &xyz) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", xyz );
	}

	UInt2 UInt2Ctor (const Bool4 &xyzw) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", xyzw );
	}

	UInt2 UInt2Ctor (const Bool &x, const Bool &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", x, y );
	}

	UInt2 UInt2Ctor (const Int &x) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", x );
	}

	UInt2 UInt2Ctor (const Int2 &xy) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", xy );
	}

	UInt2 UInt2Ctor (const Int3 &xyz) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", xyz );
	}

	UInt2 UInt2Ctor (const Int4 &xyzw) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", xyzw );
	}

	UInt2 UInt2Ctor (const Int &x, const Int &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", x, y );
	}

	UInt2 UInt2Ctor (const UInt &x) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", x );
	}

	UInt2 UInt2Ctor (const UInt3 &xyz) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", xyz );
	}

	UInt2 UInt2Ctor (const UInt4 &xyzw) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", xyzw );
	}

	UInt2 UInt2Ctor (const UInt &x, const UInt &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", x, y );
	}

	UInt2 UInt2Ctor (const Long &x) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", x );
	}

	UInt2 UInt2Ctor (const Long2 &xy) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", xy );
	}

	UInt2 UInt2Ctor (const Long3 &xyz) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", xyz );
	}

	UInt2 UInt2Ctor (const Long4 &xyzw) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", xyzw );
	}

	UInt2 UInt2Ctor (const Long &x, const Long &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", x, y );
	}

	UInt2 UInt2Ctor (const ULong &x) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", x );
	}

	UInt2 UInt2Ctor (const ULong2 &xy) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", xy );
	}

	UInt2 UInt2Ctor (const ULong3 &xyz) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", xyz );
	}

	UInt2 UInt2Ctor (const ULong4 &xyzw) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", xyzw );
	}

	UInt2 UInt2Ctor (const ULong &x, const ULong &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", x, y );
	}

	UInt2 UInt2Ctor (const Float &x) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", x );
	}

	UInt2 UInt2Ctor (const Float2 &xy) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", xy );
	}

	UInt2 UInt2Ctor (const Float3 &xyz) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", xyz );
	}

	UInt2 UInt2Ctor (const Float4 &xyzw) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", xyzw );
	}

	UInt2 UInt2Ctor (const Float &x, const Float &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", x, y );
	}

	UInt2 UInt2Ctor (const Double &x) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", x );
	}

	UInt2 UInt2Ctor (const Double2 &xy) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", xy );
	}

	UInt2 UInt2Ctor (const Double3 &xyz) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", xyz );
	}

	UInt2 UInt2Ctor (const Double4 &xyzw) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", xyzw );
	}

	UInt2 UInt2Ctor (const Double &x, const Double &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "UInt2Ctor", x, y );
	}

	UInt3 UInt3Ctor (const Bool &x) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", x );
	}

	UInt3 UInt3Ctor (const Bool2 &xy) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xy );
	}

	UInt3 UInt3Ctor (const Bool3 &xyz) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xyz );
	}

	UInt3 UInt3Ctor (const Bool4 &xyzw) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xyzw );
	}

	UInt3 UInt3Ctor (const Bool2 &xy, const Bool &z) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xy, z );
	}

	UInt3 UInt3Ctor (const Bool &x, const Bool2 &yz) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", x, yz );
	}

	UInt3 UInt3Ctor (const Bool &x, const Bool &y, const Bool &z) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", x, y, z );
	}

	UInt3 UInt3Ctor (const Int &x) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", x );
	}

	UInt3 UInt3Ctor (const Int2 &xy) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xy );
	}

	UInt3 UInt3Ctor (const Int3 &xyz) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xyz );
	}

	UInt3 UInt3Ctor (const Int4 &xyzw) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xyzw );
	}

	UInt3 UInt3Ctor (const Int2 &xy, const Int &z) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xy, z );
	}

	UInt3 UInt3Ctor (const Int &x, const Int2 &yz) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", x, yz );
	}

	UInt3 UInt3Ctor (const Int &x, const Int &y, const Int &z) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", x, y, z );
	}

	UInt3 UInt3Ctor (const UInt &x) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", x );
	}

	UInt3 UInt3Ctor (const UInt2 &xy) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xy );
	}

	UInt3 UInt3Ctor (const UInt4 &xyzw) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xyzw );
	}

	UInt3 UInt3Ctor (const UInt2 &xy, const UInt &z) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xy, z );
	}

	UInt3 UInt3Ctor (const UInt &x, const UInt2 &yz) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", x, yz );
	}

	UInt3 UInt3Ctor (const UInt &x, const UInt &y, const UInt &z) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", x, y, z );
	}

	UInt3 UInt3Ctor (const Long &x) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", x );
	}

	UInt3 UInt3Ctor (const Long2 &xy) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xy );
	}

	UInt3 UInt3Ctor (const Long3 &xyz) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xyz );
	}

	UInt3 UInt3Ctor (const Long4 &xyzw) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xyzw );
	}

	UInt3 UInt3Ctor (const Long2 &xy, const Long &z) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xy, z );
	}

	UInt3 UInt3Ctor (const Long &x, const Long2 &yz) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", x, yz );
	}

	UInt3 UInt3Ctor (const Long &x, const Long &y, const Long &z) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", x, y, z );
	}

	UInt3 UInt3Ctor (const ULong &x) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", x );
	}

	UInt3 UInt3Ctor (const ULong2 &xy) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xy );
	}

	UInt3 UInt3Ctor (const ULong3 &xyz) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xyz );
	}

	UInt3 UInt3Ctor (const ULong4 &xyzw) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xyzw );
	}

	UInt3 UInt3Ctor (const ULong2 &xy, const ULong &z) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xy, z );
	}

	UInt3 UInt3Ctor (const ULong &x, const ULong2 &yz) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", x, yz );
	}

	UInt3 UInt3Ctor (const ULong &x, const ULong &y, const ULong &z) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", x, y, z );
	}

	UInt3 UInt3Ctor (const Float &x) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", x );
	}

	UInt3 UInt3Ctor (const Float2 &xy) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xy );
	}

	UInt3 UInt3Ctor (const Float3 &xyz) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xyz );
	}

	UInt3 UInt3Ctor (const Float4 &xyzw) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xyzw );
	}

	UInt3 UInt3Ctor (const Float2 &xy, const Float &z) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xy, z );
	}

	UInt3 UInt3Ctor (const Float &x, const Float2 &yz) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", x, yz );
	}

	UInt3 UInt3Ctor (const Float &x, const Float &y, const Float &z) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", x, y, z );
	}

	UInt3 UInt3Ctor (const Double &x) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", x );
	}

	UInt3 UInt3Ctor (const Double2 &xy) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xy );
	}

	UInt3 UInt3Ctor (const Double3 &xyz) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xyz );
	}

	UInt3 UInt3Ctor (const Double4 &xyzw) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xyzw );
	}

	UInt3 UInt3Ctor (const Double2 &xy, const Double &z) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", xy, z );
	}

	UInt3 UInt3Ctor (const Double &x, const Double2 &yz) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", x, yz );
	}

	UInt3 UInt3Ctor (const Double &x, const Double &y, const Double &z) {
		return NodeFunctions::Instance()->Build< UInt3 >( "UInt3Ctor", x, y, z );
	}

	UInt4 UInt4Ctor (const Bool &x) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x );
	}

	UInt4 UInt4Ctor (const Bool2 &xy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xy );
	}

	UInt4 UInt4Ctor (const Bool3 &xyz) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xyz );
	}

	UInt4 UInt4Ctor (const Bool4 &xyzw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xyzw );
	}

	UInt4 UInt4Ctor (const Bool3 &xyz, const Bool &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xyz, w );
	}

	UInt4 UInt4Ctor (const Bool2 &xy, const Bool2 &zw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xy, zw );
	}

	UInt4 UInt4Ctor (const Bool &x, const Bool3 &yzw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, yzw );
	}

	UInt4 UInt4Ctor (const Bool2 &xy, const Bool &z, const Bool &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xy, z, w );
	}

	UInt4 UInt4Ctor (const Bool &x, const Bool2 &yz, const Bool &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, yz, w );
	}

	UInt4 UInt4Ctor (const Bool &x, const Bool &y, const Bool2 &zw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, y, zw );
	}

	UInt4 UInt4Ctor (const Bool &x, const Bool &y, const Bool &z, const Bool &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, y, z, w );
	}

	UInt4 UInt4Ctor (const Int &x) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x );
	}

	UInt4 UInt4Ctor (const Int2 &xy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xy );
	}

	UInt4 UInt4Ctor (const Int3 &xyz) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xyz );
	}

	UInt4 UInt4Ctor (const Int4 &xyzw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xyzw );
	}

	UInt4 UInt4Ctor (const Int3 &xyz, const Int &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xyz, w );
	}

	UInt4 UInt4Ctor (const Int2 &xy, const Int2 &zw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xy, zw );
	}

	UInt4 UInt4Ctor (const Int &x, const Int3 &yzw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, yzw );
	}

	UInt4 UInt4Ctor (const Int2 &xy, const Int &z, const Int &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xy, z, w );
	}

	UInt4 UInt4Ctor (const Int &x, const Int2 &yz, const Int &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, yz, w );
	}

	UInt4 UInt4Ctor (const Int &x, const Int &y, const Int2 &zw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, y, zw );
	}

	UInt4 UInt4Ctor (const Int &x, const Int &y, const Int &z, const Int &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, y, z, w );
	}

	UInt4 UInt4Ctor (const UInt &x) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x );
	}

	UInt4 UInt4Ctor (const UInt2 &xy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xy );
	}

	UInt4 UInt4Ctor (const UInt3 &xyz) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xyz );
	}

	UInt4 UInt4Ctor (const UInt3 &xyz, const UInt &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xyz, w );
	}

	UInt4 UInt4Ctor (const UInt2 &xy, const UInt2 &zw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xy, zw );
	}

	UInt4 UInt4Ctor (const UInt &x, const UInt3 &yzw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, yzw );
	}

	UInt4 UInt4Ctor (const UInt2 &xy, const UInt &z, const UInt &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xy, z, w );
	}

	UInt4 UInt4Ctor (const UInt &x, const UInt2 &yz, const UInt &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, yz, w );
	}

	UInt4 UInt4Ctor (const UInt &x, const UInt &y, const UInt2 &zw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, y, zw );
	}

	UInt4 UInt4Ctor (const UInt &x, const UInt &y, const UInt &z, const UInt &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, y, z, w );
	}

	UInt4 UInt4Ctor (const Long &x) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x );
	}

	UInt4 UInt4Ctor (const Long2 &xy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xy );
	}

	UInt4 UInt4Ctor (const Long3 &xyz) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xyz );
	}

	UInt4 UInt4Ctor (const Long4 &xyzw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xyzw );
	}

	UInt4 UInt4Ctor (const Long3 &xyz, const Long &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xyz, w );
	}

	UInt4 UInt4Ctor (const Long2 &xy, const Long2 &zw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xy, zw );
	}

	UInt4 UInt4Ctor (const Long &x, const Long3 &yzw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, yzw );
	}

	UInt4 UInt4Ctor (const Long2 &xy, const Long &z, const Long &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xy, z, w );
	}

	UInt4 UInt4Ctor (const Long &x, const Long2 &yz, const Long &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, yz, w );
	}

	UInt4 UInt4Ctor (const Long &x, const Long &y, const Long2 &zw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, y, zw );
	}

	UInt4 UInt4Ctor (const Long &x, const Long &y, const Long &z, const Long &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, y, z, w );
	}

	UInt4 UInt4Ctor (const ULong &x) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x );
	}

	UInt4 UInt4Ctor (const ULong2 &xy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xy );
	}

	UInt4 UInt4Ctor (const ULong3 &xyz) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xyz );
	}

	UInt4 UInt4Ctor (const ULong4 &xyzw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xyzw );
	}

	UInt4 UInt4Ctor (const ULong3 &xyz, const ULong &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xyz, w );
	}

	UInt4 UInt4Ctor (const ULong2 &xy, const ULong2 &zw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xy, zw );
	}

	UInt4 UInt4Ctor (const ULong &x, const ULong3 &yzw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, yzw );
	}

	UInt4 UInt4Ctor (const ULong2 &xy, const ULong &z, const ULong &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xy, z, w );
	}

	UInt4 UInt4Ctor (const ULong &x, const ULong2 &yz, const ULong &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, yz, w );
	}

	UInt4 UInt4Ctor (const ULong &x, const ULong &y, const ULong2 &zw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, y, zw );
	}

	UInt4 UInt4Ctor (const ULong &x, const ULong &y, const ULong &z, const ULong &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, y, z, w );
	}

	UInt4 UInt4Ctor (const Float &x) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x );
	}

	UInt4 UInt4Ctor (const Float2 &xy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xy );
	}

	UInt4 UInt4Ctor (const Float3 &xyz) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xyz );
	}

	UInt4 UInt4Ctor (const Float4 &xyzw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xyzw );
	}

	UInt4 UInt4Ctor (const Float3 &xyz, const Float &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xyz, w );
	}

	UInt4 UInt4Ctor (const Float2 &xy, const Float2 &zw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xy, zw );
	}

	UInt4 UInt4Ctor (const Float &x, const Float3 &yzw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, yzw );
	}

	UInt4 UInt4Ctor (const Float2 &xy, const Float &z, const Float &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xy, z, w );
	}

	UInt4 UInt4Ctor (const Float &x, const Float2 &yz, const Float &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, yz, w );
	}

	UInt4 UInt4Ctor (const Float &x, const Float &y, const Float2 &zw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, y, zw );
	}

	UInt4 UInt4Ctor (const Float &x, const Float &y, const Float &z, const Float &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, y, z, w );
	}

	UInt4 UInt4Ctor (const Double &x) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x );
	}

	UInt4 UInt4Ctor (const Double2 &xy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xy );
	}

	UInt4 UInt4Ctor (const Double3 &xyz) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xyz );
	}

	UInt4 UInt4Ctor (const Double4 &xyzw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xyzw );
	}

	UInt4 UInt4Ctor (const Double3 &xyz, const Double &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xyz, w );
	}

	UInt4 UInt4Ctor (const Double2 &xy, const Double2 &zw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xy, zw );
	}

	UInt4 UInt4Ctor (const Double &x, const Double3 &yzw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, yzw );
	}

	UInt4 UInt4Ctor (const Double2 &xy, const Double &z, const Double &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", xy, z, w );
	}

	UInt4 UInt4Ctor (const Double &x, const Double2 &yz, const Double &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, yz, w );
	}

	UInt4 UInt4Ctor (const Double &x, const Double &y, const Double2 &zw) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, y, zw );
	}

	UInt4 UInt4Ctor (const Double &x, const Double &y, const Double &z, const Double &w) {
		return NodeFunctions::Instance()->Build< UInt4 >( "UInt4Ctor", x, y, z, w );
	}

	Long LongCtor (const Bool &x) {
		return NodeFunctions::Instance()->Build< Long >( "LongCtor", x );
	}

	Long LongCtor (const Int &x) {
		return NodeFunctions::Instance()->Build< Long >( "LongCtor", x );
	}

	Long LongCtor (const UInt &x) {
		return NodeFunctions::Instance()->Build< Long >( "LongCtor", x );
	}

	Long LongCtor (const ULong &x) {
		return NodeFunctions::Instance()->Build< Long >( "LongCtor", x );
	}

	Long LongCtor (const Float &x) {
		return NodeFunctions::Instance()->Build< Long >( "LongCtor", x );
	}

	Long LongCtor (const Double &x) {
		return NodeFunctions::Instance()->Build< Long >( "LongCtor", x );
	}

	Long2 Long2Ctor (const Bool &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", x );
	}

	Long2 Long2Ctor (const Bool2 &xy) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", xy );
	}

	Long2 Long2Ctor (const Bool3 &xyz) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", xyz );
	}

	Long2 Long2Ctor (const Bool4 &xyzw) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", xyzw );
	}

	Long2 Long2Ctor (const Bool &x, const Bool &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", x, y );
	}

	Long2 Long2Ctor (const Int &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", x );
	}

	Long2 Long2Ctor (const Int2 &xy) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", xy );
	}

	Long2 Long2Ctor (const Int3 &xyz) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", xyz );
	}

	Long2 Long2Ctor (const Int4 &xyzw) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", xyzw );
	}

	Long2 Long2Ctor (const Int &x, const Int &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", x, y );
	}

	Long2 Long2Ctor (const UInt &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", x );
	}

	Long2 Long2Ctor (const UInt2 &xy) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", xy );
	}

	Long2 Long2Ctor (const UInt3 &xyz) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", xyz );
	}

	Long2 Long2Ctor (const UInt4 &xyzw) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", xyzw );
	}

	Long2 Long2Ctor (const UInt &x, const UInt &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", x, y );
	}

	Long2 Long2Ctor (const Long &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", x );
	}

	Long2 Long2Ctor (const Long3 &xyz) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", xyz );
	}

	Long2 Long2Ctor (const Long4 &xyzw) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", xyzw );
	}

	Long2 Long2Ctor (const Long &x, const Long &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", x, y );
	}

	Long2 Long2Ctor (const ULong &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", x );
	}

	Long2 Long2Ctor (const ULong2 &xy) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", xy );
	}

	Long2 Long2Ctor (const ULong3 &xyz) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", xyz );
	}

	Long2 Long2Ctor (const ULong4 &xyzw) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", xyzw );
	}

	Long2 Long2Ctor (const ULong &x, const ULong &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", x, y );
	}

	Long2 Long2Ctor (const Float &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", x );
	}

	Long2 Long2Ctor (const Float2 &xy) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", xy );
	}

	Long2 Long2Ctor (const Float3 &xyz) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", xyz );
	}

	Long2 Long2Ctor (const Float4 &xyzw) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", xyzw );
	}

	Long2 Long2Ctor (const Float &x, const Float &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", x, y );
	}

	Long2 Long2Ctor (const Double &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", x );
	}

	Long2 Long2Ctor (const Double2 &xy) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", xy );
	}

	Long2 Long2Ctor (const Double3 &xyz) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", xyz );
	}

	Long2 Long2Ctor (const Double4 &xyzw) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", xyzw );
	}

	Long2 Long2Ctor (const Double &x, const Double &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "Long2Ctor", x, y );
	}

	Long3 Long3Ctor (const Bool &x) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", x );
	}

	Long3 Long3Ctor (const Bool2 &xy) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xy );
	}

	Long3 Long3Ctor (const Bool3 &xyz) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xyz );
	}

	Long3 Long3Ctor (const Bool4 &xyzw) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xyzw );
	}

	Long3 Long3Ctor (const Bool2 &xy, const Bool &z) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xy, z );
	}

	Long3 Long3Ctor (const Bool &x, const Bool2 &yz) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", x, yz );
	}

	Long3 Long3Ctor (const Bool &x, const Bool &y, const Bool &z) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", x, y, z );
	}

	Long3 Long3Ctor (const Int &x) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", x );
	}

	Long3 Long3Ctor (const Int2 &xy) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xy );
	}

	Long3 Long3Ctor (const Int3 &xyz) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xyz );
	}

	Long3 Long3Ctor (const Int4 &xyzw) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xyzw );
	}

	Long3 Long3Ctor (const Int2 &xy, const Int &z) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xy, z );
	}

	Long3 Long3Ctor (const Int &x, const Int2 &yz) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", x, yz );
	}

	Long3 Long3Ctor (const Int &x, const Int &y, const Int &z) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", x, y, z );
	}

	Long3 Long3Ctor (const UInt &x) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", x );
	}

	Long3 Long3Ctor (const UInt2 &xy) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xy );
	}

	Long3 Long3Ctor (const UInt3 &xyz) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xyz );
	}

	Long3 Long3Ctor (const UInt4 &xyzw) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xyzw );
	}

	Long3 Long3Ctor (const UInt2 &xy, const UInt &z) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xy, z );
	}

	Long3 Long3Ctor (const UInt &x, const UInt2 &yz) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", x, yz );
	}

	Long3 Long3Ctor (const UInt &x, const UInt &y, const UInt &z) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", x, y, z );
	}

	Long3 Long3Ctor (const Long &x) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", x );
	}

	Long3 Long3Ctor (const Long2 &xy) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xy );
	}

	Long3 Long3Ctor (const Long4 &xyzw) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xyzw );
	}

	Long3 Long3Ctor (const Long2 &xy, const Long &z) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xy, z );
	}

	Long3 Long3Ctor (const Long &x, const Long2 &yz) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", x, yz );
	}

	Long3 Long3Ctor (const Long &x, const Long &y, const Long &z) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", x, y, z );
	}

	Long3 Long3Ctor (const ULong &x) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", x );
	}

	Long3 Long3Ctor (const ULong2 &xy) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xy );
	}

	Long3 Long3Ctor (const ULong3 &xyz) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xyz );
	}

	Long3 Long3Ctor (const ULong4 &xyzw) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xyzw );
	}

	Long3 Long3Ctor (const ULong2 &xy, const ULong &z) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xy, z );
	}

	Long3 Long3Ctor (const ULong &x, const ULong2 &yz) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", x, yz );
	}

	Long3 Long3Ctor (const ULong &x, const ULong &y, const ULong &z) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", x, y, z );
	}

	Long3 Long3Ctor (const Float &x) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", x );
	}

	Long3 Long3Ctor (const Float2 &xy) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xy );
	}

	Long3 Long3Ctor (const Float3 &xyz) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xyz );
	}

	Long3 Long3Ctor (const Float4 &xyzw) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xyzw );
	}

	Long3 Long3Ctor (const Float2 &xy, const Float &z) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xy, z );
	}

	Long3 Long3Ctor (const Float &x, const Float2 &yz) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", x, yz );
	}

	Long3 Long3Ctor (const Float &x, const Float &y, const Float &z) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", x, y, z );
	}

	Long3 Long3Ctor (const Double &x) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", x );
	}

	Long3 Long3Ctor (const Double2 &xy) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xy );
	}

	Long3 Long3Ctor (const Double3 &xyz) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xyz );
	}

	Long3 Long3Ctor (const Double4 &xyzw) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xyzw );
	}

	Long3 Long3Ctor (const Double2 &xy, const Double &z) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", xy, z );
	}

	Long3 Long3Ctor (const Double &x, const Double2 &yz) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", x, yz );
	}

	Long3 Long3Ctor (const Double &x, const Double &y, const Double &z) {
		return NodeFunctions::Instance()->Build< Long3 >( "Long3Ctor", x, y, z );
	}

	Long4 Long4Ctor (const Bool &x) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x );
	}

	Long4 Long4Ctor (const Bool2 &xy) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xy );
	}

	Long4 Long4Ctor (const Bool3 &xyz) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xyz );
	}

	Long4 Long4Ctor (const Bool4 &xyzw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xyzw );
	}

	Long4 Long4Ctor (const Bool3 &xyz, const Bool &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xyz, w );
	}

	Long4 Long4Ctor (const Bool2 &xy, const Bool2 &zw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xy, zw );
	}

	Long4 Long4Ctor (const Bool &x, const Bool3 &yzw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, yzw );
	}

	Long4 Long4Ctor (const Bool2 &xy, const Bool &z, const Bool &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xy, z, w );
	}

	Long4 Long4Ctor (const Bool &x, const Bool2 &yz, const Bool &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, yz, w );
	}

	Long4 Long4Ctor (const Bool &x, const Bool &y, const Bool2 &zw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, y, zw );
	}

	Long4 Long4Ctor (const Bool &x, const Bool &y, const Bool &z, const Bool &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, y, z, w );
	}

	Long4 Long4Ctor (const Int &x) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x );
	}

	Long4 Long4Ctor (const Int2 &xy) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xy );
	}

	Long4 Long4Ctor (const Int3 &xyz) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xyz );
	}

	Long4 Long4Ctor (const Int4 &xyzw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xyzw );
	}

	Long4 Long4Ctor (const Int3 &xyz, const Int &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xyz, w );
	}

	Long4 Long4Ctor (const Int2 &xy, const Int2 &zw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xy, zw );
	}

	Long4 Long4Ctor (const Int &x, const Int3 &yzw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, yzw );
	}

	Long4 Long4Ctor (const Int2 &xy, const Int &z, const Int &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xy, z, w );
	}

	Long4 Long4Ctor (const Int &x, const Int2 &yz, const Int &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, yz, w );
	}

	Long4 Long4Ctor (const Int &x, const Int &y, const Int2 &zw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, y, zw );
	}

	Long4 Long4Ctor (const Int &x, const Int &y, const Int &z, const Int &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, y, z, w );
	}

	Long4 Long4Ctor (const UInt &x) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x );
	}

	Long4 Long4Ctor (const UInt2 &xy) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xy );
	}

	Long4 Long4Ctor (const UInt3 &xyz) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xyz );
	}

	Long4 Long4Ctor (const UInt4 &xyzw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xyzw );
	}

	Long4 Long4Ctor (const UInt3 &xyz, const UInt &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xyz, w );
	}

	Long4 Long4Ctor (const UInt2 &xy, const UInt2 &zw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xy, zw );
	}

	Long4 Long4Ctor (const UInt &x, const UInt3 &yzw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, yzw );
	}

	Long4 Long4Ctor (const UInt2 &xy, const UInt &z, const UInt &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xy, z, w );
	}

	Long4 Long4Ctor (const UInt &x, const UInt2 &yz, const UInt &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, yz, w );
	}

	Long4 Long4Ctor (const UInt &x, const UInt &y, const UInt2 &zw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, y, zw );
	}

	Long4 Long4Ctor (const UInt &x, const UInt &y, const UInt &z, const UInt &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, y, z, w );
	}

	Long4 Long4Ctor (const Long &x) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x );
	}

	Long4 Long4Ctor (const Long2 &xy) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xy );
	}

	Long4 Long4Ctor (const Long3 &xyz) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xyz );
	}

	Long4 Long4Ctor (const Long3 &xyz, const Long &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xyz, w );
	}

	Long4 Long4Ctor (const Long2 &xy, const Long2 &zw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xy, zw );
	}

	Long4 Long4Ctor (const Long &x, const Long3 &yzw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, yzw );
	}

	Long4 Long4Ctor (const Long2 &xy, const Long &z, const Long &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xy, z, w );
	}

	Long4 Long4Ctor (const Long &x, const Long2 &yz, const Long &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, yz, w );
	}

	Long4 Long4Ctor (const Long &x, const Long &y, const Long2 &zw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, y, zw );
	}

	Long4 Long4Ctor (const Long &x, const Long &y, const Long &z, const Long &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, y, z, w );
	}

	Long4 Long4Ctor (const ULong &x) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x );
	}

	Long4 Long4Ctor (const ULong2 &xy) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xy );
	}

	Long4 Long4Ctor (const ULong3 &xyz) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xyz );
	}

	Long4 Long4Ctor (const ULong4 &xyzw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xyzw );
	}

	Long4 Long4Ctor (const ULong3 &xyz, const ULong &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xyz, w );
	}

	Long4 Long4Ctor (const ULong2 &xy, const ULong2 &zw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xy, zw );
	}

	Long4 Long4Ctor (const ULong &x, const ULong3 &yzw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, yzw );
	}

	Long4 Long4Ctor (const ULong2 &xy, const ULong &z, const ULong &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xy, z, w );
	}

	Long4 Long4Ctor (const ULong &x, const ULong2 &yz, const ULong &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, yz, w );
	}

	Long4 Long4Ctor (const ULong &x, const ULong &y, const ULong2 &zw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, y, zw );
	}

	Long4 Long4Ctor (const ULong &x, const ULong &y, const ULong &z, const ULong &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, y, z, w );
	}

	Long4 Long4Ctor (const Float &x) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x );
	}

	Long4 Long4Ctor (const Float2 &xy) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xy );
	}

	Long4 Long4Ctor (const Float3 &xyz) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xyz );
	}

	Long4 Long4Ctor (const Float4 &xyzw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xyzw );
	}

	Long4 Long4Ctor (const Float3 &xyz, const Float &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xyz, w );
	}

	Long4 Long4Ctor (const Float2 &xy, const Float2 &zw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xy, zw );
	}

	Long4 Long4Ctor (const Float &x, const Float3 &yzw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, yzw );
	}

	Long4 Long4Ctor (const Float2 &xy, const Float &z, const Float &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xy, z, w );
	}

	Long4 Long4Ctor (const Float &x, const Float2 &yz, const Float &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, yz, w );
	}

	Long4 Long4Ctor (const Float &x, const Float &y, const Float2 &zw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, y, zw );
	}

	Long4 Long4Ctor (const Float &x, const Float &y, const Float &z, const Float &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, y, z, w );
	}

	Long4 Long4Ctor (const Double &x) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x );
	}

	Long4 Long4Ctor (const Double2 &xy) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xy );
	}

	Long4 Long4Ctor (const Double3 &xyz) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xyz );
	}

	Long4 Long4Ctor (const Double4 &xyzw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xyzw );
	}

	Long4 Long4Ctor (const Double3 &xyz, const Double &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xyz, w );
	}

	Long4 Long4Ctor (const Double2 &xy, const Double2 &zw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xy, zw );
	}

	Long4 Long4Ctor (const Double &x, const Double3 &yzw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, yzw );
	}

	Long4 Long4Ctor (const Double2 &xy, const Double &z, const Double &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", xy, z, w );
	}

	Long4 Long4Ctor (const Double &x, const Double2 &yz, const Double &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, yz, w );
	}

	Long4 Long4Ctor (const Double &x, const Double &y, const Double2 &zw) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, y, zw );
	}

	Long4 Long4Ctor (const Double &x, const Double &y, const Double &z, const Double &w) {
		return NodeFunctions::Instance()->Build< Long4 >( "Long4Ctor", x, y, z, w );
	}

	ULong ULongCtor (const Bool &x) {
		return NodeFunctions::Instance()->Build< ULong >( "ULongCtor", x );
	}

	ULong ULongCtor (const Int &x) {
		return NodeFunctions::Instance()->Build< ULong >( "ULongCtor", x );
	}

	ULong ULongCtor (const UInt &x) {
		return NodeFunctions::Instance()->Build< ULong >( "ULongCtor", x );
	}

	ULong ULongCtor (const Long &x) {
		return NodeFunctions::Instance()->Build< ULong >( "ULongCtor", x );
	}

	ULong ULongCtor (const Float &x) {
		return NodeFunctions::Instance()->Build< ULong >( "ULongCtor", x );
	}

	ULong ULongCtor (const Double &x) {
		return NodeFunctions::Instance()->Build< ULong >( "ULongCtor", x );
	}

	ULong2 ULong2Ctor (const Bool &x) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", x );
	}

	ULong2 ULong2Ctor (const Bool2 &xy) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", xy );
	}

	ULong2 ULong2Ctor (const Bool3 &xyz) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", xyz );
	}

	ULong2 ULong2Ctor (const Bool4 &xyzw) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", xyzw );
	}

	ULong2 ULong2Ctor (const Bool &x, const Bool &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", x, y );
	}

	ULong2 ULong2Ctor (const Int &x) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", x );
	}

	ULong2 ULong2Ctor (const Int2 &xy) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", xy );
	}

	ULong2 ULong2Ctor (const Int3 &xyz) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", xyz );
	}

	ULong2 ULong2Ctor (const Int4 &xyzw) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", xyzw );
	}

	ULong2 ULong2Ctor (const Int &x, const Int &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", x, y );
	}

	ULong2 ULong2Ctor (const UInt &x) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", x );
	}

	ULong2 ULong2Ctor (const UInt2 &xy) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", xy );
	}

	ULong2 ULong2Ctor (const UInt3 &xyz) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", xyz );
	}

	ULong2 ULong2Ctor (const UInt4 &xyzw) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", xyzw );
	}

	ULong2 ULong2Ctor (const UInt &x, const UInt &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", x, y );
	}

	ULong2 ULong2Ctor (const Long &x) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", x );
	}

	ULong2 ULong2Ctor (const Long2 &xy) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", xy );
	}

	ULong2 ULong2Ctor (const Long3 &xyz) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", xyz );
	}

	ULong2 ULong2Ctor (const Long4 &xyzw) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", xyzw );
	}

	ULong2 ULong2Ctor (const Long &x, const Long &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", x, y );
	}

	ULong2 ULong2Ctor (const ULong &x) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", x );
	}

	ULong2 ULong2Ctor (const ULong3 &xyz) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", xyz );
	}

	ULong2 ULong2Ctor (const ULong4 &xyzw) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", xyzw );
	}

	ULong2 ULong2Ctor (const ULong &x, const ULong &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", x, y );
	}

	ULong2 ULong2Ctor (const Float &x) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", x );
	}

	ULong2 ULong2Ctor (const Float2 &xy) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", xy );
	}

	ULong2 ULong2Ctor (const Float3 &xyz) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", xyz );
	}

	ULong2 ULong2Ctor (const Float4 &xyzw) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", xyzw );
	}

	ULong2 ULong2Ctor (const Float &x, const Float &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", x, y );
	}

	ULong2 ULong2Ctor (const Double &x) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", x );
	}

	ULong2 ULong2Ctor (const Double2 &xy) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", xy );
	}

	ULong2 ULong2Ctor (const Double3 &xyz) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", xyz );
	}

	ULong2 ULong2Ctor (const Double4 &xyzw) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", xyzw );
	}

	ULong2 ULong2Ctor (const Double &x, const Double &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ULong2Ctor", x, y );
	}

	ULong3 ULong3Ctor (const Bool &x) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", x );
	}

	ULong3 ULong3Ctor (const Bool2 &xy) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xy );
	}

	ULong3 ULong3Ctor (const Bool3 &xyz) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xyz );
	}

	ULong3 ULong3Ctor (const Bool4 &xyzw) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xyzw );
	}

	ULong3 ULong3Ctor (const Bool2 &xy, const Bool &z) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xy, z );
	}

	ULong3 ULong3Ctor (const Bool &x, const Bool2 &yz) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", x, yz );
	}

	ULong3 ULong3Ctor (const Bool &x, const Bool &y, const Bool &z) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", x, y, z );
	}

	ULong3 ULong3Ctor (const Int &x) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", x );
	}

	ULong3 ULong3Ctor (const Int2 &xy) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xy );
	}

	ULong3 ULong3Ctor (const Int3 &xyz) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xyz );
	}

	ULong3 ULong3Ctor (const Int4 &xyzw) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xyzw );
	}

	ULong3 ULong3Ctor (const Int2 &xy, const Int &z) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xy, z );
	}

	ULong3 ULong3Ctor (const Int &x, const Int2 &yz) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", x, yz );
	}

	ULong3 ULong3Ctor (const Int &x, const Int &y, const Int &z) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", x, y, z );
	}

	ULong3 ULong3Ctor (const UInt &x) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", x );
	}

	ULong3 ULong3Ctor (const UInt2 &xy) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xy );
	}

	ULong3 ULong3Ctor (const UInt3 &xyz) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xyz );
	}

	ULong3 ULong3Ctor (const UInt4 &xyzw) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xyzw );
	}

	ULong3 ULong3Ctor (const UInt2 &xy, const UInt &z) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xy, z );
	}

	ULong3 ULong3Ctor (const UInt &x, const UInt2 &yz) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", x, yz );
	}

	ULong3 ULong3Ctor (const UInt &x, const UInt &y, const UInt &z) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", x, y, z );
	}

	ULong3 ULong3Ctor (const Long &x) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", x );
	}

	ULong3 ULong3Ctor (const Long2 &xy) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xy );
	}

	ULong3 ULong3Ctor (const Long3 &xyz) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xyz );
	}

	ULong3 ULong3Ctor (const Long4 &xyzw) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xyzw );
	}

	ULong3 ULong3Ctor (const Long2 &xy, const Long &z) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xy, z );
	}

	ULong3 ULong3Ctor (const Long &x, const Long2 &yz) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", x, yz );
	}

	ULong3 ULong3Ctor (const Long &x, const Long &y, const Long &z) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", x, y, z );
	}

	ULong3 ULong3Ctor (const ULong &x) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", x );
	}

	ULong3 ULong3Ctor (const ULong2 &xy) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xy );
	}

	ULong3 ULong3Ctor (const ULong4 &xyzw) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xyzw );
	}

	ULong3 ULong3Ctor (const ULong2 &xy, const ULong &z) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xy, z );
	}

	ULong3 ULong3Ctor (const ULong &x, const ULong2 &yz) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", x, yz );
	}

	ULong3 ULong3Ctor (const ULong &x, const ULong &y, const ULong &z) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", x, y, z );
	}

	ULong3 ULong3Ctor (const Float &x) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", x );
	}

	ULong3 ULong3Ctor (const Float2 &xy) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xy );
	}

	ULong3 ULong3Ctor (const Float3 &xyz) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xyz );
	}

	ULong3 ULong3Ctor (const Float4 &xyzw) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xyzw );
	}

	ULong3 ULong3Ctor (const Float2 &xy, const Float &z) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xy, z );
	}

	ULong3 ULong3Ctor (const Float &x, const Float2 &yz) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", x, yz );
	}

	ULong3 ULong3Ctor (const Float &x, const Float &y, const Float &z) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", x, y, z );
	}

	ULong3 ULong3Ctor (const Double &x) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", x );
	}

	ULong3 ULong3Ctor (const Double2 &xy) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xy );
	}

	ULong3 ULong3Ctor (const Double3 &xyz) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xyz );
	}

	ULong3 ULong3Ctor (const Double4 &xyzw) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xyzw );
	}

	ULong3 ULong3Ctor (const Double2 &xy, const Double &z) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", xy, z );
	}

	ULong3 ULong3Ctor (const Double &x, const Double2 &yz) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", x, yz );
	}

	ULong3 ULong3Ctor (const Double &x, const Double &y, const Double &z) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ULong3Ctor", x, y, z );
	}

	ULong4 ULong4Ctor (const Bool &x) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x );
	}

	ULong4 ULong4Ctor (const Bool2 &xy) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xy );
	}

	ULong4 ULong4Ctor (const Bool3 &xyz) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xyz );
	}

	ULong4 ULong4Ctor (const Bool4 &xyzw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xyzw );
	}

	ULong4 ULong4Ctor (const Bool3 &xyz, const Bool &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xyz, w );
	}

	ULong4 ULong4Ctor (const Bool2 &xy, const Bool2 &zw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xy, zw );
	}

	ULong4 ULong4Ctor (const Bool &x, const Bool3 &yzw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, yzw );
	}

	ULong4 ULong4Ctor (const Bool2 &xy, const Bool &z, const Bool &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xy, z, w );
	}

	ULong4 ULong4Ctor (const Bool &x, const Bool2 &yz, const Bool &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, yz, w );
	}

	ULong4 ULong4Ctor (const Bool &x, const Bool &y, const Bool2 &zw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, y, zw );
	}

	ULong4 ULong4Ctor (const Bool &x, const Bool &y, const Bool &z, const Bool &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, y, z, w );
	}

	ULong4 ULong4Ctor (const Int &x) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x );
	}

	ULong4 ULong4Ctor (const Int2 &xy) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xy );
	}

	ULong4 ULong4Ctor (const Int3 &xyz) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xyz );
	}

	ULong4 ULong4Ctor (const Int4 &xyzw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xyzw );
	}

	ULong4 ULong4Ctor (const Int3 &xyz, const Int &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xyz, w );
	}

	ULong4 ULong4Ctor (const Int2 &xy, const Int2 &zw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xy, zw );
	}

	ULong4 ULong4Ctor (const Int &x, const Int3 &yzw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, yzw );
	}

	ULong4 ULong4Ctor (const Int2 &xy, const Int &z, const Int &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xy, z, w );
	}

	ULong4 ULong4Ctor (const Int &x, const Int2 &yz, const Int &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, yz, w );
	}

	ULong4 ULong4Ctor (const Int &x, const Int &y, const Int2 &zw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, y, zw );
	}

	ULong4 ULong4Ctor (const Int &x, const Int &y, const Int &z, const Int &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, y, z, w );
	}

	ULong4 ULong4Ctor (const UInt &x) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x );
	}

	ULong4 ULong4Ctor (const UInt2 &xy) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xy );
	}

	ULong4 ULong4Ctor (const UInt3 &xyz) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xyz );
	}

	ULong4 ULong4Ctor (const UInt4 &xyzw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xyzw );
	}

	ULong4 ULong4Ctor (const UInt3 &xyz, const UInt &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xyz, w );
	}

	ULong4 ULong4Ctor (const UInt2 &xy, const UInt2 &zw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xy, zw );
	}

	ULong4 ULong4Ctor (const UInt &x, const UInt3 &yzw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, yzw );
	}

	ULong4 ULong4Ctor (const UInt2 &xy, const UInt &z, const UInt &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xy, z, w );
	}

	ULong4 ULong4Ctor (const UInt &x, const UInt2 &yz, const UInt &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, yz, w );
	}

	ULong4 ULong4Ctor (const UInt &x, const UInt &y, const UInt2 &zw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, y, zw );
	}

	ULong4 ULong4Ctor (const UInt &x, const UInt &y, const UInt &z, const UInt &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, y, z, w );
	}

	ULong4 ULong4Ctor (const Long &x) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x );
	}

	ULong4 ULong4Ctor (const Long2 &xy) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xy );
	}

	ULong4 ULong4Ctor (const Long3 &xyz) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xyz );
	}

	ULong4 ULong4Ctor (const Long4 &xyzw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xyzw );
	}

	ULong4 ULong4Ctor (const Long3 &xyz, const Long &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xyz, w );
	}

	ULong4 ULong4Ctor (const Long2 &xy, const Long2 &zw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xy, zw );
	}

	ULong4 ULong4Ctor (const Long &x, const Long3 &yzw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, yzw );
	}

	ULong4 ULong4Ctor (const Long2 &xy, const Long &z, const Long &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xy, z, w );
	}

	ULong4 ULong4Ctor (const Long &x, const Long2 &yz, const Long &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, yz, w );
	}

	ULong4 ULong4Ctor (const Long &x, const Long &y, const Long2 &zw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, y, zw );
	}

	ULong4 ULong4Ctor (const Long &x, const Long &y, const Long &z, const Long &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, y, z, w );
	}

	ULong4 ULong4Ctor (const ULong &x) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x );
	}

	ULong4 ULong4Ctor (const ULong2 &xy) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xy );
	}

	ULong4 ULong4Ctor (const ULong3 &xyz) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xyz );
	}

	ULong4 ULong4Ctor (const ULong3 &xyz, const ULong &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xyz, w );
	}

	ULong4 ULong4Ctor (const ULong2 &xy, const ULong2 &zw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xy, zw );
	}

	ULong4 ULong4Ctor (const ULong &x, const ULong3 &yzw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, yzw );
	}

	ULong4 ULong4Ctor (const ULong2 &xy, const ULong &z, const ULong &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xy, z, w );
	}

	ULong4 ULong4Ctor (const ULong &x, const ULong2 &yz, const ULong &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, yz, w );
	}

	ULong4 ULong4Ctor (const ULong &x, const ULong &y, const ULong2 &zw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, y, zw );
	}

	ULong4 ULong4Ctor (const ULong &x, const ULong &y, const ULong &z, const ULong &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, y, z, w );
	}

	ULong4 ULong4Ctor (const Float &x) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x );
	}

	ULong4 ULong4Ctor (const Float2 &xy) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xy );
	}

	ULong4 ULong4Ctor (const Float3 &xyz) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xyz );
	}

	ULong4 ULong4Ctor (const Float4 &xyzw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xyzw );
	}

	ULong4 ULong4Ctor (const Float3 &xyz, const Float &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xyz, w );
	}

	ULong4 ULong4Ctor (const Float2 &xy, const Float2 &zw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xy, zw );
	}

	ULong4 ULong4Ctor (const Float &x, const Float3 &yzw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, yzw );
	}

	ULong4 ULong4Ctor (const Float2 &xy, const Float &z, const Float &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xy, z, w );
	}

	ULong4 ULong4Ctor (const Float &x, const Float2 &yz, const Float &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, yz, w );
	}

	ULong4 ULong4Ctor (const Float &x, const Float &y, const Float2 &zw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, y, zw );
	}

	ULong4 ULong4Ctor (const Float &x, const Float &y, const Float &z, const Float &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, y, z, w );
	}

	ULong4 ULong4Ctor (const Double &x) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x );
	}

	ULong4 ULong4Ctor (const Double2 &xy) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xy );
	}

	ULong4 ULong4Ctor (const Double3 &xyz) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xyz );
	}

	ULong4 ULong4Ctor (const Double4 &xyzw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xyzw );
	}

	ULong4 ULong4Ctor (const Double3 &xyz, const Double &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xyz, w );
	}

	ULong4 ULong4Ctor (const Double2 &xy, const Double2 &zw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xy, zw );
	}

	ULong4 ULong4Ctor (const Double &x, const Double3 &yzw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, yzw );
	}

	ULong4 ULong4Ctor (const Double2 &xy, const Double &z, const Double &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", xy, z, w );
	}

	ULong4 ULong4Ctor (const Double &x, const Double2 &yz, const Double &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, yz, w );
	}

	ULong4 ULong4Ctor (const Double &x, const Double &y, const Double2 &zw) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, y, zw );
	}

	ULong4 ULong4Ctor (const Double &x, const Double &y, const Double &z, const Double &w) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ULong4Ctor", x, y, z, w );
	}

	Float FloatCtor (const Bool &x) {
		return NodeFunctions::Instance()->Build< Float >( "FloatCtor", x );
	}

	Float FloatCtor (const Int &x) {
		return NodeFunctions::Instance()->Build< Float >( "FloatCtor", x );
	}

	Float FloatCtor (const UInt &x) {
		return NodeFunctions::Instance()->Build< Float >( "FloatCtor", x );
	}

	Float FloatCtor (const Long &x) {
		return NodeFunctions::Instance()->Build< Float >( "FloatCtor", x );
	}

	Float FloatCtor (const ULong &x) {
		return NodeFunctions::Instance()->Build< Float >( "FloatCtor", x );
	}

	Float FloatCtor (const Double &x) {
		return NodeFunctions::Instance()->Build< Float >( "FloatCtor", x );
	}

	Float2 Float2Ctor (const Bool &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", x );
	}

	Float2 Float2Ctor (const Bool2 &xy) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", xy );
	}

	Float2 Float2Ctor (const Bool3 &xyz) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", xyz );
	}

	Float2 Float2Ctor (const Bool4 &xyzw) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", xyzw );
	}

	Float2 Float2Ctor (const Bool &x, const Bool &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", x, y );
	}

	Float2 Float2Ctor (const Int &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", x );
	}

	Float2 Float2Ctor (const Int2 &xy) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", xy );
	}

	Float2 Float2Ctor (const Int3 &xyz) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", xyz );
	}

	Float2 Float2Ctor (const Int4 &xyzw) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", xyzw );
	}

	Float2 Float2Ctor (const Int &x, const Int &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", x, y );
	}

	Float2 Float2Ctor (const UInt &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", x );
	}

	Float2 Float2Ctor (const UInt2 &xy) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", xy );
	}

	Float2 Float2Ctor (const UInt3 &xyz) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", xyz );
	}

	Float2 Float2Ctor (const UInt4 &xyzw) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", xyzw );
	}

	Float2 Float2Ctor (const UInt &x, const UInt &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", x, y );
	}

	Float2 Float2Ctor (const Long &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", x );
	}

	Float2 Float2Ctor (const Long2 &xy) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", xy );
	}

	Float2 Float2Ctor (const Long3 &xyz) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", xyz );
	}

	Float2 Float2Ctor (const Long4 &xyzw) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", xyzw );
	}

	Float2 Float2Ctor (const Long &x, const Long &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", x, y );
	}

	Float2 Float2Ctor (const ULong &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", x );
	}

	Float2 Float2Ctor (const ULong2 &xy) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", xy );
	}

	Float2 Float2Ctor (const ULong3 &xyz) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", xyz );
	}

	Float2 Float2Ctor (const ULong4 &xyzw) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", xyzw );
	}

	Float2 Float2Ctor (const ULong &x, const ULong &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", x, y );
	}

	Float2 Float2Ctor (const Float &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", x );
	}

	Float2 Float2Ctor (const Float3 &xyz) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", xyz );
	}

	Float2 Float2Ctor (const Float4 &xyzw) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", xyzw );
	}

	Float2 Float2Ctor (const Float &x, const Float &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", x, y );
	}

	Float2 Float2Ctor (const Double &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", x );
	}

	Float2 Float2Ctor (const Double2 &xy) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", xy );
	}

	Float2 Float2Ctor (const Double3 &xyz) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", xyz );
	}

	Float2 Float2Ctor (const Double4 &xyzw) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", xyzw );
	}

	Float2 Float2Ctor (const Double &x, const Double &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "Float2Ctor", x, y );
	}

	Float3 Float3Ctor (const Bool &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", x );
	}

	Float3 Float3Ctor (const Bool2 &xy) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xy );
	}

	Float3 Float3Ctor (const Bool3 &xyz) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xyz );
	}

	Float3 Float3Ctor (const Bool4 &xyzw) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xyzw );
	}

	Float3 Float3Ctor (const Bool2 &xy, const Bool &z) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xy, z );
	}

	Float3 Float3Ctor (const Bool &x, const Bool2 &yz) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", x, yz );
	}

	Float3 Float3Ctor (const Bool &x, const Bool &y, const Bool &z) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", x, y, z );
	}

	Float3 Float3Ctor (const Int &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", x );
	}

	Float3 Float3Ctor (const Int2 &xy) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xy );
	}

	Float3 Float3Ctor (const Int3 &xyz) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xyz );
	}

	Float3 Float3Ctor (const Int4 &xyzw) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xyzw );
	}

	Float3 Float3Ctor (const Int2 &xy, const Int &z) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xy, z );
	}

	Float3 Float3Ctor (const Int &x, const Int2 &yz) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", x, yz );
	}

	Float3 Float3Ctor (const Int &x, const Int &y, const Int &z) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", x, y, z );
	}

	Float3 Float3Ctor (const UInt &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", x );
	}

	Float3 Float3Ctor (const UInt2 &xy) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xy );
	}

	Float3 Float3Ctor (const UInt3 &xyz) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xyz );
	}

	Float3 Float3Ctor (const UInt4 &xyzw) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xyzw );
	}

	Float3 Float3Ctor (const UInt2 &xy, const UInt &z) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xy, z );
	}

	Float3 Float3Ctor (const UInt &x, const UInt2 &yz) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", x, yz );
	}

	Float3 Float3Ctor (const UInt &x, const UInt &y, const UInt &z) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", x, y, z );
	}

	Float3 Float3Ctor (const Long &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", x );
	}

	Float3 Float3Ctor (const Long2 &xy) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xy );
	}

	Float3 Float3Ctor (const Long3 &xyz) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xyz );
	}

	Float3 Float3Ctor (const Long4 &xyzw) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xyzw );
	}

	Float3 Float3Ctor (const Long2 &xy, const Long &z) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xy, z );
	}

	Float3 Float3Ctor (const Long &x, const Long2 &yz) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", x, yz );
	}

	Float3 Float3Ctor (const Long &x, const Long &y, const Long &z) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", x, y, z );
	}

	Float3 Float3Ctor (const ULong &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", x );
	}

	Float3 Float3Ctor (const ULong2 &xy) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xy );
	}

	Float3 Float3Ctor (const ULong3 &xyz) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xyz );
	}

	Float3 Float3Ctor (const ULong4 &xyzw) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xyzw );
	}

	Float3 Float3Ctor (const ULong2 &xy, const ULong &z) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xy, z );
	}

	Float3 Float3Ctor (const ULong &x, const ULong2 &yz) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", x, yz );
	}

	Float3 Float3Ctor (const ULong &x, const ULong &y, const ULong &z) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", x, y, z );
	}

	Float3 Float3Ctor (const Float &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", x );
	}

	Float3 Float3Ctor (const Float2 &xy) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xy );
	}

	Float3 Float3Ctor (const Float4 &xyzw) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xyzw );
	}

	Float3 Float3Ctor (const Float2 &xy, const Float &z) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xy, z );
	}

	Float3 Float3Ctor (const Float &x, const Float2 &yz) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", x, yz );
	}

	Float3 Float3Ctor (const Float &x, const Float &y, const Float &z) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", x, y, z );
	}

	Float3 Float3Ctor (const Double &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", x );
	}

	Float3 Float3Ctor (const Double2 &xy) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xy );
	}

	Float3 Float3Ctor (const Double3 &xyz) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xyz );
	}

	Float3 Float3Ctor (const Double4 &xyzw) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xyzw );
	}

	Float3 Float3Ctor (const Double2 &xy, const Double &z) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", xy, z );
	}

	Float3 Float3Ctor (const Double &x, const Double2 &yz) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", x, yz );
	}

	Float3 Float3Ctor (const Double &x, const Double &y, const Double &z) {
		return NodeFunctions::Instance()->Build< Float3 >( "Float3Ctor", x, y, z );
	}

	Float4 Float4Ctor (const Bool &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x );
	}

	Float4 Float4Ctor (const Bool2 &xy) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xy );
	}

	Float4 Float4Ctor (const Bool3 &xyz) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xyz );
	}

	Float4 Float4Ctor (const Bool4 &xyzw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xyzw );
	}

	Float4 Float4Ctor (const Bool3 &xyz, const Bool &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xyz, w );
	}

	Float4 Float4Ctor (const Bool2 &xy, const Bool2 &zw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xy, zw );
	}

	Float4 Float4Ctor (const Bool &x, const Bool3 &yzw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, yzw );
	}

	Float4 Float4Ctor (const Bool2 &xy, const Bool &z, const Bool &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xy, z, w );
	}

	Float4 Float4Ctor (const Bool &x, const Bool2 &yz, const Bool &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, yz, w );
	}

	Float4 Float4Ctor (const Bool &x, const Bool &y, const Bool2 &zw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, y, zw );
	}

	Float4 Float4Ctor (const Bool &x, const Bool &y, const Bool &z, const Bool &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, y, z, w );
	}

	Float4 Float4Ctor (const Int &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x );
	}

	Float4 Float4Ctor (const Int2 &xy) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xy );
	}

	Float4 Float4Ctor (const Int3 &xyz) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xyz );
	}

	Float4 Float4Ctor (const Int4 &xyzw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xyzw );
	}

	Float4 Float4Ctor (const Int3 &xyz, const Int &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xyz, w );
	}

	Float4 Float4Ctor (const Int2 &xy, const Int2 &zw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xy, zw );
	}

	Float4 Float4Ctor (const Int &x, const Int3 &yzw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, yzw );
	}

	Float4 Float4Ctor (const Int2 &xy, const Int &z, const Int &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xy, z, w );
	}

	Float4 Float4Ctor (const Int &x, const Int2 &yz, const Int &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, yz, w );
	}

	Float4 Float4Ctor (const Int &x, const Int &y, const Int2 &zw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, y, zw );
	}

	Float4 Float4Ctor (const Int &x, const Int &y, const Int &z, const Int &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, y, z, w );
	}

	Float4 Float4Ctor (const UInt &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x );
	}

	Float4 Float4Ctor (const UInt2 &xy) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xy );
	}

	Float4 Float4Ctor (const UInt3 &xyz) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xyz );
	}

	Float4 Float4Ctor (const UInt4 &xyzw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xyzw );
	}

	Float4 Float4Ctor (const UInt3 &xyz, const UInt &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xyz, w );
	}

	Float4 Float4Ctor (const UInt2 &xy, const UInt2 &zw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xy, zw );
	}

	Float4 Float4Ctor (const UInt &x, const UInt3 &yzw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, yzw );
	}

	Float4 Float4Ctor (const UInt2 &xy, const UInt &z, const UInt &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xy, z, w );
	}

	Float4 Float4Ctor (const UInt &x, const UInt2 &yz, const UInt &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, yz, w );
	}

	Float4 Float4Ctor (const UInt &x, const UInt &y, const UInt2 &zw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, y, zw );
	}

	Float4 Float4Ctor (const UInt &x, const UInt &y, const UInt &z, const UInt &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, y, z, w );
	}

	Float4 Float4Ctor (const Long &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x );
	}

	Float4 Float4Ctor (const Long2 &xy) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xy );
	}

	Float4 Float4Ctor (const Long3 &xyz) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xyz );
	}

	Float4 Float4Ctor (const Long4 &xyzw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xyzw );
	}

	Float4 Float4Ctor (const Long3 &xyz, const Long &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xyz, w );
	}

	Float4 Float4Ctor (const Long2 &xy, const Long2 &zw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xy, zw );
	}

	Float4 Float4Ctor (const Long &x, const Long3 &yzw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, yzw );
	}

	Float4 Float4Ctor (const Long2 &xy, const Long &z, const Long &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xy, z, w );
	}

	Float4 Float4Ctor (const Long &x, const Long2 &yz, const Long &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, yz, w );
	}

	Float4 Float4Ctor (const Long &x, const Long &y, const Long2 &zw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, y, zw );
	}

	Float4 Float4Ctor (const Long &x, const Long &y, const Long &z, const Long &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, y, z, w );
	}

	Float4 Float4Ctor (const ULong &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x );
	}

	Float4 Float4Ctor (const ULong2 &xy) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xy );
	}

	Float4 Float4Ctor (const ULong3 &xyz) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xyz );
	}

	Float4 Float4Ctor (const ULong4 &xyzw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xyzw );
	}

	Float4 Float4Ctor (const ULong3 &xyz, const ULong &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xyz, w );
	}

	Float4 Float4Ctor (const ULong2 &xy, const ULong2 &zw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xy, zw );
	}

	Float4 Float4Ctor (const ULong &x, const ULong3 &yzw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, yzw );
	}

	Float4 Float4Ctor (const ULong2 &xy, const ULong &z, const ULong &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xy, z, w );
	}

	Float4 Float4Ctor (const ULong &x, const ULong2 &yz, const ULong &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, yz, w );
	}

	Float4 Float4Ctor (const ULong &x, const ULong &y, const ULong2 &zw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, y, zw );
	}

	Float4 Float4Ctor (const ULong &x, const ULong &y, const ULong &z, const ULong &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, y, z, w );
	}

	Float4 Float4Ctor (const Float &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x );
	}

	Float4 Float4Ctor (const Float2 &xy) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xy );
	}

	Float4 Float4Ctor (const Float3 &xyz) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xyz );
	}

	Float4 Float4Ctor (const Float3 &xyz, const Float &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xyz, w );
	}

	Float4 Float4Ctor (const Float2 &xy, const Float2 &zw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xy, zw );
	}

	Float4 Float4Ctor (const Float &x, const Float3 &yzw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, yzw );
	}

	Float4 Float4Ctor (const Float2 &xy, const Float &z, const Float &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xy, z, w );
	}

	Float4 Float4Ctor (const Float &x, const Float2 &yz, const Float &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, yz, w );
	}

	Float4 Float4Ctor (const Float &x, const Float &y, const Float2 &zw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, y, zw );
	}

	Float4 Float4Ctor (const Float &x, const Float &y, const Float &z, const Float &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, y, z, w );
	}

	Float4 Float4Ctor (const Double &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x );
	}

	Float4 Float4Ctor (const Double2 &xy) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xy );
	}

	Float4 Float4Ctor (const Double3 &xyz) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xyz );
	}

	Float4 Float4Ctor (const Double4 &xyzw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xyzw );
	}

	Float4 Float4Ctor (const Double3 &xyz, const Double &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xyz, w );
	}

	Float4 Float4Ctor (const Double2 &xy, const Double2 &zw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xy, zw );
	}

	Float4 Float4Ctor (const Double &x, const Double3 &yzw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, yzw );
	}

	Float4 Float4Ctor (const Double2 &xy, const Double &z, const Double &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", xy, z, w );
	}

	Float4 Float4Ctor (const Double &x, const Double2 &yz, const Double &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, yz, w );
	}

	Float4 Float4Ctor (const Double &x, const Double &y, const Double2 &zw) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, y, zw );
	}

	Float4 Float4Ctor (const Double &x, const Double &y, const Double &z, const Double &w) {
		return NodeFunctions::Instance()->Build< Float4 >( "Float4Ctor", x, y, z, w );
	}

	Double DoubleCtor (const Bool &x) {
		return NodeFunctions::Instance()->Build< Double >( "DoubleCtor", x );
	}

	Double DoubleCtor (const Int &x) {
		return NodeFunctions::Instance()->Build< Double >( "DoubleCtor", x );
	}

	Double DoubleCtor (const UInt &x) {
		return NodeFunctions::Instance()->Build< Double >( "DoubleCtor", x );
	}

	Double DoubleCtor (const Long &x) {
		return NodeFunctions::Instance()->Build< Double >( "DoubleCtor", x );
	}

	Double DoubleCtor (const ULong &x) {
		return NodeFunctions::Instance()->Build< Double >( "DoubleCtor", x );
	}

	Double DoubleCtor (const Float &x) {
		return NodeFunctions::Instance()->Build< Double >( "DoubleCtor", x );
	}

	Double2 Double2Ctor (const Bool &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", x );
	}

	Double2 Double2Ctor (const Bool2 &xy) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", xy );
	}

	Double2 Double2Ctor (const Bool3 &xyz) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", xyz );
	}

	Double2 Double2Ctor (const Bool4 &xyzw) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", xyzw );
	}

	Double2 Double2Ctor (const Bool &x, const Bool &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", x, y );
	}

	Double2 Double2Ctor (const Int &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", x );
	}

	Double2 Double2Ctor (const Int2 &xy) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", xy );
	}

	Double2 Double2Ctor (const Int3 &xyz) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", xyz );
	}

	Double2 Double2Ctor (const Int4 &xyzw) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", xyzw );
	}

	Double2 Double2Ctor (const Int &x, const Int &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", x, y );
	}

	Double2 Double2Ctor (const UInt &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", x );
	}

	Double2 Double2Ctor (const UInt2 &xy) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", xy );
	}

	Double2 Double2Ctor (const UInt3 &xyz) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", xyz );
	}

	Double2 Double2Ctor (const UInt4 &xyzw) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", xyzw );
	}

	Double2 Double2Ctor (const UInt &x, const UInt &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", x, y );
	}

	Double2 Double2Ctor (const Long &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", x );
	}

	Double2 Double2Ctor (const Long2 &xy) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", xy );
	}

	Double2 Double2Ctor (const Long3 &xyz) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", xyz );
	}

	Double2 Double2Ctor (const Long4 &xyzw) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", xyzw );
	}

	Double2 Double2Ctor (const Long &x, const Long &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", x, y );
	}

	Double2 Double2Ctor (const ULong &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", x );
	}

	Double2 Double2Ctor (const ULong2 &xy) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", xy );
	}

	Double2 Double2Ctor (const ULong3 &xyz) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", xyz );
	}

	Double2 Double2Ctor (const ULong4 &xyzw) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", xyzw );
	}

	Double2 Double2Ctor (const ULong &x, const ULong &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", x, y );
	}

	Double2 Double2Ctor (const Float &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", x );
	}

	Double2 Double2Ctor (const Float2 &xy) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", xy );
	}

	Double2 Double2Ctor (const Float3 &xyz) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", xyz );
	}

	Double2 Double2Ctor (const Float4 &xyzw) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", xyzw );
	}

	Double2 Double2Ctor (const Float &x, const Float &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", x, y );
	}

	Double2 Double2Ctor (const Double &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", x );
	}

	Double2 Double2Ctor (const Double3 &xyz) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", xyz );
	}

	Double2 Double2Ctor (const Double4 &xyzw) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", xyzw );
	}

	Double2 Double2Ctor (const Double &x, const Double &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "Double2Ctor", x, y );
	}

	Double3 Double3Ctor (const Bool &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", x );
	}

	Double3 Double3Ctor (const Bool2 &xy) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xy );
	}

	Double3 Double3Ctor (const Bool3 &xyz) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xyz );
	}

	Double3 Double3Ctor (const Bool4 &xyzw) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xyzw );
	}

	Double3 Double3Ctor (const Bool2 &xy, const Bool &z) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xy, z );
	}

	Double3 Double3Ctor (const Bool &x, const Bool2 &yz) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", x, yz );
	}

	Double3 Double3Ctor (const Bool &x, const Bool &y, const Bool &z) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", x, y, z );
	}

	Double3 Double3Ctor (const Int &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", x );
	}

	Double3 Double3Ctor (const Int2 &xy) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xy );
	}

	Double3 Double3Ctor (const Int3 &xyz) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xyz );
	}

	Double3 Double3Ctor (const Int4 &xyzw) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xyzw );
	}

	Double3 Double3Ctor (const Int2 &xy, const Int &z) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xy, z );
	}

	Double3 Double3Ctor (const Int &x, const Int2 &yz) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", x, yz );
	}

	Double3 Double3Ctor (const Int &x, const Int &y, const Int &z) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", x, y, z );
	}

	Double3 Double3Ctor (const UInt &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", x );
	}

	Double3 Double3Ctor (const UInt2 &xy) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xy );
	}

	Double3 Double3Ctor (const UInt3 &xyz) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xyz );
	}

	Double3 Double3Ctor (const UInt4 &xyzw) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xyzw );
	}

	Double3 Double3Ctor (const UInt2 &xy, const UInt &z) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xy, z );
	}

	Double3 Double3Ctor (const UInt &x, const UInt2 &yz) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", x, yz );
	}

	Double3 Double3Ctor (const UInt &x, const UInt &y, const UInt &z) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", x, y, z );
	}

	Double3 Double3Ctor (const Long &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", x );
	}

	Double3 Double3Ctor (const Long2 &xy) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xy );
	}

	Double3 Double3Ctor (const Long3 &xyz) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xyz );
	}

	Double3 Double3Ctor (const Long4 &xyzw) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xyzw );
	}

	Double3 Double3Ctor (const Long2 &xy, const Long &z) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xy, z );
	}

	Double3 Double3Ctor (const Long &x, const Long2 &yz) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", x, yz );
	}

	Double3 Double3Ctor (const Long &x, const Long &y, const Long &z) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", x, y, z );
	}

	Double3 Double3Ctor (const ULong &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", x );
	}

	Double3 Double3Ctor (const ULong2 &xy) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xy );
	}

	Double3 Double3Ctor (const ULong3 &xyz) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xyz );
	}

	Double3 Double3Ctor (const ULong4 &xyzw) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xyzw );
	}

	Double3 Double3Ctor (const ULong2 &xy, const ULong &z) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xy, z );
	}

	Double3 Double3Ctor (const ULong &x, const ULong2 &yz) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", x, yz );
	}

	Double3 Double3Ctor (const ULong &x, const ULong &y, const ULong &z) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", x, y, z );
	}

	Double3 Double3Ctor (const Float &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", x );
	}

	Double3 Double3Ctor (const Float2 &xy) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xy );
	}

	Double3 Double3Ctor (const Float3 &xyz) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xyz );
	}

	Double3 Double3Ctor (const Float4 &xyzw) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xyzw );
	}

	Double3 Double3Ctor (const Float2 &xy, const Float &z) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xy, z );
	}

	Double3 Double3Ctor (const Float &x, const Float2 &yz) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", x, yz );
	}

	Double3 Double3Ctor (const Float &x, const Float &y, const Float &z) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", x, y, z );
	}

	Double3 Double3Ctor (const Double &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", x );
	}

	Double3 Double3Ctor (const Double2 &xy) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xy );
	}

	Double3 Double3Ctor (const Double4 &xyzw) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xyzw );
	}

	Double3 Double3Ctor (const Double2 &xy, const Double &z) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", xy, z );
	}

	Double3 Double3Ctor (const Double &x, const Double2 &yz) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", x, yz );
	}

	Double3 Double3Ctor (const Double &x, const Double &y, const Double &z) {
		return NodeFunctions::Instance()->Build< Double3 >( "Double3Ctor", x, y, z );
	}

	Double4 Double4Ctor (const Bool &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x );
	}

	Double4 Double4Ctor (const Bool2 &xy) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xy );
	}

	Double4 Double4Ctor (const Bool3 &xyz) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xyz );
	}

	Double4 Double4Ctor (const Bool4 &xyzw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xyzw );
	}

	Double4 Double4Ctor (const Bool3 &xyz, const Bool &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xyz, w );
	}

	Double4 Double4Ctor (const Bool2 &xy, const Bool2 &zw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xy, zw );
	}

	Double4 Double4Ctor (const Bool &x, const Bool3 &yzw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, yzw );
	}

	Double4 Double4Ctor (const Bool2 &xy, const Bool &z, const Bool &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xy, z, w );
	}

	Double4 Double4Ctor (const Bool &x, const Bool2 &yz, const Bool &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, yz, w );
	}

	Double4 Double4Ctor (const Bool &x, const Bool &y, const Bool2 &zw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, y, zw );
	}

	Double4 Double4Ctor (const Bool &x, const Bool &y, const Bool &z, const Bool &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, y, z, w );
	}

	Double4 Double4Ctor (const Int &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x );
	}

	Double4 Double4Ctor (const Int2 &xy) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xy );
	}

	Double4 Double4Ctor (const Int3 &xyz) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xyz );
	}

	Double4 Double4Ctor (const Int4 &xyzw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xyzw );
	}

	Double4 Double4Ctor (const Int3 &xyz, const Int &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xyz, w );
	}

	Double4 Double4Ctor (const Int2 &xy, const Int2 &zw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xy, zw );
	}

	Double4 Double4Ctor (const Int &x, const Int3 &yzw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, yzw );
	}

	Double4 Double4Ctor (const Int2 &xy, const Int &z, const Int &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xy, z, w );
	}

	Double4 Double4Ctor (const Int &x, const Int2 &yz, const Int &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, yz, w );
	}

	Double4 Double4Ctor (const Int &x, const Int &y, const Int2 &zw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, y, zw );
	}

	Double4 Double4Ctor (const Int &x, const Int &y, const Int &z, const Int &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, y, z, w );
	}

	Double4 Double4Ctor (const UInt &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x );
	}

	Double4 Double4Ctor (const UInt2 &xy) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xy );
	}

	Double4 Double4Ctor (const UInt3 &xyz) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xyz );
	}

	Double4 Double4Ctor (const UInt4 &xyzw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xyzw );
	}

	Double4 Double4Ctor (const UInt3 &xyz, const UInt &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xyz, w );
	}

	Double4 Double4Ctor (const UInt2 &xy, const UInt2 &zw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xy, zw );
	}

	Double4 Double4Ctor (const UInt &x, const UInt3 &yzw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, yzw );
	}

	Double4 Double4Ctor (const UInt2 &xy, const UInt &z, const UInt &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xy, z, w );
	}

	Double4 Double4Ctor (const UInt &x, const UInt2 &yz, const UInt &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, yz, w );
	}

	Double4 Double4Ctor (const UInt &x, const UInt &y, const UInt2 &zw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, y, zw );
	}

	Double4 Double4Ctor (const UInt &x, const UInt &y, const UInt &z, const UInt &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, y, z, w );
	}

	Double4 Double4Ctor (const Long &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x );
	}

	Double4 Double4Ctor (const Long2 &xy) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xy );
	}

	Double4 Double4Ctor (const Long3 &xyz) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xyz );
	}

	Double4 Double4Ctor (const Long4 &xyzw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xyzw );
	}

	Double4 Double4Ctor (const Long3 &xyz, const Long &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xyz, w );
	}

	Double4 Double4Ctor (const Long2 &xy, const Long2 &zw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xy, zw );
	}

	Double4 Double4Ctor (const Long &x, const Long3 &yzw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, yzw );
	}

	Double4 Double4Ctor (const Long2 &xy, const Long &z, const Long &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xy, z, w );
	}

	Double4 Double4Ctor (const Long &x, const Long2 &yz, const Long &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, yz, w );
	}

	Double4 Double4Ctor (const Long &x, const Long &y, const Long2 &zw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, y, zw );
	}

	Double4 Double4Ctor (const Long &x, const Long &y, const Long &z, const Long &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, y, z, w );
	}

	Double4 Double4Ctor (const ULong &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x );
	}

	Double4 Double4Ctor (const ULong2 &xy) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xy );
	}

	Double4 Double4Ctor (const ULong3 &xyz) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xyz );
	}

	Double4 Double4Ctor (const ULong4 &xyzw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xyzw );
	}

	Double4 Double4Ctor (const ULong3 &xyz, const ULong &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xyz, w );
	}

	Double4 Double4Ctor (const ULong2 &xy, const ULong2 &zw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xy, zw );
	}

	Double4 Double4Ctor (const ULong &x, const ULong3 &yzw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, yzw );
	}

	Double4 Double4Ctor (const ULong2 &xy, const ULong &z, const ULong &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xy, z, w );
	}

	Double4 Double4Ctor (const ULong &x, const ULong2 &yz, const ULong &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, yz, w );
	}

	Double4 Double4Ctor (const ULong &x, const ULong &y, const ULong2 &zw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, y, zw );
	}

	Double4 Double4Ctor (const ULong &x, const ULong &y, const ULong &z, const ULong &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, y, z, w );
	}

	Double4 Double4Ctor (const Float &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x );
	}

	Double4 Double4Ctor (const Float2 &xy) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xy );
	}

	Double4 Double4Ctor (const Float3 &xyz) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xyz );
	}

	Double4 Double4Ctor (const Float4 &xyzw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xyzw );
	}

	Double4 Double4Ctor (const Float3 &xyz, const Float &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xyz, w );
	}

	Double4 Double4Ctor (const Float2 &xy, const Float2 &zw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xy, zw );
	}

	Double4 Double4Ctor (const Float &x, const Float3 &yzw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, yzw );
	}

	Double4 Double4Ctor (const Float2 &xy, const Float &z, const Float &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xy, z, w );
	}

	Double4 Double4Ctor (const Float &x, const Float2 &yz, const Float &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, yz, w );
	}

	Double4 Double4Ctor (const Float &x, const Float &y, const Float2 &zw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, y, zw );
	}

	Double4 Double4Ctor (const Float &x, const Float &y, const Float &z, const Float &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, y, z, w );
	}

	Double4 Double4Ctor (const Double &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x );
	}

	Double4 Double4Ctor (const Double2 &xy) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xy );
	}

	Double4 Double4Ctor (const Double3 &xyz) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xyz );
	}

	Double4 Double4Ctor (const Double3 &xyz, const Double &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xyz, w );
	}

	Double4 Double4Ctor (const Double2 &xy, const Double2 &zw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xy, zw );
	}

	Double4 Double4Ctor (const Double &x, const Double3 &yzw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, yzw );
	}

	Double4 Double4Ctor (const Double2 &xy, const Double &z, const Double &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", xy, z, w );
	}

	Double4 Double4Ctor (const Double &x, const Double2 &yz, const Double &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, yz, w );
	}

	Double4 Double4Ctor (const Double &x, const Double &y, const Double2 &zw) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, y, zw );
	}

	Double4 Double4Ctor (const Double &x, const Double &y, const Double &z, const Double &w) {
		return NodeFunctions::Instance()->Build< Double4 >( "Double4Ctor", x, y, z, w );
	}

	// Math operators
	Bool  operator ! (const Bool  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "Not", right );
	}

	Bool2 operator ! (const Bool2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Not", right );
	}

	Bool3 operator ! (const Bool3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Not", right );
	}

	Bool4 operator ! (const Bool4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Not", right );
	}

	Bool  operator && (const Bool  &left, const Bool  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "BoolAnd", left, right );
	}

	Bool2 operator && (const Bool2 &left, const Bool2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "BoolAnd", left, right );
	}

	Bool2 operator && (const Bool &left, const Bool2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "BoolAnd", left, right );
	}

	Bool2 operator && (const Bool2 &left, const Bool &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "BoolAnd", left, right );
	}

	Bool3 operator && (const Bool3 &left, const Bool3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "BoolAnd", left, right );
	}

	Bool3 operator && (const Bool &left, const Bool3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "BoolAnd", left, right );
	}

	Bool3 operator && (const Bool3 &left, const Bool &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "BoolAnd", left, right );
	}

	Bool4 operator && (const Bool4 &left, const Bool4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "BoolAnd", left, right );
	}

	Bool4 operator && (const Bool &left, const Bool4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "BoolAnd", left, right );
	}

	Bool4 operator && (const Bool4 &left, const Bool &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "BoolAnd", left, right );
	}

	Bool  operator || (const Bool  &left, const Bool  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "BoolOr", left, right );
	}

	Bool2 operator || (const Bool2 &left, const Bool2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "BoolOr", left, right );
	}

	Bool2 operator || (const Bool &left, const Bool2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "BoolOr", left, right );
	}

	Bool2 operator || (const Bool2 &left, const Bool &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "BoolOr", left, right );
	}

	Bool3 operator || (const Bool3 &left, const Bool3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "BoolOr", left, right );
	}

	Bool3 operator || (const Bool &left, const Bool3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "BoolOr", left, right );
	}

	Bool3 operator || (const Bool3 &left, const Bool &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "BoolOr", left, right );
	}

	Bool4 operator || (const Bool4 &left, const Bool4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "BoolOr", left, right );
	}

	Bool4 operator || (const Bool &left, const Bool4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "BoolOr", left, right );
	}

	Bool4 operator || (const Bool4 &left, const Bool &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "BoolOr", left, right );
	}

	Bool  operator == (const Bool  &left, const Bool  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "Equal", left, right );
	}

	Bool2 operator == (const Bool2 &left, const Bool2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Equal", left, right );
	}

	Bool2 operator == (const Bool &left, const Bool2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Equal", left, right );
	}

	Bool2 operator == (const Bool2 &left, const Bool &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Equal", left, right );
	}

	Bool3 operator == (const Bool3 &left, const Bool3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Equal", left, right );
	}

	Bool3 operator == (const Bool &left, const Bool3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Equal", left, right );
	}

	Bool3 operator == (const Bool3 &left, const Bool &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Equal", left, right );
	}

	Bool4 operator == (const Bool4 &left, const Bool4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Equal", left, right );
	}

	Bool4 operator == (const Bool &left, const Bool4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Equal", left, right );
	}

	Bool4 operator == (const Bool4 &left, const Bool &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Equal", left, right );
	}

	Bool  operator != (const Bool  &left, const Bool  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "NotEqual", left, right );
	}

	Bool2 operator != (const Bool2 &left, const Bool2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "NotEqual", left, right );
	}

	Bool2 operator != (const Bool &left, const Bool2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "NotEqual", left, right );
	}

	Bool2 operator != (const Bool2 &left, const Bool &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "NotEqual", left, right );
	}

	Bool3 operator != (const Bool3 &left, const Bool3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "NotEqual", left, right );
	}

	Bool3 operator != (const Bool &left, const Bool3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "NotEqual", left, right );
	}

	Bool3 operator != (const Bool3 &left, const Bool &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "NotEqual", left, right );
	}

	Bool4 operator != (const Bool4 &left, const Bool4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "NotEqual", left, right );
	}

	Bool4 operator != (const Bool &left, const Bool4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "NotEqual", left, right );
	}

	Bool4 operator != (const Bool4 &left, const Bool &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "NotEqual", left, right );
	}

	Int  operator ~ (const Int  &right) {
		return NodeFunctions::Instance()->Build< Int  >( "BitInverse", right );
	}

	Int2 operator ~ (const Int2 &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "BitInverse", right );
	}

	Int3 operator ~ (const Int3 &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "BitInverse", right );
	}

	Int4 operator ~ (const Int4 &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "BitInverse", right );
	}

	Int  operator + (const Int  &right) {
		return NodeFunctions::Instance()->Build< Int  >( "Plus", right );
	}

	Int2 operator + (const Int2 &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "Plus", right );
	}

	Int3 operator + (const Int3 &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "Plus", right );
	}

	Int4 operator + (const Int4 &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "Plus", right );
	}

	Int  operator - (const Int  &right) {
		return NodeFunctions::Instance()->Build< Int  >( "Minus", right );
	}

	Int2 operator - (const Int2 &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "Minus", right );
	}

	Int3 operator - (const Int3 &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "Minus", right );
	}

	Int4 operator - (const Int4 &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "Minus", right );
	}

	Int  operator + (const Int  &left, const Int  &right) {
		return NodeFunctions::Instance()->Build< Int  >( "Add", left, right );
	}

	Int2 operator + (const Int2 &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "Add", left, right );
	}

	Int2 operator + (const Int &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "Add", left, right );
	}

	Int2 operator + (const Int2 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "Add", left, right );
	}

	Int3 operator + (const Int3 &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "Add", left, right );
	}

	Int3 operator + (const Int &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "Add", left, right );
	}

	Int3 operator + (const Int3 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "Add", left, right );
	}

	Int4 operator + (const Int4 &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "Add", left, right );
	}

	Int4 operator + (const Int &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "Add", left, right );
	}

	Int4 operator + (const Int4 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "Add", left, right );
	}

	Int  operator - (const Int  &left, const Int  &right) {
		return NodeFunctions::Instance()->Build< Int  >( "Sub", left, right );
	}

	Int2 operator - (const Int2 &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "Sub", left, right );
	}

	Int2 operator - (const Int &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "Sub", left, right );
	}

	Int2 operator - (const Int2 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "Sub", left, right );
	}

	Int3 operator - (const Int3 &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "Sub", left, right );
	}

	Int3 operator - (const Int &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "Sub", left, right );
	}

	Int3 operator - (const Int3 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "Sub", left, right );
	}

	Int4 operator - (const Int4 &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "Sub", left, right );
	}

	Int4 operator - (const Int &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "Sub", left, right );
	}

	Int4 operator - (const Int4 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "Sub", left, right );
	}

	Int  operator * (const Int  &left, const Int  &right) {
		return NodeFunctions::Instance()->Build< Int  >( "Mul", left, right );
	}

	Int2 operator * (const Int2 &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "Mul", left, right );
	}

	Int2 operator * (const Int &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "Mul", left, right );
	}

	Int2 operator * (const Int2 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "Mul", left, right );
	}

	Int3 operator * (const Int3 &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "Mul", left, right );
	}

	Int3 operator * (const Int &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "Mul", left, right );
	}

	Int3 operator * (const Int3 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "Mul", left, right );
	}

	Int4 operator * (const Int4 &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "Mul", left, right );
	}

	Int4 operator * (const Int &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "Mul", left, right );
	}

	Int4 operator * (const Int4 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "Mul", left, right );
	}

	Int  operator / (const Int  &left, const Int  &right) {
		return NodeFunctions::Instance()->Build< Int  >( "Div", left, right );
	}

	Int2 operator / (const Int2 &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "Div", left, right );
	}

	Int2 operator / (const Int &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "Div", left, right );
	}

	Int2 operator / (const Int2 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "Div", left, right );
	}

	Int3 operator / (const Int3 &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "Div", left, right );
	}

	Int3 operator / (const Int &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "Div", left, right );
	}

	Int3 operator / (const Int3 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "Div", left, right );
	}

	Int4 operator / (const Int4 &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "Div", left, right );
	}

	Int4 operator / (const Int &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "Div", left, right );
	}

	Int4 operator / (const Int4 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "Div", left, right );
	}

	Int  operator % (const Int  &left, const Int  &right) {
		return NodeFunctions::Instance()->Build< Int  >( "Mod", left, right );
	}

	Int2 operator % (const Int2 &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "Mod", left, right );
	}

	Int2 operator % (const Int &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "Mod", left, right );
	}

	Int2 operator % (const Int2 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "Mod", left, right );
	}

	Int3 operator % (const Int3 &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "Mod", left, right );
	}

	Int3 operator % (const Int &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "Mod", left, right );
	}

	Int3 operator % (const Int3 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "Mod", left, right );
	}

	Int4 operator % (const Int4 &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "Mod", left, right );
	}

	Int4 operator % (const Int &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "Mod", left, right );
	}

	Int4 operator % (const Int4 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "Mod", left, right );
	}

	Int  operator & (const Int  &left, const Int  &right) {
		return NodeFunctions::Instance()->Build< Int  >( "And", left, right );
	}

	Int2 operator & (const Int2 &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "And", left, right );
	}

	Int2 operator & (const Int &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "And", left, right );
	}

	Int2 operator & (const Int2 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "And", left, right );
	}

	Int3 operator & (const Int3 &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "And", left, right );
	}

	Int3 operator & (const Int &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "And", left, right );
	}

	Int3 operator & (const Int3 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "And", left, right );
	}

	Int4 operator & (const Int4 &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "And", left, right );
	}

	Int4 operator & (const Int &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "And", left, right );
	}

	Int4 operator & (const Int4 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "And", left, right );
	}

	Int  operator | (const Int  &left, const Int  &right) {
		return NodeFunctions::Instance()->Build< Int  >( "Or", left, right );
	}

	Int2 operator | (const Int2 &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "Or", left, right );
	}

	Int2 operator | (const Int &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "Or", left, right );
	}

	Int2 operator | (const Int2 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "Or", left, right );
	}

	Int3 operator | (const Int3 &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "Or", left, right );
	}

	Int3 operator | (const Int &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "Or", left, right );
	}

	Int3 operator | (const Int3 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "Or", left, right );
	}

	Int4 operator | (const Int4 &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "Or", left, right );
	}

	Int4 operator | (const Int &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "Or", left, right );
	}

	Int4 operator | (const Int4 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "Or", left, right );
	}

	Int  operator ^ (const Int  &left, const Int  &right) {
		return NodeFunctions::Instance()->Build< Int  >( "Xor", left, right );
	}

	Int2 operator ^ (const Int2 &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "Xor", left, right );
	}

	Int2 operator ^ (const Int &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "Xor", left, right );
	}

	Int2 operator ^ (const Int2 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "Xor", left, right );
	}

	Int3 operator ^ (const Int3 &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "Xor", left, right );
	}

	Int3 operator ^ (const Int &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "Xor", left, right );
	}

	Int3 operator ^ (const Int3 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "Xor", left, right );
	}

	Int4 operator ^ (const Int4 &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "Xor", left, right );
	}

	Int4 operator ^ (const Int &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "Xor", left, right );
	}

	Int4 operator ^ (const Int4 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "Xor", left, right );
	}

	Int  operator << (const Int  &left, const Int  &right) {
		return NodeFunctions::Instance()->Build< Int  >( "BitLShift", left, right );
	}

	Int2 operator << (const Int2 &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "BitLShift", left, right );
	}

	Int2 operator << (const Int &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "BitLShift", left, right );
	}

	Int2 operator << (const Int2 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "BitLShift", left, right );
	}

	Int3 operator << (const Int3 &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "BitLShift", left, right );
	}

	Int3 operator << (const Int &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "BitLShift", left, right );
	}

	Int3 operator << (const Int3 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "BitLShift", left, right );
	}

	Int4 operator << (const Int4 &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "BitLShift", left, right );
	}

	Int4 operator << (const Int &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "BitLShift", left, right );
	}

	Int4 operator << (const Int4 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "BitLShift", left, right );
	}

	Int  operator >> (const Int  &left, const Int  &right) {
		return NodeFunctions::Instance()->Build< Int  >( "BitRShift", left, right );
	}

	Int2 operator >> (const Int2 &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "BitRShift", left, right );
	}

	Int2 operator >> (const Int &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "BitRShift", left, right );
	}

	Int2 operator >> (const Int2 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int2 >( "BitRShift", left, right );
	}

	Int3 operator >> (const Int3 &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "BitRShift", left, right );
	}

	Int3 operator >> (const Int &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "BitRShift", left, right );
	}

	Int3 operator >> (const Int3 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int3 >( "BitRShift", left, right );
	}

	Int4 operator >> (const Int4 &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "BitRShift", left, right );
	}

	Int4 operator >> (const Int &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "BitRShift", left, right );
	}

	Int4 operator >> (const Int4 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Int4 >( "BitRShift", left, right );
	}

	Bool  operator == (const Int  &left, const Int  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "Equal", left, right );
	}

	Bool2 operator == (const Int2 &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Equal", left, right );
	}

	Bool2 operator == (const Int &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Equal", left, right );
	}

	Bool2 operator == (const Int2 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Equal", left, right );
	}

	Bool3 operator == (const Int3 &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Equal", left, right );
	}

	Bool3 operator == (const Int &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Equal", left, right );
	}

	Bool3 operator == (const Int3 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Equal", left, right );
	}

	Bool4 operator == (const Int4 &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Equal", left, right );
	}

	Bool4 operator == (const Int &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Equal", left, right );
	}

	Bool4 operator == (const Int4 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Equal", left, right );
	}

	Bool  operator != (const Int  &left, const Int  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "NotEqual", left, right );
	}

	Bool2 operator != (const Int2 &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "NotEqual", left, right );
	}

	Bool2 operator != (const Int &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "NotEqual", left, right );
	}

	Bool2 operator != (const Int2 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "NotEqual", left, right );
	}

	Bool3 operator != (const Int3 &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "NotEqual", left, right );
	}

	Bool3 operator != (const Int &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "NotEqual", left, right );
	}

	Bool3 operator != (const Int3 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "NotEqual", left, right );
	}

	Bool4 operator != (const Int4 &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "NotEqual", left, right );
	}

	Bool4 operator != (const Int &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "NotEqual", left, right );
	}

	Bool4 operator != (const Int4 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "NotEqual", left, right );
	}

	Bool  operator > (const Int  &left, const Int  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "GreaterThan", left, right );
	}

	Bool2 operator > (const Int2 &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThan", left, right );
	}

	Bool2 operator > (const Int &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThan", left, right );
	}

	Bool2 operator > (const Int2 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThan", left, right );
	}

	Bool3 operator > (const Int3 &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThan", left, right );
	}

	Bool3 operator > (const Int &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThan", left, right );
	}

	Bool3 operator > (const Int3 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThan", left, right );
	}

	Bool4 operator > (const Int4 &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThan", left, right );
	}

	Bool4 operator > (const Int &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThan", left, right );
	}

	Bool4 operator > (const Int4 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThan", left, right );
	}

	Bool  operator < (const Int  &left, const Int  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "LessThan", left, right );
	}

	Bool2 operator < (const Int2 &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThan", left, right );
	}

	Bool2 operator < (const Int &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThan", left, right );
	}

	Bool2 operator < (const Int2 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThan", left, right );
	}

	Bool3 operator < (const Int3 &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThan", left, right );
	}

	Bool3 operator < (const Int &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThan", left, right );
	}

	Bool3 operator < (const Int3 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThan", left, right );
	}

	Bool4 operator < (const Int4 &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThan", left, right );
	}

	Bool4 operator < (const Int &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThan", left, right );
	}

	Bool4 operator < (const Int4 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThan", left, right );
	}

	Bool  operator >= (const Int  &left, const Int  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "GreaterThanEqual", left, right );
	}

	Bool2 operator >= (const Int2 &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThanEqual", left, right );
	}

	Bool2 operator >= (const Int &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThanEqual", left, right );
	}

	Bool2 operator >= (const Int2 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThanEqual", left, right );
	}

	Bool3 operator >= (const Int3 &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThanEqual", left, right );
	}

	Bool3 operator >= (const Int &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThanEqual", left, right );
	}

	Bool3 operator >= (const Int3 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThanEqual", left, right );
	}

	Bool4 operator >= (const Int4 &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThanEqual", left, right );
	}

	Bool4 operator >= (const Int &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThanEqual", left, right );
	}

	Bool4 operator >= (const Int4 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThanEqual", left, right );
	}

	Bool  operator <= (const Int  &left, const Int  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "LessThanEqual", left, right );
	}

	Bool2 operator <= (const Int2 &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThanEqual", left, right );
	}

	Bool2 operator <= (const Int &left, const Int2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThanEqual", left, right );
	}

	Bool2 operator <= (const Int2 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThanEqual", left, right );
	}

	Bool3 operator <= (const Int3 &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThanEqual", left, right );
	}

	Bool3 operator <= (const Int &left, const Int3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThanEqual", left, right );
	}

	Bool3 operator <= (const Int3 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThanEqual", left, right );
	}

	Bool4 operator <= (const Int4 &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThanEqual", left, right );
	}

	Bool4 operator <= (const Int &left, const Int4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThanEqual", left, right );
	}

	Bool4 operator <= (const Int4 &left, const Int &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThanEqual", left, right );
	}

	UInt  operator ~ (const UInt  &right) {
		return NodeFunctions::Instance()->Build< UInt  >( "BitInverse", right );
	}

	UInt2 operator ~ (const UInt2 &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "BitInverse", right );
	}

	UInt3 operator ~ (const UInt3 &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "BitInverse", right );
	}

	UInt4 operator ~ (const UInt4 &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "BitInverse", right );
	}

	UInt  operator + (const UInt  &right) {
		return NodeFunctions::Instance()->Build< UInt  >( "Plus", right );
	}

	UInt2 operator + (const UInt2 &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Plus", right );
	}

	UInt3 operator + (const UInt3 &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Plus", right );
	}

	UInt4 operator + (const UInt4 &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Plus", right );
	}

	UInt  operator - (const UInt  &right) {
		return NodeFunctions::Instance()->Build< UInt  >( "Minus", right );
	}

	UInt2 operator - (const UInt2 &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Minus", right );
	}

	UInt3 operator - (const UInt3 &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Minus", right );
	}

	UInt4 operator - (const UInt4 &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Minus", right );
	}

	UInt  operator + (const UInt  &left, const UInt  &right) {
		return NodeFunctions::Instance()->Build< UInt  >( "Add", left, right );
	}

	UInt2 operator + (const UInt2 &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Add", left, right );
	}

	UInt2 operator + (const UInt &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Add", left, right );
	}

	UInt2 operator + (const UInt2 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Add", left, right );
	}

	UInt3 operator + (const UInt3 &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Add", left, right );
	}

	UInt3 operator + (const UInt &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Add", left, right );
	}

	UInt3 operator + (const UInt3 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Add", left, right );
	}

	UInt4 operator + (const UInt4 &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Add", left, right );
	}

	UInt4 operator + (const UInt &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Add", left, right );
	}

	UInt4 operator + (const UInt4 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Add", left, right );
	}

	UInt  operator - (const UInt  &left, const UInt  &right) {
		return NodeFunctions::Instance()->Build< UInt  >( "Sub", left, right );
	}

	UInt2 operator - (const UInt2 &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Sub", left, right );
	}

	UInt2 operator - (const UInt &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Sub", left, right );
	}

	UInt2 operator - (const UInt2 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Sub", left, right );
	}

	UInt3 operator - (const UInt3 &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Sub", left, right );
	}

	UInt3 operator - (const UInt &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Sub", left, right );
	}

	UInt3 operator - (const UInt3 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Sub", left, right );
	}

	UInt4 operator - (const UInt4 &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Sub", left, right );
	}

	UInt4 operator - (const UInt &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Sub", left, right );
	}

	UInt4 operator - (const UInt4 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Sub", left, right );
	}

	UInt  operator * (const UInt  &left, const UInt  &right) {
		return NodeFunctions::Instance()->Build< UInt  >( "Mul", left, right );
	}

	UInt2 operator * (const UInt2 &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Mul", left, right );
	}

	UInt2 operator * (const UInt &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Mul", left, right );
	}

	UInt2 operator * (const UInt2 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Mul", left, right );
	}

	UInt3 operator * (const UInt3 &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Mul", left, right );
	}

	UInt3 operator * (const UInt &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Mul", left, right );
	}

	UInt3 operator * (const UInt3 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Mul", left, right );
	}

	UInt4 operator * (const UInt4 &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Mul", left, right );
	}

	UInt4 operator * (const UInt &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Mul", left, right );
	}

	UInt4 operator * (const UInt4 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Mul", left, right );
	}

	UInt  operator / (const UInt  &left, const UInt  &right) {
		return NodeFunctions::Instance()->Build< UInt  >( "Div", left, right );
	}

	UInt2 operator / (const UInt2 &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Div", left, right );
	}

	UInt2 operator / (const UInt &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Div", left, right );
	}

	UInt2 operator / (const UInt2 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Div", left, right );
	}

	UInt3 operator / (const UInt3 &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Div", left, right );
	}

	UInt3 operator / (const UInt &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Div", left, right );
	}

	UInt3 operator / (const UInt3 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Div", left, right );
	}

	UInt4 operator / (const UInt4 &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Div", left, right );
	}

	UInt4 operator / (const UInt &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Div", left, right );
	}

	UInt4 operator / (const UInt4 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Div", left, right );
	}

	UInt  operator % (const UInt  &left, const UInt  &right) {
		return NodeFunctions::Instance()->Build< UInt  >( "Mod", left, right );
	}

	UInt2 operator % (const UInt2 &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Mod", left, right );
	}

	UInt2 operator % (const UInt &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Mod", left, right );
	}

	UInt2 operator % (const UInt2 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Mod", left, right );
	}

	UInt3 operator % (const UInt3 &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Mod", left, right );
	}

	UInt3 operator % (const UInt &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Mod", left, right );
	}

	UInt3 operator % (const UInt3 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Mod", left, right );
	}

	UInt4 operator % (const UInt4 &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Mod", left, right );
	}

	UInt4 operator % (const UInt &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Mod", left, right );
	}

	UInt4 operator % (const UInt4 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Mod", left, right );
	}

	UInt  operator & (const UInt  &left, const UInt  &right) {
		return NodeFunctions::Instance()->Build< UInt  >( "And", left, right );
	}

	UInt2 operator & (const UInt2 &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "And", left, right );
	}

	UInt2 operator & (const UInt &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "And", left, right );
	}

	UInt2 operator & (const UInt2 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "And", left, right );
	}

	UInt3 operator & (const UInt3 &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "And", left, right );
	}

	UInt3 operator & (const UInt &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "And", left, right );
	}

	UInt3 operator & (const UInt3 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "And", left, right );
	}

	UInt4 operator & (const UInt4 &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "And", left, right );
	}

	UInt4 operator & (const UInt &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "And", left, right );
	}

	UInt4 operator & (const UInt4 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "And", left, right );
	}

	UInt  operator | (const UInt  &left, const UInt  &right) {
		return NodeFunctions::Instance()->Build< UInt  >( "Or", left, right );
	}

	UInt2 operator | (const UInt2 &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Or", left, right );
	}

	UInt2 operator | (const UInt &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Or", left, right );
	}

	UInt2 operator | (const UInt2 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Or", left, right );
	}

	UInt3 operator | (const UInt3 &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Or", left, right );
	}

	UInt3 operator | (const UInt &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Or", left, right );
	}

	UInt3 operator | (const UInt3 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Or", left, right );
	}

	UInt4 operator | (const UInt4 &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Or", left, right );
	}

	UInt4 operator | (const UInt &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Or", left, right );
	}

	UInt4 operator | (const UInt4 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Or", left, right );
	}

	UInt  operator ^ (const UInt  &left, const UInt  &right) {
		return NodeFunctions::Instance()->Build< UInt  >( "Xor", left, right );
	}

	UInt2 operator ^ (const UInt2 &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Xor", left, right );
	}

	UInt2 operator ^ (const UInt &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Xor", left, right );
	}

	UInt2 operator ^ (const UInt2 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Xor", left, right );
	}

	UInt3 operator ^ (const UInt3 &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Xor", left, right );
	}

	UInt3 operator ^ (const UInt &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Xor", left, right );
	}

	UInt3 operator ^ (const UInt3 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Xor", left, right );
	}

	UInt4 operator ^ (const UInt4 &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Xor", left, right );
	}

	UInt4 operator ^ (const UInt &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Xor", left, right );
	}

	UInt4 operator ^ (const UInt4 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Xor", left, right );
	}

	UInt  operator << (const UInt  &left, const UInt  &right) {
		return NodeFunctions::Instance()->Build< UInt  >( "BitLShift", left, right );
	}

	UInt2 operator << (const UInt2 &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "BitLShift", left, right );
	}

	UInt2 operator << (const UInt &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "BitLShift", left, right );
	}

	UInt2 operator << (const UInt2 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "BitLShift", left, right );
	}

	UInt3 operator << (const UInt3 &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "BitLShift", left, right );
	}

	UInt3 operator << (const UInt &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "BitLShift", left, right );
	}

	UInt3 operator << (const UInt3 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "BitLShift", left, right );
	}

	UInt4 operator << (const UInt4 &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "BitLShift", left, right );
	}

	UInt4 operator << (const UInt &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "BitLShift", left, right );
	}

	UInt4 operator << (const UInt4 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "BitLShift", left, right );
	}

	UInt  operator >> (const UInt  &left, const UInt  &right) {
		return NodeFunctions::Instance()->Build< UInt  >( "BitRShift", left, right );
	}

	UInt2 operator >> (const UInt2 &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "BitRShift", left, right );
	}

	UInt2 operator >> (const UInt &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "BitRShift", left, right );
	}

	UInt2 operator >> (const UInt2 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt2 >( "BitRShift", left, right );
	}

	UInt3 operator >> (const UInt3 &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "BitRShift", left, right );
	}

	UInt3 operator >> (const UInt &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "BitRShift", left, right );
	}

	UInt3 operator >> (const UInt3 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt3 >( "BitRShift", left, right );
	}

	UInt4 operator >> (const UInt4 &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "BitRShift", left, right );
	}

	UInt4 operator >> (const UInt &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "BitRShift", left, right );
	}

	UInt4 operator >> (const UInt4 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< UInt4 >( "BitRShift", left, right );
	}

	Bool  operator == (const UInt  &left, const UInt  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "Equal", left, right );
	}

	Bool2 operator == (const UInt2 &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Equal", left, right );
	}

	Bool2 operator == (const UInt &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Equal", left, right );
	}

	Bool2 operator == (const UInt2 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Equal", left, right );
	}

	Bool3 operator == (const UInt3 &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Equal", left, right );
	}

	Bool3 operator == (const UInt &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Equal", left, right );
	}

	Bool3 operator == (const UInt3 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Equal", left, right );
	}

	Bool4 operator == (const UInt4 &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Equal", left, right );
	}

	Bool4 operator == (const UInt &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Equal", left, right );
	}

	Bool4 operator == (const UInt4 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Equal", left, right );
	}

	Bool  operator != (const UInt  &left, const UInt  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "NotEqual", left, right );
	}

	Bool2 operator != (const UInt2 &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "NotEqual", left, right );
	}

	Bool2 operator != (const UInt &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "NotEqual", left, right );
	}

	Bool2 operator != (const UInt2 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "NotEqual", left, right );
	}

	Bool3 operator != (const UInt3 &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "NotEqual", left, right );
	}

	Bool3 operator != (const UInt &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "NotEqual", left, right );
	}

	Bool3 operator != (const UInt3 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "NotEqual", left, right );
	}

	Bool4 operator != (const UInt4 &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "NotEqual", left, right );
	}

	Bool4 operator != (const UInt &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "NotEqual", left, right );
	}

	Bool4 operator != (const UInt4 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "NotEqual", left, right );
	}

	Bool  operator > (const UInt  &left, const UInt  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "GreaterThan", left, right );
	}

	Bool2 operator > (const UInt2 &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThan", left, right );
	}

	Bool2 operator > (const UInt &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThan", left, right );
	}

	Bool2 operator > (const UInt2 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThan", left, right );
	}

	Bool3 operator > (const UInt3 &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThan", left, right );
	}

	Bool3 operator > (const UInt &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThan", left, right );
	}

	Bool3 operator > (const UInt3 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThan", left, right );
	}

	Bool4 operator > (const UInt4 &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThan", left, right );
	}

	Bool4 operator > (const UInt &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThan", left, right );
	}

	Bool4 operator > (const UInt4 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThan", left, right );
	}

	Bool  operator < (const UInt  &left, const UInt  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "LessThan", left, right );
	}

	Bool2 operator < (const UInt2 &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThan", left, right );
	}

	Bool2 operator < (const UInt &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThan", left, right );
	}

	Bool2 operator < (const UInt2 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThan", left, right );
	}

	Bool3 operator < (const UInt3 &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThan", left, right );
	}

	Bool3 operator < (const UInt &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThan", left, right );
	}

	Bool3 operator < (const UInt3 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThan", left, right );
	}

	Bool4 operator < (const UInt4 &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThan", left, right );
	}

	Bool4 operator < (const UInt &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThan", left, right );
	}

	Bool4 operator < (const UInt4 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThan", left, right );
	}

	Bool  operator >= (const UInt  &left, const UInt  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "GreaterThanEqual", left, right );
	}

	Bool2 operator >= (const UInt2 &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThanEqual", left, right );
	}

	Bool2 operator >= (const UInt &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThanEqual", left, right );
	}

	Bool2 operator >= (const UInt2 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThanEqual", left, right );
	}

	Bool3 operator >= (const UInt3 &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThanEqual", left, right );
	}

	Bool3 operator >= (const UInt &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThanEqual", left, right );
	}

	Bool3 operator >= (const UInt3 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThanEqual", left, right );
	}

	Bool4 operator >= (const UInt4 &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThanEqual", left, right );
	}

	Bool4 operator >= (const UInt &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThanEqual", left, right );
	}

	Bool4 operator >= (const UInt4 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThanEqual", left, right );
	}

	Bool  operator <= (const UInt  &left, const UInt  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "LessThanEqual", left, right );
	}

	Bool2 operator <= (const UInt2 &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThanEqual", left, right );
	}

	Bool2 operator <= (const UInt &left, const UInt2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThanEqual", left, right );
	}

	Bool2 operator <= (const UInt2 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThanEqual", left, right );
	}

	Bool3 operator <= (const UInt3 &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThanEqual", left, right );
	}

	Bool3 operator <= (const UInt &left, const UInt3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThanEqual", left, right );
	}

	Bool3 operator <= (const UInt3 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThanEqual", left, right );
	}

	Bool4 operator <= (const UInt4 &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThanEqual", left, right );
	}

	Bool4 operator <= (const UInt &left, const UInt4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThanEqual", left, right );
	}

	Bool4 operator <= (const UInt4 &left, const UInt &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThanEqual", left, right );
	}

	Long  operator ~ (const Long  &right) {
		return NodeFunctions::Instance()->Build< Long  >( "BitInverse", right );
	}

	Long2 operator ~ (const Long2 &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "BitInverse", right );
	}

	Long3 operator ~ (const Long3 &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "BitInverse", right );
	}

	Long4 operator ~ (const Long4 &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "BitInverse", right );
	}

	Long  operator + (const Long  &right) {
		return NodeFunctions::Instance()->Build< Long  >( "Plus", right );
	}

	Long2 operator + (const Long2 &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "Plus", right );
	}

	Long3 operator + (const Long3 &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "Plus", right );
	}

	Long4 operator + (const Long4 &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "Plus", right );
	}

	Long  operator - (const Long  &right) {
		return NodeFunctions::Instance()->Build< Long  >( "Minus", right );
	}

	Long2 operator - (const Long2 &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "Minus", right );
	}

	Long3 operator - (const Long3 &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "Minus", right );
	}

	Long4 operator - (const Long4 &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "Minus", right );
	}

	Long  operator + (const Long  &left, const Long  &right) {
		return NodeFunctions::Instance()->Build< Long  >( "Add", left, right );
	}

	Long2 operator + (const Long2 &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "Add", left, right );
	}

	Long2 operator + (const Long &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "Add", left, right );
	}

	Long2 operator + (const Long2 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "Add", left, right );
	}

	Long3 operator + (const Long3 &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "Add", left, right );
	}

	Long3 operator + (const Long &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "Add", left, right );
	}

	Long3 operator + (const Long3 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "Add", left, right );
	}

	Long4 operator + (const Long4 &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "Add", left, right );
	}

	Long4 operator + (const Long &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "Add", left, right );
	}

	Long4 operator + (const Long4 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "Add", left, right );
	}

	Long  operator - (const Long  &left, const Long  &right) {
		return NodeFunctions::Instance()->Build< Long  >( "Sub", left, right );
	}

	Long2 operator - (const Long2 &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "Sub", left, right );
	}

	Long2 operator - (const Long &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "Sub", left, right );
	}

	Long2 operator - (const Long2 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "Sub", left, right );
	}

	Long3 operator - (const Long3 &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "Sub", left, right );
	}

	Long3 operator - (const Long &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "Sub", left, right );
	}

	Long3 operator - (const Long3 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "Sub", left, right );
	}

	Long4 operator - (const Long4 &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "Sub", left, right );
	}

	Long4 operator - (const Long &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "Sub", left, right );
	}

	Long4 operator - (const Long4 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "Sub", left, right );
	}

	Long  operator * (const Long  &left, const Long  &right) {
		return NodeFunctions::Instance()->Build< Long  >( "Mul", left, right );
	}

	Long2 operator * (const Long2 &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "Mul", left, right );
	}

	Long2 operator * (const Long &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "Mul", left, right );
	}

	Long2 operator * (const Long2 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "Mul", left, right );
	}

	Long3 operator * (const Long3 &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "Mul", left, right );
	}

	Long3 operator * (const Long &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "Mul", left, right );
	}

	Long3 operator * (const Long3 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "Mul", left, right );
	}

	Long4 operator * (const Long4 &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "Mul", left, right );
	}

	Long4 operator * (const Long &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "Mul", left, right );
	}

	Long4 operator * (const Long4 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "Mul", left, right );
	}

	Long  operator / (const Long  &left, const Long  &right) {
		return NodeFunctions::Instance()->Build< Long  >( "Div", left, right );
	}

	Long2 operator / (const Long2 &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "Div", left, right );
	}

	Long2 operator / (const Long &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "Div", left, right );
	}

	Long2 operator / (const Long2 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "Div", left, right );
	}

	Long3 operator / (const Long3 &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "Div", left, right );
	}

	Long3 operator / (const Long &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "Div", left, right );
	}

	Long3 operator / (const Long3 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "Div", left, right );
	}

	Long4 operator / (const Long4 &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "Div", left, right );
	}

	Long4 operator / (const Long &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "Div", left, right );
	}

	Long4 operator / (const Long4 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "Div", left, right );
	}

	Long  operator % (const Long  &left, const Long  &right) {
		return NodeFunctions::Instance()->Build< Long  >( "Mod", left, right );
	}

	Long2 operator % (const Long2 &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "Mod", left, right );
	}

	Long2 operator % (const Long &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "Mod", left, right );
	}

	Long2 operator % (const Long2 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "Mod", left, right );
	}

	Long3 operator % (const Long3 &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "Mod", left, right );
	}

	Long3 operator % (const Long &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "Mod", left, right );
	}

	Long3 operator % (const Long3 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "Mod", left, right );
	}

	Long4 operator % (const Long4 &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "Mod", left, right );
	}

	Long4 operator % (const Long &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "Mod", left, right );
	}

	Long4 operator % (const Long4 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "Mod", left, right );
	}

	Long  operator & (const Long  &left, const Long  &right) {
		return NodeFunctions::Instance()->Build< Long  >( "And", left, right );
	}

	Long2 operator & (const Long2 &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "And", left, right );
	}

	Long2 operator & (const Long &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "And", left, right );
	}

	Long2 operator & (const Long2 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "And", left, right );
	}

	Long3 operator & (const Long3 &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "And", left, right );
	}

	Long3 operator & (const Long &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "And", left, right );
	}

	Long3 operator & (const Long3 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "And", left, right );
	}

	Long4 operator & (const Long4 &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "And", left, right );
	}

	Long4 operator & (const Long &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "And", left, right );
	}

	Long4 operator & (const Long4 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "And", left, right );
	}

	Long  operator | (const Long  &left, const Long  &right) {
		return NodeFunctions::Instance()->Build< Long  >( "Or", left, right );
	}

	Long2 operator | (const Long2 &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "Or", left, right );
	}

	Long2 operator | (const Long &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "Or", left, right );
	}

	Long2 operator | (const Long2 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "Or", left, right );
	}

	Long3 operator | (const Long3 &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "Or", left, right );
	}

	Long3 operator | (const Long &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "Or", left, right );
	}

	Long3 operator | (const Long3 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "Or", left, right );
	}

	Long4 operator | (const Long4 &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "Or", left, right );
	}

	Long4 operator | (const Long &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "Or", left, right );
	}

	Long4 operator | (const Long4 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "Or", left, right );
	}

	Long  operator ^ (const Long  &left, const Long  &right) {
		return NodeFunctions::Instance()->Build< Long  >( "Xor", left, right );
	}

	Long2 operator ^ (const Long2 &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "Xor", left, right );
	}

	Long2 operator ^ (const Long &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "Xor", left, right );
	}

	Long2 operator ^ (const Long2 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "Xor", left, right );
	}

	Long3 operator ^ (const Long3 &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "Xor", left, right );
	}

	Long3 operator ^ (const Long &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "Xor", left, right );
	}

	Long3 operator ^ (const Long3 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "Xor", left, right );
	}

	Long4 operator ^ (const Long4 &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "Xor", left, right );
	}

	Long4 operator ^ (const Long &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "Xor", left, right );
	}

	Long4 operator ^ (const Long4 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "Xor", left, right );
	}

	Long  operator << (const Long  &left, const Long  &right) {
		return NodeFunctions::Instance()->Build< Long  >( "BitLShift", left, right );
	}

	Long2 operator << (const Long2 &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "BitLShift", left, right );
	}

	Long2 operator << (const Long &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "BitLShift", left, right );
	}

	Long2 operator << (const Long2 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "BitLShift", left, right );
	}

	Long3 operator << (const Long3 &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "BitLShift", left, right );
	}

	Long3 operator << (const Long &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "BitLShift", left, right );
	}

	Long3 operator << (const Long3 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "BitLShift", left, right );
	}

	Long4 operator << (const Long4 &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "BitLShift", left, right );
	}

	Long4 operator << (const Long &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "BitLShift", left, right );
	}

	Long4 operator << (const Long4 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "BitLShift", left, right );
	}

	Long  operator >> (const Long  &left, const Long  &right) {
		return NodeFunctions::Instance()->Build< Long  >( "BitRShift", left, right );
	}

	Long2 operator >> (const Long2 &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "BitRShift", left, right );
	}

	Long2 operator >> (const Long &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "BitRShift", left, right );
	}

	Long2 operator >> (const Long2 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long2 >( "BitRShift", left, right );
	}

	Long3 operator >> (const Long3 &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "BitRShift", left, right );
	}

	Long3 operator >> (const Long &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "BitRShift", left, right );
	}

	Long3 operator >> (const Long3 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long3 >( "BitRShift", left, right );
	}

	Long4 operator >> (const Long4 &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "BitRShift", left, right );
	}

	Long4 operator >> (const Long &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "BitRShift", left, right );
	}

	Long4 operator >> (const Long4 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Long4 >( "BitRShift", left, right );
	}

	Bool  operator == (const Long  &left, const Long  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "Equal", left, right );
	}

	Bool2 operator == (const Long2 &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Equal", left, right );
	}

	Bool2 operator == (const Long &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Equal", left, right );
	}

	Bool2 operator == (const Long2 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Equal", left, right );
	}

	Bool3 operator == (const Long3 &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Equal", left, right );
	}

	Bool3 operator == (const Long &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Equal", left, right );
	}

	Bool3 operator == (const Long3 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Equal", left, right );
	}

	Bool4 operator == (const Long4 &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Equal", left, right );
	}

	Bool4 operator == (const Long &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Equal", left, right );
	}

	Bool4 operator == (const Long4 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Equal", left, right );
	}

	Bool  operator != (const Long  &left, const Long  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "NotEqual", left, right );
	}

	Bool2 operator != (const Long2 &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "NotEqual", left, right );
	}

	Bool2 operator != (const Long &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "NotEqual", left, right );
	}

	Bool2 operator != (const Long2 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "NotEqual", left, right );
	}

	Bool3 operator != (const Long3 &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "NotEqual", left, right );
	}

	Bool3 operator != (const Long &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "NotEqual", left, right );
	}

	Bool3 operator != (const Long3 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "NotEqual", left, right );
	}

	Bool4 operator != (const Long4 &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "NotEqual", left, right );
	}

	Bool4 operator != (const Long &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "NotEqual", left, right );
	}

	Bool4 operator != (const Long4 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "NotEqual", left, right );
	}

	Bool  operator > (const Long  &left, const Long  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "GreaterThan", left, right );
	}

	Bool2 operator > (const Long2 &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThan", left, right );
	}

	Bool2 operator > (const Long &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThan", left, right );
	}

	Bool2 operator > (const Long2 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThan", left, right );
	}

	Bool3 operator > (const Long3 &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThan", left, right );
	}

	Bool3 operator > (const Long &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThan", left, right );
	}

	Bool3 operator > (const Long3 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThan", left, right );
	}

	Bool4 operator > (const Long4 &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThan", left, right );
	}

	Bool4 operator > (const Long &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThan", left, right );
	}

	Bool4 operator > (const Long4 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThan", left, right );
	}

	Bool  operator < (const Long  &left, const Long  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "LessThan", left, right );
	}

	Bool2 operator < (const Long2 &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThan", left, right );
	}

	Bool2 operator < (const Long &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThan", left, right );
	}

	Bool2 operator < (const Long2 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThan", left, right );
	}

	Bool3 operator < (const Long3 &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThan", left, right );
	}

	Bool3 operator < (const Long &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThan", left, right );
	}

	Bool3 operator < (const Long3 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThan", left, right );
	}

	Bool4 operator < (const Long4 &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThan", left, right );
	}

	Bool4 operator < (const Long &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThan", left, right );
	}

	Bool4 operator < (const Long4 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThan", left, right );
	}

	Bool  operator >= (const Long  &left, const Long  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "GreaterThanEqual", left, right );
	}

	Bool2 operator >= (const Long2 &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThanEqual", left, right );
	}

	Bool2 operator >= (const Long &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThanEqual", left, right );
	}

	Bool2 operator >= (const Long2 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThanEqual", left, right );
	}

	Bool3 operator >= (const Long3 &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThanEqual", left, right );
	}

	Bool3 operator >= (const Long &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThanEqual", left, right );
	}

	Bool3 operator >= (const Long3 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThanEqual", left, right );
	}

	Bool4 operator >= (const Long4 &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThanEqual", left, right );
	}

	Bool4 operator >= (const Long &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThanEqual", left, right );
	}

	Bool4 operator >= (const Long4 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThanEqual", left, right );
	}

	Bool  operator <= (const Long  &left, const Long  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "LessThanEqual", left, right );
	}

	Bool2 operator <= (const Long2 &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThanEqual", left, right );
	}

	Bool2 operator <= (const Long &left, const Long2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThanEqual", left, right );
	}

	Bool2 operator <= (const Long2 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThanEqual", left, right );
	}

	Bool3 operator <= (const Long3 &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThanEqual", left, right );
	}

	Bool3 operator <= (const Long &left, const Long3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThanEqual", left, right );
	}

	Bool3 operator <= (const Long3 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThanEqual", left, right );
	}

	Bool4 operator <= (const Long4 &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThanEqual", left, right );
	}

	Bool4 operator <= (const Long &left, const Long4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThanEqual", left, right );
	}

	Bool4 operator <= (const Long4 &left, const Long &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThanEqual", left, right );
	}

	ULong  operator ~ (const ULong  &right) {
		return NodeFunctions::Instance()->Build< ULong  >( "BitInverse", right );
	}

	ULong2 operator ~ (const ULong2 &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "BitInverse", right );
	}

	ULong3 operator ~ (const ULong3 &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "BitInverse", right );
	}

	ULong4 operator ~ (const ULong4 &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "BitInverse", right );
	}

	ULong  operator + (const ULong  &right) {
		return NodeFunctions::Instance()->Build< ULong  >( "Plus", right );
	}

	ULong2 operator + (const ULong2 &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Plus", right );
	}

	ULong3 operator + (const ULong3 &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Plus", right );
	}

	ULong4 operator + (const ULong4 &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Plus", right );
	}

	ULong  operator - (const ULong  &right) {
		return NodeFunctions::Instance()->Build< ULong  >( "Minus", right );
	}

	ULong2 operator - (const ULong2 &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Minus", right );
	}

	ULong3 operator - (const ULong3 &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Minus", right );
	}

	ULong4 operator - (const ULong4 &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Minus", right );
	}

	ULong  operator + (const ULong  &left, const ULong  &right) {
		return NodeFunctions::Instance()->Build< ULong  >( "Add", left, right );
	}

	ULong2 operator + (const ULong2 &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Add", left, right );
	}

	ULong2 operator + (const ULong &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Add", left, right );
	}

	ULong2 operator + (const ULong2 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Add", left, right );
	}

	ULong3 operator + (const ULong3 &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Add", left, right );
	}

	ULong3 operator + (const ULong &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Add", left, right );
	}

	ULong3 operator + (const ULong3 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Add", left, right );
	}

	ULong4 operator + (const ULong4 &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Add", left, right );
	}

	ULong4 operator + (const ULong &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Add", left, right );
	}

	ULong4 operator + (const ULong4 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Add", left, right );
	}

	ULong  operator - (const ULong  &left, const ULong  &right) {
		return NodeFunctions::Instance()->Build< ULong  >( "Sub", left, right );
	}

	ULong2 operator - (const ULong2 &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Sub", left, right );
	}

	ULong2 operator - (const ULong &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Sub", left, right );
	}

	ULong2 operator - (const ULong2 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Sub", left, right );
	}

	ULong3 operator - (const ULong3 &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Sub", left, right );
	}

	ULong3 operator - (const ULong &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Sub", left, right );
	}

	ULong3 operator - (const ULong3 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Sub", left, right );
	}

	ULong4 operator - (const ULong4 &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Sub", left, right );
	}

	ULong4 operator - (const ULong &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Sub", left, right );
	}

	ULong4 operator - (const ULong4 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Sub", left, right );
	}

	ULong  operator * (const ULong  &left, const ULong  &right) {
		return NodeFunctions::Instance()->Build< ULong  >( "Mul", left, right );
	}

	ULong2 operator * (const ULong2 &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Mul", left, right );
	}

	ULong2 operator * (const ULong &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Mul", left, right );
	}

	ULong2 operator * (const ULong2 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Mul", left, right );
	}

	ULong3 operator * (const ULong3 &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Mul", left, right );
	}

	ULong3 operator * (const ULong &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Mul", left, right );
	}

	ULong3 operator * (const ULong3 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Mul", left, right );
	}

	ULong4 operator * (const ULong4 &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Mul", left, right );
	}

	ULong4 operator * (const ULong &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Mul", left, right );
	}

	ULong4 operator * (const ULong4 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Mul", left, right );
	}

	ULong  operator / (const ULong  &left, const ULong  &right) {
		return NodeFunctions::Instance()->Build< ULong  >( "Div", left, right );
	}

	ULong2 operator / (const ULong2 &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Div", left, right );
	}

	ULong2 operator / (const ULong &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Div", left, right );
	}

	ULong2 operator / (const ULong2 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Div", left, right );
	}

	ULong3 operator / (const ULong3 &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Div", left, right );
	}

	ULong3 operator / (const ULong &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Div", left, right );
	}

	ULong3 operator / (const ULong3 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Div", left, right );
	}

	ULong4 operator / (const ULong4 &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Div", left, right );
	}

	ULong4 operator / (const ULong &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Div", left, right );
	}

	ULong4 operator / (const ULong4 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Div", left, right );
	}

	ULong  operator % (const ULong  &left, const ULong  &right) {
		return NodeFunctions::Instance()->Build< ULong  >( "Mod", left, right );
	}

	ULong2 operator % (const ULong2 &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Mod", left, right );
	}

	ULong2 operator % (const ULong &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Mod", left, right );
	}

	ULong2 operator % (const ULong2 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Mod", left, right );
	}

	ULong3 operator % (const ULong3 &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Mod", left, right );
	}

	ULong3 operator % (const ULong &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Mod", left, right );
	}

	ULong3 operator % (const ULong3 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Mod", left, right );
	}

	ULong4 operator % (const ULong4 &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Mod", left, right );
	}

	ULong4 operator % (const ULong &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Mod", left, right );
	}

	ULong4 operator % (const ULong4 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Mod", left, right );
	}

	ULong  operator & (const ULong  &left, const ULong  &right) {
		return NodeFunctions::Instance()->Build< ULong  >( "And", left, right );
	}

	ULong2 operator & (const ULong2 &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "And", left, right );
	}

	ULong2 operator & (const ULong &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "And", left, right );
	}

	ULong2 operator & (const ULong2 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "And", left, right );
	}

	ULong3 operator & (const ULong3 &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "And", left, right );
	}

	ULong3 operator & (const ULong &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "And", left, right );
	}

	ULong3 operator & (const ULong3 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "And", left, right );
	}

	ULong4 operator & (const ULong4 &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "And", left, right );
	}

	ULong4 operator & (const ULong &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "And", left, right );
	}

	ULong4 operator & (const ULong4 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "And", left, right );
	}

	ULong  operator | (const ULong  &left, const ULong  &right) {
		return NodeFunctions::Instance()->Build< ULong  >( "Or", left, right );
	}

	ULong2 operator | (const ULong2 &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Or", left, right );
	}

	ULong2 operator | (const ULong &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Or", left, right );
	}

	ULong2 operator | (const ULong2 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Or", left, right );
	}

	ULong3 operator | (const ULong3 &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Or", left, right );
	}

	ULong3 operator | (const ULong &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Or", left, right );
	}

	ULong3 operator | (const ULong3 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Or", left, right );
	}

	ULong4 operator | (const ULong4 &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Or", left, right );
	}

	ULong4 operator | (const ULong &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Or", left, right );
	}

	ULong4 operator | (const ULong4 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Or", left, right );
	}

	ULong  operator ^ (const ULong  &left, const ULong  &right) {
		return NodeFunctions::Instance()->Build< ULong  >( "Xor", left, right );
	}

	ULong2 operator ^ (const ULong2 &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Xor", left, right );
	}

	ULong2 operator ^ (const ULong &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Xor", left, right );
	}

	ULong2 operator ^ (const ULong2 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Xor", left, right );
	}

	ULong3 operator ^ (const ULong3 &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Xor", left, right );
	}

	ULong3 operator ^ (const ULong &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Xor", left, right );
	}

	ULong3 operator ^ (const ULong3 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Xor", left, right );
	}

	ULong4 operator ^ (const ULong4 &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Xor", left, right );
	}

	ULong4 operator ^ (const ULong &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Xor", left, right );
	}

	ULong4 operator ^ (const ULong4 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Xor", left, right );
	}

	ULong  operator << (const ULong  &left, const ULong  &right) {
		return NodeFunctions::Instance()->Build< ULong  >( "BitLShift", left, right );
	}

	ULong2 operator << (const ULong2 &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "BitLShift", left, right );
	}

	ULong2 operator << (const ULong &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "BitLShift", left, right );
	}

	ULong2 operator << (const ULong2 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "BitLShift", left, right );
	}

	ULong3 operator << (const ULong3 &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "BitLShift", left, right );
	}

	ULong3 operator << (const ULong &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "BitLShift", left, right );
	}

	ULong3 operator << (const ULong3 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "BitLShift", left, right );
	}

	ULong4 operator << (const ULong4 &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "BitLShift", left, right );
	}

	ULong4 operator << (const ULong &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "BitLShift", left, right );
	}

	ULong4 operator << (const ULong4 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "BitLShift", left, right );
	}

	ULong  operator >> (const ULong  &left, const ULong  &right) {
		return NodeFunctions::Instance()->Build< ULong  >( "BitRShift", left, right );
	}

	ULong2 operator >> (const ULong2 &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "BitRShift", left, right );
	}

	ULong2 operator >> (const ULong &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "BitRShift", left, right );
	}

	ULong2 operator >> (const ULong2 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong2 >( "BitRShift", left, right );
	}

	ULong3 operator >> (const ULong3 &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "BitRShift", left, right );
	}

	ULong3 operator >> (const ULong &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "BitRShift", left, right );
	}

	ULong3 operator >> (const ULong3 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong3 >( "BitRShift", left, right );
	}

	ULong4 operator >> (const ULong4 &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "BitRShift", left, right );
	}

	ULong4 operator >> (const ULong &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "BitRShift", left, right );
	}

	ULong4 operator >> (const ULong4 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< ULong4 >( "BitRShift", left, right );
	}

	Bool  operator == (const ULong  &left, const ULong  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "Equal", left, right );
	}

	Bool2 operator == (const ULong2 &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Equal", left, right );
	}

	Bool2 operator == (const ULong &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Equal", left, right );
	}

	Bool2 operator == (const ULong2 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Equal", left, right );
	}

	Bool3 operator == (const ULong3 &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Equal", left, right );
	}

	Bool3 operator == (const ULong &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Equal", left, right );
	}

	Bool3 operator == (const ULong3 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Equal", left, right );
	}

	Bool4 operator == (const ULong4 &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Equal", left, right );
	}

	Bool4 operator == (const ULong &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Equal", left, right );
	}

	Bool4 operator == (const ULong4 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Equal", left, right );
	}

	Bool  operator != (const ULong  &left, const ULong  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "NotEqual", left, right );
	}

	Bool2 operator != (const ULong2 &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "NotEqual", left, right );
	}

	Bool2 operator != (const ULong &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "NotEqual", left, right );
	}

	Bool2 operator != (const ULong2 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "NotEqual", left, right );
	}

	Bool3 operator != (const ULong3 &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "NotEqual", left, right );
	}

	Bool3 operator != (const ULong &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "NotEqual", left, right );
	}

	Bool3 operator != (const ULong3 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "NotEqual", left, right );
	}

	Bool4 operator != (const ULong4 &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "NotEqual", left, right );
	}

	Bool4 operator != (const ULong &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "NotEqual", left, right );
	}

	Bool4 operator != (const ULong4 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "NotEqual", left, right );
	}

	Bool  operator > (const ULong  &left, const ULong  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "GreaterThan", left, right );
	}

	Bool2 operator > (const ULong2 &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThan", left, right );
	}

	Bool2 operator > (const ULong &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThan", left, right );
	}

	Bool2 operator > (const ULong2 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThan", left, right );
	}

	Bool3 operator > (const ULong3 &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThan", left, right );
	}

	Bool3 operator > (const ULong &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThan", left, right );
	}

	Bool3 operator > (const ULong3 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThan", left, right );
	}

	Bool4 operator > (const ULong4 &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThan", left, right );
	}

	Bool4 operator > (const ULong &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThan", left, right );
	}

	Bool4 operator > (const ULong4 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThan", left, right );
	}

	Bool  operator < (const ULong  &left, const ULong  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "LessThan", left, right );
	}

	Bool2 operator < (const ULong2 &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThan", left, right );
	}

	Bool2 operator < (const ULong &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThan", left, right );
	}

	Bool2 operator < (const ULong2 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThan", left, right );
	}

	Bool3 operator < (const ULong3 &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThan", left, right );
	}

	Bool3 operator < (const ULong &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThan", left, right );
	}

	Bool3 operator < (const ULong3 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThan", left, right );
	}

	Bool4 operator < (const ULong4 &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThan", left, right );
	}

	Bool4 operator < (const ULong &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThan", left, right );
	}

	Bool4 operator < (const ULong4 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThan", left, right );
	}

	Bool  operator >= (const ULong  &left, const ULong  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "GreaterThanEqual", left, right );
	}

	Bool2 operator >= (const ULong2 &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThanEqual", left, right );
	}

	Bool2 operator >= (const ULong &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThanEqual", left, right );
	}

	Bool2 operator >= (const ULong2 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThanEqual", left, right );
	}

	Bool3 operator >= (const ULong3 &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThanEqual", left, right );
	}

	Bool3 operator >= (const ULong &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThanEqual", left, right );
	}

	Bool3 operator >= (const ULong3 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThanEqual", left, right );
	}

	Bool4 operator >= (const ULong4 &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThanEqual", left, right );
	}

	Bool4 operator >= (const ULong &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThanEqual", left, right );
	}

	Bool4 operator >= (const ULong4 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThanEqual", left, right );
	}

	Bool  operator <= (const ULong  &left, const ULong  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "LessThanEqual", left, right );
	}

	Bool2 operator <= (const ULong2 &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThanEqual", left, right );
	}

	Bool2 operator <= (const ULong &left, const ULong2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThanEqual", left, right );
	}

	Bool2 operator <= (const ULong2 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThanEqual", left, right );
	}

	Bool3 operator <= (const ULong3 &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThanEqual", left, right );
	}

	Bool3 operator <= (const ULong &left, const ULong3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThanEqual", left, right );
	}

	Bool3 operator <= (const ULong3 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThanEqual", left, right );
	}

	Bool4 operator <= (const ULong4 &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThanEqual", left, right );
	}

	Bool4 operator <= (const ULong &left, const ULong4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThanEqual", left, right );
	}

	Bool4 operator <= (const ULong4 &left, const ULong &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThanEqual", left, right );
	}

	Float  operator + (const Float  &right) {
		return NodeFunctions::Instance()->Build< Float  >( "Plus", right );
	}

	Float2 operator + (const Float2 &right) {
		return NodeFunctions::Instance()->Build< Float2 >( "Plus", right );
	}

	Float3 operator + (const Float3 &right) {
		return NodeFunctions::Instance()->Build< Float3 >( "Plus", right );
	}

	Float4 operator + (const Float4 &right) {
		return NodeFunctions::Instance()->Build< Float4 >( "Plus", right );
	}

	Float  operator - (const Float  &right) {
		return NodeFunctions::Instance()->Build< Float  >( "Minus", right );
	}

	Float2 operator - (const Float2 &right) {
		return NodeFunctions::Instance()->Build< Float2 >( "Minus", right );
	}

	Float3 operator - (const Float3 &right) {
		return NodeFunctions::Instance()->Build< Float3 >( "Minus", right );
	}

	Float4 operator - (const Float4 &right) {
		return NodeFunctions::Instance()->Build< Float4 >( "Minus", right );
	}

	Float  operator + (const Float  &left, const Float  &right) {
		return NodeFunctions::Instance()->Build< Float  >( "Add", left, right );
	}

	Float2 operator + (const Float2 &left, const Float2 &right) {
		return NodeFunctions::Instance()->Build< Float2 >( "Add", left, right );
	}

	Float2 operator + (const Float &left, const Float2 &right) {
		return NodeFunctions::Instance()->Build< Float2 >( "Add", left, right );
	}

	Float2 operator + (const Float2 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Float2 >( "Add", left, right );
	}

	Float3 operator + (const Float3 &left, const Float3 &right) {
		return NodeFunctions::Instance()->Build< Float3 >( "Add", left, right );
	}

	Float3 operator + (const Float &left, const Float3 &right) {
		return NodeFunctions::Instance()->Build< Float3 >( "Add", left, right );
	}

	Float3 operator + (const Float3 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Float3 >( "Add", left, right );
	}

	Float4 operator + (const Float4 &left, const Float4 &right) {
		return NodeFunctions::Instance()->Build< Float4 >( "Add", left, right );
	}

	Float4 operator + (const Float &left, const Float4 &right) {
		return NodeFunctions::Instance()->Build< Float4 >( "Add", left, right );
	}

	Float4 operator + (const Float4 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Float4 >( "Add", left, right );
	}

	Float  operator - (const Float  &left, const Float  &right) {
		return NodeFunctions::Instance()->Build< Float  >( "Sub", left, right );
	}

	Float2 operator - (const Float2 &left, const Float2 &right) {
		return NodeFunctions::Instance()->Build< Float2 >( "Sub", left, right );
	}

	Float2 operator - (const Float &left, const Float2 &right) {
		return NodeFunctions::Instance()->Build< Float2 >( "Sub", left, right );
	}

	Float2 operator - (const Float2 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Float2 >( "Sub", left, right );
	}

	Float3 operator - (const Float3 &left, const Float3 &right) {
		return NodeFunctions::Instance()->Build< Float3 >( "Sub", left, right );
	}

	Float3 operator - (const Float &left, const Float3 &right) {
		return NodeFunctions::Instance()->Build< Float3 >( "Sub", left, right );
	}

	Float3 operator - (const Float3 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Float3 >( "Sub", left, right );
	}

	Float4 operator - (const Float4 &left, const Float4 &right) {
		return NodeFunctions::Instance()->Build< Float4 >( "Sub", left, right );
	}

	Float4 operator - (const Float &left, const Float4 &right) {
		return NodeFunctions::Instance()->Build< Float4 >( "Sub", left, right );
	}

	Float4 operator - (const Float4 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Float4 >( "Sub", left, right );
	}

	Float  operator * (const Float  &left, const Float  &right) {
		return NodeFunctions::Instance()->Build< Float  >( "Mul", left, right );
	}

	Float2 operator * (const Float2 &left, const Float2 &right) {
		return NodeFunctions::Instance()->Build< Float2 >( "Mul", left, right );
	}

	Float2 operator * (const Float &left, const Float2 &right) {
		return NodeFunctions::Instance()->Build< Float2 >( "Mul", left, right );
	}

	Float2 operator * (const Float2 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Float2 >( "Mul", left, right );
	}

	Float3 operator * (const Float3 &left, const Float3 &right) {
		return NodeFunctions::Instance()->Build< Float3 >( "Mul", left, right );
	}

	Float3 operator * (const Float &left, const Float3 &right) {
		return NodeFunctions::Instance()->Build< Float3 >( "Mul", left, right );
	}

	Float3 operator * (const Float3 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Float3 >( "Mul", left, right );
	}

	Float4 operator * (const Float4 &left, const Float4 &right) {
		return NodeFunctions::Instance()->Build< Float4 >( "Mul", left, right );
	}

	Float4 operator * (const Float &left, const Float4 &right) {
		return NodeFunctions::Instance()->Build< Float4 >( "Mul", left, right );
	}

	Float4 operator * (const Float4 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Float4 >( "Mul", left, right );
	}

	Float  operator / (const Float  &left, const Float  &right) {
		return NodeFunctions::Instance()->Build< Float  >( "Div", left, right );
	}

	Float2 operator / (const Float2 &left, const Float2 &right) {
		return NodeFunctions::Instance()->Build< Float2 >( "Div", left, right );
	}

	Float2 operator / (const Float &left, const Float2 &right) {
		return NodeFunctions::Instance()->Build< Float2 >( "Div", left, right );
	}

	Float2 operator / (const Float2 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Float2 >( "Div", left, right );
	}

	Float3 operator / (const Float3 &left, const Float3 &right) {
		return NodeFunctions::Instance()->Build< Float3 >( "Div", left, right );
	}

	Float3 operator / (const Float &left, const Float3 &right) {
		return NodeFunctions::Instance()->Build< Float3 >( "Div", left, right );
	}

	Float3 operator / (const Float3 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Float3 >( "Div", left, right );
	}

	Float4 operator / (const Float4 &left, const Float4 &right) {
		return NodeFunctions::Instance()->Build< Float4 >( "Div", left, right );
	}

	Float4 operator / (const Float &left, const Float4 &right) {
		return NodeFunctions::Instance()->Build< Float4 >( "Div", left, right );
	}

	Float4 operator / (const Float4 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Float4 >( "Div", left, right );
	}

	Float  operator % (const Float  &left, const Float  &right) {
		return NodeFunctions::Instance()->Build< Float  >( "Mod", left, right );
	}

	Float2 operator % (const Float2 &left, const Float2 &right) {
		return NodeFunctions::Instance()->Build< Float2 >( "Mod", left, right );
	}

	Float2 operator % (const Float &left, const Float2 &right) {
		return NodeFunctions::Instance()->Build< Float2 >( "Mod", left, right );
	}

	Float2 operator % (const Float2 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Float2 >( "Mod", left, right );
	}

	Float3 operator % (const Float3 &left, const Float3 &right) {
		return NodeFunctions::Instance()->Build< Float3 >( "Mod", left, right );
	}

	Float3 operator % (const Float &left, const Float3 &right) {
		return NodeFunctions::Instance()->Build< Float3 >( "Mod", left, right );
	}

	Float3 operator % (const Float3 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Float3 >( "Mod", left, right );
	}

	Float4 operator % (const Float4 &left, const Float4 &right) {
		return NodeFunctions::Instance()->Build< Float4 >( "Mod", left, right );
	}

	Float4 operator % (const Float &left, const Float4 &right) {
		return NodeFunctions::Instance()->Build< Float4 >( "Mod", left, right );
	}

	Float4 operator % (const Float4 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Float4 >( "Mod", left, right );
	}

	Bool  operator == (const Float  &left, const Float  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "Equal", left, right );
	}

	Bool2 operator == (const Float2 &left, const Float2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Equal", left, right );
	}

	Bool2 operator == (const Float &left, const Float2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Equal", left, right );
	}

	Bool2 operator == (const Float2 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Equal", left, right );
	}

	Bool3 operator == (const Float3 &left, const Float3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Equal", left, right );
	}

	Bool3 operator == (const Float &left, const Float3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Equal", left, right );
	}

	Bool3 operator == (const Float3 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Equal", left, right );
	}

	Bool4 operator == (const Float4 &left, const Float4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Equal", left, right );
	}

	Bool4 operator == (const Float &left, const Float4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Equal", left, right );
	}

	Bool4 operator == (const Float4 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Equal", left, right );
	}

	Bool  operator != (const Float  &left, const Float  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "NotEqual", left, right );
	}

	Bool2 operator != (const Float2 &left, const Float2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "NotEqual", left, right );
	}

	Bool2 operator != (const Float &left, const Float2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "NotEqual", left, right );
	}

	Bool2 operator != (const Float2 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "NotEqual", left, right );
	}

	Bool3 operator != (const Float3 &left, const Float3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "NotEqual", left, right );
	}

	Bool3 operator != (const Float &left, const Float3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "NotEqual", left, right );
	}

	Bool3 operator != (const Float3 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "NotEqual", left, right );
	}

	Bool4 operator != (const Float4 &left, const Float4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "NotEqual", left, right );
	}

	Bool4 operator != (const Float &left, const Float4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "NotEqual", left, right );
	}

	Bool4 operator != (const Float4 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "NotEqual", left, right );
	}

	Bool  operator > (const Float  &left, const Float  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "GreaterThan", left, right );
	}

	Bool2 operator > (const Float2 &left, const Float2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThan", left, right );
	}

	Bool2 operator > (const Float &left, const Float2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThan", left, right );
	}

	Bool2 operator > (const Float2 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThan", left, right );
	}

	Bool3 operator > (const Float3 &left, const Float3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThan", left, right );
	}

	Bool3 operator > (const Float &left, const Float3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThan", left, right );
	}

	Bool3 operator > (const Float3 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThan", left, right );
	}

	Bool4 operator > (const Float4 &left, const Float4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThan", left, right );
	}

	Bool4 operator > (const Float &left, const Float4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThan", left, right );
	}

	Bool4 operator > (const Float4 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThan", left, right );
	}

	Bool  operator < (const Float  &left, const Float  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "LessThan", left, right );
	}

	Bool2 operator < (const Float2 &left, const Float2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThan", left, right );
	}

	Bool2 operator < (const Float &left, const Float2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThan", left, right );
	}

	Bool2 operator < (const Float2 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThan", left, right );
	}

	Bool3 operator < (const Float3 &left, const Float3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThan", left, right );
	}

	Bool3 operator < (const Float &left, const Float3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThan", left, right );
	}

	Bool3 operator < (const Float3 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThan", left, right );
	}

	Bool4 operator < (const Float4 &left, const Float4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThan", left, right );
	}

	Bool4 operator < (const Float &left, const Float4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThan", left, right );
	}

	Bool4 operator < (const Float4 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThan", left, right );
	}

	Bool  operator >= (const Float  &left, const Float  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "GreaterThanEqual", left, right );
	}

	Bool2 operator >= (const Float2 &left, const Float2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThanEqual", left, right );
	}

	Bool2 operator >= (const Float &left, const Float2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThanEqual", left, right );
	}

	Bool2 operator >= (const Float2 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThanEqual", left, right );
	}

	Bool3 operator >= (const Float3 &left, const Float3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThanEqual", left, right );
	}

	Bool3 operator >= (const Float &left, const Float3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThanEqual", left, right );
	}

	Bool3 operator >= (const Float3 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThanEqual", left, right );
	}

	Bool4 operator >= (const Float4 &left, const Float4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThanEqual", left, right );
	}

	Bool4 operator >= (const Float &left, const Float4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThanEqual", left, right );
	}

	Bool4 operator >= (const Float4 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThanEqual", left, right );
	}

	Bool  operator <= (const Float  &left, const Float  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "LessThanEqual", left, right );
	}

	Bool2 operator <= (const Float2 &left, const Float2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThanEqual", left, right );
	}

	Bool2 operator <= (const Float &left, const Float2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThanEqual", left, right );
	}

	Bool2 operator <= (const Float2 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThanEqual", left, right );
	}

	Bool3 operator <= (const Float3 &left, const Float3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThanEqual", left, right );
	}

	Bool3 operator <= (const Float &left, const Float3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThanEqual", left, right );
	}

	Bool3 operator <= (const Float3 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThanEqual", left, right );
	}

	Bool4 operator <= (const Float4 &left, const Float4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThanEqual", left, right );
	}

	Bool4 operator <= (const Float &left, const Float4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThanEqual", left, right );
	}

	Bool4 operator <= (const Float4 &left, const Float &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThanEqual", left, right );
	}

	Double  operator + (const Double  &right) {
		return NodeFunctions::Instance()->Build< Double  >( "Plus", right );
	}

	Double2 operator + (const Double2 &right) {
		return NodeFunctions::Instance()->Build< Double2 >( "Plus", right );
	}

	Double3 operator + (const Double3 &right) {
		return NodeFunctions::Instance()->Build< Double3 >( "Plus", right );
	}

	Double4 operator + (const Double4 &right) {
		return NodeFunctions::Instance()->Build< Double4 >( "Plus", right );
	}

	Double  operator - (const Double  &right) {
		return NodeFunctions::Instance()->Build< Double  >( "Minus", right );
	}

	Double2 operator - (const Double2 &right) {
		return NodeFunctions::Instance()->Build< Double2 >( "Minus", right );
	}

	Double3 operator - (const Double3 &right) {
		return NodeFunctions::Instance()->Build< Double3 >( "Minus", right );
	}

	Double4 operator - (const Double4 &right) {
		return NodeFunctions::Instance()->Build< Double4 >( "Minus", right );
	}

	Double  operator + (const Double  &left, const Double  &right) {
		return NodeFunctions::Instance()->Build< Double  >( "Add", left, right );
	}

	Double2 operator + (const Double2 &left, const Double2 &right) {
		return NodeFunctions::Instance()->Build< Double2 >( "Add", left, right );
	}

	Double2 operator + (const Double &left, const Double2 &right) {
		return NodeFunctions::Instance()->Build< Double2 >( "Add", left, right );
	}

	Double2 operator + (const Double2 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Double2 >( "Add", left, right );
	}

	Double3 operator + (const Double3 &left, const Double3 &right) {
		return NodeFunctions::Instance()->Build< Double3 >( "Add", left, right );
	}

	Double3 operator + (const Double &left, const Double3 &right) {
		return NodeFunctions::Instance()->Build< Double3 >( "Add", left, right );
	}

	Double3 operator + (const Double3 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Double3 >( "Add", left, right );
	}

	Double4 operator + (const Double4 &left, const Double4 &right) {
		return NodeFunctions::Instance()->Build< Double4 >( "Add", left, right );
	}

	Double4 operator + (const Double &left, const Double4 &right) {
		return NodeFunctions::Instance()->Build< Double4 >( "Add", left, right );
	}

	Double4 operator + (const Double4 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Double4 >( "Add", left, right );
	}

	Double  operator - (const Double  &left, const Double  &right) {
		return NodeFunctions::Instance()->Build< Double  >( "Sub", left, right );
	}

	Double2 operator - (const Double2 &left, const Double2 &right) {
		return NodeFunctions::Instance()->Build< Double2 >( "Sub", left, right );
	}

	Double2 operator - (const Double &left, const Double2 &right) {
		return NodeFunctions::Instance()->Build< Double2 >( "Sub", left, right );
	}

	Double2 operator - (const Double2 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Double2 >( "Sub", left, right );
	}

	Double3 operator - (const Double3 &left, const Double3 &right) {
		return NodeFunctions::Instance()->Build< Double3 >( "Sub", left, right );
	}

	Double3 operator - (const Double &left, const Double3 &right) {
		return NodeFunctions::Instance()->Build< Double3 >( "Sub", left, right );
	}

	Double3 operator - (const Double3 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Double3 >( "Sub", left, right );
	}

	Double4 operator - (const Double4 &left, const Double4 &right) {
		return NodeFunctions::Instance()->Build< Double4 >( "Sub", left, right );
	}

	Double4 operator - (const Double &left, const Double4 &right) {
		return NodeFunctions::Instance()->Build< Double4 >( "Sub", left, right );
	}

	Double4 operator - (const Double4 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Double4 >( "Sub", left, right );
	}

	Double  operator * (const Double  &left, const Double  &right) {
		return NodeFunctions::Instance()->Build< Double  >( "Mul", left, right );
	}

	Double2 operator * (const Double2 &left, const Double2 &right) {
		return NodeFunctions::Instance()->Build< Double2 >( "Mul", left, right );
	}

	Double2 operator * (const Double &left, const Double2 &right) {
		return NodeFunctions::Instance()->Build< Double2 >( "Mul", left, right );
	}

	Double2 operator * (const Double2 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Double2 >( "Mul", left, right );
	}

	Double3 operator * (const Double3 &left, const Double3 &right) {
		return NodeFunctions::Instance()->Build< Double3 >( "Mul", left, right );
	}

	Double3 operator * (const Double &left, const Double3 &right) {
		return NodeFunctions::Instance()->Build< Double3 >( "Mul", left, right );
	}

	Double3 operator * (const Double3 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Double3 >( "Mul", left, right );
	}

	Double4 operator * (const Double4 &left, const Double4 &right) {
		return NodeFunctions::Instance()->Build< Double4 >( "Mul", left, right );
	}

	Double4 operator * (const Double &left, const Double4 &right) {
		return NodeFunctions::Instance()->Build< Double4 >( "Mul", left, right );
	}

	Double4 operator * (const Double4 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Double4 >( "Mul", left, right );
	}

	Double  operator / (const Double  &left, const Double  &right) {
		return NodeFunctions::Instance()->Build< Double  >( "Div", left, right );
	}

	Double2 operator / (const Double2 &left, const Double2 &right) {
		return NodeFunctions::Instance()->Build< Double2 >( "Div", left, right );
	}

	Double2 operator / (const Double &left, const Double2 &right) {
		return NodeFunctions::Instance()->Build< Double2 >( "Div", left, right );
	}

	Double2 operator / (const Double2 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Double2 >( "Div", left, right );
	}

	Double3 operator / (const Double3 &left, const Double3 &right) {
		return NodeFunctions::Instance()->Build< Double3 >( "Div", left, right );
	}

	Double3 operator / (const Double &left, const Double3 &right) {
		return NodeFunctions::Instance()->Build< Double3 >( "Div", left, right );
	}

	Double3 operator / (const Double3 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Double3 >( "Div", left, right );
	}

	Double4 operator / (const Double4 &left, const Double4 &right) {
		return NodeFunctions::Instance()->Build< Double4 >( "Div", left, right );
	}

	Double4 operator / (const Double &left, const Double4 &right) {
		return NodeFunctions::Instance()->Build< Double4 >( "Div", left, right );
	}

	Double4 operator / (const Double4 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Double4 >( "Div", left, right );
	}

	Double  operator % (const Double  &left, const Double  &right) {
		return NodeFunctions::Instance()->Build< Double  >( "Mod", left, right );
	}

	Double2 operator % (const Double2 &left, const Double2 &right) {
		return NodeFunctions::Instance()->Build< Double2 >( "Mod", left, right );
	}

	Double2 operator % (const Double &left, const Double2 &right) {
		return NodeFunctions::Instance()->Build< Double2 >( "Mod", left, right );
	}

	Double2 operator % (const Double2 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Double2 >( "Mod", left, right );
	}

	Double3 operator % (const Double3 &left, const Double3 &right) {
		return NodeFunctions::Instance()->Build< Double3 >( "Mod", left, right );
	}

	Double3 operator % (const Double &left, const Double3 &right) {
		return NodeFunctions::Instance()->Build< Double3 >( "Mod", left, right );
	}

	Double3 operator % (const Double3 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Double3 >( "Mod", left, right );
	}

	Double4 operator % (const Double4 &left, const Double4 &right) {
		return NodeFunctions::Instance()->Build< Double4 >( "Mod", left, right );
	}

	Double4 operator % (const Double &left, const Double4 &right) {
		return NodeFunctions::Instance()->Build< Double4 >( "Mod", left, right );
	}

	Double4 operator % (const Double4 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Double4 >( "Mod", left, right );
	}

	Bool  operator == (const Double  &left, const Double  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "Equal", left, right );
	}

	Bool2 operator == (const Double2 &left, const Double2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Equal", left, right );
	}

	Bool2 operator == (const Double &left, const Double2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Equal", left, right );
	}

	Bool2 operator == (const Double2 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "Equal", left, right );
	}

	Bool3 operator == (const Double3 &left, const Double3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Equal", left, right );
	}

	Bool3 operator == (const Double &left, const Double3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Equal", left, right );
	}

	Bool3 operator == (const Double3 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "Equal", left, right );
	}

	Bool4 operator == (const Double4 &left, const Double4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Equal", left, right );
	}

	Bool4 operator == (const Double &left, const Double4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Equal", left, right );
	}

	Bool4 operator == (const Double4 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "Equal", left, right );
	}

	Bool  operator != (const Double  &left, const Double  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "NotEqual", left, right );
	}

	Bool2 operator != (const Double2 &left, const Double2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "NotEqual", left, right );
	}

	Bool2 operator != (const Double &left, const Double2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "NotEqual", left, right );
	}

	Bool2 operator != (const Double2 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "NotEqual", left, right );
	}

	Bool3 operator != (const Double3 &left, const Double3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "NotEqual", left, right );
	}

	Bool3 operator != (const Double &left, const Double3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "NotEqual", left, right );
	}

	Bool3 operator != (const Double3 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "NotEqual", left, right );
	}

	Bool4 operator != (const Double4 &left, const Double4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "NotEqual", left, right );
	}

	Bool4 operator != (const Double &left, const Double4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "NotEqual", left, right );
	}

	Bool4 operator != (const Double4 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "NotEqual", left, right );
	}

	Bool  operator > (const Double  &left, const Double  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "GreaterThan", left, right );
	}

	Bool2 operator > (const Double2 &left, const Double2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThan", left, right );
	}

	Bool2 operator > (const Double &left, const Double2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThan", left, right );
	}

	Bool2 operator > (const Double2 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThan", left, right );
	}

	Bool3 operator > (const Double3 &left, const Double3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThan", left, right );
	}

	Bool3 operator > (const Double &left, const Double3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThan", left, right );
	}

	Bool3 operator > (const Double3 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThan", left, right );
	}

	Bool4 operator > (const Double4 &left, const Double4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThan", left, right );
	}

	Bool4 operator > (const Double &left, const Double4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThan", left, right );
	}

	Bool4 operator > (const Double4 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThan", left, right );
	}

	Bool  operator < (const Double  &left, const Double  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "LessThan", left, right );
	}

	Bool2 operator < (const Double2 &left, const Double2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThan", left, right );
	}

	Bool2 operator < (const Double &left, const Double2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThan", left, right );
	}

	Bool2 operator < (const Double2 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThan", left, right );
	}

	Bool3 operator < (const Double3 &left, const Double3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThan", left, right );
	}

	Bool3 operator < (const Double &left, const Double3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThan", left, right );
	}

	Bool3 operator < (const Double3 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThan", left, right );
	}

	Bool4 operator < (const Double4 &left, const Double4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThan", left, right );
	}

	Bool4 operator < (const Double &left, const Double4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThan", left, right );
	}

	Bool4 operator < (const Double4 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThan", left, right );
	}

	Bool  operator >= (const Double  &left, const Double  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "GreaterThanEqual", left, right );
	}

	Bool2 operator >= (const Double2 &left, const Double2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThanEqual", left, right );
	}

	Bool2 operator >= (const Double &left, const Double2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThanEqual", left, right );
	}

	Bool2 operator >= (const Double2 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "GreaterThanEqual", left, right );
	}

	Bool3 operator >= (const Double3 &left, const Double3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThanEqual", left, right );
	}

	Bool3 operator >= (const Double &left, const Double3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThanEqual", left, right );
	}

	Bool3 operator >= (const Double3 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "GreaterThanEqual", left, right );
	}

	Bool4 operator >= (const Double4 &left, const Double4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThanEqual", left, right );
	}

	Bool4 operator >= (const Double &left, const Double4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThanEqual", left, right );
	}

	Bool4 operator >= (const Double4 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "GreaterThanEqual", left, right );
	}

	Bool  operator <= (const Double  &left, const Double  &right) {
		return NodeFunctions::Instance()->Build< Bool  >( "LessThanEqual", left, right );
	}

	Bool2 operator <= (const Double2 &left, const Double2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThanEqual", left, right );
	}

	Bool2 operator <= (const Double &left, const Double2 &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThanEqual", left, right );
	}

	Bool2 operator <= (const Double2 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Bool2 >( "LessThanEqual", left, right );
	}

	Bool3 operator <= (const Double3 &left, const Double3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThanEqual", left, right );
	}

	Bool3 operator <= (const Double &left, const Double3 &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThanEqual", left, right );
	}

	Bool3 operator <= (const Double3 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Bool3 >( "LessThanEqual", left, right );
	}

	Bool4 operator <= (const Double4 &left, const Double4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThanEqual", left, right );
	}

	Bool4 operator <= (const Double &left, const Double4 &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThanEqual", left, right );
	}

	Bool4 operator <= (const Double4 &left, const Double &right) {
		return NodeFunctions::Instance()->Build< Bool4 >( "LessThanEqual", left, right );
	}


	// Type reinterpret cast
	Int AsInt (const UInt &x) {
		return NodeFunctions::Instance()->Build< Int >( "AsInt", x );
	}

	Int AsInt (const Float &x) {
		return NodeFunctions::Instance()->Build< Int >( "AsInt", x );
	}

	Int2 AsInt2 (const UInt2 &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "AsInt2", x );
	}

	Int2 AsInt2 (const Long &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "AsInt2", x );
	}

	Int2 AsInt2 (const ULong &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "AsInt2", x );
	}

	Int2 AsInt2 (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "AsInt2", x );
	}

	Int2 AsInt2 (const Double &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "AsInt2", x );
	}

	Int3 AsInt3 (const UInt3 &x) {
		return NodeFunctions::Instance()->Build< Int3 >( "AsInt3", x );
	}

	Int3 AsInt3 (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Int3 >( "AsInt3", x );
	}

	Int4 AsInt4 (const UInt4 &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "AsInt4", x );
	}

	Int4 AsInt4 (const Long2 &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "AsInt4", x );
	}

	Int4 AsInt4 (const ULong2 &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "AsInt4", x );
	}

	Int4 AsInt4 (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "AsInt4", x );
	}

	Int4 AsInt4 (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "AsInt4", x );
	}

	UInt AsUInt (const Int &x) {
		return NodeFunctions::Instance()->Build< UInt >( "AsUInt", x );
	}

	UInt AsUInt (const Float &x) {
		return NodeFunctions::Instance()->Build< UInt >( "AsUInt", x );
	}

	UInt2 AsUInt2 (const Int2 &x) {
		return NodeFunctions::Instance()->Build< UInt2 >( "AsUInt2", x );
	}

	UInt2 AsUInt2 (const Long &x) {
		return NodeFunctions::Instance()->Build< UInt2 >( "AsUInt2", x );
	}

	UInt2 AsUInt2 (const ULong &x) {
		return NodeFunctions::Instance()->Build< UInt2 >( "AsUInt2", x );
	}

	UInt2 AsUInt2 (const Float2 &x) {
		return NodeFunctions::Instance()->Build< UInt2 >( "AsUInt2", x );
	}

	UInt2 AsUInt2 (const Double &x) {
		return NodeFunctions::Instance()->Build< UInt2 >( "AsUInt2", x );
	}

	UInt3 AsUInt3 (const Int3 &x) {
		return NodeFunctions::Instance()->Build< UInt3 >( "AsUInt3", x );
	}

	UInt3 AsUInt3 (const Float3 &x) {
		return NodeFunctions::Instance()->Build< UInt3 >( "AsUInt3", x );
	}

	UInt4 AsUInt4 (const Int4 &x) {
		return NodeFunctions::Instance()->Build< UInt4 >( "AsUInt4", x );
	}

	UInt4 AsUInt4 (const Long2 &x) {
		return NodeFunctions::Instance()->Build< UInt4 >( "AsUInt4", x );
	}

	UInt4 AsUInt4 (const ULong2 &x) {
		return NodeFunctions::Instance()->Build< UInt4 >( "AsUInt4", x );
	}

	UInt4 AsUInt4 (const Float4 &x) {
		return NodeFunctions::Instance()->Build< UInt4 >( "AsUInt4", x );
	}

	UInt4 AsUInt4 (const Double2 &x) {
		return NodeFunctions::Instance()->Build< UInt4 >( "AsUInt4", x );
	}

	Long AsLong (const Int2 &x) {
		return NodeFunctions::Instance()->Build< Long >( "AsLong", x );
	}

	Long AsLong (const UInt2 &x) {
		return NodeFunctions::Instance()->Build< Long >( "AsLong", x );
	}

	Long AsLong (const ULong &x) {
		return NodeFunctions::Instance()->Build< Long >( "AsLong", x );
	}

	Long AsLong (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Long >( "AsLong", x );
	}

	Long AsLong (const Double &x) {
		return NodeFunctions::Instance()->Build< Long >( "AsLong", x );
	}

	Long2 AsLong2 (const Int4 &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "AsLong2", x );
	}

	Long2 AsLong2 (const UInt4 &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "AsLong2", x );
	}

	Long2 AsLong2 (const ULong2 &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "AsLong2", x );
	}

	Long2 AsLong2 (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "AsLong2", x );
	}

	Long2 AsLong2 (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "AsLong2", x );
	}

	Long3 AsLong3 (const ULong3 &x) {
		return NodeFunctions::Instance()->Build< Long3 >( "AsLong3", x );
	}

	Long3 AsLong3 (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Long3 >( "AsLong3", x );
	}

	Long4 AsLong4 (const ULong4 &x) {
		return NodeFunctions::Instance()->Build< Long4 >( "AsLong4", x );
	}

	Long4 AsLong4 (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Long4 >( "AsLong4", x );
	}

	ULong AsULong (const Int2 &x) {
		return NodeFunctions::Instance()->Build< ULong >( "AsULong", x );
	}

	ULong AsULong (const UInt2 &x) {
		return NodeFunctions::Instance()->Build< ULong >( "AsULong", x );
	}

	ULong AsULong (const Long &x) {
		return NodeFunctions::Instance()->Build< ULong >( "AsULong", x );
	}

	ULong AsULong (const Float2 &x) {
		return NodeFunctions::Instance()->Build< ULong >( "AsULong", x );
	}

	ULong AsULong (const Double &x) {
		return NodeFunctions::Instance()->Build< ULong >( "AsULong", x );
	}

	ULong2 AsULong2 (const Int4 &x) {
		return NodeFunctions::Instance()->Build< ULong2 >( "AsULong2", x );
	}

	ULong2 AsULong2 (const UInt4 &x) {
		return NodeFunctions::Instance()->Build< ULong2 >( "AsULong2", x );
	}

	ULong2 AsULong2 (const Long2 &x) {
		return NodeFunctions::Instance()->Build< ULong2 >( "AsULong2", x );
	}

	ULong2 AsULong2 (const Float4 &x) {
		return NodeFunctions::Instance()->Build< ULong2 >( "AsULong2", x );
	}

	ULong2 AsULong2 (const Double2 &x) {
		return NodeFunctions::Instance()->Build< ULong2 >( "AsULong2", x );
	}

	ULong3 AsULong3 (const Long3 &x) {
		return NodeFunctions::Instance()->Build< ULong3 >( "AsULong3", x );
	}

	ULong3 AsULong3 (const Double3 &x) {
		return NodeFunctions::Instance()->Build< ULong3 >( "AsULong3", x );
	}

	ULong4 AsULong4 (const Long4 &x) {
		return NodeFunctions::Instance()->Build< ULong4 >( "AsULong4", x );
	}

	ULong4 AsULong4 (const Double4 &x) {
		return NodeFunctions::Instance()->Build< ULong4 >( "AsULong4", x );
	}

	Float AsFloat (const Int &x) {
		return NodeFunctions::Instance()->Build< Float >( "AsFloat", x );
	}

	Float AsFloat (const UInt &x) {
		return NodeFunctions::Instance()->Build< Float >( "AsFloat", x );
	}

	Float2 AsFloat2 (const Int2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "AsFloat2", x );
	}

	Float2 AsFloat2 (const UInt2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "AsFloat2", x );
	}

	Float2 AsFloat2 (const Long &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "AsFloat2", x );
	}

	Float2 AsFloat2 (const ULong &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "AsFloat2", x );
	}

	Float2 AsFloat2 (const Double &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "AsFloat2", x );
	}

	Float3 AsFloat3 (const Int3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "AsFloat3", x );
	}

	Float3 AsFloat3 (const UInt3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "AsFloat3", x );
	}

	Float4 AsFloat4 (const Int4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "AsFloat4", x );
	}

	Float4 AsFloat4 (const UInt4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "AsFloat4", x );
	}

	Float4 AsFloat4 (const Long2 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "AsFloat4", x );
	}

	Float4 AsFloat4 (const ULong2 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "AsFloat4", x );
	}

	Float4 AsFloat4 (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "AsFloat4", x );
	}

	Double AsDouble (const Int2 &x) {
		return NodeFunctions::Instance()->Build< Double >( "AsDouble", x );
	}

	Double AsDouble (const UInt2 &x) {
		return NodeFunctions::Instance()->Build< Double >( "AsDouble", x );
	}

	Double AsDouble (const Long &x) {
		return NodeFunctions::Instance()->Build< Double >( "AsDouble", x );
	}

	Double AsDouble (const ULong &x) {
		return NodeFunctions::Instance()->Build< Double >( "AsDouble", x );
	}

	Double AsDouble (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Double >( "AsDouble", x );
	}

	Double2 AsDouble2 (const Int4 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "AsDouble2", x );
	}

	Double2 AsDouble2 (const UInt4 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "AsDouble2", x );
	}

	Double2 AsDouble2 (const Long2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "AsDouble2", x );
	}

	Double2 AsDouble2 (const ULong2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "AsDouble2", x );
	}

	Double2 AsDouble2 (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "AsDouble2", x );
	}

	Double3 AsDouble3 (const Long3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "AsDouble3", x );
	}

	Double3 AsDouble3 (const ULong3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "AsDouble3", x );
	}

	Double4 AsDouble4 (const Long4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "AsDouble4", x );
	}

	Double4 AsDouble4 (const ULong4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "AsDouble4", x );
	}


	// All, Any
	Bool All (const Bool  &x) {
		return NodeFunctions::Instance()->Build< Bool >( "All", x );
	}

	Bool All (const Bool2 &x) {
		return NodeFunctions::Instance()->Build< Bool >( "All", x );
	}

	Bool All (const Bool3 &x) {
		return NodeFunctions::Instance()->Build< Bool >( "All", x );
	}

	Bool All (const Bool4 &x) {
		return NodeFunctions::Instance()->Build< Bool >( "All", x );
	}

	Bool Any (const Bool  &x) {
		return NodeFunctions::Instance()->Build< Bool >( "Any", x );
	}

	Bool Any (const Bool2 &x) {
		return NodeFunctions::Instance()->Build< Bool >( "Any", x );
	}

	Bool Any (const Bool3 &x) {
		return NodeFunctions::Instance()->Build< Bool >( "Any", x );
	}

	Bool Any (const Bool4 &x) {
		return NodeFunctions::Instance()->Build< Bool >( "Any", x );
	}


	// Sin, Cos, Tan, CoTan
	Float  Sin (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "Sin", x );
	}

	Float2 Sin (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Sin", x );
	}

	Float3 Sin (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Sin", x );
	}

	Float4 Sin (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Sin", x );
	}

	Double  Sin (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "Sin", x );
	}

	Double2 Sin (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Sin", x );
	}

	Double3 Sin (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Sin", x );
	}

	Double4 Sin (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Sin", x );
	}

	Float  Cos (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "Cos", x );
	}

	Float2 Cos (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Cos", x );
	}

	Float3 Cos (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Cos", x );
	}

	Float4 Cos (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Cos", x );
	}

	Double  Cos (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "Cos", x );
	}

	Double2 Cos (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Cos", x );
	}

	Double3 Cos (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Cos", x );
	}

	Double4 Cos (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Cos", x );
	}

	Float  Tan (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "Tan", x );
	}

	Float2 Tan (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Tan", x );
	}

	Float3 Tan (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Tan", x );
	}

	Float4 Tan (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Tan", x );
	}

	Double  Tan (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "Tan", x );
	}

	Double2 Tan (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Tan", x );
	}

	Double3 Tan (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Tan", x );
	}

	Double4 Tan (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Tan", x );
	}

	Float  CoTan (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "CoTan", x );
	}

	Float2 CoTan (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "CoTan", x );
	}

	Float3 CoTan (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "CoTan", x );
	}

	Float4 CoTan (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "CoTan", x );
	}

	Double  CoTan (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "CoTan", x );
	}

	Double2 CoTan (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "CoTan", x );
	}

	Double3 CoTan (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "CoTan", x );
	}

	Double4 CoTan (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "CoTan", x );
	}


	// ASin, ACos, ATan, ACoTan
	Float  ASin (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "ASin", x );
	}

	Float2 ASin (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "ASin", x );
	}

	Float3 ASin (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "ASin", x );
	}

	Float4 ASin (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "ASin", x );
	}

	Double  ASin (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "ASin", x );
	}

	Double2 ASin (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "ASin", x );
	}

	Double3 ASin (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "ASin", x );
	}

	Double4 ASin (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "ASin", x );
	}

	Float  ACos (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "ACos", x );
	}

	Float2 ACos (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "ACos", x );
	}

	Float3 ACos (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "ACos", x );
	}

	Float4 ACos (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "ACos", x );
	}

	Double  ACos (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "ACos", x );
	}

	Double2 ACos (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "ACos", x );
	}

	Double3 ACos (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "ACos", x );
	}

	Double4 ACos (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "ACos", x );
	}

	Float  ATan (const Float  &y, const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "ATan", y, x );
	}

	Float  ATan (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "ATan", x );
	}

	Float2 ATan (const Float2 &y, const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "ATan", y, x );
	}

	Float2 ATan (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "ATan", x );
	}

	Float3 ATan (const Float3 &y, const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "ATan", y, x );
	}

	Float3 ATan (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "ATan", x );
	}

	Float4 ATan (const Float4 &y, const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "ATan", y, x );
	}

	Float4 ATan (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "ATan", x );
	}

	Double  ATan (const Double  &y, const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "ATan", y, x );
	}

	Double  ATan (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "ATan", x );
	}

	Double2 ATan (const Double2 &y, const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "ATan", y, x );
	}

	Double2 ATan (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "ATan", x );
	}

	Double3 ATan (const Double3 &y, const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "ATan", y, x );
	}

	Double3 ATan (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "ATan", x );
	}

	Double4 ATan (const Double4 &y, const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "ATan", y, x );
	}

	Double4 ATan (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "ATan", x );
	}

	Float  ACoTan (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "ACoTan", x );
	}

	Float2 ACoTan (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "ACoTan", x );
	}

	Float3 ACoTan (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "ACoTan", x );
	}

	Float4 ACoTan (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "ACoTan", x );
	}

	Double  ACoTan (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "ACoTan", x );
	}

	Double2 ACoTan (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "ACoTan", x );
	}

	Double3 ACoTan (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "ACoTan", x );
	}

	Double4 ACoTan (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "ACoTan", x );
	}


	// SinH, CosH, TanH, CoTanH
	Float  SinH (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "SinH", x );
	}

	Float2 SinH (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "SinH", x );
	}

	Float3 SinH (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "SinH", x );
	}

	Float4 SinH (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "SinH", x );
	}

	Double  SinH (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "SinH", x );
	}

	Double2 SinH (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "SinH", x );
	}

	Double3 SinH (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "SinH", x );
	}

	Double4 SinH (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "SinH", x );
	}

	Float  CosH (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "CosH", x );
	}

	Float2 CosH (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "CosH", x );
	}

	Float3 CosH (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "CosH", x );
	}

	Float4 CosH (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "CosH", x );
	}

	Double  CosH (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "CosH", x );
	}

	Double2 CosH (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "CosH", x );
	}

	Double3 CosH (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "CosH", x );
	}

	Double4 CosH (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "CosH", x );
	}

	Float  TanH (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "TanH", x );
	}

	Float2 TanH (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "TanH", x );
	}

	Float3 TanH (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "TanH", x );
	}

	Float4 TanH (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "TanH", x );
	}

	Double  TanH (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "TanH", x );
	}

	Double2 TanH (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "TanH", x );
	}

	Double3 TanH (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "TanH", x );
	}

	Double4 TanH (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "TanH", x );
	}

	Float  CoTanH (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "CoTanH", x );
	}

	Float2 CoTanH (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "CoTanH", x );
	}

	Float3 CoTanH (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "CoTanH", x );
	}

	Float4 CoTanH (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "CoTanH", x );
	}

	Double  CoTanH (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "CoTanH", x );
	}

	Double2 CoTanH (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "CoTanH", x );
	}

	Double3 CoTanH (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "CoTanH", x );
	}

	Double4 CoTanH (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "CoTanH", x );
	}


	// ASinH, ACosH, ATanH, ACoTanH
	Float  ASinH (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "ASinH", x );
	}

	Float2 ASinH (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "ASinH", x );
	}

	Float3 ASinH (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "ASinH", x );
	}

	Float4 ASinH (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "ASinH", x );
	}

	Double  ASinH (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "ASinH", x );
	}

	Double2 ASinH (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "ASinH", x );
	}

	Double3 ASinH (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "ASinH", x );
	}

	Double4 ASinH (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "ASinH", x );
	}

	Float  ACosH (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "ACosH", x );
	}

	Float2 ACosH (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "ACosH", x );
	}

	Float3 ACosH (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "ACosH", x );
	}

	Float4 ACosH (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "ACosH", x );
	}

	Double  ACosH (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "ACosH", x );
	}

	Double2 ACosH (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "ACosH", x );
	}

	Double3 ACosH (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "ACosH", x );
	}

	Double4 ACosH (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "ACosH", x );
	}

	Float  ATanH (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "ATanH", x );
	}

	Float2 ATanH (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "ATanH", x );
	}

	Float3 ATanH (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "ATanH", x );
	}

	Float4 ATanH (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "ATanH", x );
	}

	Double  ATanH (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "ATanH", x );
	}

	Double2 ATanH (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "ATanH", x );
	}

	Double3 ATanH (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "ATanH", x );
	}

	Double4 ATanH (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "ATanH", x );
	}

	Float  ACoTanH (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "ACoTanH", x );
	}

	Float2 ACoTanH (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "ACoTanH", x );
	}

	Float3 ACoTanH (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "ACoTanH", x );
	}

	Float4 ACoTanH (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "ACoTanH", x );
	}

	Double  ACoTanH (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "ACoTanH", x );
	}

	Double2 ACoTanH (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "ACoTanH", x );
	}

	Double3 ACoTanH (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "ACoTanH", x );
	}

	Double4 ACoTanH (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "ACoTanH", x );
	}


	// Abs, Square, SquareSign, Sign, SignOrZero
	Int  Abs (const Int  &x) {
		return NodeFunctions::Instance()->Build< Int  >( "Abs", x );
	}

	Int2 Abs (const Int2 &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "Abs", x );
	}

	Int3 Abs (const Int3 &x) {
		return NodeFunctions::Instance()->Build< Int3 >( "Abs", x );
	}

	Int4 Abs (const Int4 &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "Abs", x );
	}

	UInt  Abs (const UInt  &x) {
		return NodeFunctions::Instance()->Build< UInt  >( "Abs", x );
	}

	UInt2 Abs (const UInt2 &x) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Abs", x );
	}

	UInt3 Abs (const UInt3 &x) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Abs", x );
	}

	UInt4 Abs (const UInt4 &x) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Abs", x );
	}

	Long  Abs (const Long  &x) {
		return NodeFunctions::Instance()->Build< Long  >( "Abs", x );
	}

	Long2 Abs (const Long2 &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "Abs", x );
	}

	Long3 Abs (const Long3 &x) {
		return NodeFunctions::Instance()->Build< Long3 >( "Abs", x );
	}

	Long4 Abs (const Long4 &x) {
		return NodeFunctions::Instance()->Build< Long4 >( "Abs", x );
	}

	ULong  Abs (const ULong  &x) {
		return NodeFunctions::Instance()->Build< ULong  >( "Abs", x );
	}

	ULong2 Abs (const ULong2 &x) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Abs", x );
	}

	ULong3 Abs (const ULong3 &x) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Abs", x );
	}

	ULong4 Abs (const ULong4 &x) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Abs", x );
	}

	Float  Abs (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "Abs", x );
	}

	Float2 Abs (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Abs", x );
	}

	Float3 Abs (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Abs", x );
	}

	Float4 Abs (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Abs", x );
	}

	Double  Abs (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "Abs", x );
	}

	Double2 Abs (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Abs", x );
	}

	Double3 Abs (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Abs", x );
	}

	Double4 Abs (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Abs", x );
	}

	Int  Square (const Int  &x) {
		return NodeFunctions::Instance()->Build< Int  >( "Square", x );
	}

	Int2 Square (const Int2 &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "Square", x );
	}

	Int3 Square (const Int3 &x) {
		return NodeFunctions::Instance()->Build< Int3 >( "Square", x );
	}

	Int4 Square (const Int4 &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "Square", x );
	}

	UInt  Square (const UInt  &x) {
		return NodeFunctions::Instance()->Build< UInt  >( "Square", x );
	}

	UInt2 Square (const UInt2 &x) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Square", x );
	}

	UInt3 Square (const UInt3 &x) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Square", x );
	}

	UInt4 Square (const UInt4 &x) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Square", x );
	}

	Long  Square (const Long  &x) {
		return NodeFunctions::Instance()->Build< Long  >( "Square", x );
	}

	Long2 Square (const Long2 &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "Square", x );
	}

	Long3 Square (const Long3 &x) {
		return NodeFunctions::Instance()->Build< Long3 >( "Square", x );
	}

	Long4 Square (const Long4 &x) {
		return NodeFunctions::Instance()->Build< Long4 >( "Square", x );
	}

	ULong  Square (const ULong  &x) {
		return NodeFunctions::Instance()->Build< ULong  >( "Square", x );
	}

	ULong2 Square (const ULong2 &x) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Square", x );
	}

	ULong3 Square (const ULong3 &x) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Square", x );
	}

	ULong4 Square (const ULong4 &x) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Square", x );
	}

	Float  Square (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "Square", x );
	}

	Float2 Square (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Square", x );
	}

	Float3 Square (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Square", x );
	}

	Float4 Square (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Square", x );
	}

	Double  Square (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "Square", x );
	}

	Double2 Square (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Square", x );
	}

	Double3 Square (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Square", x );
	}

	Double4 Square (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Square", x );
	}

	Int  Sign (const Int  &x) {
		return NodeFunctions::Instance()->Build< Int  >( "Sign", x );
	}

	Int2 Sign (const Int2 &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "Sign", x );
	}

	Int3 Sign (const Int3 &x) {
		return NodeFunctions::Instance()->Build< Int3 >( "Sign", x );
	}

	Int4 Sign (const Int4 &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "Sign", x );
	}

	UInt  Sign (const UInt  &x) {
		return NodeFunctions::Instance()->Build< UInt  >( "Sign", x );
	}

	UInt2 Sign (const UInt2 &x) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Sign", x );
	}

	UInt3 Sign (const UInt3 &x) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Sign", x );
	}

	UInt4 Sign (const UInt4 &x) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Sign", x );
	}

	Long  Sign (const Long  &x) {
		return NodeFunctions::Instance()->Build< Long  >( "Sign", x );
	}

	Long2 Sign (const Long2 &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "Sign", x );
	}

	Long3 Sign (const Long3 &x) {
		return NodeFunctions::Instance()->Build< Long3 >( "Sign", x );
	}

	Long4 Sign (const Long4 &x) {
		return NodeFunctions::Instance()->Build< Long4 >( "Sign", x );
	}

	ULong  Sign (const ULong  &x) {
		return NodeFunctions::Instance()->Build< ULong  >( "Sign", x );
	}

	ULong2 Sign (const ULong2 &x) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Sign", x );
	}

	ULong3 Sign (const ULong3 &x) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Sign", x );
	}

	ULong4 Sign (const ULong4 &x) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Sign", x );
	}

	Float  Sign (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "Sign", x );
	}

	Float2 Sign (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Sign", x );
	}

	Float3 Sign (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Sign", x );
	}

	Float4 Sign (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Sign", x );
	}

	Double  Sign (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "Sign", x );
	}

	Double2 Sign (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Sign", x );
	}

	Double3 Sign (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Sign", x );
	}

	Double4 Sign (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Sign", x );
	}

	Int  SignOrZero (const Int  &x) {
		return NodeFunctions::Instance()->Build< Int  >( "SignOrZero", x );
	}

	Int2 SignOrZero (const Int2 &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "SignOrZero", x );
	}

	Int3 SignOrZero (const Int3 &x) {
		return NodeFunctions::Instance()->Build< Int3 >( "SignOrZero", x );
	}

	Int4 SignOrZero (const Int4 &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "SignOrZero", x );
	}

	UInt  SignOrZero (const UInt  &x) {
		return NodeFunctions::Instance()->Build< UInt  >( "SignOrZero", x );
	}

	UInt2 SignOrZero (const UInt2 &x) {
		return NodeFunctions::Instance()->Build< UInt2 >( "SignOrZero", x );
	}

	UInt3 SignOrZero (const UInt3 &x) {
		return NodeFunctions::Instance()->Build< UInt3 >( "SignOrZero", x );
	}

	UInt4 SignOrZero (const UInt4 &x) {
		return NodeFunctions::Instance()->Build< UInt4 >( "SignOrZero", x );
	}

	Long  SignOrZero (const Long  &x) {
		return NodeFunctions::Instance()->Build< Long  >( "SignOrZero", x );
	}

	Long2 SignOrZero (const Long2 &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "SignOrZero", x );
	}

	Long3 SignOrZero (const Long3 &x) {
		return NodeFunctions::Instance()->Build< Long3 >( "SignOrZero", x );
	}

	Long4 SignOrZero (const Long4 &x) {
		return NodeFunctions::Instance()->Build< Long4 >( "SignOrZero", x );
	}

	ULong  SignOrZero (const ULong  &x) {
		return NodeFunctions::Instance()->Build< ULong  >( "SignOrZero", x );
	}

	ULong2 SignOrZero (const ULong2 &x) {
		return NodeFunctions::Instance()->Build< ULong2 >( "SignOrZero", x );
	}

	ULong3 SignOrZero (const ULong3 &x) {
		return NodeFunctions::Instance()->Build< ULong3 >( "SignOrZero", x );
	}

	ULong4 SignOrZero (const ULong4 &x) {
		return NodeFunctions::Instance()->Build< ULong4 >( "SignOrZero", x );
	}

	Float  SignOrZero (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "SignOrZero", x );
	}

	Float2 SignOrZero (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "SignOrZero", x );
	}

	Float3 SignOrZero (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "SignOrZero", x );
	}

	Float4 SignOrZero (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "SignOrZero", x );
	}

	Double  SignOrZero (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "SignOrZero", x );
	}

	Double2 SignOrZero (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "SignOrZero", x );
	}

	Double3 SignOrZero (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "SignOrZero", x );
	}

	Double4 SignOrZero (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "SignOrZero", x );
	}

	Int  SquareSign (const Int  &x) {
		return NodeFunctions::Instance()->Build< Int  >( "SquareSign", x );
	}

	Int2 SquareSign (const Int2 &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "SquareSign", x );
	}

	Int3 SquareSign (const Int3 &x) {
		return NodeFunctions::Instance()->Build< Int3 >( "SquareSign", x );
	}

	Int4 SquareSign (const Int4 &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "SquareSign", x );
	}

	UInt  SquareSign (const UInt  &x) {
		return NodeFunctions::Instance()->Build< UInt  >( "SquareSign", x );
	}

	UInt2 SquareSign (const UInt2 &x) {
		return NodeFunctions::Instance()->Build< UInt2 >( "SquareSign", x );
	}

	UInt3 SquareSign (const UInt3 &x) {
		return NodeFunctions::Instance()->Build< UInt3 >( "SquareSign", x );
	}

	UInt4 SquareSign (const UInt4 &x) {
		return NodeFunctions::Instance()->Build< UInt4 >( "SquareSign", x );
	}

	Long  SquareSign (const Long  &x) {
		return NodeFunctions::Instance()->Build< Long  >( "SquareSign", x );
	}

	Long2 SquareSign (const Long2 &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "SquareSign", x );
	}

	Long3 SquareSign (const Long3 &x) {
		return NodeFunctions::Instance()->Build< Long3 >( "SquareSign", x );
	}

	Long4 SquareSign (const Long4 &x) {
		return NodeFunctions::Instance()->Build< Long4 >( "SquareSign", x );
	}

	ULong  SquareSign (const ULong  &x) {
		return NodeFunctions::Instance()->Build< ULong  >( "SquareSign", x );
	}

	ULong2 SquareSign (const ULong2 &x) {
		return NodeFunctions::Instance()->Build< ULong2 >( "SquareSign", x );
	}

	ULong3 SquareSign (const ULong3 &x) {
		return NodeFunctions::Instance()->Build< ULong3 >( "SquareSign", x );
	}

	ULong4 SquareSign (const ULong4 &x) {
		return NodeFunctions::Instance()->Build< ULong4 >( "SquareSign", x );
	}

	Float  SquareSign (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "SquareSign", x );
	}

	Float2 SquareSign (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "SquareSign", x );
	}

	Float3 SquareSign (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "SquareSign", x );
	}

	Float4 SquareSign (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "SquareSign", x );
	}

	Double  SquareSign (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "SquareSign", x );
	}

	Double2 SquareSign (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "SquareSign", x );
	}

	Double3 SquareSign (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "SquareSign", x );
	}

	Double4 SquareSign (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "SquareSign", x );
	}


	// SafeDiv, SafeMod
	Int  SafeDiv (const Int  &x, const Int  &y, const Int  &def) {
		return NodeFunctions::Instance()->Build< Int  >( "SafeDiv", x, y, def );
	}

	Int2 SafeDiv (const Int2 &x, const Int2 &y, const Int2 &def) {
		return NodeFunctions::Instance()->Build< Int2 >( "SafeDiv", x, y, def );
	}

	Int2 SafeDiv (const Int2 &x, const Int &y, const Int &def) {
		return NodeFunctions::Instance()->Build< Int2 >( "SafeDiv", x, y, def );
	}

	Int3 SafeDiv (const Int3 &x, const Int3 &y, const Int3 &def) {
		return NodeFunctions::Instance()->Build< Int3 >( "SafeDiv", x, y, def );
	}

	Int3 SafeDiv (const Int3 &x, const Int &y, const Int &def) {
		return NodeFunctions::Instance()->Build< Int3 >( "SafeDiv", x, y, def );
	}

	Int4 SafeDiv (const Int4 &x, const Int4 &y, const Int4 &def) {
		return NodeFunctions::Instance()->Build< Int4 >( "SafeDiv", x, y, def );
	}

	Int4 SafeDiv (const Int4 &x, const Int &y, const Int &def) {
		return NodeFunctions::Instance()->Build< Int4 >( "SafeDiv", x, y, def );
	}

	UInt  SafeDiv (const UInt  &x, const UInt  &y, const UInt  &def) {
		return NodeFunctions::Instance()->Build< UInt  >( "SafeDiv", x, y, def );
	}

	UInt2 SafeDiv (const UInt2 &x, const UInt2 &y, const UInt2 &def) {
		return NodeFunctions::Instance()->Build< UInt2 >( "SafeDiv", x, y, def );
	}

	UInt2 SafeDiv (const UInt2 &x, const UInt &y, const UInt &def) {
		return NodeFunctions::Instance()->Build< UInt2 >( "SafeDiv", x, y, def );
	}

	UInt3 SafeDiv (const UInt3 &x, const UInt3 &y, const UInt3 &def) {
		return NodeFunctions::Instance()->Build< UInt3 >( "SafeDiv", x, y, def );
	}

	UInt3 SafeDiv (const UInt3 &x, const UInt &y, const UInt &def) {
		return NodeFunctions::Instance()->Build< UInt3 >( "SafeDiv", x, y, def );
	}

	UInt4 SafeDiv (const UInt4 &x, const UInt4 &y, const UInt4 &def) {
		return NodeFunctions::Instance()->Build< UInt4 >( "SafeDiv", x, y, def );
	}

	UInt4 SafeDiv (const UInt4 &x, const UInt &y, const UInt &def) {
		return NodeFunctions::Instance()->Build< UInt4 >( "SafeDiv", x, y, def );
	}

	Long  SafeDiv (const Long  &x, const Long  &y, const Long  &def) {
		return NodeFunctions::Instance()->Build< Long  >( "SafeDiv", x, y, def );
	}

	Long2 SafeDiv (const Long2 &x, const Long2 &y, const Long2 &def) {
		return NodeFunctions::Instance()->Build< Long2 >( "SafeDiv", x, y, def );
	}

	Long2 SafeDiv (const Long2 &x, const Long &y, const Long &def) {
		return NodeFunctions::Instance()->Build< Long2 >( "SafeDiv", x, y, def );
	}

	Long3 SafeDiv (const Long3 &x, const Long3 &y, const Long3 &def) {
		return NodeFunctions::Instance()->Build< Long3 >( "SafeDiv", x, y, def );
	}

	Long3 SafeDiv (const Long3 &x, const Long &y, const Long &def) {
		return NodeFunctions::Instance()->Build< Long3 >( "SafeDiv", x, y, def );
	}

	Long4 SafeDiv (const Long4 &x, const Long4 &y, const Long4 &def) {
		return NodeFunctions::Instance()->Build< Long4 >( "SafeDiv", x, y, def );
	}

	Long4 SafeDiv (const Long4 &x, const Long &y, const Long &def) {
		return NodeFunctions::Instance()->Build< Long4 >( "SafeDiv", x, y, def );
	}

	ULong  SafeDiv (const ULong  &x, const ULong  &y, const ULong  &def) {
		return NodeFunctions::Instance()->Build< ULong  >( "SafeDiv", x, y, def );
	}

	ULong2 SafeDiv (const ULong2 &x, const ULong2 &y, const ULong2 &def) {
		return NodeFunctions::Instance()->Build< ULong2 >( "SafeDiv", x, y, def );
	}

	ULong2 SafeDiv (const ULong2 &x, const ULong &y, const ULong &def) {
		return NodeFunctions::Instance()->Build< ULong2 >( "SafeDiv", x, y, def );
	}

	ULong3 SafeDiv (const ULong3 &x, const ULong3 &y, const ULong3 &def) {
		return NodeFunctions::Instance()->Build< ULong3 >( "SafeDiv", x, y, def );
	}

	ULong3 SafeDiv (const ULong3 &x, const ULong &y, const ULong &def) {
		return NodeFunctions::Instance()->Build< ULong3 >( "SafeDiv", x, y, def );
	}

	ULong4 SafeDiv (const ULong4 &x, const ULong4 &y, const ULong4 &def) {
		return NodeFunctions::Instance()->Build< ULong4 >( "SafeDiv", x, y, def );
	}

	ULong4 SafeDiv (const ULong4 &x, const ULong &y, const ULong &def) {
		return NodeFunctions::Instance()->Build< ULong4 >( "SafeDiv", x, y, def );
	}

	Float  SafeDiv (const Float  &x, const Float  &y, const Float  &def) {
		return NodeFunctions::Instance()->Build< Float  >( "SafeDiv", x, y, def );
	}

	Float2 SafeDiv (const Float2 &x, const Float2 &y, const Float2 &def) {
		return NodeFunctions::Instance()->Build< Float2 >( "SafeDiv", x, y, def );
	}

	Float2 SafeDiv (const Float2 &x, const Float &y, const Float &def) {
		return NodeFunctions::Instance()->Build< Float2 >( "SafeDiv", x, y, def );
	}

	Float3 SafeDiv (const Float3 &x, const Float3 &y, const Float3 &def) {
		return NodeFunctions::Instance()->Build< Float3 >( "SafeDiv", x, y, def );
	}

	Float3 SafeDiv (const Float3 &x, const Float &y, const Float &def) {
		return NodeFunctions::Instance()->Build< Float3 >( "SafeDiv", x, y, def );
	}

	Float4 SafeDiv (const Float4 &x, const Float4 &y, const Float4 &def) {
		return NodeFunctions::Instance()->Build< Float4 >( "SafeDiv", x, y, def );
	}

	Float4 SafeDiv (const Float4 &x, const Float &y, const Float &def) {
		return NodeFunctions::Instance()->Build< Float4 >( "SafeDiv", x, y, def );
	}

	Double  SafeDiv (const Double  &x, const Double  &y, const Double  &def) {
		return NodeFunctions::Instance()->Build< Double  >( "SafeDiv", x, y, def );
	}

	Double2 SafeDiv (const Double2 &x, const Double2 &y, const Double2 &def) {
		return NodeFunctions::Instance()->Build< Double2 >( "SafeDiv", x, y, def );
	}

	Double2 SafeDiv (const Double2 &x, const Double &y, const Double &def) {
		return NodeFunctions::Instance()->Build< Double2 >( "SafeDiv", x, y, def );
	}

	Double3 SafeDiv (const Double3 &x, const Double3 &y, const Double3 &def) {
		return NodeFunctions::Instance()->Build< Double3 >( "SafeDiv", x, y, def );
	}

	Double3 SafeDiv (const Double3 &x, const Double &y, const Double &def) {
		return NodeFunctions::Instance()->Build< Double3 >( "SafeDiv", x, y, def );
	}

	Double4 SafeDiv (const Double4 &x, const Double4 &y, const Double4 &def) {
		return NodeFunctions::Instance()->Build< Double4 >( "SafeDiv", x, y, def );
	}

	Double4 SafeDiv (const Double4 &x, const Double &y, const Double &def) {
		return NodeFunctions::Instance()->Build< Double4 >( "SafeDiv", x, y, def );
	}

	Int  SafeMod (const Int  &x, const Int  &y, const Int  &def) {
		return NodeFunctions::Instance()->Build< Int  >( "SafeMod", x, y, def );
	}

	Int2 SafeMod (const Int2 &x, const Int2 &y, const Int2 &def) {
		return NodeFunctions::Instance()->Build< Int2 >( "SafeMod", x, y, def );
	}

	Int2 SafeMod (const Int2 &x, const Int &y, const Int &def) {
		return NodeFunctions::Instance()->Build< Int2 >( "SafeMod", x, y, def );
	}

	Int3 SafeMod (const Int3 &x, const Int3 &y, const Int3 &def) {
		return NodeFunctions::Instance()->Build< Int3 >( "SafeMod", x, y, def );
	}

	Int3 SafeMod (const Int3 &x, const Int &y, const Int &def) {
		return NodeFunctions::Instance()->Build< Int3 >( "SafeMod", x, y, def );
	}

	Int4 SafeMod (const Int4 &x, const Int4 &y, const Int4 &def) {
		return NodeFunctions::Instance()->Build< Int4 >( "SafeMod", x, y, def );
	}

	Int4 SafeMod (const Int4 &x, const Int &y, const Int &def) {
		return NodeFunctions::Instance()->Build< Int4 >( "SafeMod", x, y, def );
	}

	UInt  SafeMod (const UInt  &x, const UInt  &y, const UInt  &def) {
		return NodeFunctions::Instance()->Build< UInt  >( "SafeMod", x, y, def );
	}

	UInt2 SafeMod (const UInt2 &x, const UInt2 &y, const UInt2 &def) {
		return NodeFunctions::Instance()->Build< UInt2 >( "SafeMod", x, y, def );
	}

	UInt2 SafeMod (const UInt2 &x, const UInt &y, const UInt &def) {
		return NodeFunctions::Instance()->Build< UInt2 >( "SafeMod", x, y, def );
	}

	UInt3 SafeMod (const UInt3 &x, const UInt3 &y, const UInt3 &def) {
		return NodeFunctions::Instance()->Build< UInt3 >( "SafeMod", x, y, def );
	}

	UInt3 SafeMod (const UInt3 &x, const UInt &y, const UInt &def) {
		return NodeFunctions::Instance()->Build< UInt3 >( "SafeMod", x, y, def );
	}

	UInt4 SafeMod (const UInt4 &x, const UInt4 &y, const UInt4 &def) {
		return NodeFunctions::Instance()->Build< UInt4 >( "SafeMod", x, y, def );
	}

	UInt4 SafeMod (const UInt4 &x, const UInt &y, const UInt &def) {
		return NodeFunctions::Instance()->Build< UInt4 >( "SafeMod", x, y, def );
	}

	Long  SafeMod (const Long  &x, const Long  &y, const Long  &def) {
		return NodeFunctions::Instance()->Build< Long  >( "SafeMod", x, y, def );
	}

	Long2 SafeMod (const Long2 &x, const Long2 &y, const Long2 &def) {
		return NodeFunctions::Instance()->Build< Long2 >( "SafeMod", x, y, def );
	}

	Long2 SafeMod (const Long2 &x, const Long &y, const Long &def) {
		return NodeFunctions::Instance()->Build< Long2 >( "SafeMod", x, y, def );
	}

	Long3 SafeMod (const Long3 &x, const Long3 &y, const Long3 &def) {
		return NodeFunctions::Instance()->Build< Long3 >( "SafeMod", x, y, def );
	}

	Long3 SafeMod (const Long3 &x, const Long &y, const Long &def) {
		return NodeFunctions::Instance()->Build< Long3 >( "SafeMod", x, y, def );
	}

	Long4 SafeMod (const Long4 &x, const Long4 &y, const Long4 &def) {
		return NodeFunctions::Instance()->Build< Long4 >( "SafeMod", x, y, def );
	}

	Long4 SafeMod (const Long4 &x, const Long &y, const Long &def) {
		return NodeFunctions::Instance()->Build< Long4 >( "SafeMod", x, y, def );
	}

	ULong  SafeMod (const ULong  &x, const ULong  &y, const ULong  &def) {
		return NodeFunctions::Instance()->Build< ULong  >( "SafeMod", x, y, def );
	}

	ULong2 SafeMod (const ULong2 &x, const ULong2 &y, const ULong2 &def) {
		return NodeFunctions::Instance()->Build< ULong2 >( "SafeMod", x, y, def );
	}

	ULong2 SafeMod (const ULong2 &x, const ULong &y, const ULong &def) {
		return NodeFunctions::Instance()->Build< ULong2 >( "SafeMod", x, y, def );
	}

	ULong3 SafeMod (const ULong3 &x, const ULong3 &y, const ULong3 &def) {
		return NodeFunctions::Instance()->Build< ULong3 >( "SafeMod", x, y, def );
	}

	ULong3 SafeMod (const ULong3 &x, const ULong &y, const ULong &def) {
		return NodeFunctions::Instance()->Build< ULong3 >( "SafeMod", x, y, def );
	}

	ULong4 SafeMod (const ULong4 &x, const ULong4 &y, const ULong4 &def) {
		return NodeFunctions::Instance()->Build< ULong4 >( "SafeMod", x, y, def );
	}

	ULong4 SafeMod (const ULong4 &x, const ULong &y, const ULong &def) {
		return NodeFunctions::Instance()->Build< ULong4 >( "SafeMod", x, y, def );
	}

	Float  SafeMod (const Float  &x, const Float  &y, const Float  &def) {
		return NodeFunctions::Instance()->Build< Float  >( "SafeMod", x, y, def );
	}

	Float2 SafeMod (const Float2 &x, const Float2 &y, const Float2 &def) {
		return NodeFunctions::Instance()->Build< Float2 >( "SafeMod", x, y, def );
	}

	Float2 SafeMod (const Float2 &x, const Float &y, const Float &def) {
		return NodeFunctions::Instance()->Build< Float2 >( "SafeMod", x, y, def );
	}

	Float3 SafeMod (const Float3 &x, const Float3 &y, const Float3 &def) {
		return NodeFunctions::Instance()->Build< Float3 >( "SafeMod", x, y, def );
	}

	Float3 SafeMod (const Float3 &x, const Float &y, const Float &def) {
		return NodeFunctions::Instance()->Build< Float3 >( "SafeMod", x, y, def );
	}

	Float4 SafeMod (const Float4 &x, const Float4 &y, const Float4 &def) {
		return NodeFunctions::Instance()->Build< Float4 >( "SafeMod", x, y, def );
	}

	Float4 SafeMod (const Float4 &x, const Float &y, const Float &def) {
		return NodeFunctions::Instance()->Build< Float4 >( "SafeMod", x, y, def );
	}

	Double  SafeMod (const Double  &x, const Double  &y, const Double  &def) {
		return NodeFunctions::Instance()->Build< Double  >( "SafeMod", x, y, def );
	}

	Double2 SafeMod (const Double2 &x, const Double2 &y, const Double2 &def) {
		return NodeFunctions::Instance()->Build< Double2 >( "SafeMod", x, y, def );
	}

	Double2 SafeMod (const Double2 &x, const Double &y, const Double &def) {
		return NodeFunctions::Instance()->Build< Double2 >( "SafeMod", x, y, def );
	}

	Double3 SafeMod (const Double3 &x, const Double3 &y, const Double3 &def) {
		return NodeFunctions::Instance()->Build< Double3 >( "SafeMod", x, y, def );
	}

	Double3 SafeMod (const Double3 &x, const Double &y, const Double &def) {
		return NodeFunctions::Instance()->Build< Double3 >( "SafeMod", x, y, def );
	}

	Double4 SafeMod (const Double4 &x, const Double4 &y, const Double4 &def) {
		return NodeFunctions::Instance()->Build< Double4 >( "SafeMod", x, y, def );
	}

	Double4 SafeMod (const Double4 &x, const Double &y, const Double &def) {
		return NodeFunctions::Instance()->Build< Double4 >( "SafeMod", x, y, def );
	}


	// IsZero, IsNotZero, IsOdd, IsEven
	Bool  IsZero (const Int  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsZero", x );
	}

	Bool2 IsZero (const Int2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsZero", x );
	}

	Bool3 IsZero (const Int3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsZero", x );
	}

	Bool4 IsZero (const Int4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsZero", x );
	}

	Bool  IsZero (const UInt  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsZero", x );
	}

	Bool2 IsZero (const UInt2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsZero", x );
	}

	Bool3 IsZero (const UInt3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsZero", x );
	}

	Bool4 IsZero (const UInt4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsZero", x );
	}

	Bool  IsZero (const Long  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsZero", x );
	}

	Bool2 IsZero (const Long2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsZero", x );
	}

	Bool3 IsZero (const Long3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsZero", x );
	}

	Bool4 IsZero (const Long4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsZero", x );
	}

	Bool  IsZero (const ULong  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsZero", x );
	}

	Bool2 IsZero (const ULong2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsZero", x );
	}

	Bool3 IsZero (const ULong3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsZero", x );
	}

	Bool4 IsZero (const ULong4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsZero", x );
	}

	Bool  IsZero (const Float  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsZero", x );
	}

	Bool2 IsZero (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsZero", x );
	}

	Bool3 IsZero (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsZero", x );
	}

	Bool4 IsZero (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsZero", x );
	}

	Bool  IsZero (const Double  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsZero", x );
	}

	Bool2 IsZero (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsZero", x );
	}

	Bool3 IsZero (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsZero", x );
	}

	Bool4 IsZero (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsZero", x );
	}

	Bool  IsNotZero (const Int  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsNotZero", x );
	}

	Bool2 IsNotZero (const Int2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsNotZero", x );
	}

	Bool3 IsNotZero (const Int3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsNotZero", x );
	}

	Bool4 IsNotZero (const Int4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsNotZero", x );
	}

	Bool  IsNotZero (const UInt  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsNotZero", x );
	}

	Bool2 IsNotZero (const UInt2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsNotZero", x );
	}

	Bool3 IsNotZero (const UInt3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsNotZero", x );
	}

	Bool4 IsNotZero (const UInt4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsNotZero", x );
	}

	Bool  IsNotZero (const Long  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsNotZero", x );
	}

	Bool2 IsNotZero (const Long2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsNotZero", x );
	}

	Bool3 IsNotZero (const Long3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsNotZero", x );
	}

	Bool4 IsNotZero (const Long4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsNotZero", x );
	}

	Bool  IsNotZero (const ULong  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsNotZero", x );
	}

	Bool2 IsNotZero (const ULong2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsNotZero", x );
	}

	Bool3 IsNotZero (const ULong3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsNotZero", x );
	}

	Bool4 IsNotZero (const ULong4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsNotZero", x );
	}

	Bool  IsNotZero (const Float  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsNotZero", x );
	}

	Bool2 IsNotZero (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsNotZero", x );
	}

	Bool3 IsNotZero (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsNotZero", x );
	}

	Bool4 IsNotZero (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsNotZero", x );
	}

	Bool  IsNotZero (const Double  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsNotZero", x );
	}

	Bool2 IsNotZero (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsNotZero", x );
	}

	Bool3 IsNotZero (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsNotZero", x );
	}

	Bool4 IsNotZero (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsNotZero", x );
	}

	Bool  IsOdd (const Int  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsOdd", x );
	}

	Bool2 IsOdd (const Int2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsOdd", x );
	}

	Bool3 IsOdd (const Int3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsOdd", x );
	}

	Bool4 IsOdd (const Int4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsOdd", x );
	}

	Bool  IsOdd (const UInt  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsOdd", x );
	}

	Bool2 IsOdd (const UInt2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsOdd", x );
	}

	Bool3 IsOdd (const UInt3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsOdd", x );
	}

	Bool4 IsOdd (const UInt4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsOdd", x );
	}

	Bool  IsOdd (const Long  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsOdd", x );
	}

	Bool2 IsOdd (const Long2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsOdd", x );
	}

	Bool3 IsOdd (const Long3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsOdd", x );
	}

	Bool4 IsOdd (const Long4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsOdd", x );
	}

	Bool  IsOdd (const ULong  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsOdd", x );
	}

	Bool2 IsOdd (const ULong2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsOdd", x );
	}

	Bool3 IsOdd (const ULong3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsOdd", x );
	}

	Bool4 IsOdd (const ULong4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsOdd", x );
	}

	Bool  IsOdd (const Float  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsOdd", x );
	}

	Bool2 IsOdd (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsOdd", x );
	}

	Bool3 IsOdd (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsOdd", x );
	}

	Bool4 IsOdd (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsOdd", x );
	}

	Bool  IsOdd (const Double  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsOdd", x );
	}

	Bool2 IsOdd (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsOdd", x );
	}

	Bool3 IsOdd (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsOdd", x );
	}

	Bool4 IsOdd (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsOdd", x );
	}

	Bool  IsEven (const Int  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsEven", x );
	}

	Bool2 IsEven (const Int2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsEven", x );
	}

	Bool3 IsEven (const Int3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsEven", x );
	}

	Bool4 IsEven (const Int4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsEven", x );
	}

	Bool  IsEven (const UInt  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsEven", x );
	}

	Bool2 IsEven (const UInt2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsEven", x );
	}

	Bool3 IsEven (const UInt3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsEven", x );
	}

	Bool4 IsEven (const UInt4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsEven", x );
	}

	Bool  IsEven (const Long  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsEven", x );
	}

	Bool2 IsEven (const Long2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsEven", x );
	}

	Bool3 IsEven (const Long3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsEven", x );
	}

	Bool4 IsEven (const Long4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsEven", x );
	}

	Bool  IsEven (const ULong  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsEven", x );
	}

	Bool2 IsEven (const ULong2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsEven", x );
	}

	Bool3 IsEven (const ULong3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsEven", x );
	}

	Bool4 IsEven (const ULong4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsEven", x );
	}

	Bool  IsEven (const Float  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsEven", x );
	}

	Bool2 IsEven (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsEven", x );
	}

	Bool3 IsEven (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsEven", x );
	}

	Bool4 IsEven (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsEven", x );
	}

	Bool  IsEven (const Double  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsEven", x );
	}

	Bool2 IsEven (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsEven", x );
	}

	Bool3 IsEven (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsEven", x );
	}

	Bool4 IsEven (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsEven", x );
	}


	// Floor, Ceil, Trunc, Fract, Round, RoundEven, RoundToInt, RoundEvenToInt
	Float  Floor (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "Floor", x );
	}

	Float2 Floor (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Floor", x );
	}

	Float3 Floor (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Floor", x );
	}

	Float4 Floor (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Floor", x );
	}

	Double  Floor (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "Floor", x );
	}

	Double2 Floor (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Floor", x );
	}

	Double3 Floor (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Floor", x );
	}

	Double4 Floor (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Floor", x );
	}

	Float  Ceil (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "Ceil", x );
	}

	Float2 Ceil (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Ceil", x );
	}

	Float3 Ceil (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Ceil", x );
	}

	Float4 Ceil (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Ceil", x );
	}

	Double  Ceil (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "Ceil", x );
	}

	Double2 Ceil (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Ceil", x );
	}

	Double3 Ceil (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Ceil", x );
	}

	Double4 Ceil (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Ceil", x );
	}

	Float  Trunc (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "Trunc", x );
	}

	Float2 Trunc (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Trunc", x );
	}

	Float3 Trunc (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Trunc", x );
	}

	Float4 Trunc (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Trunc", x );
	}

	Double  Trunc (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "Trunc", x );
	}

	Double2 Trunc (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Trunc", x );
	}

	Double3 Trunc (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Trunc", x );
	}

	Double4 Trunc (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Trunc", x );
	}

	Float  Round (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "Round", x );
	}

	Float2 Round (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Round", x );
	}

	Float3 Round (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Round", x );
	}

	Float4 Round (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Round", x );
	}

	Double  Round (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "Round", x );
	}

	Double2 Round (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Round", x );
	}

	Double3 Round (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Round", x );
	}

	Double4 Round (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Round", x );
	}

	Float  RoundEven (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "RoundEven", x );
	}

	Float2 RoundEven (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "RoundEven", x );
	}

	Float3 RoundEven (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "RoundEven", x );
	}

	Float4 RoundEven (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "RoundEven", x );
	}

	Double  RoundEven (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "RoundEven", x );
	}

	Double2 RoundEven (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "RoundEven", x );
	}

	Double3 RoundEven (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "RoundEven", x );
	}

	Double4 RoundEven (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "RoundEven", x );
	}

	Int  RoundToInt (const Float  &x) {
		return NodeFunctions::Instance()->Build< Int  >( "RoundToInt", x );
	}

	Int2 RoundToInt (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "RoundToInt", x );
	}

	Int3 RoundToInt (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Int3 >( "RoundToInt", x );
	}

	Int4 RoundToInt (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "RoundToInt", x );
	}

	Long  RoundToInt (const Double  &x) {
		return NodeFunctions::Instance()->Build< Long  >( "RoundToInt", x );
	}

	Long2 RoundToInt (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "RoundToInt", x );
	}

	Long3 RoundToInt (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Long3 >( "RoundToInt", x );
	}

	Long4 RoundToInt (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Long4 >( "RoundToInt", x );
	}

	Int  RoundEvenToInt (const Float  &x) {
		return NodeFunctions::Instance()->Build< Int  >( "RoundEvenToInt", x );
	}

	Int2 RoundEvenToInt (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "RoundEvenToInt", x );
	}

	Int3 RoundEvenToInt (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Int3 >( "RoundEvenToInt", x );
	}

	Int4 RoundEvenToInt (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "RoundEvenToInt", x );
	}

	Long  RoundEvenToInt (const Double  &x) {
		return NodeFunctions::Instance()->Build< Long  >( "RoundEvenToInt", x );
	}

	Long2 RoundEvenToInt (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "RoundEvenToInt", x );
	}

	Long3 RoundEvenToInt (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Long3 >( "RoundEvenToInt", x );
	}

	Long4 RoundEvenToInt (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Long4 >( "RoundEvenToInt", x );
	}


	// Pow, Ln, Log, Log2, Log10, Exp, Exp2, Exp10, RoundTo
	Float  Ln (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "Ln", x );
	}

	Float2 Ln (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Ln", x );
	}

	Float3 Ln (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Ln", x );
	}

	Float4 Ln (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Ln", x );
	}

	Double  Ln (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "Ln", x );
	}

	Double2 Ln (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Ln", x );
	}

	Double3 Ln (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Ln", x );
	}

	Double4 Ln (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Ln", x );
	}

	Float  Log2 (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "Log2", x );
	}

	Float2 Log2 (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Log2", x );
	}

	Float3 Log2 (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Log2", x );
	}

	Float4 Log2 (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Log2", x );
	}

	Double  Log2 (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "Log2", x );
	}

	Double2 Log2 (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Log2", x );
	}

	Double3 Log2 (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Log2", x );
	}

	Double4 Log2 (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Log2", x );
	}

	Float  Log10 (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "Log10", x );
	}

	Float2 Log10 (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Log10", x );
	}

	Float3 Log10 (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Log10", x );
	}

	Float4 Log10 (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Log10", x );
	}

	Double  Log10 (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "Log10", x );
	}

	Double2 Log10 (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Log10", x );
	}

	Double3 Log10 (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Log10", x );
	}

	Double4 Log10 (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Log10", x );
	}

	Float  Exp (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "Exp", x );
	}

	Float2 Exp (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Exp", x );
	}

	Float3 Exp (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Exp", x );
	}

	Float4 Exp (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Exp", x );
	}

	Double  Exp (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "Exp", x );
	}

	Double2 Exp (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Exp", x );
	}

	Double3 Exp (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Exp", x );
	}

	Double4 Exp (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Exp", x );
	}

	Float  Exp2 (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "Exp2", x );
	}

	Float2 Exp2 (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Exp2", x );
	}

	Float3 Exp2 (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Exp2", x );
	}

	Float4 Exp2 (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Exp2", x );
	}

	Double  Exp2 (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "Exp2", x );
	}

	Double2 Exp2 (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Exp2", x );
	}

	Double3 Exp2 (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Exp2", x );
	}

	Double4 Exp2 (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Exp2", x );
	}

	Float  Exp10 (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "Exp10", x );
	}

	Float2 Exp10 (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Exp10", x );
	}

	Float3 Exp10 (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Exp10", x );
	}

	Float4 Exp10 (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Exp10", x );
	}

	Double  Exp10 (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "Exp10", x );
	}

	Double2 Exp10 (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Exp10", x );
	}

	Double3 Exp10 (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Exp10", x );
	}

	Double4 Exp10 (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Exp10", x );
	}

	Float  Pow (const Float  &x, const Float  &base) {
		return NodeFunctions::Instance()->Build< Float  >( "Pow", x, base );
	}

	Float2 Pow (const Float2 &x, const Float2 &base) {
		return NodeFunctions::Instance()->Build< Float2 >( "Pow", x, base );
	}

	Float3 Pow (const Float3 &x, const Float3 &base) {
		return NodeFunctions::Instance()->Build< Float3 >( "Pow", x, base );
	}

	Float4 Pow (const Float4 &x, const Float4 &base) {
		return NodeFunctions::Instance()->Build< Float4 >( "Pow", x, base );
	}

	Double  Pow (const Double  &x, const Double  &base) {
		return NodeFunctions::Instance()->Build< Double  >( "Pow", x, base );
	}

	Double2 Pow (const Double2 &x, const Double2 &base) {
		return NodeFunctions::Instance()->Build< Double2 >( "Pow", x, base );
	}

	Double3 Pow (const Double3 &x, const Double3 &base) {
		return NodeFunctions::Instance()->Build< Double3 >( "Pow", x, base );
	}

	Double4 Pow (const Double4 &x, const Double4 &base) {
		return NodeFunctions::Instance()->Build< Double4 >( "Pow", x, base );
	}

	Float  Log (const Float  &x, const Float  &base) {
		return NodeFunctions::Instance()->Build< Float  >( "Log", x, base );
	}

	Float2 Log (const Float2 &x, const Float2 &base) {
		return NodeFunctions::Instance()->Build< Float2 >( "Log", x, base );
	}

	Float3 Log (const Float3 &x, const Float3 &base) {
		return NodeFunctions::Instance()->Build< Float3 >( "Log", x, base );
	}

	Float4 Log (const Float4 &x, const Float4 &base) {
		return NodeFunctions::Instance()->Build< Float4 >( "Log", x, base );
	}

	Double  Log (const Double  &x, const Double  &base) {
		return NodeFunctions::Instance()->Build< Double  >( "Log", x, base );
	}

	Double2 Log (const Double2 &x, const Double2 &base) {
		return NodeFunctions::Instance()->Build< Double2 >( "Log", x, base );
	}

	Double3 Log (const Double3 &x, const Double3 &base) {
		return NodeFunctions::Instance()->Build< Double3 >( "Log", x, base );
	}

	Double4 Log (const Double4 &x, const Double4 &base) {
		return NodeFunctions::Instance()->Build< Double4 >( "Log", x, base );
	}

	Float  RoundTo (const Float  &x, const Float  &base) {
		return NodeFunctions::Instance()->Build< Float  >( "RoundTo", x, base );
	}

	Float2 RoundTo (const Float2 &x, const Float2 &base) {
		return NodeFunctions::Instance()->Build< Float2 >( "RoundTo", x, base );
	}

	Float3 RoundTo (const Float3 &x, const Float3 &base) {
		return NodeFunctions::Instance()->Build< Float3 >( "RoundTo", x, base );
	}

	Float4 RoundTo (const Float4 &x, const Float4 &base) {
		return NodeFunctions::Instance()->Build< Float4 >( "RoundTo", x, base );
	}

	Double  RoundTo (const Double  &x, const Double  &base) {
		return NodeFunctions::Instance()->Build< Double  >( "RoundTo", x, base );
	}

	Double2 RoundTo (const Double2 &x, const Double2 &base) {
		return NodeFunctions::Instance()->Build< Double2 >( "RoundTo", x, base );
	}

	Double3 RoundTo (const Double3 &x, const Double3 &base) {
		return NodeFunctions::Instance()->Build< Double3 >( "RoundTo", x, base );
	}

	Double4 RoundTo (const Double4 &x, const Double4 &base) {
		return NodeFunctions::Instance()->Build< Double4 >( "RoundTo", x, base );
	}


	// IntLog2, CeilPowerOfTwo, FloorPowerOfTwo, NearPowerOfTwo, IsPowerOfTwo
	Int  CeilPowerOfTwo (const Int  &x) {
		return NodeFunctions::Instance()->Build< Int  >( "CeilPowerOfTwo", x );
	}

	Int2 CeilPowerOfTwo (const Int2 &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "CeilPowerOfTwo", x );
	}

	Int3 CeilPowerOfTwo (const Int3 &x) {
		return NodeFunctions::Instance()->Build< Int3 >( "CeilPowerOfTwo", x );
	}

	Int4 CeilPowerOfTwo (const Int4 &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "CeilPowerOfTwo", x );
	}

	UInt  CeilPowerOfTwo (const UInt  &x) {
		return NodeFunctions::Instance()->Build< UInt  >( "CeilPowerOfTwo", x );
	}

	UInt2 CeilPowerOfTwo (const UInt2 &x) {
		return NodeFunctions::Instance()->Build< UInt2 >( "CeilPowerOfTwo", x );
	}

	UInt3 CeilPowerOfTwo (const UInt3 &x) {
		return NodeFunctions::Instance()->Build< UInt3 >( "CeilPowerOfTwo", x );
	}

	UInt4 CeilPowerOfTwo (const UInt4 &x) {
		return NodeFunctions::Instance()->Build< UInt4 >( "CeilPowerOfTwo", x );
	}

	Long  CeilPowerOfTwo (const Long  &x) {
		return NodeFunctions::Instance()->Build< Long  >( "CeilPowerOfTwo", x );
	}

	Long2 CeilPowerOfTwo (const Long2 &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "CeilPowerOfTwo", x );
	}

	Long3 CeilPowerOfTwo (const Long3 &x) {
		return NodeFunctions::Instance()->Build< Long3 >( "CeilPowerOfTwo", x );
	}

	Long4 CeilPowerOfTwo (const Long4 &x) {
		return NodeFunctions::Instance()->Build< Long4 >( "CeilPowerOfTwo", x );
	}

	ULong  CeilPowerOfTwo (const ULong  &x) {
		return NodeFunctions::Instance()->Build< ULong  >( "CeilPowerOfTwo", x );
	}

	ULong2 CeilPowerOfTwo (const ULong2 &x) {
		return NodeFunctions::Instance()->Build< ULong2 >( "CeilPowerOfTwo", x );
	}

	ULong3 CeilPowerOfTwo (const ULong3 &x) {
		return NodeFunctions::Instance()->Build< ULong3 >( "CeilPowerOfTwo", x );
	}

	ULong4 CeilPowerOfTwo (const ULong4 &x) {
		return NodeFunctions::Instance()->Build< ULong4 >( "CeilPowerOfTwo", x );
	}

	Int  FloorPowerOfTwo (const Int  &x) {
		return NodeFunctions::Instance()->Build< Int  >( "FloorPowerOfTwo", x );
	}

	Int2 FloorPowerOfTwo (const Int2 &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "FloorPowerOfTwo", x );
	}

	Int3 FloorPowerOfTwo (const Int3 &x) {
		return NodeFunctions::Instance()->Build< Int3 >( "FloorPowerOfTwo", x );
	}

	Int4 FloorPowerOfTwo (const Int4 &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "FloorPowerOfTwo", x );
	}

	UInt  FloorPowerOfTwo (const UInt  &x) {
		return NodeFunctions::Instance()->Build< UInt  >( "FloorPowerOfTwo", x );
	}

	UInt2 FloorPowerOfTwo (const UInt2 &x) {
		return NodeFunctions::Instance()->Build< UInt2 >( "FloorPowerOfTwo", x );
	}

	UInt3 FloorPowerOfTwo (const UInt3 &x) {
		return NodeFunctions::Instance()->Build< UInt3 >( "FloorPowerOfTwo", x );
	}

	UInt4 FloorPowerOfTwo (const UInt4 &x) {
		return NodeFunctions::Instance()->Build< UInt4 >( "FloorPowerOfTwo", x );
	}

	Long  FloorPowerOfTwo (const Long  &x) {
		return NodeFunctions::Instance()->Build< Long  >( "FloorPowerOfTwo", x );
	}

	Long2 FloorPowerOfTwo (const Long2 &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "FloorPowerOfTwo", x );
	}

	Long3 FloorPowerOfTwo (const Long3 &x) {
		return NodeFunctions::Instance()->Build< Long3 >( "FloorPowerOfTwo", x );
	}

	Long4 FloorPowerOfTwo (const Long4 &x) {
		return NodeFunctions::Instance()->Build< Long4 >( "FloorPowerOfTwo", x );
	}

	ULong  FloorPowerOfTwo (const ULong  &x) {
		return NodeFunctions::Instance()->Build< ULong  >( "FloorPowerOfTwo", x );
	}

	ULong2 FloorPowerOfTwo (const ULong2 &x) {
		return NodeFunctions::Instance()->Build< ULong2 >( "FloorPowerOfTwo", x );
	}

	ULong3 FloorPowerOfTwo (const ULong3 &x) {
		return NodeFunctions::Instance()->Build< ULong3 >( "FloorPowerOfTwo", x );
	}

	ULong4 FloorPowerOfTwo (const ULong4 &x) {
		return NodeFunctions::Instance()->Build< ULong4 >( "FloorPowerOfTwo", x );
	}

	Int  NearPowerOfTwo (const Int  &x) {
		return NodeFunctions::Instance()->Build< Int  >( "NearPowerOfTwo", x );
	}

	Int2 NearPowerOfTwo (const Int2 &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "NearPowerOfTwo", x );
	}

	Int3 NearPowerOfTwo (const Int3 &x) {
		return NodeFunctions::Instance()->Build< Int3 >( "NearPowerOfTwo", x );
	}

	Int4 NearPowerOfTwo (const Int4 &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "NearPowerOfTwo", x );
	}

	UInt  NearPowerOfTwo (const UInt  &x) {
		return NodeFunctions::Instance()->Build< UInt  >( "NearPowerOfTwo", x );
	}

	UInt2 NearPowerOfTwo (const UInt2 &x) {
		return NodeFunctions::Instance()->Build< UInt2 >( "NearPowerOfTwo", x );
	}

	UInt3 NearPowerOfTwo (const UInt3 &x) {
		return NodeFunctions::Instance()->Build< UInt3 >( "NearPowerOfTwo", x );
	}

	UInt4 NearPowerOfTwo (const UInt4 &x) {
		return NodeFunctions::Instance()->Build< UInt4 >( "NearPowerOfTwo", x );
	}

	Long  NearPowerOfTwo (const Long  &x) {
		return NodeFunctions::Instance()->Build< Long  >( "NearPowerOfTwo", x );
	}

	Long2 NearPowerOfTwo (const Long2 &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "NearPowerOfTwo", x );
	}

	Long3 NearPowerOfTwo (const Long3 &x) {
		return NodeFunctions::Instance()->Build< Long3 >( "NearPowerOfTwo", x );
	}

	Long4 NearPowerOfTwo (const Long4 &x) {
		return NodeFunctions::Instance()->Build< Long4 >( "NearPowerOfTwo", x );
	}

	ULong  NearPowerOfTwo (const ULong  &x) {
		return NodeFunctions::Instance()->Build< ULong  >( "NearPowerOfTwo", x );
	}

	ULong2 NearPowerOfTwo (const ULong2 &x) {
		return NodeFunctions::Instance()->Build< ULong2 >( "NearPowerOfTwo", x );
	}

	ULong3 NearPowerOfTwo (const ULong3 &x) {
		return NodeFunctions::Instance()->Build< ULong3 >( "NearPowerOfTwo", x );
	}

	ULong4 NearPowerOfTwo (const ULong4 &x) {
		return NodeFunctions::Instance()->Build< ULong4 >( "NearPowerOfTwo", x );
	}

	Bool  IsPowerOfTwo (const Int  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsPowerOfTwo", x );
	}

	Bool2 IsPowerOfTwo (const Int2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsPowerOfTwo", x );
	}

	Bool3 IsPowerOfTwo (const Int3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsPowerOfTwo", x );
	}

	Bool4 IsPowerOfTwo (const Int4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsPowerOfTwo", x );
	}

	Bool  IsPowerOfTwo (const UInt  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsPowerOfTwo", x );
	}

	Bool2 IsPowerOfTwo (const UInt2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsPowerOfTwo", x );
	}

	Bool3 IsPowerOfTwo (const UInt3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsPowerOfTwo", x );
	}

	Bool4 IsPowerOfTwo (const UInt4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsPowerOfTwo", x );
	}

	Bool  IsPowerOfTwo (const Long  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsPowerOfTwo", x );
	}

	Bool2 IsPowerOfTwo (const Long2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsPowerOfTwo", x );
	}

	Bool3 IsPowerOfTwo (const Long3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsPowerOfTwo", x );
	}

	Bool4 IsPowerOfTwo (const Long4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsPowerOfTwo", x );
	}

	Bool  IsPowerOfTwo (const ULong  &x) {
		return NodeFunctions::Instance()->Build< Bool  >( "IsPowerOfTwo", x );
	}

	Bool2 IsPowerOfTwo (const ULong2 &x) {
		return NodeFunctions::Instance()->Build< Bool2 >( "IsPowerOfTwo", x );
	}

	Bool3 IsPowerOfTwo (const ULong3 &x) {
		return NodeFunctions::Instance()->Build< Bool3 >( "IsPowerOfTwo", x );
	}

	Bool4 IsPowerOfTwo (const ULong4 &x) {
		return NodeFunctions::Instance()->Build< Bool4 >( "IsPowerOfTwo", x );
	}


	// Sqrt, InvSqrt
	Float  Sqrt (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "Sqrt", x );
	}

	Float2 Sqrt (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Sqrt", x );
	}

	Float3 Sqrt (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Sqrt", x );
	}

	Float4 Sqrt (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Sqrt", x );
	}

	Double  Sqrt (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "Sqrt", x );
	}

	Double2 Sqrt (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Sqrt", x );
	}

	Double3 Sqrt (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Sqrt", x );
	}

	Double4 Sqrt (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Sqrt", x );
	}

	Float  InvSqrt (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "InvSqrt", x );
	}

	Float2 InvSqrt (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "InvSqrt", x );
	}

	Float3 InvSqrt (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "InvSqrt", x );
	}

	Float4 InvSqrt (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "InvSqrt", x );
	}

	Double  InvSqrt (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "InvSqrt", x );
	}

	Double2 InvSqrt (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "InvSqrt", x );
	}

	Double3 InvSqrt (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "InvSqrt", x );
	}

	Double4 InvSqrt (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "InvSqrt", x );
	}


	// Lerp, InvLerp, BiLerp
	Float  Lerp (const Float  &x, const Float  &y, const Float  &factor) {
		return NodeFunctions::Instance()->Build< Float  >( "Lerp", x, y, factor );
	}

	Float  InvLerp (const Float  &x, const Float  &y, const Float  &interpolated) {
		return NodeFunctions::Instance()->Build< Float  >( "InvLerp", x, y, interpolated );
	}

	Float  BiLerp (const Float  &v0, const Float  &v1, const Float  &v2, const Float  &v3, const Float &factorX, const Float &factorY) {
		return NodeFunctions::Instance()->Build< Float  >( "BiLerp", v0, v1, v2, v3, factorX, factorY );
	}

	Float2 Lerp (const Float2 &x, const Float2 &y, const Float2 &factor) {
		return NodeFunctions::Instance()->Build< Float2 >( "Lerp", x, y, factor );
	}

	Float2 Lerp (const Float2 &x, const Float2 &y, const Float &factor) {
		return NodeFunctions::Instance()->Build< Float2 >( "Lerp", x, y, factor );
	}

	Float2 InvLerp (const Float2 &x, const Float2 &y, const Float2 &interpolated) {
		return NodeFunctions::Instance()->Build< Float2 >( "InvLerp", x, y, interpolated );
	}

	Float2 BiLerp (const Float2 &v0, const Float2 &v1, const Float2 &v2, const Float2 &v3, const Float &factorX, const Float &factorY) {
		return NodeFunctions::Instance()->Build< Float2 >( "BiLerp", v0, v1, v2, v3, factorX, factorY );
	}

	Float3 Lerp (const Float3 &x, const Float3 &y, const Float3 &factor) {
		return NodeFunctions::Instance()->Build< Float3 >( "Lerp", x, y, factor );
	}

	Float3 Lerp (const Float3 &x, const Float3 &y, const Float &factor) {
		return NodeFunctions::Instance()->Build< Float3 >( "Lerp", x, y, factor );
	}

	Float3 InvLerp (const Float3 &x, const Float3 &y, const Float3 &interpolated) {
		return NodeFunctions::Instance()->Build< Float3 >( "InvLerp", x, y, interpolated );
	}

	Float3 BiLerp (const Float3 &v0, const Float3 &v1, const Float3 &v2, const Float3 &v3, const Float &factorX, const Float &factorY) {
		return NodeFunctions::Instance()->Build< Float3 >( "BiLerp", v0, v1, v2, v3, factorX, factorY );
	}

	Float4 Lerp (const Float4 &x, const Float4 &y, const Float4 &factor) {
		return NodeFunctions::Instance()->Build< Float4 >( "Lerp", x, y, factor );
	}

	Float4 Lerp (const Float4 &x, const Float4 &y, const Float &factor) {
		return NodeFunctions::Instance()->Build< Float4 >( "Lerp", x, y, factor );
	}

	Float4 InvLerp (const Float4 &x, const Float4 &y, const Float4 &interpolated) {
		return NodeFunctions::Instance()->Build< Float4 >( "InvLerp", x, y, interpolated );
	}

	Float4 BiLerp (const Float4 &v0, const Float4 &v1, const Float4 &v2, const Float4 &v3, const Float &factorX, const Float &factorY) {
		return NodeFunctions::Instance()->Build< Float4 >( "BiLerp", v0, v1, v2, v3, factorX, factorY );
	}

	Double  Lerp (const Double  &x, const Double  &y, const Double  &factor) {
		return NodeFunctions::Instance()->Build< Double  >( "Lerp", x, y, factor );
	}

	Double  InvLerp (const Double  &x, const Double  &y, const Double  &interpolated) {
		return NodeFunctions::Instance()->Build< Double  >( "InvLerp", x, y, interpolated );
	}

	Double  BiLerp (const Double  &v0, const Double  &v1, const Double  &v2, const Double  &v3, const Double &factorX, const Double &factorY) {
		return NodeFunctions::Instance()->Build< Double  >( "BiLerp", v0, v1, v2, v3, factorX, factorY );
	}

	Double2 Lerp (const Double2 &x, const Double2 &y, const Double2 &factor) {
		return NodeFunctions::Instance()->Build< Double2 >( "Lerp", x, y, factor );
	}

	Double2 Lerp (const Double2 &x, const Double2 &y, const Double &factor) {
		return NodeFunctions::Instance()->Build< Double2 >( "Lerp", x, y, factor );
	}

	Double2 InvLerp (const Double2 &x, const Double2 &y, const Double2 &interpolated) {
		return NodeFunctions::Instance()->Build< Double2 >( "InvLerp", x, y, interpolated );
	}

	Double2 BiLerp (const Double2 &v0, const Double2 &v1, const Double2 &v2, const Double2 &v3, const Double &factorX, const Double &factorY) {
		return NodeFunctions::Instance()->Build< Double2 >( "BiLerp", v0, v1, v2, v3, factorX, factorY );
	}

	Double3 Lerp (const Double3 &x, const Double3 &y, const Double3 &factor) {
		return NodeFunctions::Instance()->Build< Double3 >( "Lerp", x, y, factor );
	}

	Double3 Lerp (const Double3 &x, const Double3 &y, const Double &factor) {
		return NodeFunctions::Instance()->Build< Double3 >( "Lerp", x, y, factor );
	}

	Double3 InvLerp (const Double3 &x, const Double3 &y, const Double3 &interpolated) {
		return NodeFunctions::Instance()->Build< Double3 >( "InvLerp", x, y, interpolated );
	}

	Double3 BiLerp (const Double3 &v0, const Double3 &v1, const Double3 &v2, const Double3 &v3, const Double &factorX, const Double &factorY) {
		return NodeFunctions::Instance()->Build< Double3 >( "BiLerp", v0, v1, v2, v3, factorX, factorY );
	}

	Double4 Lerp (const Double4 &x, const Double4 &y, const Double4 &factor) {
		return NodeFunctions::Instance()->Build< Double4 >( "Lerp", x, y, factor );
	}

	Double4 Lerp (const Double4 &x, const Double4 &y, const Double &factor) {
		return NodeFunctions::Instance()->Build< Double4 >( "Lerp", x, y, factor );
	}

	Double4 InvLerp (const Double4 &x, const Double4 &y, const Double4 &interpolated) {
		return NodeFunctions::Instance()->Build< Double4 >( "InvLerp", x, y, interpolated );
	}

	Double4 BiLerp (const Double4 &v0, const Double4 &v1, const Double4 &v2, const Double4 &v3, const Double &factorX, const Double &factorY) {
		return NodeFunctions::Instance()->Build< Double4 >( "BiLerp", v0, v1, v2, v3, factorX, factorY );
	}


	// AlignToLarge
	Int  AlignToLarge (const Int  &x, const Int &align) {
		return NodeFunctions::Instance()->Build< Int  >( "AlignToLarge", x, align );
	}

	Int2 AlignToLarge (const Int2 &x, const Int &align) {
		return NodeFunctions::Instance()->Build< Int2 >( "AlignToLarge", x, align );
	}

	Int3 AlignToLarge (const Int3 &x, const Int &align) {
		return NodeFunctions::Instance()->Build< Int3 >( "AlignToLarge", x, align );
	}

	Int4 AlignToLarge (const Int4 &x, const Int &align) {
		return NodeFunctions::Instance()->Build< Int4 >( "AlignToLarge", x, align );
	}

	UInt  AlignToLarge (const UInt  &x, const Int &align) {
		return NodeFunctions::Instance()->Build< UInt  >( "AlignToLarge", x, align );
	}

	UInt2 AlignToLarge (const UInt2 &x, const Int &align) {
		return NodeFunctions::Instance()->Build< UInt2 >( "AlignToLarge", x, align );
	}

	UInt3 AlignToLarge (const UInt3 &x, const Int &align) {
		return NodeFunctions::Instance()->Build< UInt3 >( "AlignToLarge", x, align );
	}

	UInt4 AlignToLarge (const UInt4 &x, const Int &align) {
		return NodeFunctions::Instance()->Build< UInt4 >( "AlignToLarge", x, align );
	}

	Long  AlignToLarge (const Long  &x, const Int &align) {
		return NodeFunctions::Instance()->Build< Long  >( "AlignToLarge", x, align );
	}

	Long2 AlignToLarge (const Long2 &x, const Int &align) {
		return NodeFunctions::Instance()->Build< Long2 >( "AlignToLarge", x, align );
	}

	Long3 AlignToLarge (const Long3 &x, const Int &align) {
		return NodeFunctions::Instance()->Build< Long3 >( "AlignToLarge", x, align );
	}

	Long4 AlignToLarge (const Long4 &x, const Int &align) {
		return NodeFunctions::Instance()->Build< Long4 >( "AlignToLarge", x, align );
	}

	ULong  AlignToLarge (const ULong  &x, const Int &align) {
		return NodeFunctions::Instance()->Build< ULong  >( "AlignToLarge", x, align );
	}

	ULong2 AlignToLarge (const ULong2 &x, const Int &align) {
		return NodeFunctions::Instance()->Build< ULong2 >( "AlignToLarge", x, align );
	}

	ULong3 AlignToLarge (const ULong3 &x, const Int &align) {
		return NodeFunctions::Instance()->Build< ULong3 >( "AlignToLarge", x, align );
	}

	ULong4 AlignToLarge (const ULong4 &x, const Int &align) {
		return NodeFunctions::Instance()->Build< ULong4 >( "AlignToLarge", x, align );
	}


	// Step, LinearStep, SmoothStep, BumpStep, SmoothBumpStep
	Float  Step (const Float  &x, const Float  &edge0, const Float  &edge1) {
		return NodeFunctions::Instance()->Build< Float  >( "Step", x, edge0, edge1 );
	}

	Float2 Step (const Float2 &x, const Float2 &edge0, const Float2 &edge1) {
		return NodeFunctions::Instance()->Build< Float2 >( "Step", x, edge0, edge1 );
	}

	Float2 Step (const Float2 &x, const Float &edge0, const Float &edge1) {
		return NodeFunctions::Instance()->Build< Float2 >( "Step", x, edge0, edge1 );
	}

	Float3 Step (const Float3 &x, const Float3 &edge0, const Float3 &edge1) {
		return NodeFunctions::Instance()->Build< Float3 >( "Step", x, edge0, edge1 );
	}

	Float3 Step (const Float3 &x, const Float &edge0, const Float &edge1) {
		return NodeFunctions::Instance()->Build< Float3 >( "Step", x, edge0, edge1 );
	}

	Float4 Step (const Float4 &x, const Float4 &edge0, const Float4 &edge1) {
		return NodeFunctions::Instance()->Build< Float4 >( "Step", x, edge0, edge1 );
	}

	Float4 Step (const Float4 &x, const Float &edge0, const Float &edge1) {
		return NodeFunctions::Instance()->Build< Float4 >( "Step", x, edge0, edge1 );
	}

	Double  Step (const Double  &x, const Double  &edge0, const Double  &edge1) {
		return NodeFunctions::Instance()->Build< Double  >( "Step", x, edge0, edge1 );
	}

	Double2 Step (const Double2 &x, const Double2 &edge0, const Double2 &edge1) {
		return NodeFunctions::Instance()->Build< Double2 >( "Step", x, edge0, edge1 );
	}

	Double2 Step (const Double2 &x, const Double &edge0, const Double &edge1) {
		return NodeFunctions::Instance()->Build< Double2 >( "Step", x, edge0, edge1 );
	}

	Double3 Step (const Double3 &x, const Double3 &edge0, const Double3 &edge1) {
		return NodeFunctions::Instance()->Build< Double3 >( "Step", x, edge0, edge1 );
	}

	Double3 Step (const Double3 &x, const Double &edge0, const Double &edge1) {
		return NodeFunctions::Instance()->Build< Double3 >( "Step", x, edge0, edge1 );
	}

	Double4 Step (const Double4 &x, const Double4 &edge0, const Double4 &edge1) {
		return NodeFunctions::Instance()->Build< Double4 >( "Step", x, edge0, edge1 );
	}

	Double4 Step (const Double4 &x, const Double &edge0, const Double &edge1) {
		return NodeFunctions::Instance()->Build< Double4 >( "Step", x, edge0, edge1 );
	}

	Float  LinearStep (const Float  &x, const Float  &edge0, const Float  &edge1) {
		return NodeFunctions::Instance()->Build< Float  >( "LinearStep", x, edge0, edge1 );
	}

	Float2 LinearStep (const Float2 &x, const Float2 &edge0, const Float2 &edge1) {
		return NodeFunctions::Instance()->Build< Float2 >( "LinearStep", x, edge0, edge1 );
	}

	Float2 LinearStep (const Float2 &x, const Float &edge0, const Float &edge1) {
		return NodeFunctions::Instance()->Build< Float2 >( "LinearStep", x, edge0, edge1 );
	}

	Float3 LinearStep (const Float3 &x, const Float3 &edge0, const Float3 &edge1) {
		return NodeFunctions::Instance()->Build< Float3 >( "LinearStep", x, edge0, edge1 );
	}

	Float3 LinearStep (const Float3 &x, const Float &edge0, const Float &edge1) {
		return NodeFunctions::Instance()->Build< Float3 >( "LinearStep", x, edge0, edge1 );
	}

	Float4 LinearStep (const Float4 &x, const Float4 &edge0, const Float4 &edge1) {
		return NodeFunctions::Instance()->Build< Float4 >( "LinearStep", x, edge0, edge1 );
	}

	Float4 LinearStep (const Float4 &x, const Float &edge0, const Float &edge1) {
		return NodeFunctions::Instance()->Build< Float4 >( "LinearStep", x, edge0, edge1 );
	}

	Double  LinearStep (const Double  &x, const Double  &edge0, const Double  &edge1) {
		return NodeFunctions::Instance()->Build< Double  >( "LinearStep", x, edge0, edge1 );
	}

	Double2 LinearStep (const Double2 &x, const Double2 &edge0, const Double2 &edge1) {
		return NodeFunctions::Instance()->Build< Double2 >( "LinearStep", x, edge0, edge1 );
	}

	Double2 LinearStep (const Double2 &x, const Double &edge0, const Double &edge1) {
		return NodeFunctions::Instance()->Build< Double2 >( "LinearStep", x, edge0, edge1 );
	}

	Double3 LinearStep (const Double3 &x, const Double3 &edge0, const Double3 &edge1) {
		return NodeFunctions::Instance()->Build< Double3 >( "LinearStep", x, edge0, edge1 );
	}

	Double3 LinearStep (const Double3 &x, const Double &edge0, const Double &edge1) {
		return NodeFunctions::Instance()->Build< Double3 >( "LinearStep", x, edge0, edge1 );
	}

	Double4 LinearStep (const Double4 &x, const Double4 &edge0, const Double4 &edge1) {
		return NodeFunctions::Instance()->Build< Double4 >( "LinearStep", x, edge0, edge1 );
	}

	Double4 LinearStep (const Double4 &x, const Double &edge0, const Double &edge1) {
		return NodeFunctions::Instance()->Build< Double4 >( "LinearStep", x, edge0, edge1 );
	}

	Float  SmoothStep (const Float  &x, const Float  &edge0, const Float  &edge1) {
		return NodeFunctions::Instance()->Build< Float  >( "SmoothStep", x, edge0, edge1 );
	}

	Float2 SmoothStep (const Float2 &x, const Float2 &edge0, const Float2 &edge1) {
		return NodeFunctions::Instance()->Build< Float2 >( "SmoothStep", x, edge0, edge1 );
	}

	Float2 SmoothStep (const Float2 &x, const Float &edge0, const Float &edge1) {
		return NodeFunctions::Instance()->Build< Float2 >( "SmoothStep", x, edge0, edge1 );
	}

	Float3 SmoothStep (const Float3 &x, const Float3 &edge0, const Float3 &edge1) {
		return NodeFunctions::Instance()->Build< Float3 >( "SmoothStep", x, edge0, edge1 );
	}

	Float3 SmoothStep (const Float3 &x, const Float &edge0, const Float &edge1) {
		return NodeFunctions::Instance()->Build< Float3 >( "SmoothStep", x, edge0, edge1 );
	}

	Float4 SmoothStep (const Float4 &x, const Float4 &edge0, const Float4 &edge1) {
		return NodeFunctions::Instance()->Build< Float4 >( "SmoothStep", x, edge0, edge1 );
	}

	Float4 SmoothStep (const Float4 &x, const Float &edge0, const Float &edge1) {
		return NodeFunctions::Instance()->Build< Float4 >( "SmoothStep", x, edge0, edge1 );
	}

	Double  SmoothStep (const Double  &x, const Double  &edge0, const Double  &edge1) {
		return NodeFunctions::Instance()->Build< Double  >( "SmoothStep", x, edge0, edge1 );
	}

	Double2 SmoothStep (const Double2 &x, const Double2 &edge0, const Double2 &edge1) {
		return NodeFunctions::Instance()->Build< Double2 >( "SmoothStep", x, edge0, edge1 );
	}

	Double2 SmoothStep (const Double2 &x, const Double &edge0, const Double &edge1) {
		return NodeFunctions::Instance()->Build< Double2 >( "SmoothStep", x, edge0, edge1 );
	}

	Double3 SmoothStep (const Double3 &x, const Double3 &edge0, const Double3 &edge1) {
		return NodeFunctions::Instance()->Build< Double3 >( "SmoothStep", x, edge0, edge1 );
	}

	Double3 SmoothStep (const Double3 &x, const Double &edge0, const Double &edge1) {
		return NodeFunctions::Instance()->Build< Double3 >( "SmoothStep", x, edge0, edge1 );
	}

	Double4 SmoothStep (const Double4 &x, const Double4 &edge0, const Double4 &edge1) {
		return NodeFunctions::Instance()->Build< Double4 >( "SmoothStep", x, edge0, edge1 );
	}

	Double4 SmoothStep (const Double4 &x, const Double &edge0, const Double &edge1) {
		return NodeFunctions::Instance()->Build< Double4 >( "SmoothStep", x, edge0, edge1 );
	}

	Float  BumpStep (const Float  &x, const Float  &edge0, const Float  &edge1) {
		return NodeFunctions::Instance()->Build< Float  >( "BumpStep", x, edge0, edge1 );
	}

	Float2 BumpStep (const Float2 &x, const Float2 &edge0, const Float2 &edge1) {
		return NodeFunctions::Instance()->Build< Float2 >( "BumpStep", x, edge0, edge1 );
	}

	Float2 BumpStep (const Float2 &x, const Float &edge0, const Float &edge1) {
		return NodeFunctions::Instance()->Build< Float2 >( "BumpStep", x, edge0, edge1 );
	}

	Float3 BumpStep (const Float3 &x, const Float3 &edge0, const Float3 &edge1) {
		return NodeFunctions::Instance()->Build< Float3 >( "BumpStep", x, edge0, edge1 );
	}

	Float3 BumpStep (const Float3 &x, const Float &edge0, const Float &edge1) {
		return NodeFunctions::Instance()->Build< Float3 >( "BumpStep", x, edge0, edge1 );
	}

	Float4 BumpStep (const Float4 &x, const Float4 &edge0, const Float4 &edge1) {
		return NodeFunctions::Instance()->Build< Float4 >( "BumpStep", x, edge0, edge1 );
	}

	Float4 BumpStep (const Float4 &x, const Float &edge0, const Float &edge1) {
		return NodeFunctions::Instance()->Build< Float4 >( "BumpStep", x, edge0, edge1 );
	}

	Double  BumpStep (const Double  &x, const Double  &edge0, const Double  &edge1) {
		return NodeFunctions::Instance()->Build< Double  >( "BumpStep", x, edge0, edge1 );
	}

	Double2 BumpStep (const Double2 &x, const Double2 &edge0, const Double2 &edge1) {
		return NodeFunctions::Instance()->Build< Double2 >( "BumpStep", x, edge0, edge1 );
	}

	Double2 BumpStep (const Double2 &x, const Double &edge0, const Double &edge1) {
		return NodeFunctions::Instance()->Build< Double2 >( "BumpStep", x, edge0, edge1 );
	}

	Double3 BumpStep (const Double3 &x, const Double3 &edge0, const Double3 &edge1) {
		return NodeFunctions::Instance()->Build< Double3 >( "BumpStep", x, edge0, edge1 );
	}

	Double3 BumpStep (const Double3 &x, const Double &edge0, const Double &edge1) {
		return NodeFunctions::Instance()->Build< Double3 >( "BumpStep", x, edge0, edge1 );
	}

	Double4 BumpStep (const Double4 &x, const Double4 &edge0, const Double4 &edge1) {
		return NodeFunctions::Instance()->Build< Double4 >( "BumpStep", x, edge0, edge1 );
	}

	Double4 BumpStep (const Double4 &x, const Double &edge0, const Double &edge1) {
		return NodeFunctions::Instance()->Build< Double4 >( "BumpStep", x, edge0, edge1 );
	}

	Float  SmoothBumpStep (const Float  &x, const Float  &edge0, const Float  &edge1) {
		return NodeFunctions::Instance()->Build< Float  >( "SmoothBumpStep", x, edge0, edge1 );
	}

	Float2 SmoothBumpStep (const Float2 &x, const Float2 &edge0, const Float2 &edge1) {
		return NodeFunctions::Instance()->Build< Float2 >( "SmoothBumpStep", x, edge0, edge1 );
	}

	Float2 SmoothBumpStep (const Float2 &x, const Float &edge0, const Float &edge1) {
		return NodeFunctions::Instance()->Build< Float2 >( "SmoothBumpStep", x, edge0, edge1 );
	}

	Float3 SmoothBumpStep (const Float3 &x, const Float3 &edge0, const Float3 &edge1) {
		return NodeFunctions::Instance()->Build< Float3 >( "SmoothBumpStep", x, edge0, edge1 );
	}

	Float3 SmoothBumpStep (const Float3 &x, const Float &edge0, const Float &edge1) {
		return NodeFunctions::Instance()->Build< Float3 >( "SmoothBumpStep", x, edge0, edge1 );
	}

	Float4 SmoothBumpStep (const Float4 &x, const Float4 &edge0, const Float4 &edge1) {
		return NodeFunctions::Instance()->Build< Float4 >( "SmoothBumpStep", x, edge0, edge1 );
	}

	Float4 SmoothBumpStep (const Float4 &x, const Float &edge0, const Float &edge1) {
		return NodeFunctions::Instance()->Build< Float4 >( "SmoothBumpStep", x, edge0, edge1 );
	}

	Double  SmoothBumpStep (const Double  &x, const Double  &edge0, const Double  &edge1) {
		return NodeFunctions::Instance()->Build< Double  >( "SmoothBumpStep", x, edge0, edge1 );
	}

	Double2 SmoothBumpStep (const Double2 &x, const Double2 &edge0, const Double2 &edge1) {
		return NodeFunctions::Instance()->Build< Double2 >( "SmoothBumpStep", x, edge0, edge1 );
	}

	Double2 SmoothBumpStep (const Double2 &x, const Double &edge0, const Double &edge1) {
		return NodeFunctions::Instance()->Build< Double2 >( "SmoothBumpStep", x, edge0, edge1 );
	}

	Double3 SmoothBumpStep (const Double3 &x, const Double3 &edge0, const Double3 &edge1) {
		return NodeFunctions::Instance()->Build< Double3 >( "SmoothBumpStep", x, edge0, edge1 );
	}

	Double3 SmoothBumpStep (const Double3 &x, const Double &edge0, const Double &edge1) {
		return NodeFunctions::Instance()->Build< Double3 >( "SmoothBumpStep", x, edge0, edge1 );
	}

	Double4 SmoothBumpStep (const Double4 &x, const Double4 &edge0, const Double4 &edge1) {
		return NodeFunctions::Instance()->Build< Double4 >( "SmoothBumpStep", x, edge0, edge1 );
	}

	Double4 SmoothBumpStep (const Double4 &x, const Double &edge0, const Double &edge1) {
		return NodeFunctions::Instance()->Build< Double4 >( "SmoothBumpStep", x, edge0, edge1 );
	}


	// SafeLeftBitShift, SafeRightBitShift, BitRotateLeft, BitRotateRight
	Int  SafeLeftBitShift (const Int  &x, const Int  &shift) {
		return NodeFunctions::Instance()->Build< Int  >( "SafeLeftBitShift", x, shift );
	}

	Int2 SafeLeftBitShift (const Int2 &x, const Int2 &shift) {
		return NodeFunctions::Instance()->Build< Int2 >( "SafeLeftBitShift", x, shift );
	}

	Int3 SafeLeftBitShift (const Int3 &x, const Int3 &shift) {
		return NodeFunctions::Instance()->Build< Int3 >( "SafeLeftBitShift", x, shift );
	}

	Int4 SafeLeftBitShift (const Int4 &x, const Int4 &shift) {
		return NodeFunctions::Instance()->Build< Int4 >( "SafeLeftBitShift", x, shift );
	}

	UInt  SafeLeftBitShift (const UInt  &x, const UInt  &shift) {
		return NodeFunctions::Instance()->Build< UInt  >( "SafeLeftBitShift", x, shift );
	}

	UInt2 SafeLeftBitShift (const UInt2 &x, const UInt2 &shift) {
		return NodeFunctions::Instance()->Build< UInt2 >( "SafeLeftBitShift", x, shift );
	}

	UInt3 SafeLeftBitShift (const UInt3 &x, const UInt3 &shift) {
		return NodeFunctions::Instance()->Build< UInt3 >( "SafeLeftBitShift", x, shift );
	}

	UInt4 SafeLeftBitShift (const UInt4 &x, const UInt4 &shift) {
		return NodeFunctions::Instance()->Build< UInt4 >( "SafeLeftBitShift", x, shift );
	}

	Long  SafeLeftBitShift (const Long  &x, const Long  &shift) {
		return NodeFunctions::Instance()->Build< Long  >( "SafeLeftBitShift", x, shift );
	}

	Long2 SafeLeftBitShift (const Long2 &x, const Long2 &shift) {
		return NodeFunctions::Instance()->Build< Long2 >( "SafeLeftBitShift", x, shift );
	}

	Long3 SafeLeftBitShift (const Long3 &x, const Long3 &shift) {
		return NodeFunctions::Instance()->Build< Long3 >( "SafeLeftBitShift", x, shift );
	}

	Long4 SafeLeftBitShift (const Long4 &x, const Long4 &shift) {
		return NodeFunctions::Instance()->Build< Long4 >( "SafeLeftBitShift", x, shift );
	}

	ULong  SafeLeftBitShift (const ULong  &x, const ULong  &shift) {
		return NodeFunctions::Instance()->Build< ULong  >( "SafeLeftBitShift", x, shift );
	}

	ULong2 SafeLeftBitShift (const ULong2 &x, const ULong2 &shift) {
		return NodeFunctions::Instance()->Build< ULong2 >( "SafeLeftBitShift", x, shift );
	}

	ULong3 SafeLeftBitShift (const ULong3 &x, const ULong3 &shift) {
		return NodeFunctions::Instance()->Build< ULong3 >( "SafeLeftBitShift", x, shift );
	}

	ULong4 SafeLeftBitShift (const ULong4 &x, const ULong4 &shift) {
		return NodeFunctions::Instance()->Build< ULong4 >( "SafeLeftBitShift", x, shift );
	}

	Int  SafeRightBitShift (const Int  &x, const Int  &shift) {
		return NodeFunctions::Instance()->Build< Int  >( "SafeRightBitShift", x, shift );
	}

	Int2 SafeRightBitShift (const Int2 &x, const Int2 &shift) {
		return NodeFunctions::Instance()->Build< Int2 >( "SafeRightBitShift", x, shift );
	}

	Int3 SafeRightBitShift (const Int3 &x, const Int3 &shift) {
		return NodeFunctions::Instance()->Build< Int3 >( "SafeRightBitShift", x, shift );
	}

	Int4 SafeRightBitShift (const Int4 &x, const Int4 &shift) {
		return NodeFunctions::Instance()->Build< Int4 >( "SafeRightBitShift", x, shift );
	}

	UInt  SafeRightBitShift (const UInt  &x, const UInt  &shift) {
		return NodeFunctions::Instance()->Build< UInt  >( "SafeRightBitShift", x, shift );
	}

	UInt2 SafeRightBitShift (const UInt2 &x, const UInt2 &shift) {
		return NodeFunctions::Instance()->Build< UInt2 >( "SafeRightBitShift", x, shift );
	}

	UInt3 SafeRightBitShift (const UInt3 &x, const UInt3 &shift) {
		return NodeFunctions::Instance()->Build< UInt3 >( "SafeRightBitShift", x, shift );
	}

	UInt4 SafeRightBitShift (const UInt4 &x, const UInt4 &shift) {
		return NodeFunctions::Instance()->Build< UInt4 >( "SafeRightBitShift", x, shift );
	}

	Long  SafeRightBitShift (const Long  &x, const Long  &shift) {
		return NodeFunctions::Instance()->Build< Long  >( "SafeRightBitShift", x, shift );
	}

	Long2 SafeRightBitShift (const Long2 &x, const Long2 &shift) {
		return NodeFunctions::Instance()->Build< Long2 >( "SafeRightBitShift", x, shift );
	}

	Long3 SafeRightBitShift (const Long3 &x, const Long3 &shift) {
		return NodeFunctions::Instance()->Build< Long3 >( "SafeRightBitShift", x, shift );
	}

	Long4 SafeRightBitShift (const Long4 &x, const Long4 &shift) {
		return NodeFunctions::Instance()->Build< Long4 >( "SafeRightBitShift", x, shift );
	}

	ULong  SafeRightBitShift (const ULong  &x, const ULong  &shift) {
		return NodeFunctions::Instance()->Build< ULong  >( "SafeRightBitShift", x, shift );
	}

	ULong2 SafeRightBitShift (const ULong2 &x, const ULong2 &shift) {
		return NodeFunctions::Instance()->Build< ULong2 >( "SafeRightBitShift", x, shift );
	}

	ULong3 SafeRightBitShift (const ULong3 &x, const ULong3 &shift) {
		return NodeFunctions::Instance()->Build< ULong3 >( "SafeRightBitShift", x, shift );
	}

	ULong4 SafeRightBitShift (const ULong4 &x, const ULong4 &shift) {
		return NodeFunctions::Instance()->Build< ULong4 >( "SafeRightBitShift", x, shift );
	}

	Int  BitRotateLeft (const Int  &x, const Int  &shift) {
		return NodeFunctions::Instance()->Build< Int  >( "BitRotateLeft", x, shift );
	}

	Int2 BitRotateLeft (const Int2 &x, const Int2 &shift) {
		return NodeFunctions::Instance()->Build< Int2 >( "BitRotateLeft", x, shift );
	}

	Int3 BitRotateLeft (const Int3 &x, const Int3 &shift) {
		return NodeFunctions::Instance()->Build< Int3 >( "BitRotateLeft", x, shift );
	}

	Int4 BitRotateLeft (const Int4 &x, const Int4 &shift) {
		return NodeFunctions::Instance()->Build< Int4 >( "BitRotateLeft", x, shift );
	}

	UInt  BitRotateLeft (const UInt  &x, const UInt  &shift) {
		return NodeFunctions::Instance()->Build< UInt  >( "BitRotateLeft", x, shift );
	}

	UInt2 BitRotateLeft (const UInt2 &x, const UInt2 &shift) {
		return NodeFunctions::Instance()->Build< UInt2 >( "BitRotateLeft", x, shift );
	}

	UInt3 BitRotateLeft (const UInt3 &x, const UInt3 &shift) {
		return NodeFunctions::Instance()->Build< UInt3 >( "BitRotateLeft", x, shift );
	}

	UInt4 BitRotateLeft (const UInt4 &x, const UInt4 &shift) {
		return NodeFunctions::Instance()->Build< UInt4 >( "BitRotateLeft", x, shift );
	}

	Long  BitRotateLeft (const Long  &x, const Long  &shift) {
		return NodeFunctions::Instance()->Build< Long  >( "BitRotateLeft", x, shift );
	}

	Long2 BitRotateLeft (const Long2 &x, const Long2 &shift) {
		return NodeFunctions::Instance()->Build< Long2 >( "BitRotateLeft", x, shift );
	}

	Long3 BitRotateLeft (const Long3 &x, const Long3 &shift) {
		return NodeFunctions::Instance()->Build< Long3 >( "BitRotateLeft", x, shift );
	}

	Long4 BitRotateLeft (const Long4 &x, const Long4 &shift) {
		return NodeFunctions::Instance()->Build< Long4 >( "BitRotateLeft", x, shift );
	}

	ULong  BitRotateLeft (const ULong  &x, const ULong  &shift) {
		return NodeFunctions::Instance()->Build< ULong  >( "BitRotateLeft", x, shift );
	}

	ULong2 BitRotateLeft (const ULong2 &x, const ULong2 &shift) {
		return NodeFunctions::Instance()->Build< ULong2 >( "BitRotateLeft", x, shift );
	}

	ULong3 BitRotateLeft (const ULong3 &x, const ULong3 &shift) {
		return NodeFunctions::Instance()->Build< ULong3 >( "BitRotateLeft", x, shift );
	}

	ULong4 BitRotateLeft (const ULong4 &x, const ULong4 &shift) {
		return NodeFunctions::Instance()->Build< ULong4 >( "BitRotateLeft", x, shift );
	}

	Int  BitRotateRight (const Int  &x, const Int  &shift) {
		return NodeFunctions::Instance()->Build< Int  >( "BitRotateRight", x, shift );
	}

	Int2 BitRotateRight (const Int2 &x, const Int2 &shift) {
		return NodeFunctions::Instance()->Build< Int2 >( "BitRotateRight", x, shift );
	}

	Int3 BitRotateRight (const Int3 &x, const Int3 &shift) {
		return NodeFunctions::Instance()->Build< Int3 >( "BitRotateRight", x, shift );
	}

	Int4 BitRotateRight (const Int4 &x, const Int4 &shift) {
		return NodeFunctions::Instance()->Build< Int4 >( "BitRotateRight", x, shift );
	}

	UInt  BitRotateRight (const UInt  &x, const UInt  &shift) {
		return NodeFunctions::Instance()->Build< UInt  >( "BitRotateRight", x, shift );
	}

	UInt2 BitRotateRight (const UInt2 &x, const UInt2 &shift) {
		return NodeFunctions::Instance()->Build< UInt2 >( "BitRotateRight", x, shift );
	}

	UInt3 BitRotateRight (const UInt3 &x, const UInt3 &shift) {
		return NodeFunctions::Instance()->Build< UInt3 >( "BitRotateRight", x, shift );
	}

	UInt4 BitRotateRight (const UInt4 &x, const UInt4 &shift) {
		return NodeFunctions::Instance()->Build< UInt4 >( "BitRotateRight", x, shift );
	}

	Long  BitRotateRight (const Long  &x, const Long  &shift) {
		return NodeFunctions::Instance()->Build< Long  >( "BitRotateRight", x, shift );
	}

	Long2 BitRotateRight (const Long2 &x, const Long2 &shift) {
		return NodeFunctions::Instance()->Build< Long2 >( "BitRotateRight", x, shift );
	}

	Long3 BitRotateRight (const Long3 &x, const Long3 &shift) {
		return NodeFunctions::Instance()->Build< Long3 >( "BitRotateRight", x, shift );
	}

	Long4 BitRotateRight (const Long4 &x, const Long4 &shift) {
		return NodeFunctions::Instance()->Build< Long4 >( "BitRotateRight", x, shift );
	}

	ULong  BitRotateRight (const ULong  &x, const ULong  &shift) {
		return NodeFunctions::Instance()->Build< ULong  >( "BitRotateRight", x, shift );
	}

	ULong2 BitRotateRight (const ULong2 &x, const ULong2 &shift) {
		return NodeFunctions::Instance()->Build< ULong2 >( "BitRotateRight", x, shift );
	}

	ULong3 BitRotateRight (const ULong3 &x, const ULong3 &shift) {
		return NodeFunctions::Instance()->Build< ULong3 >( "BitRotateRight", x, shift );
	}

	ULong4 BitRotateRight (const ULong4 &x, const ULong4 &shift) {
		return NodeFunctions::Instance()->Build< ULong4 >( "BitRotateRight", x, shift );
	}


	// ReverseBitOrder, BitScanReverse, BitScanForward
	Int  ReverseBitOrder (const Int  &x) {
		return NodeFunctions::Instance()->Build< Int  >( "ReverseBitOrder", x );
	}

	Int2 ReverseBitOrder (const Int2 &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "ReverseBitOrder", x );
	}

	Int3 ReverseBitOrder (const Int3 &x) {
		return NodeFunctions::Instance()->Build< Int3 >( "ReverseBitOrder", x );
	}

	Int4 ReverseBitOrder (const Int4 &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "ReverseBitOrder", x );
	}

	UInt  ReverseBitOrder (const UInt  &x) {
		return NodeFunctions::Instance()->Build< UInt  >( "ReverseBitOrder", x );
	}

	UInt2 ReverseBitOrder (const UInt2 &x) {
		return NodeFunctions::Instance()->Build< UInt2 >( "ReverseBitOrder", x );
	}

	UInt3 ReverseBitOrder (const UInt3 &x) {
		return NodeFunctions::Instance()->Build< UInt3 >( "ReverseBitOrder", x );
	}

	UInt4 ReverseBitOrder (const UInt4 &x) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ReverseBitOrder", x );
	}

	Long  ReverseBitOrder (const Long  &x) {
		return NodeFunctions::Instance()->Build< Long  >( "ReverseBitOrder", x );
	}

	Long2 ReverseBitOrder (const Long2 &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "ReverseBitOrder", x );
	}

	Long3 ReverseBitOrder (const Long3 &x) {
		return NodeFunctions::Instance()->Build< Long3 >( "ReverseBitOrder", x );
	}

	Long4 ReverseBitOrder (const Long4 &x) {
		return NodeFunctions::Instance()->Build< Long4 >( "ReverseBitOrder", x );
	}

	ULong  ReverseBitOrder (const ULong  &x) {
		return NodeFunctions::Instance()->Build< ULong  >( "ReverseBitOrder", x );
	}

	ULong2 ReverseBitOrder (const ULong2 &x) {
		return NodeFunctions::Instance()->Build< ULong2 >( "ReverseBitOrder", x );
	}

	ULong3 ReverseBitOrder (const ULong3 &x) {
		return NodeFunctions::Instance()->Build< ULong3 >( "ReverseBitOrder", x );
	}

	ULong4 ReverseBitOrder (const ULong4 &x) {
		return NodeFunctions::Instance()->Build< ULong4 >( "ReverseBitOrder", x );
	}

	Int  BitScanReverse (const Int  &x) {
		return NodeFunctions::Instance()->Build< Int  >( "BitScanReverse", x );
	}

	Int2 BitScanReverse (const Int2 &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "BitScanReverse", x );
	}

	Int3 BitScanReverse (const Int3 &x) {
		return NodeFunctions::Instance()->Build< Int3 >( "BitScanReverse", x );
	}

	Int4 BitScanReverse (const Int4 &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "BitScanReverse", x );
	}

	UInt  BitScanReverse (const UInt  &x) {
		return NodeFunctions::Instance()->Build< UInt  >( "BitScanReverse", x );
	}

	UInt2 BitScanReverse (const UInt2 &x) {
		return NodeFunctions::Instance()->Build< UInt2 >( "BitScanReverse", x );
	}

	UInt3 BitScanReverse (const UInt3 &x) {
		return NodeFunctions::Instance()->Build< UInt3 >( "BitScanReverse", x );
	}

	UInt4 BitScanReverse (const UInt4 &x) {
		return NodeFunctions::Instance()->Build< UInt4 >( "BitScanReverse", x );
	}

	Long  BitScanReverse (const Long  &x) {
		return NodeFunctions::Instance()->Build< Long  >( "BitScanReverse", x );
	}

	Long2 BitScanReverse (const Long2 &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "BitScanReverse", x );
	}

	Long3 BitScanReverse (const Long3 &x) {
		return NodeFunctions::Instance()->Build< Long3 >( "BitScanReverse", x );
	}

	Long4 BitScanReverse (const Long4 &x) {
		return NodeFunctions::Instance()->Build< Long4 >( "BitScanReverse", x );
	}

	ULong  BitScanReverse (const ULong  &x) {
		return NodeFunctions::Instance()->Build< ULong  >( "BitScanReverse", x );
	}

	ULong2 BitScanReverse (const ULong2 &x) {
		return NodeFunctions::Instance()->Build< ULong2 >( "BitScanReverse", x );
	}

	ULong3 BitScanReverse (const ULong3 &x) {
		return NodeFunctions::Instance()->Build< ULong3 >( "BitScanReverse", x );
	}

	ULong4 BitScanReverse (const ULong4 &x) {
		return NodeFunctions::Instance()->Build< ULong4 >( "BitScanReverse", x );
	}

	Int  BitScanForward (const Int  &x) {
		return NodeFunctions::Instance()->Build< Int  >( "BitScanForward", x );
	}

	Int2 BitScanForward (const Int2 &x) {
		return NodeFunctions::Instance()->Build< Int2 >( "BitScanForward", x );
	}

	Int3 BitScanForward (const Int3 &x) {
		return NodeFunctions::Instance()->Build< Int3 >( "BitScanForward", x );
	}

	Int4 BitScanForward (const Int4 &x) {
		return NodeFunctions::Instance()->Build< Int4 >( "BitScanForward", x );
	}

	UInt  BitScanForward (const UInt  &x) {
		return NodeFunctions::Instance()->Build< UInt  >( "BitScanForward", x );
	}

	UInt2 BitScanForward (const UInt2 &x) {
		return NodeFunctions::Instance()->Build< UInt2 >( "BitScanForward", x );
	}

	UInt3 BitScanForward (const UInt3 &x) {
		return NodeFunctions::Instance()->Build< UInt3 >( "BitScanForward", x );
	}

	UInt4 BitScanForward (const UInt4 &x) {
		return NodeFunctions::Instance()->Build< UInt4 >( "BitScanForward", x );
	}

	Long  BitScanForward (const Long  &x) {
		return NodeFunctions::Instance()->Build< Long  >( "BitScanForward", x );
	}

	Long2 BitScanForward (const Long2 &x) {
		return NodeFunctions::Instance()->Build< Long2 >( "BitScanForward", x );
	}

	Long3 BitScanForward (const Long3 &x) {
		return NodeFunctions::Instance()->Build< Long3 >( "BitScanForward", x );
	}

	Long4 BitScanForward (const Long4 &x) {
		return NodeFunctions::Instance()->Build< Long4 >( "BitScanForward", x );
	}

	ULong  BitScanForward (const ULong  &x) {
		return NodeFunctions::Instance()->Build< ULong  >( "BitScanForward", x );
	}

	ULong2 BitScanForward (const ULong2 &x) {
		return NodeFunctions::Instance()->Build< ULong2 >( "BitScanForward", x );
	}

	ULong3 BitScanForward (const ULong3 &x) {
		return NodeFunctions::Instance()->Build< ULong3 >( "BitScanForward", x );
	}

	ULong4 BitScanForward (const ULong4 &x) {
		return NodeFunctions::Instance()->Build< ULong4 >( "BitScanForward", x );
	}


	// Clamp, ClampOut, Wrap
	Float  Clamp (const Float  &x, const Float  &min, const Float  &max) {
		return NodeFunctions::Instance()->Build< Float  >( "Clamp", x, min, max );
	}

	Float2 Clamp (const Float2 &x, const Float2 &min, const Float2 &max) {
		return NodeFunctions::Instance()->Build< Float2 >( "Clamp", x, min, max );
	}

	Float2 Clamp (const Float2 &x, const Float &min, const Float &max) {
		return NodeFunctions::Instance()->Build< Float2 >( "Clamp", x, min, max );
	}

	Float3 Clamp (const Float3 &x, const Float3 &min, const Float3 &max) {
		return NodeFunctions::Instance()->Build< Float3 >( "Clamp", x, min, max );
	}

	Float3 Clamp (const Float3 &x, const Float &min, const Float &max) {
		return NodeFunctions::Instance()->Build< Float3 >( "Clamp", x, min, max );
	}

	Float4 Clamp (const Float4 &x, const Float4 &min, const Float4 &max) {
		return NodeFunctions::Instance()->Build< Float4 >( "Clamp", x, min, max );
	}

	Float4 Clamp (const Float4 &x, const Float &min, const Float &max) {
		return NodeFunctions::Instance()->Build< Float4 >( "Clamp", x, min, max );
	}

	Double  Clamp (const Double  &x, const Double  &min, const Double  &max) {
		return NodeFunctions::Instance()->Build< Double  >( "Clamp", x, min, max );
	}

	Double2 Clamp (const Double2 &x, const Double2 &min, const Double2 &max) {
		return NodeFunctions::Instance()->Build< Double2 >( "Clamp", x, min, max );
	}

	Double2 Clamp (const Double2 &x, const Double &min, const Double &max) {
		return NodeFunctions::Instance()->Build< Double2 >( "Clamp", x, min, max );
	}

	Double3 Clamp (const Double3 &x, const Double3 &min, const Double3 &max) {
		return NodeFunctions::Instance()->Build< Double3 >( "Clamp", x, min, max );
	}

	Double3 Clamp (const Double3 &x, const Double &min, const Double &max) {
		return NodeFunctions::Instance()->Build< Double3 >( "Clamp", x, min, max );
	}

	Double4 Clamp (const Double4 &x, const Double4 &min, const Double4 &max) {
		return NodeFunctions::Instance()->Build< Double4 >( "Clamp", x, min, max );
	}

	Double4 Clamp (const Double4 &x, const Double &min, const Double &max) {
		return NodeFunctions::Instance()->Build< Double4 >( "Clamp", x, min, max );
	}

	Float  ClampOut (const Float  &x, const Float  &min, const Float  &max) {
		return NodeFunctions::Instance()->Build< Float  >( "ClampOut", x, min, max );
	}

	Float2 ClampOut (const Float2 &x, const Float2 &min, const Float2 &max) {
		return NodeFunctions::Instance()->Build< Float2 >( "ClampOut", x, min, max );
	}

	Float2 ClampOut (const Float2 &x, const Float &min, const Float &max) {
		return NodeFunctions::Instance()->Build< Float2 >( "ClampOut", x, min, max );
	}

	Float3 ClampOut (const Float3 &x, const Float3 &min, const Float3 &max) {
		return NodeFunctions::Instance()->Build< Float3 >( "ClampOut", x, min, max );
	}

	Float3 ClampOut (const Float3 &x, const Float &min, const Float &max) {
		return NodeFunctions::Instance()->Build< Float3 >( "ClampOut", x, min, max );
	}

	Float4 ClampOut (const Float4 &x, const Float4 &min, const Float4 &max) {
		return NodeFunctions::Instance()->Build< Float4 >( "ClampOut", x, min, max );
	}

	Float4 ClampOut (const Float4 &x, const Float &min, const Float &max) {
		return NodeFunctions::Instance()->Build< Float4 >( "ClampOut", x, min, max );
	}

	Double  ClampOut (const Double  &x, const Double  &min, const Double  &max) {
		return NodeFunctions::Instance()->Build< Double  >( "ClampOut", x, min, max );
	}

	Double2 ClampOut (const Double2 &x, const Double2 &min, const Double2 &max) {
		return NodeFunctions::Instance()->Build< Double2 >( "ClampOut", x, min, max );
	}

	Double2 ClampOut (const Double2 &x, const Double &min, const Double &max) {
		return NodeFunctions::Instance()->Build< Double2 >( "ClampOut", x, min, max );
	}

	Double3 ClampOut (const Double3 &x, const Double3 &min, const Double3 &max) {
		return NodeFunctions::Instance()->Build< Double3 >( "ClampOut", x, min, max );
	}

	Double3 ClampOut (const Double3 &x, const Double &min, const Double &max) {
		return NodeFunctions::Instance()->Build< Double3 >( "ClampOut", x, min, max );
	}

	Double4 ClampOut (const Double4 &x, const Double4 &min, const Double4 &max) {
		return NodeFunctions::Instance()->Build< Double4 >( "ClampOut", x, min, max );
	}

	Double4 ClampOut (const Double4 &x, const Double &min, const Double &max) {
		return NodeFunctions::Instance()->Build< Double4 >( "ClampOut", x, min, max );
	}

	Float  Wrap (const Float  &x, const Float  &min, const Float  &max) {
		return NodeFunctions::Instance()->Build< Float  >( "Wrap", x, min, max );
	}

	Float2 Wrap (const Float2 &x, const Float2 &min, const Float2 &max) {
		return NodeFunctions::Instance()->Build< Float2 >( "Wrap", x, min, max );
	}

	Float2 Wrap (const Float2 &x, const Float &min, const Float &max) {
		return NodeFunctions::Instance()->Build< Float2 >( "Wrap", x, min, max );
	}

	Float3 Wrap (const Float3 &x, const Float3 &min, const Float3 &max) {
		return NodeFunctions::Instance()->Build< Float3 >( "Wrap", x, min, max );
	}

	Float3 Wrap (const Float3 &x, const Float &min, const Float &max) {
		return NodeFunctions::Instance()->Build< Float3 >( "Wrap", x, min, max );
	}

	Float4 Wrap (const Float4 &x, const Float4 &min, const Float4 &max) {
		return NodeFunctions::Instance()->Build< Float4 >( "Wrap", x, min, max );
	}

	Float4 Wrap (const Float4 &x, const Float &min, const Float &max) {
		return NodeFunctions::Instance()->Build< Float4 >( "Wrap", x, min, max );
	}

	Double  Wrap (const Double  &x, const Double  &min, const Double  &max) {
		return NodeFunctions::Instance()->Build< Double  >( "Wrap", x, min, max );
	}

	Double2 Wrap (const Double2 &x, const Double2 &min, const Double2 &max) {
		return NodeFunctions::Instance()->Build< Double2 >( "Wrap", x, min, max );
	}

	Double2 Wrap (const Double2 &x, const Double &min, const Double &max) {
		return NodeFunctions::Instance()->Build< Double2 >( "Wrap", x, min, max );
	}

	Double3 Wrap (const Double3 &x, const Double3 &min, const Double3 &max) {
		return NodeFunctions::Instance()->Build< Double3 >( "Wrap", x, min, max );
	}

	Double3 Wrap (const Double3 &x, const Double &min, const Double &max) {
		return NodeFunctions::Instance()->Build< Double3 >( "Wrap", x, min, max );
	}

	Double4 Wrap (const Double4 &x, const Double4 &min, const Double4 &max) {
		return NodeFunctions::Instance()->Build< Double4 >( "Wrap", x, min, max );
	}

	Double4 Wrap (const Double4 &x, const Double &min, const Double &max) {
		return NodeFunctions::Instance()->Build< Double4 >( "Wrap", x, min, max );
	}


	// Min, Max, MinAbs, MaxAbs, MinMag, MaxMag, Mid
	Int  Min (const Int  &x, const Int  &y) {
		return NodeFunctions::Instance()->Build< Int  >( "Min", x, y );
	}

	Int2 Min (const Int2 &x, const Int2 &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "Min", x, y );
	}

	Int2 Min (const Int &x, const Int2 &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "Min", x, y );
	}

	Int2 Min (const Int2 &x, const Int &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "Min", x, y );
	}

	Int3 Min (const Int3 &x, const Int3 &y) {
		return NodeFunctions::Instance()->Build< Int3 >( "Min", x, y );
	}

	Int3 Min (const Int &x, const Int3 &y) {
		return NodeFunctions::Instance()->Build< Int3 >( "Min", x, y );
	}

	Int3 Min (const Int3 &x, const Int &y) {
		return NodeFunctions::Instance()->Build< Int3 >( "Min", x, y );
	}

	Int4 Min (const Int4 &x, const Int4 &y) {
		return NodeFunctions::Instance()->Build< Int4 >( "Min", x, y );
	}

	Int4 Min (const Int &x, const Int4 &y) {
		return NodeFunctions::Instance()->Build< Int4 >( "Min", x, y );
	}

	Int4 Min (const Int4 &x, const Int &y) {
		return NodeFunctions::Instance()->Build< Int4 >( "Min", x, y );
	}

	UInt  Min (const UInt  &x, const UInt  &y) {
		return NodeFunctions::Instance()->Build< UInt  >( "Min", x, y );
	}

	UInt2 Min (const UInt2 &x, const UInt2 &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Min", x, y );
	}

	UInt2 Min (const UInt &x, const UInt2 &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Min", x, y );
	}

	UInt2 Min (const UInt2 &x, const UInt &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Min", x, y );
	}

	UInt3 Min (const UInt3 &x, const UInt3 &y) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Min", x, y );
	}

	UInt3 Min (const UInt &x, const UInt3 &y) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Min", x, y );
	}

	UInt3 Min (const UInt3 &x, const UInt &y) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Min", x, y );
	}

	UInt4 Min (const UInt4 &x, const UInt4 &y) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Min", x, y );
	}

	UInt4 Min (const UInt &x, const UInt4 &y) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Min", x, y );
	}

	UInt4 Min (const UInt4 &x, const UInt &y) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Min", x, y );
	}

	Long  Min (const Long  &x, const Long  &y) {
		return NodeFunctions::Instance()->Build< Long  >( "Min", x, y );
	}

	Long2 Min (const Long2 &x, const Long2 &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "Min", x, y );
	}

	Long2 Min (const Long &x, const Long2 &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "Min", x, y );
	}

	Long2 Min (const Long2 &x, const Long &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "Min", x, y );
	}

	Long3 Min (const Long3 &x, const Long3 &y) {
		return NodeFunctions::Instance()->Build< Long3 >( "Min", x, y );
	}

	Long3 Min (const Long &x, const Long3 &y) {
		return NodeFunctions::Instance()->Build< Long3 >( "Min", x, y );
	}

	Long3 Min (const Long3 &x, const Long &y) {
		return NodeFunctions::Instance()->Build< Long3 >( "Min", x, y );
	}

	Long4 Min (const Long4 &x, const Long4 &y) {
		return NodeFunctions::Instance()->Build< Long4 >( "Min", x, y );
	}

	Long4 Min (const Long &x, const Long4 &y) {
		return NodeFunctions::Instance()->Build< Long4 >( "Min", x, y );
	}

	Long4 Min (const Long4 &x, const Long &y) {
		return NodeFunctions::Instance()->Build< Long4 >( "Min", x, y );
	}

	ULong  Min (const ULong  &x, const ULong  &y) {
		return NodeFunctions::Instance()->Build< ULong  >( "Min", x, y );
	}

	ULong2 Min (const ULong2 &x, const ULong2 &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Min", x, y );
	}

	ULong2 Min (const ULong &x, const ULong2 &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Min", x, y );
	}

	ULong2 Min (const ULong2 &x, const ULong &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Min", x, y );
	}

	ULong3 Min (const ULong3 &x, const ULong3 &y) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Min", x, y );
	}

	ULong3 Min (const ULong &x, const ULong3 &y) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Min", x, y );
	}

	ULong3 Min (const ULong3 &x, const ULong &y) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Min", x, y );
	}

	ULong4 Min (const ULong4 &x, const ULong4 &y) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Min", x, y );
	}

	ULong4 Min (const ULong &x, const ULong4 &y) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Min", x, y );
	}

	ULong4 Min (const ULong4 &x, const ULong &y) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Min", x, y );
	}

	Float  Min (const Float  &x, const Float  &y) {
		return NodeFunctions::Instance()->Build< Float  >( "Min", x, y );
	}

	Float2 Min (const Float2 &x, const Float2 &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "Min", x, y );
	}

	Float2 Min (const Float &x, const Float2 &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "Min", x, y );
	}

	Float2 Min (const Float2 &x, const Float &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "Min", x, y );
	}

	Float3 Min (const Float3 &x, const Float3 &y) {
		return NodeFunctions::Instance()->Build< Float3 >( "Min", x, y );
	}

	Float3 Min (const Float &x, const Float3 &y) {
		return NodeFunctions::Instance()->Build< Float3 >( "Min", x, y );
	}

	Float3 Min (const Float3 &x, const Float &y) {
		return NodeFunctions::Instance()->Build< Float3 >( "Min", x, y );
	}

	Float4 Min (const Float4 &x, const Float4 &y) {
		return NodeFunctions::Instance()->Build< Float4 >( "Min", x, y );
	}

	Float4 Min (const Float &x, const Float4 &y) {
		return NodeFunctions::Instance()->Build< Float4 >( "Min", x, y );
	}

	Float4 Min (const Float4 &x, const Float &y) {
		return NodeFunctions::Instance()->Build< Float4 >( "Min", x, y );
	}

	Double  Min (const Double  &x, const Double  &y) {
		return NodeFunctions::Instance()->Build< Double  >( "Min", x, y );
	}

	Double2 Min (const Double2 &x, const Double2 &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "Min", x, y );
	}

	Double2 Min (const Double &x, const Double2 &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "Min", x, y );
	}

	Double2 Min (const Double2 &x, const Double &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "Min", x, y );
	}

	Double3 Min (const Double3 &x, const Double3 &y) {
		return NodeFunctions::Instance()->Build< Double3 >( "Min", x, y );
	}

	Double3 Min (const Double &x, const Double3 &y) {
		return NodeFunctions::Instance()->Build< Double3 >( "Min", x, y );
	}

	Double3 Min (const Double3 &x, const Double &y) {
		return NodeFunctions::Instance()->Build< Double3 >( "Min", x, y );
	}

	Double4 Min (const Double4 &x, const Double4 &y) {
		return NodeFunctions::Instance()->Build< Double4 >( "Min", x, y );
	}

	Double4 Min (const Double &x, const Double4 &y) {
		return NodeFunctions::Instance()->Build< Double4 >( "Min", x, y );
	}

	Double4 Min (const Double4 &x, const Double &y) {
		return NodeFunctions::Instance()->Build< Double4 >( "Min", x, y );
	}

	Int  Max (const Int  &x, const Int  &y) {
		return NodeFunctions::Instance()->Build< Int  >( "Max", x, y );
	}

	Int2 Max (const Int2 &x, const Int2 &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "Max", x, y );
	}

	Int2 Max (const Int &x, const Int2 &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "Max", x, y );
	}

	Int2 Max (const Int2 &x, const Int &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "Max", x, y );
	}

	Int3 Max (const Int3 &x, const Int3 &y) {
		return NodeFunctions::Instance()->Build< Int3 >( "Max", x, y );
	}

	Int3 Max (const Int &x, const Int3 &y) {
		return NodeFunctions::Instance()->Build< Int3 >( "Max", x, y );
	}

	Int3 Max (const Int3 &x, const Int &y) {
		return NodeFunctions::Instance()->Build< Int3 >( "Max", x, y );
	}

	Int4 Max (const Int4 &x, const Int4 &y) {
		return NodeFunctions::Instance()->Build< Int4 >( "Max", x, y );
	}

	Int4 Max (const Int &x, const Int4 &y) {
		return NodeFunctions::Instance()->Build< Int4 >( "Max", x, y );
	}

	Int4 Max (const Int4 &x, const Int &y) {
		return NodeFunctions::Instance()->Build< Int4 >( "Max", x, y );
	}

	UInt  Max (const UInt  &x, const UInt  &y) {
		return NodeFunctions::Instance()->Build< UInt  >( "Max", x, y );
	}

	UInt2 Max (const UInt2 &x, const UInt2 &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Max", x, y );
	}

	UInt2 Max (const UInt &x, const UInt2 &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Max", x, y );
	}

	UInt2 Max (const UInt2 &x, const UInt &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Max", x, y );
	}

	UInt3 Max (const UInt3 &x, const UInt3 &y) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Max", x, y );
	}

	UInt3 Max (const UInt &x, const UInt3 &y) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Max", x, y );
	}

	UInt3 Max (const UInt3 &x, const UInt &y) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Max", x, y );
	}

	UInt4 Max (const UInt4 &x, const UInt4 &y) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Max", x, y );
	}

	UInt4 Max (const UInt &x, const UInt4 &y) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Max", x, y );
	}

	UInt4 Max (const UInt4 &x, const UInt &y) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Max", x, y );
	}

	Long  Max (const Long  &x, const Long  &y) {
		return NodeFunctions::Instance()->Build< Long  >( "Max", x, y );
	}

	Long2 Max (const Long2 &x, const Long2 &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "Max", x, y );
	}

	Long2 Max (const Long &x, const Long2 &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "Max", x, y );
	}

	Long2 Max (const Long2 &x, const Long &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "Max", x, y );
	}

	Long3 Max (const Long3 &x, const Long3 &y) {
		return NodeFunctions::Instance()->Build< Long3 >( "Max", x, y );
	}

	Long3 Max (const Long &x, const Long3 &y) {
		return NodeFunctions::Instance()->Build< Long3 >( "Max", x, y );
	}

	Long3 Max (const Long3 &x, const Long &y) {
		return NodeFunctions::Instance()->Build< Long3 >( "Max", x, y );
	}

	Long4 Max (const Long4 &x, const Long4 &y) {
		return NodeFunctions::Instance()->Build< Long4 >( "Max", x, y );
	}

	Long4 Max (const Long &x, const Long4 &y) {
		return NodeFunctions::Instance()->Build< Long4 >( "Max", x, y );
	}

	Long4 Max (const Long4 &x, const Long &y) {
		return NodeFunctions::Instance()->Build< Long4 >( "Max", x, y );
	}

	ULong  Max (const ULong  &x, const ULong  &y) {
		return NodeFunctions::Instance()->Build< ULong  >( "Max", x, y );
	}

	ULong2 Max (const ULong2 &x, const ULong2 &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Max", x, y );
	}

	ULong2 Max (const ULong &x, const ULong2 &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Max", x, y );
	}

	ULong2 Max (const ULong2 &x, const ULong &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Max", x, y );
	}

	ULong3 Max (const ULong3 &x, const ULong3 &y) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Max", x, y );
	}

	ULong3 Max (const ULong &x, const ULong3 &y) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Max", x, y );
	}

	ULong3 Max (const ULong3 &x, const ULong &y) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Max", x, y );
	}

	ULong4 Max (const ULong4 &x, const ULong4 &y) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Max", x, y );
	}

	ULong4 Max (const ULong &x, const ULong4 &y) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Max", x, y );
	}

	ULong4 Max (const ULong4 &x, const ULong &y) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Max", x, y );
	}

	Float  Max (const Float  &x, const Float  &y) {
		return NodeFunctions::Instance()->Build< Float  >( "Max", x, y );
	}

	Float2 Max (const Float2 &x, const Float2 &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "Max", x, y );
	}

	Float2 Max (const Float &x, const Float2 &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "Max", x, y );
	}

	Float2 Max (const Float2 &x, const Float &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "Max", x, y );
	}

	Float3 Max (const Float3 &x, const Float3 &y) {
		return NodeFunctions::Instance()->Build< Float3 >( "Max", x, y );
	}

	Float3 Max (const Float &x, const Float3 &y) {
		return NodeFunctions::Instance()->Build< Float3 >( "Max", x, y );
	}

	Float3 Max (const Float3 &x, const Float &y) {
		return NodeFunctions::Instance()->Build< Float3 >( "Max", x, y );
	}

	Float4 Max (const Float4 &x, const Float4 &y) {
		return NodeFunctions::Instance()->Build< Float4 >( "Max", x, y );
	}

	Float4 Max (const Float &x, const Float4 &y) {
		return NodeFunctions::Instance()->Build< Float4 >( "Max", x, y );
	}

	Float4 Max (const Float4 &x, const Float &y) {
		return NodeFunctions::Instance()->Build< Float4 >( "Max", x, y );
	}

	Double  Max (const Double  &x, const Double  &y) {
		return NodeFunctions::Instance()->Build< Double  >( "Max", x, y );
	}

	Double2 Max (const Double2 &x, const Double2 &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "Max", x, y );
	}

	Double2 Max (const Double &x, const Double2 &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "Max", x, y );
	}

	Double2 Max (const Double2 &x, const Double &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "Max", x, y );
	}

	Double3 Max (const Double3 &x, const Double3 &y) {
		return NodeFunctions::Instance()->Build< Double3 >( "Max", x, y );
	}

	Double3 Max (const Double &x, const Double3 &y) {
		return NodeFunctions::Instance()->Build< Double3 >( "Max", x, y );
	}

	Double3 Max (const Double3 &x, const Double &y) {
		return NodeFunctions::Instance()->Build< Double3 >( "Max", x, y );
	}

	Double4 Max (const Double4 &x, const Double4 &y) {
		return NodeFunctions::Instance()->Build< Double4 >( "Max", x, y );
	}

	Double4 Max (const Double &x, const Double4 &y) {
		return NodeFunctions::Instance()->Build< Double4 >( "Max", x, y );
	}

	Double4 Max (const Double4 &x, const Double &y) {
		return NodeFunctions::Instance()->Build< Double4 >( "Max", x, y );
	}

	Int  MinAbs (const Int  &x, const Int  &y) {
		return NodeFunctions::Instance()->Build< Int  >( "MinAbs", x, y );
	}

	Int2 MinAbs (const Int2 &x, const Int2 &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "MinAbs", x, y );
	}

	Int2 MinAbs (const Int &x, const Int2 &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "MinAbs", x, y );
	}

	Int2 MinAbs (const Int2 &x, const Int &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "MinAbs", x, y );
	}

	Int3 MinAbs (const Int3 &x, const Int3 &y) {
		return NodeFunctions::Instance()->Build< Int3 >( "MinAbs", x, y );
	}

	Int3 MinAbs (const Int &x, const Int3 &y) {
		return NodeFunctions::Instance()->Build< Int3 >( "MinAbs", x, y );
	}

	Int3 MinAbs (const Int3 &x, const Int &y) {
		return NodeFunctions::Instance()->Build< Int3 >( "MinAbs", x, y );
	}

	Int4 MinAbs (const Int4 &x, const Int4 &y) {
		return NodeFunctions::Instance()->Build< Int4 >( "MinAbs", x, y );
	}

	Int4 MinAbs (const Int &x, const Int4 &y) {
		return NodeFunctions::Instance()->Build< Int4 >( "MinAbs", x, y );
	}

	Int4 MinAbs (const Int4 &x, const Int &y) {
		return NodeFunctions::Instance()->Build< Int4 >( "MinAbs", x, y );
	}

	UInt  MinAbs (const UInt  &x, const UInt  &y) {
		return NodeFunctions::Instance()->Build< UInt  >( "MinAbs", x, y );
	}

	UInt2 MinAbs (const UInt2 &x, const UInt2 &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "MinAbs", x, y );
	}

	UInt2 MinAbs (const UInt &x, const UInt2 &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "MinAbs", x, y );
	}

	UInt2 MinAbs (const UInt2 &x, const UInt &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "MinAbs", x, y );
	}

	UInt3 MinAbs (const UInt3 &x, const UInt3 &y) {
		return NodeFunctions::Instance()->Build< UInt3 >( "MinAbs", x, y );
	}

	UInt3 MinAbs (const UInt &x, const UInt3 &y) {
		return NodeFunctions::Instance()->Build< UInt3 >( "MinAbs", x, y );
	}

	UInt3 MinAbs (const UInt3 &x, const UInt &y) {
		return NodeFunctions::Instance()->Build< UInt3 >( "MinAbs", x, y );
	}

	UInt4 MinAbs (const UInt4 &x, const UInt4 &y) {
		return NodeFunctions::Instance()->Build< UInt4 >( "MinAbs", x, y );
	}

	UInt4 MinAbs (const UInt &x, const UInt4 &y) {
		return NodeFunctions::Instance()->Build< UInt4 >( "MinAbs", x, y );
	}

	UInt4 MinAbs (const UInt4 &x, const UInt &y) {
		return NodeFunctions::Instance()->Build< UInt4 >( "MinAbs", x, y );
	}

	Long  MinAbs (const Long  &x, const Long  &y) {
		return NodeFunctions::Instance()->Build< Long  >( "MinAbs", x, y );
	}

	Long2 MinAbs (const Long2 &x, const Long2 &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "MinAbs", x, y );
	}

	Long2 MinAbs (const Long &x, const Long2 &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "MinAbs", x, y );
	}

	Long2 MinAbs (const Long2 &x, const Long &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "MinAbs", x, y );
	}

	Long3 MinAbs (const Long3 &x, const Long3 &y) {
		return NodeFunctions::Instance()->Build< Long3 >( "MinAbs", x, y );
	}

	Long3 MinAbs (const Long &x, const Long3 &y) {
		return NodeFunctions::Instance()->Build< Long3 >( "MinAbs", x, y );
	}

	Long3 MinAbs (const Long3 &x, const Long &y) {
		return NodeFunctions::Instance()->Build< Long3 >( "MinAbs", x, y );
	}

	Long4 MinAbs (const Long4 &x, const Long4 &y) {
		return NodeFunctions::Instance()->Build< Long4 >( "MinAbs", x, y );
	}

	Long4 MinAbs (const Long &x, const Long4 &y) {
		return NodeFunctions::Instance()->Build< Long4 >( "MinAbs", x, y );
	}

	Long4 MinAbs (const Long4 &x, const Long &y) {
		return NodeFunctions::Instance()->Build< Long4 >( "MinAbs", x, y );
	}

	ULong  MinAbs (const ULong  &x, const ULong  &y) {
		return NodeFunctions::Instance()->Build< ULong  >( "MinAbs", x, y );
	}

	ULong2 MinAbs (const ULong2 &x, const ULong2 &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "MinAbs", x, y );
	}

	ULong2 MinAbs (const ULong &x, const ULong2 &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "MinAbs", x, y );
	}

	ULong2 MinAbs (const ULong2 &x, const ULong &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "MinAbs", x, y );
	}

	ULong3 MinAbs (const ULong3 &x, const ULong3 &y) {
		return NodeFunctions::Instance()->Build< ULong3 >( "MinAbs", x, y );
	}

	ULong3 MinAbs (const ULong &x, const ULong3 &y) {
		return NodeFunctions::Instance()->Build< ULong3 >( "MinAbs", x, y );
	}

	ULong3 MinAbs (const ULong3 &x, const ULong &y) {
		return NodeFunctions::Instance()->Build< ULong3 >( "MinAbs", x, y );
	}

	ULong4 MinAbs (const ULong4 &x, const ULong4 &y) {
		return NodeFunctions::Instance()->Build< ULong4 >( "MinAbs", x, y );
	}

	ULong4 MinAbs (const ULong &x, const ULong4 &y) {
		return NodeFunctions::Instance()->Build< ULong4 >( "MinAbs", x, y );
	}

	ULong4 MinAbs (const ULong4 &x, const ULong &y) {
		return NodeFunctions::Instance()->Build< ULong4 >( "MinAbs", x, y );
	}

	Float  MinAbs (const Float  &x, const Float  &y) {
		return NodeFunctions::Instance()->Build< Float  >( "MinAbs", x, y );
	}

	Float2 MinAbs (const Float2 &x, const Float2 &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "MinAbs", x, y );
	}

	Float2 MinAbs (const Float &x, const Float2 &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "MinAbs", x, y );
	}

	Float2 MinAbs (const Float2 &x, const Float &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "MinAbs", x, y );
	}

	Float3 MinAbs (const Float3 &x, const Float3 &y) {
		return NodeFunctions::Instance()->Build< Float3 >( "MinAbs", x, y );
	}

	Float3 MinAbs (const Float &x, const Float3 &y) {
		return NodeFunctions::Instance()->Build< Float3 >( "MinAbs", x, y );
	}

	Float3 MinAbs (const Float3 &x, const Float &y) {
		return NodeFunctions::Instance()->Build< Float3 >( "MinAbs", x, y );
	}

	Float4 MinAbs (const Float4 &x, const Float4 &y) {
		return NodeFunctions::Instance()->Build< Float4 >( "MinAbs", x, y );
	}

	Float4 MinAbs (const Float &x, const Float4 &y) {
		return NodeFunctions::Instance()->Build< Float4 >( "MinAbs", x, y );
	}

	Float4 MinAbs (const Float4 &x, const Float &y) {
		return NodeFunctions::Instance()->Build< Float4 >( "MinAbs", x, y );
	}

	Double  MinAbs (const Double  &x, const Double  &y) {
		return NodeFunctions::Instance()->Build< Double  >( "MinAbs", x, y );
	}

	Double2 MinAbs (const Double2 &x, const Double2 &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "MinAbs", x, y );
	}

	Double2 MinAbs (const Double &x, const Double2 &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "MinAbs", x, y );
	}

	Double2 MinAbs (const Double2 &x, const Double &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "MinAbs", x, y );
	}

	Double3 MinAbs (const Double3 &x, const Double3 &y) {
		return NodeFunctions::Instance()->Build< Double3 >( "MinAbs", x, y );
	}

	Double3 MinAbs (const Double &x, const Double3 &y) {
		return NodeFunctions::Instance()->Build< Double3 >( "MinAbs", x, y );
	}

	Double3 MinAbs (const Double3 &x, const Double &y) {
		return NodeFunctions::Instance()->Build< Double3 >( "MinAbs", x, y );
	}

	Double4 MinAbs (const Double4 &x, const Double4 &y) {
		return NodeFunctions::Instance()->Build< Double4 >( "MinAbs", x, y );
	}

	Double4 MinAbs (const Double &x, const Double4 &y) {
		return NodeFunctions::Instance()->Build< Double4 >( "MinAbs", x, y );
	}

	Double4 MinAbs (const Double4 &x, const Double &y) {
		return NodeFunctions::Instance()->Build< Double4 >( "MinAbs", x, y );
	}

	Int  MaxAbs (const Int  &x, const Int  &y) {
		return NodeFunctions::Instance()->Build< Int  >( "MaxAbs", x, y );
	}

	Int2 MaxAbs (const Int2 &x, const Int2 &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "MaxAbs", x, y );
	}

	Int2 MaxAbs (const Int &x, const Int2 &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "MaxAbs", x, y );
	}

	Int2 MaxAbs (const Int2 &x, const Int &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "MaxAbs", x, y );
	}

	Int3 MaxAbs (const Int3 &x, const Int3 &y) {
		return NodeFunctions::Instance()->Build< Int3 >( "MaxAbs", x, y );
	}

	Int3 MaxAbs (const Int &x, const Int3 &y) {
		return NodeFunctions::Instance()->Build< Int3 >( "MaxAbs", x, y );
	}

	Int3 MaxAbs (const Int3 &x, const Int &y) {
		return NodeFunctions::Instance()->Build< Int3 >( "MaxAbs", x, y );
	}

	Int4 MaxAbs (const Int4 &x, const Int4 &y) {
		return NodeFunctions::Instance()->Build< Int4 >( "MaxAbs", x, y );
	}

	Int4 MaxAbs (const Int &x, const Int4 &y) {
		return NodeFunctions::Instance()->Build< Int4 >( "MaxAbs", x, y );
	}

	Int4 MaxAbs (const Int4 &x, const Int &y) {
		return NodeFunctions::Instance()->Build< Int4 >( "MaxAbs", x, y );
	}

	UInt  MaxAbs (const UInt  &x, const UInt  &y) {
		return NodeFunctions::Instance()->Build< UInt  >( "MaxAbs", x, y );
	}

	UInt2 MaxAbs (const UInt2 &x, const UInt2 &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "MaxAbs", x, y );
	}

	UInt2 MaxAbs (const UInt &x, const UInt2 &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "MaxAbs", x, y );
	}

	UInt2 MaxAbs (const UInt2 &x, const UInt &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "MaxAbs", x, y );
	}

	UInt3 MaxAbs (const UInt3 &x, const UInt3 &y) {
		return NodeFunctions::Instance()->Build< UInt3 >( "MaxAbs", x, y );
	}

	UInt3 MaxAbs (const UInt &x, const UInt3 &y) {
		return NodeFunctions::Instance()->Build< UInt3 >( "MaxAbs", x, y );
	}

	UInt3 MaxAbs (const UInt3 &x, const UInt &y) {
		return NodeFunctions::Instance()->Build< UInt3 >( "MaxAbs", x, y );
	}

	UInt4 MaxAbs (const UInt4 &x, const UInt4 &y) {
		return NodeFunctions::Instance()->Build< UInt4 >( "MaxAbs", x, y );
	}

	UInt4 MaxAbs (const UInt &x, const UInt4 &y) {
		return NodeFunctions::Instance()->Build< UInt4 >( "MaxAbs", x, y );
	}

	UInt4 MaxAbs (const UInt4 &x, const UInt &y) {
		return NodeFunctions::Instance()->Build< UInt4 >( "MaxAbs", x, y );
	}

	Long  MaxAbs (const Long  &x, const Long  &y) {
		return NodeFunctions::Instance()->Build< Long  >( "MaxAbs", x, y );
	}

	Long2 MaxAbs (const Long2 &x, const Long2 &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "MaxAbs", x, y );
	}

	Long2 MaxAbs (const Long &x, const Long2 &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "MaxAbs", x, y );
	}

	Long2 MaxAbs (const Long2 &x, const Long &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "MaxAbs", x, y );
	}

	Long3 MaxAbs (const Long3 &x, const Long3 &y) {
		return NodeFunctions::Instance()->Build< Long3 >( "MaxAbs", x, y );
	}

	Long3 MaxAbs (const Long &x, const Long3 &y) {
		return NodeFunctions::Instance()->Build< Long3 >( "MaxAbs", x, y );
	}

	Long3 MaxAbs (const Long3 &x, const Long &y) {
		return NodeFunctions::Instance()->Build< Long3 >( "MaxAbs", x, y );
	}

	Long4 MaxAbs (const Long4 &x, const Long4 &y) {
		return NodeFunctions::Instance()->Build< Long4 >( "MaxAbs", x, y );
	}

	Long4 MaxAbs (const Long &x, const Long4 &y) {
		return NodeFunctions::Instance()->Build< Long4 >( "MaxAbs", x, y );
	}

	Long4 MaxAbs (const Long4 &x, const Long &y) {
		return NodeFunctions::Instance()->Build< Long4 >( "MaxAbs", x, y );
	}

	ULong  MaxAbs (const ULong  &x, const ULong  &y) {
		return NodeFunctions::Instance()->Build< ULong  >( "MaxAbs", x, y );
	}

	ULong2 MaxAbs (const ULong2 &x, const ULong2 &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "MaxAbs", x, y );
	}

	ULong2 MaxAbs (const ULong &x, const ULong2 &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "MaxAbs", x, y );
	}

	ULong2 MaxAbs (const ULong2 &x, const ULong &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "MaxAbs", x, y );
	}

	ULong3 MaxAbs (const ULong3 &x, const ULong3 &y) {
		return NodeFunctions::Instance()->Build< ULong3 >( "MaxAbs", x, y );
	}

	ULong3 MaxAbs (const ULong &x, const ULong3 &y) {
		return NodeFunctions::Instance()->Build< ULong3 >( "MaxAbs", x, y );
	}

	ULong3 MaxAbs (const ULong3 &x, const ULong &y) {
		return NodeFunctions::Instance()->Build< ULong3 >( "MaxAbs", x, y );
	}

	ULong4 MaxAbs (const ULong4 &x, const ULong4 &y) {
		return NodeFunctions::Instance()->Build< ULong4 >( "MaxAbs", x, y );
	}

	ULong4 MaxAbs (const ULong &x, const ULong4 &y) {
		return NodeFunctions::Instance()->Build< ULong4 >( "MaxAbs", x, y );
	}

	ULong4 MaxAbs (const ULong4 &x, const ULong &y) {
		return NodeFunctions::Instance()->Build< ULong4 >( "MaxAbs", x, y );
	}

	Float  MaxAbs (const Float  &x, const Float  &y) {
		return NodeFunctions::Instance()->Build< Float  >( "MaxAbs", x, y );
	}

	Float2 MaxAbs (const Float2 &x, const Float2 &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "MaxAbs", x, y );
	}

	Float2 MaxAbs (const Float &x, const Float2 &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "MaxAbs", x, y );
	}

	Float2 MaxAbs (const Float2 &x, const Float &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "MaxAbs", x, y );
	}

	Float3 MaxAbs (const Float3 &x, const Float3 &y) {
		return NodeFunctions::Instance()->Build< Float3 >( "MaxAbs", x, y );
	}

	Float3 MaxAbs (const Float &x, const Float3 &y) {
		return NodeFunctions::Instance()->Build< Float3 >( "MaxAbs", x, y );
	}

	Float3 MaxAbs (const Float3 &x, const Float &y) {
		return NodeFunctions::Instance()->Build< Float3 >( "MaxAbs", x, y );
	}

	Float4 MaxAbs (const Float4 &x, const Float4 &y) {
		return NodeFunctions::Instance()->Build< Float4 >( "MaxAbs", x, y );
	}

	Float4 MaxAbs (const Float &x, const Float4 &y) {
		return NodeFunctions::Instance()->Build< Float4 >( "MaxAbs", x, y );
	}

	Float4 MaxAbs (const Float4 &x, const Float &y) {
		return NodeFunctions::Instance()->Build< Float4 >( "MaxAbs", x, y );
	}

	Double  MaxAbs (const Double  &x, const Double  &y) {
		return NodeFunctions::Instance()->Build< Double  >( "MaxAbs", x, y );
	}

	Double2 MaxAbs (const Double2 &x, const Double2 &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "MaxAbs", x, y );
	}

	Double2 MaxAbs (const Double &x, const Double2 &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "MaxAbs", x, y );
	}

	Double2 MaxAbs (const Double2 &x, const Double &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "MaxAbs", x, y );
	}

	Double3 MaxAbs (const Double3 &x, const Double3 &y) {
		return NodeFunctions::Instance()->Build< Double3 >( "MaxAbs", x, y );
	}

	Double3 MaxAbs (const Double &x, const Double3 &y) {
		return NodeFunctions::Instance()->Build< Double3 >( "MaxAbs", x, y );
	}

	Double3 MaxAbs (const Double3 &x, const Double &y) {
		return NodeFunctions::Instance()->Build< Double3 >( "MaxAbs", x, y );
	}

	Double4 MaxAbs (const Double4 &x, const Double4 &y) {
		return NodeFunctions::Instance()->Build< Double4 >( "MaxAbs", x, y );
	}

	Double4 MaxAbs (const Double &x, const Double4 &y) {
		return NodeFunctions::Instance()->Build< Double4 >( "MaxAbs", x, y );
	}

	Double4 MaxAbs (const Double4 &x, const Double &y) {
		return NodeFunctions::Instance()->Build< Double4 >( "MaxAbs", x, y );
	}

	Int  MinMag (const Int  &x, const Int  &y) {
		return NodeFunctions::Instance()->Build< Int  >( "MinMag", x, y );
	}

	Int2 MinMag (const Int2 &x, const Int2 &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "MinMag", x, y );
	}

	Int2 MinMag (const Int &x, const Int2 &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "MinMag", x, y );
	}

	Int2 MinMag (const Int2 &x, const Int &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "MinMag", x, y );
	}

	Int3 MinMag (const Int3 &x, const Int3 &y) {
		return NodeFunctions::Instance()->Build< Int3 >( "MinMag", x, y );
	}

	Int3 MinMag (const Int &x, const Int3 &y) {
		return NodeFunctions::Instance()->Build< Int3 >( "MinMag", x, y );
	}

	Int3 MinMag (const Int3 &x, const Int &y) {
		return NodeFunctions::Instance()->Build< Int3 >( "MinMag", x, y );
	}

	Int4 MinMag (const Int4 &x, const Int4 &y) {
		return NodeFunctions::Instance()->Build< Int4 >( "MinMag", x, y );
	}

	Int4 MinMag (const Int &x, const Int4 &y) {
		return NodeFunctions::Instance()->Build< Int4 >( "MinMag", x, y );
	}

	Int4 MinMag (const Int4 &x, const Int &y) {
		return NodeFunctions::Instance()->Build< Int4 >( "MinMag", x, y );
	}

	UInt  MinMag (const UInt  &x, const UInt  &y) {
		return NodeFunctions::Instance()->Build< UInt  >( "MinMag", x, y );
	}

	UInt2 MinMag (const UInt2 &x, const UInt2 &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "MinMag", x, y );
	}

	UInt2 MinMag (const UInt &x, const UInt2 &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "MinMag", x, y );
	}

	UInt2 MinMag (const UInt2 &x, const UInt &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "MinMag", x, y );
	}

	UInt3 MinMag (const UInt3 &x, const UInt3 &y) {
		return NodeFunctions::Instance()->Build< UInt3 >( "MinMag", x, y );
	}

	UInt3 MinMag (const UInt &x, const UInt3 &y) {
		return NodeFunctions::Instance()->Build< UInt3 >( "MinMag", x, y );
	}

	UInt3 MinMag (const UInt3 &x, const UInt &y) {
		return NodeFunctions::Instance()->Build< UInt3 >( "MinMag", x, y );
	}

	UInt4 MinMag (const UInt4 &x, const UInt4 &y) {
		return NodeFunctions::Instance()->Build< UInt4 >( "MinMag", x, y );
	}

	UInt4 MinMag (const UInt &x, const UInt4 &y) {
		return NodeFunctions::Instance()->Build< UInt4 >( "MinMag", x, y );
	}

	UInt4 MinMag (const UInt4 &x, const UInt &y) {
		return NodeFunctions::Instance()->Build< UInt4 >( "MinMag", x, y );
	}

	Long  MinMag (const Long  &x, const Long  &y) {
		return NodeFunctions::Instance()->Build< Long  >( "MinMag", x, y );
	}

	Long2 MinMag (const Long2 &x, const Long2 &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "MinMag", x, y );
	}

	Long2 MinMag (const Long &x, const Long2 &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "MinMag", x, y );
	}

	Long2 MinMag (const Long2 &x, const Long &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "MinMag", x, y );
	}

	Long3 MinMag (const Long3 &x, const Long3 &y) {
		return NodeFunctions::Instance()->Build< Long3 >( "MinMag", x, y );
	}

	Long3 MinMag (const Long &x, const Long3 &y) {
		return NodeFunctions::Instance()->Build< Long3 >( "MinMag", x, y );
	}

	Long3 MinMag (const Long3 &x, const Long &y) {
		return NodeFunctions::Instance()->Build< Long3 >( "MinMag", x, y );
	}

	Long4 MinMag (const Long4 &x, const Long4 &y) {
		return NodeFunctions::Instance()->Build< Long4 >( "MinMag", x, y );
	}

	Long4 MinMag (const Long &x, const Long4 &y) {
		return NodeFunctions::Instance()->Build< Long4 >( "MinMag", x, y );
	}

	Long4 MinMag (const Long4 &x, const Long &y) {
		return NodeFunctions::Instance()->Build< Long4 >( "MinMag", x, y );
	}

	ULong  MinMag (const ULong  &x, const ULong  &y) {
		return NodeFunctions::Instance()->Build< ULong  >( "MinMag", x, y );
	}

	ULong2 MinMag (const ULong2 &x, const ULong2 &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "MinMag", x, y );
	}

	ULong2 MinMag (const ULong &x, const ULong2 &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "MinMag", x, y );
	}

	ULong2 MinMag (const ULong2 &x, const ULong &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "MinMag", x, y );
	}

	ULong3 MinMag (const ULong3 &x, const ULong3 &y) {
		return NodeFunctions::Instance()->Build< ULong3 >( "MinMag", x, y );
	}

	ULong3 MinMag (const ULong &x, const ULong3 &y) {
		return NodeFunctions::Instance()->Build< ULong3 >( "MinMag", x, y );
	}

	ULong3 MinMag (const ULong3 &x, const ULong &y) {
		return NodeFunctions::Instance()->Build< ULong3 >( "MinMag", x, y );
	}

	ULong4 MinMag (const ULong4 &x, const ULong4 &y) {
		return NodeFunctions::Instance()->Build< ULong4 >( "MinMag", x, y );
	}

	ULong4 MinMag (const ULong &x, const ULong4 &y) {
		return NodeFunctions::Instance()->Build< ULong4 >( "MinMag", x, y );
	}

	ULong4 MinMag (const ULong4 &x, const ULong &y) {
		return NodeFunctions::Instance()->Build< ULong4 >( "MinMag", x, y );
	}

	Float  MinMag (const Float  &x, const Float  &y) {
		return NodeFunctions::Instance()->Build< Float  >( "MinMag", x, y );
	}

	Float2 MinMag (const Float2 &x, const Float2 &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "MinMag", x, y );
	}

	Float2 MinMag (const Float &x, const Float2 &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "MinMag", x, y );
	}

	Float2 MinMag (const Float2 &x, const Float &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "MinMag", x, y );
	}

	Float3 MinMag (const Float3 &x, const Float3 &y) {
		return NodeFunctions::Instance()->Build< Float3 >( "MinMag", x, y );
	}

	Float3 MinMag (const Float &x, const Float3 &y) {
		return NodeFunctions::Instance()->Build< Float3 >( "MinMag", x, y );
	}

	Float3 MinMag (const Float3 &x, const Float &y) {
		return NodeFunctions::Instance()->Build< Float3 >( "MinMag", x, y );
	}

	Float4 MinMag (const Float4 &x, const Float4 &y) {
		return NodeFunctions::Instance()->Build< Float4 >( "MinMag", x, y );
	}

	Float4 MinMag (const Float &x, const Float4 &y) {
		return NodeFunctions::Instance()->Build< Float4 >( "MinMag", x, y );
	}

	Float4 MinMag (const Float4 &x, const Float &y) {
		return NodeFunctions::Instance()->Build< Float4 >( "MinMag", x, y );
	}

	Double  MinMag (const Double  &x, const Double  &y) {
		return NodeFunctions::Instance()->Build< Double  >( "MinMag", x, y );
	}

	Double2 MinMag (const Double2 &x, const Double2 &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "MinMag", x, y );
	}

	Double2 MinMag (const Double &x, const Double2 &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "MinMag", x, y );
	}

	Double2 MinMag (const Double2 &x, const Double &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "MinMag", x, y );
	}

	Double3 MinMag (const Double3 &x, const Double3 &y) {
		return NodeFunctions::Instance()->Build< Double3 >( "MinMag", x, y );
	}

	Double3 MinMag (const Double &x, const Double3 &y) {
		return NodeFunctions::Instance()->Build< Double3 >( "MinMag", x, y );
	}

	Double3 MinMag (const Double3 &x, const Double &y) {
		return NodeFunctions::Instance()->Build< Double3 >( "MinMag", x, y );
	}

	Double4 MinMag (const Double4 &x, const Double4 &y) {
		return NodeFunctions::Instance()->Build< Double4 >( "MinMag", x, y );
	}

	Double4 MinMag (const Double &x, const Double4 &y) {
		return NodeFunctions::Instance()->Build< Double4 >( "MinMag", x, y );
	}

	Double4 MinMag (const Double4 &x, const Double &y) {
		return NodeFunctions::Instance()->Build< Double4 >( "MinMag", x, y );
	}

	Int  MaxMag (const Int  &x, const Int  &y) {
		return NodeFunctions::Instance()->Build< Int  >( "MaxMag", x, y );
	}

	Int2 MaxMag (const Int2 &x, const Int2 &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "MaxMag", x, y );
	}

	Int2 MaxMag (const Int &x, const Int2 &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "MaxMag", x, y );
	}

	Int2 MaxMag (const Int2 &x, const Int &y) {
		return NodeFunctions::Instance()->Build< Int2 >( "MaxMag", x, y );
	}

	Int3 MaxMag (const Int3 &x, const Int3 &y) {
		return NodeFunctions::Instance()->Build< Int3 >( "MaxMag", x, y );
	}

	Int3 MaxMag (const Int &x, const Int3 &y) {
		return NodeFunctions::Instance()->Build< Int3 >( "MaxMag", x, y );
	}

	Int3 MaxMag (const Int3 &x, const Int &y) {
		return NodeFunctions::Instance()->Build< Int3 >( "MaxMag", x, y );
	}

	Int4 MaxMag (const Int4 &x, const Int4 &y) {
		return NodeFunctions::Instance()->Build< Int4 >( "MaxMag", x, y );
	}

	Int4 MaxMag (const Int &x, const Int4 &y) {
		return NodeFunctions::Instance()->Build< Int4 >( "MaxMag", x, y );
	}

	Int4 MaxMag (const Int4 &x, const Int &y) {
		return NodeFunctions::Instance()->Build< Int4 >( "MaxMag", x, y );
	}

	UInt  MaxMag (const UInt  &x, const UInt  &y) {
		return NodeFunctions::Instance()->Build< UInt  >( "MaxMag", x, y );
	}

	UInt2 MaxMag (const UInt2 &x, const UInt2 &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "MaxMag", x, y );
	}

	UInt2 MaxMag (const UInt &x, const UInt2 &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "MaxMag", x, y );
	}

	UInt2 MaxMag (const UInt2 &x, const UInt &y) {
		return NodeFunctions::Instance()->Build< UInt2 >( "MaxMag", x, y );
	}

	UInt3 MaxMag (const UInt3 &x, const UInt3 &y) {
		return NodeFunctions::Instance()->Build< UInt3 >( "MaxMag", x, y );
	}

	UInt3 MaxMag (const UInt &x, const UInt3 &y) {
		return NodeFunctions::Instance()->Build< UInt3 >( "MaxMag", x, y );
	}

	UInt3 MaxMag (const UInt3 &x, const UInt &y) {
		return NodeFunctions::Instance()->Build< UInt3 >( "MaxMag", x, y );
	}

	UInt4 MaxMag (const UInt4 &x, const UInt4 &y) {
		return NodeFunctions::Instance()->Build< UInt4 >( "MaxMag", x, y );
	}

	UInt4 MaxMag (const UInt &x, const UInt4 &y) {
		return NodeFunctions::Instance()->Build< UInt4 >( "MaxMag", x, y );
	}

	UInt4 MaxMag (const UInt4 &x, const UInt &y) {
		return NodeFunctions::Instance()->Build< UInt4 >( "MaxMag", x, y );
	}

	Long  MaxMag (const Long  &x, const Long  &y) {
		return NodeFunctions::Instance()->Build< Long  >( "MaxMag", x, y );
	}

	Long2 MaxMag (const Long2 &x, const Long2 &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "MaxMag", x, y );
	}

	Long2 MaxMag (const Long &x, const Long2 &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "MaxMag", x, y );
	}

	Long2 MaxMag (const Long2 &x, const Long &y) {
		return NodeFunctions::Instance()->Build< Long2 >( "MaxMag", x, y );
	}

	Long3 MaxMag (const Long3 &x, const Long3 &y) {
		return NodeFunctions::Instance()->Build< Long3 >( "MaxMag", x, y );
	}

	Long3 MaxMag (const Long &x, const Long3 &y) {
		return NodeFunctions::Instance()->Build< Long3 >( "MaxMag", x, y );
	}

	Long3 MaxMag (const Long3 &x, const Long &y) {
		return NodeFunctions::Instance()->Build< Long3 >( "MaxMag", x, y );
	}

	Long4 MaxMag (const Long4 &x, const Long4 &y) {
		return NodeFunctions::Instance()->Build< Long4 >( "MaxMag", x, y );
	}

	Long4 MaxMag (const Long &x, const Long4 &y) {
		return NodeFunctions::Instance()->Build< Long4 >( "MaxMag", x, y );
	}

	Long4 MaxMag (const Long4 &x, const Long &y) {
		return NodeFunctions::Instance()->Build< Long4 >( "MaxMag", x, y );
	}

	ULong  MaxMag (const ULong  &x, const ULong  &y) {
		return NodeFunctions::Instance()->Build< ULong  >( "MaxMag", x, y );
	}

	ULong2 MaxMag (const ULong2 &x, const ULong2 &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "MaxMag", x, y );
	}

	ULong2 MaxMag (const ULong &x, const ULong2 &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "MaxMag", x, y );
	}

	ULong2 MaxMag (const ULong2 &x, const ULong &y) {
		return NodeFunctions::Instance()->Build< ULong2 >( "MaxMag", x, y );
	}

	ULong3 MaxMag (const ULong3 &x, const ULong3 &y) {
		return NodeFunctions::Instance()->Build< ULong3 >( "MaxMag", x, y );
	}

	ULong3 MaxMag (const ULong &x, const ULong3 &y) {
		return NodeFunctions::Instance()->Build< ULong3 >( "MaxMag", x, y );
	}

	ULong3 MaxMag (const ULong3 &x, const ULong &y) {
		return NodeFunctions::Instance()->Build< ULong3 >( "MaxMag", x, y );
	}

	ULong4 MaxMag (const ULong4 &x, const ULong4 &y) {
		return NodeFunctions::Instance()->Build< ULong4 >( "MaxMag", x, y );
	}

	ULong4 MaxMag (const ULong &x, const ULong4 &y) {
		return NodeFunctions::Instance()->Build< ULong4 >( "MaxMag", x, y );
	}

	ULong4 MaxMag (const ULong4 &x, const ULong &y) {
		return NodeFunctions::Instance()->Build< ULong4 >( "MaxMag", x, y );
	}

	Float  MaxMag (const Float  &x, const Float  &y) {
		return NodeFunctions::Instance()->Build< Float  >( "MaxMag", x, y );
	}

	Float2 MaxMag (const Float2 &x, const Float2 &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "MaxMag", x, y );
	}

	Float2 MaxMag (const Float &x, const Float2 &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "MaxMag", x, y );
	}

	Float2 MaxMag (const Float2 &x, const Float &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "MaxMag", x, y );
	}

	Float3 MaxMag (const Float3 &x, const Float3 &y) {
		return NodeFunctions::Instance()->Build< Float3 >( "MaxMag", x, y );
	}

	Float3 MaxMag (const Float &x, const Float3 &y) {
		return NodeFunctions::Instance()->Build< Float3 >( "MaxMag", x, y );
	}

	Float3 MaxMag (const Float3 &x, const Float &y) {
		return NodeFunctions::Instance()->Build< Float3 >( "MaxMag", x, y );
	}

	Float4 MaxMag (const Float4 &x, const Float4 &y) {
		return NodeFunctions::Instance()->Build< Float4 >( "MaxMag", x, y );
	}

	Float4 MaxMag (const Float &x, const Float4 &y) {
		return NodeFunctions::Instance()->Build< Float4 >( "MaxMag", x, y );
	}

	Float4 MaxMag (const Float4 &x, const Float &y) {
		return NodeFunctions::Instance()->Build< Float4 >( "MaxMag", x, y );
	}

	Double  MaxMag (const Double  &x, const Double  &y) {
		return NodeFunctions::Instance()->Build< Double  >( "MaxMag", x, y );
	}

	Double2 MaxMag (const Double2 &x, const Double2 &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "MaxMag", x, y );
	}

	Double2 MaxMag (const Double &x, const Double2 &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "MaxMag", x, y );
	}

	Double2 MaxMag (const Double2 &x, const Double &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "MaxMag", x, y );
	}

	Double3 MaxMag (const Double3 &x, const Double3 &y) {
		return NodeFunctions::Instance()->Build< Double3 >( "MaxMag", x, y );
	}

	Double3 MaxMag (const Double &x, const Double3 &y) {
		return NodeFunctions::Instance()->Build< Double3 >( "MaxMag", x, y );
	}

	Double3 MaxMag (const Double3 &x, const Double &y) {
		return NodeFunctions::Instance()->Build< Double3 >( "MaxMag", x, y );
	}

	Double4 MaxMag (const Double4 &x, const Double4 &y) {
		return NodeFunctions::Instance()->Build< Double4 >( "MaxMag", x, y );
	}

	Double4 MaxMag (const Double &x, const Double4 &y) {
		return NodeFunctions::Instance()->Build< Double4 >( "MaxMag", x, y );
	}

	Double4 MaxMag (const Double4 &x, const Double &y) {
		return NodeFunctions::Instance()->Build< Double4 >( "MaxMag", x, y );
	}

	Int  Mid (const Int  &x, const Int  &y, const Int  &z) {
		return NodeFunctions::Instance()->Build< Int  >( "Mid", x, y, z );
	}

	Int2 Mid (const Int2 &x, const Int2 &y, const Int2 &z) {
		return NodeFunctions::Instance()->Build< Int2 >( "Mid", x, y, z );
	}

	Int3 Mid (const Int3 &x, const Int3 &y, const Int3 &z) {
		return NodeFunctions::Instance()->Build< Int3 >( "Mid", x, y, z );
	}

	Int4 Mid (const Int4 &x, const Int4 &y, const Int4 &z) {
		return NodeFunctions::Instance()->Build< Int4 >( "Mid", x, y, z );
	}

	UInt  Mid (const UInt  &x, const UInt  &y, const UInt  &z) {
		return NodeFunctions::Instance()->Build< UInt  >( "Mid", x, y, z );
	}

	UInt2 Mid (const UInt2 &x, const UInt2 &y, const UInt2 &z) {
		return NodeFunctions::Instance()->Build< UInt2 >( "Mid", x, y, z );
	}

	UInt3 Mid (const UInt3 &x, const UInt3 &y, const UInt3 &z) {
		return NodeFunctions::Instance()->Build< UInt3 >( "Mid", x, y, z );
	}

	UInt4 Mid (const UInt4 &x, const UInt4 &y, const UInt4 &z) {
		return NodeFunctions::Instance()->Build< UInt4 >( "Mid", x, y, z );
	}

	Long  Mid (const Long  &x, const Long  &y, const Long  &z) {
		return NodeFunctions::Instance()->Build< Long  >( "Mid", x, y, z );
	}

	Long2 Mid (const Long2 &x, const Long2 &y, const Long2 &z) {
		return NodeFunctions::Instance()->Build< Long2 >( "Mid", x, y, z );
	}

	Long3 Mid (const Long3 &x, const Long3 &y, const Long3 &z) {
		return NodeFunctions::Instance()->Build< Long3 >( "Mid", x, y, z );
	}

	Long4 Mid (const Long4 &x, const Long4 &y, const Long4 &z) {
		return NodeFunctions::Instance()->Build< Long4 >( "Mid", x, y, z );
	}

	ULong  Mid (const ULong  &x, const ULong  &y, const ULong  &z) {
		return NodeFunctions::Instance()->Build< ULong  >( "Mid", x, y, z );
	}

	ULong2 Mid (const ULong2 &x, const ULong2 &y, const ULong2 &z) {
		return NodeFunctions::Instance()->Build< ULong2 >( "Mid", x, y, z );
	}

	ULong3 Mid (const ULong3 &x, const ULong3 &y, const ULong3 &z) {
		return NodeFunctions::Instance()->Build< ULong3 >( "Mid", x, y, z );
	}

	ULong4 Mid (const ULong4 &x, const ULong4 &y, const ULong4 &z) {
		return NodeFunctions::Instance()->Build< ULong4 >( "Mid", x, y, z );
	}

	Float  Mid (const Float  &x, const Float  &y, const Float  &z) {
		return NodeFunctions::Instance()->Build< Float  >( "Mid", x, y, z );
	}

	Float2 Mid (const Float2 &x, const Float2 &y, const Float2 &z) {
		return NodeFunctions::Instance()->Build< Float2 >( "Mid", x, y, z );
	}

	Float3 Mid (const Float3 &x, const Float3 &y, const Float3 &z) {
		return NodeFunctions::Instance()->Build< Float3 >( "Mid", x, y, z );
	}

	Float4 Mid (const Float4 &x, const Float4 &y, const Float4 &z) {
		return NodeFunctions::Instance()->Build< Float4 >( "Mid", x, y, z );
	}

	Double  Mid (const Double  &x, const Double  &y, const Double  &z) {
		return NodeFunctions::Instance()->Build< Double  >( "Mid", x, y, z );
	}

	Double2 Mid (const Double2 &x, const Double2 &y, const Double2 &z) {
		return NodeFunctions::Instance()->Build< Double2 >( "Mid", x, y, z );
	}

	Double3 Mid (const Double3 &x, const Double3 &y, const Double3 &z) {
		return NodeFunctions::Instance()->Build< Double3 >( "Mid", x, y, z );
	}

	Double4 Mid (const Double4 &x, const Double4 &y, const Double4 &z) {
		return NodeFunctions::Instance()->Build< Double4 >( "Mid", x, y, z );
	}


	// Dot, Cross, Length, Distance, Normalize, FaceForward, OuterProduct, Reflect, Refract
	Float  Length (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "Length", x );
	}

	Float2 Length (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Length", x );
	}

	Float3 Length (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Length", x );
	}

	Float4 Length (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Length", x );
	}

	Double  Length (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "Length", x );
	}

	Double2 Length (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Length", x );
	}

	Double3 Length (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Length", x );
	}

	Double4 Length (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Length", x );
	}

	Float  Normalize (const Float  &x) {
		return NodeFunctions::Instance()->Build< Float  >( "Normalize", x );
	}

	Float2 Normalize (const Float2 &x) {
		return NodeFunctions::Instance()->Build< Float2 >( "Normalize", x );
	}

	Float3 Normalize (const Float3 &x) {
		return NodeFunctions::Instance()->Build< Float3 >( "Normalize", x );
	}

	Float4 Normalize (const Float4 &x) {
		return NodeFunctions::Instance()->Build< Float4 >( "Normalize", x );
	}

	Double  Normalize (const Double  &x) {
		return NodeFunctions::Instance()->Build< Double  >( "Normalize", x );
	}

	Double2 Normalize (const Double2 &x) {
		return NodeFunctions::Instance()->Build< Double2 >( "Normalize", x );
	}

	Double3 Normalize (const Double3 &x) {
		return NodeFunctions::Instance()->Build< Double3 >( "Normalize", x );
	}

	Double4 Normalize (const Double4 &x) {
		return NodeFunctions::Instance()->Build< Double4 >( "Normalize", x );
	}

	Float2 Dot (const Float2 &x, const Float2 &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "Dot", x, y );
	}

	Double2 Dot (const Double2 &x, const Double2 &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "Dot", x, y );
	}

	Float3 Cross (const Float3 &x, const Float3 &y) {
		return NodeFunctions::Instance()->Build< Float3 >( "Cross", x, y );
	}

	Double3 Cross (const Double3 &x, const Double3 &y) {
		return NodeFunctions::Instance()->Build< Double3 >( "Cross", x, y );
	}

	Float  Distance (const Float  &x, const Float  &y) {
		return NodeFunctions::Instance()->Build< Float  >( "Distance", x, y );
	}

	Float2 Distance (const Float2 &x, const Float2 &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "Distance", x, y );
	}

	Float3 Distance (const Float3 &x, const Float3 &y) {
		return NodeFunctions::Instance()->Build< Float3 >( "Distance", x, y );
	}

	Float4 Distance (const Float4 &x, const Float4 &y) {
		return NodeFunctions::Instance()->Build< Float4 >( "Distance", x, y );
	}

	Double  Distance (const Double  &x, const Double  &y) {
		return NodeFunctions::Instance()->Build< Double  >( "Distance", x, y );
	}

	Double2 Distance (const Double2 &x, const Double2 &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "Distance", x, y );
	}

	Double3 Distance (const Double3 &x, const Double3 &y) {
		return NodeFunctions::Instance()->Build< Double3 >( "Distance", x, y );
	}

	Double4 Distance (const Double4 &x, const Double4 &y) {
		return NodeFunctions::Instance()->Build< Double4 >( "Distance", x, y );
	}

	Float  Reflect (const Float  &x, const Float  &y) {
		return NodeFunctions::Instance()->Build< Float  >( "Reflect", x, y );
	}

	Float2 Reflect (const Float2 &x, const Float2 &y) {
		return NodeFunctions::Instance()->Build< Float2 >( "Reflect", x, y );
	}

	Float3 Reflect (const Float3 &x, const Float3 &y) {
		return NodeFunctions::Instance()->Build< Float3 >( "Reflect", x, y );
	}

	Float4 Reflect (const Float4 &x, const Float4 &y) {
		return NodeFunctions::Instance()->Build< Float4 >( "Reflect", x, y );
	}

	Double  Reflect (const Double  &x, const Double  &y) {
		return NodeFunctions::Instance()->Build< Double  >( "Reflect", x, y );
	}

	Double2 Reflect (const Double2 &x, const Double2 &y) {
		return NodeFunctions::Instance()->Build< Double2 >( "Reflect", x, y );
	}

	Double3 Reflect (const Double3 &x, const Double3 &y) {
		return NodeFunctions::Instance()->Build< Double3 >( "Reflect", x, y );
	}

	Double4 Reflect (const Double4 &x, const Double4 &y) {
		return NodeFunctions::Instance()->Build< Double4 >( "Reflect", x, y );
	}

	Float  Refract (const Float  &I, const Float  &N, const Float &eta) {
		return NodeFunctions::Instance()->Build< Float  >( "Refract", I, N, eta );
	}

	Float2 Refract (const Float2 &I, const Float2 &N, const Float &eta) {
		return NodeFunctions::Instance()->Build< Float2 >( "Refract", I, N, eta );
	}

	Float3 Refract (const Float3 &I, const Float3 &N, const Float &eta) {
		return NodeFunctions::Instance()->Build< Float3 >( "Refract", I, N, eta );
	}

	Float4 Refract (const Float4 &I, const Float4 &N, const Float &eta) {
		return NodeFunctions::Instance()->Build< Float4 >( "Refract", I, N, eta );
	}

	Double  Refract (const Double  &I, const Double  &N, const Double &eta) {
		return NodeFunctions::Instance()->Build< Double  >( "Refract", I, N, eta );
	}

	Double2 Refract (const Double2 &I, const Double2 &N, const Double &eta) {
		return NodeFunctions::Instance()->Build< Double2 >( "Refract", I, N, eta );
	}

	Double3 Refract (const Double3 &I, const Double3 &N, const Double &eta) {
		return NodeFunctions::Instance()->Build< Double3 >( "Refract", I, N, eta );
	}

	Double4 Refract (const Double4 &I, const Double4 &N, const Double &eta) {
		return NodeFunctions::Instance()->Build< Double4 >( "Refract", I, N, eta );
	}

	Float  FaceForward (const Float  &N, const Float  &I, const Float  &Nref) {
		return NodeFunctions::Instance()->Build< Float  >( "FaceForward", N, I, Nref );
	}

	Float2 FaceForward (const Float2 &N, const Float2 &I, const Float2 &Nref) {
		return NodeFunctions::Instance()->Build< Float2 >( "FaceForward", N, I, Nref );
	}

	Float3 FaceForward (const Float3 &N, const Float3 &I, const Float3 &Nref) {
		return NodeFunctions::Instance()->Build< Float3 >( "FaceForward", N, I, Nref );
	}

	Float4 FaceForward (const Float4 &N, const Float4 &I, const Float4 &Nref) {
		return NodeFunctions::Instance()->Build< Float4 >( "FaceForward", N, I, Nref );
	}

	Double  FaceForward (const Double  &N, const Double  &I, const Double  &Nref) {
		return NodeFunctions::Instance()->Build< Double  >( "FaceForward", N, I, Nref );
	}

	Double2 FaceForward (const Double2 &N, const Double2 &I, const Double2 &Nref) {
		return NodeFunctions::Instance()->Build< Double2 >( "FaceForward", N, I, Nref );
	}

	Double3 FaceForward (const Double3 &N, const Double3 &I, const Double3 &Nref) {
		return NodeFunctions::Instance()->Build< Double3 >( "FaceForward", N, I, Nref );
	}

	Double4 FaceForward (const Double4 &N, const Double4 &I, const Double4 &Nref) {
		return NodeFunctions::Instance()->Build< Double4 >( "FaceForward", N, I, Nref );
	}


	// Inverse, Translate, Transpose, Determinant

	// dFdx, dFdy, FWidth

	// Pack***, Unpack***

	// ImageLoad, ImageStore
	Int4 ImageLoad (const Image<ImageType1D, ReadOnly, Int4> &image, const Int &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageType1D, ReadOnly, UInt4> &image, const Int &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageType1D, ReadOnly, Float4> &image, const Int &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageType1D, Coherent, Int4> &image, const Int &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageType1D, Coherent, UInt4> &image, const Int &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageType1D, Coherent, Float4> &image, const Int &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageType1D, Volatile, Int4> &image, const Int &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageType1D, Volatile, UInt4> &image, const Int &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageType1D, Volatile, Float4> &image, const Int &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageType1D, Restrict, Int4> &image, const Int &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageType1D, Restrict, UInt4> &image, const Int &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageType1D, Restrict, Float4> &image, const Int &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageType1DArray, ReadOnly, Int4> &image, const Int2 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageType1DArray, ReadOnly, UInt4> &image, const Int2 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageType1DArray, ReadOnly, Float4> &image, const Int2 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageType1DArray, Coherent, Int4> &image, const Int2 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageType1DArray, Coherent, UInt4> &image, const Int2 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageType1DArray, Coherent, Float4> &image, const Int2 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageType1DArray, Volatile, Int4> &image, const Int2 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageType1DArray, Volatile, UInt4> &image, const Int2 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageType1DArray, Volatile, Float4> &image, const Int2 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageType1DArray, Restrict, Int4> &image, const Int2 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageType1DArray, Restrict, UInt4> &image, const Int2 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageType1DArray, Restrict, Float4> &image, const Int2 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageType2D, ReadOnly, Int4> &image, const Int2 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageType2D, ReadOnly, UInt4> &image, const Int2 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageType2D, ReadOnly, Float4> &image, const Int2 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageType2D, Coherent, Int4> &image, const Int2 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageType2D, Coherent, UInt4> &image, const Int2 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageType2D, Coherent, Float4> &image, const Int2 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageType2D, Volatile, Int4> &image, const Int2 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageType2D, Volatile, UInt4> &image, const Int2 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageType2D, Volatile, Float4> &image, const Int2 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageType2D, Restrict, Int4> &image, const Int2 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageType2D, Restrict, UInt4> &image, const Int2 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageType2D, Restrict, Float4> &image, const Int2 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageType2DArray, ReadOnly, Int4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageType2DArray, ReadOnly, UInt4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageType2DArray, ReadOnly, Float4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageType2DArray, Coherent, Int4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageType2DArray, Coherent, UInt4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageType2DArray, Coherent, Float4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageType2DArray, Volatile, Int4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageType2DArray, Volatile, UInt4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageType2DArray, Volatile, Float4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageType2DArray, Restrict, Int4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageType2DArray, Restrict, UInt4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageType2DArray, Restrict, Float4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageType2DMS, ReadOnly, Int4> &image, const Int2 &point, const Int &sample) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point, sample );
	}

	UInt4 ImageLoad (const Image<ImageType2DMS, ReadOnly, UInt4> &image, const Int2 &point, const Int &sample) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point, sample );
	}

	Float4 ImageLoad (const Image<ImageType2DMS, ReadOnly, Float4> &image, const Int2 &point, const Int &sample) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point, sample );
	}

	Int4 ImageLoad (const Image<ImageType2DMS, Coherent, Int4> &image, const Int2 &point, const Int &sample) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point, sample );
	}

	UInt4 ImageLoad (const Image<ImageType2DMS, Coherent, UInt4> &image, const Int2 &point, const Int &sample) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point, sample );
	}

	Float4 ImageLoad (const Image<ImageType2DMS, Coherent, Float4> &image, const Int2 &point, const Int &sample) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point, sample );
	}

	Int4 ImageLoad (const Image<ImageType2DMS, Volatile, Int4> &image, const Int2 &point, const Int &sample) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point, sample );
	}

	UInt4 ImageLoad (const Image<ImageType2DMS, Volatile, UInt4> &image, const Int2 &point, const Int &sample) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point, sample );
	}

	Float4 ImageLoad (const Image<ImageType2DMS, Volatile, Float4> &image, const Int2 &point, const Int &sample) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point, sample );
	}

	Int4 ImageLoad (const Image<ImageType2DMS, Restrict, Int4> &image, const Int2 &point, const Int &sample) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point, sample );
	}

	UInt4 ImageLoad (const Image<ImageType2DMS, Restrict, UInt4> &image, const Int2 &point, const Int &sample) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point, sample );
	}

	Float4 ImageLoad (const Image<ImageType2DMS, Restrict, Float4> &image, const Int2 &point, const Int &sample) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point, sample );
	}

	Int4 ImageLoad (const Image<ImageType2DMSArray, ReadOnly, Int4> &image, const Int3 &point, const Int &sample) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point, sample );
	}

	UInt4 ImageLoad (const Image<ImageType2DMSArray, ReadOnly, UInt4> &image, const Int3 &point, const Int &sample) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point, sample );
	}

	Float4 ImageLoad (const Image<ImageType2DMSArray, ReadOnly, Float4> &image, const Int3 &point, const Int &sample) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point, sample );
	}

	Int4 ImageLoad (const Image<ImageType2DMSArray, Coherent, Int4> &image, const Int3 &point, const Int &sample) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point, sample );
	}

	UInt4 ImageLoad (const Image<ImageType2DMSArray, Coherent, UInt4> &image, const Int3 &point, const Int &sample) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point, sample );
	}

	Float4 ImageLoad (const Image<ImageType2DMSArray, Coherent, Float4> &image, const Int3 &point, const Int &sample) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point, sample );
	}

	Int4 ImageLoad (const Image<ImageType2DMSArray, Volatile, Int4> &image, const Int3 &point, const Int &sample) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point, sample );
	}

	UInt4 ImageLoad (const Image<ImageType2DMSArray, Volatile, UInt4> &image, const Int3 &point, const Int &sample) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point, sample );
	}

	Float4 ImageLoad (const Image<ImageType2DMSArray, Volatile, Float4> &image, const Int3 &point, const Int &sample) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point, sample );
	}

	Int4 ImageLoad (const Image<ImageType2DMSArray, Restrict, Int4> &image, const Int3 &point, const Int &sample) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point, sample );
	}

	UInt4 ImageLoad (const Image<ImageType2DMSArray, Restrict, UInt4> &image, const Int3 &point, const Int &sample) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point, sample );
	}

	Float4 ImageLoad (const Image<ImageType2DMSArray, Restrict, Float4> &image, const Int3 &point, const Int &sample) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point, sample );
	}

	Int4 ImageLoad (const Image<ImageTypeCube, ReadOnly, Int4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageTypeCube, ReadOnly, UInt4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageTypeCube, ReadOnly, Float4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageTypeCube, Coherent, Int4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageTypeCube, Coherent, UInt4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageTypeCube, Coherent, Float4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageTypeCube, Volatile, Int4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageTypeCube, Volatile, UInt4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageTypeCube, Volatile, Float4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageTypeCube, Restrict, Int4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageTypeCube, Restrict, UInt4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageTypeCube, Restrict, Float4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageTypeCubeArray, ReadOnly, Int4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageTypeCubeArray, ReadOnly, UInt4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageTypeCubeArray, ReadOnly, Float4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageTypeCubeArray, Coherent, Int4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageTypeCubeArray, Coherent, UInt4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageTypeCubeArray, Coherent, Float4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageTypeCubeArray, Volatile, Int4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageTypeCubeArray, Volatile, UInt4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageTypeCubeArray, Volatile, Float4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageTypeCubeArray, Restrict, Int4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageTypeCubeArray, Restrict, UInt4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageTypeCubeArray, Restrict, Float4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageType3D, ReadOnly, Int4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageType3D, ReadOnly, UInt4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageType3D, ReadOnly, Float4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageType3D, Coherent, Int4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageType3D, Coherent, UInt4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageType3D, Coherent, Float4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageType3D, Volatile, Int4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageType3D, Volatile, UInt4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageType3D, Volatile, Float4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	Int4 ImageLoad (const Image<ImageType3D, Restrict, Int4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "ImageLoad", image, point );
	}

	UInt4 ImageLoad (const Image<ImageType3D, Restrict, UInt4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "ImageLoad", image, point );
	}

	Float4 ImageLoad (const Image<ImageType3D, Restrict, Float4> &image, const Int3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "ImageLoad", image, point );
	}

	void ImageStore (const Image<ImageType1D, WriteOnly, Int4> &image, const Int &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType1D, WriteOnly, UInt4> &image, const Int &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType1D, WriteOnly, Float4> &image, const Int &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType1D, Coherent, Int4> &image, const Int &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType1D, Coherent, UInt4> &image, const Int &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType1D, Coherent, Float4> &image, const Int &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType1D, Volatile, Int4> &image, const Int &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType1D, Volatile, UInt4> &image, const Int &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType1D, Volatile, Float4> &image, const Int &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType1D, Restrict, Int4> &image, const Int &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType1D, Restrict, UInt4> &image, const Int &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType1D, Restrict, Float4> &image, const Int &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType1DArray, WriteOnly, Int4> &image, const Int2 &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType1DArray, WriteOnly, UInt4> &image, const Int2 &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType1DArray, WriteOnly, Float4> &image, const Int2 &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType1DArray, Coherent, Int4> &image, const Int2 &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType1DArray, Coherent, UInt4> &image, const Int2 &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType1DArray, Coherent, Float4> &image, const Int2 &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType1DArray, Volatile, Int4> &image, const Int2 &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType1DArray, Volatile, UInt4> &image, const Int2 &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType1DArray, Volatile, Float4> &image, const Int2 &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType1DArray, Restrict, Int4> &image, const Int2 &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType1DArray, Restrict, UInt4> &image, const Int2 &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType1DArray, Restrict, Float4> &image, const Int2 &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2D, WriteOnly, Int4> &image, const Int2 &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2D, WriteOnly, UInt4> &image, const Int2 &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2D, WriteOnly, Float4> &image, const Int2 &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2D, Coherent, Int4> &image, const Int2 &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2D, Coherent, UInt4> &image, const Int2 &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2D, Coherent, Float4> &image, const Int2 &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2D, Volatile, Int4> &image, const Int2 &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2D, Volatile, UInt4> &image, const Int2 &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2D, Volatile, Float4> &image, const Int2 &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2D, Restrict, Int4> &image, const Int2 &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2D, Restrict, UInt4> &image, const Int2 &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2D, Restrict, Float4> &image, const Int2 &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2DArray, WriteOnly, Int4> &image, const Int3 &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2DArray, WriteOnly, UInt4> &image, const Int3 &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2DArray, WriteOnly, Float4> &image, const Int3 &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2DArray, Coherent, Int4> &image, const Int3 &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2DArray, Coherent, UInt4> &image, const Int3 &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2DArray, Coherent, Float4> &image, const Int3 &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2DArray, Volatile, Int4> &image, const Int3 &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2DArray, Volatile, UInt4> &image, const Int3 &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2DArray, Volatile, Float4> &image, const Int3 &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2DArray, Restrict, Int4> &image, const Int3 &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2DArray, Restrict, UInt4> &image, const Int3 &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2DArray, Restrict, Float4> &image, const Int3 &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType2DMS, WriteOnly, Int4> &image, const Int2 &point, const Int &sample, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, sample, data ) );
	}

	void ImageStore (const Image<ImageType2DMS, WriteOnly, UInt4> &image, const Int2 &point, const Int &sample, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, sample, data ) );
	}

	void ImageStore (const Image<ImageType2DMS, WriteOnly, Float4> &image, const Int2 &point, const Int &sample, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, sample, data ) );
	}

	void ImageStore (const Image<ImageType2DMS, Coherent, Int4> &image, const Int2 &point, const Int &sample, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, sample, data ) );
	}

	void ImageStore (const Image<ImageType2DMS, Coherent, UInt4> &image, const Int2 &point, const Int &sample, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, sample, data ) );
	}

	void ImageStore (const Image<ImageType2DMS, Coherent, Float4> &image, const Int2 &point, const Int &sample, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, sample, data ) );
	}

	void ImageStore (const Image<ImageType2DMS, Volatile, Int4> &image, const Int2 &point, const Int &sample, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, sample, data ) );
	}

	void ImageStore (const Image<ImageType2DMS, Volatile, UInt4> &image, const Int2 &point, const Int &sample, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, sample, data ) );
	}

	void ImageStore (const Image<ImageType2DMS, Volatile, Float4> &image, const Int2 &point, const Int &sample, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, sample, data ) );
	}

	void ImageStore (const Image<ImageType2DMS, Restrict, Int4> &image, const Int2 &point, const Int &sample, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, sample, data ) );
	}

	void ImageStore (const Image<ImageType2DMS, Restrict, UInt4> &image, const Int2 &point, const Int &sample, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, sample, data ) );
	}

	void ImageStore (const Image<ImageType2DMS, Restrict, Float4> &image, const Int2 &point, const Int &sample, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, sample, data ) );
	}

	void ImageStore (const Image<ImageType2DMSArray, WriteOnly, Int4> &image, const Int3 &point, const Int &sample, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, sample, data ) );
	}

	void ImageStore (const Image<ImageType2DMSArray, WriteOnly, UInt4> &image, const Int3 &point, const Int &sample, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, sample, data ) );
	}

	void ImageStore (const Image<ImageType2DMSArray, WriteOnly, Float4> &image, const Int3 &point, const Int &sample, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, sample, data ) );
	}

	void ImageStore (const Image<ImageType2DMSArray, Coherent, Int4> &image, const Int3 &point, const Int &sample, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, sample, data ) );
	}

	void ImageStore (const Image<ImageType2DMSArray, Coherent, UInt4> &image, const Int3 &point, const Int &sample, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, sample, data ) );
	}

	void ImageStore (const Image<ImageType2DMSArray, Coherent, Float4> &image, const Int3 &point, const Int &sample, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, sample, data ) );
	}

	void ImageStore (const Image<ImageType2DMSArray, Volatile, Int4> &image, const Int3 &point, const Int &sample, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, sample, data ) );
	}

	void ImageStore (const Image<ImageType2DMSArray, Volatile, UInt4> &image, const Int3 &point, const Int &sample, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, sample, data ) );
	}

	void ImageStore (const Image<ImageType2DMSArray, Volatile, Float4> &image, const Int3 &point, const Int &sample, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, sample, data ) );
	}

	void ImageStore (const Image<ImageType2DMSArray, Restrict, Int4> &image, const Int3 &point, const Int &sample, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, sample, data ) );
	}

	void ImageStore (const Image<ImageType2DMSArray, Restrict, UInt4> &image, const Int3 &point, const Int &sample, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, sample, data ) );
	}

	void ImageStore (const Image<ImageType2DMSArray, Restrict, Float4> &image, const Int3 &point, const Int &sample, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, sample, data ) );
	}

	void ImageStore (const Image<ImageTypeCube, WriteOnly, Int4> &image, const Int3 &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageTypeCube, WriteOnly, UInt4> &image, const Int3 &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageTypeCube, WriteOnly, Float4> &image, const Int3 &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageTypeCube, Coherent, Int4> &image, const Int3 &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageTypeCube, Coherent, UInt4> &image, const Int3 &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageTypeCube, Coherent, Float4> &image, const Int3 &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageTypeCube, Volatile, Int4> &image, const Int3 &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageTypeCube, Volatile, UInt4> &image, const Int3 &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageTypeCube, Volatile, Float4> &image, const Int3 &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageTypeCube, Restrict, Int4> &image, const Int3 &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageTypeCube, Restrict, UInt4> &image, const Int3 &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageTypeCube, Restrict, Float4> &image, const Int3 &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageTypeCubeArray, WriteOnly, Int4> &image, const Int3 &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageTypeCubeArray, WriteOnly, UInt4> &image, const Int3 &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageTypeCubeArray, WriteOnly, Float4> &image, const Int3 &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageTypeCubeArray, Coherent, Int4> &image, const Int3 &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageTypeCubeArray, Coherent, UInt4> &image, const Int3 &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageTypeCubeArray, Coherent, Float4> &image, const Int3 &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageTypeCubeArray, Volatile, Int4> &image, const Int3 &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageTypeCubeArray, Volatile, UInt4> &image, const Int3 &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageTypeCubeArray, Volatile, Float4> &image, const Int3 &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageTypeCubeArray, Restrict, Int4> &image, const Int3 &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageTypeCubeArray, Restrict, UInt4> &image, const Int3 &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageTypeCubeArray, Restrict, Float4> &image, const Int3 &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType3D, WriteOnly, Int4> &image, const Int3 &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType3D, WriteOnly, UInt4> &image, const Int3 &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType3D, WriteOnly, Float4> &image, const Int3 &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType3D, Coherent, Int4> &image, const Int3 &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType3D, Coherent, UInt4> &image, const Int3 &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType3D, Coherent, Float4> &image, const Int3 &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType3D, Volatile, Int4> &image, const Int3 &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType3D, Volatile, UInt4> &image, const Int3 &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType3D, Volatile, Float4> &image, const Int3 &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType3D, Restrict, Int4> &image, const Int3 &point, const Int4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType3D, Restrict, UInt4> &image, const Int3 &point, const UInt4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	void ImageStore (const Image<ImageType3D, Restrict, Float4> &image, const Int3 &point, const Float4 &data) {
		NodeGetter::Get( image )->Root()->AddField( NodeFunctions::Instance()->Build< ShaderOutputNode >( "ImageStore", image, point, data ) );
	}


	// ImageGetSize, ImageGetSamples
	Int ImageGetSize (const Image<ImageType1D, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSize", image );
	}

	Int ImageGetSize (const Image<ImageType1D, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSize", image );
	}

	Int ImageGetSize (const Image<ImageType1D, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSize", image );
	}

	Int ImageGetSize (const Image<ImageType1D, WriteOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSize", image );
	}

	Int ImageGetSize (const Image<ImageType1D, WriteOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSize", image );
	}

	Int ImageGetSize (const Image<ImageType1D, WriteOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSize", image );
	}

	Int ImageGetSize (const Image<ImageType1D, Coherent, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSize", image );
	}

	Int ImageGetSize (const Image<ImageType1D, Coherent, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSize", image );
	}

	Int ImageGetSize (const Image<ImageType1D, Coherent, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSize", image );
	}

	Int ImageGetSize (const Image<ImageType1D, Volatile, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSize", image );
	}

	Int ImageGetSize (const Image<ImageType1D, Volatile, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSize", image );
	}

	Int ImageGetSize (const Image<ImageType1D, Volatile, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSize", image );
	}

	Int ImageGetSize (const Image<ImageType1D, Restrict, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSize", image );
	}

	Int ImageGetSize (const Image<ImageType1D, Restrict, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSize", image );
	}

	Int ImageGetSize (const Image<ImageType1D, Restrict, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType1DArray, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType1DArray, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType1DArray, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType1DArray, WriteOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType1DArray, WriteOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType1DArray, WriteOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType1DArray, Coherent, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType1DArray, Coherent, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType1DArray, Coherent, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType1DArray, Volatile, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType1DArray, Volatile, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType1DArray, Volatile, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType1DArray, Restrict, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType1DArray, Restrict, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType1DArray, Restrict, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType2D, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType2D, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType2D, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType2D, WriteOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType2D, WriteOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType2D, WriteOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType2D, Coherent, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType2D, Coherent, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType2D, Coherent, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType2D, Volatile, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType2D, Volatile, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType2D, Volatile, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType2D, Restrict, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType2D, Restrict, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType2D, Restrict, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DArray, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DArray, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DArray, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DArray, WriteOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DArray, WriteOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DArray, WriteOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DArray, Coherent, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DArray, Coherent, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DArray, Coherent, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DArray, Volatile, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DArray, Volatile, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DArray, Volatile, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DArray, Restrict, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DArray, Restrict, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DArray, Restrict, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int2 ImageGetSize (const Image<ImageType2DMS, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMS, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int2 ImageGetSize (const Image<ImageType2DMS, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMS, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int2 ImageGetSize (const Image<ImageType2DMS, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMS, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int2 ImageGetSize (const Image<ImageType2DMS, WriteOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMS, WriteOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int2 ImageGetSize (const Image<ImageType2DMS, WriteOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMS, WriteOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int2 ImageGetSize (const Image<ImageType2DMS, WriteOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMS, WriteOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int2 ImageGetSize (const Image<ImageType2DMS, Coherent, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMS, Coherent, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int2 ImageGetSize (const Image<ImageType2DMS, Coherent, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMS, Coherent, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int2 ImageGetSize (const Image<ImageType2DMS, Coherent, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMS, Coherent, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int2 ImageGetSize (const Image<ImageType2DMS, Volatile, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMS, Volatile, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int2 ImageGetSize (const Image<ImageType2DMS, Volatile, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMS, Volatile, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int2 ImageGetSize (const Image<ImageType2DMS, Volatile, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMS, Volatile, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int2 ImageGetSize (const Image<ImageType2DMS, Restrict, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMS, Restrict, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int2 ImageGetSize (const Image<ImageType2DMS, Restrict, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMS, Restrict, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int2 ImageGetSize (const Image<ImageType2DMS, Restrict, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMS, Restrict, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DMSArray, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMSArray, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DMSArray, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMSArray, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DMSArray, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMSArray, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DMSArray, WriteOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMSArray, WriteOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DMSArray, WriteOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMSArray, WriteOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DMSArray, WriteOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMSArray, WriteOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DMSArray, Coherent, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMSArray, Coherent, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DMSArray, Coherent, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMSArray, Coherent, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DMSArray, Coherent, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMSArray, Coherent, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DMSArray, Volatile, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMSArray, Volatile, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DMSArray, Volatile, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMSArray, Volatile, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DMSArray, Volatile, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMSArray, Volatile, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DMSArray, Restrict, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMSArray, Restrict, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DMSArray, Restrict, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMSArray, Restrict, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int3 ImageGetSize (const Image<ImageType2DMSArray, Restrict, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int ImageGetSamples (const Image<ImageType2DMSArray, Restrict, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "ImageGetSamples", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCube, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCube, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCube, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCube, WriteOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCube, WriteOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCube, WriteOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCube, Coherent, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCube, Coherent, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCube, Coherent, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCube, Volatile, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCube, Volatile, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCube, Volatile, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCube, Restrict, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCube, Restrict, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCube, Restrict, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCubeArray, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCubeArray, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCubeArray, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCubeArray, WriteOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCubeArray, WriteOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCubeArray, WriteOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCubeArray, Coherent, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCubeArray, Coherent, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCubeArray, Coherent, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCubeArray, Volatile, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCubeArray, Volatile, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCubeArray, Volatile, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCubeArray, Restrict, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCubeArray, Restrict, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageTypeCubeArray, Restrict, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType3D, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType3D, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType3D, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType3D, WriteOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType3D, WriteOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType3D, WriteOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType3D, Coherent, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType3D, Coherent, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType3D, Coherent, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType3D, Volatile, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType3D, Volatile, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType3D, Volatile, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType3D, Restrict, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType3D, Restrict, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}

	Int3 ImageGetSize (const Image<ImageType3D, Restrict, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "ImageGetSize", image );
	}


	// ImageAtomicAdd, ImageAtomicExchange, ImageAtomicMax, ImageAtomicMin, ImageAtomicAnd, ImageAtomicOr, ImageAtomicXor
	UInt ImageAtomicAdd (const Image<ImageType1D, WriteOnly, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType1D, WriteOnly, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType1D, WriteOnly, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType1D, Coherent, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType1D, Coherent, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType1D, Coherent, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType1D, Volatile, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType1D, Volatile, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType1D, Volatile, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType1D, Restrict, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType1D, Restrict, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType1D, Restrict, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType1DArray, WriteOnly, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType1DArray, WriteOnly, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType1DArray, WriteOnly, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType1DArray, Coherent, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType1DArray, Coherent, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType1DArray, Coherent, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType1DArray, Volatile, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType1DArray, Volatile, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType1DArray, Volatile, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType1DArray, Restrict, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType1DArray, Restrict, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType1DArray, Restrict, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2D, WriteOnly, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2D, WriteOnly, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2D, WriteOnly, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2D, Coherent, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2D, Coherent, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2D, Coherent, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2D, Volatile, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2D, Volatile, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2D, Volatile, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2D, Restrict, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2D, Restrict, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2D, Restrict, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DArray, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DArray, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DArray, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DArray, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DArray, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DArray, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DArray, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DArray, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DArray, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DArray, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DArray, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DArray, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DMS, WriteOnly, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DMS, WriteOnly, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DMS, WriteOnly, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DMS, Coherent, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DMS, Coherent, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DMS, Coherent, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DMS, Volatile, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DMS, Volatile, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DMS, Volatile, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DMS, Restrict, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DMS, Restrict, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DMS, Restrict, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DMSArray, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DMSArray, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DMSArray, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DMSArray, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DMSArray, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DMSArray, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DMSArray, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DMSArray, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DMSArray, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DMSArray, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DMSArray, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType2DMSArray, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageTypeCube, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageTypeCube, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageTypeCube, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageTypeCube, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageTypeCube, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageTypeCube, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageTypeCube, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageTypeCube, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageTypeCube, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageTypeCube, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageTypeCube, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageTypeCube, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageTypeCubeArray, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageTypeCubeArray, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageTypeCubeArray, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageTypeCubeArray, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageTypeCubeArray, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageTypeCubeArray, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageTypeCubeArray, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageTypeCubeArray, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageTypeCubeArray, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageTypeCubeArray, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageTypeCubeArray, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageTypeCubeArray, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType3D, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType3D, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType3D, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType3D, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType3D, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType3D, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType3D, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType3D, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType3D, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType3D, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType3D, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicAdd (const Image<ImageType3D, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAdd", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType1D, WriteOnly, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType1D, WriteOnly, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType1D, WriteOnly, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType1D, Coherent, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType1D, Coherent, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType1D, Coherent, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType1D, Volatile, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType1D, Volatile, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType1D, Volatile, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType1D, Restrict, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType1D, Restrict, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType1D, Restrict, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType1DArray, WriteOnly, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType1DArray, WriteOnly, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType1DArray, WriteOnly, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType1DArray, Coherent, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType1DArray, Coherent, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType1DArray, Coherent, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType1DArray, Volatile, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType1DArray, Volatile, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType1DArray, Volatile, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType1DArray, Restrict, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType1DArray, Restrict, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType1DArray, Restrict, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2D, WriteOnly, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2D, WriteOnly, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2D, WriteOnly, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2D, Coherent, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2D, Coherent, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2D, Coherent, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2D, Volatile, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2D, Volatile, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2D, Volatile, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2D, Restrict, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2D, Restrict, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2D, Restrict, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DArray, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DArray, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DArray, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DArray, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DArray, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DArray, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DArray, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DArray, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DArray, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DArray, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DArray, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DArray, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DMS, WriteOnly, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DMS, WriteOnly, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DMS, WriteOnly, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DMS, Coherent, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DMS, Coherent, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DMS, Coherent, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DMS, Volatile, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DMS, Volatile, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DMS, Volatile, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DMS, Restrict, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DMS, Restrict, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DMS, Restrict, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DMSArray, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DMSArray, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DMSArray, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DMSArray, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DMSArray, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DMSArray, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DMSArray, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DMSArray, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DMSArray, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DMSArray, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DMSArray, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType2DMSArray, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageTypeCube, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageTypeCube, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageTypeCube, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageTypeCube, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageTypeCube, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageTypeCube, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageTypeCube, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageTypeCube, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageTypeCube, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageTypeCube, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageTypeCube, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageTypeCube, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageTypeCubeArray, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageTypeCubeArray, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageTypeCubeArray, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageTypeCubeArray, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageTypeCubeArray, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageTypeCubeArray, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageTypeCubeArray, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageTypeCubeArray, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageTypeCubeArray, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageTypeCubeArray, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageTypeCubeArray, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageTypeCubeArray, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType3D, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType3D, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType3D, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType3D, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType3D, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType3D, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType3D, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType3D, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType3D, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType3D, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType3D, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicExchange (const Image<ImageType3D, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicExchange", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType1D, WriteOnly, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType1D, WriteOnly, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType1D, WriteOnly, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType1D, Coherent, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType1D, Coherent, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType1D, Coherent, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType1D, Volatile, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType1D, Volatile, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType1D, Volatile, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType1D, Restrict, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType1D, Restrict, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType1D, Restrict, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType1DArray, WriteOnly, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType1DArray, WriteOnly, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType1DArray, WriteOnly, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType1DArray, Coherent, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType1DArray, Coherent, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType1DArray, Coherent, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType1DArray, Volatile, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType1DArray, Volatile, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType1DArray, Volatile, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType1DArray, Restrict, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType1DArray, Restrict, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType1DArray, Restrict, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2D, WriteOnly, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2D, WriteOnly, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2D, WriteOnly, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2D, Coherent, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2D, Coherent, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2D, Coherent, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2D, Volatile, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2D, Volatile, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2D, Volatile, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2D, Restrict, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2D, Restrict, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2D, Restrict, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DArray, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DArray, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DArray, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DArray, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DArray, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DArray, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DArray, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DArray, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DArray, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DArray, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DArray, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DArray, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DMS, WriteOnly, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DMS, WriteOnly, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DMS, WriteOnly, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DMS, Coherent, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DMS, Coherent, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DMS, Coherent, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DMS, Volatile, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DMS, Volatile, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DMS, Volatile, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DMS, Restrict, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DMS, Restrict, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DMS, Restrict, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DMSArray, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DMSArray, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DMSArray, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DMSArray, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DMSArray, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DMSArray, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DMSArray, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DMSArray, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DMSArray, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DMSArray, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DMSArray, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType2DMSArray, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageTypeCube, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageTypeCube, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageTypeCube, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageTypeCube, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageTypeCube, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageTypeCube, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageTypeCube, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageTypeCube, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageTypeCube, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageTypeCube, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageTypeCube, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageTypeCube, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageTypeCubeArray, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageTypeCubeArray, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageTypeCubeArray, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageTypeCubeArray, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageTypeCubeArray, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageTypeCubeArray, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageTypeCubeArray, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageTypeCubeArray, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageTypeCubeArray, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageTypeCubeArray, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageTypeCubeArray, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageTypeCubeArray, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType3D, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType3D, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType3D, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType3D, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType3D, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType3D, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType3D, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType3D, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType3D, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType3D, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType3D, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMax (const Image<ImageType3D, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMax", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType1D, WriteOnly, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType1D, WriteOnly, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType1D, WriteOnly, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType1D, Coherent, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType1D, Coherent, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType1D, Coherent, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType1D, Volatile, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType1D, Volatile, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType1D, Volatile, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType1D, Restrict, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType1D, Restrict, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType1D, Restrict, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType1DArray, WriteOnly, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType1DArray, WriteOnly, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType1DArray, WriteOnly, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType1DArray, Coherent, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType1DArray, Coherent, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType1DArray, Coherent, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType1DArray, Volatile, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType1DArray, Volatile, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType1DArray, Volatile, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType1DArray, Restrict, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType1DArray, Restrict, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType1DArray, Restrict, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2D, WriteOnly, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2D, WriteOnly, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2D, WriteOnly, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2D, Coherent, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2D, Coherent, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2D, Coherent, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2D, Volatile, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2D, Volatile, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2D, Volatile, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2D, Restrict, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2D, Restrict, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2D, Restrict, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DArray, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DArray, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DArray, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DArray, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DArray, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DArray, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DArray, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DArray, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DArray, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DArray, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DArray, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DArray, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DMS, WriteOnly, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DMS, WriteOnly, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DMS, WriteOnly, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DMS, Coherent, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DMS, Coherent, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DMS, Coherent, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DMS, Volatile, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DMS, Volatile, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DMS, Volatile, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DMS, Restrict, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DMS, Restrict, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DMS, Restrict, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DMSArray, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DMSArray, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DMSArray, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DMSArray, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DMSArray, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DMSArray, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DMSArray, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DMSArray, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DMSArray, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DMSArray, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DMSArray, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType2DMSArray, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageTypeCube, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageTypeCube, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageTypeCube, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageTypeCube, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageTypeCube, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageTypeCube, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageTypeCube, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageTypeCube, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageTypeCube, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageTypeCube, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageTypeCube, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageTypeCube, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageTypeCubeArray, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageTypeCubeArray, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageTypeCubeArray, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageTypeCubeArray, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageTypeCubeArray, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageTypeCubeArray, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageTypeCubeArray, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageTypeCubeArray, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageTypeCubeArray, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageTypeCubeArray, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageTypeCubeArray, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageTypeCubeArray, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType3D, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType3D, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType3D, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType3D, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType3D, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType3D, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType3D, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType3D, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType3D, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType3D, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType3D, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicMin (const Image<ImageType3D, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicMin", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType1D, WriteOnly, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType1D, WriteOnly, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType1D, WriteOnly, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType1D, Coherent, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType1D, Coherent, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType1D, Coherent, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType1D, Volatile, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType1D, Volatile, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType1D, Volatile, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType1D, Restrict, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType1D, Restrict, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType1D, Restrict, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType1DArray, WriteOnly, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType1DArray, WriteOnly, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType1DArray, WriteOnly, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType1DArray, Coherent, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType1DArray, Coherent, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType1DArray, Coherent, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType1DArray, Volatile, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType1DArray, Volatile, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType1DArray, Volatile, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType1DArray, Restrict, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType1DArray, Restrict, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType1DArray, Restrict, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2D, WriteOnly, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2D, WriteOnly, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2D, WriteOnly, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2D, Coherent, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2D, Coherent, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2D, Coherent, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2D, Volatile, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2D, Volatile, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2D, Volatile, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2D, Restrict, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2D, Restrict, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2D, Restrict, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DArray, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DArray, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DArray, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DArray, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DArray, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DArray, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DArray, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DArray, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DArray, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DArray, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DArray, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DArray, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DMS, WriteOnly, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DMS, WriteOnly, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DMS, WriteOnly, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DMS, Coherent, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DMS, Coherent, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DMS, Coherent, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DMS, Volatile, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DMS, Volatile, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DMS, Volatile, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DMS, Restrict, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DMS, Restrict, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DMS, Restrict, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DMSArray, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DMSArray, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DMSArray, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DMSArray, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DMSArray, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DMSArray, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DMSArray, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DMSArray, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DMSArray, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DMSArray, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DMSArray, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType2DMSArray, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageTypeCube, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageTypeCube, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageTypeCube, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageTypeCube, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageTypeCube, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageTypeCube, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageTypeCube, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageTypeCube, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageTypeCube, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageTypeCube, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageTypeCube, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageTypeCube, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageTypeCubeArray, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageTypeCubeArray, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageTypeCubeArray, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageTypeCubeArray, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageTypeCubeArray, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageTypeCubeArray, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageTypeCubeArray, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageTypeCubeArray, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageTypeCubeArray, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageTypeCubeArray, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageTypeCubeArray, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageTypeCubeArray, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType3D, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType3D, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType3D, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType3D, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType3D, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType3D, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType3D, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType3D, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType3D, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType3D, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType3D, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicAnd (const Image<ImageType3D, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicAnd", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType1D, WriteOnly, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType1D, WriteOnly, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType1D, WriteOnly, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType1D, Coherent, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType1D, Coherent, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType1D, Coherent, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType1D, Volatile, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType1D, Volatile, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType1D, Volatile, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType1D, Restrict, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType1D, Restrict, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType1D, Restrict, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType1DArray, WriteOnly, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType1DArray, WriteOnly, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType1DArray, WriteOnly, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType1DArray, Coherent, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType1DArray, Coherent, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType1DArray, Coherent, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType1DArray, Volatile, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType1DArray, Volatile, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType1DArray, Volatile, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType1DArray, Restrict, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType1DArray, Restrict, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType1DArray, Restrict, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2D, WriteOnly, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2D, WriteOnly, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2D, WriteOnly, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2D, Coherent, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2D, Coherent, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2D, Coherent, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2D, Volatile, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2D, Volatile, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2D, Volatile, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2D, Restrict, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2D, Restrict, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2D, Restrict, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DArray, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DArray, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DArray, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DArray, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DArray, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DArray, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DArray, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DArray, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DArray, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DArray, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DArray, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DArray, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DMS, WriteOnly, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DMS, WriteOnly, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DMS, WriteOnly, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DMS, Coherent, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DMS, Coherent, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DMS, Coherent, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DMS, Volatile, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DMS, Volatile, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DMS, Volatile, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DMS, Restrict, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DMS, Restrict, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DMS, Restrict, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DMSArray, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DMSArray, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DMSArray, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DMSArray, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DMSArray, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DMSArray, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DMSArray, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DMSArray, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DMSArray, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DMSArray, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DMSArray, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType2DMSArray, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageTypeCube, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageTypeCube, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageTypeCube, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageTypeCube, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageTypeCube, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageTypeCube, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageTypeCube, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageTypeCube, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageTypeCube, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageTypeCube, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageTypeCube, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageTypeCube, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageTypeCubeArray, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageTypeCubeArray, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageTypeCubeArray, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageTypeCubeArray, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageTypeCubeArray, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageTypeCubeArray, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageTypeCubeArray, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageTypeCubeArray, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageTypeCubeArray, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageTypeCubeArray, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageTypeCubeArray, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageTypeCubeArray, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType3D, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType3D, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType3D, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType3D, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType3D, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType3D, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType3D, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType3D, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType3D, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType3D, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType3D, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicOr (const Image<ImageType3D, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicOr", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType1D, WriteOnly, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType1D, WriteOnly, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType1D, WriteOnly, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType1D, Coherent, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType1D, Coherent, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType1D, Coherent, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType1D, Volatile, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType1D, Volatile, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType1D, Volatile, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType1D, Restrict, Int4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType1D, Restrict, UInt4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType1D, Restrict, Float4> &image, const Int &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType1DArray, WriteOnly, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType1DArray, WriteOnly, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType1DArray, WriteOnly, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType1DArray, Coherent, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType1DArray, Coherent, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType1DArray, Coherent, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType1DArray, Volatile, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType1DArray, Volatile, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType1DArray, Volatile, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType1DArray, Restrict, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType1DArray, Restrict, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType1DArray, Restrict, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2D, WriteOnly, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2D, WriteOnly, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2D, WriteOnly, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2D, Coherent, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2D, Coherent, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2D, Coherent, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2D, Volatile, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2D, Volatile, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2D, Volatile, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2D, Restrict, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2D, Restrict, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2D, Restrict, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DArray, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DArray, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DArray, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DArray, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DArray, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DArray, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DArray, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DArray, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DArray, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DArray, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DArray, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DArray, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DMS, WriteOnly, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DMS, WriteOnly, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DMS, WriteOnly, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DMS, Coherent, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DMS, Coherent, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DMS, Coherent, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DMS, Volatile, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DMS, Volatile, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DMS, Volatile, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DMS, Restrict, Int4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DMS, Restrict, UInt4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DMS, Restrict, Float4> &image, const Int2 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DMSArray, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DMSArray, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DMSArray, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DMSArray, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DMSArray, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DMSArray, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DMSArray, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DMSArray, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DMSArray, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DMSArray, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DMSArray, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType2DMSArray, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageTypeCube, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageTypeCube, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageTypeCube, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageTypeCube, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageTypeCube, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageTypeCube, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageTypeCube, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageTypeCube, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageTypeCube, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageTypeCube, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageTypeCube, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageTypeCube, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageTypeCubeArray, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageTypeCubeArray, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageTypeCubeArray, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageTypeCubeArray, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageTypeCubeArray, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageTypeCubeArray, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageTypeCubeArray, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageTypeCubeArray, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageTypeCubeArray, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageTypeCubeArray, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageTypeCubeArray, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageTypeCubeArray, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType3D, WriteOnly, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType3D, WriteOnly, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType3D, WriteOnly, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType3D, Coherent, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType3D, Coherent, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType3D, Coherent, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType3D, Volatile, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType3D, Volatile, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType3D, Volatile, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType3D, Restrict, Int4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType3D, Restrict, UInt4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}

	UInt ImageAtomicXor (const Image<ImageType3D, Restrict, Float4> &image, const Int3 &point, const UInt &data) {
		return NodeFunctions::Instance()->Build< UInt >( "ImageAtomicXor", image, point, data );
	}


	// TextureSample, TextureSampleOffset
	Int4 TextureSample (const Image<ImageType1D, ReadOnly, Int4> &image, const Sampler &sampler, const Float &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSample", image, sampler, point );
	}

	Int4 TextureSampleOffset (const Image<ImageType1D, ReadOnly, Int4> &image, const Sampler &sampler, const Float &point, const Int &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	UInt4 TextureSample (const Image<ImageType1D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSample", image, sampler, point );
	}

	UInt4 TextureSampleOffset (const Image<ImageType1D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float &point, const Int &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	Float4 TextureSample (const Image<ImageType1D, ReadOnly, Float4> &image, const Sampler &sampler, const Float &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSample", image, sampler, point );
	}

	Float4 TextureSampleOffset (const Image<ImageType1D, ReadOnly, Float4> &image, const Sampler &sampler, const Float &point, const Int &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	Int4 TextureSample (const Image<ImageType1DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float2 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSample", image, sampler, point );
	}

	Int4 TextureSampleOffset (const Image<ImageType1DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float2 &point, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	UInt4 TextureSample (const Image<ImageType1DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float2 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSample", image, sampler, point );
	}

	UInt4 TextureSampleOffset (const Image<ImageType1DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float2 &point, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	Float4 TextureSample (const Image<ImageType1DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSample", image, sampler, point );
	}

	Float4 TextureSampleOffset (const Image<ImageType1DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &point, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	Int4 TextureSample (const Image<ImageType2D, ReadOnly, Int4> &image, const Sampler &sampler, const Float2 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSample", image, sampler, point );
	}

	Int4 TextureSampleOffset (const Image<ImageType2D, ReadOnly, Int4> &image, const Sampler &sampler, const Float2 &point, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	UInt4 TextureSample (const Image<ImageType2D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float2 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSample", image, sampler, point );
	}

	UInt4 TextureSampleOffset (const Image<ImageType2D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float2 &point, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	Float4 TextureSample (const Image<ImageType2D, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSample", image, sampler, point );
	}

	Float4 TextureSampleOffset (const Image<ImageType2D, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &point, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	Int4 TextureSample (const Image<ImageType2DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSample", image, sampler, point );
	}

	Int4 TextureSampleOffset (const Image<ImageType2DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	UInt4 TextureSample (const Image<ImageType2DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSample", image, sampler, point );
	}

	UInt4 TextureSampleOffset (const Image<ImageType2DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	Float4 TextureSample (const Image<ImageType2DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSample", image, sampler, point );
	}

	Float4 TextureSampleOffset (const Image<ImageType2DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	Int4 TextureSample (const Image<ImageType2DMS, ReadOnly, Int4> &image, const Sampler &sampler, const Float2 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSample", image, sampler, point );
	}

	Int4 TextureSampleOffset (const Image<ImageType2DMS, ReadOnly, Int4> &image, const Sampler &sampler, const Float2 &point, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	UInt4 TextureSample (const Image<ImageType2DMS, ReadOnly, UInt4> &image, const Sampler &sampler, const Float2 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSample", image, sampler, point );
	}

	UInt4 TextureSampleOffset (const Image<ImageType2DMS, ReadOnly, UInt4> &image, const Sampler &sampler, const Float2 &point, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	Float4 TextureSample (const Image<ImageType2DMS, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSample", image, sampler, point );
	}

	Float4 TextureSampleOffset (const Image<ImageType2DMS, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &point, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	Int4 TextureSample (const Image<ImageType2DMSArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSample", image, sampler, point );
	}

	Int4 TextureSampleOffset (const Image<ImageType2DMSArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	UInt4 TextureSample (const Image<ImageType2DMSArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSample", image, sampler, point );
	}

	UInt4 TextureSampleOffset (const Image<ImageType2DMSArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	Float4 TextureSample (const Image<ImageType2DMSArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSample", image, sampler, point );
	}

	Float4 TextureSampleOffset (const Image<ImageType2DMSArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	Int4 TextureSample (const Image<ImageTypeCube, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSample", image, sampler, point );
	}

	Int4 TextureSampleOffset (const Image<ImageTypeCube, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	UInt4 TextureSample (const Image<ImageTypeCube, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSample", image, sampler, point );
	}

	UInt4 TextureSampleOffset (const Image<ImageTypeCube, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	Float4 TextureSample (const Image<ImageTypeCube, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSample", image, sampler, point );
	}

	Float4 TextureSampleOffset (const Image<ImageTypeCube, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	Int4 TextureSample (const Image<ImageTypeCubeArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSample", image, sampler, point );
	}

	Int4 TextureSampleOffset (const Image<ImageTypeCubeArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	UInt4 TextureSample (const Image<ImageTypeCubeArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSample", image, sampler, point );
	}

	UInt4 TextureSampleOffset (const Image<ImageTypeCubeArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	Float4 TextureSample (const Image<ImageTypeCubeArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSample", image, sampler, point );
	}

	Float4 TextureSampleOffset (const Image<ImageTypeCubeArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	Int4 TextureSample (const Image<ImageType3D, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSample", image, sampler, point );
	}

	Int4 TextureSampleOffset (const Image<ImageType3D, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	UInt4 TextureSample (const Image<ImageType3D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSample", image, sampler, point );
	}

	UInt4 TextureSampleOffset (const Image<ImageType3D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleOffset", image, sampler, point, offset );
	}

	Float4 TextureSample (const Image<ImageType3D, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSample", image, sampler, point );
	}

	Float4 TextureSampleOffset (const Image<ImageType3D, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleOffset", image, sampler, point, offset );
	}


	// TextureSampleLod, TextureSampleLodOffset
	Int4 TextureSampleLod (const Image<ImageType1D, ReadOnly, Int4> &image, const Sampler &sampler, const Float &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	Int4 TextureSampleOffsetLod (const Image<ImageType1D, ReadOnly, Int4> &image, const Sampler &sampler, const Float &point, const Float &lod, Int &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	UInt4 TextureSampleLod (const Image<ImageType1D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	UInt4 TextureSampleOffsetLod (const Image<ImageType1D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float &point, const Float &lod, Int &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	Float4 TextureSampleLod (const Image<ImageType1D, ReadOnly, Float4> &image, const Sampler &sampler, const Float &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	Float4 TextureSampleOffsetLod (const Image<ImageType1D, ReadOnly, Float4> &image, const Sampler &sampler, const Float &point, const Float &lod, Int &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	Int4 TextureSampleLod (const Image<ImageType1DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float2 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	Int4 TextureSampleOffsetLod (const Image<ImageType1DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float2 &point, const Float &lod, Int2 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	UInt4 TextureSampleLod (const Image<ImageType1DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float2 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	UInt4 TextureSampleOffsetLod (const Image<ImageType1DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float2 &point, const Float &lod, Int2 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	Float4 TextureSampleLod (const Image<ImageType1DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	Float4 TextureSampleOffsetLod (const Image<ImageType1DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &point, const Float &lod, Int2 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	Int4 TextureSampleLod (const Image<ImageType2D, ReadOnly, Int4> &image, const Sampler &sampler, const Float2 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	Int4 TextureSampleOffsetLod (const Image<ImageType2D, ReadOnly, Int4> &image, const Sampler &sampler, const Float2 &point, const Float &lod, Int2 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	UInt4 TextureSampleLod (const Image<ImageType2D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float2 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	UInt4 TextureSampleOffsetLod (const Image<ImageType2D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float2 &point, const Float &lod, Int2 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	Float4 TextureSampleLod (const Image<ImageType2D, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	Float4 TextureSampleOffsetLod (const Image<ImageType2D, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &point, const Float &lod, Int2 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	Int4 TextureSampleLod (const Image<ImageType2DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	Int4 TextureSampleOffsetLod (const Image<ImageType2DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Float &lod, Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	UInt4 TextureSampleLod (const Image<ImageType2DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	UInt4 TextureSampleOffsetLod (const Image<ImageType2DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Float &lod, Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	Float4 TextureSampleLod (const Image<ImageType2DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	Float4 TextureSampleOffsetLod (const Image<ImageType2DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Float &lod, Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	Int4 TextureSampleLod (const Image<ImageType2DMS, ReadOnly, Int4> &image, const Sampler &sampler, const Float2 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	Int4 TextureSampleOffsetLod (const Image<ImageType2DMS, ReadOnly, Int4> &image, const Sampler &sampler, const Float2 &point, const Float &lod, Int2 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	UInt4 TextureSampleLod (const Image<ImageType2DMS, ReadOnly, UInt4> &image, const Sampler &sampler, const Float2 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	UInt4 TextureSampleOffsetLod (const Image<ImageType2DMS, ReadOnly, UInt4> &image, const Sampler &sampler, const Float2 &point, const Float &lod, Int2 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	Float4 TextureSampleLod (const Image<ImageType2DMS, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	Float4 TextureSampleOffsetLod (const Image<ImageType2DMS, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &point, const Float &lod, Int2 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	Int4 TextureSampleLod (const Image<ImageType2DMSArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	Int4 TextureSampleOffsetLod (const Image<ImageType2DMSArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Float &lod, Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	UInt4 TextureSampleLod (const Image<ImageType2DMSArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	UInt4 TextureSampleOffsetLod (const Image<ImageType2DMSArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Float &lod, Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	Float4 TextureSampleLod (const Image<ImageType2DMSArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	Float4 TextureSampleOffsetLod (const Image<ImageType2DMSArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Float &lod, Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	Int4 TextureSampleLod (const Image<ImageTypeCube, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	Int4 TextureSampleOffsetLod (const Image<ImageTypeCube, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Float &lod, Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	UInt4 TextureSampleLod (const Image<ImageTypeCube, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	UInt4 TextureSampleOffsetLod (const Image<ImageTypeCube, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Float &lod, Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	Float4 TextureSampleLod (const Image<ImageTypeCube, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	Float4 TextureSampleOffsetLod (const Image<ImageTypeCube, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Float &lod, Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	Int4 TextureSampleLod (const Image<ImageTypeCubeArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	Int4 TextureSampleOffsetLod (const Image<ImageTypeCubeArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Float &lod, Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	UInt4 TextureSampleLod (const Image<ImageTypeCubeArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	UInt4 TextureSampleOffsetLod (const Image<ImageTypeCubeArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Float &lod, Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	Float4 TextureSampleLod (const Image<ImageTypeCubeArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	Float4 TextureSampleOffsetLod (const Image<ImageTypeCubeArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Float &lod, Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	Int4 TextureSampleLod (const Image<ImageType3D, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	Int4 TextureSampleOffsetLod (const Image<ImageType3D, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Float &lod, Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	UInt4 TextureSampleLod (const Image<ImageType3D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	UInt4 TextureSampleOffsetLod (const Image<ImageType3D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Float &lod, Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}

	Float4 TextureSampleLod (const Image<ImageType3D, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleLod", image, sampler, point, lod );
	}

	Float4 TextureSampleOffsetLod (const Image<ImageType3D, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Float &lod, Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleOffsetLod", image, sampler, point, lod, offset );
	}


	// TextureSampleGrad, TextureSampleGradOffset
	Int4 TextureSampleGrad (const Image<ImageType1D, ReadOnly, Int4> &image, const Sampler &sampler, const Float &point, const Float &dPdx, const Float &dPdy) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	Int4 TextureSampleGradOffset (const Image<ImageType1D, ReadOnly, Int4> &image, const Sampler &sampler, const Float &point, const Float &dPdx, const Float &dPdy, const Int &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	UInt4 TextureSampleGrad (const Image<ImageType1D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float &point, const Float &dPdx, const Float &dPdy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	UInt4 TextureSampleGradOffset (const Image<ImageType1D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float &point, const Float &dPdx, const Float &dPdy, const Int &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Float4 TextureSampleGrad (const Image<ImageType1D, ReadOnly, Float4> &image, const Sampler &sampler, const Float &point, const Float &dPdx, const Float &dPdy) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	Float4 TextureSampleGradOffset (const Image<ImageType1D, ReadOnly, Float4> &image, const Sampler &sampler, const Float &point, const Float &dPdx, const Float &dPdy, const Int &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Int4 TextureSampleGrad (const Image<ImageType1DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float2 &point, const Float &dPdx, const Float &dPdy) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	Int4 TextureSampleGradOffset (const Image<ImageType1DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float2 &point, const Float &dPdx, const Float &dPdy, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	UInt4 TextureSampleGrad (const Image<ImageType1DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float2 &point, const Float &dPdx, const Float &dPdy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	UInt4 TextureSampleGradOffset (const Image<ImageType1DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float2 &point, const Float &dPdx, const Float &dPdy, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Float4 TextureSampleGrad (const Image<ImageType1DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &point, const Float &dPdx, const Float &dPdy) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	Float4 TextureSampleGradOffset (const Image<ImageType1DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &point, const Float &dPdx, const Float &dPdy, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Int4 TextureSampleGrad (const Image<ImageType2D, ReadOnly, Int4> &image, const Sampler &sampler, const Float2 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	Int4 TextureSampleGradOffset (const Image<ImageType2D, ReadOnly, Int4> &image, const Sampler &sampler, const Float2 &point, const Float2 &dPdx, const Float2 &dPdy, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	UInt4 TextureSampleGrad (const Image<ImageType2D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float2 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	UInt4 TextureSampleGradOffset (const Image<ImageType2D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float2 &point, const Float2 &dPdx, const Float2 &dPdy, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Float4 TextureSampleGrad (const Image<ImageType2D, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	Float4 TextureSampleGradOffset (const Image<ImageType2D, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &point, const Float2 &dPdx, const Float2 &dPdy, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Int4 TextureSampleGrad (const Image<ImageType2DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	Int4 TextureSampleGradOffset (const Image<ImageType2DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Float2 &dPdx, const Float2 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	UInt4 TextureSampleGrad (const Image<ImageType2DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	UInt4 TextureSampleGradOffset (const Image<ImageType2DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Float2 &dPdx, const Float2 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Float4 TextureSampleGrad (const Image<ImageType2DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	Float4 TextureSampleGradOffset (const Image<ImageType2DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Float2 &dPdx, const Float2 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Int4 TextureSampleGrad (const Image<ImageType2DMS, ReadOnly, Int4> &image, const Sampler &sampler, const Float2 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	Int4 TextureSampleGradOffset (const Image<ImageType2DMS, ReadOnly, Int4> &image, const Sampler &sampler, const Float2 &point, const Float2 &dPdx, const Float2 &dPdy, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	UInt4 TextureSampleGrad (const Image<ImageType2DMS, ReadOnly, UInt4> &image, const Sampler &sampler, const Float2 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	UInt4 TextureSampleGradOffset (const Image<ImageType2DMS, ReadOnly, UInt4> &image, const Sampler &sampler, const Float2 &point, const Float2 &dPdx, const Float2 &dPdy, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Float4 TextureSampleGrad (const Image<ImageType2DMS, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	Float4 TextureSampleGradOffset (const Image<ImageType2DMS, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &point, const Float2 &dPdx, const Float2 &dPdy, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Int4 TextureSampleGrad (const Image<ImageType2DMSArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	Int4 TextureSampleGradOffset (const Image<ImageType2DMSArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Float2 &dPdx, const Float2 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	UInt4 TextureSampleGrad (const Image<ImageType2DMSArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	UInt4 TextureSampleGradOffset (const Image<ImageType2DMSArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Float2 &dPdx, const Float2 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Float4 TextureSampleGrad (const Image<ImageType2DMSArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	Float4 TextureSampleGradOffset (const Image<ImageType2DMSArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Float2 &dPdx, const Float2 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Int4 TextureSampleGrad (const Image<ImageTypeCube, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	Int4 TextureSampleGradOffset (const Image<ImageTypeCube, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Float2 &dPdx, const Float2 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	UInt4 TextureSampleGrad (const Image<ImageTypeCube, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	UInt4 TextureSampleGradOffset (const Image<ImageTypeCube, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Float2 &dPdx, const Float2 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Float4 TextureSampleGrad (const Image<ImageTypeCube, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	Float4 TextureSampleGradOffset (const Image<ImageTypeCube, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Float2 &dPdx, const Float2 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Int4 TextureSampleGrad (const Image<ImageTypeCubeArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	Int4 TextureSampleGradOffset (const Image<ImageTypeCubeArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Float2 &dPdx, const Float2 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	UInt4 TextureSampleGrad (const Image<ImageTypeCubeArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	UInt4 TextureSampleGradOffset (const Image<ImageTypeCubeArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Float2 &dPdx, const Float2 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Float4 TextureSampleGrad (const Image<ImageTypeCubeArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	Float4 TextureSampleGradOffset (const Image<ImageTypeCubeArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Float2 &dPdx, const Float2 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Int4 TextureSampleGrad (const Image<ImageType3D, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Float3 &dPdx, const Float3 &dPdy) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	Int4 TextureSampleGradOffset (const Image<ImageType3D, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Float3 &dPdx, const Float3 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	UInt4 TextureSampleGrad (const Image<ImageType3D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Float3 &dPdx, const Float3 &dPdy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	UInt4 TextureSampleGradOffset (const Image<ImageType3D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Float3 &dPdx, const Float3 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Float4 TextureSampleGrad (const Image<ImageType3D, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Float3 &dPdx, const Float3 &dPdy) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	Float4 TextureSampleGradOffset (const Image<ImageType3D, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Float3 &dPdx, const Float3 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}


	// TextureSampleGrather, TextureSampleGatherOffset
	Int4 TextureSampleGather (const Image<ImageType1D, ReadOnly, Int4> &image, const Sampler &sampler, const Float &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGather", image, sampler, point );
	}

	Int4 TextureSampleGatherOffset (const Image<ImageType1D, ReadOnly, Int4> &image, const Sampler &sampler, const Float &point, const Int &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGatherOffset", image, sampler, point, offset );
	}

	UInt4 TextureSampleGather (const Image<ImageType1D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGather", image, sampler, point );
	}

	UInt4 TextureSampleGatherOffset (const Image<ImageType1D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float &point, const Int &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGatherOffset", image, sampler, point, offset );
	}

	Float4 TextureSampleGather (const Image<ImageType1D, ReadOnly, Float4> &image, const Sampler &sampler, const Float &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGather", image, sampler, point );
	}

	Float4 TextureSampleGatherOffset (const Image<ImageType1D, ReadOnly, Float4> &image, const Sampler &sampler, const Float &point, const Int &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGatherOffset", image, sampler, point, offset );
	}

	Int4 TextureSampleGather (const Image<ImageType1DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float2 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGather", image, sampler, point );
	}

	Int4 TextureSampleGatherOffset (const Image<ImageType1DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float2 &point, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGatherOffset", image, sampler, point, offset );
	}

	UInt4 TextureSampleGather (const Image<ImageType1DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float2 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGather", image, sampler, point );
	}

	UInt4 TextureSampleGatherOffset (const Image<ImageType1DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float2 &point, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGatherOffset", image, sampler, point, offset );
	}

	Float4 TextureSampleGather (const Image<ImageType1DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGather", image, sampler, point );
	}

	Float4 TextureSampleGatherOffset (const Image<ImageType1DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &point, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGatherOffset", image, sampler, point, offset );
	}

	Int4 TextureSampleGather (const Image<ImageType2D, ReadOnly, Int4> &image, const Sampler &sampler, const Float2 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGather", image, sampler, point );
	}

	Int4 TextureSampleGatherOffset (const Image<ImageType2D, ReadOnly, Int4> &image, const Sampler &sampler, const Float2 &point, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGatherOffset", image, sampler, point, offset );
	}

	UInt4 TextureSampleGather (const Image<ImageType2D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float2 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGather", image, sampler, point );
	}

	UInt4 TextureSampleGatherOffset (const Image<ImageType2D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float2 &point, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGatherOffset", image, sampler, point, offset );
	}

	Float4 TextureSampleGather (const Image<ImageType2D, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGather", image, sampler, point );
	}

	Float4 TextureSampleGatherOffset (const Image<ImageType2D, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &point, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGatherOffset", image, sampler, point, offset );
	}

	Int4 TextureSampleGather (const Image<ImageType2DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGather", image, sampler, point );
	}

	Int4 TextureSampleGatherOffset (const Image<ImageType2DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGatherOffset", image, sampler, point, offset );
	}

	UInt4 TextureSampleGather (const Image<ImageType2DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGather", image, sampler, point );
	}

	UInt4 TextureSampleGatherOffset (const Image<ImageType2DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGatherOffset", image, sampler, point, offset );
	}

	Float4 TextureSampleGather (const Image<ImageType2DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGather", image, sampler, point );
	}

	Float4 TextureSampleGatherOffset (const Image<ImageType2DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGatherOffset", image, sampler, point, offset );
	}

	Int4 TextureSampleGather (const Image<ImageTypeCube, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGather", image, sampler, point );
	}

	Int4 TextureSampleGatherOffset (const Image<ImageTypeCube, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGatherOffset", image, sampler, point, offset );
	}

	UInt4 TextureSampleGather (const Image<ImageTypeCube, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGather", image, sampler, point );
	}

	UInt4 TextureSampleGatherOffset (const Image<ImageTypeCube, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGatherOffset", image, sampler, point, offset );
	}

	Float4 TextureSampleGather (const Image<ImageTypeCube, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGather", image, sampler, point );
	}

	Float4 TextureSampleGatherOffset (const Image<ImageTypeCube, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGatherOffset", image, sampler, point, offset );
	}

	Int4 TextureSampleGather (const Image<ImageTypeCubeArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGather", image, sampler, point );
	}

	Int4 TextureSampleGatherOffset (const Image<ImageTypeCubeArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGatherOffset", image, sampler, point, offset );
	}

	UInt4 TextureSampleGather (const Image<ImageTypeCubeArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGather", image, sampler, point );
	}

	UInt4 TextureSampleGatherOffset (const Image<ImageTypeCubeArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGatherOffset", image, sampler, point, offset );
	}

	Float4 TextureSampleGather (const Image<ImageTypeCubeArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGather", image, sampler, point );
	}

	Float4 TextureSampleGatherOffset (const Image<ImageTypeCubeArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGatherOffset", image, sampler, point, offset );
	}

	Int4 TextureSampleGather (const Image<ImageType3D, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGather", image, sampler, point );
	}

	Int4 TextureSampleGatherOffset (const Image<ImageType3D, ReadOnly, Int4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleGatherOffset", image, sampler, point, offset );
	}

	UInt4 TextureSampleGather (const Image<ImageType3D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGather", image, sampler, point );
	}

	UInt4 TextureSampleGatherOffset (const Image<ImageType3D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleGatherOffset", image, sampler, point, offset );
	}

	Float4 TextureSampleGather (const Image<ImageType3D, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGather", image, sampler, point );
	}

	Float4 TextureSampleGatherOffset (const Image<ImageType3D, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleGatherOffset", image, sampler, point, offset );
	}


	// TextureSampleProj, TextureSampleProjOffset
	Int4 TextureSampleProj (const Image<ImageType1D, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProj", image, sampler, point );
	}

	Int4 TextureSampleProjOffset (const Image<ImageType1D, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Int &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	UInt4 TextureSampleProj (const Image<ImageType1D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProj", image, sampler, point );
	}

	UInt4 TextureSampleProjOffset (const Image<ImageType1D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Int &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	Float4 TextureSampleProj (const Image<ImageType1D, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProj", image, sampler, point );
	}

	Float4 TextureSampleProjOffset (const Image<ImageType1D, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Int &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	Int4 TextureSampleProj (const Image<ImageType1DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProj", image, sampler, point );
	}

	Int4 TextureSampleProjOffset (const Image<ImageType1DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	UInt4 TextureSampleProj (const Image<ImageType1DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProj", image, sampler, point );
	}

	UInt4 TextureSampleProjOffset (const Image<ImageType1DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	Float4 TextureSampleProj (const Image<ImageType1DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProj", image, sampler, point );
	}

	Float4 TextureSampleProjOffset (const Image<ImageType1DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	Int4 TextureSampleProj (const Image<ImageType2D, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProj", image, sampler, point );
	}

	Int4 TextureSampleProjOffset (const Image<ImageType2D, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	UInt4 TextureSampleProj (const Image<ImageType2D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProj", image, sampler, point );
	}

	UInt4 TextureSampleProjOffset (const Image<ImageType2D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	Float4 TextureSampleProj (const Image<ImageType2D, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProj", image, sampler, point );
	}

	Float4 TextureSampleProjOffset (const Image<ImageType2D, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	Int4 TextureSampleProj (const Image<ImageType2DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProj", image, sampler, point );
	}

	Int4 TextureSampleProjOffset (const Image<ImageType2DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	UInt4 TextureSampleProj (const Image<ImageType2DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProj", image, sampler, point );
	}

	UInt4 TextureSampleProjOffset (const Image<ImageType2DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	Float4 TextureSampleProj (const Image<ImageType2DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProj", image, sampler, point );
	}

	Float4 TextureSampleProjOffset (const Image<ImageType2DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	Int4 TextureSampleProj (const Image<ImageType2DMS, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProj", image, sampler, point );
	}

	Int4 TextureSampleProjOffset (const Image<ImageType2DMS, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	UInt4 TextureSampleProj (const Image<ImageType2DMS, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProj", image, sampler, point );
	}

	UInt4 TextureSampleProjOffset (const Image<ImageType2DMS, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	Float4 TextureSampleProj (const Image<ImageType2DMS, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProj", image, sampler, point );
	}

	Float4 TextureSampleProjOffset (const Image<ImageType2DMS, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	Int4 TextureSampleProj (const Image<ImageType2DMSArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProj", image, sampler, point );
	}

	Int4 TextureSampleProjOffset (const Image<ImageType2DMSArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	UInt4 TextureSampleProj (const Image<ImageType2DMSArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProj", image, sampler, point );
	}

	UInt4 TextureSampleProjOffset (const Image<ImageType2DMSArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	Float4 TextureSampleProj (const Image<ImageType2DMSArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProj", image, sampler, point );
	}

	Float4 TextureSampleProjOffset (const Image<ImageType2DMSArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	Int4 TextureSampleProj (const Image<ImageTypeCube, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProj", image, sampler, point );
	}

	Int4 TextureSampleProjOffset (const Image<ImageTypeCube, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	UInt4 TextureSampleProj (const Image<ImageTypeCube, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProj", image, sampler, point );
	}

	UInt4 TextureSampleProjOffset (const Image<ImageTypeCube, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	Float4 TextureSampleProj (const Image<ImageTypeCube, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProj", image, sampler, point );
	}

	Float4 TextureSampleProjOffset (const Image<ImageTypeCube, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	Int4 TextureSampleProj (const Image<ImageTypeCubeArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProj", image, sampler, point );
	}

	Int4 TextureSampleProjOffset (const Image<ImageTypeCubeArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	UInt4 TextureSampleProj (const Image<ImageTypeCubeArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProj", image, sampler, point );
	}

	UInt4 TextureSampleProjOffset (const Image<ImageTypeCubeArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	Float4 TextureSampleProj (const Image<ImageTypeCubeArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProj", image, sampler, point );
	}

	Float4 TextureSampleProjOffset (const Image<ImageTypeCubeArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	Int4 TextureSampleProj (const Image<ImageType3D, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProj", image, sampler, point );
	}

	Int4 TextureSampleProjOffset (const Image<ImageType3D, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	UInt4 TextureSampleProj (const Image<ImageType3D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProj", image, sampler, point );
	}

	UInt4 TextureSampleProjOffset (const Image<ImageType3D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}

	Float4 TextureSampleProj (const Image<ImageType3D, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProj", image, sampler, point );
	}

	Float4 TextureSampleProjOffset (const Image<ImageType3D, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjOffset", image, sampler, point, offset );
	}


	// TextureSampleProjLod, TextureSampleProjLodOffset
	Int4 TextureSampleProjLod (const Image<ImageType1D, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	Int4 TextureSampleProjLodOffset (const Image<ImageType1D, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	UInt4 TextureSampleProjLod (const Image<ImageType1D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	UInt4 TextureSampleProjLodOffset (const Image<ImageType1D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	Float4 TextureSampleProjLod (const Image<ImageType1D, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	Float4 TextureSampleProjLodOffset (const Image<ImageType1D, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	Int4 TextureSampleProjLod (const Image<ImageType1DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	Int4 TextureSampleProjLodOffset (const Image<ImageType1DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	UInt4 TextureSampleProjLod (const Image<ImageType1DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	UInt4 TextureSampleProjLodOffset (const Image<ImageType1DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	Float4 TextureSampleProjLod (const Image<ImageType1DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	Float4 TextureSampleProjLodOffset (const Image<ImageType1DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	Int4 TextureSampleProjLod (const Image<ImageType2D, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	Int4 TextureSampleProjLodOffset (const Image<ImageType2D, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	UInt4 TextureSampleProjLod (const Image<ImageType2D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	UInt4 TextureSampleProjLodOffset (const Image<ImageType2D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	Float4 TextureSampleProjLod (const Image<ImageType2D, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	Float4 TextureSampleProjLodOffset (const Image<ImageType2D, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	Int4 TextureSampleProjLod (const Image<ImageType2DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	Int4 TextureSampleProjLodOffset (const Image<ImageType2DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	UInt4 TextureSampleProjLod (const Image<ImageType2DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	UInt4 TextureSampleProjLodOffset (const Image<ImageType2DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	Float4 TextureSampleProjLod (const Image<ImageType2DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	Float4 TextureSampleProjLodOffset (const Image<ImageType2DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	Int4 TextureSampleProjLod (const Image<ImageType2DMS, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	Int4 TextureSampleProjLodOffset (const Image<ImageType2DMS, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	UInt4 TextureSampleProjLod (const Image<ImageType2DMS, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	UInt4 TextureSampleProjLodOffset (const Image<ImageType2DMS, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	Float4 TextureSampleProjLod (const Image<ImageType2DMS, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	Float4 TextureSampleProjLodOffset (const Image<ImageType2DMS, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	Int4 TextureSampleProjLod (const Image<ImageType2DMSArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	Int4 TextureSampleProjLodOffset (const Image<ImageType2DMSArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	UInt4 TextureSampleProjLod (const Image<ImageType2DMSArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	UInt4 TextureSampleProjLodOffset (const Image<ImageType2DMSArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	Float4 TextureSampleProjLod (const Image<ImageType2DMSArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	Float4 TextureSampleProjLodOffset (const Image<ImageType2DMSArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	Int4 TextureSampleProjLod (const Image<ImageTypeCube, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	Int4 TextureSampleProjLodOffset (const Image<ImageTypeCube, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	UInt4 TextureSampleProjLod (const Image<ImageTypeCube, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	UInt4 TextureSampleProjLodOffset (const Image<ImageTypeCube, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	Float4 TextureSampleProjLod (const Image<ImageTypeCube, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	Float4 TextureSampleProjLodOffset (const Image<ImageTypeCube, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	Int4 TextureSampleProjLod (const Image<ImageTypeCubeArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	Int4 TextureSampleProjLodOffset (const Image<ImageTypeCubeArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	UInt4 TextureSampleProjLod (const Image<ImageTypeCubeArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	UInt4 TextureSampleProjLodOffset (const Image<ImageTypeCubeArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	Float4 TextureSampleProjLod (const Image<ImageTypeCubeArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	Float4 TextureSampleProjLodOffset (const Image<ImageTypeCubeArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	Int4 TextureSampleProjLod (const Image<ImageType3D, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	Int4 TextureSampleProjLodOffset (const Image<ImageType3D, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	UInt4 TextureSampleProjLod (const Image<ImageType3D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	UInt4 TextureSampleProjLodOffset (const Image<ImageType3D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}

	Float4 TextureSampleProjLod (const Image<ImageType3D, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	Float4 TextureSampleProjLodOffset (const Image<ImageType3D, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}


	// TextureSampleProjGrad, TextureSampleProjGradOffset
	Int4 TextureSampleProjGrad (const Image<ImageType1D, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float &dPdx, const Float &dPdy) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	Int4 TextureSampleProjGradOffset (const Image<ImageType1D, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float &dPdx, const Float &dPdy, const Int &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	UInt4 TextureSampleProjGrad (const Image<ImageType1D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float &dPdx, const Float &dPdy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	UInt4 TextureSampleProjGradOffset (const Image<ImageType1D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float &dPdx, const Float &dPdy, const Int &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Float4 TextureSampleProjGrad (const Image<ImageType1D, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float &dPdx, const Float &dPdy) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	Float4 TextureSampleProjGradOffset (const Image<ImageType1D, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float &dPdx, const Float &dPdy, const Int &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Int4 TextureSampleProjGrad (const Image<ImageType1DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float &dPdx, const Float &dPdy) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	Int4 TextureSampleProjGradOffset (const Image<ImageType1DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float &dPdx, const Float &dPdy, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	UInt4 TextureSampleProjGrad (const Image<ImageType1DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float &dPdx, const Float &dPdy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	UInt4 TextureSampleProjGradOffset (const Image<ImageType1DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float &dPdx, const Float &dPdy, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Float4 TextureSampleProjGrad (const Image<ImageType1DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float &dPdx, const Float &dPdy) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	Float4 TextureSampleProjGradOffset (const Image<ImageType1DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float &dPdx, const Float &dPdy, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Int4 TextureSampleProjGrad (const Image<ImageType2D, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	Int4 TextureSampleProjGradOffset (const Image<ImageType2D, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	UInt4 TextureSampleProjGrad (const Image<ImageType2D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	UInt4 TextureSampleProjGradOffset (const Image<ImageType2D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Float4 TextureSampleProjGrad (const Image<ImageType2D, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	Float4 TextureSampleProjGradOffset (const Image<ImageType2D, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Int4 TextureSampleProjGrad (const Image<ImageType2DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	Int4 TextureSampleProjGradOffset (const Image<ImageType2DArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	UInt4 TextureSampleProjGrad (const Image<ImageType2DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	UInt4 TextureSampleProjGradOffset (const Image<ImageType2DArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Float4 TextureSampleProjGrad (const Image<ImageType2DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	Float4 TextureSampleProjGradOffset (const Image<ImageType2DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Int4 TextureSampleProjGrad (const Image<ImageType2DMS, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	Int4 TextureSampleProjGradOffset (const Image<ImageType2DMS, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	UInt4 TextureSampleProjGrad (const Image<ImageType2DMS, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	UInt4 TextureSampleProjGradOffset (const Image<ImageType2DMS, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Float4 TextureSampleProjGrad (const Image<ImageType2DMS, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	Float4 TextureSampleProjGradOffset (const Image<ImageType2DMS, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Int4 TextureSampleProjGrad (const Image<ImageType2DMSArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	Int4 TextureSampleProjGradOffset (const Image<ImageType2DMSArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	UInt4 TextureSampleProjGrad (const Image<ImageType2DMSArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	UInt4 TextureSampleProjGradOffset (const Image<ImageType2DMSArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Float4 TextureSampleProjGrad (const Image<ImageType2DMSArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	Float4 TextureSampleProjGradOffset (const Image<ImageType2DMSArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Int4 TextureSampleProjGrad (const Image<ImageTypeCube, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	Int4 TextureSampleProjGradOffset (const Image<ImageTypeCube, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	UInt4 TextureSampleProjGrad (const Image<ImageTypeCube, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	UInt4 TextureSampleProjGradOffset (const Image<ImageTypeCube, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Float4 TextureSampleProjGrad (const Image<ImageTypeCube, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	Float4 TextureSampleProjGradOffset (const Image<ImageTypeCube, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Int4 TextureSampleProjGrad (const Image<ImageTypeCubeArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	Int4 TextureSampleProjGradOffset (const Image<ImageTypeCubeArray, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	UInt4 TextureSampleProjGrad (const Image<ImageTypeCubeArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	UInt4 TextureSampleProjGradOffset (const Image<ImageTypeCubeArray, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Float4 TextureSampleProjGrad (const Image<ImageTypeCubeArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	Float4 TextureSampleProjGradOffset (const Image<ImageTypeCubeArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float2 &dPdx, const Float2 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Int4 TextureSampleProjGrad (const Image<ImageType3D, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float3 &dPdx, const Float3 &dPdy) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	Int4 TextureSampleProjGradOffset (const Image<ImageType3D, ReadOnly, Int4> &image, const Sampler &sampler, const Float4 &point, const Float3 &dPdx, const Float3 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	UInt4 TextureSampleProjGrad (const Image<ImageType3D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float3 &dPdx, const Float3 &dPdy) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	UInt4 TextureSampleProjGradOffset (const Image<ImageType3D, ReadOnly, UInt4> &image, const Sampler &sampler, const Float4 &point, const Float3 &dPdx, const Float3 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}

	Float4 TextureSampleProjGrad (const Image<ImageType3D, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float3 &dPdx, const Float3 &dPdy) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	Float4 TextureSampleProjGradOffset (const Image<ImageType3D, ReadOnly, Float4> &image, const Sampler &sampler, const Float4 &point, const Float3 &dPdx, const Float3 &dPdy, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}


	// TextureFetch, TextureFetchOffset
	Int4 TextureFetch (const Image<ImageType1D, ReadOnly, Int4> &image, const Int &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureFetch", image, point, lod );
	}

	Int4 TextureFetch (const Image<ImageType1D, ReadOnly, Int4> &image, const Int &point, const Int &lod, const Int &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureFetch", image, point, lod, offset );
	}

	UInt4 TextureFetch (const Image<ImageType1D, ReadOnly, UInt4> &image, const Int &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureFetch", image, point, lod );
	}

	UInt4 TextureFetch (const Image<ImageType1D, ReadOnly, UInt4> &image, const Int &point, const Int &lod, const Int &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureFetch", image, point, lod, offset );
	}

	Float4 TextureFetch (const Image<ImageType1D, ReadOnly, Float4> &image, const Int &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureFetch", image, point, lod );
	}

	Float4 TextureFetch (const Image<ImageType1D, ReadOnly, Float4> &image, const Int &point, const Int &lod, const Int &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureFetch", image, point, lod, offset );
	}

	Int4 TextureFetch (const Image<ImageType1DArray, ReadOnly, Int4> &image, const Int2 &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureFetch", image, point, lod );
	}

	Int4 TextureFetch (const Image<ImageType1DArray, ReadOnly, Int4> &image, const Int2 &point, const Int &lod, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureFetch", image, point, lod, offset );
	}

	UInt4 TextureFetch (const Image<ImageType1DArray, ReadOnly, UInt4> &image, const Int2 &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureFetch", image, point, lod );
	}

	UInt4 TextureFetch (const Image<ImageType1DArray, ReadOnly, UInt4> &image, const Int2 &point, const Int &lod, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureFetch", image, point, lod, offset );
	}

	Float4 TextureFetch (const Image<ImageType1DArray, ReadOnly, Float4> &image, const Int2 &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureFetch", image, point, lod );
	}

	Float4 TextureFetch (const Image<ImageType1DArray, ReadOnly, Float4> &image, const Int2 &point, const Int &lod, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureFetch", image, point, lod, offset );
	}

	Int4 TextureFetch (const Image<ImageType2D, ReadOnly, Int4> &image, const Int2 &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureFetch", image, point, lod );
	}

	Int4 TextureFetch (const Image<ImageType2D, ReadOnly, Int4> &image, const Int2 &point, const Int &lod, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureFetch", image, point, lod, offset );
	}

	UInt4 TextureFetch (const Image<ImageType2D, ReadOnly, UInt4> &image, const Int2 &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureFetch", image, point, lod );
	}

	UInt4 TextureFetch (const Image<ImageType2D, ReadOnly, UInt4> &image, const Int2 &point, const Int &lod, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureFetch", image, point, lod, offset );
	}

	Float4 TextureFetch (const Image<ImageType2D, ReadOnly, Float4> &image, const Int2 &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureFetch", image, point, lod );
	}

	Float4 TextureFetch (const Image<ImageType2D, ReadOnly, Float4> &image, const Int2 &point, const Int &lod, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureFetch", image, point, lod, offset );
	}

	Int4 TextureFetch (const Image<ImageType2DArray, ReadOnly, Int4> &image, const Int3 &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureFetch", image, point, lod );
	}

	Int4 TextureFetch (const Image<ImageType2DArray, ReadOnly, Int4> &image, const Int3 &point, const Int &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureFetch", image, point, lod, offset );
	}

	UInt4 TextureFetch (const Image<ImageType2DArray, ReadOnly, UInt4> &image, const Int3 &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureFetch", image, point, lod );
	}

	UInt4 TextureFetch (const Image<ImageType2DArray, ReadOnly, UInt4> &image, const Int3 &point, const Int &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureFetch", image, point, lod, offset );
	}

	Float4 TextureFetch (const Image<ImageType2DArray, ReadOnly, Float4> &image, const Int3 &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureFetch", image, point, lod );
	}

	Float4 TextureFetch (const Image<ImageType2DArray, ReadOnly, Float4> &image, const Int3 &point, const Int &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureFetch", image, point, lod, offset );
	}

	Int4 TextureFetch (const Image<ImageType2DMS, ReadOnly, Int4> &image, const Int2 &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureFetch", image, point, lod );
	}

	Int4 TextureFetch (const Image<ImageType2DMS, ReadOnly, Int4> &image, const Int2 &point, const Int &lod, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureFetch", image, point, lod, offset );
	}

	UInt4 TextureFetch (const Image<ImageType2DMS, ReadOnly, UInt4> &image, const Int2 &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureFetch", image, point, lod );
	}

	UInt4 TextureFetch (const Image<ImageType2DMS, ReadOnly, UInt4> &image, const Int2 &point, const Int &lod, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureFetch", image, point, lod, offset );
	}

	Float4 TextureFetch (const Image<ImageType2DMS, ReadOnly, Float4> &image, const Int2 &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureFetch", image, point, lod );
	}

	Float4 TextureFetch (const Image<ImageType2DMS, ReadOnly, Float4> &image, const Int2 &point, const Int &lod, const Int2 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureFetch", image, point, lod, offset );
	}

	Int4 TextureFetch (const Image<ImageType2DMSArray, ReadOnly, Int4> &image, const Int3 &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureFetch", image, point, lod );
	}

	Int4 TextureFetch (const Image<ImageType2DMSArray, ReadOnly, Int4> &image, const Int3 &point, const Int &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureFetch", image, point, lod, offset );
	}

	UInt4 TextureFetch (const Image<ImageType2DMSArray, ReadOnly, UInt4> &image, const Int3 &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureFetch", image, point, lod );
	}

	UInt4 TextureFetch (const Image<ImageType2DMSArray, ReadOnly, UInt4> &image, const Int3 &point, const Int &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureFetch", image, point, lod, offset );
	}

	Float4 TextureFetch (const Image<ImageType2DMSArray, ReadOnly, Float4> &image, const Int3 &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureFetch", image, point, lod );
	}

	Float4 TextureFetch (const Image<ImageType2DMSArray, ReadOnly, Float4> &image, const Int3 &point, const Int &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureFetch", image, point, lod, offset );
	}

	Int4 TextureFetch (const Image<ImageTypeCube, ReadOnly, Int4> &image, const Int3 &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureFetch", image, point, lod );
	}

	Int4 TextureFetch (const Image<ImageTypeCube, ReadOnly, Int4> &image, const Int3 &point, const Int &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureFetch", image, point, lod, offset );
	}

	UInt4 TextureFetch (const Image<ImageTypeCube, ReadOnly, UInt4> &image, const Int3 &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureFetch", image, point, lod );
	}

	UInt4 TextureFetch (const Image<ImageTypeCube, ReadOnly, UInt4> &image, const Int3 &point, const Int &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureFetch", image, point, lod, offset );
	}

	Float4 TextureFetch (const Image<ImageTypeCube, ReadOnly, Float4> &image, const Int3 &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureFetch", image, point, lod );
	}

	Float4 TextureFetch (const Image<ImageTypeCube, ReadOnly, Float4> &image, const Int3 &point, const Int &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureFetch", image, point, lod, offset );
	}

	Int4 TextureFetch (const Image<ImageTypeCubeArray, ReadOnly, Int4> &image, const Int3 &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureFetch", image, point, lod );
	}

	Int4 TextureFetch (const Image<ImageTypeCubeArray, ReadOnly, Int4> &image, const Int3 &point, const Int &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureFetch", image, point, lod, offset );
	}

	UInt4 TextureFetch (const Image<ImageTypeCubeArray, ReadOnly, UInt4> &image, const Int3 &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureFetch", image, point, lod );
	}

	UInt4 TextureFetch (const Image<ImageTypeCubeArray, ReadOnly, UInt4> &image, const Int3 &point, const Int &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureFetch", image, point, lod, offset );
	}

	Float4 TextureFetch (const Image<ImageTypeCubeArray, ReadOnly, Float4> &image, const Int3 &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureFetch", image, point, lod );
	}

	Float4 TextureFetch (const Image<ImageTypeCubeArray, ReadOnly, Float4> &image, const Int3 &point, const Int &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureFetch", image, point, lod, offset );
	}

	Int4 TextureFetch (const Image<ImageType3D, ReadOnly, Int4> &image, const Int3 &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureFetch", image, point, lod );
	}

	Int4 TextureFetch (const Image<ImageType3D, ReadOnly, Int4> &image, const Int3 &point, const Int &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Int4 >( "TextureFetch", image, point, lod, offset );
	}

	UInt4 TextureFetch (const Image<ImageType3D, ReadOnly, UInt4> &image, const Int3 &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureFetch", image, point, lod );
	}

	UInt4 TextureFetch (const Image<ImageType3D, ReadOnly, UInt4> &image, const Int3 &point, const Int &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< UInt4 >( "TextureFetch", image, point, lod, offset );
	}

	Float4 TextureFetch (const Image<ImageType3D, ReadOnly, Float4> &image, const Int3 &point, const Int &lod) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureFetch", image, point, lod );
	}

	Float4 TextureFetch (const Image<ImageType3D, ReadOnly, Float4> &image, const Int3 &point, const Int &lod, const Int3 &offset) {
		return NodeFunctions::Instance()->Build< Float4 >( "TextureFetch", image, point, lod, offset );
	}


	// TextureGetSamples, TextureGetSize, TextureQueryLod, TextureQueryLevels
	Int TextureGetSize (const Image<ImageType1D, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureGetSize", image );
	}

	Float2 TextureQueryLod (const Image<ImageType1D, ReadOnly, Int4> &image, const Float &point) {
		return NodeFunctions::Instance()->Build< Float2 >( "TextureQueryLod", image, point );
	}

	Int TextureQueryLevels (const Image<ImageType1D, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureQueryLevels", image );
	}

	Int TextureGetSize (const Image<ImageType1D, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureGetSize", image );
	}

	Float2 TextureQueryLod (const Image<ImageType1D, ReadOnly, UInt4> &image, const Float &point) {
		return NodeFunctions::Instance()->Build< Float2 >( "TextureQueryLod", image, point );
	}

	Int TextureQueryLevels (const Image<ImageType1D, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureQueryLevels", image );
	}

	Int TextureGetSize (const Image<ImageType1D, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureGetSize", image );
	}

	Float2 TextureQueryLod (const Image<ImageType1D, ReadOnly, Float4> &image, const Float &point) {
		return NodeFunctions::Instance()->Build< Float2 >( "TextureQueryLod", image, point );
	}

	Int TextureQueryLevels (const Image<ImageType1D, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureQueryLevels", image );
	}

	Int2 TextureGetSize (const Image<ImageType1DArray, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "TextureGetSize", image );
	}

	Float2 TextureQueryLod (const Image<ImageType1DArray, ReadOnly, Int4> &image, const Float &point) {
		return NodeFunctions::Instance()->Build< Float2 >( "TextureQueryLod", image, point );
	}

	Int TextureQueryLevels (const Image<ImageType1DArray, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureQueryLevels", image );
	}

	Int2 TextureGetSize (const Image<ImageType1DArray, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "TextureGetSize", image );
	}

	Float2 TextureQueryLod (const Image<ImageType1DArray, ReadOnly, UInt4> &image, const Float &point) {
		return NodeFunctions::Instance()->Build< Float2 >( "TextureQueryLod", image, point );
	}

	Int TextureQueryLevels (const Image<ImageType1DArray, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureQueryLevels", image );
	}

	Int2 TextureGetSize (const Image<ImageType1DArray, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "TextureGetSize", image );
	}

	Float2 TextureQueryLod (const Image<ImageType1DArray, ReadOnly, Float4> &image, const Float &point) {
		return NodeFunctions::Instance()->Build< Float2 >( "TextureQueryLod", image, point );
	}

	Int TextureQueryLevels (const Image<ImageType1DArray, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureQueryLevels", image );
	}

	Int2 TextureGetSize (const Image<ImageType2D, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "TextureGetSize", image );
	}

	Float2 TextureQueryLod (const Image<ImageType2D, ReadOnly, Int4> &image, const Float2 &point) {
		return NodeFunctions::Instance()->Build< Float2 >( "TextureQueryLod", image, point );
	}

	Int TextureQueryLevels (const Image<ImageType2D, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureQueryLevels", image );
	}

	Int2 TextureGetSize (const Image<ImageType2D, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "TextureGetSize", image );
	}

	Float2 TextureQueryLod (const Image<ImageType2D, ReadOnly, UInt4> &image, const Float2 &point) {
		return NodeFunctions::Instance()->Build< Float2 >( "TextureQueryLod", image, point );
	}

	Int TextureQueryLevels (const Image<ImageType2D, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureQueryLevels", image );
	}

	Int2 TextureGetSize (const Image<ImageType2D, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int2 >( "TextureGetSize", image );
	}

	Float2 TextureQueryLod (const Image<ImageType2D, ReadOnly, Float4> &image, const Float2 &point) {
		return NodeFunctions::Instance()->Build< Float2 >( "TextureQueryLod", image, point );
	}

	Int TextureQueryLevels (const Image<ImageType2D, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureQueryLevels", image );
	}

	Int3 TextureGetSize (const Image<ImageType2DArray, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "TextureGetSize", image );
	}

	Float2 TextureQueryLod (const Image<ImageType2DArray, ReadOnly, Int4> &image, const Float2 &point) {
		return NodeFunctions::Instance()->Build< Float2 >( "TextureQueryLod", image, point );
	}

	Int TextureQueryLevels (const Image<ImageType2DArray, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureQueryLevels", image );
	}

	Int3 TextureGetSize (const Image<ImageType2DArray, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "TextureGetSize", image );
	}

	Float2 TextureQueryLod (const Image<ImageType2DArray, ReadOnly, UInt4> &image, const Float2 &point) {
		return NodeFunctions::Instance()->Build< Float2 >( "TextureQueryLod", image, point );
	}

	Int TextureQueryLevels (const Image<ImageType2DArray, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureQueryLevels", image );
	}

	Int3 TextureGetSize (const Image<ImageType2DArray, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "TextureGetSize", image );
	}

	Float2 TextureQueryLod (const Image<ImageType2DArray, ReadOnly, Float4> &image, const Float2 &point) {
		return NodeFunctions::Instance()->Build< Float2 >( "TextureQueryLod", image, point );
	}

	Int TextureQueryLevels (const Image<ImageType2DArray, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureQueryLevels", image );
	}

	Int TextureGetSamples (const Image<ImageType2DMS, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureGetSamples", image );
	}

	Int2 TextureGetSize (const Image<ImageType2DMS, ReadOnly, Int4> &image, const Int &lod) {
		return NodeFunctions::Instance()->Build< Int2 >( "TextureGetSize", image, lod );
	}

	Int TextureGetSamples (const Image<ImageType2DMS, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureGetSamples", image );
	}

	Int2 TextureGetSize (const Image<ImageType2DMS, ReadOnly, UInt4> &image, const Int &lod) {
		return NodeFunctions::Instance()->Build< Int2 >( "TextureGetSize", image, lod );
	}

	Int TextureGetSamples (const Image<ImageType2DMS, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureGetSamples", image );
	}

	Int2 TextureGetSize (const Image<ImageType2DMS, ReadOnly, Float4> &image, const Int &lod) {
		return NodeFunctions::Instance()->Build< Int2 >( "TextureGetSize", image, lod );
	}

	Int TextureGetSamples (const Image<ImageType2DMSArray, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureGetSamples", image );
	}

	Int3 TextureGetSize (const Image<ImageType2DMSArray, ReadOnly, Int4> &image, const Int &lod) {
		return NodeFunctions::Instance()->Build< Int3 >( "TextureGetSize", image, lod );
	}

	Int TextureGetSamples (const Image<ImageType2DMSArray, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureGetSamples", image );
	}

	Int3 TextureGetSize (const Image<ImageType2DMSArray, ReadOnly, UInt4> &image, const Int &lod) {
		return NodeFunctions::Instance()->Build< Int3 >( "TextureGetSize", image, lod );
	}

	Int TextureGetSamples (const Image<ImageType2DMSArray, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureGetSamples", image );
	}

	Int3 TextureGetSize (const Image<ImageType2DMSArray, ReadOnly, Float4> &image, const Int &lod) {
		return NodeFunctions::Instance()->Build< Int3 >( "TextureGetSize", image, lod );
	}

	Int3 TextureGetSize (const Image<ImageTypeCube, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "TextureGetSize", image );
	}

	Float2 TextureQueryLod (const Image<ImageTypeCube, ReadOnly, Int4> &image, const Float2 &point) {
		return NodeFunctions::Instance()->Build< Float2 >( "TextureQueryLod", image, point );
	}

	Int TextureQueryLevels (const Image<ImageTypeCube, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureQueryLevels", image );
	}

	Int3 TextureGetSize (const Image<ImageTypeCube, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "TextureGetSize", image );
	}

	Float2 TextureQueryLod (const Image<ImageTypeCube, ReadOnly, UInt4> &image, const Float2 &point) {
		return NodeFunctions::Instance()->Build< Float2 >( "TextureQueryLod", image, point );
	}

	Int TextureQueryLevels (const Image<ImageTypeCube, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureQueryLevels", image );
	}

	Int3 TextureGetSize (const Image<ImageTypeCube, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "TextureGetSize", image );
	}

	Float2 TextureQueryLod (const Image<ImageTypeCube, ReadOnly, Float4> &image, const Float2 &point) {
		return NodeFunctions::Instance()->Build< Float2 >( "TextureQueryLod", image, point );
	}

	Int TextureQueryLevels (const Image<ImageTypeCube, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureQueryLevels", image );
	}

	Int3 TextureGetSize (const Image<ImageTypeCubeArray, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "TextureGetSize", image );
	}

	Float2 TextureQueryLod (const Image<ImageTypeCubeArray, ReadOnly, Int4> &image, const Float2 &point) {
		return NodeFunctions::Instance()->Build< Float2 >( "TextureQueryLod", image, point );
	}

	Int TextureQueryLevels (const Image<ImageTypeCubeArray, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureQueryLevels", image );
	}

	Int3 TextureGetSize (const Image<ImageTypeCubeArray, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "TextureGetSize", image );
	}

	Float2 TextureQueryLod (const Image<ImageTypeCubeArray, ReadOnly, UInt4> &image, const Float2 &point) {
		return NodeFunctions::Instance()->Build< Float2 >( "TextureQueryLod", image, point );
	}

	Int TextureQueryLevels (const Image<ImageTypeCubeArray, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureQueryLevels", image );
	}

	Int3 TextureGetSize (const Image<ImageTypeCubeArray, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "TextureGetSize", image );
	}

	Float2 TextureQueryLod (const Image<ImageTypeCubeArray, ReadOnly, Float4> &image, const Float2 &point) {
		return NodeFunctions::Instance()->Build< Float2 >( "TextureQueryLod", image, point );
	}

	Int TextureQueryLevels (const Image<ImageTypeCubeArray, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureQueryLevels", image );
	}

	Int3 TextureGetSize (const Image<ImageType3D, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "TextureGetSize", image );
	}

	Float2 TextureQueryLod (const Image<ImageType3D, ReadOnly, Int4> &image, const Float3 &point) {
		return NodeFunctions::Instance()->Build< Float2 >( "TextureQueryLod", image, point );
	}

	Int TextureQueryLevels (const Image<ImageType3D, ReadOnly, Int4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureQueryLevels", image );
	}

	Int3 TextureGetSize (const Image<ImageType3D, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "TextureGetSize", image );
	}

	Float2 TextureQueryLod (const Image<ImageType3D, ReadOnly, UInt4> &image, const Float3 &point) {
		return NodeFunctions::Instance()->Build< Float2 >( "TextureQueryLod", image, point );
	}

	Int TextureQueryLevels (const Image<ImageType3D, ReadOnly, UInt4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureQueryLevels", image );
	}

	Int3 TextureGetSize (const Image<ImageType3D, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int3 >( "TextureGetSize", image );
	}

	Float2 TextureQueryLod (const Image<ImageType3D, ReadOnly, Float4> &image, const Float3 &point) {
		return NodeFunctions::Instance()->Build< Float2 >( "TextureQueryLod", image, point );
	}

	Int TextureQueryLevels (const Image<ImageType3D, ReadOnly, Float4> &image) {
		return NodeFunctions::Instance()->Build< Int >( "TextureQueryLevels", image );
	}


	// BufferLoad, BufferStore

	// global/group/local index/coord/size

	// global linear index
	Int   CS_GetGlobalIndexSize () {
		return NodeFunctions::Instance()->Build< Int >( "CS_GetGlobalIndexSize" );
	}

	Int   CS_GetGlobalIndex () {
		return NodeFunctions::Instance()->Build< Int >( "CS_GetGlobalIndex" );
	}

	// local linear index
	Int   CS_GetLocalIndexSize () {
		return NodeFunctions::Instance()->Build< Int >( "CS_GetLocalIndexSize" );
	}

	Int   CS_GetLocalIndex () {
		return NodeFunctions::Instance()->Build< Int >( "CS_GetLocalIndex" );
	}

	// group linear index
	Int   CS_GetGroupIndexSize () {
		return NodeFunctions::Instance()->Build< Int >( "CS_GetGroupIndexSize" );
	}

	Int   CS_GetGroupIndex () {
		return NodeFunctions::Instance()->Build< Int >( "CS_GetGroupIndex" );
	}

	// global coordinate in 3D
	Int3   CS_GetGlobalCoordSize () {
		return NodeFunctions::Instance()->Build< Int3 >( "CS_GetGlobalCoordSize" );
	}

	Int3   CS_GetGlobalCoord () {
		return NodeFunctions::Instance()->Build< Int3 >( "CS_GetGlobalCoord" );
	}

	// local coordinate in 3D
	Int3   CS_GetLocalCoordSize () {
		return NodeFunctions::Instance()->Build< Int3 >( "CS_GetLocalCoordSize" );
	}

	Int3   CS_GetLocalCoord () {
		return NodeFunctions::Instance()->Build< Int3 >( "CS_GetLocalCoord" );
	}

	// group coordinate in 3D
	Int3   CS_GetGroupCoordSize () {
		return NodeFunctions::Instance()->Build< Int3 >( "CS_GetGroupCoordSize" );
	}

	Int3   CS_GetGroupCoord () {
		return NodeFunctions::Instance()->Build< Int3 >( "CS_GetGroupCoord" );
	}
		
	// shader inputs

	Int  VS_GetInstanceID () {
		return NodeFunctions::Instance()->Build< Int >( "VS_GetInstanceID" );
	}

	Int  VS_GetVertexID () {
		return NodeFunctions::Instance()->Build< Int >( "VS_GetVertexID" );
	}

	Int  TCS_GetInvocationID () {
		return NodeFunctions::Instance()->Build< Int >( "TCS_GetInvocationID" );
	}

	Int  TCS_GetPatchVerticesIn () {
		return NodeFunctions::Instance()->Build< Int >( "TCS_GetPatchVerticesIn" );
	}

	Int  TCS_GetPrimitiveID () {
		return NodeFunctions::Instance()->Build< Int >( "TCS_GetPrimitiveID" );
	}

	Int    TES_GetPatchVerticesIn () {
		return NodeFunctions::Instance()->Build< Int >( "TES_GetPatchVerticesIn" );
	}

	Int    TES_GetPrimitiveID () {
		return NodeFunctions::Instance()->Build< Int >( "TES_GetPrimitiveID" );
	}

	Float3 TES_GetTessCoord () {
		return NodeFunctions::Instance()->Build< Float3 >( "TES_GetTessCoord" );
	}

	Float2 TES_GetTessLevelInner () {
		return NodeFunctions::Instance()->Build< Float2 >( "TES_GetTessLevelInner" );
	}

	Float4 TES_GetTessLevelOuter () {
		return NodeFunctions::Instance()->Build< Float4 >( "TES_GetTessLevelOuter" );
	}

	Int  GS_GetPrimitiveIDIn () {
		return NodeFunctions::Instance()->Build< Int >( "GS_GetPrimitiveIDIn" );
	}

	Int2   FS_GetScreenResolution () {
		return NodeFunctions::Instance()->Build< Int2 >( "FS_GetScreenResolution" );
	}

	Float4 FS_GetFragCoord () {
		return NodeFunctions::Instance()->Build< Float4 >( "FS_GetFragCoord" );
	}

	Bool   FS_GetFrontFacing () {
		return NodeFunctions::Instance()->Build< Bool >( "FS_GetFrontFacing" );
	}

	Bool   FS_GetHelperInvocation () {
		return NodeFunctions::Instance()->Build< Bool >( "FS_GetHelperInvocation" );
	}

	Int    FS_GetLayerIndex () {
		return NodeFunctions::Instance()->Build< Int >( "FS_GetLayerIndex" );
	}

	Int    FS_GetNumSamples () {
		return NodeFunctions::Instance()->Build< Int >( "FS_GetNumSamples" );
	}

	Float2 FS_GetPointCoord () {
		return NodeFunctions::Instance()->Build< Float2 >( "FS_GetPointCoord" );
	}

	Int    FS_GetPrimitiveID () {
		return NodeFunctions::Instance()->Build< Int >( "FS_GetPrimitiveID" );
	}

	Int    FS_GetSampleID () {
		return NodeFunctions::Instance()->Build< Int >( "FS_GetSampleID" );
	}

	Int    FS_GetViewportIndex () {
		return NodeFunctions::Instance()->Build< Int >( "FS_GetViewportIndex" );
	}
		

}	// ShaderNodes
}	// ShaderEditor
