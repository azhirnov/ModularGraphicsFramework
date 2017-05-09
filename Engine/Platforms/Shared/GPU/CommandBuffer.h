// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"

namespace Engine
{
namespace Platforms
{

	//
	// GPU Command Buffer Descriptor
	//

	struct CommandBufferDescriptor : CompileTime::PODStruct
	{
		bool	isSecondary = false;
	};


}	// Platforms


namespace CreateInfo
{

	//
	// GPU Command Buffer Create Info
	//

	struct GpuCommandBuffer
	{
		ModulePtr							gpuThread;
		ModulePtr							builder;
		Platforms::CommandBufferDescriptor	descr;
	};


	//
	// GPU Command Buffer Builder Create Info
	//

	struct GpuCommandBufferBuilder
	{
		ModulePtr	gpuThread;
	};
	
}	// CreateInfo


namespace ModuleMsg
{

	//
	// Get GPU Command Buffer Descriptor
	//
	struct GetGpuCommandBufferDescriptor
	{
		Out< Platforms::CommandBufferDescriptor >	result;
	};


	// platform-dependent
	struct GetGpuCommandBufferID;


	//
	// Set Viewport
	//
	struct CommandBufferSetViewport
	{
	// types
		struct Viewport : CompileTime::PODStruct
		{
			GXMath::uint2	offset;
			GXMath::uint2	size;
			GXMath::float2	depthRange;

			Viewport (const GXMath::uint2 &offset, const GXMath::uint2 &size, const GXMath::float2 &depth) :
				offset(offset), size(size), depthRange(depth)
			{}

			Viewport (const GXMath::RectU &rect, const GXMath::float2 &depth) :
				offset( rect.LeftBottom() ),
				size( rect.Size() ),
				depthRange( depth )
			{}
		};

		using Viewports_t	= FixedSizeArray< Viewport, GlobalConst::Graphics_MaxColorBuffers >;


	// variables
		Viewports_t		viewports;
		uint			firstViewport	= 0;

	// methods
		CommandBufferSetViewport (const GXMath::uint2 &offset,
								  const GXMath::uint2 &size,
								  const GXMath::float2 &depthRange,
								  uint firstViewport = 0)
		{
			viewports.PushBack( Viewport( offset, size, depthRange ) );
			this->firstViewport = firstViewport;
		}

		CommandBufferSetViewport (std::initializer_list<Viewport> list, uint firstViewport = 0) :
			viewports(list), firstViewport(firstViewport)
		{}
	};


	//
	// Set Scissor
	//
	struct CommandBufferSetScissor
	{
	// types
		using Scissors_t	= FixedSizeArray< GXMath::RectU, GlobalConst::Graphics_MaxColorBuffers >;

	// variables
		Scissors_t		scissors;
		uint			firstScissor	= 0;

	// methods
		CommandBufferSetScissor (const GXMath::RectU &scissorRect, uint firstScissor = 0)
		{
			scissors.PushBack( scissorRect );
			this->firstScissor = firstScissor;
		}

		CommandBufferSetScissor (std::initializer_list<GXMath::RectU> list, uint firstScissor = 0) :
			scissors(list), firstScissor(firstScissor)
		{}
	};


	//
	// Set Depth Bounds
	//
	struct CommandBufferSetDepthBounds
	{
		float	min;
		float	max;
	};


	//
	// Set Blend Color
	//
	struct CommandBufferSetBlendColor
	{
		GXMath::color4f		color;
	};


	//
	// Set Depth Bias
	//
	struct CommandBufferSetDepthBias
	{
		float	biasConstFactor;
		float	biasClamp;
		float	biasSlopeFactor;
	};


	//
	// Set Line Width
	//
	struct CommandBufferSetLineWidth
	{
		float	width;
	};

	
	//
	// Set Stencil Compare Mask
	//
	struct CommandBufferSetStencilCompareMask
	{
		Platforms::EPolygonFace::type	face;
		uint							cmpMask;
	};


	//
	// Set Stencil Write Mask
	//
	struct CommandBufferSetStencilWriteMask
	{
		Platforms::EPolygonFace::type	face;
		uint							mask;
	};


	//
	// Set Stencil Reference
	//
	struct CommandBufferSetStencilReference
	{
		Platforms::EPolygonFace::type	face;
		uint							ref;
	};


