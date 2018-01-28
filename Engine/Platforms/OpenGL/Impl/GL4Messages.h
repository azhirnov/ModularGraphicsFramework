// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/OpenGL/Impl/GL4Enums.h"
#include "Engine/Platforms/Shared/GPU/CommandBuffer.h"
#include "Engine/Platforms/Shared/GPU/Image.h"

#if defined( GRAPHICS_API_OPENGL )

namespace Engine
{
namespace GpuMsg
{

	//
	// Get Device Info
	//
	struct GetGLDeviceInfo
	{
	};


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
		Out< gl::GLuint >		result;
	};


	//
	// Blit Framebuffers
	//
	struct CmdBlitGLFramebuffers
	{
	// types
		using EImageAspect	= Platforms::EImageAspect;

		struct Region : CompileTime::FastCopyable
		{
			uint2		srcOffset0;		// start offset
			uint2		srcOffset1;		// end offset

			uint2		dstOffset0;		// start offset
			uint2		dstOffset1;		// end offset
		};
		using Regions_t	= FixedSizeArray< Region, 16 >;


	// variables
		ModulePtr			srcFramebuffer;
		ModulePtr			dstFramebuffer;
		EImageAspect::bits	imageAspect;
		bool				linearFilter	= false;
		Regions_t			regions;


	// methods
		CmdBlitGLFramebuffers (GX_DEFCTOR) {}

		CmdBlitGLFramebuffers (const ModulePtr		&srcFramebuffer,
							   const ModulePtr		&dstFramebuffer,
							   EImageAspect::bits	imageAspect,
							   bool					linearFilter,
							   ArrayCRef<Region>	regions) :
			srcFramebuffer{srcFramebuffer}, dstFramebuffer{dstFramebuffer},
			imageAspect{imageAspect}, linearFilter{linearFilter}, regions{regions}
		{}
	};


	//
	// Get GPU Render Pass ID
	//
	struct GetGLRenderPassID
	{
	// types
		enum class EClearValue {
			Float,
			Int,
			UInt,
			FloatInt	// depth stencil
		};

		using Buffers_t			= FixedSizeArray< gl::GLenum, GlobalConst::Graphics_MaxColorBuffers+2 >;
		using ClearBuffers_t	= FixedSizeArray<Pair< gl::GLenum, EClearValue >, GlobalConst::Graphics_MaxColorBuffers+2 >;

		struct DrawBuffers {
			Buffers_t		colorBuffers;				// color attachment indices
			bool			depthBuffer		= false;	// is depth buffer enabled ?
			bool			stencilBuffer	= false;	// is stencil buffer enabled ?
		};

		struct RenderPassID {
			DrawBuffers		draw;				// draw buffers that will be used
			ClearBuffers_t	clear;				// buffers that will be cleared before rendering
			Buffers_t		invalidateBefore;	// buffers that will be invalidated before rendering
			Buffers_t		invalidateAfter;	// buffers that will be invalidated after rendering

			RenderPassID (GX_DEFCTOR) {}
		};

	// variables
		Out< RenderPassID >		result;
	};
	

	//
	// Get GPU Sampler ID
	//
	struct GetGLSamplerID
	{
		Out< gl::GLuint >		result;
	};


	//
	// Get GPU Image ID
	//
	struct GetGLImageID
	{
		Out< gl::GLuint >		result;
		Out< gl::GLuint >		defaultView;
	};


	//
	// Create GPU Image View
	//
	struct CreateGLImageView
	{
		Platforms::ImageViewDescriptor		viewDescr;
		Out< gl::GLuint >					result;
	};


	//
	// Get GPU Shader Module IDs
	//
	struct GetGLShaderModuleIDs
	{
		struct ShaderModule : CompileTime::PODStruct
		{
			StaticString<64>			entry;
			gl::GLuint					id		= 0;
			Platforms::EShader::type	type	= Platforms::EShader::Unknown;
		};
		using Shaders_t		= FixedSizeArray< ShaderModule, Platforms::EShader::_Count >;

		Out< Shaders_t >	result;
	};


	//
	// Get Graphics Pipeline ID
	//
	struct GetGLGraphicsPipelineID
	{
		using Programs_t	= StaticArray< gl::GLuint, Platforms::EShader::_Count >;
		struct PipelineIDs {
			Programs_t	programs;
			gl::GLuint	pipeline		= 0;	// only graphics programs
			gl::GLuint	vertexAttribs	= 0;	// only format without buffer bindings
		};

		Out< PipelineIDs >		result;
	};


