// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/Common.h"

namespace Engine
{
namespace Platforms
{

	//
	// Image Array Layer
	//
	
	struct ImageLayer final : CompileTime::PODStruct
	{
	// types
	private:
		using Self	= ImageLayer;


	// variables
	private:
		uint		_value;


	// methods
	public:
		constexpr ImageLayer (GX_DEFCTOR) : _value(UMax) {}

		explicit constexpr ImageLayer (uint value) : _value(value) {}

		ND_ constexpr bool IsDefined ()	const	{ return _value != UMax; }

		ND_ constexpr uint Get ()		const	{ return IsDefined() ? _value : 0; }

		_GX_DIM_CMP_OPERATORS_SELF( _value )
	};
	

	ND_ inline constexpr ImageLayer operator "" _layer (unsigned long long value)		{ return ImageLayer( uint(value) ); }


}	// Platforms
}	// Engine

namespace GX_STL
{
namespace GXTypes
{
	template <>
	struct Hash< Engine::Platforms::ImageLayer >
	{
		ND_ HashResult  operator () (const Engine::Platforms::ImageLayer &value) const
		{
			return HashOf( value.Get() );
		}
	};

}	// GXTypes
}	// GX_STL
