// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_OPENGL

#include "Engine/Platforms/Public/GPU/Sync.h"
#include "Engine/Platforms/OpenGL/450/GL4BaseModule.h"
#include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"
#include "Engine/Platforms/Public/Tools/AsyncCommandsEmulator.h"

namespace Engine
{
namespace PlatformGL
{

	//
	// OpenGL Command Queue
	//

	class GL4CommandQueue final : public GL4BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= GL4BaseModule::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::Update,
											GpuMsg::SubmitGraphicsQueueCommands,
											GpuMsg::SubmitComputeQueueCommands,
											GpuMsg::SyncGLClientWithDevice,
											GpuMsg::GLFlushQueue
										> >;

		using SupportedEvents_t		= GL4BaseModule::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::DeviceLost
										> >;

		using SyncMngrMsgList_t		= MessageListFrom<
											GpuMsg::WaitGLSemaphore,
											GpuMsg::GLSemaphoreEnqueue,
											GpuMsg::GLFenceSync
										>;

		using AsyncCommands_t		= PlatformTools::AsyncCommandsEmulator;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ModulePtr			_syncManager;
		EQueueFamily::bits	_family;

		AsyncCommands_t		_commands;
		uint				_framesWithoutSubmitting;


	// methods
	public:
		GL4CommandQueue (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuCommandQueue &ci);
		~GL4CommandQueue ();


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _Update (const Message< ModuleMsg::Update > &);
		
		bool _SubmitGraphicsQueueCommands (const Message< GpuMsg::SubmitGraphicsQueueCommands > &);
		bool _SubmitComputeQueueCommands (const Message< GpuMsg::SubmitComputeQueueCommands > &);
		bool _SyncGLClientWithDevice (const Message< GpuMsg::SyncGLClientWithDevice > &);
		bool _GLFlushQueue (const Message< GpuMsg::GLFlushQueue > &);

