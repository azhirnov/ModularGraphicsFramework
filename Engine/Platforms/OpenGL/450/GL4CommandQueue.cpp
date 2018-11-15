// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_OPENGL

#include "Engine/Platforms/Public/GPU/Sync.h"
#include "Engine/Platforms/OpenGL/450/GL4BaseModule.h"
#include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"
#include "Engine/Platforms/Public/Tools/AsyncCommandsEmulator.h"
#include "Engine/Platforms/OpenGL/450/GL4ResourceCache.h"

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
											GpuMsg::SubmitCommands,
											GpuMsg::QueueBindSparse,
											GpuMsg::GetCommandQueueDescription,
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
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ModulePtr					_syncManager;
		CommandQueueDescription		_descr;

		AsyncCommands_t				_commands;
		uint						_framesWithoutSubmitting;


	// methods
	public:
		GL4CommandQueue (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuCommandQueue &ci);
		~GL4CommandQueue ();


	// message handlers
	private:
		bool _Link (const ModuleMsg::Link &);
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);
		bool _Update (const ModuleMsg::Update &);
		
		bool _SubmitCommands (const GpuMsg::SubmitCommands &);
		bool _QueueBindSparse (const GpuMsg::QueueBindSparse &);
		bool _GetCommandQueueDescription (const GpuMsg::GetCommandQueueDescription &);
		bool _SyncGLClientWithDevice (const GpuMsg::SyncGLClientWithDevice &);
		bool _GLFlushQueue (const GpuMsg::GLFlushQueue &);

	private:
		bool _SubmitQueue (const GpuMsg::SubmitCommands &) const;
	};
