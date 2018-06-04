// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/ResourcePacker/Pipelines/PipelineConverter.h"
#include "Engine/ResourcePacker/FilePacker/IFileSystemPacker.h"

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
		String					_exeName;


	// methods
	protected:
		ResourcePacker ();
		~ResourcePacker ();

	public:
		static Ptr<ResourcePacker> Instance ();

		bool Initialize ();

		void SetExecutable (StringCRef name);

		bool Run (StringCRef res);
		
		void AddPipeline (const PipelinePtr &ppln);

		StringCRef GetExecutableName () const	{ return _exeName; }


	private:
		//bool _PackFile (IFilePackerPtr &&file, StringCRef input, StringCRef output);

		static void _Bind (GXScript::ScriptEnginePtr se);
	};

}	// ResPack
