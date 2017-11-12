// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "VkApp.h"
#include "../Pipelines/all_pipelines.h"


struct Vertex
{
	float2	position;
	float2	texcoord;
};

inline ModulePtr GetVkThread (GlobalSystemsRef gs)
{
	using VkThreadMsgList_t		= Module::MessageListFrom< GpuMsg::ThreadBeginFrame, GpuMsg::ThreadEndFrame, GpuMsg::GetDeviceInfo >;
	using VkThreadEventList_t	= Module::MessageListFrom< GpuMsg::DeviceCreated, GpuMsg::DeviceBeforeDestroy >;

	return gs->Get< ParallelThread >()->GetModuleByMsgEvent< VkThreadMsgList_t, VkThreadEventList_t >();
}


VkApp::VkApp ()
{
	ms = GetMainSystemInstace();

	Platforms::RegisterPlatforms();
}


void VkApp::Initialize ()
{
	ms->AddModule( WinPlatformModuleID, CreateInfo::Platform{} );
	ms->AddModule( InputManagerModuleID, CreateInfo::InputManager{} );
	ms->AddModule( StreamManagerModuleID, CreateInfo::StreamManager{} );
	ms->AddModule( VkContextModuleID, CreateInfo::GpuContext{} );

	auto	thread	= ms->GlobalSystems()->Get< ParallelThread >();
	
	thread->AddModule( WinWindowModuleID, CreateInfo::Window{} );
	thread->AddModule( InputThreadModuleID, CreateInfo::InputThread{} );
	thread->AddModule( VkThreadModuleID, CreateInfo::GpuThread{ null, null, 
					   CreateInfo::GpuContext{
							"VK 1.0"_GAPI/*,
							CreateInfo::GpuContext::EFlags::bits() | CreateInfo::GpuContext::EFlags::DebugContext*/
						} } );

	auto	window		= thread->GetModuleByID( WinWindowModuleID );
	auto	input		= thread->GetModuleByID( InputThreadModuleID );
	auto	vkthread	= thread->GetModuleByID( VkThreadModuleID );

	window->AddModule( WinKeyInputModuleID, CreateInfo::RawInputHandler{} );
	window->AddModule( WinMouseInputModuleID, CreateInfo::RawInputHandler{} );

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

	if ( pipelineTemplate1 )
		pipelineTemplate1->Send( del_msg );
	
	if ( pipelineTemplate2 )
		pipelineTemplate2->Send( del_msg );

	auto	thread		= ms->GlobalSystems()->Get< ParallelThread >();
	auto	window		= thread->GetModuleByID( WinWindowModuleID );
	auto	input		= thread->GetModuleByID( InputThreadModuleID );
	auto	vkthread	= GetVkThread( ms->GlobalSystems() );

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


bool VkApp::_OnWindowClosed (const Message<OSMsg::WindowAfterDestroy> &)
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


bool VkApp::_Draw (const Message< ModuleMsg::Update > &)
{
	if ( not looping )
		return false;

	auto	vkthread	= GetVkThread( ms->GlobalSystems() );

	Message< GpuMsg::ThreadBeginFrame >	begin_frame;
	vkthread->Send( begin_frame );

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

		builder->Send< GpuMsg::CmdBeginRenderPass >({ render_pass, framebuffer, RectU(area.Center().x, area.bottom, area.right, area.top) });
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

	vkthread->Send< GpuMsg::ThreadEndFrame >({ system_fb, InitializerList<ModulePtr>{ cmd_end->cmdBuffer.Get(null) } });
	return true;
}


bool VkApp::_CreatePipeline1 ()
{
	auto	vkthread = GetVkThread( ms->GlobalSystems() );
	auto	factory	 = ms->GlobalSystems()->Get< ModulesFactory >();
	
	CreateInfo::PipelineTemplate	pp_templ;
	Pipelines::Create_default( OUT pp_templ.descr );

	CHECK_ERR( factory->Create(
					PipelineTemplateModuleID,
					ms->GlobalSystems(),
					pp_templ,
					OUT pipelineTemplate1 )
	);

	Message< GpuMsg::CreateGraphicsPipeline >	create_gpp;
	
	create_gpp->gpuThread	= vkthread;
	create_gpp->moduleID	= VkGraphicsPipelineModuleID;
	create_gpp->topology	= EPrimitive::TriangleList;
	create_gpp->renderPass	= framebuffer->GetModuleByMsg< RenderPassMsgList_t >();

	pipelineTemplate1->Send( create_gpp );
	gpipeline1 = create_gpp->result.Get();

	CHECK_ERR( factory->Create(
					VkPipelineResourceTableModuleID,
					ms->GlobalSystems(),
					CreateInfo::PipelineResourceTable{},
					OUT resourceTable1 )
	);

	resourceTable1->Send< ModuleMsg::AttachModule >({ "pipeline", gpipeline1 });
	resourceTable1->Send< ModuleMsg::AttachModule >({ "un_ColorTexture", texture });
	resourceTable1->Send< ModuleMsg::AttachModule >({ "un_ColorTexture.sampler", sampler });
	return true;
}


bool VkApp::_CreatePipeline2 ()
{
	auto	vkthread = GetVkThread( ms->GlobalSystems() );
	auto	factory  = ms->GlobalSystems()->Get< ModulesFactory >();
	
	CreateInfo::PipelineTemplate	pp_templ;
	Pipelines::Create_default2( OUT pp_templ.descr );

	CHECK_ERR( factory->Create(
					PipelineTemplateModuleID,
					ms->GlobalSystems(),
					pp_templ,
					OUT pipelineTemplate2 )
	);

	Message< GpuMsg::CreateGraphicsPipeline >	create_gpp;

	create_gpp->gpuThread	= vkthread;
	create_gpp->moduleID	= VkGraphicsPipelineModuleID;
	create_gpp->topology	= EPrimitive::TriangleList;
	create_gpp->vertexInput.Add( "at_Position", &Vertex::position )
							.Add( "at_Texcoord", &Vertex::texcoord )
							.Bind( "", SizeOf<Vertex>() );
	
	pipelineTemplate2->Send( create_gpp );
	gpipeline2 = create_gpp->result.Get();

	CHECK_ERR( factory->Create(
					VkPipelineResourceTableModuleID,
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


bool VkApp::_CreateCmdBuffers ()
{
	auto	vkthread = GetVkThread( ms->GlobalSystems() );
	auto	factory  = ms->GlobalSystems()->Get< ModulesFactory >();

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


bool VkApp::_VkInit (const Message< GpuMsg::DeviceCreated > &)
{
	auto	vkthread = GetVkThread( ms->GlobalSystems() );
	auto	factory	 = vkthread->GlobalSystems()->Get< ModulesFactory >();

	ModulePtr	texmem_module;
	ModulePtr	imgmem_module;
	ModulePtr	vbufmem_module;
	ModulePtr	ibufmem_module;
	ModulePtr	ubufmem_module;

	CHECK_ERR( factory->Create(
					VkImageModuleID,
					vkthread->GlobalSystems(),
					CreateInfo::GpuImage{	vkthread,
											ImageDescriptor{
												EImage::Tex2D,
												uint4( 1024, 1024, 0, 0 ),
												EPixelFormat::RGBA8_UNorm,
												EImageUsage::bits() | EImageUsage::Sampled | EImageUsage::ColorAttachment
											} },
					OUT fbColorImage ) );

	CHECK_ERR( factory->Create(
					VkMemoryModuleID,
					vkthread->GlobalSystems(),
					CreateInfo::GpuMemory{	vkthread,
											EGpuMemory::bits() | EGpuMemory::LocalInGPU,
											EMemoryAccess::GpuRead | EMemoryAccess::GpuWrite },
					OUT imgmem_module ) );

	CHECK_ERR( factory->Create(
					VkMemoryModuleID,
					vkthread->GlobalSystems(),
					CreateInfo::GpuMemory{	vkthread,
											EGpuMemory::bits() | EGpuMemory::LocalInGPU,
											EMemoryAccess::GpuRead | EMemoryAccess::GpuWrite },
					OUT texmem_module ) );

	CHECK_ERR( factory->Create(
					VkMemoryModuleID,
					vkthread->GlobalSystems(),
					CreateInfo::GpuMemory{	vkthread,
											EGpuMemory::bits() | EGpuMemory::CoherentWithCPU,
											EMemoryAccess::CpuRead | EMemoryAccess::CpuWrite },
					OUT vbufmem_module ) );

	CHECK_ERR( factory->Create(
					VkMemoryModuleID,
					vkthread->GlobalSystems(),
					CreateInfo::GpuMemory{	vkthread,
											EGpuMemory::bits() | EGpuMemory::CoherentWithCPU,
											EMemoryAccess::CpuRead | EMemoryAccess::CpuWrite },
					OUT ibufmem_module ) );

	CHECK_ERR( factory->Create(
					VkMemoryModuleID,
					vkthread->GlobalSystems(),
					CreateInfo::GpuMemory{	vkthread,
											EGpuMemory::bits() | EGpuMemory::CoherentWithCPU,
											EMemoryAccess::CpuRead | EMemoryAccess::CpuWrite },
					OUT ubufmem_module ) );

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
												SizeOf<Vertex>() * 4,
												EBufferUsage::bits() | EBufferUsage::Vertex
											} },
					OUT vbuffer ) );

	CHECK_ERR( factory->Create(
					VkBufferModuleID,
					vkthread->GlobalSystems(),
					CreateInfo::GpuBuffer{	vkthread,
											BufferDescriptor{
												SizeOf<uint>() * 6,
												EBufferUsage::bits() | EBufferUsage::Index
											} },
					OUT ibuffer ) );

	CHECK_ERR( factory->Create(
					VkBufferModuleID,
					vkthread->GlobalSystems(),
					CreateInfo::GpuBuffer{	vkthread,
											BufferDescriptor{
												SizeOf<Pipelines::UB>(),
												EBufferUsage::bits() | EBufferUsage::Uniform
											} },
					OUT ubuffer ) );

	CHECK_ERR( factory->Create(
					VkFramebufferModuleID,
					vkthread->GlobalSystems(),
					CreateInfo::GpuFramebuffer{
						vkthread,
						uint2(1024, 1024), 1,
						{{ "Color0", fbColorImage }}
					},
					OUT framebuffer ) );
	
	fbColorImage->Send< ModuleMsg::AttachModule >({ imgmem_module });
	texture->Send< ModuleMsg::AttachModule >({ texmem_module });
	vbuffer->Send< ModuleMsg::AttachModule >({ vbufmem_module });
	ibuffer->Send< ModuleMsg::AttachModule >({ ibufmem_module });
	ubuffer->Send< ModuleMsg::AttachModule >({ ubufmem_module });
	
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

	Pipelines::UB	ub_data;	ub_data.color = float4(1.0f);

	using EMappingFlags = GpuMsg::MapMemoryToCpu::EMappingFlags;
	/*
	vbuffer->Send< GpuMsg::MapMemoryToCpu >({ EMappingFlags::Write, SizeOf(vertices) });
	vbuffer->Send< ModuleMsg::WriteToStream >({ vertices });
	vbuffer->Send< GpuMsg::UnmapMemory >({});

	ibuffer->Send< GpuMsg::MapMemoryToCpu >({ EMappingFlags::Write, SizeOf(indices) });
	ibuffer->Send< ModuleMsg::WriteToStream >({ indices });
	ibuffer->Send< GpuMsg::UnmapMemory >({});

	ubuffer->Send< GpuMsg::MapMemoryToCpu >({ EMappingFlags::Write, SizeOf(ub_data) });
	ubuffer->Send< ModuleMsg::WriteToStream >({ BinArrayCRef::FromValue(ub_data) });
	ubuffer->Send< GpuMsg::UnmapMemory >({});*/
	
	vbuffer->Send< GpuMsg::WriteToGpuMemory >({ vertices });
	ibuffer->Send< GpuMsg::WriteToGpuMemory >({ indices });
	ubuffer->Send< GpuMsg::WriteToGpuMemory >({ BinArrayCRef::FromValue(ub_data) });

	return true;
}


bool VkApp::_VkDelete (const Message< GpuMsg::DeviceBeforeDestroy > &)
{
	return true;
}
