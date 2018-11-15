// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Input/InputManager.h"

namespace Engine
{
namespace Platforms
{

	//
	// Input Thread
	//

	class InputThread final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= MessageListFrom<
											ModuleMsg::InputKeyBind,
											ModuleMsg::InputMotionBind,
											ModuleMsg::InputKeyUnbindAll,
											ModuleMsg::InputMotionUnbindAll
										>;
		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											ModuleMsg::InputKey,		// not recomended to use
											ModuleMsg::InputMotion		// not recomended to use
										> >;

		struct KeyBindState
		{
		// variables
			ModuleMsg::InputKeyBind		bind;
			bool						pressed	= false;

		// methods
			KeyBindState () {}
			explicit KeyBindState (const ModuleMsg::InputKeyBind &bind) : bind{RVREF(bind)} {}
		};

		using KeyBinds_t			= MultiMap< KeyID::type, KeyBindState >;
		using MotionBinds_t			= MultiMap< MotionID::type, ModuleMsg::InputMotionBind >;

		using ModulesSet_t			= Set< ModulePtr >;


	// constants
	private:
		static const TypeIdList		_eventTypes;

		
	// variables
	private:
		ModulesSet_t	_inputs;

		KeyBinds_t		_keyBinds;
		MotionBinds_t	_motionBinds;
		
		bool			_lockBindings;


	// methods
	public:
		InputThread (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::InputThread &ci);
		~InputThread ();
		

