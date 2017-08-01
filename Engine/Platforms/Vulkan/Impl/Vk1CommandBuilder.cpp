// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/Impl/Vk1CommandBuilder.h"
#include "Engine/Platforms/Shared/GPU/Texture.h"
#include "Engine/Platforms/Shared/GPU/Buffer.h"
#include "Engine/Platforms/Shared/GPU/Framebuffer.h"
#include "Engine/Platforms/Shared/GPU/RenderPass.h"
#include "Engine/Platforms/Vulkan/VulkanThread.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	
	using Viewports_t			= FixedSizeArray< vk::VkViewport, GlobalConst::Graphics_MaxColorBuffers >;
	using Scissors_t			= FixedSizeArray< vk::VkRect2D, GlobalConst::Graphics_MaxColorBuffers >;
	using ClearValues_t			= FixedSizeArray< vk::VkClearValue, GlobalConst::Graphics_MaxColorBuffers >;
	using VertexBuffers_t		= FixedSizeArray< vk::VkBuffer, GlobalConst::Graphics_MaxAttribs >;
	using CmdBuffers_t			= FixedSizeArray< vk::VkCommandBuffer, ModuleMsg::GpuCmdExecute::CmdBuffers_t::MemoryContainer_t::SIZE >;
	using BufferCopyRegions_t	= FixedSizeArray< vk::VkBufferCopy, ModuleMsg::GpuCmdCopyBuffer::Regions_t::MemoryContainer_t::SIZE >;
	using ImageCopyRegions_t	= FixedSizeArray< vk::VkImageCopy, ModuleMsg::GpuCmdCopyImage::Regions_t::MemoryContainer_t::SIZE >;
	using BufImgCopyRegions_t	= FixedSizeArray< vk::VkBufferImageCopy, ModuleMsg::GpuCmdCopyBufferToImage::Regions_t::MemoryContainer_t::SIZE >;
	using ImgBlitRegions_t		= FixedSizeArray< vk::VkImageBlit, ModuleMsg::GpuCmdBlitImage::Regions_t::MemoryContainer_t::SIZE >;
	using ClearAttachments_t	= FixedSizeArray< vk::VkClearAttachment, ModuleMsg::GpuCmdClearAttachments::Attachments_t::MemoryContainer_t::SIZE >;
	using ClearRects_t			= FixedSizeArray< vk::VkClearRect, ModuleMsg::GpuCmdClearAttachments::ClearRects_t::MemoryContainer_t::SIZE >;
	using ImageRanges_t			= FixedSizeArray< vk::VkImageSubresourceRange, ModuleMsg::GpuCmdClearColorImage::Ranges_t::MemoryContainer_t::SIZE >;


	const Runtime::VirtualTypeList	Vk1CommandBuilder::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	Vk1CommandBuilder::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1CommandBuilder::Vk1CommandBuilder (const GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci) :
		Vk1BaseModule( gs, ci.gpuThread, ModuleConfig{ GetStaticID(), ~0u }, &_msgTypes, &_eventTypes ),
		_cmdPool( VK_NULL_HANDLE ),
		_scope( EScope::None ),
		_subpassIndex( 0 ),
		_maxSubpasses( 0 )
	{
		SetDebugName( "Vk1CommandBuilder" );

		_SubscribeOnMsg( this, &Vk1CommandBuilder::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_AttachModule_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_DetachModule_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_FindModule_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_Link_Impl );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_Compose );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_Delete );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuDeviceBeforeDestory );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdSetViewport );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdSetScissor );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdSetDepthBounds );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdSetBlendColor );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdSetDepthBias );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdSetLineWidth );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdSetStencilCompareMask );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdSetStencilWriteMask );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdSetStencilReference );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdBegin );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdEnd );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdBeginRenderPass );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdBeginRenderPassID );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdEndRenderPass );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdNextSubpass );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdBindPipeline );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdBindGraphicsPipelineID );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdBindComputePipelineID );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdBindVertexBuffers );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdBindVertexBufferIDs );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdBindIndexBuffer );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdBindIndexBufferID );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdDraw );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdDrawIndexed );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdDrawIndirect );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdDrawIndexedIndirect );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdDrawIndirectID );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdDrawIndexedIndirectID );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdExecute );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdExecuteID );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdBindDescriptorSet );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdCopyBuffer );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdCopyImage );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdCopyBufferToImage );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdCopyImageToBuffer );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdBlitImage );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdUpdateBuffer );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdFillBuffer );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdClearAttachments );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdClearColorImage );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GpuCmdClearDepthStencilImage );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, VulkanThread::GetStaticID(), true );
	}
		
