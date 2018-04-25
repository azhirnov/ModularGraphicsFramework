// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/Common.h"

namespace Engine
{
namespace Platforms
{
	
	struct EShader
	{
		enum type : uint
		{
			Vertex		= 0,
			TessControl,
			TessEvaluation,
			Geometry,
			Fragment,
			Compute,

			_Count,
			Unknown		= ~0u,
		};

		GX_ENUM_BITFIELD( EShader );

		static constexpr bits	All	= bits().SetInterval( Vertex, Fragment );

		static StringCRef ToString (type value);
	};
	
	
	struct EShaderMemoryModel
	{
		// see https://www.khronos.org/opengl/wiki/Image_Load_Store#Memory_qualifiers
		// https://www.khronos.org/opengl/wiki/Memory_Model

		enum type : uint
		{
			Default,	// == Coherent

			Coherent,
			Volatile,
			Restrict,
			ReadOnly,
			WriteOnly,

			Unknown		= ~0u,
		};

		static StringCRef ToString (type value);
		
		static constexpr bool HasReadWriteAccess (type value);
		static constexpr bool HasWriteAccess (type value);
		static constexpr bool HasReadAccess (type value);
	};
	


//-----------------------------------------------------------------------------//
// EShaderMemoryModel
	
	inline constexpr bool EShaderMemoryModel::HasReadAccess (type value)
	{
		switch ( value ) {
			case Coherent :
			case Volatile :
			case Restrict :
			case ReadOnly :	return true;
		}
		return false;
	}

	inline constexpr bool EShaderMemoryModel::HasWriteAccess (type value)
	{
		switch ( value ) {
			case Coherent :
			case Volatile :
			case Restrict :
			case WriteOnly : return true;
		}
		return false;
	}

	inline constexpr bool EShaderMemoryModel::HasReadWriteAccess (type value)
	{
		return HasReadAccess( value ) and HasWriteAccess( value );
	}

}	// Platforms
}	// Engine
