// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/OpenGL/OpenGLThread.h"
#include "Engine/Platforms/OpenGL/OpenGLContext.h"
#include "Engine/Platforms/Windows/WinMessages.h"

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
		Module( gs, ModuleConfig{ GLThreadModuleID, 1 }, &_msgTypes, &_eventTypes ),
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

		_AttachSelfToManager( ci.context, GLContextModuleID, true );
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

		// TODO: use SearchModule message
		// TODO: reset to initial state if window was detached
		_window = _GetParents().Front()->GetModuleByID( WinWindowModuleID );
		CHECK_ATTACHMENT( _window );

		_window->Subscribe( this, &OpenGLThread::_WindowCreated );
		_window->Subscribe( this, &OpenGLThread::_WindowBeforeDestroy );
		_window->Subscribe( this, &OpenGLThread::_WindowDescriptorChanged );
		
		CHECK_ERR( Module::_Link_Impl( msg ) );

		if ( _IsComposedState( _window->GetState() ) )
		{
			Message< ModuleMsg::WindowGetHandle >	request_hwnd;

			SendTo( _window, request_hwnd );

			if ( request_hwnd->hwnd.Get().IsDefined() )
			{
				_SendMsg< ModuleMsg::WindowCreated >({ WindowDesc(), request_hwnd->hwnd.Get() });
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
			CHECK( _DefCompose( true ) );
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

		_SendEvent( Message< GpuMsg::DeviceCreated >{} );
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
			_SendEvent( Message< GpuMsg::DeviceBeforeDestroy >{} );
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
	
/*
=================================================
	_IsWindow
=================================================
*
	bool OpenGLThread::_IsWindow (const ModulePtr &mod)
	{
		using WindowEventList_t		= MessageListFrom< ModuleMsg::WindowCreated, ModuleMsg::WindowBeforeDestroy >;
		using WindowMsgList_t		= MessageListFrom< ModuleMsg::WindowGetHandle >;

		return	mod->GetSupportedEvents().HasAllTypes< WindowEventList_t >() and
				mod->GetSupportedMessages().HasAllTypes< WindowMsgList_t >();
	}*/


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
