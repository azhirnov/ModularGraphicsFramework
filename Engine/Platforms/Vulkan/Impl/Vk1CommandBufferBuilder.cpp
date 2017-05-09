// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/Impl/Vk1CommandBufferBuilder.h"
#include "Engine/Platforms/Vulkan/VulkanThread.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	const Runtime::VirtualTypeList	Vk1CommandBufferBuilder::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	Vk1CommandBufferBuilder::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1CommandBufferBuilder::Vk1CommandBufferBuilder (const GlobalSystemsRef gs, const CreateInfo::GpuCommandBufferBuilder &ci) :
		Vk1BaseModule( gs, ci.gpuThread, ModuleConfig{ GetStaticID(), ~0u }, &_msgTypes, &_eventTypes ),
		_cmdPool( VK_NULL_HANDLE ),
		_scope( EScope::None ),
		_subpassIndex( 0 ),
		_maxSubpasses( 0 )
	{
		SetDebugName( "Vk1CommandBufferBuilder" );

		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_AttachModule_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_DetachModule_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_FindModule_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_Link_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_Compose );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_Delete );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_GpuDeviceBeforeDestory );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferSetViewport );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferSetScissor );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferSetDepthBounds );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferSetBlendColor );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferSetDepthBias );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferSetLineWidth );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferSetStencilCompareMask );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferSetStencilWriteMask );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferSetStencilReference );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferBegin );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferEnd );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferBeginRenderPass );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferBeginRenderPassID );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferEndRenderPass );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferNextSubpass );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferBindPipeline );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferBindGraphicsPipelineID );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferBindComputePipelineID );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferBindVertexBuffers );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferBindVertexBufferIDs );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferBindIndexBuffer );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferBindIndexBufferID );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferDraw );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferDrawIndexed );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferDrawIndirect );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferDrawIndexedIndirect );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferDrawIndirectID );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferDrawIndexedIndirectID );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferExecute );
		_SubscribeOnMsg( this, &Vk1CommandBufferBuilder::_CommandBufferExecuteID );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, VulkanThread::GetStaticID(), true );
	}
		
/*
=================================================
	desctructor
=================================================
*/
	Vk1CommandBufferBuilder::~Vk1CommandBufferBuilder ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool Vk1CommandBufferBuilder::_Compose (const  Message< ModuleMsg::Compose > &msg)
	{
		CHECK_ERR( GetState() == EState::Linked );

		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );

		CHECK_ERR( _CreateCmdBufferPool() );

		CHECK( _SetState( EState::ComposedMutable ) );
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool Vk1CommandBufferBuilder::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyCmdBufferPool();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_CommandBufferSetViewport
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferSetViewport (const Message< ModuleMsg::CommandBufferSetViewport > &msg)
	{
		using namespace vk;
		
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::Viewport ) );

		Viewports_t		viewports;		viewports.Resize( msg->viewports.Count() );

		FOR( i, msg->viewports )
		{
			auto const&	vp = msg->viewports[i];

			viewports[i] = VkViewport{	float(vp.offset.x), float(vp.offset.y),
										float(vp.size.x), float(vp.size.y),
										vp.depthRange.x, vp.depthRange.y };
		}

		vkCmdSetViewport( _cmdBuffer->GetCmdBufferID(), msg->firstViewport, viewports.Count(), viewports.ptr() );
		return true;
	}
	
