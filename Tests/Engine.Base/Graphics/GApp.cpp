// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "GApp.h"
#include "../Pipelines/all_pipelines.h"
#include "Engine/Profilers/Engine.Profilers.h"
#include "Engine/Platforms/Public/Tools/GPUThreadHelper.h"


using Vertex = Pipelines::NativeVertex_default2;


GApp::GApp ()
{
	ms = GetMainSystemInstance();

	Platforms::RegisterPlatforms();
	Profilers::RegisterProfilers();
}


bool GApp::Initialize (GAPI::type api)
{
	auto	factory	= ms->GlobalSystems()->modulesFactory;

	{
		ModulePtr	platform;
		CHECK_ERR( factory->Create( 0, ms->GlobalSystems(), CreateInfo::Platform{}, OUT platform ) );
		ms->Send< ModuleMsg::AttachModule >({ platform });

		Message< OSMsg::GetOSModules >	req_ids;
		platform->Send( req_ids );
		osIDs = *req_ids->result;
	}

	ms->AddModule( InputManagerModuleID, CreateInfo::InputManager{} );
	ms->AddModule( DataProviderManagerModuleID, CreateInfo::DataProviderManager() );

	{
		ModulePtr	context;
		CHECK_ERR( factory->Create( 0, ms->GlobalSystems(), CreateInfo::GpuContext{ api }, OUT context ) );
		ms->Send< ModuleMsg::AttachModule >({ context });

		Message< GpuMsg::GetGraphicsModules >	req_ids;
		context->Send( req_ids );
		gpuIDs = *req_ids->graphics;
	}

	auto		thread	= ms->GlobalSystems()->parallelThread;
	
	ModulePtr	window;
	factory->Create( 0, ms->GlobalSystems(), CreateInfo::Window{}, OUT window );
	thread->Send< ModuleMsg::AttachModule >({ window });

	thread->AddModule( InputThreadModuleID, CreateInfo::InputThread{} );
	thread->AddModule( gpuIDs.thread, CreateInfo::GpuThread{
					   GraphicsSettings{
							api,
							CreateInfo::GpuContext::EFlags::DebugContext
						} } );

	auto	input		= thread->GetModuleByID( InputThreadModuleID );
	auto	gthread		= thread->GetModuleByID( gpuIDs.thread );

	window->AddModule( osIDs.keyInput, CreateInfo::RawInputHandler{} );
	window->AddModule( osIDs.mouseInput, CreateInfo::RawInputHandler{} );
	window->AddModule( Profilers::FPSCounterModuleID, CreateInfo::FPSCounter{} );

	window->Subscribe( this, &GApp::_OnWindowClosed );
	input->Subscribe( this, &GApp::_OnKey );
	input->Subscribe( this, &GApp::_OnMotion );
	gthread->Subscribe( this, &GApp::_Draw );
	gthread->Subscribe( this, &GApp::_GInit );
	gthread->Subscribe( this, &GApp::_GDelete );

	// finish initialization
	ModuleUtils::Initialize({ ms });
	return true;
}


void GApp::Quit ()
{
	Message< ModuleMsg::Delete >	del_msg;

	if ( pipelineTemplate1 )
		pipelineTemplate1->Send( del_msg );
	
	if ( pipelineTemplate2 )
		pipelineTemplate2->Send( del_msg );
	
	looping = false;
}


bool GApp::Update ()
{
	if ( not looping )
		return false;

	ms->Send< ModuleMsg::Update >({});
	return true;
}


bool GApp::_OnWindowClosed (const Message<OSMsg::WindowAfterDestroy> &)
{
	looping = false;
	return true;
}


bool GApp::_OnKey (const Message< ModuleMsg::InputKey > &msg)
{
	DEBUG_CONSOLE( ("Key '"_str << KeyID::ToString( msg->key ) <<
					"', pressure " << msg->pressure).cstr() );
	return true;
}


