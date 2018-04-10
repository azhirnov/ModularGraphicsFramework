// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/STL/Common/Platforms.h"

#ifdef PLATFORM_SDL

#include "Engine/Platforms/Public/OS/Input.h"
#include "Engine/Platforms/SDL/SDLMessages.h"
#include "Engine/Platforms/SDL/SDLObjectsConstructor.h"

namespace Engine
{
namespace PlatformSDL
{
	using namespace Platforms;


	//
	// SDL Mouse Input
	//

	class SDLMouseInput final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Compose
										> >
										::Append< MessageListFrom<
											ModuleMsg::OnManagerChanged,
											OSMsg::WindowDescriptorChanged,
											OSMsg::WindowCreated,
											OSMsg::WindowBeforeDestroy,
											OSMsg::OnSDLWindowRawMessage
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											ModuleMsg::InputMotion
										> >;

		using WindowMsgList_t		= MessageListFrom< OSMsg::GetSDLWindowHandle >;
		using WindowEventList_t		= MessageListFrom< OSMsg::WindowCreated, OSMsg::WindowBeforeDestroy, OSMsg::OnSDLWindowRawMessage >;

		struct TouchData
		{
			MotionID::type	xMotion		= Uninitialized;
			MotionID::type	yMotion		= Uninitialized;
			float2			absolute;
			float2			relative;
		};
		using TouchState_t			= StaticArray< TouchData, 8 >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;

		
	// variables
	private:
		ModulePtr			_window;
		uint2				_surfaceSize;

		TouchState_t		_touches;
		float2				_mousePos;
		float2				_mouseDiff;
		float2				_wheelDelta;
		

	// methods
	public:
		SDLMouseInput (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::RawInputHandler &ci);
		~SDLMouseInput ();


	// message handlers
	private:
		bool _WindowDescriptorChanged (const Message< OSMsg::WindowDescriptorChanged > &);
		bool _WindowCreated (const Message< OSMsg::WindowCreated > &);
		bool _WindowBeforeDestroy (const Message< OSMsg::WindowBeforeDestroy > &);
		bool _OnSDLWindowRawMessage (const Message< OSMsg::OnSDLWindowRawMessage > &);
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Update (const Message< ModuleMsg::Update > &);
		bool _DetachModule (const Message< ModuleMsg::DetachModule > &);

