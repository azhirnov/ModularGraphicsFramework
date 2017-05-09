// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/Stream/OutputStream.h"
#include "Engine/Base/Files/FileManager.h"

namespace Engine
{
namespace Base
{
	using namespace GXMath;
	
	const Runtime::VirtualTypeList	OutputStream::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	OutputStream::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	OutputStream::OutputStream (const GlobalSystemsRef gs, const CreateInfo::OutStreamFromUri &ci) :
		Module( gs, ModuleConfig{ GetStaticID(), ~0u }, &_msgTypes, &_eventTypes )
	{
		_SubscribeOnMsg( this, &OutputStream::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &OutputStream::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &OutputStream::_AttachModule_Empty );
		_SubscribeOnMsg( this, &OutputStream::_DetachModule_Empty );
		_SubscribeOnMsg( this, &OutputStream::_FindModule_Empty );
		_SubscribeOnMsg( this, &OutputStream::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &OutputStream::_Delete_Impl );
		_SubscribeOnMsg( this, &OutputStream::_GetFileStreamDescriptor );
		_SubscribeOnMsg( this, &OutputStream::_WriteToFileStream );

		CHECK( _ValidateMsgSubscriptions() );

		String	uri = RVREF( ci.uri.Get() );

		CHECK( gs->Get< FileManager >()->OpenForWrite( uri, OUT _file ) );
		
		if ( _file )
		{
			CHECK( _SetState( EState::ComposedImmutable ) );
		}
		else
			_SetState( EState::ComposingFailed );
	}
	
/*
=================================================
	constructor
=================================================
*/
	OutputStream::OutputStream (const GlobalSystemsRef gs, const CreateInfo::OutStreamFromFile &ci) :
		Module( gs, ModuleConfig{ GetStaticID(), ~0u }, &_msgTypes, &_eventTypes )
	{
		_SubscribeOnMsg( this, &OutputStream::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &OutputStream::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &OutputStream::_AttachModule_Empty );
		_SubscribeOnMsg( this, &OutputStream::_DetachModule_Empty );
		_SubscribeOnMsg( this, &OutputStream::_FindModule_Empty );
		_SubscribeOnMsg( this, &OutputStream::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &OutputStream::_Delete_Impl );
		_SubscribeOnMsg( this, &OutputStream::_GetFileStreamDescriptor );
		_SubscribeOnMsg( this, &OutputStream::_WriteToFileStream );
		
		CHECK( _ValidateMsgSubscriptions() );

		CHECK( ci.file );

		if ( ci.file )
		{
			_file = File::SubWFile::New( ci.file, ci.file->Pos(), ci.file->RemainingSize() );
			
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
	OutputStream::~OutputStream ()
	{
	}

/*
=================================================
	_GetFileStreamDescriptor
=================================================
*/
	bool OutputStream::_GetFileStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &msg)
	{
		CHECK_ERR( _file and _file->IsOpened() );

		StreamDescriptor	descr;

		descr.memoryFlags	|= EMemoryAccess::CpuWrite;
		descr.available		 = BytesUL();	// data is not cached
		descr.totalSize		 = BytesUL( _file->Size() );

		msg->result.Set( descr );
		return true;
	}
	
/*
=================================================
	_GetMemStreamDescriptor
=================================================
*
	bool OutputStream::_GetMemStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &msg)
	{
		StreamDescriptor	descr;

		descr.memoryFlags	|= EMemoryAccess::CpuWrite;
		descr.available		 = BytesUL();
		descr.totalSize		 = BytesUL( _cache.Size() );

		msg->result.Set( descr );
		return true;
	}
	
/*
=================================================
	_WriteToFileStream
=================================================
*/
	bool OutputStream::_WriteToFileStream (const Message< ModuleMsg::WriteToStream > &msg)
	{
		CHECK_ERR( _file and _file->IsOpened() );

		const BytesU	offset	= Min( _file->Size(), BytesU(msg->offset) );

		CHECK( _file->SeekSet( offset ) );

		const BytesU	size	= Min( _cache.Size(), msg->data.Size() );
		const BytesU	written	= _file->WriteBuf( _cache.ptr(), size );

		msg->wasWritten.Set( BytesUL( written ) );

		_SendEvent( Message< ModuleMsg::DataRegionChanged >{ this, EMemoryAccess::CpuWrite, offset, written } );
		return true;
	}
	
/*
=================================================
	_WriteToMemStream
=================================================
*
	bool OutputStream::_WriteToMemStream (const Message< ModuleMsg::WriteToStream > &msg)
	{
		const BytesU	cache_size	= _cache.Size();
		const BytesU	offset		= Min( cache_size, BytesUL( msg->offset ) );
		const BytesU	size		= Min( cache_size - offset, msg->data.Size() );

		MemCopy( _cache.SubArray( usize(offset), usize(size) ), msg->data );
		return true;
	}
	*/

}	// Base
}	// Engine
