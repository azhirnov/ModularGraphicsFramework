// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/Stream/InputStream.h"
#include "Engine/Base/Files/FileManager.h"

namespace Engine
{
namespace Base
{
	using namespace GXMath;
	
	const Runtime::VirtualTypeList	InputStream::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	InputStream::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	InputStream::InputStream (const GlobalSystemsRef gs, const CreateInfo::InStreamFromUri &ci) :
		Module( gs, GetStaticID(), &_msgTypes, &_eventTypes )
	{
		_SubscribeOnMsg( this, &InputStream::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &InputStream::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &InputStream::_AttachModule_Empty );
		_SubscribeOnMsg( this, &InputStream::_DetachModule_Empty );
		_SubscribeOnMsg( this, &InputStream::_FindModule_Empty );
		_SubscribeOnMsg( this, &InputStream::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &InputStream::_Delete_Impl );
		_SubscribeOnMsg( this, &InputStream::_GetFileStreamDescriptor );
		_SubscribeOnMsg( this, &InputStream::_ReadFromFileStream );

		CHECK( _ValidateMsgSubscriptions() );

		String	uri = RVREF( ci.uri.Get() );

		CHECK( gs->Get< FileManager >()->OpenForRead( uri, OUT _file ) );
		
		if ( _file )
		{
			_CreateCache();
		}
		else
			_SetState( EState::ComposingFailed );
	}
	
/*
=================================================
	constructor
=================================================
*/
	InputStream::InputStream (const GlobalSystemsRef gs, const CreateInfo::InStreamFromFile &ci) :
		Module( gs, GetStaticID(), &_msgTypes, &_eventTypes )
	{
		_SubscribeOnMsg( this, &InputStream::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &InputStream::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &InputStream::_AttachModule_Empty );
		_SubscribeOnMsg( this, &InputStream::_DetachModule_Empty );
		_SubscribeOnMsg( this, &InputStream::_FindModule_Empty );
		_SubscribeOnMsg( this, &InputStream::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &InputStream::_Delete_Impl );
		_SubscribeOnMsg( this, &InputStream::_GetFileStreamDescriptor );
		_SubscribeOnMsg( this, &InputStream::_ReadFromFileStream );
		
		CHECK( _ValidateMsgSubscriptions() );


		CHECK( ci.file );

		if ( ci.file )
		{
			_file = File::SubRFile::New( ci.file, ci.file->Pos(), ci.file->RemainingSize() );

			_CreateCache();
		}
		else
			_SetState( EState::ComposingFailed );
	}
	
/*
=================================================
	constructor
=================================================
*/
	InputStream::InputStream (const GlobalSystemsRef gs, const CreateInfo::InStreamFromMemory &ci) :
		Module( gs, GetStaticID(), &_msgTypes, &_eventTypes )
	{
		_SubscribeOnMsg( this, &InputStream::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &InputStream::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &InputStream::_AttachModule_Empty );
		_SubscribeOnMsg( this, &InputStream::_DetachModule_Empty );
		_SubscribeOnMsg( this, &InputStream::_FindModule_Empty );
		_SubscribeOnMsg( this, &InputStream::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &InputStream::_Delete_Impl );
		_SubscribeOnMsg( this, &InputStream::_GetMemStreamDescriptor );
		_SubscribeOnMsg( this, &InputStream::_ReadFromMemStream );
		
		CHECK( _ValidateMsgSubscriptions() );


		_cache = RVREF( ci.memory.Get() );

		if ( not _cache.Empty() )
		{
			CHECK( _SetState( EState::ComposedImmutable ) );
		}
		else
			_SetState( EState::ComposingFailed );
	}

/*
=================================================
	destructor
=================================================
*/
	InputStream::~InputStream ()
	{
	}
	
/*
=================================================
	_CreateCache
=================================================
*/
	void InputStream::_CreateCache ()
	{
		BytesU	cache_size = Min( _MaxCacheSize(), _file->Size() );

		_cache.Reserve( (usize) cache_size );

		CHECK( _SetState( EState::ComposedImmutable ) );
	}

/*
=================================================
	_GetFileStreamDescriptor
=================================================
*/
	bool InputStream::_GetFileStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &msg)
	{
		CHECK_ERR( _file and _file->IsOpened() );

		StreamDescriptor	descr;

		descr.memoryFlags	|= EMemoryAccess::CpuRead;
		descr.available		 = BytesUL();	// data is not cached
		descr.totalSize		 = BytesUL( _file->Size() );

		msg->result.Set( descr );
		return true;
	}
	
/*
=================================================
	_GetMemStreamDescriptor
=================================================
*/
	bool InputStream::_GetMemStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &msg)
	{
		StreamDescriptor	descr;

		descr.memoryFlags	|= EMemoryAccess::CpuRead;
		descr.available		 = BytesUL( _cache.Size() );
		descr.totalSize		 = descr.available;

		msg->result.Set( descr );
		return true;
	}
	
/*
=================================================
	_ReadFromFileStream
=================================================
*/
	bool InputStream::_ReadFromFileStream (const Message< ModuleMsg::ReadFromStream > &msg)
	{
		CHECK_ERR( _file and _file->IsOpened() );

		const BytesU	offset	= Min( _file->Size(), BytesU(msg->offset) );

		CHECK( _file->SeekSet( offset ) );

		const BytesU	size	= Min( _cache.Size(), BytesU( msg->size.Get(BytesUL(-1)) ) );
		const usize		readn	= (usize) _file->ReadBuf( _cache.ptr(), size );

		msg->result.Set( _cache.SubArray( 0, readn ) );

		_SendEvent( Message< ModuleMsg::DataRegionChanged >{ this, EMemoryAccess::CpuRead, offset, BytesU(readn) } );
		return true;
	}
	
/*
=================================================
	_ReadFromMemStream
=================================================
*/
	bool InputStream::_ReadFromMemStream (const Message< ModuleMsg::ReadFromStream > &msg)
	{
		const BytesUL	cache_size	= BytesUL( _cache.Size() );
		const BytesUL	offset		= Min( cache_size, msg->offset );
		const BytesUL	size		= Min( cache_size - offset, msg->size.Get( BytesUL(-1) ) );

		msg->result.Set( _cache.SubArray( usize(offset), usize(size) ) );

		_SendEvent( Message< ModuleMsg::DataRegionChanged >{ this, EMemoryAccess::CpuRead, offset, size } );
		return true;
	}


}	// Base
}	// Engine
