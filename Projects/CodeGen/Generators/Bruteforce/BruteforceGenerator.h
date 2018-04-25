// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Generators/IGenerator.h"

namespace CodeGen
{

	//
	// Bruteforce Generator
	//

	class BruteforceGenerator final : public IGenerator
	{
	// types
	private:
		struct _Atomics
		{
			uint	total;
			uint	total2;		// if 'total' overflow
			uint	results;
			//uint	checked;
		};

		struct _ConstData
		{
			//BigInt	initHash;
			float	maxAccuracy;
			uint	resultsCapacity;
			uint	testCasesCount;
		};
		
		struct _Result
		{
			//BigInt	hash;
			float	ticks;
			float	accuracy;
		};

		using _BigIntBuf = FixedSizeArray< uint, 16 >;


	// variables
	private:
		ModulePtr		_gpuThread;
		ModulePtr		_syncManager;
		
		ModulePtr		_cmdBuilder;
		ModulePtr		_cmdBuffer;
		GpuFenceId		_fence;

		ModulePtr		_inBuffer;
		ModulePtr		_outBuffer;

		ModulePtr		_pipelineTempl;
		ModulePtr		_pipeline;
		ModulePtr		_resourceTable;
		
		BytesU			_testCaseSize;
		BytesU			_bigIntSize;
		BytesU			_constSize;
		BytesU			_atomicsSize;
		BytesU			_resultSize;

		uint			_maxCommands;
		uint			_maxInputs;
		uint			_bigintCount;

		uint			_iterationsCount;
		uint			_groupSize;

		const uint		_localThreads;


	// methods
	public:
		BruteforceGenerator ();

		bool Generate (VariantCRef testCases, ECommandSet::bits commandSetType, EConstantSet::bits constantType,
						float maxAccuracy, uint maxCommands, OUT GenFunctions_t &functions) override;
		
		bool ToSource (const GenFunction &func, OUT String &src) const override;


	private:
		template <typename T>
		bool _Gen (ArrayCRef<TestCase<T>> testCases, ECommandSet::bits commandSetType, EConstantSet::bits constantType,
					float maxAccuracy, uint maxCommands, OUT GenFunctions_t &functions);
		
		void _Clear ();

		template <typename T>	bool _Prepare (ArrayCRef<TestCase<T>> testCases, ECommandSet::bits commandSetType);
		template <typename T>	bool _CheckTests (ArrayCRef<TestCase<T>> testCases, OUT uint &maxInputs) const;
		template <typename T>	bool _CreateSource (ECommandSet::bits commandSetType);
								bool _CompileProgram (ArrayCRef<StringCRef> src, OUT CreateInfo::PipelineTemplate &ci);
								bool _CreateResources (usize testsCount, usize resultsCount);
		template <typename T>	bool _InitBuffer (ArrayCRef<TestCase<T>> testCases, float maxAccuracy, usize resultsCount);
								bool _BuildCommandBuffer ();
								bool _Execute ();
								bool _ProcessResults (usize resultsCount, ValueID::type typeId,
													  ECommandSet::bits commandSetType, EConstantSet::bits constantType,
													  OUT GenFunctions_t &functions);
	};

}	// CodeGen
