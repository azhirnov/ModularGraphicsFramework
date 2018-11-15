// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Public/GPU/CommandQueue.h"
#include "Engine/Platforms/Public/GPU/CommandBuffer.h"
#include "Engine/Platforms/Vulkan/110/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/110/Vk1ResourceCache.h"
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
											GpuMsg::SubmitCommands,
											GpuMsg::QueueBindSparse,
											GpuMsg::GetCommandQueueDescription,
											GpuMsg::GetVkQueueID
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::DeviceLost
										> >;
		
		using SyncMngrMsgList_t		= MessageListFrom<
											GpuMsg::GetVkSemaphore,
											GpuMsg::GetVkFence
										>;

		using VkCmdBuffers_t		= FixedSizeArray< VkCommandBuffer, GpuMsg::SubmitCommands::Commands_t::MemoryContainer_t::SIZE >;
		using VkSemaphores_t		= FixedSizeArray< VkSemaphore, GpuMsg::SubmitCommands::Semaphores_t::MemoryContainer_t::SIZE + 2 >;
		using PipelineStages_t		= FixedSizeArray< VkPipelineStageFlags, VkSemaphores_t::MemoryContainer_t::SIZE >;


	// constants
	private:
		static const TypeIdList		_eventTypes;


	// variables
	private:
		VkQueue						_queue;
		uint						_queueFamilyIndex;
		uint						_queueIndex;

		ModulePtr					_syncManager;
		CommandQueueDescription		_descr;


	// methods
	public:
		Vk1CommandQueue (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuCommandQueue &ci);
		~Vk1CommandQueue ();


	// message handlers
	private:
		bool _Link (const ModuleMsg::Link &);
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);
		
		bool _SubmitCommands (const GpuMsg::SubmitCommands &);
		bool _QueueBindSparse (const GpuMsg::QueueBindSparse &);
		bool _GetCommandQueueDescription (const GpuMsg::GetCommandQueueDescription &);
		bool _GetVkQueueID (const GpuMsg::GetVkQueueID &);

	private:
		bool _GetCmdBufferIds (ArrayCRef<ModulePtr> in, OUT VkCmdBuffers_t &out) const;
		bool _GetSemaphoreIds (ArrayCRef<GpuSemaphoreId> in, OUT VkSemaphores_t &out) const;
		bool _GetSemaphoreIds (ArrayCRef<Pair<GpuSemaphoreId, EPipelineStage::bits>> in, OUT VkSemaphores_t &out, OUT PipelineStages_t &stages) const;
	};
