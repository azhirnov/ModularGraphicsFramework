// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
		using SupportedMessages_t	= Module::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::InputKeyBind,
											ModuleMsg::InputMotionBind,
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager,
											ModuleMsg::OnManagerChanged
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											ModuleMsg::InputKey,		// not recomended to use
											ModuleMsg::InputMotion		// not recomended to use
										> >;

		using KeyCollback_t			= ModuleMsg::InputKeyBind::Callback_t;
		using KeyBinds_t			= MultiMap< KeyID::type, Pair< EKeyState, KeyCollback_t > >;

		using MotionCallback_t		= ModuleMsg::InputMotionBind::Callback_t;
		using MotionBinds_t			= MultiMap< MotionID::type, MotionCallback_t >;

		using ModulesSet_t			= Set< ModulePtr >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;

		
	// variables
	private:
		ModulesSet_t	_inputs;

		KeyBinds_t		_keyBinds;
		MotionBinds_t	_motionBinds;
		

	// methods
	public:
		InputThread (GlobalSystemsRef gs, const CreateInfo::InputThread &ci);
		~InputThread ();
		

	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);
		bool _InputKey (const Message< ModuleMsg::InputKey > &);
		bool _InputMotion (const Message< ModuleMsg::InputMotion > &);
		bool _InputKeyBind (const Message< ModuleMsg::InputKeyBind > &);
		bool _InputMotionBind (const Message< ModuleMsg::InputMotionBind > &);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	InputThread::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	InputThread::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	InputThread::InputThread (GlobalSystemsRef gs, const CreateInfo::InputThread &) :
		Module( gs, ModuleConfig{ InputThreadModuleID, 1 }, &_msgTypes, &_eventTypes )
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
		_SubscribeOnMsg( this, &InputThread::_AddToManager );
		_SubscribeOnMsg( this, &InputThread::_RemoveFromManager );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( null, InputManagerModuleID, false );
	}
	
/*
=================================================
	destructor
=================================================
*/
	InputThread::~InputThread ()
	{
		LOG( "InputThread finalized", ELog::Debug );

		ASSERT( _inputs.Empty() );
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool InputThread::_Delete (const Message< ModuleMsg::Delete > &msg)
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
	bool InputThread::_AddToManager (const Message< ModuleMsg::AddToManager > &msg)
	{
		using KeyEventsList_t		= MessageListFrom< ModuleMsg::InputKey >;
		using MotionEventsList_t	= MessageListFrom< ModuleMsg::InputMotion >;

		CHECK_ERR( msg->module );
		ASSERT( not _inputs.IsExist( msg->module ) );

		_inputs.Add( msg->module );

		bool	subscribed = false;

		// key events
		if ( msg->module->GetSupportedEvents().HasAllTypes< KeyEventsList_t >() )
		{
			subscribed |= true;
			msg->module->Subscribe( this, &InputThread::_InputKey );
		}

		// motion events
		if ( msg->module->GetSupportedEvents().HasAllTypes< MotionEventsList_t >() )
		{
			subscribed |= true;
			msg->module->Subscribe( this, &InputThread::_InputMotion );
		}

		// attached module haven't any known input events
		CHECK( subscribed );
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool InputThread::_RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &msg)
	{
		CHECK_ERR( msg->module );

		ModulePtr	module = msg->module.Lock();

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
	bool InputThread::_InputKey (const Message< ModuleMsg::InputKey > &msg)
	{
		_SendEvent( msg );

		KeyBinds_t::values_range_t	range;
		_keyBinds.FindAll( msg->key, OUT range );

		FOR( i, range ) {
			range[i].second.second.Call( msg.Data() );
		}
		return true;
	}
	
/*
=================================================
	_InputMotion
=================================================
*/
	bool InputThread::_InputMotion (const Message< ModuleMsg::InputMotion > &msg)
	{
		_SendEvent( msg );

		MotionBinds_t::values_range_t	range;
		_motionBinds.FindAll( msg->motion, OUT range );

		FOR( i, range ) {
			range[i].second.Call( msg.Data() );
		}
		return true;
	}
	
/*
=================================================
	_InputKeyBind
=================================================
*/
	bool InputThread::_InputKeyBind (const Message< ModuleMsg::InputKeyBind > &msg)
	{
		_keyBinds.Add( RVREF(msg->key), KeyBinds_t::Value_t( RVREF(msg->state), RVREF(msg->cb.Get()) ) );
		return true;
	}
	
/*
=================================================
	_InputMotionBind
=================================================
*/
	bool InputThread::_InputMotionBind (const Message< ModuleMsg::InputMotionBind > &msg)
	{
		_motionBinds.Add( RVREF(msg->motion), RVREF(msg->cb.Get()) );
		return true;
	}
//-----------------------------------------------------------------------------
	


	ModulePtr InputManager::_CreateInputThread (GlobalSystemsRef gs, const CreateInfo::InputThread &ci)
	{
		return New< InputThread >( gs, ci );
	}

}	// Platforms
}	// Engine
