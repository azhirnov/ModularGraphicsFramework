// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Scene/Shared/Scene.h"
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
		SceneMainThread (GlobalSystemsRef, const CreateInfo::SceneMain &);
		~SceneMainThread ();


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


	
	const TypeIdList	SceneMainThread::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	SceneMainThread::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SceneMainThread::SceneMainThread (GlobalSystemsRef gs, const CreateInfo::SceneMain &ci) :
		Module( gs, ModuleConfig{ SceneModuleID, 1 }, &_msgTypes, &_eventTypes )
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
		
		CHECK( _ValidateMsgSubscriptions() );

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
	bool SceneMainThread::_Link (const Message< ModuleMsg::Link > &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( GetState() == EState::Initial or GetState() == EState::LinkingFailed );

		// TODO

		CHECK_ERR( Module::_Link_Impl( msg ) );
		return true;
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool SceneMainThread::_Compose (const Message< ModuleMsg::Compose > &msg)
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
	bool SceneMainThread::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		//TODO( "" );
		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_AddToManager
=================================================
*/
	bool SceneMainThread::_AddToManager (const Message< ModuleMsg::AddToManager > &msg)
	{
		// TODO
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool SceneMainThread::_RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &msg)
	{
		// TODO
		return true;
	}
	
/*
=================================================
	_GetScenePrivateClasses
=================================================
*/
	bool SceneMainThread::_GetScenePrivateClasses (const Message< SceneMsg::GetScenePrivateClasses > &msg)
	{
		msg->result.Set({ });	// TODO
		return true;
	}
//-----------------------------------------------------------------------------

	
/*
=================================================
	CreateSceneMainThread
=================================================
*/
	ModulePtr  SceneObjectConstructor::CreateSceneMainThread (GlobalSystemsRef gs, const CreateInfo::SceneMain &ci)
	{
		return New<SceneMainThread>( gs, ci );
	}

}	// Scene
}	// Engine
