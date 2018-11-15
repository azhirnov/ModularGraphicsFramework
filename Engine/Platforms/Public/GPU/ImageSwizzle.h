// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/Common.h"

namespace Engine
{
namespace Platforms
{

	//
	// Image Color Swizzle
	//

	struct ImageSwizzle
	{
	// types
	public:
		using Self	= ImageSwizzle;

		
	// variables
	private:
		uint	_value	= 0x4321;	// 0 - unknown, 1 - R, 2 - G, 3 - B, 4 - A, 5 - O, 6 - 1, example: RGB0 - 0x1235


	// methods
	public:
		ImageSwizzle (GX_DEFCTOR) {}

		ImageSwizzle (Swizzle::type swizzle) :
			_value{ GXMath::_math_hidden_::_ParseSwizzle( swizzle ) }
		{
			ASSERT( _value != UMax );
		}

		ND_ uint Get () const
		{
			return _value;
		}

		ND_ uint4 ToVec () const
		{
			return uint4( _value & 0xF, (_value >> 4) & 0xF, (_value >> 8) & 0xF, (_value >> 12) & 0xF );
		}

		ND_ bool IsDefault () const
		{
			return _value == ImageSwizzle().Get();
		}

		DEBUG_ONLY(
		ND_ String ToString () const
		{
			const char	sw_values[8] = { ' ', 'R', 'G', 'B', 'A', '0', '1' };
			String	str;
			str << sw_values[ (_value >> 12) & 7 ];
			str << sw_values[ (_value >> 8) & 7 ];
			str << sw_values[ (_value >> 4) & 7 ];
			str << sw_values[ (_value >> 0) & 7 ];
			return str;
		})

		_GX_DIM_CMP_OPERATORS_SELF( _value )
	};


}	// Platforms
}	// Engine

namespace GX_STL
{
namespace GXTypes
{
	template <>
	struct Hash< Engine::Platforms::ImageSwizzle >
	{
		ND_ HashResult  operator () (const Engine::Platforms::ImageSwizzle &value) const
		{
			return HashOf( value.Get() == 0 ? Engine::Platforms::ImageSwizzle().Get() : value.Get() );
		}
	};

}	// GXTypes
}	// GX_STL
