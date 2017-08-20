// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Shaders/Pipeline/BasePipelineCompiler.h"

namespace ShaderEditor
{
namespace PipelineNodes
{

	//
	// Vulkan Pipeline Compiler
	//

	class VkPipelineCompiler final : public BasePipelineCompiler
	{
	// types
	private:


	// variables
	private:


	// methods
	public:
		VkPipelineCompiler ();
		
		bool Compile (const Array<NodePtr> &nodes, EShader::type shaderType, OUT ModulePtr &program) override;
	};


}	// PipelineNodes
}	// ShaderEditor
