// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_OPENGL

#include "Engine/Platforms/OpenGL/450/GL4Enums.h"
#include "Engine/Platforms/Public/GPU/CommandBuffer.h"
#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/PipelineLayout.h"

namespace Engine
{
namespace GpuMsg
{

	//
	// Get Device Info
	//
	struct GetGLDeviceInfo : _MsgBase_
	{
	};


	//
	// Sync Client With Device
	//
	struct SyncGLClientWithDevice : _MsgBase_
	{
	// variables
		Platforms::GpuFenceId	fenceId;	// if null then wait untill all commands complete

	// methods
		SyncGLClientWithDevice () {}
		explicit SyncGLClientWithDevice (Platforms::GpuFenceId id) : fenceId{id} {}
	};


	//
	// Get GPU Buffer ID
	//
	struct GetGLBufferID : _MsgBase_
	{
		Out< gl::GLuint >		result;
	};
	

	//
	// Get GPU Framebuffer ID
	//
	struct GetGLFramebufferID : _MsgBase_
	{
		Out< gl::GLuint >		result;
	};


	//
	// Blit Framebuffers
	//
	struct CmdBlitGLFramebuffers : _MsgBase_
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
		CmdBlitGLFramebuffers () {}

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
	struct GetGLRenderPassID : _MsgBase_
	{
	// types
		enum class EClearValue {
			Float,
			Int,
			UInt,
			FloatInt	// depth stencil
		};

		using Buffers_t			= FixedSizeArray< gl::GLenum, GlobalConst::GAPI_MaxColorBuffers+2 >;
		using ClearBuffers_t	= FixedSizeArray<Pair< gl::GLenum, EClearValue >, GlobalConst::GAPI_MaxColorBuffers+2 >;

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

			RenderPassID () {}
		};

	// variables
		Out< RenderPassID >		result;
	};
	

	//
	// Get GPU Sampler ID
	//
	struct GetGLSamplerID : _MsgBase_
	{
		Out< gl::GLuint >		result;
	};


	//
	// Get GPU Image ID
	//
	struct GetGLImageID : _MsgBase_
	{
		Out< gl::GLuint >		result;
	};


	//
	// Create GPU Image View
	//
	struct CreateGLImageView : _MsgBase_
	{
	// variables
		Platforms::ImageViewDescription		viewDescr;
		Out< gl::GLuint >					result;

	// methods
		explicit CreateGLImageView (const Platforms::ImageDescription &descr) : viewDescr{descr} {}
		explicit CreateGLImageView (const Platforms::ImageViewDescription &descr) : viewDescr{descr} {}
	};


	//
	// Get GPU Shader Module IDs
	//
	struct GetGLShaderModuleIDs : _MsgBase_
	{
	// types
		struct ShaderModule : CompileTime::PODStruct
		{
			StaticString<64>			entry;
			gl::GLuint					id		= 0;
			Platforms::EShader::type	type	= Platforms::EShader::Unknown;
		};
		using Shaders_t		= FixedSizeArray< ShaderModule, Platforms::EShader::_Count >;

	// variables
		Out< Shaders_t >	result;
	};


	//
	// Get Graphics Pipeline ID
	//
	struct GetGLGraphicsPipelineID : _MsgBase_
	{
	// types
		using Programs_t	= StaticArray< gl::GLuint, Platforms::EShader::_Count >;
		struct PipelineIDs {
			Programs_t	programs;
			gl::GLuint	pipeline		= 0;	// only graphics programs
			gl::GLuint	vertexAttribs	= 0;	// only format without buffer bindings
		};
		
	// variables
		Out< PipelineIDs >		result;
	};


	//
	// Get Compute Pipeline ID
	//
	struct GetGLComputePipelineID : _MsgBase_
	{
	// types
		struct PipelineIDs {
			gl::GLuint	pipeline	= 0;
			gl::GLuint	program		= 0;	// only compute program
		};
		
	// variables
		Out< PipelineIDs >		result;
	};


	//
	// Get Pipeline Layout Push Constants Mapping
	//
	struct GetGLPipelineLayoutPushConstants : _MsgBase_
	{
	// types
		using EShader	= Platforms::EShader;

		struct PushConstant : CompileTime::FastCopyable
		{
			EShader::bits	stages;
			ushort			offset	= 0;
			ushort			size	= 0;

			PushConstant (EShader::bits stages, BytesU off, BytesU sz) : stages{stages}, offset{ushort(off)}, size{ushort(sz)} {}
		};

		using Name_t			= Platforms::PipelineLayoutDescription::Name_t;
		using PushConstants_t	= FixedSizeMultiHashMap< Name_t, PushConstant, 8 >;

	// variables
		Out< PushConstants_t >		result;
	};
	

	//
	// GPU Pipeline Resource Table ID (uniforms)
	//
	struct GLPipelineResourceTableApply : _MsgBase_
	{
	// types
		using Programs_t	= StaticArray< gl::GLuint, Platforms::EShader::_Count >;

		struct GLPushConstants
		{
			gl::GLuint	bufferID	= 0;
			usize		offset		= 0;
			usize		size		= 0;

			GLPushConstants () {}
		};

	// variables
		Programs_t			programs;
		GLPushConstants		pushConstants;

	// methods
		GLPipelineResourceTableApply () {}