bool GApp::_OnMotion (const Message< ModuleMsg::InputMotion > &msg)
{
	DEBUG_CONSOLE( ("Motion '"_str << MotionID::ToString( msg->motion ) <<
					"', relative " << msg->relative <<
					", absolute " << msg->absolute).cstr() );
	return true;
}


bool GApp::_Draw (const Message< ModuleMsg::Update > &)
{
	if ( not looping )
		return false;

	auto		gthread		= PlatformTools::GPUThreadHelper::FindGraphicsThread( ms->GlobalSystems() );
	uint		prev_idx	= cmdBufIndex % cmdBuffers.Count();
	uint		index		= (++cmdBufIndex) % cmdBuffers.Count();

	if ( not firstCall ) {
		syncManager->Send< GpuMsg::ClientWaitFence >({ cmdFence[prev_idx] });
	}
	firstCall = false;

	Message< GpuMsg::ThreadBeginFrame >	begin_frame;
	gthread->Send( begin_frame );

	ModulePtr	system_fb	= begin_frame->result->framebuffer;

	cmdBuilder->Send< GpuMsg::CmdBegin >({ cmdBuffers[index] });

	// clear image
	{
		cmdBuilder->Send< GpuMsg::CmdPipelineBarrier >({ EPipelineStage::TopOfPipe, EPipelineStage::Transfer,
														 EPipelineAccess::bits(), EPipelineAccess::TransferWrite,
														 EImageLayout::Undefined, EImageLayout::TransferDstOptimal,
														 texture, EImageAspect::Color });

		cmdBuilder->Send< GpuMsg::CmdClearColorImage >({ texture, EImageLayout::TransferDstOptimal, float4(0.5f) });

		cmdBuilder->Send< GpuMsg::CmdPipelineBarrier >({ EPipelineStage::Transfer, EPipelineStage::FragmentShader,
														 EPipelineAccess::TransferWrite, EPipelineAccess::ShaderRead,
														 EImageLayout::TransferDstOptimal, EImageLayout::ShaderReadOnlyOptimal,
														 texture, EImageAspect::Color });
	}

	// draw triangle to framebuffer
	{
		ModulePtr	render_pass	= framebuffer->GetModuleByMsg< RenderPassMsgList_t >();

		Message< GpuMsg::GetFramebufferDescriptor >	fb_descr_request;
		framebuffer->Send( fb_descr_request );

		auto const&	fb_descr	= fb_descr_request->result.Get();
		RectU		area		= RectU( 0, 0, fb_descr.size.x, fb_descr.size.y );

		GpuMsg::CmdClearAttachments	clear;
		clear.attachments.PushBack({ EImageAspect::Color, 0, float4(1.0f) });
		clear.clearRects.PushBack({ area });

		cmdBuilder->Send< GpuMsg::CmdBeginRenderPass >({ render_pass, framebuffer, area });
		//cmdBuilder->Send< GpuMsg::CmdClearAttachments >( clear );
		cmdBuilder->Send< GpuMsg::CmdBindGraphicsPipeline >({ gpipeline1 });
		cmdBuilder->Send< GpuMsg::CmdBindGraphicsResourceTable >({ resourceTable1 });
		cmdBuilder->Send< GpuMsg::CmdSetViewport >({ area, float2(0.0f, 1.0f) });
		cmdBuilder->Send< GpuMsg::CmdSetScissor >({ area });
		cmdBuilder->Send< GpuMsg::CmdDraw >({ 3u });
		cmdBuilder->Send< GpuMsg::CmdEndRenderPass >({});
	}

	// draw quad to screen
	{
		ModulePtr	render_pass	= system_fb->GetModuleByMsg< RenderPassMsgList_t >();

		Message< GpuMsg::GetFramebufferDescriptor >	fb_descr_request;
		system_fb->Send( fb_descr_request );

		auto const&	fb_descr	= fb_descr_request->result.Get();
		RectU		area		= RectU( 0, 0, fb_descr.size.x, fb_descr.size.y );

		cmdBuilder->Send< GpuMsg::CmdBeginRenderPass >({ render_pass, system_fb, area });
		cmdBuilder->Send< GpuMsg::CmdBindGraphicsPipeline >({ gpipeline2 });
		cmdBuilder->Send< GpuMsg::CmdBindGraphicsResourceTable >({ resourceTable2 });
		cmdBuilder->Send< GpuMsg::CmdSetViewport >({ area, float2(0.0f, 1.0f) });
		cmdBuilder->Send< GpuMsg::CmdSetScissor >({ area });
		cmdBuilder->Send< GpuMsg::CmdBindVertexBuffers >({ vbuffer });
		cmdBuilder->Send< GpuMsg::CmdBindIndexBuffer >({ ibuffer, EIndex::UInt });
		cmdBuilder->Send< GpuMsg::CmdDrawIndexed >({ 6u });
		cmdBuilder->Send< GpuMsg::CmdEndRenderPass >({});
	}

	Message< GpuMsg::CmdEnd >	cmd_end = {};
	cmdBuilder->Send( cmd_end );

	gthread->Send< GpuMsg::ThreadEndFrame >({ system_fb, cmd_end->result.Get(null), cmdFence[index] });
	return true;
}


