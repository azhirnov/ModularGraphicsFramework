// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/Stream/StreamManager.h"
#include "Engine/Base/Main/MainSystem.h"
#include "Engine/Base/Stream/InputStream.h"
#include "Engine/Base/Stream/OutputStream.h"

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
		Module( gs, ModuleConfig{ GetStaticID(), 1 }, &_msgTypes, &_eventTypes )
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

		CHECK( mf->Register( StreamManager::GetStaticID(), &_CreateStreamManager ) );
		CHECK( mf->Register( InputStream::GetStaticID(), &_CreateInStreamFromFile ) );
		CHECK( mf->Register( InputStream::GetStaticID(), &_CreateInStreamFromUri ) );
		CHECK( mf->Register( InputStream::GetStaticID(), &_CreateInStreamFromMemory ) );
		CHECK( mf->Register( OutputStream::GetStaticID(), &_CreateOutStreamFromFile ) );
		CHECK( mf->Register( OutputStream::GetStaticID(), &_CreateOutStreamFromUri ) );
	}
	
/*
=================================================
	Unregister
=================================================
*/
	void StreamManager::Unregister (GlobalSystemsRef gs)
	{
		auto	mf = gs->Get< ModulesFactory >();

		mf->UnregisterAll< StreamManager >();
		mf->UnregisterAll< OutputStream >();
		mf->UnregisterAll< InputStream >();
	}
	
/*
=================================================
	_CreateStreamManager
=================================================
*/
	ModulePtr StreamManager::_CreateStreamManager (GlobalSystemsRef gs, const CreateInfo::StreamManager &ci)
	{
		return GXTypes::New< StreamManager >( gs, ci );
	}
	
/*
=================================================
	_CreateInStreamFromFile
=================================================
*/
	ModulePtr StreamManager::_CreateInStreamFromFile (GlobalSystemsRef gs, const CreateInfo::InStreamFromFile &ci)
	{
		return GXTypes::New< InputStream >( gs, ci );
	}
	
/*
=================================================
	_CreateInStreamFromUri
=================================================
*/
	ModulePtr StreamManager::_CreateInStreamFromUri (GlobalSystemsRef gs, const CreateInfo::InStreamFromUri &ci)
	{
		return GXTypes::New< InputStream >( gs, ci );
	}
	
/*
=================================================
	_CreateInStreamFromMemory
=================================================
*/
	ModulePtr StreamManager::_CreateInStreamFromMemory (GlobalSystemsRef gs, const CreateInfo::InStreamFromMemory &ci)
	{
		return GXTypes::New< InputStream >( gs, ci );
	}
	
/*
=================================================
	_CreateOutStreamFromFile
=================================================
*/
	ModulePtr StreamManager::_CreateOutStreamFromFile (GlobalSystemsRef gs, const CreateInfo::OutStreamFromFile &ci)
	{
		return GXTypes::New< OutputStream >( gs, ci );
	}
	
/*
=================================================
	_CreateOutStreamFromUri
=================================================
*/
	ModulePtr StreamManager::_CreateOutStreamFromUri (GlobalSystemsRef gs, const CreateInfo::OutStreamFromUri &ci)
	{
		return GXTypes::New< OutputStream >( gs, ci );
	}

}	// Base
}	// Engine
