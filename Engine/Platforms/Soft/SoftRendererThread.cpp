// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_SOFT

#include "Engine/Platforms/Public/Tools/WindowHelper.h"
#include "Engine/Platforms/Soft/SoftRendererObjectsConstructor.h"
#include "Engine/Platforms/Soft/Impl/SWBaseModule.h"
#include "Engine/Platforms/Soft/Impl/SWSamplerCache.h"
#include "Engine/Platforms/Soft/Windows/SwWinSurface.h"
#include "Engine/Platforms/Soft/Android/SwAndSurface.h"

namespace Engine
{
namespace Platforms
{

	//
	// Software Renderer Thread
	//
	
	class SoftRendererThread final : public Module
	{
	// types
	private:
		using QueueMsgList_t		= MessageListFrom<
											GpuMsg::SubmitCommands
										>;
		using QueueEventList_t		= MessageListFrom< GpuMsg::DeviceLost >;

		using SupportedMessages_t	= MessageListFrom<
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
											GpuMsg::GetSWDeviceInfo,
											GpuMsg::GetSWPrivateClasses,
											GpuMsg::GetDeviceProperties
										>;

		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::ThreadBeginFrame,
											GpuMsg::ThreadEndFrame,
											GpuMsg::DeviceCreated,
											GpuMsg::DeviceBeforeDestroy
										> >::Append< QueueEventList_t >;

		using Surface_t				= PlatformSW::SWSurface;
		using Device_t				= PlatformSW::SWDevice;
		using SamplerCache_t		= PlatformSW::SWSamplerCache;


	// constants
	private:
		static const TypeIdList		_eventTypes;

		static constexpr uint		_ver_major		= 0;
		static constexpr uint		_ver_minor		= 3;
		static const char			_renderer_name[];

		
	// variables
	private:
		Device_t			_device;
		GraphicsSettings	_settings;

		ModulePtr			_window;
		ModulePtr			_syncManager;
		ModulePtr			_cmdQueue;

		Surface_t			_surface;
		ModulePtr			_framebuffer;
		ModulePtr			_renderPass;

		SamplerCache_t		_samplerCache;

		bool				_isFrameStarted;
		bool				_isWindowVisible;


	// methods
	public:
		SoftRendererThread (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuThread &ci);
		~SoftRendererThread ();


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
		bool _GetSWDeviceInfo (const GpuMsg::GetSWDeviceInfo &);
		bool _GetSWPrivateClasses (const GpuMsg::GetSWPrivateClasses &);
		bool _GetDeviceProperties (const GpuMsg::GetDeviceProperties &);

	private:
		bool _CreateDevice ();
		void _DestroyDevice ();
		void _WriteDeviceInfo ();

		void _DetachWindow ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	SoftRendererThread::_eventTypes{ UninitializedT< SupportedEvents_t >() };
	
