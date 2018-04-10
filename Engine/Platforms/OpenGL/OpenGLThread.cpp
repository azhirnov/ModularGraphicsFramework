// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_OPENGL

#include "Engine/Platforms/Public/Tools/WindowHelper.h"
#include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"
#include "Engine/Platforms/OpenGL/450/GL4BaseModule.h"
#include "Engine/Platforms/OpenGL/450/GL4Sampler.h"
#include "Engine/Platforms/OpenGL/Windows/GLWinContext.h"
#include "Engine/Platforms/Public/Tools/AsyncCommandsEmulator.h"

namespace Engine
{
namespace Platforms
{

	//
	// OpenGL Thread
	//
	
	class OpenGLThread final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Compose
										> >
										::Append< MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager,
											ModuleMsg::OnManagerChanged,
											OSMsg::WindowCreated,
											OSMsg::WindowBeforeDestroy,
											GpuMsg::GetGraphicsModules,
											GpuMsg::ThreadBeginFrame,
											GpuMsg::ThreadEndFrame,
											GpuMsg::SubmitGraphicsQueueCommands,
											GpuMsg::SubmitComputeQueueCommands,
											GpuMsg::GetDeviceInfo,
											GpuMsg::GetGLDeviceInfo,
											GpuMsg::GetGLPrivateClasses,
											GpuMsg::GetGraphicsSettings,
											GpuMsg::GetComputeSettings,
											GpuMsg::SyncGLClientWithDevice
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::ThreadBeginFrame,
											GpuMsg::ThreadEndFrame,
											GpuMsg::DeviceCreated,
											GpuMsg::DeviceBeforeDestroy,
											GpuMsg::DeviceLost
										> >;

		using AsyncCommands_t		= PlatformTools::AsyncCommandsEmulator;

		using Context				= PlatformGL::GLRenderingContext;
		using Device				= PlatformGL::GL4Device;
		using SamplerCache			= PlatformGL::GL4SamplerCache;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;

		
	// variables
	private:
		GraphicsSettings	_settings;

		ModulePtr			_window;
		ModulePtr			_syncManager;
		
		Context				_context;
		Device				_device;
		
		SamplerCache		_samplerCache;

		AsyncCommands_t		_commands;
		uint				_framesWithoutSubmitting;

		bool				_isWindowVisible;


	// methods
	public:
		OpenGLThread (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuThread &ci);
		~OpenGLThread ();
		
		Ptr< Device >	GetDevice ()		{ return &_device; }


	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Update (const Message< ModuleMsg::Update > &);
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);
		bool _GetGraphicsModules (const Message< GpuMsg::GetGraphicsModules > &);
		
		bool _ThreadBeginFrame (const Message< GpuMsg::ThreadBeginFrame > &);
		bool _ThreadEndFrame (const Message< GpuMsg::ThreadEndFrame > &);
		bool _SubmitGraphicsQueueCommands (const Message< GpuMsg::SubmitGraphicsQueueCommands > &);
		bool _SubmitComputeQueueCommands (const Message< GpuMsg::SubmitComputeQueueCommands > &);
		bool _GetGraphicsSettings (const Message< GpuMsg::GetGraphicsSettings > &);
		bool _GetComputeSettings (const Message< GpuMsg::GetComputeSettings > &);

		bool _WindowCreated (const Message< OSMsg::WindowCreated > &);
		bool _WindowBeforeDestroy (const Message< OSMsg::WindowBeforeDestroy > &);
		bool _WindowVisibilityChanged (const Message< OSMsg::WindowVisibilityChanged > &);
		bool _WindowDescriptorChanged (const Message< OSMsg::WindowDescriptorChanged > &);
		
		bool _GetDeviceInfo (const Message< GpuMsg::GetDeviceInfo > &);
		bool _GetGLDeviceInfo (const Message< GpuMsg::GetGLDeviceInfo > &);
		bool _GetGLPrivateClasses (const Message< GpuMsg::GetGLPrivateClasses > &);
		bool _SyncGLClientWithDevice (const Message< GpuMsg::SyncGLClientWithDevice > &);

