// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/RenderPassEnums.h"
#include "Engine/Platforms/Shared/GPU/RenderStateEnums.h"
#include "Engine/Platforms/Shared/GPU/VertexEnums.h"
#include "Engine/Platforms/Shared/GPU/ImageEnums.h"
#include "Engine/Platforms/Shared/GPU/CommandEnums.h"
#include "Engine/Platforms/Shared/GPU/MipmapLevel.h"
#include "Engine/Platforms/Shared/GPU/ImgArrLayer.h"

namespace Engine
{
namespace Platforms
{

	//
	// GPU Command Buffer Descriptor
	//

	struct CommandBufferDescriptor : CompileTime::PODStruct
	{
		// set 'true' to create secondary command buffer
		bool	isSecondary			= false;

		// if 'false' then you need return command buffer to initial state before begin recording
		// this value depends of command builder module
		bool	implicitResetable	= true;

		CommandBufferDescriptor () {}

		explicit CommandBufferDescriptor (bool isSecondary, bool implicitResetable = true) :
			isSecondary( isSecondary ), implicitResetable( implicitResetable )
		{}
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


namespace GpuMsg
{

	// platform-dependent
	struct GetVkCommandBufferID;
	struct GetVkCommandPoolID;


	//
	// Get Command Buffer Descriptor
	//
	struct GetCommandBufferDescriptor
	{
		Out< Platforms::CommandBufferDescriptor >	result;
	};


	//
	// Set/Get Command Buffer State
	//
	struct SetCommandBufferState
	{
		enum class EState : uint
		{
			Initial,		// initial or reseted state
			Recording,
			Executable,
			Pending,
			Completed,		// it is temporary state, cb will pass to 'Initial', 'Executable' or 'Deleted' state depends of cb settings
			Deleted,
			Unknown		= ~0u
		};

		EState	newState	= EState::Unknown;
	};

	struct GetCommandBufferState
	{
		using EState = SetCommandBufferState::EState;

		Out< EState >	result;
	};


	//
	// Command Buffer State Changed Event
	//
	struct OnCommandBufferStateChanged
	{
		using EState = SetCommandBufferState::EState;

		EState		oldState;
		EState		newState;
	};


	//
	// Set Command Buffer Dependency
	//
	struct SetCommandBufferDependency
	{
		SingleRead< Set<ModulePtr> >	resources;
	};


	//
	// Set Viewport
	//
	struct CmdSetViewport
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
		CmdSetViewport (const GXMath::uint2 &offset,
						const GXMath::uint2 &size,
						const GXMath::float2 &depthRange,
						uint firstViewport = 0)
		{
			viewports.PushBack( Viewport( offset, size, depthRange ) );
			this->firstViewport = firstViewport;
		}

		CmdSetViewport (InitializerList<Viewport> list, uint firstViewport = 0) :
			viewports(list), firstViewport(firstViewport)
		{}
	};


	//
	// Set Scissor
	//
	struct CmdSetScissor
	{
	// types
		using Scissors_t	= FixedSizeArray< GXMath::RectU, GlobalConst::Graphics_MaxColorBuffers >;

	// variables
		Scissors_t		scissors;
		uint			firstScissor	= 0;

	// methods
		CmdSetScissor (const GXMath::RectU &scissorRect, uint firstScissor = 0)
		{
			scissors.PushBack( scissorRect );
			this->firstScissor = firstScissor;
		}

		CmdSetScissor (InitializerList<GXMath::RectU> list, uint firstScissor = 0) :
			scissors(list), firstScissor(firstScissor)
		{}
	};


	//
	// Set Depth Bounds
	//
	struct CmdSetDepthBounds
	{
		float	min;
		float	max;
	};


	//
	// Set Blend Color
	//
	struct CmdSetBlendColor
	{
		GXMath::color4f		color;
	};


	//
	// Set Depth Bias
	//
	struct CmdSetDepthBias
	{
		float	biasConstFactor;
		float	biasClamp;
		float	biasSlopeFactor;
	};


	//
	// Set Line Width
	//
	struct CmdSetLineWidth
	{
		float	width;
	};

	
	//
	// Set Stencil Compare Mask
	//
	struct CmdSetStencilCompareMask
	{
		Platforms::EPolygonFace::type	face;
		uint							cmpMask;
	};


