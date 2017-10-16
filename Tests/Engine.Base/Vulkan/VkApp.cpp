// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "VkApp.h"
#include "../Pipelines/all_pipelines.h"


VkApp::VkApp ()
{
	ms = GetMainSystemInstace();

	Platforms::RegisterPlatforms();
}


void VkApp::Initialize ()
{
	ms->AddModule( WinPlatformModuleID, CreateInfo::Platform() );
	ms->AddModule( InputManagerModuleID, CreateInfo::InputManager() );
	ms->AddModule( StreamManagerModuleID, CreateInfo::StreamManager() );
	ms->AddModule( VkContextModuleID, CreateInfo::GpuContext() );

	auto	thread	= ms->GlobalSystems()->Get< ParallelThread >();
	
	thread->AddModule( WinWindowModuleID, CreateInfo::Window() );
	thread->AddModule( InputThreadModuleID, CreateInfo::InputThread() );
	thread->AddModule( VkThreadModuleID, CreateInfo::GpuThread() );

	auto	window		= thread->GetModuleByID( WinWindowModuleID );
	auto	input		= thread->GetModuleByID( InputThreadModuleID );
	auto	vkthread	= thread->GetModuleByID( VkThreadModuleID );

	window->AddModule( WinKeyInputModuleID, CreateInfo::RawInputHandler() );
	window->AddModule( WinMouseInputModuleID, CreateInfo::RawInputHandler() );

	window->Subscribe( this, &VkApp::_OnWindowClosed );
	input->Subscribe( this, &VkApp::_OnKey );
	//input->Subscribe( this, &VkApp::_OnMotion );
	vkthread->Subscribe( this, &VkApp::_Draw );
	vkthread->Subscribe( this, &VkApp::_VkInit );
	vkthread->Subscribe( this, &VkApp::_VkDelete );

	// finish initialization
	ModuleUtils::Initialize({ ms });
}


void VkApp::Quit ()
{
	Message< ModuleMsg::Delete >	del_msg;

	if ( pipelineTemplate )
		pipelineTemplate->Send( del_msg );

	auto	thread		= ms->GlobalSystems()->Get< ParallelThread >();
	auto	window		= thread->GetModuleByID( WinWindowModuleID );
	auto	input		= thread->GetModuleByID( InputThreadModuleID );
	auto	vkthread	= thread->GetModuleByID( VkThreadModuleID );

	if ( window )
		window->UnsubscribeAll( this );
	
	if ( input )
		input->UnsubscribeAll( this );
	
	if ( vkthread )
		vkthread->UnsubscribeAll( this );

	looping = false;
}


bool VkApp::Update ()
{
	if ( not looping )
		return false;

	ms->Send< ModuleMsg::Update >({});
	return true;
}


bool VkApp::_OnWindowClosed (const Message<ModuleMsg::WindowAfterDestroy> &)
{
	looping = false;
	return true;
}


bool VkApp::_OnKey (const Message< ModuleMsg::InputKey > &msg)
{
	DEBUG_CONSOLE( ("Key '"_str << KeyID::ToString( msg->key ) <<
					"', pressure " << msg->pressure).cstr() );
	return true;
}


bool VkApp::_OnMotion (const Message< ModuleMsg::InputMotion > &msg)
{
	DEBUG_CONSOLE( ("Motion '"_str << MotionID::ToString( msg->motion ) <<
					"', relative " << msg->relative <<
					", absolute " << msg->absolute).cstr() );
	return true;
}