/*
=================================================
	_CommandBufferSetScissor
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferSetScissor (const Message< ModuleMsg::CommandBufferSetScissor > &msg)
	{
		using namespace vk;
		
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::Scissor ) );

		Scissors_t		scissors;		scissors.Resize( msg->scissors.Count() );

		FOR( i, msg->scissors )
		{
			auto const&	sc = msg->scissors[i];

			scissors[i] = VkRect2D{ VkOffset2D{ int(sc.left), int(sc.bottom) }, VkExtent2D{ sc.Width(), sc.Height() } };
		}

		vkCmdSetScissor( _cmdBuffer->GetCmdBufferID(), msg->firstScissor, scissors.Count(), scissors.ptr() );
		return true;
	}
	
/*
=================================================
	_CommandBufferSetDepthBounds
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferSetDepthBounds (const Message< ModuleMsg::CommandBufferSetDepthBounds > &msg)
	{
		using namespace vk;
		
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::DepthBounds ) );

		vkCmdSetDepthBounds( _cmdBuffer->GetCmdBufferID(), msg->min, msg->max );
		return true;
	}
	
/*
=================================================
	_CommandBufferSetBlendColor
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferSetBlendColor (const Message< ModuleMsg::CommandBufferSetBlendColor > &msg)
	{
		using namespace vk;
		
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::BlendConstants ) );
		
		vkCmdSetBlendConstants( _cmdBuffer->GetCmdBufferID(), msg->color.ptr() );
		return true;
	}
	
/*
=================================================
	_CommandBufferSetDepthBias
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferSetDepthBias (const Message< ModuleMsg::CommandBufferSetDepthBias > &msg)
	{
		using namespace vk;
		
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::DepthBias ) );
		
		vkCmdSetDepthBias( _cmdBuffer->GetCmdBufferID(), msg->biasConstFactor, msg->biasClamp, msg->biasSlopeFactor );
		return true;
	}
	
/*
=================================================
	_CommandBufferSetLineWidth
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferSetLineWidth (const Message< ModuleMsg::CommandBufferSetLineWidth > &msg)
	{
		using namespace vk;
		
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::LineWidth ) );
		
		vkCmdSetLineWidth( _cmdBuffer->GetCmdBufferID(), msg->width );
		return true;
	}
	
/*
=================================================
	_CommandBufferSetStencilCompareMask
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferSetStencilCompareMask (const Message< ModuleMsg::CommandBufferSetStencilCompareMask > &msg)
	{
		using namespace vk;
		
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::StencilCompareMask ) );
		
		VkStencilFaceFlagBits	flags;
		Vk1Enum( msg->face, OUT flags );

		vkCmdSetStencilCompareMask( _cmdBuffer->GetCmdBufferID(), flags, msg->cmpMask );
		return true;
	}

/*
=================================================
	_CommandBufferSetStencilWriteMask
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferSetStencilWriteMask (const Message< ModuleMsg::CommandBufferSetStencilWriteMask > &msg)
	{
		using namespace vk;
		
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::StencilWriteMask ) );
		
		VkStencilFaceFlagBits	flags;
		Vk1Enum( msg->face, OUT flags );

		vkCmdSetStencilWriteMask( _cmdBuffer->GetCmdBufferID(), flags, msg->mask );
		return true;
	}
	
/*
=================================================
	_CommandBufferSetStencilReference
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferSetStencilReference (const Message< ModuleMsg::CommandBufferSetStencilReference > &msg)
	{
		using namespace vk;
		
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::StencilReference ) );
		
		VkStencilFaceFlagBits	flags;
		Vk1Enum( msg->face, OUT flags );
		
		vkCmdSetStencilReference( _cmdBuffer->GetCmdBufferID(), flags, msg->ref );
		return true;
	}
	
/*
=================================================
	_CommandBufferBegin
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferBegin (const Message< ModuleMsg::CommandBufferBegin > &msg)
	{
		CHECK_ERR( not _cmdBuffer );
		CHECK_ERR( _scope == EScope::None );

		_cmdBuffer = GXTypes::New< Vk1CommandBuffer >( GlobalSystems(), CreateInfo::GpuCommandBuffer{ _GetManager(), this, msg->descr } );
		_cmdBuffer->Send( Message< ModuleMsg::Link >() );
		_cmdBuffer->Send( Message< ModuleMsg::Compose >() );

		CHECK_ERR( _cmdBuffer->_BeginRecording() );

		_scope = EScope::Command;
		return true;
	}
	
/*
=================================================
	_CommandBufferEnd
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferEnd (const Message< ModuleMsg::CommandBufferEnd > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		
		CHECK_ERR( _cmdBuffer->_EndRecording() );

		msg->cmdBuffer.Set( _cmdBuffer );

		_cmdBuffer	= null;
		_scope		= EScope::None;
		return true;
	}
	
/*
=================================================
	_CommandBufferBeginRenderPass
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferBeginRenderPass (const Message< ModuleMsg::CommandBufferBeginRenderPass > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( msg->framebuffer );
		CHECK_ERR( msg->renderPass );
		CHECK_ERR( _scope == EScope::Command );

		// requests
		Message< ModuleMsg::GetGpuFramebufferID >			fb_request;
		Message< ModuleMsg::GetGpuFramebufferDescriptor >	fb_descr_request;
		Message< ModuleMsg::GetGpuRenderPassID >			rp_request;
		Message< ModuleMsg::GetGpuRenderPassDescriptor >	rp_descr_request;

		msg->framebuffer->Send( fb_request );
		msg->framebuffer->Send( fb_descr_request );

		msg->renderPass->Send( rp_request );
		msg->renderPass->Send( rp_descr_request );

		auto const&	fb_descr	= fb_descr_request->result.Get();
		auto const&	rp_descr	= rp_descr_request->result.Get();

		CHECK_ERR( _CheckCompatibility( fb_descr, rp_descr ) );
		

		// prepare clear values
		ClearValues_t	clear_values;

		clear_values.Resize( fb_descr.colorAttachments.Count() + usize(fb_descr.depthStencilAttachment.IsEnabled()) );

		FOR( i, clear_values )
		{
			if ( i < msg->clearValues.Count() )
				MemCopy( BinArrayRef::FromValue( clear_values[i] ), msg->clearValues[i].GetData() );	// TODO
			else
				ZeroMem( clear_values[i] );
		}


		// create render pass
		VkRenderPassBeginInfo	pass_info = {};
		pass_info.sType						= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		pass_info.renderPass				<< rp_request->result;
		pass_info.renderArea.offset.x		= msg->area.left;
		pass_info.renderArea.offset.y		= msg->area.bottom;
		pass_info.renderArea.extent.width	= msg->area.Width();
		pass_info.renderArea.extent.height	= msg->area.Height();
		pass_info.clearValueCount			= clear_values.Count();
		pass_info.pClearValues				= clear_values.ptr();
		pass_info.framebuffer				<< fb_request->result;
		
		vkCmdBeginRenderPass( _cmdBuffer->GetCmdBufferID(), &pass_info, VK_SUBPASS_CONTENTS_INLINE );


		// chenge states
		_cmdBuffer->_resources.Add( msg->framebuffer );
		_cmdBuffer->_resources.Add( msg->renderPass );

		_scope			= EScope::RenderPass;
		_dynamicStates	= Uninitialized;
		_subpassIndex	= 0;
		_maxSubpasses	= 1;	// TODO
		return true;
	}
	
/*
=================================================
	_CommandBufferBeginRenderPassID
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferBeginRenderPassID (const Message< ModuleMsg::CommandBufferBeginRenderPassID > &msg)
	{
		TODO( "" );
		return true;
	}
	
/*
=================================================
	_CommandBufferEndRenderPass
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferEndRenderPass (const Message< ModuleMsg::CommandBufferEndRenderPass > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );

		vkCmdEndRenderPass( _cmdBuffer->GetCmdBufferID() );

		_scope			= EScope::Command;
		_dynamicStates	= Uninitialized;
		_subpassIndex	= 0;
		_maxSubpasses	= 0;
		return true;
	}
	
/*
=================================================
	_CommandBufferNextSubpass
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferNextSubpass (const Message< ModuleMsg::CommandBufferNextSubpass > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( ++_subpassIndex < _maxSubpasses );

		vkCmdNextSubpass( _cmdBuffer->GetCmdBufferID(), VK_SUBPASS_CONTENTS_INLINE );
		return true;
	}
	
/*
=================================================
	_CommandBufferBindPipeline
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferBindPipeline (const Message< ModuleMsg::CommandBufferBindPipeline > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg->pipeline );

		Message< ModuleMsg::GetGraphicsPipelineID >			id_request;
		Message< ModuleMsg::GetGraphicsPipelineDescriptor >	descr_request;

		msg->pipeline->Send( id_request );
		msg->pipeline->Send( descr_request );

		CHECK_ERR( id_request->result.IsDefined() and descr_request->result.IsDefined() );

		VkPipeline					pipeline	= id_request->result.Get( VK_NULL_HANDLE );
		GraphicsPipelineDescriptor	descr;		descr << descr_request->result;

		CHECK_ERR( descr.subpass == _subpassIndex );

		_cmdBuffer->_resources.Add( msg->pipeline );

		vkCmdBindPipeline( _cmdBuffer->GetCmdBufferID(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline );

		_dynamicStates = descr.dynamicStates;
		return true;
	}
	
/*
=================================================
	_CommandBufferBindGraphicsPipelineID
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferBindGraphicsPipelineID (const Message< ModuleMsg::CommandBufferBindGraphicsPipelineID > &msg)
	{
		TODO( "" );
		return true;
	}
	
/*
=================================================
	_CommandBufferBindComputePipelineID
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferBindComputePipelineID (const Message< ModuleMsg::CommandBufferBindComputePipelineID > &msg)
	{
		TODO( "" );
		return true;
	}
	
/*
=================================================
	_CommandBufferBindVertexBuffers
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferBindVertexBuffers (const Message< ModuleMsg::CommandBufferBindVertexBuffers > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );

		CompileTime::MustBeSameSize< VkDeviceSize, decltype(msg->offsets)::value_t >();

		VertexBuffers_t		buffers;	buffers.Resize( msg->vertexBuffers.Count() );

		FOR( i, msg->vertexBuffers )
		{
			auto const&	vb = msg->vertexBuffers[i];

			Message< ModuleMsg::GetGpuBufferID >	id_request;
			vb->Send( id_request );

			buffers[i] << id_request->result;
		}
		
		vkCmdBindVertexBuffers( _cmdBuffer->GetCmdBufferID(),
								msg->firstBinding,
								buffers.Count(),
								buffers.ptr(),
								reinterpret_cast< VkDeviceSize const *>( msg->offsets.ptr() ) );
		return true;
	}
	
/*
=================================================
	_CommandBufferBindVertexBufferIDs
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferBindVertexBufferIDs (const Message< ModuleMsg::CommandBufferBindVertexBufferIDs > &msg)
	{
		TODO( "" );
		return true;
	}
	
/*
=================================================
	_CommandBufferBindIndexBuffer
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferBindIndexBuffer (const Message< ModuleMsg::CommandBufferBindIndexBuffer > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg->indexBuffer );

		Message< ModuleMsg::GetGpuBufferID >	id_request;
		msg->indexBuffer->Send( id_request );

		vkCmdBindIndexBuffer( _cmdBuffer->GetCmdBufferID(),
							  id_request->result.Get( VK_NULL_HANDLE ),
							  (VkDeviceSize) msg->offset,
							  Vk1Enum( msg->indexType ) );
		return true;
	}
	
/*
=================================================
	_CommandBufferBindIndexBufferID
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferBindIndexBufferID (const Message< ModuleMsg::CommandBufferBindIndexBufferID > &msg)
	{
		TODO( "" );
		return true;
	}
	
/*
=================================================
	_CommandBufferDraw
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferDraw (const Message< ModuleMsg::CommandBufferDraw > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		
		vkCmdDraw( _cmdBuffer->GetCmdBufferID(),
				   msg->vertexCount,
				   msg->instanceCount,
				   msg->firstVertex,
				   msg->firstInstance );
		return true;
	}
	
/*
=================================================
	_CommandBufferDrawIndexed
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferDrawIndexed (const Message< ModuleMsg::CommandBufferDrawIndexed > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		
		vkCmdDrawIndexed( _cmdBuffer->GetCmdBufferID(),
						  msg->indexCount,
						  msg->instanceCount,
						  msg->firstVertex,
						  msg->vertexOffset,
						  msg->firstInstance );
		return true;
	}
	
/*
=================================================
	_CommandBufferDrawIndirect
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferDrawIndirect (const Message< ModuleMsg::CommandBufferDrawIndirect > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg->indirectBuffer );

		Message< ModuleMsg::GetGpuBufferID >	id_request;
		msg->indirectBuffer->Send( id_request );
		
		vkCmdDrawIndirect(	_cmdBuffer->GetCmdBufferID(),
							id_request->result.Get( VK_NULL_HANDLE ),
							(VkDeviceSize) msg->offset,
							msg->drawCount,
							(vk::uint32_t) msg->stride );
		return true;
	}
	
/*
=================================================
	_CommandBufferDrawIndexedIndirect
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferDrawIndexedIndirect (const Message< ModuleMsg::CommandBufferDrawIndexedIndirect > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg->indirectBuffer );

		Message< ModuleMsg::GetGpuBufferID >	id_request;
		msg->indirectBuffer->Send( id_request );
		
		vkCmdDrawIndexedIndirect( _cmdBuffer->GetCmdBufferID(),
								  id_request->result.Get( VK_NULL_HANDLE ),
								  (VkDeviceSize) msg->offset,
								  msg->drawCount,
								  (vk::uint32_t) msg->stride );
		return true;
	}
	
/*
=================================================
	_CommandBufferDrawIndirectID
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferDrawIndirectID (const Message< ModuleMsg::CommandBufferDrawIndirectID > &msg)
	{
		TODO( "" );
		return true;
	}
	
/*
=================================================
	_CommandBufferDrawIndexedIndirectID
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferDrawIndexedIndirectID (const Message< ModuleMsg::CommandBufferDrawIndexedIndirectID > &msg)
	{
		TODO( "" );
		return true;
	}
	
/*
=================================================
	_CommandBufferExecute
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferExecute (const Message< ModuleMsg::CommandBufferExecute > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );

		CmdBuffers_t	cmd_buffers;

		for (usize i = 0; i < msg->cmdBuffers.Count(); i += cmd_buffers.MaxCapacity())
		{
			cmd_buffers.Clear();

			for (usize j = 0; j < cmd_buffers.MaxCapacity(); ++i)
			{
				Message< ModuleMsg::GetGpuCommandBufferID >		id_request;
				msg->cmdBuffers[i + j]->Send( id_request );

				cmd_buffers.PushBack( id_request->result.Get( VK_NULL_HANDLE ) );
			}
				
			vkCmdExecuteCommands( _cmdBuffer->GetCmdBufferID(), cmd_buffers.Count(), cmd_buffers.ptr() );
		}
		return true;
	}
	
/*
=================================================
	_CommandBufferExecuteID
=================================================
*/
	bool Vk1CommandBufferBuilder::_CommandBufferExecuteID (const Message< ModuleMsg::CommandBufferExecuteID > &msg)
	{
		TODO( "" );
		return true;
	}
	
