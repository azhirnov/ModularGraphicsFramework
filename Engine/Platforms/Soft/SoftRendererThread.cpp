// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/Tools/WindowHelper.h"
#include "Engine/Platforms/Soft/SoftRendererObjectsConstructor.h"
#include "Engine/Platforms/Soft/Impl/SWBaseModule.h"
#include "Engine/Platforms/Soft/Windows/SwWinSurface.h"

#ifdef GRAPHICS_API_SOFT

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
											GpuMsg::SubmitGraphicsQueueCommands,
											GpuMsg::SubmitComputeQueueCommands
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
											GpuMsg::GetDeviceInfo,
											GpuMsg::GetSWDeviceInfo,
											GpuMsg::GetSWPrivateClasses,
											GpuMsg::GetGraphicsSettings,
											GpuMsg::GetComputeSettings
										> >::Append< QueueMsgList_t >;

		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::ThreadBeginFrame,
											GpuMsg::ThreadEndFrame,
											GpuMsg::DeviceCreated,
											GpuMsg::DeviceBeforeDestroy
										> >::Append< QueueEventList_t >;

		using Surface_t				= PlatformSW::SWSurface;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;

		
	// variables
	private:
		GraphicsSettings	_settings;

		ModulePtr			_window;
		ModulePtr			_syncManager;
		ModulePtr			_cmdQueue;

		Surface_t			_surface;
		ModulePtr			_framebuffer;
		ModulePtr			_renderPass;
		uint2				_surfaceSize;

		bool				_isCreated;
		bool				_isFrameStarted;
		bool				_isWindowVisible;


	// methods
	public:
		SoftRendererThread (GlobalSystemsRef gs, const CreateInfo::GpuThread &ci);
		~SoftRendererThread ();


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
		bool _GetGraphicsSettings (const Message< GpuMsg::GetGraphicsSettings > &);
		bool _GetComputeSettings (const Message< GpuMsg::GetComputeSettings > &);

		bool _WindowCreated (const Message< OSMsg::WindowCreated > &);
		bool _WindowBeforeDestroy (const Message< OSMsg::WindowBeforeDestroy > &);
		bool _WindowVisibilityChanged (const Message< OSMsg::WindowVisibilityChanged > &);
		bool _WindowDescriptorChanged (const Message< OSMsg::WindowDescriptorChanged > &);
		
		bool _GetDeviceInfo (const Message< GpuMsg::GetDeviceInfo > &);
		bool _GetSWDeviceInfo (const Message< GpuMsg::GetSWDeviceInfo > &);
		bool _GetSWPrivateClasses (const Message< GpuMsg::GetSWPrivateClasses > &);

	private:
		bool _CreateDevice ();
		void _DestroyDevice ();
		void _DetachWindow ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	SoftRendererThread::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	SoftRendererThread::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SoftRendererThread::SoftRendererThread (GlobalSystemsRef gs, const CreateInfo::GpuThread &ci) :
		Module( gs, ModuleConfig{ SWThreadModuleID, 1 }, &_msgTypes, &_eventTypes ),
		_settings( ci.settings ),
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
		LOG( "SoftRendererThread finalized", ELog::Debug );

		ASSERT( _window.IsNull() );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool SoftRendererThread::_Link (const Message< ModuleMsg::Link > &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( GetState() == EState::Initial or GetState() == EState::LinkingFailed );
		
		CHECK_LINKING(( _window = PlatformTools::WindowHelper::FindWindow( GlobalSystems() ) ));

		_window->Subscribe( this, &SoftRendererThread::_WindowCreated );
		_window->Subscribe( this, &SoftRendererThread::_WindowBeforeDestroy );
		_window->Subscribe( this, &SoftRendererThread::_WindowVisibilityChanged );
		_window->Subscribe( this, &SoftRendererThread::_WindowDescriptorChanged );
		
		CHECK_ERR( Module::_Link_Impl( msg ) );

		// create sync manager
		{
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
											SWSyncManagerModuleID,
											GlobalSystems(),
											CreateInfo::GpuSyncManager{ this },
											OUT _syncManager ) );

			CHECK_ERR( _Attach( "sync", _syncManager, true ) );
		}

		// create queue
		{
			using EQueueFamily = CreateInfo::GpuCommandQueue::EQueueFamily;

			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
											SWCommandQueueModuleID,
											GlobalSystems(),
											CreateInfo::GpuCommandQueue{ this, EQueueFamily::All },
											OUT _cmdQueue ) );
			
			CHECK_ERR( _Attach( "queue", _cmdQueue, true ) );

			CHECK_ERR( _CopySubscriptions< QueueMsgList_t >( _cmdQueue ) );
			CHECK_ERR( ReceiveEvents< QueueEventList_t >( _cmdQueue ) );
		}

		_syncManager->Send( msg );
		_cmdQueue->Send( msg );

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
	bool SoftRendererThread::_Update (const Message< ModuleMsg::Update > &msg)
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
	bool SoftRendererThread::_Delete (const Message< ModuleMsg::Delete > &msg)
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
	bool SoftRendererThread::_AddToManager (const Message< ModuleMsg::AddToManager > &)
	{
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool SoftRendererThread::_RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &)
	{
		return true;
	}
		
/*
=================================================
	_GetGraphicsModules
=================================================
*/	
	bool SoftRendererThread::_GetGraphicsModules (const Message< GpuMsg::GetGraphicsModules > &msg)
	{
		msg->compute.Set( SoftRendererObjectsConstructor::GetComputeModules() );
		msg->graphics.Set( SoftRendererObjectsConstructor::GetGraphicsModules() );
		return true;
	}

