// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CApp.h"

bool CApp::_Test_CopyBufferToImage2D ()
{
	using ImageLayers	= GpuMsg::CmdCopyBufferToImage::ImageLayers;
	using Pixel			= ubyte4;

	const uint2		img_dim		{125, 125};
	const uint2		img2_dim	= img_dim * 2;

	const uint3		src_off1	= uint3(0);
	const uint3		dst_off1	= uint3(24, 27, 0);
	const uint3		size1		= uint3(64, 34, 1);

	const uint3		src_off2	= uint3(32, 54, 0);
	const uint3		dst_off2	= uint3(88, 66, 0);
	const uint3		size2		= uint3(83, 51, 1);

	const BytesU	align_bytes	= 4_b;
	const BytesUL	row_pitch	= BytesUL(AlignToLarge( img_dim.x * sizeof(Pixel), align_bytes ));
	const uint		row_length	= uint(row_pitch / sizeof(Pixel));

	// check alignment
	CHECK_ERR( row_pitch == BytesUL(row_length * sizeof(Pixel)) );

	// generate buffer data
	BinaryArray		buf_data;	buf_data.Resize( usize(row_pitch * img_dim.y) );

	FOR( i, buf_data ) {
		buf_data[i] = Random::Int<ubyte>();
	}

	// create resources
	auto	factory	= ms->GlobalSystems()->modulesFactory;

	GpuMsg::CreateFence		fence_ctor;
	syncManager->Send( fence_ctor );

	ModulePtr	cmd_buffer;
	CHECK_ERR( factory->Create(
					gpuIDs.commandBuffer,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuCommandBuffer{},
					OUT cmd_buffer ) );
	cmdBuilder->Send( ModuleMsg::AttachModule{ cmd_buffer });

	ModulePtr	src_buffer;
	CHECK_ERR( factory->Create(
					gpuIDs.buffer,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuBuffer{
						BufferDescription{ buf_data.Size(), EBufferUsage::TransferSrc },
						EGpuMemory::CoherentWithCPU },
					OUT src_buffer ) );
	
	ModulePtr	dst_image;
	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescription{ EImage::Tex2D, uint4(img2_dim), EPixelFormat::RGBA8U, EImageUsage::TransferDst },
						EGpuMemory::CoherentWithCPU },
					OUT dst_image ) );

	ModuleUtils::Initialize({ cmd_buffer, src_buffer, dst_image });


	// write data to image
	GpuMsg::WriteToGpuMemory	write_cmd{ buf_data };
	src_buffer->Send( write_cmd );
	CHECK_ERR( *write_cmd.wasWritten == BytesUL(buf_data.Size()) );


	// build command buffer
	cmdBuilder->Send( GpuMsg::CmdBegin{ cmd_buffer });

	cmdBuilder->Send( GpuMsg::CmdPipelineBarrier{ EPipelineStage::Host, EPipelineStage::Transfer }
						.AddBuffer({ src_buffer,
									 EPipelineAccess::HostWrite,
									 EPipelineAccess::TransferRead,
									 BytesUL(0), BytesUL(buf_data.Size()) }) );
	
	cmdBuilder->Send( GpuMsg::CmdPipelineBarrier{ EPipelineStage::TopOfPipe, EPipelineStage::Transfer }
						.AddImage({	dst_image,
									EPipelineAccess::bits(),
									EPipelineAccess::TransferWrite,
									EImageLayout::Undefined,
									EImageLayout::TransferDstOptimal,
									EImageAspect::Color }) );

	cmdBuilder->Send( GpuMsg::CmdCopyBufferToImage{ src_buffer, dst_image, EImageLayout::TransferDstOptimal }
						.AddRegion( BytesUL(src_off1.x * sizeof(Pixel) + src_off1.y * row_pitch),
									row_length, img_dim.y,
									ImageLayers{EImageAspect::Color, MipmapLevel(0), ImageLayer(0), 1},
									dst_off1,
									size1 )
						.AddRegion( BytesUL(src_off2.x * sizeof(Pixel) + src_off2.y * row_pitch),
									row_length, img_dim.y,
									ImageLayers{EImageAspect::Color, MipmapLevel(0), ImageLayer(0), 1},
									dst_off2,
									size2 ));
	
	GpuMsg::CmdEnd	cmd_end;
	cmdBuilder->Send( cmd_end );


	// submit and sync
	gpuThread->Send( GpuMsg::SubmitComputeQueueCommands{ *cmd_end.result, *fence_ctor.result });

	syncManager->Send( GpuMsg::ClientWaitFence{ *fence_ctor.result });


	// read image
	GpuMsg::GetImageMemoryLayout	req_dst_layout;
	dst_image->Send( req_dst_layout );

	BinaryArray	dst_data;	dst_data.Resize( usize(req_dst_layout.result->rowPitch * req_dst_layout.result->dimension.y) );

	GpuMsg::ReadFromImageMemory		read_cmd{ dst_data, uint3(), uint3(img2_dim), req_dst_layout.result->rowPitch };
	dst_image->Send( read_cmd );
	CHECK_ERR( dst_data.Size() == read_cmd.result->Size() );
	

	// compare
	for (uint y = 0; y < size1.y; ++y)
	{
		Pixel const*	src_row = Cast<Pixel const *>(buf_data.ptr() + (y + src_off1.y) * row_pitch);
		Pixel const*	dst_row = Cast<Pixel const *>(dst_data.ptr() + req_dst_layout.result->rowPitch * (y + dst_off1.y));

		for (uint x = 0; x < size1.x; ++x)
		{
			Pixel	src = src_row[ x + src_off1.x ];
			Pixel	dst = dst_row[ x + dst_off1.x ];

			CHECK_ERR(All( src == dst ));
		}
	}
	
	for (uint y = 0; y < size2.y; ++y)
	{
		Pixel const*	src_row = Cast<Pixel const *>(buf_data.ptr() + (y + src_off2.y) * row_pitch);
		Pixel const*	dst_row = Cast<Pixel const *>(dst_data.ptr() + req_dst_layout.result->rowPitch * (y + dst_off2.y));

		for (uint x = 0; x < size2.x; ++x)
		{
			ubyte4	src = src_row[ x + src_off2.x ];
			ubyte4	dst = dst_row[ x + dst_off2.x ];

			CHECK_ERR(All( src == dst ));
		}
	}
	LOG( "CopyBufferToImage2D - OK", ELog::Info );
	return true;
}
