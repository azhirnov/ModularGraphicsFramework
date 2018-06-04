// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Common.h"

namespace CodeGen
{

	struct EFitnessFunction
	{
		enum type : uint
		{
			FloatLinear,	// compatible with float, double and any integer.

			Bitwise,		// compatible with any integer, float and double, but not recomended to use with float point types.

			Unknown		= ~0u
		};
	};


	struct FitnessFunctions final : public Noninstancable
	{
		static StringCRef GetSource (EFitnessFunction::type func);
	};

}	// CodeGen
