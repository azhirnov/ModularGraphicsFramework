// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_SOFT

#include "Engine/Platforms/Soft/Impl/SWEnums.h"
#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/Public/GPU/CommandBuffer.h"
#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Sampler.h"

namespace Engine
{
namespace PlatformSW
{
	//
	// Sync Primitives
	//
	using SWFencePtr		= SharedPointerType< class SWFence >;
	using SWEventPtr		= SharedPointerType< class SWEvent >;
	using SWSemaphorePtr	= SharedPointerType< class SWSemaphore >;

}	// PlatformSW


namespace CreateInfo
{

	//
	// Command Queue Create Info
	//
	struct GpuCommandQueue
	{
	// types
		struct EQueueFamily
		{
			enum type : uint
			{
				Graphics,
				Compute,
				Transfer,
				_Count,

				Unknown = ~0u
			};
			
			GX_ENUM_BITFIELD( EQueueFamily );

			static constexpr bits	All = bits().SetAll();
		};

	// variables
		ModulePtr			gpuThread;
		EQueueFamily::bits	family;
	};

}	// CreateInfo


namespace GpuMsg
{

	//
	// Get GPU Device Info
	//
	struct GetSWDeviceInfo
	{
	};


	//
	// Get Image Memory Layout
	//
	struct GetSWImageMemoryLayout
	{
	// types
		using EPixelFormat		= Platforms::EPixelFormat;
		using EImageLayout		= Platforms::EImageLayout;
		using EPipelineAccess	= Platforms::EPipelineAccess;
		using EPipelineStage	= Platforms::EPipelineStage;
		using EMemoryAccess		= Platforms::EMemoryAccess;

		struct ImgLayer : CompileTime::FastCopyable
		{
		// variables
			uint2					dimension;
			void *					memory	= null;	// initialized when binded to memory
			BytesU					size;
			EPixelFormat::type		format	= Uninitialized;
			EImageLayout::type		layout	= EImageLayout::Undefined;
			EPipelineAccess::bits	access;								// last access type
			EPipelineStage::type	stage	= EPipelineStage::Unknown;	// stage of last access

		// methods
			ImgLayer (GX_DEFCTOR) {}

			BinArrayRef		Data ()			{ return BinArrayRef::FromVoid( memory, size ); }
			BinArrayCRef	Data () const	{ return BinArrayCRef::FromVoid( memory, size ); }
		};

		struct ImgLayers2D : CompileTime::FastCopyable
		{
		// types
			using ImgLayers_t = FixedSizeArray< ImgLayer, 14 >;
			
		// variables
			ImgLayers_t			mipmaps;
			uint2				dimension;		// must be equal to mipmaps[0].dimension
			BytesU				size;			// summary size of all mipmaps
			
		// methods
			ImgLayers2D (GX_DEFCTOR) {}
		};

		struct ImgLayers3D
		{
		// types
			using ImgLayers2D_t	= Array< ImgLayers2D >;
			
		// variables
			ImgLayers2D_t		layers;
			uint3				dimension;		// must be equal to uint3( layers[0].dimension, ... )
			BytesU				size;			// summary size of all layers
			BytesU				align	= 4_b;	// base align
			EMemoryAccess::bits	memAccess;		// TODO: remove, use 'access'
			
		// methods
			ImgLayers3D (GX_DEFCTOR) {}
		};

		// image1D:  { layers[1], dim(w,1,1) }, { mipmaps[], dim(w,1) }
		// image2D:  { layers[1], dim(w,h,1) }, { mipmaps[], dim(w,h) }
		// image2DA: { layers[a], dim(w,h,1) }, { mipmaps[], dim(w,h) }
		// image3D:  { layers[d], dim(w,h,d) }, { mipmaps[], dim(w,h) }

	// variables
		EPipelineAccess::bits	accessMask;
		EPipelineStage::type	stage	= EPipelineStage::Unknown;
		Out< ImgLayers3D >		result;

	// methods
		GetSWImageMemoryLayout (EPipelineAccess::bits access, EPipelineStage::type stage) :
			accessMask{access}, stage{stage} {}
	};


	//
	// Get Image View Memory Layout
	//
	struct GetSWImageViewMemoryLayout
	{
	// types
		using ImgLayers3D		= GetSWImageMemoryLayout::ImgLayers3D;
		using EPipelineAccess	= Platforms::EPipelineAccess;
		using EPipelineStage	= Platforms::EPipelineStage;

	// variables
		Platforms::ImageViewDescriptor	viewDescr;
		EPipelineAccess::bits			accessMask;
		EPipelineStage::type			stage	= EPipelineStage::Unknown;
		Out< ImgLayers3D >				result;

