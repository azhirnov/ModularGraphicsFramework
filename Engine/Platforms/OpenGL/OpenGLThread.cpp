// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_OPENGL

#include "Engine/Platforms/Public/Tools/WindowHelper.h"
#include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"
#include "Engine/Platforms/OpenGL/450/GL4BaseModule.h"
#include "Engine/Platforms/OpenGL/450/GL4SamplerCache.h"
#include "Engine/Platforms/OpenGL/450/GL4ResourceCache.h"
#include "Engine/Platforms/OpenGL/Windows/GLWinContext.h"

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
		using QueueMsgList_t		= MessageListFrom<
											GpuMsg::SubmitGraphicsQueueCommands,
											GpuMsg::SubmitComputeQueueCommands,
											GpuMsg::SyncGLClientWithDevice
										>;
		using QueueEventList_t		= MessageListFrom< GpuMsg::DeviceLost >;

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
											GpuMsg::GetGraphicsSettings,
											GpuMsg::GetComputeSettings,
											GpuMsg::GetDeviceInfo,
											GpuMsg::GetGLDeviceInfo,
											GpuMsg::GetGLPrivateClasses,
											GpuMsg::GetDeviceProperties
										> >::Append< QueueMsgList_t >;

		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::ThreadBeginFrame,
											GpuMsg::ThreadEndFrame,
											GpuMsg::DeviceCreated,
											GpuMsg::DeviceBeforeDestroy
										> >::Append< QueueEventList_t >;

		using Context				= PlatformGL::GLRenderingContext;
		using Device				= PlatformGL::GL4Device;
		using SamplerCache			= PlatformGL::GL4SamplerCache;
		using ResourceCache			= PlatformGL::GL4ResourceCache;


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
		ModulePtr			_cmdQueue;
		
		SamplerCache		_samplerCache;
		ResourceCache		_resourceCache;

		bool				_isWindowVisible;


	// methods
	public:
		OpenGLThread (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuThread &ci);
		~OpenGLThread ();
		
		Ptr< Device >	GetDevice ()		{ return &_device; }


	// message handlers
	private:
		bool _Delete (const ModuleMsg::Delete &);
		bool _Link (const ModuleMsg::Link &);
		bool _Update (const ModuleMsg::Update &);
		bool _AddToManager (const ModuleMsg::AddToManager &);
		bool _RemoveFromManager (const ModuleMsg::RemoveFromManager &);
		bool _GetGraphicsModules (const GpuMsg::GetGraphicsModules &);
		
		bool _ThreadBeginFrame (const GpuMsg::ThreadBeginFrame &);
		bool _ThreadEndFrame (const GpuMsg::ThreadEndFrame &);
		bool _GetGraphicsSettings (const GpuMsg::GetGraphicsSettings &);
		bool _GetComputeSettings (const GpuMsg::GetComputeSettings &);

		bool _WindowCreated (const OSMsg::WindowCreated &);
		bool _WindowBeforeDestroy (const OSMsg::WindowBeforeDestroy &);
		bool _WindowVisibilityChanged (const OSMsg::WindowVisibilityChanged &);
		bool _WindowDescriptionChanged (const OSMsg::WindowDescriptionChanged &);
		
		bool _GetDeviceInfo (const GpuMsg::GetDeviceInfo &);
		bool _GetGLDeviceInfo (const GpuMsg::GetGLDeviceInfo &);
		bool _GetGLPrivateClasses (const GpuMsg::GetGLPrivateClasses &);
		bool _GetDeviceProperties (const GpuMsg::GetDeviceProperties &);

	private:
		bool _CreateDevice ();
		void _DestroyDevice ();
		void _DetachWindow ();
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
		_SubscribeOnMsg( this, &OpenGLThread::_WindowCreated );
		_SubscribeOnMsg( this, &OpenGLThread::_WindowBeforeDestroy );
		_SubscribeOnMsg( this, &OpenGLThread::_GetDeviceInfo );
		_SubscribeOnMsg( this, &OpenGLThread::_GetGLDeviceInfo );
		_SubscribeOnMsg( this, &OpenGLThread::_GetGLPrivateClasses );
		_SubscribeOnMsg( this, &OpenGLThread::_GetGraphicsSettings );
		_SubscribeOnMsg( this, &OpenGLThread::_GetComputeSettings );
		_SubscribeOnMsg( this, &OpenGLThread::_GetDeviceProperties );

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
	bool OpenGLThread::_Link (const ModuleMsg::Link &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( _IsInitialState( GetState() ) );

		CHECK_LINKING(( _window = PlatformTools::WindowHelper::FindWindow( GlobalSystems() ) ));

		_window->Subscribe( this, &OpenGLThread::_WindowCreated );
		_window->Subscribe( this, &OpenGLThread::_WindowBeforeDestroy );
		_window->Subscribe( this, &OpenGLThread::_WindowVisibilityChanged );
		_window->Subscribe( this, &OpenGLThread::_WindowDescriptionChanged );
		
		CHECK_ERR( Module::_Link_Impl( msg ) );

		// create sync manager
		{
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
											GLSyncManagerModuleID,
											GlobalSystems(),
											CreateInfo::GpuSyncManager{ this },
											OUT _syncManager ) );

			CHECK_ERR( _Attach( "sync", _syncManager ) );
		}

		// create queue
		{
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
											GLCommandQueueModuleID,
											GlobalSystems(),
											CreateInfo::GpuCommandQueue{ this, EQueueFamily::Default },
											OUT _cmdQueue ) );
			
			CHECK_ERR( _Attach( "queue", _cmdQueue ) );

			CHECK_ERR( _CopySubscriptions< QueueMsgList_t >( _cmdQueue ) );
			CHECK_ERR( ReceiveEvents< QueueEventList_t >( _cmdQueue ) );
		}

		_syncManager->Send( msg );
		_cmdQueue->Send( msg );

		// if window already created
		if ( _IsComposedState( _window->GetState() ) )
		{
			_SendMsg( OSMsg::WindowCreated{} );
		}
		return true;
	}
	
