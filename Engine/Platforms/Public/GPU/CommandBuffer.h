// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/CommandQueue.h"
#include "Engine/Platforms/Public/GPU/RenderStateEnums.h"
#include "Engine/Platforms/Public/GPU/VertexEnums.h"
#include "Engine/Platforms/Public/GPU/ImageEnums.h"
#include "Engine/Platforms/Public/GPU/ShaderEnums.h"

namespace Engine
{
namespace Platforms
{

	//
	// GPU Command Buffer Description
	//

	struct CommandBufferDescription : CompileTime::PODStruct
	{
	// variables
		ECmdBufferCreate::bits	flags;
		
	// methods
		CommandBufferDescription () {}

		explicit CommandBufferDescription (ECmdBufferCreate::bits flags) : flags{flags} {}
	};

}	// Platforms


namespace CreateInfo
{

	//
	// GPU Command Buffer Create Info
	//
	struct GpuCommandBuffer
	{
	// variables
		ModulePtr							gpuThread;			// can be null
		//ModulePtr							builder;
		Platforms::CommandBufferDescription	descr;

	// methods
		GpuCommandBuffer () {}
		explicit GpuCommandBuffer (const Platforms::CommandBufferDescription &descr) : descr{descr} {}
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
	using EImageLayout		= Platforms::EImageLayout;
	using GpuEventId		= Platforms::GpuEventId;
	
	//
	// Image Subresource Range
	//
	struct ImageRange final : CompileTime::PODStruct
	{
	// variables
		EImageAspect::bits	aspectMask;
		MipmapLevel			mipLevel;
		ImageLayer			baseLayer;
		uint				layerCount = 1;

	// methods
		ImageRange () {}

		ImageRange (EImageAspect::bits	aspectMask,
					MipmapLevel			mipLevel,
					ImageLayer			baseLayer,
					uint				layerCount) :
			aspectMask(aspectMask), mipLevel(mipLevel), baseLayer(baseLayer), layerCount(layerCount)
		{}
	};


	//
	// Get Command Buffer Description
	//
	struct GetCommandBufferDescription : _MsgBase_
	{
		Out< Platforms::CommandBufferDescription >	result;
	};


	//
	// Set/Get Command Buffer State
	//
	struct SetCommandBufferState : _MsgBase_
	{
	// types
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
		
	// variables
		EState	newState	= EState::Unknown;
		
	// methods
		explicit SetCommandBufferState (EState state) : newState{state} {}
	};


	struct GetCommandBufferState : _MsgBase_
	{
	// types
		using EState = SetCommandBufferState::EState;
		
	// variables
		Out< EState >	result;
	};


	//
	// Command Buffer State Changed Event
	//
	struct OnCommandBufferStateChanged : _MsgBase_
	{
	// types
		using EState = SetCommandBufferState::EState;
		
	// variables
		EState		oldState	= EState::Unknown;
		EState		newState	= EState::Unknown;
		
	// methods
		OnCommandBufferStateChanged (EState oldState, EState newState) : oldState{oldState}, newState{newState} {}
	};


	//
	// Set Command Buffer Dependency
	//
	struct SetCommandBufferDependency : _MsgBase_
	{
	// variables
		ReadOnce< Set<ModulePtr> >		resources;
		
	// methods
		explicit SetCommandBufferDependency (Set<ModulePtr> &&res) : resources{ RVREF(res) } {}
	};


	//
	// Set Viewport
	//
	struct CmdSetViewport : _MsgBase_
	{
	// types
		struct Viewport : CompileTime::PODStruct
		{
			RectU		rect;
			float2		depthRange;

			Viewport () {}
			Viewport (const RectU &rect, const float2 &depth) : rect(rect), depthRange(depth) {}
		};

		using Viewports_t	= FixedSizeArray< Viewport, GlobalConst::GAPI_MaxViewports >;


	// variables
		Viewports_t		viewports;
		uint			firstViewport	= 0;

	// methods
		CmdSetViewport () {}

		CmdSetViewport (const RectU &rect,
						const float2 &depthRange,
						uint firstViewport = 0)
		{
			viewports.PushBack( Viewport( rect, depthRange ) );
			this->firstViewport = firstViewport;
		}

		explicit CmdSetViewport (ArrayCRef<Viewport> list, uint firstViewport = 0) :
			viewports(list), firstViewport(firstViewport)  {}
	};


	//
	// Set Scissor
	//
	struct CmdSetScissor : _MsgBase_
	{
	// types
		using Scissors_t	= FixedSizeArray< RectU, GlobalConst::GAPI_MaxViewports >;

	// variables
		Scissors_t		scissors;
		uint			firstScissor	= 0;

	// methods
		CmdSetScissor () {}

		explicit CmdSetScissor (const RectU &scissorRect, uint firstScissor = 0)
		{
			scissors.PushBack( scissorRect );
			this->firstScissor = firstScissor;
		}

		explicit CmdSetScissor (ArrayCRef<RectU> list, uint firstScissor = 0) :
			scissors(list), firstScissor(firstScissor)  {}
	};


	//
	// Set Depth Bounds
	//
	struct CmdSetDepthBounds : _MsgBase_
	{
	// variables
		float	min		= 0.0f;
		float	max		= 1.0f;
		
	// methods
		CmdSetDepthBounds () {}
		CmdSetDepthBounds (float min, float max) : min{min}, max{max} {}
	};


	//
	// Set Blend Color
	//
	struct CmdSetBlendColor : _MsgBase_
	{
	// variables
		color4f		color;
		
	// methods
		CmdSetBlendColor () {}
		explicit CmdSetBlendColor (const color4f &color) : color{color} {}
	};