bool VkApp::_Draw (const Message< ModuleMsg::Update > &msg)
{
	if ( not looping )
		return false;

	auto	vkthread	= ms->GlobalSystems()->Get< ParallelThread >()->GetModuleByID( VkThreadModuleID );

	Message< GpuMsg::ThreadBeginFrame >	begin_frame;
	vkthread->Send( begin_frame );

	ModulePtr	framebuffer	= begin_frame->result->framebuffer;
	ModulePtr	builder		= begin_frame->result->commandBuilder;
	ModulePtr	render_pass	= framebuffer->GetModuleByMsg< GpuMsg::GetVkRenderPassID >();

	Message< GpuMsg::GetFramebufferDescriptor >	fb_descr_request;
	framebuffer->Send( fb_descr_request );

	auto const&	fb_descr	= fb_descr_request->result.Get();
	RectU		area		= RectU( 0, 0, fb_descr.size.x, fb_descr.size.y );

	builder->Send< GpuMsg::CmdBegin >({ false, cmdBuffers[cmdBufIndex++ % cmdBuffers.Count()] });

	// clear image
	{
		builder->Send< GpuMsg::CmdPipelineBarrier >({ EPipelineStage::bits() | EPipelineStage::Transfer,  EPipelineStage::bits() | EPipelineStage::Transfer,
					  EImageLayout::Undefined, EImageLayout::TransferDstOptimal, texture, EImageAspect::bits() | EImageAspect::Color });

		builder->Send< GpuMsg::CmdClearColorImage >({ texture, EImageLayout::TransferDstOptimal, float4(0.5f) });

		builder->Send< GpuMsg::CmdPipelineBarrier >({  EPipelineStage::bits() | EPipelineStage::Transfer,  EPipelineStage::AllGraphics,
					  EImageLayout::TransferDstOptimal, EImageLayout::ShaderReadOnlyOptimal, texture, EImageAspect::bits() | EImageAspect::Color });
	}

	builder->Send< GpuMsg::CmdBeginRenderPass >({ render_pass, framebuffer, area });
	builder->Send< GpuMsg::CmdBindGraphicsPipeline >({ graphicsPipeline });
	builder->Send< GpuMsg::CmdBindGraphicsResourceTable >({ resourceTable });
	builder->Send< GpuMsg::CmdSetViewport >({ uint2(), fb_descr.size, float2(0.0f, 1.0f) });
	builder->Send< GpuMsg::CmdSetScissor >({ area });
	builder->Send< GpuMsg::CmdDraw >({ 3u, 1u });
	builder->Send< GpuMsg::CmdEndRenderPass >({});

	Message< GpuMsg::CmdEnd >	cmd_end = {};
	builder->Send( cmd_end );

	vkthread->Send< GpuMsg::ThreadEndFrame >({ framebuffer, InitializerList<ModulePtr>{ cmd_end->cmdBuffer.Get(null) } });
	return true;
}


bool VkApp::_CreatePipeline ()
{
	auto	vkthread = ms->GlobalSystems()->Get< ParallelThread >()->GetModuleByID( VkThreadModuleID );
	auto	factory = ms->GlobalSystems()->Get< ModulesFactory >();

	CreateInfo::PipelineTemplate	pp_templ;
	Pipelines::Create_default( OUT pp_templ.descr );

	CHECK_ERR( factory->Create(
					PipelineTemplateModuleID,
					ms->GlobalSystems(),
					pp_templ,
					OUT pipelineTemplate )
	);

	Message< GpuMsg::CreateGraphicsPipeline >	create_gpp;

	create_gpp->gpuThread	= vkthread;
	create_gpp->topology	= EPrimitive::TriangleList;
	
	pipelineTemplate->Send( create_gpp );
	graphicsPipeline = create_gpp->result.Get();

	CHECK_ERR( factory->Create(
					VkPipelineResourceTableModuleID,
					ms->GlobalSystems(),
					CreateInfo::PipelineResourceTable(),
					OUT resourceTable )
	);

	resourceTable->Send< ModuleMsg::AttachModule >({ "pipeline", graphicsPipeline });
	resourceTable->Send< ModuleMsg::AttachModule >({ "un_ColorTexture", texture });
	resourceTable->Send< ModuleMsg::AttachModule >({ "un_ColorTexture.sampler", sampler });
	return true;
}


