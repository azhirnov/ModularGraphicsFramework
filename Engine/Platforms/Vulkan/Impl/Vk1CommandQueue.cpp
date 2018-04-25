// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Public/GPU/Thread.h"
#include "Engine/Platforms/Public/GPU/CommandBuffer.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/VulkanObjectsConstructor.h"

namespace Engine
{
namespace PlatformVK
{
	using namespace vk;


	//
	// Vulkan Command Queue
	//

	class Vk1CommandQueue final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::SubmitGraphicsQueueCommands,
											GpuMsg::SubmitComputeQueueCommands
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::DeviceLost
										> >;
		
		using SyncMngrMsgList_t		= MessageListFrom<
											GpuMsg::GetVkSemaphore,
											GpuMsg::GetVkFence
										>;

		using CmdBUffers_t			= GpuMsg::ThreadEndFrame::Commands_t;
		using VkCmdBuffers_t		= FixedSizeArray< VkCommandBuffer, CmdBUffers_t::MemoryContainer_t::SIZE >;
		using Semaphores_t			= FixedSizeArray< VkSemaphore, GpuMsg::SubmitGraphicsQueueCommands::Semaphores_t::MemoryContainer_t::SIZE + 2 >;
		using PipelineStages_t		= FixedSizeArray< VkPipelineStageFlags, Semaphores_t::MemoryContainer_t::SIZE >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ModulePtr			_syncManager;
		EQueueFamily::bits	_family;


	// methods
	public:
		Vk1CommandQueue (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuCommandQueue &ci);
		~Vk1CommandQueue ();


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		
		bool _SubmitGraphicsQueueCommands (const Message< GpuMsg::SubmitGraphicsQueueCommands > &);
		bool _SubmitComputeQueueCommands (const Message< GpuMsg::SubmitComputeQueueCommands > &);

	private:
		bool _SubmitQueue (const GpuMsg::SubmitGraphicsQueueCommands *msg);

		static bool _GetCmdBufferIds (const CmdBUffers_t &in, OUT VkCmdBuffers_t &out);
	};
//-----------------------------------------------------------------------------

	
	const TypeIdList	Vk1CommandQueue::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	Vk1CommandQueue::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1CommandQueue::Vk1CommandQueue (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandQueue &ci) :
		Vk1BaseModule( gs, ModuleConfig{ id, 1 }, &_msgTypes, &_eventTypes ),
		_family{ ci.family }
	{
		SetDebugName( "Vk1CommandQueue" );

		_SubscribeOnMsg( this, &Vk1CommandQueue::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_AttachModule_Empty );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_DetachModule_Empty );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_FindModule_Impl );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_Link );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_Compose_Impl );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_Delete );\
		_SubscribeOnMsg( this, &Vk1CommandQueue::_GetDeviceInfo );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_GetVkDeviceInfo );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_GetVkPrivateClasses );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_SubmitGraphicsQueueCommands );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_SubmitComputeQueueCommands );\

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Vk1CommandQueue::~Vk1CommandQueue ()
	{
	}

/*
=================================================
	_Delete
=================================================
*/
	bool Vk1CommandQueue::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		GetDevice()->DeviceWaitIdle();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool Vk1CommandQueue::_Link (const Message< ModuleMsg::Link > &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked
		
		CHECK_ERR( GetState() == EState::Initial or GetState() == EState::LinkingFailed );
		CHECK_ERR( _GetManager() );

		CHECK_LINKING( _syncManager = _GetManager()->GetModuleByMsg< SyncMngrMsgList_t >() );
		
		return Module::_Link_Impl( msg );
	}
	