	// methods
		GetSWImageViewMemoryLayout (EPipelineAccess::bits access, EPipelineStage::type stage) :
			viewDescr{}, accessMask{access}, stage{stage} {}

		GetSWImageViewMemoryLayout (const Platforms::ImageViewDescriptor &descr, EPipelineAccess::bits access, EPipelineStage::type stage) :
			viewDescr{descr}, accessMask{access}, stage{stage} {}
	};


	//
	// Get Image Memory Requirements
	//
	struct GetSWImageMemoryRequirements
	{
	// types
		struct MemReq {
			BytesU		size;
			BytesU		align;
		};

	// variables
		Out< MemReq >	result;
	};


	//
	// Get Texture Memory Layout
	//
	struct GetSWTextureMemoryLayout : GetSWImageViewMemoryLayout
	{
	// variables
		Out< Platforms::SamplerDescriptor >		sampler;

	// methods
		//GetSWTextureMemoryLayout () {}
		//explicit GetSWTextureMemoryLayout (const Platforms::ImageViewDescriptor &descr) : GetSWImageViewMemoryLayout{descr} {}
	};


	//
	// Image Barrier
	//
	struct SWImageBarrier
	{
	// types
		using Barrier_t			= CmdPipelineBarrier::ImageMemoryBarrier;
		using Barriers_t		= CmdPipelineBarrier::ImageMemoryBarriers;
		using EPipelineStage	= Platforms::EPipelineStage;

	// variables
		Barriers_t				barriers;
		EPipelineStage::bits	srcStageMask;
		EPipelineStage::bits	dstStageMask;

	// methods
		SWImageBarrier () {}

		explicit SWImageBarrier (const Barrier_t &imgBarrier, EPipelineStage::bits srcStage, EPipelineStage::bits dstStage) :
			srcStageMask{srcStage}, dstStageMask{dstStage} { barriers.PushBack(imgBarrier); }
	};


	//
	// Get Buffer Memory Layout
	//
	struct GetSWBufferMemoryLayout
	{
	// types
		using EMemoryAccess		= Platforms::EMemoryAccess;
		using EPipelineAccess	= Platforms::EPipelineAccess;
		using EPipelineStage	= Platforms::EPipelineStage;

		struct Data {
			BinArrayRef			memory;
			BytesU				align;
			EMemoryAccess::bits	memAccess;	// TODO: remove, use 'access'
		};

	// variables
		BytesU					offset;
		BytesU					size;
		EPipelineAccess::bits	access;
		EPipelineStage::type	stage	= EPipelineStage::Unknown;
		Out< Data >				result;

	// methods
		GetSWBufferMemoryLayout (EPipelineAccess::bits access, EPipelineStage::type stage) : 
			offset{0}, size{usize(UMax)}, access{access}, stage{stage} {}
		
		GetSWBufferMemoryLayout (BytesU offset, BytesU size, EPipelineAccess::bits access, EPipelineStage::type stage) : 
			offset{offset}, size{size}, access{access}, stage{stage} {}
	};


	//
	// Get Buffer Memory Requirements
	//
	struct GetSWBufferMemoryRequirements
	{
	// types
		struct MemReq {
			BytesU		size;
			BytesU		align;
		};

	// variables
		Out< MemReq >	result;
	};


	//
	// Buffer Barrier
	//
	struct SWBufferBarrier
	{
	// types
		using Barrier_t			= CmdPipelineBarrier::BufferMemoryBarrier;
		using Barriers_t		= CmdPipelineBarrier::BufferMemoryBarriers;
		using EPipelineStage	= Platforms::EPipelineStage;

	// variables
		Barriers_t				barriers;
		EPipelineStage::bits	srcStageMask;
		EPipelineStage::bits	dstStageMask;

	// methods
		SWBufferBarrier () {}

		explicit SWBufferBarrier (const Barrier_t &bufBarrier, EPipelineStage::bits srcStage, EPipelineStage::bits dstStage) :
			srcStageMask{srcStage}, dstStageMask{dstStage} { barriers.PushBack(bufBarrier); }
	};


	//
	// Get Memory Data
	//
	struct GetSWMemoryData
	{
		Out< BinArrayRef >	result;
	};


	//
	// Memory Barrier
	//
	struct SWMemoryBarrier
	{
	// types
		using Barrier_t			= CmdPipelineBarrier::MemoryBarrier;
		using Barriers_t		= CmdPipelineBarrier::MemoryBarriers;
		using EPipelineStage	= Platforms::EPipelineStage;

	// variables
		Barriers_t				barriers;
		EPipelineStage::bits	srcStageMask;
		EPipelineStage::bits	dstStageMask;

