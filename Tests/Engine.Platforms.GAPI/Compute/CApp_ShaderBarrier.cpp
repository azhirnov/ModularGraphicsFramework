// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CApp.h"
#include "Pipelines/all_pipelines.h"

bool CApp::_Test_ShaderBarrier ()
{
	using Region		= GpuMsg::CmdCopyImage::Region;
	using ImageLayers	= GpuMsg::CmdCopyImage::ImageLayers;
	

	CreateInfo::PipelineTemplate	pt_ci;
	Pipelines::Create_shaderbarrier( OUT pt_ci.descr );

	const uint2		group_count	= uint2{ 2 };
	const uint2		local_size	= pt_ci.descr.localGroupSize.xy();
	const uint2		img_dim		= local_size * group_count;
	const float		max_value	= 100.0f;

	// generate data
	Array<float4>		data;	data.Resize( img_dim.x * img_dim.y );

	for (uint y1 = 0; y1 < group_count.y; ++y1)
	for (uint x1 = 0; x1 < group_count.x; ++x1)
	{
		const float4	rnd = Random::Float( float4() );

		for (uint y2 = 0; y2 < local_size.y; ++y2)
		for (uint x2 = 0; x2 < local_size.x; ++x2)
		{
			const uint		i = (x1 * local_size.x + x2) + (y1 * local_size.y + y2) * (local_size.x * group_count.x);

			const float4	off = float4{ float(x2), float(y2), float((local_size.x - x2) * 2), float((local_size.y - y2) * 2) };

			data[i] = rnd + off;
		}
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

	ModulePtr	staging_src_image;
	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescriptor{ EImage::Tex2D, uint4(img_dim), EPixelFormat::RGBA32F, EImageUsage::TransferSrc },
						EGpuMemory::CoherentWithCPU,
						EMemoryAccess::All
					},
					OUT staging_src_image ) );
	
	ModulePtr	staging_dst_image;
	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescriptor{ EImage::Tex2D, uint4(img_dim), EPixelFormat::RGBA32F, EImageUsage::TransferDst },
						EGpuMemory::CoherentWithCPU,
						EMemoryAccess::All
					},
					OUT staging_dst_image ) );

	ModulePtr	src_image;
	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescriptor{ EImage::Tex2D, uint4(img_dim), EPixelFormat::RGBA32F, EImageUsage::TransferDst | EImageUsage::Storage },
						EGpuMemory::LocalInGPU,
						EMemoryAccess::GpuReadWrite
					},
					OUT src_image
				) );
	
	ModulePtr	dst_image;
	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescriptor{ EImage::Tex2D, uint4(img_dim), EPixelFormat::RGBA32F, EImageUsage::TransferDst | EImageUsage::TransferSrc | EImageUsage::Storage },
						EGpuMemory::LocalInGPU,
						EMemoryAccess::GpuReadWrite
					},
					OUT dst_image
				) );

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

	
	// write data to staging image
	Message< GpuMsg::WriteToImageMemory >	write_cmd{ BinArrayCRef::From(data), uint3(img_dim), SizeOf<float4> };
	staging_src_image->Send( write_cmd );
	CHECK_ERR( *write_cmd->wasWritten == BytesUL(data.Size()) );


	// build command buffer
	cmdBuilder->Send< GpuMsg::CmdBegin >({ cmd_buffer });
	
	// copy image to gpu
	{
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
		cmdBuilder->Send< GpuMsg::CmdDispatch >({ uint3(group_count, 1) });
	}

	// copy image back to cpu
	{
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
	Array<float4>	dst_data;	dst_data.Resize( img_dim.x * img_dim.y );

	Message< GpuMsg::ReadFromImageMemory >	read_cmd{ BinArrayRef::From(dst_data), uint3(img_dim), SizeOf<float4> };
	staging_dst_image->Send( read_cmd );
	CHECK_ERR( dst_data.Size() == read_cmd->result->Size() );

	for (uint y = 0; y < img_dim.y; ++y)
	for (uint x = 0; x < img_dim.x; ++x)
	{
		const uint	i  = x + y * img_dim.x;
		const float	vx = (float) MaxMag( int(x % local_size.x) - int(local_size.x-1), int(x % local_size.x) );
		const float	vy = (float) MaxMag( int(y % local_size.y) - int(local_size.y-1), int(y % local_size.y) );

		CHECK_ERR( Equals( dst_data[i].x, vx ) );
		CHECK_ERR( Equals( dst_data[i].y, vy ) );
		CHECK_ERR( Equals( dst_data[i].z, vx * -2.0f ) );
		CHECK_ERR( Equals( dst_data[i].w, vy * -2.0f ) );
	}

	LOG( "ShaderBarrier - OK", ELog::Info );
	return true;
}
