// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/OpenGL/Impl/GL4Enums.h"

#if defined( GRAPHICS_API_OPENGL )

namespace Engine
{
namespace ModuleMsg
{

	//
	// Get GPU Buffer ID
	//
	struct GetGLBufferID
	{
		Out< gl::GLuint >		result;
	};
	

	//
	// Get GPU Framebuffer ID
	//
	struct GetGLFramebufferID
	{
		Out< gl::GLuint >	result;
	};
	

	//
	// Get GPU Sampler ID
	//
	struct GetGLSamplerID
	{
		Out< gl::GLuint >	result;
	};


	//
	// Get GPU Texture ID
	//
	struct GetGLTextureID
	{
		Out< gl::GLuint >		result;
	};


	//
	// Get GPU Shader Module IDs
	//
	struct GetGLShaderModuleIDs
	{
		struct ShaderModule
		{
			StaticString<64>			entry;
			gl::GLuint					id;
			Platforms::EShader::type	type;
		};

		Out< Array< ShaderModule > >	result;
	};

	
	//
	// Get GPU Command Buffer ID
	//
	struct GetGLCommandBufferID
	{
		//Out< vk::VkCommandBuffer >	result;
	};

	/*
	struct GpuCmdBeginRenderPassID
	{
		// TODO
	};

	struct GpuCmdBindGraphicsPipelineID
	{
		// TODO
	};

	struct GpuCmdBindComputePipelineID
	{
		// TODO
	};

	struct GpuCmdBindVertexBufferIDs
	{
		// TODO
	};
	
	struct GpuCmdBindIndexBufferID
	{
		// TODO
	};

	struct GpuCmdDrawIndirectID
	{
		// TODO
	};

	struct GpuCmdDrawIndexedIndirectID
	{
		// TODO
	};

	struct GpuCmdExecuteID
	{
		// TODO
	};
	*/

}	// ModuleMsg
}	// Engine

#endif	// GRAPHICS_API_OPENGL