	//
	// Set Depth Bias
	//
	struct CmdSetDepthBias : _MsgBase_
	{
	// variables
		float	biasConstFactor		= 0.0f;
		float	biasClamp			= 0.0f;
		float	biasSlopeFactor		= 0.0f;
		
	// methods
		CmdSetDepthBias () {}

		CmdSetDepthBias (float biasConstFactor, float biasClamp, float biasSlopeFactor) :
			biasConstFactor{biasConstFactor}, biasClamp{biasClamp}, biasSlopeFactor{biasSlopeFactor}  {}
	};


	//
	// Set Line Width
	//
	struct CmdSetLineWidth : _MsgBase_
	{
	// variables
		float	width	= 0.0f;
		
	// methods
		CmdSetLineWidth () {}
		explicit CmdSetLineWidth (float width) : width{width} {}
	};

	
	//
	// Set Stencil Compare Mask
	//
	struct CmdSetStencilCompareMask : _MsgBase_
	{
	// variables
		Platforms::EPolygonFace::type	face		= Uninitialized;
		uint							cmpMask		= 0;
		
	// methods
		CmdSetStencilCompareMask () {}
		CmdSetStencilCompareMask (Platforms::EPolygonFace::type face, uint cmpMask) : face{face}, cmpMask{cmpMask} {}
	};


	//
	// Set Stencil Write Mask
	//
	struct CmdSetStencilWriteMask : _MsgBase_
	{
	// variables
		Platforms::EPolygonFace::type	face	= Uninitialized;
		uint							mask	= 0;
		
	// methods
		CmdSetStencilWriteMask () {}
		CmdSetStencilWriteMask (Platforms::EPolygonFace::type face, uint mask) : face{face}, mask{mask} {}
	};


	//
	// Set Stencil Reference
	//
	struct CmdSetStencilReference : _MsgBase_
	{
	// variables
		Platforms::EPolygonFace::type	face	= Uninitialized;
		uint							ref		= 0;
		
	// methods
		CmdSetStencilReference () {}
		CmdSetStencilReference (Platforms::EPolygonFace::type face, uint ref) : face{face}, ref{ref} {}
	};


	//
	// Begin
	//
	struct CmdBegin : _MsgBase_
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
	struct CmdEnd : _MsgBase_
	{
		Out_opt< ModulePtr >		result;
	};


	//
	// Begin Render Pass
	//
	struct CmdBeginRenderPass : _MsgBase_
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
		using ClearValues_t	= FixedSizeArray< ClearValue_t, GlobalConst::GAPI_MaxColorBuffers+1 >;

	// variables
		ModulePtr		renderPass;
		ModulePtr		framebuffer;
		RectU			area;
		ClearValues_t	clearValues;	// TODO: map names to indices ?

	// methods
		CmdBeginRenderPass () {}

		CmdBeginRenderPass (const ModulePtr &renderPass, const ModulePtr &framebuffer, const RectU &area, ArrayCRef<ClearValue_t> clearValues = Uninitialized) :
			renderPass{renderPass}, framebuffer{framebuffer}, area{area}, clearValues(clearValues)  {}
	};


	//
	// End Render Pass
	//
	struct CmdEndRenderPass : _MsgBase_
	{
	};


	//
	// Next Subpass
	//
	struct CmdNextSubpass : _MsgBase_
	{
	};


	//
	// Bind Graphics Pipeline
	//
	struct CmdBindGraphicsPipeline : _MsgBase_
	{
	// variables
		ModulePtr	pipeline;
		
	// methods
		explicit CmdBindGraphicsPipeline (const ModulePtr &pipeline) : pipeline{pipeline} {}
	};
	
	struct CmdBindComputePipeline : _MsgBase_
	{
	// variables
		ModulePtr	pipeline;
		
	// methods
		explicit CmdBindComputePipeline (const ModulePtr &pipeline) : pipeline{pipeline} {}
	};


	//
	// Bind Vertex Buffers
	//
	struct CmdBindVertexBuffers : _MsgBase_
	{
	// types
		using Buffers_t		= FixedSizeArray< ModulePtr, GlobalConst::GAPI_MaxAttribs >;
		using Offsets_t		= FixedSizeArray< BytesU, GlobalConst::GAPI_MaxAttribs >;

	// variables
		Buffers_t		vertexBuffers;
		Offsets_t		offsets;
		uint			firstBinding	= 0;

	// methods
		CmdBindVertexBuffers () {}

		explicit CmdBindVertexBuffers (const ModulePtr &vb, BytesU off = Uninitialized)
		{
			vertexBuffers.PushBack( vb );
			offsets.PushBack( off );
		}
	};


	//
	// Bind Index Buffer
	//
	struct CmdBindIndexBuffer : _MsgBase_
	{
	// types
		using EIndex	= Platforms::EIndex;

	// variables
		ModulePtr		buffer;
		BytesU			offset;
		EIndex::type	indexType	= EIndex::Unknown;

	// methods
		CmdBindIndexBuffer () {}

		CmdBindIndexBuffer (const ModulePtr &ib, EIndex::type type, BytesU off = Uninitialized) :
			buffer(ib), offset(off), indexType(type)  {}
	};


	//
	// Draw
	//
	struct CmdDraw : _MsgBase_
	{
	// variables
		uint	vertexCount		= 0;
		uint	instanceCount	= 1;
		uint	firstVertex		= 0;
		uint	firstInstance	= 0;

	// methods
		CmdDraw () {}

