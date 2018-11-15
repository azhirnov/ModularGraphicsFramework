// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "Engine/Platforms/Public/GPU/Sync.h"
#include "Engine/Platforms/OpenCL/120/CL1BaseModule.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"

namespace Engine
{
namespace PlatformCL
{

	//
	// OpenCL Command Queue
	//

	class CL1CommandQueue final : public CL1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= CL1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::Update,
											GpuMsg::SubmitCommands,
											GpuMsg::GetCommandQueueDescription
										> >;

		using SupportedEvents_t		= CL1BaseModule::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::DeviceLost
										> >;

		using SyncMngrMsgList_t		= MessageListFrom<
											GpuMsg::CLSemaphoreEnqueue,
											GpuMsg::WaitCLSemaphore,
											GpuMsg::CLFenceSync
										>;


	// constants
	private:
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ModulePtr					_syncManager;
		CommandQueueDescription		_descr;


	// methods
	public:
		CL1CommandQueue (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuCommandQueue &ci);
		~CL1CommandQueue ();


	// message handlers
	private:
		bool _Link (const ModuleMsg::Link &);
		bool _Delete (const ModuleMsg::Delete &);
		bool _Update (const ModuleMsg::Update &);
		
		bool _SubmitCommands (const GpuMsg::SubmitCommands &);
		bool _GetCommandQueueDescription (const GpuMsg::GetCommandQueueDescription &);
	};
//-----------------------------------------------------------------------------

	
	const TypeIdList	CL1CommandQueue::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	CL1CommandQueue::CL1CommandQueue (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandQueue &ci) :
		CL1BaseModule( gs, ModuleConfig{ id, 1 }, &_eventTypes ),
		_descr{ ci.descr }
	{
		SetDebugName( "CL1CommandQueue" );

		_SubscribeOnMsg( this, &CL1CommandQueue::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &CL1CommandQueue::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &CL1CommandQueue::_AttachModule_Empty );
		_SubscribeOnMsg( this, &CL1CommandQueue::_DetachModule_Empty );
		_SubscribeOnMsg( this, &CL1CommandQueue::_FindModule_Impl );
		_SubscribeOnMsg( this, &CL1CommandQueue::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &CL1CommandQueue::_Link );
		_SubscribeOnMsg( this, &CL1CommandQueue::_Compose_Impl );
		_SubscribeOnMsg( this, &CL1CommandQueue::_Delete );
		_SubscribeOnMsg( this, &CL1CommandQueue::_Update );
		_SubscribeOnMsg( this, &CL1CommandQueue::_GetDeviceInfo );
		_SubscribeOnMsg( this, &CL1CommandQueue::_GetCLDeviceInfo );
		_SubscribeOnMsg( this, &CL1CommandQueue::_GetCLPrivateClasses );
		_SubscribeOnMsg( this, &CL1CommandQueue::_OnManagerChanged );
		_SubscribeOnMsg( this, &CL1CommandQueue::_SubmitCommands );
		_SubscribeOnMsg( this, &CL1CommandQueue::_GetCommandQueueDescription );

		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	CL1CommandQueue::~CL1CommandQueue ()
	{
	}

/*
=================================================
	_Delete
=================================================
*/
	bool CL1CommandQueue::_Delete (const ModuleMsg::Delete &msg)
	{
		_descr = Uninitialized;

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool CL1CommandQueue::_Link (const ModuleMsg::Link &msg)
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
	_Update
=================================================
*/
	bool CL1CommandQueue::_Update (const ModuleMsg::Update &)
	{
		if ( not _IsComposedState( GetState() ) )
			return false;

		return true;
	}

/*
=================================================
	_SubmitCommands
=================================================
*/
	bool CL1CommandQueue::_SubmitCommands (const GpuMsg::SubmitCommands &msg)
	{
		using namespace cl;
		
		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( GetDevice()->HasCommandQueue() );
		
		DEBUG_ONLY(
		for (auto& cmd : msg.commands) {
			CHECK_ERR( not cmd->Request(GpuMsg::GetCommandBufferDescription{}).flags[ ECmdBufferCreate::Secondary ] );
		});
		
		ModuleUtils::Send( msg.commands, GpuMsg::SetCommandBufferState{ GpuMsg::SetCommandBufferState::EState::Pending });

		// wait for signal
		{
			GpuMsg::WaitCLSemaphore		wait;

			for (auto& sem : msg.waitSemaphores) {
				ASSERT(sem.second == EPipelineStage::AllCommands );
				wait.semaphores.PushBack( sem.first );
			}

			if ( not wait.semaphores.Empty() ) {
				CHECK( _syncManager->Send( wait ) );
			}
		}

		// execute command buffers
		ModuleUtils::Send( msg.commands, GpuMsg::ExecuteCLCommandBuffer{} );
		
		// enqueue fence
		if ( msg.fence != GpuFenceId::Unknown )
		{
			GpuMsg::CLFenceSync		fence_sync{ msg.fence };
			CHECK( _syncManager->Send( fence_sync ) );
		}

		// enqueue semaphores
		FOR( i, msg.signalSemaphores )
		{
			GpuMsg::CLSemaphoreEnqueue	sem_sync{ msg.signalSemaphores[i] };
			CHECK( _syncManager->Send( sem_sync ) );
		}
		return true;
	}
	
/*
=================================================
	_GetCommandQueueDescription
=================================================
*/
	bool CL1CommandQueue::_GetCommandQueueDescription (const GpuMsg::GetCommandQueueDescription &msg)
	{
		msg.result.Set( _descr );
		return true;
	}

}	// PlatformCL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenCLObjectsConstructor::CreateCL1CommandQueue (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandQueue &ci)
	{
		return New< PlatformCL::CL1CommandQueue >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
