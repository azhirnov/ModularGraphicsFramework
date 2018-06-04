// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_SOFT

#include "Engine/Platforms/Public/GPU/Sync.h"
#include "Engine/Platforms/Soft/Impl/SWBaseModule.h"
#include "Engine/Platforms/Soft/Impl/SWSyncObjects.h"
#include "Engine/Platforms/Soft/SoftRendererObjectsConstructor.h"

namespace Engine
{
namespace PlatformSW
{

	//
	// Software Renderer Command Queue
	//

	class SWCommandQueue final : public SWBaseModule
	{
	// types
	private:
		using SupportedMessages_t	= SWBaseModule::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::Update,
											GpuMsg::SubmitGraphicsQueueCommands,
											GpuMsg::SubmitComputeQueueCommands,
											GpuMsg::SWPresent,
											GpuMsg::ClientWaitFence,
											GpuMsg::ClientWaitDeviceIdle
										> >;

		using SupportedEvents_t		= SWBaseModule::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::DeviceLost
										> >;

		using SyncMngrMsgList_t		= MessageListFrom<
											GpuMsg::GetSWFence,
											GpuMsg::GetSWEvent,
											GpuMsg::GetSWSemaphore
										>;

		using CmdBufferMsgList_t	= MessageListFrom<
											GpuMsg::ExecuteSWCommandBuffer
										>;

		using ExecuteCmdBufferMsg	= GpuMsg::ExecuteSWCommandBuffer;

		struct Submitted : CompileTime::FastCopyable
		{
		// types
			static constexpr uint	CMD_SIZE	= GpuMsg::SubmitGraphicsQueueCommands::Commands_t::MemoryContainer_t::SIZE;
			static constexpr uint	SS_SIZE		= GpuMsg::SubmitGraphicsQueueCommands::Semaphores_t::MemoryContainer_t::SIZE;
			static constexpr uint	WS_SIZE		= GpuMsg::SubmitGraphicsQueueCommands::WaitSemaphores_t::MemoryContainer_t::SIZE;

			using Commands_t		= FixedSizeArray<Pair< ModulePtr, ExecuteCmdBufferMsg >, CMD_SIZE >;
			using SignalSemaphores_t= FixedSizeArray< SWSemaphorePtr, SS_SIZE >;
			using WaitSemaphores_t	= FixedSizeArray<Pair< SWSemaphorePtr, EPipelineStage::bits >, WS_SIZE >;

		// variables
			Commands_t			commands;
			SignalSemaphores_t	signalSemaphores;
			WaitSemaphores_t	waitSemaphores;
			SWFencePtr			signalFence;
		};

		using CmdQueue_t			= Queue< Submitted >;
		using FencesSet_t			= FixedSizeSet< SWFencePtr, GpuMsg::ClientWaitFence::Fences_t::MemoryContainer_t::SIZE >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ModulePtr			_syncManager;
		CmdQueue_t			_queue;
		EQueueFamily::bits	_family;
		bool				_lockQueue;


	// methods
	public:
		SWCommandQueue (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuCommandQueue &ci);
		~SWCommandQueue ();


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _Update (const Message< ModuleMsg::Update > &);
		
		bool _SubmitGraphicsQueueCommands (const Message< GpuMsg::SubmitGraphicsQueueCommands > &);
		bool _SubmitComputeQueueCommands (const Message< GpuMsg::SubmitComputeQueueCommands > &);
		bool _SWPresent (const Message< GpuMsg::SWPresent > &);
		bool _ClientWaitFence (const Message< GpuMsg::ClientWaitFence > &);
		bool _ClientWaitDeviceIdle (const Message< GpuMsg::ClientWaitDeviceIdle > &);

