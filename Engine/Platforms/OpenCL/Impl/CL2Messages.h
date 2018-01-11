// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/OpenCL/Impl/CL2Enums.h"
#include "Engine/Platforms/Shared/GPU/CommandBuffer.h"

#if defined( COMPUTE_API_OPENCL )

namespace Engine
{
namespace GpuMsg
{

	//
	// Get Device Info
	//
	struct GetCLDeviceInfo
	{
		struct Info {
			cl::cl_device_id		device		= null;
			cl::cl_context			context		= null;
			cl::cl_command_queue	queue		= null;
		};

		Out< Info >					result;
	};


	//
	// Get GPU Buffer ID
	//
	struct GetCLBufferID
	{
		Out< cl::cl_mem >			result;
	};
	

	//
	// Get GPU Compute Pipeline ID
	//
	struct GetCLComputePipelineID
	{
		struct Pipeline {
			cl::cl_program		progId;
			cl::cl_kernel		kernel;
		};
		Out< Pipeline >		result;
	};


	//
	// GPU Pipeline Resource Table ID (program arguments)
	//
	struct CLPipelineResourceTableApply
	{
		cl::cl_kernel	kernelId = null;
	};

	/*struct GetCLPipelineResourceTableArgs
	{
		struct Arg {
			BinArrayCRef	value;
			uint			index = 0;
		};
		using Args_t	= FixedSizeArray< Arg, 32 >;

		Out< Args_t >	result;
	};*/


	//
	// Get GPU Image ID
	//
	struct GetCLImageID
	{
		Out< cl::cl_mem >			result;
	};


	//
	// Get GPU Shader Module IDs
	//
	struct GetCLShaderModuleIDs
	{
		struct ShaderModule : CompileTime::PODStruct
		{
			StaticString<64>			entry;
			cl::cl_program				id		= null;
			Platforms::EShader::type	type	= Platforms::EShader::Unknown;
		};
		using Shaders_t		= FixedSizeArray< ShaderModule, Platforms::EShader::_Count >;

		Out< Shaders_t >	result;
	};


	//
	// OpenCL Commands
	//
	struct SetCLCommandBufferQueue
	{
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
								 CmdPipelineBarrier >;

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

		ReadOnce< Array<Command> >	commands;
	};


}	// GpuMsg
}	// Engine

#endif	// COMPUTE_API_OPENCL