	const char			SoftRendererThread::_renderer_name[]	= "gx-soft";

/*
=================================================
	constructor
=================================================
*/
	SoftRendererThread::SoftRendererThread (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuThread &ci) :
		Module( gs, ModuleConfig{ id, 1 }, &_eventTypes ),
		_device{ gs },				_settings{ ci.settings },
		_isFrameStarted{ false },	_isWindowVisible{ false }
	{
		SetDebugName( "SoftRendererThread" );
		
		_SubscribeOnMsg( this, &SoftRendererThread::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &SoftRendererThread::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &SoftRendererThread::_AttachModule_Impl );
		_SubscribeOnMsg( this, &SoftRendererThread::_DetachModule_Impl );
		_SubscribeOnMsg( this, &SoftRendererThread::_AddToManager );
		_SubscribeOnMsg( this, &SoftRendererThread::_RemoveFromManager );
		_SubscribeOnMsg( this, &SoftRendererThread::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &SoftRendererThread::_FindModule_Impl );
		_SubscribeOnMsg( this, &SoftRendererThread::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &SoftRendererThread::_Update );
		_SubscribeOnMsg( this, &SoftRendererThread::_Link );
		_SubscribeOnMsg( this, &SoftRendererThread::_Delete );
		_SubscribeOnMsg( this, &SoftRendererThread::_GetGraphicsModules );
		_SubscribeOnMsg( this, &SoftRendererThread::_ThreadBeginFrame );
		_SubscribeOnMsg( this, &SoftRendererThread::_ThreadEndFrame );
		_SubscribeOnMsg( this, &SoftRendererThread::_WindowCreated );
		_SubscribeOnMsg( this, &SoftRendererThread::_WindowBeforeDestroy );
		_SubscribeOnMsg( this, &SoftRendererThread::_GetDeviceInfo );
		_SubscribeOnMsg( this, &SoftRendererThread::_GetSWDeviceInfo );
		_SubscribeOnMsg( this, &SoftRendererThread::_GetSWPrivateClasses );
		_SubscribeOnMsg( this, &SoftRendererThread::_GetGraphicsSettings );
		_SubscribeOnMsg( this, &SoftRendererThread::_GetComputeSettings );
		_SubscribeOnMsg( this, &SoftRendererThread::_GetDeviceProperties );
		
		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

		CHECK( ci.shared.IsNull() );	// sharing is not supported yet

		_AttachSelfToManager( ci.context, SWContextModuleID, false );
	}
	
/*
=================================================
	destructor
=================================================
*/
	SoftRendererThread::~SoftRendererThread ()
	{
		//LOG( "SoftRendererThread finalized", ELog::Debug );

		ASSERT( _window.IsNull() );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool SoftRendererThread::_Link (const ModuleMsg::Link &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( _IsInitialState( GetState() ) );
		
		CHECK_LINKING(( _window = PlatformTools::WindowHelper::FindWindow( GlobalSystems() ) ));

		_window->Subscribe( this, &SoftRendererThread::_WindowCreated );
		_window->Subscribe( this, &SoftRendererThread::_WindowBeforeDestroy );
		_window->Subscribe( this, &SoftRendererThread::_WindowVisibilityChanged );
		_window->Subscribe( this, &SoftRendererThread::_WindowDescriptionChanged );
		
		CHECK_ERR( Module::_Link_Impl( msg ) );

		// create sync manager
		{
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
											SWSyncManagerModuleID,
											GlobalSystems(),
											CreateInfo::GpuSyncManager{ this },
											OUT _syncManager ) );

			CHECK_ERR( _Attach( "sync", _syncManager ) );
		}

