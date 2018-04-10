// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Public/GPU/CommandBuffer.h"
#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Framebuffer.h"
#include "Engine/Platforms/Public/GPU/RenderPass.h"
#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/VulkanObjectsConstructor.h"

namespace Engine
{
namespace PlatformVK
{
	using namespace vk;



	//
	// Vulkan Command Buffer Builder
	//

	class Vk1CommandBuilder final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::CmdSetViewport,
											GpuMsg::CmdSetScissor,
											GpuMsg::CmdSetDepthBounds,
											GpuMsg::CmdSetBlendColor,
											GpuMsg::CmdSetDepthBias,
											GpuMsg::CmdSetLineWidth,
											GpuMsg::CmdSetStencilCompareMask,
											GpuMsg::CmdSetStencilWriteMask,
											GpuMsg::CmdSetStencilReference,
											GpuMsg::CmdBegin,
											GpuMsg::CmdEnd,
											GpuMsg::CmdBeginRenderPass,
											GpuMsg::CmdEndRenderPass,
											GpuMsg::CmdNextSubpass,
											GpuMsg::CmdBindGraphicsPipeline,
											GpuMsg::CmdBindComputePipeline,
											GpuMsg::CmdBindVertexBuffers,
											GpuMsg::CmdBindIndexBuffer,
											GpuMsg::CmdDraw,
											GpuMsg::CmdDrawIndexed,
											GpuMsg::CmdDrawIndirect,
											GpuMsg::CmdDrawIndexedIndirect,
											GpuMsg::CmdDispatch,
											GpuMsg::CmdDispatchIndirect,
											GpuMsg::CmdExecute,
											GpuMsg::CmdBindGraphicsResourceTable,
											GpuMsg::CmdBindComputeResourceTable,
											GpuMsg::CmdCopyBuffer,
											GpuMsg::CmdCopyImage,
											GpuMsg::CmdCopyBufferToImage,
											GpuMsg::CmdCopyImageToBuffer,
											GpuMsg::CmdBlitImage,
											GpuMsg::CmdUpdateBuffer,
											GpuMsg::CmdFillBuffer,
											GpuMsg::CmdClearAttachments,
											GpuMsg::CmdClearColorImage,
											GpuMsg::CmdClearDepthStencilImage,
											GpuMsg::CmdSetEvent,
											GpuMsg::CmdResetEvent,
											GpuMsg::CmdWaitEvents,
											GpuMsg::CmdPipelineBarrier,
											GpuMsg::CmdPushConstants,
											GpuMsg::CmdPushNamedConstants,
											GpuMsg::SetCommandBufferDependency,
											GpuMsg::GetCommandBufferState,
											GpuMsg::GetVkCommandPoolID
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;
		
		using CmdBufferMsg_t		= MessageListFrom< GpuMsg::SetCommandBufferDependency, GpuMsg::SetCommandBufferState,
														GpuMsg::GetCommandBufferState >;

		using DynamicStates_t		= EPipelineDynamicState::bits;
		using UsedResources_t		= Set< ModulePtr >;
		using ERecordingState		= GpuMsg::SetCommandBufferState::EState;

		enum class EScope
		{
			None,
			Command,
			RenderPass,
		};


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;

		static constexpr BytesU		_maxUpdateBuffer = 65536_b;		// as specification said


	// variables
	private:
		VkCommandPool			_cmdPool;
		ModulePtr				_syncManager;

		UsedResources_t			_resources;
		ModulePtr				_cmdBuffer;		// current command buffer
		VkCommandBuffer			_cmdId;			// cached id
		EScope					_scope;

		//Layout_t				_currGraphicsLayout;
		//Layout_t				_currComputeLayout;

		DynamicStates_t			_dynamicStates;
		uint					_subpassIndex;
		uint					_maxSubpasses;


	// methods
	public:
		Vk1CommandBuilder (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci);
		~Vk1CommandBuilder ();


	// message handlers
	private:
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _SetCommandBufferDependency (const Message< GpuMsg::SetCommandBufferDependency > &);
		bool _GetCommandBufferState (const Message< GpuMsg::GetCommandBufferState > &);
		bool _GetVkCommandPoolID (const Message< GpuMsg::GetVkCommandPoolID > &);
		
		bool _CmdSetViewport (const Message< GpuMsg::CmdSetViewport > &);
		bool _CmdSetScissor (const Message< GpuMsg::CmdSetScissor > &);
		bool _CmdSetDepthBounds (const Message< GpuMsg::CmdSetDepthBounds > &);
		bool _CmdSetBlendColor (const Message< GpuMsg::CmdSetBlendColor > &);
		bool _CmdSetDepthBias (const Message< GpuMsg::CmdSetDepthBias > &);
		bool _CmdSetLineWidth (const Message< GpuMsg::CmdSetLineWidth > &);
		bool _CmdSetStencilCompareMask (const Message< GpuMsg::CmdSetStencilCompareMask > &);
		bool _CmdSetStencilWriteMask (const Message< GpuMsg::CmdSetStencilWriteMask > &);
		bool _CmdSetStencilReference (const Message< GpuMsg::CmdSetStencilReference > &);
		bool _CmdBegin (const Message< GpuMsg::CmdBegin > &);
		bool _CmdEnd (const Message< GpuMsg::CmdEnd > &);
		bool _CmdBeginRenderPass (const Message< GpuMsg::CmdBeginRenderPass > &);
		bool _CmdEndRenderPass (const Message< GpuMsg::CmdEndRenderPass > &);
		bool _CmdNextSubpass (const Message< GpuMsg::CmdNextSubpass > &);
		bool _CmdBindGraphicsPipeline (const Message< GpuMsg::CmdBindGraphicsPipeline > &);
		bool _CmdBindComputePipeline (const Message< GpuMsg::CmdBindComputePipeline > &);
		bool _CmdBindVertexBuffers (const Message< GpuMsg::CmdBindVertexBuffers > &);
		bool _CmdBindIndexBuffer (const Message< GpuMsg::CmdBindIndexBuffer > &);
		bool _CmdDraw (const Message< GpuMsg::CmdDraw > &);
		bool _CmdDrawIndexed (const Message< GpuMsg::CmdDrawIndexed > &);
		bool _CmdDrawIndirect (const Message< GpuMsg::CmdDrawIndirect > &);
		bool _CmdDrawIndexedIndirect (const Message< GpuMsg::CmdDrawIndexedIndirect > &);
		bool _CmdDispatch (const Message< GpuMsg::CmdDispatch > &);
		bool _CmdDispatchIndirect (const Message< GpuMsg::CmdDispatchIndirect > &);
		bool _CmdExecute (const Message< GpuMsg::CmdExecute > &);
		bool _CmdBindGraphicsResourceTable (const Message< GpuMsg::CmdBindGraphicsResourceTable > &);
		bool _CmdBindComputeResourceTable (const Message< GpuMsg::CmdBindComputeResourceTable > &);
		bool _CmdCopyBuffer (const Message< GpuMsg::CmdCopyBuffer > &);
		bool _CmdCopyImage (const Message< GpuMsg::CmdCopyImage > &);
		bool _CmdCopyBufferToImage (const Message< GpuMsg::CmdCopyBufferToImage > &);
		bool _CmdCopyImageToBuffer (const Message< GpuMsg::CmdCopyImageToBuffer > &);
		bool _CmdBlitImage (const Message< GpuMsg::CmdBlitImage > &);
		bool _CmdUpdateBuffer (const Message< GpuMsg::CmdUpdateBuffer > &);
		bool _CmdFillBuffer (const Message< GpuMsg::CmdFillBuffer > &);
		bool _CmdClearAttachments (const Message< GpuMsg::CmdClearAttachments > &);
		bool _CmdClearColorImage (const Message< GpuMsg::CmdClearColorImage > &);
		bool _CmdClearDepthStencilImage (const Message< GpuMsg::CmdClearDepthStencilImage > &);
		bool _CmdSetEvent (const Message< GpuMsg::CmdSetEvent > &);
		bool _CmdResetEvent (const Message< GpuMsg::CmdResetEvent > &);
		bool _CmdWaitEvents (const Message< GpuMsg::CmdWaitEvents > &);
		bool _CmdPipelineBarrier (const Message< GpuMsg::CmdPipelineBarrier > &);
		bool _CmdPushConstants (const Message< GpuMsg::CmdPushConstants > &);
		bool _CmdPushNamedConstants (const Message< GpuMsg::CmdPushNamedConstants > &);
		
