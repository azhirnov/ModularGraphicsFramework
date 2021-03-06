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
		using SupportedMessages_t	= MessageListFrom<
											SceneMsg::GetScenePrivateClasses
										>;
		
		using SupportedEvents_t		= Module::SupportedEvents_t;


	// constants
	private:
		static const TypeIdList		_eventTypes;


	// variables
	private:


	// methods
	public:
		ScenePhysicsMainThread (UntypedID_t, GlobalSystemsRef, const CreateInfo::ScenePhysics &);
		~ScenePhysicsMainThread ();


	// message handlers
	protected:
		bool _Link (const ModuleMsg::Link &);
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);
		bool _AddToManager (const ModuleMsg::AddToManager &);
		bool _RemoveFromManager (const ModuleMsg::RemoveFromManager &);
		bool _GetScenePrivateClasses (const SceneMsg::GetScenePrivateClasses &);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	ScenePhysicsMainThread::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	ScenePhysicsMainThread::ScenePhysicsMainThread (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::ScenePhysics &ci) :
		Module( gs, ModuleConfig{ id, 1 }, &_eventTypes )
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
		
		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

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
	bool ScenePhysicsMainThread::_Link (const ModuleMsg::Link &msg)
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
	bool ScenePhysicsMainThread::_Compose (const ModuleMsg::Compose &msg)
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
	bool ScenePhysicsMainThread::_Delete (const ModuleMsg::Delete &msg)
	{
		//TODO( "" );
		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_AddToManager
=================================================
*/
	bool ScenePhysicsMainThread::_AddToManager (const ModuleMsg::AddToManager &msg)
	{
		// TODO
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool ScenePhysicsMainThread::_RemoveFromManager (const ModuleMsg::RemoveFromManager &msg)
	{
		// TODO
		return true;
	}
	
/*
=================================================
	_GetScenePrivateClasses
=================================================
*/
	bool ScenePhysicsMainThread::_GetScenePrivateClasses (const SceneMsg::GetScenePrivateClasses &msg)
	{
		msg.result.Set({ });	// TODO
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
