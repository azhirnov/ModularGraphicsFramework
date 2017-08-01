// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Input/InputThread.h"
#include "Engine/Platforms/Input/InputManager.h"

namespace Engine
{
namespace Platforms
{
	
	const Runtime::VirtualTypeList	InputThread::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	InputThread::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	InputThread::InputThread (const GlobalSystemsRef gs, const CreateInfo::InputThread &ci) :
		Module( gs, ModuleConfig{ GetStaticID(), 1 }, &_msgTypes, &_eventTypes )
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

		_AttachSelfToManager( null, InputManager::GetStaticID(), true );
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
		CHECK_ERR( msg->module );
		ASSERT( not _inputs.IsExist( msg->module ) );

		_inputs.Add( msg->module );

		bool	subscribed = false;

		// key events
		if ( msg->module->GetSupportedEvents().HasType< Message< ModuleMsg::InputKey > >() )
		{
			subscribed |= true;
			msg->module->Subscribe( this, &InputThread::_InputKey );
		}

		// motion events
		if ( msg->module->GetSupportedEvents().HasType< Message< ModuleMsg::InputMotion > >() )
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
		ASSERT( _inputs.IsExist( msg->module ) );

		_inputs.Erase( msg->module );

		msg->module->UnsubscribeAll( this );
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
		_keyBinds.Add( RVREF(msg->key), KeyBinds_t::value_t( RVREF(msg->state), RVREF(msg->cb.Get()) ) );
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


}	// Platforms
}	// Engine