	private:
		bool _IsCreated () const;
		bool _CreateCmdBufferPool ();
		void _DestroyCmdBufferPool ();
		
		bool _BindDescriptorSet (const ModulePtr &resourceTable, uint firstIndex, VkPipelineBindPoint bindPoint);

		bool _CheckGraphicsPipeline ();
		bool _CheckComputePipeline ();

		bool _CheckGraphicsWritableResources ();
		bool _CheckComputeWritableResources ();

		bool _CheckDynamicState (EPipelineDynamicState::type state) const;
		bool _CheckCompatibility (const FramebufferDescriptor &fbDescr,
								  const RenderPassDescriptor &rpDescr) const;
	};
//-----------------------------------------------------------------------------


	
	using Viewports_t				= FixedSizeArray< VkViewport, GlobalConst::GAPI_MaxColorBuffers >;
	using Scissors_t				= FixedSizeArray< VkRect2D, GlobalConst::GAPI_MaxColorBuffers >;
	using ClearValues_t				= FixedSizeArray< VkClearValue, GlobalConst::GAPI_MaxColorBuffers >;
	using VertexBuffers_t			= FixedSizeArray< VkBuffer, GlobalConst::GAPI_MaxAttribs >;
	using CmdBuffers_t				= FixedSizeArray< VkCommandBuffer, GpuMsg::CmdExecute::CmdBuffers_t::MemoryContainer_t::SIZE >;
	using BufferCopyRegions_t		= FixedSizeArray< VkBufferCopy, GpuMsg::CmdCopyBuffer::Regions_t::MemoryContainer_t::SIZE >;
	using ImageCopyRegions_t		= FixedSizeArray< VkImageCopy, GpuMsg::CmdCopyImage::Regions_t::MemoryContainer_t::SIZE >;
	using BufImgCopyRegions_t		= FixedSizeArray< VkBufferImageCopy, GpuMsg::CmdCopyBufferToImage::Regions_t::MemoryContainer_t::SIZE >;
	using ImgBlitRegions_t			= FixedSizeArray< VkImageBlit, GpuMsg::CmdBlitImage::Regions_t::MemoryContainer_t::SIZE >;
	using ClearAttachments_t		= FixedSizeArray< VkClearAttachment, GpuMsg::CmdClearAttachments::Attachments_t::MemoryContainer_t::SIZE >;
	using ClearRects_t				= FixedSizeArray< VkClearRect, GpuMsg::CmdClearAttachments::ClearRects_t::MemoryContainer_t::SIZE >;
	using ImageRanges_t				= FixedSizeArray< VkImageSubresourceRange, GpuMsg::CmdClearColorImage::Ranges_t::MemoryContainer_t::SIZE >;
	using MemoryBarriers_t			= FixedSizeArray< VkMemoryBarrier, GpuMsg::CmdPipelineBarrier::MAX_BARRIERS >;
	using ImageMemoryBarriers_t		= FixedSizeArray< VkImageMemoryBarrier, GpuMsg::CmdPipelineBarrier::MAX_BARRIERS >;
	using BufferMemoryBarriers_t	= FixedSizeArray< VkBufferMemoryBarrier, GpuMsg::CmdPipelineBarrier::MAX_BARRIERS >;


	const TypeIdList	Vk1CommandBuilder::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	Vk1CommandBuilder::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1CommandBuilder::Vk1CommandBuilder (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci) :
		Vk1BaseModule( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes ),
		_cmdPool( VK_NULL_HANDLE ),		_cmdId( VK_NULL_HANDLE ),
		_scope( EScope::None ),			_subpassIndex( 0 ),
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
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GetDeviceInfo );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GetVkDeviceInfo );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GetVkPrivateClasses );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GetVkCommandPoolID );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GetCommandBufferState );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_SetCommandBufferDependency );

		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdSetViewport );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdSetScissor );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdSetDepthBounds );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdSetBlendColor );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdSetDepthBias );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdSetLineWidth );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdSetStencilCompareMask );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdSetStencilWriteMask );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdSetStencilReference );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdBegin );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdEnd );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdBeginRenderPass );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdEndRenderPass );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdNextSubpass );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdBindGraphicsPipeline );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdBindComputePipeline );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdBindVertexBuffers );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdBindIndexBuffer );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdDraw );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdDrawIndexed );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdDrawIndirect );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdDrawIndexedIndirect );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdDispatch );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdDispatchIndirect );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdExecute );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdBindGraphicsResourceTable );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdBindComputeResourceTable );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdCopyBuffer );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdCopyImage );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdCopyBufferToImage );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdCopyImageToBuffer );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdBlitImage );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdUpdateBuffer );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdFillBuffer );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdClearAttachments );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdClearColorImage );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdClearDepthStencilImage );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdSetEvent );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdResetEvent );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdWaitEvents );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdPipelineBarrier );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdPushConstants );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdPushNamedConstants );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
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
	bool Vk1CommandBuilder::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreateCmdBufferPool() );

		Message< GpuMsg::GetDeviceInfo >	req_dev;
		_GetManager()->Send( req_dev );
		CHECK_COMPOSING( _syncManager = req_dev->result->syncManager );

		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );

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
		_SendForEachAttachments( msg );

		_DestroyCmdBufferPool();

		_syncManager = null;

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_SetCommandBufferDependency
=================================================
*/
	bool Vk1CommandBuilder::_SetCommandBufferDependency (const Message< GpuMsg::SetCommandBufferDependency > &msg)
	{
		_resources.AddArray( msg->resources.Get() );
		return true;
	}
	
/*
=================================================
	_GetCommandBufferState
=================================================
*/
	bool Vk1CommandBuilder::_GetCommandBufferState (const Message< GpuMsg::GetCommandBufferState > &msg)
	{
		if ( not _cmdBuffer )
			return false;
			
		return _cmdBuffer->Send( msg );
	}
	
/*
=================================================
	_GetVkCommandPoolID
=================================================
*/
	bool Vk1CommandBuilder::_GetVkCommandPoolID (const Message< GpuMsg::GetVkCommandPoolID > &msg)
	{
		msg->result.Set( _cmdPool );
		return true;
	}

/*
=================================================
	_CmdSetViewport
=================================================
*/
	bool Vk1CommandBuilder::_CmdSetViewport (const Message< GpuMsg::CmdSetViewport > &msg)
	{
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::Viewport ) );
		CHECK_ERR( _scope == EScope::Command or _scope == EScope::RenderPass );

		Viewports_t		viewports;		viewports.Resize( msg->viewports.Count() );

		FOR( i, msg->viewports )
		{
			auto const&	vp = msg->viewports[i];

			viewports[i] = VkViewport{	float(vp.rect.left),    float(vp.rect.bottom),
										float(vp.rect.Width()), float(vp.rect.Height()),
										vp.depthRange.x,        vp.depthRange.y   };
		}

		vkCmdSetViewport( _cmdId, msg->firstViewport, (uint32_t)viewports.Count(), viewports.ptr() );
		return true;
	}
	
