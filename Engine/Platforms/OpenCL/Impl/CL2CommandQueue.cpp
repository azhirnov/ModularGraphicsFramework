// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "Engine/Platforms/Public/GPU/Sync.h"
#include "Engine/Platforms/OpenCL/Impl/CL2BaseModule.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"

namespace Engine
{
namespace PlatformCL
{

	//
	// OpenCL Command Queue
	//

	class CL2CommandQueue final : public CL2BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= CL2BaseModule::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::Update,
											GpuMsg::SubmitComputeQueueCommands
										> >;

		using SupportedEvents_t		= CL2BaseModule::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::DeviceLost
										> >;

		using SyncMngrMsgList_t		= MessageListFrom<
											GpuMsg::CLSemaphoreEnqueue,
											GpuMsg::WaitCLSemaphore,
											GpuMsg::CLFenceSync
										>;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ModulePtr		_syncManager;


	// methods
	public:
		CL2CommandQueue (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuCommandQueue &ci);
		~CL2CommandQueue ();


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _Update (const Message< ModuleMsg::Update > &);
		
		bool _SubmitComputeQueueCommands (const Message< GpuMsg::SubmitComputeQueueCommands > &);

	private:
		bool _Submit (const GpuMsg::SubmitGraphicsQueueCommands &msg);
		bool _SubmitQueue (const GpuMsg::SubmitGraphicsQueueCommands &cmd) const;
	};
//-----------------------------------------------------------------------------

	
	const TypeIdList	CL2CommandQueue::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	CL2CommandQueue::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	CL2CommandQueue::CL2CommandQueue (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandQueue &ci) :
		CL2BaseModule( gs, ModuleConfig{ id, 1 }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( "CL2CommandQueue" );

		_SubscribeOnMsg( this, &CL2CommandQueue::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &CL2CommandQueue::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &CL2CommandQueue::_AttachModule_Empty );
		_SubscribeOnMsg( this, &CL2CommandQueue::_DetachModule_Empty );
		_SubscribeOnMsg( this, &CL2CommandQueue::_FindModule_Impl );
		_SubscribeOnMsg( this, &CL2CommandQueue::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &CL2CommandQueue::_Link );
		_SubscribeOnMsg( this, &CL2CommandQueue::_Compose_Impl );
		_SubscribeOnMsg( this, &CL2CommandQueue::_Delete );
		_SubscribeOnMsg( this, &CL2CommandQueue::_Update );
		_SubscribeOnMsg( this, &CL2CommandQueue::_GetDeviceInfo );
		_SubscribeOnMsg( this, &CL2CommandQueue::_GetCLDeviceInfo );
		_SubscribeOnMsg( this, &CL2CommandQueue::_GetCLPrivateClasses );
		_SubscribeOnMsg( this, &CL2CommandQueue::_OnManagerChanged );
		_SubscribeOnMsg( this, &CL2CommandQueue::_SubmitComputeQueueCommands );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	CL2CommandQueue::~CL2CommandQueue ()
	{
	}

/*
=================================================
	_Delete
=================================================
*/
	bool CL2CommandQueue::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool CL2CommandQueue::_Link (const Message< ModuleMsg::Link > &msg)
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
	_Update
=================================================
*/
	bool CL2CommandQueue::_Update (const Message< ModuleMsg::Update > &)
	{
		if ( not _IsComposedState( GetState() ) )
			return false;

		return true;
	}

/*
=================================================
	_SubmitComputeQueueCommands
=================================================
*/
	bool CL2CommandQueue::_SubmitComputeQueueCommands (const Message< GpuMsg::SubmitComputeQueueCommands > &msg)
	{
		using namespace cl;
		
		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( GetDevice()->HasCommandQueue() );
		
		DEBUG_ONLY(
			FOR( i, msg->commands )
			{
				Message< GpuMsg::GetCommandBufferDescriptor >	req_descr;
				msg->commands[i]->Send( req_descr );
				CHECK_ERR( req_descr->result and not req_descr->result->flags[ ECmdBufferCreate::Secondary ] );
			}
		);
		
		ModuleUtils::Send( msg->commands, Message<GpuMsg::SetCommandBufferState>{ GpuMsg::SetCommandBufferState::EState::Pending });

		// wait for signal
		{
			Message< GpuMsg::WaitCLSemaphore >	wait;

			for (auto& sem : Range(msg->waitSemaphores)) {
				ASSERT(sem.second == EPipelineStage::AllCommands );
				wait->semaphores.PushBack( sem.first );
			}

			if ( not wait->semaphores.Empty() ) {
				CHECK( _syncManager->Send( wait ) );
			}
		}

		// execute command buffers
		ModuleUtils::Send( msg->commands, Message<GpuMsg::ExecuteCLCommandBuffer>{} );
		
		// enqueue fence
		if ( msg->fence != GpuFenceId::Unknown )
		{
			Message< GpuMsg::CLFenceSync >	fence_sync{ msg->fence };
			CHECK( _syncManager->Send( fence_sync ) );
		}

		// enqueue semaphores
		FOR( i, msg->signalSemaphores )
		{
			Message< GpuMsg::CLSemaphoreEnqueue >	sem_sync{ msg->signalSemaphores[i] };
			CHECK( _syncManager->Send( sem_sync ) );
		}
		return true;
	}

}	// PlatformCL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenCLObjectsConstructor::CreateCL2CommandQueue (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandQueue &ci)
	{
		return New< PlatformCL::CL2CommandQueue >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
