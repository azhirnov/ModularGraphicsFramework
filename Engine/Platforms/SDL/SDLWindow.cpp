// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/OS/Window.h"
#include "Engine/Platforms/Shared/OS/Display.h"
#include "Engine/Platforms/Shared/OS/Platform.h"
#include "Engine/Platforms/SDL/SDLMessages.h"
#include "Engine/Platforms/SDL/SDLObjectsConstructor.h"
#include "Engine/Platforms/SDL/SDLDisplay.h"

#ifdef PLATFORM_SDL

namespace Engine
{
namespace PlatformSDL
{

	//
	// SDL Window
	//

	class SDLWindow final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Link,
											ModuleMsg::Compose
										> >
										::Append< MessageListFrom<
											ModuleMsg::OnManagerChanged,
											OSMsg::WindowSetDescriptor,
											OSMsg::WindowGetDescriptor,
											OSMsg::OnSDLPlatformCreated,
											OSMsg::GetSDLWindowHandle,
											OSMsg::GetDisplays,
											OSMsg::GetOSModules
										> >;
		using SupportedEvents_t		= MessageListFrom<
											ModuleMsg::Update,
											ModuleMsg::Delete,
											OSMsg::WindowDescriptorChanged,
											OSMsg::WindowVisibilityChanged,
											OSMsg::WindowBeforeCreate,
											OSMsg::WindowCreated,
											OSMsg::WindowBeforeDestroy,
											OSMsg::WindowAfterDestroy,
											OSMsg::OnSDLWindowRawMessage
										>;

		using EVisibility			= CreateInfo::Window::EVisibility;
		using EWindowFlags			= CreateInfo::Window::EWindowFlags;
		

	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		CreateInfo::Window	_createInfo;
		SDLDisplay			_display;

		WindowDesc			_windowDesc;
		Ptr<SDL_Window>		_wnd;

		TimeProfilerD		_timer;

		bool				_requestQuit;
		bool				_looping;


	// methods
	public:
		SDLWindow (GlobalSystemsRef gs, const CreateInfo::Window &ci);
		~SDLWindow ();
		

	// message handlers
	private:
		bool _Update (const Message< ModuleMsg::Update > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _OnSDLPlatformCreated (const Message< OSMsg::OnSDLPlatformCreated > &);
		bool _PlatformDeleted (const Message< ModuleMsg::Delete > &);
		bool _WindowSetDescriptor (const Message< OSMsg::WindowSetDescriptor > &);
		bool _WindowGetDescriptor (const Message< OSMsg::WindowGetDescriptor > &);
		bool _GetSDLWindowHandle (const Message< OSMsg::GetSDLWindowHandle > &);
		bool _GetDisplays (const Message< OSMsg::GetDisplays > &);
		bool _GetOSModules (const Message< OSMsg::GetOSModules > &);


	private:
		bool _IsCreated () const;
		void _Destroy ();
		
		bool _Create ();
		bool _CreateWindow (const Display &disp, const CreateInfo::Window &info);
		
		void _ShowWindow (EVisibility flags);

		void _ProcessEvents ();
		bool _UpdateDescriptor ();
		
		Ptr<const Display> _GetDisplayByCoord (const int2 &point) const;
	};
//-----------------------------------------------------------------------------