		// create queue
		{
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
											SWCommandQueueModuleID,
											GlobalSystems(),
											CreateInfo::GpuCommandQueue{ this, { EQueueFamily::Default, 1.0f } },
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
			Send( OSMsg::WindowCreated{} );
		}
		return true;
	}
	
/*
=================================================
	_Update
=================================================
*/
	bool SoftRendererThread::_Update (const ModuleMsg::Update &msg)
	{
		if ( not _IsComposedState( GetState() ) or
			 not _isWindowVisible )
		{
			return true;
		}

		CHECK_ERR( Module::_Update_Impl( msg ) );


		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool SoftRendererThread::_Delete (const ModuleMsg::Delete &msg)
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
	bool SoftRendererThread::_AddToManager (const ModuleMsg::AddToManager &)
	{
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool SoftRendererThread::_RemoveFromManager (const ModuleMsg::RemoveFromManager &)
	{
		return true;
	}
		
/*
=================================================
	_GetGraphicsModules
=================================================
*/	
	bool SoftRendererThread::_GetGraphicsModules (const GpuMsg::GetGraphicsModules &msg)
	{
		msg.result.Set({ SoftRendererObjectsConstructor::GetComputeModules(),
						 SoftRendererObjectsConstructor::GetGraphicsModules() });
		return true;
	}

/*
=================================================
	_ThreadBeginFrame
=================================================
*/
	bool SoftRendererThread::_ThreadBeginFrame (const GpuMsg::ThreadBeginFrame &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( not _isFrameStarted );
		
		_isFrameStarted = true;

		msg.result.Set({ _framebuffer, null, 0u });		// TODO
		return true;
	}

/*
=================================================
	_ThreadEndFrame
=================================================
*/
	bool SoftRendererThread::_ThreadEndFrame (const GpuMsg::ThreadEndFrame &msg)
	{
		CHECK_ERR( _isFrameStarted );

		if ( msg.framebuffer )
			CHECK_ERR( msg.framebuffer == _framebuffer );

		_isFrameStarted = false;

		_cmdQueue->Send( Cast<GpuMsg::SubmitCommands const &>( msg ));

		_cmdQueue->Send( GpuMsg::SWPresent{ LAMBDA(this) () {{ _surface.SwapBuffers(); }} });
		return true;
	}

/*
=================================================
	_WindowCreated
=================================================
*/
	bool SoftRendererThread::_WindowCreated (const OSMsg::WindowCreated &)
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
	bool SoftRendererThread::_CreateDevice ()
	{
		// choose version
		switch ( _settings.version )
		{
			case "SW 1.0"_GAPI :
			case GAPI::type(0) :	break;
			default :				RETURN_ERR( "unsupported software renderer version" );
		}

		_device.Initialize();

		if ( _settings.flags[ GraphicsSettings::EFlags::DebugContext ] )
		{
			//_device.InitDebugReport();
		}
		
		// create surface
		{
			using namespace Engine::PlatformTools;

			CHECK_ERR( WindowHelper::GetWindowHandle( _window,
							LAMBDA( this ) (const auto &data)
							{
								return _surface.Create( data.window, OUT _settings );
							}) );
		}

		_cmdQueue->Send( ModuleMsg::Compose{} );

		_WriteDeviceInfo();
		
		CHECK( _DefCompose( false ) );

		_SendEvent( GpuMsg::DeviceCreated{} );
		return true;
	}
	
/*
=================================================
	_WriteDeviceInfo
=================================================
*/
	void SoftRendererThread::_WriteDeviceInfo ()
	{
		String	log;

		log << "Software renderer info\n---------------"
			<< "\nRenderer:     " << _renderer_name
			<< "\nVersion:      " << _ver_major << '.' << _ver_minor
			<< "\n---------------";

		LOG( log, ELog::Debug | ELog::SpoilerFlag );
	}

/*
=================================================
	_WindowBeforeDestroy
=================================================
*/
	bool SoftRendererThread::_WindowBeforeDestroy (const OSMsg::WindowBeforeDestroy &)
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
	void SoftRendererThread::_DestroyDevice ()
	{
		if ( _device.IsDeviceCreated() )
		{
			_device.Deinitialize();

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

		_surface.Destroy();
	}
	
/*
=================================================
	_WindowVisibilityChanged
=================================================
*/
	bool SoftRendererThread::_WindowVisibilityChanged (const OSMsg::WindowVisibilityChanged &msg)
	{
		_isWindowVisible = (msg.state != WindowDesc::EVisibility::Invisible);
		return true;
	}
	
/*
=================================================
	_WindowDescriptionChanged
=================================================
*/
	bool SoftRendererThread::_WindowDescriptionChanged (const OSMsg::WindowDescriptionChanged &msg)
	{
		if ( _device.IsDeviceCreated()		and
			 msg.descr.visibility != WindowDesc::EVisibility::Invisible )
		{
			_device.Resize( msg.descr.surfaceSize );
		}
		return true;
	}

/*
=================================================
	_WindowBeforeDestroy
=================================================
*/
	void SoftRendererThread::_DetachWindow ()
	{
		if ( _window )
		{
			CHECK( not _device.IsDeviceCreated() );

			_window->UnsubscribeAll( this );
			_window = null;
		}
	}
	
/*
=================================================
	_GetDeviceInfo
=================================================
*/
	bool SoftRendererThread::_GetDeviceInfo (const GpuMsg::GetDeviceInfo &msg)
	{
		GpuMsg::GetDeviceInfo::Info	info;
		info.gpuThread		= this;
		info.sharedThread	= null;
		info.syncManager	= _syncManager;
		info.memManager		= null;
		info.renderPass		= _renderPass;
		info.imageCount		= 2;

		msg.result.Set( info );
		return true;
	}
	
/*
=================================================
	_GetSWDeviceInfo
=================================================
*/
	bool SoftRendererThread::_GetSWDeviceInfo (const GpuMsg::GetSWDeviceInfo &)
	{
		TODO("");
		return true;
	}
		
/*
=================================================
	_GetSWPrivateClasses
=================================================
*/
	bool SoftRendererThread::_GetSWPrivateClasses (const GpuMsg::GetSWPrivateClasses &msg)
	{
		msg.result.Set({ &_device, &_samplerCache });
		return true;
	}
	
/*
=================================================
	_GetGraphicsSettings
=================================================
*/
	bool SoftRendererThread::_GetGraphicsSettings (const GpuMsg::GetGraphicsSettings &msg)
	{
		msg.result.Set({ _settings, _device.SurfaceSize() });
		return true;
	}
	
/*
=================================================
	_GetComputeSettings
=================================================
*/
	bool SoftRendererThread::_GetComputeSettings (const GpuMsg::GetComputeSettings &msg)
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
	bool SoftRendererThread::_GetDeviceProperties (const GpuMsg::GetDeviceProperties &msg)
	{
		msg.result.Set( _device.GetProperties() );
		return true;
	}
//-----------------------------------------------------------------------------

	
/*
=================================================
	CreateSoftRendererThread
=================================================
*/
	ModulePtr SoftRendererObjectsConstructor::CreateSoftRendererThread (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuThread &ci)
	{
		return New< SoftRendererThread >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_SOFT
