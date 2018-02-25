// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CApp.h"

bool CApp::_Test_CopyBuffer ()
{
	// generate data
	BinaryArray	data;	data.Resize( 512 );

	FOR( i, data ) {
		data[i] = Random::Int<ubyte>();
	}


	// create resources
	auto	factory	= ms->GlobalSystems()->modulesFactory;

	Message< GpuMsg::CreateFence >	fence_ctor;
	syncManager->Send( fence_ctor );

	ModulePtr	cmd_buffer;
	CHECK_ERR( factory->Create(
					gpuIDs.commandBuffer,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuCommandBuffer{},
					OUT cmd_buffer ) );
	cmdBuilder->Send< ModuleMsg::AttachModule >({ cmd_buffer });

	ModulePtr	src_buffer;
	CHECK_ERR( factory->Create(
					gpuIDs.buffer,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuBuffer{
						BufferDescriptor{ data.Size(), EBufferUsage::bits() | EBufferUsage::TransferSrc },
						EGpuMemory::bits() | EGpuMemory::CoherentWithCPU,
						EMemoryAccess::All
					},
					OUT src_buffer
				) );
	
	ModulePtr	dst_buffer;
	CHECK_ERR( factory->Create(
					gpuIDs.buffer,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuBuffer{
						BufferDescriptor{ data.Size(), EBufferUsage::bits() | EBufferUsage::TransferDst },
						EGpuMemory::bits() | EGpuMemory::CoherentWithCPU,
						EMemoryAccess::All
					},
					OUT dst_buffer
				) );

	ModuleUtils::Initialize({ cmd_buffer, src_buffer, dst_buffer });


	// write data to buffer
	Message< GpuMsg::WriteToGpuMemory >	write_cmd{ data };
	src_buffer->Send( write_cmd );
	CHECK_ERR( *write_cmd->wasWritten == BytesUL(data.Size()) );


	// build command buffer
	cmdBuilder->Send< GpuMsg::CmdBegin >({ cmd_buffer });

	using Region = GpuMsg::CmdCopyBuffer::Region;
	Message< GpuMsg::CmdCopyBuffer >	copy_cmd;

	copy_cmd->srcBuffer = src_buffer;
	copy_cmd->dstBuffer = dst_buffer;
	copy_cmd->regions	= ArrayCRef<Region>{ Region{0_b, 0_b, 128_b}, Region{128_b, 128_b, data.Size()-128_b} };

	cmdBuilder->Send( copy_cmd );
	
	Message< GpuMsg::CmdEnd >	cmd_end;
	cmdBuilder->Send( cmd_end );


	// submit and sync
	gpuThread->Send< GpuMsg::SubmitComputeQueueCommands >({ cmd_end->result.Get(), *fence_ctor->result });

	syncManager->Send< GpuMsg::ClientWaitFence >({ *fence_ctor->result });


	// read
	BinaryArray	dst_data;	dst_data.Resize( data.Count() );

	Message< GpuMsg::ReadFromGpuMemory >	read_cmd{ dst_data };
	dst_buffer->Send( read_cmd );

	CHECK_ERR( data == *read_cmd->result );

	LOG( "CopyBuffer - OK", ELog::Info );
	return true;
}
