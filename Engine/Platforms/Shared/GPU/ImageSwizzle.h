// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"

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

		uint Get () const
		{
			return _value;
		}

		uint4 ToVec () const
		{
			return uint4( _value & 0xF, (_value >> 4) & 0xF, (_value >> 8) & 0xF, (_value >> 12) & 0xF );
		}

		bool IsDefault () const
		{
			return _value == ImageSwizzle().Get();
		}

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
		using Result_t	= HashResult;

		Result_t operator () (const Engine::Platforms::ImageSwizzle &value) const
		{
			return HashOf( value.Get() == 0 ? Engine::Platforms::ImageSwizzle().Get() : value.Get() );
		}
	};

}	// GXTypes
}	// GX_STL