		explicit CmdDraw (uint vertexCount,
						  uint instanceCount = 1,
						  uint firstVertex	 = 0,
						  uint firstInstance = 0) :
			vertexCount(vertexCount),	instanceCount(instanceCount),
			firstVertex(firstVertex),	firstInstance(firstInstance)	{}
	};

	struct CmdDrawIndexed : _MsgBase_
	{
	// variables
		uint	indexCount		= 0;
		uint	instanceCount	= 1;
		uint	firstIndex		= 0;
		int		vertexOffset	= 0;
		uint	firstInstance	= 0;

	// methods
		CmdDrawIndexed () {}

		explicit CmdDrawIndexed (uint indexCount,
								 uint instanceCount	= 1,
								 uint firstIndex	= 0,
								 int  vertexOffset	= 0,
								 uint firstInstance	= 0) :
			indexCount(indexCount),		instanceCount(instanceCount),
			firstIndex(firstIndex),		vertexOffset(vertexOffset),
			firstInstance(firstInstance)	{}
	};

	struct CmdDrawIndirect : _MsgBase_
	{
	// variables
		ModulePtr	indirectBuffer;
		BytesU		offset;
		uint		drawCount	= 1;
		BytesU		stride;

	// methods
		CmdDrawIndirect () {}

		CmdDrawIndirect (const ModulePtr &indirectBuffer,
						 BytesU			 offset,
						 uint			 drawCount,
						 BytesU			 stride) :
			indirectBuffer{indirectBuffer},		offset{offset},
			drawCount{drawCount},				stride{stride}	{}
	};

	struct CmdDrawIndexedIndirect : _MsgBase_
	{
	// variables
		ModulePtr	indirectBuffer;
		BytesU		offset;
		uint		drawCount	= 1;
		BytesU		stride;
		
	// methods
		CmdDrawIndexedIndirect () {}

		CmdDrawIndexedIndirect (const ModulePtr &indirectBuffer,
								BytesU			 offset,
								uint			 drawCount,
								BytesU			 stride) :
			indirectBuffer{indirectBuffer},		offset{offset},
			drawCount{drawCount},				stride{stride}	{}
	};

	struct CmdDrawIndirectCount : _MsgBase_
	{
	// variables
		ModulePtr	indirectBuffer;
		BytesU		indirectBufferOffset;
		ModulePtr	countBuffer;
		BytesU		countBufferOffset;
		uint		maxDrawCount	= 1;
		BytesU		stride;

	// methods
		CmdDrawIndirectCount () {}
		
		CmdDrawIndirectCount (const ModulePtr &indirectBuffer,
							  BytesU		  indirectBufferOffset,
							  const ModulePtr &countBuffer,
							  BytesU		  countBufferOffset,
							  uint			  maxDrawCount,
							  BytesU		  stride) :
			indirectBuffer{indirectBuffer},		indirectBufferOffset{indirectBufferOffset},\
			countBuffer{countBuffer},			countBufferOffset{countBufferOffset},
			maxDrawCount{maxDrawCount},			stride{stride}	{}
	};

	struct CmdDrawIndexedIndirectCount : _MsgBase_
	{
	// variables
		ModulePtr	indirectBuffer;
		BytesU		indirectBufferOffset;
		ModulePtr	countBuffer;
		BytesU		countBufferOffset;
		uint		maxDrawCount	= 1;
		BytesU		stride;
		
	// methods
		CmdDrawIndexedIndirectCount () {}

		CmdDrawIndexedIndirectCount (const ModulePtr &indirectBuffer,
									 BytesU			 indirectBufferOffset,
									 const ModulePtr &countBuffer,
									 BytesU			 countBufferOffset,
									 uint			 maxDrawCount,
									 BytesU			 stride) :
			indirectBuffer{indirectBuffer},		indirectBufferOffset{indirectBufferOffset},\
			countBuffer{countBuffer},			countBufferOffset{countBufferOffset},
			maxDrawCount{maxDrawCount},			stride{stride}	{}
	};


	//
	// Dispatch
	//
	struct CmdDispatch : _MsgBase_
	{
	// variables
		uint3			groupCount;
		
	// methods
		CmdDispatch () {}
		explicit CmdDispatch (const uint3 &groupCount) : groupCount{groupCount} {}
	};

	struct CmdDispatchIndirect : _MsgBase_
	{
	// variables
		ModulePtr		indirectBuffer;
		BytesU			offset;

	// methods
		CmdDispatchIndirect () {}
		explicit CmdDispatchIndirect (const ModulePtr &indirectBuffer, BytesU offset = Uninitialized) : indirectBuffer{indirectBuffer}, offset{offset} {}
	};

	struct CmdDispatchBase : _MsgBase_
	{
	// variables
		uint3			baseGroup;
		uint3			groupCount;
		
	// methods
		CmdDispatchBase () {}
		explicit CmdDispatchBase (const uint3 baseGroup, const uint3 &groupCount) : baseGroup{baseGroup}, groupCount{groupCount} {}
	};


	//
	// Execute
	//
	struct CmdExecute : _MsgBase_
	{
	// types
		using CmdBuffers_t	= FixedSizeArray< ModulePtr, 16 >;

	// variables
		CmdBuffers_t	cmdBuffers;

	// variables
		CmdExecute () {}
		explicit CmdExecute (const ModulePtr cmd) { cmdBuffers.PushBack( cmd ); }
		explicit CmdExecute (ArrayCRef<ModulePtr> cmds) : cmdBuffers{cmds} {}
	};


	//
	// Bind Graphics/Compute Resource Table (DescriptorSet)
	//
	struct CmdBindGraphicsResourceTable : _MsgBase_
	{
	// variables
		ModulePtr		resourceTable;
		uint			index		= 0;	// descriptor set index

