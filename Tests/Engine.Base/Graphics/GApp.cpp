// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "GApp.h"
#include "../Pipelines/all_pipelines.h"


struct Vertex
{
	float2	position;
	float2	texcoord;
};

inline ModulePtr GetGThread (GlobalSystemsRef gs)
{
	using GThreadMsgList_t		= CompileTime::TypeListFrom< Message<GpuMsg::ThreadBeginFrame>, Message<GpuMsg::ThreadEndFrame>, Message<GpuMsg::GetDeviceInfo> >;
	using GThreadEventList_t	= CompileTime::TypeListFrom< Message<GpuMsg::DeviceCreated>, Message<GpuMsg::DeviceBeforeDestroy> >;

	return gs->Get< ParallelThread >()->GetModuleByMsgEvent< GThreadMsgList_t, GThreadEventList_t >();
}


GApp::GApp ()
{
	ms = GetMainSystemInstace();

	Platforms::RegisterPlatforms();
}


bool GApp::Initialize (GAPI::type api)
{
	ms->AddModule( GModID::type(0), CreateInfo::Platform{} );
	ms->AddModule( InputManagerModuleID, CreateInfo::InputManager{} );
	ms->AddModule( StreamManagerModuleID, CreateInfo::StreamManager{} );

	{
		auto		factory	= ms->GlobalSystems()->Get< ModulesFactory >();
		ModulePtr	context;

		CHECK_ERR( factory->Create( 0, ms->GlobalSystems(), CreateInfo::GpuContext{ api }, OUT context ) );
		ms->Send< ModuleMsg::AttachModule >({ context });

		Message< GpuMsg::GetGraphicsModules >	req_ids;
		context->Send( req_ids );
		ids << req_ids->graphics;
	}

	auto	thread	= ms->GlobalSystems()->Get< ParallelThread >();
	
	thread->AddModule( WinWindowModuleID, CreateInfo::Window{} );
	thread->AddModule( InputThreadModuleID, CreateInfo::InputThread{} );
	thread->AddModule( ids.thread, CreateInfo::GpuThread{
					   GraphicsSettings{
							api,
							CreateInfo::GpuContext::EFlags::bits() | CreateInfo::GpuContext::EFlags::DebugContext
						} } );

	auto	window		= thread->GetModuleByID( WinWindowModuleID );
	auto	input		= thread->GetModuleByID( InputThreadModuleID );
	auto	gthread		= thread->GetModuleByID( ids.thread );

	window->AddModule( WinKeyInputModuleID, CreateInfo::RawInputHandler{} );
	window->AddModule( WinMouseInputModuleID, CreateInfo::RawInputHandler{} );

	window->Subscribe( this, &GApp::_OnWindowClosed );
	input->Subscribe( this, &GApp::_OnKey );
	//input->Subscribe( this, &GApp::_OnMotion );
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

	auto	thread		= ms->GlobalSystems()->Get< ParallelThread >();
	auto	window		= thread->GetModuleByID( WinWindowModuleID );
	auto	input		= thread->GetModuleByID( InputThreadModuleID );
	auto	gthread		= thread->GetModuleByID( ids.thread );

	if ( window )
		window->UnsubscribeAll( this );
	
	if ( input )
		input->UnsubscribeAll( this );
	
	if ( gthread )
		gthread->UnsubscribeAll( this );

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

	auto	gthread	= GetGThread( ms->GlobalSystems() );

	Message< GpuMsg::ThreadBeginFrame >	begin_frame;
	gthread->Send( begin_frame );

	ModulePtr	system_fb	= begin_frame->result->framebuffer;
	ModulePtr	builder		= cmdBuilder;	//begin_frame->result->commandBuilder;

	builder->Send< GpuMsg::CmdBegin >({ cmdBuffers[cmdBufIndex++ % cmdBuffers.Count()] });

	// clear image
	{
		builder->Send< GpuMsg::CmdPipelineBarrier >({ EPipelineStage::bits() | EPipelineStage::Transfer,  EPipelineStage::bits() | EPipelineStage::Transfer,
					  EImageLayout::Undefined, EImageLayout::TransferDstOptimal, texture, EImageAspect::bits() | EImageAspect::Color });

		builder->Send< GpuMsg::CmdClearColorImage >({ texture, EImageLayout::TransferDstOptimal, float4(0.5f) });

		builder->Send< GpuMsg::CmdPipelineBarrier >({  EPipelineStage::bits() | EPipelineStage::Transfer,  EPipelineStage::AllGraphics,
					  EImageLayout::TransferDstOptimal, EImageLayout::ShaderReadOnlyOptimal, texture, EImageAspect::bits() | EImageAspect::Color });
	}

	// draw triangle to framebuffer
	{
		ModulePtr	render_pass	= framebuffer->GetModuleByMsg< RenderPassMsgList_t >();

		Message< GpuMsg::GetFramebufferDescriptor >	fb_descr_request;
		framebuffer->Send( fb_descr_request );

		auto const&	fb_descr	= fb_descr_request->result.Get();
		RectU		area		= RectU( 0, 0, fb_descr.size.x, fb_descr.size.y );

		GpuMsg::CmdClearAttachments	clear;
		clear.attachments.PushBack({ EImageAspect::bits() | EImageAspect::Color, 0, float4(1.0f) });
		clear.clearRects.PushBack({ area });

		builder->Send< GpuMsg::CmdBeginRenderPass >({ render_pass, framebuffer, area });
		builder->Send< GpuMsg::CmdClearAttachments >( clear );
		builder->Send< GpuMsg::CmdBindGraphicsPipeline >({ gpipeline1 });
		builder->Send< GpuMsg::CmdBindGraphicsResourceTable >({ resourceTable1 });
		builder->Send< GpuMsg::CmdSetViewport >({ area, float2(0.0f, 1.0f) });
		builder->Send< GpuMsg::CmdSetScissor >({ area });
		builder->Send< GpuMsg::CmdDraw >({ 3u });
		builder->Send< GpuMsg::CmdEndRenderPass >({});
	}

	// draw quad to screen
	{
		ModulePtr	render_pass	= system_fb->GetModuleByMsg< RenderPassMsgList_t >();

		Message< GpuMsg::GetFramebufferDescriptor >	fb_descr_request;
		system_fb->Send( fb_descr_request );

		auto const&	fb_descr	= fb_descr_request->result.Get();
		RectU		area		= RectU( 0, 0, fb_descr.size.x, fb_descr.size.y );

		builder->Send< GpuMsg::CmdBeginRenderPass >({ render_pass, system_fb, area });
		builder->Send< GpuMsg::CmdBindGraphicsPipeline >({ gpipeline2 });
		builder->Send< GpuMsg::CmdBindGraphicsResourceTable >({ resourceTable2 });
		builder->Send< GpuMsg::CmdSetViewport >({ area, float2(0.0f, 1.0f) });
		builder->Send< GpuMsg::CmdSetScissor >({ area });
		builder->Send< GpuMsg::CmdBindVertexBuffers >({ vbuffer });
		builder->Send< GpuMsg::CmdBindIndexBuffer >({ ibuffer, EIndex::UInt });
		builder->Send< GpuMsg::CmdDrawIndexed >({ 6u });
		builder->Send< GpuMsg::CmdEndRenderPass >({});
	}

	Message< GpuMsg::CmdEnd >	cmd_end = {};
	builder->Send( cmd_end );

	gthread->Send< GpuMsg::ThreadEndFrame >({ system_fb, InitializerList<ModulePtr>{ cmd_end->cmdBuffer.Get(null) } });
	return true;
}