	private:
		bool _Submit (const GpuMsg::SubmitGraphicsQueueCommands &msg);
		bool _Execute (INOUT FencesSet_t &fences);
		bool _Execute ();
	};
//-----------------------------------------------------------------------------

	
	const TypeIdList	SWCommandQueue::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	SWCommandQueue::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SWCommandQueue::SWCommandQueue (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandQueue &ci) :
		SWBaseModule( gs, ModuleConfig{ id, 1 }, &_msgTypes, &_eventTypes ),
		_family{ ci.family },	_lockQueue{ false }
	{
		SetDebugName( "SWCommandQueue" );

		_SubscribeOnMsg( this, &SWCommandQueue::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &SWCommandQueue::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &SWCommandQueue::_AttachModule_Empty );
		_SubscribeOnMsg( this, &SWCommandQueue::_DetachModule_Empty );
		_SubscribeOnMsg( this, &SWCommandQueue::_FindModule_Impl );
		_SubscribeOnMsg( this, &SWCommandQueue::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &SWCommandQueue::_Link );
		_SubscribeOnMsg( this, &SWCommandQueue::_Compose_Impl );
		_SubscribeOnMsg( this, &SWCommandQueue::_Delete );
		_SubscribeOnMsg( this, &SWCommandQueue::_Update );
		_SubscribeOnMsg( this, &SWCommandQueue::_GetDeviceInfo );
		_SubscribeOnMsg( this, &SWCommandQueue::_GetSWDeviceInfo );
		_SubscribeOnMsg( this, &SWCommandQueue::_GetSWPrivateClasses );
		_SubscribeOnMsg( this, &SWCommandQueue::_OnManagerChanged );
		_SubscribeOnMsg( this, &SWCommandQueue::_SubmitGraphicsQueueCommands );
		_SubscribeOnMsg( this, &SWCommandQueue::_SubmitComputeQueueCommands );
		_SubscribeOnMsg( this, &SWCommandQueue::_SWPresent );
		_SubscribeOnMsg( this, &SWCommandQueue::_ClientWaitFence );
		_SubscribeOnMsg( this, &SWCommandQueue::_ClientWaitDeviceIdle );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	SWCommandQueue::~SWCommandQueue ()
	{
		ASSERT( _queue.Empty() );
	}

/*
=================================================
	_Delete
=================================================
*/
	bool SWCommandQueue::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		CHECK( _Execute() );

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool SWCommandQueue::_Link (const Message< ModuleMsg::Link > &msg)
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
	bool SWCommandQueue::_Update (const Message< ModuleMsg::Update > &)
	{
		if ( _IsComposedState( GetState() ) )
		{
			CHECK( _Execute() );
			return true;
		}
		return false;
	}

/*
=================================================
	_SubmitGraphicsQueueCommands
=================================================
*/
	bool SWCommandQueue::_SubmitGraphicsQueueCommands (const Message< GpuMsg::SubmitGraphicsQueueCommands > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( _family[ EQueueFamily::Graphics ] );
		
		return _Submit( *msg );
	}
	
/*
=================================================
	_SubmitComputeQueueCommands
=================================================
*/
	bool SWCommandQueue::_SubmitComputeQueueCommands (const Message< GpuMsg::SubmitComputeQueueCommands > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( _family[ EQueueFamily::Compute ] );
		
		return _Submit( *msg );
	}
	
/*
=================================================
	_Submit
=================================================
*/
	bool SWCommandQueue::_Submit (const GpuMsg::SubmitGraphicsQueueCommands &msg)
	{
		CHECK_ERR( not _lockQueue );

		Submitted	submit;

		// copy command buffers
		for (auto& cmd : msg.commands)
		{
			CHECK_ERR( cmd and cmd->GetSupportedMessages().HasAllTypes< CmdBufferMsgList_t >() );

			submit.commands.PushBack({ cmd, ExecuteCmdBufferMsg{} });
			
			SendTo( cmd, Message<GpuMsg::SetCommandBufferState>{ GpuMsg::SetCommandBufferState::EState::Pending });
		}

		// copy signal semaphores
		for (auto& sem : msg.signalSemaphores)
		{
			Message< GpuMsg::GetSWSemaphore >	req_sem{ sem };
			SendTo( _syncManager, req_sem );

			submit.signalSemaphores.PushBack( *req_sem->result );
		}

		// copy wait semaphores
		for (auto& sem : msg.waitSemaphores)
		{
			Message< GpuMsg::GetSWSemaphore >	req_sem{ sem.first };
			SendTo( _syncManager, req_sem );

			submit.waitSemaphores.PushBack({ *req_sem->result, sem.second });
		}

		// copy fence
		if ( msg.fence != GpuFenceId::Unknown )
		{
			Message< GpuMsg::GetSWFence >	req_fence{ msg.fence };
			SendTo( _syncManager, req_fence );

			submit.signalFence = *req_fence->result;
			submit.signalFence->Enqueue();
		}

		_queue.PushBack( RVREF(submit) );
		return true;
	}
	
/*
=================================================
	_Execute
=================================================
*/
	bool SWCommandQueue::_Execute ()
	{
		FencesSet_t		empty;
		return _Execute( empty );
	}

	bool SWCommandQueue::_Execute (INOUT FencesSet_t &fences)
	{
		SCOPE_SETTER( _lockQueue = true, false );

		const bool	run_to_fence = not fences.Empty();
		bool		changed		 = true;

		for (uint k = 0; k < 10; ++k)
		{
			changed = false;

			FOR( i, _queue )
			{
				auto&	submited = _queue[i];

				// wait semaphores
				FOR( j, submited.waitSemaphores )
				{
					auto&	sem = submited.waitSemaphores[j];

					if ( sem.first->TryLock( sem.second ) )
					{
						submited.waitSemaphores.Erase( j );
						--j;
					}
				}

				// execute command buffers
				FOR( j, submited.commands )
				{
					auto&	cmd = submited.commands[j];

					Message< GpuMsg::ExecuteSWCommandBuffer >	execute{ cmd.second };
					SendTo( cmd.first, execute );

					cmd.second	= *execute;

					// TODO: compare lastIndex and set 'changed' to true

					// remove command buffer if completed
					if ( *cmd.second.completed )
					{
						SendTo( cmd.first, Message<GpuMsg::SetCommandBufferState>{ GpuMsg::SetCommandBufferState::EState::Completed });

						submited.commands.Erase( j );
						--j;

						changed = true;
					}
				}

				// finish
				if ( submited.commands.Empty() )
				{
					for (auto& sem : submited.signalSemaphores) {
						sem->Unlock();
					}

					if ( submited.signalFence )
					{
						submited.signalFence->Signal();

						fences.Erase( submited.signalFence );
					}

					_queue.Erase( i );
					--i;

					changed = true;
				}

				// exit if all fences was signaled
				if ( run_to_fence and fences.Empty() )
					return true;
			}

			// reset counter if something changed
			if ( changed )
			{
				k = 0;
			}
		}
		
		// check for deadlock on semaphore or event
		if ( not changed and not _queue.Empty() )
		{
			_SendEvent< GpuMsg::DeviceLost >({});
			return false;
		}

		// check for deadlock on fence
		if ( run_to_fence )
		{
			_SendEvent< GpuMsg::DeviceLost >({});
			return false;
		}
		return true;
	}

/*
=================================================
	_SWPresent
=================================================
*/
	bool SWCommandQueue::_SWPresent (const Message< GpuMsg::SWPresent > &)
	{
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_ClientWaitFence
=================================================
*/
	bool SWCommandQueue::_ClientWaitFence (const Message< GpuMsg::ClientWaitFence > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( not msg->fences.Empty() );

		FencesSet_t		fences;

		for (auto& wfence : msg->fences)
		{
			Message< GpuMsg::GetSWFence >	req_fence{ wfence };
			SendTo( _syncManager, req_fence );

			CHECK_ERR( req_fence->result and (*req_fence->result)->IsEnqueued() );
			fences.Add( *req_fence->result );
		}

		if ( fences.Empty() )
			return true;

		CHECK( _Execute( fences ) );
		return true;
	}
	
/*
=================================================
	_ClientWaitDeviceIdle
=================================================
*/
	bool SWCommandQueue::_ClientWaitDeviceIdle (const Message< GpuMsg::ClientWaitDeviceIdle > &)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );

		CHECK( _Execute() );
		return true;
	}

}	// PlatformSW
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr SoftRendererObjectsConstructor::CreateSWCommandQueue (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandQueue &ci)
	{
		return New< PlatformSW::SWCommandQueue >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_SOFT
