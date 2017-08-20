// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Shaders/Shader/VecTypes.h"
#include "Engine/Shaders/Shader/ImageTypes.h"
#include "Engine/Shaders/Shader/BufferTypes.h"
#include "Engine/Shaders/Shader/OtherTypes.h"

namespace ShaderEditor
{
namespace _ShaderNodesHidden_
{
	struct NodeGetter {
		static ISrcNodePtr Get (const Node &node)	{ return node._self; }
	};

}	// _ShaderNodesHidden_


namespace ShaderNodes
{
namespace Helpers
{
	namespace _hidden_
	{
		template <typename T>			struct _GetNVecElemType {};
		template <typename T, uint I>	struct _GetNVecElemType< NVec<T,I> > { using type = T; };
		template <typename T>			using  GetNVecElemType	= typename _GetNVecElemType<T>::type;

	}	// _hidden_
	

	using NodeGetter			= _ShaderNodesHidden_::NodeGetter;

	/*
	template <typename T, uint I>
	using IVecFrom				= NVec< GX_STL::CompileTime::NearInt<T>, I >;

	template <typename T, uint I>
	using UVecFrom				= NVec< GX_STL::CompileTime::NearUInt<T>, I >;

	template <typename T, uint I>
	using FVecFrom				= NVec< GX_STL::CompileTime::NearFloat<T>, I >;
	*/

	template <typename T>
	constexpr bool	IsBool		= GX_STL::CompileTime::IsSameTypes< _hidden_::GetNVecElemType<T>, bool >;
	
	template <typename T>
	constexpr bool	IsInt		= GX_STL::CompileTime::IsSameTypes< _hidden_::GetNVecElemType<T>, int >;
	
	template <typename T>
	constexpr bool	IsUInt		= GX_STL::CompileTime::IsSameTypes< _hidden_::GetNVecElemType<T>, uint >;
	
	template <typename T>
	constexpr bool	IsILong		= GX_STL::CompileTime::IsSameTypes< _hidden_::GetNVecElemType<T>, ilong >;
	
	template <typename T>
	constexpr bool	IsULong		= GX_STL::CompileTime::IsSameTypes< _hidden_::GetNVecElemType<T>, ulong >;
	
	template <typename T>
	constexpr bool	IsFloat		= GX_STL::CompileTime::IsSameTypes< _hidden_::GetNVecElemType<T>, float >;
	
	template <typename T>
	constexpr bool	IsDouble	= GX_STL::CompileTime::IsSameTypes< _hidden_::GetNVecElemType<T>, double >;
	

	template <typename T, uint I, uint J>
	using MaxNVec	= NVec< T, GX_STL::CompileTime::Max< uint, I, J > >;
	

/*
=================================================
	<Type>Only
=================================================
*/
	template <typename T>
	struct BooleanOnly {
		STATIC_ASSERT( IsBool<T> );
	};

	template <typename T>
	struct IntegerOnly {
		STATIC_ASSERT( IsInt<T> or IsUInt<T> or IsILong<T> or IsULong<T> );
	};

	template <typename T>
	struct Integer32Only {
		STATIC_ASSERT( IsInt<T> or IsUInt<T> );
	};

	template <typename T>
	struct Integer64Only {
		STATIC_ASSERT( IsILong<T> or IsULong<T> );
	};

	template <typename T>
	struct FloatOnly {
		STATIC_ASSERT( IsFloat<T> or IsDouble<T> );
	};
	
	template <typename T>
	struct Float32Only {
		STATIC_ASSERT( IsFloat<T> );
	};

	template <typename T>
	struct Float64Only {
		STATIC_ASSERT( IsDouble<T> );
	};

	template <typename T>
	struct IntegerOrFloatOnly {
		STATIC_ASSERT( IsInt<T> or IsUInt<T> or IsILong<T> or IsULong<T> or IsFloat<T> or IsDouble<T> );
	};
	
/*
=================================================
	TypeList
=================================================
*/
	using BooleanTypes			= GX_STL::CompileTime::TypeListFrom< Bool, Bool2, Bool3, Bool4 >;
	using SInteger32Types		= GX_STL::CompileTime::TypeListFrom< Int, Int2, Int3, Int4 >;
	using UInteger32Types		= GX_STL::CompileTime::TypeListFrom< UInt, UInt2, UInt3, UInt4 >;
	using Integer32Types		= SInteger32Types::Append< UInteger32Types >;
	using SInteger64Types		= GX_STL::CompileTime::TypeListFrom< ILong, ILong2, ILong3, ILong4 >;
	using UInteger64Types		= GX_STL::CompileTime::TypeListFrom< ULong, ULong2, ULong3, ULong4 >;
	using Integer64Types		= SInteger64Types::Append< UInteger64Types >;
	using Float32Types			= GX_STL::CompileTime::TypeListFrom< Float, Float2, Float3, Float4 >;
	using Float64Types			= GX_STL::CompileTime::TypeListFrom< Double, Double2, Double3, Double4 >;
	using IntegerTypes			= Integer32Types::Append< Integer64Types >;
	using FloatTypes			= Float32Types::Append< Float64Types >;
	using IntegerOrFloatTypes	= IntegerTypes::Append< FloatTypes >;
	using AllTypes				= IntegerOrFloatTypes::Append< BooleanTypes >;

