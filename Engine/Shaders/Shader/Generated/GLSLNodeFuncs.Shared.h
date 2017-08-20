
// This is generated file, don't change anything!

#pragma once

#include "Engine/Shaders/Shader/Helpers.h"
#include "AllNodeFuncs.h"

namespace ShaderEditor
{
namespace GLSLShaderNodes
{
	using namespace ShaderNodes;

	using ShaderOutputNode	= _ShaderNodesHidden_::ShaderOutputNode;
	using FunctionNode		= _ShaderNodesHidden_::FunctionNode;

	template <typename T>			using ArrayCRef			= GX_STL::GXTypes::ArrayCRef<T>;
	template <typename T>			using ArrayRef			= GX_STL::GXTypes::ArrayRef<T>;
	template <typename T>			using Array				= GX_STL::GXTypes::Array<T>;
	template <typename T, uint S>	using FixedSizeArray	= GX_STL::GXTypes::FixedSizeArray<T,S>;

	using VariantCRef		= GX_STL::GXTypes::VariantCRef;
	using VariantRef		= GX_STL::GXTypes::VariantRef;

	using StringCRef		= GX_STL::GXTypes::StringCRef;
	using String			= GX_STL::GXTypes::String;

	using usize				= GX_STL::GXTypes::usize;
	using usize3			= GX_STL::GXMath::usize3;

	
/*
=================================================
	Replace
=================================================
*/
	template <typename T>
	inline void Replace (OUT T &node, StringCRef name, FunctionNode &parent) noexcept
	{
		node.~T();
		new (&node) T( &parent, name );
	}

	inline void Replace (OUT ShaderOutputNode &node, StringCRef, FunctionNode &parent) noexcept
	{
		node.~ShaderOutputNode();
		new (&node) ShaderOutputNode( &parent );
	}
	
/*
=================================================
	TypeIdOf
=================================================
*/
	template <typename T>
	forceinline GX_STL::GXTypes::TypeId  TypeIdOf () noexcept
	{
		return GX_STL::GXTypes::TypeIdOf<T>();
	}

	template <typename T>
	forceinline GX_STL::GXTypes::TypeId  TypeIdOf (const T&) noexcept
	{
		return GX_STL::GXTypes::TypeIdOf<T>();
	}

/*
=================================================
	ExtractArgs
=================================================
*/
	using ExtractedArgs_t	= FixedSizeArray<StringCRef, 16>;

