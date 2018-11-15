// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CApp.h"
#include "Pipelines/all_pipelines.h"

bool CApp::_Test_Image2DNearestFilter ()
{
	using Pixel		= ubyte4;
	
	const uint2		src_img_dim {128, 128};
	const uint2		dst_img_dim = src_img_dim / 2;


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

	ModulePtr	staging_src_image;
	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescription{ EImage::Tex2D, uint4(src_img_dim), EPixelFormat::RGBA8_UNorm, EImageUsage::TransferSrc },
						EGpuMemory::CoherentWithCPU },
					OUT staging_src_image ) );
	
	ModulePtr	staging_dst_image;
	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescription{ EImage::Tex2D, uint4(dst_img_dim), EPixelFormat::RGBA8_UNorm, EImageUsage::TransferDst },
						EGpuMemory::CoherentWithCPU },
					OUT staging_dst_image ) );

	ModulePtr	src_image;
	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescription{ EImage::Tex2D, uint4(src_img_dim), EPixelFormat::RGBA8_UNorm, EImageUsage::TransferDst | EImageUsage::Sampled },
						EGpuMemory::LocalInGPU | EGpuMemory::Dedicated,
						EMemoryAccess::GpuReadWrite },
					OUT src_image ) );
	
	ModulePtr	dst_image;
	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescription{ EImage::Tex2D, uint4(dst_img_dim), EPixelFormat::RGBA8_UNorm, EImageUsage::TransferDst | EImageUsage::TransferSrc | EImageUsage::Storage },
						EGpuMemory::LocalInGPU | EGpuMemory::Dedicated,
						EMemoryAccess::GpuReadWrite },
					OUT dst_image ) );

	ModulePtr	sampler;
	CHECK_ERR( factory->Create(
					gpuIDs.sampler,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuSampler{
						SamplerDescription::Builder()
						.SetFilter( EFilter::MinMagMipNearest )
						.SetAddressMode( EAddressMode::ClampToBorder )
						.Finish()
					},
					OUT sampler ) );

	CreateInfo::PipelineTemplate	pt_ci;
	Pipelines::Create_image2dnearestfilter( OUT pt_ci.descr );
	
	ModulePtr	pipeline_template;
	CHECK_ERR( factory->Create(
					PipelineTemplateModuleID,
					gpuThread->GlobalSystems(),
					pt_ci,
					OUT pipeline_template ) );
	ModuleUtils::Initialize({ pipeline_template });

	GpuMsg::CreateComputePipeline	cppl_ctor{ gpuIDs.pipeline, gpuThread };
	pipeline_template->Send( cppl_ctor );

	ModulePtr	pipeline	= *cppl_ctor.result;
	ModulePtr	resource_table;
	CHECK_ERR( factory->Create(
					gpuIDs.resourceTable,
					gpuThread->GlobalSystems(),
					CreateInfo::PipelineResourceTable{},
					OUT resource_table ) );
	
	resource_table->Send( ModuleMsg::AttachModule{ "pipeline", pipeline });
	resource_table->Send( GpuMsg::PipelineAttachTexture{ "un_SrcTexture", src_image, sampler, EImageLayout::ShaderReadOnlyOptimal });
	resource_table->Send( ModuleMsg::AttachModule{ "un_DstImage", dst_image });

	ModuleUtils::Initialize({ cmd_buffer, staging_src_image, staging_dst_image, src_image, dst_image, sampler, pipeline, resource_table });

	
	// generate image data
	GpuMsg::GetImageMemoryLayout	req_src_layout;
	staging_src_image->Send( req_src_layout );
	
	BinaryArray		image_data;		image_data.Resize( usize(req_src_layout.result->rowPitch * req_src_layout.result->dimension.y) );

	FOR( i, image_data ) {
		image_data[i] = Random::Int<ubyte>();
	}

	// write data to staging image
	GpuMsg::WriteToImageMemory	write_cmd{ image_data, uint3(), uint3(src_img_dim), req_src_layout.result->rowPitch };
	staging_src_image->Send( write_cmd );
	CHECK_ERR( *write_cmd.wasWritten == image_data.Size() );


	// build command buffer
	cmdBuilder->Send( GpuMsg::CmdBegin{ cmd_buffer });
	
	// copy image to gpu
	{
		cmdBuilder->Send( GpuMsg::CmdPipelineBarrier{ EPipelineStage::Host, EPipelineStage::Transfer }
							.AddImage({	staging_src_image,
										EPipelineAccess::HostWrite,
										EPipelineAccess::TransferRead,
										EImageLayout::Preinitialized,
										EImageLayout::TransferSrcOptimal,
										EImageAspect::Color }) );
	
		cmdBuilder->Send( GpuMsg::CmdPipelineBarrier{ EPipelineStage::TopOfPipe, EPipelineStage::Transfer }
							.AddImage({	src_image,
										EPipelineAccess::bits(),
										EPipelineAccess::TransferWrite,
										EImageLayout::Undefined,
										EImageLayout::TransferDstOptimal,
										EImageAspect::Color }) );
	
		cmdBuilder->Send( GpuMsg::CmdCopyImage{}.SetSource( staging_src_image, EImageLayout::TransferSrcOptimal )
												.SetDestination( src_image, EImageLayout::TransferDstOptimal )
												.AddRegion( ImageRange{ EImageAspect::Color, 0_mipmap, 0_layer, 1 }, uint3(),
															ImageRange{ EImageAspect::Color, 0_mipmap, 0_layer, 1 }, uint3(),
															uint3(src_img_dim, 1) ));
	}

	// run pipeline
	{
		cmdBuilder->Send( GpuMsg::CmdPipelineBarrier{ EPipelineStage::Transfer, EPipelineStage::ComputeShader }
							.AddImage({	src_image,
										EPipelineAccess::TransferWrite,
										EPipelineAccess::ShaderRead,
										EImageLayout::TransferDstOptimal,
										EImageLayout::ShaderReadOnlyOptimal,
										EImageAspect::Color }) );

		cmdBuilder->Send( GpuMsg::CmdPipelineBarrier{ EPipelineStage::TopOfPipe, EPipelineStage::ComputeShader }
							.AddImage({	dst_image,
										EPipelineAccess::bits(),
										EPipelineAccess::ShaderWrite,
										EImageLayout::Undefined,
										EImageLayout::General,
										EImageAspect::Color }) );

		cmdBuilder->Send( GpuMsg::CmdBindComputePipeline{ pipeline });
		cmdBuilder->Send( GpuMsg::CmdBindComputeResourceTable{ resource_table });
		cmdBuilder->Send( GpuMsg::CmdDispatch{ uint3(dst_img_dim, 1) });
	}

	// copy image back to cpu
	{
		cmdBuilder->Send( GpuMsg::CmdPipelineBarrier{ EPipelineStage::Host, EPipelineStage::Transfer }
							.AddImage({	staging_dst_image,
										EPipelineAccess::bits(),
										EPipelineAccess::TransferWrite,
										EImageLayout::Preinitialized,
										EImageLayout::TransferDstOptimal,
										EImageAspect::Color }) );
		
		cmdBuilder->Send( GpuMsg::CmdCopyImage{}.SetSource( dst_image, EImageLayout::General )
												.SetDestination( staging_dst_image, EImageLayout::TransferDstOptimal )
												.AddRegion( ImageRange{ EImageAspect::Color, 0_mipmap, 0_layer, 1 }, uint3(),
															ImageRange{ EImageAspect::Color, 0_mipmap, 0_layer, 1 }, uint3(),
															uint3(dst_img_dim, 1) ));
		
		cmdBuilder->Send( GpuMsg::CmdPipelineBarrier{ EPipelineStage::Transfer, EPipelineStage::Host }
							.AddImage({	staging_dst_image,
										EPipelineAccess::TransferWrite,
										EPipelineAccess::HostRead,
										EImageLayout::TransferDstOptimal,
										EImageLayout::General,
										EImageAspect::Color }) );
	}

	GpuMsg::CmdEnd	cmd_end;
	cmdBuilder->Send( cmd_end );


	// submit and sync
	gpuThread->Send( GpuMsg::SubmitCommands{ *cmd_end.result }.SetFence( *fence_ctor.result ));

	syncManager->Send( GpuMsg::ClientWaitFence{ *fence_ctor.result });


	// read from image
	GpuMsg::GetImageMemoryLayout	req_dst_layout;
	staging_dst_image->Send( req_dst_layout );

	BinaryArray	dst_data;	dst_data.Resize( usize(req_dst_layout.result->rowPitch * req_dst_layout.result->dimension.y) );

	GpuMsg::ReadFromImageMemory		read_cmd{ dst_data, uint3(), uint3(dst_img_dim), req_dst_layout.result->rowPitch };
	staging_dst_image->Send( read_cmd );
	CHECK_ERR( dst_data.Size() == read_cmd.result->Size() );

	const Pixel  white { 255 };

	for (uint y = 0; y < dst_img_dim.y-1; ++y)
	{
		uint			y1		= y * 2 + (y >= dst_img_dim.y/2 ? 1 : 0);
		Pixel const*	src_row = Cast<Pixel const*>(image_data.ptr() + req_src_layout.result->rowPitch * y1);
		Pixel const*	dst_row = Cast<Pixel const*>(dst_data.ptr() + req_dst_layout.result->rowPitch * y);

		for (uint x = 0; x < dst_img_dim.x-1; ++x)
		{
			uint	x1  = x * 2 + (x >= dst_img_dim.x/2 ? 1 : 0);
			Pixel	src = src_row[x1];
			Pixel	dst = dst_row[x];

			CHECK(All( src == dst ));
		}
	}

	LOG( "Image2DNearestFilter - OK", ELog::Info );
	return true;
}
