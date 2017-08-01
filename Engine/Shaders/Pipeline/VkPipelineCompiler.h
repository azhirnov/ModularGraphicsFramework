// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Shaders/Pipeline/Pipeline.h"

namespace ShaderEditor
{
namespace PipelineNodes
{

	//
	// Vulkan Pipeline Compiler
	//

	class VkPipelineCompiler final : public GX_STL::GXTypes::StaticRefCountedObject
	{
	// types
	private:
		template <typename T>				using Array	= GX_STL::GXTypes::Array<T>;

		using NodePtr		= _ShaderNodesHidden_::ISrcNodePtr;
		using ModulePtr		= Engine::Base::ModulePtr;
		using EShader		= Engine::Platforms::EShader;


	// variables
	private:


	// methods
	public:
		VkPipelineCompiler ();
		
		bool Compile (const Array<NodePtr> &nodes, EShader::type shaderType, OUT ModulePtr &program);
	};


}	// PipelineNodes
}	// ShaderEditor
