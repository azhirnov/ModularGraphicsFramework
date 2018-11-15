// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "Engine/Platforms/OpenCL/120/CL1Enums.h"
#include "Engine/Platforms/Public/GPU/CommandBuffer.h"
#include "Engine/Platforms/Public/GPU/Thread.h"

namespace Engine
{
namespace GpuMsg
{

	//
	// Get Device Info
	//
	struct GetCLDeviceInfo : _MsgBase_
	{
	// types
		struct Info {
			cl::cl_device_id	device		= null;
			cl::cl_context		context		= null;

			Info (cl::cl_device_id dev, cl::cl_context ctx) : device{dev}, context{ctx} {}
		};

	// variables
		Out< Info >				result;
	};


	//
	// Get Buffer ID
	//
	struct GetCLBufferID : _MsgBase_
	{
	// types
		using ESharing	= PlatformCL::ESharing::type;

		struct Data {
			cl::cl_mem		id			= null;
			ESharing		sharing		= ESharing::None;

			Data () {}
			Data (cl::cl_mem id, ESharing sharing) : id{id}, sharing{sharing} {}
		};

	// variables
		Out< Data >				result;
	};


	//
	// Create Sub Buffer
	//
	struct CreateCLSubBuffer : GetCLBufferID
	{
	// variables
		BytesU					offset;
		BytesU					size;

	// methods
		CreateCLSubBuffer (BytesU off, BytesU size) : offset{off}, size{size} {};
	};


	//
	// Get Image ID
	//
	struct GetCLImageID : GetCLBufferID
	{
	};
	

	//
	// Get Sampler ID
	//
	struct GetCLSamplerID : _MsgBase_
	{
		Out< cl::cl_sampler >	result;
	};


	//
	// Get Compute Pipeline ID
	//
	struct GetCLComputePipelineID : _MsgBase_
	{
	// types
		struct Pipeline {
			cl::cl_program		progId	= null;
			cl::cl_kernel		kernel	= null;

			Pipeline () {}
			Pipeline (cl::cl_program prog, cl::cl_kernel kernel) : progId{prog}, kernel{kernel} {}
		};

	// variables
		Out< Pipeline >		result;
	};


	//
	// Pipeline Resource Table ID (program arguments)
	//
	struct CLBindPipelineResourceTable : _MsgBase_
	{
	// variables
		cl::cl_kernel	kernelId = null;

	// methods
		explicit CLBindPipelineResourceTable (cl::cl_kernel kernel) : kernelId{kernel} {}
	};


	//
	// Get Shader Module IDs
	//
	struct GetCLShaderModuleIDs : _MsgBase_
	{
	// types
		struct ShaderModule : CompileTime::PODStruct
		{
			StaticString<64>			entry;
			cl::cl_program				id		= null;
			Platforms::EShader::type	type	= Platforms::EShader::Unknown;
		};
		using Shaders_t		= FixedSizeArray< ShaderModule, Platforms::EShader::_Count >;
		
	// variables
		Out< Shaders_t >	result;
	};


	//
	// Fence Sync
	//
	struct CLFenceSync : _MsgBase_
	{
	// variables
		Platforms::GpuFenceId		fenceId;
		Out_opt< cl::cl_event >		result;
		
	// methods
		explicit CLFenceSync (Platforms::GpuFenceId id) : fenceId{id} {}
	};


	//
	// Get Event
	//
	struct GetCLEvent : _MsgBase_
	{
	// types
		using Event_t	= Platforms::GpuEventId;

	// variables
		Event_t					eventId;
		Out< cl::cl_event >		result;

	// methods
		explicit GetCLEvent (Event_t id) : eventId(id) {}
	};


	//
	// Semaphore
	//
	struct CLSemaphoreEnqueue : _MsgBase_
	{
	// variables
		Platforms::GpuSemaphoreId	semId;
		Out_opt< cl::cl_event >		result;
		
	// methods
		explicit CLSemaphoreEnqueue (Platforms::GpuSemaphoreId id) : semId{id} {}
	};


	struct WaitCLSemaphore : _MsgBase_
	{
	// types
		using Semaphores_t	= FixedSizeArray< Platforms::GpuSemaphoreId, GpuMsg::SubmitCommands::WaitSemaphores_t::MemoryContainer_t::SIZE + 4 >;

	// variables
		Semaphores_t		semaphores;

	// methods
		WaitCLSemaphore () {}
		explicit WaitCLSemaphore (Platforms::GpuSemaphoreId sem) { semaphores.PushBack( sem ); }
	};


	//
	// Update Buffer Command
	//
	struct CLCmdUpdateBuffer : _MsgBase_
	{
	// variables
		ModulePtr		dstBuffer;
		BytesU			dstOffset;
		BytesU			size;
		BytesU			srcOffset;

	// methods
		CLCmdUpdateBuffer (const CmdUpdateBuffer &cmd, BytesU offset) :
			dstBuffer(cmd.dstBuffer), dstOffset(cmd.dstOffset), size(cmd.data.Size()), srcOffset(offset)
		{}
	};


	//
	// OpenCL Commands
	//
	struct SetCLCommandBufferQueue : _MsgBase_
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
								 CLCmdUpdateBuffer,
								 CmdFillBuffer,
								 CmdClearColorImage,
								 CmdPipelineBarrier,
								 CmdPushConstants,
								 CmdPushNamedConstants,
								 CmdDebugMarker,
								 CmdPushDebugGroup,
								 CmdPopDebugGroup >;

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
		SetCLCommandBufferQueue (Array<Command> &&commands, BinaryArray &&bufferData, BinaryArray &&pushConstData) :
			commands{ RVREF(commands) }, bufferData{ RVREF(bufferData) }, pushConstData{ RVREF(pushConstData) } {}
	};
	

	//
	// Execute CL Command Buffer
	//
	struct ExecuteCLCommandBuffer : _MsgBase_
	{};


}	// GpuMsg
}	// Engine

#endif	// COMPUTE_API_OPENCL
