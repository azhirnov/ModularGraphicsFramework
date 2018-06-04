// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/DataProvider/DataMessages.h"
#include "Engine/Base/DataProvider/DataProviderObjectsConstructor.h"
#include "Engine/Base/Modules/Module.h"

namespace Engine
{
namespace Base
{
	
	//
	// File Data Output Module
	//

	class FileDataOutput final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Update
										> >::Append< MessageListFrom< 
											ModuleMsg::OnManagerChanged,
											DSMsg::GetDataSourceDescriptor,
											DSMsg::WriteRegion,
											DSMsg::ReleaseData
										> >;

		//using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
		//									DSMsg::DataRegionChanged
		//								> >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		const String		_filename;
		GXFile::WFilePtr	_file;


	// methods
	public:
		FileDataOutput (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::DataOutput &ci);
		~FileDataOutput ();


	// message handlers
	private:
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);

		bool _GetDataSourceDescriptor (const Message< DSMsg::GetDataSourceDescriptor > &);
		bool _WriteRegion_Empty (const Message< DSMsg::WriteRegion > &);
		bool _WriteRegion (const Message< DSMsg::WriteRegion > &);
		bool _ReleaseData (const Message< DSMsg::ReleaseData > &);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	FileDataOutput::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	FileDataOutput::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	FileDataOutput::FileDataOutput (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::DataOutput &ci) :
		Module( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes ),
		_filename{ ci.uri }
	{
		_SubscribeOnMsg( this, &FileDataOutput::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &FileDataOutput::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &FileDataOutput::_AttachModule_Empty );
		_SubscribeOnMsg( this, &FileDataOutput::_DetachModule_Empty );
		_SubscribeOnMsg( this, &FileDataOutput::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &FileDataOutput::_FindModule_Empty );
		_SubscribeOnMsg( this, &FileDataOutput::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &FileDataOutput::_Link_Impl );
		_SubscribeOnMsg( this, &FileDataOutput::_Compose );
		_SubscribeOnMsg( this, &FileDataOutput::_Delete );
		_SubscribeOnMsg( this, &FileDataOutput::_GetDataSourceDescriptor );
		_SubscribeOnMsg( this, &FileDataOutput::_WriteRegion_Empty );
		_SubscribeOnMsg( this, &FileDataOutput::_ReleaseData );

		CHECK( _ValidateMsgSubscriptions() );
		
		_AttachSelfToManager( ci.provider, LocalStorageDataProviderModuleID, true );
	}

/*
=================================================
	destructor
=================================================
*/
	FileDataOutput::~FileDataOutput ()
	{
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool FileDataOutput::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _GetManager() );
		
		Message< DSMsg::IsUriExists >	is_exist{ _filename };
		_GetManager()->Send( is_exist );

		CHECK_COMPOSING( is_exist->result.Get(false) );

		return Module::_DefCompose( true );
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool FileDataOutput::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_ReleaseData( {} );
		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_GetDataSourceDescriptor
=================================================
*/
	bool FileDataOutput::_GetDataSourceDescriptor (const Message< DSMsg::GetDataSourceDescriptor > &msg)
	{
		if ( not _IsComposedState( GetState() ) )
			return false;

		DataSourceDescriptor	descr;

		descr.memoryFlags	|= EMemoryAccess::CpuWrite;
		descr.available		 = BytesUL();	// data is not cached
		descr.totalSize		 = BytesUL( _file->Size() );

		msg->result.Set( descr );
		return true;
	}
	
/*
=================================================
	_WriteRegion_Empty
=================================================
*/
	bool FileDataOutput::_WriteRegion_Empty (const Message< DSMsg::WriteRegion > &msg)
	{
		if ( not _IsComposedState( GetState() ) )
			return false;

		Unsubscribe( this, &FileDataOutput::_WriteRegion_Empty );
		_SubscribeOnMsg( this, &FileDataOutput::_WriteRegion );
		
		Message< DSMsg::OpenFileForWrite >		open_file{ _filename };
		CHECK( _GetManager()->Send( open_file ) );

		_file = *open_file->result;
		CHECK_ERR( _file );

		return _WriteRegion( msg );
	}
	
/*
=================================================
	_WriteRegion
=================================================
*/
	bool FileDataOutput::_WriteRegion (const Message< DSMsg::WriteRegion > &msg)
	{
		CHECK( _file->SeekSet( BytesU(msg->position) ) );

		msg->wasWritten.Set( BytesUL(_file->Write( msg->data )) );
		return true;
	}
	
/*
=================================================
	_ReleaseData
=================================================
*/
	bool FileDataOutput::_ReleaseData (const Message< DSMsg::ReleaseData > &)
	{
		if ( _file ) {
			_file->Close();
			_file = null;
		}
		
		Unsubscribe( this, &FileDataOutput::_WriteRegion );
		_SubscribeOnMsg( this, &FileDataOutput::_WriteRegion_Empty );

		return true;
	}
//-----------------------------------------------------------------------------
	
/*
=================================================
	CreateFileDataOutput
=================================================
*/
	ModulePtr DataProviderObjectsConstructor::CreateFileDataOutput (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::DataOutput &ci)
	{
		return New< FileDataOutput >( id, gs, ci );
	}

}	// Base
}	// Engine