/*
=================================================
	_CmdSetScissor
=================================================
*/
	bool Vk1CommandBuilder::_CmdSetScissor (const Message< GpuMsg::CmdSetScissor > &msg)
	{
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::Scissor ) );
		CHECK_ERR( _scope == EScope::Command or _scope == EScope::RenderPass );

		Scissors_t		scissors;		scissors.Resize( msg->scissors.Count() );

		FOR( i, msg->scissors )
		{
			auto const&	sc = msg->scissors[i];

			scissors[i] = VkRect2D{ VkOffset2D{ int(sc.left), int(sc.bottom) },
									VkExtent2D{ sc.Width(),   sc.Height()    } };
		}

		vkCmdSetScissor( _cmdId, msg->firstScissor, (uint32_t)scissors.Count(), scissors.ptr() );
		return true;
	}
	
/*
=================================================
	_CmdSetDepthBounds
=================================================
*/
	bool Vk1CommandBuilder::_CmdSetDepthBounds (const Message< GpuMsg::CmdSetDepthBounds > &msg)
	{
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::DepthBounds ) );
		CHECK_ERR( _scope == EScope::Command or _scope == EScope::RenderPass );

		vkCmdSetDepthBounds( _cmdId, msg->min, msg->max );
		return true;
	}
	
/*
=================================================
	_CmdSetBlendColor
=================================================
*/
	bool Vk1CommandBuilder::_CmdSetBlendColor (const Message< GpuMsg::CmdSetBlendColor > &msg)
	{
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::BlendConstants ) );
		CHECK_ERR( _scope == EScope::Command or _scope == EScope::RenderPass );
		
		vkCmdSetBlendConstants( _cmdId, msg->color.ptr() );
		return true;
	}
	
/*
=================================================
	_CmdSetDepthBias
=================================================
*/
	bool Vk1CommandBuilder::_CmdSetDepthBias (const Message< GpuMsg::CmdSetDepthBias > &msg)
	{
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::DepthBias ) );
		CHECK_ERR( _scope == EScope::Command or _scope == EScope::RenderPass );
		
		vkCmdSetDepthBias( _cmdId, msg->biasConstFactor, msg->biasClamp, msg->biasSlopeFactor );
		return true;
	}
	
/*
=================================================
	_CmdSetLineWidth
=================================================
*/
	bool Vk1CommandBuilder::_CmdSetLineWidth (const Message< GpuMsg::CmdSetLineWidth > &msg)
	{
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::LineWidth ) );
		CHECK_ERR( _scope == EScope::Command or _scope == EScope::RenderPass );
		
		vkCmdSetLineWidth( _cmdId, msg->width );
		return true;
	}
	
/*
=================================================
	_CmdSetStencilCompareMask
=================================================
*/
	bool Vk1CommandBuilder::_CmdSetStencilCompareMask (const Message< GpuMsg::CmdSetStencilCompareMask > &msg)
	{
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::StencilCompareMask ) );
		CHECK_ERR( _scope == EScope::Command or _scope == EScope::RenderPass );
		
		VkStencilFaceFlagBits	flags;
		Vk1Enum( msg->face, OUT flags );

		vkCmdSetStencilCompareMask( _cmdId, flags, msg->cmpMask );
		return true;
	}

/*
=================================================
	_CmdSetStencilWriteMask
=================================================
*/
	bool Vk1CommandBuilder::_CmdSetStencilWriteMask (const Message< GpuMsg::CmdSetStencilWriteMask > &msg)
	{
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::StencilWriteMask ) );
		CHECK_ERR( _scope == EScope::Command or _scope == EScope::RenderPass );
		
		VkStencilFaceFlagBits	flags;
		Vk1Enum( msg->face, OUT flags );

		vkCmdSetStencilWriteMask( _cmdId, flags, msg->mask );
		return true;
	}
	
/*
=================================================
	_CmdSetStencilReference
=================================================
*/
	bool Vk1CommandBuilder::_CmdSetStencilReference (const Message< GpuMsg::CmdSetStencilReference > &msg)
	{
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::StencilReference ) );
		CHECK_ERR( _scope == EScope::Command or _scope == EScope::RenderPass );
		
		VkStencilFaceFlagBits	flags;
		Vk1Enum( msg->face, OUT flags );
		
		vkCmdSetStencilReference( _cmdId, flags, msg->ref );
		return true;
	}
	
/*
=================================================
	_CmdBegin
=================================================
*/
	bool Vk1CommandBuilder::_CmdBegin (const Message< GpuMsg::CmdBegin > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( not _cmdBuffer );
		CHECK_ERR( _scope == EScope::None );
		
		// use target command buffer
		if ( msg->targetCmdBuffer )
		{
			// it is bad practice, becouse command buffer depends of other commands pool
			//ASSERT( msg->targetCmdBuffer->_GetParents().IsExist( this ) );		// TODO
			
			CHECK_ERR( msg->targetCmdBuffer->GetSupportedMessages().HasAllTypes< CmdBufferMsg_t >() );

			_cmdBuffer = msg->targetCmdBuffer;
		}
		else
		// create new command buffer
		{
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
							VkCommandBufferModuleID,
							GlobalSystems(),
							CreateInfo::GpuCommandBuffer{
								_GetManager(),
								CommandBufferDescriptor{ msg->flags }
							},
							OUT _cmdBuffer )
			);
			CHECK_ERR( _Attach( "", _cmdBuffer, false ) );
		}

		ModuleUtils::Initialize( {_cmdBuffer}, this );

		//SendTo( _cmdBuffer, Message< GpuMsg::SetCommandBufferState >{ ERecordingState::Initial } );

		// get command buffer id
		Message< GpuMsg::GetVkCommandBufferID >		req_cmd_id;
		SendTo( _cmdBuffer, req_cmd_id );
		CHECK_ERR( req_cmd_id->result and *req_cmd_id->result != VK_NULL_HANDLE );

		_cmdId = *req_cmd_id->result;

		// begin
		VkCommandBufferBeginInfo	cmd_info = {};
		cmd_info.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		cmd_info.pNext				= null;
		cmd_info.flags				= 0;	// TODO		VK_QUERY_CONTROL_PRECISE_BIT
		cmd_info.pInheritanceInfo	= null;	// TODO

		VK_CHECK( vkBeginCommandBuffer( _cmdId, &cmd_info ) );
		SendTo( _cmdBuffer, Message< GpuMsg::SetCommandBufferState >{ ERecordingState::Recording } );

		// check buffer state
		Message< GpuMsg::GetCommandBufferState >	req_state;
		SendTo( _cmdBuffer, req_state );
		CHECK_ERR( req_state->result.Get() == ERecordingState::Recording );

		_scope = EScope::Command;
		return true;
	}
	