	private:
		bool _Submit (const GpuMsg::SubmitGraphicsQueueCommands &msg);
		bool _SubmitQueue (const GpuMsg::SubmitGraphicsQueueCommands &cmd) const;
	};
//-----------------------------------------------------------------------------

	
	const TypeIdList	GL4CommandQueue::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	GL4CommandQueue::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4CommandQueue::GL4CommandQueue (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandQueue &ci) :
		GL4BaseModule( gs, ModuleConfig{ id, 1 }, &_msgTypes, &_eventTypes ),
		_family{ ci.family },		_framesWithoutSubmitting{ 0 }
	{
		SetDebugName( "GL4CommandQueue" );

		_SubscribeOnMsg( this, &GL4CommandQueue::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &GL4CommandQueue::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &GL4CommandQueue::_AttachModule_Empty );
		_SubscribeOnMsg( this, &GL4CommandQueue::_DetachModule_Empty );
		_SubscribeOnMsg( this, &GL4CommandQueue::_FindModule_Impl );
		_SubscribeOnMsg( this, &GL4CommandQueue::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &GL4CommandQueue::_Link );
		_SubscribeOnMsg( this, &GL4CommandQueue::_Compose_Impl );
		_SubscribeOnMsg( this, &GL4CommandQueue::_Delete );
		_SubscribeOnMsg( this, &GL4CommandQueue::_Update );
		_SubscribeOnMsg( this, &GL4CommandQueue::_GetDeviceInfo );
		_SubscribeOnMsg( this, &GL4CommandQueue::_GetGLDeviceInfo );
		_SubscribeOnMsg( this, &GL4CommandQueue::_GetGLPrivateClasses );
		_SubscribeOnMsg( this, &GL4CommandQueue::_OnManagerChanged );
		_SubscribeOnMsg( this, &GL4CommandQueue::_SubmitGraphicsQueueCommands );
		_SubscribeOnMsg( this, &GL4CommandQueue::_SubmitComputeQueueCommands );
		_SubscribeOnMsg( this, &GL4CommandQueue::_SyncGLClientWithDevice );
		_SubscribeOnMsg( this, &GL4CommandQueue::_GLFlushQueue );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	GL4CommandQueue::~GL4CommandQueue ()
	{
		ASSERT( _commands.Empty() );
	}

/*
=================================================
	_Delete
=================================================
*/
	bool GL4CommandQueue::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_GLFlushQueue( {} );

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool GL4CommandQueue::_Link (const Message< ModuleMsg::Link > &msg)
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
	bool GL4CommandQueue::_Update (const Message< ModuleMsg::Update > &)
	{
		if ( not _IsComposedState( GetState() ) )
			return false;
		
		if ( not _commands.Empty() )
		{
			if ( ++_framesWithoutSubmitting > 2 )
			{
				_GLFlushQueue( {} );

				_framesWithoutSubmitting = 0;
			}
		}
		else
			_framesWithoutSubmitting = 0;

		return true;
	}

/*
=================================================
	_SubmitGraphicsQueueCommands
=================================================
*/
	bool GL4CommandQueue::_SubmitGraphicsQueueCommands (const Message< GpuMsg::SubmitGraphicsQueueCommands > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( _family[ EQueueFamily::Graphics ] );
		
		_commands.Add( *msg );
		return true;
	}
	
/*
=================================================
	_SubmitComputeQueueCommands
=================================================
*/
	bool GL4CommandQueue::_SubmitComputeQueueCommands (const Message< GpuMsg::SubmitComputeQueueCommands > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( _family[ EQueueFamily::Compute ] );
		
		_commands.Add( *msg );
		return true;
	}
	
/*
=================================================
	_SyncGLClientWithDevice
=================================================
*/
	bool GL4CommandQueue::_SyncGLClientWithDevice (const Message< GpuMsg::SyncGLClientWithDevice > &msg)
	{
		CHECK_ERR( _commands.Run( msg->fenceId, DelegateBuilder( this, &GL4CommandQueue::_SubmitQueue ) ) );
		return true;
	}

/*
=================================================
	_SubmitQueue
=================================================
*/
	bool GL4CommandQueue::_SubmitQueue (const GpuMsg::SubmitGraphicsQueueCommands &cmd) const
	{
		DEBUG_ONLY(
			FOR( i, cmd.commands )
			{
				Message< GpuMsg::GetCommandBufferDescriptor >	req_descr;
				cmd.commands[i]->Send( req_descr );
				CHECK_ERR( req_descr->result and not req_descr->result->flags[ ECmdBufferCreate::Secondary ] );
			}
		);

		// wait for signal
		for (auto& sem : Range(cmd.waitSemaphores))
		{
			ASSERT(sem.second == EPipelineStage::AllCommands );
			Message< GpuMsg::WaitGLSemaphore >	wait{ sem.first };
			CHECK( _syncManager->Send( wait ) );
		}

		// execute command buffers
		ModuleUtils::Send( cmd.commands, Message<GpuMsg::ExecuteGLCommandBuffer>{} );
		
		// enqueue fence
		if ( cmd.fence != GpuFenceId::Unknown )
		{
			Message< GpuMsg::GLFenceSync >	fence_sync{ cmd.fence };
			CHECK( _syncManager->Send( fence_sync ) );
		}

		// enqueue semaphores
		for (auto& sem : Range(cmd.signalSemaphores))
		{
			Message< GpuMsg::GLSemaphoreEnqueue >	sem_sync{ sem };
			CHECK( _syncManager->Send( sem_sync ) );
		}
		return true;
	}
	
/*
=================================================
	_GLFlushQueue
=================================================
*/
	bool GL4CommandQueue::_GLFlushQueue (const Message< GpuMsg::GLFlushQueue > &)
	{
		CHECK_ERR( _commands.RunAll( DelegateBuilder( this, &GL4CommandQueue::_SubmitQueue ) ) );
		return true;
	}

}	// PlatformGL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenGLObjectsConstructor::CreateGL4CommandQueue (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandQueue &ci)
	{
		return New< PlatformGL::GL4CommandQueue >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
