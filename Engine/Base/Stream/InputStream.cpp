// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/Stream/Stream.h"
#include "Engine/Base/Modules/Module.h"
#include "Engine/Base/Files/FileManager.h"
#include "Engine/Base/Stream/StreamManager.h"

namespace Engine
{
namespace Base
{
	using namespace GXMath;
	
	
	//
	// Input Stream Module
	//

	class InputStream final : public Module
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
											ModuleMsg::GetStreamDescriptor,
											ModuleMsg::ReadFromStream
										>;

		using SupportedEvents_t		= MessageListFrom<
											ModuleMsg::DataRegionChanged,
											ModuleMsg::Delete
										>;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		File::RFilePtr	_file;
		BinaryArray		_cache;


	// methods
	public:
		InputStream (const GlobalSystemsRef gs, const CreateInfo::InStreamFromUri &ci);
		InputStream (const GlobalSystemsRef gs, const CreateInfo::InStreamFromFile &ci);
		InputStream (const GlobalSystemsRef gs, const CreateInfo::InStreamFromMemory &ci);
		~InputStream ();


	// message handlers
	private:
		bool _GetFileStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &);
		bool _GetMemStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &);
		bool _ReadFromFileStream (const Message< ModuleMsg::ReadFromStream > &);
		bool _ReadFromMemStream (const Message< ModuleMsg::ReadFromStream > &);

	private:
		void _CreateCache ();

		static BytesU	_MaxCacheSize ()	{ return BytesU::FromMb( 1 ); }
	};
//-----------------------------------------------------------------------------


	
	const Runtime::VirtualTypeList	InputStream::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	InputStream::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	InputStream::InputStream (const GlobalSystemsRef gs, const CreateInfo::InStreamFromUri &ci) :
		Module( gs, ModuleConfig{ InputStreamModuleID, ~0u }, &_msgTypes, &_eventTypes )
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
		Module( gs, ModuleConfig{ InputStreamModuleID, ~0u }, &_msgTypes, &_eventTypes )
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
		Module( gs, ModuleConfig{ InputStreamModuleID, ~0u }, &_msgTypes, &_eventTypes )
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

		_SendEvent< ModuleMsg::DataRegionChanged >({ EMemoryAccess::CpuRead, offset, BytesU(readn) });
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

		_SendEvent< ModuleMsg::DataRegionChanged >({ EMemoryAccess::CpuRead, offset, size });
		return true;
	}
//-----------------------------------------------------------------------------
	
	
/*
=================================================
	_CreateInputStream*
=================================================
*/
	ModulePtr StreamManager::_CreateInStreamFromFile (GlobalSystemsRef gs, const CreateInfo::InStreamFromFile &ci)
	{
		return New< InputStream >( gs, ci );
	}
	
	ModulePtr StreamManager::_CreateInStreamFromUri (GlobalSystemsRef gs, const CreateInfo::InStreamFromUri &ci)
	{
		return New< InputStream >( gs, ci );
	}

	ModulePtr StreamManager::_CreateInStreamFromMemory (GlobalSystemsRef gs, const CreateInfo::InStreamFromMemory &ci)
	{
		return New< InputStream >( gs, ci );
	}

}	// Base
}	// Engine
