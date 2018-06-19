// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Scene/Public/Scene.h"
#include "Engine/Scene/Impl/SceneObjectConstructor.h"
#include "Engine/Scene/Impl/BaseSceneModule.h"

namespace Engine
{
namespace Scene
{

	//
	// Scene Logic Main Thread
	//

	class SceneLogicMainThread final : public Module
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
		SceneLogicMainThread (UntypedID_t, GlobalSystemsRef, const CreateInfo::SceneLogic &);
		~SceneLogicMainThread ();


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


	
	const TypeIdList	SceneLogicMainThread::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	SceneLogicMainThread::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SceneLogicMainThread::SceneLogicMainThread (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::SceneLogic &ci) :
		Module( gs, ModuleConfig{ id, 1 }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( "SceneLogicMainThread" );
		
		_SubscribeOnMsg( this, &SceneLogicMainThread::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &SceneLogicMainThread::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &SceneLogicMainThread::_AttachModule_Impl );
		_SubscribeOnMsg( this, &SceneLogicMainThread::_DetachModule_Impl );
		_SubscribeOnMsg( this, &SceneLogicMainThread::_FindModule_Empty );
		_SubscribeOnMsg( this, &SceneLogicMainThread::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &SceneLogicMainThread::_Update_Empty );
		_SubscribeOnMsg( this, &SceneLogicMainThread::_Link );
		_SubscribeOnMsg( this, &SceneLogicMainThread::_Compose );
		_SubscribeOnMsg( this, &SceneLogicMainThread::_Delete );
		_SubscribeOnMsg( this, &SceneLogicMainThread::_AddToManager );
		_SubscribeOnMsg( this, &SceneLogicMainThread::_RemoveFromManager );
		_SubscribeOnMsg( this, &SceneLogicMainThread::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &SceneLogicMainThread::_GetScenePrivateClasses );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( null, SceneManagerModuleID, false );
	}
	
/*
=================================================
	destructor
=================================================
*/
	SceneLogicMainThread::~SceneLogicMainThread ()
	{
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool SceneLogicMainThread::_Link (const ModuleMsg::Link &msg)
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
	bool SceneLogicMainThread::_Compose (const ModuleMsg::Compose &msg)
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
	bool SceneLogicMainThread::_Delete (const ModuleMsg::Delete &msg)
	{
		//TODO( "" );
		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_AddToManager
=================================================
*/
	bool SceneLogicMainThread::_AddToManager (const ModuleMsg::AddToManager &msg)
	{
		// TODO
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool SceneLogicMainThread::_RemoveFromManager (const ModuleMsg::RemoveFromManager &msg)
	{
		// TODO
		return true;
	}
	
/*
=================================================
	_GetScenePrivateClasses
=================================================
*/
	bool SceneLogicMainThread::_GetScenePrivateClasses (const SceneMsg::GetScenePrivateClasses &msg)
	{
		msg.result.Set({ });	// TODO
		return true;
	}
//-----------------------------------------------------------------------------

	
/*
=================================================
	CreateSceneLogic
=================================================
*/
	ModulePtr  SceneObjectConstructor::CreateSceneLogic (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::SceneLogic &ci)
	{
		return New< SceneLogicMainThread >( id, gs, ci );
	}

}	// Scene
}	// Engine
