// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/Common.h"
#include "Engine/STL/Dimensions/PowerOfTwoValue.h"

namespace Engine
{
namespace Platforms
{

	//
	// Multi Samples
	//
	
	struct MultiSamples : CompileTime::PODStruct
	{
	// types
	private:
		using Self	= MultiSamples;


	// variables
	private:
		PowOf2Value<uint>	_value;


	// methods
	public:
		MultiSamples (GX_DEFCTOR) : _value(0)
		{}

		explicit
		MultiSamples (PowOf2Value<uint> samples) : _value(samples)
		{}

		explicit
		MultiSamples (uint samples) : _value( PowOf2Value<uint>::From(samples) )
		{}

		ND_ uint Get ()			const		{ return _value.GetValue(); }
		ND_ uint GetPowerOf2 ()	const		{ return _value.GetPower(); }

		ND_ bool IsEnabled ()	const		{ return _value.GetPower() > 0; }

		_GX_DIM_CMP_OPERATORS_SELF( _value )
	};


}	// Platforms
}	// Engine