/*
=================================================
	desctructor
=================================================
*/
	Vk1CommandBuilder::~Vk1CommandBuilder ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool Vk1CommandBuilder::_Compose (const  Message< ModuleMsg::Compose > &msg)
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
	bool Vk1CommandBuilder::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyCmdBufferPool();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GpuCmdSetViewport
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdSetViewport (const Message< ModuleMsg::GpuCmdSetViewport > &msg)
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
	_GpuCmdSetScissor
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdSetScissor (const Message< ModuleMsg::GpuCmdSetScissor > &msg)
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
	_GpuCmdSetDepthBounds
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdSetDepthBounds (const Message< ModuleMsg::GpuCmdSetDepthBounds > &msg)
	{
		using namespace vk;
		
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::DepthBounds ) );

		vkCmdSetDepthBounds( _cmdBuffer->GetCmdBufferID(), msg->min, msg->max );
		return true;
	}
	
/*
=================================================
	_GpuCmdSetBlendColor
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdSetBlendColor (const Message< ModuleMsg::GpuCmdSetBlendColor > &msg)
	{
		using namespace vk;
		
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::BlendConstants ) );
		
		vkCmdSetBlendConstants( _cmdBuffer->GetCmdBufferID(), msg->color.ptr() );
		return true;
	}
	
/*
=================================================
	_GpuCmdSetDepthBias
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdSetDepthBias (const Message< ModuleMsg::GpuCmdSetDepthBias > &msg)
	{
		using namespace vk;
		
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::DepthBias ) );
		
		vkCmdSetDepthBias( _cmdBuffer->GetCmdBufferID(), msg->biasConstFactor, msg->biasClamp, msg->biasSlopeFactor );
		return true;
	}
	
/*
=================================================
	_GpuCmdSetLineWidth
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdSetLineWidth (const Message< ModuleMsg::GpuCmdSetLineWidth > &msg)
	{
		using namespace vk;
		
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::LineWidth ) );
		
		vkCmdSetLineWidth( _cmdBuffer->GetCmdBufferID(), msg->width );
		return true;
	}
	
/*
=================================================
	_GpuCmdSetStencilCompareMask
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdSetStencilCompareMask (const Message< ModuleMsg::GpuCmdSetStencilCompareMask > &msg)
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
	_GpuCmdSetStencilWriteMask
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdSetStencilWriteMask (const Message< ModuleMsg::GpuCmdSetStencilWriteMask > &msg)
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
	_GpuCmdSetStencilReference
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdSetStencilReference (const Message< ModuleMsg::GpuCmdSetStencilReference > &msg)
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
	_GpuCmdBegin
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdBegin (const Message< ModuleMsg::GpuCmdBegin > &msg)
	{
		CHECK_ERR( not _cmdBuffer );
		CHECK_ERR( _scope == EScope::None );

		_cmdBuffer = New< Vk1CommandBuffer >( GlobalSystems(), CreateInfo::GpuCommandBuffer{ _GetManager(), this, msg->descr } );
		SendTo( _cmdBuffer, Message< ModuleMsg::Link >() );
		SendTo( _cmdBuffer, Message< ModuleMsg::Compose >() );

		CHECK_ERR( _cmdBuffer->_BeginRecording() );

		_scope = EScope::Command;
		return true;
	}
	
/*
=================================================
	_GpuCmdEnd
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdEnd (const Message< ModuleMsg::GpuCmdEnd > &msg)
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
	_GpuCmdBeginRenderPass
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdBeginRenderPass (const Message< ModuleMsg::GpuCmdBeginRenderPass > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( msg->framebuffer );
		CHECK_ERR( msg->renderPass );
		CHECK_ERR( _scope == EScope::Command );

		// requests
		Message< ModuleMsg::GetVkFramebufferID >			fb_request;
		Message< ModuleMsg::GetGpuFramebufferDescriptor >	fb_descr_request;
		Message< ModuleMsg::GetVkRenderPassID >				rp_request;
		Message< ModuleMsg::GetGpuRenderPassDescriptor >	rp_descr_request;

		SendTo( msg->framebuffer, fb_request );
		SendTo( msg->framebuffer, fb_descr_request );

		SendTo( msg->renderPass, rp_request );
		SendTo( msg->renderPass, rp_descr_request );

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
		_maxSubpasses	= rp_descr.Subpasses().Count();
		return true;
	}
	
/*
=================================================
	_GpuCmdBeginRenderPassID
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdBeginRenderPassID (const Message< ModuleMsg::GpuCmdBeginRenderPassID > &msg)
	{
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_GpuCmdEndRenderPass
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdEndRenderPass (const Message< ModuleMsg::GpuCmdEndRenderPass > &msg)
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
	_GpuCmdNextSubpass
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdNextSubpass (const Message< ModuleMsg::GpuCmdNextSubpass > &msg)
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
	_GpuCmdBindPipeline
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdBindPipeline (const Message< ModuleMsg::GpuCmdBindPipeline > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg->pipeline );

		Message< ModuleMsg::GetVkGraphicsPipelineID >		id_request;
		Message< ModuleMsg::GetGraphicsPipelineDescriptor >	descr_request;

		SendTo( msg->pipeline, id_request );
		SendTo( msg->pipeline, descr_request );

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
	_GpuCmdBindGraphicsPipelineID
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdBindGraphicsPipelineID (const Message< ModuleMsg::GpuCmdBindGraphicsPipelineID > &msg)
	{
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_GpuCmdBindComputePipelineID
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdBindComputePipelineID (const Message< ModuleMsg::GpuCmdBindComputePipelineID > &msg)
	{
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_GpuCmdBindVertexBuffers
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdBindVertexBuffers (const Message< ModuleMsg::GpuCmdBindVertexBuffers > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );

		CompileTime::MustBeSameSize< VkDeviceSize, decltype(msg->offsets)::value_t >();

		VertexBuffers_t		buffers;	buffers.Resize( msg->vertexBuffers.Count() );

		FOR( i, msg->vertexBuffers )
		{
			auto const&	vb = msg->vertexBuffers[i];

			Message< ModuleMsg::GetVkBufferID >				id_request;
			Message< ModuleMsg::GetGpuBufferDescriptor >	descr_request;

			SendTo( vb, id_request );
			SendTo( vb, descr_request );

			CHECK_ERR( descr_request->result.Get().usage.Get( EBufferUsage::Vertex ) );

			buffers[i] << id_request->result;
			
			_cmdBuffer->_resources.Add( vb );
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
	_GpuCmdBindVertexBufferIDs
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdBindVertexBufferIDs (const Message< ModuleMsg::GpuCmdBindVertexBufferIDs > &msg)
	{
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_GpuCmdBindIndexBuffer
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdBindIndexBuffer (const Message< ModuleMsg::GpuCmdBindIndexBuffer > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg->indexBuffer );

		Message< ModuleMsg::GetVkBufferID >				id_request;
		Message< ModuleMsg::GetGpuBufferDescriptor >	descr_request;

		SendTo( msg->indexBuffer, id_request );
		SendTo( msg->indexBuffer, descr_request );
		
		CHECK_ERR( descr_request->result.Get().usage.Get( EBufferUsage::Index ) );
		
		_cmdBuffer->_resources.Add( msg->indexBuffer );

		vkCmdBindIndexBuffer( _cmdBuffer->GetCmdBufferID(),
							  id_request->result.Get( VK_NULL_HANDLE ),
							  (VkDeviceSize) msg->offset,
							  Vk1Enum( msg->indexType ) );
		return true;
	}
	
/*
=================================================
	_GpuCmdBindIndexBufferID
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdBindIndexBufferID (const Message< ModuleMsg::GpuCmdBindIndexBufferID > &msg)
	{
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_GpuCmdDraw
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdDraw (const Message< ModuleMsg::GpuCmdDraw > &msg)
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
	_GpuCmdDrawIndexed
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdDrawIndexed (const Message< ModuleMsg::GpuCmdDrawIndexed > &msg)
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
	_GpuCmdDrawIndirect
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdDrawIndirect (const Message< ModuleMsg::GpuCmdDrawIndirect > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg->indirectBuffer );

		Message< ModuleMsg::GetVkBufferID >				id_request;
		Message< ModuleMsg::GetGpuBufferDescriptor >	descr_request;

		SendTo( msg->indirectBuffer, id_request );
		SendTo( msg->indirectBuffer, descr_request );
		
		CHECK_ERR( descr_request->result.Get().usage.Get( EBufferUsage::Indirect ) );
		
		_cmdBuffer->_resources.Add( msg->indirectBuffer );

		vkCmdDrawIndirect(	_cmdBuffer->GetCmdBufferID(),
							id_request->result.Get( VK_NULL_HANDLE ),
							(VkDeviceSize) msg->offset,
							msg->drawCount,
							(vk::uint32_t) msg->stride );
		return true;
	}
	
/*
=================================================
	_GpuCmdDrawIndexedIndirect
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdDrawIndexedIndirect (const Message< ModuleMsg::GpuCmdDrawIndexedIndirect > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg->indirectBuffer );

		Message< ModuleMsg::GetVkBufferID >				id_request;
		Message< ModuleMsg::GetGpuBufferDescriptor >	descr_request;

		SendTo( msg->indirectBuffer, id_request );
		SendTo( msg->indirectBuffer, descr_request );
		
		CHECK_ERR( descr_request->result.Get().usage.Get( EBufferUsage::Indirect ) );
		
		_cmdBuffer->_resources.Add( msg->indirectBuffer );

		vkCmdDrawIndexedIndirect( _cmdBuffer->GetCmdBufferID(),
								  id_request->result.Get( VK_NULL_HANDLE ),
								  (VkDeviceSize) msg->offset,
								  msg->drawCount,
								  (vk::uint32_t) msg->stride );
		return true;
	}
	
/*
=================================================
	_GpuCmdDrawIndirectID
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdDrawIndirectID (const Message< ModuleMsg::GpuCmdDrawIndirectID > &msg)
	{
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_GpuCmdDrawIndexedIndirectID
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdDrawIndexedIndirectID (const Message< ModuleMsg::GpuCmdDrawIndexedIndirectID > &msg)
	{
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_GpuCmdExecute
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdExecute (const Message< ModuleMsg::GpuCmdExecute > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );

		CmdBuffers_t	cmd_buffers;

		FOR( i, msg->cmdBuffers )
		{
			auto const&	cmd = msg->cmdBuffers[i];

			Message< ModuleMsg::GetVkCommandBufferID >		id_request;
			SendTo( cmd, id_request );

			cmd_buffers.PushBack( id_request->result.Get( VK_NULL_HANDLE ) );

			_cmdBuffer->_resources.Add( cmd );
		}
			
		vkCmdExecuteCommands( _cmdBuffer->GetCmdBufferID(), cmd_buffers.Count(), cmd_buffers.ptr() );
		return true;
	}
	
/*
=================================================
	_GpuCmdExecuteID
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdExecuteID (const Message< ModuleMsg::GpuCmdExecuteID > &msg)
	{
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_GpuCmdBindDescriptorSet
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdBindDescriptorSet (const Message< ModuleMsg::GpuCmdBindDescriptorSet > &msg)
	{
		return false;
	}
	
/*
=================================================
	_GpuCmdCopyBuffer
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdCopyBuffer (const Message< ModuleMsg::GpuCmdCopyBuffer > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->srcBuffer and msg->dstBuffer );
		
		Message< ModuleMsg::GetVkBufferID >				src_id_request;
		Message< ModuleMsg::GetVkBufferID >				dst_id_request;
		Message< ModuleMsg::GetGpuBufferDescriptor >	src_descr_request;
		Message< ModuleMsg::GetGpuBufferDescriptor >	dst_descr_request;

		SendTo( msg->srcBuffer, src_id_request );
		SendTo( msg->dstBuffer, dst_id_request );
		SendTo( msg->srcBuffer, src_descr_request );
		SendTo( msg->dstBuffer, dst_descr_request );
		
		CHECK_ERR( src_descr_request->result.Get().usage.Get( EBufferUsage::TransferSrc ) );
		CHECK_ERR( dst_descr_request->result.Get().usage.Get( EBufferUsage::TransferDst ) );

		BufferCopyRegions_t	regions;

		FOR( i, msg->regions )
		{
			auto const&		src = msg->regions[i];
			VkBufferCopy	dst;

			dst.srcOffset	= (VkDeviceSize) src.srcOffset;
			dst.dstOffset	= (VkDeviceSize) src.dstOffset;
			dst.size		= (VkDeviceSize) src.size;

			regions.PushBack( dst );
		}
		
		_cmdBuffer->_resources.Add( msg->srcBuffer );
		_cmdBuffer->_resources.Add( msg->dstBuffer );

		vkCmdCopyBuffer( _cmdBuffer->GetCmdBufferID(),
						 src_id_request->result.Get( VK_NULL_HANDLE ),
						 dst_id_request->result.Get( VK_NULL_HANDLE ),
						 regions.Count(),
						 regions.ptr() );
		return true;
	}
	
/*
=================================================
	_GpuCmdCopyImage
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdCopyImage (const Message< ModuleMsg::GpuCmdCopyImage > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->srcImage and msg->dstImage );

		Message< ModuleMsg::GetVkTextureID >			src_id_request;
		Message< ModuleMsg::GetVkTextureID >			dst_id_request;
		Message< ModuleMsg::GetGpuTextureDescriptor >	src_descr_request;
		Message< ModuleMsg::GetGpuTextureDescriptor >	dst_descr_request;
		
		SendTo( msg->srcImage, src_id_request );
		SendTo( msg->dstImage, dst_id_request );
		SendTo( msg->srcImage, src_descr_request );
		SendTo( msg->dstImage, dst_descr_request );
		
		CHECK_ERR( src_descr_request->result.Get().usage.Get( EImageUsage::TransferSrc ) );
		CHECK_ERR( dst_descr_request->result.Get().usage.Get( EImageUsage::TransferDst ) );

		ImageCopyRegions_t	regions;

		FOR( i, msg->regions )
		{
			auto const&		src = msg->regions[i];
			VkImageCopy		dst;

			dst.srcSubresource.aspectMask		= Vk1Enum( src.srcLayers.aspectMask );
			dst.srcSubresource.mipLevel			= src.srcLayers.mipLevel.Get();
			dst.srcSubresource.baseArrayLayer	= src.srcLayers.baseLayer.Get();
			dst.srcSubresource.layerCount		= src.srcLayers.layerCount;
			dst.srcOffset						= VkOffset3D{ int(src.srcOffset.x), int(src.srcOffset.y), int(src.srcOffset.z) };

			dst.dstSubresource.aspectMask		= Vk1Enum( src.dstLayers.aspectMask );
			dst.dstSubresource.mipLevel			= src.dstLayers.mipLevel.Get();
			dst.dstSubresource.baseArrayLayer	= src.dstLayers.baseLayer.Get();
			dst.dstSubresource.layerCount		= src.dstLayers.layerCount;
			dst.dstOffset						= VkOffset3D{ int(src.dstOffset.x), int(src.dstOffset.y), int(src.dstOffset.z) };

			dst.extent							= VkExtent3D{ src.size.x, src.size.y, src.size.z };

			regions.PushBack( dst );
		}
		
		_cmdBuffer->_resources.Add( msg->srcImage );
		_cmdBuffer->_resources.Add( msg->dstImage );

		vkCmdCopyImage( _cmdBuffer->GetCmdBufferID(),
						src_id_request->result.Get(),
						Vk1Enum( msg->srcLayout ),
						dst_id_request->result.Get(),
						Vk1Enum( msg->dstLayout ),
						regions.Count(),
						(VkImageCopy const*) regions.ptr() );
		return true;
	}
	
/*
=================================================
	_GpuCmdCopyBufferToImage
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdCopyBufferToImage (const Message< ModuleMsg::GpuCmdCopyBufferToImage > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->srcBuffer and msg->dstImage );
		
		Message< ModuleMsg::GetVkBufferID >				src_id_request;
		Message< ModuleMsg::GetVkTextureID >			dst_id_request;
		Message< ModuleMsg::GetGpuBufferDescriptor >	src_descr_request;
		Message< ModuleMsg::GetGpuTextureDescriptor >	dst_descr_request;
		
		SendTo( msg->srcBuffer, src_id_request );
		SendTo( msg->srcBuffer, src_descr_request );
		SendTo( msg->dstImage, dst_id_request );
		SendTo( msg->dstImage, dst_descr_request );
		
		CHECK_ERR( src_descr_request->result.Get().usage.Get( EBufferUsage::TransferSrc ) );
		CHECK_ERR( dst_descr_request->result.Get().usage.Get( EImageUsage::TransferDst ) );

		BufImgCopyRegions_t	regions;

		FOR( i, msg->regions )
		{
			auto const&			src = msg->regions[i];
			VkBufferImageCopy	dst;

			dst.bufferOffset					= (VkDeviceSize) src.bufferOffset;
			dst.bufferRowLength					= src.bufferRowLength;
			dst.bufferImageHeight				= src.bufferImageHeight;

			dst.imageSubresource.aspectMask		= Vk1Enum( src.imageLayers.aspectMask );
			dst.imageSubresource.mipLevel		= src.imageLayers.mipLevel.Get();
			dst.imageSubresource.baseArrayLayer	= src.imageLayers.baseLayer.Get();
			dst.imageSubresource.layerCount		= src.imageLayers.layerCount;
			dst.imageOffset						= VkOffset3D{ int(src.imageOffset.x), int(src.imageOffset.y), int(src.imageOffset.z) };
			dst.imageExtent						= VkExtent3D{ src.imageSize.x, src.imageSize.y, src.imageSize.z };

			regions.PushBack( dst );
		}
		
		_cmdBuffer->_resources.Add( msg->srcBuffer );
		_cmdBuffer->_resources.Add( msg->dstImage );

		vkCmdCopyBufferToImage( _cmdBuffer->GetCmdBufferID(),
								src_id_request->result.Get(),
								dst_id_request->result.Get(),
								Vk1Enum( msg->dstLayout ),
								regions.Count(),
								regions.ptr() );
		return true;
	}
	
/*
=================================================
	_GpuCmdCopyImageToBuffer
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdCopyImageToBuffer (const Message< ModuleMsg::GpuCmdCopyImageToBuffer > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->srcImage and msg->dstBuffer );

		Message< ModuleMsg::GetVkTextureID >			src_id_request;
		Message< ModuleMsg::GetVkBufferID >				dst_id_request;
		Message< ModuleMsg::GetGpuTextureDescriptor >	src_descr_request;
		Message< ModuleMsg::GetGpuBufferDescriptor >	dst_descr_request;
		
		SendTo( msg->dstBuffer, dst_id_request );
		SendTo( msg->dstBuffer, dst_descr_request );
		SendTo( msg->srcImage, src_id_request );
		SendTo( msg->srcImage, src_descr_request );
		
		CHECK_ERR( src_descr_request->result.Get().usage.Get( EImageUsage::TransferSrc ) );
		CHECK_ERR( dst_descr_request->result.Get().usage.Get( EBufferUsage::TransferDst ) );

		BufImgCopyRegions_t	regions;

		FOR( i, msg->regions )
		{
			auto const&			src = msg->regions[i];
			VkBufferImageCopy	dst;

			dst.bufferOffset					= (VkDeviceSize) src.bufferOffset;
			dst.bufferRowLength					= src.bufferRowLength;
			dst.bufferImageHeight				= src.bufferImageHeight;

			dst.imageSubresource.aspectMask		= Vk1Enum( src.imageLayers.aspectMask );
			dst.imageSubresource.mipLevel		= src.imageLayers.mipLevel.Get();
			dst.imageSubresource.baseArrayLayer	= src.imageLayers.baseLayer.Get();
			dst.imageSubresource.layerCount		= src.imageLayers.layerCount;
			dst.imageOffset						= VkOffset3D{ int(src.imageOffset.x), int(src.imageOffset.y), int(src.imageOffset.z) };
			dst.imageExtent						= VkExtent3D{ src.imageSize.x, src.imageSize.y, src.imageSize.z };

			regions.PushBack( dst );
		}
		
		_cmdBuffer->_resources.Add( msg->srcImage );
		_cmdBuffer->_resources.Add( msg->dstBuffer );

		vkCmdCopyImageToBuffer( _cmdBuffer->GetCmdBufferID(),
								src_id_request->result.Get(),
								Vk1Enum( msg->srcLayout ),
								dst_id_request->result.Get(),
								regions.Count(),
								regions.ptr() );
		return true;
	}
	
/*
=================================================
	_GpuCmdBlitImage
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdBlitImage (const Message< ModuleMsg::GpuCmdBlitImage > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->srcImage and msg->dstImage );
		
		Message< ModuleMsg::GetVkTextureID >			src_id_request;
		Message< ModuleMsg::GetVkTextureID >			dst_id_request;
		Message< ModuleMsg::GetGpuTextureDescriptor >	src_descr_request;
		Message< ModuleMsg::GetGpuTextureDescriptor >	dst_descr_request;
		
		SendTo( msg->srcImage, src_id_request );
		SendTo( msg->dstImage, dst_id_request );
		SendTo( msg->srcImage, src_descr_request );
		SendTo( msg->dstImage, dst_descr_request );
		
		CHECK_ERR( src_descr_request->result.Get().usage.Get( EImageUsage::TransferSrc ) );
		CHECK_ERR( dst_descr_request->result.Get().usage.Get( EImageUsage::TransferDst ) );

		ImgBlitRegions_t	regions;

		FOR( i, msg->regions )
		{
			auto const&		src = msg->regions[i];
			VkImageBlit		dst;

			dst.srcSubresource.aspectMask		= Vk1Enum( src.srcLayers.aspectMask );
			dst.srcSubresource.mipLevel			= src.srcLayers.mipLevel.Get();
			dst.srcSubresource.baseArrayLayer	= src.srcLayers.baseLayer.Get();
			dst.srcSubresource.layerCount		= src.srcLayers.layerCount;
			dst.srcOffsets[0]					= VkOffset3D{ int(src.srcOffset0.x), int(src.srcOffset0.y), int(src.srcOffset0.z) };
			dst.srcOffsets[1]					= VkOffset3D{ int(src.srcOffset1.x), int(src.srcOffset1.y), int(src.srcOffset1.z) };
			
			dst.dstSubresource.aspectMask		= Vk1Enum( src.dstLayers.aspectMask );
			dst.dstSubresource.mipLevel			= src.dstLayers.mipLevel.Get();
			dst.dstSubresource.baseArrayLayer	= src.dstLayers.baseLayer.Get();
			dst.dstSubresource.layerCount		= src.dstLayers.layerCount;
			dst.dstOffsets[0]					= VkOffset3D{ int(src.dstOffset0.x), int(src.dstOffset0.y), int(src.dstOffset0.z) };
			dst.dstOffsets[1]					= VkOffset3D{ int(src.dstOffset1.x), int(src.dstOffset1.y), int(src.dstOffset1.z) };

			regions.PushBack( dst );
		}
		
		_cmdBuffer->_resources.Add( msg->srcImage );
		_cmdBuffer->_resources.Add( msg->dstImage );

		vkCmdBlitImage( _cmdBuffer->GetCmdBufferID(),
						src_id_request->result.Get(),
						Vk1Enum( msg->srcLayout ),
						dst_id_request->result.Get(),
						Vk1Enum( msg->dstLayout ),
						regions.Count(),
						regions.ptr(),
						msg->linearFilter ? VK_FILTER_LINEAR: VK_FILTER_NEAREST );	// TODO
		return true;
	}
	
/*
=================================================
	_GpuCmdUpdateBuffer
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdUpdateBuffer (const Message< ModuleMsg::GpuCmdUpdateBuffer > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->dstBuffer and not msg->data.Empty() and msg->data.Size() < 65536_b );
		
		Message< ModuleMsg::GetVkBufferID >				id_request;
		Message< ModuleMsg::GetGpuBufferDescriptor >	descr_request;
		
		SendTo( msg->dstBuffer, id_request );
		SendTo( msg->dstBuffer, descr_request );
		
		CHECK_ERR( descr_request->result.Get().usage.Get( EBufferUsage::TransferDst ) );
		
		_cmdBuffer->_resources.Add( msg->dstBuffer );

		vkCmdUpdateBuffer( _cmdBuffer->GetCmdBufferID(),
						   id_request->result.Get(),
						   (VkDeviceSize) msg->dstOffset,
						   (VkDeviceSize) msg->data.Size(),
						   msg->data.ptr() );
		return true;
	}
	
/*
=================================================
	_GpuCmdFillBuffer
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdFillBuffer (const Message< ModuleMsg::GpuCmdFillBuffer > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->dstBuffer );
		
		Message< ModuleMsg::GetVkBufferID >				id_request;
		Message< ModuleMsg::GetGpuBufferDescriptor >	descr_request;
		
		SendTo( msg->dstBuffer, id_request );
		SendTo( msg->dstBuffer, descr_request );
		
		CHECK_ERR( descr_request->result.Get().usage.Get( EBufferUsage::TransferDst ) );
		
		_cmdBuffer->_resources.Add( msg->dstBuffer );

		vkCmdFillBuffer( _cmdBuffer->GetCmdBufferID(),
						 id_request->result.Get(),
						 (VkDeviceSize) msg->dstOffset,
						 (VkDeviceSize) Min( msg->size, descr_request->result.Get().size ),
						 msg->pattern );
		return true;
	}
	
/*
=================================================
	_GpuCmdClearAttachments
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdClearAttachments (const Message< ModuleMsg::GpuCmdClearAttachments > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );

		ClearAttachments_t	attachments;
		ClearRects_t		clear_rects;

		FOR( i, msg->attachments )
		{
			auto const&			src = msg->attachments[i];
			VkClearAttachment	dst;

			dst.aspectMask		= Vk1Enum( src.aspectMask );
			dst.colorAttachment	= src.attachmentIdx;
			MemCopy( BinArrayRef::FromValue( dst.clearValue ), src.clearValue.GetData() );	// TODO

			attachments.PushBack( dst );
		}

		FOR( i, msg->clearRects )
		{
			auto const&		src = msg->clearRects[i];
			VkClearRect		dst;

			dst.baseArrayLayer	= src.baseLayer.Get();
			dst.layerCount		= src.layerCount;
			dst.rect			= VkRect2D{ VkOffset2D{ int(src.rect.left), int(src.rect.bottom) },
											VkExtent2D{ src.rect.Width(), src.rect.Height() } };

			clear_rects.PushBack( dst );
		}

		vkCmdClearAttachments( _cmdBuffer->GetCmdBufferID(),
							   attachments.Count(),
							   attachments.ptr(),
							   clear_rects.Count(),
							   clear_rects.ptr() );
		return true;
	}
	
/*
=================================================
	_GpuCmdClearColorImage
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdClearColorImage (const Message< ModuleMsg::GpuCmdClearColorImage > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->image );
		
		Message< ModuleMsg::GetVkTextureID >			id_request;
		Message< ModuleMsg::GetGpuTextureDescriptor >	descr_request;
		
		SendTo( msg->image, id_request );
		SendTo( msg->image, descr_request );
		
		CHECK_ERR( descr_request->result.Get().usage.Get( EImageUsage::TransferDst ) );

		ImageRanges_t		ranges;
		VkClearColorValue	clear_value;
		MemCopy( BinArrayRef::FromValue( clear_value ), msg->clearValue.GetData() );	// TODO

		FOR( i, msg->ranges )
		{
			auto const&					src = msg->ranges[i];
			VkImageSubresourceRange		dst;

			dst.aspectMask		= Vk1Enum( src.aspectMask );
			dst.baseMipLevel	= src.baseMipLevel.Get();
			dst.levelCount		= src.levelCount;
			dst.baseArrayLayer	= src.baseLayer.Get();
			dst.layerCount		= src.layerCount;

			ranges.PushBack( dst );
		}
		
		_cmdBuffer->_resources.Add( msg->image );

		vkCmdClearColorImage( _cmdBuffer->GetCmdBufferID(),
							  id_request->result.Get(),
							  Vk1Enum( msg->layout ),
							  &clear_value,
							  ranges.Count(),
							  ranges.ptr() );
		return true;
	}
	
/*
=================================================
	_GpuCmdClearDepthStencilImage
=================================================
*/
	bool Vk1CommandBuilder::_GpuCmdClearDepthStencilImage (const Message< ModuleMsg::GpuCmdClearDepthStencilImage > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->image );
		
		Message< ModuleMsg::GetVkTextureID >			id_request;
		Message< ModuleMsg::GetGpuTextureDescriptor >	descr_request;
		
		SendTo( msg->image, id_request );
		SendTo( msg->image, descr_request );
		
		CHECK_ERR( descr_request->result.Get().usage.Get( EImageUsage::TransferDst ) );

		ImageRanges_t				ranges;
		VkClearDepthStencilValue	clear_value;

		clear_value.depth	= msg->clearValue.depth;
		clear_value.stencil	= msg->clearValue.stencil;
			
		FOR( i, msg->ranges )
		{
			auto const&					src = msg->ranges[i];
			VkImageSubresourceRange		dst;

			dst.aspectMask		= Vk1Enum( src.aspectMask );
			dst.baseMipLevel	= src.baseMipLevel.Get();
			dst.levelCount		= src.levelCount;
			dst.baseArrayLayer	= src.baseLayer.Get();
			dst.layerCount		= src.layerCount;

			ranges.PushBack( dst );
		}
		
		_cmdBuffer->_resources.Add( msg->image );

		vkCmdClearDepthStencilImage( _cmdBuffer->GetCmdBufferID(),
									 id_request->result.Get(),
									 Vk1Enum( msg->layout ),
									 &clear_value,
									 ranges.Count(),
									 ranges.ptr() );
		return true;
	}

