// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/DataProvider/DataMessages.h"
#include "Engine/Base/DataProvider/DataProviderObjectsConstructor.h"
#include "Engine/Base/Modules/Module.h"

namespace Engine
{
namespace Base
{
	
	//
	// Output File Stream Module
	//

	class FileOutputStream final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= MessageListFrom< 
											ModuleMsg::OnManagerChanged,
											DSMsg::GetDataSourceDescription,
											DSMsg::WriteToStream,
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
		FileOutputStream (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::OutputStream &ci);
		~FileOutputStream ();


	// message handlers
	private:
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);

		bool _GetDataSourceDescription (const DSMsg::GetDataSourceDescription &);
		bool _WriteToStream_Empty (const DSMsg::WriteToStream &);
		bool _WriteToStream (const DSMsg::WriteToStream &);
		bool _ReleaseData (const DSMsg::ReleaseData &);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	FileOutputStream::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	FileOutputStream::FileOutputStream (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::OutputStream &ci) :
		Module( gs, ModuleConfig{ id, UMax }, &_eventTypes ),
		_filename{ ci.uri }
	{
		SetDebugName( "FileOutputStream: "_str << _filename );

		_SubscribeOnMsg( this, &FileOutputStream::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &FileOutputStream::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &FileOutputStream::_AttachModule_Empty );
		_SubscribeOnMsg( this, &FileOutputStream::_DetachModule_Empty );
		_SubscribeOnMsg( this, &FileOutputStream::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &FileOutputStream::_FindModule_Empty );
		_SubscribeOnMsg( this, &FileOutputStream::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &FileOutputStream::_Link_Impl );
		_SubscribeOnMsg( this, &FileOutputStream::_Compose );
		_SubscribeOnMsg( this, &FileOutputStream::_Delete );
		_SubscribeOnMsg( this, &FileOutputStream::_GetDataSourceDescription );
		_SubscribeOnMsg( this, &FileOutputStream::_WriteToStream_Empty );
		_SubscribeOnMsg( this, &FileOutputStream::_ReleaseData );

		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );
		
		_AttachSelfToManager( ci.provider, LocalStorageDataProviderModuleID, true );
	}

/*
=================================================
	destructor
=================================================
*/
	FileOutputStream::~FileOutputStream ()
	{
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool FileOutputStream::_Compose (const ModuleMsg::Compose &)
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
	bool FileOutputStream::_Delete (const ModuleMsg::Delete &msg)
	{
		_ReleaseData( {} );

		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_GetDataSourceDescription
=================================================
*/
	bool FileOutputStream::_GetDataSourceDescription (const DSMsg::GetDataSourceDescription &msg)
	{
		if ( not _IsComposedState( GetState() ) )
			return false;

		DataSourceDescription	descr;

		descr.memoryFlags	|= EMemoryAccess::CpuWrite | EMemoryAccess::SequentialOnly;
		descr.available		 = 0_b;	// data is not cached
		descr.totalSize		 = _file->Size();

		msg.result.Set( descr );
		return true;
	}
	
/*
=================================================
	_WriteToStream_Empty
=================================================
*/
	bool FileOutputStream::_WriteToStream_Empty (const DSMsg::WriteToStream &msg)
	{
		if ( not _IsComposedState( GetState() ) )
			return false;

		Unsubscribe( this, &FileOutputStream::_WriteToStream_Empty );
		_SubscribeOnMsg( this, &FileOutputStream::_WriteToStream );
		
		DSMsg::OpenFileForWrite		open_file{ _filename };
		CHECK( _GetManager()->Send( open_file ) );

		_file = *open_file.result;
		CHECK_ERR( _file );

		return _WriteToStream( msg );
	}
	
/*
=================================================
	_WriteToStream
=================================================
*/
	bool FileOutputStream::_WriteToStream (const DSMsg::WriteToStream &msg)
	{
		msg.wasWritten.Set( _file->WriteBuf( msg.data.RawPtr(), msg.data.Size() ) );

		return true;
	}
	
/*
=================================================
	_ReleaseData
=================================================
*/
	bool FileOutputStream::_ReleaseData (const DSMsg::ReleaseData &)
	{
		if ( _file ) {
			_file->Close();
			_file = null;
		}
		
		Unsubscribe( this, &FileOutputStream::_WriteToStream );
		_SubscribeOnMsg( this, &FileOutputStream::_WriteToStream_Empty );

		return true;
	}
//-----------------------------------------------------------------------------
	
/*
=================================================
	CreateFileOutputStream
=================================================
*/
	ModulePtr DataProviderObjectsConstructor::CreateFileOutputStream (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::OutputStream &ci)
	{
		return New< FileOutputStream >( id, gs, ci );
	}

}	// Base
}	// Engine
