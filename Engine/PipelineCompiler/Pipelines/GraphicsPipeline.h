// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/PipelineCompiler/Pipelines/BasePipeline.h"

namespace PipelineCompiler
{

	//
	// Graphics Pipeline
	//

	class GraphicsPipeline : public BasePipeline
	{
	// types
	protected:
		struct Shaders
		{
		// variables
			ShaderModule	vertex			{ EShader::Vertex };
			ShaderModule	tessControl		{ EShader::TessControl };
			ShaderModule	tessEvaluation	{ EShader::TessEvaluation };
			ShaderModule	geometry		{ EShader::Geometry };
			ShaderModule	fragment		{ EShader::Fragment };

		// methods
			Shaders () {}
		};
	

	// variables
	protected:
		RenderState						renderState;
		Shaders							shaders;
		EPipelineDynamicState::bits		dynamicStates;
		EPrimitive::bits				supportedTopology;

		// TODO: render pass, subpass
		// - textures, images can be defined in C++ code or in shader source
		// - buffers description loaded from shader source
		// - shader input arguments must be defined in shader source
		// - vertex attributes can be defined in C++ code or in shader source
		// - fragment outputs can be defined in C++ code (render pass) or in shader source
		// - patchControlPoints must be defined in shader source
		// - constant_id - 'dynamic' constants (can be changed before pipeline will be created)
		// - subpassInput
		// - sampler - separate sampler declaration


	// methods
	protected:
		explicit GraphicsPipeline (StringCRef name);

	private:
		bool Prepare (const ConverterConfig &cfg) override final;
		bool Convert (OUT String &src, Ptr<ISerializer> ser, const ConverterConfig &cfg) const override final;

		bool _ConvertVertexShader (INOUT String &src, Ptr<ISerializer> ser, const ConverterConfig &cfg) const;
		bool _ConvertTessControlShader (INOUT String &src, Ptr<ISerializer> ser, const ConverterConfig &cfg) const;
		bool _ConvertTessEvaluationShader (INOUT String &src, Ptr<ISerializer> ser, const ConverterConfig &cfg) const;
		bool _ConvertGeometryShader (INOUT String &src, Ptr<ISerializer> ser, const ConverterConfig &cfg) const;
		bool _ConvertFragmentShader (INOUT String &src, Ptr<ISerializer> ser, const ConverterConfig &cfg) const;

		bool _CheckShaderCompatibility (INOUT ShaderDisasemblies &compiled);
		bool _CompareVaryings (const Array<Varying> &input, INOUT Array<Varying> &output) const;
	};


}	// PipelineCompiler
