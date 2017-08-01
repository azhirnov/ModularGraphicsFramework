// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"

namespace Engine
{
namespace Platforms
{

	//
	// Texture Array Layer
	//
	
	struct TexArrLayer
	{
	// types
	private:
		using Self	= TexArrLayer;


	// variables
	private:
		uint		_value;


	// methods
	public:
		TexArrLayer (GX_DEFCTOR) : _value(-1)
		{}

		explicit
		TexArrLayer (uint value) : _value(value)
		{}

		bool IsDefined ()	const	{ return _value != -1; }

		uint Get ()			const	{ return IsDefined() ? _value : 0; }

		_GX_DIM_CMP_OPERATORS_SELF( _value )
	};


}	// Platforms
}	// Engine