/*
=================================================
	_ThreadBeginFrame
=================================================
*/
	bool SoftRendererThread::_ThreadBeginFrame (const Message< GpuMsg::ThreadBeginFrame > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( not _isFrameStarted );
		
		_isFrameStarted = true;

		msg->result.Set({ _framebuffer, 0u });
		return true;
	}

/*
=================================================
	_ThreadEndFrame
=================================================
*/
	bool SoftRendererThread::_ThreadEndFrame (const Message< GpuMsg::ThreadEndFrame > &msg)
	{
		CHECK_ERR( _isFrameStarted );

		if ( msg->framebuffer )
			CHECK_ERR( msg->framebuffer == _framebuffer );

		_isFrameStarted = false;

		_cmdQueue->Send< GpuMsg::SubmitGraphicsQueueCommands >({ *msg });

		_cmdQueue->Send< GpuMsg::SWPresent >({ LAMBDA(this) () {{ _surface.SwapBuffers(); }} });
		return true;
	}

/*
=================================================
	_WindowCreated
=================================================
*/
	bool SoftRendererThread::_WindowCreated (const Message< OSMsg::WindowCreated > &)
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
	bool SoftRendererThread::_CreateDevice ()
	{
		Message< OSMsg::WindowGetDescriptor >	req_descr;
		SendTo( _window, req_descr );
		
		// choose version
		switch ( _settings.version )
		{
			case "SW 1.0"_GAPI :
			case GAPI::type(0) :	break;
			default :				RETURN_ERR( "unsupported software renderer version" );
		}
		
		// create surface
		{
			using namespace Engine::PlatformTools;

			CHECK_ERR( WindowHelper::GetWindowHandle( _window,
							LAMBDA( this ) (const WindowHelper::WinAPIWindow &data)
							{
								return _surface.Create( data.window, OUT _settings );
							}) );
		}

		_isCreated = true;
		_SendEvent( Message< GpuMsg::DeviceCreated >{} );
		return true;
	}

/*
=================================================
	_WindowBeforeDestroy
=================================================
*/
	bool SoftRendererThread::_WindowBeforeDestroy (const Message< OSMsg::WindowBeforeDestroy > &)
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
		if ( _isCreated )
		{
			_isCreated = false;

			//_commands.RunAll( DelegateBuilder( this, &SoftRendererThread::_SubmitQueue ) );

			_SendEvent( Message< GpuMsg::DeviceBeforeDestroy >{} );
		}

		if ( _syncManager )
		{
			_syncManager->Send< ModuleMsg::Delete >({});
			_syncManager = null;
		}

		if ( _cmdQueue )
		{
			_cmdQueue->Send< ModuleMsg::Delete >({});
			_cmdQueue = null;
		}

		_surface.Destroy();
	}
	
/*
=================================================
	_WindowVisibilityChanged
=================================================
*/
	bool SoftRendererThread::_WindowVisibilityChanged (const Message< OSMsg::WindowVisibilityChanged > &msg)
	{
		_isWindowVisible = (msg->state != WindowDesc::EVisibility::Invisible);
		return true;
	}
	
/*
=================================================
	_WindowDescriptorChanged
=================================================
*/
	bool SoftRendererThread::_WindowDescriptorChanged (const Message< OSMsg::WindowDescriptorChanged > &msg)
	{
		if ( _isCreated		and
			 msg->desc.visibility != WindowDesc::EVisibility::Invisible )
		{
			_surfaceSize = msg->desc.surfaceSize;

			// TODO: resize framebuffer
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
			CHECK( not _isCreated );

			_window->UnsubscribeAll( this );
			_window = null;
		}
	}
	
/*
=================================================
	_GetDeviceInfo
=================================================
*/
	bool SoftRendererThread::_GetDeviceInfo (const Message< GpuMsg::GetDeviceInfo > &msg)
	{
		msg->result.Set({
			this,
			null,
			_syncManager,
			_renderPass,
			1
		});
		return true;
	}
	
/*
=================================================
	_GetSWDeviceInfo
=================================================
*/
	bool SoftRendererThread::_GetSWDeviceInfo (const Message< GpuMsg::GetSWDeviceInfo > &msg)
	{
		TODO("");
		return true;
	}
		
/*
=================================================
	_GetSWPrivateClasses
=================================================
*/
	bool SoftRendererThread::_GetSWPrivateClasses (const Message< GpuMsg::GetSWPrivateClasses > &msg)
	{
		//msg->result.Set({ &_device });
		return true;
	}
	
/*
=================================================
	_GetGraphicsSettings
=================================================
*/
	bool SoftRendererThread::_GetGraphicsSettings (const Message< GpuMsg::GetGraphicsSettings > &msg)
	{
		msg->result.Set({ _settings, _surfaceSize });
		return true;
	}
	
/*
=================================================
	_GetComputeSettings
=================================================
*/
	bool SoftRendererThread::_GetComputeSettings (const Message< GpuMsg::GetComputeSettings > &msg)
	{
		ComputeSettings	cs;
		cs.device	= _settings.device;
		cs.isDebug	= _settings.flags[ GraphicsSettings::EFlags::DebugContext ];
		cs.version	= _settings.version;

		msg->result.Set( RVREF(cs) );
		return true;
	}
//-----------------------------------------------------------------------------

	
/*
=================================================
	CreateSoftRendererThread
=================================================
*/
	ModulePtr SoftRendererObjectsConstructor::CreateSoftRendererThread (GlobalSystemsRef gs, const CreateInfo::GpuThread &ci)
	{
		return New< SoftRendererThread >( gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_SOFT