	private:
		void _Destroy ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	SDLMouseInput::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	SDLMouseInput::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SDLMouseInput::SDLMouseInput (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::RawInputHandler &) :
		Module( gs, ModuleConfig{ id, 1 }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( "SDLMouseInput" );
		
		_SubscribeOnMsg( this, &SDLMouseInput::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &SDLMouseInput::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &SDLMouseInput::_AttachModule_Impl );
		_SubscribeOnMsg( this, &SDLMouseInput::_DetachModule );
		_SubscribeOnMsg( this, &SDLMouseInput::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &SDLMouseInput::_FindModule_Impl );
		_SubscribeOnMsg( this, &SDLMouseInput::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &SDLMouseInput::_Update );
		_SubscribeOnMsg( this, &SDLMouseInput::_Link );
		_SubscribeOnMsg( this, &SDLMouseInput::_Delete_Impl );
		_SubscribeOnMsg( this, &SDLMouseInput::_WindowDescriptorChanged );
		_SubscribeOnMsg( this, &SDLMouseInput::_WindowCreated );
		_SubscribeOnMsg( this, &SDLMouseInput::_WindowBeforeDestroy );
		_SubscribeOnMsg( this, &SDLMouseInput::_OnSDLWindowRawMessage );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( null, InputThreadModuleID, false );

		_touches[0].xMotion = "touch-0.x"_MotionID;
		_touches[0].yMotion = "touch-0.y"_MotionID;
		
		_touches[1].xMotion = "touch-1.x"_MotionID;
		_touches[1].yMotion = "touch-1.y"_MotionID;
		
		_touches[2].xMotion = "touch-2.x"_MotionID;
		_touches[2].yMotion = "touch-2.y"_MotionID;
		
		_touches[3].xMotion = "touch-3.x"_MotionID;
		_touches[3].yMotion = "touch-3.y"_MotionID;
		
		_touches[4].xMotion = "touch-4.x"_MotionID;
		_touches[4].yMotion = "touch-4.y"_MotionID;
		
		_touches[5].xMotion = "touch-5.x"_MotionID;
		_touches[5].yMotion = "touch-5.y"_MotionID;
		
		_touches[6].xMotion = "touch-6.x"_MotionID;
		_touches[6].yMotion = "touch-6.y"_MotionID;
		
		_touches[7].xMotion = "touch-7.x"_MotionID;
		_touches[7].yMotion = "touch-7.y"_MotionID;
	}
	
/*
=================================================
	destructor
=================================================
*/
	SDLMouseInput::~SDLMouseInput ()
	{
		LOG( "SDLMouseInput finalized", ELog::Debug );
	}

/*
=================================================
	_WindowDescriptorChanged
=================================================
*/
	bool SDLMouseInput::_WindowDescriptorChanged (const Message< OSMsg::WindowDescriptorChanged > &msg)
	{
		_surfaceSize = msg->desc.surfaceSize;
		return true;
	}
	
/*
=================================================
	_WindowCreated
=================================================
*/
	bool SDLMouseInput::_WindowCreated (const Message< OSMsg::WindowCreated > &)
	{
		CHECK( _DefCompose( false ) );
		return true;
	}
	
/*
=================================================
	_WindowBeforeDestroy
=================================================
*/
	bool SDLMouseInput::_WindowBeforeDestroy (const Message< OSMsg::WindowBeforeDestroy > &)
	{
		_Destroy();

		_SendMsg< ModuleMsg::Delete >({});
		return true;
	}
	
/*
=================================================
	_OnSDLWindowRawMessage
=================================================
*/
	bool SDLMouseInput::_OnSDLWindowRawMessage (const Message< OSMsg::OnSDLWindowRawMessage > &msg)
	{
		const auto	OnTouchMotion = LAMBDA(this) (uint index, const float2 &absolute, const float2 &relative)
		{{
			_touches[index].absolute  = absolute;
			_touches[index].relative += relative;
		}};

		switch ( msg->event.type )
		{
			// touch screen //
			case SDL_FINGERMOTION :
			{
				TODO( "" );
				break;
			}

			case SDL_FINGERDOWN :
			case SDL_FINGERUP :
			{
				TODO( "" );
				break;
			}

			// mouse //
			case SDL_MOUSEMOTION :
			{
				const float2	absolute = float2(int2( msg->event.motion.x, _surfaceSize.y - msg->event.motion.y ));
				const float2	relative = float2(int2( msg->event.motion.xrel, msg->event.motion.yrel ));

				_mousePos  = absolute;
				_mouseDiff = relative;

				// touch emulation
				if ( msg->event.motion.state & SDL_BUTTON_LMASK )	OnTouchMotion( 0, absolute, relative );
				if ( msg->event.motion.state & SDL_BUTTON_MMASK )	OnTouchMotion( 1, absolute, relative );
				if ( msg->event.motion.state & SDL_BUTTON_RMASK )	OnTouchMotion( 2, absolute, relative );
				if ( msg->event.motion.state & SDL_BUTTON_X1MASK )	OnTouchMotion( 3, absolute, relative );
				if ( msg->event.motion.state & SDL_BUTTON_X2MASK )	OnTouchMotion( 4, absolute, relative );
				
				break;
			}

			// mouse wheel //
			case SDL_MOUSEWHEEL :
			{
				_wheelDelta += float2(int2( msg->event.wheel.x, msg->event.wheel.y ));
				break;
			}
		}
		return true;
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool SDLMouseInput::_Link (const Message< ModuleMsg::Link > &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( Module::_Link_Impl( msg ) );
		
		// subscribe on window events
		{
			CHECK_ATTACHMENT(( _window = GetParentByMsgEvent< WindowMsgList_t, WindowEventList_t >() ));

			if ( _IsComposedState( _window->GetState() ) )
			{
				_SendMsg< OSMsg::WindowCreated >({});
			}

			_window->Subscribe( this, &SDLMouseInput::_WindowDescriptorChanged );
			_window->Subscribe( this, &SDLMouseInput::_WindowCreated );
			_window->Subscribe( this, &SDLMouseInput::_WindowBeforeDestroy );
			_window->Subscribe( this, &SDLMouseInput::_OnSDLWindowRawMessage );
		}
		return true;
	}
	
/*
=================================================
	_Update
=================================================
*/
	bool SDLMouseInput::_Update (const Message< ModuleMsg::Update > &)
	{
		// send events to InputThread

		FOR( i, _touches )
		{
			float2 const&	absolute	= _touches[i].absolute;
			float2&			relative	= _touches[i].relative;

			if ( All( relative.IsZero() ) )
			{
				if ( IsNotZero( relative.x ) )
					_SendEvent< ModuleMsg::InputMotion >({_touches[i].xMotion, relative.x, absolute.x });
	
				if ( IsNotZero( relative.y ) )
					_SendEvent< ModuleMsg::InputMotion >({_touches[i].yMotion, relative.y, absolute.y });
			}
			relative = float2();
		}
		
		if ( Any( _mouseDiff.IsNotZero() ) )
		{
			if ( IsNotZero( _mouseDiff.x ) )
				_SendEvent< ModuleMsg::InputMotion >({ "mouse.x"_MotionID, _mouseDiff.x, _mousePos.x });
			
			if ( IsNotZero( _mouseDiff.y ) )
				_SendEvent< ModuleMsg::InputMotion >({ "mouse.y"_MotionID, _mouseDiff.y, _mousePos.y });
		}

		if ( Any( _wheelDelta.IsNotZero() ) )
		{
			if ( IsNotZero( _wheelDelta.x ) )
				_SendEvent< ModuleMsg::InputMotion >({ "scroll.x"_MotionID, _wheelDelta.x, 0.0f });

			if ( IsNotZero( _wheelDelta.y ) )
				_SendEvent< ModuleMsg::InputMotion >({ "scroll.y"_MotionID, _wheelDelta.y, 0.0f });
		}
		
		_mouseDiff  = float2();
		_wheelDelta = float2();
		return true;
	}
	
/*
=================================================
	_DetachModule
=================================================
*/
	bool SDLMouseInput::_DetachModule (const Message< ModuleMsg::DetachModule > &msg)
	{
		if ( _Detach( msg->oldModule ) and msg->oldModule == _window )
		{
			CHECK( _SetState( EState::Initial ) );
			_Destroy();
		}
		return true;
	}
	
/*
=================================================
	_Destroy
=================================================
*/
	void SDLMouseInput::_Destroy ()
	{
		if ( _window )
			_window->UnsubscribeAll( this );

		_window = null;
	}

}	// PlatformSDL
//-----------------------------------------------------------------------------
	

namespace Platforms
{
/*
=================================================
	CreateSDLMouseInput
=================================================
*/
	ModulePtr SDLObjectsConstructor::CreateSDLMouseInput (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::RawInputHandler &ci)
	{
		return New< PlatformSDL::SDLMouseInput >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// PLATFORM_SDL
