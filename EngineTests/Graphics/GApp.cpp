// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "GApp.h"
#include "Pipelines/all_pipelines.h"
#include "Engine/Profilers/Engine.Profilers.h"
#include "Engine/ImportExport/Engine.ImportExport.h"


struct Vertex1 : Graphics::DefVertices::Vertex2D
{
	int		_padding[ (CompileTime::AlignToLarge< uint, sizeof(Self), 16 > - sizeof(Self)) / sizeof(int) ];		// TODO

	Vertex1 ()
	{}

	Vertex1 (const float2 &pos, const float2 &texc, const color4u &color) :
		Vertex2D{ pos, texc, color }
	{}

	ND_ static VertexInputState  GetAttribs ()
	{
		return VertexInputState()
			.Add( "at_Position",	&Self::position )
			.Add( "at_Texcoord",	&Self::texcoord )
			.Add( "at_Color",		&Self::color,	true )
			.Bind( "", SizeOf<Vertex1> );
	}

	ND_ static TypeId GetTypeId ()
	{
		return TypeIdOf< Vertex1 >();
	}
};

using Rectangle1	= Graphics::DefPrimitives::Rectangle< Vertex1 >;




/*
=================================================
	GetGThread
=================================================
*/
inline ModulePtr GetGThread (GlobalSystemsRef gs)
{
	using GThreadMsgList_t		= ModuleMsg::MessageListFrom< GpuMsg::ThreadBeginFrame, GpuMsg::ThreadEndFrame, GpuMsg::GetDeviceInfo >;
	using GThreadEventList_t	= ModuleMsg::MessageListFrom< GpuMsg::DeviceCreated, GpuMsg::DeviceBeforeDestroy >;

	return gs->parallelThread->GetModuleByMsgEvent< GThreadMsgList_t, GThreadEventList_t >();
}

/*
=================================================
	constructor
=================================================
*/
GApp::GApp ()
{
	ms = GetMainSystemInstance();

	Platforms::RegisterPlatforms();
	Profilers::RegisterProfilers();
	//ImportExport::RegisterImportExport();
	Graphics::RegisterGraphics();
}

