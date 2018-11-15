// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/Common.h"
#include "Core/STL/Dimensions/PowerOfTwoValue.h"

namespace Engine
{
namespace Platforms
{

	//
	// Multi Samples
	//
	
	struct MultiSamples final : CompileTime::PODStruct
	{
	// types
	private:
		using Self	= MultiSamples;


	// variables
	private:
		PowOf2Value<uint>	_value;


	// methods
	public:
		constexpr MultiSamples (GX_DEFCTOR) : _value(0) {}

		explicit constexpr MultiSamples (PowOf2Value<uint> samples) : _value(samples) {}

		explicit constexpr MultiSamples (uint samples) : _value( PowOf2Value<uint>::From(samples) ) {}

		ND_ constexpr uint Get ()			const		{ return _value.GetValue(); }
		ND_ constexpr uint GetPowerOf2 ()	const		{ return _value.GetPower(); }

		ND_ constexpr bool IsEnabled ()		const		{ return _value.GetPower() > 0; }

		_GX_DIM_CMP_OPERATORS_SELF( _value )
	};
	

	ND_ inline constexpr MultiSamples operator "" _samples (unsigned long long value)	{ return MultiSamples( uint(value) ); }


}	// Platforms
}	// Engine