bool GApp::_CreatePipeline1 ()
{
	auto	gthread	= PlatformTools::GPUThreadHelper::FindGraphicsThread( ms->GlobalSystems() );
	auto	factory	= ms->GlobalSystems()->modulesFactory;
	
	CreateInfo::PipelineTemplate	pp_templ;
	Pipelines::Create_default( OUT pp_templ.descr );

	CHECK_ERR( factory->Create(
					PipelineTemplateModuleID,
					ms->GlobalSystems(),
					pp_templ,
					OUT pipelineTemplate1 )
	);

	Message< GpuMsg::CreateGraphicsPipeline >	create_gpp;
	
	create_gpp->gpuThread	= gthread;
	create_gpp->moduleID	= gpuIDs.pipeline;
	create_gpp->topology	= EPrimitive::TriangleList;
	create_gpp->renderPass	= framebuffer->GetModuleByMsg< RenderPassMsgList_t >();

	pipelineTemplate1->Send( create_gpp );
	gpipeline1 = create_gpp->result.Get();

	CHECK_ERR( factory->Create(
					gpuIDs.resourceTable,
					ms->GlobalSystems(),
					CreateInfo::PipelineResourceTable{},
					OUT resourceTable1 )
	);
	
	Message< GpuMsg::GetImageDescriptor >	req_img_descr;
	texture->Send( req_img_descr );

	resourceTable1->Send< ModuleMsg::AttachModule >({ "pipeline", gpipeline1 });
	resourceTable1->Send< GpuMsg::PipelineAttachImage >({ "un_ColorTexture", texture, ImageViewDescriptor{*req_img_descr->result}, EImageLayout::ShaderReadOnlyOptimal });
	resourceTable1->Send< ModuleMsg::AttachModule >({ "un_ColorTexture.sampler", sampler });
	return true;
}