	// methods
		explicit CmdBindGraphicsResourceTable (const ModulePtr &resourceTable, uint index = 0) : resourceTable{resourceTable}, index{index} {}
	};

	struct CmdBindComputeResourceTable : _MsgBase_
	{
	// variables
		ModulePtr		resourceTable;
		uint			index		= 0;	// descriptor set index
		
	// methods
		explicit CmdBindComputeResourceTable (const ModulePtr &resourceTable, uint index = 0) : resourceTable{resourceTable}, index{index} {}
	};


	//
	// Copy Buffer to Buffer
	//
	struct CmdCopyBuffer : _MsgBase_
	{
	// types
		struct Region final : CompileTime::PODStruct
		{
		// variables
			BytesU		srcOffset;
			BytesU		dstOffset;
			BytesU		size;

		// methods
			Region () {}
			Region (BytesU srcOffset, BytesU dstOffset, BytesU size) : srcOffset(srcOffset), dstOffset(dstOffset), size(size) {}
		};

		using Regions_t	= FixedSizeArray< Region, GlobalConst::GAPI_MaxCopyRegions >;


	// variables
		ModulePtr	srcBuffer;
		ModulePtr	dstBuffer;
		Regions_t	regions;


	// methods
		CmdCopyBuffer () {}

		CmdCopyBuffer (const ModulePtr &srcBuffer, const ModulePtr &dstBuffer) :
			srcBuffer(srcBuffer), dstBuffer(dstBuffer)
		{}

		CmdCopyBuffer& AddRegion (BytesU srcOffset, BytesU dstOffset, BytesU size)
		{
			regions.PushBack(Region{ srcOffset, dstOffset, size });
			return *this;
		}
	};


	//
	// Copy Image to Image
	//
	struct CmdCopyImage : _MsgBase_
	{
	// types
		struct Region final : CompileTime::PODStruct
		{
		// variables
			ImageRange	srcLayers;
			uint3		srcOffset;
			ImageRange	dstLayers;
			uint3		dstOffset;
			uint3		size;

		// methods
			Region () {}
			
			Region (const ImageRange	&srcLayers,
					const uint3			&srcOffset,
					const ImageRange	&dstLayers,
					const uint3			&dstOffset,
					const uint3			&size) :
				srcLayers{srcLayers}, srcOffset{srcOffset}, dstLayers{dstLayers}, dstOffset{dstOffset}, size{size}
			{}
		};
		using Regions_t		= FixedSizeArray< Region, GlobalConst::GAPI_MaxCopyRegions >;


	// variables
		ModulePtr			srcImage;
		EImageLayout::type	srcLayout	= EImageLayout::Unknown;
		ModulePtr			dstImage;
		EImageLayout::type	dstLayout	= EImageLayout::Unknown;
		Regions_t			regions;


	// methods
		CmdCopyImage () {}

		CmdCopyImage (ModulePtr				srcImage,
					  EImageLayout::type	srcLayout,
					  ModulePtr				dstImage,
					  EImageLayout::type	dstLayout) :
			srcImage(srcImage), srcLayout(srcLayout),
			dstImage(dstImage), dstLayout(dstLayout)
		{}

		CmdCopyImage& SetSource (const ModulePtr &image, EImageLayout::type layout)
		{
			this->srcImage  = image;
			this->srcLayout = layout;
			return *this;
		}

		CmdCopyImage& SetDestination (const ModulePtr &image, EImageLayout::type layout)
		{
			this->dstImage  = image;
			this->dstLayout = layout;
			return *this;
		}

		CmdCopyImage& AddRegion (const ImageRange	&srcLayers,
								 const uint3		&srcOffset,
								 const ImageRange	&dstLayers,
								 const uint3		&dstOffset,
								 const uint3		&size)
		{
			regions.PushBack(Region{ srcLayers, srcOffset, dstLayers, dstOffset, size });
			return *this;
		}
	};


	//
	// Copy Buffer to Image
	//
	struct CmdCopyBufferToImage : _MsgBase_
	{
	// types
		struct Region final : CompileTime::PODStruct
		{
		// variables
			BytesU		bufferOffset;
			uint		bufferRowLength		= 0;	// pixels
			uint		bufferImageHeight	= 0;	// pixels
			ImageRange	imageLayers;
			uint3		imageOffset;
			uint3		imageSize;

		// methods
			Region () {}

			Region (BytesU				bufferOffset,
					uint				bufferRowLength,
					uint				bufferImageHeight,
					const ImageRange	&imageLayers,
					const uint3			&imageOffset,
					const uint3			&imageSize) :
				bufferOffset{bufferOffset}, bufferRowLength{bufferRowLength}, bufferImageHeight{bufferImageHeight},
				imageLayers{imageLayers}, imageOffset{imageOffset}, imageSize{imageSize}
			{}
		};
		using Regions_t	= FixedSizeArray< Region, GlobalConst::GAPI_MaxCopyRegions >;


	// variables
		ModulePtr			srcBuffer;
		ModulePtr			dstImage;
		EImageLayout::type	dstLayout	= EImageLayout::Unknown;
		Regions_t			regions;


	// methods
		CmdCopyBufferToImage () {}

		CmdCopyBufferToImage (const ModulePtr		&srcBuffer,
							  const ModulePtr		&dstImage,
							  EImageLayout::type	dstLayout) :
			srcBuffer(srcBuffer),	dstImage(dstImage),
			dstLayout(dstLayout)
		{}

