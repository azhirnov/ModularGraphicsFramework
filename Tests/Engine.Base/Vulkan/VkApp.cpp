// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "VkApp.h"


VkApp::VkApp ()
{
	ms = GetMainSystemInstace();

	Platforms::RegisterPlatforms();
}


void VkApp::Initialize ()
{
	ms->AddModule( "win.platform"_GModID, CreateInfo::Platform() );
	ms->AddModule( "input.mngr"_GModID, CreateInfo::InputManager() );
	ms->AddModule( "stream.mngr"_GModID, CreateInfo::StreamManager() );
	ms->AddModule( "vk1.ctx"_GModID, CreateInfo::GpuContext() );

	auto	thread	= ms->GlobalSystems()->Get< ParallelThread >();
	
	thread->AddModule( "win.window"_TModID, CreateInfo::Window() );
	thread->AddModule( "input.thrd"_TModID, CreateInfo::InputThread() );
	thread->AddModule( "vk1.thrd"_TModID, CreateInfo::GpuThread() );

	auto	window		= thread->GetModule( "win.window"_TModID );
	auto	input		= thread->GetModule( "input.thrd"_TModID );
	auto	vkthread	= thread->GetModule( "vk1.thrd"_TModID );

	window->AddModule( "win.keys"_OModID, CreateInfo::RawInputHandler() );
	window->AddModule( "win.mouse"_OModID, CreateInfo::RawInputHandler() );

	window->Subscribe( this, &VkApp::_OnWindowClosed );
	input->Subscribe( this, &VkApp::_OnKey );
	//input->Subscribe( this, &VkApp::_OnMotion );
	vkthread->Subscribe( this, &VkApp::_Draw );
	vkthread->Subscribe( this, &VkApp::_VkInit );
	vkthread->Subscribe( this, &VkApp::_VkDelete );

	// finish initialization
	ms->Send( Message< ModuleMsg::Link >() );
	ms->Send( Message< ModuleMsg::Compose >() );
}


void VkApp::Quit ()
{
	looping = false;
}


bool VkApp::Update ()
{
	ms->Send( Message< ModuleMsg::Update >() );
	return looping;
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
	auto	vkthread	= ms->GlobalSystems()->Get< ParallelThread >()->GetModule( "vk1.thrd"_TModID );

	Message< ModuleMsg::GpuThreadBeginFrame >	begin_frame;
	vkthread->Send( begin_frame );

	ModulePtr	framebuffer	= begin_frame->framebuffer.Get();
	ModulePtr	builder		= begin_frame->commandBuilder.Get();
	ModulePtr	render_pass	= framebuffer->GetModule< ModuleMsg::GetVkRenderPassID >();

	Message< ModuleMsg::GetGpuFramebufferDescriptor >	fb_descr_request;
	framebuffer->Send( fb_descr_request );

	auto const&	fb_descr	= fb_descr_request->result.Get();
	RectU		area		= RectU( 0, 0, fb_descr.size.x, fb_descr.size.y );

	builder->Send( Message< ModuleMsg::GpuCmdBegin >{} );
	builder->Send( Message< ModuleMsg::GpuCmdBeginRenderPass >{ render_pass, framebuffer, area } );
	builder->Send( Message< ModuleMsg::GpuCmdBindPipeline >{ graphicsPipeline } );
	builder->Send( Message< ModuleMsg::GpuCmdSetViewport >{ uint2(), fb_descr.size, float2(0.0f, 1.0f) } );
	builder->Send( Message< ModuleMsg::GpuCmdSetScissor >{ area } );
	builder->Send( Message< ModuleMsg::GpuCmdDraw >{ 3u, 1u } );
	builder->Send( Message< ModuleMsg::GpuCmdEndRenderPass >{} );

	Message< ModuleMsg::GpuCmdEnd >	cmd_end = {};
	builder->Send( cmd_end );

	vkthread->Send( Message< ModuleMsg::GpuThreadEndFrame >{ framebuffer,
															 InitializerList<ModulePtr>{ cmd_end->cmdBuffer.Get(null) }
	} );
	return true;
}