/*
=================================================
	_IsCreated
=================================================
*/
	bool Vk1CommandBufferBuilder::_IsCreated () const
	{
		return _cmdPool != VK_NULL_HANDLE;
	}

/*
=================================================
	_CreateCmdBufferPool
=================================================
*/
	bool Vk1CommandBufferBuilder::_CreateCmdBufferPool ()
	{
		using namespace vk;

		CHECK_ERR( not _IsCreated() );

		VkCommandPoolCreateInfo		pool_info	= {};
		pool_info.sType				= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		pool_info.queueFamilyIndex	= GetDevice()->GetQueueFamilyIndices().graphics;
		pool_info.flags				= 0;	// TODO

		VK_CHECK( vkCreateCommandPool( GetLogicalDevice(), &pool_info, null, OUT &_cmdPool ) );
		return true;
	}

/*
=================================================
	_DestroyCmdBufferPool
=================================================
*/
	void Vk1CommandBufferBuilder::_DestroyCmdBufferPool ()
	{
		using namespace vk;

		auto	dev = GetLogicalDevice();

		if ( dev != VK_NULL_HANDLE and _cmdPool != VK_NULL_HANDLE )
		{
			vkDestroyCommandPool( dev, _cmdPool, null );
		}

		_cmdPool		= VK_NULL_HANDLE;
		_cmdBuffer		= null;
		_scope			= EScope::None;
		_dynamicStates	= Uninitialized;
		_subpassIndex	= 0;
		_maxSubpasses	= 0;
	}
	
