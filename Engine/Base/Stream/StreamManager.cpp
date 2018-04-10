// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/Stream/StreamObjectsConstructor.h"
#include "Engine/Base/Main/MainSystem.h"

namespace Engine
{
namespace Base
{

	//
	// Stream Manager
	//

	class StreamManager : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::AddOnFileModifiedListener,
											ModuleMsg::RemoveOnFileModifiedListener
										> >;

		using SupportedEvents_t		= Module::SupportedEvents_t;
		

		using FileWatchEvent_t		= Event< ModuleMsg::AddOnFileModifiedListener::Callback_t >;

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


	// methods
	public:
		StreamManager (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::StreamManager &info);
		~StreamManager ();
		

	// message handlers
	private:
		bool _Update (const Message< ModuleMsg::Update > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);

		bool _AddOnFileModifiedListener (const Message< ModuleMsg::AddOnFileModifiedListener > &);
		bool _RemoveOnFileModifiedListener (const Message< ModuleMsg::RemoveOnFileModifiedListener > &);
	};
//-----------------------------------------------------------------------------


	const TypeIdList	StreamManager::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	StreamManager::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	StreamManager::StreamManager (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::StreamManager &) :
		Module( gs, ModuleConfig{ id, 1 }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( "StreamManager" );

		_SubscribeOnMsg( this, &StreamManager::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &StreamManager::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &StreamManager::_AttachModule_Empty );
		_SubscribeOnMsg( this, &StreamManager::_DetachModule_Empty );
		_SubscribeOnMsg( this, &StreamManager::_FindModule_Empty );
		_SubscribeOnMsg( this, &StreamManager::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &StreamManager::_Update );
		_SubscribeOnMsg( this, &StreamManager::_Link_Impl );
		_SubscribeOnMsg( this, &StreamManager::_Compose_Impl );
		_SubscribeOnMsg( this, &StreamManager::_Delete );
		_SubscribeOnMsg( this, &StreamManager::_AddOnFileModifiedListener );
		_SubscribeOnMsg( this, &StreamManager::_RemoveOnFileModifiedListener );
		
		CHECK( _ValidateMsgSubscriptions() );
	}
	
/*
=================================================
	destructor
=================================================
*/
	StreamManager::~StreamManager ()
	{
		ASSERT( _fileWatch.Empty() );
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool StreamManager::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_fileWatch.Clear();

		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_Update
=================================================
*/
	bool StreamManager::_Update (const Message< ModuleMsg::Update > &)
	{
		FOR( i, _fileWatch )
		{
			StringCRef	fname	= _fileWatch[i].first;
			auto&		edit	= _fileWatch[i].second;
			ulong		time	= GlobalSystems()->fileManager->GetLastModificationTime( fname ).ToMillisecondsSince1970();
			
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
	_AddOnFileModifiedListener
=================================================
*/
	bool StreamManager::_AddOnFileModifiedListener (const Message< ModuleMsg::AddOnFileModifiedListener > &msg)
	{
		FileWatchList_t::iterator	iter;

		if ( not _fileWatch.Find( msg->filename, OUT iter ) )
		{
			iter = _fileWatch.Add( msg->filename, {} );
		}

		iter->second.callbacks.Add( msg->callback );
		return true;
	}
	
/*
=================================================
	_RemoveOnFileModifiedListener
=================================================
*/
	bool StreamManager::_RemoveOnFileModifiedListener (const Message< ModuleMsg::RemoveOnFileModifiedListener > &msg)
	{
		FOR( i, _fileWatch )
		{
			_fileWatch[i].second.callbacks.RemoveAllFor( msg->module.RawPtr() );
		}
		return true;
	}
//-----------------------------------------------------------------------------

	
/*
=================================================
	Register
=================================================
*/
	void StreamObjectsConstructor::Register (GlobalSystemsRef gs)
	{
		auto	mf = gs->modulesFactory;

		CHECK( mf->Register( StreamManagerModuleID, &CreateStreamManager ) );
		CHECK( mf->Register( InputStreamModuleID, &CreateInStreamFromFile ) );
		CHECK( mf->Register( InputStreamModuleID, &CreateInStreamFromUri ) );
		CHECK( mf->Register( InputStreamModuleID, &CreateInStreamFromMemory ) );
		CHECK( mf->Register( OutputStreamModuleID, &CreateOutStreamFromFile ) );
		CHECK( mf->Register( OutputStreamModuleID, &CreateOutStreamFromUri ) );
	}
	
/*
=================================================
	Unregister
=================================================
*/
	void StreamObjectsConstructor::Unregister (GlobalSystemsRef gs)
	{
		auto	mf = gs->modulesFactory;

		mf->UnregisterAll( StreamManagerModuleID );
		mf->UnregisterAll( OutputStreamModuleID );
		mf->UnregisterAll( InputStreamModuleID );
	}
	
/*
=================================================
	CreateStreamManager
=================================================
*/
	ModulePtr StreamObjectsConstructor::CreateStreamManager (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::StreamManager &ci)
	{
		return New< StreamManager >( id, gs, ci );
	}

}	// Base
}	// Engine
