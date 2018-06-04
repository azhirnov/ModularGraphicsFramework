// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CApp.h"

bool CApp::_Test_CopyImage2DToBuffer ()
{
	const uint2		img_dim		{125, 125};
	const uint2		img2_dim	= img_dim * 2;

	const uint3		src_off1	= uint3(0);
	const uint3		dst_off1	= uint3(24, 27, 0);
	const uint3		size1		= uint3(64, 34, 1);

	const uint3		src_off2	= uint3(32, 54, 0);
	const uint3		dst_off2	= uint3(88, 66, 0);
	const uint3		size2		= uint3(83, 51, 1);
	
	using Pixel		= ubyte4;
	const uint		img_align		= 1;
	const BytesU	buf_align		= 4_b;
	const BytesUL	buf_row_pitch	= BytesUL(AlignToLarge( img2_dim.x * sizeof(Pixel), buf_align ));
	const uint		buf_row_length	= uint(buf_row_pitch / sizeof(Pixel));
	const usize		buf_size		= usize(buf_row_pitch * img2_dim.y);


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

	ModulePtr	src_image;
	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescriptor{ EImage::Tex2D, uint4(img_dim), EPixelFormat::RGBA8U, EImageUsage::TransferSrc },
						EGpuMemory::CoherentWithCPU },
					OUT src_image ) );
	
	ModulePtr	dst_buffer;
	CHECK_ERR( factory->Create(
					gpuIDs.buffer,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuBuffer{
						BufferDescriptor{ BytesU(buf_size), EBufferUsage::TransferDst },
						EGpuMemory::CoherentWithCPU },
					OUT dst_buffer ) );

	ModuleUtils::Initialize({ cmd_buffer, src_image, dst_buffer });

	
	// generate image data
	Message< GpuMsg::GetImageMemoryLayout >	req_layout;
	src_image->Send( req_layout );

	BinaryArray		image_data;		image_data.Resize( usize(req_layout->result->rowPitch * req_layout->result->dimension.y) );

	FOR( i, image_data ) {
		image_data[i] = Random::Int<ubyte>();
	}
	
	// write data to image
	Message< GpuMsg::WriteToImageMemory >	write_cmd{ image_data, uint3(), uint3(img_dim), req_layout->result->rowPitch };
	src_image->Send( write_cmd );
	CHECK_ERR( *write_cmd->wasWritten == BytesUL(image_data.Size()) );


	// build command buffer
	cmdBuilder->Send< GpuMsg::CmdBegin >({ cmd_buffer });

	cmdBuilder->Send< GpuMsg::CmdPipelineBarrier >({ EPipelineStage::Host,
													 EPipelineStage::Transfer,
													 EPipelineAccess::HostWrite,
													 EPipelineAccess::TransferRead,
													 EImageLayout::Undefined,
												     EImageLayout::TransferSrcOptimal,
												     src_image,
												     EImageAspect::Color });
	
	cmdBuilder->Send< GpuMsg::CmdPipelineBarrier >({ EPipelineStage::TopOfPipe,
													 EPipelineStage::Transfer,
													 EPipelineAccess::bits(),
													 EPipelineAccess::TransferWrite,
												     dst_buffer,
												     0_b, BytesU(buf_size) });

	using Region = GpuMsg::CmdCopyImageToBuffer::Region;
	using ImageLayers = GpuMsg::CmdCopyImageToBuffer::ImageLayers;

	Message< GpuMsg::CmdCopyImageToBuffer >	copy_cmd;
	copy_cmd->srcImage	= src_image;
	copy_cmd->srcLayout	= EImageLayout::TransferSrcOptimal;
	copy_cmd->dstBuffer	= dst_buffer;
	copy_cmd->regions	= ArrayCRef<Region>{
							Region{ BytesUL(dst_off1.x * sizeof(Pixel) + dst_off1.y * buf_row_pitch),
									buf_row_length, img2_dim.y,
									ImageLayers{ EImageAspect::Color, MipmapLevel(0), ImageLayer(0), 1 },
									src_off1,
									size1 },
							Region{ BytesUL(dst_off2.x * sizeof(Pixel) + dst_off2.y * buf_row_pitch),
									buf_row_length, img2_dim.y,
									ImageLayers{ EImageAspect::Color, MipmapLevel(0), ImageLayer(0), 1 },
									src_off2,
									size2 },
							};

	cmdBuilder->Send( copy_cmd );
	
	Message< GpuMsg::CmdEnd >	cmd_end;
	cmdBuilder->Send( cmd_end );


	// submit and sync
	gpuThread->Send< GpuMsg::SubmitComputeQueueCommands >({ cmd_end->result.Get(), *fence_ctor->result });

	syncManager->Send< GpuMsg::ClientWaitFence >({ *fence_ctor->result });


	// read
	BinaryArray	dst_data;	dst_data.Resize( buf_size );

	Message< GpuMsg::ReadFromGpuMemory >	read_cmd{ dst_data };
	dst_buffer->Send( read_cmd );
	CHECK_ERR( dst_data.Size() == read_cmd->result->Size() );
	

	// compare
	for (uint y = 0; y < size1.y; ++y)
	{
		Pixel const*	src_row = (Pixel const*) (image_data.ptr() + req_layout->result->rowPitch * (y + src_off1.y));
		Pixel const*	dst_row = (Pixel const*) (dst_data.ptr() + buf_row_pitch * (y + dst_off1.y));

		for (uint x = 0; x < size1.x; ++x)
		{
			Pixel	src = src_row[ x + src_off1.x ];
			Pixel	dst = dst_row[ x + dst_off1.x ];

			CHECK_ERR(All( src == dst ));
		}
	}
	
	for (uint y = 0; y < size2.y; ++y)
	{
		Pixel const*	src_row = (Pixel const*) (image_data.ptr() + req_layout->result->rowPitch * (y + src_off2.y));
		Pixel const*	dst_row = (Pixel const*) (dst_data.ptr() + buf_row_pitch * (y + dst_off2.y));

		for (uint x = 0; x < size2.x; ++x)
		{
			Pixel	src = src_row[ x + src_off2.x ];
			Pixel	dst = dst_row[ x + dst_off2.x ];

			CHECK_ERR(All( src == dst ));
		}
	}
	
	LOG( "CopyImage2DToBuffer - OK", ELog::Info );
	return true;
}