	private:
		bool _CreateDevice ();
		void _DestroyDevice ();
		void _DetachWindow ();
		bool _SubmitQueue (const GpuMsg::SubmitGraphicsQueueCommands &cmd) const;
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	OpenGLThread::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	OpenGLThread::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	OpenGLThread::OpenGLThread (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuThread &ci) :
		Module( gs, ModuleConfig{ id, 1 }, &_msgTypes, &_eventTypes ),
		_settings( ci.settings ),	_device( gs ),
		_samplerCache(),			_framesWithoutSubmitting{ 0 },
		_isWindowVisible( false )
	{
		SetDebugName( "OpenGLThread" );
		
		_SubscribeOnMsg( this, &OpenGLThread::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &OpenGLThread::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &OpenGLThread::_AttachModule_Impl );
		_SubscribeOnMsg( this, &OpenGLThread::_DetachModule_Impl );
		_SubscribeOnMsg( this, &OpenGLThread::_AddToManager );
		_SubscribeOnMsg( this, &OpenGLThread::_RemoveFromManager );
		_SubscribeOnMsg( this, &OpenGLThread::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &OpenGLThread::_FindModule_Impl );
		_SubscribeOnMsg( this, &OpenGLThread::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &OpenGLThread::_Update );
		_SubscribeOnMsg( this, &OpenGLThread::_Link );
		_SubscribeOnMsg( this, &OpenGLThread::_Delete );
		_SubscribeOnMsg( this, &OpenGLThread::_GetGraphicsModules );
		_SubscribeOnMsg( this, &OpenGLThread::_ThreadBeginFrame );
		_SubscribeOnMsg( this, &OpenGLThread::_ThreadEndFrame );
		_SubscribeOnMsg( this, &OpenGLThread::_SubmitGraphicsQueueCommands );
		_SubscribeOnMsg( this, &OpenGLThread::_SubmitComputeQueueCommands );
		_SubscribeOnMsg( this, &OpenGLThread::_WindowCreated );
		_SubscribeOnMsg( this, &OpenGLThread::_WindowBeforeDestroy );
		_SubscribeOnMsg( this, &OpenGLThread::_GetDeviceInfo );
		_SubscribeOnMsg( this, &OpenGLThread::_GetGLDeviceInfo );
		_SubscribeOnMsg( this, &OpenGLThread::_GetGLPrivateClasses );
		_SubscribeOnMsg( this, &OpenGLThread::_GetGraphicsSettings );
		_SubscribeOnMsg( this, &OpenGLThread::_GetComputeSettings );
		_SubscribeOnMsg( this, &OpenGLThread::_SyncGLClientWithDevice );
		
		CHECK( _ValidateMsgSubscriptions() );

		CHECK( ci.shared.IsNull() );	// sharing is not supported yet

		_AttachSelfToManager( ci.context, GLContextModuleID, false );
	}
	
/*
=================================================
	destructor
=================================================
*/
	OpenGLThread::~OpenGLThread ()
	{
		LOG( "OpenGLThread finalized", ELog::Debug );

		ASSERT( _window.IsNull() );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool OpenGLThread::_Link (const Message< ModuleMsg::Link > &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( GetState() == EState::Initial or GetState() == EState::LinkingFailed );

		CHECK_LINKING(( _window = PlatformTools::WindowHelper::FindWindow( GlobalSystems() ) ));

		_window->Subscribe( this, &OpenGLThread::_WindowCreated );
		_window->Subscribe( this, &OpenGLThread::_WindowBeforeDestroy );
		_window->Subscribe( this, &OpenGLThread::_WindowVisibilityChanged );
		_window->Subscribe( this, &OpenGLThread::_WindowDescriptorChanged );
		
		CHECK_ERR( Module::_Link_Impl( msg ) );

		// create sync manager
		{
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
											GLSyncManagerModuleID,
											GlobalSystems(),
											CreateInfo::GpuSyncManager{ this },
											OUT _syncManager ) );

			CHECK_ERR( _Attach( "sync", _syncManager, true ) );
			_syncManager->Send( msg );
		}

		// if window already created
		if ( _IsComposedState( _window->GetState() ) )
		{
			_SendMsg< OSMsg::WindowCreated >({});
		}
		return true;
	}
	
/*
=================================================
	_Update
=================================================
*/
	bool OpenGLThread::_Update (const Message< ModuleMsg::Update > &msg)
	{
		if ( not _IsComposedState( GetState() ) or
			 not _isWindowVisible )
		{
			return true;
		}

		_context.MakeCurrent();

		CHECK_ERR( Module::_Update_Impl( msg ) );

		if ( not _commands.Empty() )
		{
			if ( ++_framesWithoutSubmitting > 2 )
			{
				CHECK( _commands.RunAll( DelegateBuilder( this, &OpenGLThread::_SubmitQueue ) ) );
				_framesWithoutSubmitting = 0;
			}
		}
		else
			_framesWithoutSubmitting = 0;

		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool OpenGLThread::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyDevice();
		_DetachWindow();

		CHECK_ERR( Module::_Delete_Impl( msg ) );
		return true;
	}
	