//-----------------------------------------------------------------------------

	
	const TypeIdList	Vk1CommandQueue::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1CommandQueue::Vk1CommandQueue (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandQueue &ci) :
		Vk1BaseModule( gs, ModuleConfig{ id, 1 }, &_eventTypes ),
		_descr{ ci.descr },			_queue{ VK_NULL_HANDLE },
		_queueFamilyIndex{ UMax },	_queueIndex{ ci.queueIndex }
	{
		SetDebugName( "Vk1CommandQueue" );

		_SubscribeOnMsg( this, &Vk1CommandQueue::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_AttachModule_Empty );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_DetachModule_Empty );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_FindModule_Impl );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_Link );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_Compose );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_Delete );\
		_SubscribeOnMsg( this, &Vk1CommandQueue::_GetDeviceInfo );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_GetVkDeviceInfo );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_GetVkPrivateClasses );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_SubmitCommands );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_QueueBindSparse );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_GetCommandQueueDescription );
		_SubscribeOnMsg( this, &Vk1CommandQueue::_GetVkQueueID );

		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

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
	bool Vk1CommandQueue::_Delete (const ModuleMsg::Delete &msg)
	{
		GetDevice()->DeviceWaitIdle();

		_descr = Uninitialized;

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool Vk1CommandQueue::_Link (const ModuleMsg::Link &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked
		
		CHECK_ERR( _IsInitialState( GetState() ) );
		CHECK_ERR( _GetManager() );

		CHECK_LINKING( _syncManager = _GetManager()->GetModuleByMsg< SyncMngrMsgList_t >() );
		
		return Module::_Link_Impl( msg );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool Vk1CommandQueue::_Compose (const ModuleMsg::Compose &)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		_descr.family		= GetDevice()->GetQueueFamily();	// TODO: remove queue from device
		_queueFamilyIndex	= GetDevice()->GetQueueFamilyIndex();
		
		vkGetDeviceQueue( GetVkDevice(), _queueFamilyIndex, _queueIndex, OUT &_queue );

		return _DefCompose( false );
	}

/*
=================================================
	_SubmitCommands
=================================================
*/
	bool Vk1CommandQueue::_SubmitCommands (const GpuMsg::SubmitCommands &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( GetDevice()->IsDeviceCreated() );
		CHECK_ERR( _descr.family & (EQueueFamily::Graphics | EQueueFamily::Compute) );

		VkCmdBuffers_t	cmd_ids;
		CHECK_ERR( _GetCmdBufferIds( msg.commands, OUT cmd_ids ) );

		VkSemaphores_t		wait_semaphores;
		PipelineStages_t	wait_stages;
		_GetSemaphoreIds( msg.waitSemaphores, OUT wait_semaphores, OUT wait_stages );

		VkSemaphores_t	signal_semaphores;
		_GetSemaphoreIds( msg.signalSemaphores, OUT signal_semaphores );

		
		// fence
		VkFence		fence = VK_NULL_HANDLE;

		if ( msg.fence != GpuFenceId::Unknown )
		{
			GpuMsg::GetVkFence	req_fence{ msg.fence };
			CHECK( _syncManager->Send( req_fence ) );

			fence = *req_fence.result;
			VK_CALL( vkResetFences( GetDevice()->GetLogicalDevice(), 1, &fence ) );
		}
		
		VkSubmitInfo					submit_info = {};
		submit_info.sType					= VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.commandBufferCount		= uint(cmd_ids.Count());
		submit_info.pCommandBuffers			= cmd_ids.RawPtr();
		submit_info.waitSemaphoreCount		= uint32_t(wait_semaphores.Count());
		submit_info.pWaitSemaphores			= wait_semaphores.RawPtr();
		submit_info.pWaitDstStageMask		= wait_stages.RawPtr();
		submit_info.signalSemaphoreCount	= uint32_t(signal_semaphores.Count());
		submit_info.pSignalSemaphores		= signal_semaphores.RawPtr();

		VkResult	vk_err = vkQueueSubmit( _queue, 1, &submit_info, fence );
		VK_CALL( vk_err );

		if ( vk_err == VK_ERROR_DEVICE_LOST )
		{
			_SendEvent( GpuMsg::DeviceLost{} );
		}
		
		ModuleUtils::Send( msg.commands, GpuMsg::SetCommandBufferState{ GpuMsg::SetCommandBufferState::EState::Pending });
		return true;
	}
	
/*
=================================================
	_QueueBindSparse
=================================================
*/
	bool Vk1CommandQueue::_QueueBindSparse (const GpuMsg::QueueBindSparse &msg)
	{
		static constexpr usize	BufferCount			= GpuMsg::QueueBindSparse::BufferBinds_t::MemoryContainer_t::SIZE;
		static constexpr usize	ImageCount			= GpuMsg::QueueBindSparse::ImageBinds_t::MemoryContainer_t::SIZE;
		static constexpr usize	OpaqueImageCount	= GpuMsg::QueueBindSparse::ImageOpaqueBinds_t::MemoryContainer_t::SIZE;
		static constexpr usize	SparseMemCount		= GpuMsg::QueueBindSparse::SparseMemoryBinds_t::MemoryContainer_t::SIZE;
		static constexpr usize	ImageSparseCount	= GpuMsg::QueueBindSparse::SparseImageBinds_t::MemoryContainer_t::SIZE;

		using VkSparseBufferMemoryBindInfos_t		= FixedSizeArray< VkSparseBufferMemoryBindInfo, BufferCount >;
		using VkSparseImageOpaqueMemoryBindInfos_t	= FixedSizeArray< VkSparseImageOpaqueMemoryBindInfo, OpaqueImageCount >;
		using VkSparseImageMemoryBindInfos_t		= FixedSizeArray< VkSparseImageMemoryBindInfo, ImageCount >;

		using VkBufferSparseMemoryBinds_t			= FixedSizeArray< VkSparseMemoryBind, BufferCount * SparseMemCount >;
		using VkImageSparseMemoryBinds_t			= FixedSizeArray< VkSparseMemoryBind, ImageCount * SparseMemCount >;
		using VkSparseImageMemoryBinds_t			= FixedSizeArray< VkSparseImageMemoryBind, OpaqueImageCount * ImageSparseCount >;


		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( _descr.family[EQueueFamily::SparseBinding] );
		
		VkSemaphores_t	signal_semaphores;
		_GetSemaphoreIds( msg.signalSemaphores, OUT signal_semaphores );
		
		VkSemaphores_t	wait_semaphores;
		_GetSemaphoreIds( msg.waitSemaphores, OUT wait_semaphores );

		// fence
		VkFence		fence = VK_NULL_HANDLE;

		if ( msg.fence != GpuFenceId::Unknown )
		{
			GpuMsg::GetVkFence	req_fence{ msg.fence };
			CHECK( _syncManager->Send( req_fence ) );

			fence = *req_fence.result;
			VK_CALL( vkResetFences( GetDevice()->GetLogicalDevice(), 1, &fence ) );
		}

		VkSparseBufferMemoryBindInfos_t			buffer_binds;
		VkBufferSparseMemoryBinds_t				buffer_sparse_binds;

		for (auto& bind : msg.bufferBinds)
		{
			VkSparseBufferMemoryBindInfo	info = {};
			info.buffer		= bind.first->Request( GpuMsg::GetVkBufferID{} );
			info.pBinds		= buffer_sparse_binds.end();
			info.bindCount	= uint32_t(bind.second.Count());
			buffer_binds << info;

			for (auto& part : bind.second)
			{
				VkSparseMemoryBind	mem = {};
				mem.resourceOffset	= VkDeviceSize(part.resourceOffset);
				mem.size			= VkDeviceSize(part.size);
				mem.memory			= part.memory->Request( GpuMsg::GetVkMemoryID{} );
				mem.memoryOffset	= VkDeviceSize(part.memoryOffset);
				mem.flags			= 0;	// TODO
				buffer_sparse_binds << mem;
			}
		}

		VkSparseImageMemoryBindInfos_t			image_binds;
		VkSparseImageMemoryBinds_t				sparse_image_binds;

		for (auto& bind : msg.imageBinds)
		{
			VkSparseImageMemoryBindInfo	info = {};
			info.image		= bind.first->Request( GpuMsg::GetVkImageID{} ).id;
			info.pBinds		= sparse_image_binds.end();
			info.bindCount	= uint32_t(bind.second.Count());
			image_binds << info;

			for (auto& part : bind.second)
			{
				VkSparseImageMemoryBind	mem = {};
				mem.subresource.aspectMask	= Vk1Enum( part.subresource.aspectMask );
				mem.subresource.mipLevel	= part.subresource.mipLevel.Get();
				mem.subresource.arrayLayer	= part.subresource.arrayLayer.Get();
				mem.offset					= { int(part.offset.x), int(part.offset.y), int(part.offset.z) };
				mem.extent					= { part.extent.x, part.extent.y, part.extent.z };
				mem.memory					= part.memory->Request( GpuMsg::GetVkMemoryID{} );
				mem.memoryOffset			= VkDeviceSize(part.memoryOffset);
				mem.flags					= 0;	// TODO
				sparse_image_binds << mem;
			}
		}

		VkSparseImageOpaqueMemoryBindInfos_t	image_opaque_binds;
		VkImageSparseMemoryBinds_t				image_sparse_binds;

		for (auto& bind : msg.imageOpaqueBings)
		{
			VkSparseImageOpaqueMemoryBindInfo	info = {};
			info.image		= bind.first->Request( GpuMsg::GetVkImageID{} ).id;
			info.pBinds		= image_sparse_binds.end();
			info.bindCount	= uint32_t(image_sparse_binds.Count());
			image_opaque_binds << info;
			
			for (auto& part : bind.second)
			{
				VkSparseMemoryBind	mem = {};
				mem.resourceOffset	= VkDeviceSize(part.resourceOffset);
				mem.size			= VkDeviceSize(part.size);
				mem.memory			= part.memory->Request( GpuMsg::GetVkMemoryID{} );
				mem.memoryOffset	= VkDeviceSize(part.memoryOffset);
				mem.flags			= 0;	// TODO
				image_sparse_binds << mem;
			}
		}


		VkBindSparseInfo	bind_info = {};
		bind_info.sType					= VK_STRUCTURE_TYPE_BIND_SPARSE_INFO;
		bind_info.waitSemaphoreCount	= uint32_t(wait_semaphores.Count());
		bind_info.pWaitSemaphores		= wait_semaphores.RawPtr();
		bind_info.signalSemaphoreCount	= uint32_t(signal_semaphores.Count());
		bind_info.pSignalSemaphores		= signal_semaphores.RawPtr();

		bind_info.pBufferBinds			= buffer_binds.RawPtr();
		bind_info.bufferBindCount		= uint32_t(buffer_binds.Count());
		bind_info.pImageBinds			= image_binds.RawPtr();
		bind_info.imageBindCount		= uint32_t(image_binds.Count());
		bind_info.pImageOpaqueBinds		= image_opaque_binds.RawPtr();
		bind_info.imageOpaqueBindCount	= uint32_t(image_opaque_binds.Count());

		vkQueueBindSparse( _queue, 1, &bind_info, fence );

		return true;
	}

/*
=================================================
	_GetCommandQueueDescription
=================================================
*/
	bool Vk1CommandQueue::_GetCommandQueueDescription (const GpuMsg::GetCommandQueueDescription &msg)
	{
		msg.result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_GetVkQueueID
=================================================
*/
	bool Vk1CommandQueue::_GetVkQueueID (const GpuMsg::GetVkQueueID &msg)
	{
		msg.result.Set( _queue );
		return true;
	}

/*
=================================================
	_GetCmdBufferIds
=================================================
*/
	bool Vk1CommandQueue::_GetCmdBufferIds (ArrayCRef<ModulePtr> inCommands, OUT VkCmdBuffers_t &out) const
	{
		for (auto& cmd : inCommands)
		{
			CHECK_ERR( cmd );
			
			const auto	cmd_res = GetResourceCache()->GetCommandBufferID( cmd );

			DEBUG_ONLY(
				CHECK_ERR( not cmd->Request( GpuMsg::GetCommandBufferDescription{} ).flags[ ECmdBufferCreate::Secondary ] );
			);

			out.PushBack( cmd_res.Get<0>() );
		}
		return true;
	}
	
/*
=================================================
	_GetSemaphoreIds
=================================================
*/
	bool Vk1CommandQueue::_GetSemaphoreIds (ArrayCRef<GpuSemaphoreId> inSemaphores, OUT VkSemaphores_t &out) const
	{
		for (auto& sem : inSemaphores)
		{
			out.PushBack( _syncManager->Request( GpuMsg::GetVkSemaphore{ sem } ) );
		}
		return true;
	}

	bool Vk1CommandQueue::_GetSemaphoreIds (ArrayCRef<Pair<GpuSemaphoreId, EPipelineStage::bits>> inSemaphores,
											OUT VkSemaphores_t &out, OUT PipelineStages_t &stages) const
	{
		for (auto& sem : inSemaphores)
		{
			out.PushBack( _syncManager->Request( GpuMsg::GetVkSemaphore{ sem.first } ) );
			
			stages.PushBack( Vk1Enum( sem.second ) );
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
