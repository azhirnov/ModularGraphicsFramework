// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/OpenGL/OpenGLThread.h"
#include "Engine/Platforms/OpenGL/OpenGLContext.h"
#include "Engine/Platforms/Windows/WinWindow.h"

#if defined( GRAPHICS_API_OPENGL )

namespace Engine
{
namespace Platforms
{
	
	const Runtime::VirtualTypeList	OpenGLThread::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	OpenGLThread::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	OpenGLThread::OpenGLThread (const GlobalSystemsRef gs, const CreateInfo::GpuThread &ci) :
		Module( gs, ModuleConfig{ GetStaticID(), 1 }, &_msgTypes, &_eventTypes ),
		_settings( ci.settings )
	{
		SetDebugName( "OpenGLThread" );

		_SubscribeOnMsg( this, &OpenGLThread::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &OpenGLThread::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &OpenGLThread::_AttachModule_Impl );
		_SubscribeOnMsg( this, &OpenGLThread::_DetachModule_Impl );
		_SubscribeOnMsg( this, &OpenGLThread::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &OpenGLThread::_FindModule_Impl );
		_SubscribeOnMsg( this, &OpenGLThread::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &OpenGLThread::_Update );
		_SubscribeOnMsg( this, &OpenGLThread::_Link );
		_SubscribeOnMsg( this, &OpenGLThread::_Delete );
		_SubscribeOnMsg( this, &OpenGLThread::_WindowCreated );
		_SubscribeOnMsg( this, &OpenGLThread::_WindowBeforeDestroy );
		_SubscribeOnMsg( this, &OpenGLThread::_WindowDescriptorChanged );
		
		CHECK( _ValidateMsgSubscriptions() );

		CHECK( ci.shared.IsNull() );	// sharing is not supported yet

		_AttachSelfToManager( ci.context, OpenGLContext::GetStaticID(), true );
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
		CHECK_ERR( _IsMutableState( GetState() ) );

		_window = _GetParents().Front()->GetModule( WinWindow::GetStaticID() );
		CHECK_ERR( _window );

		_window->Subscribe( this, &OpenGLThread::_WindowCreated );
		_window->Subscribe( this, &OpenGLThread::_WindowBeforeDestroy );
		
		CHECK_ERR( Module::_Link_Impl( msg ) );

		if ( _IsComposedState( _window->GetState() ) )
		{
			Message< ModuleMsg::WindowGetHandle >	request_hwnd;

			_window->Send( request_hwnd );

			if ( request_hwnd->hwnd.Get().IsDefined() )
			{
				_SendMsg( Message< ModuleMsg::WindowCreated >{ this, WindowDesc(), request_hwnd->hwnd.Get() } );
			}
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
		_context.MakeCurrent();

		CHECK_ERR( Module::_Update_Impl( msg ) );

		_context.SwapBuffers();
		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool OpenGLThread::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DetachWindow();

		CHECK_ERR( Module::_Delete_Impl( msg ) );
		return true;
	}
	
/*
=================================================
	_WindowCreated
=================================================
*/
	bool OpenGLThread::_WindowCreated (const Message< ModuleMsg::WindowCreated > &msg)
	{
		if ( _CreateDevice( *msg ) )
		{
			CHECK( _Compose( true ) );
		}
		else
		{
			CHECK( _SetState( EState::ComposingFailed ) );
		}
		return true;
	}
	
/*
=================================================
	_CreateDevice
=================================================
*/
	bool OpenGLThread::_CreateDevice (const ModuleMsg::WindowCreated &msg)
	{
		CHECK_ERR( _context.Create( msg.hwnd, INOUT _settings ) );

		CHECK_ERR( _device.Initialize() );

		_SendEvent( Message< ModuleMsg::GpuDeviceCreated >{ this } );
		return true;
	}

/*
=================================================
	_WindowBeforeDestroy
=================================================
*/
	bool OpenGLThread::_WindowBeforeDestroy (const Message< ModuleMsg::WindowBeforeDestroy > &msg)
	{
		if ( _device.IsInitialized() )
		{
			_SendEvent( Message< ModuleMsg::GpuDeviceBeforeDestory >{ this } );
		}

		_device.Deinitialize();
		_context.Destroy();
		_DetachWindow();
		
		CHECK( _SetState( EState::Initial ) );
		return true;
	}
	
/*
=================================================
	_WindowDescriptorChanged
=================================================
*/
	bool OpenGLThread::_WindowDescriptorChanged (const Message< ModuleMsg::WindowDescriptorChanged > &msg)
	{
		// TODO
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
			CHECK( not _device.IsInitialized() );
			CHECK( not _context.IsCreated() );

			_window->UnsubscribeAll( this );
			_window = null;
		}
	}


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
