// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/Common.h"

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
			Unknown	= ~0u,
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
			Unknown	= ~0u,
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
			Unknown	= ~0u,
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
			Unknown	= ~0u,
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
			Unknown	= ~0u,
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
			Unknown	= ~0u,
		};

		static StringCRef ToString (type value);
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
			Patch,

			_Count,
			Unknown		= ~0u,
		};

		GX_ENUM_BITFIELD( EPrimitive );

		static StringCRef ToString (type value);
	};
	

	struct EPolygonFace
	{
		enum type : uint
		{
			None		= 0,
			Front		= 1	<< 0,
			Back		= 1 << 1,
			FontAndBack	= Front | Back,

			//Unknown		= (1u << 31),
		};

		GX_ENUM_BIT_OPERATIONS( type );		// TODO: use GX_ENUM_BITFIELD
		
		static StringCRef ToString (type value);
	};
	

	struct EPipelineDynamicState
	{
		enum type : uint
		{
			Viewport,
			Scissor,
			LineWidth,
			DepthBias,
			BlendConstants,
			DepthBounds,
			StencilCompareMask,
			StencilWriteMask,
			StencilReference,

			_Count,
			Unknown		= ~0u,
		};

		GX_ENUM_BITFIELD( EPipelineDynamicState );
	};


}	// Platforms
}	// Engine