/*
=================================================
	_Update
=================================================
*/
	bool OpenGLThread::_Update (const ModuleMsg::Update &msg)
	{
		if ( not _IsComposedState( GetState() ) or
			 not _isWindowVisible )
		{
			return true;
		}

		_context.MakeCurrent();

		CHECK_ERR( Module::_Update_Impl( msg ) );

		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool OpenGLThread::_Delete (const ModuleMsg::Delete &msg)
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
	bool OpenGLThread::_AddToManager (const ModuleMsg::AddToManager &)
	{
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool OpenGLThread::_RemoveFromManager (const ModuleMsg::RemoveFromManager &)
	{
		return true;
	}
		
/*
=================================================
	_GetGraphicsModules
=================================================
*/	
	bool OpenGLThread::_GetGraphicsModules (const GpuMsg::GetGraphicsModules &msg)
	{
		msg.compute.Set( OpenGLObjectsConstructor::GetComputeModules() );
		msg.graphics.Set( OpenGLObjectsConstructor::GetGraphicsModules() );
		return true;
	}

/*
=================================================
	_ThreadBeginFrame
=================================================
*/
	bool OpenGLThread::_ThreadBeginFrame (const GpuMsg::ThreadBeginFrame &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( _device.BeginFrame() );
		
		_context.MakeCurrent();

		msg.result.Set({ _device.GetCurrentFramebuffer(), _device.GetImageIndex() });
		return true;
	}

/*
=================================================
	_ThreadEndFrame
=================================================
*/
	bool OpenGLThread::_ThreadEndFrame (const GpuMsg::ThreadEndFrame &msg)
	{
		CHECK_ERR( _device.IsFrameStarted() );

		if ( msg.framebuffer )
			CHECK_ERR( msg.framebuffer == _device.GetCurrentFramebuffer() );

		_cmdQueue->Send( msg._Cast<GpuMsg::SubmitGraphicsQueueCommands>() );
		_cmdQueue->Send( GpuMsg::GLFlushQueue{} );
		
		CHECK_ERR( _device.EndFrame() );
		_context.SwapBuffers();

		return true;
	}

/*
=================================================
	_WindowCreated
=================================================
*/
	bool OpenGLThread::_WindowCreated (const OSMsg::WindowCreated &)
	{
		CHECK_ERR( GetState() == EState::Linked );
		
		CHECK_COMPOSING( _CreateDevice() );
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

		OSMsg::WindowGetDescription	req_descr;
		_window->Send( req_descr );

		if ( _settings.colorFmt == EPixelFormat::Unknown )
			_settings.colorFmt = EPixelFormat::RGBA8_UNorm;

		CHECK_ERR( WindowHelper::GetWindowHandle( _window,
						LAMBDA( this ) (const WindowHelper::WinAPIWindow &data)
						{
							return _context.Create( data.window, INOUT _settings );
						}) );

		CHECK_ERR( _device.Initialize( req_descr.result->surfaceSize, _settings.colorFmt,
									   _settings.depthStencilFmt, _settings.samples ) );

		if ( _settings.flags[ GraphicsSettings::EFlags::DebugContext ] )
			CHECK( _device.InitDebugReport() );
		
		_device.WriteInfo();

		CHECK( _DefCompose( false ) );

		_SendEvent( GpuMsg::DeviceCreated{} );
		return true;
	}

/*
=================================================
	_WindowBeforeDestroy
=================================================
*/
	bool OpenGLThread::_WindowBeforeDestroy (const OSMsg::WindowBeforeDestroy &)
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
			_SendEvent( GpuMsg::DeviceBeforeDestroy{} );
		}

		if ( _cmdQueue )
		{
			_cmdQueue->Send( ModuleMsg::Delete{} );
			_cmdQueue = null;
		}

		if ( _syncManager )
		{
			_syncManager->Send( ModuleMsg::Delete{} );
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
	bool OpenGLThread::_WindowVisibilityChanged (const OSMsg::WindowVisibilityChanged &msg)
	{
		_isWindowVisible = (msg.state != WindowDesc::EVisibility::Invisible);
		return true;
	}
	
/*
=================================================
	_WindowDescriptionChanged
=================================================
*/
	bool OpenGLThread::_WindowDescriptionChanged (const OSMsg::WindowDescriptionChanged &msg)
	{
		if ( _device.IsDeviceCreated()									and
			 msg.descr.visibility != WindowDesc::EVisibility::Invisible )
		{
			CHECK_ERR( _device.OnResize( msg.descr.surfaceSize ) );
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
	bool OpenGLThread::_GetDeviceInfo (const GpuMsg::GetDeviceInfo &msg)
	{
		GpuMsg::GetDeviceInfo::Info	info;
		info.gpuThread		= this;
		info.sharedThread	= null;
		info.syncManager	= _syncManager;
		info.memManager		= null;
		info.renderPass		= GetDevice()->GetDefaultRenderPass();
		info.imageCount		= GetDevice()->GetSwapchainLength();

		msg.result.Set( info );
		return true;
	}
	
/*
=================================================
	_GetGLDeviceInfo
=================================================
*/
	bool OpenGLThread::_GetGLDeviceInfo (const GpuMsg::GetGLDeviceInfo &)
	{
		TODO("");
		return true;
	}
		
/*
=================================================
	_GetGLPrivateClasses
=================================================
*/
	bool OpenGLThread::_GetGLPrivateClasses (const GpuMsg::GetGLPrivateClasses &msg)
	{
		msg.result.Set({
			&_device,
			&_samplerCache,
			&_resourceCache
		});
		return true;
	}
	
/*
=================================================
	_GetGraphicsSettings
=================================================
*/
	bool OpenGLThread::_GetGraphicsSettings (const GpuMsg::GetGraphicsSettings &msg)
	{
		msg.result.Set({ _settings, _device.GetSurfaceSize() });
		return true;
	}
	
/*
=================================================
	_GetComputeSettings
=================================================
*/
	bool OpenGLThread::_GetComputeSettings (const GpuMsg::GetComputeSettings &msg)
	{
		ComputeSettings	cs;
		cs.device	= _settings.device;
		cs.isDebug	= _settings.flags[ GraphicsSettings::EFlags::DebugContext ];
		cs.version	= _settings.version;

		msg.result.Set( RVREF(cs) );
		return true;
	}
	
/*
=================================================
	_GetDeviceProperties
=================================================
*/
	bool OpenGLThread::_GetDeviceProperties (const GpuMsg::GetDeviceProperties &msg)
	{
		msg.result.Set( _device.GetProperties() );
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
