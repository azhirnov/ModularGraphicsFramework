// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "GApp.h"
#include "Pipelines/all_pipelines.h"
#include "Engine/Profilers/Engine.Profilers.h"
//#include "Engine/ImportExport/Engine.ImportExport.h"

using Vertex1		= Graphics::DefVertices::Vertex2D;
using Rectangle1	= Graphics::DefPrimitives::Rectangle< Vertex1 >;


inline ModulePtr GetGThread (GlobalSystemsRef gs)
{
	using GThreadMsgList_t		= CompileTime::TypeListFrom< Message<GpuMsg::ThreadBeginFrame>, Message<GpuMsg::ThreadEndFrame>, Message<GpuMsg::GetDeviceInfo> >;
	using GThreadEventList_t	= CompileTime::TypeListFrom< Message<GpuMsg::DeviceCreated>, Message<GpuMsg::DeviceBeforeDestroy> >;

	return gs->parallelThread->GetModuleByMsgEvent< GThreadMsgList_t, GThreadEventList_t >();
}


GApp::GApp ()
{
	ms = GetMainSystemInstance();

	Platforms::RegisterPlatforms();
	Profilers::RegisterProfilers();
	//ImportExport::RegisterImportExport();
	Graphics::RegisterGraphics();
}


bool GApp::Initialize (GAPI::type api)
{
	ms->AddModule( WinPlatformModuleID, CreateInfo::Platform{} );
	ms->AddModule( InputManagerModuleID, CreateInfo::InputManager{} );
	ms->AddModule( StreamManagerModuleID, CreateInfo::StreamManager{} );
	
	{
		auto		factory	= ms->GlobalSystems()->modulesFactory;
		ModulePtr	context;

		CHECK_ERR( factory->Create( 0, ms->GlobalSystems(), CreateInfo::GpuContext{ api }, OUT context ) );
		ms->Send< ModuleMsg::AttachModule >({ context });

		Message< GpuMsg::GetGraphicsModules >	req_ids;
		context->Send( req_ids );
		ids << req_ids->graphics;
	}

	auto	thread	= ms->GlobalSystems()->parallelThread;
	
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
	
	window->AddModule( Profilers::FPSCounterModuleID, CreateInfo::FPSCounter{} );

	gthread->Subscribe( this, &GApp::_GraphicsInit );
	gthread->Subscribe( this, &GApp::_GraphicsDelete );
	gthread->Subscribe( this, &GApp::_Draw );
	window->Subscribe( this, &GApp::_OnWindowClosed );

	// finish initialization
	ModuleUtils::Initialize({ ms });
	return true;
}


