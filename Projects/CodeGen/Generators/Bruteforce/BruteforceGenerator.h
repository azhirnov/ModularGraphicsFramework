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
			uint	results;
			uint	bestResults;
		};
		
		struct _Result
		{
			//BigInt	hash;
			float	ticks;
			float	accuracy;
		};

		struct CmdBuffer
		{
			ModulePtr		cmd;
			ModulePtr		outputBuf;
			GpuFenceId		fence	= Uninitialized;
		};

		using _BigIntBuf	= StaticArray< ubyte, FunctionHash_t::Array_t::STATIC_COUNT * sizeof(FunctionHash_t::Value_t) >;

		using CmdBuffers_t	= StaticArray< CmdBuffer, 2 >;
		using Limit1_t		= StaticLimit< uint, 0, CmdBuffers_t::STATIC_COUNT-1 >;

		using DeviceProps_t	= GpuMsg::GetDeviceProperties::Properties;
		using Timer_t		= TimeProfilerD;


	// variables
	private:
		ModulePtr		_gpuThread;
		ModulePtr		_syncManager;
		DeviceProps_t	_devProperties;
		
		ModulePtr		_cmdBuilder;
		CmdBuffers_t	_cmdBuffers;
		Limit1_t		_cmdBufferIndex;

		ModulePtr		_inBuffer;
		ModulePtr		_inStagingBuffer;
		ModulePtr		_outBuffer;
		ModulePtr		_outStagingBuffer;

		ModulePtr		_pipelineTempl;
		ModulePtr		_pipeline;
		ModulePtr		_resourceTable;
		
		BytesU			_testCaseSize;
		BytesU			_bigIntSize;
		BytesU			_atomicsSize;
		BytesU			_resultSize;
		BytesU			_inputBufferSize;
		BytesU			_outputBufferSize;
		BitsU			_maxBits;		// number of actual bits in hash
		BitsU			_maxConstBits;

		uint			_maxFuncArgs;
		uint			_bigintCount;
		uint			_testsCount;

		FunctionHash_t	_funHash;
		ConstantHash_t	_constHash;
		Config			_config;
		Statistic		_statistic;
		ValueID::type	_typeId;
		Timer_t			_timer;

		uint			_localThreads;
		uint			_groupCount;

		const uint		_maxResults;
		bool			_completed;
		bool			_firstCall;

		bool			_breakOnBestResult;
		bool			_bestResultFound;


	// methods
	public:
		BruteforceGenerator ();
		
		bool Initialize (VariantCRef testCases, const Config &cfg) override;

		bool Update () override;

		bool GetResults (OUT Statistic &stat, OUT GenFunctions_t &functions, usize maxResults) override;


	private:
		template <typename T>
		bool _Gen (ArrayCRef<TestCase<T>> testCases);
		
		void _Clear ();
		bool _SaveDataToFile (uint size);

		template <typename T, usize S>
		static void _WriteBigInt (INOUT BinArrayRef &dst, const BigInteger<T,S> &src, uint numParts);

		template <typename T>	bool _Prepare (ArrayCRef<TestCase<T>> testCases);
		template <typename T>	bool _CheckTests (ArrayCRef<TestCase<T>> testCases, OUT uint &maxInputs) const;
		template <typename T>	bool _CreateSource ();
								bool _CompileProgram (ArrayCRef<StringCRef> src, OUT CreateInfo::PipelineTemplate &ci);
								bool _CreateResources ();
		template <typename T>	bool _InitBuffer (ArrayCRef<TestCase<T>> testCases);
		template <typename T>	bool _ProcessBlock (ArrayCRef<T> constants);
		template <typename T>	bool _ProcessBlockWithConst ();
								bool _CopyOutputToStagingBuffer ();
								bool _ProcessResults (OUT GenFunctions_t &functions, usize maxResults);
	};

}	// CodeGen
