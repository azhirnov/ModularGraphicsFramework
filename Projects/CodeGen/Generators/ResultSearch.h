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
		using CmdData			= Commands::CommandData;
		
		struct CmdNode
		{
		// types
			using ArgIndices_t = FixedSizeArray< uint, IGenerator::SrcNode::ArgIndices_t::MemoryContainer_t::SIZE >;

		// variables
			uint			id;			// function id
			ArgIndices_t	args;		// node indices to put into command arguments

		// methods
			CmdNode () : id(UMax) {}
		};


	// methods
	public:
		static bool SearchResults (BinArrayCRef results, usize resultsCount, uint maxInputs, BytesU resultSize,
								   BytesU bigIntSize, ValueID::type typeId, ECommandSet::bits commandSet, EConstantSet::bits constantSet,
								   OUT GenFunctions_t &functions);
		
		static bool ToSource (const GenFunction &func, OUT String &src);

	private:
		static void _SearchBestResults (INOUT GenFunctions_t &functions, usize maxResults = 10, bool withBestAccuracy = true);

		static	float _MinAccuracy ()		{ return 0.001f; }

		template <typename T> static bool _ToSource (const GenFunction &func, OUT String &src);
		
		static bool _BuildFuncList (const ArrayCRef<CmdData> data, const BitsU bitsPerCommand,
									INOUT GenFunction &func, INOUT Array<CmdNode> &tmpNodes, INOUT Array<CmdNode> &nodes);
	};

}	// CodeGen