bool VkApp::_CreateCmdBuffers ()
{
	auto	vkthread = ms->GlobalSystems()->Get< ParallelThread >()->GetModuleByID( VkThreadModuleID );
	auto	factory = ms->GlobalSystems()->Get< ModulesFactory >();

	CHECK_ERR( factory->Create(
					VkCommandBuilderModuleID,
					vkthread->GlobalSystems(),
					CreateInfo::GpuCommandBuilder{ vkthread },
					OUT cmdBuilder )
	);
	
	cmdBufIndex = 0;
	cmdBuffers.Resize( 6 );

	FOR( i, cmdBuffers )
	{
		CHECK_ERR( factory->Create(
						VkCommandBufferModuleID,
						vkthread->GlobalSystems(),
						CreateInfo::GpuCommandBuffer{ vkthread },
						OUT cmdBuffers[i] )
		);
		cmdBuilder->Send< ModuleMsg::AttachModule >({ ""_str << i, cmdBuffers[i] });
	}
	return true;
}


bool VkApp::_VkInit (const Message< GpuMsg::DeviceCreated > &msg)
{
	auto	vkthread = ms->GlobalSystems()->Get< ParallelThread >()->GetModuleByID( VkThreadModuleID );
	auto	factory	 = vkthread->GlobalSystems()->Get< ModulesFactory >();

	ModulePtr	texmem_module;
	ModulePtr	bufmem_module;
	ModulePtr	buf_module;

	CHECK_ERR( factory->Create(
					VkMemoryModuleID,
					vkthread->GlobalSystems(),
					CreateInfo::GpuMemory{	vkthread,
											128_Mb,
											EGpuMemory::bits() | EGpuMemory::LocalInGPU,
											EMemoryAccess::GpuRead | EMemoryAccess::GpuWrite },
					OUT texmem_module ) );

	CHECK_ERR( factory->Create(
					VkMemoryModuleID,
					vkthread->GlobalSystems(),
					CreateInfo::GpuMemory{	vkthread,
											128_Mb,
											EGpuMemory::bits() | EGpuMemory::LocalInGPU,
											EMemoryAccess::GpuRead | EMemoryAccess::GpuWrite },
					OUT bufmem_module ) );

	CHECK_ERR( factory->Create(
					VkImageModuleID,
					vkthread->GlobalSystems(),
					CreateInfo::GpuImage{	vkthread,
											ImageDescriptor{
												EImage::Tex2D,
												uint4( 128, 128, 0, 0 ),
												EPixelFormat::RGBA8_UNorm,
												EImageUsage::bits() | EImageUsage::Sampled | EImageUsage::TransferDst
											} },
					OUT texture ) );

	CHECK_ERR( factory->Create(
					VkSamplerModuleID,
					vkthread->GlobalSystems(),
					CreateInfo::GpuSampler{ vkthread,
											SamplerDescriptor::Builder()
												.SetAddressMode( EAddressMode::Clamp )
												.SetFilter( EFilter::MinMagMipLinear )
												.Finish()
										  },
					OUT sampler ) );

	CHECK_ERR( factory->Create(
					VkBufferModuleID,
					vkthread->GlobalSystems(),
					CreateInfo::GpuBuffer{	vkthread,
											BufferDescriptor{
												1024_b,
												EBufferUsage::bits() | EBufferUsage::Vertex
											} },
					OUT buf_module ) );

	texture->Send< ModuleMsg::AttachModule >({ texmem_module });
	buf_module->Send< ModuleMsg::AttachModule >({ bufmem_module });
	
	CHECK_ERR( _CreatePipeline() );
	CHECK_ERR( _CreateCmdBuffers() );

	CHECK_ERR( ModuleUtils::Initialize( { texture, sampler, buf_module, graphicsPipeline, resourceTable, cmdBuilder } ) );
	return true;
}


bool VkApp::_VkDelete (const Message< GpuMsg::DeviceBeforeDestroy > &msg)
{
	return true;
}