	const TypeIdList	SDLWindow::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	SDLWindow::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SDLWindow::SDLWindow (GlobalSystemsRef gs, const CreateInfo::Window &ci) :
		Module( gs, ModuleConfig{ SDLWindowModuleID, 1 }, &_msgTypes, &_eventTypes ),
		_createInfo( ci ),
		_requestQuit( false ),
		_looping( false )
	{
		SetDebugName( "SDLWindow" );

		_SubscribeOnMsg( this, &SDLWindow::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &SDLWindow::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &SDLWindow::_AttachModule_Impl );
		_SubscribeOnMsg( this, &SDLWindow::_DetachModule_Impl );
		_SubscribeOnMsg( this, &SDLWindow::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &SDLWindow::_FindModule_Impl );
		_SubscribeOnMsg( this, &SDLWindow::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &SDLWindow::_Delete );
		_SubscribeOnMsg( this, &SDLWindow::_Update );
		_SubscribeOnMsg( this, &SDLWindow::_WindowSetDescriptor );
		_SubscribeOnMsg( this, &SDLWindow::_WindowGetDescriptor );
		_SubscribeOnMsg( this, &SDLWindow::_OnSDLPlatformCreated );
		_SubscribeOnMsg( this, &SDLWindow::_GetSDLWindowHandle );
		_SubscribeOnMsg( this, &SDLWindow::_GetDisplays );
		_SubscribeOnMsg( this, &SDLWindow::_GetOSModules );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( null, SDLPlatformModuleID, false );
	}
	
/*
=================================================
	destructor
=================================================
*/
	SDLWindow::~SDLWindow ()
	{
		LOG( "SDLWindow finalized", ELog::Debug );

		CHECK( not _IsCreated() );
		ASSERT( not _looping );
	}
	
/*
=================================================
	_Update
=================================================
*/
	bool SDLWindow::_Update (const Message< ModuleMsg::Update > &msg)
	{
		if ( not _IsComposedState( GetState() ) )
			return true;
		
		_ProcessEvents();

		_SendForEachAttachments( msg );
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool SDLWindow::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_requestQuit	= true;
		_looping		= false;

		_Destroy();

		CHECK_ERR( Module::_Delete_Impl( msg ) );
		return true;
	}

/*
=================================================
	_OnSDLPlatformCreated
=================================================
*/
	bool SDLWindow::_OnSDLPlatformCreated (const Message< OSMsg::OnSDLPlatformCreated > &msg)
	{
		CHECK_ERR( not _IsComposedState( GetState() ) );

		CHECK( _Create() );

		if ( _IsCreated() and _looping )
		{
			CHECK( _ValidateAllSubscriptions() );
			CHECK( _SetState( EState::ComposedMutable ) );

			LOG( "window created", ELog::Debug );
			
			_SendForEachAttachments< ModuleMsg::Link >({});
			_SendForEachAttachments< ModuleMsg::Compose >({});
		}
		else
		{
			CHECK( _SetState( EState::ComposingFailed ) );
		}
		return true;
	}
		
/*
=================================================
	_PlatformDeleted
=================================================
*/
	bool SDLWindow::_PlatformDeleted (const Message< ModuleMsg::Delete > &msg)
	{
		_Delete( msg );
		return true;
	}

/*
=================================================
	_WindowSetDescriptor
=================================================
*/
	bool SDLWindow::_WindowSetDescriptor (const Message< OSMsg::WindowSetDescriptor > &msg)
	{
		if ( not _IsCreated() )
		{
			CHECK_ERR( GetState() != EState::Deleting );

			_createInfo.caption				= msg->desc.caption;
			_createInfo.flags				= msg->desc.flags;
			_createInfo.initialVisibility	= msg->desc.visibility;
			_createInfo.orientation			= msg->desc.orientation;
			_createInfo.position			= msg->desc.position;
			_createInfo.surfaceSize			= msg->desc.surfaceSize;
			// ignored msg->desc.size
			return true;
		}
		
		Ptr<const Display>	disp = _GetDisplayByCoord( msg->desc.position );

		if ( not disp )
			disp = &_display.GetDisplays().Front();
		
		uint2 const		scr_res = disp->Resolution();

		// set window caption
		if ( msg->desc.caption != _windowDesc.caption )
		{
			_windowDesc.caption = msg->desc.caption;
			SDL_SetWindowTitle( _wnd, _windowDesc.caption.cstr() );
		}
		
		if ( msg->desc.flags[ EWindowFlags::Fullscreen ] )
		{
			// setup for fullscreen
			_windowDesc.position	 = disp->FullArea().LeftBottom();
			_windowDesc.size		 = scr_res;
			_windowDesc.surfaceSize	 = scr_res;

			SDL_CALL( SDL_SetWindowFullscreen( _wnd, SDL_WINDOW_FULLSCREEN ) );
			
			_windowDesc.flags[ EWindowFlags::Fullscreen ] = true;
		}
		else
		{
			// setup windowed
			_windowDesc.surfaceSize	= msg->desc.surfaceSize;
			_windowDesc.position	= msg->desc.position;

			if ( msg->desc.flags[ EWindowFlags::Centered ] )
			{
				_windowDesc.position = disp->WorkArea().Center() - int2(_windowDesc.size) / 2;		
			}
		
			if ( _windowDesc.flags[ EWindowFlags::Fullscreen ] )
			{
				SDL_CALL( SDL_SetWindowFullscreen( _wnd, 0 ) );
				
				_windowDesc.flags[ EWindowFlags::Fullscreen ] = false;
			}

			SDL_SetWindowSize( _wnd, _windowDesc.surfaceSize.x, _windowDesc.surfaceSize.y );
			SDL_SetWindowPosition( _wnd, _windowDesc.position.x, _windowDesc.position.y );
		}

		return true;
	}

/*
=================================================
	_WindowGetDescriptor
=================================================
*/
	bool SDLWindow::_WindowGetDescriptor (const Message< OSMsg::WindowGetDescriptor > &msg)
	{
		CHECK_ERR( _IsCreated() );

		msg->result.Set( _windowDesc );
		return true;
	}
	
/*
=================================================
	_GetSDLWindowHandle
=================================================
*/
	bool SDLWindow::_GetSDLWindowHandle (const Message< OSMsg::GetSDLWindowHandle > &msg)
	{
		msg->result.Set( _wnd );
		return true;
	}
	
/*
=================================================
	_GetDisplays
=================================================
*/
	bool SDLWindow::_GetDisplays (const Message< OSMsg::GetDisplays > &msg)
	{
		//_display.Update();

		msg->result.Set( _display.GetDisplays() );
		return true;
	}
	
/*
=================================================
	_GetOSModules
=================================================
*/
	bool SDLWindow::_GetOSModules (const Message< OSMsg::GetOSModules > &msg)
	{
		msg->result.Set( SDLObjectsConstructor::GetModuleIDs() );
		return true;
	}
	
/*
=================================================
	_IsCreated
=================================================
*/
	bool SDLWindow::_IsCreated () const
	{
		return _wnd.IsNotNull();
	}

/*
=================================================
	_Destroy
=================================================
*/
	void SDLWindow::_Destroy ()
	{
		if ( _IsCreated() )
		{
			_SendEvent< OSMsg::WindowBeforeDestroy >({});
	
			SDL_DestroyWindow( _wnd );
			_wnd = null;
			
			LOG( "window destroyed", ELog::Debug );

			_SendEvent< OSMsg::WindowAfterDestroy >({});
		}
	}
	
/*
=================================================
	_Create
=================================================
*/
	bool SDLWindow::_Create ()
	{
		CHECK_ERR( _display.Update() );
		
		Ptr<const Display>	disp = _GetDisplayByCoord( _createInfo.position );

		if ( not disp )
			disp = &_display.GetDisplays().Front();
		
		// validate create info
		{
			if ( _createInfo.caption.Empty() )
				_createInfo.caption = ENGINE_NAME;

			if ( not _createInfo.flags[ EWindowFlags::Fullscreen ] )
				_createInfo.surfaceSize = Clamp( _createInfo.surfaceSize, uint2(0), uint2(disp->WorkArea().Size()) );

			_createInfo.orientation = EDisplayOrientation::Default;
		}
		

		// allow subscribers to change settings
		auto const					msg		= Message< OSMsg::WindowBeforeCreate >{ _createInfo };
		CreateInfo::Window const&	info	= msg->editable;

		_SendEvent( msg );

		
		// crate window
		CHECK_ERR( _CreateWindow( *disp, info ) );

		CHECK_ERR( _UpdateDescriptor() );
		

		// send event
		_SendEvent< OSMsg::WindowCreated >({});
		_SendEvent< OSMsg::WindowDescriptorChanged >({ _windowDesc });


		// start window message loop
		_looping = true;
		_ShowWindow( _createInfo.initialVisibility );

		return true;
	}
	
/*
=================================================
	_CreateWindow
=================================================
*/
	bool SDLWindow::_CreateWindow (const Display &disp, const CreateInfo::Window &info)
	{
		CHECK_ERR( not _IsCreated() );
		
		uint2 const		scr_res = disp.Resolution();
		
		uint			flags	= int(SDL_WINDOW_HIDDEN) | int(SDL_WINDOW_ALLOW_HIGHDPI);
		
		if ( info.flags[ EWindowFlags::Fullscreen ] )
		{
			// setup for fullscreen
			flags					|= (int(SDL_WINDOW_FULLSCREEN) | int(SDL_WINDOW_BORDERLESS));
			_windowDesc.position	 = disp.FullArea().LeftBottom();
			_windowDesc.size		 = scr_res;
			_windowDesc.surfaceSize	 = scr_res;
		}
		else
		{
			// setup windowed
			if ( info.flags[ EWindowFlags::Resizable ] )
				flags |= SDL_WINDOW_RESIZABLE;

			// flags |= SDL_WINDOW_BORDERLESS;

			_windowDesc.surfaceSize	= info.surfaceSize;
			_windowDesc.position	= info.position;

			if ( info.flags[ EWindowFlags::Centered ] )
			{
				_windowDesc.position = disp.WorkArea().Center() - int2(_windowDesc.surfaceSize) / 2;	
			}
		}

		_windowDesc.caption = info.caption;

		// create window
		CHECK_ERR( (_wnd = SDL_CreateWindow( _windowDesc.caption.cstr(),
											 _windowDesc.position.x, _windowDesc.position.y,
											 _windowDesc.surfaceSize.x, _windowDesc.surfaceSize.y,
											 flags )) != null );

		SDL_SetWindowData( _wnd, "mgf", this );

		return true;
	}
	
/*
=================================================
	_ShowWindow
=================================================
*/
	void SDLWindow::_ShowWindow (EVisibility flags)
	{
		CHECK_ERR( _IsCreated(), void() );

		switch ( flags )
		{
			case EVisibility::VisibleFocused :
				//SDL_RaiseWindow( _wnd );
				SDL_ShowWindow( _wnd );
				break;

			case EVisibility::VisibleUnfocused :
				SDL_ShowWindow( _wnd );
				break;

			case EVisibility::Invisible :
				SDL_HideWindow( _wnd );
				break;

			default :
				RETURN_ERR( "unknown visibility flags", void() );
		}
	}

/*
=================================================
	_ProcessEvents
=================================================
*/
	void SDLWindow::_ProcessEvents ()
	{
		if ( not _looping or _requestQuit )
			return;

		Message< OSMsg::OnSDLWindowRawMessage >	msg;

		while ( SDL_PollEvent( OUT &msg->event ) )
		{
			switch ( msg->event.type )
			{
				// quit //
				case SDL_QUIT :
				case SDL_APP_TERMINATING :
				{
					_requestQuit = true;

					_Destroy();
					return;
				}


				// enter background //
				case SDL_APP_WILLENTERBACKGROUND :
					//_EnterBackground( false );
					break;

				case SDL_APP_DIDENTERBACKGROUND :
					//_EnterBackground( true );
					break;


				// enter foreground //
				case SDL_APP_WILLENTERFOREGROUND :
					//_EnterForeground( false );
					break;

				case SDL_APP_DIDENTERFOREGROUND :
					//_EnterForeground( true );
					break;


				// window event //
				case SDL_WINDOWEVENT :
				{
					switch ( msg->event.window.event )
					{
						// show //
						case SDL_WINDOWEVENT_SHOWN :
						{
							msg->wasProcessed = true;
			
							_windowDesc.visibility = EVisibility::VisibleFocused;
							_SendEvent< OSMsg::WindowVisibilityChanged >({ _windowDesc.visibility });
							break;
						}
						// hide //
						case SDL_WINDOWEVENT_HIDDEN :
						{
							msg->wasProcessed = true;

							_windowDesc.visibility = EVisibility::VisibleUnfocused;
							_SendEvent< OSMsg::WindowVisibilityChanged >({ _windowDesc.visibility });
							break;
						}
						// resize //
						case SDL_WINDOWEVENT_RESIZED :
						case SDL_WINDOWEVENT_MOVED :
						{
							msg->wasProcessed = true;

							_UpdateDescriptor();
							_SendEvent< OSMsg::WindowDescriptorChanged >({ _windowDesc });
							break;
						}
					}
					break;
				}
			}
			
			_SendEvent( msg );
		}
	}

/*
=================================================
	_UpdateDescriptor
=================================================
*/
	bool SDLWindow::_UpdateDescriptor ()
	{
		CHECK_ERR( _IsCreated() );

		uint	flags = SDL_GetWindowFlags( _wnd );

		if ( flags & SDL_WINDOW_SHOWN )
		{
			if ( flags & (SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS) )
				_windowDesc.visibility = EVisibility::VisibleFocused;
			else
				_windowDesc.visibility = EVisibility::VisibleUnfocused;
		}
		else
		if ( flags & SDL_WINDOW_HIDDEN )
		{
			_windowDesc.visibility = EVisibility::Invisible;
		}

		_windowDesc.caption = SDL_GetWindowTitle( _wnd );

		int2	pos;
		SDL_GetWindowPosition( _wnd, OUT &pos.x, OUT &pos.y );
		_windowDesc.position = pos;

		int2	size;
		SDL_GetWindowSize( _wnd, OUT &size.x, OUT &size.y );
		_windowDesc.surfaceSize	= uint2(size);

		RectI	border;
		SDL_CALL( SDL_GetWindowBordersSize( _wnd, OUT &border.top, OUT &border.left, OUT &border.bottom, OUT &border.right ) );
		_windowDesc.size = _windowDesc.surfaceSize + uint2(border.left + border.right, border.top + border.bottom);

		return true;
	}
	
/*
=================================================
	_GetDisplayByCoord
=================================================
*/
	Ptr<const Display> SDLWindow::_GetDisplayByCoord (const int2 &point) const
	{
		FOR( i, _display.GetDisplays() )
		{
			const auto&	disp = _display.GetDisplays()[i];

			if ( disp.FullArea().IsInnerPoint( point ) )
			{
				return &disp;
			}
		}
		return null;
	}

}	// PlatformSDL
//-----------------------------------------------------------------------------
	

namespace Platforms
{
/*
=================================================
	CreateSDLWindow
=================================================
*/
	ModulePtr SDLObjectsConstructor::CreateSDLWindow (GlobalSystemsRef gs, const CreateInfo::Window &ci)
	{
		return New< PlatformSDL::SDLWindow >( gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// PLATFORM_SDL