	inline void ExtractArgs (StringCRef signature, OUT StringCRef &name, OUT ExtractedArgs_t &inArgs, OUT ExtractedArgs_t &outArgs) noexcept
	{
		const String	in_pattern( "_in" );
		const String	out_pattern( "_out" );

		FixedSizeArray<usize3, 16>	positions;

		for (usize pos = 0; signature.Find( in_pattern, OUT pos, pos ); pos += in_pattern.Length()) {
			positions.PushBack(usize3( pos, pos + in_pattern.Length(), 0 ));
		}
		for (usize pos = 0; signature.Find( out_pattern, OUT pos, pos ); pos += out_pattern.Length()) {
			positions.PushBack(usize3( pos, pos + out_pattern.Length(), 1 ));
		}
		inArgs.Clear();
		outArgs.Clear();

		if ( positions.Empty() )
		{
			name = signature;
			return;
		}

		positions.PushBack(usize3(signature.Length(), signature.Length(), 2));

		GX_STL::GXTypes::Sort( positions, LAMBDA()(const auto &lhs, const auto &rhs) { return lhs.x > rhs.x; } );

		name = signature.SubString( 0, positions.Front().x );

		for (usize i = 0; i+1 < positions.Count(); ++i)
		{
			StringCRef s = signature.SubString( positions[i].y, positions[i+1].x - positions[i].y );

			switch ( positions[i].z )
			{
				case 0 :	inArgs.PushBack( s );		break;
				case 1 :	outArgs.PushBack( s );		break;
				default :	WARNING( "invalid type" );
			}
		}
	}

/*
=================================================
	StringToType
=================================================
*/
	inline GX_STL::GXTypes::TypeId  StringToType (StringCRef str)
	{
		if ( str == "Bool" )	return TypeIdOf<Bool>();
		if ( str == "Bool2" )	return TypeIdOf<Bool2>();
		if ( str == "Bool3" )	return TypeIdOf<Bool3>();
		if ( str == "Bool4" )	return TypeIdOf<Bool4>();
		if ( str == "Int" )		return TypeIdOf<Int>();
		if ( str == "Int2" )	return TypeIdOf<Int2>();
		if ( str == "Int3" )	return TypeIdOf<Int3>();
		if ( str == "Int4" )	return TypeIdOf<Int4>();
		if ( str == "UInt" )	return TypeIdOf<UInt>();
		if ( str == "UInt2" )	return TypeIdOf<UInt2>();
		if ( str == "UInt3" )	return TypeIdOf<UInt3>();
		if ( str == "UInt4" )	return TypeIdOf<UInt4>();
		if ( str == "ILong" )	return TypeIdOf<ILong>();
		if ( str == "ILong2" )	return TypeIdOf<ILong2>();
		if ( str == "ILong3" )	return TypeIdOf<ILong3>();
		if ( str == "ILong4" )	return TypeIdOf<ILong4>();
		if ( str == "ULong" )	return TypeIdOf<ULong>();
		if ( str == "ULong2" )	return TypeIdOf<ULong2>();
		if ( str == "ULong3" )	return TypeIdOf<ULong3>();
		if ( str == "ULong4" )	return TypeIdOf<ULong4>();
		if ( str == "Float" )	return TypeIdOf<Float>();
		if ( str == "Float2" )	return TypeIdOf<Float2>();
		if ( str == "Float3" )	return TypeIdOf<Float3>();
		if ( str == "Float4" )	return TypeIdOf<Float4>();
		if ( str == "Double" )	return TypeIdOf<Double>();
		if ( str == "Double2" )	return TypeIdOf<Double2>();
		if ( str == "Double3" )	return TypeIdOf<Double3>();
		if ( str == "Double4" )	return TypeIdOf<Double4>();
								return GX_STL::GXTypes::TypeId();
	}
	
/*
=================================================
	TypeToTypeName
=================================================
*/
	inline StringCRef TypeToTypeName (GX_STL::GXTypes::TypeId id)
	{
		if ( id == TypeIdOf<Bool>() )		return "Bool";
		if ( id == TypeIdOf<Bool2>() )		return "Bool2";
		if ( id == TypeIdOf<Bool3>() )		return "Bool3";
		if ( id == TypeIdOf<Bool4>() )		return "Bool4";
		if ( id == TypeIdOf<Int>() )		return "Int";
		if ( id == TypeIdOf<Int2>() )		return "Int2";
		if ( id == TypeIdOf<Int3>() )		return "Int3";
		if ( id == TypeIdOf<Int4>() )		return "Int4";
		if ( id == TypeIdOf<UInt>() )		return "UInt";
		if ( id == TypeIdOf<UInt2>() )		return "UInt2";
		if ( id == TypeIdOf<UInt3>() )		return "UInt3";
		if ( id == TypeIdOf<UInt4>() )		return "UInt4";
		if ( id == TypeIdOf<ILong>() )		return "ILong";
		if ( id == TypeIdOf<ILong2>() )		return "ILong2";
		if ( id == TypeIdOf<ILong3>() )		return "ILong3";
		if ( id == TypeIdOf<ILong4>() )		return "ILong4";
		if ( id == TypeIdOf<ULong>() )		return "ULong";
		if ( id == TypeIdOf<ULong2>() )		return "ULong2";
		if ( id == TypeIdOf<ULong3>() )		return "ULong3";
		if ( id == TypeIdOf<ULong4>() )		return "ULong4";
		if ( id == TypeIdOf<Float>() )		return "Float";
		if ( id == TypeIdOf<Float2>() )		return "Float2";
		if ( id == TypeIdOf<Float3>() )		return "Float3";
		if ( id == TypeIdOf<Float4>() )		return "Float4";
		if ( id == TypeIdOf<Double>() )		return "Double";
		if ( id == TypeIdOf<Double2>() )	return "Double2";
		if ( id == TypeIdOf<Double3>() )	return "Double3";
		if ( id == TypeIdOf<Double4>() )	return "Double4";
											return "";
	}

/*
=================================================
	ExtractArgsTypes
=================================================
*/
	using ExtractedTypes_t	= FixedSizeArray<GX_STL::GXTypes::TypeId, 16>;