bool GApp::_CreatePipeline2 ()
{
	auto	gthread = PlatformTools::GPUThreadHelper::FindGraphicsThread( ms->GlobalSystems() );
	auto	factory = ms->GlobalSystems()->modulesFactory;
	
	CreateInfo::PipelineTemplate	pp_templ;
	Pipelines::Create_default2( OUT pp_templ.descr );

	CHECK_ERR( factory->Create(
					PipelineTemplateModuleID,
					ms->GlobalSystems(),
					pp_templ,
					OUT pipelineTemplate2 )
	);

	Message< GpuMsg::CreateGraphicsPipeline >	create_gpp;

	create_gpp->gpuThread	= gthread;
	create_gpp->moduleID	= gpuIDs.pipeline;
	create_gpp->topology	= EPrimitive::TriangleList;
	create_gpp->vertexInput.Add( "at_Position", &Vertex::at_Position )
							.Add( "at_Texcoord", &Vertex::at_Texcoord )
							.Bind( "", SizeOf<Vertex> );
	
	pipelineTemplate2->Send( create_gpp );
	gpipeline2 = create_gpp->result.Get();

	CHECK_ERR( factory->Create(
					gpuIDs.resourceTable,
					ms->GlobalSystems(),
					CreateInfo::PipelineResourceTable{},
					OUT resourceTable2 )
	);

	Message< GpuMsg::GetImageDescriptor >	req_img_descr;
	fbColorImage->Send( req_img_descr );

	resourceTable2->Send< ModuleMsg::AttachModule >({ "pipeline", gpipeline2 });
	resourceTable2->Send< GpuMsg::PipelineAttachImage >({ "un_ColorTexture", fbColorImage, ImageViewDescriptor{*req_img_descr->result}, EImageLayout::ShaderReadOnlyOptimal });
	resourceTable2->Send< ModuleMsg::AttachModule >({ "un_ColorTexture.sampler", sampler });
	resourceTable2->Send< ModuleMsg::AttachModule >({ "ub", ubuffer });
	return true;
}


bool GApp::_CreateCmdBuffers ()
{
	auto	gthread = PlatformTools::GPUThreadHelper::FindGraphicsThread( ms->GlobalSystems() );
	auto	factory = ms->GlobalSystems()->modulesFactory;

	CHECK_ERR( factory->Create(
					gpuIDs.commandBuilder,
					gthread->GlobalSystems(),
					CreateInfo::GpuCommandBuilder{ gthread },
					OUT cmdBuilder )
	);
	
	cmdBufIndex = 0;
	cmdBuffers.Resize( 6 );
	cmdFence.Resize( 6 );

	FOR( i, cmdBuffers )
	{
		CHECK_ERR( factory->Create(
						gpuIDs.commandBuffer,
						gthread->GlobalSystems(),
						CreateInfo::GpuCommandBuffer{
							CommandBufferDescriptor{ ECmdBufferCreate::ImplicitResetable }
						},
						OUT cmdBuffers[i] )
		);
		cmdBuilder->Send< ModuleMsg::AttachModule >({ ""_str << i, cmdBuffers[i] });
	}

	FOR( i, cmdFence )
	{
		Message< GpuMsg::CreateFence >	ctor;
		syncManager->Send( ctor );
		cmdFence[i] = *ctor->result;
	}
	return true;
}


