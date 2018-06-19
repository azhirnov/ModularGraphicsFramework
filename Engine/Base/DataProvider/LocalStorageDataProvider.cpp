// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/DataProvider/DataProviderObjectsConstructor.h"
#include "Engine/Base/DataProvider/DataMessages.h"
#include "Engine/Base/Main/MainSystem.h"

namespace Engine
{
namespace Base
{

	//
	// Local Storage Data Provider
	//

	class LocalStorageDataProvider : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Update
										> >::Append< MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager,
											ModuleMsg::OnManagerChanged,
											DSMsg::AddOnDataModifiedListener,
											DSMsg::RemoveOnDataModifiedListener,
											DSMsg::OpenFileForRead,
											DSMsg::OpenFileForWrite,
											DSMsg::IsUriExists,
											DSMsg::CreateDataInputModule,
											DSMsg::CreateDataOutputModule
										> >;

		using SupportedEvents_t		= Module::SupportedEvents_t;
		

		using FileWatchEvent_t		= Event< DSMsg::AddOnDataModifiedListener::Callback_t >;

		struct FileWatchInfo
		{
			FileWatchEvent_t	callbacks;
			ulong				time		= 0;
		};
			
		using FileWatchList_t		= HashMap< String, FileWatchInfo >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		FileWatchList_t		_fileWatch;

		String				_baseFolder;


	// methods
	public:
		LocalStorageDataProvider (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::LocalStorageDataProvider &info);
		~LocalStorageDataProvider ();
		

	// message handlers
	private:
		bool _Update (const ModuleMsg::Update &);
		bool _Delete (const ModuleMsg::Delete &);
		bool _AddToManager (const ModuleMsg::AddToManager &)				{ return false; }
		bool _RemoveFromManager (const ModuleMsg::RemoveFromManager &)		{ return false; }

		bool _AddOnDataModifiedListener (const DSMsg::AddOnDataModifiedListener &);
		bool _RemoveOnDataModifiedListener (const DSMsg::RemoveOnDataModifiedListener &);
		bool _OpenFileForRead (const DSMsg::OpenFileForRead &);
		bool _OpenFileForWrite (const DSMsg::OpenFileForWrite &);
		bool _IsUriExists (const DSMsg::IsUriExists &);
		bool _CreateDataInputModule (const DSMsg::CreateDataInputModule &);
		bool _CreateDataOutputModule (const DSMsg::CreateDataOutputModule &);
	};
//-----------------------------------------------------------------------------


