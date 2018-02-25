// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/RenderPassEnums.h"
#include "Engine/Platforms/Shared/GPU/RenderStateEnums.h"
#include "Engine/Platforms/Shared/GPU/VertexEnums.h"
#include "Engine/Platforms/Shared/GPU/ImageEnums.h"
#include "Engine/Platforms/Shared/GPU/CommandEnums.h"
#include "Engine/Platforms/Shared/GPU/ShaderEnums.h"
#include "Engine/Platforms/Shared/GPU/MipmapLevel.h"
#include "Engine/Platforms/Shared/GPU/ImageLayer.h"
#include "Engine/Platforms/Shared/GPU/Sync.h"

namespace Engine
{
namespace Platforms
{

	//
	// GPU Command Buffer Descriptor
	//

	struct CommandBufferDescriptor : CompileTime::PODStruct
	{
		ECmdBufferCreate::bits	flags;

		CommandBufferDescriptor () {}

		explicit CommandBufferDescriptor (ECmdBufferCreate::bits flags) : flags{flags}
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
		ModulePtr							gpuThread;			// can be null
		//ModulePtr							builder;
		Platforms::CommandBufferDescriptor	descr;
	};


	//
	// GPU Command Buffer Builder Create Info
	//

	struct GpuCommandBuilder
	{
		ModulePtr	gpuThread;			// can be null
	};
	
}	// CreateInfo


namespace GpuMsg
{

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
			Recording,		// recording state
			Executable,		// ready to execute
			Pending,		// added to queue
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
		ReadOnce< Set<ModulePtr> >		resources;
	};


	//
	// Set Viewport
	//
	struct CmdSetViewport
	{
	// types
		struct Viewport : CompileTime::PODStruct
		{
			RectU		rect;
			float2		depthRange;

			Viewport () {}
			Viewport (const RectU &rect, const float2 &depth) : rect(rect), depthRange(depth) {}
		};

		using Viewports_t	= FixedSizeArray< Viewport, GlobalConst::Graphics_MaxViewports >;


	// variables
		Viewports_t		viewports;
		uint			firstViewport	= 0;

	// methods
		CmdSetViewport (const RectU &rect,
						const float2 &depthRange,
						uint firstViewport = 0)
		{
			viewports.PushBack( Viewport( rect, depthRange ) );
			this->firstViewport = firstViewport;
		}

		CmdSetViewport (ArrayCRef<Viewport> list, uint firstViewport = 0) :
			viewports(list), firstViewport(firstViewport)
		{}
	};


	//
	// Set Scissor
	//
	struct CmdSetScissor
	{
	// types
		using Scissors_t	= FixedSizeArray< RectU, GlobalConst::Graphics_MaxViewports >;

	// variables
		Scissors_t		scissors;
		uint			firstScissor	= 0;

	// methods
		CmdSetScissor (const RectU &scissorRect, uint firstScissor = 0)
		{
			scissors.PushBack( scissorRect );
			this->firstScissor = firstScissor;
		}

		CmdSetScissor (ArrayCRef<RectU> list, uint firstScissor = 0) :
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
		color4f		color;
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
	// variables
		Platforms::ECmdBufferCreate::bits	flags;
		ModulePtr							targetCmdBuffer;

	// methods
		CmdBegin () {}
		explicit CmdBegin (Platforms::ECmdBufferCreate::bits flags) : flags{flags} {}
		explicit CmdBegin (const ModulePtr &target) : targetCmdBuffer(target) {}
	};


	//
	// End
	//
	struct CmdEnd
	{
		Out< ModulePtr >	result;
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

		using ClearValue_t	= Union< float4, uint4, int4, DepthStencil >;
		using ClearValues_t	= FixedSizeArray< ClearValue_t, GlobalConst::Graphics_MaxColorBuffers+1 >;

	// variables
		ModulePtr		renderPass;
		ModulePtr		framebuffer;
		RectU			area;
		ClearValues_t	clearValues;	// TODO: map names to indices ?

	// methods
		CmdBeginRenderPass () {}

