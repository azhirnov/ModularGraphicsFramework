// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/PixelFormatEnums.h"

namespace Engine
{
namespace Platforms
{
	
	struct EImage
	{
		enum type : uint
		{
			Tex1D		= 0,
			Tex1DArray,
			Tex2D,
			Tex2DArray,
			Tex2DMS,
			Tex2DMSArray,
			TexCube,
			TexCubeArray,
			Tex3D,
			Buffer,

			_Count,
			Unknown		= ~0u,
		};

		static constexpr bool IsMultisampled (type value);

		static StringCRef ToString (type value);
	};


	// see https://www.khronos.org/registry/vulkan/specs/1.0/html/vkspec.html#VkImageUsageFlagBits
	struct EImageUsage
	{
		enum type : uint
		{
			TransferSrc = 0,			// for all copy operations
			TransferDst,				// for all copy operations
			Sampled,					// access in shader as texture
			Storage,					// access in shader as image
			ColorAttachment,			// color or resolve attachment
			DepthStencilAttachment,		// depth/stencil attachment
			TransientAttachment,		// color, resolve, depth/stencil, input attachment
			InputAttachment,			// input attachment in fb and shader

			_Count,
			Unknown = ~0u,
		};

		GX_ENUM_BITFIELD( EImageUsage );
	};
	

	// see https://www.khronos.org/registry/vulkan/specs/1.0/html/vkspec.html#VkImageLayout
	struct EImageLayout
	{
		enum type : uint
		{
			Undefined = 0,
			General,
			ColorAttachmentOptimal,
			DepthStencilAttachmentOptimal,
			DepthStencilReadOnlyOptimal,
			ShaderReadOnlyOptimal,
			TransferSrcOptimal,
			TransferDstOptimal,
			Preinitialized,
			PresentSrc,

			_Count,
			Unknown = ~0u,
		};
	};


	struct ERenderTarget
	{
		enum type : uint
		{
			Depth		= 0,
			Stencil,
			DepthStencil,
			Color0,
			ColorMax	= Color0 + GlobalConst::Graphics_MaxColorBuffers,

			_Count		= ColorMax,
			Unknown		= ~0u,
		};

		static constexpr type	ToColor (uint index);
		static constexpr bool	IsColor (type value);

		static constexpr type	FromPixelFormat (EPixelFormat::type fmt, uint colorIndex = 0);

		template <uint I> static constexpr type	Color	= _ToColor<I>::value;

	private:
		template <uint I> struct _ToColor;
	};



//-----------------------------------------------------------------------------//
// EImage
	
	inline constexpr bool EImage::IsMultisampled (type value)
	{
		return value == Tex2DMS or value == Tex2DMSArray;
	}



//-----------------------------------------------------------------------------//
// ERenderTarget
	
	inline constexpr ERenderTarget::type  ERenderTarget::ToColor (uint index)
	{
		//ASSERT(index < GlobalConst::Graphics_MaxColorBuffers);
		return type( index + Color0 );
	}


	inline constexpr bool  ERenderTarget::IsColor (type value)
	{
		return value >= Color0 and value < ColorMax;
	}
	

	inline constexpr ERenderTarget::type  ERenderTarget::FromPixelFormat (EPixelFormat::type fmt, uint colorIndex)
	{
		return	EPixelFormat::IsDepthStencil( fmt ) ?	DepthStencil :
				EPixelFormat::IsDepth( fmt ) ?			Depth :
				EPixelFormat::IsStencil( fmt ) ?		Stencil :
					ToColor( colorIndex );
	}


	template <uint I>
	struct ERenderTarget::_ToColor
	{
		STATIC_ASSERT( I < GlobalConst::Graphics_MaxColorBuffers );

		static const type	value = type( I + Color0 );
	};


	template <uint I>
	constexpr ERenderTarget::type	ERenderTarget::Color;


}	// Platforms
}	// Engine