	const TypeIdList	LocalStorageDataProvider::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	LocalStorageDataProvider::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	LocalStorageDataProvider::LocalStorageDataProvider (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::LocalStorageDataProvider &ci) :
		Module( gs, ModuleConfig{ id, 1 }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( "LocalStorageDataProvider" );

		_SubscribeOnMsg( this, &LocalStorageDataProvider::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &LocalStorageDataProvider::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &LocalStorageDataProvider::_AttachModule_Empty );
		_SubscribeOnMsg( this, &LocalStorageDataProvider::_DetachModule_Empty );
		_SubscribeOnMsg( this, &LocalStorageDataProvider::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &LocalStorageDataProvider::_FindModule_Empty );
		_SubscribeOnMsg( this, &LocalStorageDataProvider::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &LocalStorageDataProvider::_Update );
		_SubscribeOnMsg( this, &LocalStorageDataProvider::_Link_Impl );
		_SubscribeOnMsg( this, &LocalStorageDataProvider::_Compose_Impl );
		_SubscribeOnMsg( this, &LocalStorageDataProvider::_Delete );
		_SubscribeOnMsg( this, &LocalStorageDataProvider::_AddToManager );
		_SubscribeOnMsg( this, &LocalStorageDataProvider::_RemoveFromManager );
		_SubscribeOnMsg( this, &LocalStorageDataProvider::_AddOnDataModifiedListener );
		_SubscribeOnMsg( this, &LocalStorageDataProvider::_RemoveOnDataModifiedListener );
		_SubscribeOnMsg( this, &LocalStorageDataProvider::_OpenFileForRead );
		_SubscribeOnMsg( this, &LocalStorageDataProvider::_OpenFileForWrite );
		_SubscribeOnMsg( this, &LocalStorageDataProvider::_IsUriExists );
		_SubscribeOnMsg( this, &LocalStorageDataProvider::_CreateDataInputModule );
		_SubscribeOnMsg( this, &LocalStorageDataProvider::_CreateDataOutputModule );
		
		CHECK( _ValidateMsgSubscriptions() );

		String	dir;
		OS::FileSystem::GetCurrentDirectory( OUT dir );

		_baseFolder = FileAddress::BuildPath( dir, ci.baseFolder );
		
		_AttachSelfToManager( ci.manager, DataProviderManagerModuleID, false );
	}
	
/*
=================================================
	destructor
=================================================
*/
	LocalStorageDataProvider::~LocalStorageDataProvider ()
	{
		ASSERT( _fileWatch.Empty() );
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool LocalStorageDataProvider::_Delete (const ModuleMsg::Delete &msg)
	{
		_fileWatch.Clear();

		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_Update
=================================================
*/
	bool LocalStorageDataProvider::_Update (const ModuleMsg::Update &)
	{
		FOR( i, _fileWatch )
		{
			StringCRef	fname	= _fileWatch[i].first;
			auto&		edit	= _fileWatch[i].second;
			ulong		time	= OS::FileSystem::GetFileLastModificationTime( fname ).ToMillisecondsSince1970();
			
			if ( edit.callbacks.Empty() )
			{
				_fileWatch.EraseByIndex( i );
				--i;
				continue;
			}

			if ( edit.time < time )
			{
				edit.time = time;

				edit.callbacks( fname );
			}
		}
		return true;
	}
	
/*
=================================================
	_AddOnDataModifiedListener
=================================================
*/
	bool LocalStorageDataProvider::_AddOnDataModifiedListener (const DSMsg::AddOnDataModifiedListener &msg)
	{
		FileWatchList_t::iterator	iter;

		if ( not _fileWatch.Find( msg.filename, OUT iter ) )
		{
			iter = _fileWatch.Add( msg.filename, {} );
		}

		iter->second.callbacks.Add( msg.callback );
		return true;
	}
	
/*
=================================================
	_RemoveOnDataModifiedListener
=================================================
*/
	bool LocalStorageDataProvider::_RemoveOnDataModifiedListener (const DSMsg::RemoveOnDataModifiedListener &msg)
	{
		for (auto& file : _fileWatch)
		{
			file.second.callbacks.RemoveAllFor( msg.module.RawPtr() );
		}
		return true;
	}
	
/*
=================================================
	_OpenFileForRead
=================================================
*/
	bool LocalStorageDataProvider::_OpenFileForRead (const DSMsg::OpenFileForRead &msg)
	{
		String				path = FileAddress::BuildPath( _baseFolder, msg.filename );
		GXFile::RFilePtr	file = GXFile::HddRFile::New( path );

		CHECK_ERR( file );

		msg.result.Set( file );
		return true;
	}
	
/*
=================================================
	_OpenFileForWrite
=================================================
*/
	bool LocalStorageDataProvider::_OpenFileForWrite (const DSMsg::OpenFileForWrite &msg)
	{
		String				path = FileAddress::BuildPath( _baseFolder, msg.filename );
		GXFile::WFilePtr	file = GXFile::HddWFile::New( path );

		CHECK_ERR( file );

		msg.result.Set( file );
		return true;
	}
	
/*
=================================================
	_IsUriExists
=================================================
*/
	bool LocalStorageDataProvider::_IsUriExists (const DSMsg::IsUriExists &msg)
	{
		String	path = FileAddress::BuildPath( _baseFolder, msg.uri );

		msg.result.Set( OS::FileSystem::IsFileExist( path ) );
		return true;
	}
	
/*
=================================================
	_CreateDataInputModule
=================================================
*/
	bool LocalStorageDataProvider::_CreateDataInputModule (const DSMsg::CreateDataInputModule &msg)
	{
		msg.result.Set(
			DataProviderObjectsConstructor::CreateFileDataInput( FileDataInputModuleID, GlobalSystems(), CreateInfo::DataInput{ msg.uri, this } )
		);
		return true;
	}
	
/*
=================================================
	_CreateDataOutputModule
=================================================
*/
	bool LocalStorageDataProvider::_CreateDataOutputModule (const DSMsg::CreateDataOutputModule &msg)
	{
		msg.result.Set(
			DataProviderObjectsConstructor::CreateFileDataOutput( FileDataInputModuleID, GlobalSystems(), CreateInfo::DataOutput{ msg.uri, this } )
		);
		return true;
	}
//-----------------------------------------------------------------------------
	
/*
=================================================
	CreateLocalStorageDataProvider
=================================================
*/
	ModulePtr DataProviderObjectsConstructor::CreateLocalStorageDataProvider (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::LocalStorageDataProvider &ci)
	{
		return New< LocalStorageDataProvider >( id, gs, ci );
	}

}	// Base
}	// Engine
