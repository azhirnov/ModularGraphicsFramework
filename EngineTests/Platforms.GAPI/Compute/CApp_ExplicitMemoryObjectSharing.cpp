// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
*/

#include "CApp.h"
#include "Pipelines/all_pipelines.h"

bool CApp::_Test_ExplicitMemoryObjectSharing ()
{
	using ImageLayers	= GpuMsg::CmdCopyBufferToImage::ImageLayers;
	using Pixel			= uint;

	using MemoryMsg_t	= ModuleMsg::MessageListFrom< GpuMsg::GetGpuMemoryDescription >;
	using MemoryEvent_t	= ModuleMsg::MessageListFrom< GpuMsg::OnMemoryBindingChanged >;


	if ( not gpuThread->Request( GpuMsg::GetDeviceProperties{} ).explicitMemoryObjects )
		return true;


	const uint2		img_dim		{64, 64};

	const BytesU	align_bytes	= 4_b;
	const BytesUL	row_pitch	= BytesUL(AlignToLarge( img_dim.x * sizeof(Pixel), align_bytes ));
	const uint		row_length	= uint(row_pitch / sizeof(Pixel));

	const BytesUL	data_size	= row_pitch * img_dim.y;
	
	// check alignment
	CHECK_ERR( row_pitch == BytesUL(row_length * sizeof(Pixel)) );
	
	// generate buffer data
	Array<Pixel>	buf_data;	buf_data.Resize( usize(row_length * img_dim.y) );

	for (uint y = 0; y < img_dim.y; ++y)
	for (uint x = 0; x < img_dim.x; ++x)
	{
		buf_data[y * row_length + x] = x | (y << 16);  //(x & 3) + (y & 3) * 4;
	}


	// create resources
	auto	factory	= ms->GlobalSystems()->modulesFactory;

	ModulePtr	src_buffer;
	CHECK_ERR( factory->Create(
					gpuIDs.buffer,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuBuffer{
						BufferDescription{ data_size, EBufferUsage::TransferSrc | EBufferUsage::TransferDst },
						EGpuMemory::CoherentWithCPU },
					OUT src_buffer ));
	
	ModulePtr	dst_image;
	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuImage{ ImageDescription{ EImage::Tex2D, uint4(img_dim), EPixelFormat::R32U, EImageUsage::TransferDst | EImageUsage::ColorAttachment }},
					OUT dst_image ));

	ModulePtr	shared_buffer;
	CHECK_ERR( factory->Create(
					gpuIDs.buffer,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuBuffer{ BufferDescription{ data_size, EBufferUsage::TransferSrc }},
					OUT shared_buffer ));

	ModulePtr	shared_mem;
	CHECK_ERR( factory->Create(
					gpuIDs.memory,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuMemory{ EGpuMemory::LocalInGPU, EMemoryAccess::GpuReadWrite },
					OUT shared_mem ));
	
	dst_image->Send( ModuleMsg::AttachModule{ "mem", shared_mem });
	shared_buffer->Send( ModuleMsg::AttachModule{ "mem", shared_mem });

	ModuleUtils::Initialize({ src_buffer, dst_image, shared_buffer });
	

	// write data to buffer
	GpuMsg::WriteToGpuMemory	write_cmd{ BinArrayCRef::From(buf_data) };
	src_buffer->Send( write_cmd );
	CHECK_ERR( *write_cmd.wasWritten == BytesUL(buf_data.Size()) );


	// copy from buffer to image
	{
		cmdBuilder->Send( GpuMsg::CmdBegin{} );

		cmdBuilder->Send( GpuMsg::CmdPipelineBarrier{ EPipelineStage::Host, EPipelineStage::Transfer }
							.AddBuffer({ src_buffer,
										 EPipelineAccess::HostWrite,
										 EPipelineAccess::TransferRead,
										 BytesUL(0), data_size }) );
	
		cmdBuilder->Send( GpuMsg::CmdPipelineBarrier{ EPipelineStage::TopOfPipe, EPipelineStage::Transfer }
							.AddImage({	dst_image,
										EPipelineAccess::bits(),
										EPipelineAccess::TransferWrite,
										EImageLayout::Undefined,
										EImageLayout::TransferDstOptimal,
										EImageAspect::Color }) );

		cmdBuilder->Send( GpuMsg::CmdCopyBufferToImage{ src_buffer, dst_image, EImageLayout::TransferDstOptimal }
							.AddRegion( BytesUL(0),
										row_length, img_dim.y,
										ImageLayers{EImageAspect::Color, MipmapLevel(0), ImageLayer(0), 1},
										uint3(),
										uint3(img_dim, 0) ));
	
		cmdBuilder->Send( GpuMsg::CmdPipelineBarrier{ EPipelineStage::Transfer, EPipelineStage::BottomOfPipe }
							.AddImage({	dst_image,
										EPipelineAccess::TransferWrite,
										EPipelineAccess::ColorAttachment,
										EImageLayout::TransferDstOptimal,
										EImageLayout::General,
										EImageAspect::Color }) );

		ModulePtr	cmd_buffer	= cmdBuilder->Request( GpuMsg::CmdEnd{} );
		auto		fence		= syncManager->Request( GpuMsg::CreateFence{} );
		
		gpuThread->Send( GpuMsg::SubmitComputeQueueCommands{ cmd_buffer }.SetFence( fence ));
		syncManager->Send( GpuMsg::ClientWaitFence{ fence });
	}


	// copy buffer to buffer
	{
		cmdBuilder->Send( GpuMsg::CmdBegin{} );

		cmdBuilder->Send( GpuMsg::CmdCopyBuffer{ shared_buffer, src_buffer }.AddRegion( 0_b, 0_b, BytesU(data_size) ));

		ModulePtr	cmd_buffer	= cmdBuilder->Request( GpuMsg::CmdEnd{} );
		auto		fence		= syncManager->Request( GpuMsg::CreateFence{} );
		
		gpuThread->Send( GpuMsg::SubmitComputeQueueCommands{ cmd_buffer }.SetFence( fence ));
		syncManager->Send( GpuMsg::ClientWaitFence{ fence });
	}
	

	// read
	BinaryArray		dst_data;	dst_data.Resize( usize(data_size) );

	ArrayCRef<uint>	img_data	= ArrayCRef<uint>::From( src_buffer->Request( GpuMsg::ReadFromGpuMemory{ dst_data }) );

	// find block size
	uint2	max_coord;
	
	FOR( i, img_data )
	{
		uint2	coord = uint2(img_data[i] & 0xFFFF, img_data[i] >> 16) + 1;

		if ( max_coord.x == max_coord.y and (coord.x > max_coord.x or coord.y > max_coord.y) and i > 1 and max_coord.x * max_coord.y == i )
		{
			break;
		}

		max_coord = Max( max_coord, coord );
	}

	// get pattern
	const usize		block_size = max_coord.x * max_coord.y;

	if ( block_size != img_data.Count() )
	{
		String	str = "Image z-order:\n";

		for (usize i = 0; i < block_size and i < img_data.Count(); ++i)
		{
			uint2	coord = uint2(img_data[i] & 0xFFFF, img_data[i] >> 16);

			str << '[' << coord.x <<',' << coord.y << ']' << (i and ((i+1) % max_coord.x == 0) ? '\n' : ' ');
		}

		LOG( str, ELog::Info );
	}

	return true;
}
