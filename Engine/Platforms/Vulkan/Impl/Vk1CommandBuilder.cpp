// Copyright �  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/CommandBuffer.h"
#include "Engine/Platforms/Shared/GPU/Image.h"
#include "Engine/Platforms/Shared/GPU/Buffer.h"
#include "Engine/Platforms/Shared/GPU/Framebuffer.h"
#include "Engine/Platforms/Shared/GPU/RenderPass.h"
#include "Engine/Platforms/Shared/GPU/Pipeline.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/VulkanContext.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{

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
											GpuMsg::CmdPipelineBarrier,
											GpuMsg::SetCommandBufferDependency,
											GpuMsg::GetCommandBufferState,
											GpuMsg::GetVkCommandPoolID
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;

		using DynamicStates_t		= EPipelineDynamicState::bits;
		//using Layout_t			= Optional< Vk1PipelineLayoutPtr >;
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
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		vk::VkCommandPool		_cmdPool;

		UsedResources_t			_resources;
		ModulePtr				_cmdBuffer;		// current command buffer
		vk::VkCommandBuffer		_cmdId;			// cached id
		EScope					_scope;

		//Layout_t				_currGraphicsLayout;
		//Layout_t				_currComputeLayout;

		DynamicStates_t			_dynamicStates;
		uint					_subpassIndex;
		uint					_maxSubpasses;
		//bool					_hasIndexBuffer;


	// methods
	public:
		Vk1CommandBuilder (const GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci);
		~Vk1CommandBuilder ();


	// message handlers
	private:
		bool _Compose (const  Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _SetCommandBufferDependency (const Message< GpuMsg::SetCommandBufferDependency > &);
		bool _GetCommandBufferState (const Message< GpuMsg::GetCommandBufferState > &);
		
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
		bool _CmdPipelineBarrier (const Message< GpuMsg::CmdPipelineBarrier > &);
		bool _GetVkCommandPoolID (const Message< GpuMsg::GetVkCommandPoolID > &);
		
	private:
		bool _IsCreated () const;
		bool _CreateCmdBufferPool ();
		void _DestroyCmdBufferPool ();
		
		bool _BindDescriptorSet (const ModulePtr &resourceTable, uint firstIndex, vk::VkPipelineBindPoint bindPoint);

		bool _CheckGraphicsPipeline ();
		bool _CheckComputePipeline ();

		bool _CheckDynamicState (EPipelineDynamicState::type state) const;
		bool _CheckCompatibility (const FramebufferDescriptor &fbDescr,
								  const RenderPassDescriptor &rpDescr) const;
	};