		CmdCopyBufferToImage& AddRegion (BytesU				bufferOffset,
										 uint				bufferRowLength,
										 uint				bufferImageHeight,
										 const ImageRange	&imageLayers,
										 const uint3		&imageOffset,
										 const uint3		&imageSize)
		{
			regions.PushBack(Region{ bufferOffset, bufferRowLength, bufferImageHeight,
									 imageLayers, imageOffset, imageSize });
			return *this;
		}
	};


	//
	// Copy Image to Buffer
	//
	struct CmdCopyImageToBuffer : _MsgBase_
	{
	// types
		using Region		= CmdCopyBufferToImage::Region;
		using Regions_t		= CmdCopyBufferToImage::Regions_t;


	// variables
		ModulePtr			srcImage;
		EImageLayout::type	srcLayout	= EImageLayout::Unknown;
		ModulePtr			dstBuffer;
		Regions_t			regions;


	// methods
		CmdCopyImageToBuffer () {}

		CmdCopyImageToBuffer (const ModulePtr		&srcImage,
							  EImageLayout::type	srcLayout,
							  const ModulePtr		&dstBuffer) :
			srcImage(srcImage),		srcLayout(srcLayout),
			dstBuffer(dstBuffer)
		{}

		CmdCopyImageToBuffer& AddRegion (BytesU				bufferOffset,
										 uint				bufferRowLength,
										 uint				bufferImageHeight,
										 const ImageRange	&imageLayers,
										 const uint3		&imageOffset,
										 const uint3		&imageSize)
		{
			regions.PushBack(Region{ bufferOffset, bufferRowLength, bufferImageHeight,
									 imageLayers, imageOffset, imageSize });
			return *this;
		}
	};


	//
	// Blit Image
	//
	struct CmdBlitImage : _MsgBase_
	{
	// types
		struct Region final : CompileTime::PODStruct
		{
		// variables
			ImageRange	srcLayers;
			uint3		srcOffset0;		// start offset
			uint3		srcOffset1;		// end offset
			ImageRange	dstLayers;
			uint3		dstOffset0;		// start offset
			uint3		dstOffset1;		// end offset

		// methods
			Region () {}
			Region (const ImageRange	&srcLayers,
					const uint3			&srcOffset0,
					const uint3			&srcOffset1,
					const ImageRange	&dstLayers,
					const uint3			&dstOffset0,
					const uint3			&dstOffset1) :
				srcLayers{srcLayers}, srcOffset0{srcOffset0}, srcOffset1{srcOffset1},
				dstLayers{dstLayers}, dstOffset0{dstOffset0}, dstOffset1{dstOffset1} {}
		};
		using Regions_t = FixedSizeArray< Region, GlobalConst::GAPI_MaxBlitRegions >;


	// variables
		ModulePtr			srcImage;
		EImageLayout::type	srcLayout		= EImageLayout::Unknown;
		ModulePtr			dstImage;
		EImageLayout::type	dstLayout		= EImageLayout::Unknown;
		bool				linearFilter	= false;
		Regions_t			regions;


	// methods
		CmdBlitImage () {}

		CmdBlitImage (const ModulePtr		&srcImage,
					  EImageLayout::type	srcLayout,
					  const ModulePtr		&dstImage,
					  EImageLayout::type	dstLayout,
					  bool					linearFilter) :
			srcImage(srcImage),			srcLayout(srcLayout),
			dstImage(dstImage),			dstLayout(dstLayout),
			linearFilter(linearFilter)
		{}

		CmdBlitImage& AddRegion (const ImageRange	&srcLayers,
								 const uint3		&srcOffset0,
								 const uint3		&srcOffset1,
								 const ImageRange	&dstLayers,
								 const uint3		&dstOffset0,
								 const uint3		&dstOffset1)
		{
			regions.PushBack(Region{ srcLayers, srcOffset0, srcOffset1,
									 dstLayers, dstOffset0, dstOffset1 });
			return *this;
		}
	};


	//
	// Resolve Image
	//
	struct CmdResolveImage : _MsgBase_
	{
	// types
		struct Region final : CompileTime::PODStruct
		{
		// variables
			ImageRange	srcLayers;
			uint3		srcOffset;
			ImageRange	dstLayers;
			uint3		dstOffset;
			uint3		extent;

		// methods
			Region () {}
			Region (const ImageRange	&srcLayers,
					const uint3			&srcOffset,
					const ImageRange	&dstLayers,
					const uint3			&dstOffset,
					const uint3			&extent) :
				srcLayers{srcLayers}, srcOffset{srcOffset},
				dstLayers{dstLayers}, dstOffset{dstOffset}, extent{extent} {}
		};
		using Regions_t = FixedSizeArray< Region, GlobalConst::GAPI_MaxResolveRegions >;


	// variables
		ModulePtr			srcImage;
		EImageLayout::type	srcLayout		= EImageLayout::Unknown;
		ModulePtr			dstImage;
		EImageLayout::type	dstLayout		= EImageLayout::Unknown;
		Regions_t			regions;


	// methods
		CmdResolveImage () {}
		
		CmdResolveImage (const ModulePtr	&srcImage,
						 EImageLayout::type	srcLayout,
						 const ModulePtr	&dstImage,
						 EImageLayout::type	dstLayout) :
			srcImage(srcImage),			srcLayout(srcLayout),
			dstImage(dstImage),			dstLayout(dstLayout)
		{}

		CmdResolveImage& AddRegion (const ImageRange	&srcLayers,
									const uint3			&srcOffset,
									const ImageRange	&dstLayers,
									const uint3			&dstOffset,
									const uint3			&extent)
		{
			regions.PushBack(Region{ srcLayers, srcOffset, dstLayers, dstOffset, extent });
			return *this;
		}
	};


