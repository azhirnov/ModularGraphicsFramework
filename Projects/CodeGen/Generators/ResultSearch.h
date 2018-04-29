// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Generators/IGenerator.h"

namespace CodeGen
{

	//
	// Result Search
	//

	struct ResultSearch final
	{
	// types
	private:
		using GenFunction		= IGenerator::GenFunction;
		using GenFunctions_t	= IGenerator::GenFunctions_t;


	// methods
	public:
		static bool SearchResults (BinArrayCRef results, usize resultsCount, uint maxInputs, BytesU resultSize,
								   BytesU bigIntSize, ValueID::type typeId, ECommandSet::bits commandSet, EConstantSet::bits constantSet,
								   OUT GenFunctions_t &functions);

	private:
		static void _SearchBestResults (INOUT GenFunctions_t &functions, usize maxResults = 10, bool withBestAccuracy = true);

		static	float _MinAccuracy ()		{ return 0.001f; }
	};

}	// CodeGen