	// message handlers
	private:
		bool _Delete (const ModuleMsg::Delete &);
		bool _AddToManager (const ModuleMsg::AddToManager &);
		bool _RemoveFromManager (const ModuleMsg::RemoveFromManager &);
		bool _InputKey (const ModuleMsg::InputKey &);
		bool _InputMotion (const ModuleMsg::InputMotion &);
		bool _InputKeyBind (const ModuleMsg::InputKeyBind &);
		bool _InputMotionBind (const ModuleMsg::InputMotionBind &);
		bool _InputKeyUnbindAll (const ModuleMsg::InputKeyUnbindAll &);
		bool _InputMotionUnbindAll (const ModuleMsg::InputMotionUnbindAll &);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	InputThread::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	InputThread::InputThread (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::InputThread &) :
		Module( gs, ModuleConfig{ id, 1 }, &_eventTypes ),
		_lockBindings( false )
	{
		SetDebugName( "InputThread" );

		_SubscribeOnMsg( this, &InputThread::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &InputThread::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &InputThread::_AttachModule_Impl );
		_SubscribeOnMsg( this, &InputThread::_DetachModule_Impl );
		_SubscribeOnMsg( this, &InputThread::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &InputThread::_FindModule_Impl );
		_SubscribeOnMsg( this, &InputThread::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &InputThread::_Update_Impl );
		_SubscribeOnMsg( this, &InputThread::_Link_Impl );
		_SubscribeOnMsg( this, &InputThread::_Compose_Impl );
		_SubscribeOnMsg( this, &InputThread::_Delete );
		_SubscribeOnMsg( this, &InputThread::_InputKeyBind );
		_SubscribeOnMsg( this, &InputThread::_InputMotionBind );
		_SubscribeOnMsg( this, &InputThread::_InputKeyUnbindAll );
		_SubscribeOnMsg( this, &InputThread::_InputMotionUnbindAll );
		_SubscribeOnMsg( this, &InputThread::_AddToManager );
		_SubscribeOnMsg( this, &InputThread::_RemoveFromManager );
		
		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

		_AttachSelfToManager( null, InputManagerModuleID, false );
	}
	
/*
=================================================
	destructor
=================================================
*/
	InputThread::~InputThread ()
	{
		//LOG( "InputThread finalized", ELog::Debug );

		ASSERT( _inputs.Empty() );
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool InputThread::_Delete (const ModuleMsg::Delete &msg)
	{
		_keyBinds.Clear();
		_motionBinds.Clear();

		CHECK_ERR( Module::_Delete_Impl( msg ) );

		_inputs.Clear();
		return true;
	}
	
/*
=================================================
	_AddToManager
=================================================
*/
	bool InputThread::_AddToManager (const ModuleMsg::AddToManager &msg)
	{
		using KeyEventsList_t		= MessageListFrom< ModuleMsg::InputKey >;
		using MotionEventsList_t	= MessageListFrom< ModuleMsg::InputMotion >;

		CHECK_ERR( msg.module );
		ASSERT( not _inputs.IsExist( msg.module ) );

		_inputs.Add( msg.module );

		bool	subscribed = false;

		// key events
		if ( msg.module->GetSupportedEvents().HasAllTypes< KeyEventsList_t >() )
		{
			subscribed |= true;
			msg.module->Subscribe( this, &InputThread::_InputKey );
		}

		// motion events
		if ( msg.module->GetSupportedEvents().HasAllTypes< MotionEventsList_t >() )
		{
			subscribed |= true;
			msg.module->Subscribe( this, &InputThread::_InputMotion );
		}

		// attached module hasn't any known input events
		CHECK( subscribed );
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool InputThread::_RemoveFromManager (const ModuleMsg::RemoveFromManager &msg)
	{
		CHECK_ERR( msg.module );

		ModulePtr	module = msg.module.Lock();

		if ( not module )
			return false;

		ASSERT( _inputs.IsExist( module ) );

		_inputs.Erase( module );

		module->UnsubscribeAll( this );
		return true;
	}

/*
=================================================
	_InputKey
=================================================
*/
	bool InputThread::_InputKey (const ModuleMsg::InputKey &msg)
	{
		_SendEvent( msg );

		CHECK_ERR( not _lockBindings );
		SCOPE_SETTER( _lockBindings = true, false );

		usize	idx = 0;

		if ( _keyBinds.FindFirstIndex( msg.key, OUT idx ) )
		{
			for (usize i = idx; i < _keyBinds.Count() and _keyBinds[i].first == msg.key; ++i)
			{
				auto&	state = _keyBinds[i].second;

				if ( state.bind.callback.IsValid() )
				{
					if ( msg.IsDown() and not state.pressed )
					{
						// first press
						state.pressed = true;
						
						if ( state.bind.state == EKeyState::OnKeyDown )
							state.bind.callback( msg );
					}
					else
					if ( msg.IsDown() )
					{
						// continue pressing
						if ( state.bind.state == EKeyState::OnKeyPressed )
							state.bind.callback( msg );
					}
					else
					{
						state.pressed = false;
						
						if ( state.bind.state == EKeyState::OnKeyUp )
							state.bind.callback( msg );
					}
				}
				else {
					_keyBinds.EraseByIndex( i );
					--i;
				}
			}
		}
		return true;
	}
	
/*
=================================================
	_InputMotion
=================================================
*/
	bool InputThread::_InputMotion (const ModuleMsg::InputMotion &msg)
	{
		_SendEvent( msg );

		CHECK_ERR( not _lockBindings );
		SCOPE_SETTER( _lockBindings = true, false );

		usize	idx = 0;

		if ( _motionBinds.FindFirstIndex( msg.motion, OUT idx ) )
		{
			for (usize i = idx; i < _motionBinds.Count() and _motionBinds[i].first == msg.motion; ++i)
			{
				auto&	bind = _motionBinds[i].second;

				if ( bind.callback.IsValid() ) {
					bind.callback( msg );
				}
				else {
					_motionBinds.EraseByIndex( i );
					--i;
				}
			}
		}
		return true;
	}
	
/*
=================================================
	_InputKeyBind
=================================================
*/
	bool InputThread::_InputKeyBind (const ModuleMsg::InputKeyBind &msg)
	{
		CHECK_ERR( msg.callback );

		_keyBinds.Add( msg.key, KeyBindState{msg} );
		return true;
	}
	
/*
=================================================
	_InputMotionBind
=================================================
*/
	bool InputThread::_InputMotionBind (const ModuleMsg::InputMotionBind &msg)
	{
		CHECK_ERR( msg.callback );

		_motionBinds.Add( msg.motion, msg );
		return true;
	}
	
/*
=================================================
	_InputMotionBind
=================================================
*/
	bool InputThread::_InputKeyUnbindAll (const ModuleMsg::InputKeyUnbindAll &msg)
	{
		CHECK_ERR( not _lockBindings );
		SCOPE_SETTER( _lockBindings = true, false );

		FOR( i, _keyBinds )
		{
			if ( _keyBinds[i].second.bind.callback.EqualPointers( msg.object.RawPtr() ) )
			{
				_keyBinds.EraseByIndex( i );
				--i;
			}
		}
		return true;
	}
	
/*
=================================================
	_InputMotionBind
=================================================
*/
	bool InputThread::_InputMotionUnbindAll (const ModuleMsg::InputMotionUnbindAll &msg)
	{
		CHECK_ERR( not _lockBindings );
		SCOPE_SETTER( _lockBindings = true, false );

		FOR( i, _motionBinds )
		{
			if ( _motionBinds[i].second.callback.EqualPointers( msg.object.RawPtr() ) )
			{
				_motionBinds.EraseByIndex( i );
				--i;
			}
		}
		return true;
	}
//-----------------------------------------------------------------------------
	


	ModulePtr InputManager::_CreateInputThread (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::InputThread &ci)
	{
		return New< InputThread >( id, gs, ci );
	}

}	// Platforms
}	// Engine
