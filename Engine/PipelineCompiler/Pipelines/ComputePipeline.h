// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/PipelineCompiler/Pipelines/BasePipeline.h"

namespace PipelineCompiler
{

	//
	// Compute Pipeline
	//

	class ComputePipeline : public BasePipeline
	{
	// variables
	protected:
		ShaderModule	shader		{ EShader::Compute };


	// methods
	protected:
		explicit ComputePipeline (StringCRef name);
		
	private:
		bool Prepare (const ConverterConfig &cfg) override final;
		bool Convert (OUT String &src, Ptr<ISerializer> ser, const ConverterConfig &cfg) const override final;
		
		bool _ConvertComputeShader (INOUT String &src, Ptr<ISerializer> ser, const ConverterConfig &cfg) const;
	};

}	// PipelineCompiler