//-----------------------------------------------------------------------------


	
	using Viewports_t				= FixedSizeArray< vk::VkViewport, GlobalConst::Graphics_MaxColorBuffers >;
	using Scissors_t				= FixedSizeArray< vk::VkRect2D, GlobalConst::Graphics_MaxColorBuffers >;
	using ClearValues_t				= FixedSizeArray< vk::VkClearValue, GlobalConst::Graphics_MaxColorBuffers >;
	using VertexBuffers_t			= FixedSizeArray< vk::VkBuffer, GlobalConst::Graphics_MaxAttribs >;
	using CmdBuffers_t				= FixedSizeArray< vk::VkCommandBuffer, GpuMsg::CmdExecute::CmdBuffers_t::MemoryContainer_t::SIZE >;
	using BufferCopyRegions_t		= FixedSizeArray< vk::VkBufferCopy, GpuMsg::CmdCopyBuffer::Regions_t::MemoryContainer_t::SIZE >;
	using ImageCopyRegions_t		= FixedSizeArray< vk::VkImageCopy, GpuMsg::CmdCopyImage::Regions_t::MemoryContainer_t::SIZE >;
	using BufImgCopyRegions_t		= FixedSizeArray< vk::VkBufferImageCopy, GpuMsg::CmdCopyBufferToImage::Regions_t::MemoryContainer_t::SIZE >;
	using ImgBlitRegions_t			= FixedSizeArray< vk::VkImageBlit, GpuMsg::CmdBlitImage::Regions_t::MemoryContainer_t::SIZE >;
	using ClearAttachments_t		= FixedSizeArray< vk::VkClearAttachment, GpuMsg::CmdClearAttachments::Attachments_t::MemoryContainer_t::SIZE >;
	using ClearRects_t				= FixedSizeArray< vk::VkClearRect, GpuMsg::CmdClearAttachments::ClearRects_t::MemoryContainer_t::SIZE >;
	using ImageRanges_t				= FixedSizeArray< vk::VkImageSubresourceRange, GpuMsg::CmdClearColorImage::Ranges_t::MemoryContainer_t::SIZE >;
	using MemoryBarriers_t			= FixedSizeArray< vk::VkMemoryBarrier, GpuMsg::CmdPipelineBarrier::MAX_BARRIERS >;
	using ImageMemoryBarriers_t		= FixedSizeArray< vk::VkImageMemoryBarrier, GpuMsg::CmdPipelineBarrier::MAX_BARRIERS >;
	using BufferMemoryBarriers_t	= FixedSizeArray< vk::VkBufferMemoryBarrier, GpuMsg::CmdPipelineBarrier::MAX_BARRIERS >;


	const Runtime::VirtualTypeList	Vk1CommandBuilder::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	Vk1CommandBuilder::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1CommandBuilder::Vk1CommandBuilder (const GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci) :
		Vk1BaseModule( gs, ci.gpuThread, ModuleConfig{ VkCommandBuilderModuleID, ~0u }, &_msgTypes, &_eventTypes ),
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
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GetVkLogicDevice );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_GetVkCommandPoolID );
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_DeviceBeforeDestroy );
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
		_SubscribeOnMsg( this, &Vk1CommandBuilder::_CmdPipelineBarrier );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, Platforms::VkThreadModuleID, true );
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
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreateCmdBufferPool() );

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
		using namespace vk;
		
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::Viewport ) );

		Viewports_t		viewports;		viewports.Resize( msg->viewports.Count() );

		FOR( i, msg->viewports )
		{
			auto const&	vp = msg->viewports[i];

			viewports[i] = VkViewport{	float(vp.offset.x), float(vp.offset.y),
										float(vp.size.x),   float(vp.size.y),
										vp.depthRange.x,    vp.depthRange.y   };
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
		using namespace vk;
		
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::Scissor ) );

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
		using namespace vk;
		
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::DepthBounds ) );

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
		using namespace vk;
		
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::BlendConstants ) );
		
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
		using namespace vk;
		
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::DepthBias ) );
		
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
		using namespace vk;
		
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::LineWidth ) );
		
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
		using namespace vk;
		
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::StencilCompareMask ) );
		
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
		using namespace vk;
		
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::StencilWriteMask ) );
		
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
		using namespace vk;
		
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _CheckDynamicState( EPipelineDynamicState::StencilReference ) );
		
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
		using namespace vk;

		CHECK_ERR( not _cmdBuffer );
		CHECK_ERR( _scope == EScope::None );
		
		// use target command buffer
		if ( msg->targetCmdBuffer )
		{
			// it is bad practice, becouse command buffer depends of other commands pool
			//ASSERT( msg->targetCmdBuffer->_GetParents().IsExist( this ) );
			
			_cmdBuffer = msg->targetCmdBuffer;
		}
		else
		// create new command buffer
		{
			CHECK_ERR( GlobalSystems()->Get< ModulesFactory >()->Create(
							Platforms::VkCommandBufferModuleID,
							GlobalSystems(),
							CreateInfo::GpuCommandBuffer{
								_GetManager(),
								this,
								CommandBufferDescriptor{ msg->isSecondary, true }
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

		_cmdId << req_cmd_id->result;
		CHECK_ERR( _cmdId != VK_NULL_HANDLE );
		
		// begin
		VkCommandBufferBeginInfo	cmd_info = {};
		cmd_info.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		cmd_info.pNext				= null;
		cmd_info.flags				= 0;	// TODO
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
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		
		VK_CHECK( vkEndCommandBuffer( _cmdId ) );

		SendTo( _cmdBuffer, Message< GpuMsg::SetCommandBufferDependency >{ RVREF(_resources) } );
		SendTo( _cmdBuffer, Message< GpuMsg::SetCommandBufferState >{ ERecordingState::Executable } );

		msg->cmdBuffer.Set( _cmdBuffer );

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
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( msg->framebuffer );
		CHECK_ERR( msg->renderPass );
		CHECK_ERR( _scope == EScope::Command );

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
		pass_info.renderPass				<< req_pass->result;
		pass_info.renderArea.offset.x		= msg->area.left;
		pass_info.renderArea.offset.y		= msg->area.bottom;
		pass_info.renderArea.extent.width	= msg->area.Width();
		pass_info.renderArea.extent.height	= msg->area.Height();
		pass_info.clearValueCount			= (uint32_t) clear_values.Count();
		pass_info.pClearValues				= clear_values.ptr();
		pass_info.framebuffer				<< req_fb->result;
		
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
	_CmdBeginRenderPassID
=================================================
*
	bool Vk1CommandBuilder::_CmdBeginRenderPassID (const Message< GpuMsg::CmdBeginRenderPassID > &msg)
	{
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_CmdEndRenderPass
=================================================
*/
	bool Vk1CommandBuilder::_CmdEndRenderPass (const Message< GpuMsg::CmdEndRenderPass > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
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
	bool Vk1CommandBuilder::_CmdNextSubpass (const Message< GpuMsg::CmdNextSubpass > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
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
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg->pipeline );

		Message< GpuMsg::GetVkGraphicsPipelineID >			req_id;
		Message< GpuMsg::GetGraphicsPipelineDescriptor >	req_descr;
		
		// TODO: check result
		SendTo( msg->pipeline, req_id );
		SendTo( msg->pipeline, req_descr );

		CHECK_ERR( req_id->result.IsDefined() and req_descr->result.IsDefined() );

		VkPipeline					pipeline	= req_id->result.Get( VK_NULL_HANDLE );
		GraphicsPipelineDescriptor	descr;		descr << req_descr->result;

		CHECK_ERR( descr.subpass == _subpassIndex );

		_resources.Add( msg->pipeline );

		vkCmdBindPipeline( _cmdId, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline );

		_dynamicStates = descr.dynamicStates;
		return true;
	}
	
/*
=================================================
	_CmdBindComputePipeline
=================================================
*/
	bool Vk1CommandBuilder::_CmdBindComputePipeline (const Message< GpuMsg::CmdBindComputePipeline > &msg)
	{
		TODO( "" );
		return false;
	}

/*
=================================================
	_CmdBindGraphicsPipelineID
=================================================
*
	bool Vk1CommandBuilder::_CmdBindGraphicsPipelineID (const Message< GpuMsg::CmdBindGraphicsPipelineID > &msg)
	{
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_CmdBindComputePipelineID
=================================================
*
	bool Vk1CommandBuilder::_CmdBindComputePipelineID (const Message< GpuMsg::CmdBindComputePipelineID > &msg)
	{
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_CmdBindVertexBuffers
=================================================
*/
	bool Vk1CommandBuilder::_CmdBindVertexBuffers (const Message< GpuMsg::CmdBindVertexBuffers > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );

		CompileTime::MustBeSameSize< VkDeviceSize, decltype(msg->offsets)::value_t >();

		VertexBuffers_t		buffers;	buffers.Resize( msg->vertexBuffers.Count() );

		FOR( i, msg->vertexBuffers )
		{
			auto const&	vb = msg->vertexBuffers[i];

			Message< GpuMsg::GetVkBufferID >		req_id;
			Message< GpuMsg::GetBufferDescriptor >	req_descr;
			
			// TODO: check result
			SendTo( vb, req_id );
			SendTo( vb, req_descr );

			CHECK_ERR( req_descr->result.Get().usage.Get( EBufferUsage::Vertex ) );

			buffers[i] << req_id->result;
			
			_resources.Add( vb );
		}
		
		vkCmdBindVertexBuffers( _cmdId,
								msg->firstBinding,
								(uint32_t) buffers.Count(),
								buffers.ptr(),
								reinterpret_cast< VkDeviceSize const *>( msg->offsets.ptr() ) );
		return true;
	}
	
/*
=================================================
	_CmdBindVertexBufferIDs
=================================================
*
	bool Vk1CommandBuilder::_CmdBindVertexBufferIDs (const Message< GpuMsg::CmdBindVertexBufferIDs > &msg)
	{
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_CmdBindIndexBuffer
=================================================
*/
	bool Vk1CommandBuilder::_CmdBindIndexBuffer (const Message< GpuMsg::CmdBindIndexBuffer > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg->indexBuffer );

		Message< GpuMsg::GetVkBufferID >		req_id;
		Message< GpuMsg::GetBufferDescriptor >	req_descr;
		
		// TODO: check result
		SendTo( msg->indexBuffer, req_id );
		SendTo( msg->indexBuffer, req_descr );
		
		CHECK_ERR( req_descr->result.Get().usage.Get( EBufferUsage::Index ) );
		
		_resources.Add( msg->indexBuffer );

		vkCmdBindIndexBuffer( _cmdId,
							  req_id->result.Get( VK_NULL_HANDLE ),
							  (VkDeviceSize) msg->offset,
							  Vk1Enum( msg->indexType ) );
		return true;
	}
	
/*
=================================================
	_CmdBindIndexBufferID
=================================================
*
	bool Vk1CommandBuilder::_CmdBindIndexBufferID (const Message< GpuMsg::CmdBindIndexBufferID > &msg)
	{
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_CmdDraw
=================================================
*/
	bool Vk1CommandBuilder::_CmdDraw (const Message< GpuMsg::CmdDraw > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( _CheckGraphicsPipeline() );

		vkCmdDraw( _cmdId,
				   msg->vertexCount,
				   msg->instanceCount,
				   msg->firstVertex,
				   msg->firstInstance );
		return true;
	}
	
/*
=================================================
	_CmdDrawIndexed
=================================================
*/
	bool Vk1CommandBuilder::_CmdDrawIndexed (const Message< GpuMsg::CmdDrawIndexed > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( _CheckGraphicsPipeline() );
		
		vkCmdDrawIndexed( _cmdId,
						  msg->indexCount,
						  msg->instanceCount,
						  msg->firstVertex,
						  msg->vertexOffset,
						  msg->firstInstance );
		return true;
	}
	
/*
=================================================
	_CmdDrawIndirect
=================================================
*/
	bool Vk1CommandBuilder::_CmdDrawIndirect (const Message< GpuMsg::CmdDrawIndirect > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg->indirectBuffer );
		CHECK_ERR( _CheckGraphicsPipeline() );

		Message< GpuMsg::GetVkBufferID >		req_id;
		Message< GpuMsg::GetBufferDescriptor >	req_descr;
		
		// TODO: check result
		SendTo( msg->indirectBuffer, req_id );
		SendTo( msg->indirectBuffer, req_descr );
		
		CHECK_ERR( req_descr->result.Get().usage.Get( EBufferUsage::Indirect ) );
		
		_resources.Add( msg->indirectBuffer );

		vkCmdDrawIndirect(	_cmdId,
							req_id->result.Get( VK_NULL_HANDLE ),
							(VkDeviceSize) msg->offset,
							msg->drawCount,
							(vk::uint32_t) msg->stride );
		return true;
	}
	
/*
=================================================
	_CmdDrawIndexedIndirect
=================================================
*/
	bool Vk1CommandBuilder::_CmdDrawIndexedIndirect (const Message< GpuMsg::CmdDrawIndexedIndirect > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg->indirectBuffer );
		CHECK_ERR( _CheckGraphicsPipeline() );

		Message< GpuMsg::GetVkBufferID >		req_id;
		Message< GpuMsg::GetBufferDescriptor >	req_descr;
		
		// TODO: check result
		SendTo( msg->indirectBuffer, req_id );
		SendTo( msg->indirectBuffer, req_descr );
		
		CHECK_ERR( req_descr->result.Get().usage.Get( EBufferUsage::Indirect ) );
		
		_resources.Add( msg->indirectBuffer );

		vkCmdDrawIndexedIndirect( _cmdId,
								  req_id->result.Get( VK_NULL_HANDLE ),
								  (VkDeviceSize) msg->offset,
								  msg->drawCount,
								  (vk::uint32_t) msg->stride );
		return true;
	}
	
/*
=================================================
	_CmdDrawIndirectID
=================================================
*
	bool Vk1CommandBuilder::_CmdDrawIndirectID (const Message< GpuMsg::CmdDrawIndirectID > &msg)
	{
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_CmdDrawIndexedIndirectID
=================================================
*
	bool Vk1CommandBuilder::_CmdDrawIndexedIndirectID (const Message< GpuMsg::CmdDrawIndexedIndirectID > &msg)
	{
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_CmdDispatch
=================================================
*/
	bool Vk1CommandBuilder::_CmdDispatch (const Message< GpuMsg::CmdDispatch > &msg)
	{
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_CmdDispatchIndirect
=================================================
*/
	bool Vk1CommandBuilder::_CmdDispatchIndirect (const Message< GpuMsg::CmdDispatchIndirect > &msg)
	{
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_CmdDispatchID
=================================================
*
	bool Vk1CommandBuilder::_CmdDispatchID (const Message< GpuMsg::CmdDispatchID > &msg)
	{
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_CmdDispatchIndirectID
=================================================
*
	bool Vk1CommandBuilder::_CmdDispatchIndirectID (const Message< GpuMsg::CmdDispatchIndirectID > &msg)
	{
		TODO( "" );
		return false;
	}

/*
=================================================
	_CmdExecute
=================================================
*/
	bool Vk1CommandBuilder::_CmdExecute (const Message< GpuMsg::CmdExecute > &msg)
	{
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );

		CmdBuffers_t	cmd_buffers;

		FOR( i, msg->cmdBuffers )
		{
			auto const&	cmd = msg->cmdBuffers[i];
			
			// TODO: check result
			Message< GpuMsg::GetVkCommandBufferID >		req_id;
			SendTo( cmd, req_id );

			cmd_buffers.PushBack( req_id->result.Get( VK_NULL_HANDLE ) );

			_resources.Add( cmd );
		}
			
		vkCmdExecuteCommands( _cmdId, (uint32_t) cmd_buffers.Count(), cmd_buffers.ptr() );
		return true;
	}
	
/*
=================================================
	_CmdExecuteID
=================================================
*
	bool Vk1CommandBuilder::_CmdExecuteID (const Message< GpuMsg::CmdExecuteID > &msg)
	{
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_CmdBindGraphicsResourceTable
=================================================
*/
	bool Vk1CommandBuilder::_CmdBindGraphicsResourceTable (const Message< GpuMsg::CmdBindGraphicsResourceTable > &msg)
	{
		return _BindDescriptorSet( msg->resourceTable, msg->index, vk::VK_PIPELINE_BIND_POINT_GRAPHICS );
	}
	
/*
=================================================
	_CmdBindComputeResourceTable
=================================================
*/
	bool Vk1CommandBuilder::_CmdBindComputeResourceTable (const Message< GpuMsg::CmdBindComputeResourceTable > &msg)
	{
		return _BindDescriptorSet( msg->resourceTable, msg->index, vk::VK_PIPELINE_BIND_POINT_COMPUTE );
	}
	
/*
=================================================
	_BindDescriptorSet
=================================================
*/
	bool Vk1CommandBuilder::_BindDescriptorSet (const ModulePtr &resourceTable, uint firstIndex, vk::VkPipelineBindPoint bindPoint)
	{
		using namespace vk;
		
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( resourceTable );
		
		Message< GpuMsg::GetVkPipelineLayoutID >		req_layout;
		Message< GpuMsg::GetVkPipelineResourceTableID >	req_id;

		// TODO: check result
		SendTo( resourceTable, req_id );
		SendTo( resourceTable, req_layout );
		
		// TODO: check layout compatibility

		_resources.Add( resourceTable );

		vk::VkDescriptorSet		descr_set;	descr_set << req_id->result;

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
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->srcBuffer and msg->dstBuffer );
		
		Message< GpuMsg::GetVkBufferID >		req_src_id;
		Message< GpuMsg::GetVkBufferID >		req_dst_id;
		Message< GpuMsg::GetBufferDescriptor >	req_src_descr;
		Message< GpuMsg::GetBufferDescriptor >	req_dst_descr;

		// TODO: check result
		SendTo( msg->srcBuffer, req_src_id );
		SendTo( msg->dstBuffer, req_dst_id );
		SendTo( msg->srcBuffer, req_src_descr );
		SendTo( msg->dstBuffer, req_dst_descr );
		
		CHECK_ERR( req_src_descr->result.Get().usage.Get( EBufferUsage::TransferSrc ) );
		CHECK_ERR( req_dst_descr->result.Get().usage.Get( EBufferUsage::TransferDst ) );

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
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->srcImage and msg->dstImage );

		Message< GpuMsg::GetVkImageID >			req_src_id;
		Message< GpuMsg::GetVkImageID >			req_dst_id;
		Message< GpuMsg::GetImageDescriptor >	req_src_descr;
		Message< GpuMsg::GetImageDescriptor >	req_dst_descr;
		
		// TODO: check result
		SendTo( msg->srcImage, req_src_id );
		SendTo( msg->dstImage, req_dst_id );
		SendTo( msg->srcImage, req_src_descr );
		SendTo( msg->dstImage, req_dst_descr );
		
		CHECK_ERR( req_src_descr->result.Get().usage.Get( EImageUsage::TransferSrc ) );
		CHECK_ERR( req_dst_descr->result.Get().usage.Get( EImageUsage::TransferDst ) );

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
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->srcBuffer and msg->dstImage );
		
		Message< GpuMsg::GetVkBufferID >		req_src_id;
		Message< GpuMsg::GetVkImageID >			req_dst_id;
		Message< GpuMsg::GetBufferDescriptor >	req_src_descr;
		Message< GpuMsg::GetImageDescriptor >	req_dst_descr;
		
		// TODO: check result
		SendTo( msg->srcBuffer, req_src_id );
		SendTo( msg->srcBuffer, req_src_descr );
		SendTo( msg->dstImage, req_dst_id );
		SendTo( msg->dstImage, req_dst_descr );
		
		CHECK_ERR( req_src_descr->result.Get().usage.Get( EBufferUsage::TransferSrc ) );
		CHECK_ERR( req_dst_descr->result.Get().usage.Get( EImageUsage::TransferDst ) );

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
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->srcImage and msg->dstBuffer );

		Message< GpuMsg::GetVkImageID >			req_src_id;
		Message< GpuMsg::GetVkBufferID >		req_dst_id;
		Message< GpuMsg::GetImageDescriptor >	req_src_descr;
		Message< GpuMsg::GetBufferDescriptor >	req_dst_descr;
		
		// TODO: check result
		SendTo( msg->dstBuffer, req_dst_id );
		SendTo( msg->dstBuffer, req_dst_descr );
		SendTo( msg->srcImage, req_src_id );
		SendTo( msg->srcImage, req_src_descr );
		
		CHECK_ERR( req_src_descr->result.Get().usage.Get( EImageUsage::TransferSrc ) );
		CHECK_ERR( req_dst_descr->result.Get().usage.Get( EBufferUsage::TransferDst ) );

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
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->srcImage and msg->dstImage );
		
		Message< GpuMsg::GetVkImageID >			req_src_id;
		Message< GpuMsg::GetVkImageID >			req_dst_id;
		Message< GpuMsg::GetImageDescriptor >	req_src_descr;
		Message< GpuMsg::GetImageDescriptor >	req_dst_descr;
		
		// TODO: check result
		SendTo( msg->srcImage, req_src_id );
		SendTo( msg->dstImage, req_dst_id );
		SendTo( msg->srcImage, req_src_descr );
		SendTo( msg->dstImage, req_dst_descr );
		
		CHECK_ERR( req_src_descr->result.Get().usage.Get( EImageUsage::TransferSrc ) );
		CHECK_ERR( req_dst_descr->result.Get().usage.Get( EImageUsage::TransferDst ) );

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
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->dstBuffer and not msg->data.Empty() and msg->data.Size() < 65536_b );
		
		Message< GpuMsg::GetVkBufferID >		req_id;
		Message< GpuMsg::GetBufferDescriptor >	req_descr;
		
		// TODO: check result
		SendTo( msg->dstBuffer, req_id );
		SendTo( msg->dstBuffer, req_descr );
		
		CHECK_ERR( req_descr->result.Get().usage.Get( EBufferUsage::TransferDst ) );
		
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
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->dstBuffer );
		
		Message< GpuMsg::GetVkBufferID >		req_id;
		Message< GpuMsg::GetBufferDescriptor >	req_descr;
		
		// TODO: check result
		SendTo( msg->dstBuffer, req_id );
		SendTo( msg->dstBuffer, req_descr );
		
		CHECK_ERR( req_descr->result.Get().usage.Get( EBufferUsage::TransferDst ) );
		
		_resources.Add( msg->dstBuffer );

		vkCmdFillBuffer( _cmdId,
						 req_id->result.Get(),
						 (VkDeviceSize) msg->dstOffset,
						 (VkDeviceSize) Min( msg->size, req_descr->result.Get().size ),
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
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->image );
		
		Message< GpuMsg::GetVkImageID >			req_id;
		Message< GpuMsg::GetImageDescriptor >	req_descr;
		
		// TODO: check result
		SendTo( msg->image, req_id );
		SendTo( msg->image, req_descr );
		
		CHECK_ERR( req_descr->result.Get().usage.Get( EImageUsage::TransferDst ) );

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
		using namespace vk;

		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->image );
		
		Message< GpuMsg::GetVkImageID >			req_id;
		Message< GpuMsg::GetImageDescriptor >	req_descr;
		
		// TODO: check result
		SendTo( msg->image, req_id );
		SendTo( msg->image, req_descr );
		
		CHECK_ERR( req_descr->result.Get().usage.Get( EImageUsage::TransferDst ) );

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
	_CmdPipelineBarrier