/*
=================================================
	_GpuCmdEnd
=================================================
*/
	bool Vk1CommandBuilder::_CmdEnd (const Message< GpuMsg::CmdEnd > &msg)
	{
		CHECK_ERR( _scope == EScope::Command );
		
		VK_CHECK( vkEndCommandBuffer( _cmdId ) );

		SendTo( _cmdBuffer, Message< GpuMsg::SetCommandBufferDependency >{ RVREF(_resources) } );
		SendTo( _cmdBuffer, Message< GpuMsg::SetCommandBufferState >{ ERecordingState::Executable } );

		msg->result.Set( _cmdBuffer );

		_cmdBuffer	= null;
		_scope		= EScope::None;
		_cmdId		= VK_NULL_HANDLE;
		return true;
	}
	
/*
=================================================
	_CmdBeginRenderPass
=================================================
*/
	bool Vk1CommandBuilder::_CmdBeginRenderPass (const Message< GpuMsg::CmdBeginRenderPass > &msg)
	{
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->framebuffer );
		CHECK_ERR( msg->renderPass );

		// requests
		Message< GpuMsg::GetVkFramebufferID >		req_fb;
		Message< GpuMsg::GetFramebufferDescriptor >	req_fb_descr;
		Message< GpuMsg::GetVkRenderPassID >		req_pass;
		Message< GpuMsg::GetRenderPassDescriptor >	req_pass_descr;
		
		// TODO: check result
		SendTo( msg->framebuffer, req_fb );
		SendTo( msg->framebuffer, req_fb_descr );

		SendTo( msg->renderPass, req_pass );
		SendTo( msg->renderPass, req_pass_descr );

		auto const&	fb_descr	= req_fb_descr->result.Get();
		auto const&	rp_descr	= req_pass_descr->result.Get();

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
		pass_info.renderPass				= *req_pass->result;
		pass_info.renderArea.offset.x		= msg->area.left;
		pass_info.renderArea.offset.y		= msg->area.bottom;
		pass_info.renderArea.extent.width	= msg->area.Width();
		pass_info.renderArea.extent.height	= msg->area.Height();
		pass_info.clearValueCount			= (uint32_t) clear_values.Count();
		pass_info.pClearValues				= clear_values.ptr();
		pass_info.framebuffer				= *req_fb->result;
		
		vkCmdBeginRenderPass( _cmdId, &pass_info, VK_SUBPASS_CONTENTS_INLINE );


		// chenge states
		_resources.Add( msg->framebuffer );
		_resources.Add( msg->renderPass );

		_scope			= EScope::RenderPass;
		_dynamicStates	= Uninitialized;
		_subpassIndex	= 0;
		_maxSubpasses	= (uint32_t) rp_descr.Subpasses().Count();
		return true;
	}

/*
=================================================
	_CmdEndRenderPass
=================================================
*/
	bool Vk1CommandBuilder::_CmdEndRenderPass (const Message< GpuMsg::CmdEndRenderPass > &)
	{
		CHECK_ERR( _scope == EScope::RenderPass );

		vkCmdEndRenderPass( _cmdId );

		_scope			= EScope::Command;
		_dynamicStates	= Uninitialized;
		_subpassIndex	= 0;
		_maxSubpasses	= 0;
		return true;
	}
	
/*
=================================================
	_CmdNextSubpass
=================================================
*/
	bool Vk1CommandBuilder::_CmdNextSubpass (const Message< GpuMsg::CmdNextSubpass > &)
	{
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( ++_subpassIndex < _maxSubpasses );

		vkCmdNextSubpass( _cmdId, VK_SUBPASS_CONTENTS_INLINE );
		return true;
	}
	
/*
=================================================
	_CmdBindGraphicsPipeline
=================================================
*/
	bool Vk1CommandBuilder::_CmdBindGraphicsPipeline (const Message< GpuMsg::CmdBindGraphicsPipeline > &msg)
	{
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg->pipeline );

		Message< GpuMsg::GetVkGraphicsPipelineID >			req_id;
		Message< GpuMsg::GetGraphicsPipelineDescriptor >	req_descr;
		
		SendTo( msg->pipeline, req_id );
		SendTo( msg->pipeline, req_descr );

		_resources.Add( msg->pipeline );

		ASSERT( req_descr->result->subpass == _subpassIndex );
		vkCmdBindPipeline( _cmdId, VK_PIPELINE_BIND_POINT_GRAPHICS, req_id->result.Get( VK_NULL_HANDLE ) );

		_dynamicStates = req_descr->result->dynamicStates;
		return true;
	}
	
/*
=================================================
	_CmdBindComputePipeline
=================================================
*/
	bool Vk1CommandBuilder::_CmdBindComputePipeline (const Message< GpuMsg::CmdBindComputePipeline > &msg)
	{
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->pipeline );
		
		Message< GpuMsg::GetVkComputePipelineID >	req_id;
		SendTo( msg->pipeline, req_id );
		
		vkCmdBindPipeline( _cmdId, VK_PIPELINE_BIND_POINT_COMPUTE, req_id->result.Get( VK_NULL_HANDLE ) );
		return true;
	}

/*
=================================================
	_CmdBindVertexBuffers
=================================================
*/
	bool Vk1CommandBuilder::_CmdBindVertexBuffers (const Message< GpuMsg::CmdBindVertexBuffers > &msg)
	{
		CHECK_ERR( _scope == EScope::RenderPass );

		CompileTime::MustBeSameSize< VkDeviceSize, decltype(msg->offsets)::Value_t >();

		VertexBuffers_t		buffers;	buffers.Resize( msg->vertexBuffers.Count() );

		FOR( i, msg->vertexBuffers )
		{
			auto const&	vb = msg->vertexBuffers[i];

			Message< GpuMsg::GetVkBufferID >		req_id;
			Message< GpuMsg::GetBufferDescriptor >	req_descr;
			
			// TODO: check result
			SendTo( vb, req_id );
			SendTo( vb, req_descr );

			ASSERT( req_descr->result->usage[ EBufferUsage::Vertex ] );

			buffers[i] = *req_id->result;
			
			_resources.Add( vb );
		}
		
		vkCmdBindVertexBuffers( _cmdId,
								msg->firstBinding,
								(uint32_t) buffers.Count(),
								buffers.ptr(),
								PointerCast< VkDeviceSize const >( msg->offsets.ptr() ) );
		return true;
	}
	
/*
=================================================
	_CmdBindIndexBuffer
=================================================
*/
	bool Vk1CommandBuilder::_CmdBindIndexBuffer (const Message< GpuMsg::CmdBindIndexBuffer > &msg)
	{
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg->indexBuffer );

		Message< GpuMsg::GetVkBufferID >		req_id;
		Message< GpuMsg::GetBufferDescriptor >	req_descr;
		
		// TODO: check result
		SendTo( msg->indexBuffer, req_id );
		SendTo( msg->indexBuffer, req_descr );
		
		ASSERT( req_descr->result->usage[ EBufferUsage::Index ] );
		
		_resources.Add( msg->indexBuffer );

		vkCmdBindIndexBuffer( _cmdId,
							  req_id->result.Get( VK_NULL_HANDLE ),
							  (VkDeviceSize) msg->offset,
							  Vk1Enum( msg->indexType ) );
		return true;
	}
	
/*
=================================================
	_CmdDraw
=================================================
*/
	bool Vk1CommandBuilder::_CmdDraw (const Message< GpuMsg::CmdDraw > &msg)
	{
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( _CheckGraphicsPipeline() );

		vkCmdDraw( _cmdId,
				   msg->vertexCount,
				   msg->instanceCount,
				   msg->firstVertex,
				   msg->firstInstance );

		_CheckGraphicsWritableResources();
		return true;
	}
	