/*
=================================================
	_IsCreated
=================================================
*/
	bool Vk1CommandBuilder::_IsCreated () const
	{
		return _cmdPool != VK_NULL_HANDLE;
	}

/*
=================================================
	_CreateCmdBufferPool
=================================================
*/
	bool Vk1CommandBuilder::_CreateCmdBufferPool ()
	{
		using namespace vk;

		CHECK_ERR( not _IsCreated() );

		VkCommandPoolCreateInfo		pool_info	= {};
		pool_info.sType				= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		pool_info.queueFamilyIndex	= GetDevice()->GetQueueIndex();
		pool_info.flags				= 0;	// TODO

		VK_CHECK( vkCreateCommandPool( GetLogicalDevice(), &pool_info, null, OUT &_cmdPool ) );
		return true;
	}

/*
=================================================
	_DestroyCmdBufferPool
=================================================
*/
	void Vk1CommandBuilder::_DestroyCmdBufferPool ()
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
	bool Vk1CommandBuilder::_CheckDynamicState (EPipelineDynamicState::type state) const
	{
		return _dynamicStates.Get( state );
	}
	
/*
=================================================
	_CheckCompatibility
=================================================
*/
	bool Vk1CommandBuilder::_CheckCompatibility (const FramebufferDescriptor &fb,
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
