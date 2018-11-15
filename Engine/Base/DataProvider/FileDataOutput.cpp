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
		using SupportedMessages_t	= MessageListFrom< 
											ModuleMsg::OnManagerChanged,
											DSMsg::GetDataSourceDescription,
											DSMsg::WriteMemRange,
											DSMsg::ReleaseData
										>;


	// constants
	private:
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
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);

		bool _GetDataSourceDescription (const DSMsg::GetDataSourceDescription &);
		bool _WriteMemRange_Empty (const DSMsg::WriteMemRange &);
		bool _WriteMemRange (const DSMsg::WriteMemRange &);
		bool _ReleaseData (const DSMsg::ReleaseData &);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	FileDataOutput::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	FileDataOutput::FileDataOutput (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::DataOutput &ci) :
		Module( gs, ModuleConfig{ id, UMax }, &_eventTypes ),
		_filename{ ci.uri }
	{
		SetDebugName( "FileDataOutput: "_str << _filename );

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
		_SubscribeOnMsg( this, &FileDataOutput::_GetDataSourceDescription );
		_SubscribeOnMsg( this, &FileDataOutput::_WriteMemRange_Empty );
		_SubscribeOnMsg( this, &FileDataOutput::_ReleaseData );

		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );
		
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
	bool FileDataOutput::_Compose (const ModuleMsg::Compose &)
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
	bool FileDataOutput::_Delete (const ModuleMsg::Delete &msg)
	{
		_ReleaseData( {} );
		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_GetDataSourceDescription
=================================================
*/
	bool FileDataOutput::_GetDataSourceDescription (const DSMsg::GetDataSourceDescription &msg)
	{
		if ( not _IsComposedState( GetState() ) )
			return false;

		DataSourceDescription	descr;

		descr.memoryFlags	|= EMemoryAccess::CpuWrite;
		descr.available		 = 0_b;	// data is not cached
		descr.totalSize		 = _file->Size();

		msg.result.Set( descr );
		return true;
	}
	
/*
=================================================
	_WriteMemRange_Empty
=================================================
*/
	bool FileDataOutput::_WriteMemRange_Empty (const DSMsg::WriteMemRange &msg)
	{
		if ( not _IsComposedState( GetState() ) )
			return false;

		Unsubscribe( this, &FileDataOutput::_WriteMemRange_Empty );
		_SubscribeOnMsg( this, &FileDataOutput::_WriteMemRange );
		
		DSMsg::OpenFileForWrite		open_file{ _filename };
		CHECK( _GetManager()->Send( open_file ) );

		_file = *open_file.result;
		CHECK_ERR( _file );

		return _WriteMemRange( msg );
	}
	
/*
=================================================
	_WriteMemRange
=================================================
*/
	bool FileDataOutput::_WriteMemRange (const DSMsg::WriteMemRange &msg)
	{
		CHECK( _file->SeekSet( msg.position ) );

		msg.wasWritten.Set( _file->WriteBuf( msg.data.RawPtr(), msg.data.Size() ) );
		return true;
	}
	
/*
=================================================
	_ReleaseData
=================================================
*/
	bool FileDataOutput::_ReleaseData (const DSMsg::ReleaseData &)
	{
		if ( _file ) {
			_file->Close();
			_file = null;
		}
		
		Unsubscribe( this, &FileDataOutput::_WriteMemRange );
		_SubscribeOnMsg( this, &FileDataOutput::_WriteMemRange_Empty );

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