/*
=================================================
	_CmdDrawIndexed
=================================================
*/
	bool Vk1CommandBuilder::_CmdDrawIndexed (const Message< GpuMsg::CmdDrawIndexed > &msg)
	{
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( _CheckGraphicsPipeline() );
		
		vkCmdDrawIndexed( _cmdId,
						  msg->indexCount,
						  msg->instanceCount,
						  msg->firstIndex,
						  msg->vertexOffset,
						  msg->firstInstance );

		_CheckGraphicsWritableResources();
		return true;
	}
	
/*
=================================================
	_CmdDrawIndirect
=================================================
*/
	bool Vk1CommandBuilder::_CmdDrawIndirect (const Message< GpuMsg::CmdDrawIndirect > &msg)
	{
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg->indirectBuffer );
		CHECK_ERR( _CheckGraphicsPipeline() );

		Message< GpuMsg::GetVkBufferID >		req_id;
		Message< GpuMsg::GetBufferDescriptor >	req_descr;
		
		// TODO: check result
		SendTo( msg->indirectBuffer, req_id );
		SendTo( msg->indirectBuffer, req_descr );
		
		ASSERT( req_descr->result->usage[ EBufferUsage::Indirect ] );
		
		_resources.Add( msg->indirectBuffer );

		vkCmdDrawIndirect(	_cmdId,
							req_id->result.Get( VK_NULL_HANDLE ),
							(VkDeviceSize) msg->offset,
							msg->drawCount,
							(uint32_t) msg->stride );

		_CheckGraphicsWritableResources();
		return true;
	}
	
/*
=================================================
	_CmdDrawIndexedIndirect
=================================================
*/
	bool Vk1CommandBuilder::_CmdDrawIndexedIndirect (const Message< GpuMsg::CmdDrawIndexedIndirect > &msg)
	{
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg->indirectBuffer );
		CHECK_ERR( _CheckGraphicsPipeline() );

		Message< GpuMsg::GetVkBufferID >		req_id;
		Message< GpuMsg::GetBufferDescriptor >	req_descr;
		
		// TODO: check result
		SendTo( msg->indirectBuffer, req_id );
		SendTo( msg->indirectBuffer, req_descr );
		
		ASSERT( req_descr->result->usage[ EBufferUsage::Indirect ] );
		
		_resources.Add( msg->indirectBuffer );

		vkCmdDrawIndexedIndirect( _cmdId,
								  req_id->result.Get( VK_NULL_HANDLE ),
								  (VkDeviceSize) msg->offset,
								  msg->drawCount,
								  (uint32_t) msg->stride );

		_CheckGraphicsWritableResources();
		return true;
	}
	
/*
=================================================
	_CmdDispatch
=================================================
*/
	bool Vk1CommandBuilder::_CmdDispatch (const Message< GpuMsg::CmdDispatch > &msg)
	{
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( _CheckComputePipeline() );

		vkCmdDispatch( _cmdId, msg->groupCount.x, msg->groupCount.y, msg->groupCount.z );

		_CheckComputeWritableResources();
		return true;
	}
	
/*
=================================================
	_CmdDispatchIndirect
=================================================
*/
	bool Vk1CommandBuilder::_CmdDispatchIndirect (const Message< GpuMsg::CmdDispatchIndirect > &msg)
	{
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->indirectBuffer );
		CHECK_ERR( _CheckComputePipeline() );
		
		Message< GpuMsg::GetVkBufferID >		req_id;
		Message< GpuMsg::GetBufferDescriptor >	req_descr;

		SendTo( msg->indirectBuffer, req_id );
		SendTo( msg->indirectBuffer, req_descr );

		ASSERT( req_descr->result->usage[ EBufferUsage::Indirect ] );
		ASSERT( msg->offset < req_descr->result->size );

		vkCmdDispatchIndirect( _cmdId, *req_id->result, VkDeviceSize(msg->offset) );

		_CheckComputeWritableResources();
		return true;
	}

/*
=================================================
	_CmdExecute
=================================================
*/
	bool Vk1CommandBuilder::_CmdExecute (const Message< GpuMsg::CmdExecute > &msg)
	{
		CHECK_ERR( _scope == EScope::Command );

		CmdBuffers_t	cmd_buffers;

		FOR( i, msg->cmdBuffers )
		{
			auto const&	cmd = msg->cmdBuffers[i];

			DEBUG_ONLY(
				Message< GpuMsg::GetCommandBufferState >	req_state;
				SendTo( cmd, req_state );
				CHECK( req_state->result.Get() == GpuMsg::GetCommandBufferState::EState::Executable );
			);

			Message< GpuMsg::GetVkCommandBufferID >		req_id;
			SendTo( cmd, req_id );
			ASSERT( req_id->result and *req_id->result != VK_NULL_HANDLE );

			cmd_buffers.PushBack( *req_id->result );
			_resources.Add( cmd );
		}
			
		ASSERT( not cmd_buffers.Empty() );
		vkCmdExecuteCommands( _cmdId, Cast<uint32_t>(cmd_buffers.Count()), cmd_buffers.RawPtr() );

		return true;
	}
	
/*
=================================================
	_CmdBindGraphicsResourceTable
=================================================
*/
	bool Vk1CommandBuilder::_CmdBindGraphicsResourceTable (const Message< GpuMsg::CmdBindGraphicsResourceTable > &msg)
	{
		CHECK_ERR( _scope == EScope::RenderPass );
		return _BindDescriptorSet( msg->resourceTable, msg->index, VK_PIPELINE_BIND_POINT_GRAPHICS );
	}
	
/*
=================================================
	_CmdBindComputeResourceTable
=================================================
*/
	bool Vk1CommandBuilder::_CmdBindComputeResourceTable (const Message< GpuMsg::CmdBindComputeResourceTable > &msg)
	{
		CHECK_ERR( _scope == EScope::Command );
		return _BindDescriptorSet( msg->resourceTable, msg->index, VK_PIPELINE_BIND_POINT_COMPUTE );
	}
	
/*
=================================================
	_BindDescriptorSet
=================================================
*/
	bool Vk1CommandBuilder::_BindDescriptorSet (const ModulePtr &resourceTable, uint firstIndex, VkPipelineBindPoint bindPoint)
	{
		CHECK_ERR( resourceTable );
		
		Message< GpuMsg::GetVkPipelineLayoutID >		req_layout;
		Message< GpuMsg::GetVkPipelineResourceTableID >	req_id;

		SendTo( resourceTable, req_id );
		SendTo( resourceTable, req_layout );
		
		// TODO: check layout compatibility

		_resources.Add( resourceTable );

		VkDescriptorSet		descr_set = *req_id->result;

		vkCmdBindDescriptorSets( _cmdId,
								 bindPoint,
								 req_layout->result.Get( VK_NULL_HANDLE ),
								 firstIndex,
								 1, &descr_set,
								 0, null );
		return true;
	}
	
