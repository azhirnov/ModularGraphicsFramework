// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/RenderState.h"
#include "Engine/Platforms/Shared/GPU/VertexAttribs.h"
#include "Engine/Platforms/Shared/GPU/ShaderEnums.h"

namespace Engine
{
namespace Platforms
{

	//
	// Graphics Pipeline Descriptor
	//

	struct GraphicsPipelineDescriptor : CompileTime::PODStruct
	{
	// variables
		VertexAttribs					attribs;
		RenderState						renderState;
		EPipelineDynamicState::bits		dynamicStates;
		uint							patchControlPoints;
		uint							subpass;

	// methods
		GraphicsPipelineDescriptor (GX_DEFCTOR) : patchControlPoints(0), subpass(0)
		{}

		GraphicsPipelineDescriptor (const VertexAttribs &attribs,
									const RenderState &renderState,
									const EPipelineDynamicState::bits &dynamicStates,
									uint patchControlPoints = 0,
									uint subpass = 0) :
			attribs(attribs), renderState(renderState),
			dynamicStates(dynamicStates), patchControlPoints(patchControlPoints),
			subpass(subpass)
		{}
	};

}	// Platforms


namespace CreateInfo
{
	//
	// Graphics Pipeline Create Info
	//

	struct GraphicsPipeline
	{
	// types
		using Descriptor = Platforms::GraphicsPipelineDescriptor;

	// variables
		ModulePtr		gpuThread;
		Descriptor		descr;
		ModulePtr		shaders;
		ModulePtr		renderPass;
		uint			subpass;
	};


	//
	// Compute Pipeline Create Info
	//

	struct ComputePipeline
	{
		ModulePtr		gpuThread;
		ModulePtr		shaders;
	};


	//
	// GPU Shader Modules Create Info
	//
	struct GpuShaderModulesFromFile
	{
		ModulePtr			gpuThread;
		Array< String >		filenames;
	};

	struct GpuShaderModulesFromBuffer
	{
		ModulePtr										gpuThread;
		Map< Platforms::EShader::type, BinaryArray >	shaderData;
	};

}	// CreateInfo


namespace ModuleMsg
{
	//
	// Get Graphics Pipeline Descriptor
	//
	struct GetGraphicsPipelineDescriptor
	{
		Out< Platforms::GraphicsPipelineDescriptor >	result;
	};


	// platform-dependent
	struct GetVkGraphicsPipelineID;
	struct GetGLGraphicsPipelineID;
	struct GetVkComputePipelineID;
	struct GetGLComputePipelineID;


}	// ModuleMsg
}	// Engine
