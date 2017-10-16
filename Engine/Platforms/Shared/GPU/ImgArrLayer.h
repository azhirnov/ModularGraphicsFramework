// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"

namespace Engine
{
namespace Platforms
{

	//
	// Image Array Layer
	//
	
	struct ImgArrLayer
	{
	// types
	private:
		using Self	= ImgArrLayer;


	// variables
	private:
		uint		_value;


	// methods
	public:
		ImgArrLayer (GX_DEFCTOR) : _value(-1)
		{}

		explicit
		ImgArrLayer (uint value) : _value(value)
		{}

		bool IsDefined ()	const	{ return _value != -1; }

		uint Get ()			const	{ return IsDefined() ? _value : 0; }

		_GX_DIM_CMP_OPERATORS_SELF( _value )
	};


}	// Platforms
}	// Engine