bool GApp::_CreatePipeline1 ()
{
	auto	gthread	= GetGThread( ms->GlobalSystems() );
	auto	factory	= ms->GlobalSystems()->Get< ModulesFactory >();
	
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
	create_gpp->moduleID	= ids.pipeline;
	create_gpp->topology	= EPrimitive::TriangleList;
	create_gpp->renderPass	= framebuffer->GetModuleByMsg< RenderPassMsgList_t >();

	pipelineTemplate1->Send( create_gpp );
	gpipeline1 = create_gpp->result.Get();

	CHECK_ERR( factory->Create(
					ids.resourceTable,
					ms->GlobalSystems(),
					CreateInfo::PipelineResourceTable{},
					OUT resourceTable1 )
	);

	resourceTable1->Send< ModuleMsg::AttachModule >({ "pipeline", gpipeline1 });
	resourceTable1->Send< ModuleMsg::AttachModule >({ "un_ColorTexture", texture });
	resourceTable1->Send< ModuleMsg::AttachModule >({ "un_ColorTexture.sampler", sampler });
	return true;
}


bool GApp::_CreatePipeline2 ()
{
	auto	gthread = GetGThread( ms->GlobalSystems() );
	auto	factory = ms->GlobalSystems()->Get< ModulesFactory >();
	
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
	create_gpp->moduleID	= ids.pipeline;
	create_gpp->topology	= EPrimitive::TriangleList;
	create_gpp->vertexInput.Add( "at_Position", &Vertex::position )
							.Add( "at_Texcoord", &Vertex::texcoord )
							.Bind( "", SizeOf<Vertex>() );
	
	pipelineTemplate2->Send( create_gpp );
	gpipeline2 = create_gpp->result.Get();

	CHECK_ERR( factory->Create(
					ids.resourceTable,
					ms->GlobalSystems(),
					CreateInfo::PipelineResourceTable{},
					OUT resourceTable2 )
	);

	resourceTable2->Send< ModuleMsg::AttachModule >({ "pipeline", gpipeline2 });
	resourceTable2->Send< ModuleMsg::AttachModule >({ "un_ColorTexture", fbColorImage });
	resourceTable2->Send< ModuleMsg::AttachModule >({ "un_ColorTexture.sampler", sampler });
	resourceTable2->Send< ModuleMsg::AttachModule >({ "ub", ubuffer });
	return true;
}


