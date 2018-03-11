// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CApp.h"

bool CApp::_Test_CopyImage2DToBuffer ()
{
	const uint		align		= 4;

	const uint2		img_dim		{125, 125};
	const uint2		img2_dim	= img_dim * 2;

	const uint3		src_off1	= uint3(0);
	const uint3		dst_off1	= uint3(24, 27, 0);
	const uint3		size1		= uint3(64, 34, 1);

	const uint3		src_off2	= uint3(32, 54, 0);
	const uint3		dst_off2	= uint3(88, 66, 0);
	const uint3		size2		= uint3(83, 51, 1);

	const usize		dst_size	= AlignToLarge( img2_dim.x, align ) * img2_dim.y * sizeof(ubyte4);
	

	// generate data
	BinaryArray		data;	data.Resize( AlignToLarge( img_dim.x, align ) * img_dim.y * sizeof(ubyte4) );

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

	ModulePtr	src_image;
	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescriptor{ EImage::Tex2D, uint4(img_dim), EPixelFormat::RGBA8U, EImageUsage::TransferSrc },
						EGpuMemory::CoherentWithCPU,
						EMemoryAccess::All
					},
					OUT src_image
				) );
	
	ModulePtr	dst_buffer;
	CHECK_ERR( factory->Create(
					gpuIDs.buffer,
					gpuThread->GlobalSystems(),
					CreateInfo::GpuBuffer{
						BufferDescriptor{ BytesU(dst_size), EBufferUsage::TransferDst },
						EGpuMemory::CoherentWithCPU,
						EMemoryAccess::All
					},
					OUT dst_buffer
				) );

	ModuleUtils::Initialize({ cmd_buffer, src_image, dst_buffer });


	// write data to image
	Message< GpuMsg::WriteToImageMemory >	write_cmd{ data, uint3(img_dim), SizeOf<ubyte4>, uint3(), align * SizeOf<ubyte4> };
	src_image->Send( write_cmd );
	CHECK_ERR( *write_cmd->wasWritten == BytesUL(data.Size()) );


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
												     0_b, data.Size() });

	using Region = GpuMsg::CmdCopyImageToBuffer::Region;
	using ImageLayers = GpuMsg::CmdCopyImageToBuffer::ImageLayers;

	Message< GpuMsg::CmdCopyImageToBuffer >	copy_cmd;
	copy_cmd->srcImage	= src_image;
	copy_cmd->srcLayout	= EImageLayout::TransferSrcOptimal;
	copy_cmd->dstBuffer	= dst_buffer;
	copy_cmd->regions	= ArrayCRef<Region>{
							Region{ BytesUL::SizeOf<ubyte4>() * (dst_off1.x + dst_off1.y * AlignToLarge( img2_dim.x, align )),
									AlignToLarge( img2_dim.x, align ), img2_dim.y,
									ImageLayers{ EImageAspect::Color, MipmapLevel(0), ImageLayer(0), 1 },
									src_off1,
									size1 },
							Region{ BytesUL::SizeOf<ubyte4>() * (dst_off2.x + dst_off2.y * AlignToLarge( img2_dim.x, align )),
									AlignToLarge( img2_dim.x, align ), img2_dim.y,
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
	BinaryArray	dst_data;	dst_data.Resize( dst_size );

	Message< GpuMsg::ReadFromGpuMemory >	read_cmd{ dst_data };
	dst_buffer->Send( read_cmd );
	CHECK_ERR( dst_data.Size() == read_cmd->result->Size() );
	

	// compare
	ubyte4 const*	src = (ubyte4 const*) data.ptr();
	ubyte4 const*	dst = (ubyte4 const*) dst_data.ptr();
	uint2 c;

	for (c.y = 0; c.y < size1.y; ++c.y)
	for (c.x = 0; c.x < size1.x; ++c.x)
	{
		uint	i = (c.x + src_off1.x) + (c.y + src_off1.y) * AlignToLarge( img_dim.x, align );
		uint	j = (c.x + dst_off1.x) + (c.y + dst_off1.y) * AlignToLarge( img2_dim.x, align );

		CHECK_ERR(All( src[i] == dst[j] ));
	}
	
	for (c.y = 0; c.y < size2.y; ++c.y)
	for (c.x = 0; c.x < size2.x; ++c.x)
	{
		uint	i = (c.x + src_off2.x) + (c.y + src_off2.y) * AlignToLarge( img_dim.x, align );
		uint	j = (c.x + dst_off2.x) + (c.y + dst_off2.y) * AlignToLarge( img2_dim.x, align );

		CHECK_ERR(All( src[i] == dst[j] ));
	}
	
	LOG( "CopyImage2DToBuffer - OK", ELog::Info );
	return true;
}