bool GApp::_GInit (const Message< GpuMsg::DeviceCreated > &)
{
	auto	gthread = PlatformTools::GPUThreadHelper::FindGraphicsThread( ms->GlobalSystems() );
	auto	factory = gthread->GlobalSystems()->modulesFactory;

	syncManager = gthread->GetModuleByMsg<CompileTime::TypeListFrom< Message<GpuMsg::CreateFence> >>();
	CHECK_ERR( syncManager );

	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gthread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescriptor{ EImage::Tex2D, uint4( 1024, 1024, 0, 0 ), EPixelFormat::RGBA8_UNorm, EImageUsage::Sampled | EImageUsage::ColorAttachment },
						EGpuMemory::LocalInGPU,
						EMemoryAccess::GpuRead | EMemoryAccess::GpuWrite },
					OUT fbColorImage ) );
	/*
	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gthread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescriptor{ EImage::Tex2D, uint4( 4, 4, 0, 0 ), EPixelFormat::RGBA8_UNorm, EImageUsage::Sampled | EImageUsage::TransferDst },
						EGpuMemory::CoherentWithCPU },
					OUT texture ) );
	/*/
	CHECK_ERR( factory->Create(
					gpuIDs.image,
					gthread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescriptor{ EImage::Tex2D, uint4( 128, 128, 0, 0 ), EPixelFormat::RGBA8_UNorm, EImageUsage::Sampled | EImageUsage::TransferDst },
						EGpuMemory::LocalInGPU,
						EMemoryAccess::GpuRead | EMemoryAccess::GpuWrite },
					OUT texture ) );
	//*/
	CHECK_ERR( factory->Create(
					gpuIDs.sampler,
					gthread->GlobalSystems(),
					CreateInfo::GpuSampler{ SamplerDescriptor::Builder()
						.SetAddressMode( EAddressMode::Clamp )
						.SetFilter( EFilter::MinMagMipLinear )
						.Finish() },
					OUT sampler ) );

	CHECK_ERR( factory->Create(
					gpuIDs.buffer,
					gthread->GlobalSystems(),
					CreateInfo::GpuBuffer{
						BufferDescriptor{ SizeOf<Vertex> * 4, EBufferUsage::Vertex },
						EGpuMemory::CoherentWithCPU,
						EMemoryAccess::CpuRead | EMemoryAccess::CpuWrite  },
					OUT vbuffer ) );

	CHECK_ERR( factory->Create(
					gpuIDs.buffer,
					gthread->GlobalSystems(),
					CreateInfo::GpuBuffer{
						BufferDescriptor{ SizeOf<uint> * 6, EBufferUsage::Index },
						EGpuMemory::CoherentWithCPU,
						EMemoryAccess::CpuRead | EMemoryAccess::CpuWrite },
					OUT ibuffer ) );

	CHECK_ERR( factory->Create(
					gpuIDs.buffer,
					gthread->GlobalSystems(),
					CreateInfo::GpuBuffer{
						BufferDescriptor{ SizeOf<Pipelines::UB>, EBufferUsage::Uniform },
						EGpuMemory::CoherentWithCPU,
						EMemoryAccess::CpuRead | EMemoryAccess::CpuWrite },
					OUT ubuffer ) );

	CHECK_ERR( factory->Create(
					gpuIDs.framebuffer,
					gthread->GlobalSystems(),
					CreateInfo::GpuFramebuffer{ uint2(1024, 1024) },
					OUT framebuffer ) );
	
	ImageViewDescriptor	view_descr;

	framebuffer->Send< GpuMsg::FramebufferAttachImage >({ "Color0", fbColorImage, view_descr });

	CHECK_ERR( _CreateCmdBuffers() );
	CHECK_ERR( ModuleUtils::Initialize({ texture, sampler, vbuffer, ibuffer, ubuffer, cmdBuilder, framebuffer, fbColorImage }) );

	CHECK_ERR( _CreatePipeline1() );
	CHECK_ERR( _CreatePipeline2() );
	CHECK_ERR( ModuleUtils::Initialize({ gpipeline1, gpipeline2, resourceTable1, resourceTable2 }) );

	const Vertex vertices[] = {
		{ float2(-1.0f, -1.0f), float2(0.0f, 0.0f) },
		{ float2(-1.0f,  1.0f), float2(0.0f, 1.0f) },
		{ float2( 1.0f, -1.0f), float2(1.0f, 0.0f) },
		{ float2( 1.0f,  1.0f), float2(1.0f, 1.0f) }
	};
	const uint indices[] = {
		0, 1, 2,
		2, 1, 3
	};
	const uint pixels[] = {
		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
	};

	Pipelines::UB	ub_data;	ub_data.color = float4(1.0f);
	
	vbuffer->Send< GpuMsg::WriteToGpuMemory >({ BinArrayCRef::From(vertices) });
	ibuffer->Send< GpuMsg::WriteToGpuMemory >({ BinArrayCRef::From(indices) });
	ubuffer->Send< GpuMsg::WriteToGpuMemory >({ BinArrayCRef::FromValue(ub_data) });
	//texture->Send< GpuMsg::WriteToImageMemory >({ BinArrayCRef::From(pixels), uint4(4,4,0,0), 4_b });

	return true;
}


bool GApp::_GDelete (const Message< GpuMsg::DeviceBeforeDestroy > &)
{
	return true;
}