	//
	// Update Buffer
	//
	struct CmdUpdateBuffer : _MsgBase_
	{
	// variables
		ModulePtr		dstBuffer;
		BytesU			dstOffset;
		BinaryArray		data;

	// methods
		CmdUpdateBuffer (const ModulePtr &buf, BinArrayCRef data, BytesU offset = Uninitialized) :
			dstBuffer{buf}, dstOffset{offset}, data{data} {}
	};


	//
	// Fill Buffer
	//
	struct CmdFillBuffer : _MsgBase_
	{
	// variables
		ModulePtr		dstBuffer;
		BytesU			dstOffset;
		BytesU			size;
		uint			pattern		= 0;

	// methods
		CmdFillBuffer () {}

		explicit CmdFillBuffer (const ModulePtr &buffer, uint pattern = 0) :
			dstBuffer{buffer}, size{~0_b}, pattern{pattern} {}

		CmdFillBuffer (const ModulePtr &buffer, uint pattern, BytesU size, BytesU off = Uninitialized) :	// TODO
			dstBuffer{buffer}, dstOffset{off}, size{size}, pattern{pattern} {}
	};


	//
	// Clear Color Attachments
	//
	struct CmdClearAttachments : _MsgBase_
	{
	// types
		using DepthStencil_t	= CmdBeginRenderPass::DepthStencil;
		using ClearValue_t		= CmdBeginRenderPass::ClearValue_t;

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

		using Attachments_t		= FixedSizeArray< Attachment, GlobalConst::GAPI_MaxColorBuffers >;
		using ClearRects_t		= FixedSizeArray< ClearRect, GlobalConst::GAPI_MaxClearRects >;


	// variables
		Attachments_t	attachments;
		ClearRects_t	clearRects;

	// methods
		CmdClearAttachments () {}

		CmdClearAttachments& AddAttachment (const Attachment &value)
		{
			attachments.PushBack( value );
			return *this;
		}
		
		CmdClearAttachments& AddRect (const ClearRect &value)
		{
			clearRects.PushBack( value );
			return *this;
		}
	};


	//
	// Clear Color Image
	//
	struct CmdClearColorImage : _MsgBase_
	{
	// types
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

			explicit ImageRange (EImageAspect::bits aspect) : ImageRange{ aspect, MipmapLevel(0) } {}

			ImageRange (EImageAspect::bits aspect, MipmapLevel mip, ImageLayer layer = Uninitialized) :
				aspectMask(aspect), baseMipLevel(mip), baseLayer(layer), levelCount(1), layerCount(1)
			{}
			
			ImageRange (EImageAspect::bits aspect, MipmapLevel baseMipLevel, uint levelCount, ImageLayer baseLayer, uint layerCount) :
				aspectMask(aspect), baseMipLevel(baseMipLevel), baseLayer(baseLayer), levelCount(levelCount), layerCount(layerCount)
			{}
		};
		using Ranges_t		= FixedSizeArray< ImageRange, GlobalConst::GAPI_MaxClearRects >;
		using ClearColor_t	= Union< float4, uint4, int4 >;


	// variables
		ModulePtr			image;
		EImageLayout::type	layout;		// must be General or TransferDstOptimal
		Ranges_t			ranges;
		ClearColor_t		clearValue;


	// methods
		CmdClearColorImage () {}

		CmdClearColorImage (const ModulePtr &image, EImageLayout::type layout) :
			image( image ), layout( layout ) {}

		CmdClearColorImage& Clear (const float4 &value)			{ clearValue.Create( value );  return *this; }
		CmdClearColorImage& Clear (const uint4 &value)			{ clearValue.Create( value );  return *this; }
		CmdClearColorImage& Clear (const int4 &value)			{ clearValue.Create( value );  return *this; }

		CmdClearColorImage& AddRange (const ImageRange& value)	{ ranges.PushBack( value );  return *this; }
	};


	//
	// Clear Depth Stencil Image
	//
	struct CmdClearDepthStencilImage : _MsgBase_
	{
	// types
		using ImageRange	= CmdClearColorImage::ImageRange;
		using DepthStencil	= CmdBeginRenderPass::DepthStencil;
		using ImageRange_t	= CmdClearColorImage::ImageRange;
		using Ranges_t		= CmdClearColorImage::Ranges_t;

	// variables
		ModulePtr			image;
		EImageLayout::type	layout		= EImageLayout::Unknown;
		Ranges_t			ranges;
		DepthStencil		clearValue;

	// methods
		CmdClearDepthStencilImage () {}

		CmdClearDepthStencilImage (const ModulePtr &		image,
									EImageLayout::type		layout,
									DepthStencil			clearValue) :
			image{image}, layout{layout}, clearValue{clearValue}
		{}

		CmdClearDepthStencilImage& AddRange (const ImageRange& value)	{ ranges.PushBack( value );  return *this; }
	};
	

	//
	// Set Event State (signaled / non-signaled)
	//
	struct CmdSetEvent : _MsgBase_
	{
	// variables
		GpuEventId				eventId;
		EPipelineStage::bits	stageMask;

	// methods
		CmdSetEvent (GpuEventId id, EPipelineStage::bits stageMask) : eventId(id), stageMask(stageMask) {}
		explicit CmdSetEvent (GpuEventId id) : eventId{id}, stageMask{ EPipelineStage::BottomOfPipe } {}
	};

	struct CmdResetEvent : CmdSetEvent
	{};


	//
	// Push Constants
	//
	struct CmdPushConstants : _MsgBase_
	{
	// types
		using Data_t	= FixedSizeArray< uint, GlobalConst::GAPI_MaxPushConstants >;
		using EShader	= Platforms::EShader;