void GApp::Quit ()
{
	Message< ModuleMsg::Delete >	del_msg;

	if ( pipelineTemplate )
		pipelineTemplate->Send( del_msg );
	
	auto	thread		= ms->GlobalSystems()->parallelThread;
	auto	gthread		= thread->GetModuleByID( ids.thread );

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


bool GApp::_Draw (const Message< ModuleMsg::Update > &)
{
	if ( not looping )
		return false;
	
	Message< GraphicsMsg::CmdBeginFrame >	begin_frame;
	cmdBuilder->Send( begin_frame );

	ModulePtr	system_fb	= begin_frame->result->framebuffer;
	ModulePtr	render_pass	= system_fb->GetModuleByMsg< RenderPassMsgList_t >();
	
	Message< GpuMsg::GetFramebufferDescriptor >	fb_descr_request;
	system_fb->Send( fb_descr_request );
	
	auto const&	fb_descr	= fb_descr_request->result.Get();
	RectU		area		= RectU( 0, 0, fb_descr.size.x, fb_descr.size.y );

	cmdBuilder->Send< GraphicsMsg::CmdBegin >({});

	// draw batches
	{
		batchRenderer->Send< GraphicsMsg::BeginBatchRenderer >({});
		
		batchRenderer->Send< GraphicsMsg::BatchRendererSetCustomMaterial >({ "1", gpipeline, resourceTable });
		batchRenderer->Send< GraphicsMsg::AddBatch >({ Rectangle1{ RectF(-0.2f,  0.1f, 0.6f, 0.7f), RectF(0.0f, 0.0f, 1.0f, 1.0f), color4u(0, 255, 0, 0) } });
		batchRenderer->Send< GraphicsMsg::AddBatch >({ Rectangle1{ RectF(-0.6f, -0.6f, 0.0f, 0.0f), RectF(0.0f, 0.0f, 1.0f, 1.0f), color4u(0, 0, 255, 0) } });
		
		batchRenderer->Send< GraphicsMsg::BatchRendererSetCustomMaterial >({ "2", gpipeline, resourceTable });
		batchRenderer->Send< GraphicsMsg::AddBatch >({ Rectangle1{ RectF(-0.1f, -0.1f, 0.7f, 0.5f), RectF(0.0f, 0.0f, 1.0f, 1.0f), color4u(255, 0, 0, 0) } });

		batchRenderer->Send< GraphicsMsg::FlushBatchRenderer >({ system_fb, area, cmdBuilder });
	}

	cmdBuilder->Send< GraphicsMsg::CmdEnd >({});
	cmdBuilder->Send< GraphicsMsg::CmdEndFrame >({});

	return true;
}


bool GApp::_GraphicsInit (const Message< GpuMsg::DeviceCreated > &)
{
	auto	factory = ms->GlobalSystems()->modulesFactory;

	CHECK_ERR( factory->Create(
					BatchRendererModuleID,
					ms->GlobalSystems(),
					CreateInfo::BatchRenderer{},
					OUT batchRenderer )
	);
	
	ModuleUtils::Initialize({ batchRenderer });

	_CreatePipeline();
	return true;
}


bool GApp::_GraphicsDelete (const Message< GpuMsg::DeviceBeforeDestroy > &)
{
	return true;
}


bool GApp::_OnWindowClosed (const Message< OSMsg::WindowAfterDestroy > &)
{
	looping = false;
	return true;
}


bool GApp::_CreatePipeline ()
{
	auto	gthread	= GetGThread( ms->GlobalSystems() );
	auto	factory = ms->GlobalSystems()->modulesFactory;
	
	CreateInfo::PipelineTemplate	pp_templ;
	Pipelines::Create_default3( OUT pp_templ.descr );

	CHECK_ERR( factory->Create(
					PipelineTemplateModuleID,
					ms->GlobalSystems(),
					pp_templ,
					OUT pipelineTemplate )
	);

	Message< GpuMsg::CreateGraphicsPipeline >	create_gpp;

	create_gpp->gpuThread	= gthread;
	create_gpp->moduleID	= ids.pipeline;
	create_gpp->topology	= EPrimitive::TriangleList;
	create_gpp->vertexInput.Add( "at_Position",		&Vertex1::position )
							.Add( "at_Texcoord",	&Vertex1::texcoord )
							.Add( "at_Color",		&Vertex1::color,	true )
							.Bind( "",				AlignToLarge( SizeOf<Vertex1>(), 16_b ) );
	
	pipelineTemplate->Send( create_gpp );
	gpipeline = create_gpp->result.Get();

	CHECK_ERR( factory->Create(
					ids.resourceTable,
					gthread->GlobalSystems(),
					CreateInfo::PipelineResourceTable(),
					OUT resourceTable ) );
	
	CHECK_ERR( factory->Create(
					ids.image,
					gthread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescriptor{
							EImage::Tex2D,
							uint4( 128, 128, 0, 0 ),
							EPixelFormat::RGBA8_UNorm,
							EImageUsage::bits() | EImageUsage::Sampled | EImageUsage::TransferDst
						},
						EGpuMemory::bits() | EGpuMemory::LocalInGPU,
						EMemoryAccess::GpuRead | EMemoryAccess::GpuWrite },
					OUT texture ) );

	CHECK_ERR( factory->Create(
					ids.sampler,
					gthread->GlobalSystems(),
					CreateInfo::GpuSampler{
						gthread,
						SamplerDescriptor::Builder()
							.SetAddressMode( EAddressMode::Clamp )
							.SetFilter( EFilter::MinMagMipLinear )
							.Finish()
						},
					OUT sampler ) );

	CHECK_ERR( factory->Create(
					CommandBufferManagerModuleID,
					gthread->GlobalSystems(),
					CreateInfo::CommandBufferManager{ gthread },
					OUT cmdBuilder ) );
	
	CHECK_ERR( factory->Create(
					AsyncCommandBufferModuleID,
					gthread->GlobalSystems(),
					CreateInfo::AsyncCommandBuffer{ gthread, cmdBuilder },
					OUT asyncCmdBuilder ) );
	
	resourceTable->Send< ModuleMsg::AttachModule >({ "pipeline", gpipeline });
	resourceTable->Send< ModuleMsg::AttachModule >({ "un_ColorTexture", texture });
	resourceTable->Send< ModuleMsg::AttachModule >({ "un_ColorTexture.sampler", sampler });

	ModuleUtils::Initialize({ texture, sampler, cmdBuilder, asyncCmdBuilder, gpipeline, resourceTable });
	
	// initialize texture data
	{
		asyncCmdBuilder->Send< GraphicsMsg::CmdBeginAsync >({ GraphicsMsg::CmdBeginAsync::EMode::Sync });

		asyncCmdBuilder->Send< GpuMsg::CmdPipelineBarrier >({ EPipelineStage::bits() | EPipelineStage::Transfer,  EPipelineStage::bits() | EPipelineStage::Transfer,
						EImageLayout::Undefined, EImageLayout::TransferDstOptimal, texture, EImageAspect::bits() | EImageAspect::Color });

		asyncCmdBuilder->Send< GpuMsg::CmdClearColorImage >({ texture, EImageLayout::TransferDstOptimal, float4(1.0f) });

		asyncCmdBuilder->Send< GpuMsg::CmdPipelineBarrier >({  EPipelineStage::bits() | EPipelineStage::Transfer,  EPipelineStage::AllGraphics,
						EImageLayout::TransferDstOptimal, EImageLayout::ShaderReadOnlyOptimal, texture, EImageAspect::bits() | EImageAspect::Color });

		asyncCmdBuilder->Send< GraphicsMsg::CmdEndAsync >({});
	}
	return true;
}
