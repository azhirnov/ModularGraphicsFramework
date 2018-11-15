// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CApp.h"
#include "Pipelines/all_pipelines.h"

bool CApp::_Test_ShaderBarrier ()
{
	CreateInfo::PipelineTemplate	pt_ci;
	Pipelines::Create_shaderbarrier( OUT pt_ci.descr );

	const uint2		group_count	= uint2{ 2 };
	const uint2		local_size	= pt_ci.descr.localGroupSize.xy();
	const uint2		img_dim		= local_size * group_count;
	const float		max_value	= 100.0f;



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
						ImageDescription{ EImage::Tex2D, uint4(img_dim), EPixelFormat::RGBA32F, EImageUsage::TransferSrc },
						EGpuMemory::CoherentWithCPU },
					OUT staging_src_image ) );
	
	ModulePtr	staging_dst_image;
	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescription{ EImage::Tex2D, uint4(img_dim), EPixelFormat::RGBA32F, EImageUsage::TransferDst },
						EGpuMemory::CoherentWithCPU },
					OUT staging_dst_image ) );

	ModulePtr	src_image;
	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescription{ EImage::Tex2D, uint4(img_dim), EPixelFormat::RGBA32F, EImageUsage::TransferDst | EImageUsage::Storage },
						EGpuMemory::LocalInGPU | EGpuMemory::Dedicated,
						EMemoryAccess::GpuReadWrite },
					OUT src_image ) );
	
	ModulePtr	dst_image;
	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescription{ EImage::Tex2D, uint4(img_dim), EPixelFormat::RGBA32F, EImageUsage::TransferDst | EImageUsage::TransferSrc | EImageUsage::Storage },
						EGpuMemory::LocalInGPU | EGpuMemory::Dedicated,
						EMemoryAccess::GpuReadWrite },
					OUT dst_image ) );

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
	resource_table->Send( ModuleMsg::AttachModule{ "un_SrcImage", src_image });
	resource_table->Send( ModuleMsg::AttachModule{ "un_DstImage", dst_image });

	ModuleUtils::Initialize({ cmd_buffer, staging_src_image, staging_dst_image, src_image, dst_image, pipeline, resource_table });

	
	// generate image data
	GpuMsg::GetImageMemoryLayout	req_src_layout;
	staging_src_image->Send( req_src_layout );
	
	Array<float4>	image_data;
	image_data.Resize( usize(req_src_layout.result->rowPitch * req_src_layout.result->dimension.y) / sizeof(float4) );

	for (uint y1 = 0; y1 < group_count.y; ++y1)
	for (uint x1 = 0; x1 < group_count.x; ++x1)
	{
		const float4	rnd = Random::Float( float4() );

		for (uint y2 = 0; y2 < local_size.y; ++y2)
		for (uint x2 = 0; x2 < local_size.x; ++x2)
		{
			const usize		i = (x1 * local_size.x + x2) + usize(req_src_layout.result->rowPitch * (y1 * local_size.y + y2)) / sizeof(float4);

			const float4	off = float4{ float(x2), float(y2), float((local_size.x - x2) * 2), float((local_size.y - y2) * 2) };

			image_data[i] = rnd + off;
		}
	}

	// write data to staging image
	GpuMsg::WriteToImageMemory	write_cmd{ BinArrayCRef::From(image_data), uint3(), uint3(img_dim), req_src_layout.result->rowPitch };
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
															uint3(img_dim, 1) ));
	}

	// run pipeline
	{
		cmdBuilder->Send( GpuMsg::CmdPipelineBarrier{ EPipelineStage::Transfer, EPipelineStage::ComputeShader }
							.AddImage({	src_image,
										EPipelineAccess::TransferWrite,
										EPipelineAccess::ShaderRead,
										EImageLayout::TransferDstOptimal,
										EImageLayout::General,
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
		cmdBuilder->Send( GpuMsg::CmdDispatch{ uint3(group_count, 1) });
	}

	// copy image back to cpu
	{
		cmdBuilder->Send( GpuMsg::CmdPipelineBarrier{ EPipelineStage::ComputeShader, EPipelineStage::Transfer }
							.AddImage({	dst_image,
										EPipelineAccess::ShaderWrite,
										EPipelineAccess::TransferRead,
										EImageLayout::General,
										EImageLayout::TransferSrcOptimal,
										EImageAspect::Color }) );

		cmdBuilder->Send( GpuMsg::CmdPipelineBarrier{ EPipelineStage::Host, EPipelineStage::Transfer }
							.AddImage({	staging_dst_image,
										EPipelineAccess::bits(),
										EPipelineAccess::TransferWrite,
										EImageLayout::Preinitialized,
										EImageLayout::TransferDstOptimal,
										EImageAspect::Color }) );

		cmdBuilder->Send( GpuMsg::CmdCopyImage{}.SetSource( dst_image, EImageLayout::TransferSrcOptimal )
												.SetDestination( staging_dst_image, EImageLayout::TransferDstOptimal )
												.AddRegion( ImageRange{ EImageAspect::Color, 0_mipmap, 0_layer, 1 }, uint3(),
															ImageRange{ EImageAspect::Color, 0_mipmap, 0_layer, 1 }, uint3(),
															uint3(img_dim, 1) ));

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

	Array<float4>	dst_data;
	dst_data.Resize( usize(req_dst_layout.result->rowPitch * req_dst_layout.result->dimension.y) / sizeof(float4) );

	GpuMsg::ReadFromImageMemory		read_cmd{ BinArrayRef::From(dst_data), uint3(), uint3(img_dim), req_dst_layout.result->rowPitch };
	staging_dst_image->Send( read_cmd );
	CHECK_ERR( dst_data.Size() == read_cmd.result->Size() );

	for (uint y = 0; y < img_dim.y; ++y)
	for (uint x = 0; x < img_dim.x; ++x)
	{
		const usize	i  = x + usize(req_dst_layout.result->rowPitch * y) / sizeof(float4);
		const float	vx = float(MaxMag( int(x % local_size.x) - int(local_size.x-1), int(x % local_size.x) ));
		const float	vy = float(MaxMag( int(y % local_size.y) - int(local_size.y-1), int(y % local_size.y) ));

		CHECK_ERR( Equals( dst_data[i].x, vx ) );
		CHECK_ERR( Equals( dst_data[i].y, vy ) );
		CHECK_ERR( Equals( dst_data[i].z, vx * -2.0f ) );
		CHECK_ERR( Equals( dst_data[i].w, vy * -2.0f ) );
	}

	LOG( "ShaderBarrier - OK", ELog::Info );
	return true;
}