bool GApp::_CreateCmdBuffers ()
{
	auto	gthread = GetGThread( ms->GlobalSystems() );
	auto	factory = ms->GlobalSystems()->Get< ModulesFactory >();

	CHECK_ERR( factory->Create(
					ids.commandBuilder,
					gthread->GlobalSystems(),
					CreateInfo::GpuCommandBuilder{ gthread },
					OUT cmdBuilder )
	);
	
	cmdBufIndex = 0;
	cmdBuffers.Resize( 6 );

	FOR( i, cmdBuffers )
	{
		CHECK_ERR( factory->Create(
						ids.commandBuffer,
						gthread->GlobalSystems(),
						CreateInfo::GpuCommandBuffer{ gthread },
						OUT cmdBuffers[i] )
		);
		cmdBuilder->Send< ModuleMsg::AttachModule >({ ""_str << i, cmdBuffers[i] });
	}
	return true;
}


bool GApp::_GInit (const Message< GpuMsg::DeviceCreated > &)
{
	auto	gthread = GetGThread( ms->GlobalSystems() );
	auto	factory = gthread->GlobalSystems()->Get< ModulesFactory >();

	CHECK_ERR( factory->Create(
					ids.image,
					gthread->GlobalSystems(),
					CreateInfo::GpuImage{	ImageDescriptor{
												EImage::Tex2D,
												uint4( 1024, 1024, 0, 0 ),
												EPixelFormat::RGBA8_UNorm,
												EImageUsage::bits() | EImageUsage::Sampled | EImageUsage::ColorAttachment
											},
											EGpuMemory::bits() | EGpuMemory::LocalInGPU,
											EMemoryAccess::GpuRead | EMemoryAccess::GpuWrite },
					OUT fbColorImage ) );
	/*
	CHECK_ERR( factory->Create(
					ids.image,
					gthread->GlobalSystems(),
					CreateInfo::GpuImage{	ImageDescriptor{
												EImage::Tex2D,
												uint4( 4, 4, 0, 0 ),
												EPixelFormat::RGBA8_UNorm,
												EImageUsage::bits() | EImageUsage::Sampled | EImageUsage::TransferDst
											},
											EGpuMemory::bits() | EGpuMemory::CoherentWithCPU,
											EMemoryAccess::All },
					OUT texture ) );
	/*/
	CHECK_ERR( factory->Create(
					ids.image,
					gthread->GlobalSystems(),
					CreateInfo::GpuImage{	ImageDescriptor{
												EImage::Tex2D,
												uint4( 128, 128, 0, 0 ),
												EPixelFormat::RGBA8_UNorm,
												EImageUsage::bits() | EImageUsage::Sampled | EImageUsage::TransferDst
											},
											EGpuMemory::bits() | EGpuMemory::LocalInGPU,
											EMemoryAccess::GpuRead | EMemoryAccess::GpuWrite },
					OUT texture ) );
	//*/
	CHECK_ERR( factory->Create(
					ids.sampler,
					gthread->GlobalSystems(),
					CreateInfo::GpuSampler{ gthread,
											SamplerDescriptor::Builder()
												.SetAddressMode( EAddressMode::Clamp )
												.SetFilter( EFilter::MinMagMipLinear )
												.Finish()
										  },
					OUT sampler ) );

	CHECK_ERR( factory->Create(
					ids.buffer,
					gthread->GlobalSystems(),
					CreateInfo::GpuBuffer{	BufferDescriptor{
												SizeOf<Vertex>() * 4,
												EBufferUsage::bits() | EBufferUsage::Vertex
											},
											EGpuMemory::bits() | EGpuMemory::CoherentWithCPU,
											EMemoryAccess::CpuRead | EMemoryAccess::CpuWrite  },
					OUT vbuffer ) );

	CHECK_ERR( factory->Create(
					ids.buffer,
					gthread->GlobalSystems(),
					CreateInfo::GpuBuffer{	BufferDescriptor{
												SizeOf<uint>() * 6,
												EBufferUsage::bits() | EBufferUsage::Index
											},
											EGpuMemory::bits() | EGpuMemory::CoherentWithCPU,
											EMemoryAccess::CpuRead | EMemoryAccess::CpuWrite },
					OUT ibuffer ) );

	CHECK_ERR( factory->Create(
					ids.buffer,
					gthread->GlobalSystems(),
					CreateInfo::GpuBuffer{	BufferDescriptor{
												SizeOf<Pipelines::UB>(),
												EBufferUsage::bits() | EBufferUsage::Uniform
											},
											EGpuMemory::bits() | EGpuMemory::CoherentWithCPU,
											EMemoryAccess::CpuRead | EMemoryAccess::CpuWrite },
					OUT ubuffer ) );

	CHECK_ERR( factory->Create(
					ids.framebuffer,
					gthread->GlobalSystems(),
					CreateInfo::GpuFramebuffer{
						gthread,
						uint2(1024, 1024), 1
					},
					OUT framebuffer ) );
	
	ImageViewDescriptor	view_descr;
	//view_descr.swizzle = "RRRR"_Swizzle;

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

	vbuffer->Send< GpuMsg::WriteToGpuMemory >({ vertices });
	ibuffer->Send< GpuMsg::WriteToGpuMemory >({ indices });
	ubuffer->Send< GpuMsg::WriteToGpuMemory >({ BinArrayCRef::FromValue(ub_data) });
	//texture->Send< GpuMsg::WriteToImageMemory >({ BinArrayCRef::From(pixels), uint4(4,4,0,0), 4_b });

	return true;
}


bool GApp::_GDelete (const Message< GpuMsg::DeviceBeforeDestroy > &)
{
	return true;
}