/*
=================================================
	_CheckDynamicState
=================================================
*/
	bool Vk1CommandBufferBuilder::_CheckDynamicState (EPipelineDynamicState::type state) const
	{
		return _dynamicStates.Get( state );
	}
	
/*
=================================================
	_CheckCompatibility
=================================================
*/
	bool Vk1CommandBufferBuilder::_CheckCompatibility (const FramebufferDescriptor &fb,
													   const RenderPassDescriptor &rp) const
	{
		CHECK_ERR( fb.colorAttachments.Count() == rp.ColorAttachments().Count() );
		CHECK_ERR( fb.depthStencilAttachment.IsEnabled() == rp.DepthStencilAttachment().IsEnabled() );

		FOR( i, fb.colorAttachments )
		{
			CHECK_ERR( fb.colorAttachments[i].target == 
							ERenderTarget::FromPixelFormat( rp.ColorAttachments()[i].format, i ) );

			CHECK_ERR( ETexture::IsMultisampled( fb.colorAttachments[i].imageType ) ==
							(rp.ColorAttachments()[i].samples > MultiSamples(1)) );
		}

		if ( fb.depthStencilAttachment.IsEnabled() )
		{
			CHECK_ERR( fb.depthStencilAttachment.target == 
							ERenderTarget::FromPixelFormat( rp.DepthStencilAttachment().format ) );

			CHECK_ERR( ETexture::IsMultisampled( fb.depthStencilAttachment.imageType ) ==
							(rp.DepthStencilAttachment().samples > MultiSamples(1)) );
		}

		return true;
	}


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
