// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Generators/IGenerator.h"

namespace CodeGen
{

	//
	// GenFunction Serializer
	//

	class GenFunctionSerializer final
	{
	// types
	private:
		static constexpr uint	MAX_ARGS	= IGenerator::SrcNode::ArgIndices_t::MemoryContainer_t::SIZE;

		struct CmdNode
		{
		// types
			using ArgIndices_t = FixedSizeArray< uint, MAX_ARGS >;

		// variables
			String			src;
			uint			id;			// function id
			ArgIndices_t	args;		// node indices to put into command arguments

		// methods
			CmdNode () : id(UMax) {}
		};
		
		using GenFunction	= IGenerator::GenFunction;
		using CmdData		= Commands::CommandData;
		using ArgStrings_t	= FixedSizeArray< StringCRef, MAX_ARGS >;
		using NodeArray_t	= Array< CmdNode >;


	// methods
	public:
		static bool ToSource (const GenFunction &func, OUT String &src);


	private:
		template <typename T> static bool _ToAngelScript (const GenFunction &func, OUT String &src);
		
		static bool _BuildFuncList (ArrayCRef<CmdData> data, BitsU bitsPerCommand, uint programArgsCount,
									INOUT GenFunction &func, INOUT Array<CmdNode> &tmpNodes, INOUT Array<CmdNode> &nodes);
	};

}	// CodeGen