/*
=================================================
	_AddToManager
=================================================
*/
	bool OpenGLThread::_AddToManager (const Message< ModuleMsg::AddToManager > &)
	{
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool OpenGLThread::_RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &)
	{
		return true;
	}
		
/*
=================================================
	_GetGraphicsModules
=================================================
*/	
	bool OpenGLThread::_GetGraphicsModules (const Message< GpuMsg::GetGraphicsModules > &msg)
	{
		msg->compute.Set( OpenGLObjectsConstructor::GetComputeModules() );
		msg->graphics.Set( OpenGLObjectsConstructor::GetGraphicsModules() );
		return true;
	}

/*
=================================================
	_ThreadBeginFrame
=================================================
*/
	bool OpenGLThread::_ThreadBeginFrame (const Message< GpuMsg::ThreadBeginFrame > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( _device.BeginFrame() );
		
		_context.MakeCurrent();

		msg->result.Set({ _device.GetCurrentFramebuffer(), _device.GetImageIndex() });
		return true;
	}

/*
=================================================
	_ThreadEndFrame
=================================================
*/
	bool OpenGLThread::_ThreadEndFrame (const Message< GpuMsg::ThreadEndFrame > &msg)
	{
		CHECK_ERR( _device.IsFrameStarted() );

		if ( msg->framebuffer )
			CHECK_ERR( msg->framebuffer == _device.GetCurrentFramebuffer() );

		_commands.Add( *msg );

		CHECK_ERR( _commands.RunAll( DelegateBuilder( this, &OpenGLThread::_SubmitQueue ) ) );
		
		CHECK_ERR( _device.EndFrame() );
		_context.SwapBuffers();

		return true;
	}

/*
=================================================
	_SubmitGraphicsQueueCommands
=================================================
*/
	bool OpenGLThread::_SubmitGraphicsQueueCommands (const Message< GpuMsg::SubmitGraphicsQueueCommands > &msg)
	{
		CHECK_ERR( _device.IsDeviceCreated() );
		
		_commands.Add( *msg );
		return true;
	}
	
/*
=================================================
	_SubmitComputeQueueCommands
=================================================
*/
	bool OpenGLThread::_SubmitComputeQueueCommands (const Message< GpuMsg::SubmitComputeQueueCommands > &msg)
	{
		CHECK_ERR( _device.IsDeviceCreated() );
		
		_commands.Add( *msg );
		return true;
	}
	
/*
=================================================
	_SyncGLClientWithDevice
=================================================
*/
	bool OpenGLThread::_SyncGLClientWithDevice (const Message< GpuMsg::SyncGLClientWithDevice > &msg)
	{
		CHECK_ERR( _commands.Run( msg->fenceId, DelegateBuilder( this, &OpenGLThread::_SubmitQueue ) ) );
		return true;
	}

/*
=================================================
	_WindowCreated
=================================================
*/
	bool OpenGLThread::_WindowCreated (const Message< OSMsg::WindowCreated > &)
	{
		CHECK_ERR( GetState() == EState::Linked );

		if ( _CreateDevice() ) {
			CHECK( _DefCompose( false ) );
		} else {
			CHECK( _SetState( EState::ComposingFailed ) );
		}
		return true;
	}
	
/*
=================================================
	_CreateDevice
=================================================
*/
	bool OpenGLThread::_CreateDevice ()
	{
		using namespace Engine::PlatformTools;

		Message< OSMsg::WindowGetDescriptor >	req_descr;
		SendTo( _window, req_descr );

		if ( _settings.colorFmt == EPixelFormat::Unknown )
			_settings.colorFmt = EPixelFormat::RGBA8_UNorm;

		CHECK_ERR( WindowHelper::GetWindowHandle( _window,
						LAMBDA( this ) (const WindowHelper::WinAPIWindow &data)
						{
							return _context.Create( data.window, INOUT _settings );
						}) );

		CHECK_ERR( _device.Initialize( req_descr->result->surfaceSize, _settings.colorFmt,
									   _settings.depthStencilFmt, _settings.samples ) );

		if ( _settings.flags[ GraphicsSettings::EFlags::DebugContext ] )
			CHECK( _device.InitDebugReport() );

		_SendEvent( Message< GpuMsg::DeviceCreated >{} );
		return true;
	}

