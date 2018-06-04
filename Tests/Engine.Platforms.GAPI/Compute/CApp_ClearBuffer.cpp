// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CApp.h"

bool CApp::_Test_ClearBuffer ()
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

	ModulePtr	buffer;
	CHECK_ERR( factory->Create(
					gpuIDs.buffer,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuBuffer{
						BufferDescriptor{ data.Size(), EBufferUsage::TransferSrc | EBufferUsage::TransferDst },
						EGpuMemory::CoherentWithCPU },
					OUT buffer ) );

	ModuleUtils::Initialize({ cmd_buffer, buffer });


	// write data to buffer
	Message< GpuMsg::WriteToGpuMemory >	write_cmd{ data };
	buffer->Send( write_cmd );
	CHECK_ERR( *write_cmd->wasWritten == BytesUL(data.Size()) );


	// build command buffer
	const uint	pattern = 0x11223344;

	cmdBuilder->Send< GpuMsg::CmdBegin >({ cmd_buffer });

	cmdBuilder->Send< GpuMsg::CmdFillBuffer >({ buffer, pattern, 256_b, 256_b });
	
	Message< GpuMsg::CmdEnd >	cmd_end;
	cmdBuilder->Send( cmd_end );


	// submit and sync
	gpuThread->Send< GpuMsg::SubmitComputeQueueCommands >({ cmd_end->result.Get(), *fence_ctor->result });

	syncManager->Send< GpuMsg::ClientWaitFence >({ *fence_ctor->result });


	// read
	BinaryArray	dst_data;	dst_data.Resize( data.Count() );

	Message< GpuMsg::ReadFromGpuMemory >	read_cmd{ dst_data };
	buffer->Send( read_cmd );

	CHECK_ERR( data.Size() == read_cmd->result->Size() );

	bool		equals			= true;
	const ubyte	pattern_arr[4]	= { pattern & 0xFF,
									(pattern >> 8) & 0xFF,
									(pattern >> 16) & 0xFF,
									(pattern >> 24) & 0xFF };

	FOR( i, data )
	{
		if ( i < 256 )
			equals &= (data[i] == read_cmd->result->operator[](i));
		else
			equals &= (pattern_arr[i&3] == read_cmd->result->operator[](i));
	}
	CHECK_ERR( equals );

	LOG( "ClearBuffer - OK", ELog::Info );
	return true;
}
