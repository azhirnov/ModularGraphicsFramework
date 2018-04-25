// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	names:
		inputs		- test cases input arguments
		outputs		- test cases output arguments
		commands	- nodes with commands
		cmdArgs		- command input arguments
		hash		- unique identifer of commands
		genfunction	- generated function, contins hash and additional information
		commandData	- command descriptor
*/

#pragma once

#include "Generators/BigInteger.h"
#include "Generators/Commands.h"
#include "Generators/TestCase.h"
#include "Generators/Scalar.h"

namespace CodeGen
{
	DECL_STRING_TO_ID( ValueID, 1 );


	//
	// Generator interface
	//

	class IGenerator
	{
	// types
	public:
		using HashCode_t	= BigInteger< ulong, 1 >;

		struct GenFunction final
		{
		// variables
			HashCode_t			hash;			// function hash
			ValueID::type		typeId;			// int/float/double/scalarf
			ulong				ticks;			// determines the execution time of the function
			float				accuracy;		// function results medium accuracy
			uint				inputsCount;	// args + constants
			uint				argsCount;
			ECommandSet::bits	commandSet;
			EConstantSet::bits	constants;

		// methods
			GenFunction () :
				typeId(), ticks(0), accuracy(0.0f), inputsCount(0), argsCount(0)
			{}
		};

		struct SrcNode final
		{
		// types
			using ArgIndices_t	= FixedSizeArray< uint, 8 >;

		// variables
			String			name;
			ArgIndices_t	args;	// node indices to put into command arguments

		// methods
			SrcNode () {}
			SrcNode (StringCRef name, ArrayCRef<uint> list) : name(name), args(list) {}
		};
		
		using GenFunctions_t	= Array< GenFunction >;
		using SrcNodes_t		= ArrayCRef< SrcNode >;


	// interface
	public:
		
		virtual bool Generate (VariantCRef testCases,
							   ECommandSet::bits commandSetType, EConstantSet::bits constantType,
							   float maxAccuracy, uint maxCommands,
							   OUT GenFunctions_t &functions) = 0;
		
		//virtual bool Run (VariantCRef input, const GenFunction &func, OUT VariantRef result) = 0;

		virtual bool ToSource (const GenFunction &func, OUT String &src) const = 0;

		//virtual bool CreateFromNodes (SrcNodes_t nodes, TypeId typeId, OUT GenFunction &func) = 0;
	};

}	// CodeGen