	// methods
		SWMemoryBarrier () {}

		explicit SWMemoryBarrier (const Barrier_t &memBarrier, EPipelineStage::bits srcStage, EPipelineStage::bits dstStage) :
			srcStageMask{srcStage}, dstStageMask{dstStage} { barriers.PushBack(memBarrier); }
	};
	

	//
	// Get GPU Shader Module IDs
	//
	struct GetSWShaderModuleIDs
	{
	// types
		using SWInvoke		= Platforms::PipelineTemplateDescriptor::ShaderSource::SWInvoke;

		struct ShaderModule : CompileTime::PODStruct
		{
			SWInvoke					func	= null;
			Platforms::EShader::type	type	= Platforms::EShader::Unknown;
		};
		using Shaders_t		= FixedSizeArray< ShaderModule, Platforms::EShader::_Count >;

	// variables
		Out< Shaders_t >	result;
	};


	//
	// Get Pipeline Stage
	//
	struct GetSWPipelineStage
	{
	// types
		using SWInvoke		= Platforms::PipelineTemplateDescriptor::ShaderSource::SWInvoke;
		using EShader		= Platforms::EShader;

		struct Stage {
			SWInvoke		func;
		};

	// variables
		EShader::type	stage;
		Out< Stage >	result;

	// methods
		explicit GetSWPipelineStage (EShader::type stage) : stage{stage} {}
	};


	//
	// Resource Table Forward Message to Resource
	//
	template <typename Msg>
	struct ResourceTableForwardMsg
	{
	// variables
		mutable Msg		message;
		uint			index	= UMax;

	// methods
		explicit ResourceTableForwardMsg (uint idx) : index{idx} {}
		
		explicit ResourceTableForwardMsg (uint idx, const Msg &msg) : message{msg}, index{idx} {}

		template <typename ...ArgTypes>
		explicit ResourceTableForwardMsg (uint idx, ArgTypes&& ...args) : message{ FW<ArgTypes>(args)... }, index{idx} {}
	};


	//
	// Sync Client With Device
	//
	struct SyncSWClientWithDevice
	{
	// variables
		Platforms::GpuFenceId		fenceId	= Uninitialized;

	// methods
		explicit SyncSWClientWithDevice (Platforms::GpuFenceId id) : fenceId{id} {}
	};

	
	//
	// Software Renderer Commands
	//
	struct SetSWCommandBufferQueue
	{
	// types
		using Data_t	= Union< CmdBindComputePipeline,
								 CmdDispatch,
								 CmdExecute,
								 CmdBindComputeResourceTable,
								 CmdCopyBuffer,
								 CmdCopyImage,
								 CmdCopyBufferToImage,
								 CmdCopyImageToBuffer,
								 CmdUpdateBuffer,
								 CmdFillBuffer,
								 CmdClearColorImage,
								 CmdPipelineBarrier,
								 CmdPushConstants,
								 CmdPushNamedConstants >;

		using Func_t	= Delegate< void (VariantCRef data, StringCRef file, uint line) >;

		struct Command
		{
		// variables
			Data_t		data;

			DEBUG_ONLY(
				String	file;
				uint	line;
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
		BinaryArray					pushConstData;
	};
	

	//
	// Execute Command Buffer
	//
	struct ExecuteSWCommandBuffer : CompileTime::FastCopyable
	{
	// variables
		Editable< bool  >	completed;
		Editable< usize >	lastCmdIndex;	// to continue executing from this command

	// methods
		ExecuteSWCommandBuffer () : completed{true}, lastCmdIndex{0} {}
	};


	//
	// Present Frame
	//
	struct SWPresent
	{
		Function< void () >		callback;
	};


	//
	// Get Fence
	//
	struct GetSWFence
	{
	// variables
		Platforms::GpuFenceId			id;
		Out< PlatformSW::SWFencePtr >	result;

	// methods
		explicit GetSWFence (Platforms::GpuFenceId id) : id{id} {}
	};


	//
	// Get Event
	//
	struct GetSWEvent
	{
	// variables
		Platforms::GpuEventId			id;
		Out< PlatformSW::SWEventPtr >	result;

	// methods
		explicit GetSWEvent (Platforms::GpuEventId id) : id{id} {}
	};


	//
	// Get Semaphore
	//
	struct GetSWSemaphore
	{
	// variables
		Platforms::GpuSemaphoreId			id;
		Out< PlatformSW::SWSemaphorePtr >	result;

	// methods
		explicit GetSWSemaphore (Platforms::GpuSemaphoreId id) : id{id} {}
	};


}	// GpuMsg
}	// Engine

#endif	// GRAPHICS_API_SOFT
