
// This is generated file, don't change anything!

#include "Engine/Shaders/Shader/NodeTypes.h"
#include "Engine/Shaders/Shader/ImageType.hpp"
#include "Engine/Shaders/Shader/AllNodeFuncs.h"

namespace ShaderEditor
{
namespace GLSLShaderNodes
{
	using namespace ShaderNodes;

	using ShaderOutputNode = _ShaderNodesHidden_::ShaderOutputNode;
	using FunctionNode = _ShaderNodesHidden_::FunctionNode;

	template <typename T>
	inline void Replace (OUT T &node, GX_STL::GXTypes::StringCRef name, FunctionNode &parent)
	{
		node.~T();
		new (&node) T( &parent, name );
	}

	inline void Replace (OUT ShaderOutputNode &node, GX_STL::GXTypes::StringCRef name, FunctionNode &parent)
	{
		node.~ShaderOutputNode();
		new (&node) ShaderOutputNode( &parent );
	}
	// Math operators
	bool Not_inBool_outBool (const Bool &x, OUT Bool &out)
	{
		FunctionNode	func( "Not", "bool Not (const bool x) {\n\treturn !(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Not_inBool2_outBool2 (const Bool2 &x, OUT Bool2 &out)
	{
		FunctionNode	func( "Not", "bool2 Not (const bool2 x) {\n\treturn not(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Not_inBool3_outBool3 (const Bool3 &x, OUT Bool3 &out)
	{
		FunctionNode	func( "Not", "bool3 Not (const bool3 x) {\n\treturn not(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Not_inBool4_outBool4 (const Bool4 &x, OUT Bool4 &out)
	{
		FunctionNode	func( "Not", "bool4 Not (const bool4 x) {\n\treturn not(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool BoolAnd_inBool_inBool_outBool (const Bool &x, const Bool &y, OUT Bool &out)
	{
		FunctionNode	func( "BoolAnd", "bool BoolAnd (const bool x, const bool y) {\n\treturn (x) && (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BoolAnd_inBool2_inBool2_outBool2 (const Bool2 &x, const Bool2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "BoolAnd", "bool2 BoolAnd (const bool2 x, const bool2 y) {\n\treturn (x) && (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BoolAnd_inBool_inBool2_outBool2 (const Bool &x, const Bool2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "BoolAnd", "bool2 BoolAnd (const bool x, const bool2 y) {\n\treturn bool2(x) && (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BoolAnd_inBool2_inBool_outBool2 (const Bool2 &x, const Bool &y, OUT Bool2 &out)
	{
		FunctionNode	func( "BoolAnd", "bool2 BoolAnd (const bool2 x, const bool y) {\n\treturn (x) && bool2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BoolAnd_inBool3_inBool3_outBool3 (const Bool3 &x, const Bool3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "BoolAnd", "bool3 BoolAnd (const bool3 x, const bool3 y) {\n\treturn (x) && (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BoolAnd_inBool_inBool3_outBool3 (const Bool &x, const Bool3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "BoolAnd", "bool3 BoolAnd (const bool x, const bool3 y) {\n\treturn bool3(x) && (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BoolAnd_inBool3_inBool_outBool3 (const Bool3 &x, const Bool &y, OUT Bool3 &out)
	{
		FunctionNode	func( "BoolAnd", "bool3 BoolAnd (const bool3 x, const bool y) {\n\treturn (x) && bool3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BoolAnd_inBool4_inBool4_outBool4 (const Bool4 &x, const Bool4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "BoolAnd", "bool4 BoolAnd (const bool4 x, const bool4 y) {\n\treturn (x) && (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BoolAnd_inBool_inBool4_outBool4 (const Bool &x, const Bool4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "BoolAnd", "bool4 BoolAnd (const bool x, const bool4 y) {\n\treturn bool4(x) && (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BoolAnd_inBool4_inBool_outBool4 (const Bool4 &x, const Bool &y, OUT Bool4 &out)
	{
		FunctionNode	func( "BoolAnd", "bool4 BoolAnd (const bool4 x, const bool y) {\n\treturn (x) && bool4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BoolOr_inBool_inBool_outBool (const Bool &x, const Bool &y, OUT Bool &out)
	{
		FunctionNode	func( "BoolOr", "bool BoolOr (const bool x, const bool y) {\n\treturn (x) || (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BoolOr_inBool2_inBool2_outBool2 (const Bool2 &x, const Bool2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "BoolOr", "bool2 BoolOr (const bool2 x, const bool2 y) {\n\treturn (x) || (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BoolOr_inBool_inBool2_outBool2 (const Bool &x, const Bool2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "BoolOr", "bool2 BoolOr (const bool x, const bool2 y) {\n\treturn bool2(x) || (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BoolOr_inBool2_inBool_outBool2 (const Bool2 &x, const Bool &y, OUT Bool2 &out)
	{
		FunctionNode	func( "BoolOr", "bool2 BoolOr (const bool2 x, const bool y) {\n\treturn (x) || bool2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BoolOr_inBool3_inBool3_outBool3 (const Bool3 &x, const Bool3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "BoolOr", "bool3 BoolOr (const bool3 x, const bool3 y) {\n\treturn (x) || (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BoolOr_inBool_inBool3_outBool3 (const Bool &x, const Bool3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "BoolOr", "bool3 BoolOr (const bool x, const bool3 y) {\n\treturn bool3(x) || (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BoolOr_inBool3_inBool_outBool3 (const Bool3 &x, const Bool &y, OUT Bool3 &out)
	{
		FunctionNode	func( "BoolOr", "bool3 BoolOr (const bool3 x, const bool y) {\n\treturn (x) || bool3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BoolOr_inBool4_inBool4_outBool4 (const Bool4 &x, const Bool4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "BoolOr", "bool4 BoolOr (const bool4 x, const bool4 y) {\n\treturn (x) || (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BoolOr_inBool_inBool4_outBool4 (const Bool &x, const Bool4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "BoolOr", "bool4 BoolOr (const bool x, const bool4 y) {\n\treturn bool4(x) || (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BoolOr_inBool4_inBool_outBool4 (const Bool4 &x, const Bool &y, OUT Bool4 &out)
	{
		FunctionNode	func( "BoolOr", "bool4 BoolOr (const bool4 x, const bool y) {\n\treturn (x) || bool4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inBool_inBool_outBool (const Bool &x, const Bool &y, OUT Bool &out)
	{
		FunctionNode	func( "Equal", "bool Equal (const bool x, const bool y) {\n\treturn (x) == (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inBool2_inBool2_outBool2 (const Bool2 &x, const Bool2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "Equal", "bool2 Equal (const bool2 x, const bool2 y) {\n\treturn equal( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inBool_inBool2_outBool2 (const Bool &x, const Bool2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "Equal", "bool2 Equal (const bool x, const bool2 y) {\n\treturn equal( bool2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inBool2_inBool_outBool2 (const Bool2 &x, const Bool &y, OUT Bool2 &out)
	{
		FunctionNode	func( "Equal", "bool2 Equal (const bool2 x, const bool y) {\n\treturn equal( (x), bool2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inBool3_inBool3_outBool3 (const Bool3 &x, const Bool3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "Equal", "bool3 Equal (const bool3 x, const bool3 y) {\n\treturn equal( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inBool_inBool3_outBool3 (const Bool &x, const Bool3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "Equal", "bool3 Equal (const bool x, const bool3 y) {\n\treturn equal( bool3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inBool3_inBool_outBool3 (const Bool3 &x, const Bool &y, OUT Bool3 &out)
	{
		FunctionNode	func( "Equal", "bool3 Equal (const bool3 x, const bool y) {\n\treturn equal( (x), bool3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inBool4_inBool4_outBool4 (const Bool4 &x, const Bool4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "Equal", "bool4 Equal (const bool4 x, const bool4 y) {\n\treturn equal( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inBool_inBool4_outBool4 (const Bool &x, const Bool4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "Equal", "bool4 Equal (const bool x, const bool4 y) {\n\treturn equal( bool4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inBool4_inBool_outBool4 (const Bool4 &x, const Bool &y, OUT Bool4 &out)
	{
		FunctionNode	func( "Equal", "bool4 Equal (const bool4 x, const bool y) {\n\treturn equal( (x), bool4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inBool_inBool_outBool (const Bool &x, const Bool &y, OUT Bool &out)
	{
		FunctionNode	func( "NotEqual", "bool NotEqual (const bool x, const bool y) {\n\treturn (x) != (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inBool2_inBool2_outBool2 (const Bool2 &x, const Bool2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "NotEqual", "bool2 NotEqual (const bool2 x, const bool2 y) {\n\treturn notEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inBool_inBool2_outBool2 (const Bool &x, const Bool2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "NotEqual", "bool2 NotEqual (const bool x, const bool2 y) {\n\treturn notEqual( bool2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inBool2_inBool_outBool2 (const Bool2 &x, const Bool &y, OUT Bool2 &out)
	{
		FunctionNode	func( "NotEqual", "bool2 NotEqual (const bool2 x, const bool y) {\n\treturn notEqual( (x), bool2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inBool3_inBool3_outBool3 (const Bool3 &x, const Bool3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "NotEqual", "bool3 NotEqual (const bool3 x, const bool3 y) {\n\treturn notEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inBool_inBool3_outBool3 (const Bool &x, const Bool3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "NotEqual", "bool3 NotEqual (const bool x, const bool3 y) {\n\treturn notEqual( bool3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inBool3_inBool_outBool3 (const Bool3 &x, const Bool &y, OUT Bool3 &out)
	{
		FunctionNode	func( "NotEqual", "bool3 NotEqual (const bool3 x, const bool y) {\n\treturn notEqual( (x), bool3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inBool4_inBool4_outBool4 (const Bool4 &x, const Bool4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "NotEqual", "bool4 NotEqual (const bool4 x, const bool4 y) {\n\treturn notEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inBool_inBool4_outBool4 (const Bool &x, const Bool4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "NotEqual", "bool4 NotEqual (const bool x, const bool4 y) {\n\treturn notEqual( bool4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inBool4_inBool_outBool4 (const Bool4 &x, const Bool &y, OUT Bool4 &out)
	{
		FunctionNode	func( "NotEqual", "bool4 NotEqual (const bool4 x, const bool y) {\n\treturn notEqual( (x), bool4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitInverse_inInt_outInt (const Int &x, OUT Int &out)
	{
		FunctionNode	func( "BitInverse", "int BitInverse (const int x) {\n\treturn ~(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool BitInverse_inInt2_outInt2 (const Int2 &x, OUT Int2 &out)
	{
		FunctionNode	func( "BitInverse", "int2 BitInverse (const int2 x) {\n\treturn ~(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool BitInverse_inInt3_outInt3 (const Int3 &x, OUT Int3 &out)
	{
		FunctionNode	func( "BitInverse", "int3 BitInverse (const int3 x) {\n\treturn ~(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool BitInverse_inInt4_outInt4 (const Int4 &x, OUT Int4 &out)
	{
		FunctionNode	func( "BitInverse", "int4 BitInverse (const int4 x) {\n\treturn ~(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Plus_inInt_outInt (const Int &x, OUT Int &out)
	{
		FunctionNode	func( "Plus", "int Plus (const int x) {\n\treturn +(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Plus_inInt2_outInt2 (const Int2 &x, OUT Int2 &out)
	{
		FunctionNode	func( "Plus", "int2 Plus (const int2 x) {\n\treturn +(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Plus_inInt3_outInt3 (const Int3 &x, OUT Int3 &out)
	{
		FunctionNode	func( "Plus", "int3 Plus (const int3 x) {\n\treturn +(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Plus_inInt4_outInt4 (const Int4 &x, OUT Int4 &out)
	{
		FunctionNode	func( "Plus", "int4 Plus (const int4 x) {\n\treturn +(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Minus_inInt_outInt (const Int &x, OUT Int &out)
	{
		FunctionNode	func( "Minus", "int Minus (const int x) {\n\treturn -(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Minus_inInt2_outInt2 (const Int2 &x, OUT Int2 &out)
	{
		FunctionNode	func( "Minus", "int2 Minus (const int2 x) {\n\treturn -(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Minus_inInt3_outInt3 (const Int3 &x, OUT Int3 &out)
	{
		FunctionNode	func( "Minus", "int3 Minus (const int3 x) {\n\treturn -(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Minus_inInt4_outInt4 (const Int4 &x, OUT Int4 &out)
	{
		FunctionNode	func( "Minus", "int4 Minus (const int4 x) {\n\treturn -(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inInt_inInt_outInt (const Int &x, const Int &y, OUT Int &out)
	{
		FunctionNode	func( "Add", "int Add (const int x, const int y) {\n\treturn (x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inInt2_inInt2_outInt2 (const Int2 &x, const Int2 &y, OUT Int2 &out)
	{
		FunctionNode	func( "Add", "int2 Add (const int2 x, const int2 y) {\n\treturn (x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inInt_inInt2_outInt2 (const Int &x, const Int2 &y, OUT Int2 &out)
	{
		FunctionNode	func( "Add", "int2 Add (const int x, const int2 y) {\n\treturn int2(x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inInt2_inInt_outInt2 (const Int2 &x, const Int &y, OUT Int2 &out)
	{
		FunctionNode	func( "Add", "int2 Add (const int2 x, const int y) {\n\treturn (x) + int2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inInt3_inInt3_outInt3 (const Int3 &x, const Int3 &y, OUT Int3 &out)
	{
		FunctionNode	func( "Add", "int3 Add (const int3 x, const int3 y) {\n\treturn (x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inInt_inInt3_outInt3 (const Int &x, const Int3 &y, OUT Int3 &out)
	{
		FunctionNode	func( "Add", "int3 Add (const int x, const int3 y) {\n\treturn int3(x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inInt3_inInt_outInt3 (const Int3 &x, const Int &y, OUT Int3 &out)
	{
		FunctionNode	func( "Add", "int3 Add (const int3 x, const int y) {\n\treturn (x) + int3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inInt4_inInt4_outInt4 (const Int4 &x, const Int4 &y, OUT Int4 &out)
	{
		FunctionNode	func( "Add", "int4 Add (const int4 x, const int4 y) {\n\treturn (x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inInt_inInt4_outInt4 (const Int &x, const Int4 &y, OUT Int4 &out)
	{
		FunctionNode	func( "Add", "int4 Add (const int x, const int4 y) {\n\treturn int4(x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inInt4_inInt_outInt4 (const Int4 &x, const Int &y, OUT Int4 &out)
	{
		FunctionNode	func( "Add", "int4 Add (const int4 x, const int y) {\n\treturn (x) + int4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inInt_inInt_outInt (const Int &x, const Int &y, OUT Int &out)
	{
		FunctionNode	func( "Sub", "int Sub (const int x, const int y) {\n\treturn (x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inInt2_inInt2_outInt2 (const Int2 &x, const Int2 &y, OUT Int2 &out)
	{
		FunctionNode	func( "Sub", "int2 Sub (const int2 x, const int2 y) {\n\treturn (x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inInt_inInt2_outInt2 (const Int &x, const Int2 &y, OUT Int2 &out)
	{
		FunctionNode	func( "Sub", "int2 Sub (const int x, const int2 y) {\n\treturn int2(x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inInt2_inInt_outInt2 (const Int2 &x, const Int &y, OUT Int2 &out)
	{
		FunctionNode	func( "Sub", "int2 Sub (const int2 x, const int y) {\n\treturn (x) - int2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inInt3_inInt3_outInt3 (const Int3 &x, const Int3 &y, OUT Int3 &out)
	{
		FunctionNode	func( "Sub", "int3 Sub (const int3 x, const int3 y) {\n\treturn (x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inInt_inInt3_outInt3 (const Int &x, const Int3 &y, OUT Int3 &out)
	{
		FunctionNode	func( "Sub", "int3 Sub (const int x, const int3 y) {\n\treturn int3(x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inInt3_inInt_outInt3 (const Int3 &x, const Int &y, OUT Int3 &out)
	{
		FunctionNode	func( "Sub", "int3 Sub (const int3 x, const int y) {\n\treturn (x) - int3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inInt4_inInt4_outInt4 (const Int4 &x, const Int4 &y, OUT Int4 &out)
	{
		FunctionNode	func( "Sub", "int4 Sub (const int4 x, const int4 y) {\n\treturn (x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inInt_inInt4_outInt4 (const Int &x, const Int4 &y, OUT Int4 &out)
	{
		FunctionNode	func( "Sub", "int4 Sub (const int x, const int4 y) {\n\treturn int4(x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inInt4_inInt_outInt4 (const Int4 &x, const Int &y, OUT Int4 &out)
	{
		FunctionNode	func( "Sub", "int4 Sub (const int4 x, const int y) {\n\treturn (x) - int4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inInt_inInt_outInt (const Int &x, const Int &y, OUT Int &out)
	{
		FunctionNode	func( "Mul", "int Mul (const int x, const int y) {\n\treturn (x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inInt2_inInt2_outInt2 (const Int2 &x, const Int2 &y, OUT Int2 &out)
	{
		FunctionNode	func( "Mul", "int2 Mul (const int2 x, const int2 y) {\n\treturn (x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inInt_inInt2_outInt2 (const Int &x, const Int2 &y, OUT Int2 &out)
	{
		FunctionNode	func( "Mul", "int2 Mul (const int x, const int2 y) {\n\treturn int2(x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inInt2_inInt_outInt2 (const Int2 &x, const Int &y, OUT Int2 &out)
	{
		FunctionNode	func( "Mul", "int2 Mul (const int2 x, const int y) {\n\treturn (x) * int2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inInt3_inInt3_outInt3 (const Int3 &x, const Int3 &y, OUT Int3 &out)
	{
		FunctionNode	func( "Mul", "int3 Mul (const int3 x, const int3 y) {\n\treturn (x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inInt_inInt3_outInt3 (const Int &x, const Int3 &y, OUT Int3 &out)
	{
		FunctionNode	func( "Mul", "int3 Mul (const int x, const int3 y) {\n\treturn int3(x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inInt3_inInt_outInt3 (const Int3 &x, const Int &y, OUT Int3 &out)
	{
		FunctionNode	func( "Mul", "int3 Mul (const int3 x, const int y) {\n\treturn (x) * int3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inInt4_inInt4_outInt4 (const Int4 &x, const Int4 &y, OUT Int4 &out)
	{
		FunctionNode	func( "Mul", "int4 Mul (const int4 x, const int4 y) {\n\treturn (x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inInt_inInt4_outInt4 (const Int &x, const Int4 &y, OUT Int4 &out)
	{
		FunctionNode	func( "Mul", "int4 Mul (const int x, const int4 y) {\n\treturn int4(x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inInt4_inInt_outInt4 (const Int4 &x, const Int &y, OUT Int4 &out)
	{
		FunctionNode	func( "Mul", "int4 Mul (const int4 x, const int y) {\n\treturn (x) * int4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inInt_inInt_outInt (const Int &x, const Int &y, OUT Int &out)
	{
		FunctionNode	func( "Div", "int Div (const int x, const int y) {\n\treturn (x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inInt2_inInt2_outInt2 (const Int2 &x, const Int2 &y, OUT Int2 &out)
	{
		FunctionNode	func( "Div", "int2 Div (const int2 x, const int2 y) {\n\treturn (x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inInt_inInt2_outInt2 (const Int &x, const Int2 &y, OUT Int2 &out)
	{
		FunctionNode	func( "Div", "int2 Div (const int x, const int2 y) {\n\treturn int2(x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inInt2_inInt_outInt2 (const Int2 &x, const Int &y, OUT Int2 &out)
	{
		FunctionNode	func( "Div", "int2 Div (const int2 x, const int y) {\n\treturn (x) / int2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inInt3_inInt3_outInt3 (const Int3 &x, const Int3 &y, OUT Int3 &out)
	{
		FunctionNode	func( "Div", "int3 Div (const int3 x, const int3 y) {\n\treturn (x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inInt_inInt3_outInt3 (const Int &x, const Int3 &y, OUT Int3 &out)
	{
		FunctionNode	func( "Div", "int3 Div (const int x, const int3 y) {\n\treturn int3(x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inInt3_inInt_outInt3 (const Int3 &x, const Int &y, OUT Int3 &out)
	{
		FunctionNode	func( "Div", "int3 Div (const int3 x, const int y) {\n\treturn (x) / int3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inInt4_inInt4_outInt4 (const Int4 &x, const Int4 &y, OUT Int4 &out)
	{
		FunctionNode	func( "Div", "int4 Div (const int4 x, const int4 y) {\n\treturn (x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inInt_inInt4_outInt4 (const Int &x, const Int4 &y, OUT Int4 &out)
	{
		FunctionNode	func( "Div", "int4 Div (const int x, const int4 y) {\n\treturn int4(x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inInt4_inInt_outInt4 (const Int4 &x, const Int &y, OUT Int4 &out)
	{
		FunctionNode	func( "Div", "int4 Div (const int4 x, const int y) {\n\treturn (x) / int4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inInt_inInt_outInt (const Int &x, const Int &y, OUT Int &out)
	{
		FunctionNode	func( "Mod", "int Mod (const int x, const int y) {\n\treturn (x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inInt2_inInt2_outInt2 (const Int2 &x, const Int2 &y, OUT Int2 &out)
	{
		FunctionNode	func( "Mod", "int2 Mod (const int2 x, const int2 y) {\n\treturn (x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inInt_inInt2_outInt2 (const Int &x, const Int2 &y, OUT Int2 &out)
	{
		FunctionNode	func( "Mod", "int2 Mod (const int x, const int2 y) {\n\treturn int2(x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inInt2_inInt_outInt2 (const Int2 &x, const Int &y, OUT Int2 &out)
	{
		FunctionNode	func( "Mod", "int2 Mod (const int2 x, const int y) {\n\treturn (x) % int2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inInt3_inInt3_outInt3 (const Int3 &x, const Int3 &y, OUT Int3 &out)
	{
		FunctionNode	func( "Mod", "int3 Mod (const int3 x, const int3 y) {\n\treturn (x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inInt_inInt3_outInt3 (const Int &x, const Int3 &y, OUT Int3 &out)
	{
		FunctionNode	func( "Mod", "int3 Mod (const int x, const int3 y) {\n\treturn int3(x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inInt3_inInt_outInt3 (const Int3 &x, const Int &y, OUT Int3 &out)
	{
		FunctionNode	func( "Mod", "int3 Mod (const int3 x, const int y) {\n\treturn (x) % int3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inInt4_inInt4_outInt4 (const Int4 &x, const Int4 &y, OUT Int4 &out)
	{
		FunctionNode	func( "Mod", "int4 Mod (const int4 x, const int4 y) {\n\treturn (x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inInt_inInt4_outInt4 (const Int &x, const Int4 &y, OUT Int4 &out)
	{
		FunctionNode	func( "Mod", "int4 Mod (const int x, const int4 y) {\n\treturn int4(x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inInt4_inInt_outInt4 (const Int4 &x, const Int &y, OUT Int4 &out)
	{
		FunctionNode	func( "Mod", "int4 Mod (const int4 x, const int y) {\n\treturn (x) % int4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inInt_inInt_outInt (const Int &x, const Int &y, OUT Int &out)
	{
		FunctionNode	func( "And", "int And (const int x, const int y) {\n\treturn (x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inInt2_inInt2_outInt2 (const Int2 &x, const Int2 &y, OUT Int2 &out)
	{
		FunctionNode	func( "And", "int2 And (const int2 x, const int2 y) {\n\treturn (x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inInt_inInt2_outInt2 (const Int &x, const Int2 &y, OUT Int2 &out)
	{
		FunctionNode	func( "And", "int2 And (const int x, const int2 y) {\n\treturn int2(x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inInt2_inInt_outInt2 (const Int2 &x, const Int &y, OUT Int2 &out)
	{
		FunctionNode	func( "And", "int2 And (const int2 x, const int y) {\n\treturn (x) & int2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inInt3_inInt3_outInt3 (const Int3 &x, const Int3 &y, OUT Int3 &out)
	{
		FunctionNode	func( "And", "int3 And (const int3 x, const int3 y) {\n\treturn (x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inInt_inInt3_outInt3 (const Int &x, const Int3 &y, OUT Int3 &out)
	{
		FunctionNode	func( "And", "int3 And (const int x, const int3 y) {\n\treturn int3(x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inInt3_inInt_outInt3 (const Int3 &x, const Int &y, OUT Int3 &out)
	{
		FunctionNode	func( "And", "int3 And (const int3 x, const int y) {\n\treturn (x) & int3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inInt4_inInt4_outInt4 (const Int4 &x, const Int4 &y, OUT Int4 &out)
	{
		FunctionNode	func( "And", "int4 And (const int4 x, const int4 y) {\n\treturn (x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inInt_inInt4_outInt4 (const Int &x, const Int4 &y, OUT Int4 &out)
	{
		FunctionNode	func( "And", "int4 And (const int x, const int4 y) {\n\treturn int4(x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inInt4_inInt_outInt4 (const Int4 &x, const Int &y, OUT Int4 &out)
	{
		FunctionNode	func( "And", "int4 And (const int4 x, const int y) {\n\treturn (x) & int4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inInt_inInt_outInt (const Int &x, const Int &y, OUT Int &out)
	{
		FunctionNode	func( "Or", "int Or (const int x, const int y) {\n\treturn (x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inInt2_inInt2_outInt2 (const Int2 &x, const Int2 &y, OUT Int2 &out)
	{
		FunctionNode	func( "Or", "int2 Or (const int2 x, const int2 y) {\n\treturn (x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inInt_inInt2_outInt2 (const Int &x, const Int2 &y, OUT Int2 &out)
	{
		FunctionNode	func( "Or", "int2 Or (const int x, const int2 y) {\n\treturn int2(x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inInt2_inInt_outInt2 (const Int2 &x, const Int &y, OUT Int2 &out)
	{
		FunctionNode	func( "Or", "int2 Or (const int2 x, const int y) {\n\treturn (x) | int2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inInt3_inInt3_outInt3 (const Int3 &x, const Int3 &y, OUT Int3 &out)
	{
		FunctionNode	func( "Or", "int3 Or (const int3 x, const int3 y) {\n\treturn (x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inInt_inInt3_outInt3 (const Int &x, const Int3 &y, OUT Int3 &out)
	{
		FunctionNode	func( "Or", "int3 Or (const int x, const int3 y) {\n\treturn int3(x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inInt3_inInt_outInt3 (const Int3 &x, const Int &y, OUT Int3 &out)
	{
		FunctionNode	func( "Or", "int3 Or (const int3 x, const int y) {\n\treturn (x) | int3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inInt4_inInt4_outInt4 (const Int4 &x, const Int4 &y, OUT Int4 &out)
	{
		FunctionNode	func( "Or", "int4 Or (const int4 x, const int4 y) {\n\treturn (x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inInt_inInt4_outInt4 (const Int &x, const Int4 &y, OUT Int4 &out)
	{
		FunctionNode	func( "Or", "int4 Or (const int x, const int4 y) {\n\treturn int4(x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inInt4_inInt_outInt4 (const Int4 &x, const Int &y, OUT Int4 &out)
	{
		FunctionNode	func( "Or", "int4 Or (const int4 x, const int y) {\n\treturn (x) | int4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inInt_inInt_outInt (const Int &x, const Int &y, OUT Int &out)
	{
		FunctionNode	func( "Xor", "int Xor (const int x, const int y) {\n\treturn (x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inInt2_inInt2_outInt2 (const Int2 &x, const Int2 &y, OUT Int2 &out)
	{
		FunctionNode	func( "Xor", "int2 Xor (const int2 x, const int2 y) {\n\treturn (x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inInt_inInt2_outInt2 (const Int &x, const Int2 &y, OUT Int2 &out)
	{
		FunctionNode	func( "Xor", "int2 Xor (const int x, const int2 y) {\n\treturn int2(x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inInt2_inInt_outInt2 (const Int2 &x, const Int &y, OUT Int2 &out)
	{
		FunctionNode	func( "Xor", "int2 Xor (const int2 x, const int y) {\n\treturn (x) ^ int2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inInt3_inInt3_outInt3 (const Int3 &x, const Int3 &y, OUT Int3 &out)
	{
		FunctionNode	func( "Xor", "int3 Xor (const int3 x, const int3 y) {\n\treturn (x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inInt_inInt3_outInt3 (const Int &x, const Int3 &y, OUT Int3 &out)
	{
		FunctionNode	func( "Xor", "int3 Xor (const int x, const int3 y) {\n\treturn int3(x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inInt3_inInt_outInt3 (const Int3 &x, const Int &y, OUT Int3 &out)
	{
		FunctionNode	func( "Xor", "int3 Xor (const int3 x, const int y) {\n\treturn (x) ^ int3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inInt4_inInt4_outInt4 (const Int4 &x, const Int4 &y, OUT Int4 &out)
	{
		FunctionNode	func( "Xor", "int4 Xor (const int4 x, const int4 y) {\n\treturn (x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inInt_inInt4_outInt4 (const Int &x, const Int4 &y, OUT Int4 &out)
	{
		FunctionNode	func( "Xor", "int4 Xor (const int x, const int4 y) {\n\treturn int4(x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inInt4_inInt_outInt4 (const Int4 &x, const Int &y, OUT Int4 &out)
	{
		FunctionNode	func( "Xor", "int4 Xor (const int4 x, const int y) {\n\treturn (x) ^ int4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inInt_inInt_outInt (const Int &x, const Int &y, OUT Int &out)
	{
		FunctionNode	func( "BitLShift", "int BitLShift (const int x, const int y) {\n\treturn (x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inInt2_inInt2_outInt2 (const Int2 &x, const Int2 &y, OUT Int2 &out)
	{
		FunctionNode	func( "BitLShift", "int2 BitLShift (const int2 x, const int2 y) {\n\treturn (x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inInt_inInt2_outInt2 (const Int &x, const Int2 &y, OUT Int2 &out)
	{
		FunctionNode	func( "BitLShift", "int2 BitLShift (const int x, const int2 y) {\n\treturn int2(x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inInt2_inInt_outInt2 (const Int2 &x, const Int &y, OUT Int2 &out)
	{
		FunctionNode	func( "BitLShift", "int2 BitLShift (const int2 x, const int y) {\n\treturn (x) << int2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inInt3_inInt3_outInt3 (const Int3 &x, const Int3 &y, OUT Int3 &out)
	{
		FunctionNode	func( "BitLShift", "int3 BitLShift (const int3 x, const int3 y) {\n\treturn (x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inInt_inInt3_outInt3 (const Int &x, const Int3 &y, OUT Int3 &out)
	{
		FunctionNode	func( "BitLShift", "int3 BitLShift (const int x, const int3 y) {\n\treturn int3(x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inInt3_inInt_outInt3 (const Int3 &x, const Int &y, OUT Int3 &out)
	{
		FunctionNode	func( "BitLShift", "int3 BitLShift (const int3 x, const int y) {\n\treturn (x) << int3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inInt4_inInt4_outInt4 (const Int4 &x, const Int4 &y, OUT Int4 &out)
	{
		FunctionNode	func( "BitLShift", "int4 BitLShift (const int4 x, const int4 y) {\n\treturn (x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inInt_inInt4_outInt4 (const Int &x, const Int4 &y, OUT Int4 &out)
	{
		FunctionNode	func( "BitLShift", "int4 BitLShift (const int x, const int4 y) {\n\treturn int4(x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inInt4_inInt_outInt4 (const Int4 &x, const Int &y, OUT Int4 &out)
	{
		FunctionNode	func( "BitLShift", "int4 BitLShift (const int4 x, const int y) {\n\treturn (x) << int4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inInt_inInt_outInt (const Int &x, const Int &y, OUT Int &out)
	{
		FunctionNode	func( "BitRShift", "int BitRShift (const int x, const int y) {\n\treturn (x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inInt2_inInt2_outInt2 (const Int2 &x, const Int2 &y, OUT Int2 &out)
	{
		FunctionNode	func( "BitRShift", "int2 BitRShift (const int2 x, const int2 y) {\n\treturn (x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inInt_inInt2_outInt2 (const Int &x, const Int2 &y, OUT Int2 &out)
	{
		FunctionNode	func( "BitRShift", "int2 BitRShift (const int x, const int2 y) {\n\treturn int2(x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inInt2_inInt_outInt2 (const Int2 &x, const Int &y, OUT Int2 &out)
	{
		FunctionNode	func( "BitRShift", "int2 BitRShift (const int2 x, const int y) {\n\treturn (x) >> int2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inInt3_inInt3_outInt3 (const Int3 &x, const Int3 &y, OUT Int3 &out)
	{
		FunctionNode	func( "BitRShift", "int3 BitRShift (const int3 x, const int3 y) {\n\treturn (x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inInt_inInt3_outInt3 (const Int &x, const Int3 &y, OUT Int3 &out)
	{
		FunctionNode	func( "BitRShift", "int3 BitRShift (const int x, const int3 y) {\n\treturn int3(x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inInt3_inInt_outInt3 (const Int3 &x, const Int &y, OUT Int3 &out)
	{
		FunctionNode	func( "BitRShift", "int3 BitRShift (const int3 x, const int y) {\n\treturn (x) >> int3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inInt4_inInt4_outInt4 (const Int4 &x, const Int4 &y, OUT Int4 &out)
	{
		FunctionNode	func( "BitRShift", "int4 BitRShift (const int4 x, const int4 y) {\n\treturn (x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inInt_inInt4_outInt4 (const Int &x, const Int4 &y, OUT Int4 &out)
	{
		FunctionNode	func( "BitRShift", "int4 BitRShift (const int x, const int4 y) {\n\treturn int4(x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inInt4_inInt_outInt4 (const Int4 &x, const Int &y, OUT Int4 &out)
	{
		FunctionNode	func( "BitRShift", "int4 BitRShift (const int4 x, const int y) {\n\treturn (x) >> int4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inInt_inInt_outBool (const Int &x, const Int &y, OUT Bool &out)
	{
		FunctionNode	func( "Equal", "bool Equal (const int x, const int y) {\n\treturn (x) == (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inInt2_inInt2_outBool2 (const Int2 &x, const Int2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "Equal", "bool2 Equal (const int2 x, const int2 y) {\n\treturn equal( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inInt_inInt2_outBool2 (const Int &x, const Int2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "Equal", "bool2 Equal (const int x, const int2 y) {\n\treturn equal( int2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inInt2_inInt_outBool2 (const Int2 &x, const Int &y, OUT Bool2 &out)
	{
		FunctionNode	func( "Equal", "bool2 Equal (const int2 x, const int y) {\n\treturn equal( (x), int2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inInt3_inInt3_outBool3 (const Int3 &x, const Int3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "Equal", "bool3 Equal (const int3 x, const int3 y) {\n\treturn equal( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inInt_inInt3_outBool3 (const Int &x, const Int3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "Equal", "bool3 Equal (const int x, const int3 y) {\n\treturn equal( int3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inInt3_inInt_outBool3 (const Int3 &x, const Int &y, OUT Bool3 &out)
	{
		FunctionNode	func( "Equal", "bool3 Equal (const int3 x, const int y) {\n\treturn equal( (x), int3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inInt4_inInt4_outBool4 (const Int4 &x, const Int4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "Equal", "bool4 Equal (const int4 x, const int4 y) {\n\treturn equal( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inInt_inInt4_outBool4 (const Int &x, const Int4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "Equal", "bool4 Equal (const int x, const int4 y) {\n\treturn equal( int4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inInt4_inInt_outBool4 (const Int4 &x, const Int &y, OUT Bool4 &out)
	{
		FunctionNode	func( "Equal", "bool4 Equal (const int4 x, const int y) {\n\treturn equal( (x), int4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inInt_inInt_outBool (const Int &x, const Int &y, OUT Bool &out)
	{
		FunctionNode	func( "NotEqual", "bool NotEqual (const int x, const int y) {\n\treturn (x) != (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inInt2_inInt2_outBool2 (const Int2 &x, const Int2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "NotEqual", "bool2 NotEqual (const int2 x, const int2 y) {\n\treturn notEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inInt_inInt2_outBool2 (const Int &x, const Int2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "NotEqual", "bool2 NotEqual (const int x, const int2 y) {\n\treturn notEqual( int2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inInt2_inInt_outBool2 (const Int2 &x, const Int &y, OUT Bool2 &out)
	{
		FunctionNode	func( "NotEqual", "bool2 NotEqual (const int2 x, const int y) {\n\treturn notEqual( (x), int2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inInt3_inInt3_outBool3 (const Int3 &x, const Int3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "NotEqual", "bool3 NotEqual (const int3 x, const int3 y) {\n\treturn notEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inInt_inInt3_outBool3 (const Int &x, const Int3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "NotEqual", "bool3 NotEqual (const int x, const int3 y) {\n\treturn notEqual( int3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inInt3_inInt_outBool3 (const Int3 &x, const Int &y, OUT Bool3 &out)
	{
		FunctionNode	func( "NotEqual", "bool3 NotEqual (const int3 x, const int y) {\n\treturn notEqual( (x), int3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inInt4_inInt4_outBool4 (const Int4 &x, const Int4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "NotEqual", "bool4 NotEqual (const int4 x, const int4 y) {\n\treturn notEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inInt_inInt4_outBool4 (const Int &x, const Int4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "NotEqual", "bool4 NotEqual (const int x, const int4 y) {\n\treturn notEqual( int4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inInt4_inInt_outBool4 (const Int4 &x, const Int &y, OUT Bool4 &out)
	{
		FunctionNode	func( "NotEqual", "bool4 NotEqual (const int4 x, const int y) {\n\treturn notEqual( (x), int4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inInt_inInt_outBool (const Int &x, const Int &y, OUT Bool &out)
	{
		FunctionNode	func( "GreaterThan", "bool GreaterThan (const int x, const int y) {\n\treturn (x) > (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inInt2_inInt2_outBool2 (const Int2 &x, const Int2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThan", "bool2 GreaterThan (const int2 x, const int2 y) {\n\treturn greaterThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inInt_inInt2_outBool2 (const Int &x, const Int2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThan", "bool2 GreaterThan (const int x, const int2 y) {\n\treturn greaterThan( int2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inInt2_inInt_outBool2 (const Int2 &x, const Int &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThan", "bool2 GreaterThan (const int2 x, const int y) {\n\treturn greaterThan( (x), int2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inInt3_inInt3_outBool3 (const Int3 &x, const Int3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThan", "bool3 GreaterThan (const int3 x, const int3 y) {\n\treturn greaterThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inInt_inInt3_outBool3 (const Int &x, const Int3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThan", "bool3 GreaterThan (const int x, const int3 y) {\n\treturn greaterThan( int3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inInt3_inInt_outBool3 (const Int3 &x, const Int &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThan", "bool3 GreaterThan (const int3 x, const int y) {\n\treturn greaterThan( (x), int3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inInt4_inInt4_outBool4 (const Int4 &x, const Int4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThan", "bool4 GreaterThan (const int4 x, const int4 y) {\n\treturn greaterThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inInt_inInt4_outBool4 (const Int &x, const Int4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThan", "bool4 GreaterThan (const int x, const int4 y) {\n\treturn greaterThan( int4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inInt4_inInt_outBool4 (const Int4 &x, const Int &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThan", "bool4 GreaterThan (const int4 x, const int y) {\n\treturn greaterThan( (x), int4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inInt_inInt_outBool (const Int &x, const Int &y, OUT Bool &out)
	{
		FunctionNode	func( "LessThan", "bool LessThan (const int x, const int y) {\n\treturn (x) < (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inInt2_inInt2_outBool2 (const Int2 &x, const Int2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThan", "bool2 LessThan (const int2 x, const int2 y) {\n\treturn lessThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inInt_inInt2_outBool2 (const Int &x, const Int2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThan", "bool2 LessThan (const int x, const int2 y) {\n\treturn lessThan( int2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inInt2_inInt_outBool2 (const Int2 &x, const Int &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThan", "bool2 LessThan (const int2 x, const int y) {\n\treturn lessThan( (x), int2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inInt3_inInt3_outBool3 (const Int3 &x, const Int3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThan", "bool3 LessThan (const int3 x, const int3 y) {\n\treturn lessThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inInt_inInt3_outBool3 (const Int &x, const Int3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThan", "bool3 LessThan (const int x, const int3 y) {\n\treturn lessThan( int3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inInt3_inInt_outBool3 (const Int3 &x, const Int &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThan", "bool3 LessThan (const int3 x, const int y) {\n\treturn lessThan( (x), int3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inInt4_inInt4_outBool4 (const Int4 &x, const Int4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThan", "bool4 LessThan (const int4 x, const int4 y) {\n\treturn lessThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inInt_inInt4_outBool4 (const Int &x, const Int4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThan", "bool4 LessThan (const int x, const int4 y) {\n\treturn lessThan( int4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inInt4_inInt_outBool4 (const Int4 &x, const Int &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThan", "bool4 LessThan (const int4 x, const int y) {\n\treturn lessThan( (x), int4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inInt_inInt_outBool (const Int &x, const Int &y, OUT Bool &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool GreaterThanEqual (const int x, const int y) {\n\treturn (x) >= (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inInt2_inInt2_outBool2 (const Int2 &x, const Int2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool2 GreaterThanEqual (const int2 x, const int2 y) {\n\treturn greaterThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inInt_inInt2_outBool2 (const Int &x, const Int2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool2 GreaterThanEqual (const int x, const int2 y) {\n\treturn greaterThanEqual( int2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inInt2_inInt_outBool2 (const Int2 &x, const Int &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool2 GreaterThanEqual (const int2 x, const int y) {\n\treturn greaterThanEqual( (x), int2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inInt3_inInt3_outBool3 (const Int3 &x, const Int3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool3 GreaterThanEqual (const int3 x, const int3 y) {\n\treturn greaterThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inInt_inInt3_outBool3 (const Int &x, const Int3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool3 GreaterThanEqual (const int x, const int3 y) {\n\treturn greaterThanEqual( int3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inInt3_inInt_outBool3 (const Int3 &x, const Int &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool3 GreaterThanEqual (const int3 x, const int y) {\n\treturn greaterThanEqual( (x), int3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inInt4_inInt4_outBool4 (const Int4 &x, const Int4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool4 GreaterThanEqual (const int4 x, const int4 y) {\n\treturn greaterThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inInt_inInt4_outBool4 (const Int &x, const Int4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool4 GreaterThanEqual (const int x, const int4 y) {\n\treturn greaterThanEqual( int4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inInt4_inInt_outBool4 (const Int4 &x, const Int &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool4 GreaterThanEqual (const int4 x, const int y) {\n\treturn greaterThanEqual( (x), int4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inInt_inInt_outBool (const Int &x, const Int &y, OUT Bool &out)
	{
		FunctionNode	func( "LessThanEqual", "bool LessThanEqual (const int x, const int y) {\n\treturn (x) <= (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inInt2_inInt2_outBool2 (const Int2 &x, const Int2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool2 LessThanEqual (const int2 x, const int2 y) {\n\treturn lessThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inInt_inInt2_outBool2 (const Int &x, const Int2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool2 LessThanEqual (const int x, const int2 y) {\n\treturn lessThanEqual( int2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inInt2_inInt_outBool2 (const Int2 &x, const Int &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool2 LessThanEqual (const int2 x, const int y) {\n\treturn lessThanEqual( (x), int2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inInt3_inInt3_outBool3 (const Int3 &x, const Int3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool3 LessThanEqual (const int3 x, const int3 y) {\n\treturn lessThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inInt_inInt3_outBool3 (const Int &x, const Int3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool3 LessThanEqual (const int x, const int3 y) {\n\treturn lessThanEqual( int3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inInt3_inInt_outBool3 (const Int3 &x, const Int &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool3 LessThanEqual (const int3 x, const int y) {\n\treturn lessThanEqual( (x), int3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inInt4_inInt4_outBool4 (const Int4 &x, const Int4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool4 LessThanEqual (const int4 x, const int4 y) {\n\treturn lessThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inInt_inInt4_outBool4 (const Int &x, const Int4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool4 LessThanEqual (const int x, const int4 y) {\n\treturn lessThanEqual( int4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inInt4_inInt_outBool4 (const Int4 &x, const Int &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool4 LessThanEqual (const int4 x, const int y) {\n\treturn lessThanEqual( (x), int4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitInverse_inUInt_outUInt (const UInt &x, OUT UInt &out)
	{
		FunctionNode	func( "BitInverse", "uint BitInverse (const uint x) {\n\treturn ~(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool BitInverse_inUInt2_outUInt2 (const UInt2 &x, OUT UInt2 &out)
	{
		FunctionNode	func( "BitInverse", "uint2 BitInverse (const uint2 x) {\n\treturn ~(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool BitInverse_inUInt3_outUInt3 (const UInt3 &x, OUT UInt3 &out)
	{
		FunctionNode	func( "BitInverse", "uint3 BitInverse (const uint3 x) {\n\treturn ~(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool BitInverse_inUInt4_outUInt4 (const UInt4 &x, OUT UInt4 &out)
	{
		FunctionNode	func( "BitInverse", "uint4 BitInverse (const uint4 x) {\n\treturn ~(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Plus_inUInt_outUInt (const UInt &x, OUT UInt &out)
	{
		FunctionNode	func( "Plus", "uint Plus (const uint x) {\n\treturn +(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Plus_inUInt2_outUInt2 (const UInt2 &x, OUT UInt2 &out)
	{
		FunctionNode	func( "Plus", "uint2 Plus (const uint2 x) {\n\treturn +(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Plus_inUInt3_outUInt3 (const UInt3 &x, OUT UInt3 &out)
	{
		FunctionNode	func( "Plus", "uint3 Plus (const uint3 x) {\n\treturn +(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Plus_inUInt4_outUInt4 (const UInt4 &x, OUT UInt4 &out)
	{
		FunctionNode	func( "Plus", "uint4 Plus (const uint4 x) {\n\treturn +(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Minus_inUInt_outUInt (const UInt &x, OUT UInt &out)
	{
		FunctionNode	func( "Minus", "uint Minus (const uint x) {\n\treturn -(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Minus_inUInt2_outUInt2 (const UInt2 &x, OUT UInt2 &out)
	{
		FunctionNode	func( "Minus", "uint2 Minus (const uint2 x) {\n\treturn -(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Minus_inUInt3_outUInt3 (const UInt3 &x, OUT UInt3 &out)
	{
		FunctionNode	func( "Minus", "uint3 Minus (const uint3 x) {\n\treturn -(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Minus_inUInt4_outUInt4 (const UInt4 &x, OUT UInt4 &out)
	{
		FunctionNode	func( "Minus", "uint4 Minus (const uint4 x) {\n\treturn -(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inUInt_inUInt_outUInt (const UInt &x, const UInt &y, OUT UInt &out)
	{
		FunctionNode	func( "Add", "uint Add (const uint x, const uint y) {\n\treturn (x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inUInt2_inUInt2_outUInt2 (const UInt2 &x, const UInt2 &y, OUT UInt2 &out)
	{
		FunctionNode	func( "Add", "uint2 Add (const uint2 x, const uint2 y) {\n\treturn (x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inUInt_inUInt2_outUInt2 (const UInt &x, const UInt2 &y, OUT UInt2 &out)
	{
		FunctionNode	func( "Add", "uint2 Add (const uint x, const uint2 y) {\n\treturn uint2(x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inUInt2_inUInt_outUInt2 (const UInt2 &x, const UInt &y, OUT UInt2 &out)
	{
		FunctionNode	func( "Add", "uint2 Add (const uint2 x, const uint y) {\n\treturn (x) + uint2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inUInt3_inUInt3_outUInt3 (const UInt3 &x, const UInt3 &y, OUT UInt3 &out)
	{
		FunctionNode	func( "Add", "uint3 Add (const uint3 x, const uint3 y) {\n\treturn (x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inUInt_inUInt3_outUInt3 (const UInt &x, const UInt3 &y, OUT UInt3 &out)
	{
		FunctionNode	func( "Add", "uint3 Add (const uint x, const uint3 y) {\n\treturn uint3(x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inUInt3_inUInt_outUInt3 (const UInt3 &x, const UInt &y, OUT UInt3 &out)
	{
		FunctionNode	func( "Add", "uint3 Add (const uint3 x, const uint y) {\n\treturn (x) + uint3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inUInt4_inUInt4_outUInt4 (const UInt4 &x, const UInt4 &y, OUT UInt4 &out)
	{
		FunctionNode	func( "Add", "uint4 Add (const uint4 x, const uint4 y) {\n\treturn (x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inUInt_inUInt4_outUInt4 (const UInt &x, const UInt4 &y, OUT UInt4 &out)
	{
		FunctionNode	func( "Add", "uint4 Add (const uint x, const uint4 y) {\n\treturn uint4(x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inUInt4_inUInt_outUInt4 (const UInt4 &x, const UInt &y, OUT UInt4 &out)
	{
		FunctionNode	func( "Add", "uint4 Add (const uint4 x, const uint y) {\n\treturn (x) + uint4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inUInt_inUInt_outUInt (const UInt &x, const UInt &y, OUT UInt &out)
	{
		FunctionNode	func( "Sub", "uint Sub (const uint x, const uint y) {\n\treturn (x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inUInt2_inUInt2_outUInt2 (const UInt2 &x, const UInt2 &y, OUT UInt2 &out)
	{
		FunctionNode	func( "Sub", "uint2 Sub (const uint2 x, const uint2 y) {\n\treturn (x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inUInt_inUInt2_outUInt2 (const UInt &x, const UInt2 &y, OUT UInt2 &out)
	{
		FunctionNode	func( "Sub", "uint2 Sub (const uint x, const uint2 y) {\n\treturn uint2(x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inUInt2_inUInt_outUInt2 (const UInt2 &x, const UInt &y, OUT UInt2 &out)
	{
		FunctionNode	func( "Sub", "uint2 Sub (const uint2 x, const uint y) {\n\treturn (x) - uint2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inUInt3_inUInt3_outUInt3 (const UInt3 &x, const UInt3 &y, OUT UInt3 &out)
	{
		FunctionNode	func( "Sub", "uint3 Sub (const uint3 x, const uint3 y) {\n\treturn (x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inUInt_inUInt3_outUInt3 (const UInt &x, const UInt3 &y, OUT UInt3 &out)
	{
		FunctionNode	func( "Sub", "uint3 Sub (const uint x, const uint3 y) {\n\treturn uint3(x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inUInt3_inUInt_outUInt3 (const UInt3 &x, const UInt &y, OUT UInt3 &out)
	{
		FunctionNode	func( "Sub", "uint3 Sub (const uint3 x, const uint y) {\n\treturn (x) - uint3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inUInt4_inUInt4_outUInt4 (const UInt4 &x, const UInt4 &y, OUT UInt4 &out)
	{
		FunctionNode	func( "Sub", "uint4 Sub (const uint4 x, const uint4 y) {\n\treturn (x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inUInt_inUInt4_outUInt4 (const UInt &x, const UInt4 &y, OUT UInt4 &out)
	{
		FunctionNode	func( "Sub", "uint4 Sub (const uint x, const uint4 y) {\n\treturn uint4(x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inUInt4_inUInt_outUInt4 (const UInt4 &x, const UInt &y, OUT UInt4 &out)
	{
		FunctionNode	func( "Sub", "uint4 Sub (const uint4 x, const uint y) {\n\treturn (x) - uint4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inUInt_inUInt_outUInt (const UInt &x, const UInt &y, OUT UInt &out)
	{
		FunctionNode	func( "Mul", "uint Mul (const uint x, const uint y) {\n\treturn (x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inUInt2_inUInt2_outUInt2 (const UInt2 &x, const UInt2 &y, OUT UInt2 &out)
	{
		FunctionNode	func( "Mul", "uint2 Mul (const uint2 x, const uint2 y) {\n\treturn (x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inUInt_inUInt2_outUInt2 (const UInt &x, const UInt2 &y, OUT UInt2 &out)
	{
		FunctionNode	func( "Mul", "uint2 Mul (const uint x, const uint2 y) {\n\treturn uint2(x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inUInt2_inUInt_outUInt2 (const UInt2 &x, const UInt &y, OUT UInt2 &out)
	{
		FunctionNode	func( "Mul", "uint2 Mul (const uint2 x, const uint y) {\n\treturn (x) * uint2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inUInt3_inUInt3_outUInt3 (const UInt3 &x, const UInt3 &y, OUT UInt3 &out)
	{
		FunctionNode	func( "Mul", "uint3 Mul (const uint3 x, const uint3 y) {\n\treturn (x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inUInt_inUInt3_outUInt3 (const UInt &x, const UInt3 &y, OUT UInt3 &out)
	{
		FunctionNode	func( "Mul", "uint3 Mul (const uint x, const uint3 y) {\n\treturn uint3(x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inUInt3_inUInt_outUInt3 (const UInt3 &x, const UInt &y, OUT UInt3 &out)
	{
		FunctionNode	func( "Mul", "uint3 Mul (const uint3 x, const uint y) {\n\treturn (x) * uint3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inUInt4_inUInt4_outUInt4 (const UInt4 &x, const UInt4 &y, OUT UInt4 &out)
	{
		FunctionNode	func( "Mul", "uint4 Mul (const uint4 x, const uint4 y) {\n\treturn (x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inUInt_inUInt4_outUInt4 (const UInt &x, const UInt4 &y, OUT UInt4 &out)
	{
		FunctionNode	func( "Mul", "uint4 Mul (const uint x, const uint4 y) {\n\treturn uint4(x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inUInt4_inUInt_outUInt4 (const UInt4 &x, const UInt &y, OUT UInt4 &out)
	{
		FunctionNode	func( "Mul", "uint4 Mul (const uint4 x, const uint y) {\n\treturn (x) * uint4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inUInt_inUInt_outUInt (const UInt &x, const UInt &y, OUT UInt &out)
	{
		FunctionNode	func( "Div", "uint Div (const uint x, const uint y) {\n\treturn (x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inUInt2_inUInt2_outUInt2 (const UInt2 &x, const UInt2 &y, OUT UInt2 &out)
	{
		FunctionNode	func( "Div", "uint2 Div (const uint2 x, const uint2 y) {\n\treturn (x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inUInt_inUInt2_outUInt2 (const UInt &x, const UInt2 &y, OUT UInt2 &out)
	{
		FunctionNode	func( "Div", "uint2 Div (const uint x, const uint2 y) {\n\treturn uint2(x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inUInt2_inUInt_outUInt2 (const UInt2 &x, const UInt &y, OUT UInt2 &out)
	{
		FunctionNode	func( "Div", "uint2 Div (const uint2 x, const uint y) {\n\treturn (x) / uint2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inUInt3_inUInt3_outUInt3 (const UInt3 &x, const UInt3 &y, OUT UInt3 &out)
	{
		FunctionNode	func( "Div", "uint3 Div (const uint3 x, const uint3 y) {\n\treturn (x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inUInt_inUInt3_outUInt3 (const UInt &x, const UInt3 &y, OUT UInt3 &out)
	{
		FunctionNode	func( "Div", "uint3 Div (const uint x, const uint3 y) {\n\treturn uint3(x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inUInt3_inUInt_outUInt3 (const UInt3 &x, const UInt &y, OUT UInt3 &out)
	{
		FunctionNode	func( "Div", "uint3 Div (const uint3 x, const uint y) {\n\treturn (x) / uint3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inUInt4_inUInt4_outUInt4 (const UInt4 &x, const UInt4 &y, OUT UInt4 &out)
	{
		FunctionNode	func( "Div", "uint4 Div (const uint4 x, const uint4 y) {\n\treturn (x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inUInt_inUInt4_outUInt4 (const UInt &x, const UInt4 &y, OUT UInt4 &out)
	{
		FunctionNode	func( "Div", "uint4 Div (const uint x, const uint4 y) {\n\treturn uint4(x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inUInt4_inUInt_outUInt4 (const UInt4 &x, const UInt &y, OUT UInt4 &out)
	{
		FunctionNode	func( "Div", "uint4 Div (const uint4 x, const uint y) {\n\treturn (x) / uint4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inUInt_inUInt_outUInt (const UInt &x, const UInt &y, OUT UInt &out)
	{
		FunctionNode	func( "Mod", "uint Mod (const uint x, const uint y) {\n\treturn (x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inUInt2_inUInt2_outUInt2 (const UInt2 &x, const UInt2 &y, OUT UInt2 &out)
	{
		FunctionNode	func( "Mod", "uint2 Mod (const uint2 x, const uint2 y) {\n\treturn (x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inUInt_inUInt2_outUInt2 (const UInt &x, const UInt2 &y, OUT UInt2 &out)
	{
		FunctionNode	func( "Mod", "uint2 Mod (const uint x, const uint2 y) {\n\treturn uint2(x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inUInt2_inUInt_outUInt2 (const UInt2 &x, const UInt &y, OUT UInt2 &out)
	{
		FunctionNode	func( "Mod", "uint2 Mod (const uint2 x, const uint y) {\n\treturn (x) % uint2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inUInt3_inUInt3_outUInt3 (const UInt3 &x, const UInt3 &y, OUT UInt3 &out)
	{
		FunctionNode	func( "Mod", "uint3 Mod (const uint3 x, const uint3 y) {\n\treturn (x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inUInt_inUInt3_outUInt3 (const UInt &x, const UInt3 &y, OUT UInt3 &out)
	{
		FunctionNode	func( "Mod", "uint3 Mod (const uint x, const uint3 y) {\n\treturn uint3(x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inUInt3_inUInt_outUInt3 (const UInt3 &x, const UInt &y, OUT UInt3 &out)
	{
		FunctionNode	func( "Mod", "uint3 Mod (const uint3 x, const uint y) {\n\treturn (x) % uint3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inUInt4_inUInt4_outUInt4 (const UInt4 &x, const UInt4 &y, OUT UInt4 &out)
	{
		FunctionNode	func( "Mod", "uint4 Mod (const uint4 x, const uint4 y) {\n\treturn (x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inUInt_inUInt4_outUInt4 (const UInt &x, const UInt4 &y, OUT UInt4 &out)
	{
		FunctionNode	func( "Mod", "uint4 Mod (const uint x, const uint4 y) {\n\treturn uint4(x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inUInt4_inUInt_outUInt4 (const UInt4 &x, const UInt &y, OUT UInt4 &out)
	{
		FunctionNode	func( "Mod", "uint4 Mod (const uint4 x, const uint y) {\n\treturn (x) % uint4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inUInt_inUInt_outUInt (const UInt &x, const UInt &y, OUT UInt &out)
	{
		FunctionNode	func( "And", "uint And (const uint x, const uint y) {\n\treturn (x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inUInt2_inUInt2_outUInt2 (const UInt2 &x, const UInt2 &y, OUT UInt2 &out)
	{
		FunctionNode	func( "And", "uint2 And (const uint2 x, const uint2 y) {\n\treturn (x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inUInt_inUInt2_outUInt2 (const UInt &x, const UInt2 &y, OUT UInt2 &out)
	{
		FunctionNode	func( "And", "uint2 And (const uint x, const uint2 y) {\n\treturn uint2(x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inUInt2_inUInt_outUInt2 (const UInt2 &x, const UInt &y, OUT UInt2 &out)
	{
		FunctionNode	func( "And", "uint2 And (const uint2 x, const uint y) {\n\treturn (x) & uint2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inUInt3_inUInt3_outUInt3 (const UInt3 &x, const UInt3 &y, OUT UInt3 &out)
	{
		FunctionNode	func( "And", "uint3 And (const uint3 x, const uint3 y) {\n\treturn (x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inUInt_inUInt3_outUInt3 (const UInt &x, const UInt3 &y, OUT UInt3 &out)
	{
		FunctionNode	func( "And", "uint3 And (const uint x, const uint3 y) {\n\treturn uint3(x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inUInt3_inUInt_outUInt3 (const UInt3 &x, const UInt &y, OUT UInt3 &out)
	{
		FunctionNode	func( "And", "uint3 And (const uint3 x, const uint y) {\n\treturn (x) & uint3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inUInt4_inUInt4_outUInt4 (const UInt4 &x, const UInt4 &y, OUT UInt4 &out)
	{
		FunctionNode	func( "And", "uint4 And (const uint4 x, const uint4 y) {\n\treturn (x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inUInt_inUInt4_outUInt4 (const UInt &x, const UInt4 &y, OUT UInt4 &out)
	{
		FunctionNode	func( "And", "uint4 And (const uint x, const uint4 y) {\n\treturn uint4(x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inUInt4_inUInt_outUInt4 (const UInt4 &x, const UInt &y, OUT UInt4 &out)
	{
		FunctionNode	func( "And", "uint4 And (const uint4 x, const uint y) {\n\treturn (x) & uint4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inUInt_inUInt_outUInt (const UInt &x, const UInt &y, OUT UInt &out)
	{
		FunctionNode	func( "Or", "uint Or (const uint x, const uint y) {\n\treturn (x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inUInt2_inUInt2_outUInt2 (const UInt2 &x, const UInt2 &y, OUT UInt2 &out)
	{
		FunctionNode	func( "Or", "uint2 Or (const uint2 x, const uint2 y) {\n\treturn (x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inUInt_inUInt2_outUInt2 (const UInt &x, const UInt2 &y, OUT UInt2 &out)
	{
		FunctionNode	func( "Or", "uint2 Or (const uint x, const uint2 y) {\n\treturn uint2(x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inUInt2_inUInt_outUInt2 (const UInt2 &x, const UInt &y, OUT UInt2 &out)
	{
		FunctionNode	func( "Or", "uint2 Or (const uint2 x, const uint y) {\n\treturn (x) | uint2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inUInt3_inUInt3_outUInt3 (const UInt3 &x, const UInt3 &y, OUT UInt3 &out)
	{
		FunctionNode	func( "Or", "uint3 Or (const uint3 x, const uint3 y) {\n\treturn (x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inUInt_inUInt3_outUInt3 (const UInt &x, const UInt3 &y, OUT UInt3 &out)
	{
		FunctionNode	func( "Or", "uint3 Or (const uint x, const uint3 y) {\n\treturn uint3(x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inUInt3_inUInt_outUInt3 (const UInt3 &x, const UInt &y, OUT UInt3 &out)
	{
		FunctionNode	func( "Or", "uint3 Or (const uint3 x, const uint y) {\n\treturn (x) | uint3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inUInt4_inUInt4_outUInt4 (const UInt4 &x, const UInt4 &y, OUT UInt4 &out)
	{
		FunctionNode	func( "Or", "uint4 Or (const uint4 x, const uint4 y) {\n\treturn (x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inUInt_inUInt4_outUInt4 (const UInt &x, const UInt4 &y, OUT UInt4 &out)
	{
		FunctionNode	func( "Or", "uint4 Or (const uint x, const uint4 y) {\n\treturn uint4(x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inUInt4_inUInt_outUInt4 (const UInt4 &x, const UInt &y, OUT UInt4 &out)
	{
		FunctionNode	func( "Or", "uint4 Or (const uint4 x, const uint y) {\n\treturn (x) | uint4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inUInt_inUInt_outUInt (const UInt &x, const UInt &y, OUT UInt &out)
	{
		FunctionNode	func( "Xor", "uint Xor (const uint x, const uint y) {\n\treturn (x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inUInt2_inUInt2_outUInt2 (const UInt2 &x, const UInt2 &y, OUT UInt2 &out)
	{
		FunctionNode	func( "Xor", "uint2 Xor (const uint2 x, const uint2 y) {\n\treturn (x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inUInt_inUInt2_outUInt2 (const UInt &x, const UInt2 &y, OUT UInt2 &out)
	{
		FunctionNode	func( "Xor", "uint2 Xor (const uint x, const uint2 y) {\n\treturn uint2(x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inUInt2_inUInt_outUInt2 (const UInt2 &x, const UInt &y, OUT UInt2 &out)
	{
		FunctionNode	func( "Xor", "uint2 Xor (const uint2 x, const uint y) {\n\treturn (x) ^ uint2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inUInt3_inUInt3_outUInt3 (const UInt3 &x, const UInt3 &y, OUT UInt3 &out)
	{
		FunctionNode	func( "Xor", "uint3 Xor (const uint3 x, const uint3 y) {\n\treturn (x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inUInt_inUInt3_outUInt3 (const UInt &x, const UInt3 &y, OUT UInt3 &out)
	{
		FunctionNode	func( "Xor", "uint3 Xor (const uint x, const uint3 y) {\n\treturn uint3(x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inUInt3_inUInt_outUInt3 (const UInt3 &x, const UInt &y, OUT UInt3 &out)
	{
		FunctionNode	func( "Xor", "uint3 Xor (const uint3 x, const uint y) {\n\treturn (x) ^ uint3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inUInt4_inUInt4_outUInt4 (const UInt4 &x, const UInt4 &y, OUT UInt4 &out)
	{
		FunctionNode	func( "Xor", "uint4 Xor (const uint4 x, const uint4 y) {\n\treturn (x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inUInt_inUInt4_outUInt4 (const UInt &x, const UInt4 &y, OUT UInt4 &out)
	{
		FunctionNode	func( "Xor", "uint4 Xor (const uint x, const uint4 y) {\n\treturn uint4(x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inUInt4_inUInt_outUInt4 (const UInt4 &x, const UInt &y, OUT UInt4 &out)
	{
		FunctionNode	func( "Xor", "uint4 Xor (const uint4 x, const uint y) {\n\treturn (x) ^ uint4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inUInt_inUInt_outUInt (const UInt &x, const UInt &y, OUT UInt &out)
	{
		FunctionNode	func( "BitLShift", "uint BitLShift (const uint x, const uint y) {\n\treturn (x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inUInt2_inUInt2_outUInt2 (const UInt2 &x, const UInt2 &y, OUT UInt2 &out)
	{
		FunctionNode	func( "BitLShift", "uint2 BitLShift (const uint2 x, const uint2 y) {\n\treturn (x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inUInt_inUInt2_outUInt2 (const UInt &x, const UInt2 &y, OUT UInt2 &out)
	{
		FunctionNode	func( "BitLShift", "uint2 BitLShift (const uint x, const uint2 y) {\n\treturn uint2(x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inUInt2_inUInt_outUInt2 (const UInt2 &x, const UInt &y, OUT UInt2 &out)
	{
		FunctionNode	func( "BitLShift", "uint2 BitLShift (const uint2 x, const uint y) {\n\treturn (x) << uint2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inUInt3_inUInt3_outUInt3 (const UInt3 &x, const UInt3 &y, OUT UInt3 &out)
	{
		FunctionNode	func( "BitLShift", "uint3 BitLShift (const uint3 x, const uint3 y) {\n\treturn (x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inUInt_inUInt3_outUInt3 (const UInt &x, const UInt3 &y, OUT UInt3 &out)
	{
		FunctionNode	func( "BitLShift", "uint3 BitLShift (const uint x, const uint3 y) {\n\treturn uint3(x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inUInt3_inUInt_outUInt3 (const UInt3 &x, const UInt &y, OUT UInt3 &out)
	{
		FunctionNode	func( "BitLShift", "uint3 BitLShift (const uint3 x, const uint y) {\n\treturn (x) << uint3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inUInt4_inUInt4_outUInt4 (const UInt4 &x, const UInt4 &y, OUT UInt4 &out)
	{
		FunctionNode	func( "BitLShift", "uint4 BitLShift (const uint4 x, const uint4 y) {\n\treturn (x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inUInt_inUInt4_outUInt4 (const UInt &x, const UInt4 &y, OUT UInt4 &out)
	{
		FunctionNode	func( "BitLShift", "uint4 BitLShift (const uint x, const uint4 y) {\n\treturn uint4(x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inUInt4_inUInt_outUInt4 (const UInt4 &x, const UInt &y, OUT UInt4 &out)
	{
		FunctionNode	func( "BitLShift", "uint4 BitLShift (const uint4 x, const uint y) {\n\treturn (x) << uint4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inUInt_inUInt_outUInt (const UInt &x, const UInt &y, OUT UInt &out)
	{
		FunctionNode	func( "BitRShift", "uint BitRShift (const uint x, const uint y) {\n\treturn (x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inUInt2_inUInt2_outUInt2 (const UInt2 &x, const UInt2 &y, OUT UInt2 &out)
	{
		FunctionNode	func( "BitRShift", "uint2 BitRShift (const uint2 x, const uint2 y) {\n\treturn (x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inUInt_inUInt2_outUInt2 (const UInt &x, const UInt2 &y, OUT UInt2 &out)
	{
		FunctionNode	func( "BitRShift", "uint2 BitRShift (const uint x, const uint2 y) {\n\treturn uint2(x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inUInt2_inUInt_outUInt2 (const UInt2 &x, const UInt &y, OUT UInt2 &out)
	{
		FunctionNode	func( "BitRShift", "uint2 BitRShift (const uint2 x, const uint y) {\n\treturn (x) >> uint2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inUInt3_inUInt3_outUInt3 (const UInt3 &x, const UInt3 &y, OUT UInt3 &out)
	{
		FunctionNode	func( "BitRShift", "uint3 BitRShift (const uint3 x, const uint3 y) {\n\treturn (x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inUInt_inUInt3_outUInt3 (const UInt &x, const UInt3 &y, OUT UInt3 &out)
	{
		FunctionNode	func( "BitRShift", "uint3 BitRShift (const uint x, const uint3 y) {\n\treturn uint3(x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inUInt3_inUInt_outUInt3 (const UInt3 &x, const UInt &y, OUT UInt3 &out)
	{
		FunctionNode	func( "BitRShift", "uint3 BitRShift (const uint3 x, const uint y) {\n\treturn (x) >> uint3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inUInt4_inUInt4_outUInt4 (const UInt4 &x, const UInt4 &y, OUT UInt4 &out)
	{
		FunctionNode	func( "BitRShift", "uint4 BitRShift (const uint4 x, const uint4 y) {\n\treturn (x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inUInt_inUInt4_outUInt4 (const UInt &x, const UInt4 &y, OUT UInt4 &out)
	{
		FunctionNode	func( "BitRShift", "uint4 BitRShift (const uint x, const uint4 y) {\n\treturn uint4(x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inUInt4_inUInt_outUInt4 (const UInt4 &x, const UInt &y, OUT UInt4 &out)
	{
		FunctionNode	func( "BitRShift", "uint4 BitRShift (const uint4 x, const uint y) {\n\treturn (x) >> uint4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inUInt_inUInt_outBool (const UInt &x, const UInt &y, OUT Bool &out)
	{
		FunctionNode	func( "Equal", "bool Equal (const uint x, const uint y) {\n\treturn (x) == (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inUInt2_inUInt2_outBool2 (const UInt2 &x, const UInt2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "Equal", "bool2 Equal (const uint2 x, const uint2 y) {\n\treturn equal( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inUInt_inUInt2_outBool2 (const UInt &x, const UInt2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "Equal", "bool2 Equal (const uint x, const uint2 y) {\n\treturn equal( uint2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inUInt2_inUInt_outBool2 (const UInt2 &x, const UInt &y, OUT Bool2 &out)
	{
		FunctionNode	func( "Equal", "bool2 Equal (const uint2 x, const uint y) {\n\treturn equal( (x), uint2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inUInt3_inUInt3_outBool3 (const UInt3 &x, const UInt3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "Equal", "bool3 Equal (const uint3 x, const uint3 y) {\n\treturn equal( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inUInt_inUInt3_outBool3 (const UInt &x, const UInt3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "Equal", "bool3 Equal (const uint x, const uint3 y) {\n\treturn equal( uint3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inUInt3_inUInt_outBool3 (const UInt3 &x, const UInt &y, OUT Bool3 &out)
	{
		FunctionNode	func( "Equal", "bool3 Equal (const uint3 x, const uint y) {\n\treturn equal( (x), uint3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inUInt4_inUInt4_outBool4 (const UInt4 &x, const UInt4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "Equal", "bool4 Equal (const uint4 x, const uint4 y) {\n\treturn equal( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inUInt_inUInt4_outBool4 (const UInt &x, const UInt4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "Equal", "bool4 Equal (const uint x, const uint4 y) {\n\treturn equal( uint4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inUInt4_inUInt_outBool4 (const UInt4 &x, const UInt &y, OUT Bool4 &out)
	{
		FunctionNode	func( "Equal", "bool4 Equal (const uint4 x, const uint y) {\n\treturn equal( (x), uint4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inUInt_inUInt_outBool (const UInt &x, const UInt &y, OUT Bool &out)
	{
		FunctionNode	func( "NotEqual", "bool NotEqual (const uint x, const uint y) {\n\treturn (x) != (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inUInt2_inUInt2_outBool2 (const UInt2 &x, const UInt2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "NotEqual", "bool2 NotEqual (const uint2 x, const uint2 y) {\n\treturn notEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inUInt_inUInt2_outBool2 (const UInt &x, const UInt2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "NotEqual", "bool2 NotEqual (const uint x, const uint2 y) {\n\treturn notEqual( uint2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inUInt2_inUInt_outBool2 (const UInt2 &x, const UInt &y, OUT Bool2 &out)
	{
		FunctionNode	func( "NotEqual", "bool2 NotEqual (const uint2 x, const uint y) {\n\treturn notEqual( (x), uint2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inUInt3_inUInt3_outBool3 (const UInt3 &x, const UInt3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "NotEqual", "bool3 NotEqual (const uint3 x, const uint3 y) {\n\treturn notEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inUInt_inUInt3_outBool3 (const UInt &x, const UInt3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "NotEqual", "bool3 NotEqual (const uint x, const uint3 y) {\n\treturn notEqual( uint3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inUInt3_inUInt_outBool3 (const UInt3 &x, const UInt &y, OUT Bool3 &out)
	{
		FunctionNode	func( "NotEqual", "bool3 NotEqual (const uint3 x, const uint y) {\n\treturn notEqual( (x), uint3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inUInt4_inUInt4_outBool4 (const UInt4 &x, const UInt4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "NotEqual", "bool4 NotEqual (const uint4 x, const uint4 y) {\n\treturn notEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inUInt_inUInt4_outBool4 (const UInt &x, const UInt4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "NotEqual", "bool4 NotEqual (const uint x, const uint4 y) {\n\treturn notEqual( uint4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inUInt4_inUInt_outBool4 (const UInt4 &x, const UInt &y, OUT Bool4 &out)
	{
		FunctionNode	func( "NotEqual", "bool4 NotEqual (const uint4 x, const uint y) {\n\treturn notEqual( (x), uint4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inUInt_inUInt_outBool (const UInt &x, const UInt &y, OUT Bool &out)
	{
		FunctionNode	func( "GreaterThan", "bool GreaterThan (const uint x, const uint y) {\n\treturn (x) > (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inUInt2_inUInt2_outBool2 (const UInt2 &x, const UInt2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThan", "bool2 GreaterThan (const uint2 x, const uint2 y) {\n\treturn greaterThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inUInt_inUInt2_outBool2 (const UInt &x, const UInt2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThan", "bool2 GreaterThan (const uint x, const uint2 y) {\n\treturn greaterThan( uint2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inUInt2_inUInt_outBool2 (const UInt2 &x, const UInt &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThan", "bool2 GreaterThan (const uint2 x, const uint y) {\n\treturn greaterThan( (x), uint2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inUInt3_inUInt3_outBool3 (const UInt3 &x, const UInt3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThan", "bool3 GreaterThan (const uint3 x, const uint3 y) {\n\treturn greaterThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inUInt_inUInt3_outBool3 (const UInt &x, const UInt3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThan", "bool3 GreaterThan (const uint x, const uint3 y) {\n\treturn greaterThan( uint3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inUInt3_inUInt_outBool3 (const UInt3 &x, const UInt &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThan", "bool3 GreaterThan (const uint3 x, const uint y) {\n\treturn greaterThan( (x), uint3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inUInt4_inUInt4_outBool4 (const UInt4 &x, const UInt4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThan", "bool4 GreaterThan (const uint4 x, const uint4 y) {\n\treturn greaterThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inUInt_inUInt4_outBool4 (const UInt &x, const UInt4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThan", "bool4 GreaterThan (const uint x, const uint4 y) {\n\treturn greaterThan( uint4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inUInt4_inUInt_outBool4 (const UInt4 &x, const UInt &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThan", "bool4 GreaterThan (const uint4 x, const uint y) {\n\treturn greaterThan( (x), uint4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inUInt_inUInt_outBool (const UInt &x, const UInt &y, OUT Bool &out)
	{
		FunctionNode	func( "LessThan", "bool LessThan (const uint x, const uint y) {\n\treturn (x) < (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inUInt2_inUInt2_outBool2 (const UInt2 &x, const UInt2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThan", "bool2 LessThan (const uint2 x, const uint2 y) {\n\treturn lessThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inUInt_inUInt2_outBool2 (const UInt &x, const UInt2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThan", "bool2 LessThan (const uint x, const uint2 y) {\n\treturn lessThan( uint2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inUInt2_inUInt_outBool2 (const UInt2 &x, const UInt &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThan", "bool2 LessThan (const uint2 x, const uint y) {\n\treturn lessThan( (x), uint2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inUInt3_inUInt3_outBool3 (const UInt3 &x, const UInt3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThan", "bool3 LessThan (const uint3 x, const uint3 y) {\n\treturn lessThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inUInt_inUInt3_outBool3 (const UInt &x, const UInt3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThan", "bool3 LessThan (const uint x, const uint3 y) {\n\treturn lessThan( uint3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inUInt3_inUInt_outBool3 (const UInt3 &x, const UInt &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThan", "bool3 LessThan (const uint3 x, const uint y) {\n\treturn lessThan( (x), uint3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inUInt4_inUInt4_outBool4 (const UInt4 &x, const UInt4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThan", "bool4 LessThan (const uint4 x, const uint4 y) {\n\treturn lessThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inUInt_inUInt4_outBool4 (const UInt &x, const UInt4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThan", "bool4 LessThan (const uint x, const uint4 y) {\n\treturn lessThan( uint4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inUInt4_inUInt_outBool4 (const UInt4 &x, const UInt &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThan", "bool4 LessThan (const uint4 x, const uint y) {\n\treturn lessThan( (x), uint4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inUInt_inUInt_outBool (const UInt &x, const UInt &y, OUT Bool &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool GreaterThanEqual (const uint x, const uint y) {\n\treturn (x) >= (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inUInt2_inUInt2_outBool2 (const UInt2 &x, const UInt2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool2 GreaterThanEqual (const uint2 x, const uint2 y) {\n\treturn greaterThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inUInt_inUInt2_outBool2 (const UInt &x, const UInt2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool2 GreaterThanEqual (const uint x, const uint2 y) {\n\treturn greaterThanEqual( uint2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inUInt2_inUInt_outBool2 (const UInt2 &x, const UInt &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool2 GreaterThanEqual (const uint2 x, const uint y) {\n\treturn greaterThanEqual( (x), uint2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inUInt3_inUInt3_outBool3 (const UInt3 &x, const UInt3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool3 GreaterThanEqual (const uint3 x, const uint3 y) {\n\treturn greaterThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inUInt_inUInt3_outBool3 (const UInt &x, const UInt3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool3 GreaterThanEqual (const uint x, const uint3 y) {\n\treturn greaterThanEqual( uint3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inUInt3_inUInt_outBool3 (const UInt3 &x, const UInt &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool3 GreaterThanEqual (const uint3 x, const uint y) {\n\treturn greaterThanEqual( (x), uint3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inUInt4_inUInt4_outBool4 (const UInt4 &x, const UInt4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool4 GreaterThanEqual (const uint4 x, const uint4 y) {\n\treturn greaterThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inUInt_inUInt4_outBool4 (const UInt &x, const UInt4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool4 GreaterThanEqual (const uint x, const uint4 y) {\n\treturn greaterThanEqual( uint4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inUInt4_inUInt_outBool4 (const UInt4 &x, const UInt &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool4 GreaterThanEqual (const uint4 x, const uint y) {\n\treturn greaterThanEqual( (x), uint4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inUInt_inUInt_outBool (const UInt &x, const UInt &y, OUT Bool &out)
	{
		FunctionNode	func( "LessThanEqual", "bool LessThanEqual (const uint x, const uint y) {\n\treturn (x) <= (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inUInt2_inUInt2_outBool2 (const UInt2 &x, const UInt2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool2 LessThanEqual (const uint2 x, const uint2 y) {\n\treturn lessThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inUInt_inUInt2_outBool2 (const UInt &x, const UInt2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool2 LessThanEqual (const uint x, const uint2 y) {\n\treturn lessThanEqual( uint2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inUInt2_inUInt_outBool2 (const UInt2 &x, const UInt &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool2 LessThanEqual (const uint2 x, const uint y) {\n\treturn lessThanEqual( (x), uint2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inUInt3_inUInt3_outBool3 (const UInt3 &x, const UInt3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool3 LessThanEqual (const uint3 x, const uint3 y) {\n\treturn lessThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inUInt_inUInt3_outBool3 (const UInt &x, const UInt3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool3 LessThanEqual (const uint x, const uint3 y) {\n\treturn lessThanEqual( uint3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inUInt3_inUInt_outBool3 (const UInt3 &x, const UInt &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool3 LessThanEqual (const uint3 x, const uint y) {\n\treturn lessThanEqual( (x), uint3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inUInt4_inUInt4_outBool4 (const UInt4 &x, const UInt4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool4 LessThanEqual (const uint4 x, const uint4 y) {\n\treturn lessThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inUInt_inUInt4_outBool4 (const UInt &x, const UInt4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool4 LessThanEqual (const uint x, const uint4 y) {\n\treturn lessThanEqual( uint4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inUInt4_inUInt_outBool4 (const UInt4 &x, const UInt &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool4 LessThanEqual (const uint4 x, const uint y) {\n\treturn lessThanEqual( (x), uint4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitInverse_inLong_outLong (const Long &x, OUT Long &out)
	{
		FunctionNode	func( "BitInverse", "long BitInverse (const long x) {\n\treturn ~(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool BitInverse_inLong2_outLong2 (const Long2 &x, OUT Long2 &out)
	{
		FunctionNode	func( "BitInverse", "long2 BitInverse (const long2 x) {\n\treturn ~(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool BitInverse_inLong3_outLong3 (const Long3 &x, OUT Long3 &out)
	{
		FunctionNode	func( "BitInverse", "long3 BitInverse (const long3 x) {\n\treturn ~(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool BitInverse_inLong4_outLong4 (const Long4 &x, OUT Long4 &out)
	{
		FunctionNode	func( "BitInverse", "long4 BitInverse (const long4 x) {\n\treturn ~(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Plus_inLong_outLong (const Long &x, OUT Long &out)
	{
		FunctionNode	func( "Plus", "long Plus (const long x) {\n\treturn +(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Plus_inLong2_outLong2 (const Long2 &x, OUT Long2 &out)
	{
		FunctionNode	func( "Plus", "long2 Plus (const long2 x) {\n\treturn +(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Plus_inLong3_outLong3 (const Long3 &x, OUT Long3 &out)
	{
		FunctionNode	func( "Plus", "long3 Plus (const long3 x) {\n\treturn +(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Plus_inLong4_outLong4 (const Long4 &x, OUT Long4 &out)
	{
		FunctionNode	func( "Plus", "long4 Plus (const long4 x) {\n\treturn +(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Minus_inLong_outLong (const Long &x, OUT Long &out)
	{
		FunctionNode	func( "Minus", "long Minus (const long x) {\n\treturn -(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Minus_inLong2_outLong2 (const Long2 &x, OUT Long2 &out)
	{
		FunctionNode	func( "Minus", "long2 Minus (const long2 x) {\n\treturn -(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Minus_inLong3_outLong3 (const Long3 &x, OUT Long3 &out)
	{
		FunctionNode	func( "Minus", "long3 Minus (const long3 x) {\n\treturn -(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Minus_inLong4_outLong4 (const Long4 &x, OUT Long4 &out)
	{
		FunctionNode	func( "Minus", "long4 Minus (const long4 x) {\n\treturn -(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inLong_inLong_outLong (const Long &x, const Long &y, OUT Long &out)
	{
		FunctionNode	func( "Add", "long Add (const long x, const long y) {\n\treturn (x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inLong2_inLong2_outLong2 (const Long2 &x, const Long2 &y, OUT Long2 &out)
	{
		FunctionNode	func( "Add", "long2 Add (const long2 x, const long2 y) {\n\treturn (x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inLong_inLong2_outLong2 (const Long &x, const Long2 &y, OUT Long2 &out)
	{
		FunctionNode	func( "Add", "long2 Add (const long x, const long2 y) {\n\treturn long2(x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inLong2_inLong_outLong2 (const Long2 &x, const Long &y, OUT Long2 &out)
	{
		FunctionNode	func( "Add", "long2 Add (const long2 x, const long y) {\n\treturn (x) + long2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inLong3_inLong3_outLong3 (const Long3 &x, const Long3 &y, OUT Long3 &out)
	{
		FunctionNode	func( "Add", "long3 Add (const long3 x, const long3 y) {\n\treturn (x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inLong_inLong3_outLong3 (const Long &x, const Long3 &y, OUT Long3 &out)
	{
		FunctionNode	func( "Add", "long3 Add (const long x, const long3 y) {\n\treturn long3(x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inLong3_inLong_outLong3 (const Long3 &x, const Long &y, OUT Long3 &out)
	{
		FunctionNode	func( "Add", "long3 Add (const long3 x, const long y) {\n\treturn (x) + long3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inLong4_inLong4_outLong4 (const Long4 &x, const Long4 &y, OUT Long4 &out)
	{
		FunctionNode	func( "Add", "long4 Add (const long4 x, const long4 y) {\n\treturn (x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inLong_inLong4_outLong4 (const Long &x, const Long4 &y, OUT Long4 &out)
	{
		FunctionNode	func( "Add", "long4 Add (const long x, const long4 y) {\n\treturn long4(x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inLong4_inLong_outLong4 (const Long4 &x, const Long &y, OUT Long4 &out)
	{
		FunctionNode	func( "Add", "long4 Add (const long4 x, const long y) {\n\treturn (x) + long4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inLong_inLong_outLong (const Long &x, const Long &y, OUT Long &out)
	{
		FunctionNode	func( "Sub", "long Sub (const long x, const long y) {\n\treturn (x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inLong2_inLong2_outLong2 (const Long2 &x, const Long2 &y, OUT Long2 &out)
	{
		FunctionNode	func( "Sub", "long2 Sub (const long2 x, const long2 y) {\n\treturn (x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inLong_inLong2_outLong2 (const Long &x, const Long2 &y, OUT Long2 &out)
	{
		FunctionNode	func( "Sub", "long2 Sub (const long x, const long2 y) {\n\treturn long2(x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inLong2_inLong_outLong2 (const Long2 &x, const Long &y, OUT Long2 &out)
	{
		FunctionNode	func( "Sub", "long2 Sub (const long2 x, const long y) {\n\treturn (x) - long2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inLong3_inLong3_outLong3 (const Long3 &x, const Long3 &y, OUT Long3 &out)
	{
		FunctionNode	func( "Sub", "long3 Sub (const long3 x, const long3 y) {\n\treturn (x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inLong_inLong3_outLong3 (const Long &x, const Long3 &y, OUT Long3 &out)
	{
		FunctionNode	func( "Sub", "long3 Sub (const long x, const long3 y) {\n\treturn long3(x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inLong3_inLong_outLong3 (const Long3 &x, const Long &y, OUT Long3 &out)
	{
		FunctionNode	func( "Sub", "long3 Sub (const long3 x, const long y) {\n\treturn (x) - long3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inLong4_inLong4_outLong4 (const Long4 &x, const Long4 &y, OUT Long4 &out)
	{
		FunctionNode	func( "Sub", "long4 Sub (const long4 x, const long4 y) {\n\treturn (x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inLong_inLong4_outLong4 (const Long &x, const Long4 &y, OUT Long4 &out)
	{
		FunctionNode	func( "Sub", "long4 Sub (const long x, const long4 y) {\n\treturn long4(x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inLong4_inLong_outLong4 (const Long4 &x, const Long &y, OUT Long4 &out)
	{
		FunctionNode	func( "Sub", "long4 Sub (const long4 x, const long y) {\n\treturn (x) - long4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inLong_inLong_outLong (const Long &x, const Long &y, OUT Long &out)
	{
		FunctionNode	func( "Mul", "long Mul (const long x, const long y) {\n\treturn (x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inLong2_inLong2_outLong2 (const Long2 &x, const Long2 &y, OUT Long2 &out)
	{
		FunctionNode	func( "Mul", "long2 Mul (const long2 x, const long2 y) {\n\treturn (x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inLong_inLong2_outLong2 (const Long &x, const Long2 &y, OUT Long2 &out)
	{
		FunctionNode	func( "Mul", "long2 Mul (const long x, const long2 y) {\n\treturn long2(x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inLong2_inLong_outLong2 (const Long2 &x, const Long &y, OUT Long2 &out)
	{
		FunctionNode	func( "Mul", "long2 Mul (const long2 x, const long y) {\n\treturn (x) * long2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inLong3_inLong3_outLong3 (const Long3 &x, const Long3 &y, OUT Long3 &out)
	{
		FunctionNode	func( "Mul", "long3 Mul (const long3 x, const long3 y) {\n\treturn (x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inLong_inLong3_outLong3 (const Long &x, const Long3 &y, OUT Long3 &out)
	{
		FunctionNode	func( "Mul", "long3 Mul (const long x, const long3 y) {\n\treturn long3(x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inLong3_inLong_outLong3 (const Long3 &x, const Long &y, OUT Long3 &out)
	{
		FunctionNode	func( "Mul", "long3 Mul (const long3 x, const long y) {\n\treturn (x) * long3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inLong4_inLong4_outLong4 (const Long4 &x, const Long4 &y, OUT Long4 &out)
	{
		FunctionNode	func( "Mul", "long4 Mul (const long4 x, const long4 y) {\n\treturn (x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inLong_inLong4_outLong4 (const Long &x, const Long4 &y, OUT Long4 &out)
	{
		FunctionNode	func( "Mul", "long4 Mul (const long x, const long4 y) {\n\treturn long4(x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inLong4_inLong_outLong4 (const Long4 &x, const Long &y, OUT Long4 &out)
	{
		FunctionNode	func( "Mul", "long4 Mul (const long4 x, const long y) {\n\treturn (x) * long4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inLong_inLong_outLong (const Long &x, const Long &y, OUT Long &out)
	{
		FunctionNode	func( "Div", "long Div (const long x, const long y) {\n\treturn (x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inLong2_inLong2_outLong2 (const Long2 &x, const Long2 &y, OUT Long2 &out)
	{
		FunctionNode	func( "Div", "long2 Div (const long2 x, const long2 y) {\n\treturn (x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inLong_inLong2_outLong2 (const Long &x, const Long2 &y, OUT Long2 &out)
	{
		FunctionNode	func( "Div", "long2 Div (const long x, const long2 y) {\n\treturn long2(x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inLong2_inLong_outLong2 (const Long2 &x, const Long &y, OUT Long2 &out)
	{
		FunctionNode	func( "Div", "long2 Div (const long2 x, const long y) {\n\treturn (x) / long2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inLong3_inLong3_outLong3 (const Long3 &x, const Long3 &y, OUT Long3 &out)
	{
		FunctionNode	func( "Div", "long3 Div (const long3 x, const long3 y) {\n\treturn (x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inLong_inLong3_outLong3 (const Long &x, const Long3 &y, OUT Long3 &out)
	{
		FunctionNode	func( "Div", "long3 Div (const long x, const long3 y) {\n\treturn long3(x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inLong3_inLong_outLong3 (const Long3 &x, const Long &y, OUT Long3 &out)
	{
		FunctionNode	func( "Div", "long3 Div (const long3 x, const long y) {\n\treturn (x) / long3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inLong4_inLong4_outLong4 (const Long4 &x, const Long4 &y, OUT Long4 &out)
	{
		FunctionNode	func( "Div", "long4 Div (const long4 x, const long4 y) {\n\treturn (x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inLong_inLong4_outLong4 (const Long &x, const Long4 &y, OUT Long4 &out)
	{
		FunctionNode	func( "Div", "long4 Div (const long x, const long4 y) {\n\treturn long4(x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inLong4_inLong_outLong4 (const Long4 &x, const Long &y, OUT Long4 &out)
	{
		FunctionNode	func( "Div", "long4 Div (const long4 x, const long y) {\n\treturn (x) / long4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inLong_inLong_outLong (const Long &x, const Long &y, OUT Long &out)
	{
		FunctionNode	func( "Mod", "long Mod (const long x, const long y) {\n\treturn (x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inLong2_inLong2_outLong2 (const Long2 &x, const Long2 &y, OUT Long2 &out)
	{
		FunctionNode	func( "Mod", "long2 Mod (const long2 x, const long2 y) {\n\treturn (x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inLong_inLong2_outLong2 (const Long &x, const Long2 &y, OUT Long2 &out)
	{
		FunctionNode	func( "Mod", "long2 Mod (const long x, const long2 y) {\n\treturn long2(x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inLong2_inLong_outLong2 (const Long2 &x, const Long &y, OUT Long2 &out)
	{
		FunctionNode	func( "Mod", "long2 Mod (const long2 x, const long y) {\n\treturn (x) % long2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inLong3_inLong3_outLong3 (const Long3 &x, const Long3 &y, OUT Long3 &out)
	{
		FunctionNode	func( "Mod", "long3 Mod (const long3 x, const long3 y) {\n\treturn (x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inLong_inLong3_outLong3 (const Long &x, const Long3 &y, OUT Long3 &out)
	{
		FunctionNode	func( "Mod", "long3 Mod (const long x, const long3 y) {\n\treturn long3(x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inLong3_inLong_outLong3 (const Long3 &x, const Long &y, OUT Long3 &out)
	{
		FunctionNode	func( "Mod", "long3 Mod (const long3 x, const long y) {\n\treturn (x) % long3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inLong4_inLong4_outLong4 (const Long4 &x, const Long4 &y, OUT Long4 &out)
	{
		FunctionNode	func( "Mod", "long4 Mod (const long4 x, const long4 y) {\n\treturn (x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inLong_inLong4_outLong4 (const Long &x, const Long4 &y, OUT Long4 &out)
	{
		FunctionNode	func( "Mod", "long4 Mod (const long x, const long4 y) {\n\treturn long4(x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inLong4_inLong_outLong4 (const Long4 &x, const Long &y, OUT Long4 &out)
	{
		FunctionNode	func( "Mod", "long4 Mod (const long4 x, const long y) {\n\treturn (x) % long4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inLong_inLong_outLong (const Long &x, const Long &y, OUT Long &out)
	{
		FunctionNode	func( "And", "long And (const long x, const long y) {\n\treturn (x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inLong2_inLong2_outLong2 (const Long2 &x, const Long2 &y, OUT Long2 &out)
	{
		FunctionNode	func( "And", "long2 And (const long2 x, const long2 y) {\n\treturn (x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inLong_inLong2_outLong2 (const Long &x, const Long2 &y, OUT Long2 &out)
	{
		FunctionNode	func( "And", "long2 And (const long x, const long2 y) {\n\treturn long2(x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inLong2_inLong_outLong2 (const Long2 &x, const Long &y, OUT Long2 &out)
	{
		FunctionNode	func( "And", "long2 And (const long2 x, const long y) {\n\treturn (x) & long2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inLong3_inLong3_outLong3 (const Long3 &x, const Long3 &y, OUT Long3 &out)
	{
		FunctionNode	func( "And", "long3 And (const long3 x, const long3 y) {\n\treturn (x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inLong_inLong3_outLong3 (const Long &x, const Long3 &y, OUT Long3 &out)
	{
		FunctionNode	func( "And", "long3 And (const long x, const long3 y) {\n\treturn long3(x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inLong3_inLong_outLong3 (const Long3 &x, const Long &y, OUT Long3 &out)
	{
		FunctionNode	func( "And", "long3 And (const long3 x, const long y) {\n\treturn (x) & long3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inLong4_inLong4_outLong4 (const Long4 &x, const Long4 &y, OUT Long4 &out)
	{
		FunctionNode	func( "And", "long4 And (const long4 x, const long4 y) {\n\treturn (x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inLong_inLong4_outLong4 (const Long &x, const Long4 &y, OUT Long4 &out)
	{
		FunctionNode	func( "And", "long4 And (const long x, const long4 y) {\n\treturn long4(x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inLong4_inLong_outLong4 (const Long4 &x, const Long &y, OUT Long4 &out)
	{
		FunctionNode	func( "And", "long4 And (const long4 x, const long y) {\n\treturn (x) & long4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inLong_inLong_outLong (const Long &x, const Long &y, OUT Long &out)
	{
		FunctionNode	func( "Or", "long Or (const long x, const long y) {\n\treturn (x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inLong2_inLong2_outLong2 (const Long2 &x, const Long2 &y, OUT Long2 &out)
	{
		FunctionNode	func( "Or", "long2 Or (const long2 x, const long2 y) {\n\treturn (x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inLong_inLong2_outLong2 (const Long &x, const Long2 &y, OUT Long2 &out)
	{
		FunctionNode	func( "Or", "long2 Or (const long x, const long2 y) {\n\treturn long2(x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inLong2_inLong_outLong2 (const Long2 &x, const Long &y, OUT Long2 &out)
	{
		FunctionNode	func( "Or", "long2 Or (const long2 x, const long y) {\n\treturn (x) | long2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inLong3_inLong3_outLong3 (const Long3 &x, const Long3 &y, OUT Long3 &out)
	{
		FunctionNode	func( "Or", "long3 Or (const long3 x, const long3 y) {\n\treturn (x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inLong_inLong3_outLong3 (const Long &x, const Long3 &y, OUT Long3 &out)
	{
		FunctionNode	func( "Or", "long3 Or (const long x, const long3 y) {\n\treturn long3(x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inLong3_inLong_outLong3 (const Long3 &x, const Long &y, OUT Long3 &out)
	{
		FunctionNode	func( "Or", "long3 Or (const long3 x, const long y) {\n\treturn (x) | long3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inLong4_inLong4_outLong4 (const Long4 &x, const Long4 &y, OUT Long4 &out)
	{
		FunctionNode	func( "Or", "long4 Or (const long4 x, const long4 y) {\n\treturn (x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inLong_inLong4_outLong4 (const Long &x, const Long4 &y, OUT Long4 &out)
	{
		FunctionNode	func( "Or", "long4 Or (const long x, const long4 y) {\n\treturn long4(x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inLong4_inLong_outLong4 (const Long4 &x, const Long &y, OUT Long4 &out)
	{
		FunctionNode	func( "Or", "long4 Or (const long4 x, const long y) {\n\treturn (x) | long4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inLong_inLong_outLong (const Long &x, const Long &y, OUT Long &out)
	{
		FunctionNode	func( "Xor", "long Xor (const long x, const long y) {\n\treturn (x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inLong2_inLong2_outLong2 (const Long2 &x, const Long2 &y, OUT Long2 &out)
	{
		FunctionNode	func( "Xor", "long2 Xor (const long2 x, const long2 y) {\n\treturn (x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inLong_inLong2_outLong2 (const Long &x, const Long2 &y, OUT Long2 &out)
	{
		FunctionNode	func( "Xor", "long2 Xor (const long x, const long2 y) {\n\treturn long2(x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inLong2_inLong_outLong2 (const Long2 &x, const Long &y, OUT Long2 &out)
	{
		FunctionNode	func( "Xor", "long2 Xor (const long2 x, const long y) {\n\treturn (x) ^ long2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inLong3_inLong3_outLong3 (const Long3 &x, const Long3 &y, OUT Long3 &out)
	{
		FunctionNode	func( "Xor", "long3 Xor (const long3 x, const long3 y) {\n\treturn (x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inLong_inLong3_outLong3 (const Long &x, const Long3 &y, OUT Long3 &out)
	{
		FunctionNode	func( "Xor", "long3 Xor (const long x, const long3 y) {\n\treturn long3(x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inLong3_inLong_outLong3 (const Long3 &x, const Long &y, OUT Long3 &out)
	{
		FunctionNode	func( "Xor", "long3 Xor (const long3 x, const long y) {\n\treturn (x) ^ long3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inLong4_inLong4_outLong4 (const Long4 &x, const Long4 &y, OUT Long4 &out)
	{
		FunctionNode	func( "Xor", "long4 Xor (const long4 x, const long4 y) {\n\treturn (x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inLong_inLong4_outLong4 (const Long &x, const Long4 &y, OUT Long4 &out)
	{
		FunctionNode	func( "Xor", "long4 Xor (const long x, const long4 y) {\n\treturn long4(x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inLong4_inLong_outLong4 (const Long4 &x, const Long &y, OUT Long4 &out)
	{
		FunctionNode	func( "Xor", "long4 Xor (const long4 x, const long y) {\n\treturn (x) ^ long4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inLong_inLong_outLong (const Long &x, const Long &y, OUT Long &out)
	{
		FunctionNode	func( "BitLShift", "long BitLShift (const long x, const long y) {\n\treturn (x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inLong2_inLong2_outLong2 (const Long2 &x, const Long2 &y, OUT Long2 &out)
	{
		FunctionNode	func( "BitLShift", "long2 BitLShift (const long2 x, const long2 y) {\n\treturn (x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inLong_inLong2_outLong2 (const Long &x, const Long2 &y, OUT Long2 &out)
	{
		FunctionNode	func( "BitLShift", "long2 BitLShift (const long x, const long2 y) {\n\treturn long2(x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inLong2_inLong_outLong2 (const Long2 &x, const Long &y, OUT Long2 &out)
	{
		FunctionNode	func( "BitLShift", "long2 BitLShift (const long2 x, const long y) {\n\treturn (x) << long2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inLong3_inLong3_outLong3 (const Long3 &x, const Long3 &y, OUT Long3 &out)
	{
		FunctionNode	func( "BitLShift", "long3 BitLShift (const long3 x, const long3 y) {\n\treturn (x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inLong_inLong3_outLong3 (const Long &x, const Long3 &y, OUT Long3 &out)
	{
		FunctionNode	func( "BitLShift", "long3 BitLShift (const long x, const long3 y) {\n\treturn long3(x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inLong3_inLong_outLong3 (const Long3 &x, const Long &y, OUT Long3 &out)
	{
		FunctionNode	func( "BitLShift", "long3 BitLShift (const long3 x, const long y) {\n\treturn (x) << long3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inLong4_inLong4_outLong4 (const Long4 &x, const Long4 &y, OUT Long4 &out)
	{
		FunctionNode	func( "BitLShift", "long4 BitLShift (const long4 x, const long4 y) {\n\treturn (x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inLong_inLong4_outLong4 (const Long &x, const Long4 &y, OUT Long4 &out)
	{
		FunctionNode	func( "BitLShift", "long4 BitLShift (const long x, const long4 y) {\n\treturn long4(x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inLong4_inLong_outLong4 (const Long4 &x, const Long &y, OUT Long4 &out)
	{
		FunctionNode	func( "BitLShift", "long4 BitLShift (const long4 x, const long y) {\n\treturn (x) << long4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inLong_inLong_outLong (const Long &x, const Long &y, OUT Long &out)
	{
		FunctionNode	func( "BitRShift", "long BitRShift (const long x, const long y) {\n\treturn (x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inLong2_inLong2_outLong2 (const Long2 &x, const Long2 &y, OUT Long2 &out)
	{
		FunctionNode	func( "BitRShift", "long2 BitRShift (const long2 x, const long2 y) {\n\treturn (x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inLong_inLong2_outLong2 (const Long &x, const Long2 &y, OUT Long2 &out)
	{
		FunctionNode	func( "BitRShift", "long2 BitRShift (const long x, const long2 y) {\n\treturn long2(x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inLong2_inLong_outLong2 (const Long2 &x, const Long &y, OUT Long2 &out)
	{
		FunctionNode	func( "BitRShift", "long2 BitRShift (const long2 x, const long y) {\n\treturn (x) >> long2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inLong3_inLong3_outLong3 (const Long3 &x, const Long3 &y, OUT Long3 &out)
	{
		FunctionNode	func( "BitRShift", "long3 BitRShift (const long3 x, const long3 y) {\n\treturn (x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inLong_inLong3_outLong3 (const Long &x, const Long3 &y, OUT Long3 &out)
	{
		FunctionNode	func( "BitRShift", "long3 BitRShift (const long x, const long3 y) {\n\treturn long3(x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inLong3_inLong_outLong3 (const Long3 &x, const Long &y, OUT Long3 &out)
	{
		FunctionNode	func( "BitRShift", "long3 BitRShift (const long3 x, const long y) {\n\treturn (x) >> long3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inLong4_inLong4_outLong4 (const Long4 &x, const Long4 &y, OUT Long4 &out)
	{
		FunctionNode	func( "BitRShift", "long4 BitRShift (const long4 x, const long4 y) {\n\treturn (x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inLong_inLong4_outLong4 (const Long &x, const Long4 &y, OUT Long4 &out)
	{
		FunctionNode	func( "BitRShift", "long4 BitRShift (const long x, const long4 y) {\n\treturn long4(x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inLong4_inLong_outLong4 (const Long4 &x, const Long &y, OUT Long4 &out)
	{
		FunctionNode	func( "BitRShift", "long4 BitRShift (const long4 x, const long y) {\n\treturn (x) >> long4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inLong_inLong_outBool (const Long &x, const Long &y, OUT Bool &out)
	{
		FunctionNode	func( "Equal", "bool Equal (const long x, const long y) {\n\treturn (x) == (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inLong2_inLong2_outBool2 (const Long2 &x, const Long2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "Equal", "bool2 Equal (const long2 x, const long2 y) {\n\treturn equal( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inLong_inLong2_outBool2 (const Long &x, const Long2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "Equal", "bool2 Equal (const long x, const long2 y) {\n\treturn equal( long2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inLong2_inLong_outBool2 (const Long2 &x, const Long &y, OUT Bool2 &out)
	{
		FunctionNode	func( "Equal", "bool2 Equal (const long2 x, const long y) {\n\treturn equal( (x), long2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inLong3_inLong3_outBool3 (const Long3 &x, const Long3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "Equal", "bool3 Equal (const long3 x, const long3 y) {\n\treturn equal( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inLong_inLong3_outBool3 (const Long &x, const Long3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "Equal", "bool3 Equal (const long x, const long3 y) {\n\treturn equal( long3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inLong3_inLong_outBool3 (const Long3 &x, const Long &y, OUT Bool3 &out)
	{
		FunctionNode	func( "Equal", "bool3 Equal (const long3 x, const long y) {\n\treturn equal( (x), long3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inLong4_inLong4_outBool4 (const Long4 &x, const Long4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "Equal", "bool4 Equal (const long4 x, const long4 y) {\n\treturn equal( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inLong_inLong4_outBool4 (const Long &x, const Long4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "Equal", "bool4 Equal (const long x, const long4 y) {\n\treturn equal( long4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inLong4_inLong_outBool4 (const Long4 &x, const Long &y, OUT Bool4 &out)
	{
		FunctionNode	func( "Equal", "bool4 Equal (const long4 x, const long y) {\n\treturn equal( (x), long4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inLong_inLong_outBool (const Long &x, const Long &y, OUT Bool &out)
	{
		FunctionNode	func( "NotEqual", "bool NotEqual (const long x, const long y) {\n\treturn (x) != (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inLong2_inLong2_outBool2 (const Long2 &x, const Long2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "NotEqual", "bool2 NotEqual (const long2 x, const long2 y) {\n\treturn notEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inLong_inLong2_outBool2 (const Long &x, const Long2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "NotEqual", "bool2 NotEqual (const long x, const long2 y) {\n\treturn notEqual( long2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inLong2_inLong_outBool2 (const Long2 &x, const Long &y, OUT Bool2 &out)
	{
		FunctionNode	func( "NotEqual", "bool2 NotEqual (const long2 x, const long y) {\n\treturn notEqual( (x), long2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inLong3_inLong3_outBool3 (const Long3 &x, const Long3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "NotEqual", "bool3 NotEqual (const long3 x, const long3 y) {\n\treturn notEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inLong_inLong3_outBool3 (const Long &x, const Long3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "NotEqual", "bool3 NotEqual (const long x, const long3 y) {\n\treturn notEqual( long3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inLong3_inLong_outBool3 (const Long3 &x, const Long &y, OUT Bool3 &out)
	{
		FunctionNode	func( "NotEqual", "bool3 NotEqual (const long3 x, const long y) {\n\treturn notEqual( (x), long3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inLong4_inLong4_outBool4 (const Long4 &x, const Long4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "NotEqual", "bool4 NotEqual (const long4 x, const long4 y) {\n\treturn notEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inLong_inLong4_outBool4 (const Long &x, const Long4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "NotEqual", "bool4 NotEqual (const long x, const long4 y) {\n\treturn notEqual( long4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inLong4_inLong_outBool4 (const Long4 &x, const Long &y, OUT Bool4 &out)
	{
		FunctionNode	func( "NotEqual", "bool4 NotEqual (const long4 x, const long y) {\n\treturn notEqual( (x), long4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inLong_inLong_outBool (const Long &x, const Long &y, OUT Bool &out)
	{
		FunctionNode	func( "GreaterThan", "bool GreaterThan (const long x, const long y) {\n\treturn (x) > (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inLong2_inLong2_outBool2 (const Long2 &x, const Long2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThan", "bool2 GreaterThan (const long2 x, const long2 y) {\n\treturn greaterThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inLong_inLong2_outBool2 (const Long &x, const Long2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThan", "bool2 GreaterThan (const long x, const long2 y) {\n\treturn greaterThan( long2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inLong2_inLong_outBool2 (const Long2 &x, const Long &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThan", "bool2 GreaterThan (const long2 x, const long y) {\n\treturn greaterThan( (x), long2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inLong3_inLong3_outBool3 (const Long3 &x, const Long3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThan", "bool3 GreaterThan (const long3 x, const long3 y) {\n\treturn greaterThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inLong_inLong3_outBool3 (const Long &x, const Long3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThan", "bool3 GreaterThan (const long x, const long3 y) {\n\treturn greaterThan( long3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inLong3_inLong_outBool3 (const Long3 &x, const Long &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThan", "bool3 GreaterThan (const long3 x, const long y) {\n\treturn greaterThan( (x), long3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inLong4_inLong4_outBool4 (const Long4 &x, const Long4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThan", "bool4 GreaterThan (const long4 x, const long4 y) {\n\treturn greaterThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inLong_inLong4_outBool4 (const Long &x, const Long4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThan", "bool4 GreaterThan (const long x, const long4 y) {\n\treturn greaterThan( long4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inLong4_inLong_outBool4 (const Long4 &x, const Long &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThan", "bool4 GreaterThan (const long4 x, const long y) {\n\treturn greaterThan( (x), long4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inLong_inLong_outBool (const Long &x, const Long &y, OUT Bool &out)
	{
		FunctionNode	func( "LessThan", "bool LessThan (const long x, const long y) {\n\treturn (x) < (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inLong2_inLong2_outBool2 (const Long2 &x, const Long2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThan", "bool2 LessThan (const long2 x, const long2 y) {\n\treturn lessThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inLong_inLong2_outBool2 (const Long &x, const Long2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThan", "bool2 LessThan (const long x, const long2 y) {\n\treturn lessThan( long2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inLong2_inLong_outBool2 (const Long2 &x, const Long &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThan", "bool2 LessThan (const long2 x, const long y) {\n\treturn lessThan( (x), long2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inLong3_inLong3_outBool3 (const Long3 &x, const Long3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThan", "bool3 LessThan (const long3 x, const long3 y) {\n\treturn lessThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inLong_inLong3_outBool3 (const Long &x, const Long3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThan", "bool3 LessThan (const long x, const long3 y) {\n\treturn lessThan( long3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inLong3_inLong_outBool3 (const Long3 &x, const Long &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThan", "bool3 LessThan (const long3 x, const long y) {\n\treturn lessThan( (x), long3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inLong4_inLong4_outBool4 (const Long4 &x, const Long4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThan", "bool4 LessThan (const long4 x, const long4 y) {\n\treturn lessThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inLong_inLong4_outBool4 (const Long &x, const Long4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThan", "bool4 LessThan (const long x, const long4 y) {\n\treturn lessThan( long4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inLong4_inLong_outBool4 (const Long4 &x, const Long &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThan", "bool4 LessThan (const long4 x, const long y) {\n\treturn lessThan( (x), long4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inLong_inLong_outBool (const Long &x, const Long &y, OUT Bool &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool GreaterThanEqual (const long x, const long y) {\n\treturn (x) >= (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inLong2_inLong2_outBool2 (const Long2 &x, const Long2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool2 GreaterThanEqual (const long2 x, const long2 y) {\n\treturn greaterThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inLong_inLong2_outBool2 (const Long &x, const Long2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool2 GreaterThanEqual (const long x, const long2 y) {\n\treturn greaterThanEqual( long2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inLong2_inLong_outBool2 (const Long2 &x, const Long &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool2 GreaterThanEqual (const long2 x, const long y) {\n\treturn greaterThanEqual( (x), long2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inLong3_inLong3_outBool3 (const Long3 &x, const Long3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool3 GreaterThanEqual (const long3 x, const long3 y) {\n\treturn greaterThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inLong_inLong3_outBool3 (const Long &x, const Long3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool3 GreaterThanEqual (const long x, const long3 y) {\n\treturn greaterThanEqual( long3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inLong3_inLong_outBool3 (const Long3 &x, const Long &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool3 GreaterThanEqual (const long3 x, const long y) {\n\treturn greaterThanEqual( (x), long3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inLong4_inLong4_outBool4 (const Long4 &x, const Long4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool4 GreaterThanEqual (const long4 x, const long4 y) {\n\treturn greaterThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inLong_inLong4_outBool4 (const Long &x, const Long4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool4 GreaterThanEqual (const long x, const long4 y) {\n\treturn greaterThanEqual( long4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inLong4_inLong_outBool4 (const Long4 &x, const Long &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool4 GreaterThanEqual (const long4 x, const long y) {\n\treturn greaterThanEqual( (x), long4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inLong_inLong_outBool (const Long &x, const Long &y, OUT Bool &out)
	{
		FunctionNode	func( "LessThanEqual", "bool LessThanEqual (const long x, const long y) {\n\treturn (x) <= (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inLong2_inLong2_outBool2 (const Long2 &x, const Long2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool2 LessThanEqual (const long2 x, const long2 y) {\n\treturn lessThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inLong_inLong2_outBool2 (const Long &x, const Long2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool2 LessThanEqual (const long x, const long2 y) {\n\treturn lessThanEqual( long2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inLong2_inLong_outBool2 (const Long2 &x, const Long &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool2 LessThanEqual (const long2 x, const long y) {\n\treturn lessThanEqual( (x), long2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inLong3_inLong3_outBool3 (const Long3 &x, const Long3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool3 LessThanEqual (const long3 x, const long3 y) {\n\treturn lessThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inLong_inLong3_outBool3 (const Long &x, const Long3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool3 LessThanEqual (const long x, const long3 y) {\n\treturn lessThanEqual( long3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inLong3_inLong_outBool3 (const Long3 &x, const Long &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool3 LessThanEqual (const long3 x, const long y) {\n\treturn lessThanEqual( (x), long3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inLong4_inLong4_outBool4 (const Long4 &x, const Long4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool4 LessThanEqual (const long4 x, const long4 y) {\n\treturn lessThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inLong_inLong4_outBool4 (const Long &x, const Long4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool4 LessThanEqual (const long x, const long4 y) {\n\treturn lessThanEqual( long4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inLong4_inLong_outBool4 (const Long4 &x, const Long &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool4 LessThanEqual (const long4 x, const long y) {\n\treturn lessThanEqual( (x), long4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitInverse_inULong_outULong (const ULong &x, OUT ULong &out)
	{
		FunctionNode	func( "BitInverse", "ulong BitInverse (const ulong x) {\n\treturn ~(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool BitInverse_inULong2_outULong2 (const ULong2 &x, OUT ULong2 &out)
	{
		FunctionNode	func( "BitInverse", "ulong2 BitInverse (const ulong2 x) {\n\treturn ~(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool BitInverse_inULong3_outULong3 (const ULong3 &x, OUT ULong3 &out)
	{
		FunctionNode	func( "BitInverse", "ulong3 BitInverse (const ulong3 x) {\n\treturn ~(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool BitInverse_inULong4_outULong4 (const ULong4 &x, OUT ULong4 &out)
	{
		FunctionNode	func( "BitInverse", "ulong4 BitInverse (const ulong4 x) {\n\treturn ~(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Plus_inULong_outULong (const ULong &x, OUT ULong &out)
	{
		FunctionNode	func( "Plus", "ulong Plus (const ulong x) {\n\treturn +(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Plus_inULong2_outULong2 (const ULong2 &x, OUT ULong2 &out)
	{
		FunctionNode	func( "Plus", "ulong2 Plus (const ulong2 x) {\n\treturn +(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Plus_inULong3_outULong3 (const ULong3 &x, OUT ULong3 &out)
	{
		FunctionNode	func( "Plus", "ulong3 Plus (const ulong3 x) {\n\treturn +(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Plus_inULong4_outULong4 (const ULong4 &x, OUT ULong4 &out)
	{
		FunctionNode	func( "Plus", "ulong4 Plus (const ulong4 x) {\n\treturn +(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Minus_inULong_outULong (const ULong &x, OUT ULong &out)
	{
		FunctionNode	func( "Minus", "ulong Minus (const ulong x) {\n\treturn -(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Minus_inULong2_outULong2 (const ULong2 &x, OUT ULong2 &out)
	{
		FunctionNode	func( "Minus", "ulong2 Minus (const ulong2 x) {\n\treturn -(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Minus_inULong3_outULong3 (const ULong3 &x, OUT ULong3 &out)
	{
		FunctionNode	func( "Minus", "ulong3 Minus (const ulong3 x) {\n\treturn -(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Minus_inULong4_outULong4 (const ULong4 &x, OUT ULong4 &out)
	{
		FunctionNode	func( "Minus", "ulong4 Minus (const ulong4 x) {\n\treturn -(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inULong_inULong_outULong (const ULong &x, const ULong &y, OUT ULong &out)
	{
		FunctionNode	func( "Add", "ulong Add (const ulong x, const ulong y) {\n\treturn (x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inULong2_inULong2_outULong2 (const ULong2 &x, const ULong2 &y, OUT ULong2 &out)
	{
		FunctionNode	func( "Add", "ulong2 Add (const ulong2 x, const ulong2 y) {\n\treturn (x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inULong_inULong2_outULong2 (const ULong &x, const ULong2 &y, OUT ULong2 &out)
	{
		FunctionNode	func( "Add", "ulong2 Add (const ulong x, const ulong2 y) {\n\treturn ulong2(x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inULong2_inULong_outULong2 (const ULong2 &x, const ULong &y, OUT ULong2 &out)
	{
		FunctionNode	func( "Add", "ulong2 Add (const ulong2 x, const ulong y) {\n\treturn (x) + ulong2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inULong3_inULong3_outULong3 (const ULong3 &x, const ULong3 &y, OUT ULong3 &out)
	{
		FunctionNode	func( "Add", "ulong3 Add (const ulong3 x, const ulong3 y) {\n\treturn (x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inULong_inULong3_outULong3 (const ULong &x, const ULong3 &y, OUT ULong3 &out)
	{
		FunctionNode	func( "Add", "ulong3 Add (const ulong x, const ulong3 y) {\n\treturn ulong3(x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inULong3_inULong_outULong3 (const ULong3 &x, const ULong &y, OUT ULong3 &out)
	{
		FunctionNode	func( "Add", "ulong3 Add (const ulong3 x, const ulong y) {\n\treturn (x) + ulong3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inULong4_inULong4_outULong4 (const ULong4 &x, const ULong4 &y, OUT ULong4 &out)
	{
		FunctionNode	func( "Add", "ulong4 Add (const ulong4 x, const ulong4 y) {\n\treturn (x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inULong_inULong4_outULong4 (const ULong &x, const ULong4 &y, OUT ULong4 &out)
	{
		FunctionNode	func( "Add", "ulong4 Add (const ulong x, const ulong4 y) {\n\treturn ulong4(x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inULong4_inULong_outULong4 (const ULong4 &x, const ULong &y, OUT ULong4 &out)
	{
		FunctionNode	func( "Add", "ulong4 Add (const ulong4 x, const ulong y) {\n\treturn (x) + ulong4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inULong_inULong_outULong (const ULong &x, const ULong &y, OUT ULong &out)
	{
		FunctionNode	func( "Sub", "ulong Sub (const ulong x, const ulong y) {\n\treturn (x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inULong2_inULong2_outULong2 (const ULong2 &x, const ULong2 &y, OUT ULong2 &out)
	{
		FunctionNode	func( "Sub", "ulong2 Sub (const ulong2 x, const ulong2 y) {\n\treturn (x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inULong_inULong2_outULong2 (const ULong &x, const ULong2 &y, OUT ULong2 &out)
	{
		FunctionNode	func( "Sub", "ulong2 Sub (const ulong x, const ulong2 y) {\n\treturn ulong2(x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inULong2_inULong_outULong2 (const ULong2 &x, const ULong &y, OUT ULong2 &out)
	{
		FunctionNode	func( "Sub", "ulong2 Sub (const ulong2 x, const ulong y) {\n\treturn (x) - ulong2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inULong3_inULong3_outULong3 (const ULong3 &x, const ULong3 &y, OUT ULong3 &out)
	{
		FunctionNode	func( "Sub", "ulong3 Sub (const ulong3 x, const ulong3 y) {\n\treturn (x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inULong_inULong3_outULong3 (const ULong &x, const ULong3 &y, OUT ULong3 &out)
	{
		FunctionNode	func( "Sub", "ulong3 Sub (const ulong x, const ulong3 y) {\n\treturn ulong3(x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inULong3_inULong_outULong3 (const ULong3 &x, const ULong &y, OUT ULong3 &out)
	{
		FunctionNode	func( "Sub", "ulong3 Sub (const ulong3 x, const ulong y) {\n\treturn (x) - ulong3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inULong4_inULong4_outULong4 (const ULong4 &x, const ULong4 &y, OUT ULong4 &out)
	{
		FunctionNode	func( "Sub", "ulong4 Sub (const ulong4 x, const ulong4 y) {\n\treturn (x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inULong_inULong4_outULong4 (const ULong &x, const ULong4 &y, OUT ULong4 &out)
	{
		FunctionNode	func( "Sub", "ulong4 Sub (const ulong x, const ulong4 y) {\n\treturn ulong4(x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inULong4_inULong_outULong4 (const ULong4 &x, const ULong &y, OUT ULong4 &out)
	{
		FunctionNode	func( "Sub", "ulong4 Sub (const ulong4 x, const ulong y) {\n\treturn (x) - ulong4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inULong_inULong_outULong (const ULong &x, const ULong &y, OUT ULong &out)
	{
		FunctionNode	func( "Mul", "ulong Mul (const ulong x, const ulong y) {\n\treturn (x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inULong2_inULong2_outULong2 (const ULong2 &x, const ULong2 &y, OUT ULong2 &out)
	{
		FunctionNode	func( "Mul", "ulong2 Mul (const ulong2 x, const ulong2 y) {\n\treturn (x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inULong_inULong2_outULong2 (const ULong &x, const ULong2 &y, OUT ULong2 &out)
	{
		FunctionNode	func( "Mul", "ulong2 Mul (const ulong x, const ulong2 y) {\n\treturn ulong2(x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inULong2_inULong_outULong2 (const ULong2 &x, const ULong &y, OUT ULong2 &out)
	{
		FunctionNode	func( "Mul", "ulong2 Mul (const ulong2 x, const ulong y) {\n\treturn (x) * ulong2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inULong3_inULong3_outULong3 (const ULong3 &x, const ULong3 &y, OUT ULong3 &out)
	{
		FunctionNode	func( "Mul", "ulong3 Mul (const ulong3 x, const ulong3 y) {\n\treturn (x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inULong_inULong3_outULong3 (const ULong &x, const ULong3 &y, OUT ULong3 &out)
	{
		FunctionNode	func( "Mul", "ulong3 Mul (const ulong x, const ulong3 y) {\n\treturn ulong3(x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inULong3_inULong_outULong3 (const ULong3 &x, const ULong &y, OUT ULong3 &out)
	{
		FunctionNode	func( "Mul", "ulong3 Mul (const ulong3 x, const ulong y) {\n\treturn (x) * ulong3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inULong4_inULong4_outULong4 (const ULong4 &x, const ULong4 &y, OUT ULong4 &out)
	{
		FunctionNode	func( "Mul", "ulong4 Mul (const ulong4 x, const ulong4 y) {\n\treturn (x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inULong_inULong4_outULong4 (const ULong &x, const ULong4 &y, OUT ULong4 &out)
	{
		FunctionNode	func( "Mul", "ulong4 Mul (const ulong x, const ulong4 y) {\n\treturn ulong4(x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inULong4_inULong_outULong4 (const ULong4 &x, const ULong &y, OUT ULong4 &out)
	{
		FunctionNode	func( "Mul", "ulong4 Mul (const ulong4 x, const ulong y) {\n\treturn (x) * ulong4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inULong_inULong_outULong (const ULong &x, const ULong &y, OUT ULong &out)
	{
		FunctionNode	func( "Div", "ulong Div (const ulong x, const ulong y) {\n\treturn (x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inULong2_inULong2_outULong2 (const ULong2 &x, const ULong2 &y, OUT ULong2 &out)
	{
		FunctionNode	func( "Div", "ulong2 Div (const ulong2 x, const ulong2 y) {\n\treturn (x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inULong_inULong2_outULong2 (const ULong &x, const ULong2 &y, OUT ULong2 &out)
	{
		FunctionNode	func( "Div", "ulong2 Div (const ulong x, const ulong2 y) {\n\treturn ulong2(x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inULong2_inULong_outULong2 (const ULong2 &x, const ULong &y, OUT ULong2 &out)
	{
		FunctionNode	func( "Div", "ulong2 Div (const ulong2 x, const ulong y) {\n\treturn (x) / ulong2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inULong3_inULong3_outULong3 (const ULong3 &x, const ULong3 &y, OUT ULong3 &out)
	{
		FunctionNode	func( "Div", "ulong3 Div (const ulong3 x, const ulong3 y) {\n\treturn (x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inULong_inULong3_outULong3 (const ULong &x, const ULong3 &y, OUT ULong3 &out)
	{
		FunctionNode	func( "Div", "ulong3 Div (const ulong x, const ulong3 y) {\n\treturn ulong3(x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inULong3_inULong_outULong3 (const ULong3 &x, const ULong &y, OUT ULong3 &out)
	{
		FunctionNode	func( "Div", "ulong3 Div (const ulong3 x, const ulong y) {\n\treturn (x) / ulong3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inULong4_inULong4_outULong4 (const ULong4 &x, const ULong4 &y, OUT ULong4 &out)
	{
		FunctionNode	func( "Div", "ulong4 Div (const ulong4 x, const ulong4 y) {\n\treturn (x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inULong_inULong4_outULong4 (const ULong &x, const ULong4 &y, OUT ULong4 &out)
	{
		FunctionNode	func( "Div", "ulong4 Div (const ulong x, const ulong4 y) {\n\treturn ulong4(x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inULong4_inULong_outULong4 (const ULong4 &x, const ULong &y, OUT ULong4 &out)
	{
		FunctionNode	func( "Div", "ulong4 Div (const ulong4 x, const ulong y) {\n\treturn (x) / ulong4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inULong_inULong_outULong (const ULong &x, const ULong &y, OUT ULong &out)
	{
		FunctionNode	func( "Mod", "ulong Mod (const ulong x, const ulong y) {\n\treturn (x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inULong2_inULong2_outULong2 (const ULong2 &x, const ULong2 &y, OUT ULong2 &out)
	{
		FunctionNode	func( "Mod", "ulong2 Mod (const ulong2 x, const ulong2 y) {\n\treturn (x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inULong_inULong2_outULong2 (const ULong &x, const ULong2 &y, OUT ULong2 &out)
	{
		FunctionNode	func( "Mod", "ulong2 Mod (const ulong x, const ulong2 y) {\n\treturn ulong2(x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inULong2_inULong_outULong2 (const ULong2 &x, const ULong &y, OUT ULong2 &out)
	{
		FunctionNode	func( "Mod", "ulong2 Mod (const ulong2 x, const ulong y) {\n\treturn (x) % ulong2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inULong3_inULong3_outULong3 (const ULong3 &x, const ULong3 &y, OUT ULong3 &out)
	{
		FunctionNode	func( "Mod", "ulong3 Mod (const ulong3 x, const ulong3 y) {\n\treturn (x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inULong_inULong3_outULong3 (const ULong &x, const ULong3 &y, OUT ULong3 &out)
	{
		FunctionNode	func( "Mod", "ulong3 Mod (const ulong x, const ulong3 y) {\n\treturn ulong3(x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inULong3_inULong_outULong3 (const ULong3 &x, const ULong &y, OUT ULong3 &out)
	{
		FunctionNode	func( "Mod", "ulong3 Mod (const ulong3 x, const ulong y) {\n\treturn (x) % ulong3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inULong4_inULong4_outULong4 (const ULong4 &x, const ULong4 &y, OUT ULong4 &out)
	{
		FunctionNode	func( "Mod", "ulong4 Mod (const ulong4 x, const ulong4 y) {\n\treturn (x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inULong_inULong4_outULong4 (const ULong &x, const ULong4 &y, OUT ULong4 &out)
	{
		FunctionNode	func( "Mod", "ulong4 Mod (const ulong x, const ulong4 y) {\n\treturn ulong4(x) % (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inULong4_inULong_outULong4 (const ULong4 &x, const ULong &y, OUT ULong4 &out)
	{
		FunctionNode	func( "Mod", "ulong4 Mod (const ulong4 x, const ulong y) {\n\treturn (x) % ulong4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inULong_inULong_outULong (const ULong &x, const ULong &y, OUT ULong &out)
	{
		FunctionNode	func( "And", "ulong And (const ulong x, const ulong y) {\n\treturn (x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inULong2_inULong2_outULong2 (const ULong2 &x, const ULong2 &y, OUT ULong2 &out)
	{
		FunctionNode	func( "And", "ulong2 And (const ulong2 x, const ulong2 y) {\n\treturn (x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inULong_inULong2_outULong2 (const ULong &x, const ULong2 &y, OUT ULong2 &out)
	{
		FunctionNode	func( "And", "ulong2 And (const ulong x, const ulong2 y) {\n\treturn ulong2(x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inULong2_inULong_outULong2 (const ULong2 &x, const ULong &y, OUT ULong2 &out)
	{
		FunctionNode	func( "And", "ulong2 And (const ulong2 x, const ulong y) {\n\treturn (x) & ulong2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inULong3_inULong3_outULong3 (const ULong3 &x, const ULong3 &y, OUT ULong3 &out)
	{
		FunctionNode	func( "And", "ulong3 And (const ulong3 x, const ulong3 y) {\n\treturn (x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inULong_inULong3_outULong3 (const ULong &x, const ULong3 &y, OUT ULong3 &out)
	{
		FunctionNode	func( "And", "ulong3 And (const ulong x, const ulong3 y) {\n\treturn ulong3(x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inULong3_inULong_outULong3 (const ULong3 &x, const ULong &y, OUT ULong3 &out)
	{
		FunctionNode	func( "And", "ulong3 And (const ulong3 x, const ulong y) {\n\treturn (x) & ulong3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inULong4_inULong4_outULong4 (const ULong4 &x, const ULong4 &y, OUT ULong4 &out)
	{
		FunctionNode	func( "And", "ulong4 And (const ulong4 x, const ulong4 y) {\n\treturn (x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inULong_inULong4_outULong4 (const ULong &x, const ULong4 &y, OUT ULong4 &out)
	{
		FunctionNode	func( "And", "ulong4 And (const ulong x, const ulong4 y) {\n\treturn ulong4(x) & (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool And_inULong4_inULong_outULong4 (const ULong4 &x, const ULong &y, OUT ULong4 &out)
	{
		FunctionNode	func( "And", "ulong4 And (const ulong4 x, const ulong y) {\n\treturn (x) & ulong4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inULong_inULong_outULong (const ULong &x, const ULong &y, OUT ULong &out)
	{
		FunctionNode	func( "Or", "ulong Or (const ulong x, const ulong y) {\n\treturn (x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inULong2_inULong2_outULong2 (const ULong2 &x, const ULong2 &y, OUT ULong2 &out)
	{
		FunctionNode	func( "Or", "ulong2 Or (const ulong2 x, const ulong2 y) {\n\treturn (x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inULong_inULong2_outULong2 (const ULong &x, const ULong2 &y, OUT ULong2 &out)
	{
		FunctionNode	func( "Or", "ulong2 Or (const ulong x, const ulong2 y) {\n\treturn ulong2(x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inULong2_inULong_outULong2 (const ULong2 &x, const ULong &y, OUT ULong2 &out)
	{
		FunctionNode	func( "Or", "ulong2 Or (const ulong2 x, const ulong y) {\n\treturn (x) | ulong2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inULong3_inULong3_outULong3 (const ULong3 &x, const ULong3 &y, OUT ULong3 &out)
	{
		FunctionNode	func( "Or", "ulong3 Or (const ulong3 x, const ulong3 y) {\n\treturn (x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inULong_inULong3_outULong3 (const ULong &x, const ULong3 &y, OUT ULong3 &out)
	{
		FunctionNode	func( "Or", "ulong3 Or (const ulong x, const ulong3 y) {\n\treturn ulong3(x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inULong3_inULong_outULong3 (const ULong3 &x, const ULong &y, OUT ULong3 &out)
	{
		FunctionNode	func( "Or", "ulong3 Or (const ulong3 x, const ulong y) {\n\treturn (x) | ulong3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inULong4_inULong4_outULong4 (const ULong4 &x, const ULong4 &y, OUT ULong4 &out)
	{
		FunctionNode	func( "Or", "ulong4 Or (const ulong4 x, const ulong4 y) {\n\treturn (x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inULong_inULong4_outULong4 (const ULong &x, const ULong4 &y, OUT ULong4 &out)
	{
		FunctionNode	func( "Or", "ulong4 Or (const ulong x, const ulong4 y) {\n\treturn ulong4(x) | (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Or_inULong4_inULong_outULong4 (const ULong4 &x, const ULong &y, OUT ULong4 &out)
	{
		FunctionNode	func( "Or", "ulong4 Or (const ulong4 x, const ulong y) {\n\treturn (x) | ulong4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inULong_inULong_outULong (const ULong &x, const ULong &y, OUT ULong &out)
	{
		FunctionNode	func( "Xor", "ulong Xor (const ulong x, const ulong y) {\n\treturn (x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inULong2_inULong2_outULong2 (const ULong2 &x, const ULong2 &y, OUT ULong2 &out)
	{
		FunctionNode	func( "Xor", "ulong2 Xor (const ulong2 x, const ulong2 y) {\n\treturn (x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inULong_inULong2_outULong2 (const ULong &x, const ULong2 &y, OUT ULong2 &out)
	{
		FunctionNode	func( "Xor", "ulong2 Xor (const ulong x, const ulong2 y) {\n\treturn ulong2(x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inULong2_inULong_outULong2 (const ULong2 &x, const ULong &y, OUT ULong2 &out)
	{
		FunctionNode	func( "Xor", "ulong2 Xor (const ulong2 x, const ulong y) {\n\treturn (x) ^ ulong2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inULong3_inULong3_outULong3 (const ULong3 &x, const ULong3 &y, OUT ULong3 &out)
	{
		FunctionNode	func( "Xor", "ulong3 Xor (const ulong3 x, const ulong3 y) {\n\treturn (x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inULong_inULong3_outULong3 (const ULong &x, const ULong3 &y, OUT ULong3 &out)
	{
		FunctionNode	func( "Xor", "ulong3 Xor (const ulong x, const ulong3 y) {\n\treturn ulong3(x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inULong3_inULong_outULong3 (const ULong3 &x, const ULong &y, OUT ULong3 &out)
	{
		FunctionNode	func( "Xor", "ulong3 Xor (const ulong3 x, const ulong y) {\n\treturn (x) ^ ulong3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inULong4_inULong4_outULong4 (const ULong4 &x, const ULong4 &y, OUT ULong4 &out)
	{
		FunctionNode	func( "Xor", "ulong4 Xor (const ulong4 x, const ulong4 y) {\n\treturn (x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inULong_inULong4_outULong4 (const ULong &x, const ULong4 &y, OUT ULong4 &out)
	{
		FunctionNode	func( "Xor", "ulong4 Xor (const ulong x, const ulong4 y) {\n\treturn ulong4(x) ^ (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Xor_inULong4_inULong_outULong4 (const ULong4 &x, const ULong &y, OUT ULong4 &out)
	{
		FunctionNode	func( "Xor", "ulong4 Xor (const ulong4 x, const ulong y) {\n\treturn (x) ^ ulong4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inULong_inULong_outULong (const ULong &x, const ULong &y, OUT ULong &out)
	{
		FunctionNode	func( "BitLShift", "ulong BitLShift (const ulong x, const ulong y) {\n\treturn (x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inULong2_inULong2_outULong2 (const ULong2 &x, const ULong2 &y, OUT ULong2 &out)
	{
		FunctionNode	func( "BitLShift", "ulong2 BitLShift (const ulong2 x, const ulong2 y) {\n\treturn (x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inULong_inULong2_outULong2 (const ULong &x, const ULong2 &y, OUT ULong2 &out)
	{
		FunctionNode	func( "BitLShift", "ulong2 BitLShift (const ulong x, const ulong2 y) {\n\treturn ulong2(x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inULong2_inULong_outULong2 (const ULong2 &x, const ULong &y, OUT ULong2 &out)
	{
		FunctionNode	func( "BitLShift", "ulong2 BitLShift (const ulong2 x, const ulong y) {\n\treturn (x) << ulong2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inULong3_inULong3_outULong3 (const ULong3 &x, const ULong3 &y, OUT ULong3 &out)
	{
		FunctionNode	func( "BitLShift", "ulong3 BitLShift (const ulong3 x, const ulong3 y) {\n\treturn (x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inULong_inULong3_outULong3 (const ULong &x, const ULong3 &y, OUT ULong3 &out)
	{
		FunctionNode	func( "BitLShift", "ulong3 BitLShift (const ulong x, const ulong3 y) {\n\treturn ulong3(x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inULong3_inULong_outULong3 (const ULong3 &x, const ULong &y, OUT ULong3 &out)
	{
		FunctionNode	func( "BitLShift", "ulong3 BitLShift (const ulong3 x, const ulong y) {\n\treturn (x) << ulong3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inULong4_inULong4_outULong4 (const ULong4 &x, const ULong4 &y, OUT ULong4 &out)
	{
		FunctionNode	func( "BitLShift", "ulong4 BitLShift (const ulong4 x, const ulong4 y) {\n\treturn (x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inULong_inULong4_outULong4 (const ULong &x, const ULong4 &y, OUT ULong4 &out)
	{
		FunctionNode	func( "BitLShift", "ulong4 BitLShift (const ulong x, const ulong4 y) {\n\treturn ulong4(x) << (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitLShift_inULong4_inULong_outULong4 (const ULong4 &x, const ULong &y, OUT ULong4 &out)
	{
		FunctionNode	func( "BitLShift", "ulong4 BitLShift (const ulong4 x, const ulong y) {\n\treturn (x) << ulong4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inULong_inULong_outULong (const ULong &x, const ULong &y, OUT ULong &out)
	{
		FunctionNode	func( "BitRShift", "ulong BitRShift (const ulong x, const ulong y) {\n\treturn (x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inULong2_inULong2_outULong2 (const ULong2 &x, const ULong2 &y, OUT ULong2 &out)
	{
		FunctionNode	func( "BitRShift", "ulong2 BitRShift (const ulong2 x, const ulong2 y) {\n\treturn (x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inULong_inULong2_outULong2 (const ULong &x, const ULong2 &y, OUT ULong2 &out)
	{
		FunctionNode	func( "BitRShift", "ulong2 BitRShift (const ulong x, const ulong2 y) {\n\treturn ulong2(x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inULong2_inULong_outULong2 (const ULong2 &x, const ULong &y, OUT ULong2 &out)
	{
		FunctionNode	func( "BitRShift", "ulong2 BitRShift (const ulong2 x, const ulong y) {\n\treturn (x) >> ulong2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inULong3_inULong3_outULong3 (const ULong3 &x, const ULong3 &y, OUT ULong3 &out)
	{
		FunctionNode	func( "BitRShift", "ulong3 BitRShift (const ulong3 x, const ulong3 y) {\n\treturn (x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inULong_inULong3_outULong3 (const ULong &x, const ULong3 &y, OUT ULong3 &out)
	{
		FunctionNode	func( "BitRShift", "ulong3 BitRShift (const ulong x, const ulong3 y) {\n\treturn ulong3(x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inULong3_inULong_outULong3 (const ULong3 &x, const ULong &y, OUT ULong3 &out)
	{
		FunctionNode	func( "BitRShift", "ulong3 BitRShift (const ulong3 x, const ulong y) {\n\treturn (x) >> ulong3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inULong4_inULong4_outULong4 (const ULong4 &x, const ULong4 &y, OUT ULong4 &out)
	{
		FunctionNode	func( "BitRShift", "ulong4 BitRShift (const ulong4 x, const ulong4 y) {\n\treturn (x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inULong_inULong4_outULong4 (const ULong &x, const ULong4 &y, OUT ULong4 &out)
	{
		FunctionNode	func( "BitRShift", "ulong4 BitRShift (const ulong x, const ulong4 y) {\n\treturn ulong4(x) >> (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool BitRShift_inULong4_inULong_outULong4 (const ULong4 &x, const ULong &y, OUT ULong4 &out)
	{
		FunctionNode	func( "BitRShift", "ulong4 BitRShift (const ulong4 x, const ulong y) {\n\treturn (x) >> ulong4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inULong_inULong_outBool (const ULong &x, const ULong &y, OUT Bool &out)
	{
		FunctionNode	func( "Equal", "bool Equal (const ulong x, const ulong y) {\n\treturn (x) == (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inULong2_inULong2_outBool2 (const ULong2 &x, const ULong2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "Equal", "bool2 Equal (const ulong2 x, const ulong2 y) {\n\treturn equal( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inULong_inULong2_outBool2 (const ULong &x, const ULong2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "Equal", "bool2 Equal (const ulong x, const ulong2 y) {\n\treturn equal( ulong2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inULong2_inULong_outBool2 (const ULong2 &x, const ULong &y, OUT Bool2 &out)
	{
		FunctionNode	func( "Equal", "bool2 Equal (const ulong2 x, const ulong y) {\n\treturn equal( (x), ulong2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inULong3_inULong3_outBool3 (const ULong3 &x, const ULong3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "Equal", "bool3 Equal (const ulong3 x, const ulong3 y) {\n\treturn equal( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inULong_inULong3_outBool3 (const ULong &x, const ULong3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "Equal", "bool3 Equal (const ulong x, const ulong3 y) {\n\treturn equal( ulong3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inULong3_inULong_outBool3 (const ULong3 &x, const ULong &y, OUT Bool3 &out)
	{
		FunctionNode	func( "Equal", "bool3 Equal (const ulong3 x, const ulong y) {\n\treturn equal( (x), ulong3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inULong4_inULong4_outBool4 (const ULong4 &x, const ULong4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "Equal", "bool4 Equal (const ulong4 x, const ulong4 y) {\n\treturn equal( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inULong_inULong4_outBool4 (const ULong &x, const ULong4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "Equal", "bool4 Equal (const ulong x, const ulong4 y) {\n\treturn equal( ulong4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inULong4_inULong_outBool4 (const ULong4 &x, const ULong &y, OUT Bool4 &out)
	{
		FunctionNode	func( "Equal", "bool4 Equal (const ulong4 x, const ulong y) {\n\treturn equal( (x), ulong4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inULong_inULong_outBool (const ULong &x, const ULong &y, OUT Bool &out)
	{
		FunctionNode	func( "NotEqual", "bool NotEqual (const ulong x, const ulong y) {\n\treturn (x) != (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inULong2_inULong2_outBool2 (const ULong2 &x, const ULong2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "NotEqual", "bool2 NotEqual (const ulong2 x, const ulong2 y) {\n\treturn notEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inULong_inULong2_outBool2 (const ULong &x, const ULong2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "NotEqual", "bool2 NotEqual (const ulong x, const ulong2 y) {\n\treturn notEqual( ulong2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inULong2_inULong_outBool2 (const ULong2 &x, const ULong &y, OUT Bool2 &out)
	{
		FunctionNode	func( "NotEqual", "bool2 NotEqual (const ulong2 x, const ulong y) {\n\treturn notEqual( (x), ulong2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inULong3_inULong3_outBool3 (const ULong3 &x, const ULong3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "NotEqual", "bool3 NotEqual (const ulong3 x, const ulong3 y) {\n\treturn notEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inULong_inULong3_outBool3 (const ULong &x, const ULong3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "NotEqual", "bool3 NotEqual (const ulong x, const ulong3 y) {\n\treturn notEqual( ulong3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inULong3_inULong_outBool3 (const ULong3 &x, const ULong &y, OUT Bool3 &out)
	{
		FunctionNode	func( "NotEqual", "bool3 NotEqual (const ulong3 x, const ulong y) {\n\treturn notEqual( (x), ulong3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inULong4_inULong4_outBool4 (const ULong4 &x, const ULong4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "NotEqual", "bool4 NotEqual (const ulong4 x, const ulong4 y) {\n\treturn notEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inULong_inULong4_outBool4 (const ULong &x, const ULong4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "NotEqual", "bool4 NotEqual (const ulong x, const ulong4 y) {\n\treturn notEqual( ulong4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inULong4_inULong_outBool4 (const ULong4 &x, const ULong &y, OUT Bool4 &out)
	{
		FunctionNode	func( "NotEqual", "bool4 NotEqual (const ulong4 x, const ulong y) {\n\treturn notEqual( (x), ulong4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inULong_inULong_outBool (const ULong &x, const ULong &y, OUT Bool &out)
	{
		FunctionNode	func( "GreaterThan", "bool GreaterThan (const ulong x, const ulong y) {\n\treturn (x) > (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inULong2_inULong2_outBool2 (const ULong2 &x, const ULong2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThan", "bool2 GreaterThan (const ulong2 x, const ulong2 y) {\n\treturn greaterThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inULong_inULong2_outBool2 (const ULong &x, const ULong2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThan", "bool2 GreaterThan (const ulong x, const ulong2 y) {\n\treturn greaterThan( ulong2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inULong2_inULong_outBool2 (const ULong2 &x, const ULong &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThan", "bool2 GreaterThan (const ulong2 x, const ulong y) {\n\treturn greaterThan( (x), ulong2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inULong3_inULong3_outBool3 (const ULong3 &x, const ULong3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThan", "bool3 GreaterThan (const ulong3 x, const ulong3 y) {\n\treturn greaterThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inULong_inULong3_outBool3 (const ULong &x, const ULong3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThan", "bool3 GreaterThan (const ulong x, const ulong3 y) {\n\treturn greaterThan( ulong3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inULong3_inULong_outBool3 (const ULong3 &x, const ULong &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThan", "bool3 GreaterThan (const ulong3 x, const ulong y) {\n\treturn greaterThan( (x), ulong3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inULong4_inULong4_outBool4 (const ULong4 &x, const ULong4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThan", "bool4 GreaterThan (const ulong4 x, const ulong4 y) {\n\treturn greaterThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inULong_inULong4_outBool4 (const ULong &x, const ULong4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThan", "bool4 GreaterThan (const ulong x, const ulong4 y) {\n\treturn greaterThan( ulong4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inULong4_inULong_outBool4 (const ULong4 &x, const ULong &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThan", "bool4 GreaterThan (const ulong4 x, const ulong y) {\n\treturn greaterThan( (x), ulong4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inULong_inULong_outBool (const ULong &x, const ULong &y, OUT Bool &out)
	{
		FunctionNode	func( "LessThan", "bool LessThan (const ulong x, const ulong y) {\n\treturn (x) < (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inULong2_inULong2_outBool2 (const ULong2 &x, const ULong2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThan", "bool2 LessThan (const ulong2 x, const ulong2 y) {\n\treturn lessThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inULong_inULong2_outBool2 (const ULong &x, const ULong2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThan", "bool2 LessThan (const ulong x, const ulong2 y) {\n\treturn lessThan( ulong2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inULong2_inULong_outBool2 (const ULong2 &x, const ULong &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThan", "bool2 LessThan (const ulong2 x, const ulong y) {\n\treturn lessThan( (x), ulong2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inULong3_inULong3_outBool3 (const ULong3 &x, const ULong3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThan", "bool3 LessThan (const ulong3 x, const ulong3 y) {\n\treturn lessThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inULong_inULong3_outBool3 (const ULong &x, const ULong3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThan", "bool3 LessThan (const ulong x, const ulong3 y) {\n\treturn lessThan( ulong3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inULong3_inULong_outBool3 (const ULong3 &x, const ULong &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThan", "bool3 LessThan (const ulong3 x, const ulong y) {\n\treturn lessThan( (x), ulong3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inULong4_inULong4_outBool4 (const ULong4 &x, const ULong4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThan", "bool4 LessThan (const ulong4 x, const ulong4 y) {\n\treturn lessThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inULong_inULong4_outBool4 (const ULong &x, const ULong4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThan", "bool4 LessThan (const ulong x, const ulong4 y) {\n\treturn lessThan( ulong4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inULong4_inULong_outBool4 (const ULong4 &x, const ULong &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThan", "bool4 LessThan (const ulong4 x, const ulong y) {\n\treturn lessThan( (x), ulong4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inULong_inULong_outBool (const ULong &x, const ULong &y, OUT Bool &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool GreaterThanEqual (const ulong x, const ulong y) {\n\treturn (x) >= (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inULong2_inULong2_outBool2 (const ULong2 &x, const ULong2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool2 GreaterThanEqual (const ulong2 x, const ulong2 y) {\n\treturn greaterThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inULong_inULong2_outBool2 (const ULong &x, const ULong2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool2 GreaterThanEqual (const ulong x, const ulong2 y) {\n\treturn greaterThanEqual( ulong2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inULong2_inULong_outBool2 (const ULong2 &x, const ULong &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool2 GreaterThanEqual (const ulong2 x, const ulong y) {\n\treturn greaterThanEqual( (x), ulong2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inULong3_inULong3_outBool3 (const ULong3 &x, const ULong3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool3 GreaterThanEqual (const ulong3 x, const ulong3 y) {\n\treturn greaterThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inULong_inULong3_outBool3 (const ULong &x, const ULong3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool3 GreaterThanEqual (const ulong x, const ulong3 y) {\n\treturn greaterThanEqual( ulong3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inULong3_inULong_outBool3 (const ULong3 &x, const ULong &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool3 GreaterThanEqual (const ulong3 x, const ulong y) {\n\treturn greaterThanEqual( (x), ulong3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inULong4_inULong4_outBool4 (const ULong4 &x, const ULong4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool4 GreaterThanEqual (const ulong4 x, const ulong4 y) {\n\treturn greaterThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inULong_inULong4_outBool4 (const ULong &x, const ULong4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool4 GreaterThanEqual (const ulong x, const ulong4 y) {\n\treturn greaterThanEqual( ulong4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inULong4_inULong_outBool4 (const ULong4 &x, const ULong &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool4 GreaterThanEqual (const ulong4 x, const ulong y) {\n\treturn greaterThanEqual( (x), ulong4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inULong_inULong_outBool (const ULong &x, const ULong &y, OUT Bool &out)
	{
		FunctionNode	func( "LessThanEqual", "bool LessThanEqual (const ulong x, const ulong y) {\n\treturn (x) <= (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inULong2_inULong2_outBool2 (const ULong2 &x, const ULong2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool2 LessThanEqual (const ulong2 x, const ulong2 y) {\n\treturn lessThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inULong_inULong2_outBool2 (const ULong &x, const ULong2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool2 LessThanEqual (const ulong x, const ulong2 y) {\n\treturn lessThanEqual( ulong2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inULong2_inULong_outBool2 (const ULong2 &x, const ULong &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool2 LessThanEqual (const ulong2 x, const ulong y) {\n\treturn lessThanEqual( (x), ulong2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inULong3_inULong3_outBool3 (const ULong3 &x, const ULong3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool3 LessThanEqual (const ulong3 x, const ulong3 y) {\n\treturn lessThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inULong_inULong3_outBool3 (const ULong &x, const ULong3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool3 LessThanEqual (const ulong x, const ulong3 y) {\n\treturn lessThanEqual( ulong3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inULong3_inULong_outBool3 (const ULong3 &x, const ULong &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool3 LessThanEqual (const ulong3 x, const ulong y) {\n\treturn lessThanEqual( (x), ulong3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inULong4_inULong4_outBool4 (const ULong4 &x, const ULong4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool4 LessThanEqual (const ulong4 x, const ulong4 y) {\n\treturn lessThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inULong_inULong4_outBool4 (const ULong &x, const ULong4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool4 LessThanEqual (const ulong x, const ulong4 y) {\n\treturn lessThanEqual( ulong4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inULong4_inULong_outBool4 (const ULong4 &x, const ULong &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool4 LessThanEqual (const ulong4 x, const ulong y) {\n\treturn lessThanEqual( (x), ulong4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Plus_inFloat_outFloat (const Float &x, OUT Float &out)
	{
		FunctionNode	func( "Plus", "float Plus (const float x) {\n\treturn +(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Plus_inFloat2_outFloat2 (const Float2 &x, OUT Float2 &out)
	{
		FunctionNode	func( "Plus", "float2 Plus (const float2 x) {\n\treturn +(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Plus_inFloat3_outFloat3 (const Float3 &x, OUT Float3 &out)
	{
		FunctionNode	func( "Plus", "float3 Plus (const float3 x) {\n\treturn +(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Plus_inFloat4_outFloat4 (const Float4 &x, OUT Float4 &out)
	{
		FunctionNode	func( "Plus", "float4 Plus (const float4 x) {\n\treturn +(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Minus_inFloat_outFloat (const Float &x, OUT Float &out)
	{
		FunctionNode	func( "Minus", "float Minus (const float x) {\n\treturn -(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Minus_inFloat2_outFloat2 (const Float2 &x, OUT Float2 &out)
	{
		FunctionNode	func( "Minus", "float2 Minus (const float2 x) {\n\treturn -(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Minus_inFloat3_outFloat3 (const Float3 &x, OUT Float3 &out)
	{
		FunctionNode	func( "Minus", "float3 Minus (const float3 x) {\n\treturn -(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Minus_inFloat4_outFloat4 (const Float4 &x, OUT Float4 &out)
	{
		FunctionNode	func( "Minus", "float4 Minus (const float4 x) {\n\treturn -(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inFloat_inFloat_outFloat (const Float &x, const Float &y, OUT Float &out)
	{
		FunctionNode	func( "Add", "float Add (const float x, const float y) {\n\treturn (x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inFloat2_inFloat2_outFloat2 (const Float2 &x, const Float2 &y, OUT Float2 &out)
	{
		FunctionNode	func( "Add", "float2 Add (const float2 x, const float2 y) {\n\treturn (x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inFloat_inFloat2_outFloat2 (const Float &x, const Float2 &y, OUT Float2 &out)
	{
		FunctionNode	func( "Add", "float2 Add (const float x, const float2 y) {\n\treturn float2(x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inFloat2_inFloat_outFloat2 (const Float2 &x, const Float &y, OUT Float2 &out)
	{
		FunctionNode	func( "Add", "float2 Add (const float2 x, const float y) {\n\treturn (x) + float2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inFloat3_inFloat3_outFloat3 (const Float3 &x, const Float3 &y, OUT Float3 &out)
	{
		FunctionNode	func( "Add", "float3 Add (const float3 x, const float3 y) {\n\treturn (x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inFloat_inFloat3_outFloat3 (const Float &x, const Float3 &y, OUT Float3 &out)
	{
		FunctionNode	func( "Add", "float3 Add (const float x, const float3 y) {\n\treturn float3(x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inFloat3_inFloat_outFloat3 (const Float3 &x, const Float &y, OUT Float3 &out)
	{
		FunctionNode	func( "Add", "float3 Add (const float3 x, const float y) {\n\treturn (x) + float3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inFloat4_inFloat4_outFloat4 (const Float4 &x, const Float4 &y, OUT Float4 &out)
	{
		FunctionNode	func( "Add", "float4 Add (const float4 x, const float4 y) {\n\treturn (x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inFloat_inFloat4_outFloat4 (const Float &x, const Float4 &y, OUT Float4 &out)
	{
		FunctionNode	func( "Add", "float4 Add (const float x, const float4 y) {\n\treturn float4(x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inFloat4_inFloat_outFloat4 (const Float4 &x, const Float &y, OUT Float4 &out)
	{
		FunctionNode	func( "Add", "float4 Add (const float4 x, const float y) {\n\treturn (x) + float4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inFloat_inFloat_outFloat (const Float &x, const Float &y, OUT Float &out)
	{
		FunctionNode	func( "Sub", "float Sub (const float x, const float y) {\n\treturn (x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inFloat2_inFloat2_outFloat2 (const Float2 &x, const Float2 &y, OUT Float2 &out)
	{
		FunctionNode	func( "Sub", "float2 Sub (const float2 x, const float2 y) {\n\treturn (x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inFloat_inFloat2_outFloat2 (const Float &x, const Float2 &y, OUT Float2 &out)
	{
		FunctionNode	func( "Sub", "float2 Sub (const float x, const float2 y) {\n\treturn float2(x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inFloat2_inFloat_outFloat2 (const Float2 &x, const Float &y, OUT Float2 &out)
	{
		FunctionNode	func( "Sub", "float2 Sub (const float2 x, const float y) {\n\treturn (x) - float2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inFloat3_inFloat3_outFloat3 (const Float3 &x, const Float3 &y, OUT Float3 &out)
	{
		FunctionNode	func( "Sub", "float3 Sub (const float3 x, const float3 y) {\n\treturn (x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inFloat_inFloat3_outFloat3 (const Float &x, const Float3 &y, OUT Float3 &out)
	{
		FunctionNode	func( "Sub", "float3 Sub (const float x, const float3 y) {\n\treturn float3(x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inFloat3_inFloat_outFloat3 (const Float3 &x, const Float &y, OUT Float3 &out)
	{
		FunctionNode	func( "Sub", "float3 Sub (const float3 x, const float y) {\n\treturn (x) - float3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inFloat4_inFloat4_outFloat4 (const Float4 &x, const Float4 &y, OUT Float4 &out)
	{
		FunctionNode	func( "Sub", "float4 Sub (const float4 x, const float4 y) {\n\treturn (x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inFloat_inFloat4_outFloat4 (const Float &x, const Float4 &y, OUT Float4 &out)
	{
		FunctionNode	func( "Sub", "float4 Sub (const float x, const float4 y) {\n\treturn float4(x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inFloat4_inFloat_outFloat4 (const Float4 &x, const Float &y, OUT Float4 &out)
	{
		FunctionNode	func( "Sub", "float4 Sub (const float4 x, const float y) {\n\treturn (x) - float4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inFloat_inFloat_outFloat (const Float &x, const Float &y, OUT Float &out)
	{
		FunctionNode	func( "Mul", "float Mul (const float x, const float y) {\n\treturn (x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inFloat2_inFloat2_outFloat2 (const Float2 &x, const Float2 &y, OUT Float2 &out)
	{
		FunctionNode	func( "Mul", "float2 Mul (const float2 x, const float2 y) {\n\treturn (x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inFloat_inFloat2_outFloat2 (const Float &x, const Float2 &y, OUT Float2 &out)
	{
		FunctionNode	func( "Mul", "float2 Mul (const float x, const float2 y) {\n\treturn float2(x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inFloat2_inFloat_outFloat2 (const Float2 &x, const Float &y, OUT Float2 &out)
	{
		FunctionNode	func( "Mul", "float2 Mul (const float2 x, const float y) {\n\treturn (x) * float2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inFloat3_inFloat3_outFloat3 (const Float3 &x, const Float3 &y, OUT Float3 &out)
	{
		FunctionNode	func( "Mul", "float3 Mul (const float3 x, const float3 y) {\n\treturn (x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inFloat_inFloat3_outFloat3 (const Float &x, const Float3 &y, OUT Float3 &out)
	{
		FunctionNode	func( "Mul", "float3 Mul (const float x, const float3 y) {\n\treturn float3(x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inFloat3_inFloat_outFloat3 (const Float3 &x, const Float &y, OUT Float3 &out)
	{
		FunctionNode	func( "Mul", "float3 Mul (const float3 x, const float y) {\n\treturn (x) * float3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inFloat4_inFloat4_outFloat4 (const Float4 &x, const Float4 &y, OUT Float4 &out)
	{
		FunctionNode	func( "Mul", "float4 Mul (const float4 x, const float4 y) {\n\treturn (x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inFloat_inFloat4_outFloat4 (const Float &x, const Float4 &y, OUT Float4 &out)
	{
		FunctionNode	func( "Mul", "float4 Mul (const float x, const float4 y) {\n\treturn float4(x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inFloat4_inFloat_outFloat4 (const Float4 &x, const Float &y, OUT Float4 &out)
	{
		FunctionNode	func( "Mul", "float4 Mul (const float4 x, const float y) {\n\treturn (x) * float4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inFloat_inFloat_outFloat (const Float &x, const Float &y, OUT Float &out)
	{
		FunctionNode	func( "Div", "float Div (const float x, const float y) {\n\treturn (x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inFloat2_inFloat2_outFloat2 (const Float2 &x, const Float2 &y, OUT Float2 &out)
	{
		FunctionNode	func( "Div", "float2 Div (const float2 x, const float2 y) {\n\treturn (x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inFloat_inFloat2_outFloat2 (const Float &x, const Float2 &y, OUT Float2 &out)
	{
		FunctionNode	func( "Div", "float2 Div (const float x, const float2 y) {\n\treturn float2(x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inFloat2_inFloat_outFloat2 (const Float2 &x, const Float &y, OUT Float2 &out)
	{
		FunctionNode	func( "Div", "float2 Div (const float2 x, const float y) {\n\treturn (x) / float2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inFloat3_inFloat3_outFloat3 (const Float3 &x, const Float3 &y, OUT Float3 &out)
	{
		FunctionNode	func( "Div", "float3 Div (const float3 x, const float3 y) {\n\treturn (x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inFloat_inFloat3_outFloat3 (const Float &x, const Float3 &y, OUT Float3 &out)
	{
		FunctionNode	func( "Div", "float3 Div (const float x, const float3 y) {\n\treturn float3(x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inFloat3_inFloat_outFloat3 (const Float3 &x, const Float &y, OUT Float3 &out)
	{
		FunctionNode	func( "Div", "float3 Div (const float3 x, const float y) {\n\treturn (x) / float3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inFloat4_inFloat4_outFloat4 (const Float4 &x, const Float4 &y, OUT Float4 &out)
	{
		FunctionNode	func( "Div", "float4 Div (const float4 x, const float4 y) {\n\treturn (x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inFloat_inFloat4_outFloat4 (const Float &x, const Float4 &y, OUT Float4 &out)
	{
		FunctionNode	func( "Div", "float4 Div (const float x, const float4 y) {\n\treturn float4(x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inFloat4_inFloat_outFloat4 (const Float4 &x, const Float &y, OUT Float4 &out)
	{
		FunctionNode	func( "Div", "float4 Div (const float4 x, const float y) {\n\treturn (x) / float4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inFloat_inFloat_outFloat (const Float &x, const Float &y, OUT Float &out)
	{
		FunctionNode	func( "Mod", "float Mod (const float x, const float y) {\n\treturn mod( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inFloat2_inFloat2_outFloat2 (const Float2 &x, const Float2 &y, OUT Float2 &out)
	{
		FunctionNode	func( "Mod", "float2 Mod (const float2 x, const float2 y) {\n\treturn mod( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inFloat_inFloat2_outFloat2 (const Float &x, const Float2 &y, OUT Float2 &out)
	{
		FunctionNode	func( "Mod", "float2 Mod (const float x, const float2 y) {\n\treturn mod( float2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inFloat2_inFloat_outFloat2 (const Float2 &x, const Float &y, OUT Float2 &out)
	{
		FunctionNode	func( "Mod", "float2 Mod (const float2 x, const float y) {\n\treturn mod( (x), float2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inFloat3_inFloat3_outFloat3 (const Float3 &x, const Float3 &y, OUT Float3 &out)
	{
		FunctionNode	func( "Mod", "float3 Mod (const float3 x, const float3 y) {\n\treturn mod( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inFloat_inFloat3_outFloat3 (const Float &x, const Float3 &y, OUT Float3 &out)
	{
		FunctionNode	func( "Mod", "float3 Mod (const float x, const float3 y) {\n\treturn mod( float3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inFloat3_inFloat_outFloat3 (const Float3 &x, const Float &y, OUT Float3 &out)
	{
		FunctionNode	func( "Mod", "float3 Mod (const float3 x, const float y) {\n\treturn mod( (x), float3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inFloat4_inFloat4_outFloat4 (const Float4 &x, const Float4 &y, OUT Float4 &out)
	{
		FunctionNode	func( "Mod", "float4 Mod (const float4 x, const float4 y) {\n\treturn mod( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inFloat_inFloat4_outFloat4 (const Float &x, const Float4 &y, OUT Float4 &out)
	{
		FunctionNode	func( "Mod", "float4 Mod (const float x, const float4 y) {\n\treturn mod( float4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inFloat4_inFloat_outFloat4 (const Float4 &x, const Float &y, OUT Float4 &out)
	{
		FunctionNode	func( "Mod", "float4 Mod (const float4 x, const float y) {\n\treturn mod( (x), float4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inFloat_inFloat_outBool (const Float &x, const Float &y, OUT Bool &out)
	{
		FunctionNode	func( "Equal", "bool Equal (const float x, const float y) {\n\treturn (x) == (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inFloat2_inFloat2_outBool2 (const Float2 &x, const Float2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "Equal", "bool2 Equal (const float2 x, const float2 y) {\n\treturn equal( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inFloat_inFloat2_outBool2 (const Float &x, const Float2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "Equal", "bool2 Equal (const float x, const float2 y) {\n\treturn equal( float2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inFloat2_inFloat_outBool2 (const Float2 &x, const Float &y, OUT Bool2 &out)
	{
		FunctionNode	func( "Equal", "bool2 Equal (const float2 x, const float y) {\n\treturn equal( (x), float2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inFloat3_inFloat3_outBool3 (const Float3 &x, const Float3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "Equal", "bool3 Equal (const float3 x, const float3 y) {\n\treturn equal( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inFloat_inFloat3_outBool3 (const Float &x, const Float3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "Equal", "bool3 Equal (const float x, const float3 y) {\n\treturn equal( float3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inFloat3_inFloat_outBool3 (const Float3 &x, const Float &y, OUT Bool3 &out)
	{
		FunctionNode	func( "Equal", "bool3 Equal (const float3 x, const float y) {\n\treturn equal( (x), float3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inFloat4_inFloat4_outBool4 (const Float4 &x, const Float4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "Equal", "bool4 Equal (const float4 x, const float4 y) {\n\treturn equal( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inFloat_inFloat4_outBool4 (const Float &x, const Float4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "Equal", "bool4 Equal (const float x, const float4 y) {\n\treturn equal( float4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inFloat4_inFloat_outBool4 (const Float4 &x, const Float &y, OUT Bool4 &out)
	{
		FunctionNode	func( "Equal", "bool4 Equal (const float4 x, const float y) {\n\treturn equal( (x), float4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inFloat_inFloat_outBool (const Float &x, const Float &y, OUT Bool &out)
	{
		FunctionNode	func( "NotEqual", "bool NotEqual (const float x, const float y) {\n\treturn (x) != (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inFloat2_inFloat2_outBool2 (const Float2 &x, const Float2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "NotEqual", "bool2 NotEqual (const float2 x, const float2 y) {\n\treturn notEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inFloat_inFloat2_outBool2 (const Float &x, const Float2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "NotEqual", "bool2 NotEqual (const float x, const float2 y) {\n\treturn notEqual( float2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inFloat2_inFloat_outBool2 (const Float2 &x, const Float &y, OUT Bool2 &out)
	{
		FunctionNode	func( "NotEqual", "bool2 NotEqual (const float2 x, const float y) {\n\treturn notEqual( (x), float2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inFloat3_inFloat3_outBool3 (const Float3 &x, const Float3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "NotEqual", "bool3 NotEqual (const float3 x, const float3 y) {\n\treturn notEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inFloat_inFloat3_outBool3 (const Float &x, const Float3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "NotEqual", "bool3 NotEqual (const float x, const float3 y) {\n\treturn notEqual( float3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inFloat3_inFloat_outBool3 (const Float3 &x, const Float &y, OUT Bool3 &out)
	{
		FunctionNode	func( "NotEqual", "bool3 NotEqual (const float3 x, const float y) {\n\treturn notEqual( (x), float3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inFloat4_inFloat4_outBool4 (const Float4 &x, const Float4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "NotEqual", "bool4 NotEqual (const float4 x, const float4 y) {\n\treturn notEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inFloat_inFloat4_outBool4 (const Float &x, const Float4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "NotEqual", "bool4 NotEqual (const float x, const float4 y) {\n\treturn notEqual( float4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inFloat4_inFloat_outBool4 (const Float4 &x, const Float &y, OUT Bool4 &out)
	{
		FunctionNode	func( "NotEqual", "bool4 NotEqual (const float4 x, const float y) {\n\treturn notEqual( (x), float4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inFloat_inFloat_outBool (const Float &x, const Float &y, OUT Bool &out)
	{
		FunctionNode	func( "GreaterThan", "bool GreaterThan (const float x, const float y) {\n\treturn (x) > (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inFloat2_inFloat2_outBool2 (const Float2 &x, const Float2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThan", "bool2 GreaterThan (const float2 x, const float2 y) {\n\treturn greaterThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inFloat_inFloat2_outBool2 (const Float &x, const Float2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThan", "bool2 GreaterThan (const float x, const float2 y) {\n\treturn greaterThan( float2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inFloat2_inFloat_outBool2 (const Float2 &x, const Float &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThan", "bool2 GreaterThan (const float2 x, const float y) {\n\treturn greaterThan( (x), float2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inFloat3_inFloat3_outBool3 (const Float3 &x, const Float3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThan", "bool3 GreaterThan (const float3 x, const float3 y) {\n\treturn greaterThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inFloat_inFloat3_outBool3 (const Float &x, const Float3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThan", "bool3 GreaterThan (const float x, const float3 y) {\n\treturn greaterThan( float3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inFloat3_inFloat_outBool3 (const Float3 &x, const Float &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThan", "bool3 GreaterThan (const float3 x, const float y) {\n\treturn greaterThan( (x), float3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inFloat4_inFloat4_outBool4 (const Float4 &x, const Float4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThan", "bool4 GreaterThan (const float4 x, const float4 y) {\n\treturn greaterThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inFloat_inFloat4_outBool4 (const Float &x, const Float4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThan", "bool4 GreaterThan (const float x, const float4 y) {\n\treturn greaterThan( float4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inFloat4_inFloat_outBool4 (const Float4 &x, const Float &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThan", "bool4 GreaterThan (const float4 x, const float y) {\n\treturn greaterThan( (x), float4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inFloat_inFloat_outBool (const Float &x, const Float &y, OUT Bool &out)
	{
		FunctionNode	func( "LessThan", "bool LessThan (const float x, const float y) {\n\treturn (x) < (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inFloat2_inFloat2_outBool2 (const Float2 &x, const Float2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThan", "bool2 LessThan (const float2 x, const float2 y) {\n\treturn lessThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inFloat_inFloat2_outBool2 (const Float &x, const Float2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThan", "bool2 LessThan (const float x, const float2 y) {\n\treturn lessThan( float2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inFloat2_inFloat_outBool2 (const Float2 &x, const Float &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThan", "bool2 LessThan (const float2 x, const float y) {\n\treturn lessThan( (x), float2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inFloat3_inFloat3_outBool3 (const Float3 &x, const Float3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThan", "bool3 LessThan (const float3 x, const float3 y) {\n\treturn lessThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inFloat_inFloat3_outBool3 (const Float &x, const Float3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThan", "bool3 LessThan (const float x, const float3 y) {\n\treturn lessThan( float3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inFloat3_inFloat_outBool3 (const Float3 &x, const Float &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThan", "bool3 LessThan (const float3 x, const float y) {\n\treturn lessThan( (x), float3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inFloat4_inFloat4_outBool4 (const Float4 &x, const Float4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThan", "bool4 LessThan (const float4 x, const float4 y) {\n\treturn lessThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inFloat_inFloat4_outBool4 (const Float &x, const Float4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThan", "bool4 LessThan (const float x, const float4 y) {\n\treturn lessThan( float4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inFloat4_inFloat_outBool4 (const Float4 &x, const Float &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThan", "bool4 LessThan (const float4 x, const float y) {\n\treturn lessThan( (x), float4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inFloat_inFloat_outBool (const Float &x, const Float &y, OUT Bool &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool GreaterThanEqual (const float x, const float y) {\n\treturn (x) >= (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inFloat2_inFloat2_outBool2 (const Float2 &x, const Float2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool2 GreaterThanEqual (const float2 x, const float2 y) {\n\treturn greaterThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inFloat_inFloat2_outBool2 (const Float &x, const Float2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool2 GreaterThanEqual (const float x, const float2 y) {\n\treturn greaterThanEqual( float2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inFloat2_inFloat_outBool2 (const Float2 &x, const Float &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool2 GreaterThanEqual (const float2 x, const float y) {\n\treturn greaterThanEqual( (x), float2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inFloat3_inFloat3_outBool3 (const Float3 &x, const Float3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool3 GreaterThanEqual (const float3 x, const float3 y) {\n\treturn greaterThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inFloat_inFloat3_outBool3 (const Float &x, const Float3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool3 GreaterThanEqual (const float x, const float3 y) {\n\treturn greaterThanEqual( float3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inFloat3_inFloat_outBool3 (const Float3 &x, const Float &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool3 GreaterThanEqual (const float3 x, const float y) {\n\treturn greaterThanEqual( (x), float3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inFloat4_inFloat4_outBool4 (const Float4 &x, const Float4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool4 GreaterThanEqual (const float4 x, const float4 y) {\n\treturn greaterThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inFloat_inFloat4_outBool4 (const Float &x, const Float4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool4 GreaterThanEqual (const float x, const float4 y) {\n\treturn greaterThanEqual( float4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inFloat4_inFloat_outBool4 (const Float4 &x, const Float &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool4 GreaterThanEqual (const float4 x, const float y) {\n\treturn greaterThanEqual( (x), float4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inFloat_inFloat_outBool (const Float &x, const Float &y, OUT Bool &out)
	{
		FunctionNode	func( "LessThanEqual", "bool LessThanEqual (const float x, const float y) {\n\treturn (x) <= (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inFloat2_inFloat2_outBool2 (const Float2 &x, const Float2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool2 LessThanEqual (const float2 x, const float2 y) {\n\treturn lessThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inFloat_inFloat2_outBool2 (const Float &x, const Float2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool2 LessThanEqual (const float x, const float2 y) {\n\treturn lessThanEqual( float2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inFloat2_inFloat_outBool2 (const Float2 &x, const Float &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool2 LessThanEqual (const float2 x, const float y) {\n\treturn lessThanEqual( (x), float2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inFloat3_inFloat3_outBool3 (const Float3 &x, const Float3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool3 LessThanEqual (const float3 x, const float3 y) {\n\treturn lessThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inFloat_inFloat3_outBool3 (const Float &x, const Float3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool3 LessThanEqual (const float x, const float3 y) {\n\treturn lessThanEqual( float3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inFloat3_inFloat_outBool3 (const Float3 &x, const Float &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool3 LessThanEqual (const float3 x, const float y) {\n\treturn lessThanEqual( (x), float3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inFloat4_inFloat4_outBool4 (const Float4 &x, const Float4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool4 LessThanEqual (const float4 x, const float4 y) {\n\treturn lessThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inFloat_inFloat4_outBool4 (const Float &x, const Float4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool4 LessThanEqual (const float x, const float4 y) {\n\treturn lessThanEqual( float4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inFloat4_inFloat_outBool4 (const Float4 &x, const Float &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool4 LessThanEqual (const float4 x, const float y) {\n\treturn lessThanEqual( (x), float4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Plus_inDouble_outDouble (const Double &x, OUT Double &out)
	{
		FunctionNode	func( "Plus", "double Plus (const double x) {\n\treturn +(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Plus_inDouble2_outDouble2 (const Double2 &x, OUT Double2 &out)
	{
		FunctionNode	func( "Plus", "double2 Plus (const double2 x) {\n\treturn +(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Plus_inDouble3_outDouble3 (const Double3 &x, OUT Double3 &out)
	{
		FunctionNode	func( "Plus", "double3 Plus (const double3 x) {\n\treturn +(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Plus_inDouble4_outDouble4 (const Double4 &x, OUT Double4 &out)
	{
		FunctionNode	func( "Plus", "double4 Plus (const double4 x) {\n\treturn +(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Minus_inDouble_outDouble (const Double &x, OUT Double &out)
	{
		FunctionNode	func( "Minus", "double Minus (const double x) {\n\treturn -(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Minus_inDouble2_outDouble2 (const Double2 &x, OUT Double2 &out)
	{
		FunctionNode	func( "Minus", "double2 Minus (const double2 x) {\n\treturn -(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Minus_inDouble3_outDouble3 (const Double3 &x, OUT Double3 &out)
	{
		FunctionNode	func( "Minus", "double3 Minus (const double3 x) {\n\treturn -(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Minus_inDouble4_outDouble4 (const Double4 &x, OUT Double4 &out)
	{
		FunctionNode	func( "Minus", "double4 Minus (const double4 x) {\n\treturn -(x);\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inDouble_inDouble_outDouble (const Double &x, const Double &y, OUT Double &out)
	{
		FunctionNode	func( "Add", "double Add (const double x, const double y) {\n\treturn (x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inDouble2_inDouble2_outDouble2 (const Double2 &x, const Double2 &y, OUT Double2 &out)
	{
		FunctionNode	func( "Add", "double2 Add (const double2 x, const double2 y) {\n\treturn (x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inDouble_inDouble2_outDouble2 (const Double &x, const Double2 &y, OUT Double2 &out)
	{
		FunctionNode	func( "Add", "double2 Add (const double x, const double2 y) {\n\treturn double2(x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inDouble2_inDouble_outDouble2 (const Double2 &x, const Double &y, OUT Double2 &out)
	{
		FunctionNode	func( "Add", "double2 Add (const double2 x, const double y) {\n\treturn (x) + double2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inDouble3_inDouble3_outDouble3 (const Double3 &x, const Double3 &y, OUT Double3 &out)
	{
		FunctionNode	func( "Add", "double3 Add (const double3 x, const double3 y) {\n\treturn (x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inDouble_inDouble3_outDouble3 (const Double &x, const Double3 &y, OUT Double3 &out)
	{
		FunctionNode	func( "Add", "double3 Add (const double x, const double3 y) {\n\treturn double3(x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inDouble3_inDouble_outDouble3 (const Double3 &x, const Double &y, OUT Double3 &out)
	{
		FunctionNode	func( "Add", "double3 Add (const double3 x, const double y) {\n\treturn (x) + double3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inDouble4_inDouble4_outDouble4 (const Double4 &x, const Double4 &y, OUT Double4 &out)
	{
		FunctionNode	func( "Add", "double4 Add (const double4 x, const double4 y) {\n\treturn (x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inDouble_inDouble4_outDouble4 (const Double &x, const Double4 &y, OUT Double4 &out)
	{
		FunctionNode	func( "Add", "double4 Add (const double x, const double4 y) {\n\treturn double4(x) + (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Add_inDouble4_inDouble_outDouble4 (const Double4 &x, const Double &y, OUT Double4 &out)
	{
		FunctionNode	func( "Add", "double4 Add (const double4 x, const double y) {\n\treturn (x) + double4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inDouble_inDouble_outDouble (const Double &x, const Double &y, OUT Double &out)
	{
		FunctionNode	func( "Sub", "double Sub (const double x, const double y) {\n\treturn (x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inDouble2_inDouble2_outDouble2 (const Double2 &x, const Double2 &y, OUT Double2 &out)
	{
		FunctionNode	func( "Sub", "double2 Sub (const double2 x, const double2 y) {\n\treturn (x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inDouble_inDouble2_outDouble2 (const Double &x, const Double2 &y, OUT Double2 &out)
	{
		FunctionNode	func( "Sub", "double2 Sub (const double x, const double2 y) {\n\treturn double2(x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inDouble2_inDouble_outDouble2 (const Double2 &x, const Double &y, OUT Double2 &out)
	{
		FunctionNode	func( "Sub", "double2 Sub (const double2 x, const double y) {\n\treturn (x) - double2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inDouble3_inDouble3_outDouble3 (const Double3 &x, const Double3 &y, OUT Double3 &out)
	{
		FunctionNode	func( "Sub", "double3 Sub (const double3 x, const double3 y) {\n\treturn (x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inDouble_inDouble3_outDouble3 (const Double &x, const Double3 &y, OUT Double3 &out)
	{
		FunctionNode	func( "Sub", "double3 Sub (const double x, const double3 y) {\n\treturn double3(x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inDouble3_inDouble_outDouble3 (const Double3 &x, const Double &y, OUT Double3 &out)
	{
		FunctionNode	func( "Sub", "double3 Sub (const double3 x, const double y) {\n\treturn (x) - double3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inDouble4_inDouble4_outDouble4 (const Double4 &x, const Double4 &y, OUT Double4 &out)
	{
		FunctionNode	func( "Sub", "double4 Sub (const double4 x, const double4 y) {\n\treturn (x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inDouble_inDouble4_outDouble4 (const Double &x, const Double4 &y, OUT Double4 &out)
	{
		FunctionNode	func( "Sub", "double4 Sub (const double x, const double4 y) {\n\treturn double4(x) - (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Sub_inDouble4_inDouble_outDouble4 (const Double4 &x, const Double &y, OUT Double4 &out)
	{
		FunctionNode	func( "Sub", "double4 Sub (const double4 x, const double y) {\n\treturn (x) - double4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inDouble_inDouble_outDouble (const Double &x, const Double &y, OUT Double &out)
	{
		FunctionNode	func( "Mul", "double Mul (const double x, const double y) {\n\treturn (x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inDouble2_inDouble2_outDouble2 (const Double2 &x, const Double2 &y, OUT Double2 &out)
	{
		FunctionNode	func( "Mul", "double2 Mul (const double2 x, const double2 y) {\n\treturn (x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inDouble_inDouble2_outDouble2 (const Double &x, const Double2 &y, OUT Double2 &out)
	{
		FunctionNode	func( "Mul", "double2 Mul (const double x, const double2 y) {\n\treturn double2(x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inDouble2_inDouble_outDouble2 (const Double2 &x, const Double &y, OUT Double2 &out)
	{
		FunctionNode	func( "Mul", "double2 Mul (const double2 x, const double y) {\n\treturn (x) * double2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inDouble3_inDouble3_outDouble3 (const Double3 &x, const Double3 &y, OUT Double3 &out)
	{
		FunctionNode	func( "Mul", "double3 Mul (const double3 x, const double3 y) {\n\treturn (x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inDouble_inDouble3_outDouble3 (const Double &x, const Double3 &y, OUT Double3 &out)
	{
		FunctionNode	func( "Mul", "double3 Mul (const double x, const double3 y) {\n\treturn double3(x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inDouble3_inDouble_outDouble3 (const Double3 &x, const Double &y, OUT Double3 &out)
	{
		FunctionNode	func( "Mul", "double3 Mul (const double3 x, const double y) {\n\treturn (x) * double3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inDouble4_inDouble4_outDouble4 (const Double4 &x, const Double4 &y, OUT Double4 &out)
	{
		FunctionNode	func( "Mul", "double4 Mul (const double4 x, const double4 y) {\n\treturn (x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inDouble_inDouble4_outDouble4 (const Double &x, const Double4 &y, OUT Double4 &out)
	{
		FunctionNode	func( "Mul", "double4 Mul (const double x, const double4 y) {\n\treturn double4(x) * (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mul_inDouble4_inDouble_outDouble4 (const Double4 &x, const Double &y, OUT Double4 &out)
	{
		FunctionNode	func( "Mul", "double4 Mul (const double4 x, const double y) {\n\treturn (x) * double4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inDouble_inDouble_outDouble (const Double &x, const Double &y, OUT Double &out)
	{
		FunctionNode	func( "Div", "double Div (const double x, const double y) {\n\treturn (x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inDouble2_inDouble2_outDouble2 (const Double2 &x, const Double2 &y, OUT Double2 &out)
	{
		FunctionNode	func( "Div", "double2 Div (const double2 x, const double2 y) {\n\treturn (x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inDouble_inDouble2_outDouble2 (const Double &x, const Double2 &y, OUT Double2 &out)
	{
		FunctionNode	func( "Div", "double2 Div (const double x, const double2 y) {\n\treturn double2(x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inDouble2_inDouble_outDouble2 (const Double2 &x, const Double &y, OUT Double2 &out)
	{
		FunctionNode	func( "Div", "double2 Div (const double2 x, const double y) {\n\treturn (x) / double2(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inDouble3_inDouble3_outDouble3 (const Double3 &x, const Double3 &y, OUT Double3 &out)
	{
		FunctionNode	func( "Div", "double3 Div (const double3 x, const double3 y) {\n\treturn (x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inDouble_inDouble3_outDouble3 (const Double &x, const Double3 &y, OUT Double3 &out)
	{
		FunctionNode	func( "Div", "double3 Div (const double x, const double3 y) {\n\treturn double3(x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inDouble3_inDouble_outDouble3 (const Double3 &x, const Double &y, OUT Double3 &out)
	{
		FunctionNode	func( "Div", "double3 Div (const double3 x, const double y) {\n\treturn (x) / double3(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inDouble4_inDouble4_outDouble4 (const Double4 &x, const Double4 &y, OUT Double4 &out)
	{
		FunctionNode	func( "Div", "double4 Div (const double4 x, const double4 y) {\n\treturn (x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inDouble_inDouble4_outDouble4 (const Double &x, const Double4 &y, OUT Double4 &out)
	{
		FunctionNode	func( "Div", "double4 Div (const double x, const double4 y) {\n\treturn double4(x) / (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Div_inDouble4_inDouble_outDouble4 (const Double4 &x, const Double &y, OUT Double4 &out)
	{
		FunctionNode	func( "Div", "double4 Div (const double4 x, const double y) {\n\treturn (x) / double4(y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inDouble_inDouble_outDouble (const Double &x, const Double &y, OUT Double &out)
	{
		FunctionNode	func( "Mod", "double Mod (const double x, const double y) {\n\treturn mod( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inDouble2_inDouble2_outDouble2 (const Double2 &x, const Double2 &y, OUT Double2 &out)
	{
		FunctionNode	func( "Mod", "double2 Mod (const double2 x, const double2 y) {\n\treturn mod( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inDouble_inDouble2_outDouble2 (const Double &x, const Double2 &y, OUT Double2 &out)
	{
		FunctionNode	func( "Mod", "double2 Mod (const double x, const double2 y) {\n\treturn mod( double2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inDouble2_inDouble_outDouble2 (const Double2 &x, const Double &y, OUT Double2 &out)
	{
		FunctionNode	func( "Mod", "double2 Mod (const double2 x, const double y) {\n\treturn mod( (x), double2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inDouble3_inDouble3_outDouble3 (const Double3 &x, const Double3 &y, OUT Double3 &out)
	{
		FunctionNode	func( "Mod", "double3 Mod (const double3 x, const double3 y) {\n\treturn mod( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inDouble_inDouble3_outDouble3 (const Double &x, const Double3 &y, OUT Double3 &out)
	{
		FunctionNode	func( "Mod", "double3 Mod (const double x, const double3 y) {\n\treturn mod( double3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inDouble3_inDouble_outDouble3 (const Double3 &x, const Double &y, OUT Double3 &out)
	{
		FunctionNode	func( "Mod", "double3 Mod (const double3 x, const double y) {\n\treturn mod( (x), double3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inDouble4_inDouble4_outDouble4 (const Double4 &x, const Double4 &y, OUT Double4 &out)
	{
		FunctionNode	func( "Mod", "double4 Mod (const double4 x, const double4 y) {\n\treturn mod( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inDouble_inDouble4_outDouble4 (const Double &x, const Double4 &y, OUT Double4 &out)
	{
		FunctionNode	func( "Mod", "double4 Mod (const double x, const double4 y) {\n\treturn mod( double4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Mod_inDouble4_inDouble_outDouble4 (const Double4 &x, const Double &y, OUT Double4 &out)
	{
		FunctionNode	func( "Mod", "double4 Mod (const double4 x, const double y) {\n\treturn mod( (x), double4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inDouble_inDouble_outBool (const Double &x, const Double &y, OUT Bool &out)
	{
		FunctionNode	func( "Equal", "bool Equal (const double x, const double y) {\n\treturn (x) == (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inDouble2_inDouble2_outBool2 (const Double2 &x, const Double2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "Equal", "bool2 Equal (const double2 x, const double2 y) {\n\treturn equal( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inDouble_inDouble2_outBool2 (const Double &x, const Double2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "Equal", "bool2 Equal (const double x, const double2 y) {\n\treturn equal( double2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inDouble2_inDouble_outBool2 (const Double2 &x, const Double &y, OUT Bool2 &out)
	{
		FunctionNode	func( "Equal", "bool2 Equal (const double2 x, const double y) {\n\treturn equal( (x), double2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inDouble3_inDouble3_outBool3 (const Double3 &x, const Double3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "Equal", "bool3 Equal (const double3 x, const double3 y) {\n\treturn equal( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inDouble_inDouble3_outBool3 (const Double &x, const Double3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "Equal", "bool3 Equal (const double x, const double3 y) {\n\treturn equal( double3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inDouble3_inDouble_outBool3 (const Double3 &x, const Double &y, OUT Bool3 &out)
	{
		FunctionNode	func( "Equal", "bool3 Equal (const double3 x, const double y) {\n\treturn equal( (x), double3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inDouble4_inDouble4_outBool4 (const Double4 &x, const Double4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "Equal", "bool4 Equal (const double4 x, const double4 y) {\n\treturn equal( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inDouble_inDouble4_outBool4 (const Double &x, const Double4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "Equal", "bool4 Equal (const double x, const double4 y) {\n\treturn equal( double4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Equal_inDouble4_inDouble_outBool4 (const Double4 &x, const Double &y, OUT Bool4 &out)
	{
		FunctionNode	func( "Equal", "bool4 Equal (const double4 x, const double y) {\n\treturn equal( (x), double4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inDouble_inDouble_outBool (const Double &x, const Double &y, OUT Bool &out)
	{
		FunctionNode	func( "NotEqual", "bool NotEqual (const double x, const double y) {\n\treturn (x) != (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inDouble2_inDouble2_outBool2 (const Double2 &x, const Double2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "NotEqual", "bool2 NotEqual (const double2 x, const double2 y) {\n\treturn notEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inDouble_inDouble2_outBool2 (const Double &x, const Double2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "NotEqual", "bool2 NotEqual (const double x, const double2 y) {\n\treturn notEqual( double2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inDouble2_inDouble_outBool2 (const Double2 &x, const Double &y, OUT Bool2 &out)
	{
		FunctionNode	func( "NotEqual", "bool2 NotEqual (const double2 x, const double y) {\n\treturn notEqual( (x), double2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inDouble3_inDouble3_outBool3 (const Double3 &x, const Double3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "NotEqual", "bool3 NotEqual (const double3 x, const double3 y) {\n\treturn notEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inDouble_inDouble3_outBool3 (const Double &x, const Double3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "NotEqual", "bool3 NotEqual (const double x, const double3 y) {\n\treturn notEqual( double3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inDouble3_inDouble_outBool3 (const Double3 &x, const Double &y, OUT Bool3 &out)
	{
		FunctionNode	func( "NotEqual", "bool3 NotEqual (const double3 x, const double y) {\n\treturn notEqual( (x), double3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inDouble4_inDouble4_outBool4 (const Double4 &x, const Double4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "NotEqual", "bool4 NotEqual (const double4 x, const double4 y) {\n\treturn notEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inDouble_inDouble4_outBool4 (const Double &x, const Double4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "NotEqual", "bool4 NotEqual (const double x, const double4 y) {\n\treturn notEqual( double4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool NotEqual_inDouble4_inDouble_outBool4 (const Double4 &x, const Double &y, OUT Bool4 &out)
	{
		FunctionNode	func( "NotEqual", "bool4 NotEqual (const double4 x, const double y) {\n\treturn notEqual( (x), double4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inDouble_inDouble_outBool (const Double &x, const Double &y, OUT Bool &out)
	{
		FunctionNode	func( "GreaterThan", "bool GreaterThan (const double x, const double y) {\n\treturn (x) > (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inDouble2_inDouble2_outBool2 (const Double2 &x, const Double2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThan", "bool2 GreaterThan (const double2 x, const double2 y) {\n\treturn greaterThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inDouble_inDouble2_outBool2 (const Double &x, const Double2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThan", "bool2 GreaterThan (const double x, const double2 y) {\n\treturn greaterThan( double2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inDouble2_inDouble_outBool2 (const Double2 &x, const Double &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThan", "bool2 GreaterThan (const double2 x, const double y) {\n\treturn greaterThan( (x), double2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inDouble3_inDouble3_outBool3 (const Double3 &x, const Double3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThan", "bool3 GreaterThan (const double3 x, const double3 y) {\n\treturn greaterThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inDouble_inDouble3_outBool3 (const Double &x, const Double3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThan", "bool3 GreaterThan (const double x, const double3 y) {\n\treturn greaterThan( double3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inDouble3_inDouble_outBool3 (const Double3 &x, const Double &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThan", "bool3 GreaterThan (const double3 x, const double y) {\n\treturn greaterThan( (x), double3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inDouble4_inDouble4_outBool4 (const Double4 &x, const Double4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThan", "bool4 GreaterThan (const double4 x, const double4 y) {\n\treturn greaterThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inDouble_inDouble4_outBool4 (const Double &x, const Double4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThan", "bool4 GreaterThan (const double x, const double4 y) {\n\treturn greaterThan( double4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThan_inDouble4_inDouble_outBool4 (const Double4 &x, const Double &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThan", "bool4 GreaterThan (const double4 x, const double y) {\n\treturn greaterThan( (x), double4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inDouble_inDouble_outBool (const Double &x, const Double &y, OUT Bool &out)
	{
		FunctionNode	func( "LessThan", "bool LessThan (const double x, const double y) {\n\treturn (x) < (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inDouble2_inDouble2_outBool2 (const Double2 &x, const Double2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThan", "bool2 LessThan (const double2 x, const double2 y) {\n\treturn lessThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inDouble_inDouble2_outBool2 (const Double &x, const Double2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThan", "bool2 LessThan (const double x, const double2 y) {\n\treturn lessThan( double2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inDouble2_inDouble_outBool2 (const Double2 &x, const Double &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThan", "bool2 LessThan (const double2 x, const double y) {\n\treturn lessThan( (x), double2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inDouble3_inDouble3_outBool3 (const Double3 &x, const Double3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThan", "bool3 LessThan (const double3 x, const double3 y) {\n\treturn lessThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inDouble_inDouble3_outBool3 (const Double &x, const Double3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThan", "bool3 LessThan (const double x, const double3 y) {\n\treturn lessThan( double3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inDouble3_inDouble_outBool3 (const Double3 &x, const Double &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThan", "bool3 LessThan (const double3 x, const double y) {\n\treturn lessThan( (x), double3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inDouble4_inDouble4_outBool4 (const Double4 &x, const Double4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThan", "bool4 LessThan (const double4 x, const double4 y) {\n\treturn lessThan( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inDouble_inDouble4_outBool4 (const Double &x, const Double4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThan", "bool4 LessThan (const double x, const double4 y) {\n\treturn lessThan( double4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThan_inDouble4_inDouble_outBool4 (const Double4 &x, const Double &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThan", "bool4 LessThan (const double4 x, const double y) {\n\treturn lessThan( (x), double4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inDouble_inDouble_outBool (const Double &x, const Double &y, OUT Bool &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool GreaterThanEqual (const double x, const double y) {\n\treturn (x) >= (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inDouble2_inDouble2_outBool2 (const Double2 &x, const Double2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool2 GreaterThanEqual (const double2 x, const double2 y) {\n\treturn greaterThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inDouble_inDouble2_outBool2 (const Double &x, const Double2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool2 GreaterThanEqual (const double x, const double2 y) {\n\treturn greaterThanEqual( double2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inDouble2_inDouble_outBool2 (const Double2 &x, const Double &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool2 GreaterThanEqual (const double2 x, const double y) {\n\treturn greaterThanEqual( (x), double2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inDouble3_inDouble3_outBool3 (const Double3 &x, const Double3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool3 GreaterThanEqual (const double3 x, const double3 y) {\n\treturn greaterThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inDouble_inDouble3_outBool3 (const Double &x, const Double3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool3 GreaterThanEqual (const double x, const double3 y) {\n\treturn greaterThanEqual( double3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inDouble3_inDouble_outBool3 (const Double3 &x, const Double &y, OUT Bool3 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool3 GreaterThanEqual (const double3 x, const double y) {\n\treturn greaterThanEqual( (x), double3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inDouble4_inDouble4_outBool4 (const Double4 &x, const Double4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool4 GreaterThanEqual (const double4 x, const double4 y) {\n\treturn greaterThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inDouble_inDouble4_outBool4 (const Double &x, const Double4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool4 GreaterThanEqual (const double x, const double4 y) {\n\treturn greaterThanEqual( double4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool GreaterThanEqual_inDouble4_inDouble_outBool4 (const Double4 &x, const Double &y, OUT Bool4 &out)
	{
		FunctionNode	func( "GreaterThanEqual", "bool4 GreaterThanEqual (const double4 x, const double y) {\n\treturn greaterThanEqual( (x), double4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inDouble_inDouble_outBool (const Double &x, const Double &y, OUT Bool &out)
	{
		FunctionNode	func( "LessThanEqual", "bool LessThanEqual (const double x, const double y) {\n\treturn (x) <= (y);\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inDouble2_inDouble2_outBool2 (const Double2 &x, const Double2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool2 LessThanEqual (const double2 x, const double2 y) {\n\treturn lessThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inDouble_inDouble2_outBool2 (const Double &x, const Double2 &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool2 LessThanEqual (const double x, const double2 y) {\n\treturn lessThanEqual( double2(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inDouble2_inDouble_outBool2 (const Double2 &x, const Double &y, OUT Bool2 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool2 LessThanEqual (const double2 x, const double y) {\n\treturn lessThanEqual( (x), double2(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inDouble3_inDouble3_outBool3 (const Double3 &x, const Double3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool3 LessThanEqual (const double3 x, const double3 y) {\n\treturn lessThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inDouble_inDouble3_outBool3 (const Double &x, const Double3 &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool3 LessThanEqual (const double x, const double3 y) {\n\treturn lessThanEqual( double3(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inDouble3_inDouble_outBool3 (const Double3 &x, const Double &y, OUT Bool3 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool3 LessThanEqual (const double3 x, const double y) {\n\treturn lessThanEqual( (x), double3(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inDouble4_inDouble4_outBool4 (const Double4 &x, const Double4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool4 LessThanEqual (const double4 x, const double4 y) {\n\treturn lessThanEqual( (x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inDouble_inDouble4_outBool4 (const Double &x, const Double4 &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool4 LessThanEqual (const double x, const double4 y) {\n\treturn lessThanEqual( double4(x), (y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool LessThanEqual_inDouble4_inDouble_outBool4 (const Double4 &x, const Double &y, OUT Bool4 &out)
	{
		FunctionNode	func( "LessThanEqual", "bool4 LessThanEqual (const double4 x, const double y) {\n\treturn lessThanEqual( (x), double4(y) );\n}\n\n", x, y );
		Replace( out, "out", func );
		return true;
	}


	// Type casts
	bool Bool2Ctor_inBool_outBool2 (const Bool &x, OUT Bool2 &out)
	{
		FunctionNode	func( "GL4.bool2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Bool2Ctor_inBool3_outBool2 (const Bool3 &xyz, OUT Bool2 &out)
	{
		FunctionNode	func( "GL4.bool2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Bool2Ctor_inBool4_outBool2 (const Bool4 &xyzw, OUT Bool2 &out)
	{
		FunctionNode	func( "GL4.bool2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Bool2Ctor_inBool_inBool_outBool2 (const Bool &x, const Bool &y, OUT Bool2 &out)
	{
		FunctionNode	func( "GL4.bool2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Bool3Ctor_inBool_outBool3 (const Bool &x, OUT Bool3 &out)
	{
		FunctionNode	func( "GL4.bool3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Bool3Ctor_inBool2_outBool3 (const Bool2 &xy, OUT Bool3 &out)
	{
		FunctionNode	func( "GL4.bool3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Bool3Ctor_inBool4_outBool3 (const Bool4 &xyzw, OUT Bool3 &out)
	{
		FunctionNode	func( "GL4.bool3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Bool3Ctor_inBool2_inBool_outBool3 (const Bool2 &xy, const Bool &z, OUT Bool3 &out)
	{
		FunctionNode	func( "GL4.bool3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Bool3Ctor_inBool_inBool2_outBool3 (const Bool &x, const Bool2 &yz, OUT Bool3 &out)
	{
		FunctionNode	func( "GL4.bool3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Bool3Ctor_inBool_inBool_inBool_outBool3 (const Bool &x, const Bool &y, const Bool &z, OUT Bool3 &out)
	{
		FunctionNode	func( "GL4.bool3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Bool4Ctor_inBool_outBool4 (const Bool &x, OUT Bool4 &out)
	{
		FunctionNode	func( "GL4.bool4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Bool4Ctor_inBool2_outBool4 (const Bool2 &xy, OUT Bool4 &out)
	{
		FunctionNode	func( "GL4.bool4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Bool4Ctor_inBool3_outBool4 (const Bool3 &xyz, OUT Bool4 &out)
	{
		FunctionNode	func( "GL4.bool4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Bool4Ctor_inBool3_inBool_outBool4 (const Bool3 &xyz, const Bool &w, OUT Bool4 &out)
	{
		FunctionNode	func( "GL4.bool4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Bool4Ctor_inBool2_inBool2_outBool4 (const Bool2 &xy, const Bool2 &zw, OUT Bool4 &out)
	{
		FunctionNode	func( "GL4.bool4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Bool4Ctor_inBool_inBool3_outBool4 (const Bool &x, const Bool3 &yzw, OUT Bool4 &out)
	{
		FunctionNode	func( "GL4.bool4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Bool4Ctor_inBool2_inBool_inBool_outBool4 (const Bool2 &xy, const Bool &z, const Bool &w, OUT Bool4 &out)
	{
		FunctionNode	func( "GL4.bool4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Bool4Ctor_inBool_inBool2_inBool_outBool4 (const Bool &x, const Bool2 &yz, const Bool &w, OUT Bool4 &out)
	{
		FunctionNode	func( "GL4.bool4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Bool4Ctor_inBool_inBool_inBool2_outBool4 (const Bool &x, const Bool &y, const Bool2 &zw, OUT Bool4 &out)
	{
		FunctionNode	func( "GL4.bool4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Bool4Ctor_inBool_inBool_inBool_inBool_outBool4 (const Bool &x, const Bool &y, const Bool &z, const Bool &w, OUT Bool4 &out)
	{
		FunctionNode	func( "GL4.bool4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool IntCtor_inBool_outInt (const Bool &x, OUT Int &out)
	{
		FunctionNode	func( "GL4.int", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool IntCtor_inUInt_outInt (const UInt &x, OUT Int &out)
	{
		FunctionNode	func( "GL4.int", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool IntCtor_inLong_outInt (const Long &x, OUT Int &out)
	{
		FunctionNode	func( "GL4.int", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool IntCtor_inULong_outInt (const ULong &x, OUT Int &out)
	{
		FunctionNode	func( "GL4.int", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool IntCtor_inFloat_outInt (const Float &x, OUT Int &out)
	{
		FunctionNode	func( "GL4.int", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool IntCtor_inDouble_outInt (const Double &x, OUT Int &out)
	{
		FunctionNode	func( "GL4.int", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inBool_outInt2 (const Bool &x, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inBool2_outInt2 (const Bool2 &xy, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inBool3_outInt2 (const Bool3 &xyz, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inBool4_outInt2 (const Bool4 &xyzw, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inBool_inBool_outInt2 (const Bool &x, const Bool &y, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inInt_outInt2 (const Int &x, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inInt3_outInt2 (const Int3 &xyz, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inInt4_outInt2 (const Int4 &xyzw, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inInt_inInt_outInt2 (const Int &x, const Int &y, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inUInt_outInt2 (const UInt &x, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inUInt2_outInt2 (const UInt2 &xy, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inUInt3_outInt2 (const UInt3 &xyz, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inUInt4_outInt2 (const UInt4 &xyzw, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inUInt_inUInt_outInt2 (const UInt &x, const UInt &y, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inLong_outInt2 (const Long &x, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inLong2_outInt2 (const Long2 &xy, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inLong3_outInt2 (const Long3 &xyz, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inLong4_outInt2 (const Long4 &xyzw, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inLong_inLong_outInt2 (const Long &x, const Long &y, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inULong_outInt2 (const ULong &x, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inULong2_outInt2 (const ULong2 &xy, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inULong3_outInt2 (const ULong3 &xyz, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inULong4_outInt2 (const ULong4 &xyzw, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inULong_inULong_outInt2 (const ULong &x, const ULong &y, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inFloat_outInt2 (const Float &x, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inFloat2_outInt2 (const Float2 &xy, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inFloat3_outInt2 (const Float3 &xyz, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inFloat4_outInt2 (const Float4 &xyzw, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inFloat_inFloat_outInt2 (const Float &x, const Float &y, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inDouble_outInt2 (const Double &x, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inDouble2_outInt2 (const Double2 &xy, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inDouble3_outInt2 (const Double3 &xyz, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inDouble4_outInt2 (const Double4 &xyzw, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int2Ctor_inDouble_inDouble_outInt2 (const Double &x, const Double &y, OUT Int2 &out)
	{
		FunctionNode	func( "GL4.int2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inBool_outInt3 (const Bool &x, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inBool2_outInt3 (const Bool2 &xy, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inBool3_outInt3 (const Bool3 &xyz, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inBool4_outInt3 (const Bool4 &xyzw, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inBool2_inBool_outInt3 (const Bool2 &xy, const Bool &z, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inBool_inBool2_outInt3 (const Bool &x, const Bool2 &yz, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inBool_inBool_inBool_outInt3 (const Bool &x, const Bool &y, const Bool &z, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inInt_outInt3 (const Int &x, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inInt2_outInt3 (const Int2 &xy, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inInt4_outInt3 (const Int4 &xyzw, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inInt2_inInt_outInt3 (const Int2 &xy, const Int &z, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inInt_inInt2_outInt3 (const Int &x, const Int2 &yz, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inInt_inInt_inInt_outInt3 (const Int &x, const Int &y, const Int &z, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inUInt_outInt3 (const UInt &x, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inUInt2_outInt3 (const UInt2 &xy, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inUInt3_outInt3 (const UInt3 &xyz, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inUInt4_outInt3 (const UInt4 &xyzw, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inUInt2_inUInt_outInt3 (const UInt2 &xy, const UInt &z, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inUInt_inUInt2_outInt3 (const UInt &x, const UInt2 &yz, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inUInt_inUInt_inUInt_outInt3 (const UInt &x, const UInt &y, const UInt &z, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inLong_outInt3 (const Long &x, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inLong2_outInt3 (const Long2 &xy, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inLong3_outInt3 (const Long3 &xyz, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inLong4_outInt3 (const Long4 &xyzw, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inLong2_inLong_outInt3 (const Long2 &xy, const Long &z, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inLong_inLong2_outInt3 (const Long &x, const Long2 &yz, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inLong_inLong_inLong_outInt3 (const Long &x, const Long &y, const Long &z, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inULong_outInt3 (const ULong &x, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inULong2_outInt3 (const ULong2 &xy, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inULong3_outInt3 (const ULong3 &xyz, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inULong4_outInt3 (const ULong4 &xyzw, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inULong2_inULong_outInt3 (const ULong2 &xy, const ULong &z, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inULong_inULong2_outInt3 (const ULong &x, const ULong2 &yz, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inULong_inULong_inULong_outInt3 (const ULong &x, const ULong &y, const ULong &z, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inFloat_outInt3 (const Float &x, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inFloat2_outInt3 (const Float2 &xy, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inFloat3_outInt3 (const Float3 &xyz, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inFloat4_outInt3 (const Float4 &xyzw, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inFloat2_inFloat_outInt3 (const Float2 &xy, const Float &z, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inFloat_inFloat2_outInt3 (const Float &x, const Float2 &yz, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inFloat_inFloat_inFloat_outInt3 (const Float &x, const Float &y, const Float &z, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inDouble_outInt3 (const Double &x, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inDouble2_outInt3 (const Double2 &xy, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inDouble3_outInt3 (const Double3 &xyz, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inDouble4_outInt3 (const Double4 &xyzw, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inDouble2_inDouble_outInt3 (const Double2 &xy, const Double &z, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inDouble_inDouble2_outInt3 (const Double &x, const Double2 &yz, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Int3Ctor_inDouble_inDouble_inDouble_outInt3 (const Double &x, const Double &y, const Double &z, OUT Int3 &out)
	{
		FunctionNode	func( "GL4.int3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inBool_outInt4 (const Bool &x, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inBool2_outInt4 (const Bool2 &xy, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inBool3_outInt4 (const Bool3 &xyz, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inBool4_outInt4 (const Bool4 &xyzw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inBool3_inBool_outInt4 (const Bool3 &xyz, const Bool &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inBool2_inBool2_outInt4 (const Bool2 &xy, const Bool2 &zw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inBool_inBool3_outInt4 (const Bool &x, const Bool3 &yzw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inBool2_inBool_inBool_outInt4 (const Bool2 &xy, const Bool &z, const Bool &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inBool_inBool2_inBool_outInt4 (const Bool &x, const Bool2 &yz, const Bool &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inBool_inBool_inBool2_outInt4 (const Bool &x, const Bool &y, const Bool2 &zw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inBool_inBool_inBool_inBool_outInt4 (const Bool &x, const Bool &y, const Bool &z, const Bool &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inInt_outInt4 (const Int &x, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inInt2_outInt4 (const Int2 &xy, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inInt3_outInt4 (const Int3 &xyz, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inInt3_inInt_outInt4 (const Int3 &xyz, const Int &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inInt2_inInt2_outInt4 (const Int2 &xy, const Int2 &zw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inInt_inInt3_outInt4 (const Int &x, const Int3 &yzw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inInt2_inInt_inInt_outInt4 (const Int2 &xy, const Int &z, const Int &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inInt_inInt2_inInt_outInt4 (const Int &x, const Int2 &yz, const Int &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inInt_inInt_inInt2_outInt4 (const Int &x, const Int &y, const Int2 &zw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inInt_inInt_inInt_inInt_outInt4 (const Int &x, const Int &y, const Int &z, const Int &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inUInt_outInt4 (const UInt &x, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inUInt2_outInt4 (const UInt2 &xy, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inUInt3_outInt4 (const UInt3 &xyz, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inUInt4_outInt4 (const UInt4 &xyzw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inUInt3_inUInt_outInt4 (const UInt3 &xyz, const UInt &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inUInt2_inUInt2_outInt4 (const UInt2 &xy, const UInt2 &zw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inUInt_inUInt3_outInt4 (const UInt &x, const UInt3 &yzw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inUInt2_inUInt_inUInt_outInt4 (const UInt2 &xy, const UInt &z, const UInt &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inUInt_inUInt2_inUInt_outInt4 (const UInt &x, const UInt2 &yz, const UInt &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inUInt_inUInt_inUInt2_outInt4 (const UInt &x, const UInt &y, const UInt2 &zw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inUInt_inUInt_inUInt_inUInt_outInt4 (const UInt &x, const UInt &y, const UInt &z, const UInt &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inLong_outInt4 (const Long &x, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inLong2_outInt4 (const Long2 &xy, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inLong3_outInt4 (const Long3 &xyz, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inLong4_outInt4 (const Long4 &xyzw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inLong3_inLong_outInt4 (const Long3 &xyz, const Long &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inLong2_inLong2_outInt4 (const Long2 &xy, const Long2 &zw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inLong_inLong3_outInt4 (const Long &x, const Long3 &yzw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inLong2_inLong_inLong_outInt4 (const Long2 &xy, const Long &z, const Long &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inLong_inLong2_inLong_outInt4 (const Long &x, const Long2 &yz, const Long &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inLong_inLong_inLong2_outInt4 (const Long &x, const Long &y, const Long2 &zw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inLong_inLong_inLong_inLong_outInt4 (const Long &x, const Long &y, const Long &z, const Long &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inULong_outInt4 (const ULong &x, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inULong2_outInt4 (const ULong2 &xy, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inULong3_outInt4 (const ULong3 &xyz, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inULong4_outInt4 (const ULong4 &xyzw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inULong3_inULong_outInt4 (const ULong3 &xyz, const ULong &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inULong2_inULong2_outInt4 (const ULong2 &xy, const ULong2 &zw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inULong_inULong3_outInt4 (const ULong &x, const ULong3 &yzw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inULong2_inULong_inULong_outInt4 (const ULong2 &xy, const ULong &z, const ULong &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inULong_inULong2_inULong_outInt4 (const ULong &x, const ULong2 &yz, const ULong &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inULong_inULong_inULong2_outInt4 (const ULong &x, const ULong &y, const ULong2 &zw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inULong_inULong_inULong_inULong_outInt4 (const ULong &x, const ULong &y, const ULong &z, const ULong &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inFloat_outInt4 (const Float &x, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inFloat2_outInt4 (const Float2 &xy, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inFloat3_outInt4 (const Float3 &xyz, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inFloat4_outInt4 (const Float4 &xyzw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inFloat3_inFloat_outInt4 (const Float3 &xyz, const Float &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inFloat2_inFloat2_outInt4 (const Float2 &xy, const Float2 &zw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inFloat_inFloat3_outInt4 (const Float &x, const Float3 &yzw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inFloat2_inFloat_inFloat_outInt4 (const Float2 &xy, const Float &z, const Float &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inFloat_inFloat2_inFloat_outInt4 (const Float &x, const Float2 &yz, const Float &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inFloat_inFloat_inFloat2_outInt4 (const Float &x, const Float &y, const Float2 &zw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inFloat_inFloat_inFloat_inFloat_outInt4 (const Float &x, const Float &y, const Float &z, const Float &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inDouble_outInt4 (const Double &x, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inDouble2_outInt4 (const Double2 &xy, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inDouble3_outInt4 (const Double3 &xyz, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inDouble4_outInt4 (const Double4 &xyzw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inDouble3_inDouble_outInt4 (const Double3 &xyz, const Double &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inDouble2_inDouble2_outInt4 (const Double2 &xy, const Double2 &zw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inDouble_inDouble3_outInt4 (const Double &x, const Double3 &yzw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inDouble2_inDouble_inDouble_outInt4 (const Double2 &xy, const Double &z, const Double &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inDouble_inDouble2_inDouble_outInt4 (const Double &x, const Double2 &yz, const Double &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inDouble_inDouble_inDouble2_outInt4 (const Double &x, const Double &y, const Double2 &zw, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Int4Ctor_inDouble_inDouble_inDouble_inDouble_outInt4 (const Double &x, const Double &y, const Double &z, const Double &w, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.int4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool UIntCtor_inBool_outUInt (const Bool &x, OUT UInt &out)
	{
		FunctionNode	func( "GL4.uint", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UIntCtor_inInt_outUInt (const Int &x, OUT UInt &out)
	{
		FunctionNode	func( "GL4.uint", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UIntCtor_inLong_outUInt (const Long &x, OUT UInt &out)
	{
		FunctionNode	func( "GL4.uint", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UIntCtor_inULong_outUInt (const ULong &x, OUT UInt &out)
	{
		FunctionNode	func( "GL4.uint", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UIntCtor_inFloat_outUInt (const Float &x, OUT UInt &out)
	{
		FunctionNode	func( "GL4.uint", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UIntCtor_inDouble_outUInt (const Double &x, OUT UInt &out)
	{
		FunctionNode	func( "GL4.uint", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inBool_outUInt2 (const Bool &x, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inBool2_outUInt2 (const Bool2 &xy, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inBool3_outUInt2 (const Bool3 &xyz, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inBool4_outUInt2 (const Bool4 &xyzw, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inBool_inBool_outUInt2 (const Bool &x, const Bool &y, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inInt_outUInt2 (const Int &x, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inInt2_outUInt2 (const Int2 &xy, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inInt3_outUInt2 (const Int3 &xyz, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inInt4_outUInt2 (const Int4 &xyzw, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inInt_inInt_outUInt2 (const Int &x, const Int &y, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inUInt_outUInt2 (const UInt &x, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inUInt3_outUInt2 (const UInt3 &xyz, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inUInt4_outUInt2 (const UInt4 &xyzw, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inUInt_inUInt_outUInt2 (const UInt &x, const UInt &y, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inLong_outUInt2 (const Long &x, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inLong2_outUInt2 (const Long2 &xy, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inLong3_outUInt2 (const Long3 &xyz, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inLong4_outUInt2 (const Long4 &xyzw, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inLong_inLong_outUInt2 (const Long &x, const Long &y, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inULong_outUInt2 (const ULong &x, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inULong2_outUInt2 (const ULong2 &xy, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inULong3_outUInt2 (const ULong3 &xyz, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inULong4_outUInt2 (const ULong4 &xyzw, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inULong_inULong_outUInt2 (const ULong &x, const ULong &y, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inFloat_outUInt2 (const Float &x, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inFloat2_outUInt2 (const Float2 &xy, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inFloat3_outUInt2 (const Float3 &xyz, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inFloat4_outUInt2 (const Float4 &xyzw, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inFloat_inFloat_outUInt2 (const Float &x, const Float &y, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inDouble_outUInt2 (const Double &x, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inDouble2_outUInt2 (const Double2 &xy, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inDouble3_outUInt2 (const Double3 &xyz, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inDouble4_outUInt2 (const Double4 &xyzw, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt2Ctor_inDouble_inDouble_outUInt2 (const Double &x, const Double &y, OUT UInt2 &out)
	{
		FunctionNode	func( "GL4.uint2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inBool_outUInt3 (const Bool &x, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inBool2_outUInt3 (const Bool2 &xy, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inBool3_outUInt3 (const Bool3 &xyz, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inBool4_outUInt3 (const Bool4 &xyzw, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inBool2_inBool_outUInt3 (const Bool2 &xy, const Bool &z, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inBool_inBool2_outUInt3 (const Bool &x, const Bool2 &yz, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inBool_inBool_inBool_outUInt3 (const Bool &x, const Bool &y, const Bool &z, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inInt_outUInt3 (const Int &x, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inInt2_outUInt3 (const Int2 &xy, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inInt3_outUInt3 (const Int3 &xyz, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inInt4_outUInt3 (const Int4 &xyzw, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inInt2_inInt_outUInt3 (const Int2 &xy, const Int &z, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inInt_inInt2_outUInt3 (const Int &x, const Int2 &yz, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inInt_inInt_inInt_outUInt3 (const Int &x, const Int &y, const Int &z, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inUInt_outUInt3 (const UInt &x, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inUInt2_outUInt3 (const UInt2 &xy, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inUInt4_outUInt3 (const UInt4 &xyzw, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inUInt2_inUInt_outUInt3 (const UInt2 &xy, const UInt &z, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inUInt_inUInt2_outUInt3 (const UInt &x, const UInt2 &yz, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inUInt_inUInt_inUInt_outUInt3 (const UInt &x, const UInt &y, const UInt &z, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inLong_outUInt3 (const Long &x, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inLong2_outUInt3 (const Long2 &xy, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inLong3_outUInt3 (const Long3 &xyz, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inLong4_outUInt3 (const Long4 &xyzw, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inLong2_inLong_outUInt3 (const Long2 &xy, const Long &z, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inLong_inLong2_outUInt3 (const Long &x, const Long2 &yz, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inLong_inLong_inLong_outUInt3 (const Long &x, const Long &y, const Long &z, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inULong_outUInt3 (const ULong &x, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inULong2_outUInt3 (const ULong2 &xy, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inULong3_outUInt3 (const ULong3 &xyz, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inULong4_outUInt3 (const ULong4 &xyzw, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inULong2_inULong_outUInt3 (const ULong2 &xy, const ULong &z, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inULong_inULong2_outUInt3 (const ULong &x, const ULong2 &yz, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inULong_inULong_inULong_outUInt3 (const ULong &x, const ULong &y, const ULong &z, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inFloat_outUInt3 (const Float &x, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inFloat2_outUInt3 (const Float2 &xy, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inFloat3_outUInt3 (const Float3 &xyz, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inFloat4_outUInt3 (const Float4 &xyzw, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inFloat2_inFloat_outUInt3 (const Float2 &xy, const Float &z, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inFloat_inFloat2_outUInt3 (const Float &x, const Float2 &yz, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inFloat_inFloat_inFloat_outUInt3 (const Float &x, const Float &y, const Float &z, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inDouble_outUInt3 (const Double &x, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inDouble2_outUInt3 (const Double2 &xy, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inDouble3_outUInt3 (const Double3 &xyz, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inDouble4_outUInt3 (const Double4 &xyzw, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inDouble2_inDouble_outUInt3 (const Double2 &xy, const Double &z, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inDouble_inDouble2_outUInt3 (const Double &x, const Double2 &yz, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt3Ctor_inDouble_inDouble_inDouble_outUInt3 (const Double &x, const Double &y, const Double &z, OUT UInt3 &out)
	{
		FunctionNode	func( "GL4.uint3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inBool_outUInt4 (const Bool &x, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inBool2_outUInt4 (const Bool2 &xy, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inBool3_outUInt4 (const Bool3 &xyz, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inBool4_outUInt4 (const Bool4 &xyzw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inBool3_inBool_outUInt4 (const Bool3 &xyz, const Bool &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inBool2_inBool2_outUInt4 (const Bool2 &xy, const Bool2 &zw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inBool_inBool3_outUInt4 (const Bool &x, const Bool3 &yzw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inBool2_inBool_inBool_outUInt4 (const Bool2 &xy, const Bool &z, const Bool &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inBool_inBool2_inBool_outUInt4 (const Bool &x, const Bool2 &yz, const Bool &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inBool_inBool_inBool2_outUInt4 (const Bool &x, const Bool &y, const Bool2 &zw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inBool_inBool_inBool_inBool_outUInt4 (const Bool &x, const Bool &y, const Bool &z, const Bool &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inInt_outUInt4 (const Int &x, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inInt2_outUInt4 (const Int2 &xy, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inInt3_outUInt4 (const Int3 &xyz, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inInt4_outUInt4 (const Int4 &xyzw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inInt3_inInt_outUInt4 (const Int3 &xyz, const Int &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inInt2_inInt2_outUInt4 (const Int2 &xy, const Int2 &zw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inInt_inInt3_outUInt4 (const Int &x, const Int3 &yzw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inInt2_inInt_inInt_outUInt4 (const Int2 &xy, const Int &z, const Int &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inInt_inInt2_inInt_outUInt4 (const Int &x, const Int2 &yz, const Int &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inInt_inInt_inInt2_outUInt4 (const Int &x, const Int &y, const Int2 &zw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inInt_inInt_inInt_inInt_outUInt4 (const Int &x, const Int &y, const Int &z, const Int &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inUInt_outUInt4 (const UInt &x, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inUInt2_outUInt4 (const UInt2 &xy, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inUInt3_outUInt4 (const UInt3 &xyz, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inUInt3_inUInt_outUInt4 (const UInt3 &xyz, const UInt &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inUInt2_inUInt2_outUInt4 (const UInt2 &xy, const UInt2 &zw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inUInt_inUInt3_outUInt4 (const UInt &x, const UInt3 &yzw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inUInt2_inUInt_inUInt_outUInt4 (const UInt2 &xy, const UInt &z, const UInt &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inUInt_inUInt2_inUInt_outUInt4 (const UInt &x, const UInt2 &yz, const UInt &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inUInt_inUInt_inUInt2_outUInt4 (const UInt &x, const UInt &y, const UInt2 &zw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inUInt_inUInt_inUInt_inUInt_outUInt4 (const UInt &x, const UInt &y, const UInt &z, const UInt &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inLong_outUInt4 (const Long &x, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inLong2_outUInt4 (const Long2 &xy, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inLong3_outUInt4 (const Long3 &xyz, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inLong4_outUInt4 (const Long4 &xyzw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inLong3_inLong_outUInt4 (const Long3 &xyz, const Long &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inLong2_inLong2_outUInt4 (const Long2 &xy, const Long2 &zw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inLong_inLong3_outUInt4 (const Long &x, const Long3 &yzw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inLong2_inLong_inLong_outUInt4 (const Long2 &xy, const Long &z, const Long &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inLong_inLong2_inLong_outUInt4 (const Long &x, const Long2 &yz, const Long &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inLong_inLong_inLong2_outUInt4 (const Long &x, const Long &y, const Long2 &zw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inLong_inLong_inLong_inLong_outUInt4 (const Long &x, const Long &y, const Long &z, const Long &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inULong_outUInt4 (const ULong &x, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inULong2_outUInt4 (const ULong2 &xy, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inULong3_outUInt4 (const ULong3 &xyz, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inULong4_outUInt4 (const ULong4 &xyzw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inULong3_inULong_outUInt4 (const ULong3 &xyz, const ULong &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inULong2_inULong2_outUInt4 (const ULong2 &xy, const ULong2 &zw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inULong_inULong3_outUInt4 (const ULong &x, const ULong3 &yzw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inULong2_inULong_inULong_outUInt4 (const ULong2 &xy, const ULong &z, const ULong &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inULong_inULong2_inULong_outUInt4 (const ULong &x, const ULong2 &yz, const ULong &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inULong_inULong_inULong2_outUInt4 (const ULong &x, const ULong &y, const ULong2 &zw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inULong_inULong_inULong_inULong_outUInt4 (const ULong &x, const ULong &y, const ULong &z, const ULong &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inFloat_outUInt4 (const Float &x, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inFloat2_outUInt4 (const Float2 &xy, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inFloat3_outUInt4 (const Float3 &xyz, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inFloat4_outUInt4 (const Float4 &xyzw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inFloat3_inFloat_outUInt4 (const Float3 &xyz, const Float &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inFloat2_inFloat2_outUInt4 (const Float2 &xy, const Float2 &zw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inFloat_inFloat3_outUInt4 (const Float &x, const Float3 &yzw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inFloat2_inFloat_inFloat_outUInt4 (const Float2 &xy, const Float &z, const Float &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inFloat_inFloat2_inFloat_outUInt4 (const Float &x, const Float2 &yz, const Float &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inFloat_inFloat_inFloat2_outUInt4 (const Float &x, const Float &y, const Float2 &zw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inFloat_inFloat_inFloat_inFloat_outUInt4 (const Float &x, const Float &y, const Float &z, const Float &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inDouble_outUInt4 (const Double &x, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inDouble2_outUInt4 (const Double2 &xy, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inDouble3_outUInt4 (const Double3 &xyz, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inDouble4_outUInt4 (const Double4 &xyzw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inDouble3_inDouble_outUInt4 (const Double3 &xyz, const Double &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inDouble2_inDouble2_outUInt4 (const Double2 &xy, const Double2 &zw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inDouble_inDouble3_outUInt4 (const Double &x, const Double3 &yzw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inDouble2_inDouble_inDouble_outUInt4 (const Double2 &xy, const Double &z, const Double &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inDouble_inDouble2_inDouble_outUInt4 (const Double &x, const Double2 &yz, const Double &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inDouble_inDouble_inDouble2_outUInt4 (const Double &x, const Double &y, const Double2 &zw, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool UInt4Ctor_inDouble_inDouble_inDouble_inDouble_outUInt4 (const Double &x, const Double &y, const Double &z, const Double &w, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.uint4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool LongCtor_inBool_outLong (const Bool &x, OUT Long &out)
	{
		FunctionNode	func( "GL4.ilong", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool LongCtor_inInt_outLong (const Int &x, OUT Long &out)
	{
		FunctionNode	func( "GL4.ilong", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool LongCtor_inUInt_outLong (const UInt &x, OUT Long &out)
	{
		FunctionNode	func( "GL4.ilong", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool LongCtor_inULong_outLong (const ULong &x, OUT Long &out)
	{
		FunctionNode	func( "GL4.ilong", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool LongCtor_inFloat_outLong (const Float &x, OUT Long &out)
	{
		FunctionNode	func( "GL4.ilong", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool LongCtor_inDouble_outLong (const Double &x, OUT Long &out)
	{
		FunctionNode	func( "GL4.ilong", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inBool_outLong2 (const Bool &x, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inBool2_outLong2 (const Bool2 &xy, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inBool3_outLong2 (const Bool3 &xyz, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inBool4_outLong2 (const Bool4 &xyzw, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inBool_inBool_outLong2 (const Bool &x, const Bool &y, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inInt_outLong2 (const Int &x, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inInt2_outLong2 (const Int2 &xy, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inInt3_outLong2 (const Int3 &xyz, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inInt4_outLong2 (const Int4 &xyzw, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inInt_inInt_outLong2 (const Int &x, const Int &y, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inUInt_outLong2 (const UInt &x, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inUInt2_outLong2 (const UInt2 &xy, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inUInt3_outLong2 (const UInt3 &xyz, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inUInt4_outLong2 (const UInt4 &xyzw, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inUInt_inUInt_outLong2 (const UInt &x, const UInt &y, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inLong_outLong2 (const Long &x, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inLong3_outLong2 (const Long3 &xyz, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inLong4_outLong2 (const Long4 &xyzw, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inLong_inLong_outLong2 (const Long &x, const Long &y, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inULong_outLong2 (const ULong &x, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inULong2_outLong2 (const ULong2 &xy, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inULong3_outLong2 (const ULong3 &xyz, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inULong4_outLong2 (const ULong4 &xyzw, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inULong_inULong_outLong2 (const ULong &x, const ULong &y, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inFloat_outLong2 (const Float &x, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inFloat2_outLong2 (const Float2 &xy, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inFloat3_outLong2 (const Float3 &xyz, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inFloat4_outLong2 (const Float4 &xyzw, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inFloat_inFloat_outLong2 (const Float &x, const Float &y, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inDouble_outLong2 (const Double &x, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inDouble2_outLong2 (const Double2 &xy, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inDouble3_outLong2 (const Double3 &xyz, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inDouble4_outLong2 (const Double4 &xyzw, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long2Ctor_inDouble_inDouble_outLong2 (const Double &x, const Double &y, OUT Long2 &out)
	{
		FunctionNode	func( "GL4.ilong2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inBool_outLong3 (const Bool &x, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inBool2_outLong3 (const Bool2 &xy, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inBool3_outLong3 (const Bool3 &xyz, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inBool4_outLong3 (const Bool4 &xyzw, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inBool2_inBool_outLong3 (const Bool2 &xy, const Bool &z, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inBool_inBool2_outLong3 (const Bool &x, const Bool2 &yz, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inBool_inBool_inBool_outLong3 (const Bool &x, const Bool &y, const Bool &z, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inInt_outLong3 (const Int &x, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inInt2_outLong3 (const Int2 &xy, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inInt3_outLong3 (const Int3 &xyz, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inInt4_outLong3 (const Int4 &xyzw, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inInt2_inInt_outLong3 (const Int2 &xy, const Int &z, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inInt_inInt2_outLong3 (const Int &x, const Int2 &yz, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inInt_inInt_inInt_outLong3 (const Int &x, const Int &y, const Int &z, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inUInt_outLong3 (const UInt &x, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inUInt2_outLong3 (const UInt2 &xy, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inUInt3_outLong3 (const UInt3 &xyz, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inUInt4_outLong3 (const UInt4 &xyzw, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inUInt2_inUInt_outLong3 (const UInt2 &xy, const UInt &z, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inUInt_inUInt2_outLong3 (const UInt &x, const UInt2 &yz, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inUInt_inUInt_inUInt_outLong3 (const UInt &x, const UInt &y, const UInt &z, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inLong_outLong3 (const Long &x, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inLong2_outLong3 (const Long2 &xy, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inLong4_outLong3 (const Long4 &xyzw, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inLong2_inLong_outLong3 (const Long2 &xy, const Long &z, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inLong_inLong2_outLong3 (const Long &x, const Long2 &yz, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inLong_inLong_inLong_outLong3 (const Long &x, const Long &y, const Long &z, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inULong_outLong3 (const ULong &x, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inULong2_outLong3 (const ULong2 &xy, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inULong3_outLong3 (const ULong3 &xyz, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inULong4_outLong3 (const ULong4 &xyzw, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inULong2_inULong_outLong3 (const ULong2 &xy, const ULong &z, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inULong_inULong2_outLong3 (const ULong &x, const ULong2 &yz, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inULong_inULong_inULong_outLong3 (const ULong &x, const ULong &y, const ULong &z, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inFloat_outLong3 (const Float &x, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inFloat2_outLong3 (const Float2 &xy, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inFloat3_outLong3 (const Float3 &xyz, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inFloat4_outLong3 (const Float4 &xyzw, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inFloat2_inFloat_outLong3 (const Float2 &xy, const Float &z, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inFloat_inFloat2_outLong3 (const Float &x, const Float2 &yz, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inFloat_inFloat_inFloat_outLong3 (const Float &x, const Float &y, const Float &z, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inDouble_outLong3 (const Double &x, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inDouble2_outLong3 (const Double2 &xy, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inDouble3_outLong3 (const Double3 &xyz, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inDouble4_outLong3 (const Double4 &xyzw, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inDouble2_inDouble_outLong3 (const Double2 &xy, const Double &z, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inDouble_inDouble2_outLong3 (const Double &x, const Double2 &yz, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Long3Ctor_inDouble_inDouble_inDouble_outLong3 (const Double &x, const Double &y, const Double &z, OUT Long3 &out)
	{
		FunctionNode	func( "GL4.ilong3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inBool_outLong4 (const Bool &x, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inBool2_outLong4 (const Bool2 &xy, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inBool3_outLong4 (const Bool3 &xyz, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inBool4_outLong4 (const Bool4 &xyzw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inBool3_inBool_outLong4 (const Bool3 &xyz, const Bool &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inBool2_inBool2_outLong4 (const Bool2 &xy, const Bool2 &zw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inBool_inBool3_outLong4 (const Bool &x, const Bool3 &yzw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inBool2_inBool_inBool_outLong4 (const Bool2 &xy, const Bool &z, const Bool &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inBool_inBool2_inBool_outLong4 (const Bool &x, const Bool2 &yz, const Bool &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inBool_inBool_inBool2_outLong4 (const Bool &x, const Bool &y, const Bool2 &zw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inBool_inBool_inBool_inBool_outLong4 (const Bool &x, const Bool &y, const Bool &z, const Bool &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inInt_outLong4 (const Int &x, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inInt2_outLong4 (const Int2 &xy, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inInt3_outLong4 (const Int3 &xyz, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inInt4_outLong4 (const Int4 &xyzw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inInt3_inInt_outLong4 (const Int3 &xyz, const Int &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inInt2_inInt2_outLong4 (const Int2 &xy, const Int2 &zw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inInt_inInt3_outLong4 (const Int &x, const Int3 &yzw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inInt2_inInt_inInt_outLong4 (const Int2 &xy, const Int &z, const Int &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inInt_inInt2_inInt_outLong4 (const Int &x, const Int2 &yz, const Int &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inInt_inInt_inInt2_outLong4 (const Int &x, const Int &y, const Int2 &zw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inInt_inInt_inInt_inInt_outLong4 (const Int &x, const Int &y, const Int &z, const Int &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inUInt_outLong4 (const UInt &x, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inUInt2_outLong4 (const UInt2 &xy, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inUInt3_outLong4 (const UInt3 &xyz, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inUInt4_outLong4 (const UInt4 &xyzw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inUInt3_inUInt_outLong4 (const UInt3 &xyz, const UInt &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inUInt2_inUInt2_outLong4 (const UInt2 &xy, const UInt2 &zw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inUInt_inUInt3_outLong4 (const UInt &x, const UInt3 &yzw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inUInt2_inUInt_inUInt_outLong4 (const UInt2 &xy, const UInt &z, const UInt &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inUInt_inUInt2_inUInt_outLong4 (const UInt &x, const UInt2 &yz, const UInt &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inUInt_inUInt_inUInt2_outLong4 (const UInt &x, const UInt &y, const UInt2 &zw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inUInt_inUInt_inUInt_inUInt_outLong4 (const UInt &x, const UInt &y, const UInt &z, const UInt &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inLong_outLong4 (const Long &x, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inLong2_outLong4 (const Long2 &xy, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inLong3_outLong4 (const Long3 &xyz, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inLong3_inLong_outLong4 (const Long3 &xyz, const Long &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inLong2_inLong2_outLong4 (const Long2 &xy, const Long2 &zw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inLong_inLong3_outLong4 (const Long &x, const Long3 &yzw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inLong2_inLong_inLong_outLong4 (const Long2 &xy, const Long &z, const Long &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inLong_inLong2_inLong_outLong4 (const Long &x, const Long2 &yz, const Long &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inLong_inLong_inLong2_outLong4 (const Long &x, const Long &y, const Long2 &zw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inLong_inLong_inLong_inLong_outLong4 (const Long &x, const Long &y, const Long &z, const Long &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inULong_outLong4 (const ULong &x, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inULong2_outLong4 (const ULong2 &xy, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inULong3_outLong4 (const ULong3 &xyz, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inULong4_outLong4 (const ULong4 &xyzw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inULong3_inULong_outLong4 (const ULong3 &xyz, const ULong &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inULong2_inULong2_outLong4 (const ULong2 &xy, const ULong2 &zw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inULong_inULong3_outLong4 (const ULong &x, const ULong3 &yzw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inULong2_inULong_inULong_outLong4 (const ULong2 &xy, const ULong &z, const ULong &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inULong_inULong2_inULong_outLong4 (const ULong &x, const ULong2 &yz, const ULong &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inULong_inULong_inULong2_outLong4 (const ULong &x, const ULong &y, const ULong2 &zw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inULong_inULong_inULong_inULong_outLong4 (const ULong &x, const ULong &y, const ULong &z, const ULong &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inFloat_outLong4 (const Float &x, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inFloat2_outLong4 (const Float2 &xy, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inFloat3_outLong4 (const Float3 &xyz, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inFloat4_outLong4 (const Float4 &xyzw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inFloat3_inFloat_outLong4 (const Float3 &xyz, const Float &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inFloat2_inFloat2_outLong4 (const Float2 &xy, const Float2 &zw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inFloat_inFloat3_outLong4 (const Float &x, const Float3 &yzw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inFloat2_inFloat_inFloat_outLong4 (const Float2 &xy, const Float &z, const Float &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inFloat_inFloat2_inFloat_outLong4 (const Float &x, const Float2 &yz, const Float &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inFloat_inFloat_inFloat2_outLong4 (const Float &x, const Float &y, const Float2 &zw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inFloat_inFloat_inFloat_inFloat_outLong4 (const Float &x, const Float &y, const Float &z, const Float &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inDouble_outLong4 (const Double &x, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inDouble2_outLong4 (const Double2 &xy, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inDouble3_outLong4 (const Double3 &xyz, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inDouble4_outLong4 (const Double4 &xyzw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inDouble3_inDouble_outLong4 (const Double3 &xyz, const Double &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inDouble2_inDouble2_outLong4 (const Double2 &xy, const Double2 &zw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inDouble_inDouble3_outLong4 (const Double &x, const Double3 &yzw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inDouble2_inDouble_inDouble_outLong4 (const Double2 &xy, const Double &z, const Double &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inDouble_inDouble2_inDouble_outLong4 (const Double &x, const Double2 &yz, const Double &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inDouble_inDouble_inDouble2_outLong4 (const Double &x, const Double &y, const Double2 &zw, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Long4Ctor_inDouble_inDouble_inDouble_inDouble_outLong4 (const Double &x, const Double &y, const Double &z, const Double &w, OUT Long4 &out)
	{
		FunctionNode	func( "GL4.ilong4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULongCtor_inBool_outULong (const Bool &x, OUT ULong &out)
	{
		FunctionNode	func( "GL4.ulong", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULongCtor_inInt_outULong (const Int &x, OUT ULong &out)
	{
		FunctionNode	func( "GL4.ulong", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULongCtor_inUInt_outULong (const UInt &x, OUT ULong &out)
	{
		FunctionNode	func( "GL4.ulong", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULongCtor_inLong_outULong (const Long &x, OUT ULong &out)
	{
		FunctionNode	func( "GL4.ulong", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULongCtor_inFloat_outULong (const Float &x, OUT ULong &out)
	{
		FunctionNode	func( "GL4.ulong", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULongCtor_inDouble_outULong (const Double &x, OUT ULong &out)
	{
		FunctionNode	func( "GL4.ulong", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inBool_outULong2 (const Bool &x, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inBool2_outULong2 (const Bool2 &xy, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inBool3_outULong2 (const Bool3 &xyz, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inBool4_outULong2 (const Bool4 &xyzw, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inBool_inBool_outULong2 (const Bool &x, const Bool &y, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inInt_outULong2 (const Int &x, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inInt2_outULong2 (const Int2 &xy, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inInt3_outULong2 (const Int3 &xyz, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inInt4_outULong2 (const Int4 &xyzw, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inInt_inInt_outULong2 (const Int &x, const Int &y, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inUInt_outULong2 (const UInt &x, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inUInt2_outULong2 (const UInt2 &xy, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inUInt3_outULong2 (const UInt3 &xyz, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inUInt4_outULong2 (const UInt4 &xyzw, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inUInt_inUInt_outULong2 (const UInt &x, const UInt &y, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inLong_outULong2 (const Long &x, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inLong2_outULong2 (const Long2 &xy, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inLong3_outULong2 (const Long3 &xyz, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inLong4_outULong2 (const Long4 &xyzw, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inLong_inLong_outULong2 (const Long &x, const Long &y, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inULong_outULong2 (const ULong &x, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inULong3_outULong2 (const ULong3 &xyz, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inULong4_outULong2 (const ULong4 &xyzw, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inULong_inULong_outULong2 (const ULong &x, const ULong &y, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inFloat_outULong2 (const Float &x, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inFloat2_outULong2 (const Float2 &xy, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inFloat3_outULong2 (const Float3 &xyz, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inFloat4_outULong2 (const Float4 &xyzw, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inFloat_inFloat_outULong2 (const Float &x, const Float &y, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inDouble_outULong2 (const Double &x, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inDouble2_outULong2 (const Double2 &xy, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inDouble3_outULong2 (const Double3 &xyz, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inDouble4_outULong2 (const Double4 &xyzw, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong2Ctor_inDouble_inDouble_outULong2 (const Double &x, const Double &y, OUT ULong2 &out)
	{
		FunctionNode	func( "GL4.ulong2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inBool_outULong3 (const Bool &x, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inBool2_outULong3 (const Bool2 &xy, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inBool3_outULong3 (const Bool3 &xyz, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inBool4_outULong3 (const Bool4 &xyzw, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inBool2_inBool_outULong3 (const Bool2 &xy, const Bool &z, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inBool_inBool2_outULong3 (const Bool &x, const Bool2 &yz, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inBool_inBool_inBool_outULong3 (const Bool &x, const Bool &y, const Bool &z, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inInt_outULong3 (const Int &x, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inInt2_outULong3 (const Int2 &xy, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inInt3_outULong3 (const Int3 &xyz, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inInt4_outULong3 (const Int4 &xyzw, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inInt2_inInt_outULong3 (const Int2 &xy, const Int &z, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inInt_inInt2_outULong3 (const Int &x, const Int2 &yz, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inInt_inInt_inInt_outULong3 (const Int &x, const Int &y, const Int &z, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inUInt_outULong3 (const UInt &x, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inUInt2_outULong3 (const UInt2 &xy, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inUInt3_outULong3 (const UInt3 &xyz, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inUInt4_outULong3 (const UInt4 &xyzw, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inUInt2_inUInt_outULong3 (const UInt2 &xy, const UInt &z, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inUInt_inUInt2_outULong3 (const UInt &x, const UInt2 &yz, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inUInt_inUInt_inUInt_outULong3 (const UInt &x, const UInt &y, const UInt &z, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inLong_outULong3 (const Long &x, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inLong2_outULong3 (const Long2 &xy, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inLong3_outULong3 (const Long3 &xyz, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inLong4_outULong3 (const Long4 &xyzw, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inLong2_inLong_outULong3 (const Long2 &xy, const Long &z, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inLong_inLong2_outULong3 (const Long &x, const Long2 &yz, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inLong_inLong_inLong_outULong3 (const Long &x, const Long &y, const Long &z, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inULong_outULong3 (const ULong &x, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inULong2_outULong3 (const ULong2 &xy, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inULong4_outULong3 (const ULong4 &xyzw, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inULong2_inULong_outULong3 (const ULong2 &xy, const ULong &z, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inULong_inULong2_outULong3 (const ULong &x, const ULong2 &yz, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inULong_inULong_inULong_outULong3 (const ULong &x, const ULong &y, const ULong &z, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inFloat_outULong3 (const Float &x, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inFloat2_outULong3 (const Float2 &xy, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inFloat3_outULong3 (const Float3 &xyz, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inFloat4_outULong3 (const Float4 &xyzw, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inFloat2_inFloat_outULong3 (const Float2 &xy, const Float &z, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inFloat_inFloat2_outULong3 (const Float &x, const Float2 &yz, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inFloat_inFloat_inFloat_outULong3 (const Float &x, const Float &y, const Float &z, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inDouble_outULong3 (const Double &x, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inDouble2_outULong3 (const Double2 &xy, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inDouble3_outULong3 (const Double3 &xyz, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inDouble4_outULong3 (const Double4 &xyzw, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inDouble2_inDouble_outULong3 (const Double2 &xy, const Double &z, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inDouble_inDouble2_outULong3 (const Double &x, const Double2 &yz, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong3Ctor_inDouble_inDouble_inDouble_outULong3 (const Double &x, const Double &y, const Double &z, OUT ULong3 &out)
	{
		FunctionNode	func( "GL4.ulong3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inBool_outULong4 (const Bool &x, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inBool2_outULong4 (const Bool2 &xy, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inBool3_outULong4 (const Bool3 &xyz, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inBool4_outULong4 (const Bool4 &xyzw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inBool3_inBool_outULong4 (const Bool3 &xyz, const Bool &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inBool2_inBool2_outULong4 (const Bool2 &xy, const Bool2 &zw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inBool_inBool3_outULong4 (const Bool &x, const Bool3 &yzw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inBool2_inBool_inBool_outULong4 (const Bool2 &xy, const Bool &z, const Bool &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inBool_inBool2_inBool_outULong4 (const Bool &x, const Bool2 &yz, const Bool &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inBool_inBool_inBool2_outULong4 (const Bool &x, const Bool &y, const Bool2 &zw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inBool_inBool_inBool_inBool_outULong4 (const Bool &x, const Bool &y, const Bool &z, const Bool &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inInt_outULong4 (const Int &x, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inInt2_outULong4 (const Int2 &xy, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inInt3_outULong4 (const Int3 &xyz, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inInt4_outULong4 (const Int4 &xyzw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inInt3_inInt_outULong4 (const Int3 &xyz, const Int &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inInt2_inInt2_outULong4 (const Int2 &xy, const Int2 &zw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inInt_inInt3_outULong4 (const Int &x, const Int3 &yzw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inInt2_inInt_inInt_outULong4 (const Int2 &xy, const Int &z, const Int &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inInt_inInt2_inInt_outULong4 (const Int &x, const Int2 &yz, const Int &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inInt_inInt_inInt2_outULong4 (const Int &x, const Int &y, const Int2 &zw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inInt_inInt_inInt_inInt_outULong4 (const Int &x, const Int &y, const Int &z, const Int &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inUInt_outULong4 (const UInt &x, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inUInt2_outULong4 (const UInt2 &xy, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inUInt3_outULong4 (const UInt3 &xyz, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inUInt4_outULong4 (const UInt4 &xyzw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inUInt3_inUInt_outULong4 (const UInt3 &xyz, const UInt &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inUInt2_inUInt2_outULong4 (const UInt2 &xy, const UInt2 &zw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inUInt_inUInt3_outULong4 (const UInt &x, const UInt3 &yzw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inUInt2_inUInt_inUInt_outULong4 (const UInt2 &xy, const UInt &z, const UInt &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inUInt_inUInt2_inUInt_outULong4 (const UInt &x, const UInt2 &yz, const UInt &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inUInt_inUInt_inUInt2_outULong4 (const UInt &x, const UInt &y, const UInt2 &zw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inUInt_inUInt_inUInt_inUInt_outULong4 (const UInt &x, const UInt &y, const UInt &z, const UInt &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inLong_outULong4 (const Long &x, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inLong2_outULong4 (const Long2 &xy, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inLong3_outULong4 (const Long3 &xyz, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inLong4_outULong4 (const Long4 &xyzw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inLong3_inLong_outULong4 (const Long3 &xyz, const Long &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inLong2_inLong2_outULong4 (const Long2 &xy, const Long2 &zw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inLong_inLong3_outULong4 (const Long &x, const Long3 &yzw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inLong2_inLong_inLong_outULong4 (const Long2 &xy, const Long &z, const Long &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inLong_inLong2_inLong_outULong4 (const Long &x, const Long2 &yz, const Long &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inLong_inLong_inLong2_outULong4 (const Long &x, const Long &y, const Long2 &zw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inLong_inLong_inLong_inLong_outULong4 (const Long &x, const Long &y, const Long &z, const Long &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inULong_outULong4 (const ULong &x, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inULong2_outULong4 (const ULong2 &xy, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inULong3_outULong4 (const ULong3 &xyz, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inULong3_inULong_outULong4 (const ULong3 &xyz, const ULong &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inULong2_inULong2_outULong4 (const ULong2 &xy, const ULong2 &zw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inULong_inULong3_outULong4 (const ULong &x, const ULong3 &yzw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inULong2_inULong_inULong_outULong4 (const ULong2 &xy, const ULong &z, const ULong &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inULong_inULong2_inULong_outULong4 (const ULong &x, const ULong2 &yz, const ULong &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inULong_inULong_inULong2_outULong4 (const ULong &x, const ULong &y, const ULong2 &zw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inULong_inULong_inULong_inULong_outULong4 (const ULong &x, const ULong &y, const ULong &z, const ULong &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inFloat_outULong4 (const Float &x, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inFloat2_outULong4 (const Float2 &xy, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inFloat3_outULong4 (const Float3 &xyz, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inFloat4_outULong4 (const Float4 &xyzw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inFloat3_inFloat_outULong4 (const Float3 &xyz, const Float &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inFloat2_inFloat2_outULong4 (const Float2 &xy, const Float2 &zw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inFloat_inFloat3_outULong4 (const Float &x, const Float3 &yzw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inFloat2_inFloat_inFloat_outULong4 (const Float2 &xy, const Float &z, const Float &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inFloat_inFloat2_inFloat_outULong4 (const Float &x, const Float2 &yz, const Float &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inFloat_inFloat_inFloat2_outULong4 (const Float &x, const Float &y, const Float2 &zw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inFloat_inFloat_inFloat_inFloat_outULong4 (const Float &x, const Float &y, const Float &z, const Float &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inDouble_outULong4 (const Double &x, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inDouble2_outULong4 (const Double2 &xy, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inDouble3_outULong4 (const Double3 &xyz, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inDouble4_outULong4 (const Double4 &xyzw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inDouble3_inDouble_outULong4 (const Double3 &xyz, const Double &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inDouble2_inDouble2_outULong4 (const Double2 &xy, const Double2 &zw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inDouble_inDouble3_outULong4 (const Double &x, const Double3 &yzw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inDouble2_inDouble_inDouble_outULong4 (const Double2 &xy, const Double &z, const Double &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inDouble_inDouble2_inDouble_outULong4 (const Double &x, const Double2 &yz, const Double &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inDouble_inDouble_inDouble2_outULong4 (const Double &x, const Double &y, const Double2 &zw, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool ULong4Ctor_inDouble_inDouble_inDouble_inDouble_outULong4 (const Double &x, const Double &y, const Double &z, const Double &w, OUT ULong4 &out)
	{
		FunctionNode	func( "GL4.ulong4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool FloatCtor_inBool_outFloat (const Bool &x, OUT Float &out)
	{
		FunctionNode	func( "GL4.float", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool FloatCtor_inInt_outFloat (const Int &x, OUT Float &out)
	{
		FunctionNode	func( "GL4.float", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool FloatCtor_inUInt_outFloat (const UInt &x, OUT Float &out)
	{
		FunctionNode	func( "GL4.float", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool FloatCtor_inLong_outFloat (const Long &x, OUT Float &out)
	{
		FunctionNode	func( "GL4.float", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool FloatCtor_inULong_outFloat (const ULong &x, OUT Float &out)
	{
		FunctionNode	func( "GL4.float", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool FloatCtor_inDouble_outFloat (const Double &x, OUT Float &out)
	{
		FunctionNode	func( "GL4.float", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inBool_outFloat2 (const Bool &x, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inBool2_outFloat2 (const Bool2 &xy, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inBool3_outFloat2 (const Bool3 &xyz, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inBool4_outFloat2 (const Bool4 &xyzw, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inBool_inBool_outFloat2 (const Bool &x, const Bool &y, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inInt_outFloat2 (const Int &x, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inInt2_outFloat2 (const Int2 &xy, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inInt3_outFloat2 (const Int3 &xyz, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inInt4_outFloat2 (const Int4 &xyzw, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inInt_inInt_outFloat2 (const Int &x, const Int &y, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inUInt_outFloat2 (const UInt &x, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inUInt2_outFloat2 (const UInt2 &xy, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inUInt3_outFloat2 (const UInt3 &xyz, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inUInt4_outFloat2 (const UInt4 &xyzw, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inUInt_inUInt_outFloat2 (const UInt &x, const UInt &y, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inLong_outFloat2 (const Long &x, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inLong2_outFloat2 (const Long2 &xy, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inLong3_outFloat2 (const Long3 &xyz, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inLong4_outFloat2 (const Long4 &xyzw, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inLong_inLong_outFloat2 (const Long &x, const Long &y, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inULong_outFloat2 (const ULong &x, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inULong2_outFloat2 (const ULong2 &xy, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inULong3_outFloat2 (const ULong3 &xyz, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inULong4_outFloat2 (const ULong4 &xyzw, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inULong_inULong_outFloat2 (const ULong &x, const ULong &y, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inFloat_outFloat2 (const Float &x, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inFloat3_outFloat2 (const Float3 &xyz, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inFloat4_outFloat2 (const Float4 &xyzw, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inFloat_inFloat_outFloat2 (const Float &x, const Float &y, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inDouble_outFloat2 (const Double &x, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inDouble2_outFloat2 (const Double2 &xy, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inDouble3_outFloat2 (const Double3 &xyz, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inDouble4_outFloat2 (const Double4 &xyzw, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float2Ctor_inDouble_inDouble_outFloat2 (const Double &x, const Double &y, OUT Float2 &out)
	{
		FunctionNode	func( "GL4.float2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inBool_outFloat3 (const Bool &x, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inBool2_outFloat3 (const Bool2 &xy, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inBool3_outFloat3 (const Bool3 &xyz, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inBool4_outFloat3 (const Bool4 &xyzw, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inBool2_inBool_outFloat3 (const Bool2 &xy, const Bool &z, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inBool_inBool2_outFloat3 (const Bool &x, const Bool2 &yz, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inBool_inBool_inBool_outFloat3 (const Bool &x, const Bool &y, const Bool &z, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inInt_outFloat3 (const Int &x, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inInt2_outFloat3 (const Int2 &xy, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inInt3_outFloat3 (const Int3 &xyz, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inInt4_outFloat3 (const Int4 &xyzw, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inInt2_inInt_outFloat3 (const Int2 &xy, const Int &z, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inInt_inInt2_outFloat3 (const Int &x, const Int2 &yz, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inInt_inInt_inInt_outFloat3 (const Int &x, const Int &y, const Int &z, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inUInt_outFloat3 (const UInt &x, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inUInt2_outFloat3 (const UInt2 &xy, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inUInt3_outFloat3 (const UInt3 &xyz, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inUInt4_outFloat3 (const UInt4 &xyzw, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inUInt2_inUInt_outFloat3 (const UInt2 &xy, const UInt &z, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inUInt_inUInt2_outFloat3 (const UInt &x, const UInt2 &yz, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inUInt_inUInt_inUInt_outFloat3 (const UInt &x, const UInt &y, const UInt &z, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inLong_outFloat3 (const Long &x, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inLong2_outFloat3 (const Long2 &xy, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inLong3_outFloat3 (const Long3 &xyz, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inLong4_outFloat3 (const Long4 &xyzw, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inLong2_inLong_outFloat3 (const Long2 &xy, const Long &z, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inLong_inLong2_outFloat3 (const Long &x, const Long2 &yz, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inLong_inLong_inLong_outFloat3 (const Long &x, const Long &y, const Long &z, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inULong_outFloat3 (const ULong &x, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inULong2_outFloat3 (const ULong2 &xy, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inULong3_outFloat3 (const ULong3 &xyz, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inULong4_outFloat3 (const ULong4 &xyzw, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inULong2_inULong_outFloat3 (const ULong2 &xy, const ULong &z, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inULong_inULong2_outFloat3 (const ULong &x, const ULong2 &yz, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inULong_inULong_inULong_outFloat3 (const ULong &x, const ULong &y, const ULong &z, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inFloat_outFloat3 (const Float &x, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inFloat2_outFloat3 (const Float2 &xy, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inFloat4_outFloat3 (const Float4 &xyzw, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inFloat2_inFloat_outFloat3 (const Float2 &xy, const Float &z, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inFloat_inFloat2_outFloat3 (const Float &x, const Float2 &yz, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inFloat_inFloat_inFloat_outFloat3 (const Float &x, const Float &y, const Float &z, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inDouble_outFloat3 (const Double &x, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inDouble2_outFloat3 (const Double2 &xy, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inDouble3_outFloat3 (const Double3 &xyz, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inDouble4_outFloat3 (const Double4 &xyzw, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inDouble2_inDouble_outFloat3 (const Double2 &xy, const Double &z, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inDouble_inDouble2_outFloat3 (const Double &x, const Double2 &yz, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Float3Ctor_inDouble_inDouble_inDouble_outFloat3 (const Double &x, const Double &y, const Double &z, OUT Float3 &out)
	{
		FunctionNode	func( "GL4.float3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inBool_outFloat4 (const Bool &x, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inBool2_outFloat4 (const Bool2 &xy, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inBool3_outFloat4 (const Bool3 &xyz, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inBool4_outFloat4 (const Bool4 &xyzw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inBool3_inBool_outFloat4 (const Bool3 &xyz, const Bool &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inBool2_inBool2_outFloat4 (const Bool2 &xy, const Bool2 &zw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inBool_inBool3_outFloat4 (const Bool &x, const Bool3 &yzw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inBool2_inBool_inBool_outFloat4 (const Bool2 &xy, const Bool &z, const Bool &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inBool_inBool2_inBool_outFloat4 (const Bool &x, const Bool2 &yz, const Bool &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inBool_inBool_inBool2_outFloat4 (const Bool &x, const Bool &y, const Bool2 &zw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inBool_inBool_inBool_inBool_outFloat4 (const Bool &x, const Bool &y, const Bool &z, const Bool &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inInt_outFloat4 (const Int &x, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inInt2_outFloat4 (const Int2 &xy, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inInt3_outFloat4 (const Int3 &xyz, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inInt4_outFloat4 (const Int4 &xyzw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inInt3_inInt_outFloat4 (const Int3 &xyz, const Int &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inInt2_inInt2_outFloat4 (const Int2 &xy, const Int2 &zw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inInt_inInt3_outFloat4 (const Int &x, const Int3 &yzw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inInt2_inInt_inInt_outFloat4 (const Int2 &xy, const Int &z, const Int &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inInt_inInt2_inInt_outFloat4 (const Int &x, const Int2 &yz, const Int &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inInt_inInt_inInt2_outFloat4 (const Int &x, const Int &y, const Int2 &zw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inInt_inInt_inInt_inInt_outFloat4 (const Int &x, const Int &y, const Int &z, const Int &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inUInt_outFloat4 (const UInt &x, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inUInt2_outFloat4 (const UInt2 &xy, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inUInt3_outFloat4 (const UInt3 &xyz, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inUInt4_outFloat4 (const UInt4 &xyzw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inUInt3_inUInt_outFloat4 (const UInt3 &xyz, const UInt &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inUInt2_inUInt2_outFloat4 (const UInt2 &xy, const UInt2 &zw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inUInt_inUInt3_outFloat4 (const UInt &x, const UInt3 &yzw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inUInt2_inUInt_inUInt_outFloat4 (const UInt2 &xy, const UInt &z, const UInt &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inUInt_inUInt2_inUInt_outFloat4 (const UInt &x, const UInt2 &yz, const UInt &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inUInt_inUInt_inUInt2_outFloat4 (const UInt &x, const UInt &y, const UInt2 &zw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inUInt_inUInt_inUInt_inUInt_outFloat4 (const UInt &x, const UInt &y, const UInt &z, const UInt &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inLong_outFloat4 (const Long &x, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inLong2_outFloat4 (const Long2 &xy, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inLong3_outFloat4 (const Long3 &xyz, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inLong4_outFloat4 (const Long4 &xyzw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inLong3_inLong_outFloat4 (const Long3 &xyz, const Long &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inLong2_inLong2_outFloat4 (const Long2 &xy, const Long2 &zw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inLong_inLong3_outFloat4 (const Long &x, const Long3 &yzw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inLong2_inLong_inLong_outFloat4 (const Long2 &xy, const Long &z, const Long &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inLong_inLong2_inLong_outFloat4 (const Long &x, const Long2 &yz, const Long &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inLong_inLong_inLong2_outFloat4 (const Long &x, const Long &y, const Long2 &zw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inLong_inLong_inLong_inLong_outFloat4 (const Long &x, const Long &y, const Long &z, const Long &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inULong_outFloat4 (const ULong &x, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inULong2_outFloat4 (const ULong2 &xy, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inULong3_outFloat4 (const ULong3 &xyz, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inULong4_outFloat4 (const ULong4 &xyzw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inULong3_inULong_outFloat4 (const ULong3 &xyz, const ULong &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inULong2_inULong2_outFloat4 (const ULong2 &xy, const ULong2 &zw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inULong_inULong3_outFloat4 (const ULong &x, const ULong3 &yzw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inULong2_inULong_inULong_outFloat4 (const ULong2 &xy, const ULong &z, const ULong &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inULong_inULong2_inULong_outFloat4 (const ULong &x, const ULong2 &yz, const ULong &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inULong_inULong_inULong2_outFloat4 (const ULong &x, const ULong &y, const ULong2 &zw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inULong_inULong_inULong_inULong_outFloat4 (const ULong &x, const ULong &y, const ULong &z, const ULong &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inFloat_outFloat4 (const Float &x, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inFloat2_outFloat4 (const Float2 &xy, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inFloat3_outFloat4 (const Float3 &xyz, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inFloat3_inFloat_outFloat4 (const Float3 &xyz, const Float &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inFloat2_inFloat2_outFloat4 (const Float2 &xy, const Float2 &zw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inFloat_inFloat3_outFloat4 (const Float &x, const Float3 &yzw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inFloat2_inFloat_inFloat_outFloat4 (const Float2 &xy, const Float &z, const Float &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inFloat_inFloat2_inFloat_outFloat4 (const Float &x, const Float2 &yz, const Float &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inFloat_inFloat_inFloat2_outFloat4 (const Float &x, const Float &y, const Float2 &zw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inFloat_inFloat_inFloat_inFloat_outFloat4 (const Float &x, const Float &y, const Float &z, const Float &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inDouble_outFloat4 (const Double &x, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inDouble2_outFloat4 (const Double2 &xy, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inDouble3_outFloat4 (const Double3 &xyz, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inDouble4_outFloat4 (const Double4 &xyzw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inDouble3_inDouble_outFloat4 (const Double3 &xyz, const Double &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inDouble2_inDouble2_outFloat4 (const Double2 &xy, const Double2 &zw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inDouble_inDouble3_outFloat4 (const Double &x, const Double3 &yzw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inDouble2_inDouble_inDouble_outFloat4 (const Double2 &xy, const Double &z, const Double &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inDouble_inDouble2_inDouble_outFloat4 (const Double &x, const Double2 &yz, const Double &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inDouble_inDouble_inDouble2_outFloat4 (const Double &x, const Double &y, const Double2 &zw, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Float4Ctor_inDouble_inDouble_inDouble_inDouble_outFloat4 (const Double &x, const Double &y, const Double &z, const Double &w, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.float4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool DoubleCtor_inBool_outDouble (const Bool &x, OUT Double &out)
	{
		FunctionNode	func( "GL4.double", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool DoubleCtor_inInt_outDouble (const Int &x, OUT Double &out)
	{
		FunctionNode	func( "GL4.double", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool DoubleCtor_inUInt_outDouble (const UInt &x, OUT Double &out)
	{
		FunctionNode	func( "GL4.double", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool DoubleCtor_inLong_outDouble (const Long &x, OUT Double &out)
	{
		FunctionNode	func( "GL4.double", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool DoubleCtor_inULong_outDouble (const ULong &x, OUT Double &out)
	{
		FunctionNode	func( "GL4.double", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool DoubleCtor_inFloat_outDouble (const Float &x, OUT Double &out)
	{
		FunctionNode	func( "GL4.double", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inBool_outDouble2 (const Bool &x, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inBool2_outDouble2 (const Bool2 &xy, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inBool3_outDouble2 (const Bool3 &xyz, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inBool4_outDouble2 (const Bool4 &xyzw, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inBool_inBool_outDouble2 (const Bool &x, const Bool &y, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inInt_outDouble2 (const Int &x, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inInt2_outDouble2 (const Int2 &xy, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inInt3_outDouble2 (const Int3 &xyz, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inInt4_outDouble2 (const Int4 &xyzw, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inInt_inInt_outDouble2 (const Int &x, const Int &y, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inUInt_outDouble2 (const UInt &x, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inUInt2_outDouble2 (const UInt2 &xy, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inUInt3_outDouble2 (const UInt3 &xyz, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inUInt4_outDouble2 (const UInt4 &xyzw, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inUInt_inUInt_outDouble2 (const UInt &x, const UInt &y, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inLong_outDouble2 (const Long &x, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inLong2_outDouble2 (const Long2 &xy, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inLong3_outDouble2 (const Long3 &xyz, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inLong4_outDouble2 (const Long4 &xyzw, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inLong_inLong_outDouble2 (const Long &x, const Long &y, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inULong_outDouble2 (const ULong &x, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inULong2_outDouble2 (const ULong2 &xy, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inULong3_outDouble2 (const ULong3 &xyz, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inULong4_outDouble2 (const ULong4 &xyzw, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inULong_inULong_outDouble2 (const ULong &x, const ULong &y, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inFloat_outDouble2 (const Float &x, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inFloat2_outDouble2 (const Float2 &xy, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inFloat3_outDouble2 (const Float3 &xyz, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inFloat4_outDouble2 (const Float4 &xyzw, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inFloat_inFloat_outDouble2 (const Float &x, const Float &y, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inDouble_outDouble2 (const Double &x, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inDouble3_outDouble2 (const Double3 &xyz, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inDouble4_outDouble2 (const Double4 &xyzw, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double2Ctor_inDouble_inDouble_outDouble2 (const Double &x, const Double &y, OUT Double2 &out)
	{
		FunctionNode	func( "GL4.double2", "", x, y );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inBool_outDouble3 (const Bool &x, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inBool2_outDouble3 (const Bool2 &xy, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inBool3_outDouble3 (const Bool3 &xyz, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inBool4_outDouble3 (const Bool4 &xyzw, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inBool2_inBool_outDouble3 (const Bool2 &xy, const Bool &z, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inBool_inBool2_outDouble3 (const Bool &x, const Bool2 &yz, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inBool_inBool_inBool_outDouble3 (const Bool &x, const Bool &y, const Bool &z, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inInt_outDouble3 (const Int &x, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inInt2_outDouble3 (const Int2 &xy, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inInt3_outDouble3 (const Int3 &xyz, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inInt4_outDouble3 (const Int4 &xyzw, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inInt2_inInt_outDouble3 (const Int2 &xy, const Int &z, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inInt_inInt2_outDouble3 (const Int &x, const Int2 &yz, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inInt_inInt_inInt_outDouble3 (const Int &x, const Int &y, const Int &z, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inUInt_outDouble3 (const UInt &x, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inUInt2_outDouble3 (const UInt2 &xy, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inUInt3_outDouble3 (const UInt3 &xyz, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inUInt4_outDouble3 (const UInt4 &xyzw, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inUInt2_inUInt_outDouble3 (const UInt2 &xy, const UInt &z, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inUInt_inUInt2_outDouble3 (const UInt &x, const UInt2 &yz, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inUInt_inUInt_inUInt_outDouble3 (const UInt &x, const UInt &y, const UInt &z, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inLong_outDouble3 (const Long &x, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inLong2_outDouble3 (const Long2 &xy, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inLong3_outDouble3 (const Long3 &xyz, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inLong4_outDouble3 (const Long4 &xyzw, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inLong2_inLong_outDouble3 (const Long2 &xy, const Long &z, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inLong_inLong2_outDouble3 (const Long &x, const Long2 &yz, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inLong_inLong_inLong_outDouble3 (const Long &x, const Long &y, const Long &z, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inULong_outDouble3 (const ULong &x, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inULong2_outDouble3 (const ULong2 &xy, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inULong3_outDouble3 (const ULong3 &xyz, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inULong4_outDouble3 (const ULong4 &xyzw, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inULong2_inULong_outDouble3 (const ULong2 &xy, const ULong &z, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inULong_inULong2_outDouble3 (const ULong &x, const ULong2 &yz, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inULong_inULong_inULong_outDouble3 (const ULong &x, const ULong &y, const ULong &z, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inFloat_outDouble3 (const Float &x, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inFloat2_outDouble3 (const Float2 &xy, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inFloat3_outDouble3 (const Float3 &xyz, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inFloat4_outDouble3 (const Float4 &xyzw, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inFloat2_inFloat_outDouble3 (const Float2 &xy, const Float &z, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inFloat_inFloat2_outDouble3 (const Float &x, const Float2 &yz, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inFloat_inFloat_inFloat_outDouble3 (const Float &x, const Float &y, const Float &z, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inDouble_outDouble3 (const Double &x, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inDouble2_outDouble3 (const Double2 &xy, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inDouble4_outDouble3 (const Double4 &xyzw, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inDouble2_inDouble_outDouble3 (const Double2 &xy, const Double &z, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", xy, z );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inDouble_inDouble2_outDouble3 (const Double &x, const Double2 &yz, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", x, yz );
		Replace( out, "out", func );
		return true;
	}

	bool Double3Ctor_inDouble_inDouble_inDouble_outDouble3 (const Double &x, const Double &y, const Double &z, OUT Double3 &out)
	{
		FunctionNode	func( "GL4.double3", "", x, y, z );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inBool_outDouble4 (const Bool &x, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inBool2_outDouble4 (const Bool2 &xy, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inBool3_outDouble4 (const Bool3 &xyz, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inBool4_outDouble4 (const Bool4 &xyzw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inBool3_inBool_outDouble4 (const Bool3 &xyz, const Bool &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inBool2_inBool2_outDouble4 (const Bool2 &xy, const Bool2 &zw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inBool_inBool3_outDouble4 (const Bool &x, const Bool3 &yzw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inBool2_inBool_inBool_outDouble4 (const Bool2 &xy, const Bool &z, const Bool &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inBool_inBool2_inBool_outDouble4 (const Bool &x, const Bool2 &yz, const Bool &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inBool_inBool_inBool2_outDouble4 (const Bool &x, const Bool &y, const Bool2 &zw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inBool_inBool_inBool_inBool_outDouble4 (const Bool &x, const Bool &y, const Bool &z, const Bool &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inInt_outDouble4 (const Int &x, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inInt2_outDouble4 (const Int2 &xy, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inInt3_outDouble4 (const Int3 &xyz, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inInt4_outDouble4 (const Int4 &xyzw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inInt3_inInt_outDouble4 (const Int3 &xyz, const Int &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inInt2_inInt2_outDouble4 (const Int2 &xy, const Int2 &zw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inInt_inInt3_outDouble4 (const Int &x, const Int3 &yzw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inInt2_inInt_inInt_outDouble4 (const Int2 &xy, const Int &z, const Int &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inInt_inInt2_inInt_outDouble4 (const Int &x, const Int2 &yz, const Int &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inInt_inInt_inInt2_outDouble4 (const Int &x, const Int &y, const Int2 &zw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inInt_inInt_inInt_inInt_outDouble4 (const Int &x, const Int &y, const Int &z, const Int &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inUInt_outDouble4 (const UInt &x, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inUInt2_outDouble4 (const UInt2 &xy, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inUInt3_outDouble4 (const UInt3 &xyz, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inUInt4_outDouble4 (const UInt4 &xyzw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inUInt3_inUInt_outDouble4 (const UInt3 &xyz, const UInt &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inUInt2_inUInt2_outDouble4 (const UInt2 &xy, const UInt2 &zw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inUInt_inUInt3_outDouble4 (const UInt &x, const UInt3 &yzw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inUInt2_inUInt_inUInt_outDouble4 (const UInt2 &xy, const UInt &z, const UInt &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inUInt_inUInt2_inUInt_outDouble4 (const UInt &x, const UInt2 &yz, const UInt &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inUInt_inUInt_inUInt2_outDouble4 (const UInt &x, const UInt &y, const UInt2 &zw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inUInt_inUInt_inUInt_inUInt_outDouble4 (const UInt &x, const UInt &y, const UInt &z, const UInt &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inLong_outDouble4 (const Long &x, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inLong2_outDouble4 (const Long2 &xy, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inLong3_outDouble4 (const Long3 &xyz, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inLong4_outDouble4 (const Long4 &xyzw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inLong3_inLong_outDouble4 (const Long3 &xyz, const Long &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inLong2_inLong2_outDouble4 (const Long2 &xy, const Long2 &zw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inLong_inLong3_outDouble4 (const Long &x, const Long3 &yzw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inLong2_inLong_inLong_outDouble4 (const Long2 &xy, const Long &z, const Long &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inLong_inLong2_inLong_outDouble4 (const Long &x, const Long2 &yz, const Long &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inLong_inLong_inLong2_outDouble4 (const Long &x, const Long &y, const Long2 &zw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inLong_inLong_inLong_inLong_outDouble4 (const Long &x, const Long &y, const Long &z, const Long &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inULong_outDouble4 (const ULong &x, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inULong2_outDouble4 (const ULong2 &xy, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inULong3_outDouble4 (const ULong3 &xyz, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inULong4_outDouble4 (const ULong4 &xyzw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inULong3_inULong_outDouble4 (const ULong3 &xyz, const ULong &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inULong2_inULong2_outDouble4 (const ULong2 &xy, const ULong2 &zw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inULong_inULong3_outDouble4 (const ULong &x, const ULong3 &yzw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inULong2_inULong_inULong_outDouble4 (const ULong2 &xy, const ULong &z, const ULong &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inULong_inULong2_inULong_outDouble4 (const ULong &x, const ULong2 &yz, const ULong &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inULong_inULong_inULong2_outDouble4 (const ULong &x, const ULong &y, const ULong2 &zw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inULong_inULong_inULong_inULong_outDouble4 (const ULong &x, const ULong &y, const ULong &z, const ULong &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inFloat_outDouble4 (const Float &x, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inFloat2_outDouble4 (const Float2 &xy, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inFloat3_outDouble4 (const Float3 &xyz, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inFloat4_outDouble4 (const Float4 &xyzw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xyzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inFloat3_inFloat_outDouble4 (const Float3 &xyz, const Float &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inFloat2_inFloat2_outDouble4 (const Float2 &xy, const Float2 &zw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inFloat_inFloat3_outDouble4 (const Float &x, const Float3 &yzw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inFloat2_inFloat_inFloat_outDouble4 (const Float2 &xy, const Float &z, const Float &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inFloat_inFloat2_inFloat_outDouble4 (const Float &x, const Float2 &yz, const Float &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inFloat_inFloat_inFloat2_outDouble4 (const Float &x, const Float &y, const Float2 &zw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inFloat_inFloat_inFloat_inFloat_outDouble4 (const Float &x, const Float &y, const Float &z, const Float &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inDouble_outDouble4 (const Double &x, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inDouble2_outDouble4 (const Double2 &xy, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xy );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inDouble3_outDouble4 (const Double3 &xyz, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xyz );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inDouble3_inDouble_outDouble4 (const Double3 &xyz, const Double &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xyz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inDouble2_inDouble2_outDouble4 (const Double2 &xy, const Double2 &zw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xy, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inDouble_inDouble3_outDouble4 (const Double &x, const Double3 &yzw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, yzw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inDouble2_inDouble_inDouble_outDouble4 (const Double2 &xy, const Double &z, const Double &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", xy, z, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inDouble_inDouble2_inDouble_outDouble4 (const Double &x, const Double2 &yz, const Double &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, yz, w );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inDouble_inDouble_inDouble2_outDouble4 (const Double &x, const Double &y, const Double2 &zw, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, y, zw );
		Replace( out, "out", func );
		return true;
	}

	bool Double4Ctor_inDouble_inDouble_inDouble_inDouble_outDouble4 (const Double &x, const Double &y, const Double &z, const Double &w, OUT Double4 &out)
	{
		FunctionNode	func( "GL4.double4", "", x, y, z, w );
		Replace( out, "out", func );
		return true;
	}


	// All, Any
	bool All_inBool_outBool (const Bool &x, OUT Bool &out)
	{
		FunctionNode	func( "GL4.all", "bool All (const bool x)\n{\n\treturn x;\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool All_inBool2_outBool (const Bool2 &x, OUT Bool &out)
	{
		FunctionNode	func( "GL4.all", "bool All (const bool2 x)\n{\n\treturn all( x );\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool All_inBool3_outBool (const Bool3 &x, OUT Bool &out)
	{
		FunctionNode	func( "GL4.all", "bool All (const bool3 x)\n{\n\treturn all( x );\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool All_inBool4_outBool (const Bool4 &x, OUT Bool &out)
	{
		FunctionNode	func( "GL4.all", "bool All (const bool4 x)\n{\n\treturn all( x );\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Any_inBool_outBool (const Bool &x, OUT Bool &out)
	{
		FunctionNode	func( "GL4.any", "bool Any (const bool x)\n{\n\treturn x;\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Any_inBool2_outBool (const Bool2 &x, OUT Bool &out)
	{
		FunctionNode	func( "GL4.any", "bool Any (const bool2 x)\n{\n\treturn all( x );\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Any_inBool3_outBool (const Bool3 &x, OUT Bool &out)
	{
		FunctionNode	func( "GL4.any", "bool Any (const bool3 x)\n{\n\treturn all( x );\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}

	bool Any_inBool4_outBool (const Bool4 &x, OUT Bool &out)
	{
		FunctionNode	func( "GL4.any", "bool Any (const bool4 x)\n{\n\treturn all( x );\n}\n\n", x );
		Replace( out, "out", func );
		return true;
	}


	// ImageLoad, ImageStore
	bool ImageLoad_inImage1DReadOnlyInt4_inInt_outInt4 (const Image<ImageType1D, ReadOnly, Int4> &image, const Int &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage1DReadOnlyUInt4_inInt_outUInt4 (const Image<ImageType1D, ReadOnly, UInt4> &image, const Int &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage1DReadOnlyFloat4_inInt_outFloat4 (const Image<ImageType1D, ReadOnly, Float4> &image, const Int &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage1DCoherentInt4_inInt_outInt4 (const Image<ImageType1D, Coherent, Int4> &image, const Int &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage1DCoherentUInt4_inInt_outUInt4 (const Image<ImageType1D, Coherent, UInt4> &image, const Int &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage1DCoherentFloat4_inInt_outFloat4 (const Image<ImageType1D, Coherent, Float4> &image, const Int &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage1DVolatileInt4_inInt_outInt4 (const Image<ImageType1D, Volatile, Int4> &image, const Int &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage1DVolatileUInt4_inInt_outUInt4 (const Image<ImageType1D, Volatile, UInt4> &image, const Int &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage1DVolatileFloat4_inInt_outFloat4 (const Image<ImageType1D, Volatile, Float4> &image, const Int &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage1DRestrictInt4_inInt_outInt4 (const Image<ImageType1D, Restrict, Int4> &image, const Int &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage1DRestrictUInt4_inInt_outUInt4 (const Image<ImageType1D, Restrict, UInt4> &image, const Int &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage1DRestrictFloat4_inInt_outFloat4 (const Image<ImageType1D, Restrict, Float4> &image, const Int &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage1DAReadOnlyInt4_inInt2_outInt4 (const Image<ImageType1DArray, ReadOnly, Int4> &image, const Int2 &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage1DAReadOnlyUInt4_inInt2_outUInt4 (const Image<ImageType1DArray, ReadOnly, UInt4> &image, const Int2 &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage1DAReadOnlyFloat4_inInt2_outFloat4 (const Image<ImageType1DArray, ReadOnly, Float4> &image, const Int2 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage1DACoherentInt4_inInt2_outInt4 (const Image<ImageType1DArray, Coherent, Int4> &image, const Int2 &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage1DACoherentUInt4_inInt2_outUInt4 (const Image<ImageType1DArray, Coherent, UInt4> &image, const Int2 &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage1DACoherentFloat4_inInt2_outFloat4 (const Image<ImageType1DArray, Coherent, Float4> &image, const Int2 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage1DAVolatileInt4_inInt2_outInt4 (const Image<ImageType1DArray, Volatile, Int4> &image, const Int2 &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage1DAVolatileUInt4_inInt2_outUInt4 (const Image<ImageType1DArray, Volatile, UInt4> &image, const Int2 &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage1DAVolatileFloat4_inInt2_outFloat4 (const Image<ImageType1DArray, Volatile, Float4> &image, const Int2 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage1DARestrictInt4_inInt2_outInt4 (const Image<ImageType1DArray, Restrict, Int4> &image, const Int2 &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage1DARestrictUInt4_inInt2_outUInt4 (const Image<ImageType1DArray, Restrict, UInt4> &image, const Int2 &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage1DARestrictFloat4_inInt2_outFloat4 (const Image<ImageType1DArray, Restrict, Float4> &image, const Int2 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DReadOnlyInt4_inInt2_outInt4 (const Image<ImageType2D, ReadOnly, Int4> &image, const Int2 &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DReadOnlyUInt4_inInt2_outUInt4 (const Image<ImageType2D, ReadOnly, UInt4> &image, const Int2 &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DReadOnlyFloat4_inInt2_outFloat4 (const Image<ImageType2D, ReadOnly, Float4> &image, const Int2 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DCoherentInt4_inInt2_outInt4 (const Image<ImageType2D, Coherent, Int4> &image, const Int2 &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DCoherentUInt4_inInt2_outUInt4 (const Image<ImageType2D, Coherent, UInt4> &image, const Int2 &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DCoherentFloat4_inInt2_outFloat4 (const Image<ImageType2D, Coherent, Float4> &image, const Int2 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DVolatileInt4_inInt2_outInt4 (const Image<ImageType2D, Volatile, Int4> &image, const Int2 &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DVolatileUInt4_inInt2_outUInt4 (const Image<ImageType2D, Volatile, UInt4> &image, const Int2 &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DVolatileFloat4_inInt2_outFloat4 (const Image<ImageType2D, Volatile, Float4> &image, const Int2 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DRestrictInt4_inInt2_outInt4 (const Image<ImageType2D, Restrict, Int4> &image, const Int2 &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DRestrictUInt4_inInt2_outUInt4 (const Image<ImageType2D, Restrict, UInt4> &image, const Int2 &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DRestrictFloat4_inInt2_outFloat4 (const Image<ImageType2D, Restrict, Float4> &image, const Int2 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DAReadOnlyInt4_inInt3_outInt4 (const Image<ImageType2DArray, ReadOnly, Int4> &image, const Int3 &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DAReadOnlyUInt4_inInt3_outUInt4 (const Image<ImageType2DArray, ReadOnly, UInt4> &image, const Int3 &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DAReadOnlyFloat4_inInt3_outFloat4 (const Image<ImageType2DArray, ReadOnly, Float4> &image, const Int3 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DACoherentInt4_inInt3_outInt4 (const Image<ImageType2DArray, Coherent, Int4> &image, const Int3 &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DACoherentUInt4_inInt3_outUInt4 (const Image<ImageType2DArray, Coherent, UInt4> &image, const Int3 &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DACoherentFloat4_inInt3_outFloat4 (const Image<ImageType2DArray, Coherent, Float4> &image, const Int3 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DAVolatileInt4_inInt3_outInt4 (const Image<ImageType2DArray, Volatile, Int4> &image, const Int3 &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DAVolatileUInt4_inInt3_outUInt4 (const Image<ImageType2DArray, Volatile, UInt4> &image, const Int3 &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DAVolatileFloat4_inInt3_outFloat4 (const Image<ImageType2DArray, Volatile, Float4> &image, const Int3 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DARestrictInt4_inInt3_outInt4 (const Image<ImageType2DArray, Restrict, Int4> &image, const Int3 &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DARestrictUInt4_inInt3_outUInt4 (const Image<ImageType2DArray, Restrict, UInt4> &image, const Int3 &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DARestrictFloat4_inInt3_outFloat4 (const Image<ImageType2DArray, Restrict, Float4> &image, const Int3 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DMSReadOnlyInt4_inInt2_inInt_outInt4 (const Image<ImageType2DMS, ReadOnly, Int4> &image, const Int2 &P, const Int &sample, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P, sample );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DMSReadOnlyUInt4_inInt2_inInt_outUInt4 (const Image<ImageType2DMS, ReadOnly, UInt4> &image, const Int2 &P, const Int &sample, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P, sample );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DMSReadOnlyFloat4_inInt2_inInt_outFloat4 (const Image<ImageType2DMS, ReadOnly, Float4> &image, const Int2 &P, const Int &sample, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P, sample );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DMSCoherentInt4_inInt2_inInt_outInt4 (const Image<ImageType2DMS, Coherent, Int4> &image, const Int2 &P, const Int &sample, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P, sample );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DMSCoherentUInt4_inInt2_inInt_outUInt4 (const Image<ImageType2DMS, Coherent, UInt4> &image, const Int2 &P, const Int &sample, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P, sample );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DMSCoherentFloat4_inInt2_inInt_outFloat4 (const Image<ImageType2DMS, Coherent, Float4> &image, const Int2 &P, const Int &sample, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P, sample );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DMSVolatileInt4_inInt2_inInt_outInt4 (const Image<ImageType2DMS, Volatile, Int4> &image, const Int2 &P, const Int &sample, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P, sample );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DMSVolatileUInt4_inInt2_inInt_outUInt4 (const Image<ImageType2DMS, Volatile, UInt4> &image, const Int2 &P, const Int &sample, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P, sample );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DMSVolatileFloat4_inInt2_inInt_outFloat4 (const Image<ImageType2DMS, Volatile, Float4> &image, const Int2 &P, const Int &sample, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P, sample );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DMSRestrictInt4_inInt2_inInt_outInt4 (const Image<ImageType2DMS, Restrict, Int4> &image, const Int2 &P, const Int &sample, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P, sample );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DMSRestrictUInt4_inInt2_inInt_outUInt4 (const Image<ImageType2DMS, Restrict, UInt4> &image, const Int2 &P, const Int &sample, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P, sample );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DMSRestrictFloat4_inInt2_inInt_outFloat4 (const Image<ImageType2DMS, Restrict, Float4> &image, const Int2 &P, const Int &sample, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P, sample );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DMSAReadOnlyInt4_inInt3_inInt_outInt4 (const Image<ImageType2DMSArray, ReadOnly, Int4> &image, const Int3 &P, const Int &sample, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P, sample );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DMSAReadOnlyUInt4_inInt3_inInt_outUInt4 (const Image<ImageType2DMSArray, ReadOnly, UInt4> &image, const Int3 &P, const Int &sample, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P, sample );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DMSAReadOnlyFloat4_inInt3_inInt_outFloat4 (const Image<ImageType2DMSArray, ReadOnly, Float4> &image, const Int3 &P, const Int &sample, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P, sample );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DMSACoherentInt4_inInt3_inInt_outInt4 (const Image<ImageType2DMSArray, Coherent, Int4> &image, const Int3 &P, const Int &sample, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P, sample );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DMSACoherentUInt4_inInt3_inInt_outUInt4 (const Image<ImageType2DMSArray, Coherent, UInt4> &image, const Int3 &P, const Int &sample, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P, sample );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DMSACoherentFloat4_inInt3_inInt_outFloat4 (const Image<ImageType2DMSArray, Coherent, Float4> &image, const Int3 &P, const Int &sample, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P, sample );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DMSAVolatileInt4_inInt3_inInt_outInt4 (const Image<ImageType2DMSArray, Volatile, Int4> &image, const Int3 &P, const Int &sample, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P, sample );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DMSAVolatileUInt4_inInt3_inInt_outUInt4 (const Image<ImageType2DMSArray, Volatile, UInt4> &image, const Int3 &P, const Int &sample, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P, sample );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DMSAVolatileFloat4_inInt3_inInt_outFloat4 (const Image<ImageType2DMSArray, Volatile, Float4> &image, const Int3 &P, const Int &sample, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P, sample );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DMSARestrictInt4_inInt3_inInt_outInt4 (const Image<ImageType2DMSArray, Restrict, Int4> &image, const Int3 &P, const Int &sample, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P, sample );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DMSARestrictUInt4_inInt3_inInt_outUInt4 (const Image<ImageType2DMSArray, Restrict, UInt4> &image, const Int3 &P, const Int &sample, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P, sample );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage2DMSARestrictFloat4_inInt3_inInt_outFloat4 (const Image<ImageType2DMSArray, Restrict, Float4> &image, const Int3 &P, const Int &sample, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P, sample );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImageCubeReadOnlyInt4_inInt3_outInt4 (const Image<ImageTypeCube, ReadOnly, Int4> &image, const Int3 &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImageCubeReadOnlyUInt4_inInt3_outUInt4 (const Image<ImageTypeCube, ReadOnly, UInt4> &image, const Int3 &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImageCubeReadOnlyFloat4_inInt3_outFloat4 (const Image<ImageTypeCube, ReadOnly, Float4> &image, const Int3 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImageCubeCoherentInt4_inInt3_outInt4 (const Image<ImageTypeCube, Coherent, Int4> &image, const Int3 &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImageCubeCoherentUInt4_inInt3_outUInt4 (const Image<ImageTypeCube, Coherent, UInt4> &image, const Int3 &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImageCubeCoherentFloat4_inInt3_outFloat4 (const Image<ImageTypeCube, Coherent, Float4> &image, const Int3 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImageCubeVolatileInt4_inInt3_outInt4 (const Image<ImageTypeCube, Volatile, Int4> &image, const Int3 &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImageCubeVolatileUInt4_inInt3_outUInt4 (const Image<ImageTypeCube, Volatile, UInt4> &image, const Int3 &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImageCubeVolatileFloat4_inInt3_outFloat4 (const Image<ImageTypeCube, Volatile, Float4> &image, const Int3 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImageCubeRestrictInt4_inInt3_outInt4 (const Image<ImageTypeCube, Restrict, Int4> &image, const Int3 &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImageCubeRestrictUInt4_inInt3_outUInt4 (const Image<ImageTypeCube, Restrict, UInt4> &image, const Int3 &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImageCubeRestrictFloat4_inInt3_outFloat4 (const Image<ImageTypeCube, Restrict, Float4> &image, const Int3 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImageCubeAReadOnlyInt4_inInt3_outInt4 (const Image<ImageTypeCubeArray, ReadOnly, Int4> &image, const Int3 &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImageCubeAReadOnlyUInt4_inInt3_outUInt4 (const Image<ImageTypeCubeArray, ReadOnly, UInt4> &image, const Int3 &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImageCubeAReadOnlyFloat4_inInt3_outFloat4 (const Image<ImageTypeCubeArray, ReadOnly, Float4> &image, const Int3 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImageCubeACoherentInt4_inInt3_outInt4 (const Image<ImageTypeCubeArray, Coherent, Int4> &image, const Int3 &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImageCubeACoherentUInt4_inInt3_outUInt4 (const Image<ImageTypeCubeArray, Coherent, UInt4> &image, const Int3 &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImageCubeACoherentFloat4_inInt3_outFloat4 (const Image<ImageTypeCubeArray, Coherent, Float4> &image, const Int3 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImageCubeAVolatileInt4_inInt3_outInt4 (const Image<ImageTypeCubeArray, Volatile, Int4> &image, const Int3 &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImageCubeAVolatileUInt4_inInt3_outUInt4 (const Image<ImageTypeCubeArray, Volatile, UInt4> &image, const Int3 &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImageCubeAVolatileFloat4_inInt3_outFloat4 (const Image<ImageTypeCubeArray, Volatile, Float4> &image, const Int3 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImageCubeARestrictInt4_inInt3_outInt4 (const Image<ImageTypeCubeArray, Restrict, Int4> &image, const Int3 &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImageCubeARestrictUInt4_inInt3_outUInt4 (const Image<ImageTypeCubeArray, Restrict, UInt4> &image, const Int3 &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImageCubeARestrictFloat4_inInt3_outFloat4 (const Image<ImageTypeCubeArray, Restrict, Float4> &image, const Int3 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage3DReadOnlyInt4_inInt3_outInt4 (const Image<ImageType3D, ReadOnly, Int4> &image, const Int3 &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage3DReadOnlyUInt4_inInt3_outUInt4 (const Image<ImageType3D, ReadOnly, UInt4> &image, const Int3 &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage3DReadOnlyFloat4_inInt3_outFloat4 (const Image<ImageType3D, ReadOnly, Float4> &image, const Int3 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage3DCoherentInt4_inInt3_outInt4 (const Image<ImageType3D, Coherent, Int4> &image, const Int3 &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage3DCoherentUInt4_inInt3_outUInt4 (const Image<ImageType3D, Coherent, UInt4> &image, const Int3 &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage3DCoherentFloat4_inInt3_outFloat4 (const Image<ImageType3D, Coherent, Float4> &image, const Int3 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage3DVolatileInt4_inInt3_outInt4 (const Image<ImageType3D, Volatile, Int4> &image, const Int3 &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage3DVolatileUInt4_inInt3_outUInt4 (const Image<ImageType3D, Volatile, UInt4> &image, const Int3 &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage3DVolatileFloat4_inInt3_outFloat4 (const Image<ImageType3D, Volatile, Float4> &image, const Int3 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage3DRestrictInt4_inInt3_outInt4 (const Image<ImageType3D, Restrict, Int4> &image, const Int3 &P, OUT Int4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage3DRestrictUInt4_inInt3_outUInt4 (const Image<ImageType3D, Restrict, UInt4> &image, const Int3 &P, OUT UInt4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageLoad_inImage3DRestrictFloat4_inInt3_outFloat4 (const Image<ImageType3D, Restrict, Float4> &image, const Int3 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.imageLoad", "", image, P );
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage1DWriteOnlyInt4_inInt_inInt4_outSync (const Image<ImageType1D, WriteOnly, Int4> &image, const Int &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage1DWriteOnlyUInt4_inInt_inUInt4_outSync (const Image<ImageType1D, WriteOnly, UInt4> &image, const Int &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage1DWriteOnlyFloat4_inInt_inFloat4_outSync (const Image<ImageType1D, WriteOnly, Float4> &image, const Int &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage1DCoherentInt4_inInt_inInt4_outSync (const Image<ImageType1D, Coherent, Int4> &image, const Int &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage1DCoherentUInt4_inInt_inUInt4_outSync (const Image<ImageType1D, Coherent, UInt4> &image, const Int &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage1DCoherentFloat4_inInt_inFloat4_outSync (const Image<ImageType1D, Coherent, Float4> &image, const Int &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage1DVolatileInt4_inInt_inInt4_outSync (const Image<ImageType1D, Volatile, Int4> &image, const Int &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage1DVolatileUInt4_inInt_inUInt4_outSync (const Image<ImageType1D, Volatile, UInt4> &image, const Int &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage1DVolatileFloat4_inInt_inFloat4_outSync (const Image<ImageType1D, Volatile, Float4> &image, const Int &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage1DRestrictInt4_inInt_inInt4_outSync (const Image<ImageType1D, Restrict, Int4> &image, const Int &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage1DRestrictUInt4_inInt_inUInt4_outSync (const Image<ImageType1D, Restrict, UInt4> &image, const Int &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage1DRestrictFloat4_inInt_inFloat4_outSync (const Image<ImageType1D, Restrict, Float4> &image, const Int &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage1DAWriteOnlyInt4_inInt2_inInt4_outSync (const Image<ImageType1DArray, WriteOnly, Int4> &image, const Int2 &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage1DAWriteOnlyUInt4_inInt2_inUInt4_outSync (const Image<ImageType1DArray, WriteOnly, UInt4> &image, const Int2 &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage1DAWriteOnlyFloat4_inInt2_inFloat4_outSync (const Image<ImageType1DArray, WriteOnly, Float4> &image, const Int2 &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage1DACoherentInt4_inInt2_inInt4_outSync (const Image<ImageType1DArray, Coherent, Int4> &image, const Int2 &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage1DACoherentUInt4_inInt2_inUInt4_outSync (const Image<ImageType1DArray, Coherent, UInt4> &image, const Int2 &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage1DACoherentFloat4_inInt2_inFloat4_outSync (const Image<ImageType1DArray, Coherent, Float4> &image, const Int2 &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage1DAVolatileInt4_inInt2_inInt4_outSync (const Image<ImageType1DArray, Volatile, Int4> &image, const Int2 &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage1DAVolatileUInt4_inInt2_inUInt4_outSync (const Image<ImageType1DArray, Volatile, UInt4> &image, const Int2 &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage1DAVolatileFloat4_inInt2_inFloat4_outSync (const Image<ImageType1DArray, Volatile, Float4> &image, const Int2 &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage1DARestrictInt4_inInt2_inInt4_outSync (const Image<ImageType1DArray, Restrict, Int4> &image, const Int2 &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage1DARestrictUInt4_inInt2_inUInt4_outSync (const Image<ImageType1DArray, Restrict, UInt4> &image, const Int2 &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage1DARestrictFloat4_inInt2_inFloat4_outSync (const Image<ImageType1DArray, Restrict, Float4> &image, const Int2 &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DWriteOnlyInt4_inInt2_inInt4_outSync (const Image<ImageType2D, WriteOnly, Int4> &image, const Int2 &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DWriteOnlyUInt4_inInt2_inUInt4_outSync (const Image<ImageType2D, WriteOnly, UInt4> &image, const Int2 &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DWriteOnlyFloat4_inInt2_inFloat4_outSync (const Image<ImageType2D, WriteOnly, Float4> &image, const Int2 &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DCoherentInt4_inInt2_inInt4_outSync (const Image<ImageType2D, Coherent, Int4> &image, const Int2 &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DCoherentUInt4_inInt2_inUInt4_outSync (const Image<ImageType2D, Coherent, UInt4> &image, const Int2 &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DCoherentFloat4_inInt2_inFloat4_outSync (const Image<ImageType2D, Coherent, Float4> &image, const Int2 &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DVolatileInt4_inInt2_inInt4_outSync (const Image<ImageType2D, Volatile, Int4> &image, const Int2 &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DVolatileUInt4_inInt2_inUInt4_outSync (const Image<ImageType2D, Volatile, UInt4> &image, const Int2 &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DVolatileFloat4_inInt2_inFloat4_outSync (const Image<ImageType2D, Volatile, Float4> &image, const Int2 &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DRestrictInt4_inInt2_inInt4_outSync (const Image<ImageType2D, Restrict, Int4> &image, const Int2 &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DRestrictUInt4_inInt2_inUInt4_outSync (const Image<ImageType2D, Restrict, UInt4> &image, const Int2 &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DRestrictFloat4_inInt2_inFloat4_outSync (const Image<ImageType2D, Restrict, Float4> &image, const Int2 &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DAWriteOnlyInt4_inInt3_inInt4_outSync (const Image<ImageType2DArray, WriteOnly, Int4> &image, const Int3 &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DAWriteOnlyUInt4_inInt3_inUInt4_outSync (const Image<ImageType2DArray, WriteOnly, UInt4> &image, const Int3 &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DAWriteOnlyFloat4_inInt3_inFloat4_outSync (const Image<ImageType2DArray, WriteOnly, Float4> &image, const Int3 &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DACoherentInt4_inInt3_inInt4_outSync (const Image<ImageType2DArray, Coherent, Int4> &image, const Int3 &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DACoherentUInt4_inInt3_inUInt4_outSync (const Image<ImageType2DArray, Coherent, UInt4> &image, const Int3 &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DACoherentFloat4_inInt3_inFloat4_outSync (const Image<ImageType2DArray, Coherent, Float4> &image, const Int3 &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DAVolatileInt4_inInt3_inInt4_outSync (const Image<ImageType2DArray, Volatile, Int4> &image, const Int3 &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DAVolatileUInt4_inInt3_inUInt4_outSync (const Image<ImageType2DArray, Volatile, UInt4> &image, const Int3 &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DAVolatileFloat4_inInt3_inFloat4_outSync (const Image<ImageType2DArray, Volatile, Float4> &image, const Int3 &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DARestrictInt4_inInt3_inInt4_outSync (const Image<ImageType2DArray, Restrict, Int4> &image, const Int3 &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DARestrictUInt4_inInt3_inUInt4_outSync (const Image<ImageType2DArray, Restrict, UInt4> &image, const Int3 &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DARestrictFloat4_inInt3_inFloat4_outSync (const Image<ImageType2DArray, Restrict, Float4> &image, const Int3 &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DMSWriteOnlyInt4_inInt2_inInt_inInt4_outSync (const Image<ImageType2DMS, WriteOnly, Int4> &image, const Int2 &P, const Int &sample, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, sample, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DMSWriteOnlyUInt4_inInt2_inInt_inUInt4_outSync (const Image<ImageType2DMS, WriteOnly, UInt4> &image, const Int2 &P, const Int &sample, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, sample, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DMSWriteOnlyFloat4_inInt2_inInt_inFloat4_outSync (const Image<ImageType2DMS, WriteOnly, Float4> &image, const Int2 &P, const Int &sample, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, sample, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DMSCoherentInt4_inInt2_inInt_inInt4_outSync (const Image<ImageType2DMS, Coherent, Int4> &image, const Int2 &P, const Int &sample, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, sample, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DMSCoherentUInt4_inInt2_inInt_inUInt4_outSync (const Image<ImageType2DMS, Coherent, UInt4> &image, const Int2 &P, const Int &sample, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, sample, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DMSCoherentFloat4_inInt2_inInt_inFloat4_outSync (const Image<ImageType2DMS, Coherent, Float4> &image, const Int2 &P, const Int &sample, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, sample, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DMSVolatileInt4_inInt2_inInt_inInt4_outSync (const Image<ImageType2DMS, Volatile, Int4> &image, const Int2 &P, const Int &sample, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, sample, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DMSVolatileUInt4_inInt2_inInt_inUInt4_outSync (const Image<ImageType2DMS, Volatile, UInt4> &image, const Int2 &P, const Int &sample, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, sample, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DMSVolatileFloat4_inInt2_inInt_inFloat4_outSync (const Image<ImageType2DMS, Volatile, Float4> &image, const Int2 &P, const Int &sample, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, sample, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DMSRestrictInt4_inInt2_inInt_inInt4_outSync (const Image<ImageType2DMS, Restrict, Int4> &image, const Int2 &P, const Int &sample, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, sample, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DMSRestrictUInt4_inInt2_inInt_inUInt4_outSync (const Image<ImageType2DMS, Restrict, UInt4> &image, const Int2 &P, const Int &sample, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, sample, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DMSRestrictFloat4_inInt2_inInt_inFloat4_outSync (const Image<ImageType2DMS, Restrict, Float4> &image, const Int2 &P, const Int &sample, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, sample, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DMSAWriteOnlyInt4_inInt3_inInt_inInt4_outSync (const Image<ImageType2DMSArray, WriteOnly, Int4> &image, const Int3 &P, const Int &sample, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, sample, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DMSAWriteOnlyUInt4_inInt3_inInt_inUInt4_outSync (const Image<ImageType2DMSArray, WriteOnly, UInt4> &image, const Int3 &P, const Int &sample, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, sample, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DMSAWriteOnlyFloat4_inInt3_inInt_inFloat4_outSync (const Image<ImageType2DMSArray, WriteOnly, Float4> &image, const Int3 &P, const Int &sample, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, sample, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DMSACoherentInt4_inInt3_inInt_inInt4_outSync (const Image<ImageType2DMSArray, Coherent, Int4> &image, const Int3 &P, const Int &sample, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, sample, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DMSACoherentUInt4_inInt3_inInt_inUInt4_outSync (const Image<ImageType2DMSArray, Coherent, UInt4> &image, const Int3 &P, const Int &sample, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, sample, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DMSACoherentFloat4_inInt3_inInt_inFloat4_outSync (const Image<ImageType2DMSArray, Coherent, Float4> &image, const Int3 &P, const Int &sample, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, sample, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DMSAVolatileInt4_inInt3_inInt_inInt4_outSync (const Image<ImageType2DMSArray, Volatile, Int4> &image, const Int3 &P, const Int &sample, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, sample, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DMSAVolatileUInt4_inInt3_inInt_inUInt4_outSync (const Image<ImageType2DMSArray, Volatile, UInt4> &image, const Int3 &P, const Int &sample, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, sample, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DMSAVolatileFloat4_inInt3_inInt_inFloat4_outSync (const Image<ImageType2DMSArray, Volatile, Float4> &image, const Int3 &P, const Int &sample, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, sample, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DMSARestrictInt4_inInt3_inInt_inInt4_outSync (const Image<ImageType2DMSArray, Restrict, Int4> &image, const Int3 &P, const Int &sample, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, sample, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DMSARestrictUInt4_inInt3_inInt_inUInt4_outSync (const Image<ImageType2DMSArray, Restrict, UInt4> &image, const Int3 &P, const Int &sample, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, sample, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage2DMSARestrictFloat4_inInt3_inInt_inFloat4_outSync (const Image<ImageType2DMSArray, Restrict, Float4> &image, const Int3 &P, const Int &sample, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, sample, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImageCubeWriteOnlyInt4_inInt3_inInt4_outSync (const Image<ImageTypeCube, WriteOnly, Int4> &image, const Int3 &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImageCubeWriteOnlyUInt4_inInt3_inUInt4_outSync (const Image<ImageTypeCube, WriteOnly, UInt4> &image, const Int3 &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImageCubeWriteOnlyFloat4_inInt3_inFloat4_outSync (const Image<ImageTypeCube, WriteOnly, Float4> &image, const Int3 &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImageCubeCoherentInt4_inInt3_inInt4_outSync (const Image<ImageTypeCube, Coherent, Int4> &image, const Int3 &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImageCubeCoherentUInt4_inInt3_inUInt4_outSync (const Image<ImageTypeCube, Coherent, UInt4> &image, const Int3 &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImageCubeCoherentFloat4_inInt3_inFloat4_outSync (const Image<ImageTypeCube, Coherent, Float4> &image, const Int3 &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImageCubeVolatileInt4_inInt3_inInt4_outSync (const Image<ImageTypeCube, Volatile, Int4> &image, const Int3 &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImageCubeVolatileUInt4_inInt3_inUInt4_outSync (const Image<ImageTypeCube, Volatile, UInt4> &image, const Int3 &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImageCubeVolatileFloat4_inInt3_inFloat4_outSync (const Image<ImageTypeCube, Volatile, Float4> &image, const Int3 &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImageCubeRestrictInt4_inInt3_inInt4_outSync (const Image<ImageTypeCube, Restrict, Int4> &image, const Int3 &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImageCubeRestrictUInt4_inInt3_inUInt4_outSync (const Image<ImageTypeCube, Restrict, UInt4> &image, const Int3 &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImageCubeRestrictFloat4_inInt3_inFloat4_outSync (const Image<ImageTypeCube, Restrict, Float4> &image, const Int3 &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImageCubeAWriteOnlyInt4_inInt3_inInt4_outSync (const Image<ImageTypeCubeArray, WriteOnly, Int4> &image, const Int3 &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImageCubeAWriteOnlyUInt4_inInt3_inUInt4_outSync (const Image<ImageTypeCubeArray, WriteOnly, UInt4> &image, const Int3 &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImageCubeAWriteOnlyFloat4_inInt3_inFloat4_outSync (const Image<ImageTypeCubeArray, WriteOnly, Float4> &image, const Int3 &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImageCubeACoherentInt4_inInt3_inInt4_outSync (const Image<ImageTypeCubeArray, Coherent, Int4> &image, const Int3 &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImageCubeACoherentUInt4_inInt3_inUInt4_outSync (const Image<ImageTypeCubeArray, Coherent, UInt4> &image, const Int3 &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImageCubeACoherentFloat4_inInt3_inFloat4_outSync (const Image<ImageTypeCubeArray, Coherent, Float4> &image, const Int3 &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImageCubeAVolatileInt4_inInt3_inInt4_outSync (const Image<ImageTypeCubeArray, Volatile, Int4> &image, const Int3 &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImageCubeAVolatileUInt4_inInt3_inUInt4_outSync (const Image<ImageTypeCubeArray, Volatile, UInt4> &image, const Int3 &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImageCubeAVolatileFloat4_inInt3_inFloat4_outSync (const Image<ImageTypeCubeArray, Volatile, Float4> &image, const Int3 &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImageCubeARestrictInt4_inInt3_inInt4_outSync (const Image<ImageTypeCubeArray, Restrict, Int4> &image, const Int3 &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImageCubeARestrictUInt4_inInt3_inUInt4_outSync (const Image<ImageTypeCubeArray, Restrict, UInt4> &image, const Int3 &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImageCubeARestrictFloat4_inInt3_inFloat4_outSync (const Image<ImageTypeCubeArray, Restrict, Float4> &image, const Int3 &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage3DWriteOnlyInt4_inInt3_inInt4_outSync (const Image<ImageType3D, WriteOnly, Int4> &image, const Int3 &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage3DWriteOnlyUInt4_inInt3_inUInt4_outSync (const Image<ImageType3D, WriteOnly, UInt4> &image, const Int3 &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage3DWriteOnlyFloat4_inInt3_inFloat4_outSync (const Image<ImageType3D, WriteOnly, Float4> &image, const Int3 &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage3DCoherentInt4_inInt3_inInt4_outSync (const Image<ImageType3D, Coherent, Int4> &image, const Int3 &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage3DCoherentUInt4_inInt3_inUInt4_outSync (const Image<ImageType3D, Coherent, UInt4> &image, const Int3 &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage3DCoherentFloat4_inInt3_inFloat4_outSync (const Image<ImageType3D, Coherent, Float4> &image, const Int3 &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage3DVolatileInt4_inInt3_inInt4_outSync (const Image<ImageType3D, Volatile, Int4> &image, const Int3 &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage3DVolatileUInt4_inInt3_inUInt4_outSync (const Image<ImageType3D, Volatile, UInt4> &image, const Int3 &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage3DVolatileFloat4_inInt3_inFloat4_outSync (const Image<ImageType3D, Volatile, Float4> &image, const Int3 &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage3DRestrictInt4_inInt3_inInt4_outSync (const Image<ImageType3D, Restrict, Int4> &image, const Int3 &P, const Int4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage3DRestrictUInt4_inInt3_inUInt4_outSync (const Image<ImageType3D, Restrict, UInt4> &image, const Int3 &P, const UInt4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}

	bool ImageStore_inImage3DRestrictFloat4_inInt3_inFloat4_outSync (const Image<ImageType3D, Restrict, Float4> &image, const Int3 &P, const Float4 &data, OUT ShaderOutputNode &out)
	{
		FunctionNode	func( "GL4.imageStore", "", image, P, data);
		Replace( out, "out", func );
		return true;
	}


	// TextureSample, TextureSampleOffset
	bool TextureSample_inImage1DFloat4_inSampler_inFloat_outFloat4 (const Image<ImageType1D, ReadOnly, Float4> &image, const Sampler &sampler, const Float &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.texture", "", image, sampler, P);
		Replace( out, "out", func );
		return true;
	}

	bool TextureSampleOffset_inImage1DFloat4_inSampler_inFloat_inInt_outFloat4 (const Image<ImageType1D, ReadOnly, Float4> &image, const Sampler &sampler, const Float &P, const Int &offset, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.textureOffset", "", image, sampler, P, offset);
		Replace( out, "out", func );
		return true;
	}

	bool TextureSample_inImage1DAFloat4_inSampler_inFloat2_outFloat4 (const Image<ImageType1DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.texture", "", image, sampler, P);
		Replace( out, "out", func );
		return true;
	}

	bool TextureSampleOffset_inImage1DAFloat4_inSampler_inFloat2_inInt2_outFloat4 (const Image<ImageType1DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &P, const Int2 &offset, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.textureOffset", "", image, sampler, P, offset);
		Replace( out, "out", func );
		return true;
	}

	bool TextureSample_inImage2DFloat4_inSampler_inFloat2_outFloat4 (const Image<ImageType2D, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.texture", "", image, sampler, P);
		Replace( out, "out", func );
		return true;
	}

	bool TextureSampleOffset_inImage2DFloat4_inSampler_inFloat2_inInt2_outFloat4 (const Image<ImageType2D, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &P, const Int2 &offset, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.textureOffset", "", image, sampler, P, offset);
		Replace( out, "out", func );
		return true;
	}

	bool TextureSample_inImage2DAFloat4_inSampler_inFloat3_outFloat4 (const Image<ImageType2DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.texture", "", image, sampler, P);
		Replace( out, "out", func );
		return true;
	}

	bool TextureSampleOffset_inImage2DAFloat4_inSampler_inFloat3_inInt3_outFloat4 (const Image<ImageType2DArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &P, const Int3 &offset, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.textureOffset", "", image, sampler, P, offset);
		Replace( out, "out", func );
		return true;
	}

	bool TextureSample_inImage2DMSFloat4_inSampler_inFloat2_outFloat4 (const Image<ImageType2DMS, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.texture", "", image, sampler, P);
		Replace( out, "out", func );
		return true;
	}

	bool TextureSampleOffset_inImage2DMSFloat4_inSampler_inFloat2_inInt2_outFloat4 (const Image<ImageType2DMS, ReadOnly, Float4> &image, const Sampler &sampler, const Float2 &P, const Int2 &offset, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.textureOffset", "", image, sampler, P, offset);
		Replace( out, "out", func );
		return true;
	}

	bool TextureSample_inImage2DMSAFloat4_inSampler_inFloat3_outFloat4 (const Image<ImageType2DMSArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.texture", "", image, sampler, P);
		Replace( out, "out", func );
		return true;
	}

	bool TextureSampleOffset_inImage2DMSAFloat4_inSampler_inFloat3_inInt3_outFloat4 (const Image<ImageType2DMSArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &P, const Int3 &offset, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.textureOffset", "", image, sampler, P, offset);
		Replace( out, "out", func );
		return true;
	}

	bool TextureSample_inImageCubeFloat4_inSampler_inFloat3_outFloat4 (const Image<ImageTypeCube, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.texture", "", image, sampler, P);
		Replace( out, "out", func );
		return true;
	}

	bool TextureSampleOffset_inImageCubeFloat4_inSampler_inFloat3_inInt3_outFloat4 (const Image<ImageTypeCube, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &P, const Int3 &offset, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.textureOffset", "", image, sampler, P, offset);
		Replace( out, "out", func );
		return true;
	}

	bool TextureSample_inImageCubeAFloat4_inSampler_inFloat3_outFloat4 (const Image<ImageTypeCubeArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.texture", "", image, sampler, P);
		Replace( out, "out", func );
		return true;
	}

	bool TextureSampleOffset_inImageCubeAFloat4_inSampler_inFloat3_inInt3_outFloat4 (const Image<ImageTypeCubeArray, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &P, const Int3 &offset, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.textureOffset", "", image, sampler, P, offset);
		Replace( out, "out", func );
		return true;
	}

	bool TextureSample_inImage3DFloat4_inSampler_inFloat3_outFloat4 (const Image<ImageType3D, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &P, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.texture", "", image, sampler, P);
		Replace( out, "out", func );
		return true;
	}

	bool TextureSampleOffset_inImage3DFloat4_inSampler_inFloat3_inInt3_outFloat4 (const Image<ImageType3D, ReadOnly, Float4> &image, const Sampler &sampler, const Float3 &P, const Int3 &offset, OUT Float4 &out)
	{
		FunctionNode	func( "GL4.textureOffset", "", image, sampler, P, offset);
		Replace( out, "out", func );
		return true;
	}


	// global/group/local index/coord/size

	bool CS_GetGlobalIndexSize_outInt (OUT Int &out)
	{
		FunctionNode	func( "CS_GetGlobalIndexSize", "" );
		Replace( out, "globalIndexSize", func );
		return true;
	}

	bool CS_GetGlobalIndex_outInt (OUT Int &out)
	{
		FunctionNode	func( "CS_GetGlobalIndex", "" );
		Replace( out, "globalIndex", func );
		return true;
	}

	bool CS_GetLocalIndexSize_outInt (OUT Int &out)
	{
		FunctionNode	func( "CS_GetLocalIndexSize", "" );
		Replace( out, "localIndexSize", func );
		return true;
	}

	bool CS_GetLocalIndex_outInt (OUT Int &out)
	{
		FunctionNode	func( "CS_GetLocalIndex", "" );
		Replace( out, "localIndex", func );
		return true;
	}

	bool CS_GetGroupIndexSize_outInt (OUT Int &out)
	{
		FunctionNode	func( "CS_GetGroupIndexSize", "" );
		Replace( out, "groupIndexSize", func );
		return true;
	}

	bool CS_GetGroupIndex_outInt (OUT Int &out)
	{
		FunctionNode	func( "CS_GetGroupIndex", "" );
		Replace( out, "groupIndex", func );
		return true;
	}

	bool CS_GetGlobalCoordSize_outInt3 (OUT Int3 &out)
	{
		FunctionNode	func( "CS_GetGlobalCoordSize", "" );
		Replace( out, "globalCoordSize", func );
		return true;
	}

	bool CS_GetGlobalCoord_outInt3 (OUT Int3 &out)
	{
		FunctionNode	func( "CS_GetGlobalCoord", "" );
		Replace( out, "globalCoord", func );
		return true;
	}

	bool CS_GetLocalCoordSize_outInt3 (OUT Int3 &out)
	{
		FunctionNode	func( "CS_GetLocalCoordSize", "" );
		Replace( out, "localCoordSize", func );
		return true;
	}

	bool CS_GetLocalCoord_outInt3 (OUT Int3 &out)
	{
		FunctionNode	func( "CS_GetLocalCoord", "" );
		Replace( out, "localCoord", func );
		return true;
	}

	bool CS_GetGroupCoordSize_outInt3 (OUT Int3 &out)
	{
		FunctionNode	func( "CS_GetGroupCoordSize", "" );
		Replace( out, "groupCoordSize", func );
		return true;
	}

	bool CS_GetGroupCoord_outInt3 (OUT Int3 &out)
	{
		FunctionNode	func( "CS_GetGroupCoord", "" );
		Replace( out, "groupCoord", func );
		return true;
	}
	}	// GLSLShaderNodes


namespace _ShaderNodesHidden_
{

/*
=================================================
	_RegisterDefaultGLSLFunctions
=================================================
*/
void NodeFunctions::_RegisterDefaultGLSLFunctions ()
{		Register( "GL4.Not", &GLSLShaderNodes::Not_inBool_outBool );
		Register( "GL4.Not", &GLSLShaderNodes::Not_inBool2_outBool2 );
		Register( "GL4.Not", &GLSLShaderNodes::Not_inBool3_outBool3 );
		Register( "GL4.Not", &GLSLShaderNodes::Not_inBool4_outBool4 );
		Register( "GL4.BoolAnd", &GLSLShaderNodes::BoolAnd_inBool_inBool_outBool );
		Register( "GL4.BoolAnd", &GLSLShaderNodes::BoolAnd_inBool2_inBool2_outBool2 );
		Register( "GL4.BoolAnd", &GLSLShaderNodes::BoolAnd_inBool_inBool2_outBool2 );
		Register( "GL4.BoolAnd", &GLSLShaderNodes::BoolAnd_inBool2_inBool_outBool2 );
		Register( "GL4.BoolAnd", &GLSLShaderNodes::BoolAnd_inBool3_inBool3_outBool3 );
		Register( "GL4.BoolAnd", &GLSLShaderNodes::BoolAnd_inBool_inBool3_outBool3 );
		Register( "GL4.BoolAnd", &GLSLShaderNodes::BoolAnd_inBool3_inBool_outBool3 );
		Register( "GL4.BoolAnd", &GLSLShaderNodes::BoolAnd_inBool4_inBool4_outBool4 );
		Register( "GL4.BoolAnd", &GLSLShaderNodes::BoolAnd_inBool_inBool4_outBool4 );
		Register( "GL4.BoolAnd", &GLSLShaderNodes::BoolAnd_inBool4_inBool_outBool4 );
		Register( "GL4.BoolOr", &GLSLShaderNodes::BoolOr_inBool_inBool_outBool );
		Register( "GL4.BoolOr", &GLSLShaderNodes::BoolOr_inBool2_inBool2_outBool2 );
		Register( "GL4.BoolOr", &GLSLShaderNodes::BoolOr_inBool_inBool2_outBool2 );
		Register( "GL4.BoolOr", &GLSLShaderNodes::BoolOr_inBool2_inBool_outBool2 );
		Register( "GL4.BoolOr", &GLSLShaderNodes::BoolOr_inBool3_inBool3_outBool3 );
		Register( "GL4.BoolOr", &GLSLShaderNodes::BoolOr_inBool_inBool3_outBool3 );
		Register( "GL4.BoolOr", &GLSLShaderNodes::BoolOr_inBool3_inBool_outBool3 );
		Register( "GL4.BoolOr", &GLSLShaderNodes::BoolOr_inBool4_inBool4_outBool4 );
		Register( "GL4.BoolOr", &GLSLShaderNodes::BoolOr_inBool_inBool4_outBool4 );
		Register( "GL4.BoolOr", &GLSLShaderNodes::BoolOr_inBool4_inBool_outBool4 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inBool_inBool_outBool );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inBool2_inBool2_outBool2 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inBool_inBool2_outBool2 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inBool2_inBool_outBool2 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inBool3_inBool3_outBool3 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inBool_inBool3_outBool3 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inBool3_inBool_outBool3 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inBool4_inBool4_outBool4 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inBool_inBool4_outBool4 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inBool4_inBool_outBool4 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inBool_inBool_outBool );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inBool2_inBool2_outBool2 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inBool_inBool2_outBool2 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inBool2_inBool_outBool2 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inBool3_inBool3_outBool3 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inBool_inBool3_outBool3 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inBool3_inBool_outBool3 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inBool4_inBool4_outBool4 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inBool_inBool4_outBool4 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inBool4_inBool_outBool4 );
		Register( "GL4.BitInverse", &GLSLShaderNodes::BitInverse_inInt_outInt );
		Register( "GL4.BitInverse", &GLSLShaderNodes::BitInverse_inInt2_outInt2 );
		Register( "GL4.BitInverse", &GLSLShaderNodes::BitInverse_inInt3_outInt3 );
		Register( "GL4.BitInverse", &GLSLShaderNodes::BitInverse_inInt4_outInt4 );
		Register( "GL4.Plus", &GLSLShaderNodes::Plus_inInt_outInt );
		Register( "GL4.Plus", &GLSLShaderNodes::Plus_inInt2_outInt2 );
		Register( "GL4.Plus", &GLSLShaderNodes::Plus_inInt3_outInt3 );
		Register( "GL4.Plus", &GLSLShaderNodes::Plus_inInt4_outInt4 );
		Register( "GL4.Minus", &GLSLShaderNodes::Minus_inInt_outInt );
		Register( "GL4.Minus", &GLSLShaderNodes::Minus_inInt2_outInt2 );
		Register( "GL4.Minus", &GLSLShaderNodes::Minus_inInt3_outInt3 );
		Register( "GL4.Minus", &GLSLShaderNodes::Minus_inInt4_outInt4 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inInt_inInt_outInt );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inInt2_inInt2_outInt2 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inInt_inInt2_outInt2 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inInt2_inInt_outInt2 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inInt3_inInt3_outInt3 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inInt_inInt3_outInt3 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inInt3_inInt_outInt3 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inInt4_inInt4_outInt4 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inInt_inInt4_outInt4 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inInt4_inInt_outInt4 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inInt_inInt_outInt );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inInt2_inInt2_outInt2 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inInt_inInt2_outInt2 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inInt2_inInt_outInt2 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inInt3_inInt3_outInt3 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inInt_inInt3_outInt3 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inInt3_inInt_outInt3 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inInt4_inInt4_outInt4 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inInt_inInt4_outInt4 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inInt4_inInt_outInt4 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inInt_inInt_outInt );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inInt2_inInt2_outInt2 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inInt_inInt2_outInt2 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inInt2_inInt_outInt2 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inInt3_inInt3_outInt3 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inInt_inInt3_outInt3 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inInt3_inInt_outInt3 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inInt4_inInt4_outInt4 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inInt_inInt4_outInt4 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inInt4_inInt_outInt4 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inInt_inInt_outInt );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inInt2_inInt2_outInt2 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inInt_inInt2_outInt2 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inInt2_inInt_outInt2 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inInt3_inInt3_outInt3 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inInt_inInt3_outInt3 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inInt3_inInt_outInt3 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inInt4_inInt4_outInt4 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inInt_inInt4_outInt4 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inInt4_inInt_outInt4 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inInt_inInt_outInt );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inInt2_inInt2_outInt2 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inInt_inInt2_outInt2 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inInt2_inInt_outInt2 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inInt3_inInt3_outInt3 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inInt_inInt3_outInt3 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inInt3_inInt_outInt3 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inInt4_inInt4_outInt4 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inInt_inInt4_outInt4 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inInt4_inInt_outInt4 );
		Register( "GL4.And", &GLSLShaderNodes::And_inInt_inInt_outInt );
		Register( "GL4.And", &GLSLShaderNodes::And_inInt2_inInt2_outInt2 );
		Register( "GL4.And", &GLSLShaderNodes::And_inInt_inInt2_outInt2 );
		Register( "GL4.And", &GLSLShaderNodes::And_inInt2_inInt_outInt2 );
		Register( "GL4.And", &GLSLShaderNodes::And_inInt3_inInt3_outInt3 );
		Register( "GL4.And", &GLSLShaderNodes::And_inInt_inInt3_outInt3 );
		Register( "GL4.And", &GLSLShaderNodes::And_inInt3_inInt_outInt3 );
		Register( "GL4.And", &GLSLShaderNodes::And_inInt4_inInt4_outInt4 );
		Register( "GL4.And", &GLSLShaderNodes::And_inInt_inInt4_outInt4 );
		Register( "GL4.And", &GLSLShaderNodes::And_inInt4_inInt_outInt4 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inInt_inInt_outInt );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inInt2_inInt2_outInt2 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inInt_inInt2_outInt2 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inInt2_inInt_outInt2 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inInt3_inInt3_outInt3 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inInt_inInt3_outInt3 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inInt3_inInt_outInt3 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inInt4_inInt4_outInt4 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inInt_inInt4_outInt4 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inInt4_inInt_outInt4 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inInt_inInt_outInt );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inInt2_inInt2_outInt2 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inInt_inInt2_outInt2 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inInt2_inInt_outInt2 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inInt3_inInt3_outInt3 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inInt_inInt3_outInt3 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inInt3_inInt_outInt3 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inInt4_inInt4_outInt4 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inInt_inInt4_outInt4 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inInt4_inInt_outInt4 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inInt_inInt_outInt );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inInt2_inInt2_outInt2 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inInt_inInt2_outInt2 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inInt2_inInt_outInt2 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inInt3_inInt3_outInt3 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inInt_inInt3_outInt3 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inInt3_inInt_outInt3 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inInt4_inInt4_outInt4 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inInt_inInt4_outInt4 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inInt4_inInt_outInt4 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inInt_inInt_outInt );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inInt2_inInt2_outInt2 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inInt_inInt2_outInt2 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inInt2_inInt_outInt2 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inInt3_inInt3_outInt3 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inInt_inInt3_outInt3 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inInt3_inInt_outInt3 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inInt4_inInt4_outInt4 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inInt_inInt4_outInt4 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inInt4_inInt_outInt4 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inInt_inInt_outBool );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inInt2_inInt2_outBool2 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inInt_inInt2_outBool2 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inInt2_inInt_outBool2 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inInt3_inInt3_outBool3 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inInt_inInt3_outBool3 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inInt3_inInt_outBool3 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inInt4_inInt4_outBool4 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inInt_inInt4_outBool4 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inInt4_inInt_outBool4 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inInt_inInt_outBool );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inInt2_inInt2_outBool2 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inInt_inInt2_outBool2 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inInt2_inInt_outBool2 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inInt3_inInt3_outBool3 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inInt_inInt3_outBool3 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inInt3_inInt_outBool3 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inInt4_inInt4_outBool4 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inInt_inInt4_outBool4 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inInt4_inInt_outBool4 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inInt_inInt_outBool );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inInt2_inInt2_outBool2 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inInt_inInt2_outBool2 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inInt2_inInt_outBool2 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inInt3_inInt3_outBool3 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inInt_inInt3_outBool3 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inInt3_inInt_outBool3 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inInt4_inInt4_outBool4 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inInt_inInt4_outBool4 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inInt4_inInt_outBool4 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inInt_inInt_outBool );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inInt2_inInt2_outBool2 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inInt_inInt2_outBool2 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inInt2_inInt_outBool2 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inInt3_inInt3_outBool3 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inInt_inInt3_outBool3 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inInt3_inInt_outBool3 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inInt4_inInt4_outBool4 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inInt_inInt4_outBool4 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inInt4_inInt_outBool4 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inInt_inInt_outBool );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inInt2_inInt2_outBool2 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inInt_inInt2_outBool2 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inInt2_inInt_outBool2 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inInt3_inInt3_outBool3 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inInt_inInt3_outBool3 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inInt3_inInt_outBool3 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inInt4_inInt4_outBool4 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inInt_inInt4_outBool4 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inInt4_inInt_outBool4 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inInt_inInt_outBool );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inInt2_inInt2_outBool2 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inInt_inInt2_outBool2 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inInt2_inInt_outBool2 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inInt3_inInt3_outBool3 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inInt_inInt3_outBool3 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inInt3_inInt_outBool3 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inInt4_inInt4_outBool4 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inInt_inInt4_outBool4 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inInt4_inInt_outBool4 );
		Register( "GL4.BitInverse", &GLSLShaderNodes::BitInverse_inUInt_outUInt );
		Register( "GL4.BitInverse", &GLSLShaderNodes::BitInverse_inUInt2_outUInt2 );
		Register( "GL4.BitInverse", &GLSLShaderNodes::BitInverse_inUInt3_outUInt3 );
		Register( "GL4.BitInverse", &GLSLShaderNodes::BitInverse_inUInt4_outUInt4 );
		Register( "GL4.Plus", &GLSLShaderNodes::Plus_inUInt_outUInt );
		Register( "GL4.Plus", &GLSLShaderNodes::Plus_inUInt2_outUInt2 );
		Register( "GL4.Plus", &GLSLShaderNodes::Plus_inUInt3_outUInt3 );
		Register( "GL4.Plus", &GLSLShaderNodes::Plus_inUInt4_outUInt4 );
		Register( "GL4.Minus", &GLSLShaderNodes::Minus_inUInt_outUInt );
		Register( "GL4.Minus", &GLSLShaderNodes::Minus_inUInt2_outUInt2 );
		Register( "GL4.Minus", &GLSLShaderNodes::Minus_inUInt3_outUInt3 );
		Register( "GL4.Minus", &GLSLShaderNodes::Minus_inUInt4_outUInt4 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inUInt_inUInt_outUInt );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inUInt2_inUInt2_outUInt2 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inUInt_inUInt2_outUInt2 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inUInt2_inUInt_outUInt2 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inUInt3_inUInt3_outUInt3 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inUInt_inUInt3_outUInt3 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inUInt3_inUInt_outUInt3 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inUInt4_inUInt4_outUInt4 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inUInt_inUInt4_outUInt4 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inUInt4_inUInt_outUInt4 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inUInt_inUInt_outUInt );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inUInt2_inUInt2_outUInt2 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inUInt_inUInt2_outUInt2 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inUInt2_inUInt_outUInt2 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inUInt3_inUInt3_outUInt3 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inUInt_inUInt3_outUInt3 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inUInt3_inUInt_outUInt3 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inUInt4_inUInt4_outUInt4 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inUInt_inUInt4_outUInt4 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inUInt4_inUInt_outUInt4 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inUInt_inUInt_outUInt );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inUInt2_inUInt2_outUInt2 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inUInt_inUInt2_outUInt2 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inUInt2_inUInt_outUInt2 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inUInt3_inUInt3_outUInt3 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inUInt_inUInt3_outUInt3 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inUInt3_inUInt_outUInt3 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inUInt4_inUInt4_outUInt4 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inUInt_inUInt4_outUInt4 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inUInt4_inUInt_outUInt4 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inUInt_inUInt_outUInt );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inUInt2_inUInt2_outUInt2 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inUInt_inUInt2_outUInt2 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inUInt2_inUInt_outUInt2 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inUInt3_inUInt3_outUInt3 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inUInt_inUInt3_outUInt3 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inUInt3_inUInt_outUInt3 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inUInt4_inUInt4_outUInt4 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inUInt_inUInt4_outUInt4 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inUInt4_inUInt_outUInt4 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inUInt_inUInt_outUInt );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inUInt2_inUInt2_outUInt2 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inUInt_inUInt2_outUInt2 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inUInt2_inUInt_outUInt2 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inUInt3_inUInt3_outUInt3 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inUInt_inUInt3_outUInt3 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inUInt3_inUInt_outUInt3 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inUInt4_inUInt4_outUInt4 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inUInt_inUInt4_outUInt4 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inUInt4_inUInt_outUInt4 );
		Register( "GL4.And", &GLSLShaderNodes::And_inUInt_inUInt_outUInt );
		Register( "GL4.And", &GLSLShaderNodes::And_inUInt2_inUInt2_outUInt2 );
		Register( "GL4.And", &GLSLShaderNodes::And_inUInt_inUInt2_outUInt2 );
		Register( "GL4.And", &GLSLShaderNodes::And_inUInt2_inUInt_outUInt2 );
		Register( "GL4.And", &GLSLShaderNodes::And_inUInt3_inUInt3_outUInt3 );
		Register( "GL4.And", &GLSLShaderNodes::And_inUInt_inUInt3_outUInt3 );
		Register( "GL4.And", &GLSLShaderNodes::And_inUInt3_inUInt_outUInt3 );
		Register( "GL4.And", &GLSLShaderNodes::And_inUInt4_inUInt4_outUInt4 );
		Register( "GL4.And", &GLSLShaderNodes::And_inUInt_inUInt4_outUInt4 );
		Register( "GL4.And", &GLSLShaderNodes::And_inUInt4_inUInt_outUInt4 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inUInt_inUInt_outUInt );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inUInt2_inUInt2_outUInt2 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inUInt_inUInt2_outUInt2 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inUInt2_inUInt_outUInt2 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inUInt3_inUInt3_outUInt3 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inUInt_inUInt3_outUInt3 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inUInt3_inUInt_outUInt3 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inUInt4_inUInt4_outUInt4 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inUInt_inUInt4_outUInt4 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inUInt4_inUInt_outUInt4 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inUInt_inUInt_outUInt );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inUInt2_inUInt2_outUInt2 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inUInt_inUInt2_outUInt2 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inUInt2_inUInt_outUInt2 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inUInt3_inUInt3_outUInt3 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inUInt_inUInt3_outUInt3 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inUInt3_inUInt_outUInt3 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inUInt4_inUInt4_outUInt4 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inUInt_inUInt4_outUInt4 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inUInt4_inUInt_outUInt4 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inUInt_inUInt_outUInt );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inUInt2_inUInt2_outUInt2 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inUInt_inUInt2_outUInt2 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inUInt2_inUInt_outUInt2 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inUInt3_inUInt3_outUInt3 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inUInt_inUInt3_outUInt3 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inUInt3_inUInt_outUInt3 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inUInt4_inUInt4_outUInt4 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inUInt_inUInt4_outUInt4 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inUInt4_inUInt_outUInt4 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inUInt_inUInt_outUInt );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inUInt2_inUInt2_outUInt2 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inUInt_inUInt2_outUInt2 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inUInt2_inUInt_outUInt2 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inUInt3_inUInt3_outUInt3 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inUInt_inUInt3_outUInt3 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inUInt3_inUInt_outUInt3 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inUInt4_inUInt4_outUInt4 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inUInt_inUInt4_outUInt4 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inUInt4_inUInt_outUInt4 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inUInt_inUInt_outBool );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inUInt2_inUInt2_outBool2 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inUInt_inUInt2_outBool2 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inUInt2_inUInt_outBool2 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inUInt3_inUInt3_outBool3 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inUInt_inUInt3_outBool3 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inUInt3_inUInt_outBool3 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inUInt4_inUInt4_outBool4 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inUInt_inUInt4_outBool4 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inUInt4_inUInt_outBool4 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inUInt_inUInt_outBool );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inUInt2_inUInt2_outBool2 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inUInt_inUInt2_outBool2 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inUInt2_inUInt_outBool2 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inUInt3_inUInt3_outBool3 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inUInt_inUInt3_outBool3 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inUInt3_inUInt_outBool3 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inUInt4_inUInt4_outBool4 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inUInt_inUInt4_outBool4 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inUInt4_inUInt_outBool4 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inUInt_inUInt_outBool );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inUInt2_inUInt2_outBool2 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inUInt_inUInt2_outBool2 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inUInt2_inUInt_outBool2 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inUInt3_inUInt3_outBool3 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inUInt_inUInt3_outBool3 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inUInt3_inUInt_outBool3 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inUInt4_inUInt4_outBool4 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inUInt_inUInt4_outBool4 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inUInt4_inUInt_outBool4 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inUInt_inUInt_outBool );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inUInt2_inUInt2_outBool2 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inUInt_inUInt2_outBool2 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inUInt2_inUInt_outBool2 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inUInt3_inUInt3_outBool3 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inUInt_inUInt3_outBool3 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inUInt3_inUInt_outBool3 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inUInt4_inUInt4_outBool4 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inUInt_inUInt4_outBool4 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inUInt4_inUInt_outBool4 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inUInt_inUInt_outBool );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inUInt2_inUInt2_outBool2 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inUInt_inUInt2_outBool2 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inUInt2_inUInt_outBool2 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inUInt3_inUInt3_outBool3 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inUInt_inUInt3_outBool3 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inUInt3_inUInt_outBool3 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inUInt4_inUInt4_outBool4 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inUInt_inUInt4_outBool4 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inUInt4_inUInt_outBool4 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inUInt_inUInt_outBool );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inUInt2_inUInt2_outBool2 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inUInt_inUInt2_outBool2 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inUInt2_inUInt_outBool2 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inUInt3_inUInt3_outBool3 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inUInt_inUInt3_outBool3 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inUInt3_inUInt_outBool3 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inUInt4_inUInt4_outBool4 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inUInt_inUInt4_outBool4 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inUInt4_inUInt_outBool4 );
		Register( "GL4.BitInverse", &GLSLShaderNodes::BitInverse_inLong_outLong );
		Register( "GL4.BitInverse", &GLSLShaderNodes::BitInverse_inLong2_outLong2 );
		Register( "GL4.BitInverse", &GLSLShaderNodes::BitInverse_inLong3_outLong3 );
		Register( "GL4.BitInverse", &GLSLShaderNodes::BitInverse_inLong4_outLong4 );
		Register( "GL4.Plus", &GLSLShaderNodes::Plus_inLong_outLong );
		Register( "GL4.Plus", &GLSLShaderNodes::Plus_inLong2_outLong2 );
		Register( "GL4.Plus", &GLSLShaderNodes::Plus_inLong3_outLong3 );
		Register( "GL4.Plus", &GLSLShaderNodes::Plus_inLong4_outLong4 );
		Register( "GL4.Minus", &GLSLShaderNodes::Minus_inLong_outLong );
		Register( "GL4.Minus", &GLSLShaderNodes::Minus_inLong2_outLong2 );
		Register( "GL4.Minus", &GLSLShaderNodes::Minus_inLong3_outLong3 );
		Register( "GL4.Minus", &GLSLShaderNodes::Minus_inLong4_outLong4 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inLong_inLong_outLong );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inLong2_inLong2_outLong2 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inLong_inLong2_outLong2 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inLong2_inLong_outLong2 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inLong3_inLong3_outLong3 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inLong_inLong3_outLong3 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inLong3_inLong_outLong3 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inLong4_inLong4_outLong4 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inLong_inLong4_outLong4 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inLong4_inLong_outLong4 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inLong_inLong_outLong );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inLong2_inLong2_outLong2 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inLong_inLong2_outLong2 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inLong2_inLong_outLong2 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inLong3_inLong3_outLong3 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inLong_inLong3_outLong3 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inLong3_inLong_outLong3 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inLong4_inLong4_outLong4 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inLong_inLong4_outLong4 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inLong4_inLong_outLong4 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inLong_inLong_outLong );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inLong2_inLong2_outLong2 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inLong_inLong2_outLong2 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inLong2_inLong_outLong2 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inLong3_inLong3_outLong3 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inLong_inLong3_outLong3 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inLong3_inLong_outLong3 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inLong4_inLong4_outLong4 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inLong_inLong4_outLong4 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inLong4_inLong_outLong4 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inLong_inLong_outLong );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inLong2_inLong2_outLong2 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inLong_inLong2_outLong2 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inLong2_inLong_outLong2 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inLong3_inLong3_outLong3 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inLong_inLong3_outLong3 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inLong3_inLong_outLong3 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inLong4_inLong4_outLong4 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inLong_inLong4_outLong4 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inLong4_inLong_outLong4 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inLong_inLong_outLong );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inLong2_inLong2_outLong2 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inLong_inLong2_outLong2 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inLong2_inLong_outLong2 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inLong3_inLong3_outLong3 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inLong_inLong3_outLong3 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inLong3_inLong_outLong3 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inLong4_inLong4_outLong4 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inLong_inLong4_outLong4 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inLong4_inLong_outLong4 );
		Register( "GL4.And", &GLSLShaderNodes::And_inLong_inLong_outLong );
		Register( "GL4.And", &GLSLShaderNodes::And_inLong2_inLong2_outLong2 );
		Register( "GL4.And", &GLSLShaderNodes::And_inLong_inLong2_outLong2 );
		Register( "GL4.And", &GLSLShaderNodes::And_inLong2_inLong_outLong2 );
		Register( "GL4.And", &GLSLShaderNodes::And_inLong3_inLong3_outLong3 );
		Register( "GL4.And", &GLSLShaderNodes::And_inLong_inLong3_outLong3 );
		Register( "GL4.And", &GLSLShaderNodes::And_inLong3_inLong_outLong3 );
		Register( "GL4.And", &GLSLShaderNodes::And_inLong4_inLong4_outLong4 );
		Register( "GL4.And", &GLSLShaderNodes::And_inLong_inLong4_outLong4 );
		Register( "GL4.And", &GLSLShaderNodes::And_inLong4_inLong_outLong4 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inLong_inLong_outLong );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inLong2_inLong2_outLong2 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inLong_inLong2_outLong2 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inLong2_inLong_outLong2 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inLong3_inLong3_outLong3 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inLong_inLong3_outLong3 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inLong3_inLong_outLong3 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inLong4_inLong4_outLong4 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inLong_inLong4_outLong4 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inLong4_inLong_outLong4 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inLong_inLong_outLong );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inLong2_inLong2_outLong2 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inLong_inLong2_outLong2 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inLong2_inLong_outLong2 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inLong3_inLong3_outLong3 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inLong_inLong3_outLong3 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inLong3_inLong_outLong3 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inLong4_inLong4_outLong4 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inLong_inLong4_outLong4 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inLong4_inLong_outLong4 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inLong_inLong_outLong );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inLong2_inLong2_outLong2 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inLong_inLong2_outLong2 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inLong2_inLong_outLong2 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inLong3_inLong3_outLong3 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inLong_inLong3_outLong3 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inLong3_inLong_outLong3 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inLong4_inLong4_outLong4 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inLong_inLong4_outLong4 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inLong4_inLong_outLong4 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inLong_inLong_outLong );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inLong2_inLong2_outLong2 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inLong_inLong2_outLong2 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inLong2_inLong_outLong2 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inLong3_inLong3_outLong3 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inLong_inLong3_outLong3 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inLong3_inLong_outLong3 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inLong4_inLong4_outLong4 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inLong_inLong4_outLong4 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inLong4_inLong_outLong4 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inLong_inLong_outBool );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inLong2_inLong2_outBool2 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inLong_inLong2_outBool2 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inLong2_inLong_outBool2 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inLong3_inLong3_outBool3 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inLong_inLong3_outBool3 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inLong3_inLong_outBool3 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inLong4_inLong4_outBool4 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inLong_inLong4_outBool4 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inLong4_inLong_outBool4 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inLong_inLong_outBool );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inLong2_inLong2_outBool2 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inLong_inLong2_outBool2 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inLong2_inLong_outBool2 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inLong3_inLong3_outBool3 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inLong_inLong3_outBool3 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inLong3_inLong_outBool3 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inLong4_inLong4_outBool4 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inLong_inLong4_outBool4 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inLong4_inLong_outBool4 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inLong_inLong_outBool );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inLong2_inLong2_outBool2 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inLong_inLong2_outBool2 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inLong2_inLong_outBool2 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inLong3_inLong3_outBool3 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inLong_inLong3_outBool3 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inLong3_inLong_outBool3 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inLong4_inLong4_outBool4 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inLong_inLong4_outBool4 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inLong4_inLong_outBool4 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inLong_inLong_outBool );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inLong2_inLong2_outBool2 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inLong_inLong2_outBool2 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inLong2_inLong_outBool2 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inLong3_inLong3_outBool3 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inLong_inLong3_outBool3 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inLong3_inLong_outBool3 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inLong4_inLong4_outBool4 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inLong_inLong4_outBool4 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inLong4_inLong_outBool4 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inLong_inLong_outBool );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inLong2_inLong2_outBool2 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inLong_inLong2_outBool2 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inLong2_inLong_outBool2 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inLong3_inLong3_outBool3 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inLong_inLong3_outBool3 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inLong3_inLong_outBool3 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inLong4_inLong4_outBool4 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inLong_inLong4_outBool4 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inLong4_inLong_outBool4 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inLong_inLong_outBool );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inLong2_inLong2_outBool2 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inLong_inLong2_outBool2 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inLong2_inLong_outBool2 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inLong3_inLong3_outBool3 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inLong_inLong3_outBool3 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inLong3_inLong_outBool3 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inLong4_inLong4_outBool4 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inLong_inLong4_outBool4 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inLong4_inLong_outBool4 );
		Register( "GL4.BitInverse", &GLSLShaderNodes::BitInverse_inULong_outULong );
		Register( "GL4.BitInverse", &GLSLShaderNodes::BitInverse_inULong2_outULong2 );
		Register( "GL4.BitInverse", &GLSLShaderNodes::BitInverse_inULong3_outULong3 );
		Register( "GL4.BitInverse", &GLSLShaderNodes::BitInverse_inULong4_outULong4 );
		Register( "GL4.Plus", &GLSLShaderNodes::Plus_inULong_outULong );
		Register( "GL4.Plus", &GLSLShaderNodes::Plus_inULong2_outULong2 );
		Register( "GL4.Plus", &GLSLShaderNodes::Plus_inULong3_outULong3 );
		Register( "GL4.Plus", &GLSLShaderNodes::Plus_inULong4_outULong4 );
		Register( "GL4.Minus", &GLSLShaderNodes::Minus_inULong_outULong );
		Register( "GL4.Minus", &GLSLShaderNodes::Minus_inULong2_outULong2 );
		Register( "GL4.Minus", &GLSLShaderNodes::Minus_inULong3_outULong3 );
		Register( "GL4.Minus", &GLSLShaderNodes::Minus_inULong4_outULong4 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inULong_inULong_outULong );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inULong2_inULong2_outULong2 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inULong_inULong2_outULong2 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inULong2_inULong_outULong2 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inULong3_inULong3_outULong3 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inULong_inULong3_outULong3 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inULong3_inULong_outULong3 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inULong4_inULong4_outULong4 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inULong_inULong4_outULong4 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inULong4_inULong_outULong4 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inULong_inULong_outULong );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inULong2_inULong2_outULong2 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inULong_inULong2_outULong2 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inULong2_inULong_outULong2 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inULong3_inULong3_outULong3 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inULong_inULong3_outULong3 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inULong3_inULong_outULong3 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inULong4_inULong4_outULong4 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inULong_inULong4_outULong4 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inULong4_inULong_outULong4 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inULong_inULong_outULong );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inULong2_inULong2_outULong2 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inULong_inULong2_outULong2 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inULong2_inULong_outULong2 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inULong3_inULong3_outULong3 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inULong_inULong3_outULong3 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inULong3_inULong_outULong3 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inULong4_inULong4_outULong4 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inULong_inULong4_outULong4 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inULong4_inULong_outULong4 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inULong_inULong_outULong );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inULong2_inULong2_outULong2 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inULong_inULong2_outULong2 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inULong2_inULong_outULong2 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inULong3_inULong3_outULong3 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inULong_inULong3_outULong3 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inULong3_inULong_outULong3 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inULong4_inULong4_outULong4 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inULong_inULong4_outULong4 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inULong4_inULong_outULong4 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inULong_inULong_outULong );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inULong2_inULong2_outULong2 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inULong_inULong2_outULong2 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inULong2_inULong_outULong2 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inULong3_inULong3_outULong3 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inULong_inULong3_outULong3 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inULong3_inULong_outULong3 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inULong4_inULong4_outULong4 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inULong_inULong4_outULong4 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inULong4_inULong_outULong4 );
		Register( "GL4.And", &GLSLShaderNodes::And_inULong_inULong_outULong );
		Register( "GL4.And", &GLSLShaderNodes::And_inULong2_inULong2_outULong2 );
		Register( "GL4.And", &GLSLShaderNodes::And_inULong_inULong2_outULong2 );
		Register( "GL4.And", &GLSLShaderNodes::And_inULong2_inULong_outULong2 );
		Register( "GL4.And", &GLSLShaderNodes::And_inULong3_inULong3_outULong3 );
		Register( "GL4.And", &GLSLShaderNodes::And_inULong_inULong3_outULong3 );
		Register( "GL4.And", &GLSLShaderNodes::And_inULong3_inULong_outULong3 );
		Register( "GL4.And", &GLSLShaderNodes::And_inULong4_inULong4_outULong4 );
		Register( "GL4.And", &GLSLShaderNodes::And_inULong_inULong4_outULong4 );
		Register( "GL4.And", &GLSLShaderNodes::And_inULong4_inULong_outULong4 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inULong_inULong_outULong );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inULong2_inULong2_outULong2 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inULong_inULong2_outULong2 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inULong2_inULong_outULong2 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inULong3_inULong3_outULong3 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inULong_inULong3_outULong3 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inULong3_inULong_outULong3 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inULong4_inULong4_outULong4 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inULong_inULong4_outULong4 );
		Register( "GL4.Or", &GLSLShaderNodes::Or_inULong4_inULong_outULong4 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inULong_inULong_outULong );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inULong2_inULong2_outULong2 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inULong_inULong2_outULong2 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inULong2_inULong_outULong2 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inULong3_inULong3_outULong3 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inULong_inULong3_outULong3 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inULong3_inULong_outULong3 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inULong4_inULong4_outULong4 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inULong_inULong4_outULong4 );
		Register( "GL4.Xor", &GLSLShaderNodes::Xor_inULong4_inULong_outULong4 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inULong_inULong_outULong );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inULong2_inULong2_outULong2 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inULong_inULong2_outULong2 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inULong2_inULong_outULong2 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inULong3_inULong3_outULong3 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inULong_inULong3_outULong3 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inULong3_inULong_outULong3 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inULong4_inULong4_outULong4 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inULong_inULong4_outULong4 );
		Register( "GL4.BitLShift", &GLSLShaderNodes::BitLShift_inULong4_inULong_outULong4 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inULong_inULong_outULong );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inULong2_inULong2_outULong2 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inULong_inULong2_outULong2 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inULong2_inULong_outULong2 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inULong3_inULong3_outULong3 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inULong_inULong3_outULong3 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inULong3_inULong_outULong3 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inULong4_inULong4_outULong4 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inULong_inULong4_outULong4 );
		Register( "GL4.BitRShift", &GLSLShaderNodes::BitRShift_inULong4_inULong_outULong4 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inULong_inULong_outBool );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inULong2_inULong2_outBool2 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inULong_inULong2_outBool2 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inULong2_inULong_outBool2 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inULong3_inULong3_outBool3 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inULong_inULong3_outBool3 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inULong3_inULong_outBool3 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inULong4_inULong4_outBool4 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inULong_inULong4_outBool4 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inULong4_inULong_outBool4 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inULong_inULong_outBool );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inULong2_inULong2_outBool2 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inULong_inULong2_outBool2 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inULong2_inULong_outBool2 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inULong3_inULong3_outBool3 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inULong_inULong3_outBool3 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inULong3_inULong_outBool3 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inULong4_inULong4_outBool4 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inULong_inULong4_outBool4 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inULong4_inULong_outBool4 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inULong_inULong_outBool );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inULong2_inULong2_outBool2 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inULong_inULong2_outBool2 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inULong2_inULong_outBool2 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inULong3_inULong3_outBool3 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inULong_inULong3_outBool3 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inULong3_inULong_outBool3 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inULong4_inULong4_outBool4 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inULong_inULong4_outBool4 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inULong4_inULong_outBool4 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inULong_inULong_outBool );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inULong2_inULong2_outBool2 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inULong_inULong2_outBool2 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inULong2_inULong_outBool2 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inULong3_inULong3_outBool3 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inULong_inULong3_outBool3 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inULong3_inULong_outBool3 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inULong4_inULong4_outBool4 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inULong_inULong4_outBool4 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inULong4_inULong_outBool4 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inULong_inULong_outBool );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inULong2_inULong2_outBool2 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inULong_inULong2_outBool2 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inULong2_inULong_outBool2 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inULong3_inULong3_outBool3 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inULong_inULong3_outBool3 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inULong3_inULong_outBool3 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inULong4_inULong4_outBool4 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inULong_inULong4_outBool4 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inULong4_inULong_outBool4 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inULong_inULong_outBool );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inULong2_inULong2_outBool2 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inULong_inULong2_outBool2 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inULong2_inULong_outBool2 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inULong3_inULong3_outBool3 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inULong_inULong3_outBool3 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inULong3_inULong_outBool3 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inULong4_inULong4_outBool4 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inULong_inULong4_outBool4 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inULong4_inULong_outBool4 );
		Register( "GL4.Plus", &GLSLShaderNodes::Plus_inFloat_outFloat );
		Register( "GL4.Plus", &GLSLShaderNodes::Plus_inFloat2_outFloat2 );
		Register( "GL4.Plus", &GLSLShaderNodes::Plus_inFloat3_outFloat3 );
		Register( "GL4.Plus", &GLSLShaderNodes::Plus_inFloat4_outFloat4 );
		Register( "GL4.Minus", &GLSLShaderNodes::Minus_inFloat_outFloat );
		Register( "GL4.Minus", &GLSLShaderNodes::Minus_inFloat2_outFloat2 );
		Register( "GL4.Minus", &GLSLShaderNodes::Minus_inFloat3_outFloat3 );
		Register( "GL4.Minus", &GLSLShaderNodes::Minus_inFloat4_outFloat4 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inFloat_inFloat_outFloat );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inFloat2_inFloat2_outFloat2 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inFloat_inFloat2_outFloat2 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inFloat2_inFloat_outFloat2 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inFloat3_inFloat3_outFloat3 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inFloat_inFloat3_outFloat3 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inFloat3_inFloat_outFloat3 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inFloat4_inFloat4_outFloat4 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inFloat_inFloat4_outFloat4 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inFloat4_inFloat_outFloat4 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inFloat_inFloat_outFloat );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inFloat2_inFloat2_outFloat2 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inFloat_inFloat2_outFloat2 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inFloat2_inFloat_outFloat2 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inFloat3_inFloat3_outFloat3 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inFloat_inFloat3_outFloat3 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inFloat3_inFloat_outFloat3 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inFloat4_inFloat4_outFloat4 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inFloat_inFloat4_outFloat4 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inFloat4_inFloat_outFloat4 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inFloat_inFloat_outFloat );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inFloat2_inFloat2_outFloat2 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inFloat_inFloat2_outFloat2 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inFloat2_inFloat_outFloat2 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inFloat3_inFloat3_outFloat3 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inFloat_inFloat3_outFloat3 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inFloat3_inFloat_outFloat3 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inFloat4_inFloat4_outFloat4 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inFloat_inFloat4_outFloat4 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inFloat4_inFloat_outFloat4 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inFloat_inFloat_outFloat );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inFloat2_inFloat2_outFloat2 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inFloat_inFloat2_outFloat2 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inFloat2_inFloat_outFloat2 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inFloat3_inFloat3_outFloat3 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inFloat_inFloat3_outFloat3 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inFloat3_inFloat_outFloat3 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inFloat4_inFloat4_outFloat4 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inFloat_inFloat4_outFloat4 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inFloat4_inFloat_outFloat4 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inFloat_inFloat_outFloat );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inFloat2_inFloat2_outFloat2 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inFloat_inFloat2_outFloat2 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inFloat2_inFloat_outFloat2 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inFloat3_inFloat3_outFloat3 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inFloat_inFloat3_outFloat3 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inFloat3_inFloat_outFloat3 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inFloat4_inFloat4_outFloat4 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inFloat_inFloat4_outFloat4 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inFloat4_inFloat_outFloat4 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inFloat_inFloat_outBool );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inFloat2_inFloat2_outBool2 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inFloat_inFloat2_outBool2 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inFloat2_inFloat_outBool2 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inFloat3_inFloat3_outBool3 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inFloat_inFloat3_outBool3 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inFloat3_inFloat_outBool3 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inFloat4_inFloat4_outBool4 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inFloat_inFloat4_outBool4 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inFloat4_inFloat_outBool4 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inFloat_inFloat_outBool );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inFloat2_inFloat2_outBool2 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inFloat_inFloat2_outBool2 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inFloat2_inFloat_outBool2 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inFloat3_inFloat3_outBool3 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inFloat_inFloat3_outBool3 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inFloat3_inFloat_outBool3 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inFloat4_inFloat4_outBool4 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inFloat_inFloat4_outBool4 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inFloat4_inFloat_outBool4 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inFloat_inFloat_outBool );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inFloat2_inFloat2_outBool2 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inFloat_inFloat2_outBool2 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inFloat2_inFloat_outBool2 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inFloat3_inFloat3_outBool3 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inFloat_inFloat3_outBool3 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inFloat3_inFloat_outBool3 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inFloat4_inFloat4_outBool4 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inFloat_inFloat4_outBool4 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inFloat4_inFloat_outBool4 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inFloat_inFloat_outBool );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inFloat2_inFloat2_outBool2 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inFloat_inFloat2_outBool2 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inFloat2_inFloat_outBool2 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inFloat3_inFloat3_outBool3 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inFloat_inFloat3_outBool3 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inFloat3_inFloat_outBool3 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inFloat4_inFloat4_outBool4 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inFloat_inFloat4_outBool4 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inFloat4_inFloat_outBool4 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inFloat_inFloat_outBool );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inFloat2_inFloat2_outBool2 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inFloat_inFloat2_outBool2 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inFloat2_inFloat_outBool2 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inFloat3_inFloat3_outBool3 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inFloat_inFloat3_outBool3 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inFloat3_inFloat_outBool3 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inFloat4_inFloat4_outBool4 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inFloat_inFloat4_outBool4 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inFloat4_inFloat_outBool4 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inFloat_inFloat_outBool );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inFloat2_inFloat2_outBool2 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inFloat_inFloat2_outBool2 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inFloat2_inFloat_outBool2 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inFloat3_inFloat3_outBool3 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inFloat_inFloat3_outBool3 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inFloat3_inFloat_outBool3 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inFloat4_inFloat4_outBool4 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inFloat_inFloat4_outBool4 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inFloat4_inFloat_outBool4 );
		Register( "GL4.Plus", &GLSLShaderNodes::Plus_inDouble_outDouble );
		Register( "GL4.Plus", &GLSLShaderNodes::Plus_inDouble2_outDouble2 );
		Register( "GL4.Plus", &GLSLShaderNodes::Plus_inDouble3_outDouble3 );
		Register( "GL4.Plus", &GLSLShaderNodes::Plus_inDouble4_outDouble4 );
		Register( "GL4.Minus", &GLSLShaderNodes::Minus_inDouble_outDouble );
		Register( "GL4.Minus", &GLSLShaderNodes::Minus_inDouble2_outDouble2 );
		Register( "GL4.Minus", &GLSLShaderNodes::Minus_inDouble3_outDouble3 );
		Register( "GL4.Minus", &GLSLShaderNodes::Minus_inDouble4_outDouble4 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inDouble_inDouble_outDouble );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inDouble2_inDouble2_outDouble2 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inDouble_inDouble2_outDouble2 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inDouble2_inDouble_outDouble2 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inDouble3_inDouble3_outDouble3 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inDouble_inDouble3_outDouble3 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inDouble3_inDouble_outDouble3 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inDouble4_inDouble4_outDouble4 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inDouble_inDouble4_outDouble4 );
		Register( "GL4.Add", &GLSLShaderNodes::Add_inDouble4_inDouble_outDouble4 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inDouble_inDouble_outDouble );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inDouble2_inDouble2_outDouble2 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inDouble_inDouble2_outDouble2 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inDouble2_inDouble_outDouble2 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inDouble3_inDouble3_outDouble3 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inDouble_inDouble3_outDouble3 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inDouble3_inDouble_outDouble3 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inDouble4_inDouble4_outDouble4 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inDouble_inDouble4_outDouble4 );
		Register( "GL4.Sub", &GLSLShaderNodes::Sub_inDouble4_inDouble_outDouble4 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inDouble_inDouble_outDouble );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inDouble2_inDouble2_outDouble2 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inDouble_inDouble2_outDouble2 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inDouble2_inDouble_outDouble2 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inDouble3_inDouble3_outDouble3 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inDouble_inDouble3_outDouble3 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inDouble3_inDouble_outDouble3 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inDouble4_inDouble4_outDouble4 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inDouble_inDouble4_outDouble4 );
		Register( "GL4.Mul", &GLSLShaderNodes::Mul_inDouble4_inDouble_outDouble4 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inDouble_inDouble_outDouble );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inDouble2_inDouble2_outDouble2 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inDouble_inDouble2_outDouble2 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inDouble2_inDouble_outDouble2 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inDouble3_inDouble3_outDouble3 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inDouble_inDouble3_outDouble3 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inDouble3_inDouble_outDouble3 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inDouble4_inDouble4_outDouble4 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inDouble_inDouble4_outDouble4 );
		Register( "GL4.Div", &GLSLShaderNodes::Div_inDouble4_inDouble_outDouble4 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inDouble_inDouble_outDouble );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inDouble2_inDouble2_outDouble2 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inDouble_inDouble2_outDouble2 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inDouble2_inDouble_outDouble2 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inDouble3_inDouble3_outDouble3 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inDouble_inDouble3_outDouble3 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inDouble3_inDouble_outDouble3 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inDouble4_inDouble4_outDouble4 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inDouble_inDouble4_outDouble4 );
		Register( "GL4.Mod", &GLSLShaderNodes::Mod_inDouble4_inDouble_outDouble4 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inDouble_inDouble_outBool );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inDouble2_inDouble2_outBool2 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inDouble_inDouble2_outBool2 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inDouble2_inDouble_outBool2 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inDouble3_inDouble3_outBool3 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inDouble_inDouble3_outBool3 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inDouble3_inDouble_outBool3 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inDouble4_inDouble4_outBool4 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inDouble_inDouble4_outBool4 );
		Register( "GL4.Equal", &GLSLShaderNodes::Equal_inDouble4_inDouble_outBool4 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inDouble_inDouble_outBool );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inDouble2_inDouble2_outBool2 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inDouble_inDouble2_outBool2 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inDouble2_inDouble_outBool2 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inDouble3_inDouble3_outBool3 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inDouble_inDouble3_outBool3 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inDouble3_inDouble_outBool3 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inDouble4_inDouble4_outBool4 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inDouble_inDouble4_outBool4 );
		Register( "GL4.NotEqual", &GLSLShaderNodes::NotEqual_inDouble4_inDouble_outBool4 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inDouble_inDouble_outBool );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inDouble2_inDouble2_outBool2 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inDouble_inDouble2_outBool2 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inDouble2_inDouble_outBool2 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inDouble3_inDouble3_outBool3 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inDouble_inDouble3_outBool3 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inDouble3_inDouble_outBool3 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inDouble4_inDouble4_outBool4 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inDouble_inDouble4_outBool4 );
		Register( "GL4.GreaterThan", &GLSLShaderNodes::GreaterThan_inDouble4_inDouble_outBool4 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inDouble_inDouble_outBool );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inDouble2_inDouble2_outBool2 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inDouble_inDouble2_outBool2 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inDouble2_inDouble_outBool2 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inDouble3_inDouble3_outBool3 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inDouble_inDouble3_outBool3 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inDouble3_inDouble_outBool3 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inDouble4_inDouble4_outBool4 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inDouble_inDouble4_outBool4 );
		Register( "GL4.LessThan", &GLSLShaderNodes::LessThan_inDouble4_inDouble_outBool4 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inDouble_inDouble_outBool );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inDouble2_inDouble2_outBool2 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inDouble_inDouble2_outBool2 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inDouble2_inDouble_outBool2 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inDouble3_inDouble3_outBool3 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inDouble_inDouble3_outBool3 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inDouble3_inDouble_outBool3 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inDouble4_inDouble4_outBool4 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inDouble_inDouble4_outBool4 );
		Register( "GL4.GreaterThanEqual", &GLSLShaderNodes::GreaterThanEqual_inDouble4_inDouble_outBool4 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inDouble_inDouble_outBool );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inDouble2_inDouble2_outBool2 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inDouble_inDouble2_outBool2 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inDouble2_inDouble_outBool2 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inDouble3_inDouble3_outBool3 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inDouble_inDouble3_outBool3 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inDouble3_inDouble_outBool3 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inDouble4_inDouble4_outBool4 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inDouble_inDouble4_outBool4 );
		Register( "GL4.LessThanEqual", &GLSLShaderNodes::LessThanEqual_inDouble4_inDouble_outBool4 );
		Register( "GL4.Bool2Ctor", &GLSLShaderNodes::Bool2Ctor_inBool_outBool2 );
		Register( "GL4.Bool2Ctor", &GLSLShaderNodes::Bool2Ctor_inBool3_outBool2 );
		Register( "GL4.Bool2Ctor", &GLSLShaderNodes::Bool2Ctor_inBool4_outBool2 );
		Register( "GL4.Bool2Ctor", &GLSLShaderNodes::Bool2Ctor_inBool_inBool_outBool2 );
		Register( "GL4.Bool3Ctor", &GLSLShaderNodes::Bool3Ctor_inBool_outBool3 );
		Register( "GL4.Bool3Ctor", &GLSLShaderNodes::Bool3Ctor_inBool2_outBool3 );
		Register( "GL4.Bool3Ctor", &GLSLShaderNodes::Bool3Ctor_inBool4_outBool3 );
		Register( "GL4.Bool3Ctor", &GLSLShaderNodes::Bool3Ctor_inBool2_inBool_outBool3 );
		Register( "GL4.Bool3Ctor", &GLSLShaderNodes::Bool3Ctor_inBool_inBool2_outBool3 );
		Register( "GL4.Bool3Ctor", &GLSLShaderNodes::Bool3Ctor_inBool_inBool_inBool_outBool3 );
		Register( "GL4.Bool4Ctor", &GLSLShaderNodes::Bool4Ctor_inBool_outBool4 );
		Register( "GL4.Bool4Ctor", &GLSLShaderNodes::Bool4Ctor_inBool2_outBool4 );
		Register( "GL4.Bool4Ctor", &GLSLShaderNodes::Bool4Ctor_inBool3_outBool4 );
		Register( "GL4.Bool4Ctor", &GLSLShaderNodes::Bool4Ctor_inBool3_inBool_outBool4 );
		Register( "GL4.Bool4Ctor", &GLSLShaderNodes::Bool4Ctor_inBool2_inBool2_outBool4 );
		Register( "GL4.Bool4Ctor", &GLSLShaderNodes::Bool4Ctor_inBool_inBool3_outBool4 );
		Register( "GL4.Bool4Ctor", &GLSLShaderNodes::Bool4Ctor_inBool2_inBool_inBool_outBool4 );
		Register( "GL4.Bool4Ctor", &GLSLShaderNodes::Bool4Ctor_inBool_inBool2_inBool_outBool4 );
		Register( "GL4.Bool4Ctor", &GLSLShaderNodes::Bool4Ctor_inBool_inBool_inBool2_outBool4 );
		Register( "GL4.Bool4Ctor", &GLSLShaderNodes::Bool4Ctor_inBool_inBool_inBool_inBool_outBool4 );
		Register( "GL4.IntCtor", &GLSLShaderNodes::IntCtor_inBool_outInt );
		Register( "GL4.IntCtor", &GLSLShaderNodes::IntCtor_inUInt_outInt );
		Register( "GL4.IntCtor", &GLSLShaderNodes::IntCtor_inLong_outInt );
		Register( "GL4.IntCtor", &GLSLShaderNodes::IntCtor_inULong_outInt );
		Register( "GL4.IntCtor", &GLSLShaderNodes::IntCtor_inFloat_outInt );
		Register( "GL4.IntCtor", &GLSLShaderNodes::IntCtor_inDouble_outInt );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inBool_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inBool2_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inBool3_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inBool4_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inBool_inBool_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inInt_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inInt3_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inInt4_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inInt_inInt_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inUInt_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inUInt2_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inUInt3_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inUInt4_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inUInt_inUInt_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inLong_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inLong2_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inLong3_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inLong4_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inLong_inLong_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inULong_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inULong2_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inULong3_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inULong4_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inULong_inULong_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inFloat_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inFloat2_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inFloat3_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inFloat4_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inFloat_inFloat_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inDouble_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inDouble2_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inDouble3_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inDouble4_outInt2 );
		Register( "GL4.Int2Ctor", &GLSLShaderNodes::Int2Ctor_inDouble_inDouble_outInt2 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inBool_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inBool2_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inBool3_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inBool4_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inBool2_inBool_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inBool_inBool2_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inBool_inBool_inBool_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inInt_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inInt2_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inInt4_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inInt2_inInt_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inInt_inInt2_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inInt_inInt_inInt_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inUInt_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inUInt2_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inUInt3_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inUInt4_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inUInt2_inUInt_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inUInt_inUInt2_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inUInt_inUInt_inUInt_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inLong_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inLong2_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inLong3_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inLong4_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inLong2_inLong_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inLong_inLong2_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inLong_inLong_inLong_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inULong_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inULong2_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inULong3_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inULong4_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inULong2_inULong_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inULong_inULong2_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inULong_inULong_inULong_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inFloat_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inFloat2_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inFloat3_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inFloat4_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inFloat2_inFloat_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inFloat_inFloat2_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inFloat_inFloat_inFloat_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inDouble_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inDouble2_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inDouble3_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inDouble4_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inDouble2_inDouble_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inDouble_inDouble2_outInt3 );
		Register( "GL4.Int3Ctor", &GLSLShaderNodes::Int3Ctor_inDouble_inDouble_inDouble_outInt3 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inBool_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inBool2_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inBool3_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inBool4_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inBool3_inBool_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inBool2_inBool2_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inBool_inBool3_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inBool2_inBool_inBool_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inBool_inBool2_inBool_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inBool_inBool_inBool2_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inBool_inBool_inBool_inBool_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inInt_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inInt2_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inInt3_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inInt3_inInt_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inInt2_inInt2_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inInt_inInt3_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inInt2_inInt_inInt_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inInt_inInt2_inInt_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inInt_inInt_inInt2_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inInt_inInt_inInt_inInt_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inUInt_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inUInt2_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inUInt3_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inUInt4_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inUInt3_inUInt_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inUInt2_inUInt2_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inUInt_inUInt3_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inUInt2_inUInt_inUInt_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inUInt_inUInt2_inUInt_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inUInt_inUInt_inUInt2_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inUInt_inUInt_inUInt_inUInt_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inLong_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inLong2_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inLong3_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inLong4_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inLong3_inLong_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inLong2_inLong2_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inLong_inLong3_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inLong2_inLong_inLong_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inLong_inLong2_inLong_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inLong_inLong_inLong2_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inLong_inLong_inLong_inLong_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inULong_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inULong2_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inULong3_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inULong4_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inULong3_inULong_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inULong2_inULong2_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inULong_inULong3_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inULong2_inULong_inULong_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inULong_inULong2_inULong_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inULong_inULong_inULong2_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inULong_inULong_inULong_inULong_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inFloat_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inFloat2_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inFloat3_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inFloat4_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inFloat3_inFloat_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inFloat2_inFloat2_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inFloat_inFloat3_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inFloat2_inFloat_inFloat_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inFloat_inFloat2_inFloat_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inFloat_inFloat_inFloat2_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inFloat_inFloat_inFloat_inFloat_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inDouble_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inDouble2_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inDouble3_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inDouble4_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inDouble3_inDouble_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inDouble2_inDouble2_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inDouble_inDouble3_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inDouble2_inDouble_inDouble_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inDouble_inDouble2_inDouble_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inDouble_inDouble_inDouble2_outInt4 );
		Register( "GL4.Int4Ctor", &GLSLShaderNodes::Int4Ctor_inDouble_inDouble_inDouble_inDouble_outInt4 );
		Register( "GL4.UIntCtor", &GLSLShaderNodes::UIntCtor_inBool_outUInt );
		Register( "GL4.UIntCtor", &GLSLShaderNodes::UIntCtor_inInt_outUInt );
		Register( "GL4.UIntCtor", &GLSLShaderNodes::UIntCtor_inLong_outUInt );
		Register( "GL4.UIntCtor", &GLSLShaderNodes::UIntCtor_inULong_outUInt );
		Register( "GL4.UIntCtor", &GLSLShaderNodes::UIntCtor_inFloat_outUInt );
		Register( "GL4.UIntCtor", &GLSLShaderNodes::UIntCtor_inDouble_outUInt );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inBool_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inBool2_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inBool3_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inBool4_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inBool_inBool_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inInt_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inInt2_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inInt3_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inInt4_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inInt_inInt_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inUInt_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inUInt3_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inUInt4_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inUInt_inUInt_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inLong_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inLong2_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inLong3_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inLong4_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inLong_inLong_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inULong_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inULong2_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inULong3_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inULong4_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inULong_inULong_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inFloat_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inFloat2_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inFloat3_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inFloat4_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inFloat_inFloat_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inDouble_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inDouble2_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inDouble3_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inDouble4_outUInt2 );
		Register( "GL4.UInt2Ctor", &GLSLShaderNodes::UInt2Ctor_inDouble_inDouble_outUInt2 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inBool_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inBool2_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inBool3_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inBool4_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inBool2_inBool_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inBool_inBool2_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inBool_inBool_inBool_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inInt_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inInt2_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inInt3_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inInt4_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inInt2_inInt_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inInt_inInt2_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inInt_inInt_inInt_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inUInt_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inUInt2_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inUInt4_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inUInt2_inUInt_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inUInt_inUInt2_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inUInt_inUInt_inUInt_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inLong_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inLong2_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inLong3_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inLong4_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inLong2_inLong_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inLong_inLong2_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inLong_inLong_inLong_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inULong_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inULong2_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inULong3_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inULong4_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inULong2_inULong_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inULong_inULong2_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inULong_inULong_inULong_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inFloat_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inFloat2_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inFloat3_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inFloat4_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inFloat2_inFloat_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inFloat_inFloat2_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inFloat_inFloat_inFloat_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inDouble_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inDouble2_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inDouble3_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inDouble4_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inDouble2_inDouble_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inDouble_inDouble2_outUInt3 );
		Register( "GL4.UInt3Ctor", &GLSLShaderNodes::UInt3Ctor_inDouble_inDouble_inDouble_outUInt3 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inBool_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inBool2_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inBool3_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inBool4_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inBool3_inBool_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inBool2_inBool2_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inBool_inBool3_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inBool2_inBool_inBool_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inBool_inBool2_inBool_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inBool_inBool_inBool2_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inBool_inBool_inBool_inBool_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inInt_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inInt2_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inInt3_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inInt4_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inInt3_inInt_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inInt2_inInt2_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inInt_inInt3_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inInt2_inInt_inInt_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inInt_inInt2_inInt_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inInt_inInt_inInt2_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inInt_inInt_inInt_inInt_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inUInt_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inUInt2_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inUInt3_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inUInt3_inUInt_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inUInt2_inUInt2_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inUInt_inUInt3_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inUInt2_inUInt_inUInt_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inUInt_inUInt2_inUInt_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inUInt_inUInt_inUInt2_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inUInt_inUInt_inUInt_inUInt_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inLong_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inLong2_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inLong3_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inLong4_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inLong3_inLong_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inLong2_inLong2_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inLong_inLong3_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inLong2_inLong_inLong_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inLong_inLong2_inLong_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inLong_inLong_inLong2_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inLong_inLong_inLong_inLong_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inULong_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inULong2_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inULong3_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inULong4_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inULong3_inULong_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inULong2_inULong2_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inULong_inULong3_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inULong2_inULong_inULong_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inULong_inULong2_inULong_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inULong_inULong_inULong2_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inULong_inULong_inULong_inULong_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inFloat_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inFloat2_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inFloat3_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inFloat4_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inFloat3_inFloat_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inFloat2_inFloat2_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inFloat_inFloat3_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inFloat2_inFloat_inFloat_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inFloat_inFloat2_inFloat_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inFloat_inFloat_inFloat2_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inFloat_inFloat_inFloat_inFloat_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inDouble_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inDouble2_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inDouble3_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inDouble4_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inDouble3_inDouble_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inDouble2_inDouble2_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inDouble_inDouble3_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inDouble2_inDouble_inDouble_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inDouble_inDouble2_inDouble_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inDouble_inDouble_inDouble2_outUInt4 );
		Register( "GL4.UInt4Ctor", &GLSLShaderNodes::UInt4Ctor_inDouble_inDouble_inDouble_inDouble_outUInt4 );
		Register( "GL4.LongCtor", &GLSLShaderNodes::LongCtor_inBool_outLong );
		Register( "GL4.LongCtor", &GLSLShaderNodes::LongCtor_inInt_outLong );
		Register( "GL4.LongCtor", &GLSLShaderNodes::LongCtor_inUInt_outLong );
		Register( "GL4.LongCtor", &GLSLShaderNodes::LongCtor_inULong_outLong );
		Register( "GL4.LongCtor", &GLSLShaderNodes::LongCtor_inFloat_outLong );
		Register( "GL4.LongCtor", &GLSLShaderNodes::LongCtor_inDouble_outLong );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inBool_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inBool2_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inBool3_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inBool4_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inBool_inBool_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inInt_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inInt2_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inInt3_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inInt4_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inInt_inInt_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inUInt_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inUInt2_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inUInt3_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inUInt4_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inUInt_inUInt_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inLong_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inLong3_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inLong4_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inLong_inLong_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inULong_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inULong2_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inULong3_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inULong4_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inULong_inULong_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inFloat_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inFloat2_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inFloat3_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inFloat4_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inFloat_inFloat_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inDouble_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inDouble2_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inDouble3_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inDouble4_outLong2 );
		Register( "GL4.Long2Ctor", &GLSLShaderNodes::Long2Ctor_inDouble_inDouble_outLong2 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inBool_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inBool2_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inBool3_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inBool4_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inBool2_inBool_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inBool_inBool2_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inBool_inBool_inBool_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inInt_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inInt2_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inInt3_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inInt4_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inInt2_inInt_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inInt_inInt2_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inInt_inInt_inInt_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inUInt_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inUInt2_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inUInt3_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inUInt4_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inUInt2_inUInt_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inUInt_inUInt2_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inUInt_inUInt_inUInt_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inLong_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inLong2_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inLong4_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inLong2_inLong_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inLong_inLong2_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inLong_inLong_inLong_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inULong_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inULong2_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inULong3_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inULong4_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inULong2_inULong_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inULong_inULong2_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inULong_inULong_inULong_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inFloat_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inFloat2_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inFloat3_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inFloat4_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inFloat2_inFloat_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inFloat_inFloat2_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inFloat_inFloat_inFloat_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inDouble_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inDouble2_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inDouble3_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inDouble4_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inDouble2_inDouble_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inDouble_inDouble2_outLong3 );
		Register( "GL4.Long3Ctor", &GLSLShaderNodes::Long3Ctor_inDouble_inDouble_inDouble_outLong3 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inBool_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inBool2_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inBool3_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inBool4_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inBool3_inBool_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inBool2_inBool2_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inBool_inBool3_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inBool2_inBool_inBool_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inBool_inBool2_inBool_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inBool_inBool_inBool2_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inBool_inBool_inBool_inBool_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inInt_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inInt2_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inInt3_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inInt4_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inInt3_inInt_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inInt2_inInt2_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inInt_inInt3_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inInt2_inInt_inInt_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inInt_inInt2_inInt_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inInt_inInt_inInt2_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inInt_inInt_inInt_inInt_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inUInt_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inUInt2_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inUInt3_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inUInt4_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inUInt3_inUInt_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inUInt2_inUInt2_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inUInt_inUInt3_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inUInt2_inUInt_inUInt_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inUInt_inUInt2_inUInt_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inUInt_inUInt_inUInt2_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inUInt_inUInt_inUInt_inUInt_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inLong_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inLong2_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inLong3_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inLong3_inLong_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inLong2_inLong2_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inLong_inLong3_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inLong2_inLong_inLong_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inLong_inLong2_inLong_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inLong_inLong_inLong2_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inLong_inLong_inLong_inLong_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inULong_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inULong2_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inULong3_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inULong4_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inULong3_inULong_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inULong2_inULong2_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inULong_inULong3_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inULong2_inULong_inULong_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inULong_inULong2_inULong_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inULong_inULong_inULong2_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inULong_inULong_inULong_inULong_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inFloat_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inFloat2_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inFloat3_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inFloat4_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inFloat3_inFloat_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inFloat2_inFloat2_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inFloat_inFloat3_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inFloat2_inFloat_inFloat_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inFloat_inFloat2_inFloat_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inFloat_inFloat_inFloat2_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inFloat_inFloat_inFloat_inFloat_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inDouble_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inDouble2_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inDouble3_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inDouble4_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inDouble3_inDouble_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inDouble2_inDouble2_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inDouble_inDouble3_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inDouble2_inDouble_inDouble_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inDouble_inDouble2_inDouble_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inDouble_inDouble_inDouble2_outLong4 );
		Register( "GL4.Long4Ctor", &GLSLShaderNodes::Long4Ctor_inDouble_inDouble_inDouble_inDouble_outLong4 );
		Register( "GL4.ULongCtor", &GLSLShaderNodes::ULongCtor_inBool_outULong );
		Register( "GL4.ULongCtor", &GLSLShaderNodes::ULongCtor_inInt_outULong );
		Register( "GL4.ULongCtor", &GLSLShaderNodes::ULongCtor_inUInt_outULong );
		Register( "GL4.ULongCtor", &GLSLShaderNodes::ULongCtor_inLong_outULong );
		Register( "GL4.ULongCtor", &GLSLShaderNodes::ULongCtor_inFloat_outULong );
		Register( "GL4.ULongCtor", &GLSLShaderNodes::ULongCtor_inDouble_outULong );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inBool_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inBool2_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inBool3_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inBool4_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inBool_inBool_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inInt_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inInt2_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inInt3_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inInt4_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inInt_inInt_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inUInt_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inUInt2_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inUInt3_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inUInt4_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inUInt_inUInt_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inLong_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inLong2_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inLong3_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inLong4_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inLong_inLong_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inULong_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inULong3_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inULong4_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inULong_inULong_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inFloat_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inFloat2_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inFloat3_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inFloat4_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inFloat_inFloat_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inDouble_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inDouble2_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inDouble3_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inDouble4_outULong2 );
		Register( "GL4.ULong2Ctor", &GLSLShaderNodes::ULong2Ctor_inDouble_inDouble_outULong2 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inBool_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inBool2_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inBool3_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inBool4_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inBool2_inBool_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inBool_inBool2_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inBool_inBool_inBool_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inInt_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inInt2_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inInt3_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inInt4_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inInt2_inInt_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inInt_inInt2_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inInt_inInt_inInt_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inUInt_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inUInt2_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inUInt3_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inUInt4_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inUInt2_inUInt_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inUInt_inUInt2_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inUInt_inUInt_inUInt_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inLong_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inLong2_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inLong3_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inLong4_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inLong2_inLong_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inLong_inLong2_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inLong_inLong_inLong_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inULong_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inULong2_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inULong4_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inULong2_inULong_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inULong_inULong2_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inULong_inULong_inULong_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inFloat_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inFloat2_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inFloat3_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inFloat4_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inFloat2_inFloat_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inFloat_inFloat2_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inFloat_inFloat_inFloat_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inDouble_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inDouble2_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inDouble3_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inDouble4_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inDouble2_inDouble_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inDouble_inDouble2_outULong3 );
		Register( "GL4.ULong3Ctor", &GLSLShaderNodes::ULong3Ctor_inDouble_inDouble_inDouble_outULong3 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inBool_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inBool2_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inBool3_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inBool4_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inBool3_inBool_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inBool2_inBool2_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inBool_inBool3_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inBool2_inBool_inBool_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inBool_inBool2_inBool_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inBool_inBool_inBool2_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inBool_inBool_inBool_inBool_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inInt_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inInt2_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inInt3_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inInt4_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inInt3_inInt_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inInt2_inInt2_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inInt_inInt3_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inInt2_inInt_inInt_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inInt_inInt2_inInt_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inInt_inInt_inInt2_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inInt_inInt_inInt_inInt_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inUInt_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inUInt2_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inUInt3_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inUInt4_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inUInt3_inUInt_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inUInt2_inUInt2_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inUInt_inUInt3_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inUInt2_inUInt_inUInt_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inUInt_inUInt2_inUInt_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inUInt_inUInt_inUInt2_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inUInt_inUInt_inUInt_inUInt_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inLong_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inLong2_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inLong3_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inLong4_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inLong3_inLong_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inLong2_inLong2_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inLong_inLong3_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inLong2_inLong_inLong_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inLong_inLong2_inLong_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inLong_inLong_inLong2_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inLong_inLong_inLong_inLong_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inULong_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inULong2_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inULong3_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inULong3_inULong_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inULong2_inULong2_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inULong_inULong3_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inULong2_inULong_inULong_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inULong_inULong2_inULong_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inULong_inULong_inULong2_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inULong_inULong_inULong_inULong_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inFloat_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inFloat2_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inFloat3_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inFloat4_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inFloat3_inFloat_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inFloat2_inFloat2_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inFloat_inFloat3_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inFloat2_inFloat_inFloat_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inFloat_inFloat2_inFloat_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inFloat_inFloat_inFloat2_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inFloat_inFloat_inFloat_inFloat_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inDouble_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inDouble2_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inDouble3_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inDouble4_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inDouble3_inDouble_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inDouble2_inDouble2_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inDouble_inDouble3_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inDouble2_inDouble_inDouble_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inDouble_inDouble2_inDouble_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inDouble_inDouble_inDouble2_outULong4 );
		Register( "GL4.ULong4Ctor", &GLSLShaderNodes::ULong4Ctor_inDouble_inDouble_inDouble_inDouble_outULong4 );
		Register( "GL4.FloatCtor", &GLSLShaderNodes::FloatCtor_inBool_outFloat );
		Register( "GL4.FloatCtor", &GLSLShaderNodes::FloatCtor_inInt_outFloat );
		Register( "GL4.FloatCtor", &GLSLShaderNodes::FloatCtor_inUInt_outFloat );
		Register( "GL4.FloatCtor", &GLSLShaderNodes::FloatCtor_inLong_outFloat );
		Register( "GL4.FloatCtor", &GLSLShaderNodes::FloatCtor_inULong_outFloat );
		Register( "GL4.FloatCtor", &GLSLShaderNodes::FloatCtor_inDouble_outFloat );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inBool_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inBool2_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inBool3_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inBool4_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inBool_inBool_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inInt_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inInt2_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inInt3_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inInt4_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inInt_inInt_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inUInt_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inUInt2_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inUInt3_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inUInt4_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inUInt_inUInt_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inLong_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inLong2_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inLong3_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inLong4_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inLong_inLong_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inULong_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inULong2_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inULong3_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inULong4_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inULong_inULong_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inFloat_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inFloat3_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inFloat4_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inFloat_inFloat_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inDouble_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inDouble2_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inDouble3_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inDouble4_outFloat2 );
		Register( "GL4.Float2Ctor", &GLSLShaderNodes::Float2Ctor_inDouble_inDouble_outFloat2 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inBool_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inBool2_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inBool3_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inBool4_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inBool2_inBool_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inBool_inBool2_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inBool_inBool_inBool_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inInt_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inInt2_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inInt3_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inInt4_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inInt2_inInt_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inInt_inInt2_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inInt_inInt_inInt_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inUInt_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inUInt2_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inUInt3_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inUInt4_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inUInt2_inUInt_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inUInt_inUInt2_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inUInt_inUInt_inUInt_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inLong_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inLong2_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inLong3_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inLong4_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inLong2_inLong_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inLong_inLong2_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inLong_inLong_inLong_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inULong_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inULong2_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inULong3_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inULong4_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inULong2_inULong_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inULong_inULong2_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inULong_inULong_inULong_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inFloat_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inFloat2_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inFloat4_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inFloat2_inFloat_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inFloat_inFloat2_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inFloat_inFloat_inFloat_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inDouble_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inDouble2_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inDouble3_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inDouble4_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inDouble2_inDouble_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inDouble_inDouble2_outFloat3 );
		Register( "GL4.Float3Ctor", &GLSLShaderNodes::Float3Ctor_inDouble_inDouble_inDouble_outFloat3 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inBool_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inBool2_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inBool3_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inBool4_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inBool3_inBool_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inBool2_inBool2_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inBool_inBool3_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inBool2_inBool_inBool_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inBool_inBool2_inBool_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inBool_inBool_inBool2_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inBool_inBool_inBool_inBool_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inInt_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inInt2_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inInt3_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inInt4_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inInt3_inInt_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inInt2_inInt2_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inInt_inInt3_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inInt2_inInt_inInt_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inInt_inInt2_inInt_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inInt_inInt_inInt2_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inInt_inInt_inInt_inInt_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inUInt_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inUInt2_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inUInt3_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inUInt4_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inUInt3_inUInt_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inUInt2_inUInt2_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inUInt_inUInt3_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inUInt2_inUInt_inUInt_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inUInt_inUInt2_inUInt_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inUInt_inUInt_inUInt2_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inUInt_inUInt_inUInt_inUInt_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inLong_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inLong2_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inLong3_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inLong4_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inLong3_inLong_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inLong2_inLong2_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inLong_inLong3_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inLong2_inLong_inLong_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inLong_inLong2_inLong_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inLong_inLong_inLong2_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inLong_inLong_inLong_inLong_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inULong_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inULong2_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inULong3_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inULong4_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inULong3_inULong_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inULong2_inULong2_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inULong_inULong3_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inULong2_inULong_inULong_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inULong_inULong2_inULong_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inULong_inULong_inULong2_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inULong_inULong_inULong_inULong_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inFloat_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inFloat2_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inFloat3_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inFloat3_inFloat_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inFloat2_inFloat2_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inFloat_inFloat3_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inFloat2_inFloat_inFloat_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inFloat_inFloat2_inFloat_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inFloat_inFloat_inFloat2_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inFloat_inFloat_inFloat_inFloat_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inDouble_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inDouble2_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inDouble3_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inDouble4_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inDouble3_inDouble_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inDouble2_inDouble2_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inDouble_inDouble3_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inDouble2_inDouble_inDouble_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inDouble_inDouble2_inDouble_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inDouble_inDouble_inDouble2_outFloat4 );
		Register( "GL4.Float4Ctor", &GLSLShaderNodes::Float4Ctor_inDouble_inDouble_inDouble_inDouble_outFloat4 );
		Register( "GL4.DoubleCtor", &GLSLShaderNodes::DoubleCtor_inBool_outDouble );
		Register( "GL4.DoubleCtor", &GLSLShaderNodes::DoubleCtor_inInt_outDouble );
		Register( "GL4.DoubleCtor", &GLSLShaderNodes::DoubleCtor_inUInt_outDouble );
		Register( "GL4.DoubleCtor", &GLSLShaderNodes::DoubleCtor_inLong_outDouble );
		Register( "GL4.DoubleCtor", &GLSLShaderNodes::DoubleCtor_inULong_outDouble );
		Register( "GL4.DoubleCtor", &GLSLShaderNodes::DoubleCtor_inFloat_outDouble );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inBool_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inBool2_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inBool3_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inBool4_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inBool_inBool_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inInt_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inInt2_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inInt3_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inInt4_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inInt_inInt_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inUInt_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inUInt2_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inUInt3_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inUInt4_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inUInt_inUInt_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inLong_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inLong2_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inLong3_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inLong4_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inLong_inLong_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inULong_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inULong2_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inULong3_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inULong4_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inULong_inULong_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inFloat_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inFloat2_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inFloat3_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inFloat4_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inFloat_inFloat_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inDouble_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inDouble3_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inDouble4_outDouble2 );
		Register( "GL4.Double2Ctor", &GLSLShaderNodes::Double2Ctor_inDouble_inDouble_outDouble2 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inBool_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inBool2_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inBool3_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inBool4_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inBool2_inBool_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inBool_inBool2_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inBool_inBool_inBool_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inInt_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inInt2_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inInt3_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inInt4_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inInt2_inInt_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inInt_inInt2_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inInt_inInt_inInt_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inUInt_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inUInt2_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inUInt3_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inUInt4_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inUInt2_inUInt_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inUInt_inUInt2_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inUInt_inUInt_inUInt_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inLong_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inLong2_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inLong3_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inLong4_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inLong2_inLong_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inLong_inLong2_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inLong_inLong_inLong_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inULong_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inULong2_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inULong3_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inULong4_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inULong2_inULong_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inULong_inULong2_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inULong_inULong_inULong_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inFloat_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inFloat2_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inFloat3_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inFloat4_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inFloat2_inFloat_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inFloat_inFloat2_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inFloat_inFloat_inFloat_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inDouble_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inDouble2_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inDouble4_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inDouble2_inDouble_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inDouble_inDouble2_outDouble3 );
		Register( "GL4.Double3Ctor", &GLSLShaderNodes::Double3Ctor_inDouble_inDouble_inDouble_outDouble3 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inBool_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inBool2_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inBool3_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inBool4_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inBool3_inBool_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inBool2_inBool2_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inBool_inBool3_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inBool2_inBool_inBool_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inBool_inBool2_inBool_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inBool_inBool_inBool2_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inBool_inBool_inBool_inBool_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inInt_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inInt2_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inInt3_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inInt4_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inInt3_inInt_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inInt2_inInt2_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inInt_inInt3_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inInt2_inInt_inInt_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inInt_inInt2_inInt_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inInt_inInt_inInt2_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inInt_inInt_inInt_inInt_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inUInt_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inUInt2_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inUInt3_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inUInt4_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inUInt3_inUInt_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inUInt2_inUInt2_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inUInt_inUInt3_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inUInt2_inUInt_inUInt_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inUInt_inUInt2_inUInt_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inUInt_inUInt_inUInt2_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inUInt_inUInt_inUInt_inUInt_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inLong_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inLong2_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inLong3_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inLong4_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inLong3_inLong_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inLong2_inLong2_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inLong_inLong3_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inLong2_inLong_inLong_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inLong_inLong2_inLong_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inLong_inLong_inLong2_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inLong_inLong_inLong_inLong_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inULong_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inULong2_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inULong3_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inULong4_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inULong3_inULong_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inULong2_inULong2_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inULong_inULong3_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inULong2_inULong_inULong_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inULong_inULong2_inULong_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inULong_inULong_inULong2_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inULong_inULong_inULong_inULong_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inFloat_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inFloat2_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inFloat3_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inFloat4_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inFloat3_inFloat_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inFloat2_inFloat2_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inFloat_inFloat3_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inFloat2_inFloat_inFloat_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inFloat_inFloat2_inFloat_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inFloat_inFloat_inFloat2_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inFloat_inFloat_inFloat_inFloat_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inDouble_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inDouble2_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inDouble3_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inDouble3_inDouble_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inDouble2_inDouble2_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inDouble_inDouble3_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inDouble2_inDouble_inDouble_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inDouble_inDouble2_inDouble_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inDouble_inDouble_inDouble2_outDouble4 );
		Register( "GL4.Double4Ctor", &GLSLShaderNodes::Double4Ctor_inDouble_inDouble_inDouble_inDouble_outDouble4 );
		Register( "GL4.All", &GLSLShaderNodes::All_inBool_outBool );
		Register( "GL4.All", &GLSLShaderNodes::All_inBool2_outBool );
		Register( "GL4.All", &GLSLShaderNodes::All_inBool3_outBool );
		Register( "GL4.All", &GLSLShaderNodes::All_inBool4_outBool );
		Register( "GL4.Any", &GLSLShaderNodes::Any_inBool_outBool );
		Register( "GL4.Any", &GLSLShaderNodes::Any_inBool2_outBool );
		Register( "GL4.Any", &GLSLShaderNodes::Any_inBool3_outBool );
		Register( "GL4.Any", &GLSLShaderNodes::Any_inBool4_outBool );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage1DReadOnlyInt4_inInt_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage1DReadOnlyUInt4_inInt_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage1DReadOnlyFloat4_inInt_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage1DCoherentInt4_inInt_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage1DCoherentUInt4_inInt_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage1DCoherentFloat4_inInt_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage1DVolatileInt4_inInt_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage1DVolatileUInt4_inInt_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage1DVolatileFloat4_inInt_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage1DRestrictInt4_inInt_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage1DRestrictUInt4_inInt_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage1DRestrictFloat4_inInt_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage1DAReadOnlyInt4_inInt2_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage1DAReadOnlyUInt4_inInt2_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage1DAReadOnlyFloat4_inInt2_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage1DACoherentInt4_inInt2_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage1DACoherentUInt4_inInt2_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage1DACoherentFloat4_inInt2_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage1DAVolatileInt4_inInt2_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage1DAVolatileUInt4_inInt2_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage1DAVolatileFloat4_inInt2_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage1DARestrictInt4_inInt2_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage1DARestrictUInt4_inInt2_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage1DARestrictFloat4_inInt2_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DReadOnlyInt4_inInt2_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DReadOnlyUInt4_inInt2_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DReadOnlyFloat4_inInt2_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DCoherentInt4_inInt2_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DCoherentUInt4_inInt2_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DCoherentFloat4_inInt2_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DVolatileInt4_inInt2_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DVolatileUInt4_inInt2_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DVolatileFloat4_inInt2_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DRestrictInt4_inInt2_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DRestrictUInt4_inInt2_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DRestrictFloat4_inInt2_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DAReadOnlyInt4_inInt3_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DAReadOnlyUInt4_inInt3_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DAReadOnlyFloat4_inInt3_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DACoherentInt4_inInt3_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DACoherentUInt4_inInt3_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DACoherentFloat4_inInt3_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DAVolatileInt4_inInt3_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DAVolatileUInt4_inInt3_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DAVolatileFloat4_inInt3_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DARestrictInt4_inInt3_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DARestrictUInt4_inInt3_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DARestrictFloat4_inInt3_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DMSReadOnlyInt4_inInt2_inInt_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DMSReadOnlyUInt4_inInt2_inInt_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DMSReadOnlyFloat4_inInt2_inInt_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DMSCoherentInt4_inInt2_inInt_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DMSCoherentUInt4_inInt2_inInt_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DMSCoherentFloat4_inInt2_inInt_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DMSVolatileInt4_inInt2_inInt_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DMSVolatileUInt4_inInt2_inInt_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DMSVolatileFloat4_inInt2_inInt_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DMSRestrictInt4_inInt2_inInt_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DMSRestrictUInt4_inInt2_inInt_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DMSRestrictFloat4_inInt2_inInt_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DMSAReadOnlyInt4_inInt3_inInt_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DMSAReadOnlyUInt4_inInt3_inInt_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DMSAReadOnlyFloat4_inInt3_inInt_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DMSACoherentInt4_inInt3_inInt_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DMSACoherentUInt4_inInt3_inInt_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DMSACoherentFloat4_inInt3_inInt_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DMSAVolatileInt4_inInt3_inInt_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DMSAVolatileUInt4_inInt3_inInt_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DMSAVolatileFloat4_inInt3_inInt_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DMSARestrictInt4_inInt3_inInt_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DMSARestrictUInt4_inInt3_inInt_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage2DMSARestrictFloat4_inInt3_inInt_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImageCubeReadOnlyInt4_inInt3_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImageCubeReadOnlyUInt4_inInt3_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImageCubeReadOnlyFloat4_inInt3_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImageCubeCoherentInt4_inInt3_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImageCubeCoherentUInt4_inInt3_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImageCubeCoherentFloat4_inInt3_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImageCubeVolatileInt4_inInt3_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImageCubeVolatileUInt4_inInt3_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImageCubeVolatileFloat4_inInt3_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImageCubeRestrictInt4_inInt3_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImageCubeRestrictUInt4_inInt3_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImageCubeRestrictFloat4_inInt3_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImageCubeAReadOnlyInt4_inInt3_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImageCubeAReadOnlyUInt4_inInt3_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImageCubeAReadOnlyFloat4_inInt3_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImageCubeACoherentInt4_inInt3_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImageCubeACoherentUInt4_inInt3_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImageCubeACoherentFloat4_inInt3_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImageCubeAVolatileInt4_inInt3_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImageCubeAVolatileUInt4_inInt3_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImageCubeAVolatileFloat4_inInt3_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImageCubeARestrictInt4_inInt3_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImageCubeARestrictUInt4_inInt3_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImageCubeARestrictFloat4_inInt3_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage3DReadOnlyInt4_inInt3_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage3DReadOnlyUInt4_inInt3_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage3DReadOnlyFloat4_inInt3_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage3DCoherentInt4_inInt3_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage3DCoherentUInt4_inInt3_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage3DCoherentFloat4_inInt3_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage3DVolatileInt4_inInt3_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage3DVolatileUInt4_inInt3_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage3DVolatileFloat4_inInt3_outFloat4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage3DRestrictInt4_inInt3_outInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage3DRestrictUInt4_inInt3_outUInt4 );
		Register( "GL4.ImageLoad", &GLSLShaderNodes::ImageLoad_inImage3DRestrictFloat4_inInt3_outFloat4 );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage1DWriteOnlyInt4_inInt_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage1DWriteOnlyUInt4_inInt_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage1DWriteOnlyFloat4_inInt_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage1DCoherentInt4_inInt_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage1DCoherentUInt4_inInt_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage1DCoherentFloat4_inInt_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage1DVolatileInt4_inInt_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage1DVolatileUInt4_inInt_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage1DVolatileFloat4_inInt_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage1DRestrictInt4_inInt_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage1DRestrictUInt4_inInt_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage1DRestrictFloat4_inInt_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage1DAWriteOnlyInt4_inInt2_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage1DAWriteOnlyUInt4_inInt2_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage1DAWriteOnlyFloat4_inInt2_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage1DACoherentInt4_inInt2_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage1DACoherentUInt4_inInt2_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage1DACoherentFloat4_inInt2_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage1DAVolatileInt4_inInt2_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage1DAVolatileUInt4_inInt2_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage1DAVolatileFloat4_inInt2_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage1DARestrictInt4_inInt2_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage1DARestrictUInt4_inInt2_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage1DARestrictFloat4_inInt2_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DWriteOnlyInt4_inInt2_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DWriteOnlyUInt4_inInt2_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DWriteOnlyFloat4_inInt2_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DCoherentInt4_inInt2_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DCoherentUInt4_inInt2_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DCoherentFloat4_inInt2_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DVolatileInt4_inInt2_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DVolatileUInt4_inInt2_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DVolatileFloat4_inInt2_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DRestrictInt4_inInt2_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DRestrictUInt4_inInt2_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DRestrictFloat4_inInt2_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DAWriteOnlyInt4_inInt3_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DAWriteOnlyUInt4_inInt3_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DAWriteOnlyFloat4_inInt3_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DACoherentInt4_inInt3_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DACoherentUInt4_inInt3_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DACoherentFloat4_inInt3_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DAVolatileInt4_inInt3_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DAVolatileUInt4_inInt3_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DAVolatileFloat4_inInt3_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DARestrictInt4_inInt3_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DARestrictUInt4_inInt3_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DARestrictFloat4_inInt3_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DMSWriteOnlyInt4_inInt2_inInt_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DMSWriteOnlyUInt4_inInt2_inInt_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DMSWriteOnlyFloat4_inInt2_inInt_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DMSCoherentInt4_inInt2_inInt_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DMSCoherentUInt4_inInt2_inInt_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DMSCoherentFloat4_inInt2_inInt_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DMSVolatileInt4_inInt2_inInt_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DMSVolatileUInt4_inInt2_inInt_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DMSVolatileFloat4_inInt2_inInt_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DMSRestrictInt4_inInt2_inInt_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DMSRestrictUInt4_inInt2_inInt_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DMSRestrictFloat4_inInt2_inInt_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DMSAWriteOnlyInt4_inInt3_inInt_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DMSAWriteOnlyUInt4_inInt3_inInt_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DMSAWriteOnlyFloat4_inInt3_inInt_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DMSACoherentInt4_inInt3_inInt_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DMSACoherentUInt4_inInt3_inInt_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DMSACoherentFloat4_inInt3_inInt_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DMSAVolatileInt4_inInt3_inInt_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DMSAVolatileUInt4_inInt3_inInt_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DMSAVolatileFloat4_inInt3_inInt_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DMSARestrictInt4_inInt3_inInt_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DMSARestrictUInt4_inInt3_inInt_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage2DMSARestrictFloat4_inInt3_inInt_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImageCubeWriteOnlyInt4_inInt3_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImageCubeWriteOnlyUInt4_inInt3_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImageCubeWriteOnlyFloat4_inInt3_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImageCubeCoherentInt4_inInt3_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImageCubeCoherentUInt4_inInt3_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImageCubeCoherentFloat4_inInt3_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImageCubeVolatileInt4_inInt3_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImageCubeVolatileUInt4_inInt3_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImageCubeVolatileFloat4_inInt3_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImageCubeRestrictInt4_inInt3_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImageCubeRestrictUInt4_inInt3_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImageCubeRestrictFloat4_inInt3_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImageCubeAWriteOnlyInt4_inInt3_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImageCubeAWriteOnlyUInt4_inInt3_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImageCubeAWriteOnlyFloat4_inInt3_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImageCubeACoherentInt4_inInt3_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImageCubeACoherentUInt4_inInt3_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImageCubeACoherentFloat4_inInt3_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImageCubeAVolatileInt4_inInt3_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImageCubeAVolatileUInt4_inInt3_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImageCubeAVolatileFloat4_inInt3_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImageCubeARestrictInt4_inInt3_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImageCubeARestrictUInt4_inInt3_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImageCubeARestrictFloat4_inInt3_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage3DWriteOnlyInt4_inInt3_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage3DWriteOnlyUInt4_inInt3_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage3DWriteOnlyFloat4_inInt3_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage3DCoherentInt4_inInt3_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage3DCoherentUInt4_inInt3_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage3DCoherentFloat4_inInt3_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage3DVolatileInt4_inInt3_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage3DVolatileUInt4_inInt3_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage3DVolatileFloat4_inInt3_inFloat4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage3DRestrictInt4_inInt3_inInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage3DRestrictUInt4_inInt3_inUInt4_outSync );
		Register( "GL4.ImageStore", &GLSLShaderNodes::ImageStore_inImage3DRestrictFloat4_inInt3_inFloat4_outSync );
		Register( "GL4.TextureSample", &GLSLShaderNodes::TextureSample_inImage1DFloat4_inSampler_inFloat_outFloat4 );
		Register( "GL4.TextureSampleOffset", &GLSLShaderNodes::TextureSampleOffset_inImage1DFloat4_inSampler_inFloat_inInt_outFloat4 );
		Register( "GL4.TextureSample", &GLSLShaderNodes::TextureSample_inImage1DAFloat4_inSampler_inFloat2_outFloat4 );
		Register( "GL4.TextureSampleOffset", &GLSLShaderNodes::TextureSampleOffset_inImage1DAFloat4_inSampler_inFloat2_inInt2_outFloat4 );
		Register( "GL4.TextureSample", &GLSLShaderNodes::TextureSample_inImage2DFloat4_inSampler_inFloat2_outFloat4 );
		Register( "GL4.TextureSampleOffset", &GLSLShaderNodes::TextureSampleOffset_inImage2DFloat4_inSampler_inFloat2_inInt2_outFloat4 );
		Register( "GL4.TextureSample", &GLSLShaderNodes::TextureSample_inImage2DAFloat4_inSampler_inFloat3_outFloat4 );
		Register( "GL4.TextureSampleOffset", &GLSLShaderNodes::TextureSampleOffset_inImage2DAFloat4_inSampler_inFloat3_inInt3_outFloat4 );
		Register( "GL4.TextureSample", &GLSLShaderNodes::TextureSample_inImage2DMSFloat4_inSampler_inFloat2_outFloat4 );
		Register( "GL4.TextureSampleOffset", &GLSLShaderNodes::TextureSampleOffset_inImage2DMSFloat4_inSampler_inFloat2_inInt2_outFloat4 );
		Register( "GL4.TextureSample", &GLSLShaderNodes::TextureSample_inImage2DMSAFloat4_inSampler_inFloat3_outFloat4 );
		Register( "GL4.TextureSampleOffset", &GLSLShaderNodes::TextureSampleOffset_inImage2DMSAFloat4_inSampler_inFloat3_inInt3_outFloat4 );
		Register( "GL4.TextureSample", &GLSLShaderNodes::TextureSample_inImageCubeFloat4_inSampler_inFloat3_outFloat4 );
		Register( "GL4.TextureSampleOffset", &GLSLShaderNodes::TextureSampleOffset_inImageCubeFloat4_inSampler_inFloat3_inInt3_outFloat4 );
		Register( "GL4.TextureSample", &GLSLShaderNodes::TextureSample_inImageCubeAFloat4_inSampler_inFloat3_outFloat4 );
		Register( "GL4.TextureSampleOffset", &GLSLShaderNodes::TextureSampleOffset_inImageCubeAFloat4_inSampler_inFloat3_inInt3_outFloat4 );
		Register( "GL4.TextureSample", &GLSLShaderNodes::TextureSample_inImage3DFloat4_inSampler_inFloat3_outFloat4 );
		Register( "GL4.TextureSampleOffset", &GLSLShaderNodes::TextureSampleOffset_inImage3DFloat4_inSampler_inFloat3_inInt3_outFloat4 );

		Register( "GL4.CS_GetGlobalIndexSize", &GLSLShaderNodes::CS_GetGlobalIndexSize_outInt );
		Register( "GL4.CS_GetGlobalIndex", &GLSLShaderNodes::CS_GetGlobalIndex_outInt );
		Register( "GL4.CS_GetLocalIndexSize", &GLSLShaderNodes::CS_GetLocalIndexSize_outInt );
		Register( "GL4.CS_GetLocalIndex", &GLSLShaderNodes::CS_GetLocalIndex_outInt );
		Register( "GL4.CS_GetGroupIndexSize", &GLSLShaderNodes::CS_GetGroupIndexSize_outInt );
		Register( "GL4.CS_GetGroupIndex", &GLSLShaderNodes::CS_GetGroupIndex_outInt );
		Register( "GL4.CS_GetGlobalCoordSize", &GLSLShaderNodes::CS_GetGlobalCoordSize_outInt3 );
		Register( "GL4.CS_GetGlobalCoord", &GLSLShaderNodes::CS_GetGlobalCoord_outInt3 );
		Register( "GL4.CS_GetLocalCoordSize", &GLSLShaderNodes::CS_GetLocalCoordSize_outInt3 );
		Register( "GL4.CS_GetLocalCoord", &GLSLShaderNodes::CS_GetLocalCoord_outInt3 );
		Register( "GL4.CS_GetGroupCoordSize", &GLSLShaderNodes::CS_GetGroupCoordSize_outInt3 );
		Register( "GL4.CS_GetGroupCoord", &GLSLShaderNodes::CS_GetGroupCoord_outInt3 );
	
	}


const char* g_GLSLSharedShaderSource = R"#(
#define and			&&
#define or			||
#define not			!

#define bool2		bvec2
#define bool3		bvec3
#define bool4		bvec4

#define int2		ivec2
#define int3		ivec3
#define int4		ivec4

#define uint2		uvec2
#define uint3		uvec3
#define uint4		uvec4

#ifdef GL_ARB_gpu_shader_int64

# define ilong		int64_t
# define ilong2		i64vec2
# define ilong3		i64vec3
# define ilong4		i64vec4

# define ulong		uint64_t
# define ulong2		u64vec2
# define ulong3		u64vec3
# define ulong4		u64vec4

#endif // GL_ARB_gpu_shader_int64

#define float2		vec2
#define float3		vec3
#define float4		vec4
			
#define float2x2	mat2
#define float3x3	mat3
#define float4x4	mat4

#define double2		dvec2
#define double3		dvec3
#define double4		dvec4
			
#define double2x2	dmat2
#define double3x3	dmat3
#define double4x4	dmat4

#define SH_VERTEX           0
#define SH_TESS_CONTROL     1
#define SH_TESS_EVALUATION  2
#define SH_GEOMETRY         3
#define SH_FRAGMENT         4
#define SH_COMPUTE          5

#if SHADER == SH_COMPUTE

int CS_GetLocalIndex ()
{
	return int( gl_LocalInvocationIndex );
}

int3 CS_GetGlobalCoord ()
{
	return int3( gl_GlobalInvocationID );
}

int3 CS_GetGroupCoordSize ()
{
	return int3( gl_NumWorkGroups );
}

int3 CS_GetLocalCoordSize ()
{
#ifdef GL_ARB_compute_variable_group_size
	return int3( gl_LocalGroupSizeARB );
#else
	return int3( gl_WorkGroupSize );
#endif
}

int3 CS_GetGlobalCoordSize ()
{
	return CS_GetGroupCoordSize() * CS_GetLocalCoordSize();
}

int3 CS_GetLocalCoord ()
{
	return int3( gl_LocalInvocationID );
}

int3 CS_GetGroupCoord ()
{
	return int3( gl_WorkGroupID );
}

int CS_GetGlobalIndex ()
{
	int3 coord = CS_GetGlobalCoord();
	int3 size  = CS_GetGlobalCoordSize();
	return coord.x + (coord.y * size.x) + (coord.z * size.x * size.y);
}

int CS_GetGlobalIndexSize ()
{
	int3 size  = CS_GetGlobalCoordSize();
	return size.x * size.y * size.z;
}

int CS_GetLocalIndexSize ()
{
	int3 size  = CS_GetLocalCoordSize();
	return size.x * size.y * size.z;
}

int CS_GetGroupIndex ()
{
	int3 coord = CS_GetGroupCoord();
	int3 size  = CS_GetGroupCoordSize();
	return coord.x + (coord.y * size.x) + (coord.z * size.x * size.y);
}

int CS_GetGroupIndexSize ()
{
	int3 size  = CS_GetGroupCoordSize();
	return size.x * size.y * size.z;
}
#endif	// SH_COMPUTE

	)#";

}	// _ShaderNodesHidden_
}	// ShaderEditor
