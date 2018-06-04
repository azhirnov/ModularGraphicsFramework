// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_OPENGL

#include "Engine/Platforms/Public/GPU/Sync.h"
#include "Engine/Platforms/OpenGL/450/GL4BaseModule.h"
#include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"

namespace Engine
{
namespace PlatformGL
{
	using namespace gl;

	
	//
	// OpenGL Sync Manager
	//

	class GL4SyncManager final : public GL4BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= GL4BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::ClientWaitDeviceIdle,
											GpuMsg::CreateFence,
											GpuMsg::DestroyFence,
											GpuMsg::GLFenceSync,
											GpuMsg::ClientWaitFence,
											GpuMsg::CreateEvent,
											GpuMsg::DestroyEvent,
											GpuMsg::SetEvent,
											GpuMsg::ResetEvent,
											GpuMsg::GetGLEvent,
											GpuMsg::CreateSemaphore,
											GpuMsg::DestroySemaphore,
											GpuMsg::GLSemaphoreEnqueue,
											GpuMsg::GetGLSemaphore,
											GpuMsg::WaitGLSemaphore
										> >;

		using SupportedEvents_t		= GL4BaseModule::SupportedEvents_t;

		using SyncUnion_t			= Union< GLsync >;
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
		GL4SyncManager (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuSyncManager &ci);
		~GL4SyncManager ();


	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _OnManagerChanged2 (const Message< ModuleMsg::OnManagerChanged > &msg);