/*
=================================================
	Initialize
=================================================
*/
bool GApp::Initialize (GAPI::type api)
{
	auto	factory	= ms->GlobalSystems()->modulesFactory;

	ms->AddModule( 0, CreateInfo::Platform{} );
	ms->AddModule( InputManagerModuleID, CreateInfo::InputManager{} );
	ms->AddModule( DataProviderManagerModuleID, CreateInfo::DataProviderManager{} );
	
	{
		ModulePtr	context;
		CHECK_ERR( factory->Create( 0, ms->GlobalSystems(), CreateInfo::GpuContext{ api }, OUT context ) );
		ms->Send( ModuleMsg::AttachModule{ context });

		ids = context->Request( GpuMsg::GetGraphicsModules{} ).graphics;
	}

	auto		thread	= ms->GlobalSystems()->parallelThread;
	
	ModulePtr	window;
	factory->Create( 0, ms->GlobalSystems(), CreateInfo::Window{}, OUT window );
	thread->Send( ModuleMsg::AttachModule{ window });

	thread->AddModule( InputThreadModuleID, CreateInfo::InputThread{} );
	thread->AddModule( ids.thread, CreateInfo::GpuThread{
					   GraphicsSettings{
							api,
							CreateInfo::GpuContext::EFlags::DebugContext
						} } );

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

/*
=================================================
	Quit
=================================================
*/
void GApp::Quit ()
{
	if ( pipelineTemplate )
		pipelineTemplate->Send( ModuleMsg::Delete{} );
	
	auto	thread		= ms->GlobalSystems()->parallelThread;
	auto	gthread		= thread->GetModuleByID( ids.thread );

	if ( gthread )
		gthread->UnsubscribeAll( this );

	looping = false;
}

/*
=================================================
	Update
=================================================
*/
bool GApp::Update ()
{
	if ( not looping )
		return false;

	ms->Send( ModuleMsg::Update{} );
	return true;
}

/*
=================================================
	_Draw
=================================================
*/
bool GApp::_Draw (const ModuleMsg::Update &)
{
	if ( not looping )
		return false;
	
	GraphicsMsg::CmdBeginFrame	begin_frame;
	cmdBuilder->Send( begin_frame );

	ModulePtr	system_fb	= begin_frame.result->framebuffer;
	ModulePtr	render_pass	= system_fb->GetModuleByMsg< RenderPassMsgList_t >();
	
	GpuMsg::GetFramebufferDescription	fb_descr_request;
	system_fb->Send( fb_descr_request );
	
	auto const&	fb_descr	= *fb_descr_request.result;
	RectU		area		= RectU( 0, 0, fb_descr.size.x, fb_descr.size.y );

	cmdBuilder->Send( GraphicsMsg::CmdBegin{} );

	// draw batches
	{
		batchRenderer->Send( GraphicsMsg::BeginBatchRenderer{} );
		
		batchRenderer->Send( GraphicsMsg::BatchRendererSetCustomMaterial{ "1", gpipeline, resourceTable });
		batchRenderer->Send( GraphicsMsg::AddBatch{ Rectangle1{ RectF(-0.2f,  0.1f, 0.6f, 0.7f), RectF(0.0f, 0.0f, 1.0f, 1.0f), color4u(0, 255, 0, 0) } });
		batchRenderer->Send( GraphicsMsg::AddBatch{ Rectangle1{ RectF(-0.6f, -0.6f, 0.0f, 0.0f), RectF(0.0f, 0.0f, 1.0f, 1.0f), color4u(0, 0, 255, 0) } });
		
		batchRenderer->Send( GraphicsMsg::BatchRendererSetCustomMaterial{ "2", gpipeline, resourceTable });
		batchRenderer->Send( GraphicsMsg::AddBatch{ Rectangle1{ RectF(-0.1f, -0.1f, 0.7f, 0.5f), RectF(0.0f, 0.0f, 1.0f, 1.0f), color4u(255, 0, 0, 0) } });

		batchRenderer->Send( GraphicsMsg::FlushBatchRenderer{ system_fb, area, cmdBuilder });
	}

	cmdBuilder->Send( GraphicsMsg::CmdEnd{} );
	cmdBuilder->Send( GraphicsMsg::CmdEndFrame{} );

	return true;
}

/*
=================================================
	_GraphicsInit
=================================================
*/
bool GApp::_GraphicsInit (const GpuMsg::DeviceCreated &)
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

/*
=================================================
	_GraphicsDelete
=================================================
*/
bool GApp::_GraphicsDelete (const GpuMsg::DeviceBeforeDestroy &)
{
	return true;
}

/*
=================================================
	_OnWindowClosed
=================================================
*/
bool GApp::_OnWindowClosed (const OSMsg::WindowAfterDestroy &)
{
	looping = false;
	return true;
}

/*
=================================================
	_CreatePipeline
=================================================
*/
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

	GpuMsg::CreateGraphicsPipeline	create_gpp;
	create_gpp.gpuThread	= gthread;
	create_gpp.moduleID		= ids.pipeline;
	create_gpp.topology		= EPrimitive::TriangleList;
	create_gpp.vertexInput	= Vertex1::GetAttribs();
	
	pipelineTemplate->Send( create_gpp );
	gpipeline = *create_gpp.result;

	CHECK_ERR( factory->Create(
					ids.resourceTable,
					gthread->GlobalSystems(),
					CreateInfo::PipelineResourceTable(),
					OUT resourceTable ) );
	
	CHECK_ERR( factory->Create(
					ids.image,
					gthread->GlobalSystems(),
					CreateInfo::GpuImage{
						ImageDescription{ EImage::Tex2D, uint4( 128, 128, 0, 0 ), EPixelFormat::RGBA8_UNorm, EImageUsage::Sampled | EImageUsage::TransferDst },
						EGpuMemory::LocalInGPU,
						EMemoryAccess::GpuRead | EMemoryAccess::GpuWrite },
					OUT texture ) );

	CHECK_ERR( factory->Create(
					ids.sampler,
					gthread->GlobalSystems(),
					CreateInfo::GpuSampler{	SamplerDescription::Builder()
						.SetAddressMode( EAddressMode::Clamp )
						.SetFilter( EFilter::MinMagMipLinear )
						.Finish() },
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
	
	resourceTable->Send( ModuleMsg::AttachModule{ "pipeline", gpipeline });
	resourceTable->Send( ModuleMsg::AttachModule{ "un_ColorTexture", texture });
	resourceTable->Send( ModuleMsg::AttachModule{ "un_ColorTexture.sampler", sampler });

	ModuleUtils::Initialize({ texture, sampler, cmdBuilder, asyncCmdBuilder, gpipeline, resourceTable });
	
	// initialize texture data
	{
		asyncCmdBuilder->Send( GraphicsMsg::CmdBeginAsync{ GraphicsMsg::CmdBeginAsync::EMode::Sync });

		asyncCmdBuilder->Send( GpuMsg::CmdPipelineBarrier{ EPipelineStage::TopOfPipe, EPipelineStage::Transfer }
								.AddImage({	texture,
											EPipelineAccess::bits(), EPipelineAccess::TransferWrite,
											EImageLayout::Undefined, EImageLayout::TransferDstOptimal,
											EImageAspect::Color }) );

		asyncCmdBuilder->Send( GpuMsg::CmdClearColorImage{ texture, EImageLayout::TransferDstOptimal }
								.Clear( float4(1.0f) ).AddRange({ EImageAspect::Color, 0_mipmap }) );

		asyncCmdBuilder->Send( GpuMsg::CmdPipelineBarrier{ EPipelineStage::Transfer, EPipelineStage::FragmentShader }
								.AddImage({	texture,
											EPipelineAccess::TransferWrite, EPipelineAccess::ShaderRead,
											EImageLayout::TransferDstOptimal, EImageLayout::ShaderReadOnlyOptimal,
											EImageAspect::Color }) );

		asyncCmdBuilder->Send( GraphicsMsg::CmdEndAsync{} );
	}
	return true;
}