/*
=================================================
	_CmdCopyBuffer
=================================================
*/
	bool Vk1CommandBuilder::_CmdCopyBuffer (const Message< GpuMsg::CmdCopyBuffer > &msg)
	{
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->srcBuffer and msg->dstBuffer );
		
		Message< GpuMsg::GetVkBufferID >		req_src_id;
		Message< GpuMsg::GetVkBufferID >		req_dst_id;
		Message< GpuMsg::GetBufferDescriptor >	req_src_descr;
		Message< GpuMsg::GetBufferDescriptor >	req_dst_descr;

		SendTo( msg->srcBuffer, req_src_id );
		SendTo( msg->dstBuffer, req_dst_id );
		SendTo( msg->srcBuffer, req_src_descr );
		SendTo( msg->dstBuffer, req_dst_descr );
		
		ASSERT( req_src_descr->result->usage[ EBufferUsage::TransferSrc ] );
		ASSERT( req_dst_descr->result->usage[ EBufferUsage::TransferDst ] );

		BufferCopyRegions_t	regions;

		FOR( i, msg->regions )
		{
			auto const&		src = msg->regions[i];
			VkBufferCopy	dst;

			dst.srcOffset	= (VkDeviceSize) src.srcOffset;
			dst.dstOffset	= (VkDeviceSize) src.dstOffset;
			dst.size		= (VkDeviceSize) src.size;

			regions.PushBack( dst );

			ASSERT( src.size + src.srcOffset <= req_src_descr->result->size );
			ASSERT( src.size + src.dstOffset <= req_dst_descr->result->size );
		}
		
		_resources.Add( msg->srcBuffer );
		_resources.Add( msg->dstBuffer );

		vkCmdCopyBuffer( _cmdId,
						 req_src_id->result.Get( VK_NULL_HANDLE ),
						 req_dst_id->result.Get( VK_NULL_HANDLE ),
						 (uint32_t) regions.Count(),
						 regions.ptr() );
		return true;
	}
	
/*
=================================================
	_CmdCopyImage
=================================================
*/
	bool Vk1CommandBuilder::_CmdCopyImage (const Message< GpuMsg::CmdCopyImage > &msg)
	{
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->srcImage and msg->dstImage );

		Message< GpuMsg::GetVkImageID >			req_src_id;
		Message< GpuMsg::GetVkImageID >			req_dst_id;
		Message< GpuMsg::GetImageDescriptor >	req_src_descr;
		Message< GpuMsg::GetImageDescriptor >	req_dst_descr;
		
		SendTo( msg->srcImage, req_src_id );
		SendTo( msg->dstImage, req_dst_id );
		SendTo( msg->srcImage, req_src_descr );
		SendTo( msg->dstImage, req_dst_descr );
		
		ASSERT( req_src_descr->result->usage[ EImageUsage::TransferSrc ] );
		ASSERT( req_dst_descr->result->usage[ EImageUsage::TransferDst ] );
		ASSERT( msg->srcLayout == EImageLayout::TransferSrcOptimal or msg->srcLayout == EImageLayout::General );
		ASSERT( msg->dstLayout == EImageLayout::TransferDstOptimal or msg->dstLayout == EImageLayout::General );
		ASSERT( EPixelFormat::BitPerPixel( req_src_descr->result->format ) == EPixelFormat::BitPerPixel( req_dst_descr->result->format ) );
		ASSERT( req_src_descr->result->samples == req_dst_descr->result->samples );

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

			ASSERT( src.srcLayers.mipLevel < req_src_descr->result->maxLevel );
			ASSERT( src.dstLayers.mipLevel < req_dst_descr->result->maxLevel );
			ASSERT( src.srcLayers.baseLayer.Get() + src.srcLayers.layerCount <= req_src_descr->result->dimension.w );
			ASSERT( src.dstLayers.baseLayer.Get() + src.dstLayers.layerCount <= req_dst_descr->result->dimension.w );
			ASSERT(All( src.srcOffset + src.size <= Max(1u, req_src_descr->result->dimension.xyz() >> src.srcLayers.mipLevel.Get()) ));
			ASSERT(All( src.dstOffset + src.size <= Max(1u, req_dst_descr->result->dimension.xyz() >> src.dstLayers.mipLevel.Get()) ));

			regions.PushBack( dst );
		}
		
		_resources.Add( msg->srcImage );
		_resources.Add( msg->dstImage );

		vkCmdCopyImage( _cmdId,
						req_src_id->result.Get(),
						Vk1Enum( msg->srcLayout ),
						req_dst_id->result.Get(),
						Vk1Enum( msg->dstLayout ),
						(uint32_t) regions.Count(),
						(VkImageCopy const*) regions.ptr() );
		return true;
	}
	
/*
=================================================
	_CmdCopyBufferToImage
=================================================
*/
	bool Vk1CommandBuilder::_CmdCopyBufferToImage (const Message< GpuMsg::CmdCopyBufferToImage > &msg)
	{
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->srcBuffer and msg->dstImage );
		
		Message< GpuMsg::GetVkBufferID >		req_src_id;
		Message< GpuMsg::GetVkImageID >			req_dst_id;
		Message< GpuMsg::GetBufferDescriptor >	req_src_descr;
		Message< GpuMsg::GetImageDescriptor >	req_dst_descr;
		
		SendTo( msg->srcBuffer, req_src_id );
		SendTo( msg->srcBuffer, req_src_descr );
		SendTo( msg->dstImage, req_dst_id );
		SendTo( msg->dstImage, req_dst_descr );
		
		ASSERT( req_src_descr->result->usage[ EBufferUsage::TransferSrc ] );
		ASSERT( req_dst_descr->result->usage[ EImageUsage::TransferDst ] );
		ASSERT( msg->dstLayout == EImageLayout::TransferDstOptimal or msg->dstLayout == EImageLayout::General );

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
		
		_resources.Add( msg->srcBuffer );
		_resources.Add( msg->dstImage );

		vkCmdCopyBufferToImage( _cmdId,
								req_src_id->result.Get(),
								req_dst_id->result.Get(),
								Vk1Enum( msg->dstLayout ),
								(uint32_t) regions.Count(),
								regions.ptr() );
		return true;
	}
	
/*
=================================================
	_CmdCopyImageToBuffer
=================================================
*/
	bool Vk1CommandBuilder::_CmdCopyImageToBuffer (const Message< GpuMsg::CmdCopyImageToBuffer > &msg)
	{
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->srcImage and msg->dstBuffer );

		Message< GpuMsg::GetVkImageID >			req_src_id;
		Message< GpuMsg::GetVkBufferID >		req_dst_id;
		Message< GpuMsg::GetImageDescriptor >	req_src_descr;
		Message< GpuMsg::GetBufferDescriptor >	req_dst_descr;
		
		SendTo( msg->dstBuffer, req_dst_id );
		SendTo( msg->dstBuffer, req_dst_descr );
		SendTo( msg->srcImage, req_src_id );
		SendTo( msg->srcImage, req_src_descr );
		
		ASSERT( req_src_descr->result->usage[ EImageUsage::TransferSrc ] );
		ASSERT( req_dst_descr->result->usage[ EBufferUsage::TransferDst ] );
		ASSERT( msg->srcLayout == EImageLayout::TransferSrcOptimal or msg->srcLayout == EImageLayout::General );

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
		
		_resources.Add( msg->srcImage );
		_resources.Add( msg->dstBuffer );

		vkCmdCopyImageToBuffer( _cmdId,
								req_src_id->result.Get(),
								Vk1Enum( msg->srcLayout ),
								req_dst_id->result.Get(),
								(uint32_t) regions.Count(),
								regions.ptr() );
		return true;
	}
	