	// variables
		ModulePtr		pipelineLayout;		// (resourceTable)
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

	struct CmdPushNamedConstants : _MsgBase_
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
		using ValueMap_t	= FixedSizeArray< Pair< Name_t, Value_t >, GlobalConst::GAPI_MaxPushConstants / 4 >;

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
	// Begin Query
	//
	struct CmdBeginQuery : _MsgBase_
	{
	// variables
		ModulePtr	queryPool;
		uint		queryIndex	= 0;
		bool		precise		= true;	// for occlusion query only,
										// if 'false' then result may be zero or non-zero value instead of precise result.

	// methods
		CmdBeginQuery (const ModulePtr &queryPool, uint index, bool precise = true) :
			queryPool{queryPool}, queryIndex{index}, precise{precise} {}
	};


	//
	// End Query
	//
	struct CmdEndQuery : _MsgBase_
	{
	// variables
		ModulePtr	queryPool;
		uint		queryIndex	= 0;

	// methods
		CmdEndQuery (const ModulePtr &queryPool, uint index) : queryPool{queryPool}, queryIndex{index} {}
	};


	//
	// Copy Query Pool Results
	//
	struct CmdCopyQueryPoolResults : _MsgBase_
	{
	// variables
		ModulePtr	queryPool;
		uint		firstQueryIndex	= 0;
		uint		queryCount		= 1;
		ModulePtr	dstBuffer;
		BytesU		dstOffset;
		BytesU		stride;
		
	// methods
		CmdCopyQueryPoolResults () {}

		CmdCopyQueryPoolResults&  SetSource (const ModulePtr &query, uint firstIndex, uint count) {
			this->queryPool			= query;
			this->firstQueryIndex	= firstIndex;
			this->queryCount		= count;
			return *this;
		}

		CmdCopyQueryPoolResults&  SetDestination (const ModulePtr &buffer, BytesU offset, BytesU valStride = SizeOf<ulong>) {
			this->dstBuffer	= buffer;
			this->dstOffset	= offset;
			this->stride	= valStride;
			return *this;
		}
	};


	//
	// Write Timestamp
	//
	struct CmdWriteTimestamp : _MsgBase_
	{
	// variables
		ModulePtr				queryPool;
		uint					queryIndex	= 0;
		EPipelineStage::type	stage		= Uninitialized;

	// methods
		CmdWriteTimestamp (const ModulePtr &queryPool, uint index, EPipelineStage::type stage) :
			queryPool{queryPool}, queryIndex{index}, stage{stage} {}
	};


	//
	// Reset Query Pool
	//
	struct CmdResetQueryPool : _MsgBase_
	{
	// variables
		ModulePtr	queryPool;
		uint		firstQueryIndex	= 0;
		uint		queryCount		= 1;
		
	// methods
		CmdResetQueryPool () {}

		CmdResetQueryPool (const ModulePtr &queryPool, uint firstIndex, uint count) :
			queryPool{queryPool}, firstQueryIndex{firstIndex}, queryCount{count} {}
	};


	//
	// Pipeline Memory Barrier
	//
	struct CmdPipelineBarrier : _MsgBase_
	{
	// types
		using EPipelineAccess	= Platforms::EPipelineAccess;
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
								MipmapLevel mip = Uninitialized, uint levelCount = 1,
								ImageLayer layer = Uninitialized, uint layerCount = 1) :
				image(image), srcAccessMask(srcAccess), dstAccessMask(dstAccess), oldLayout(oldLayout), newLayout(newLayout),
				range{ aspect, mip, levelCount, layer, layerCount }
			{}
		};


		struct BufferMemoryBarrier : CompileTime::PODStruct
		{
		// variables
			ModulePtr				buffer;
			BytesU					offset;
			BytesU					size;
			EPipelineAccess::bits	srcAccessMask;
			EPipelineAccess::bits	dstAccessMask;

		// methods
			BufferMemoryBarrier () {}

			BufferMemoryBarrier (const ModulePtr &buffer, EPipelineAccess::bits srcAccess, EPipelineAccess::bits dstAccess, BytesU offset, BytesU size) :
				buffer(buffer), offset(offset), size(size), srcAccessMask(srcAccess), dstAccessMask(dstAccess) {}
		};

		using MemoryBarriers_t			= FixedSizeArray< MemoryBarrier, GlobalConst::GAPI_MaxBarriers >;
		using ImageMemoryBarriers_t		= FixedSizeArray< ImageMemoryBarrier, GlobalConst::GAPI_MaxBarriers >;
		using BufferMemoryBarriers_t	= FixedSizeArray< BufferMemoryBarrier, GlobalConst::GAPI_MaxBarriers >;


	// variables
		EPipelineStage::bits	srcStageMask;
		EPipelineStage::bits	dstStageMask;
		MemoryBarriers_t		memoryBarriers;
		ImageMemoryBarriers_t	imageBarriers;
		BufferMemoryBarriers_t	bufferBarriers;


	// methods
		CmdPipelineBarrier () {}
		
		CmdPipelineBarrier (EPipelineStage::bits srcStage, EPipelineStage::bits dstStage) :
			srcStageMask( srcStage ),	dstStageMask( dstStage ) {}