		explicit GLPipelineResourceTableApply (const Programs_t &progs, GLPushConstants pc = Uninitialized) : programs{progs}, pushConstants{pc}
		{}

		explicit GLPipelineResourceTableApply (gl::GLuint compProg, GLPushConstants pc = Uninitialized) : programs{}, pushConstants{pc}
		{ programs[Platforms::EShader::Compute] = compProg; }
	};


	//
	// Fence Sync
	//
	struct GLFenceSync : _MsgBase_
	{
	// variables
		Platforms::GpuFenceId		fenceId;
		Out_opt< gl::GLsync >		result;
		
	// methods
		explicit GLFenceSync (Platforms::GpuFenceId id) : fenceId{id} {}
	};


	//
	// Event
	//
	struct GetGLEvent : _MsgBase_
	{
	// variables
		Platforms::GpuEventId	eventId;
		Out< gl::GLsync >		result;
		
	// methods
		explicit GetGLEvent (Platforms::GpuEventId id) : eventId{id} {}
	};


	//
	// Semaphore
	//
	struct GLSemaphoreEnqueue : _MsgBase_
	{
	// variables
		Platforms::GpuSemaphoreId	semId;
		Out_opt< gl::GLsync >		result;
		
	// methods
		explicit GLSemaphoreEnqueue (Platforms::GpuSemaphoreId id) : semId{id} {}
	};


	struct GetGLSemaphore : _MsgBase_
	{
	// variables
		Platforms::GpuSemaphoreId	semId;
		Out< gl::GLsync >			result;
		
	// methods
		explicit GetGLSemaphore (Platforms::GpuSemaphoreId id) : semId{id} {}
	};


	struct WaitGLSemaphore : _MsgBase_
	{
	// variables
		Platforms::GpuSemaphoreId	semId;
		
	// methods
		explicit WaitGLSemaphore (Platforms::GpuSemaphoreId id) : semId{id} {}
	};


	//
	// Flush Queue
	//
	struct GLFlushQueue : _MsgBase_
	{
	};


	//
	// Update Buffer Command
	//
	struct GLCmdUpdateBuffer : _MsgBase_
	{
	// variables
		ModulePtr		dstBuffer;
		BytesU			dstOffset;
		BytesU			size;
		BytesU			srcOffset;

	// methods
		GLCmdUpdateBuffer (const CmdUpdateBuffer &cmd, BytesU offset) :
			dstBuffer(cmd.dstBuffer), dstOffset(cmd.dstOffset), size(cmd.data.Size()), srcOffset(offset)
		{}
	};


	//
	// Push Constants Command
	//
	struct GLCmdPushConstants : _MsgBase_
	{
	// types
		using EShader = Platforms::EShader;

	// variables
		ModulePtr		pipelineLayout;
		EShader::bits	stages;
		BytesU			dstOffset;
		BytesU			size;
		BytesU			srcOffset;

	// methods
		GLCmdPushConstants (const CmdPushConstants &cmd, BytesU offset) :
			pipelineLayout(cmd.pipelineLayout), stages(cmd.stages),
			dstOffset(cmd.offset), size(cmd.data.Size()), srcOffset(offset)
		{}
	};


	//
	// Get Occlusion/Timestamp Query ID
	//
	struct GetGLQueryID : _MsgBase_
	{
	// types
		struct Data {
			gl::GLuint		id;
			gl::GLenum		target;
		};

	// variables
		gl::GLenum			target	= 0;
		uint				index	= 0;
		Out< Data >			result;

	// methods
		GetGLQueryID (gl::GLenum target, uint index) : target{target}, index{index} {}
	};


	//
	// Get Pipeline Statistic Queries
	//
	struct GetGLPipelineStatisticQueries : _MsgBase_
	{
	// types
		using Data		= GetGLQueryID::Data;
		using Queries_t	= FixedSizeArray< Data, Platforms::EPipelineStatistic::_Count >;

	// variables
		uint				index	= 0;
		Out< Queries_t >	result;

	// methods
		explicit GetGLPipelineStatisticQueries (uint index) : index{index} {}
	};

	
	//
	// OpenGL Commands
	//
	struct SetGLCommandBufferQueue : _MsgBase_
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
								CmdResolveImage,
								CmdBlitGLFramebuffers,
								GLCmdUpdateBuffer,
								CmdFillBuffer,
								CmdClearAttachments,
								CmdClearColorImage,
								CmdClearDepthStencilImage,
								CmdPipelineBarrier,
								GLCmdPushConstants,
								CmdDebugMarker,
								CmdPushDebugGroup,
								CmdPopDebugGroup,
								CmdBeginQuery,
								CmdEndQuery,
								CmdCopyQueryPoolResults,
								CmdWriteTimestamp,
								CmdResetQueryPool >;

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
		BinaryArray					pushConstData;
		
	// methods
		SetGLCommandBufferQueue (Array<Command> &&commands, BinaryArray &&bufferData, BinaryArray &&pushConstData) :
			commands{ RVREF(commands) }, bufferData{ RVREF(bufferData) }, pushConstData{ RVREF(pushConstData) } {}
	};


	//
	// Execute GL Command Buffer
	//
	struct ExecuteGLCommandBuffer : _MsgBase_
	{};


}	// GpuMsg
}	// Engine

#endif	// GRAPHICS_API_OPENGL