/*
=================================================
	_CmdBlitImage
=================================================
*/
	bool Vk1CommandBuilder::_CmdBlitImage (const Message< GpuMsg::CmdBlitImage > &msg)
	{
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->srcImage and msg->dstImage );
		
		Message< GpuMsg::GetVkImageID >			req_src_id;
		Message< GpuMsg::GetVkImageID >			req_dst_id;
		Message< GpuMsg::GetImageDescriptor >	req_src_descr;
		Message< GpuMsg::GetImageDescriptor >	req_dst_descr;
		
		SendTo( msg->srcImage, req_src_id );
		SendTo( msg->dstImage, req_dst_id );
		SendTo( msg->srcImage, req_src_descr );
		SendTo( msg->dstImage, req_dst_descr );
		
		ASSERT( req_src_descr->result->usage[ EImageUsage::TransferSrc ] );
		ASSERT( req_dst_descr->result->usage[ EImageUsage::TransferDst ] );
		ASSERT( msg->srcLayout == EImageLayout::TransferSrcOptimal or msg->srcLayout == EImageLayout::General );
		ASSERT( msg->dstLayout == EImageLayout::TransferDstOptimal or msg->dstLayout == EImageLayout::General );

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
		
		_resources.Add( msg->srcImage );
		_resources.Add( msg->dstImage );

		vkCmdBlitImage( _cmdId,
						req_src_id->result.Get(),
						Vk1Enum( msg->srcLayout ),
						req_dst_id->result.Get(),
						Vk1Enum( msg->dstLayout ),
						(uint32_t) regions.Count(),
						regions.ptr(),
						msg->linearFilter ? VK_FILTER_LINEAR: VK_FILTER_NEAREST );	// TODO
		return true;
	}
	
/*
=================================================
	_GpuCmdUpdateBuffer
=================================================
*/
	bool Vk1CommandBuilder::_CmdUpdateBuffer (const Message< GpuMsg::CmdUpdateBuffer > &msg)
	{
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->dstBuffer and not msg->data.Empty() and msg->data.Size() < _maxUpdateBuffer );
		
		Message< GpuMsg::GetVkBufferID >		req_id;
		Message< GpuMsg::GetBufferDescriptor >	req_descr;
		
		// TODO: check result
		SendTo( msg->dstBuffer, req_id );
		SendTo( msg->dstBuffer, req_descr );
		
		ASSERT( req_descr->result->usage[ EBufferUsage::TransferDst ] );
		
		_resources.Add( msg->dstBuffer );

		vkCmdUpdateBuffer( _cmdId,
						   req_id->result.Get(),
						   (VkDeviceSize) msg->dstOffset,
						   (VkDeviceSize) msg->data.Size(),
						   msg->data.ptr() );
		return true;
	}
	
/*
=================================================
	_CmdFillBuffer
=================================================
*/
	bool Vk1CommandBuilder::_CmdFillBuffer (const Message< GpuMsg::CmdFillBuffer > &msg)
	{
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->dstBuffer );
		
		Message< GpuMsg::GetVkBufferID >		req_id;
		Message< GpuMsg::GetBufferDescriptor >	req_descr;
		
		// TODO: check result
		SendTo( msg->dstBuffer, req_id );
		SendTo( msg->dstBuffer, req_descr );
		
		ASSERT( req_descr->result->usage[ EBufferUsage::TransferDst ] );
		
		_resources.Add( msg->dstBuffer );

		vkCmdFillBuffer( _cmdId,
						 req_id->result.Get(),
						 (VkDeviceSize) msg->dstOffset,
						 (VkDeviceSize) Min( msg->size, req_descr->result.Get().size - msg->dstOffset ),
						 msg->pattern );
		return true;
	}
	
/*
=================================================
	_CmdClearAttachments
=================================================
*/
	bool Vk1CommandBuilder::_CmdClearAttachments (const Message< GpuMsg::CmdClearAttachments > &msg)
	{
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

		vkCmdClearAttachments( _cmdId,
							   (uint32_t) attachments.Count(),
							   attachments.ptr(),
							   (uint32_t) clear_rects.Count(),
							   clear_rects.ptr() );
		return true;
	}
	
/*
=================================================
	_CmdClearColorImage
=================================================
*/
	bool Vk1CommandBuilder::_CmdClearColorImage (const Message< GpuMsg::CmdClearColorImage > &msg)
	{
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->image );
		
		Message< GpuMsg::GetVkImageID >			req_id;
		Message< GpuMsg::GetImageDescriptor >	req_descr;
		
		// TODO: check result
		SendTo( msg->image, req_id );
		SendTo( msg->image, req_descr );
		
		ASSERT( req_descr->result->usage[ EImageUsage::TransferDst ] );
		ASSERT( msg->layout == EImageLayout::TransferDstOptimal or msg->layout == EImageLayout::General );

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
		
		_resources.Add( msg->image );

		vkCmdClearColorImage( _cmdId,
							  req_id->result.Get(),
							  Vk1Enum( msg->layout ),
							  &clear_value,
							  (uint32_t) ranges.Count(),
							  ranges.ptr() );
		return true;
	}
	
/*
=================================================
	_CmdClearDepthStencilImage
=================================================
*/
	bool Vk1CommandBuilder::_CmdClearDepthStencilImage (const Message< GpuMsg::CmdClearDepthStencilImage > &msg)
	{
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->image );
		
		Message< GpuMsg::GetVkImageID >			req_id;
		Message< GpuMsg::GetImageDescriptor >	req_descr;
		
		// TODO: check result
		SendTo( msg->image, req_id );
		SendTo( msg->image, req_descr );
		
		ASSERT( req_descr->result->usage[ EImageUsage::TransferDst ] );
		ASSERT( msg->layout == EImageLayout::TransferDstOptimal or msg->layout == EImageLayout::General );

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
		
		_resources.Add( msg->image );

		vkCmdClearDepthStencilImage( _cmdId,
									 req_id->result.Get(),
									 Vk1Enum( msg->layout ),
									 &clear_value,
									 (uint32_t) ranges.Count(),
									 ranges.ptr() );
		return true;
	}

/*
=================================================
	_CmdSetEvent
=================================================
*/
	bool Vk1CommandBuilder::_CmdSetEvent (const Message< GpuMsg::CmdSetEvent > &msg)
	{
		CHECK_ERR( _scope == EScope::Command );

		Message< GpuMsg::GetVkEvent >	req_event{ msg->eventId };
		_syncManager->Send( req_event );

		vkCmdSetEvent( _cmdId, *req_event->result, Vk1Enum( msg->stageMask ) );
		return true;
	}

/*
=================================================
	_CmdResetEvent
=================================================
*/
	bool Vk1CommandBuilder::_CmdResetEvent (const Message< GpuMsg::CmdResetEvent > &msg)
	{
		CHECK_ERR( _scope == EScope::Command );

		Message< GpuMsg::GetVkEvent >	req_event{ msg->eventId };
		_syncManager->Send( req_event );

		vkCmdResetEvent( _cmdId, *req_event->result, Vk1Enum( msg->stageMask ) );
		return true;
	}
	
/*
=================================================
	_CmdWaitEvents
=================================================
*/
	bool Vk1CommandBuilder::_CmdWaitEvents (const Message< GpuMsg::CmdWaitEvents > &msg)
	{
		TODO("");
		return false;
	}

