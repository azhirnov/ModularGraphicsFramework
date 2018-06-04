// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Scene/Public/Scene.h"
#include "Engine/Scene/Impl/SceneObjectConstructor.h"
#include "Engine/Scene/Impl/BaseSceneModule.h"

namespace Engine
{
namespace Scene
{

	//
	// Scene Physics Main Thread
	//

	class ScenePhysicsMainThread final : public Module
	{
	// types
	protected:
		using SupportedMessages_t	= Module::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager,
											ModuleMsg::OnManagerChanged,
											SceneMsg::GetScenePrivateClasses
										> >;
		
		using SupportedEvents_t		= Module::SupportedEvents_t;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:


	// methods
	public:
		ScenePhysicsMainThread (UntypedID_t, GlobalSystemsRef, const CreateInfo::ScenePhysics &);
		~ScenePhysicsMainThread ();


	// message handlers
	protected:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);
		bool _GetScenePrivateClasses (const Message< SceneMsg::GetScenePrivateClasses > &);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	ScenePhysicsMainThread::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	ScenePhysicsMainThread::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	ScenePhysicsMainThread::ScenePhysicsMainThread (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::ScenePhysics &ci) :
		Module( gs, ModuleConfig{ id, 1 }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( "ScenePhysicsMainThread" );
		
		_SubscribeOnMsg( this, &ScenePhysicsMainThread::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &ScenePhysicsMainThread::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &ScenePhysicsMainThread::_AttachModule_Impl );
		_SubscribeOnMsg( this, &ScenePhysicsMainThread::_DetachModule_Impl );
		_SubscribeOnMsg( this, &ScenePhysicsMainThread::_FindModule_Empty );
		_SubscribeOnMsg( this, &ScenePhysicsMainThread::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &ScenePhysicsMainThread::_Update_Empty );
		_SubscribeOnMsg( this, &ScenePhysicsMainThread::_Link );
		_SubscribeOnMsg( this, &ScenePhysicsMainThread::_Compose );
		_SubscribeOnMsg( this, &ScenePhysicsMainThread::_Delete );
		_SubscribeOnMsg( this, &ScenePhysicsMainThread::_AddToManager );
		_SubscribeOnMsg( this, &ScenePhysicsMainThread::_RemoveFromManager );
		_SubscribeOnMsg( this, &ScenePhysicsMainThread::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &ScenePhysicsMainThread::_GetScenePrivateClasses );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( null, SceneManagerModuleID, false );
	}
	
/*
=================================================
	destructor
=================================================
*/
	ScenePhysicsMainThread::~ScenePhysicsMainThread ()
	{
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool ScenePhysicsMainThread::_Link (const Message< ModuleMsg::Link > &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( _IsInitialState( GetState() ) );

		// TODO
		
		return Module::_Link_Impl( msg );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool ScenePhysicsMainThread::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		// TODO

		return _DefCompose( true );
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool ScenePhysicsMainThread::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		//TODO( "" );
		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_AddToManager
=================================================
*/
	bool ScenePhysicsMainThread::_AddToManager (const Message< ModuleMsg::AddToManager > &msg)
	{
		// TODO
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool ScenePhysicsMainThread::_RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &msg)
	{
		// TODO
		return true;
	}
	
/*
=================================================
	_GetScenePrivateClasses
=================================================
*/
	bool ScenePhysicsMainThread::_GetScenePrivateClasses (const Message< SceneMsg::GetScenePrivateClasses > &msg)
	{
		msg->result.Set({ });	// TODO
		return true;
	}
//-----------------------------------------------------------------------------

	
/*
=================================================
	CreateScenePhysics
=================================================
*/
	ModulePtr  SceneObjectConstructor::CreateScenePhysics (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::ScenePhysics &ci)
	{
		return New< ScenePhysicsMainThread >( id, gs, ci );
	}

}	// Scene
}	// Engine