/*
=================================================
	_WindowBeforeDestroy
=================================================
*/
	bool OpenGLThread::_WindowBeforeDestroy (const Message< OSMsg::WindowBeforeDestroy > &)
	{
		_DestroyDevice();
		_DetachWindow();
		
		if ( GetState() != EState::Deleting ) {
			CHECK( _SetState( EState::Initial ) );
		}
		return true;
	}
	
/*
=================================================
	_WindowBeforeDestroy
=================================================
*/
	void OpenGLThread::_DestroyDevice ()
	{
		if ( _device.IsDeviceCreated() )
		{
			_commands.RunAll( DelegateBuilder( this, &OpenGLThread::_SubmitQueue ) );

			_SendEvent( Message< GpuMsg::DeviceBeforeDestroy >{} );
		}

		if ( _syncManager )
		{
			_syncManager->Send< ModuleMsg::Delete >({});
			_syncManager = null;
		}

		_device.Deinitialize();
		_context.Destroy();
	}
	
/*
=================================================
	_WindowVisibilityChanged
=================================================
*/
	bool OpenGLThread::_WindowVisibilityChanged (const Message< OSMsg::WindowVisibilityChanged > &msg)
	{
		_isWindowVisible = (msg->state != WindowDesc::EVisibility::Invisible);
		return true;
	}
	
/*
=================================================
	_WindowDescriptorChanged
=================================================
*/
	bool OpenGLThread::_WindowDescriptorChanged (const Message< OSMsg::WindowDescriptorChanged > &msg)
	{
		if ( _device.IsDeviceCreated()									and
			 msg->desc.visibility != WindowDesc::EVisibility::Invisible )
		{
			CHECK_ERR( _device.OnResize( msg->desc.surfaceSize ) );
		}
		return true;
	}

/*
=================================================
	_WindowBeforeDestroy
=================================================
*/
	void OpenGLThread::_DetachWindow ()
	{
		if ( _window )
		{
			CHECK( not _device.IsDeviceCreated() );
			CHECK( not _context.IsCreated() );

			_window->UnsubscribeAll( this );
			_window = null;
		}
	}
	
/*
=================================================
	_GetDeviceInfo
=================================================
*/
	bool OpenGLThread::_GetDeviceInfo (const Message< GpuMsg::GetDeviceInfo > &msg)
	{
		msg->result.Set({
			this,
			null,
			_syncManager,
			GetDevice()->GetDefaultRenderPass(),
			GetDevice()->GetSwapchainLength()
		});
		return true;
	}
	
/*
=================================================
	_GetGLDeviceInfo
=================================================
*/
	bool OpenGLThread::_GetGLDeviceInfo (const Message< GpuMsg::GetGLDeviceInfo > &msg)
	{
		TODO("");
		return true;
	}
		
/*
=================================================
	_GetGLPrivateClasses
=================================================
*/
	bool OpenGLThread::_GetGLPrivateClasses (const Message< GpuMsg::GetGLPrivateClasses > &msg)
	{
		msg->result.Set({ &_device, &_samplerCache });
		return true;
	}
	
/*
=================================================
	_GetGraphicsSettings
=================================================
*/
	bool OpenGLThread::_GetGraphicsSettings (const Message< GpuMsg::GetGraphicsSettings > &msg)
	{
		msg->result.Set({ _settings, _device.GetSurfaceSize() });
		return true;
	}
	
/*
=================================================
	_GetComputeSettings
=================================================
*/
	bool OpenGLThread::_GetComputeSettings (const Message< GpuMsg::GetComputeSettings > &msg)
	{
		ComputeSettings	cs;
		cs.device	= _settings.device;
		cs.isDebug	= _settings.flags[ GraphicsSettings::EFlags::DebugContext ];
		cs.version	= _settings.version;

		msg->result.Set( RVREF(cs) );
		return true;
	}

/*
=================================================
	_SubmitQueue
=================================================
*/
	bool OpenGLThread::_SubmitQueue (const GpuMsg::SubmitGraphicsQueueCommands &cmd) const
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
//-----------------------------------------------------------------------------

	
/*
=================================================
	CreateOpenGLThread
=================================================
*/
	ModulePtr OpenGLObjectsConstructor::CreateOpenGLThread (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuThread &ci)
	{
		return New< OpenGLThread >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
