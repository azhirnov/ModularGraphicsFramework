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

	input = null;
	window = null;
	thread = null;
	vkthread = null;

	// finish initialization
	ms->Send( Message< ModuleMsg::Link >() );
	ms->Send( Message< ModuleMsg::Compose >() );
}


void VkApp::Quit ()
{
	ms->Send( Message< ModuleMsg::Delete >() );
}


bool VkApp::Update ()
{
	ms->Send( Message< ModuleMsg::Update >() );
	return looping;
}


bool VkApp::_OnWindowClosed (const Message<ModuleMsg::WindowAfterDestroy> &)
{
	looping = false;

	ms->Send( Message< ModuleMsg::Delete >() );
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
	auto			vkthread	= ms->GlobalSystems()->Get< ParallelThread >()->GetModule( "vk1.thrd"_TModID );
	Ptr<Vk1Device>	vkdev		= vkthread.ToPtr< VulkanThread >()->GetDevice();

	vkdev->DrawFrame();
	return true;
}


static bool CompileShader (Ptr< Vk1Device > device, BinArrayCRef spvSource, OUT vk::VkShaderModule &shader)
{
	using namespace vk;

	VkShaderModuleCreateInfo	shader_info = {};
	shader_info.sType		= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shader_info.codeSize	= (usize) spvSource.Size();
	shader_info.pCode		= (vk::uint32_t*) spvSource.ptr();

	VK_CHECK( vkCreateShaderModule( device->GetDevice(), &shader_info, null, OUT &shader ) );
	return true;
}


bool VkApp::_CompileShaders (Ptr< Vk1Device > device)
{
	RFilePtr	file;
	BinaryArray	data;

	CHECK_ERR( ms->GlobalSystems()->Get< FileManager >()->OpenForRead( "frag.spv", OUT file ) );

	data.Resize( (usize) file->Size() );
	file->Read( BinArrayRef( data ) );
	
	CHECK_ERR( CompileShader( device, data, OUT fshader ) );


	CHECK_ERR( ms->GlobalSystems()->Get< FileManager >()->OpenForRead( "vert.spv", OUT file ) );

	data.Resize( (usize) file->Size() );
	file->Read( BinArrayRef( data ) );

	CHECK_ERR( CompileShader( device, data, OUT vshader ) );
	return true;
}


bool VkApp::_CreatePipeline (Ptr< Vk1Device > device)
{
	using namespace vk;

	CHECK_ERR( _CompileShaders( device ) );
	
    VkPipelineShaderStageCreateInfo			vshader_info = {};
    vshader_info.sType	= VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vshader_info.stage	= VK_SHADER_STAGE_VERTEX_BIT;
    vshader_info.module	= vshader;
    vshader_info.pName	= "main";

    VkPipelineShaderStageCreateInfo			fshader_info = {};
    fshader_info.sType	= VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fshader_info.stage	= VK_SHADER_STAGE_FRAGMENT_BIT;
    fshader_info.module	= fshader;
    fshader_info.pName	= "main";

    VkPipelineShaderStageCreateInfo			shader_stages[] = { vshader_info, fshader_info };

    VkPipelineVertexInputStateCreateInfo	vertex_info = {};
    vertex_info.sType							= VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_info.vertexBindingDescriptionCount	= 0;
    vertex_info.vertexAttributeDescriptionCount	= 0;

    VkPipelineInputAssemblyStateCreateInfo	input_assembly = {};
    input_assembly.sType					= VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly.topology					= VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    input_assembly.primitiveRestartEnable	= VK_FALSE;

    VkViewport	viewport = {};
    viewport.x			= 0.0f;
    viewport.y			= 0.0f;
    viewport.width		= (float) device->GetSurfaceSize().x;
    viewport.height		= (float) device->GetSurfaceSize().y;
    viewport.minDepth	= 0.0f;
    viewport.maxDepth	= 1.0f;

    VkRect2D	scissor = {};
    scissor.offset = {0, 0};
	scissor.extent = { device->GetSurfaceSize().x, device->GetSurfaceSize().y };

    VkPipelineViewportStateCreateInfo	viewport_state = {};
    viewport_state.sType			= VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state.viewportCount	= 1;
    viewport_state.pViewports		= &viewport;
    viewport_state.scissorCount		= 1;
    viewport_state.pScissors		= &scissor;

    VkPipelineRasterizationStateCreateInfo	rasterizer = {};
    rasterizer.sType					= VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable			= VK_FALSE;
    rasterizer.rasterizerDiscardEnable	= VK_FALSE;
    rasterizer.polygonMode				= VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth				= 1.0f;
    rasterizer.cullMode					= VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace				= VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable			= VK_FALSE;

    VkPipelineMultisampleStateCreateInfo	multisampling = {};
    multisampling.sType					= VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable	= VK_FALSE;
    multisampling.rasterizationSamples	= VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState		color_blend_attachment = {};
    color_blend_attachment.colorWriteMask	= VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    color_blend_attachment.blendEnable		= VK_FALSE;

    VkPipelineColorBlendStateCreateInfo		color_blending = {};
    color_blending.sType				= VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blending.logicOpEnable		= VK_FALSE;
    color_blending.logicOp				= VK_LOGIC_OP_COPY;
    color_blending.attachmentCount		= 1;
    color_blending.pAttachments			= &color_blend_attachment;
    color_blending.blendConstants[0]	= 0.0f;
    color_blending.blendConstants[1]	= 0.0f;
    color_blending.blendConstants[2]	= 0.0f;
    color_blending.blendConstants[3]	= 0.0f;

    VkPipelineLayoutCreateInfo		pipeline_layout_info = {};
    pipeline_layout_info.sType					= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_info.setLayoutCount			= 0;
    pipeline_layout_info.pushConstantRangeCount	= 0;

    VK_CHECK( vkCreatePipelineLayout( device->GetDevice(), &pipeline_layout_info, null, OUT &pipelineLayout ) );

    VkGraphicsPipelineCreateInfo	pipeline_info = {};
    pipeline_info.sType					= VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_info.stageCount			= 2;
    pipeline_info.pStages				= shader_stages;
    pipeline_info.pVertexInputState		= &vertex_info;
    pipeline_info.pInputAssemblyState	= &input_assembly;
    pipeline_info.pViewportState		= &viewport_state;
    pipeline_info.pRasterizationState	= &rasterizer;
    pipeline_info.pMultisampleState		= &multisampling;
    pipeline_info.pColorBlendState		= &color_blending;
    pipeline_info.layout				= pipelineLayout;
    pipeline_info.renderPass			= device->GetDefaultRenderPass();
    pipeline_info.subpass				= 0;
    pipeline_info.basePipelineHandle	= VK_NULL_HANDLE;

    VK_CHECK( vkCreateGraphicsPipelines( device->GetDevice(), VK_NULL_HANDLE, 1, &pipeline_info, null, OUT &graphicsPipeline ) );

	return true;
}