		bool _ClientWaitDeviceIdle (const Message< GpuMsg::ClientWaitDeviceIdle > &);
		bool _CreateFence (const Message< GpuMsg::CreateFence > &);
		bool _DestroyFence (const Message< GpuMsg::DestroyFence > &);
		bool _ClientWaitFence (const Message< GpuMsg::ClientWaitFence > &);
		bool _CreateEvent (const Message< GpuMsg::CreateEvent > &);
		bool _DestroyEvent (const Message< GpuMsg::DestroyEvent > &);
		bool _SetEvent (const Message< GpuMsg::SetEvent > &);
		bool _ResetEvent (const Message< GpuMsg::ResetEvent > &);
		bool _GetGLEvent (const Message< GpuMsg::GetGLEvent > &);
		bool _CreateSemaphore (const Message< GpuMsg::CreateSemaphore > &);
		bool _DestroySemaphore (const Message< GpuMsg::DestroySemaphore > &);
		bool _GLFenceSync (const Message< GpuMsg::GLFenceSync > &);
		bool _GLSemaphoreEnqueue (const Message< GpuMsg::GLSemaphoreEnqueue > &);
		bool _GetGLSemaphore (const Message< GpuMsg::GetGLSemaphore > &);
		bool _WaitGLSemaphore (const Message< GpuMsg::WaitGLSemaphore > &);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	GL4SyncManager::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	GL4SyncManager::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4SyncManager::GL4SyncManager (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuSyncManager &ci) :
		GL4BaseModule( gs, ModuleConfig{ id, 1 }, &_msgTypes, &_eventTypes ),
		_counter{ 0 }
	{
		SetDebugName( "GL4SyncManager" );

		_SubscribeOnMsg( this, &GL4SyncManager::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &GL4SyncManager::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &GL4SyncManager::_AttachModule_Empty );
		_SubscribeOnMsg( this, &GL4SyncManager::_DetachModule_Empty );
		_SubscribeOnMsg( this, &GL4SyncManager::_FindModule_Impl );
		_SubscribeOnMsg( this, &GL4SyncManager::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &GL4SyncManager::_Link_Impl );
		_SubscribeOnMsg( this, &GL4SyncManager::_Compose_Impl );
		_SubscribeOnMsg( this, &GL4SyncManager::_Delete );
		_SubscribeOnMsg( this, &GL4SyncManager::_OnManagerChanged2 );
		_SubscribeOnMsg( this, &GL4SyncManager::_GetDeviceInfo );
		_SubscribeOnMsg( this, &GL4SyncManager::_GetGLDeviceInfo );
		_SubscribeOnMsg( this, &GL4SyncManager::_GetGLPrivateClasses );
		_SubscribeOnMsg( this, &GL4SyncManager::_ClientWaitDeviceIdle );
		_SubscribeOnMsg( this, &GL4SyncManager::_CreateFence );
		_SubscribeOnMsg( this, &GL4SyncManager::_DestroyFence );
		_SubscribeOnMsg( this, &GL4SyncManager::_ClientWaitFence );
		_SubscribeOnMsg( this, &GL4SyncManager::_CreateEvent );
		_SubscribeOnMsg( this, &GL4SyncManager::_DestroyEvent );
		_SubscribeOnMsg( this, &GL4SyncManager::_SetEvent );
		_SubscribeOnMsg( this, &GL4SyncManager::_ResetEvent );
		_SubscribeOnMsg( this, &GL4SyncManager::_GetGLEvent );
		_SubscribeOnMsg( this, &GL4SyncManager::_CreateSemaphore );
		_SubscribeOnMsg( this, &GL4SyncManager::_DestroySemaphore );
		_SubscribeOnMsg( this, &GL4SyncManager::_GLFenceSync );
		//_SubscribeOnMsg( this, &GL4SyncManager::_GetGLFence );
		_SubscribeOnMsg( this, &GL4SyncManager::_GLSemaphoreEnqueue );
		_SubscribeOnMsg( this, &GL4SyncManager::_GetGLSemaphore );
		_SubscribeOnMsg( this, &GL4SyncManager::_WaitGLSemaphore );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	GL4SyncManager::~GL4SyncManager ()
	{
		ASSERT( _syncs.Empty() );
	}
	
/*
=================================================
	_DeleteSync_Func
=================================================
*/
	struct GL4SyncManager::_DeleteSync_Func
	{
		_DeleteSync_Func ()
		{}

		void operator () (GLsync fence) const
		{
			GL_CALL( glDeleteSync( fence ) );
		}
	};

/*
=================================================
	_Delete
=================================================
*/
	bool GL4SyncManager::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DeleteSync_Func	func;

		for (auto& sync : _syncs) {
			sync.second.Apply( func );
		}

		_syncs.Clear();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_OnManagerChanged2
=================================================
*/
	bool GL4SyncManager::_OnManagerChanged2 (const Message< ModuleMsg::OnManagerChanged > &msg)
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
	bool GL4SyncManager::_ClientWaitDeviceIdle (const Message< GpuMsg::ClientWaitDeviceIdle > &)
	{
		CHECK( _GetManager()->Send< GpuMsg::SyncGLClientWithDevice >({}) );

		GL_CALL( glFinish() );
		return true;
	}

/*
=================================================
	_CreateFence
=================================================
*/
	bool GL4SyncManager::_CreateFence (const Message< GpuMsg::CreateFence > &msg)
	{
		for (;;)
		{
			++_counter;

			if ( not _syncs.IsExist( _counter ) )
				break;
		}
		_syncs.Add( _counter, SyncUnion_t{ GLsync(null) } );

		msg->result.Set( GpuFenceId(_counter) );
		return true;
	}
	
/*
=================================================
	_DestroyFence
=================================================
*/
	bool GL4SyncManager::_DestroyFence (const Message< GpuMsg::DestroyFence > &msg)
	{
		SyncArray_t::iterator	iter;

		if ( _syncs.Find( ulong(msg->id), OUT iter ) )
		{
			CHECK_ERR( iter->second.Is< GLsync >() );
			
			GLsync&	fence = iter->second.Get< GLsync >();

			if ( fence ) {
				GL_CALL( glDeleteSync( fence ) );
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
	bool GL4SyncManager::_ClientWaitFence (const Message< GpuMsg::ClientWaitFence > &msg)
	{
		const GLuint64	timeout = msg->timeout.NanoSeconds();

		for (auto& wfence : msg->fences)
		{
			SyncArray_t::iterator	iter;
			CHECK_ERR( _syncs.Find( ulong(wfence), OUT iter ) );

			GLsync	fence = iter->second.Get< GLsync >();

			if ( fence ) {
				GL_CALL( glClientWaitSync( fence, GL_SYNC_FLUSH_COMMANDS_BIT, timeout ) );
			}
			else {
				CHECK( _GetManager()->Send< GpuMsg::SyncGLClientWithDevice >( wfence ) );
			}
		}
		return true;
	}

/*
=================================================
	_GLFenceSync
=================================================
*/
	bool GL4SyncManager::_GLFenceSync (const Message< GpuMsg::GLFenceSync > &msg)
	{
		SyncArray_t::iterator	iter;
		CHECK_ERR( _syncs.Find( ulong(msg->fenceId), OUT iter ) );
		CHECK_ERR( iter->second.Is< GLsync >() );
		
		GLsync&	fence = iter->second.Get< GLsync >();

		if ( fence ) {
			GL_CALL( glDeleteSync( fence ) );
		}
			
		GL_CALL( fence = glFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 ) );

		msg->result.Set( fence );
		return true;
	}
	
/*
=================================================
	_GetGLFence
=================================================
*
	bool GL4SyncManager::_GetGLFence (const Message< GpuMsg::GetGLFence > &msg)
	{
		SyncArray_t::iterator	iter;
		CHECK_ERR( _syncs.Find( ulong(msg->fenceId), OUT iter ) );
		CHECK_ERR( iter->second.Is< GLsync >() );
		
		msg->result.Set( iter->second.Get< GLsync >() );
		return true;
	}

/*
=================================================
	_CreateEvent
=================================================
*/
	bool GL4SyncManager::_CreateEvent (const Message< GpuMsg::CreateEvent > &msg)
	{
		for (;;)
		{
			++_counter;

			if ( not _syncs.IsExist( _counter ) )
				break;
		}
		_syncs.Add( _counter, SyncUnion_t{ GLsync(null) } );

		msg->result.Set( GpuEventId(_counter) );
		return true;
	}
	
/*
=================================================
	_DestroyEvent
=================================================
*/
	bool GL4SyncManager::_DestroyEvent (const Message< GpuMsg::DestroyEvent > &msg)
	{
		SyncArray_t::iterator	iter;

		if ( _syncs.Find( ulong(msg->id), OUT iter ) )
		{
			CHECK_ERR( iter->second.Is< GLsync >() );
			
			GLsync&	sem = iter->second.Get< GLsync >();

			if ( sem ) {
				GL_CALL( glDeleteSync( sem ) );
			}

			_syncs.EraseByIter( iter );
		}
		return true;
	}
	
/*
=================================================
	_SetEvent
=================================================
*/
	bool GL4SyncManager::_SetEvent (const Message< GpuMsg::SetEvent > &msg)
	{
		SyncArray_t::iterator	iter;
		CHECK_ERR( _syncs.Find( ulong(msg->id), OUT iter ) );
		CHECK_ERR( iter->second.Is< GLsync >() );

		GLsync&	event = iter->second.Get< GLsync >();

		// don't create fence, it is same to make event signaled
		if ( event == null )
		{
			return true;
		}

		// fence already created and may be unsignaled, and we can't change fence state
		GLint	values[1] = {};
		GLsizei	length = 0;
		GL_CALL( glGetSynciv( event, GL_SYNC_STATUS, sizeof(values), OUT &length, OUT values ) );
		ASSERT( values[0] == GL_SIGNALED or values[0] == GL_UNSIGNALED );

		// fence is already signaled, 'SetEvent' has no effect
		CHECK_ERR( values[0] == GL_SIGNALED );
		return true;
	}
	
/*
=================================================
	_ResetEvent
=================================================
*/
	bool GL4SyncManager::_ResetEvent (const Message< GpuMsg::ResetEvent > &msg)
	{
		SyncArray_t::iterator	iter;
		CHECK_ERR( _syncs.Find( ulong(msg->id), OUT iter ) );
		CHECK_ERR( iter->second.Is< GLsync >() );

		GLsync&	event = iter->second.Get< GLsync >();

		// create fence, it is same to make event unsignaled
		if ( event == null )
		{
			GL_CALL( event = glFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 ) );
			return true;
		}

		// fence already created and may be unsignaled, and we can't change fence state
		GLint	values[1] = {};
		GLsizei	length = 0;
		GL_CALL( glGetSynciv( event, GL_SYNC_STATUS, sizeof(values), OUT &length, OUT values ) );
		ASSERT( values[0] == GL_SIGNALED or values[0] == GL_UNSIGNALED );

		// fence is already unsignaled, 'ResetEvent' has no effect
		CHECK_ERR( values[0] == GL_UNSIGNALED );
		
		// TODO
		// it's bad, but it's only one way to reset event
		//GL_CALL( glWaitSync( event, 0, GLuint64(GL_TIMEOUT_IGNORED) ) );
		//GL_CALL( glClientWaitSync( event, GL_SYNC_FLUSH_COMMANDS_BIT, 1000_sec.NanoSeconds() ) );
		return true;
	}
	
/*
=================================================
	_GetGLEvent
=================================================
*/
	bool GL4SyncManager::_GetGLEvent (const Message< GpuMsg::GetGLEvent > &msg)
	{
		SyncArray_t::iterator	iter;
		CHECK_ERR( _syncs.Find( ulong(msg->eventId), OUT iter ) );
		CHECK_ERR( iter->second.Is< GLsync >() );
		
		msg->result.Set( iter->second.Get< GLsync >() );
		return true;
	}

/*
=================================================
	_CreateSemaphore
=================================================
*/
	bool GL4SyncManager::_CreateSemaphore (const Message< GpuMsg::CreateSemaphore > &msg)
	{
		for (;;)
		{
			++_counter;

			if ( not _syncs.IsExist( _counter ) )
				break;
		}
		_syncs.Add( _counter, SyncUnion_t{ GLsync(null) } );

		msg->result.Set( GpuSemaphoreId(_counter) );
		return true;
	}
	
/*
=================================================
	_DestroySemaphore
=================================================
*/
	bool GL4SyncManager::_DestroySemaphore (const Message< GpuMsg::DestroySemaphore > &msg)
	{
		SyncArray_t::iterator	iter;

		if ( _syncs.Find( ulong(msg->id), OUT iter ) )
		{
			CHECK_ERR( iter->second.Is< GLsync >() );
			
			GLsync&	sem = iter->second.Get< GLsync >();

			if ( sem ) {
				GL_CALL( glDeleteSync( sem ) );
			}

			_syncs.EraseByIter( iter );
		}
		return true;
	}
	
/*
=================================================
	_GLSemaphoreEnqueue
=================================================
*/
	bool GL4SyncManager::_GLSemaphoreEnqueue (const Message< GpuMsg::GLSemaphoreEnqueue > &msg)
	{
		SyncArray_t::iterator	iter;
		CHECK_ERR( _syncs.Find( ulong(msg->semId), OUT iter ) );
		CHECK_ERR( iter->second.Is< GLsync >() );
		
		GLsync&	sem = iter->second.Get< GLsync >();

		if ( sem ) {
			GL_CALL( glDeleteSync( sem ) );
		}
			
		GL_CALL( sem = glFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 ) );

		msg->result.Set( sem );
		return true;
	}
	
/*
=================================================
	_GetGLSemaphore
=================================================
*/
	bool GL4SyncManager::_GetGLSemaphore (const Message< GpuMsg::GetGLSemaphore > &msg)
	{
		SyncArray_t::iterator	iter;
		CHECK_ERR( _syncs.Find( ulong(msg->semId), OUT iter ) );
		CHECK_ERR( iter->second.Is< GLsync >() );
		
		msg->result.Set( iter->second.Get< GLsync >() );
		return true;
	}
	
/*
=================================================
	_WaitGLSemaphore
=================================================
*/
	bool GL4SyncManager::_WaitGLSemaphore (const Message< GpuMsg::WaitGLSemaphore > &msg)
	{
		Message< GpuMsg::GetGLSemaphore >	req_sem{ msg->semId };
		CHECK_ERR( _GetGLSemaphore( req_sem ) );
		CHECK_ERR( req_sem->result and *req_sem->result != null );

		GL_CALL( glWaitSync( *req_sem->result, 0, GLuint64(GL_TIMEOUT_IGNORED) ) );
		return true;
	}

}	// PlatformGL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenGLObjectsConstructor::CreateGL4SyncManager (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuSyncManager &ci)
	{
		return New< PlatformGL::GL4SyncManager >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