	inline void ExtractArgsTypes (StringCRef signature, OUT StringCRef &name, OUT ExtractedTypes_t &inArgs, OUT ExtractedTypes_t &outArgs)
	{
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( signature, OUT name, OUT in_args, OUT out_args );

		FOR( i, in_args ) {
			inArgs.PushBack( StringToType( in_args[i] ) );
		}
		
		FOR( i, out_args ) {
			outArgs.PushBack( StringToType( out_args[i] ) );
		}
	}

/*
=================================================
	ToLowerCase
=================================================
*/
	inline String  ToLowerCase (StringCRef str)
	{
		String	result;	result.Resize( str.Length(), '\0' );

		FOR( i, str ) {
			result[i] = GX_STL::GXTypes::StringUtils::ToLower( str[i] );
		}
		return result;
	}

/*
=================================================
	IsLetter
=================================================
*/
	inline bool  IsLetter (char c)
	{
		return (c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z');
	}

	inline bool  IsNumber (char c)
	{
		return (c >= '0' and c <= '9');
	}

/*
=================================================
	IsSameBaseType
=================================================
*/
	inline bool IsSameBaseType (StringCRef lname, StringCRef rname, OUT String &sharedType, OUT uint &lvecSize, OUT uint &rvecSize)
	{
		lvecSize = IsNumber( lname.Back() ) ? (lname.Back() - '0') : 1;
		rvecSize = IsNumber( rname.Back() ) ? (rname.Back() - '0') : 1;

		// same size and same types
		if ( lvecSize == rvecSize and lname == rname ) {
			sharedType = lname;
			return true;
		}

		// compare vector with vector
		if ( IsNumber( lname.Back() ) == IsNumber( rname.Back() ) )
		{
			// compare types without size
			if ( lname.SubString( 0, lname.Length()-1 ) == rname.SubString( 0, rname.Length()-1 ) ) {
				sharedType = (lvecSize > rvecSize ? lname : rname);
				return true;
			}
			return false;
		}

		// compare scalar with vector
		if ( lname.Length() < rname.Length() )
			GX_STL::GXTypes::SwapValues( lname, rname );

		if ( lname.StartsWith( rname ) ) {
			sharedType = (lvecSize > rvecSize ? lname : rname);
			return true;
		}
		return false;
	}

/*
=================================================
	TypeNameFromType
=================================================
*/
	inline String TypeFromType (StringCRef srcTypeName, StringCRef dstTypeName)
	{
		return String(dstTypeName) << (IsNumber( srcTypeName.Back() ) ? srcTypeName.Back() : '\0');
	}

/*
=================================================
	Is***Typename
=================================================
*/
	inline bool IsFloatTypename (StringCRef typeName)
	{
		return typeName.StartsWith("Float") or typeName.StartsWith("Double");
	}

	inline bool IsImageTypename (StringCRef typeName)
	{
		return typeName.StartsWith("Image");
	}

}	// GLSLShaderNodes

namespace _ShaderNodesHidden_
{

	void RegisterGLSLShaderNodes0 (NodeFunctions *nf);
	void RegisterGLSLShaderNodes1 (NodeFunctions *nf);
	void RegisterGLSLShaderNodes2 (NodeFunctions *nf);
	void RegisterGLSLShaderNodes3 (NodeFunctions *nf);
	void RegisterGLSLShaderNodes4 (NodeFunctions *nf);
	void RegisterGLSLShaderNodes5 (NodeFunctions *nf);
	void RegisterGLSLShaderNodes6 (NodeFunctions *nf);

}	// _ShaderNodesHidden_
}	// ShaderEditor
