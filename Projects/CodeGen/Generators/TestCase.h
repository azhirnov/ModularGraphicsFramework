// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Common.h"

namespace CodeGen
{

	//
	// Test Case
	//

	template <typename T>
	struct TestCase final
	{
	// types
	public:
		using Self			= TestCase<T>;
		//using Value_t		= T;
		using InputArr_t	= FixedSizeArray< T, 8 >;
		using Output_t		= T;


	// variables
	public:
		InputArr_t		input;
		Output_t		output;		// expected result


	// methods
	public:
		TestCase ()
		{}
	};

}	// CodeGen
