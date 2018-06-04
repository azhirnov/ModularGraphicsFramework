// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "SApp.h"

bool SApp::_Test_BufferSharing ()
{
	// generate data
	BinaryArray	data;	data.Resize( 512 );

	FOR( i, data ) {
		data[i] = Random::Int<ubyte>();
	}


	// create resources
	auto	factory	= ms->GlobalSystems()->modulesFactory;


	ModulePtr	src_buffer;
	CHECK_ERR( factory->Create(
					gpuIDs.buffer,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuBuffer{
						BufferDescriptor{ data.Size(), EBufferUsage::TransferSrc },
						EGpuMemory::CoherentWithCPU },
					OUT src_buffer ) );
	
	ModulePtr	dst_buffer;
	CHECK_ERR( factory->Create(
					gpuIDs.buffer,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuBuffer{
						BufferDescriptor{ data.Size(), EBufferUsage::TransferDst },
						EGpuMemory::CoherentWithCPU },
					OUT dst_buffer ) );
	
	
	Message< GpuMsg::CreateFence >	fence2_ctor;
	sharedGpu.syncManager->Send( fence2_ctor );

	ModulePtr	cmd_buffer2;
	CHECK_ERR( factory->Create(
					sharedGpu.gpuIDs.commandBuffer,
					sharedGpu.gpuThread->GlobalSystems(),
					CreateInfo::GpuCommandBuffer{},
					OUT cmd_buffer2 ) );
	sharedGpu.cmdBuilder->Send< ModuleMsg::AttachModule >({ cmd_buffer2 });

	ModulePtr	src_buffer2;
	CHECK_ERR( factory->Create(
					sharedGpu.gpuIDs.buffer,
					sharedGpu.gpuThread->GlobalSystems(),
					CreateInfo::GpuBuffer{},
					OUT src_buffer2 ) );
	
	ModulePtr	dst_buffer2;
	CHECK_ERR( factory->Create(
					sharedGpu.gpuIDs.buffer,
					sharedGpu.gpuThread->GlobalSystems(),
					CreateInfo::GpuBuffer{},
					OUT dst_buffer2 ) );

	src_buffer2->Send< ModuleMsg::AttachModule >({ "shared", src_buffer });
	dst_buffer2->Send< ModuleMsg::AttachModule >({ "shared", dst_buffer });
	
	ModuleUtils::Initialize({ cmd_buffer2, src_buffer2, dst_buffer2 });
	ModuleUtils::Initialize({ src_buffer, dst_buffer });


	// write data to buffer
	Message< GpuMsg::WriteToGpuMemory >	write_cmd{ data };
	src_buffer->Send( write_cmd );
	CHECK_ERR( *write_cmd->wasWritten == BytesUL(data.Size()) );


	// build command buffer
	sharedGpu.cmdBuilder->Send< GpuMsg::CmdBegin >({ cmd_buffer2 });

	using Region = GpuMsg::CmdCopyBuffer::Region;
	Message< GpuMsg::CmdCopyBuffer >	copy_cmd;

	copy_cmd->srcBuffer = src_buffer2;
	copy_cmd->dstBuffer = dst_buffer2;
	copy_cmd->regions	= ArrayCRef<Region>{ Region{0_b, 0_b, 128_b}, Region{128_b, 128_b, data.Size()-128_b} };

	sharedGpu.cmdBuilder->Send( copy_cmd );
	
	Message< GpuMsg::CmdEnd >	cmd_end;
	sharedGpu.cmdBuilder->Send( cmd_end );


	// submit and sync
	sharedGpu.gpuThread->Send< GpuMsg::SubmitComputeQueueCommands >({ cmd_end->result.Get(), *fence2_ctor->result });

	sharedGpu.syncManager->Send< GpuMsg::ClientWaitFence >({ *fence2_ctor->result });


	// read
	BinaryArray	dst_data;	dst_data.Resize( data.Count() );

	Message< GpuMsg::ReadFromGpuMemory >	read_cmd{ dst_data };
	dst_buffer2->Send( read_cmd );

	CHECK_ERR( data == *read_cmd->result );

	LOG( "BufferSharing - OK", ELog::Info );
	return true;
}
