// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/Stream/StreamManager.h"
#include "Engine/Base/Main/MainSystem.h"

namespace Engine
{
namespace Base
{
	
	const Runtime::VirtualTypeList	StreamManager::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	StreamManager::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	StreamManager::StreamManager (const GlobalSystemsRef gs, const CreateInfo::StreamManager &info) :
		Module( gs, ModuleConfig{ StreamManagerModuleID, 1 }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( "StreamManager" );

		_SubscribeOnMsg( this, &StreamManager::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &StreamManager::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &StreamManager::_AttachModule_Empty );
		_SubscribeOnMsg( this, &StreamManager::_DetachModule_Empty );
		_SubscribeOnMsg( this, &StreamManager::_FindModule_Empty );
		_SubscribeOnMsg( this, &StreamManager::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &StreamManager::_Update_Impl );
		_SubscribeOnMsg( this, &StreamManager::_Link_Impl );
		_SubscribeOnMsg( this, &StreamManager::_Compose_Impl );
		_SubscribeOnMsg( this, &StreamManager::_Delete_Impl );
		
		CHECK( _ValidateMsgSubscriptions() );
	}
	
/*
=================================================
	destructor
=================================================
*/
	StreamManager::~StreamManager ()
	{
	}
	
/*
=================================================
	Register
=================================================
*/
	void StreamManager::Register (GlobalSystemsRef gs)
	{
		auto	mf = gs->Get< ModulesFactory >();

		CHECK( mf->Register( StreamManagerModuleID, &_CreateStreamManager ) );
		CHECK( mf->Register( InputStreamModuleID, &_CreateInStreamFromFile ) );
		CHECK( mf->Register( InputStreamModuleID, &_CreateInStreamFromUri ) );
		CHECK( mf->Register( InputStreamModuleID, &_CreateInStreamFromMemory ) );
		CHECK( mf->Register( OutputStreamModuleID, &_CreateOutStreamFromFile ) );
		CHECK( mf->Register( OutputStreamModuleID, &_CreateOutStreamFromUri ) );
	}
	
/*
=================================================
	Unregister
=================================================
*/
	void StreamManager::Unregister (GlobalSystemsRef gs)
	{
		auto	mf = gs->Get< ModulesFactory >();

		mf->UnregisterAll( StreamManagerModuleID );
		mf->UnregisterAll( OutputStreamModuleID );
		mf->UnregisterAll( InputStreamModuleID );
	}
	
/*
=================================================
	_CreateStreamManager
=================================================
*/
	ModulePtr StreamManager::_CreateStreamManager (GlobalSystemsRef gs, const CreateInfo::StreamManager &ci)
	{
		return New< StreamManager >( gs, ci );
	}

}	// Base
}	// Engine
