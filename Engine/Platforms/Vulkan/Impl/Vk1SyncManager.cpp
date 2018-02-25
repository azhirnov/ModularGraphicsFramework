// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Sync.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/VulkanObjectsConstructor.h"

#ifdef GRAPHICS_API_VULKAN

namespace Engine
{
namespace PlatformVK
{
	using namespace vk;

	
	//
	// Vulkan Sync Manager
	//

	class Vk1SyncManager final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::ClientWaitDeviceIdle,
											GpuMsg::CreateFence,
											GpuMsg::DestroyFence,
											GpuMsg::ClientWaitFence,
											GpuMsg::CreateEvent,
											GpuMsg::DestroyEvent,
											GpuMsg::SetEvent,
											GpuMsg::ResetEvent,
											GpuMsg::CreateSemaphore,
											GpuMsg::DestroySemaphore,
											GpuMsg::GetVkFence,
											GpuMsg::GetVkEvent,
											GpuMsg::GetVkSemaphore
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;


		template <typename T, usize UID>
		struct _VkWrap
		{
			T	ptr;

			_VkWrap () : ptr{VK_NULL_HANDLE} {}
			explicit _VkWrap (T value) : ptr{value} {}
		};

		using Fence_t		= _VkWrap< VkFence, 1 >;
		using Event_t		= _VkWrap< VkEvent, 2 >;
		using Semaphore_t	= _VkWrap< VkSemaphore, 3 >;


		using SyncUnion_t			= Union< Fence_t, Event_t, Semaphore_t >;
		using SyncArray_t			= Map< ulong, SyncUnion_t >;

		struct _DeleteSync_Func;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		SyncArray_t		_syncs;
		ulong			_counter;


	// methods
	public:
		Vk1SyncManager (GlobalSystemsRef gs, const CreateInfo::GpuSyncManager &ci);
		~Vk1SyncManager ();


	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _OnManagerChanged2 (const Message< ModuleMsg::OnManagerChanged > &);

