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
	Run
=================================================
*/
	void ComputePipeline::Run (const GX_STL::GXMath::uint3 &globalSize, const GX_STL::GXMath::uint3 &localSize)
	{
		TODO( "" );
	}


}	// PipelineNodes
}	// ShaderEditor