	//
	// Begin
	//
	struct CommandBufferBegin
	{
		//ModulePtr		targetCmdBuffer;
		Platforms::CommandBufferDescriptor	descr;
	};


	//
	// End
	//
	struct CommandBufferEnd
	{
		Out< ModulePtr >	cmdBuffer;
	};


	//
	// Begin Render Pass
	//
	struct CommandBufferBeginRenderPass
	{
	// types
		struct DepthStencil : CompileTime::PODStruct
		{
		// variables
			float	depth;
			uint	stencil;

		// mathods
			DepthStencil () : depth(0.0f), stencil(0) {}
			explicit DepthStencil (float depth, uint stencil = 0) : depth(depth), stencil(stencil) {}
		};

		using ClearValue_t	= Union< GXMath::float4, GXMath::uint4, GXMath::int4, DepthStencil >;
		using ClearValues_t	= FixedSizeArray< ClearValue_t, GlobalConst::Graphics_MaxColorBuffers >;

	// variables
		ModulePtr		renderPass;
		ModulePtr		framebuffer;
		GXMath::RectU	area;
		ClearValues_t	clearValues;
	};
	
	// platform-dependent
	struct CommandBufferBeginRenderPassID;


	//
	// End Render Pass
	//
	struct CommandBufferEndRenderPass
	{
	};


	//
	// Next Subpass
	//
	struct CommandBufferNextSubpass
	{
	};


	//
	// Bind Pipeline
	//
	struct CommandBufferBindPipeline
	{
		ModulePtr	pipeline;
	};
	
	// platform-dependent
	struct CommandBufferBindGraphicsPipelineID;
	struct CommandBufferBindComputePipelineID;


	//
	// Bind Vertex Buffers
	//
	struct CommandBufferBindVertexBuffers
	{
	// types
		using Buffers_t		= FixedSizeArray< ModulePtr, GlobalConst::Graphics_MaxAttribs >;
		using Offsets_t		= FixedSizeArray< BytesUL, GlobalConst::Graphics_MaxAttribs >;

	// variables
		Buffers_t		vertexBuffers;
		Offsets_t		offsets;
		uint			firstBinding	= 0;
	};
	
	// platform-dependent
	struct CommandBufferBindVertexBufferIDs;


	//
	// Bind Index Buffer
	//
	struct CommandBufferBindIndexBuffer
	{
		ModulePtr					indexBuffer;
		BytesUL						offset;
		Platforms::EIndex::type		indexType;
	};
	
	// platform-dependent
	struct CommandBufferBindIndexBufferID;


	//
	// Draw
	//
	struct CommandBufferDraw
	{
	// variables
		uint	vertexCount;
		uint	instanceCount;
		uint	firstVertex;
		uint	firstInstance;

	// methods
		explicit
		CommandBufferDraw (uint vertexCount,
						   uint instanceCount = 1,
						   uint firstVertex	  = 0,
						   uint firstInstance = 0) :
			vertexCount(vertexCount),
			instanceCount(instanceCount),
			firstVertex(firstVertex),
			firstInstance(firstInstance)
		{}
	};

	struct CommandBufferDrawIndexed
	{
	// variables
		uint	indexCount;
		uint	instanceCount;
		uint	firstVertex;
		int		vertexOffset;
		uint	firstInstance;

	// methods
		explicit
		CommandBufferDrawIndexed (uint indexCount,
								  uint instanceCount = 1,
								  uint firstVertex   = 0,
								  int  vertexOffset	 = 0,
								  uint firstInstance = 0) :
			indexCount(indexCount),
			instanceCount(instanceCount),
			firstVertex(firstVertex),
			vertexOffset(vertexOffset),
			firstInstance(firstInstance)
		{}
	};

	struct CommandBufferDrawIndirect
	{
		ModulePtr	indirectBuffer;
		BytesU		offset;
		uint		drawCount;
		BytesU		stride;
	};

	struct CommandBufferDrawIndexedIndirect
	{
		ModulePtr	indirectBuffer;
		BytesU		offset;
		uint		drawCount;
		BytesU		stride;
	};
	
	// platform-dependent
	struct CommandBufferDrawIndirectID;
	struct CommandBufferDrawIndexedIndirectID;


	//
	// Execute
	//
	struct CommandBufferExecute
	{
		FixedSizeArray< ModulePtr, 16 >		cmdBuffers;
	};
	
	// platform-dependent
	struct CommandBufferExecuteID;


}	// ModuleMsg
}	// Engine