bool VkApp::_InitCommandBuffers (Ptr< Vk1Device > device)
{
	using namespace vk;

	FOR( i, device->GetCommandBuffers() )
	{
		auto	cmd = device->GetCommandBuffers()[i];
		
        VkCommandBufferBeginInfo	begin_info = {};
        begin_info.sType	= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags	= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

        VK_CALL( vkBeginCommandBuffer( cmd, &begin_info ) );

        VkClearValue				clear_color = {0.0f, 0.0f, 0.0f, 1.0f};
        VkRenderPassBeginInfo		render_pass_info = {};
        render_pass_info.sType				= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_info.renderPass			= device->GetDefaultRenderPass();
        render_pass_info.framebuffer		= device->GetFramebuffers()[i];
        render_pass_info.renderArea.offset	= { 0, 0 };
        render_pass_info.renderArea.extent	= { device->GetSurfaceSize().x, device->GetSurfaceSize().y };
        render_pass_info.clearValueCount	= 1;
        render_pass_info.pClearValues		= &clear_color;

        vkCmdBeginRenderPass( cmd, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE );

            vkCmdBindPipeline( cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline );

            vkCmdDraw( cmd, 3, 1, 0, 0 );

        vkCmdEndRenderPass( cmd );

        VK_CALL( vkEndCommandBuffer( cmd ) );
	}
	return true;
}


bool VkApp::_VkInit (const Message< ModuleMsg::GpuDeviceCreated > &msg)
{
	auto			vkthread	= ms->GlobalSystems()->Get< ParallelThread >()->GetModule( "vk1.thrd"_TModID );
	Ptr<Vk1Device>	vkdev		= vkthread.ToPtr< VulkanThread >()->GetDevice();

	CHECK_ERR( _CreatePipeline( vkdev ) );
	CHECK_ERR( _InitCommandBuffers( vkdev ) );

	return true;
}


bool VkApp::_VkDelete (const Message< ModuleMsg::GpuDeviceBeforeDestory > &msg)
{
	using namespace vk;
	
	auto			vkthread	= ms->GlobalSystems()->Get< ParallelThread >()->GetModule( "vk1.thrd"_TModID );
	Ptr<Vk1Device>	device		= vkthread.ToPtr< VulkanThread >()->GetDevice();

	if ( graphicsPipeline )
	{
		vkDestroyPipeline( device->GetDevice(), graphicsPipeline, null );
		graphicsPipeline = VK_NULL_HANDLE;
	}

	if ( pipelineLayout )
	{
		vkDestroyPipelineLayout( device->GetDevice(), pipelineLayout, null );
		pipelineLayout = VK_NULL_HANDLE;
	}

	if ( vshader )
	{
		vkDestroyShaderModule( device->GetDevice(), vshader, null );
		vshader = VK_NULL_HANDLE;
	}

	if ( fshader )
	{
		vkDestroyShaderModule( device->GetDevice(), fshader, null );
		fshader = VK_NULL_HANDLE;
	}
	return true;
}
