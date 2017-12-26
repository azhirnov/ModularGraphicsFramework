// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Windows/WinMessages.h"
#include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"
#include "Engine/Platforms/OpenGL/Impl/GL4BaseModule.h"
#include "Engine/Platforms/OpenGL/Windows/GLWinContext.h"

#if defined( GRAPHICS_API_OPENGL )

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
											GpuMsg::GetGraphicsSettings
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::DeviceCreated,
											GpuMsg::DeviceBeforeDestroy
											// TODO: device lost event
										> >;

		using WindowMsgList_t		= MessageListFrom< OSMsg::GetWinWindowHandle >;
		using WindowEventList_t		= MessageListFrom< OSMsg::WindowCreated, OSMsg::WindowBeforeDestroy, OSMsg::OnWinWindowRawMessage >;

		using GLContext				= PlatformGL::GLRenderingContext;
		using GLDevice				= PlatformGL::GL4Device;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;

		
	// variables
	private:
		GraphicsSettings	_settings;

		ModulePtr			_window;
		
		GLContext			_context;
		GLDevice			_device;
	
		bool				_isWindowVisible;


	// methods
	public:
		OpenGLThread (GlobalSystemsRef gs, const CreateInfo::GpuThread &ci);
		~OpenGLThread ();
		
		Ptr< GLDevice >			GetDevice ()			{ return &_device; }


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

		bool _WindowCreated (const Message< OSMsg::WindowCreated > &);
		bool _WindowBeforeDestroy (const Message< OSMsg::WindowBeforeDestroy > &);
		bool _WindowVisibilityChanged (const Message< OSMsg::WindowVisibilityChanged > &);
		bool _WindowDescriptorChanged (const Message< OSMsg::WindowDescriptorChanged > &);
		
		bool _GetDeviceInfo (const Message< GpuMsg::GetDeviceInfo > &);
		bool _GetGLDeviceInfo (const Message< GpuMsg::GetGLDeviceInfo > &);
		bool _GetGLPrivateClasses (const Message< GpuMsg::GetGLPrivateClasses > &);

	private:
		bool _CreateDevice ();
		void _DetachWindow ();

		static bool _IsWindow (const ModulePtr &mod);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	OpenGLThread::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	OpenGLThread::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	OpenGLThread::OpenGLThread (GlobalSystemsRef gs, const CreateInfo::GpuThread &ci) :
		Module( gs, ModuleConfig{ GLThreadModuleID, 1 }, &_msgTypes, &_eventTypes ),
		_settings( ci.settings ),
		_device( gs ),
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

		// TODO: use SearchModule message
		CHECK_ATTACHMENT(( _window = GlobalSystems()->parallelThread->GetModuleByMsgEvent< WindowMsgList_t, WindowEventList_t >() ));

		_window->Subscribe( this, &OpenGLThread::_WindowCreated );
		_window->Subscribe( this, &OpenGLThread::_WindowBeforeDestroy );
		_window->Subscribe( this, &OpenGLThread::_WindowVisibilityChanged );
		_window->Subscribe( this, &OpenGLThread::_WindowDescriptorChanged );
		
		CHECK_ERR( Module::_Link_Impl( msg ) );

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
		
		if ( msg->commands )
		{
			Message< GpuMsg::GetCommandBufferDescriptor >	req_descr;
			msg->commands->Send( req_descr );
			CHECK_ERR( req_descr->result and not req_descr->result->flags[ ECmdBufferCreate::Secondary ] );
			
			msg->commands->Send< GpuMsg::SetCommandBufferState >({ GpuMsg::SetCommandBufferState::EState::Pending });

			CHECK_ERR( _device.EndFrame() );

			_context.SwapBuffers();
			
			if ( not req_descr->result->flags[ ECmdBufferCreate::UseFence ] ) {
				msg->commands->Send< GpuMsg::SetCommandBufferState >({ GpuMsg::SetCommandBufferState::EState::Completed });
			}
		}
		else
		{
			CHECK_ERR( _device.EndFrame() );

			_context.SwapBuffers();
		}
		return true;
	}

/*
=================================================
	_SubmitGraphicsQueueCommands
=================================================
*/
	bool OpenGLThread::_SubmitGraphicsQueueCommands (const Message< GpuMsg::SubmitGraphicsQueueCommands > &msg)
	{
		using namespace gl;

		CHECK_ERR( _device.IsDeviceCreated() );

		Message< GpuMsg::SetCommandBufferState >	pending_state{ GpuMsg::SetCommandBufferState::EState::Pending };
		Message< GpuMsg::SetCommandBufferState >	completed_state{ GpuMsg::SetCommandBufferState::EState::Completed };

		FOR( i, msg->commands )
		{
			CHECK_ERR( msg->commands[i] );
			
			Message< GpuMsg::GetCommandBufferDescriptor >	req_descr;
			msg->commands[i]->Send( req_descr );
			CHECK_ERR( req_descr->result and not req_descr->result->flags[ ECmdBufferCreate::Secondary ] );

			msg->commands[i]->Send( pending_state );
			
			if ( not req_descr->result->flags[ ECmdBufferCreate::UseFence ] ) {
				msg->commands[i]->Send( completed_state );
			}
		}
		return true;
	}
	
/*
=================================================
	_SubmitComputeQueueCommands
=================================================
*/
	bool OpenGLThread::_SubmitComputeQueueCommands (const Message< GpuMsg::SubmitComputeQueueCommands > &msg)
	{
		using namespace gl;
		
		CHECK_ERR( _device.IsDeviceCreated() );

		Message< GpuMsg::SetCommandBufferState >	pending_state{ GpuMsg::SetCommandBufferState::EState::Pending };
		Message< GpuMsg::SetCommandBufferState >	completed_state{ GpuMsg::SetCommandBufferState::EState::Completed };
		
		FOR( i, msg->commands )
		{
			CHECK_ERR( msg->commands[i] );
			
			Message< GpuMsg::GetCommandBufferDescriptor >	req_descr;
			msg->commands[i]->Send( req_descr );
			CHECK_ERR( req_descr->result and not req_descr->result->flags[ ECmdBufferCreate::Secondary ] );

			msg->commands[i]->Send( pending_state );
			
			if ( not req_descr->result->flags[ ECmdBufferCreate::UseFence ] ) {
				msg->commands[i]->Send( completed_state );
			}
		}
		return true;
	}

/*
=================================================
	_WindowCreated
=================================================
*/
	bool OpenGLThread::_WindowCreated (const Message< OSMsg::WindowCreated > &)
	{
		if ( _CreateDevice() ) {
			CHECK( _DefCompose( false ) );
		}
		else {
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
		Message< OSMsg::WindowGetDescriptor >	req_descr;
		Message< OSMsg::GetWinWindowHandle >	req_hwnd;

		SendTo( _window, req_hwnd );
		SendTo( _window, req_descr );

		CHECK_ERR( _context.Create( *req_hwnd->result, INOUT _settings ) );

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
		if ( _device.IsDeviceCreated() )
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
		msg->result.Set({ &_device });
		return true;
	}
	
/*
=================================================
	_GetGraphicsSettings
=================================================
*/
	bool OpenGLThread::_GetGraphicsSettings (const Message< GpuMsg::GetGraphicsSettings > &msg)
	{
		msg->result.Set( _settings );
		return true;
	}
//-----------------------------------------------------------------------------

	
/*
=================================================
	CreateOpenGLThread
=================================================
*/
	ModulePtr OpenGLObjectsConstructor::CreateOpenGLThread (GlobalSystemsRef gs, const CreateInfo::GpuThread &ci)
	{
		return New< OpenGLThread >( gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
