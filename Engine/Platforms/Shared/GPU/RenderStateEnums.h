// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"

namespace Engine
{
namespace Platforms
{
	
	struct EBlendFunc
	{
		// result = srcColor * srcBlend [blendOp] dstColor * dstBlend;
		enum type : uint
		{
			Zero = 0,					// 0
			One,						// 1
			SrcColor,					// src
			OneMinusSrcColor,			// 1 - src
			DstColor,					// dst
			OneMinusDstColor,			// 1 - dst
			SrcAlpha,					// src.a
			OneMinusSrcAlpha,			// 1 - src.a
			DstAlpha,					// dst.a
			OneMinusDstAlpha,			// 1 - dst.a
			ConstColor,					// cc
			OneMinusConstColor,			// 1 - cc
			ConstAlpha,					// cc.a
			OneMinusConstAlpha,			// 1 - cc.a
			SrcAlphaSaturate,			// rgb * min( src.a, dst.a ), a * 1
			
			_Count,
			Unknown	= uint(-1),
		};

		static StringCRef ToString (type value);
	};


	struct EBlendEq
	{
		// result = srcColor * srcBlend [blendOp] dstColor * dstBlend;
		enum type : uint
		{
			Add = 0,		// S+D
			Sub,			// S-D
			RevSub,			// D-S
			Min,			// min(S,D)
			Max,			// max(S,D)
			
			_Count,
			Unknown	= uint(-1),
		};

		// TODO: add from https://www.opengl.org/registry/specs/KHR/blend_equation_advanced.txt

		static StringCRef ToString (type value);
	};


	struct ELogicOp
	{
		enum type : uint
		{
			None = 0,			// disabled
			Clear,				// 0
			Set,				// 1
			Copy,				// S
			CopyInverted,		// ~S
			Noop,				// D
			Invert,				// ~D
			And,				// S & D
			NotAnd,				// ~ ( S & D )
			Or,					// S | D
			NotOr,				// ~ ( S | D )
			Xor,				// S ^ D
			Equiv,				// ~ ( S ^ D )
			AndReverse,			// S & ~D
			AndInverted,		// ~S & D
			OrReverse,			// S | ~D
			OrInverted,			// ~S | D

			_Count,
			Unknown	= uint(-1),
		};

		static StringCRef ToString (type value);
	};


	struct ECompareFunc
	{
		enum type : uint
		{
			None = 0,		// (disable)
			Never,			// false
			Less,			// <
			Equal,			// ==
			LEqual,			// <=
			Greater,		// >
			NotEqual,		// !=
			GEqual,			// >=
			Always,			// true
			
			_Count,
			Unknown	= uint(-1),
		};

		static StringCRef ToString (type value);
	};


	struct EStencilOp
	{
		enum type : uint
		{
			Keep = 0,		// s
			Zero,			// 0
			Replace,		// ref
			Incr,			// min( ++s, 0 )
			IncrWrap,		// ++s & maxvalue
			Decr,			// max( --s, 0 )
			DecrWrap,		// --s & maxvalue
			Invert,			// ~s
			
			_Count,
			Unknown	= uint(-1),
		};

		static StringCRef ToString (type value);
	};


	struct EPolygonMode
	{
		enum type : uint
		{
			Point = 0,
			Line,
			Fill,

			_Count,
			Unknown	= uint(-1),
		};

		static StringCRef ToString (type value);
	};


	struct EAddressMode
	{
		enum type : uint
		{
			Clamp = 0,
			ClampToEdge = Clamp,
			ClampToBorder,
			Repeat,
			MirroredRepeat,
			MirroredClamp,
			ClampUnnorm,

			_Count,
			Unknown	= uint(-1),
		};

		static StringCRef ToString (type value);
	};


	struct EFilter
	{
		enum type : uint
		{
			Unknown				= 0,

			_MIN_NEAREST		= 1 << 8,
			_MIN_LINEAR			= 1 << 9,
			_MAG_NEAREST		= 1 << 10,
			_MAG_LINEAR			= 1 << 11,
			_MIP_NEAREST		= 1 << 12,
			_MIP_LINEAR			= 1 << 13,
			_ANISOTROPIC		= 1 << 14,
			_A_FACTOR_MASK		= 0xFF,