/*
=================================================
	_SubmitQueue
=================================================
*/
	bool Vk1CommandQueue::_SubmitQueue (const GpuMsg::SubmitGraphicsQueueCommands *msg)
	{
		// commands
		VkCmdBuffers_t	cmd_ids;
		CHECK_ERR( _GetCmdBufferIds( msg->commands, OUT cmd_ids ) );

		// wait semaphores
		Semaphores_t		wait_semaphores;
		PipelineStages_t	wait_stages;

		FOR( i, msg->waitSemaphores )
		{
			Message< GpuMsg::GetVkSemaphore >	req_sem{ msg->waitSemaphores[i].first };
			_syncManager->Send( req_sem );

			wait_semaphores.PushBack( *req_sem->result );
			wait_stages.PushBack( PlatformVK::Vk1Enum( msg->waitSemaphores[i].second ) );
		}

		// signal semaphores
		Semaphores_t	signal_semaphores;

		FOR( i, msg->signalSemaphores )
		{
			Message< GpuMsg::GetVkSemaphore >	req_sem{ msg->signalSemaphores[i] };
			_syncManager->Send( req_sem );

			signal_semaphores.PushBack( *req_sem->result );
		}
		
		// fence
		VkFence		fence = VK_NULL_HANDLE;

		if ( msg->fence != GpuFenceId::Unknown )
		{
			Message< GpuMsg::GetVkFence >	req_fence{ msg->fence };
			CHECK( _syncManager->Send( req_fence ) );

			fence = *req_fence->result;
			VK_CALL( vkResetFences( GetDevice()->GetLogicalDevice(), 1, &fence ) );
		}
		
		VkSubmitInfo					submit_info = {};
		submit_info.sType					= VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.commandBufferCount		= (uint) cmd_ids.Count();
		submit_info.pCommandBuffers			= cmd_ids.RawPtr();
		submit_info.waitSemaphoreCount		= (uint32_t) wait_semaphores.Count();
		submit_info.pWaitSemaphores			= wait_semaphores.RawPtr();
		submit_info.pWaitDstStageMask		= wait_stages.RawPtr();
		submit_info.signalSemaphoreCount	= (uint32_t) signal_semaphores.Count();
		submit_info.pSignalSemaphores		= signal_semaphores.RawPtr();

		VkResult	vk_err = vkQueueSubmit( GetDevice()->GetQueue(), 1, &submit_info, fence );
		VK_CALL( vk_err );

		if ( vk_err == VK_ERROR_DEVICE_LOST )
		{
			_SendEvent< GpuMsg::DeviceLost >({});
		}
		
		ModuleUtils::Send( msg->commands, Message< GpuMsg::SetCommandBufferState >{ GpuMsg::SetCommandBufferState::EState::Pending });
		return true;
	}
	
/*
=================================================
	_SubmitGraphicsQueueCommands
=================================================
*/
	bool Vk1CommandQueue::_SubmitGraphicsQueueCommands (const Message< GpuMsg::SubmitGraphicsQueueCommands > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( GetDevice()->IsDeviceCreated() );
		CHECK_ERR( GetDevice()->GetQueueFamily()[ EQueueFamily::Graphics ] );

		return _SubmitQueue( msg.operator->() );
	}
	
/*
=================================================
	_SubmitComputeQueueCommands
=================================================
*/
	bool Vk1CommandQueue::_SubmitComputeQueueCommands (const Message< GpuMsg::SubmitComputeQueueCommands > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( GetDevice()->IsDeviceCreated() );
		CHECK_ERR( GetDevice()->GetQueueFamily()[ EQueueFamily::Compute ] );
		
		return _SubmitQueue( msg.operator->() );
	}
	
/*
=================================================
	_GetCmdBufferIds
=================================================
*/
	bool Vk1CommandQueue::_GetCmdBufferIds (const CmdBUffers_t &in, OUT VkCmdBuffers_t &out)
	{
		FOR( i, in )
		{
			CHECK_ERR( in[i] );

			Message< GpuMsg::GetVkCommandBufferID >	req_id;
			in[i]->Send( req_id );

			DEBUG_ONLY(
				Message< GpuMsg::GetCommandBufferDescriptor >	req_descr;
				in[i]->Send( req_descr );
				CHECK_ERR( req_descr->result and not req_descr->result->flags[ ECmdBufferCreate::Secondary ] );
			);

			out.PushBack( *req_id->result );
		}
		return true;
	}
	
}	// PlatformVK
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr VulkanObjectsConstructor::CreateVk1CommandQueue (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandQueue &ci)
	{
		return New< PlatformVK::Vk1CommandQueue >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
