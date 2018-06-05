// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CApp.h"
#include "Pipelines/all_pipelines.h"

bool CApp::_Test_ConvertFloatImage2D ()
{
	using Region		= GpuMsg::CmdCopyImage::Region;
	using ImageLayers	= GpuMsg::CmdCopyImage::ImageLayers;

	using SrcPixel		= ubyte4;	// normalized
	using DstPixel		= float4;

	const uint2		img_dim {128, 128};

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

	ModulePtr	staging_src_image;
	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescriptor{ EImage::Tex2D, uint4(img_dim), EPixelFormat::RGBA8_UNorm, EImageUsage::TransferSrc },
						EGpuMemory::CoherentWithCPU },
					OUT staging_src_image ) );
	
	ModulePtr	staging_dst_image;
	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescriptor{ EImage::Tex2D, uint4(img_dim), EPixelFormat::RGBA32F, EImageUsage::TransferDst },
						EGpuMemory::CoherentWithCPU },
					OUT staging_dst_image ) );

	ModulePtr	src_image;
	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescriptor{ EImage::Tex2D, uint4(img_dim), EPixelFormat::RGBA8_UNorm, EImageUsage::TransferDst | EImageUsage::Storage },
						EGpuMemory::LocalInGPU,
						EMemoryAccess::GpuReadWrite },
					OUT src_image ) );
	
	ModulePtr	dst_image;
	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescriptor{ EImage::Tex2D, uint4(img_dim), EPixelFormat::RGBA32F, EImageUsage::TransferDst | EImageUsage::TransferSrc | EImageUsage::Storage },
						EGpuMemory::LocalInGPU,
						EMemoryAccess::GpuReadWrite },
					OUT dst_image ) );

	CreateInfo::PipelineTemplate	pt_ci;
	Pipelines::Create_copyfloatimage2d( OUT pt_ci.descr );
	
	ModulePtr	pipeline_template;
	CHECK_ERR( factory->Create(
					PipelineTemplateModuleID,
					gpuThread->GlobalSystems(),
					pt_ci,
					OUT pipeline_template ) );
	ModuleUtils::Initialize({ pipeline_template });

	Message< GpuMsg::CreateComputePipeline >	cppl_ctor{ gpuIDs.pipeline, gpuThread };
	pipeline_template->Send( cppl_ctor );

	ModulePtr	pipeline	= *cppl_ctor->result;
	ModulePtr	resource_table;
	CHECK_ERR( factory->Create(
					gpuIDs.resourceTable,
					gpuThread->GlobalSystems(),
					CreateInfo::PipelineResourceTable{},
					OUT resource_table ) );
	
	resource_table->Send< ModuleMsg::AttachModule >({ "pipeline", pipeline });
	resource_table->Send< ModuleMsg::AttachModule >({ "un_SrcImage", src_image });
	resource_table->Send< ModuleMsg::AttachModule >({ "un_DstImage", dst_image });

	ModuleUtils::Initialize({ cmd_buffer, staging_src_image, staging_dst_image, src_image, dst_image, pipeline, resource_table });

	
	// generate image data
	Message< GpuMsg::GetImageMemoryLayout >	req_src_layout;
	staging_src_image->Send( req_src_layout );
	
	BinaryArray		image_data;		image_data.Resize( usize(req_src_layout->result->rowPitch * req_src_layout->result->dimension.y) );

	FOR( i, image_data ) {
		image_data[i] = Random::Int<ubyte>();
	}

	// write data to staging image
	Message< GpuMsg::WriteToImageMemory >	write_cmd{ image_data, uint3(), uint3(img_dim), req_src_layout->result->rowPitch };
	staging_src_image->Send( write_cmd );
	CHECK_ERR( *write_cmd->wasWritten == BytesUL(image_data.Size()) );


	// build command buffer
	cmdBuilder->Send< GpuMsg::CmdBegin >({ cmd_buffer });
	
	// copy image to gpu
	{
		//cmdBuilder->Send< GpuMsg::CmdDebugMarker >({ "copy image to gpu" });

		cmdBuilder->Send< GpuMsg::CmdPipelineBarrier >({ EPipelineStage::Host,
														 EPipelineStage::Transfer,
														 EPipelineAccess::HostWrite,
														 EPipelineAccess::TransferRead,
														 EImageLayout::Preinitialized,
														 EImageLayout::TransferSrcOptimal,
														 staging_src_image,
														 EImageAspect::Color });
	
		cmdBuilder->Send< GpuMsg::CmdPipelineBarrier >({ EPipelineStage::TopOfPipe,
														 EPipelineStage::Transfer,
														 EPipelineAccess::bits(),
														 EPipelineAccess::TransferWrite,
														 EImageLayout::Undefined,
														 EImageLayout::TransferDstOptimal,
														 src_image,
														 EImageAspect::Color });
	
		Message< GpuMsg::CmdCopyImage >	copy_cmd;
		copy_cmd->srcImage	= staging_src_image;
		copy_cmd->srcLayout	= EImageLayout::TransferSrcOptimal;
		copy_cmd->dstImage	= src_image;
		copy_cmd->dstLayout = EImageLayout::TransferDstOptimal;
		copy_cmd->regions	= ArrayCRef<Region>{
								Region{ ImageLayers{ EImageAspect::Color, MipmapLevel(0), ImageLayer(0), 1 }, uint3(),
										ImageLayers{ EImageAspect::Color, MipmapLevel(0), ImageLayer(0), 1 }, uint3(),
										uint3(img_dim, 1) },
								};
		cmdBuilder->Send( copy_cmd );
	}

	// run pipeline
	{
		//cmdBuilder->Send< GpuMsg::CmdDebugMarker >({ "run pipeline" });

		cmdBuilder->Send< GpuMsg::CmdPipelineBarrier >({ EPipelineStage::Transfer,
														 EPipelineStage::ComputeShader,
														 EPipelineAccess::TransferWrite,
														 EPipelineAccess::ShaderRead,
														 EImageLayout::TransferDstOptimal,
														 EImageLayout::General,
														 src_image,
														 EImageAspect::Color });

		cmdBuilder->Send< GpuMsg::CmdPipelineBarrier >({ EPipelineStage::TopOfPipe,
														 EPipelineStage::ComputeShader,
														 EPipelineAccess::bits(),
														 EPipelineAccess::ShaderWrite,
														 EImageLayout::Undefined,
														 EImageLayout::General,
														 dst_image,
														 EImageAspect::Color });

		cmdBuilder->Send< GpuMsg::CmdBindComputePipeline >({ pipeline });
		cmdBuilder->Send< GpuMsg::CmdBindComputeResourceTable >({ resource_table });
		cmdBuilder->Send< GpuMsg::CmdDispatch >({ uint3(img_dim, 1) });
	}

	// copy image back to cpu
	{
		//cmdBuilder->Send< GpuMsg::CmdDebugMarker >({ "copy image back to cpu" });

		cmdBuilder->Send< GpuMsg::CmdPipelineBarrier >({ EPipelineStage::ComputeShader,
														 EPipelineStage::Transfer,
														 EPipelineAccess::ShaderWrite,
														 EPipelineAccess::TransferRead,
														 EImageLayout::General,
														 EImageLayout::TransferSrcOptimal,
														 dst_image,
														 EImageAspect::Color });
		cmdBuilder->Send< GpuMsg::CmdPipelineBarrier >({ EPipelineStage::Host,
														 EPipelineStage::Transfer,
														 EPipelineAccess::bits(),
														 EPipelineAccess::TransferWrite,
														 EImageLayout::Preinitialized,
														 EImageLayout::TransferDstOptimal,
														 staging_dst_image,
														 EImageAspect::Color });
		
		Message< GpuMsg::CmdCopyImage >	copy_cmd;
		copy_cmd->srcImage	= dst_image;
		copy_cmd->srcLayout	= EImageLayout::TransferSrcOptimal;
		copy_cmd->dstImage	= staging_dst_image;
		copy_cmd->dstLayout = EImageLayout::TransferDstOptimal;
		copy_cmd->regions	= ArrayCRef<Region>{
								Region{ ImageLayers{ EImageAspect::Color, MipmapLevel(0), ImageLayer(0), 1 }, uint3(),
										ImageLayers{ EImageAspect::Color, MipmapLevel(0), ImageLayer(0), 1 }, uint3(),
										uint3(img_dim, 1) },
								};
		cmdBuilder->Send( copy_cmd );
	}

	Message< GpuMsg::CmdEnd >	cmd_end;
	cmdBuilder->Send( cmd_end );


	// submit and sync
	gpuThread->Send< GpuMsg::SubmitComputeQueueCommands >({ cmd_end->result.Get(), *fence_ctor->result });

	syncManager->Send< GpuMsg::ClientWaitFence >({ *fence_ctor->result });


	// read from image
	Message< GpuMsg::GetImageMemoryLayout >	req_dst_layout;
	staging_dst_image->Send( req_dst_layout );

	BinaryArray	dst_data;	dst_data.Resize( usize(req_dst_layout->result->rowPitch * req_dst_layout->result->dimension.y) );

	Message< GpuMsg::ReadFromImageMemory >	read_cmd{ dst_data, uint3(), uint3(img_dim), req_dst_layout->result->rowPitch };
	staging_dst_image->Send( read_cmd );
	CHECK_ERR( dst_data.Size() == read_cmd->result->Size() );


	for (uint y = 0; y < img_dim.y; ++y)
	{
		SrcPixel const*	src_row = Cast<SrcPixel const*>(image_data.ptr() + req_src_layout->result->rowPitch * y);
		DstPixel const*	dst_row = Cast<DstPixel const*>(dst_data.ptr() + req_dst_layout->result->rowPitch * y);

		for (uint x = 0; x < img_dim.x; ++x)
		{
			DstPixel	src = TypeCast::IntToUNorm<float>( src_row[x] ).To<DstPixel>();
			DstPixel	dst = dst_row[x];

			CHECK_ERR(All( Equals( src, dst ) ));
		}
	}
	LOG( "CopyFloatImage2D - OK", ELog::Info );
	return true;
}