			_MIP_MASK			= _MIP_NEAREST | _MIP_LINEAR,
			_MIN_MASK			= _MIN_NEAREST | _MIN_LINEAR,
			_MAG_MASK			= _MAG_NEAREST | _MAG_LINEAR,

			MinMagMipNearest				= _MIN_NEAREST | _MAG_NEAREST | _MIP_NEAREST,
			MinMagNearest_MipLinear			= _MIN_NEAREST | _MAG_NEAREST | _MIP_LINEAR,
			MinNearest_MagLinear_MipNearest	= _MIN_NEAREST | _MAG_LINEAR  | _MIP_NEAREST,
			MinNearest_MagMipLinear			= _MIN_NEAREST | _MAG_LINEAR  | _MIP_LINEAR,
			MinLinear_MagMipNearest			= _MIN_LINEAR  | _MAG_NEAREST | _MIP_NEAREST,
			MinLinear_MagNearest_MipLinear	= _MIN_LINEAR  | _MAG_NEAREST | _MIP_LINEAR,
			MinMagLinear_MipNearest			= _MIN_LINEAR  | _MAG_LINEAR  | _MIP_NEAREST,
			MinMagMipLinear					= _MIN_LINEAR  | _MAG_LINEAR  | _MIP_LINEAR,
			Anisotropic_2					= _ANISOTROPIC | MinMagMipLinear | 2,
			Anisotropic_4					= _ANISOTROPIC | MinMagMipLinear | 4,
			Anisotropic_8					= _ANISOTROPIC | MinMagMipLinear | 8,
			Anisotropic_12					= _ANISOTROPIC | MinMagMipLinear | 12,
			Anisotropic_16					= _ANISOTROPIC | MinMagMipLinear | 16,
		};

		GX_ENUM_BIT_OPERATIONS( type );

		static StringCRef ToString (type value);

		static bool IsMipmapLinear (type value);
		static bool IsMinLinear (type value);
		static bool IsMagLinear (type value);
		static uint GetAnisotropic (type value);
	};

	
	struct EPrimitive
	{
		enum type : uint
		{
			Point		= 0,
			LineList,
			LineStrip,
			TriangleList,
			TriangleStrip,

			_Count,
			Unknown		= uint(-1),
		};

		static StringCRef ToString (type value);
	};
	

	struct EPolygonFace
	{
		enum type : uint
		{
			None		= 0,
			Front		= 1,
			Back		= 2,
			FontAndBack	= Front | Back,

			Unknown		= 0,
		};

		GX_ENUM_BIT_OPERATIONS( type );
		
		static StringCRef ToString (type value);
	};

	
	struct EVertexInputRate
	{
		enum type : uint
		{
			Vertex	= 0,
			Instance,

			_Count,
			Unknown		= uint(-1),
		};
	};
	

	/*struct EFragOutput
	{
		enum type : uint
		{
			Unknown		= 0,
			Int4		= EValueTypeInfo::Int4,
			UInt4		= EValueTypeInfo::UInt4,
			Float4		= EValueTypeInfo::Float4,
		};

		static type From (EPixelFormat::type fmt);
		static StringCRef ToString (type value);
	};*/
	

	/*struct EPointSpriteCoordOrigin
	{
		enum type : uint
		{
			UpperLeft	= 0,
			LowerLeft,

			_Count,
			Unknown		= uint(-1),
		};

		static StringCRef ToString (type value);
	};*/
	

	struct EPipelineDynamicState
	{
		enum type : uint
		{
			Viewport	= 0,
			Scissor,
			LineWidth,
			DepthBias,
			BlendConstants,
			DepthBounds,
			StencilCompareMask,
			StencilWriteMask,
			StencilReference,

			_Count,
			Unknown		= uint(-1),
		};

		using bits = EnumSet< EPipelineDynamicState >;
	};

}	// Platforms
}	// Engine
