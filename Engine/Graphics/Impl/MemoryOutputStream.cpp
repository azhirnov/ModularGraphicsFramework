// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#if 0
#include "Engine/Graphics/Public/Common.h"

namespace Engine
{
namespace Graphics
{	
	
	//
	// Image Output Stream Module
	//

	class ImageOutputStream final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= MessageListFrom< 
											ModuleMsg::AttachModule,
											ModuleMsg::DetachModule,
											ModuleMsg::OnModuleAttached,
											ModuleMsg::OnModuleDetached,
											ModuleMsg::FindModule,
											ModuleMsg::ModulesDeepSearch,
											ModuleMsg::Delete,
											DSMsg::GetDataSourceDescriptor,
											DSMsg::WriteRegion
										>;

		using SupportedEvents_t		= MessageListFrom< 
											ModuleMsg::DataRegionChanged,
											ModuleMsg::Delete
										>;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		WFilePtr		_file;
		BinaryArray		_cache;


	// methods
	public:
		ImageOutputStream (GlobalSystemsRef gs, const CreateInfo::OutStreamFromUri &ci);
		ImageOutputStream (GlobalSystemsRef gs, const CreateInfo::OutStreamFromFile &ci);
		~ImageOutputStream ();


	// message handlers
	private:
		bool _GetFileStreamDescriptor (const Message< DSMsg::GetDataSourceDescriptor > &);
		bool _GetMemStreamDescriptor (const Message< DSMsg::GetDataSourceDescriptor > &);
		bool _WriteToFileStream (const Message< DSMsg::WriteRegion > &);
		bool _WriteToMemStream (const Message< DSMsg::WriteRegion > &);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	OutputStream::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	OutputStream::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	OutputStream::OutputStream (GlobalSystemsRef gs, const CreateInfo::OutStreamFromUri &ci) :
		Module( gs, ModuleConfig{ OutputStreamModuleID, ~0u }, &_msgTypes, &_eventTypes )
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

		CHECK( gs->fileManager->OpenForWrite( uri, OUT _file ) );
		
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
	OutputStream::OutputStream (GlobalSystemsRef gs, const CreateInfo::OutStreamFromFile &ci) :
		Module( gs, ModuleConfig{ OutputStreamModuleID, ~0u }, &_msgTypes, &_eventTypes )
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
			_file = GXFile::SubWFile::New( ci.file, ci.file->Pos(), ci.file->RemainingSize() );
			
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
	bool OutputStream::_GetFileStreamDescriptor (const Message< DSMsg::GetDataSourceDescriptor > &msg)
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
	bool OutputStream::_GetMemStreamDescriptor (const Message< DSMsg::GetDataSourceDescriptor > &msg)
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
	bool OutputStream::_WriteToFileStream (const Message< DSMsg::WriteRegion > &msg)
	{
		CHECK_ERR( _file and _file->IsOpened() );

		const BytesU	offset	= Min( _file->Size(), BytesU(msg->offset) );

		CHECK( _file->SeekSet( offset ) );

		const BytesU	size	= Min( _cache.Size(), msg->data.Size() );
		const BytesU	written	= _file->WriteBuf( _cache.ptr(), size );

		msg->wasWritten.Set( BytesUL( written ) );

		_SendEvent< ModuleMsg::DataRegionChanged >({ EMemoryAccess::CpuWrite, offset, written });
		return true;
	}
	
/*
=================================================
	_WriteToMemStream
=================================================
*
	bool OutputStream::_WriteToMemStream (const Message< DSMsg::WriteRegion > &msg)
	{
		const BytesU	cache_size	= _cache.Size();
		const BytesU	offset		= Min( cache_size, BytesUL( msg->offset ) );
		const BytesU	size		= Min( cache_size - offset, msg->data.Size() );

		MemCopy( _cache.SubArray( usize(offset), usize(size) ), msg->data );
		return true;
	}
	*/
//-----------------------------------------------------------------------------
	
/*
=================================================
	_CreateOutputStream*
=================================================
*/
	ModulePtr StreamManager::_CreateOutStreamFromFile (GlobalSystemsRef gs, const CreateInfo::OutStreamFromFile &ci)
	{
		return New< OutputStream >( gs, ci );
	}
	
	ModulePtr StreamManager::_CreateOutStreamFromUri (GlobalSystemsRef gs, const CreateInfo::OutStreamFromUri &ci)
	{
		return New< OutputStream >( gs, ci );
	}

}	// Graphics
}	// Engine
#endif