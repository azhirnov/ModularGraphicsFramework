// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "SApp.h"

bool SApp::_Test_ImageSharing ()
{
	using ImageLayers	= GpuMsg::CmdCopyImage::ImageLayers;
	using Pixel			= ubyte4;

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

	ModulePtr	src_image;
	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescription{ EImage::Tex2D, uint4(img_dim), EPixelFormat::RGBA8U, EImageUsage::TransferSrc },
						EGpuMemory::CoherentWithCPU },
					OUT src_image ) );
	
	ModulePtr	dst_image;
	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescription{ EImage::Tex2D, uint4(img2_dim), EPixelFormat::RGBA8U, EImageUsage::TransferDst },
						EGpuMemory::CoherentWithCPU },
					OUT dst_image ) );

	
	GpuMsg::CreateFence		fence2_ctor;
	sharedGpu.syncManager->Send( fence2_ctor );

	ModulePtr	cmd_buffer2;
	CHECK_ERR( factory->Create(
					sharedGpu.gpuIDs.commandBuffer,
					sharedGpu.gpuThread->GlobalSystems(),
					CreateInfo::GpuCommandBuffer{},
					OUT cmd_buffer2 ) );
	sharedGpu.cmdBuilder->Send( ModuleMsg::AttachModule{ cmd_buffer2 });
	
	ModulePtr	src_image2;
	CHECK_ERR( factory->Create(
					sharedGpu.gpuIDs.image,
					sharedGpu.gpuThread->GlobalSystems(),
					CreateInfo::GpuImage{},
					OUT src_image2 ) );
	
	ModulePtr	dst_image2;
	CHECK_ERR( factory->Create(
					sharedGpu.gpuIDs.image,
					sharedGpu.gpuThread->GlobalSystems(),
					CreateInfo::GpuImage{},
					OUT dst_image2 ) );

	
	src_image2->Send( ModuleMsg::AttachModule{ "shared", src_image });
	dst_image2->Send( ModuleMsg::AttachModule{ "shared", dst_image });

	ModuleUtils::Initialize({ src_image, dst_image });
	ModuleUtils::Initialize({ cmd_buffer2, src_image2, dst_image2 });
	
	
	// generate image data
	GpuMsg::GetImageMemoryLayout	req_src_layout;
	src_image->Send( req_src_layout );

	BinaryArray		image_data;		image_data.Resize( usize(req_src_layout.result->rowPitch * req_src_layout.result->dimension.y) );

	FOR( i, image_data ) {
		image_data[i] = Random::Int<ubyte>();
	}

	// write data to image
	GpuMsg::WriteToImageMemory		write_cmd{ image_data, uint3(), uint3(img_dim), req_src_layout.result->rowPitch };
	src_image->Send( write_cmd );
	CHECK_ERR( *write_cmd.wasWritten == BytesUL(image_data.Size()) );


	// build command buffer
	sharedGpu.cmdBuilder->Send( GpuMsg::CmdBegin{ cmd_buffer2 });

	sharedGpu.cmdBuilder->Send( GpuMsg::CmdPipelineBarrier{	EPipelineStage::Host, EPipelineStage::Transfer }
								.AddImage({	src_image2,
											EPipelineAccess::HostWrite,
											EPipelineAccess::TransferRead,
											EImageLayout::Preinitialized,
											EImageLayout::TransferSrcOptimal,
											EImageAspect::Color }) );
	
	sharedGpu.cmdBuilder->Send( GpuMsg::CmdPipelineBarrier{	EPipelineStage::TopOfPipe, EPipelineStage::Transfer }
								.AddImage({ dst_image2,
											EPipelineAccess::bits(),
											EPipelineAccess::TransferWrite,
											EImageLayout::Preinitialized,
											EImageLayout::TransferDstOptimal,
											EImageAspect::Color }) );

	sharedGpu.cmdBuilder->Send( GpuMsg::CmdCopyImage{}.SetSource( src_image2, EImageLayout::TransferSrcOptimal )
													.SetDestination( dst_image2, EImageLayout::TransferDstOptimal )
													.AddRegion( ImageLayers{ EImageAspect::Color, MipmapLevel(0), ImageLayer(0), 1 },
																src_off1,
																ImageLayers{ EImageAspect::Color, MipmapLevel(0), ImageLayer(0), 1 },
																dst_off1,
																size1 )
													.AddRegion( ImageLayers{ EImageAspect::Color, MipmapLevel(0), ImageLayer(0), 1 },
																src_off2,
																ImageLayers{ EImageAspect::Color, MipmapLevel(0), ImageLayer(0), 1 },
																dst_off2,
																size2 ));
	
	GpuMsg::CmdEnd	cmd_end;
	sharedGpu.cmdBuilder->Send( cmd_end );


	// submit and sync
	sharedGpu.gpuThread->Send( GpuMsg::SubmitComputeQueueCommands{ *cmd_end.result, *fence2_ctor.result });

	sharedGpu.syncManager->Send( GpuMsg::ClientWaitFence{ *fence2_ctor.result });

	
	// read from image
	GpuMsg::GetImageMemoryLayout	req_dst_layout;
	dst_image->Send( req_dst_layout );

	BinaryArray	dst_data;	dst_data.Resize( usize(req_dst_layout.result->rowPitch * req_dst_layout.result->dimension.y) );

	GpuMsg::ReadFromImageMemory		read_cmd{ dst_data, uint3(), uint3(img2_dim), req_dst_layout.result->rowPitch };
	dst_image->Send( read_cmd );
	CHECK_ERR( dst_data.Size() == read_cmd.result->Size() );
	

	// compare
	for (uint y = 0; y < size1.y; ++y)
	{
		Pixel const*	src_row = (Pixel const*) (image_data.ptr() + req_src_layout.result->rowPitch * (y + src_off1.y));
		Pixel const*	dst_row = (Pixel const*) (dst_data.ptr() + req_dst_layout.result->rowPitch * (y + dst_off1.y));

		for (uint x = 0; x < size1.x; ++x)
		{
			Pixel	src = src_row[ x + src_off1.x ];
			Pixel	dst = dst_row[ x + dst_off1.x ];

			CHECK_ERR(All( src == dst ));
		}
	}
	
	for (uint y = 0; y < size2.y; ++y)
	{
		Pixel const*	src_row = (Pixel const*) (image_data.ptr() + req_src_layout.result->rowPitch * (y + src_off2.y));
		Pixel const*	dst_row = (Pixel const*) (dst_data.ptr() + req_dst_layout.result->rowPitch * (y + dst_off2.y));

		for (uint x = 0; x < size2.x; ++x)
		{
			Pixel	src = src_row[ x + src_off2.x ];
			Pixel	dst = dst_row[ x + dst_off2.x ];

			CHECK_ERR(All( src == dst ));
		}
	}

	LOG( "ImageSharing - OK", ELog::Info );
	return true;
}
