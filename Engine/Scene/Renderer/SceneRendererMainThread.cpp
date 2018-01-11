// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Scene/Shared/Scene.h"
#include "Engine/Scene/Impl/SceneObjectConstructor.h"
#include "Engine/Scene/Impl/BaseSceneModule.h"

namespace Engine
{
namespace Scene
{

	//
	// Scene Renderer Main Thread
	//

	class SceneRendererMainThread final : public Module
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
		SceneRendererMainThread (GlobalSystemsRef, const CreateInfo::SceneRenderer &);
		~SceneRendererMainThread ();


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


	
	const TypeIdList	SceneRendererMainThread::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	SceneRendererMainThread::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SceneRendererMainThread::SceneRendererMainThread (GlobalSystemsRef gs, const CreateInfo::SceneRenderer &ci) :
		Module( gs, ModuleConfig{ SceneRendererModuleID, 1 }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( "SceneRendererMainThread" );
		
		_SubscribeOnMsg( this, &SceneRendererMainThread::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &SceneRendererMainThread::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &SceneRendererMainThread::_AttachModule_Impl );
		_SubscribeOnMsg( this, &SceneRendererMainThread::_DetachModule_Impl );
		_SubscribeOnMsg( this, &SceneRendererMainThread::_FindModule_Empty );
		_SubscribeOnMsg( this, &SceneRendererMainThread::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &SceneRendererMainThread::_Update_Empty );
		_SubscribeOnMsg( this, &SceneRendererMainThread::_Link );
		_SubscribeOnMsg( this, &SceneRendererMainThread::_Compose );
		_SubscribeOnMsg( this, &SceneRendererMainThread::_Delete );
		_SubscribeOnMsg( this, &SceneRendererMainThread::_AddToManager );
		_SubscribeOnMsg( this, &SceneRendererMainThread::_RemoveFromManager );
		_SubscribeOnMsg( this, &SceneRendererMainThread::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &SceneRendererMainThread::_GetScenePrivateClasses );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( null, SceneManagerModuleID, false );
	}
	
/*
=================================================
	destructor
=================================================
*/
	SceneRendererMainThread::~SceneRendererMainThread ()
	{
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool SceneRendererMainThread::_Link (const Message< ModuleMsg::Link > &msg)
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
	bool SceneRendererMainThread::_Compose (const Message< ModuleMsg::Compose > &msg)
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
	bool SceneRendererMainThread::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		//TODO( "" );
		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_AddToManager
=================================================
*/
	bool SceneRendererMainThread::_AddToManager (const Message< ModuleMsg::AddToManager > &msg)
	{
		// TODO
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool SceneRendererMainThread::_RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &msg)
	{
		// TODO
		return true;
	}
	
/*
=================================================
	_GetScenePrivateClasses
=================================================
*/
	bool SceneRendererMainThread::_GetScenePrivateClasses (const Message< SceneMsg::GetScenePrivateClasses > &msg)
	{
		msg->result.Set({ });	// TODO
		return true;
	}
//-----------------------------------------------------------------------------

	
/*
=================================================
	CreateSceneRenderer
=================================================
*/
	ModulePtr  SceneObjectConstructor::CreateSceneRenderer (GlobalSystemsRef gs, const CreateInfo::SceneRenderer &ci)
	{
		return New<SceneRendererMainThread>( gs, ci );
	}

}	// Scene
}	// Engine