	//
	// Get Compute Pipeline ID
	//
	struct GetGLComputePipelineID
	{
		struct PipelineIDs {
			gl::GLuint	pipeline	= 0;
			gl::GLuint	program		= 0;	// only compute program
		};

		Out< PipelineIDs >		result;
	};
	

	//
	// GPU Pipeline Resource Table ID (uniforms)
	//
	struct GLPipelineResourceTableApply
	{
	// types
		using Programs_t	= StaticArray< gl::GLuint, Platforms::EShader::_Count >;

	// variables
		Programs_t	programs;

	// methods
		GLPipelineResourceTableApply () {}
		explicit GLPipelineResourceTableApply (const Programs_t &progs) : programs(progs) {}
		explicit GLPipelineResourceTableApply (gl::GLuint compProg): programs{} { programs[Platforms::EShader::Compute] = compProg; }
	};


	//
	// Fence Sync
	//
	struct GLFenceSync
	{
		Platforms::GpuFenceId	fenceId;
		Out< gl::GLsync >		result;
	};

	/*struct GetGLFence
	{
		Platforms::GpuFenceId	fenceId;
		Out< gl::GLsync >		result;
	};*/


	//
	// Event
	//
	struct GetGLEvent
	{
		Platforms::GpuEventId	eventId;
		Out< gl::GLsync >		result;
	};


	//
	// Semaphore
	//
	struct GLSemaphoreEnqueue
	{
		Platforms::GpuSemaphoreId	semId;
		Out< gl::GLsync >			result;
	};

	struct GetGLSemaphore
	{
		Platforms::GpuSemaphoreId	semId;
		Out< gl::GLsync >			result;
	};

	struct WaitGLSemaphore
	{
		Platforms::GpuSemaphoreId	semId;
	};


	//
	// Update Buffer Command
	//
	struct GLCmdUpdateBuffer
	{
	// variables
		ModulePtr		dstBuffer;
		BytesUL			dstOffset;
		BytesUL			size;
		BytesUL			srcOffset;

	// methods
		GLCmdUpdateBuffer (const CmdUpdateBuffer &cmd, BytesU offset) :
			dstBuffer(cmd.dstBuffer), dstOffset(cmd.dstOffset), size(cmd.data.Size()), srcOffset(offset)
		{}
	};

	
	//
	// OpenGL Commands
	//
	struct SetGLCommandBufferQueue
	{
	// types
		using Data_t	= Union< CmdSetViewport,
								CmdSetScissor,
								CmdSetDepthBounds,
								CmdSetBlendColor,
								CmdSetDepthBias,
								CmdSetLineWidth,
								CmdSetStencilCompareMask,
								CmdSetStencilWriteMask,
								CmdSetStencilReference,
								CmdBeginRenderPass,
								CmdEndRenderPass,
								CmdBindGraphicsPipeline,
								CmdBindComputePipeline,
								CmdBindVertexBuffers,
								CmdBindIndexBuffer,
								CmdDraw,
								CmdDrawIndexed,
								CmdDrawIndirect,
								CmdDrawIndexedIndirect,
								CmdDispatch,
								CmdDispatchIndirect,
								CmdExecute,
								CmdBindGraphicsResourceTable,
								CmdBindComputeResourceTable,
								CmdCopyBuffer,
								CmdCopyImage,
								CmdCopyBufferToImage,
								CmdCopyImageToBuffer,
								CmdBlitImage,
								CmdBlitGLFramebuffers,
								GLCmdUpdateBuffer,
								CmdFillBuffer,
								CmdClearAttachments,
								CmdClearColorImage,
								CmdClearDepthStencilImage,
								CmdPipelineBarrier >;

		struct Command
		{
		// variables
			Data_t		data;

			DEBUG_ONLY(
				String	file;
				uint	line	= 0;
			)

		// methods
			Command () {}
			Command (Command &&) = default;
			Command (const Command &) = default;

			template <typename Data>
			Command (Data &&data, StringCRef file = StringCRef(), uint line = 0) :
				data(RVREF(data)) DEBUG_ONLY(, file(file), line(line) )
			{}
			
			template <typename Data>
			Command (const Data &data, StringCRef file = StringCRef(), uint line = 0) :
				data(data) DEBUG_ONLY(, file(file), line(line) )
			{}
		};

	// variables
		ReadOnce< Array<Command> >	commands;
		BinaryArray					bufferData;
	};


	//
	// Execute GL Command Buffer
	//
	struct ExecuteGLCommandBuffer
	{};


}	// GpuMsg
}	// Engine

#endif	// GRAPHICS_API_OPENGL
