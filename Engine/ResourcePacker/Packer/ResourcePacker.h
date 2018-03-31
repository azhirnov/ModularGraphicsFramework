// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/ResourcePacker/Pipelines/PipelineConverter.h"

namespace ResPack
{

	//
	// Resource Packer
	//

	class ResourcePacker final
	{
		friend struct ScriptHelper;

	// types
	private:
		class ScriptResourcePacker;

		using PipelinePtr				= PipelineConverter::PipelinePtr;
		using PipelineConverterConfig	= PipelineConverter::ConverterConfig;


	// variables
	private:
		PipelineConverter		_pplnConverter;


	// methods
	protected:
		ResourcePacker ();
		~ResourcePacker ();

	public:
		static Ptr<ResourcePacker> Instance ();

		bool Initialize ();

		bool Run (StringCRef res);
		
		void AddPipeline (const PipelinePtr &ppln);


	private:
		static void _Bind (GXScript::ScriptEnginePtr se);
	};

}	// ResPack
