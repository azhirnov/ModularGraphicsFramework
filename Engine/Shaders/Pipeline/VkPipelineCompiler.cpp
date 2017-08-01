// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Shaders/Pipeline/VkPipelineCompiler.h"

namespace ShaderEditor
{
namespace PipelineNodes
{
	
/*
=================================================
	constructor
=================================================
*/
	VkPipelineCompiler::VkPipelineCompiler ()
	{
	}
	
/*
=================================================
	Compile
=================================================
*/
	bool VkPipelineCompiler::Compile (const Array<NodePtr> &nodes, EShader::type shaderType, OUT ModulePtr &program)
	{
		return true;
	}


}	// PipelineNodes
}	// ShaderEditor