bool VkApp::_CreatePipeline ()
{
	auto	factory = ms->GlobalSystems()->Get< ModulesFactory >();

	CHECK_ERR( factory->Create(
					"vk1.shaders"_OModID,
					ms->GlobalSystems(),
					CreateInfo::GpuShaderModulesFromFile{
						null,
						{ "Vulkan/shader.spv.frag"_str, "Vulkan/shader.spv.vert"_str }
					},
					OUT shaderModules )
	);

	GraphicsPipelineDescriptor	descr;

	descr.patchControlPoints	= 0;
	descr.dynamicStates			= EPipelineDynamicState::bits() | EPipelineDynamicState::Viewport | EPipelineDynamicState::Scissor;

	descr.renderState.inputAssembly.topology	= EPrimitive::TriangleList;
	descr.renderState.rasterization.polygonMode	= EPolygonMode::Fill;


	CHECK_ERR( factory->Create(
					"vk1.g-ppln"_OModID,
					ms->GlobalSystems(),
					CreateInfo::GraphicsPipeline{
						null,
						descr,
						shaderModules,
						null,
						0
					},
					OUT graphicsPipeline )
	);
	return true;
}


bool VkApp::_VkInit (const Message< ModuleMsg::GpuDeviceCreated > &msg)
{
	auto	vkthread = ms->GlobalSystems()->Get< ParallelThread >()->GetModule( "vk1.thrd"_TModID );
	auto	factory	 = vkthread->GlobalSystems()->Get< ModulesFactory >();

	CHECK_ERR( _CreatePipeline() );
	
	/*CHECK_ERR( vkthread->GlobalSystems()->Get< ModulesFactory >()->Create(
					"vk1.cmdbld"_OModID,
					vkthread->GlobalSystems(),
					CreateInfo::GpuCommandBuilder{ vkthread },
					OUT commandBuilder ) );*/

	ModulePtr	texmem_module;
	ModulePtr	bufmem_module;
	ModulePtr	tex_module;
	ModulePtr	buf_module;

	CHECK_ERR( factory->Create(
					"vk1.memory"_OModID,
					vkthread->GlobalSystems(),
					CreateInfo::GpuMemory{	vkthread,
											BytesUL::FromMb( 128 ),
											EGpuMemory::bits() | EGpuMemory::LocalInGPU,
											EMemoryAccess::bits() | EMemoryAccess::GpuRead | EMemoryAccess::GpuWrite },
					OUT texmem_module ) );

	CHECK_ERR( factory->Create(
					"vk1.memory"_OModID,
					vkthread->GlobalSystems(),
					CreateInfo::GpuMemory{	vkthread,
											BytesUL::FromMb( 128 ),
											EGpuMemory::bits() | EGpuMemory::LocalInGPU,
											EMemoryAccess::bits() | EMemoryAccess::GpuRead | EMemoryAccess::GpuWrite },
					OUT bufmem_module ) );

	CHECK_ERR( factory->Create(
					"vk1.texture"_OModID,
					vkthread->GlobalSystems(),
					CreateInfo::GpuTexture{	vkthread,
											TextureDescriptor{
												ETexture::Tex2D,
												uint4( 128, 128, 0, 0 ),
												EPixelFormat::RGBA8_UNorm,
												EImageUsage::bits() | EImageUsage::Sampled
											} },
					OUT tex_module ) );

	CHECK_ERR( factory->Create(
					"vk1.buffer"_OModID,
					vkthread->GlobalSystems(),
					CreateInfo::GpuBuffer{	vkthread,
											BufferDescriptor{
												BytesUL(1024),
												EBufferUsage::bits() | EBufferUsage::Vertex
											} },
					OUT buf_module ) );

	tex_module->Send( Message< ModuleMsg::AttachModule >{ texmem_module } );
	buf_module->Send( Message< ModuleMsg::AttachModule >{ bufmem_module } );

	Array<ModulePtr>	to_initialize{ /*commandBuilder,*/ tex_module, buf_module };

	FOR( i, to_initialize )
	{
		to_initialize[i]->Send( Message< ModuleMsg::Link >() );
		to_initialize[i]->Send( Message< ModuleMsg::Compose >() );
	}

	return true;
}


bool VkApp::_VkDelete (const Message< ModuleMsg::GpuDeviceBeforeDestory > &msg)
{
	return true;
}
