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
#include "Generators/Fitness/FitnessFunctions.h"
#include "Generators/TestCase.h"
#include "Generators/Mixed.h"

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
		using FunctionHash_t	= BigInteger< uint, 4 >;
		using ConstantHash_t	= BigInteger< uint, 4 >;

		struct Parameters
		{
		// variables
			ValueID::type			typeId;			// int/float/double/scalarf/...
			ECommandSet::bits		commandSet;
			EConstantSet::bits		constants;
			EFitnessFunction::type	fitnessFunc;

		// methods
			Parameters (GX_DEFCTOR) :
				typeId(Uninitialized), fitnessFunc{Uninitialized}
			{}
		};


		struct GenFunction final : Parameters
		{
		// variables
			FunctionHash_t			funHash;
			ConstantHash_t			constHash;
			ulong					ticks;			// determines the execution time of the function
			float					accuracy;		// function results medium accuracy
			uint					maxInputs;		// args + max constants
			uint					argsCount;

		// methods
			GenFunction (GX_DEFCTOR) :
				ticks(0), accuracy(0.0f), maxInputs(0), argsCount(0)
			{}

			void SetParams (const Parameters &p) {
				typeId		= p.typeId;
				commandSet	= p.commandSet;
				constants	= p.constants;
				fitnessFunc	= p.fitnessFunc;
			}
		};


		struct SrcNode final
		{
		// types
			using ArgIndices_t	= FixedSizeArray< uint, 8 >;

		// variables
			String			name;
			ArgIndices_t	args;	// node indices to put into command arguments

		// methods
			SrcNode (GX_DEFCTOR) {}
			SrcNode (StringCRef name, ArrayCRef<uint> list) : name(name), args(list) {}
		};
		
		using GenFunctions_t	= Array< GenFunction >;
		using SrcNodes_t		= ArrayCRef< SrcNode >;


		struct Config
		{
		// variables
			ECommandSet::bits		commandSetType;
			EConstantSet::bits		constantType;
			EFitnessFunction::type	fitnessFunc;
			float					maxAccuracy;
			uint					maxConstants;
			uint					maxCommands;

		// methods
			Config (GX_DEFCTOR) :
				fitnessFunc{Uninitialized}, maxAccuracy{0.001f}, maxConstants{0}, maxCommands{10}
			{}
		};


		struct Statistic
		{
		// variables
			TimeD	duration;
			uint	commandCount;
			uint	resultCount;

		// methods
			Statistic (GX_DEFCTOR) :
				commandCount{0}, resultCount{0}
			{}
		};


	// interface
	public:
		virtual ~IGenerator () {}

		virtual bool Initialize (VariantCRef testCases, const Config &cfg) = 0;

		virtual bool Update () = 0;

		virtual bool GetResults (OUT Statistic &stat, OUT GenFunctions_t &functions, usize maxResults = 10) = 0;
	};

}	// CodeGen