		CmdPipelineBarrier& AddMemory (const MemoryBarrier &barrier)		{ memoryBarriers.PushBack( barrier );  return *this; }
		CmdPipelineBarrier& AddImage (const ImageMemoryBarrier &barrier)	{ imageBarriers.PushBack( barrier );  return *this; }
		CmdPipelineBarrier& AddBuffer (const BufferMemoryBarrier &barrier)	{ bufferBarriers.PushBack( barrier );  return *this; }
	};


	//
	// Wait Events
	//
	struct CmdWaitEvents : _MsgBase_
	{
	// types
		using Events_t					= FixedSizeArray< GpuEventId, GlobalConst::GAPI_MaxWaitEvents >;
		using MemoryBarrier				= CmdPipelineBarrier::MemoryBarrier;
		using ImageMemoryBarrier		= CmdPipelineBarrier::ImageMemoryBarrier;
		using BufferMemoryBarrier		= CmdPipelineBarrier::BufferMemoryBarrier;
		using MemoryBarriers_t			= CmdPipelineBarrier::MemoryBarriers_t;
		using ImageMemoryBarriers_t		= CmdPipelineBarrier::ImageMemoryBarriers_t;
		using BufferMemoryBarriers_t	= CmdPipelineBarrier::BufferMemoryBarriers_t;

	// variables
		Events_t				events;
		EPipelineStage::bits	srcStageMask;
		EPipelineStage::bits	dstStageMask;
		MemoryBarriers_t		memoryBarriers;
		ImageMemoryBarriers_t	imageBarriers;
		BufferMemoryBarriers_t	bufferBarriers;

	// methods
		CmdWaitEvents () {}

		CmdWaitEvents (GpuEventId event, EPipelineStage::bits srcStage, EPipelineStage::bits dstStage) :
			events{{ event }}, srcStageMask( srcStage ), dstStageMask( dstStage ) {}

		CmdWaitEvents (ArrayCRef<GpuEventId> events, EPipelineStage::bits srcStage, EPipelineStage::bits dstStage) :
			events(events), srcStageMask( srcStage ), dstStageMask( dstStage ) {}
		
		CmdWaitEvents& AddMemory (const MemoryBarrier &barrier)			{ memoryBarriers.PushBack( barrier );  return *this; }
		CmdWaitEvents& AddImage (const ImageMemoryBarrier &barrier)		{ imageBarriers.PushBack( barrier );  return *this; }
		CmdWaitEvents& AddBuffer (const BufferMemoryBarrier &barrier)	{ bufferBarriers.PushBack( barrier );  return *this; }
	};


	//
	// Debug Marker
	//
	struct CmdDebugMarker : _MsgBase_
	{
	// variables
		String		info;
		bool		breakPoint	= false;

	// methods
		explicit CmdDebugMarker (StringCRef info, bool breakPoint = false) : info{info}, breakPoint{breakPoint} {}
	};
	


	//
	// Push / Pop Debug Group
	//
	struct CmdPushDebugGroup : _MsgBase_
	{
	// variables
		String		info;

	// methods
		explicit CmdPushDebugGroup (StringCRef info) : info{info} {}
	};
	

	struct CmdPopDebugGroup : _MsgBase_
	{
	};



	using DefaultComputeCommands_t		= MessageListFrom<
											CmdBegin,
											CmdEnd,
											CmdBindComputePipeline,
											CmdBindComputeResourceTable,
											CmdDispatch,
											CmdDispatchIndirect,
											CmdExecute,
											CmdCopyBuffer,
											CmdCopyImage,
											CmdCopyBufferToImage,
											CmdCopyImageToBuffer,
											CmdUpdateBuffer,
											CmdFillBuffer,
											CmdClearColorImage,
											//CmdSetEvent,
											//CmdResetEvent,
											//CmdWaitEvents,
											CmdPipelineBarrier,
											CmdPushConstants,
											CmdPushNamedConstants,
											CmdDebugMarker,
											CmdPushDebugGroup,
											CmdPopDebugGroup,
											CmdBeginQuery,
											CmdEndQuery,
											CmdCopyQueryPoolResults,
											CmdWriteTimestamp,
											CmdResetQueryPool >;

	using DefaultRenderPassCommands_t	= MessageListFrom<
											CmdBeginRenderPass,
											CmdEndRenderPass,
											CmdNextSubpass,
											CmdSetViewport,
											CmdSetScissor,
											CmdSetDepthBounds,
											CmdSetBlendColor,
											CmdSetDepthBias,
											CmdSetLineWidth,
											CmdSetStencilCompareMask,
											CmdSetStencilWriteMask,
											CmdSetStencilReference,
											CmdBindVertexBuffers,
											CmdBindIndexBuffer,
											CmdDraw,
											CmdDrawIndexed,
											CmdDrawIndirect,
											CmdDrawIndexedIndirect,
											//CmdDrawIndirectCount,
											//CmdDrawIndexedIndirectCount,
											CmdClearAttachments,
											CmdBindGraphicsPipeline,
											CmdBindGraphicsResourceTable,
											CmdPushConstants,
											CmdPushNamedConstants >;

	using DefaultGraphicsCommands_t		= MessageListFrom<
											CmdBegin,
											CmdEnd,
											CmdExecute,
											CmdCopyBuffer,
											CmdCopyImage,
											CmdCopyBufferToImage,
											CmdCopyImageToBuffer,
											CmdBlitImage,
											CmdResolveImage,
											CmdUpdateBuffer,
											CmdFillBuffer,
											CmdClearColorImage,
											CmdClearDepthStencilImage,
											//CmdSetEvent,
											//CmdResetEvent,
											//CmdWaitEvents,
											CmdPipelineBarrier,
											CmdDebugMarker,
											CmdPushDebugGroup,
											CmdPopDebugGroup >
										::Append< DefaultRenderPassCommands_t >;

//-----------------------------------------------------------------------------

}	// GpuMsg
}	// Engine
