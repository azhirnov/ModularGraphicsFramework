// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CApp.h"

bool CApp::_Test_CopyImage2D ()
{
	using Pixel		= ubyte4;

	const uint2		img_dim		{125, 125};
	const uint2		img2_dim	= img_dim * 2;

	const uint3		src_off1	= uint3(0);
	const uint3		dst_off1	= uint3(24, 27, 0);
	const uint3		size1		= uint3(64, 34, 1);

	const uint3		src_off2	= uint3(32, 54, 0);
	const uint3		dst_off2	= uint3(88, 66, 0);
	const uint3		size2		= uint3(83, 51, 1);


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
	
	ModulePtr	dst_image;
	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescriptor{ EImage::Tex2D, uint4(img2_dim), EPixelFormat::RGBA8U, EImageUsage::TransferDst },
						EGpuMemory::CoherentWithCPU },
					OUT dst_image ) );

	ModuleUtils::Initialize({ cmd_buffer, src_image, dst_image });

	
	// generate image data
	Message< GpuMsg::GetImageMemoryLayout >	req_src_layout;
	src_image->Send( req_src_layout );

	BinaryArray		image_data;		image_data.Resize( usize(req_src_layout->result->rowPitch * req_src_layout->result->dimension.y) );

	FOR( i, image_data ) {
		image_data[i] = Random::Int<ubyte>();
	}

	// write data to image
	Message< GpuMsg::WriteToImageMemory >	write_cmd{ image_data, uint3(), uint3(img_dim), req_src_layout->result->rowPitch };
	src_image->Send( write_cmd );
	CHECK_ERR( *write_cmd->wasWritten == BytesUL(image_data.Size()) );


	// build command buffer
	cmdBuilder->Send< GpuMsg::CmdBegin >({ cmd_buffer });

	cmdBuilder->Send< GpuMsg::CmdPipelineBarrier >({ EPipelineStage::Host,
													 EPipelineStage::Transfer,
													 EPipelineAccess::HostWrite,
												     EPipelineAccess::TransferRead,
													 EImageLayout::Preinitialized,
												     EImageLayout::TransferSrcOptimal,
												     src_image,
												     EImageAspect::Color });
	
	cmdBuilder->Send< GpuMsg::CmdPipelineBarrier >({ EPipelineStage::TopOfPipe,
													 EPipelineStage::Transfer,
													 EPipelineAccess::bits(),
													 EPipelineAccess::TransferWrite,
													 EImageLayout::Preinitialized,
												     EImageLayout::TransferDstOptimal,
												     dst_image,
												     EImageAspect::Color });

	using Region = GpuMsg::CmdCopyImage::Region;
	using ImageLayers = GpuMsg::CmdCopyImage::ImageLayers;

	Message< GpuMsg::CmdCopyImage >	copy_cmd;
	copy_cmd->srcImage	= src_image;
	copy_cmd->srcLayout	= EImageLayout::TransferSrcOptimal;
	copy_cmd->dstImage	= dst_image;
	copy_cmd->dstLayout = EImageLayout::TransferDstOptimal;
	copy_cmd->regions	= ArrayCRef<Region>{
							Region{ ImageLayers{ EImageAspect::Color, MipmapLevel(0), ImageLayer(0), 1 },
									src_off1,
									ImageLayers{ EImageAspect::Color, MipmapLevel(0), ImageLayer(0), 1 },
									dst_off1,
									size1 },
							Region{ ImageLayers{ EImageAspect::Color, MipmapLevel(0), ImageLayer(0), 1 },
									src_off2,
									ImageLayers{ EImageAspect::Color, MipmapLevel(0), ImageLayer(0), 1 },
									dst_off2,
									size2 },
							};

	cmdBuilder->Send( copy_cmd );
	
	Message< GpuMsg::CmdEnd >	cmd_end;
	cmdBuilder->Send( cmd_end );


	// submit and sync
	gpuThread->Send< GpuMsg::SubmitComputeQueueCommands >({ cmd_end->result.Get(), *fence_ctor->result });

	syncManager->Send< GpuMsg::ClientWaitFence >({ *fence_ctor->result });


	// read from image
	Message< GpuMsg::GetImageMemoryLayout >	req_dst_layout;
	dst_image->Send( req_dst_layout );

	BinaryArray	dst_data;	dst_data.Resize( usize(req_dst_layout->result->rowPitch * req_dst_layout->result->dimension.y) );

	Message< GpuMsg::ReadFromImageMemory >	read_cmd{ dst_data, uint3(), uint3(img2_dim), req_dst_layout->result->rowPitch };
	dst_image->Send( read_cmd );
	CHECK_ERR( dst_data.Size() == read_cmd->result->Size() );
	

	// compare
	for (uint y = 0; y < size1.y; ++y)
	{
		Pixel const*	src_row = (Pixel const*) (image_data.ptr() + req_src_layout->result->rowPitch * (y + src_off1.y));
		Pixel const*	dst_row = (Pixel const*) (dst_data.ptr() + req_dst_layout->result->rowPitch * (y + dst_off1.y));

		for (uint x = 0; x < size1.x; ++x)
		{
			Pixel	src = src_row[ x + src_off1.x ];
			Pixel	dst = dst_row[ x + dst_off1.x ];

			CHECK_ERR(All( src == dst ));
		}
	}
	
	for (uint y = 0; y < size2.y; ++y)
	{
		Pixel const*	src_row = (Pixel const*) (image_data.ptr() + req_src_layout->result->rowPitch * (y + src_off2.y));
		Pixel const*	dst_row = (Pixel const*) (dst_data.ptr() + req_dst_layout->result->rowPitch * (y + dst_off2.y));

		for (uint x = 0; x < size2.x; ++x)
		{
			Pixel	src = src_row[ x + src_off2.x ];
			Pixel	dst = dst_row[ x + dst_off2.x ];

			CHECK_ERR(All( src == dst ));
		}
	}
	
	LOG( "CopyImage2D - OK", ELog::Info );
	return true;
}
