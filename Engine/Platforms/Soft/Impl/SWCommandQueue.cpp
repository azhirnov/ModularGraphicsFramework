// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

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
											GpuMsg::SubmitCommands,
											GpuMsg::QueueBindSparse,
											GpuMsg::GetCommandQueueDescription,
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
			static constexpr uint	CMD_SIZE	= GpuMsg::SubmitCommands::Commands_t::MemoryContainer_t::SIZE;
			static constexpr uint	SS_SIZE		= GpuMsg::SubmitCommands::Semaphores_t::MemoryContainer_t::SIZE;
			static constexpr uint	WS_SIZE		= GpuMsg::SubmitCommands::WaitSemaphores_t::MemoryContainer_t::SIZE;

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
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ModulePtr					_syncManager;
		CmdQueue_t					_queue;
		CommandQueueDescription		_descr;
		bool						_lockQueue;


	// methods
	public:
		SWCommandQueue (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuCommandQueue &ci);
		~SWCommandQueue ();


	// message handlers
	private:
		bool _Link (const ModuleMsg::Link &);
		bool _Delete (const ModuleMsg::Delete &);
		bool _Update (const ModuleMsg::Update &);
		
		bool _SubmitCommands (const GpuMsg::SubmitCommands &);
		bool _QueueBindSparse (const GpuMsg::QueueBindSparse &);
		bool _GetCommandQueueDescription (const GpuMsg::GetCommandQueueDescription &);
		bool _SWPresent (const GpuMsg::SWPresent &);
		bool _ClientWaitFence (const GpuMsg::ClientWaitFence &);
		bool _ClientWaitDeviceIdle (const GpuMsg::ClientWaitDeviceIdle &);

	private:
		bool _Execute (INOUT FencesSet_t &fences);
		bool _Execute ();
	};
//-----------------------------------------------------------------------------

	
	const TypeIdList	SWCommandQueue::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SWCommandQueue::SWCommandQueue (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandQueue &ci) :
		SWBaseModule( gs, ModuleConfig{ id, 1 }, &_eventTypes ),
		_descr{ ci.descr },		_lockQueue{ false }
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
		_SubscribeOnMsg( this, &SWCommandQueue::_SubmitCommands );
		_SubscribeOnMsg( this, &SWCommandQueue::_QueueBindSparse );
		_SubscribeOnMsg( this, &SWCommandQueue::_GetCommandQueueDescription );
		_SubscribeOnMsg( this, &SWCommandQueue::_SWPresent );
		_SubscribeOnMsg( this, &SWCommandQueue::_ClientWaitFence );
		_SubscribeOnMsg( this, &SWCommandQueue::_ClientWaitDeviceIdle );

		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

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
	bool SWCommandQueue::_Delete (const ModuleMsg::Delete &msg)
	{
		CHECK( _Execute() );

		_descr = Uninitialized;

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool SWCommandQueue::_Link (const ModuleMsg::Link &msg)
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
	bool SWCommandQueue::_Update (const ModuleMsg::Update &)
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
	_SubmitCommands
=================================================
*/
	bool SWCommandQueue::_SubmitCommands (const GpuMsg::SubmitCommands &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( _descr.family & (EQueueFamily::Graphics | EQueueFamily::Compute) );
		CHECK_ERR( not _lockQueue );

		Submitted	submit;

		// copy command buffers
		for (auto& cmd : msg.commands)
		{
			CHECK_ERR( cmd and cmd->SupportsAllMessages< CmdBufferMsgList_t >() );

			submit.commands.PushBack({ cmd, ExecuteCmdBufferMsg{} });
			
			cmd->Send( GpuMsg::SetCommandBufferState{ GpuMsg::SetCommandBufferState::EState::Pending });
		}

		// copy signal semaphores
		for (auto& sem : msg.signalSemaphores)
		{
			const auto	id = _syncManager->Request(GpuMsg::GetSWSemaphore{ sem });

			submit.signalSemaphores.PushBack( id );
		}

		// copy wait semaphores
		for (auto& sem : msg.waitSemaphores)
		{
			const auto	id = _syncManager->Request( GpuMsg::GetSWSemaphore{ sem.first } );

			submit.waitSemaphores.PushBack({ id, sem.second });
		}

		// copy fence
		if ( msg.fence != GpuFenceId::Unknown )
		{
			submit.signalFence = _syncManager->Request(GpuMsg::GetSWFence{ msg.fence });
			submit.signalFence->Enqueue();
		}

		_queue.PushBack( RVREF(submit) );
		return true;
	}
	
/*
=================================================
	_QueueBindSparse
=================================================
*/
	bool SWCommandQueue::_QueueBindSparse (const GpuMsg::QueueBindSparse &)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( _descr.family[EQueueFamily::SparseBinding] );
		CHECK_ERR( not _lockQueue );

		TODO( "" );
		return true;
	}

/*
=================================================
	_GetCommandQueueDescription
=================================================
*/
	bool SWCommandQueue::_GetCommandQueueDescription (const GpuMsg::GetCommandQueueDescription &msg)
	{
		msg.result.Set( _descr );
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

					cmd.first->Send( cmd.second );

					// TODO: compare lastIndex and set 'changed' to true

					// remove command buffer if completed
					if ( *cmd.second.completed )
					{
						cmd.first->Send( GpuMsg::SetCommandBufferState{ GpuMsg::SetCommandBufferState::EState::Completed });

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
			_SendEvent( GpuMsg::DeviceLost{} );
			return false;
		}

		// check for deadlock on fence
		if ( run_to_fence )
		{
			_SendEvent( GpuMsg::DeviceLost{} );
			return false;
		}
		return true;
	}

/*
=================================================
	_SWPresent
=================================================
*/
	bool SWCommandQueue::_SWPresent (const GpuMsg::SWPresent &)
	{
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_ClientWaitFence
=================================================
*/
	bool SWCommandQueue::_ClientWaitFence (const GpuMsg::ClientWaitFence &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( not msg.fences.Empty() );

		FencesSet_t		fences;

		for (auto& wfence : msg.fences)
		{
			const auto	fence = _syncManager->Request( GpuMsg::GetSWFence{ wfence } );

			CHECK_ERR( fence->IsEnqueued() );
			fences.Add( fence );
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
	bool SWCommandQueue::_ClientWaitDeviceIdle (const GpuMsg::ClientWaitDeviceIdle &)
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
