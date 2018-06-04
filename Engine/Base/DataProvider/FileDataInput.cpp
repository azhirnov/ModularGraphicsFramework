// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/DataProvider/DataMessages.h"
#include "Engine/Base/DataProvider/DataProviderObjectsConstructor.h"
#include "Engine/Base/Modules/Module.h"

namespace Engine
{
namespace Base
{
	
	//
	// File Data Input Module
	//

	class FileDataInput final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Update
										> >::Append< MessageListFrom<
											ModuleMsg::OnManagerChanged,
											DSMsg::GetDataSourceDescriptor,
											DSMsg::ReadRegion,
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
		GXFile::RFilePtr	_file;


	// methods
	public:
		FileDataInput (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::DataInput &ci);
		~FileDataInput ();


	// message handlers
	private:
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);

		bool _GetDataSourceDescriptor (const Message< DSMsg::GetDataSourceDescriptor > &);
		bool _ReadRegion_Empty (const Message< DSMsg::ReadRegion > &);
		bool _ReadRegion (const Message< DSMsg::ReadRegion > &);
		bool _ReleaseData (const Message< DSMsg::ReleaseData > &);


		static BytesU	_MaxCacheSize ()	{ return 1_Kb; }
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	FileDataInput::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	FileDataInput::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	FileDataInput::FileDataInput (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::DataInput &ci) :
		Module( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes ),
		_filename{ ci.uri }
	{
		_SubscribeOnMsg( this, &FileDataInput::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &FileDataInput::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &FileDataInput::_AttachModule_Empty );
		_SubscribeOnMsg( this, &FileDataInput::_DetachModule_Empty );
		_SubscribeOnMsg( this, &FileDataInput::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &FileDataInput::_FindModule_Empty );
		_SubscribeOnMsg( this, &FileDataInput::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &FileDataInput::_Link_Impl );
		_SubscribeOnMsg( this, &FileDataInput::_Compose );
		_SubscribeOnMsg( this, &FileDataInput::_Delete );
		_SubscribeOnMsg( this, &FileDataInput::_GetDataSourceDescriptor );
		_SubscribeOnMsg( this, &FileDataInput::_ReadRegion_Empty );
		_SubscribeOnMsg( this, &FileDataInput::_ReleaseData );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.provider, LocalStorageDataProviderModuleID, true );
	}

/*
=================================================
	destructor
=================================================
*/
	FileDataInput::~FileDataInput ()
	{
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool FileDataInput::_Compose (const Message< ModuleMsg::Compose > &msg)
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
	bool FileDataInput::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_ReleaseData( {} );

		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_GetDataSourceDescriptor
=================================================
*/
	bool FileDataInput::_GetDataSourceDescriptor (const Message< DSMsg::GetDataSourceDescriptor > &msg)
	{
		if ( not _IsComposedState( GetState() ) )
			return false;

		DataSourceDescriptor	descr;

		descr.memoryFlags	|= EMemoryAccess::CpuRead | EMemoryAccess::SequentialOnly;
		descr.available		 = BytesUL();	// data is not cached
		descr.totalSize		 = BytesUL( _file->Size() );

		msg->result.Set( descr );
		return true;
	}
	
/*
=================================================
	_ReadRegion_Empty
=================================================
*/
	bool FileDataInput::_ReadRegion_Empty (const Message< DSMsg::ReadRegion > &msg)
	{
		if ( not _IsComposedState( GetState() ) )
			return false;

		Unsubscribe( this, &FileDataInput::_ReadRegion_Empty );
		_SubscribeOnMsg( this, &FileDataInput::_ReadRegion );

		Message< DSMsg::OpenFileForRead >	open_file{ _filename };
		CHECK( _GetManager()->Send( open_file ) );

		_file = *open_file->result;
		CHECK_ERR( _file );

		return _ReadRegion( msg );
	}

/*
=================================================
	_ReadRegion
=================================================
*/
	bool FileDataInput::_ReadRegion (const Message< DSMsg::ReadRegion > &msg)
	{
		BytesU	readn = _file->ReadBuf( msg->writableBuffer->RawPtr(), msg->writableBuffer->Size() );
		
		msg->result.Set( msg->writableBuffer->SubArray( 0, usize(readn) ) );
		return true;
	}
	
/*
=================================================
	_ReleaseData
=================================================
*/
	bool FileDataInput::_ReleaseData (const Message< DSMsg::ReleaseData > &msg)
	{
		if ( _file ) {
			_file->Close();
			_file = null;
		}
		
		Unsubscribe( this, &FileDataInput::_ReadRegion );
		_SubscribeOnMsg( this, &FileDataInput::_ReadRegion_Empty );

		return true;
	}
//-----------------------------------------------------------------------------
	
/*
=================================================
	CreateFileDataInput
=================================================
*/
	ModulePtr DataProviderObjectsConstructor::CreateFileDataInput (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::DataInput &ci)
	{
		return New< FileDataInput >( id, gs, ci );
	}

}	// Base
}	// Engine