		bool _ClientWaitDeviceIdle (const Message< GpuMsg::ClientWaitDeviceIdle > &);
		bool _CreateFence (const Message< GpuMsg::CreateFence > &);
		bool _DestroyFence (const Message< GpuMsg::DestroyFence > &);
		bool _ClientWaitFence (const Message< GpuMsg::ClientWaitFence > &);
		bool _CreateEvent (const Message< GpuMsg::CreateEvent > &);
		bool _DestroyEvent (const Message< GpuMsg::DestroyEvent > &);
		bool _SetEvent (const Message< GpuMsg::SetEvent > &);
		bool _ResetEvent (const Message< GpuMsg::ResetEvent > &);
		bool _CreateSemaphore (const Message< GpuMsg::CreateSemaphore > &);
		bool _DestroySemaphore (const Message< GpuMsg::DestroySemaphore > &);
		bool _GetVkFence (const Message< GpuMsg::GetVkFence > &);
		bool _GetVkEvent (const Message< GpuMsg::GetVkEvent > &);
		bool _GetVkSemaphore (const Message< GpuMsg::GetVkSemaphore > &);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	Vk1SyncManager::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	Vk1SyncManager::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1SyncManager::Vk1SyncManager (GlobalSystemsRef gs, const CreateInfo::GpuSyncManager &ci) :
		Vk1BaseModule( gs, ModuleConfig{ VkSyncManagerModuleID, UMax }, &_msgTypes, &_eventTypes ),
		_counter{ 0 }
	{
		SetDebugName( "Vk1SyncManager" );

		_SubscribeOnMsg( this, &Vk1SyncManager::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1SyncManager::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1SyncManager::_AttachModule_Empty );
		_SubscribeOnMsg( this, &Vk1SyncManager::_DetachModule_Empty );
		_SubscribeOnMsg( this, &Vk1SyncManager::_FindModule_Impl );
		_SubscribeOnMsg( this, &Vk1SyncManager::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Vk1SyncManager::_Link_Impl );
		_SubscribeOnMsg( this, &Vk1SyncManager::_Compose_Impl );
		_SubscribeOnMsg( this, &Vk1SyncManager::_Delete );
		_SubscribeOnMsg( this, &Vk1SyncManager::_OnManagerChanged2 );
		_SubscribeOnMsg( this, &Vk1SyncManager::_GetDeviceInfo );
		_SubscribeOnMsg( this, &Vk1SyncManager::_GetVkDeviceInfo );
		_SubscribeOnMsg( this, &Vk1SyncManager::_GetVkPrivateClasses );
		_SubscribeOnMsg( this, &Vk1SyncManager::_ClientWaitDeviceIdle );
		_SubscribeOnMsg( this, &Vk1SyncManager::_CreateFence );
		_SubscribeOnMsg( this, &Vk1SyncManager::_DestroyFence );
		_SubscribeOnMsg( this, &Vk1SyncManager::_ClientWaitFence );
		_SubscribeOnMsg( this, &Vk1SyncManager::_CreateEvent );
		_SubscribeOnMsg( this, &Vk1SyncManager::_DestroyEvent );
		_SubscribeOnMsg( this, &Vk1SyncManager::_SetEvent );
		_SubscribeOnMsg( this, &Vk1SyncManager::_ResetEvent );
		_SubscribeOnMsg( this, &Vk1SyncManager::_CreateSemaphore );
		_SubscribeOnMsg( this, &Vk1SyncManager::_DestroySemaphore );
		_SubscribeOnMsg( this, &Vk1SyncManager::_GetVkFence );
		_SubscribeOnMsg( this, &Vk1SyncManager::_GetVkEvent );
		_SubscribeOnMsg( this, &Vk1SyncManager::_GetVkSemaphore );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Vk1SyncManager::~Vk1SyncManager ()
	{
		ASSERT( _syncs.Empty() );
	}
	
/*
=================================================
	_DeleteSync_Func
=================================================
*/
	struct Vk1SyncManager::_DeleteSync_Func
	{
	// variables
		VkDevice		_dev;

	// methods
		explicit _DeleteSync_Func (VkDevice dev) : _dev(dev)
		{}

		void operator () (Fence_t fence) const
		{
			vkDestroyFence( _dev, fence.ptr, null );
		}
		
		void operator () (Event_t event) const
		{
			vkDestroyEvent( _dev, event.ptr, null );
		}
		
		void operator () (Semaphore_t sem) const
		{
			vkDestroySemaphore( _dev, sem.ptr, null );
		}
	};

/*
=================================================
	_Delete
=================================================
*/
	bool Vk1SyncManager::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		VkDevice	dev = GetVkDevice();
		
		if ( dev != VK_NULL_HANDLE )
		{
			_DeleteSync_Func	func{ dev };

			FOR( i, _syncs ) {
				_syncs[i].second.Apply( func );
			}
		}

		_syncs.Clear();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_OnManagerChanged2
=================================================
*/
	bool Vk1SyncManager::_OnManagerChanged2 (const Message< ModuleMsg::OnManagerChanged > &msg)
	{
		_OnManagerChanged( msg );
		
		if ( msg->newManager )
		{
			msg->newManager->UnsubscribeAll( this );
		}
		return true;
	}

/*
=================================================
	_ClientWaitDeviceIdle
=================================================
*/
	bool Vk1SyncManager::_ClientWaitDeviceIdle (const Message< GpuMsg::ClientWaitDeviceIdle > &)
	{
		GetDevice()->DeviceWaitIdle();
		return true;
	}

/*
=================================================
	_CreateFence
=================================================
*/
	bool Vk1SyncManager::_CreateFence (const Message< GpuMsg::CreateFence > &msg)
	{
		for (;;)
		{
			++_counter;

			if ( not _syncs.IsExist( _counter ) )
				break;
		}
		_syncs.Add( _counter, SyncUnion_t{ Fence_t() } );

		msg->result.Set( GpuFenceId(_counter) );
		return true;
	}
	
/*
=================================================
	_DestroyFence
=================================================
*/
	bool Vk1SyncManager::_DestroyFence (const Message< GpuMsg::DestroyFence > &msg)
	{
		SyncArray_t::iterator	iter;

		VkDevice	dev = GetVkDevice();
		
		if ( _syncs.Find( ulong(msg->id), OUT iter ) )
		{
			CHECK_ERR( iter->second.Is< Fence_t >() );
			
			if ( dev != VK_NULL_HANDLE ) {
				vkDestroyFence( dev, iter->second.Get< Fence_t >().ptr, null );
			}		
			_syncs.EraseByIter( iter );
		}
		return true;
	}
	
/*
=================================================
	_ClientWaitFence
=================================================
*/
	bool Vk1SyncManager::_ClientWaitFence (const Message< GpuMsg::ClientWaitFence > &msg)
	{
		using Fences_t = FixedSizeArray< vk::VkFence, GpuMsg::ClientWaitFence::Fences_t::MemoryContainer_t::SIZE >;

		Fences_t	fence_ids;

		FOR( i, msg->fences )
		{
			SyncArray_t::iterator	iter;
			CHECK_ERR( _syncs.Find( ulong(msg->fences[i]), OUT iter ) );

			VkFence		fence = iter->second.Get< Fence_t >().ptr;

			if ( fence ) {
				fence_ids.PushBack( fence );
			}
		}

		if ( not fence_ids.Empty() ) {
			VK_CALL( vkWaitForFences( GetVkDevice(), fence_ids.Count(), fence_ids.ptr(), true, msg->timeout.NanoSeconds() ) );
		}
		return true;
	}

/*
=================================================
	_GetVkFence
=================================================
*/
	bool Vk1SyncManager::_GetVkFence (const Message< GpuMsg::GetVkFence > &msg)
	{
		SyncArray_t::iterator	iter;
		CHECK_ERR( _syncs.Find( ulong(msg->fenceId), OUT iter ) );
		CHECK_ERR( iter->second.Is< Fence_t >() );
		
		VkFence &	fence = iter->second.Get< Fence_t >().ptr;

		if ( not fence )
		{
			VkFenceCreateInfo	fence_info	= {};

			fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fence_info.flags = /*msg->signaled ? VK_FENCE_CREATE_SIGNALED_BIT :*/ 0;

			VK_CHECK( vkCreateFence( GetVkDevice(), &fence_info, null, OUT &fence ) );
		}

		msg->result.Set( fence );
		return true;
	}

/*
=================================================
	_CreateEvent
=================================================
*/
	bool Vk1SyncManager::_CreateEvent (const Message< GpuMsg::CreateEvent > &msg)
	{
		for (;;)
		{
			++_counter;

			if ( not _syncs.IsExist( _counter ) )
				break;
		}
		_syncs.Add( _counter, SyncUnion_t{ Event_t() } );

		msg->result.Set( GpuEventId(_counter) );
		return true;
	}
	
/*
=================================================
	_DestroyEvent
=================================================
*/
	bool Vk1SyncManager::_DestroyEvent (const Message< GpuMsg::DestroyEvent > &msg)
	{
		SyncArray_t::iterator	iter;

		VkDevice	dev = GetVkDevice();
		
		if ( _syncs.Find( ulong(msg->id), OUT iter ) )
		{
			CHECK_ERR( iter->second.Is< Event_t >() );
			
			if ( dev != VK_NULL_HANDLE ) {
				vkDestroyEvent( dev, iter->second.Get< Event_t >().ptr, null );
			}		
			_syncs.EraseByIter( iter );
		}
		return true;
	}
	
/*
=================================================
	_GetVkEvent
=================================================
*/
	bool Vk1SyncManager::_GetVkEvent (const Message< GpuMsg::GetVkEvent > &msg)
	{
		SyncArray_t::iterator	iter;
		CHECK_ERR( _syncs.Find( ulong(msg->eventId), OUT iter ) );
		CHECK_ERR( iter->second.Is< Event_t >() );
		
		VkEvent &	event = iter->second.Get< Event_t >().ptr;

		if ( not event )
		{
			VkEventCreateInfo	event_info	= {};

			event_info.sType = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO;
			event_info.flags = 0;

			VK_CHECK( vkCreateEvent( GetVkDevice(), &event_info, null, OUT &event ) );
		}

		msg->result.Set( event );
		return true;
	}
	
/*
=================================================
	_SetEvent
=================================================
*/
	bool Vk1SyncManager::_SetEvent (const Message< GpuMsg::SetEvent > &msg)
	{
		Message< GpuMsg::GetVkEvent >	req_event{ msg->id };
		_GetVkEvent( req_event );

		VK_CALL( vkSetEvent( GetVkDevice(), *req_event->result ) );
		return true;
	}
	
/*
=================================================
	_ResetEvent
=================================================
*/
	bool Vk1SyncManager::_ResetEvent (const Message< GpuMsg::ResetEvent > &msg)
	{
		Message< GpuMsg::GetVkEvent >	req_event{ msg->id };
		_GetVkEvent( req_event );

		VK_CALL( vkResetEvent( GetVkDevice(), *req_event->result ) );
		return true;
	}

/*
=================================================
	_CreateSemaphore
=================================================
*/
	bool Vk1SyncManager::_CreateSemaphore (const Message< GpuMsg::CreateSemaphore > &msg)
	{
		for (;;)
		{
			++_counter;

			if ( not _syncs.IsExist( _counter ) )
				break;
		}

		VkSemaphore				semaphore	= VK_NULL_HANDLE;
		VkSemaphoreCreateInfo	sem_info	= {};

		sem_info.sType	= VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		sem_info.flags	= 0;

		VK_CALL( vkCreateSemaphore( GetVkDevice(), &sem_info, null, OUT &semaphore ) );

		_syncs.Add( _counter, SyncUnion_t{ Semaphore_t(semaphore) } );

		msg->result.Set( GpuSemaphoreId(_counter) );
		return true;
	}
	
/*
=================================================
	_DestroySemaphore
=================================================
*/
	bool Vk1SyncManager::_DestroySemaphore (const Message< GpuMsg::DestroySemaphore > &msg)
	{
		SyncArray_t::iterator	iter;

		VkDevice	dev = GetVkDevice();
		
		if ( _syncs.Find( ulong(msg->id), OUT iter ) )
		{
			CHECK_ERR( iter->second.Is< Semaphore_t >() );
			
			if ( dev != VK_NULL_HANDLE ) {
				vkDestroySemaphore( dev, iter->second.Get< Semaphore_t >().ptr, null );
			}		
			_syncs.EraseByIter( iter );
		}
		return true;
	}
	
/*
=================================================
	_GetVkSemaphore
=================================================
*/
	bool Vk1SyncManager::_GetVkSemaphore (const Message< GpuMsg::GetVkSemaphore > &msg)
	{
		SyncArray_t::iterator	iter;
		CHECK_ERR( _syncs.Find( ulong(msg->semId), OUT iter ) );
		CHECK_ERR( iter->second.Is< Semaphore_t >() );
		
		msg->result.Set( iter->second.Get< Semaphore_t >().ptr );
		return true;
	}

}	// PlatformVK
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr VulkanObjectsConstructor::CreateVk1SyncManager (GlobalSystemsRef gs, const CreateInfo::GpuSyncManager &ci)
	{
		return New< PlatformVK::Vk1SyncManager >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
