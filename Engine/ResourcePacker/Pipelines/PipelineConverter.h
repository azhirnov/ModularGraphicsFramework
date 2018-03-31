// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/ResourcePacker/Pipelines/ScriptGraphicsPipeline.h"
#include "Engine/ResourcePacker/Pipelines/ScriptComputePipeline.h"

namespace ResPack
{

	//
	// Pipeline Converter
	//

	class PipelineConverter final
	{
	// types
	public:
		using ConverterConfig	= PipelineCompiler::PipelineManager::ConverterConfig;
		using PipelinePtr		= PipelineCompiler::BasePipelinePtr;
		using PipelinesSet		= Set< PipelinePtr >;
		using PepelineTemplates	= HashMap< String, PipelinePtr >;


	// variables
	private:
		ConverterConfig		_pplnConfig;

		PipelinesSet		_pipelines;
		PepelineTemplates	_templates;


	// methods
	public:
		PipelineConverter ();
		~PipelineConverter ();
		
		void AddPipeline (const PipelinePtr &ppln);
		
		bool LoadPipeline (StringCRef fname);
		bool LoadAllPipelines (StringCRef folder);

		bool LoadPipelineTemplate (StringCRef fname, StringCRef funcName);
		
		void SetConfig (const ConverterConfig &cfg);
		bool ConvertPipelines (StringCRef outFolder);

	private:
		static void _GetGraphicsPipelineTemplate (AngelScript::asIScriptGeneric *);
		static void _GetComputePipelineTemplate (AngelScript::asIScriptGeneric *);
	};

}	// ResPack