//-----------------------------------------------------------------------------

	
	const TypeIdList	GL4CommandQueue::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4CommandQueue::GL4CommandQueue (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandQueue &ci) :
		GL4BaseModule( gs, ModuleConfig{ id, 1 }, &_eventTypes ),
		_descr{ ci.descr },		_framesWithoutSubmitting{ 0 }
	{
		SetDebugName( "GL4CommandQueue" );

		_SubscribeOnMsg( this, &GL4CommandQueue::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &GL4CommandQueue::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &GL4CommandQueue::_AttachModule_Empty );
		_SubscribeOnMsg( this, &GL4CommandQueue::_DetachModule_Empty );
		_SubscribeOnMsg( this, &GL4CommandQueue::_FindModule_Impl );
		_SubscribeOnMsg( this, &GL4CommandQueue::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &GL4CommandQueue::_Link );
		_SubscribeOnMsg( this, &GL4CommandQueue::_Compose );
		_SubscribeOnMsg( this, &GL4CommandQueue::_Delete );
		_SubscribeOnMsg( this, &GL4CommandQueue::_Update );
		_SubscribeOnMsg( this, &GL4CommandQueue::_GetDeviceInfo );
		_SubscribeOnMsg( this, &GL4CommandQueue::_GetGLDeviceInfo );
		_SubscribeOnMsg( this, &GL4CommandQueue::_GetGLPrivateClasses );
		_SubscribeOnMsg( this, &GL4CommandQueue::_OnManagerChanged );
		_SubscribeOnMsg( this, &GL4CommandQueue::_SubmitCommands );
		_SubscribeOnMsg( this, &GL4CommandQueue::_QueueBindSparse );
		_SubscribeOnMsg( this, &GL4CommandQueue::_GetCommandQueueDescription );
		_SubscribeOnMsg( this, &GL4CommandQueue::_SyncGLClientWithDevice );
		_SubscribeOnMsg( this, &GL4CommandQueue::_GLFlushQueue );

		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

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
	bool GL4CommandQueue::_Delete (const ModuleMsg::Delete &msg)
	{
		_GLFlushQueue( {} );
		
		_descr = Uninitialized;

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool GL4CommandQueue::_Link (const ModuleMsg::Link &msg)
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
	bool GL4CommandQueue::_Compose (const ModuleMsg::Compose &)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		if ( _descr.family[EQueueFamily::SparseBinding] )
		{
			_descr.family[EQueueFamily::SparseBinding] =
				GetDevice()->IsExtensionSupported( "GL_ARB_sparse_texture" ) and
				GetDevice()->IsExtensionSupported( "GL_ARB_sparse_buffer" );
		}

		return Module::_DefCompose( false );
	}

/*
=================================================
	_Update
=================================================
*/
	bool GL4CommandQueue::_Update (const ModuleMsg::Update &)
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
	_SubmitCommands
=================================================
*/
	bool GL4CommandQueue::_SubmitCommands (const GpuMsg::SubmitCommands &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( _descr.family & (EQueueFamily::Graphics | EQueueFamily::Compute) );
		
		_commands.Add( msg );
		return true;
	}
	
/*
=================================================
	_QueueBindSparse
=================================================
*/
	bool GL4CommandQueue::_QueueBindSparse (const GpuMsg::QueueBindSparse &msg)
	{
		using namespace gl;

		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( _descr.family[EQueueFamily::SparseBinding] );
		CHECK_ERR( msg.imageOpaqueBings.Empty() );	// not supported

		
		for (auto& bind : msg.bufferBinds)
		{
			GLuint	buffer_id = bind.first->Request( GpuMsg::GetGLBufferID{} );
			
			for (auto& part : bind.second)
			{
				GL_CALL( glNamedBufferPageCommitmentARB( buffer_id, GLintptr(part.resourceOffset), GLsizeiptr(part.size), GL_TRUE ) );
			}
		}

		for (auto& bind : msg.imageBinds)
		{
			auto&	img_res = GetResourceCache()->GetImageID( bind.first );
			GLenum	target	= GL4Enum( img_res.Get<1>().imageType );

			GL_CALL( glBindTexture( target, img_res.Get<0>() ) );

			for (auto& part : bind.second)
			{
				GL_CALL( glTexPageCommitmentARB( target, part.subresource.mipLevel.Get(),
												 part.offset.x, part.offset.y, part.offset.z,
												 part.extent.x, part.extent.y, part.extent.z, GL_TRUE ) );
			}
		}

		return true;
	}

/*
=================================================
	_GetCommandQueueDescription
=================================================
*/
	bool GL4CommandQueue::_GetCommandQueueDescription (const GpuMsg::GetCommandQueueDescription &msg)
	{
		msg.result.Set( _descr );
		return true;
	}

/*
=================================================
	_SyncGLClientWithDevice
=================================================
*/
	bool GL4CommandQueue::_SyncGLClientWithDevice (const GpuMsg::SyncGLClientWithDevice &msg)
	{
		CHECK_ERR( _commands.Run( msg.fenceId, DelegateBuilder( this, &GL4CommandQueue::_SubmitQueue ) ) );
		return true;
	}
	
/*
=================================================
	_SubmitQueue
=================================================
*/
	bool GL4CommandQueue::_SubmitQueue (const GpuMsg::SubmitCommands &cmd) const
	{
		DEBUG_ONLY(
		for (auto& com : cmd.commands ) {
			CHECK_ERR( not com->Request(GpuMsg::GetCommandBufferDescription{}).flags[ ECmdBufferCreate::Secondary ] );
		});

		// wait for signal
		for (auto& sem : cmd.waitSemaphores)
		{
			ASSERT(sem.second == EPipelineStage::AllCommands );
			GpuMsg::WaitGLSemaphore		wait{ sem.first };
			CHECK( _syncManager->Send( wait ) );
		}

		// execute command buffers
		ModuleUtils::Send( cmd.commands, GpuMsg::ExecuteGLCommandBuffer{} );
		
		// enqueue fence
		if ( cmd.fence != GpuFenceId::Unknown )
		{
			GpuMsg::GLFenceSync		fence_sync{ cmd.fence };
			CHECK( _syncManager->Send( fence_sync ) );
		}

		// enqueue semaphores
		for (auto& sem : cmd.signalSemaphores)
		{
			GpuMsg::GLSemaphoreEnqueue	sem_sync{ sem };
			CHECK( _syncManager->Send( sem_sync ) );
		}
		return true;
	}
	
/*
=================================================
	_GLFlushQueue
=================================================
*/
	bool GL4CommandQueue::_GLFlushQueue (const GpuMsg::GLFlushQueue &)
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
