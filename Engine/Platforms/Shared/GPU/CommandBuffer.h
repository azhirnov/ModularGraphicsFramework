// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/RenderPassEnums.h"
#include "Engine/Platforms/Shared/GPU/RenderStateEnums.h"
#include "Engine/Platforms/Shared/GPU/VertexEnums.h"
#include "Engine/Platforms/Shared/GPU/TextureEnums.h"
#include "Engine/Platforms/Shared/GPU/CommandEnums.h"
#include "Engine/Platforms/Shared/GPU/MipmapLevel.h"
#include "Engine/Platforms/Shared/GPU/TexArrLayer.h"

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

	struct GpuCommandBuilder
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
	struct GetVkCommandBufferID;


	//
	// Set Viewport
	//
	struct GpuCmdSetViewport
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
		GpuCmdSetViewport (const GXMath::uint2 &offset,
							  const GXMath::uint2 &size,
							  const GXMath::float2 &depthRange,
							  uint firstViewport = 0)
		{
			viewports.PushBack( Viewport( offset, size, depthRange ) );
			this->firstViewport = firstViewport;
		}

		GpuCmdSetViewport (InitializerList<Viewport> list, uint firstViewport = 0) :
			viewports(list), firstViewport(firstViewport)
		{}
	};


	//
	// Set Scissor
	//
	struct GpuCmdSetScissor
	{
	// types
		using Scissors_t	= FixedSizeArray< GXMath::RectU, GlobalConst::Graphics_MaxColorBuffers >;

	// variables
		Scissors_t		scissors;
		uint			firstScissor	= 0;

	// methods
		GpuCmdSetScissor (const GXMath::RectU &scissorRect, uint firstScissor = 0)
		{
			scissors.PushBack( scissorRect );
			this->firstScissor = firstScissor;
		}

		GpuCmdSetScissor (InitializerList<GXMath::RectU> list, uint firstScissor = 0) :
			scissors(list), firstScissor(firstScissor)
		{}
	};


	//
	// Set Depth Bounds
	//
	struct GpuCmdSetDepthBounds
	{
		float	min;
		float	max;
	};


	//
	// Set Blend Color
	//
	struct GpuCmdSetBlendColor
	{
		GXMath::color4f		color;
	};


	//
	// Set Depth Bias
	//
	struct GpuCmdSetDepthBias
	{
		float	biasConstFactor;
		float	biasClamp;
		float	biasSlopeFactor;
	};


	//
	// Set Line Width
	//
	struct GpuCmdSetLineWidth
	{
		float	width;
	};

	
	//
	// Set Stencil Compare Mask
	//
	struct GpuCmdSetStencilCompareMask
	{
		Platforms::EPolygonFace::type	face;
		uint							cmpMask;
	};


	//
	// Set Stencil Write Mask
	//
	struct GpuCmdSetStencilWriteMask
	{
		Platforms::EPolygonFace::type	face;
		uint							mask;
	};


	//
	// Set Stencil Reference
	//
	struct GpuCmdSetStencilReference
	{
		Platforms::EPolygonFace::type	face;
		uint							ref;
	};


	//
	// Begin
	//
	struct GpuCmdBegin
	{
		//ModulePtr		targetCmdBuffer;
		Platforms::CommandBufferDescriptor	descr;
	};


	//
	// End
	//
	struct GpuCmdEnd
	{
		Out< ModulePtr >	cmdBuffer;
	};


	//
	// Begin Render Pass
	//
	struct GpuCmdBeginRenderPass
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
	struct GpuCmdBeginRenderPassID;


	//
	// End Render Pass
	//
	struct GpuCmdEndRenderPass
	{
	};


	//
	// Next Subpass
	//
	struct GpuCmdNextSubpass
	{
	};


	//
	// Bind Pipeline
	//
	struct GpuCmdBindPipeline
	{
		ModulePtr	pipeline;
	};
	
	// platform-dependent
	struct GpuCmdBindGraphicsPipelineID;
	struct GpuCmdBindComputePipelineID;


	//
	// Bind Vertex Buffers
	//
	struct GpuCmdBindVertexBuffers
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
	struct GpuCmdBindVertexBufferIDs;


	//
	// Bind Index Buffer
	//
	struct GpuCmdBindIndexBuffer
	{
		ModulePtr					indexBuffer;
		BytesUL						offset;
		Platforms::EIndex::type		indexType;
	};
	
	// platform-dependent
	struct GpuCmdBindIndexBufferID;


	//
	// Draw
	//
	struct GpuCmdDraw
	{
	// variables
		uint	vertexCount;
		uint	instanceCount;
		uint	firstVertex;
		uint	firstInstance;

	// methods
		explicit
		GpuCmdDraw (uint vertexCount,
						   uint instanceCount = 1,
						   uint firstVertex	  = 0,
						   uint firstInstance = 0) :
			vertexCount(vertexCount),
			instanceCount(instanceCount),
			firstVertex(firstVertex),
			firstInstance(firstInstance)
		{}
	};

	struct GpuCmdDrawIndexed
	{
	// variables
		uint	indexCount;
		uint	instanceCount;
		uint	firstVertex;
		int		vertexOffset;
		uint	firstInstance;

	// methods
		explicit
		GpuCmdDrawIndexed (uint indexCount,
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

	struct GpuCmdDrawIndirect
	{
		ModulePtr	indirectBuffer;
		BytesU		offset;
		uint		drawCount;
		BytesU		stride;
	};

	struct GpuCmdDrawIndexedIndirect
	{
		ModulePtr	indirectBuffer;
		BytesU		offset;
		uint		drawCount;
		BytesU		stride;
	};
	
	// platform-dependent
	struct GpuCmdDrawIndirectID;
	struct GpuCmdDrawIndexedIndirectID;


	//
	// Execute
	//
	struct GpuCmdExecute
	{
	// types
		using CmdBuffers_t	= FixedSizeArray< ModulePtr, 16 >;

	// variables
		CmdBuffers_t	cmdBuffers;
	};
	
	// platform-dependent
	struct GpuCmdExecuteID;


	//
	// Bind Descriptor Set
	//
	struct GpuCmdBindDescriptorSet
	{
		// TODO
	};


	//
	// Copy Buffer to Buffer
	//
	struct GpuCmdCopyBuffer
	{
	// types
		struct Region : CompileTime::PODStruct
		{
		// variables
			BytesUL		srcOffset;
			BytesUL		dstOffset;
			BytesUL		size;

		// methods
			Region () {}
			Region (BytesUL srcOffset, BytesUL dstOffset, BytesUL size) {}
		};

		using Regions_t	= FixedSizeArray< Region, 8 >;


	// variables
		ModulePtr	srcBuffer;
		ModulePtr	dstBuffer;
		Regions_t	regions;


	// methods
		GpuCmdCopyBuffer (const ModulePtr &srcBuffer,
						  const ModulePtr &dstBuffer,
						  BytesUL srcOffset,
						  BytesUL dstOffset,
						  BytesUL size) :
			srcBuffer(srcBuffer),
			dstBuffer(dstBuffer)
		{
			regions.PushBack( Region{ srcOffset, dstOffset, size } );
		}

		GpuCmdCopyBuffer (const ModulePtr &srcBuffer,
						  const ModulePtr &dstBuffer,
						  InitializerList<Region> regions) :
			srcBuffer(srcBuffer),
			dstBuffer(dstBuffer),
			regions(regions)
		{}
	};


	//
	// Copy Image to Image
	//
	struct GpuCmdCopyImage
	{
	// types
		using EImageLayout	= Platforms::EImageLayout;
		using EImageAspect	= Platforms::EImageAspect;
		using MipmapLevel	= Platforms::MipmapLevel;
		using TexArrLayer	= Platforms::TexArrLayer;
		using uint3			= GXMath::uint3;

		struct ImageLayers : CompileTime::PODStruct
		{
			EImageAspect::bits	aspectMask;
			MipmapLevel			mipLevel;
			TexArrLayer			baseLayer;
			uint				layerCount;
		};

		struct Region : CompileTime::PODStruct
		{
			ImageLayers		srcLayers;
			uint3			srcOffset;
			ImageLayers		dstLayers;
			uint3			dstOffset;
			uint3			size;
		};
		using Regions_t		= FixedSizeArray< Region, 8 >;


	// variables
		ModulePtr			srcImage;
		EImageLayout::type	srcLayout;
		ModulePtr			dstImage;
		EImageLayout::type	dstLayout;
		Regions_t			regions;


	// methods
		GpuCmdCopyImage (ModulePtr			srcImage,
						 EImageLayout::type	srcLayout,
						 ModulePtr			dstImage,
						 EImageLayout::type	dstLayout,
						 InitializerList<Region> regions) :
			srcImage(srcImage), srcLayout(srcLayout),
			dstImage(dstImage), dstLayout(dstLayout),
			regions(regions)
		{}
	};


	//
	// Copy Buffer to Image
	//
	struct GpuCmdCopyBufferToImage
	{
	// types
		using EImageLayout	= Platforms::EImageLayout;
		using EImageAspect	= Platforms::EImageAspect;
		using ImageLayers	= GpuCmdCopyImage::ImageLayers;
		using uint3			= GXMath::uint3;

		struct Region : CompileTime::PODStruct
		{
			// src
			BytesUL		bufferOffset;
			uint		bufferRowLength;
			uint		bufferImageHeight;
			// dst
			ImageLayers	imageLayers;
			uint3		imageOffset;
			uint3		imageSize;
		};
		using Regions_t	= FixedSizeArray< Region, 8 >;


	// variables
		ModulePtr			srcBuffer;
		ModulePtr			dstImage;
		EImageLayout::type	dstLayout;
		Regions_t			regions;


	// methods
		GpuCmdCopyBufferToImage (const ModulePtr		&srcBuffer,
								 const ModulePtr		&dstImage,
								 EImageLayout::type		dstLayout,
								 InitializerList<Region>	regions) :
			srcBuffer(srcBuffer),
			dstImage(dstImage), dstLayout(dstLayout),
			regions(regions)
		{}
	};


	//
	// Copy Image to Buffer
	//
	struct GpuCmdCopyImageToBuffer
	{
	// types
		using EImageLayout	= Platforms::EImageLayout;
		using EImageAspect	= Platforms::EImageAspect;
		using ImageLayers	= GpuCmdCopyImage::ImageLayers;
		using uint3			= GXMath::uint3;
		using Region		= GpuCmdCopyBufferToImage::Region;
		using Regions_t		= GpuCmdCopyBufferToImage::Regions_t;


	// variables
		ModulePtr			srcImage;
		EImageLayout::type	srcLayout;
		ModulePtr			dstBuffer;
		Regions_t			regions;


	// methods
		GpuCmdCopyImageToBuffer (const ModulePtr		&srcImage,
								 EImageLayout::type		srcLayout,
								 const ModulePtr		&dstBuffer,
								 InitializerList<Region>	regions) :
			srcImage(srcImage), srcLayout(srcLayout),
			dstBuffer(dstBuffer),
			regions(regions)
		{}
	};


	//
	// Blit Image
	//
	struct GpuCmdBlitImage
	{
	// types
		using EImageLayout	= Platforms::EImageLayout;
		using EImageAspect	= Platforms::EImageAspect;
		using ImageLayers	= GpuCmdCopyImage::ImageLayers;
		using uint3			= GXMath::uint3;

		struct Region : CompileTime::PODStruct
		{
			ImageLayers		srcLayers;
			uint3			srcOffset0;
			uint3			srcOffset1;
			ImageLayers		dstLayers;
			uint3			dstOffset0;
			uint3			dstOffset1;
		};
		using Regions_t		= FixedSizeArray< Region, 8 >;


	// variables
		ModulePtr			srcImage;
		EImageLayout::type	srcLayout;
		ModulePtr			dstImage;
		EImageLayout::type	dstLayout;
		bool				linearFilter;
		Regions_t			regions;


	// methods
		GpuCmdBlitImage (const ModulePtr		&srcImage,
						 EImageLayout::type		srcLayout,
						 const ModulePtr		&dstImage,
						 EImageLayout::type		dstLayout,
						 bool					linearFilter,
						 InitializerList<Region>	regions) :
			srcImage(srcImage), srcLayout(srcLayout),
			dstImage(dstImage), dstLayout(dstLayout),
			linearFilter(linearFilter), regions(regions)
		{}
	};


	//
	// Update Buffer
	//
	struct GpuCmdUpdateBuffer
	{
		ModulePtr		dstBuffer;
		BytesUL			dstOffset;
		BinaryArray		data;
	};


	//
	// Fill Buffer
	//
	struct GpuCmdFillBuffer
	{
		ModulePtr		dstBuffer;
		BytesUL			dstOffset;
		BytesUL			size;
		uint			pattern;
	};


	//
	// Clear Color Attachments
	//
	struct GpuCmdClearAttachments
	{
	// types
		using EImageAspect	= Platforms::EImageAspect;
		using ClearValue_t	= GpuCmdBeginRenderPass::ClearValue_t;
		using TexArrLayer	= Platforms::TexArrLayer;
		using RectU			= GXMath::RectU;

		struct Attachment : CompileTime::PODStruct
		{
			EImageAspect::bits	aspectMask;
			uint				attachmentIdx;
			ClearValue_t		clearValue;
		};

		struct ClearRect : CompileTime::PODStruct
		{
			RectU			rect;
			TexArrLayer		baseLayer;
			uint			layerCount;
		};

		using Attachments_t		= FixedSizeArray< Attachment, GlobalConst::Graphics_MaxColorBuffers >;
		using ClearRects_t		= FixedSizeArray< ClearRect, 16 >;


	// variables
		Attachments_t	attachments;
		ClearRects_t	clearRects;
	};


	//
	// Clear Color Image
	//
	struct GpuCmdClearColorImage
	{
	// types
		using EImageAspect	= Platforms::EImageAspect;
		using EImageLayout	= Platforms::EImageLayout;
		using MipmapLevel	= Platforms::MipmapLevel;
		using TexArrLayer	= Platforms::TexArrLayer;
		using ClearColor_t	= Union< GXMath::float4, GXMath::uint4, GXMath::int4 >;

		struct ImageRange : CompileTime::PODStruct
		{
			EImageAspect::bits	aspectMask;
			MipmapLevel			baseMipLevel;
			uint				levelCount;
			TexArrLayer			baseLayer;
			uint				layerCount;
		};
		using Ranges_t		= FixedSizeArray< ImageRange, 16 >;

	// variables
		ModulePtr			image;
		EImageLayout::type	layout;
		Ranges_t			ranges;
		ClearColor_t		clearValue;
	};


	//
	// Clear Depth Stencil Image
	//
	struct GpuCmdClearDepthStencilImage
	{
	// types
		using EImageLayout	= Platforms::EImageLayout;
		using ImageRange	= GpuCmdClearColorImage::ImageRange;
		using DepthStencil	= GpuCmdBeginRenderPass::DepthStencil;
		using Ranges_t		= GpuCmdClearColorImage::Ranges_t;

	// variables
		ModulePtr			image;
		EImageLayout::type	layout;
		Ranges_t			ranges;
		DepthStencil		clearValue;
	};


}	// ModuleMsg
}	// Engine
