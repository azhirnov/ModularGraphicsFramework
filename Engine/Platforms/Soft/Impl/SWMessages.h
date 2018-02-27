// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Soft/Impl/SWEnums.h"
#include "Engine/Platforms/Shared/GPU/Pipeline.h"
#include "Engine/Platforms/Shared/GPU/CommandBuffer.h"
#include "Engine/Platforms/Shared/GPU/Image.h"
#include "Engine/Platforms/Shared/GPU/Sampler.h"

#ifdef GRAPHICS_API_SOFT

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
		using EPixelFormat	= Platforms::EPixelFormat;
		using EMemoryAccess = Platforms::EMemoryAccess;

		struct ImgLayer : CompileTime::FastCopyable
		{
			uint2				dimension;
			void *				memory	= null;	// initialized when binded to memory
			BytesU				size;
			EPixelFormat::type	format	= Uninitialized;
		};

		struct ImgLayers2D : CompileTime::FastCopyable
		{
			using ImgLayers_t = FixedSizeArray< ImgLayer, 14 >;

			ImgLayers_t			mipmaps;
			uint2				dimension;		// must be equal to mipmaps[0].dimension
			BytesU				size;			// summary size of all mipmaps
		};

		struct ImgLayers3D
		{
			using ImgLayers2D_t	= Array< ImgLayers2D >;

			ImgLayers2D_t		layers;
			uint3				dimension;		// must be equal to uint3( layers[0].dimension, ... )
			BytesU				size;			// summary size of all layers
			BytesU				align	= 4_b;	// base align
			EMemoryAccess::bits	access;			// access flags

			ImgLayers3D (GX_DEFCTOR) {}
		};

		// image1D:  { layers[1], dim(w,1,1) }, { mipmaps[], dim(w,1) }
		// image2D:  { layers[1], dim(w,h,1) }, { mipmaps[], dim(w,h) }
		// image2DA: { layers[a], dim(w,h,1) }, { mipmaps[], dim(w,h) }
		// image3D:  { layers[d], dim(w,h,d) }, { mipmaps[], dim(w,h) }

	// variables
		Out< ImgLayers3D >	result;
	};
	

	//
	// Get Image View Memory Layout
	//
	struct GetSWImageViewMemoryLayout
	{
	// types
		using ImgLayers3D = GetSWImageMemoryLayout::ImgLayers3D;

	// variables
		Platforms::ImageViewDescriptor	viewDescr;
		Out< ImgLayers3D >				result;

	// methods
		GetSWImageViewMemoryLayout () {}
		explicit GetSWImageViewMemoryLayout (const Platforms::ImageViewDescriptor &descr) : viewDescr{descr} {}
	};


	//
	// Get Image Memory Requirements
	//
	struct GetSWImageMemoryRequirements
	{
	// types
		struct MemReq
		{
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
		GetSWTextureMemoryLayout () {}
		explicit GetSWTextureMemoryLayout (const Platforms::ImageViewDescriptor &descr) : GetSWImageViewMemoryLayout{descr} {}
	};


	//
	// Get Buffer Memory Layout
	//
	struct GetSWBufferMemoryLayout
	{
	// types
		using EMemoryAccess = Platforms::EMemoryAccess;

		struct Data {
			BinArrayRef			memory;
			EMemoryAccess::bits	access;
			BytesU				align;
		};

	// variables
		Out< Data >		result;
	};


	//
	// Get Buffer Memory Requirements
	//
	struct GetSWBufferMemoryRequirements
	{
	// types
		struct MemReq
		{
			BytesU		size;
			BytesU		align;
		};

	// variables
		Out< MemReq >	result;
	};


	//
	// Get Memory Data
	//
	struct GetSWMemoryData
	{
		Out< BinArrayRef >	result;
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
		Msg		message;
		uint	index	= UMax;

	// methods
		ResourceTableForwardMsg (const Msg &msg, uint idx) : message{msg}, index{idx} {}
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
