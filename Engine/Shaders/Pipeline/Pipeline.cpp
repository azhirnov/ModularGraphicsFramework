// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Shaders/Pipeline/Pipeline.h"

#include "Engine/Shaders/Pipeline/VkPipelineCompiler.h"
#include "Engine/Shaders/Pipeline/GL4PipelineCompiler.h"

namespace ShaderEditor
{
namespace _ShaderNodesHidden_
{
		
/*
=================================================
	_RegisterCompilers
=================================================
*/
	void NodeFunctions::_RegisterCompilers ()
	{
		using namespace PipelineNodes;

		RegisterCompiler( "VK1", DelegateBuilder( New<VkPipelineCompiler>(), &VkPipelineCompiler::Compile ) );
		RegisterCompiler( "GL4", DelegateBuilder( New<GL4PipelineCompiler>(), &GL4PipelineCompiler::Compile ) );
	}

}	// _ShaderNodesHidden_


namespace PipelineNodes
{
		
/*
=================================================
	constructor
=================================================
*/
	Pipeline::Pipeline ()
	{
	}
	
/*
=================================================
	_CompileShaders
=================================================
*/
	bool Pipeline::_CompileShaders ()
	{
		FOR( i, _attachedShaders ) 
		{
			CHECK_ERR( _attachedShaders[i].second->_Compile() );
		}
		return true;
	}
	
/*
=================================================
	AddShader
=================================================
*/
	bool Pipeline::AddShader (ShaderNodes::Shader* shader)
	{
		CHECK_ERR( shader );
		CHECK_ERR( not _attachedShaders.IsExist( shader->_shaderType ) );

		_attachedShaders.Add( shader->_shaderType, shader );
		return true;
	}
	
//=============================================================================


	
/*
=================================================
	constructor
=================================================
*/
	GraphicsPipeline::GraphicsPipeline ()
	{
	}

/*
=================================================
	Run
=================================================
*/
	void GraphicsPipeline::Run (const GX_STL::GXMath::uint3 &areaSize)
	{
		TODO( "" );
	}
	
//=============================================================================


	
/*
=================================================
	constructor
=================================================
*/
	ComputePipeline::ComputePipeline ()
	{
	}

/*
=================================================
	Run
=================================================
*/
	void ComputePipeline::Run (const GX_STL::GXMath::uint3 &globalSize, const GX_STL::GXMath::uint3 &localSize)
	{
		TODO( "" );
	}


}	// PipelineNodes
}	// ShaderEditor
