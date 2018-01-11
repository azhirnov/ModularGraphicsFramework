// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/VertexEnums.h"

namespace Engine
{
namespace Platforms
{

	//
	// Vertex Info
	//

	template <typename T>
	struct VertexDescr
	{
	};


	// Float types
	template <>
	struct VertexDescr <half>
	{
		typedef half							type;
		static const EVertexAttribute::type		attrib	= EVertexAttribute::Half;
	};

	template <>
	struct VertexDescr <float>
	{
		typedef float							type;
		static const EVertexAttribute::type		attrib	= EVertexAttribute::Float;
	};

	template <>
	struct VertexDescr <double>
	{
		typedef double							type;
		static const EVertexAttribute::type		attrib	= EVertexAttribute::Double;
	};


	// Integer types
	template <>
	struct VertexDescr <byte>
	{
		typedef byte							type;
		static const EVertexAttribute::type		attrib	= EVertexAttribute::Byte;
	};

	template <>
	struct VertexDescr <ubyte>
	{
		typedef ubyte							type;
		static const EVertexAttribute::type		attrib	= EVertexAttribute::UByte;
	};


	template <>
	struct VertexDescr <short>
	{
		typedef short							type;
		static const EVertexAttribute::type		attrib	= EVertexAttribute::Short;
	};

	template <>
	struct VertexDescr <ushort>
	{
		typedef ushort							type;
		static const EVertexAttribute::type		attrib	= EVertexAttribute::UShort;
	};


	template <>
	struct VertexDescr <int>
	{
		typedef int								type;
		static const EVertexAttribute::type		attrib	= EVertexAttribute::Int;
	};

	template <>
	struct VertexDescr <uint>
	{
		typedef uint							type;
		static const EVertexAttribute::type		attrib	= EVertexAttribute::UInt;
	};


	template <>
	struct VertexDescr <ilong>
	{
		typedef ilong							type;
		static const EVertexAttribute::type		attrib	= EVertexAttribute::Long;
	};

	template <>
	struct VertexDescr <ulong>
	{
		typedef ulong							type;
		static const EVertexAttribute::type		attrib	= EVertexAttribute::ULong;
	};


	// Vector types
	template <typename T, usize I, ulong U>
	struct VertexDescr < Vec<T,I,U> >
	{
		typedef Vec<T,I,U>						type;
		static const EVertexAttribute::type		attrib	= EVertexAttribute::type( (VertexDescr< typename type::Value_t >
															::attrib & _platforms_hidden_::EValueTypeInfo::_TYPE_FLAG_MASK) |
																(I << _platforms_hidden_::EValueTypeInfo::_COL_OFF) );
	};


	template <>
	struct VertexDescr <color4u>
	{
		typedef color4u							type;
		static const EVertexAttribute::type		attrib	= EVertexAttribute::UByte4_Norm;
	};
	

}	// Platforms
}	// Engine
