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
		using SupportedMessages_t	= MessageListFrom<
											ModuleMsg::OnManagerChanged,
											DSMsg::GetDataSourceDescription,
											DSMsg::ReadMemRange,
											DSMsg::ReleaseData
										>;


	// constants
	private:
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
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);

		bool _GetDataSourceDescription (const DSMsg::GetDataSourceDescription &);
		bool _ReadMemRange_Empty (const DSMsg::ReadMemRange &);
		bool _ReadMemRange (const DSMsg::ReadMemRange &);
		bool _ReleaseData (const DSMsg::ReleaseData &);


		static BytesU	_MaxCacheSize ()	{ return 1_Kb; }
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	FileDataInput::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	FileDataInput::FileDataInput (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::DataInput &ci) :
		Module( gs, ModuleConfig{ id, UMax }, &_eventTypes ),
		_filename{ ci.uri }
	{
		SetDebugName( "FileDataInput: "_str << _filename );

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
		_SubscribeOnMsg( this, &FileDataInput::_GetDataSourceDescription );
		_SubscribeOnMsg( this, &FileDataInput::_ReadMemRange_Empty );
		_SubscribeOnMsg( this, &FileDataInput::_ReleaseData );

		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

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
	bool FileDataInput::_Compose (const ModuleMsg::Compose &)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _GetManager() );

		DSMsg::IsUriExists	is_exist{ _filename };
		_GetManager()->Send( is_exist );

		CHECK_COMPOSING( is_exist.result.Get(false) );

		return Module::_DefCompose( true );
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool FileDataInput::_Delete (const ModuleMsg::Delete &msg)
	{
		_ReleaseData( {} );

		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_GetDataSourceDescription
=================================================
*/
	bool FileDataInput::_GetDataSourceDescription (const DSMsg::GetDataSourceDescription &msg)
	{
		if ( not _IsComposedState( GetState() ) )
			return false;

		DataSourceDescription	descr;

		descr.memoryFlags	|= EMemoryAccess::CpuRead | EMemoryAccess::SequentialOnly;
		descr.available		 = 0_b;	// data is not cached
		descr.totalSize		 = _file->Size();

		msg.result.Set( descr );
		return true;
	}
	
/*
=================================================
	_ReadMemRange_Empty
=================================================
*/
	bool FileDataInput::_ReadMemRange_Empty (const DSMsg::ReadMemRange &msg)
	{
		if ( not _IsComposedState( GetState() ) )
			return false;

		Unsubscribe( this, &FileDataInput::_ReadMemRange_Empty );
		_SubscribeOnMsg( this, &FileDataInput::_ReadMemRange );

		DSMsg::OpenFileForRead	open_file{ _filename };
		CHECK( _GetManager()->Send( open_file ) );

		_file = *open_file.result;
		CHECK_ERR( _file );

		return _ReadMemRange( msg );
	}

/*
=================================================
	_ReadMemRange
=================================================
*/
	bool FileDataInput::_ReadMemRange (const DSMsg::ReadMemRange &msg)
	{
		BytesU	readn = _file->ReadBuf( msg.writableBuffer->RawPtr(), msg.writableBuffer->Size() );
		
		msg.result.Set( msg.writableBuffer->SubArray( 0, usize(readn) ) );
		return true;
	}
	
/*
=================================================
	_ReleaseData
=================================================
*/
	bool FileDataInput::_ReleaseData (const DSMsg::ReleaseData &)
	{
		if ( _file ) {
			_file->Close();
			_file = null;
		}
		
		Unsubscribe( this, &FileDataInput::_ReadMemRange );
		_SubscribeOnMsg( this, &FileDataInput::_ReadMemRange_Empty );

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