		CmdBeginRenderPass (const ModulePtr &renderPass, const ModulePtr &framebuffer, const RectU &area, ArrayCRef<ClearValue_t> clearValues = Uninitialized) :
			renderPass{renderPass}, framebuffer{framebuffer}, area{area}, clearValues(clearValues)
		{}
	};


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

	// methods
		explicit CmdBindVertexBuffers (const ModulePtr &vb, BytesUL off = Uninitialized)
		{
			vertexBuffers.PushBack( vb );
			offsets.PushBack( off );
		}
	};


	//
	// Bind Index Buffer
	//
	struct CmdBindIndexBuffer
	{
	// types
		using EIndex	= Platforms::EIndex;

	// variables
		ModulePtr		indexBuffer;
		BytesUL			offset;
		EIndex::type	indexType		= EIndex::Unknown;

	// methods
		CmdBindIndexBuffer (const ModulePtr &ib, EIndex::type type, BytesUL off = Uninitialized) :
			indexBuffer(ib), offset(off), indexType(type)
		{}
	};


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
		uint	firstIndex;
		int		vertexOffset;
		uint	firstInstance;

	// methods
		explicit
		CmdDrawIndexed (uint indexCount,
						uint instanceCount	= 1,
						uint firstIndex		= 0,
						int  vertexOffset	= 0,
						uint firstInstance	= 0) :
			indexCount(indexCount),		instanceCount(instanceCount),
			firstIndex(firstIndex),		vertexOffset(vertexOffset),
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


	//
	// Dispatch
	//
	struct CmdDispatch
	{
		uint3			groupCount;
	};

	struct CmdDispatchIndirect
	{
		ModulePtr		indirectBuffer;
		BytesU			offset;
	};


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
			Region (Bytes<uint> srcOffset, Bytes<uint> dstOffset, Bytes<uint> size) : srcOffset(BytesUL(srcOffset)), dstOffset(BytesUL(dstOffset)), size(BytesUL(size)) {}
			Region (Bytes<ulong> srcOffset, Bytes<ulong> dstOffset, Bytes<ulong> size) : srcOffset(srcOffset), dstOffset(dstOffset), size(size) {}
		};

		using Regions_t	= FixedSizeArray< Region, 8 >;


	// variables
		ModulePtr	srcBuffer;
		ModulePtr	dstBuffer;
		Regions_t	regions;


	// methods
		CmdCopyBuffer (GX_DEFCTOR) {}

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

		CmdCopyBuffer (const ModulePtr		&srcBuffer,
						const ModulePtr		&dstBuffer,
						ArrayCRef<Region>	regions) :
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
		using ImageLayer	= Platforms::ImageLayer;

		struct ImageLayers : CompileTime::PODStruct
		{
		// variables
			EImageAspect::bits	aspectMask;
			MipmapLevel			mipLevel;
			ImageLayer			baseLayer;
			uint				layerCount = 1;

		// methods
			ImageLayers (GX_DEFCTOR) {}

			ImageLayers (EImageAspect::bits	aspectMask,
						 MipmapLevel		mipLevel,
						 ImageLayer			baseLayer,
						 uint				layerCount) :
				aspectMask(aspectMask), mipLevel(mipLevel), baseLayer(baseLayer), layerCount(layerCount)
			{}
		};

		struct Region : CompileTime::PODStruct
		{
		// variables
			ImageLayers		srcLayers;
			uint3			srcOffset;
			ImageLayers		dstLayers;
			uint3			dstOffset;
			uint3			size;

		// methods
			Region (GX_DEFCTOR) {}
			
			Region (const ImageLayers	&srcLayers,
					const uint3			&srcOffset,
					const ImageLayers	&dstLayers,
					const uint3			&dstOffset,
					const uint3			&size) :
				srcLayers{srcLayers}, srcOffset{srcOffset}, dstLayers{dstLayers}, dstOffset{dstOffset}, size{size}
			{}
		};
		using Regions_t		= FixedSizeArray< Region, 8 >;


	// variables
		ModulePtr			srcImage;
		EImageLayout::type	srcLayout	= EImageLayout::Unknown;
		ModulePtr			dstImage;
		EImageLayout::type	dstLayout	= EImageLayout::Unknown;
		Regions_t			regions;


	// methods
		CmdCopyImage (GX_DEFCTOR) {}

		CmdCopyImage (ModulePtr				srcImage,
					  EImageLayout::type	srcLayout,
					  ModulePtr				dstImage,
					  EImageLayout::type	dstLayout,
					  ArrayCRef<Region>		regions) :
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

		struct Region : CompileTime::PODStruct
		{
			// src
			BytesUL		bufferOffset;
			uint		bufferRowLength		= 0;	// pixels ?
			uint		bufferImageHeight	= 0;	// pixels ?
			// dst
			ImageLayers	imageLayers;
			uint3		imageOffset;
			uint3		imageSize;
		};
		using Regions_t	= FixedSizeArray< Region, 8 >;


	// variables
		ModulePtr			srcBuffer;
		ModulePtr			dstImage;
		EImageLayout::type	dstLayout	= EImageLayout::Unknown;
		Regions_t			regions;


	// methods
		CmdCopyBufferToImage (GX_DEFCTOR) {}

		CmdCopyBufferToImage (const ModulePtr		&srcBuffer,
							  const ModulePtr		&dstImage,
							  EImageLayout::type	dstLayout,
							  ArrayCRef<Region>		regions) :
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
		using Region		= CmdCopyBufferToImage::Region;
		using Regions_t		= CmdCopyBufferToImage::Regions_t;


	// variables
		ModulePtr			srcImage;
		EImageLayout::type	srcLayout	= EImageLayout::Unknown;
		ModulePtr			dstBuffer;
		Regions_t			regions;


	// methods
		CmdCopyImageToBuffer (GX_DEFCTOR) {}

		CmdCopyImageToBuffer (const ModulePtr		&srcImage,
							  EImageLayout::type	srcLayout,
							  const ModulePtr		&dstBuffer,
							  ArrayCRef<Region>		regions) :
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
		using ImageLayers	= CmdCopyImage::ImageLayers;

		struct Region : CompileTime::PODStruct
		{
		// variables
			ImageLayers		srcLayers;
			uint3			srcOffset0;		// start offset
			uint3			srcOffset1;		// end offset
			ImageLayers		dstLayers;
			uint3			dstOffset0;		// start offset
			uint3			dstOffset1;		// end offset

		// methods
			Region (GX_DEFCTOR) {}

			Region (const ImageLayers	&srcLayers,
					const uint3			&srcOffset0,
					const uint3			&srcOffset1,
					const ImageLayers	&dstLayers,
					const uint3			&dstOffset0,
					const uint3			&dstOffset1) :
				srcLayers{srcLayers}, srcOffset0{srcOffset0}, srcOffset1{srcOffset1},
				dstLayers{dstLayers}, dstOffset0{dstOffset0}, dstOffset1{dstOffset1}
			{}
		};
		using Regions_t		= FixedSizeArray< Region, 8 >;


	// variables
		ModulePtr			srcImage;
		EImageLayout::type	srcLayout		= EImageLayout::Unknown;
		ModulePtr			dstImage;
		EImageLayout::type	dstLayout		= EImageLayout::Unknown;
		bool				linearFilter	= false;
		Regions_t			regions;


	// methods
		CmdBlitImage (GX_DEFCTOR) {}

		CmdBlitImage (const ModulePtr		&srcImage,
					  EImageLayout::type	srcLayout,
					  const ModulePtr		&dstImage,
					  EImageLayout::type	dstLayout,
					  bool					linearFilter,
					  ArrayCRef<Region>		regions) :
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
	// types
		template <typename T>
		using NonArray_t = CompileTime::EnableIf< CompileTime::IsMemCopyAvailable<T> and not CompileTime::IsSameTypes< T, BinArrayCRef > >;

	// variables
		ModulePtr		dstBuffer;
		BytesUL			dstOffset;
		BinaryArray		data;

	// methods
		CmdUpdateBuffer (const ModulePtr &buf, BinArrayCRef data, Bytes<uint> offset = Uninitialized) : dstBuffer{buf}, dstOffset{BytesUL(offset)}, data{data} {}
		CmdUpdateBuffer (const ModulePtr &buf, BinArrayCRef data, Bytes<ulong> offset) : dstBuffer{buf}, dstOffset{offset}, data{data} {}

		template <typename T, typename = NonArray_t<T>>
		CmdUpdateBuffer (const ModulePtr &buf, const T &data, Bytes<uint> offset = Uninitialized) : CmdUpdateBuffer{ buf, BinArrayCRef::FromValue(data), offset } {}
		
		template <typename T, typename = NonArray_t<T>>
		CmdUpdateBuffer (const ModulePtr &buf, const T &data, Bytes<ulong> offset) : CmdUpdateBuffer{ buf, BinArrayCRef::FromValue(data), offset } {}
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
		using EImageAspect		= Platforms::EImageAspect;
		using DepthStencil_t	= CmdBeginRenderPass::DepthStencil;
		using ClearValue_t		= CmdBeginRenderPass::ClearValue_t;
		using ImageLayer		= Platforms::ImageLayer;

		struct Attachment : CompileTime::PODStruct
		{
			EImageAspect::bits	aspectMask;
			uint				attachmentIdx	= 0;
			ClearValue_t		clearValue;

			Attachment () {}
			Attachment (EImageAspect::bits aspect, uint idx, const float4 &color) : aspectMask(aspect), attachmentIdx(idx), clearValue(color) {}
			Attachment (EImageAspect::bits aspect, uint idx, const int4 &color) : aspectMask(aspect), attachmentIdx(idx), clearValue(color) {}
			Attachment (EImageAspect::bits aspect, uint idx, const uint4 &color) : aspectMask(aspect), attachmentIdx(idx), clearValue(color) {}
			Attachment (EImageAspect::bits aspect, uint idx, float depth, uint stencil) : aspectMask(aspect), attachmentIdx(idx), clearValue(DepthStencil_t{depth, stencil}) {}
		};

		struct ClearRect : CompileTime::PODStruct
		{
			RectU			rect;
			ImageLayer		baseLayer;
			uint			layerCount	= 1;

			ClearRect () {}
			ClearRect (const RectU &rect, ImageLayer layer = ImageLayer(0), uint count = 1) : rect(rect), baseLayer(layer), layerCount(count) {}
		};

		using Attachments_t		= FixedSizeArray< Attachment, GlobalConst::Graphics_MaxColorBuffers >;
		using ClearRects_t		= FixedSizeArray< ClearRect, 16 >;


	// variables
		Attachments_t	attachments;
		ClearRects_t	clearRects;

	// methods
		CmdClearAttachments () {}
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
		using ImageLayer	= Platforms::ImageLayer;
		using ClearColor_t	= Union< float4, uint4, int4 >;

		struct ImageRange : CompileTime::PODStruct
		{
		// variables
			EImageAspect::bits	aspectMask;
			MipmapLevel			baseMipLevel;
			ImageLayer			baseLayer;
			uint				levelCount		= 1;
			uint				layerCount		= 1;
			
		// methods
			ImageRange () {}

			explicit ImageRange (EImageAspect::bits aspect, MipmapLevel mip = Uninitialized, ImageLayer layer = Uninitialized) :
				aspectMask(aspect), baseMipLevel(mip), baseLayer(layer), levelCount(1), layerCount(1)
			{}
			
			ImageRange (EImageAspect::bits aspect, MipmapLevel baseMipLevel, uint levelCount, ImageLayer baseLayer, uint layerCount) :
				aspectMask(aspect), baseMipLevel(baseMipLevel), baseLayer(baseLayer), levelCount(levelCount), layerCount(layerCount)
			{}
		};
		using Ranges_t		= FixedSizeArray< ImageRange, 16 >;


	// variables
		ModulePtr			image;
		EImageLayout::type	layout;		// must be General or TransferDstOptimal
		Ranges_t			ranges;
		ClearColor_t		clearValue;


	// methods
		CmdClearColorImage (const ModulePtr &image, EImageLayout::type layout, const float4 &color) :
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
	// Set Event State (signaled / non-signaled)
	//
	struct CmdSetEvent
	{
	// types
		using Event_t			= Platforms::GpuEventId;
		using EPipelineStage	= Platforms::EPipelineStage;

	// variables
		Event_t					eventId;
		EPipelineStage::bits	stageMask;

	// methods
		CmdSetEvent (Event_t id, EPipelineStage::bits stageMask) : eventId(id), stageMask(stageMask) {}
		explicit CmdSetEvent (Event_t id) : eventId{id}, stageMask{ EPipelineStage::bits() | EPipelineStage::BottomOfPipe } {}
	};

	struct CmdResetEvent : CmdSetEvent
	{};


	//
	// Wait Events
	//
	struct CmdWaitEvents
	{
	// types
		using Event_t		= Platforms::GpuEventId;
		using Events_t		= FixedSizeArray< Event_t, 16 >;

	// variables
		Events_t		events;
		// TODO

	// methods
		explicit CmdWaitEvents (Event_t event) { events.PushBack( event ); }
		explicit CmdWaitEvents (ArrayCRef<Event_t> events) : events(events) {}
	};


	//
	// Push Constants
	//
	struct CmdPushConstants
	{
	// types
		using Data_t	= FixedSizeArray< uint, 32 >;
		using EShader	= Platforms::EShader;

	// variables
		ModulePtr		pipelineLayout;
		EShader::bits	stages;
		BytesU			offset;
		Data_t			data;

	// methods
		CmdPushConstants ()
		{}

		CmdPushConstants (const ModulePtr &layout, BinArrayCRef data, EShader::bits stages = EShader::All, BytesU off = Uninitialized) :
			pipelineLayout{layout}, stages{stages}, offset{off}, data{ArrayCRef<uint>::From(data)}
		{}
	};

	struct CmdPushNamedConstants
	{
	// types
		using Value_t		= Union< int, int2, int3, int4,
									 uint, uint2, uint3, uint4,
									 ilong, ilong2, ilong3, ilong4,
									 ulong, ulong2, ulong3, ulong4,
									 float, float2, float3, float4,
									 float2x2, float3x3, float4x4,
									 double, double2, double3, double4 >;
		using Name_t		= StaticString<64>;
		using ValueMap_t	= FixedSizeArray< Pair< Name_t, Value_t >, 8 >;

	// variables
		ModulePtr		pipelineLayout;
		ValueMap_t		values;

	// methods
		explicit CmdPushNamedConstants (const ModulePtr &layout) : pipelineLayout{layout} {}

		template <typename T>
		CmdPushNamedConstants& Add (StringCRef name, const T &value)
		{
			values.PushBack({ name, value });
			return *this;
		}
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
		using ImageLayer		= Platforms::ImageLayer;
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
								MipmapLevel mip = Uninitialized, ImageLayer layer = Uninitialized) :
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
							ImageLayer baseLayer = Uninitialized, uint layerCount = 1);

		CmdPipelineBarrier (EPipelineStage::bits srcStage, EPipelineStage::bits dstStage, 
							EImageLayout::type oldLayout, EImageLayout::type newLayout,
							ModulePtr image, EImageAspect::bits aspectMask,
							MipmapLevel baseMipLevel = Uninitialized, uint levelCount = 1,
							ImageLayer baseLayer = Uninitialized, uint layerCount = 1);

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
											ImageLayer baseLayer, uint layerCount) :
		srcStageMask( srcStage ),	dstStageMask( dstStage )
	{
		imageBarriers.PushBack({ image, srcAccess, dstAccess, oldLayout, newLayout, ImageRange{aspectMask, baseMipLevel, levelCount, baseLayer, layerCount} });
	}
	
	inline
	CmdPipelineBarrier::CmdPipelineBarrier (EPipelineStage::bits srcStage, EPipelineStage::bits dstStage, 
											EImageLayout::type oldLayout, EImageLayout::type newLayout,
											ModulePtr image, EImageAspect::bits aspectMask,
											MipmapLevel baseMipLevel, uint levelCount,
											ImageLayer baseLayer, uint layerCount) :
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