	using ImageTypes			= GX_STL::CompileTime::TypeListFrom<
		Image< ImageType1D, ReadOnly, Float4 >,			Image< ImageType1D, ReadOnly, Int4 >,			Image< ImageType1D, ReadOnly, UInt4 >,
		Image< ImageType1D, WriteOnly, Float4 >,		Image< ImageType1D, WriteOnly, Int4 >,			Image< ImageType1D, WriteOnly, UInt4 >,
		Image< ImageType1D, Coherent, Float4 >,			Image< ImageType1D, Coherent, Int4 >,			Image< ImageType1D, Coherent, UInt4 >,
		Image< ImageType1D, Volatile, Float4 >,			Image< ImageType1D, Volatile, Int4 >,			Image< ImageType1D, Volatile, UInt4 >,
		Image< ImageType1D, Restrict, Float4 >,			Image< ImageType1D, Restrict, Int4 >,			Image< ImageType1D, Restrict, UInt4 >,
		Image< ImageType1DArray, ReadOnly, Float4 >,	Image< ImageType1DArray, ReadOnly, Int4 >,		Image< ImageType1DArray, ReadOnly, UInt4 >,
		Image< ImageType1DArray, WriteOnly, Float4 >,	Image< ImageType1DArray, WriteOnly, Int4 >,		Image< ImageType1DArray, WriteOnly, UInt4 >,
		Image< ImageType1DArray, Coherent, Float4 >,	Image< ImageType1DArray, Coherent, Int4 >,		Image< ImageType1DArray, Coherent, UInt4 >,
		Image< ImageType1DArray, Volatile, Float4 >,	Image< ImageType1DArray, Volatile, Int4 >,		Image< ImageType1DArray, Volatile, UInt4 >,
		Image< ImageType1DArray, Restrict, Float4 >,	Image< ImageType1DArray, Restrict, Int4 >,		Image< ImageType1DArray, Restrict, UInt4 >,
		Image< ImageType2D, ReadOnly, Float4 >,			Image< ImageType2D, ReadOnly, Int4 >,			Image< ImageType2D, ReadOnly, UInt4 >,
		Image< ImageType2D, WriteOnly, Float4 >,		Image< ImageType2D, WriteOnly, Int4 >,			Image< ImageType2D, WriteOnly, UInt4 >,
		Image< ImageType2D, Coherent, Float4 >,			Image< ImageType2D, Coherent, Int4 >,			Image< ImageType2D, Coherent, UInt4 >,
		Image< ImageType2D, Volatile, Float4 >,			Image< ImageType2D, Volatile, Int4 >,			Image< ImageType2D, Volatile, UInt4 >,
		Image< ImageType2D, Restrict, Float4 >,			Image< ImageType2D, Restrict, Int4 >,			Image< ImageType2D, Restrict, UInt4 >,
		Image< ImageType2DArray, ReadOnly, Float4 >,	Image< ImageType2DArray, ReadOnly, Int4 >,		Image< ImageType2DArray, ReadOnly, UInt4 >,
		Image< ImageType2DArray, WriteOnly, Float4 >,	Image< ImageType2DArray, WriteOnly, Int4 >,		Image< ImageType2DArray, WriteOnly, UInt4 >,
		Image< ImageType2DArray, Coherent, Float4 >,	Image< ImageType2DArray, Coherent, Int4 >,		Image< ImageType2DArray, Coherent, UInt4 >,
		Image< ImageType2DArray, Volatile, Float4 >,	Image< ImageType2DArray, Volatile, Int4 >,		Image< ImageType2DArray, Volatile, UInt4 >,
		Image< ImageType2DArray, Restrict, Float4 >,	Image< ImageType2DArray, Restrict, Int4 >,		Image< ImageType2DArray, Restrict, UInt4 >,
		Image< ImageType2DMS, ReadOnly, Float4 >,		Image< ImageType2DMS, ReadOnly, Int4 >,			Image< ImageType2DMS, ReadOnly, UInt4 >,
		Image< ImageType2DMS, WriteOnly, Float4 >,		Image< ImageType2DMS, WriteOnly, Int4 >,		Image< ImageType2DMS, WriteOnly, UInt4 >,
		Image< ImageType2DMS, Coherent, Float4 >,		Image< ImageType2DMS, Coherent, Int4 >,			Image< ImageType2DMS, Coherent, UInt4 >,
		Image< ImageType2DMS, Volatile, Float4 >,		Image< ImageType2DMS, Volatile, Int4 >,			Image< ImageType2DMS, Volatile, UInt4 >,
		Image< ImageType2DMS, Restrict, Float4 >,		Image< ImageType2DMS, Restrict, Int4 >,			Image< ImageType2DMS, Restrict, UInt4 >,
		Image< ImageType2DMSArray, ReadOnly, Float4 >,	Image< ImageType2DMSArray, ReadOnly, Int4 >,	Image< ImageType2DMSArray, ReadOnly, UInt4 >,
		Image< ImageType2DMSArray, WriteOnly, Float4 >,	Image< ImageType2DMSArray, WriteOnly, Int4 >,	Image< ImageType2DMSArray, WriteOnly, UInt4 >,
		Image< ImageType2DMSArray, Coherent, Float4 >,	Image< ImageType2DMSArray, Coherent, Int4 >,	Image< ImageType2DMSArray, Coherent, UInt4 >,
		Image< ImageType2DMSArray, Volatile, Float4 >,	Image< ImageType2DMSArray, Volatile, Int4 >,	Image< ImageType2DMSArray, Volatile, UInt4 >,
		Image< ImageType2DMSArray, Restrict, Float4 >,	Image< ImageType2DMSArray, Restrict, Int4 >,	Image< ImageType2DMSArray, Restrict, UInt4 >,
		Image< ImageTypeCube, ReadOnly, Float4 >,		Image< ImageTypeCube, ReadOnly, Int4 >,			Image< ImageTypeCube, ReadOnly, UInt4 >,
		Image< ImageTypeCube, WriteOnly, Float4 >,		Image< ImageTypeCube, WriteOnly, Int4 >,		Image< ImageTypeCube, WriteOnly, UInt4 >,
		Image< ImageTypeCube, Coherent, Float4 >,		Image< ImageTypeCube, Coherent, Int4 >,			Image< ImageTypeCube, Coherent, UInt4 >,
		Image< ImageTypeCube, Volatile, Float4 >,		Image< ImageTypeCube, Volatile, Int4 >,			Image< ImageTypeCube, Volatile, UInt4 >,
		Image< ImageTypeCube, Restrict, Float4 >,		Image< ImageTypeCube, Restrict, Int4 >,			Image< ImageTypeCube, Restrict, UInt4 >,
		Image< ImageTypeCubeArray, ReadOnly, Float4 >,	Image< ImageTypeCubeArray, ReadOnly, Int4 >,	Image< ImageTypeCubeArray, ReadOnly, UInt4 >,
		Image< ImageTypeCubeArray, WriteOnly, Float4 >,	Image< ImageTypeCubeArray, WriteOnly, Int4 >,	Image< ImageTypeCubeArray, WriteOnly, UInt4 >,
		Image< ImageTypeCubeArray, Coherent, Float4 >,	Image< ImageTypeCubeArray, Coherent, Int4 >,	Image< ImageTypeCubeArray, Coherent, UInt4 >,
		Image< ImageTypeCubeArray, Volatile, Float4 >,	Image< ImageTypeCubeArray, Volatile, Int4 >,	Image< ImageTypeCubeArray, Volatile, UInt4 >,
		Image< ImageTypeCubeArray, Restrict, Float4 >,	Image< ImageTypeCubeArray, Restrict, Int4 >,	Image< ImageTypeCubeArray, Restrict, UInt4 >,
		Image< ImageType3D, ReadOnly, Float4 >,			Image< ImageType3D, ReadOnly, Int4 >,			Image< ImageType3D, ReadOnly, UInt4 >,
		Image< ImageType3D, WriteOnly, Float4 >,		Image< ImageType3D, WriteOnly, Int4 >,			Image< ImageType3D, WriteOnly, UInt4 >,
		Image< ImageType3D, Coherent, Float4 >,			Image< ImageType3D, Coherent, Int4 >,			Image< ImageType3D, Coherent, UInt4 >,
		Image< ImageType3D, Volatile, Float4 >,			Image< ImageType3D, Volatile, Int4 >,			Image< ImageType3D, Volatile, UInt4 >,
		Image< ImageType3D, Restrict, Float4 >,			Image< ImageType3D, Restrict, Int4 >,			Image< ImageType3D, Restrict, UInt4 > >;
	
/*
=================================================
	BuildNode
=================================================
*/
	template <typename Result, typename ...Args>
	inline Result  BuildNode (_ShaderNodesHidden_::StringCRef name, const Args& ...args)
	{
		return _ShaderNodesHidden_::NodeFunctions::Instance()->Build< Result >( name, args... );
	}


}	// Helpers
}	// ShaderNodes
}	// ShaderEditor