	//
	// Set Stencil Write Mask
	//
	struct CmdSetStencilWriteMask
	{
		Platforms::EPolygonFace::type	face;
		uint							mask;
	};


	//
	// Set Stencil Reference
	//
	struct CmdSetStencilReference
	{
		Platforms::EPolygonFace::type	face;
		uint							ref;
	};


	//
	// Begin
	//
	struct CmdBegin
	{
		bool							isSecondary		= false;
		ModulePtr						targetCmdBuffer	= null;
	};


	//
	// End
	//
	struct CmdEnd
	{
		Out< ModulePtr >	cmdBuffer;
	};


	//
	// Begin Render Pass
	//
	struct CmdBeginRenderPass
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
	struct CmdBeginRenderPassID;


	//
	// End Render Pass
	//
	struct CmdEndRenderPass
	{
	};


	//
	// Next Subpass
	//
	struct CmdNextSubpass
	{
	};


	//
	// Bind Graphics Pipeline
	//
	struct CmdBindGraphicsPipeline
	{
		ModulePtr	pipeline;
	};
	
	struct CmdBindComputePipeline
	{
		ModulePtr	pipeline;
	};
	
	// platform-dependent
	struct CmdBindGraphicsPipelineID;
	struct CmdBindComputePipelineID;


	//
	// Bind Vertex Buffers
	//
	struct CmdBindVertexBuffers
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
	struct CmdBindVertexBufferIDs;


	//
	// Bind Index Buffer
	//
	struct CmdBindIndexBuffer
	{
		ModulePtr					indexBuffer;
		BytesUL						offset;
		Platforms::EIndex::type		indexType;
	};
	
	// platform-dependent
	struct CmdBindIndexBufferID;


	//
	// Draw
	//
	struct CmdDraw
	{
	// variables
		uint	vertexCount;
		uint	instanceCount;
		uint	firstVertex;
		uint	firstInstance;

	// methods
		explicit
		CmdDraw (uint vertexCount,
				 uint instanceCount = 1,
				 uint firstVertex	= 0,
				 uint firstInstance = 0) :
			vertexCount(vertexCount),	instanceCount(instanceCount),
			firstVertex(firstVertex),	firstInstance(firstInstance)
		{}
	};

	struct CmdDrawIndexed
	{
	// variables
		uint	indexCount;
		uint	instanceCount;
		uint	firstVertex;
		int		vertexOffset;
		uint	firstInstance;

	// methods
		explicit
		CmdDrawIndexed (uint indexCount,
						uint instanceCount	= 1,
						uint firstVertex	= 0,
						int  vertexOffset	= 0,
						uint firstInstance	= 0) :
			indexCount(indexCount),		instanceCount(instanceCount),
			firstVertex(firstVertex),	vertexOffset(vertexOffset),
			firstInstance(firstInstance)
		{}
	};

	struct CmdDrawIndirect
	{
		ModulePtr	indirectBuffer;
		BytesU		offset;
		uint		drawCount;
		BytesU		stride;
	};

	struct CmdDrawIndexedIndirect
	{
		ModulePtr	indirectBuffer;
		BytesU		offset;
		uint		drawCount;
		BytesU		stride;
	};
	
	// platform-dependent
	struct CmdDrawIndirectID;
	struct CmdDrawIndexedIndirectID;


	//
	// Dispatch
	//
	struct CmdDispatch
	{
		GXMath::uint3	groupCount;
	};

	struct CmdDispatchIndirect
	{
		ModulePtr		indirectBuffer;
		BytesU			offset;
	};
	
	struct CmdDispatchID;
	struct CmdDispatchIndirectID;


	//
	// Execute
	//
	struct CmdExecute
	{
	// types
		using CmdBuffers_t	= FixedSizeArray< ModulePtr, 16 >;

	// variables
		CmdBuffers_t	cmdBuffers;
	};
	
	// platform-dependent
	struct CmdExecuteID;


	//
	// Bind Graphics/Compute Resource Table (DescriptorSet)
	//
	struct CmdBindGraphicsResourceTable
	{
		ModulePtr		resourceTable;
		uint			index		= 0;	// descriptor set index
	};

