// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"

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

		static constexpr bool IsMipmapLinear (type value);
		static constexpr bool IsMinLinear (type value);
		static constexpr bool IsMagLinear (type value);
		static constexpr uint GetAnisotropic (type value);
	};

	
	struct ESamplerBorderColor
	{
		enum type : uint
		{
			Float		= 1 << 0,
			Int			= 1 << 1,

			Transparent	= 1 << 8,
			Black		= 1 << 9,
			White		= 1 << 10,

			Unknown		= 0,
		};

		static String ToString (type value);

		GX_ENUM_BIT_OPERATIONS( type );
	};

	



//-----------------------------------------------------------------------------//
// EFilter
	
	inline StringCRef EFilter::ToString (type value)
	{
		switch ( value )
		{
			case EFilter::MinMagMipNearest					:	return "MinMagMipNearest";
			case EFilter::MinMagNearest_MipLinear			:	return "MinMagNearest_MipLinear";
			case EFilter::MinNearest_MagLinear_MipNearest	:	return "MinNearest_MagLinear_MipNearest";
			case EFilter::MinNearest_MagMipLinear			:	return "MinNearest_MagMipLinear";
			case EFilter::MinLinear_MagMipNearest			:	return "MinLinear_MagMipNearest";
			case EFilter::MinLinear_MagNearest_MipLinear	:	return "MinLinear_MagNearest_MipLinear";
			case EFilter::MinMagLinear_MipNearest			:	return "MinMagLinear_MipNearest";
			case EFilter::MinMagMipLinear					:	return "MinMagMipLinear";
			case EFilter::Anisotropic_2						:	return "Anisotropic_2";
			case EFilter::Anisotropic_4						:	return "Anisotropic_4";
			case EFilter::Anisotropic_8						:	return "Anisotropic_8";
			case EFilter::Anisotropic_12					:	return "Anisotropic_12";
			case EFilter::Anisotropic_16					:	return "Anisotropic_16";
		}
		RETURN_ERR( "invalid filter type", "unknown" );
	}

	inline constexpr bool EFilter::IsMipmapLinear (type value)
	{
		return EnumEq( value, EFilter::_MIP_LINEAR );
	}

	inline constexpr bool EFilter::IsMinLinear (type value)
	{
		return EnumEq( value, EFilter::_MIN_LINEAR );
	}

	inline constexpr bool EFilter::IsMagLinear (type value)
	{
		return EnumEq( value, EFilter::_MAG_LINEAR );
	}
	
	inline constexpr uint EFilter::GetAnisotropic (type value)
	{
		return EnumEq( value, EFilter::_ANISOTROPIC ) ? uint(value & _A_FACTOR_MASK) : 0;
	}



//-----------------------------------------------------------------------------//
// EAddressMode
	
	inline StringCRef EAddressMode::ToString (type value)
	{
		switch ( value )
		{
			case EAddressMode::ClampToEdge		:	return "ClampToEdge";
			case EAddressMode::ClampToBorder	:	return "ClampToBorder";
			case EAddressMode::Repeat			:	return "Repeat";
			case EAddressMode::MirroredRepeat	:	return "MirroredRepeat";
			case EAddressMode::MirroredClamp	:	return "MirroredClamp";
			case EAddressMode::ClampUnnorm		:	return "ClampUnnorm";
		}
		RETURN_ERR( "invalid wrap mode", "unknown" );
	}
	


//-----------------------------------------------------------------------------//
// ESamplerBorderColor
	
	inline String ESamplerBorderColor::ToString (type value)
	{
		String	str;

		if ( EnumEq( value, ESamplerBorderColor::Int ) )
			str << "Int";
		else
			str << "Float";

		if ( EnumEq( value, ESamplerBorderColor::Black ) )
			str << "Black";
		else
		if ( EnumEq( value, ESamplerBorderColor::White ) )
			str << "White";
		else
			str << "Transparent";

		return str;
	}


}	// Platforms
}	// Engine
