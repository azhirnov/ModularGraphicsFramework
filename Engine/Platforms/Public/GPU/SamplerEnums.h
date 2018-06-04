// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/Common.h"

namespace Engine
{
namespace Platforms
{
	
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
			Unknown	= ~0u,
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

		static constexpr bool IsMipmapLinear (type value);
		static constexpr bool IsMinLinear (type value);
		static constexpr bool IsMagLinear (type value);
		static constexpr uint GetAnisotropic (type value);
	};

	
	struct ESamplerBorderColor
	{
		enum type : uint
		{
			Float,
			Int,
			Transparent,
			Black,
			White,

			_Count,
			Unknown		= ~0u,
		};

		GX_ENUM_BITFIELD( ESamplerBorderColor );

		static String ToString (bits value);
	};

	



//-----------------------------------------------------------------------------//
// EFilter
	
	ND_ inline constexpr bool EFilter::IsMipmapLinear (type value)
	{
		return EnumEq( value, EFilter::_MIP_LINEAR );
	}

	ND_ inline constexpr bool EFilter::IsMinLinear (type value)
	{
		return EnumEq( value, EFilter::_MIN_LINEAR );
	}

	ND_ inline constexpr bool EFilter::IsMagLinear (type value)
	{
		return EnumEq( value, EFilter::_MAG_LINEAR );
	}
	
	ND_ inline constexpr uint EFilter::GetAnisotropic (type value)
	{
		return EnumEq( value, EFilter::_ANISOTROPIC ) ? uint(value & _A_FACTOR_MASK) : 0;
	}


}	// Platforms
}	// Engine