	struct CmdBindComputeResourceTable
	{
		ModulePtr		resourceTable;
		uint			index		= 0;	// descriptor set index
	};


	//
	// Copy Buffer to Buffer
	//
	struct CmdCopyBuffer
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
		CmdCopyBuffer (const ModulePtr &srcBuffer,
					   const ModulePtr &dstBuffer,
					   BytesUL srcOffset,
					   BytesUL dstOffset,
					   BytesUL size) :
			srcBuffer(srcBuffer),
			dstBuffer(dstBuffer)
		{
			regions.PushBack( Region{ srcOffset, dstOffset, size } );
		}

		CmdCopyBuffer (const ModulePtr &srcBuffer,
						const ModulePtr &dstBuffer,
						InitializerList<Region> regions) :
			srcBuffer(srcBuffer),	dstBuffer(dstBuffer),
			regions(regions)
		{}
	};


	//
	// Copy Image to Image
	//
	struct CmdCopyImage
	{
	// types
		using EImageLayout	= Platforms::EImageLayout;
		using EImageAspect	= Platforms::EImageAspect;
		using MipmapLevel	= Platforms::MipmapLevel;
		using ImgArrLayer	= Platforms::ImgArrLayer;
		using uint3			= GXMath::uint3;

		struct ImageLayers : CompileTime::PODStruct
		{
			EImageAspect::bits	aspectMask;
			MipmapLevel			mipLevel;
			ImgArrLayer			baseLayer;
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
		CmdCopyImage (ModulePtr				srcImage,
					  EImageLayout::type	srcLayout,
					  ModulePtr				dstImage,
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
	struct CmdCopyBufferToImage
	{
	// types
		using EImageLayout	= Platforms::EImageLayout;
		using EImageAspect	= Platforms::EImageAspect;
		using ImageLayers	= CmdCopyImage::ImageLayers;
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
		CmdCopyBufferToImage (const ModulePtr		&srcBuffer,
							  const ModulePtr		&dstImage,
							  EImageLayout::type	dstLayout,
							  InitializerList<Region> regions) :
			srcBuffer(srcBuffer),
			dstImage(dstImage), dstLayout(dstLayout),
			regions(regions)
		{}
	};


	//
	// Copy Image to Buffer
	//
	struct CmdCopyImageToBuffer
	{
	// types
		using EImageLayout	= Platforms::EImageLayout;
		using EImageAspect	= Platforms::EImageAspect;
		using ImageLayers	= CmdCopyImage::ImageLayers;
		using uint3			= GXMath::uint3;
		using Region		= CmdCopyBufferToImage::Region;
		using Regions_t		= CmdCopyBufferToImage::Regions_t;


	// variables
		ModulePtr			srcImage;
		EImageLayout::type	srcLayout;
		ModulePtr			dstBuffer;
		Regions_t			regions;


	// methods
		CmdCopyImageToBuffer (const ModulePtr			&srcImage,
							  EImageLayout::type		srcLayout,
							  const ModulePtr			&dstBuffer,
							  InitializerList<Region>	regions) :
			srcImage(srcImage),		srcLayout(srcLayout),
			dstBuffer(dstBuffer),	regions(regions)
		{}
	};


	//
	// Blit Image
	//
	struct CmdBlitImage
	{
	// types
		using EImageLayout	= Platforms::EImageLayout;
		using EImageAspect	= Platforms::EImageAspect;
		using ImageLayers	= CmdCopyImage::ImageLayers;
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
		CmdBlitImage (const ModulePtr			&srcImage,
					  EImageLayout::type		srcLayout,
					  const ModulePtr			&dstImage,
					  EImageLayout::type		dstLayout,
					  bool						linearFilter,
					  InitializerList<Region>	regions) :
			srcImage(srcImage),			srcLayout(srcLayout),
			dstImage(dstImage),			dstLayout(dstLayout),
			linearFilter(linearFilter),	regions(regions)
		{}
	};


	//
	// Update Buffer
	//
	struct CmdUpdateBuffer
	{
		ModulePtr		dstBuffer;
		BytesUL			dstOffset;
		BinaryArray		data;
	};


	//
	// Fill Buffer
	//
	struct CmdFillBuffer
	{
		ModulePtr		dstBuffer;
		BytesUL			dstOffset;
		BytesUL			size;
		uint			pattern;
	};


	//
	// Clear Color Attachments
	//
	struct CmdClearAttachments
	{
	// types
		using EImageAspect	= Platforms::EImageAspect;
		using ClearValue_t	= CmdBeginRenderPass::ClearValue_t;
		using ImgArrLayer	= Platforms::ImgArrLayer;
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
			ImgArrLayer		baseLayer;
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
	struct CmdClearColorImage
	{
	// types
		using EImageAspect	= Platforms::EImageAspect;
		using EImageLayout	= Platforms::EImageLayout;
		using MipmapLevel	= Platforms::MipmapLevel;
		using ImgArrLayer	= Platforms::ImgArrLayer;
		using ClearColor_t	= Union< GXMath::float4, GXMath::uint4, GXMath::int4 >;

		struct ImageRange : CompileTime::PODStruct
		{
		// variables
			EImageAspect::bits	aspectMask;
			MipmapLevel			baseMipLevel;
			ImgArrLayer			baseLayer;
			uint				levelCount		= 1;
			uint				layerCount		= 1;
			
		// methods
			ImageRange () {}

			explicit ImageRange (EImageAspect::bits aspect, MipmapLevel mip = Uninitialized, ImgArrLayer layer = Uninitialized) :
				aspectMask(aspect), baseMipLevel(mip), levelCount(1), baseLayer(layer), layerCount(1)
			{}
			
			ImageRange (EImageAspect::bits aspect, MipmapLevel baseMipLevel, uint levelCount, ImgArrLayer baseLayer, uint layerCount) :
				aspectMask(aspect), baseMipLevel(baseMipLevel), levelCount(levelCount), baseLayer(baseLayer), layerCount(layerCount)
			{}
		};
		using Ranges_t		= FixedSizeArray< ImageRange, 16 >;


	// variables
		ModulePtr			image;
		EImageLayout::type	layout;		// must be General or TransferDstOptimal
		Ranges_t			ranges;
		ClearColor_t		clearValue;


	// methods
		CmdClearColorImage (const ModulePtr &image, EImageLayout::type layout, const GXMath::float4 &color) :
			image( image ), layout( layout ), clearValue( color )
		{
			ranges.PushBack(ImageRange{ EImageAspect::bits() | EImageAspect::Color });
		}
	};


	//
	// Clear Depth Stencil Image
	//
	struct CmdClearDepthStencilImage
	{
	// types
		using EImageLayout	= Platforms::EImageLayout;
		using ImageRange	= CmdClearColorImage::ImageRange;
		using DepthStencil	= CmdBeginRenderPass::DepthStencil;
		using Ranges_t		= CmdClearColorImage::Ranges_t;

	// variables
		ModulePtr			image;
		EImageLayout::type	layout;
		Ranges_t			ranges;
		DepthStencil		clearValue;
	};


	//
	// Pipeline Memory Barrier
	//
	struct CmdPipelineBarrier
	{
	// types
		using EImageAspect		= Platforms::EImageAspect;
		using EImageLayout		= Platforms::EImageLayout;
		using EPipelineAccess	= Platforms::EPipelineAccess;
		using EPipelineStage	= Platforms::EPipelineStage;
		using MipmapLevel		= Platforms::MipmapLevel;
		using ImgArrLayer		= Platforms::ImgArrLayer;
		using ImageRange		= CmdClearColorImage::ImageRange;


		struct MemoryBarrier : CompileTime::PODStruct
		{
		// variables
			EPipelineAccess::bits	srcAccessMask;
			EPipelineAccess::bits	dstAccessMask;
			
		// methods
			MemoryBarrier () {}

			MemoryBarrier (EPipelineAccess::bits srcAccess, EPipelineAccess::bits dstAccess) :
				srcAccessMask(srcAccess), dstAccessMask(dstAccess)
			{}
		};


		struct ImageMemoryBarrier : CompileTime::PODStruct
		{
		// variables
			ModulePtr				image;
			EPipelineAccess::bits	srcAccessMask;
			EPipelineAccess::bits	dstAccessMask;
			EImageLayout::type		oldLayout		= EImageLayout::Unknown;
			EImageLayout::type		newLayout		= EImageLayout::Unknown;
			ImageRange				range;
			
		// methods
			ImageMemoryBarrier () : range{} {}

			ImageMemoryBarrier (const ModulePtr &image, EPipelineAccess::bits srcAccess, EPipelineAccess::bits dstAccess,
								EImageLayout::type oldLayout, EImageLayout::type newLayout, const ImageRange &range) :
				image(image), srcAccessMask(srcAccess), dstAccessMask(dstAccess), oldLayout(oldLayout), newLayout(newLayout), range(range)
			{}

			ImageMemoryBarrier (const ModulePtr &image, EPipelineAccess::bits srcAccess, EPipelineAccess::bits dstAccess,
								EImageLayout::type oldLayout, EImageLayout::type newLayout, EImageAspect::bits aspect,
								MipmapLevel mip = Uninitialized, ImgArrLayer layer = Uninitialized) :
				image(image), srcAccessMask(srcAccess), dstAccessMask(dstAccess), oldLayout(oldLayout), newLayout(newLayout), range{ aspect, mip, layer }
			{}
		};


		struct BufferMemoryBarrier : CompileTime::PODStruct
		{
		// variables
			ModulePtr				buffer;
			BytesUL					offset;
			BytesUL					size;
			EPipelineAccess::bits	srcAccessMask;
			EPipelineAccess::bits	dstAccessMask;

		// methods
			BufferMemoryBarrier () {}

			BufferMemoryBarrier (const ModulePtr &buffer, EPipelineAccess::bits srcAccess, EPipelineAccess::bits dstAccess, BytesUL offset, BytesUL size) :
				buffer(buffer), offset(offset), size(size), srcAccessMask(srcAccess), dstAccessMask(dstAccess)
			{}
		};

		static const uint	MAX_BARRIERS	= 16;

		using MemoryBarriers		= FixedSizeArray< MemoryBarrier, MAX_BARRIERS >;
		using ImageMemoryBarriers	= FixedSizeArray< ImageMemoryBarrier, MAX_BARRIERS >;
		using BufferMemoryBarriers	= FixedSizeArray< BufferMemoryBarrier, MAX_BARRIERS >;


	// variables
		EPipelineStage::bits	srcStageMask;
		EPipelineStage::bits	dstStageMask;
		MemoryBarriers			memoryBarriers;
		ImageMemoryBarriers		imageBarriers;
		BufferMemoryBarriers	bufferBarriers;


	// methods
		CmdPipelineBarrier () {}

		CmdPipelineBarrier (EPipelineStage::bits srcStage, EPipelineStage::bits dstStage, 
							EPipelineAccess::bits srcAccess, EPipelineAccess::bits dstAccess);

		CmdPipelineBarrier (EPipelineStage::bits srcStage, EPipelineStage::bits dstStage, 
							EPipelineAccess::bits srcAccess, EPipelineAccess::bits dstAccess,
							EImageLayout::type oldLayout, EImageLayout::type newLayout,
							ModulePtr image, EImageAspect::bits aspectMask,
							MipmapLevel baseMipLevel = Uninitialized, uint levelCount = 1,
							ImgArrLayer baseLayer = Uninitialized, uint layerCount = 1);

		CmdPipelineBarrier (EPipelineStage::bits srcStage, EPipelineStage::bits dstStage, 
							EImageLayout::type oldLayout, EImageLayout::type newLayout,
							ModulePtr image, EImageAspect::bits aspectMask,
							MipmapLevel baseMipLevel = Uninitialized, uint levelCount = 1,
							ImgArrLayer baseLayer = Uninitialized, uint layerCount = 1);

		CmdPipelineBarrier (EPipelineStage::bits srcStage, EPipelineStage::bits dstStage, 
							EPipelineAccess::bits srcAccess, EPipelineAccess::bits dstAccess,
							ModulePtr buffer, BytesUL offset, BytesUL size);
	};

	
/*
=================================================
	constructor
=================================================
*/
	inline
	CmdPipelineBarrier::CmdPipelineBarrier (EPipelineStage::bits srcStage, EPipelineStage::bits dstStage, 
											EPipelineAccess::bits srcAccess, EPipelineAccess::bits dstAccess) :
		srcStageMask( srcStage ),	dstStageMask( dstStage )
	{
		memoryBarriers.PushBack({ srcAccess, dstAccess });
	}
	
	inline
	CmdPipelineBarrier::CmdPipelineBarrier (EPipelineStage::bits srcStage, EPipelineStage::bits dstStage, 
											EPipelineAccess::bits srcAccess, EPipelineAccess::bits dstAccess,
											EImageLayout::type oldLayout, EImageLayout::type newLayout,
											ModulePtr image, EImageAspect::bits aspectMask,
											MipmapLevel baseMipLevel, uint levelCount,
											ImgArrLayer baseLayer, uint layerCount) :
		srcStageMask( srcStage ),	dstStageMask( dstStage )
	{
		imageBarriers.PushBack({ image, srcAccess, dstAccess, oldLayout, newLayout, ImageRange{aspectMask, baseMipLevel, levelCount, baseLayer, layerCount} });
	}
	
	inline
	CmdPipelineBarrier::CmdPipelineBarrier (EPipelineStage::bits srcStage, EPipelineStage::bits dstStage, 
											EImageLayout::type oldLayout, EImageLayout::type newLayout,
											ModulePtr image, EImageAspect::bits aspectMask,
											MipmapLevel baseMipLevel, uint levelCount,
											ImgArrLayer baseLayer, uint layerCount) :
		srcStageMask( srcStage ),	dstStageMask( dstStage )
	{
		EPipelineAccess::bits	src_access;
		EPipelineAccess::bits	dst_access;

		switch ( oldLayout )
		{
			case EImageLayout::Undefined :						break;
			case EImageLayout::Preinitialized :					src_access |= EPipelineAccess::HostWrite;					break;
			case EImageLayout::ColorAttachmentOptimal :			src_access |= EPipelineAccess::ColorAttachmentWrite;		break;
			case EImageLayout::DepthStencilAttachmentOptimal :	src_access |= EPipelineAccess::DepthStencilAttachmentWrite;	break;
			case EImageLayout::TransferSrcOptimal :				src_access |= EPipelineAccess::TransferRead;				break;
			case EImageLayout::TransferDstOptimal :				src_access |= EPipelineAccess::TransferWrite;				break;
			case EImageLayout::ShaderReadOnlyOptimal :			src_access |= EPipelineAccess::ShaderRead;					break;
			default :											WARNING( "unsupported layout" );
		}

		switch ( newLayout )
		{
			case EImageLayout::TransferDstOptimal :				dst_access |= EPipelineAccess::TransferWrite;				break;
			case EImageLayout::TransferSrcOptimal :				dst_access |= EPipelineAccess::TransferRead;				break;
			case EImageLayout::ColorAttachmentOptimal :			dst_access |= EPipelineAccess::ColorAttachmentWrite;		break;
			case EImageLayout::DepthStencilAttachmentOptimal :	dst_access |= EPipelineAccess::DepthStencilAttachmentWrite;	break;
			case EImageLayout::ShaderReadOnlyOptimal :			dst_access |= EPipelineAccess::ShaderRead;
																if ( src_access.IsZero() )
																	src_access |= (EPipelineAccess::HostWrite | EPipelineAccess::TransferWrite);
																break;
			default :											WARNING( "unsupported layout" );
		}

		imageBarriers.PushBack({ image, src_access, dst_access, oldLayout, newLayout, ImageRange{aspectMask, baseMipLevel, levelCount, baseLayer, layerCount} });
	}
	
	inline
	CmdPipelineBarrier::CmdPipelineBarrier (EPipelineStage::bits srcStage, EPipelineStage::bits dstStage, 
											EPipelineAccess::bits srcAccess, EPipelineAccess::bits dstAccess,
											ModulePtr buffer, BytesUL offset, BytesUL size) :
		srcStageMask( srcStage ),	dstStageMask( dstStage )
	{
		bufferBarriers.PushBack({ buffer, srcAccess, dstAccess, offset, size });
	}
//-----------------------------------------------------------------------------

}	// GpuMsg
}	// Engine
