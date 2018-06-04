// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/DataProvider/DataMessages.h"
#include "Engine/Base/DataProvider/DataProviderObjectsConstructor.h"
#include "Engine/Base/Modules/Module.h"

namespace Engine
{
namespace Base
{
	
	//
	// Input File Stream Module
	//

	class FileInputStream final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Update
										> >::Append< MessageListFrom< 
											ModuleMsg::OnManagerChanged,
											DSMsg::GetDataSourceDescriptor,
											DSMsg::ReadFromStream,
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
		//BinaryArray			_cache;
		//BytesU				_cachePos;		// cached region in file is [_cachePos, _cachePos + _cache.Size()]
		//BytesU				_pos;			// current reading position


	// methods
	public:
		FileInputStream (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::InputStream &ci);
		~FileInputStream ();


	// message handlers
	private:
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);

		bool _GetDataSourceDescriptor (const Message< DSMsg::GetDataSourceDescriptor > &);
		bool _ReadFromStream_Empty (const Message< DSMsg::ReadFromStream > &);
		bool _ReadFromStream (const Message< DSMsg::ReadFromStream > &);
		bool _ReleaseData (const Message< DSMsg::ReleaseData > &);

		static BytesU	_MaxCacheSize ()	{ return 1_Kb; }
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	FileInputStream::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	FileInputStream::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	FileInputStream::FileInputStream (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::InputStream &ci) :
		Module( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes ),
		_filename{ ci.uri }
	{
		_SubscribeOnMsg( this, &FileInputStream::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &FileInputStream::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &FileInputStream::_AttachModule_Empty );
		_SubscribeOnMsg( this, &FileInputStream::_DetachModule_Empty );
		_SubscribeOnMsg( this, &FileInputStream::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &FileInputStream::_FindModule_Empty );
		_SubscribeOnMsg( this, &FileInputStream::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &FileInputStream::_Link_Impl );
		_SubscribeOnMsg( this, &FileInputStream::_Compose );
		_SubscribeOnMsg( this, &FileInputStream::_Delete );
		_SubscribeOnMsg( this, &FileInputStream::_GetDataSourceDescriptor );
		_SubscribeOnMsg( this, &FileInputStream::_ReadFromStream_Empty );
		_SubscribeOnMsg( this, &FileInputStream::_ReleaseData );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.provider, LocalStorageDataProviderModuleID, true );
	}

/*
=================================================
	destructor
=================================================
*/
	FileInputStream::~FileInputStream ()
	{
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool FileInputStream::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _GetManager() );
		
		Message< DSMsg::IsUriExists >	is_exist{ _filename };
		_GetManager()->Send( is_exist );

		CHECK_COMPOSING( is_exist->result.Get(false) );

		// create cache
		/*BytesU	fsize		= _file->RemainingSize();
		BytesU	cache_size;

		if ( fsize < 512_b )		cache_size = 512_b;	else
		if ( fsize < 1_Kb )			cache_size = 1_Kb;

		_cache.Resize( usize(cache_size), false );
		_pos = 0_b;*/

		return Module::_DefCompose( true );
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool FileInputStream::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_ReleaseData( {} );

		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_GetDataSourceDescriptor
=================================================
*/
	bool FileInputStream::_GetDataSourceDescriptor (const Message< DSMsg::GetDataSourceDescriptor > &msg)
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
	_ReadFromStream_Empty
=================================================
*/
	bool FileInputStream::_ReadFromStream_Empty (const Message< DSMsg::ReadFromStream > &msg)
	{
		if ( not _IsComposedState( GetState() ) )
			return false;

		Unsubscribe( this, &FileInputStream::_ReadFromStream_Empty );
		_SubscribeOnMsg( this, &FileInputStream::_ReadFromStream );

		Message< DSMsg::OpenFileForRead >	open_file{ _filename };
		CHECK( _GetManager()->Send( open_file ) );

		_file = *open_file->result;
		CHECK_ERR( _file );

		return _ReadFromStream( msg );
	}

/*
=================================================
	_ReadFromStream
=================================================
*/
	bool FileInputStream::_ReadFromStream (const Message< DSMsg::ReadFromStream > &msg)
	{
		BytesU	readn = _file->ReadBuf( msg->writableBuffer->RawPtr(), msg->writableBuffer->Size() );

		msg->result.Set( msg->writableBuffer->SubArray( 0, usize(readn) ) );

		/*BytesU	size	= msg->writableBuffer->Size();
		BytesU	readn;

		_pos += BytesU(msg->offset);

		// is full or partialy cached
		if ( _pos < _cachePos + _cache.Size() and _pos > _cachePos )
		{
			BinArrayCRef	cached = _cache.SubArray( usize(_pos - _cachePos), usize(size) );

			MemCopy( *msg->writableBuffer, cached );

			_pos	+= cached.Size();
			readn	+= cached.Size();
			size	-= readn;
		}

		// read to cache
		if ( size > 0 and size < _cache.FullSize() )
		{
			_cachePos	= _pos;
			_cache.Resize( usize(_file->ReadBuf( _cache.RawPtr(), _cache.FullSize() )), false );

			BinArrayCRef	cached	= _cache.SubArray( 0, usize(size) );
			
			MemCopy( msg->writableBuffer->SubArray( usize(readn) ), cached );

			readn += cached.Size();
		}
		else
		// read to buffer
		if ( size > 0 )
		{
			BinArrayRef		data = msg->writableBuffer->SubArray( usize(readn) );

			readn += _file->ReadBuf( data.RawPtr(), data.Size() );
		}

		msg->result.Set( msg->writableBuffer->SubArray( 0, usize(readn) ) );*/
		return true;
	}
	
/*
=================================================
	_ReleaseData
=================================================
*/
	bool FileInputStream::_ReleaseData (const Message< DSMsg::ReleaseData > &)
	{
		if ( _file ) {
			_file->Close();
			_file = null;
		}
		
		//_cache.Free();

		Unsubscribe( this, &FileInputStream::_ReadFromStream );
		_SubscribeOnMsg( this, &FileInputStream::_ReadFromStream_Empty );

		return true;
	}
//-----------------------------------------------------------------------------
	
/*
=================================================
	CreateFileInputStream
=================================================
*/
	ModulePtr DataProviderObjectsConstructor::CreateFileInputStream (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::InputStream &ci)
	{
		return New< FileInputStream >( id, gs, ci );
	}

}	// Base
}	// Engine
