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

			// TODO
			//RTX_RayGeneration,
			//RTX_ClosestHit,
			//RTX_Miss,

			_Count,
			Unknown		= ~0u,
		};

		GX_ENUM_BITFIELD( EShader );

		static constexpr bits	AllGraphics	= bits().SetInterval( Vertex, Fragment );
		static constexpr bits	All			= bits().SetAll();

		static constexpr bool IsGraphics (type value);

		static StringCRef	ToString (type value);
		static String		ToString (bits values);
	};
	


	struct EShaderLangFormat
	{
		enum type : uint
		{
			// api
			_ApiOffset		= 18,
			_ApiMask		= 0xF << _ApiOffset,
			OpenGL			= 1 << _ApiOffset,
			OpenGLES		= 2 << _ApiOffset,
			DirectX			= 3 << _ApiOffset,
			OpenCL			= 4 << _ApiOffset,
			Vulkan			= 5 << _ApiOffset,
			Metal			= 6 << _ApiOffset,
			CUDA			= 7 << _ApiOffset,
			Software		= 8 << _ApiOffset,	// c++ shader for software renderer

			// version
			_VersionOffset	= 8,
			_VersionMask	= 0x3FF << _VersionOffset,
			OpenGL_330		= (330 << _VersionOffset) | OpenGL,
			OpenGL_420		= (420 << _VersionOffset) | OpenGL,
			OpenGL_450		= (450 << _VersionOffset) | OpenGL,
			OpenGL_460		= (460 << _VersionOffset) | OpenGL,
			OpenGLES_200	= (200 << _VersionOffset) | OpenGLES,
			OpenGLES_300	= (300 << _VersionOffset) | OpenGLES,
			OpenGLES_310	= (310 << _VersionOffset) | OpenGLES,
			OpenGLES_320	= (320 << _VersionOffset) | OpenGLES,
			DirectX_10		= (100 << _VersionOffset) | DirectX,
			DirectX_11		= (110 << _VersionOffset) | DirectX,
			DirectX_12		= (120 << _VersionOffset) | DirectX,
			OpenCL_120		= (120 << _VersionOffset) | OpenCL,
			OpenCL_200		= (200 << _VersionOffset) | OpenCL,
			OpenCL_210		= (210 << _VersionOffset) | OpenCL,
			Vulkan_100		= (100 << _VersionOffset) | Vulkan,
			Vulkan_110		= (110 << _VersionOffset) | Vulkan,
			Software_100	= (100 << _VersionOffset) | Software,

			// storage
			_StorageOffset	= 4,
			_StorageMask	= 0xF << _StorageOffset,
			Source			= 1 << _StorageOffset,
			Binary			= 2 << _StorageOffset,				// compiled program (HLSL bytecode, SPIRV)
			Executable		= 3 << _StorageOffset,				// compiled program (exe, dll)

			// format
			_FormatOffset	= 0,
			_FormatMask		= 0xF << _FormatOffset,
			HighLevel		= (1 << _FormatOffset) | Source,	// GLSL, HLSL, CL
			SPIRV			= (2 << _FormatOffset) | Binary,
			GL_Binary		= (3 << _FormatOffset) | Binary,	// vendor specific
			DXBC			= (4 << _FormatOffset) | Binary,	// HLSL bytecode
			DXIL			= (5 << _FormatOffset) | Binary,	// HLSL IL
			Assembler		= (6 << _FormatOffset) | Source,
			CPP_Invocable	= (7 << _FormatOffset) | Executable,	// function pointer

			Unknown			= 0,
		};

		GX_ENUM_BIT_OPERATIONS( type );

		static constexpr type			GetAPI (type value);
		static constexpr type			GetApiVersion (type value);
		static constexpr uint			GetVersion (type value);
		static constexpr GXMath::uint2	GetVersion2 (type value);
		static constexpr type			SetVersion (uint version);
		static constexpr type			GetStorage (type value);
		static constexpr type			GetFormat (type value);
		static constexpr type			GetApiFormat (type value);
		static constexpr bool			IsComputeApi (type value);
		static constexpr bool			IsGraphicsApi (type value);
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
// EShader
	
	ND_ inline constexpr bool  EShader::IsGraphics (type value)
	{
		return value >= Vertex and value <= Fragment;
	}



//-----------------------------------------------------------------------------//
// EShaderLangFormat
	
	ND_ inline constexpr EShaderLangFormat::type  EShaderLangFormat::GetAPI (type value)
	{
		return value & _ApiMask;
	}
	
	ND_ inline constexpr EShaderLangFormat::type  EShaderLangFormat::GetApiVersion (type value)
	{
		return value & (_ApiMask | _VersionMask);
	}

	ND_ inline constexpr uint  EShaderLangFormat::GetVersion (type value)
	{
		return (value & _VersionMask) >> _VersionOffset;
	}

	ND_ inline constexpr GXMath::uint2  EShaderLangFormat::GetVersion2 (type value)
	{
		uint	ver = GetVersion( value );
		return GXMath::uint2( ver / 100, (ver / 10) % 10 );
	}
	
	ND_ inline constexpr EShaderLangFormat::type  EShaderLangFormat::SetVersion (uint version)
	{
		return (version << _VersionOffset) & _VersionMask;
	}

	ND_ inline constexpr EShaderLangFormat::type  EShaderLangFormat::GetStorage (type value)
	{
		return value & _StorageMask;
	}
	
	ND_ inline constexpr EShaderLangFormat::type  EShaderLangFormat::GetFormat (type value)
	{
		return value & (_FormatMask | _StorageMask);
	}
	
	ND_ inline constexpr EShaderLangFormat::type  EShaderLangFormat::GetApiFormat (type value)
	{
		return value & (_ApiMask | _FormatMask | _StorageMask);
	}

	ND_ inline constexpr bool  EShaderLangFormat::IsComputeApi (type value)
	{
		switch ( GetAPI( value ) )
		{
			case OpenCL :
			case CUDA :
			case Software :
				return true;
		}
		return false;
	}
	
	ND_ inline constexpr bool  EShaderLangFormat::IsGraphicsApi (type value)
	{
		return not IsComputeApi( value );
	}



//-----------------------------------------------------------------------------//
// EShaderMemoryModel
	
	ND_ inline constexpr bool EShaderMemoryModel::HasReadAccess (type value)
	{
		switch ( value ) {
			case Default :	// == Coherent
			case Coherent :
			case Volatile :
			case Restrict :
			case ReadOnly :	return true;
		}
		return false;
	}

	ND_ inline constexpr bool EShaderMemoryModel::HasWriteAccess (type value)
	{
		switch ( value ) {
			case Default :	// == Coherent
			case Coherent :
			case Volatile :
			case Restrict :
			case WriteOnly : return true;
		}
		return false;
	}

	ND_ inline constexpr bool EShaderMemoryModel::HasReadWriteAccess (type value)
	{
		return HasReadAccess( value ) and HasWriteAccess( value );
	}


}	// Platforms
}	// Engine
