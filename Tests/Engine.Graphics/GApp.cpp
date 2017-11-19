// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "GApp.h"
#include "Pipelines/all_pipelines.h"

using Vertex1		= Graphics::DefVertices::Vertex2D;
using Rectangle1	= Graphics::DefPrimitives::Rectangle< Vertex1 >;


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
	Graphics::RegisterGraphics();
}


bool GApp::Initialize (GAPI::type api)
{
	ms->AddModule( WinPlatformModuleID, CreateInfo::Platform{} );
	ms->AddModule( InputManagerModuleID, CreateInfo::InputManager{} );
	ms->AddModule( StreamManagerModuleID, CreateInfo::StreamManager{} );
	ms->AddModule( GraphicsContextModuleID, CreateInfo::GraphicsContext{ api } );
	
	auto	thread	= ms->GlobalSystems()->Get< ParallelThread >();

	thread->AddModule( GraphicsThreadModuleID, CreateInfo::GraphicsThread{ null, GraphicsSettings{ api }, CreateInfo::Window{} } );
	
	auto	gthread	= thread->GetModuleByID( GraphicsThreadModuleID );
	
	auto	gcontext = ms->GetModuleByMsg< CompileTime::TypeListFrom<Message<GpuMsg::GetGraphicsModules>> >();

	Message< GpuMsg::GetGraphicsModules >	req_ids;
	gcontext->Send( req_ids );
	ids << req_ids->graphics;

	gthread->Subscribe( this, &GApp::_GraphicsInit );
	gthread->Subscribe( this, &GApp::_GraphicsDelete );
	gthread->Subscribe( this, &GApp::_Draw );
	gthread->Subscribe( this, &GApp::_OnWindowClosed );

	// finish initialization
	ModuleUtils::Initialize({ ms });
	return true;
}


void GApp::Quit ()
{
	Message< ModuleMsg::Delete >	del_msg;

	if ( pipelineTemplate )
		pipelineTemplate->Send( del_msg );
	
	auto	thread	= ms->GlobalSystems()->Get< ParallelThread >();
	auto	gthread	= thread->GetModuleByID( GraphicsThreadModuleID );

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
	
	auto	gthread	= GetGThread( ms->GlobalSystems() );

	Message< GpuMsg::ThreadBeginFrame >	begin_frame;
	gthread->Send( begin_frame );

	ModulePtr	system_fb	= begin_frame->result->framebuffer;
	ModulePtr	builder		= begin_frame->result->commandBuilder;
	ModulePtr	render_pass	= system_fb->GetModuleByMsg< RenderPassMsgList_t >();
	
	Message< GpuMsg::GetFramebufferDescriptor >	fb_descr_request;
	system_fb->Send( fb_descr_request );
	
	auto const&	fb_descr	= fb_descr_request->result.Get();
	RectU		area		= RectU( 0, 0, fb_descr.size.x, fb_descr.size.y );

	const bool	create_cmd	= cmdBuffers.Count() < 5;
	
	if ( create_cmd )
		builder->Send< GpuMsg::CmdBegin >({});
	else
		builder->Send< GpuMsg::CmdBegin >({ cmdBuffers[cmdBufIndex++ % cmdBuffers.Count()] });

	// draw batches
	{
		batchRenderer->Send< GraphicsMsg::BeginBatchRenderer >({});
		
		batchRenderer->Send< GraphicsMsg::BatchRendererSetCustomMaterial >({ "1", gpipeline, resourceTable });
		batchRenderer->Send< GraphicsMsg::AddBatch >({ Rectangle1{ RectF(-0.2f,  0.1f, 0.6f, 0.7f), RectF(0.0f, 0.0f, 1.0f, 1.0f), color4u(0, 255, 0, 0) } });
		batchRenderer->Send< GraphicsMsg::AddBatch >({ Rectangle1{ RectF(-0.6f, -0.6f, 0.0f, 0.0f), RectF(0.0f, 0.0f, 1.0f, 1.0f), color4u(0, 0, 255, 0) } });
		
		batchRenderer->Send< GraphicsMsg::BatchRendererSetCustomMaterial >({ "2", gpipeline, resourceTable });
		batchRenderer->Send< GraphicsMsg::AddBatch >({ Rectangle1{ RectF(-0.1f, -0.1f, 0.7f, 0.5f), RectF(0.0f, 0.0f, 1.0f, 1.0f), color4u(255, 0, 0, 0) } });

		batchRenderer->Send< GraphicsMsg::FlushBatchRenderer >({ system_fb, area, builder });
	}

	Message< GpuMsg::CmdEnd >	cmd_end = {};
	builder->Send( cmd_end );

	if ( create_cmd )
		cmdBuffers.PushBack( cmd_end->cmdBuffer.Get(null) );

	gthread->Send< GpuMsg::ThreadEndFrame >({ system_fb, InitializerList<ModulePtr>{ cmd_end->cmdBuffer.Get(null) } });
	return true;
}


bool GApp::_GraphicsInit (const Message< GpuMsg::DeviceCreated > &)
{
	auto	gthread = ms->GlobalSystems()->Get< ParallelThread >()->GetModuleByID( GraphicsThreadModuleID );
	auto	factory = ms->GlobalSystems()->Get< ModulesFactory >();

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
	auto	factory = ms->GlobalSystems()->Get< ModulesFactory >();
	
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

	ModulePtr	builder;
	ModulePtr	cmd_buffer;
	
	CHECK_ERR( factory->Create(
					ids.commandBuilder,
					gthread->GlobalSystems(),
					CreateInfo::GpuCommandBuilder{ gthread },
					OUT builder ) );

	CHECK_ERR( factory->Create(
					ids.commandBuffer,
					gthread->GlobalSystems(),
					CreateInfo::GpuCommandBuffer{ gthread },
					OUT cmd_buffer ) );

	builder->Send< ModuleMsg::AttachModule >({ "", cmd_buffer });
	
	resourceTable->Send< ModuleMsg::AttachModule >({ "pipeline", gpipeline });
	resourceTable->Send< ModuleMsg::AttachModule >({ "un_ColorTexture", texture });
	resourceTable->Send< ModuleMsg::AttachModule >({ "un_ColorTexture.sampler", sampler });

	ModuleUtils::Initialize({ texture, sampler, builder, cmd_buffer, gpipeline, resourceTable });
	
	builder->Send< GpuMsg::CmdBegin >({ cmd_buffer });

	builder->Send< GpuMsg::CmdPipelineBarrier >({ EPipelineStage::bits() | EPipelineStage::Transfer,  EPipelineStage::bits() | EPipelineStage::Transfer,
					EImageLayout::Undefined, EImageLayout::TransferDstOptimal, texture, EImageAspect::bits() | EImageAspect::Color });

	builder->Send< GpuMsg::CmdClearColorImage >({ texture, EImageLayout::TransferDstOptimal, float4(1.0f) });

	builder->Send< GpuMsg::CmdPipelineBarrier >({  EPipelineStage::bits() | EPipelineStage::Transfer,  EPipelineStage::AllGraphics,
					EImageLayout::TransferDstOptimal, EImageLayout::ShaderReadOnlyOptimal, texture, EImageAspect::bits() | EImageAspect::Color });

	builder->Send< GpuMsg::CmdEnd >({});

	gthread->Send< GpuMsg::SubmitGraphicsQueueCommands >({ cmd_buffer, true });

	Message< ModuleMsg::Delete >	del_msg;

	builder->Send( del_msg );
	cmd_buffer->Send( del_msg );
	return true;
}
