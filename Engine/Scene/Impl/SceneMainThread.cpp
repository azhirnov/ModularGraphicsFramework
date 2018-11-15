// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Scene/Public/Scene.h"
#include "Engine/Scene/Impl/SceneObjectConstructor.h"
#include "Engine/Scene/Impl/BaseSceneModule.h"

namespace Engine
{
namespace Scene
{

	//
	// Scene Main Thread
	//

	class SceneMainThread final : public Module
	{
	// types
	protected:
		using SupportedMessages_t	= MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager,
											ModuleMsg::OnManagerChanged,
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
		SceneMainThread (UntypedID_t, GlobalSystemsRef, const CreateInfo::SceneMain &);
		~SceneMainThread ();


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


	
	const TypeIdList	SceneMainThread::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SceneMainThread::SceneMainThread (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::SceneMain &ci) :
		Module( gs, ModuleConfig{ id, 1 }, &_eventTypes )
	{
		SetDebugName( "SceneMainThread" );
		
		_SubscribeOnMsg( this, &SceneMainThread::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &SceneMainThread::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &SceneMainThread::_AttachModule_Impl );
		_SubscribeOnMsg( this, &SceneMainThread::_DetachModule_Impl );
		_SubscribeOnMsg( this, &SceneMainThread::_FindModule_Empty );
		_SubscribeOnMsg( this, &SceneMainThread::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &SceneMainThread::_Update_Empty );
		_SubscribeOnMsg( this, &SceneMainThread::_Link );
		_SubscribeOnMsg( this, &SceneMainThread::_Compose );
		_SubscribeOnMsg( this, &SceneMainThread::_Delete );
		_SubscribeOnMsg( this, &SceneMainThread::_AddToManager );
		_SubscribeOnMsg( this, &SceneMainThread::_RemoveFromManager );
		_SubscribeOnMsg( this, &SceneMainThread::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &SceneMainThread::_GetScenePrivateClasses );
		
		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

		_AttachSelfToManager( null, SceneManagerModuleID, false );
	}
	
/*
=================================================
	destructor
=================================================
*/
	SceneMainThread::~SceneMainThread ()
	{
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool SceneMainThread::_Link (const ModuleMsg::Link &msg)
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
	bool SceneMainThread::_Compose (const ModuleMsg::Compose &msg)
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
	bool SceneMainThread::_Delete (const ModuleMsg::Delete &msg)
	{
		//TODO( "" );
		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_AddToManager
=================================================
*/
	bool SceneMainThread::_AddToManager (const ModuleMsg::AddToManager &msg)
	{
		// TODO
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool SceneMainThread::_RemoveFromManager (const ModuleMsg::RemoveFromManager &msg)
	{
		// TODO
		return true;
	}
	
/*
=================================================
	_GetScenePrivateClasses
=================================================
*/
	bool SceneMainThread::_GetScenePrivateClasses (const SceneMsg::GetScenePrivateClasses &msg)
	{
		msg.result.Set({ });	// TODO
		return true;
	}
//-----------------------------------------------------------------------------

	
/*
=================================================
	CreateSceneMainThread
=================================================
*/
	ModulePtr  SceneObjectConstructor::CreateSceneMainThread (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::SceneMain &ci)
	{
		return New< SceneMainThread >( id, gs, ci );
	}

}	// Scene
}	// Engine