=================================================
*/
	bool Vk1CommandBuilder::_CmdPipelineBarrier (const Message< GpuMsg::CmdPipelineBarrier > &msg)
	{
		using namespace vk;
		
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
			dst.buffer				<< req_id->result;
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
			SendTo( src.image, Message< GpuMsg::SetImageLayout >{ src.newLayout } );

			dst.sType							= VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			dst.srcAccessMask					= Vk1Enum( src.srcAccessMask );
			dst.dstAccessMask					= Vk1Enum( src.dstAccessMask );
			dst.oldLayout						= Vk1Enum( src.oldLayout );
			dst.newLayout						= Vk1Enum( src.newLayout );
			dst.srcQueueFamilyIndex				= queue_index;
			dst.dstQueueFamilyIndex				= queue_index;
			dst.image							<< req_id->result;
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
		pool_info.flags				= VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		VK_CHECK( vkCreateCommandPool( GetLogicalDevice(), &pool_info, null, OUT &_cmdPool ) );

		GetDevice()->SetObjectName( _cmdPool, GetDebugName(), EGpuObject::CommandPool );
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
			CHECK_ERR( fb.colorAttachments[i].target == 
							ERenderTarget::FromPixelFormat( rp.ColorAttachments()[i].format, uint(i) ) );

			CHECK_ERR( EImage::IsMultisampled( fb.colorAttachments[i].imageType ) ==
							(rp.ColorAttachments()[i].samples > MultiSamples(1)) );
		}

		if ( fb.depthStencilAttachment.IsEnabled() )
		{
			CHECK_ERR( fb.depthStencilAttachment.target == 
							ERenderTarget::FromPixelFormat( rp.DepthStencilAttachment().format ) );

			CHECK_ERR( EImage::IsMultisampled( fb.depthStencilAttachment.imageType ) ==
							(rp.DepthStencilAttachment().samples > MultiSamples(1)) );
		}

		return true;
	}

}	// PlatformVK
//-----------------------------------------------------------------------------
	
namespace Platforms
{
	ModulePtr VulkanContext::_CreateVk1CommandBuilder (const GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci)
	{
		return New< PlatformVK::Vk1CommandBuilder >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN