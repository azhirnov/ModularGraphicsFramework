// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/DataProvider/DataProviderObjectsConstructor.h"
#include "Engine/Base/DataProvider/DataMessages.h"
#include "Engine/Base/Main/MainSystem.h"
#include "Core/STL/Files/MemFile.h"

namespace Engine
{
namespace Base
{

	//
	// Builtin Storage Data Provider
	//

	class BuiltinStorageDataProvider : public Module
	{
	// types
	private:
		using SupportedMessages_t	= MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager,
											ModuleMsg::OnManagerChanged,
											DSMsg::OpenFileForRead,
											DSMsg::IsUriExists,
											DSMsg::CreateDataInputModule
										>;

		using SupportedEvents_t		= Module::SupportedEvents_t;
		
		using LoadFunc_t			= CreateInfo::BuiltinStorageDataProvider::LoadFunc_t;
		using LoaderFunc_t			= CreateInfo::BuiltinStorageDataProvider::LoaderFunc_t;


	// constants
	private:
		static const TypeIdList		_eventTypes;


	// variables
	private:
		LoaderFunc_t	_fileMap;


	// methods
	public:
		BuiltinStorageDataProvider (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::BuiltinStorageDataProvider &info);
		~BuiltinStorageDataProvider ();
		

	// message handlers
	private:
		bool _AddToManager (const ModuleMsg::AddToManager &)				{ return false; }
		bool _RemoveFromManager (const ModuleMsg::RemoveFromManager &)		{ return false; }
		bool _OpenFileForRead (const DSMsg::OpenFileForRead &);
		bool _IsUriExists (const DSMsg::IsUriExists &);
		bool _CreateDataInputModule (const DSMsg::CreateDataInputModule &);
	};
//-----------------------------------------------------------------------------


	const TypeIdList	BuiltinStorageDataProvider::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	BuiltinStorageDataProvider::BuiltinStorageDataProvider (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::BuiltinStorageDataProvider &ci) :
		Module( gs, ModuleConfig{ id, 1 }, &_eventTypes ),
		_fileMap{ ci.func }
	{
		SetDebugName( "BuiltinStorageDataProvider" );

		_SubscribeOnMsg( this, &BuiltinStorageDataProvider::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &BuiltinStorageDataProvider::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &BuiltinStorageDataProvider::_AttachModule_Empty );
		_SubscribeOnMsg( this, &BuiltinStorageDataProvider::_DetachModule_Empty );
		_SubscribeOnMsg( this, &BuiltinStorageDataProvider::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &BuiltinStorageDataProvider::_FindModule_Empty );
		_SubscribeOnMsg( this, &BuiltinStorageDataProvider::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &BuiltinStorageDataProvider::_Link_Impl );
		_SubscribeOnMsg( this, &BuiltinStorageDataProvider::_Compose_Impl );
		_SubscribeOnMsg( this, &BuiltinStorageDataProvider::_Delete_Impl );
		_SubscribeOnMsg( this, &BuiltinStorageDataProvider::_AddToManager );
		_SubscribeOnMsg( this, &BuiltinStorageDataProvider::_RemoveFromManager );
		_SubscribeOnMsg( this, &BuiltinStorageDataProvider::_OpenFileForRead );
		_SubscribeOnMsg( this, &BuiltinStorageDataProvider::_IsUriExists );
		_SubscribeOnMsg( this, &BuiltinStorageDataProvider::_CreateDataInputModule );
		
		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );
		
		_AttachSelfToManager( ci.manager, DataProviderManagerModuleID, false );
	}
	
/*
=================================================
	destructor
=================================================
*/
	BuiltinStorageDataProvider::~BuiltinStorageDataProvider ()
	{
	}

/*
=================================================
	_OpenFileForRead
=================================================
*/
	bool BuiltinStorageDataProvider::_OpenFileForRead (const DSMsg::OpenFileForRead &msg)
	{
		CHECK_ERR( _fileMap );

		BinaryArray	data;
		LoadFunc_t	func = _fileMap( msg.filename );
		auto		file = GXFile::MemRFile::New();

		CHECK_ERR( func );
		func( OUT data );

		CHECK_ERR( file->CreateFromArray( data ) );

		msg.result.Set( file );
		return true;
	}
	
/*
=================================================
	_IsUriExists
=================================================
*/
	bool BuiltinStorageDataProvider::_IsUriExists (const DSMsg::IsUriExists &msg)
	{
		CHECK_ERR( _fileMap );

		msg.result.Set( _fileMap( msg.uri ) != null );
		return true;
	}
	
/*
=================================================
	_CreateDataInputModule
=================================================
*/
	bool BuiltinStorageDataProvider::_CreateDataInputModule (const DSMsg::CreateDataInputModule &msg)
	{
		msg.result.Set(
			DataProviderObjectsConstructor::CreateFileDataInput( FileDataInputModuleID, GlobalSystems(), CreateInfo::DataInput{ msg.uri, this } )
		);
		return true;
	}
//-----------------------------------------------------------------------------
	
/*
=================================================
	CreateBuiltinStorageDataProvider
=================================================
*/
	ModulePtr DataProviderObjectsConstructor::CreateBuiltinStorageDataProvider (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::BuiltinStorageDataProvider &ci)
	{
		return New< BuiltinStorageDataProvider >( id, gs, ci );
	}

}	// Base
}	// Engine