/*
=================================================
	_CmdPipelineBarrier
=================================================
*/
	bool Vk1CommandBuilder::_CmdPipelineBarrier (const Message< GpuMsg::CmdPipelineBarrier > &msg)
	{
		CHECK_ERR( _scope == EScope::Command );

		MemoryBarriers_t		mem_barriers;
		ImageMemoryBarriers_t	img_barriers;
		BufferMemoryBarriers_t	buf_barriers;

		uint32_t	queue_index	= GetDevice()->GetQueueIndex();

		FOR( i, msg->memoryBarriers )
		{
			const auto&		src = msg->memoryBarriers[i];
			VkMemoryBarrier	dst = {};

			dst.sType			= VK_STRUCTURE_TYPE_MEMORY_BARRIER;
			dst.srcAccessMask	= Vk1Enum( src.srcAccessMask );
			dst.dstAccessMask	= Vk1Enum( src.dstAccessMask );

			mem_barriers.PushBack( dst );
		}

		FOR( i, msg->bufferBarriers )
		{
			const auto&				src = msg->bufferBarriers[i];
			VkBufferMemoryBarrier	dst = {};
			
			// TODO: check result
			Message< GpuMsg::GetVkBufferID >	req_id;
			SendTo( src.buffer, req_id );

			dst.sType				= VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
			dst.srcAccessMask		= Vk1Enum( src.srcAccessMask );
			dst.dstAccessMask		= Vk1Enum( src.dstAccessMask );
			dst.srcQueueFamilyIndex	= queue_index;
			dst.dstQueueFamilyIndex	= queue_index;
			dst.buffer				= *req_id->result;
			dst.offset				= (VkDeviceSize) src.offset;
			dst.size				= (VkDeviceSize) src.size;

			buf_barriers.PushBack( dst );
		}
		
		FOR( i, msg->imageBarriers )
		{
			const auto&				src = msg->imageBarriers[i];
			VkImageMemoryBarrier	dst = {};
			
			// TODO: check result
			Message< GpuMsg::GetVkImageID >	req_id;
			SendTo( src.image, req_id );

			dst.sType							= VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			dst.srcAccessMask					= Vk1Enum( src.srcAccessMask );
			dst.dstAccessMask					= Vk1Enum( src.dstAccessMask );
			dst.oldLayout						= Vk1Enum( src.oldLayout );
			dst.newLayout						= Vk1Enum( src.newLayout );
			dst.srcQueueFamilyIndex				= queue_index;
			dst.dstQueueFamilyIndex				= queue_index;
			dst.image							= *req_id->result;
			dst.subresourceRange.aspectMask		= Vk1Enum( src.range.aspectMask );
			dst.subresourceRange.baseMipLevel	= src.range.baseMipLevel.Get();
			dst.subresourceRange.levelCount		= src.range.levelCount;
			dst.subresourceRange.baseArrayLayer	= src.range.baseLayer.Get();
			dst.subresourceRange.layerCount		= src.range.layerCount;

			img_barriers.PushBack( dst );
		}

		vkCmdPipelineBarrier( _cmdId,
							  Vk1Enum( msg->srcStageMask ),
							  Vk1Enum( msg->dstStageMask ),
							  0,								// TODO
							  (uint32_t) mem_barriers.Count(),  mem_barriers.RawPtr(),
							  (uint32_t) buf_barriers.Count(),  buf_barriers.RawPtr(),
							  (uint32_t) img_barriers.Count(),  img_barriers.RawPtr() );
		return true;
	}
	
/*
=================================================
	_CmdPushConstants
=================================================
*/
	bool Vk1CommandBuilder::_CmdPushConstants (const Message< GpuMsg::CmdPushConstants > &msg)
	{
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->pipelineLayout and not msg->data.Empty() );

		Message< GpuMsg::GetVkPipelineLayoutID >	req_layout;
		SendTo( msg->pipelineLayout, req_layout );

		vkCmdPushConstants( _cmdId, *req_layout->result, Vk1Enum( msg->stages ), uint32_t(msg->offset), uint32_t(msg->data.Size()), msg->data.RawPtr() );
		return true;
	}
	
/*
=================================================
	_CmdPushNamedConstants
=================================================
*/
	bool Vk1CommandBuilder::_CmdPushNamedConstants (const Message< GpuMsg::CmdPushNamedConstants > &msg)
	{
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->pipelineLayout and not msg->values.Empty() );

		using Iter_t = GpuMsg::GetVkPipelineLayoutPushConstants::PushConstants_t::iterator;

		Message< GpuMsg::GetVkPipelineLayoutPushConstants >	req_pc;
		Message< GpuMsg::GetVkPipelineLayoutID >			req_layout;

		SendTo( msg->pipelineLayout, req_pc );
		SendTo( msg->pipelineLayout, req_layout );

		FixedSizeArray<ubyte, 32*4>	data;
		EShader::bits				stages;

		FOR( i, msg->values )
		{
			Iter_t			iter;
			BinArrayCRef	val = msg->values[i].second.GetData();

			ASSERT( req_pc->result->Find( msg->values[i].first, OUT iter ) );
			ASSERT( iter->second.size == val.Size() );
			ASSERT( usize(iter->second.offset) + iter->second.size < data.Count() );

			UnsafeMem::MemCopy( &data[iter->second.offset], val.ptr(), val.Size() );

			stages |= iter->second.stages;
		}
		
		vkCmdPushConstants( _cmdId, *req_layout->result, Vk1Enum( stages ), 0, uint32_t(data.Size()), data.RawPtr() );
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
		CHECK_ERR( not _IsCreated() );

		VkCommandPoolCreateInfo		pool_info	= {};
		pool_info.sType				= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		pool_info.queueFamilyIndex	= GetDevice()->GetQueueIndex();
		pool_info.flags				= VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		VK_CHECK( vkCreateCommandPool( GetVkDevice(), &pool_info, null, OUT &_cmdPool ) );

		GetDevice()->SetObjectName( ReferenceCast<uint64_t>(_cmdPool), GetDebugName(), EGpuObject::CommandPool );
		return true;
	}

/*
=================================================
	_DestroyCmdBufferPool
=================================================
*/
	void Vk1CommandBuilder::_DestroyCmdBufferPool ()
	{
		auto	dev = GetVkDevice();

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
	_CheckGraphicsPipeline
=================================================
*/
	bool Vk1CommandBuilder::_CheckGraphicsPipeline ()
	{
		// TODO
		return true;
	}
	
/*
=================================================
	_CheckComputePipeline
=================================================
*/
	bool Vk1CommandBuilder::_CheckComputePipeline ()
	{
		// TODO
		return true;
	}
	
/*
=================================================
	_CheckGraphicsWritableResources
=================================================
*/
	bool Vk1CommandBuilder::_CheckGraphicsWritableResources ()
	{
		// TODO
		return true;
	}
	
/*
=================================================
	_CheckComputeWritableResources
=================================================
*/
	bool Vk1CommandBuilder::_CheckComputeWritableResources ()
	{
		// TODO
		return true;
	}

/*
=================================================
	_CheckDynamicState
=================================================
*/
	bool Vk1CommandBuilder::_CheckDynamicState (EPipelineDynamicState::type state) const
	{
		return _dynamicStates[ state ];
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
			//CHECK_ERR( fb.colorAttachments[i].imageType == 
			//				ERenderTarget::FromPixelFormat( rp.ColorAttachments()[i].format, uint(i) ) );

			CHECK_ERR( EImage::IsMultisampled( fb.colorAttachments[i].imageType ) ==
							(rp.ColorAttachments()[i].samples > MultiSamples(1)) );
		}

		if ( fb.depthStencilAttachment.IsEnabled() )
		{
			//CHECK_ERR( fb.depthStencilAttachment.imageType == 
			//				ERenderTarget::FromPixelFormat( rp.DepthStencilAttachment().format ) );

			CHECK_ERR( EImage::IsMultisampled( fb.depthStencilAttachment.imageType ) ==
							(rp.DepthStencilAttachment().samples > MultiSamples(1)) );
		}

		return true;
	}

}	// PlatformVK
//-----------------------------------------------------------------------------
	
namespace Platforms
{
	ModulePtr VulkanObjectsConstructor::CreateVk1CommandBuilder (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci)
	{
		return New< PlatformVK::Vk1CommandBuilder >( id, gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
