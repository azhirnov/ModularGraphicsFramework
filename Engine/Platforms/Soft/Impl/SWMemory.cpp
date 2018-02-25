// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Memory.h"
#include "Engine/Platforms/Shared/GPU/Image.h"
#include "Engine/Platforms/Shared/GPU/Buffer.h"
#include "Engine/Platforms/Shared/Tools/MemoryMapperHelper.h"
#include "Engine/Platforms/Soft/Impl/SWBaseModule.h"
#include "Engine/Platforms/Soft/SoftRendererObjectsConstructor.h"

#ifdef GRAPHICS_API_SOFT

namespace Engine
{
namespace PlatformSW
{

	//
	// Software Renderer Memory
	//

	class SWMemory final : public SWBaseModule
	{
	// types
	private:
		using SupportedMessages_t	= SWBaseModule::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::GetStreamDescriptor,
											ModuleMsg::ReadFromStream,
											ModuleMsg::WriteToStream,
											GpuMsg::ReadFromGpuMemory,
											GpuMsg::WriteToGpuMemory,
											GpuMsg::ReadFromImageMemory,
											GpuMsg::WriteToImageMemory,
											GpuMsg::MapMemoryToCpu,
											GpuMsg::MapImageToCpu,
											GpuMsg::FlushMemoryRange,
											GpuMsg::UnmapMemory,
											GpuMsg::GetGpuMemoryDescriptor,
											GpuMsg::GpuMemoryRegionChanged,
											GpuMsg::GetSWMemoryData
										> >;

		using SupportedEvents_t		= SWBaseModule::SupportedEvents_t::Append< MessageListFrom<
											ModuleMsg::DataRegionChanged,
											GpuMsg::OnMemoryBindingChanged
										> >;
		
		using ImageMessages_t		= MessageListFrom<
											GpuMsg::GetSWImageMemoryLayout,
											GpuMsg::GetSWImageMemoryRequirements
										>;

		using BufferMessages_t		= MessageListFrom<
											GpuMsg::GetSWBufferMemoryRequirements
										>;

		using EBindingTarget		= GpuMsg::OnMemoryBindingChanged::EBindingTarget;
		using MemMapper_t			= PlatformTools::MemoryMapperHelper;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		BinaryArray			_memory;
		BinArrayRef			_usedMemory;
		BytesU				_align;

		MemMapper_t			_memMapper;
		EGpuMemory::bits	_flags;
		EBindingTarget		_binding;


	// methods
	public:
		SWMemory (GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci);
		~SWMemory ();


	// message handlers
	private:
		bool _GetGpuMemoryDescriptor (const Message< GpuMsg::GetGpuMemoryDescriptor > &);
		bool _GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &);
		bool _GetStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &);
		bool _ReadFromImageMemory (const Message< GpuMsg::ReadFromImageMemory > &);
		bool _WriteToImageMemory (const Message< GpuMsg::WriteToImageMemory > &);
		bool _ReadFromGpuMemory (const Message< GpuMsg::ReadFromGpuMemory > &) const;
		bool _WriteToGpuMemory (const Message< GpuMsg::WriteToGpuMemory > &);
		bool _ReadFromStream (const Message< ModuleMsg::ReadFromStream > &);
		bool _WriteToStream (const Message< ModuleMsg::WriteToStream > &);
		bool _MapMemoryToCpu (const Message< GpuMsg::MapMemoryToCpu > &);
		bool _MapImageToCpu (const Message< GpuMsg::MapImageToCpu > &);
		bool _FlushMemoryRange (const Message< GpuMsg::FlushMemoryRange > &);
		bool _UnmapMemory (const Message< GpuMsg::UnmapMemory > &);
		bool _GetSWMemoryData (const Message< GpuMsg::GetSWMemoryData > &);

		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);

	private:
		bool _IsCreated () const;
		bool _IsMapped () const;

		bool _AllocForImage ();
		bool _AllocForBuffer ();
		void _FreeMemory ();

		static usize _GetAlignedOffset (ubyte *ptr, BytesU align);
	};
//-----------------------------------------------------------------------------



	const TypeIdList	SWMemory::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	SWMemory::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SWMemory::SWMemory (GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci) :
		SWBaseModule( gs, ModuleConfig{ SWMemoryModuleID, 1 }, &_msgTypes, &_eventTypes ),
		_memMapper( ci.memFlags, ci.access ),	_flags( ci.memFlags ),
		_binding( EBindingTarget::Unbinded )
	{
		SetDebugName( "SWMemory" );

		_SubscribeOnMsg( this, &SWMemory::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &SWMemory::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &SWMemory::_AttachModule_Empty );
		_SubscribeOnMsg( this, &SWMemory::_DetachModule_Empty );
		_SubscribeOnMsg( this, &SWMemory::_FindModule_Empty );
		_SubscribeOnMsg( this, &SWMemory::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &SWMemory::_Link_Impl );
		_SubscribeOnMsg( this, &SWMemory::_Compose );
		_SubscribeOnMsg( this, &SWMemory::_Delete );
		_SubscribeOnMsg( this, &SWMemory::_OnManagerChanged );
		_SubscribeOnMsg( this, &SWMemory::_GetStreamDescriptor );
		_SubscribeOnMsg( this, &SWMemory::_ReadFromImageMemory );
		_SubscribeOnMsg( this, &SWMemory::_WriteToImageMemory );
		_SubscribeOnMsg( this, &SWMemory::_ReadFromGpuMemory );
		_SubscribeOnMsg( this, &SWMemory::_WriteToGpuMemory );
		_SubscribeOnMsg( this, &SWMemory::_ReadFromStream );
		_SubscribeOnMsg( this, &SWMemory::_WriteToStream );
		_SubscribeOnMsg( this, &SWMemory::_MapMemoryToCpu );
		_SubscribeOnMsg( this, &SWMemory::_MapImageToCpu );
		_SubscribeOnMsg( this, &SWMemory::_FlushMemoryRange );
		_SubscribeOnMsg( this, &SWMemory::_UnmapMemory );
		_SubscribeOnMsg( this, &SWMemory::_GetDeviceInfo );
		_SubscribeOnMsg( this, &SWMemory::_GetSWDeviceInfo );
		_SubscribeOnMsg( this, &SWMemory::_GetSWPrivateClasses );
		_SubscribeOnMsg( this, &SWMemory::_GpuMemoryRegionChanged );
		_SubscribeOnMsg( this, &SWMemory::_GetGpuMemoryDescriptor );
		_SubscribeOnMsg( this, &SWMemory::_GetSWMemoryData );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}

/*
=================================================
	destructor
=================================================
*/
	SWMemory::~SWMemory ()
	{
		ASSERT( not _IsCreated() );
		ASSERT( not _memMapper.IsMapped() );
	}
	
/*
=================================================
	_IsCreated
=================================================
*/
	bool SWMemory::_IsCreated () const
	{
		return not _memory.Empty();
	}
	
/*
=================================================
	_GetAlignedOffset
=================================================
*/
	inline usize SWMemory::_GetAlignedOffset (ubyte *ptr, BytesU align)
	{
		const usize a = usize(align) - 1;
		const usize p = ReferenceCast<usize>(ptr);

		return ((p + a) & ~a) - p;
	}
	
/*
=================================================
	_AllocForImage
=================================================
*/
	bool SWMemory::_AllocForImage ()
	{
		CHECK_ERR( not _IsCreated() );

		Message< GpuMsg::GetSWImageMemoryRequirements >		req_mem;
		SendTo( _GetParents().Front(), req_mem );

		_align = req_mem->result->align;
		CHECK_ERR( IsPowerOfTwo( _align ) );

		_memory.Resize( (usize)AlignToLarge( req_mem->result->size + _align, _align ), false );
		_usedMemory = _memory.SubArray( _GetAlignedOffset( _memory.ptr(), _align ), usize(req_mem->result->size) );

		_binding = EBindingTarget::Image;
		return true;
	}
	
/*
=================================================
	_AllocForBuffer
=================================================
*/
	bool SWMemory::_AllocForBuffer ()
	{
		CHECK_ERR( not _IsCreated() );
		
		Message< GpuMsg::GetSWBufferMemoryRequirements >		req_mem;
		SendTo( _GetParents().Front(), req_mem );

		_align = req_mem->result->align;
		CHECK_ERR( IsPowerOfTwo( _align ) );

		_memory.Resize( (usize)AlignToLarge( req_mem->result->size + _align, _align ), false );
		_usedMemory = _memory.SubArray( _GetAlignedOffset( _memory.ptr(), _align ), usize(req_mem->result->size) );

		_binding = EBindingTarget::Buffer;
		return true;
	}

/*
=================================================
	_FreeMemory
=================================================
*/
	void SWMemory::_FreeMemory ()
	{
		_align		= 0_b;
		_flags		= Uninitialized;
		_binding	= EBindingTarget::Unbinded;
		_usedMemory	= Uninitialized;
		_memory.Free();
		_memMapper.Clear();
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool SWMemory::_Compose (const Message< ModuleMsg::Compose > &)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );
		//CHECK_ERR( _GetParents().IsExist( msg.Sender() ) );

		CHECK_ERR( not _IsCreated() );
		CHECK_ERR( _GetParents().Count() >= 1 );

		ModulePtr const&	parent = _GetParents().Front();

		if ( parent->GetSupportedMessages().HasAllTypes< ImageMessages_t >() )
		{
			CHECK_COMPOSING( _AllocForImage() );
		}
		else
		if ( parent->GetSupportedMessages().HasAllTypes< BufferMessages_t >() )
		{
			CHECK_COMPOSING( _AllocForBuffer() );
		}
		else {
			CHECK_COMPOSING( false );
		}

		CHECK( _DefCompose( false ) );
		CHECK( _SendEvent( Message< GpuMsg::OnMemoryBindingChanged >{ parent, _binding } ) );

		return true;
	}

/*
=================================================
	_GetStreamDescriptor
=================================================
*/
	bool SWMemory::_GetStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &msg)
	{
		StreamDescriptor	descr;

		descr.memoryFlags	= _memMapper.MappingAccess();
		descr.available		= _memMapper.MappedSize();
		descr.totalSize		= BytesUL(_usedMemory.Size());

		msg->result.Set( descr );
		return true;
	}
	
/*
=================================================
	_ReadFromStream
=================================================
*/
	bool SWMemory::_ReadFromStream (const Message< ModuleMsg::ReadFromStream > &msg)
	{
		BinArrayCRef	data;
		CHECK_ERR( _memMapper.Read( msg->offset, msg->size.Get( UMax ), OUT data ) );
		msg->result.Set( data );
		
		_SendEvent< ModuleMsg::DataRegionChanged >({ EMemoryAccess::CpuRead, _memMapper.MappedOffset() + msg->offset, BytesUL(data.Size()) });
		return true;
	}
	
/*
=================================================
	_WriteToStream
=================================================
*/
	bool SWMemory::_WriteToStream (const Message< ModuleMsg::WriteToStream > &msg)
	{
		BytesUL	written;
		CHECK_ERR( _memMapper.Write( msg->data, msg->offset, OUT written ) );
		msg->wasWritten.Set( written );

		_SendEvent< ModuleMsg::DataRegionChanged >({ EMemoryAccess::CpuWrite, _memMapper.MappedOffset() + msg->offset, written });
		return true;
	}
	
/*
=================================================
	_MapMemoryToCpu
=================================================
*/
	bool SWMemory::_MapMemoryToCpu (const Message< GpuMsg::MapMemoryToCpu > &msg)
	{
		using EMappingFlags = GpuMsg::MapMemoryToCpu::EMappingFlags;

		CHECK_ERR( _IsCreated() and _memMapper.IsMappingAllowed( msg->flags ) );
		CHECK_ERR( _binding == EBindingTarget::Buffer );
		CHECK_ERR( msg->offset < BytesUL(_usedMemory.Size()) );
		
		const BytesUL	size = Min( BytesUL(_usedMemory.Size()), msg->size );
		
		_memMapper.OnMapped( _usedMemory.ptr() + msg->offset, msg->offset, size, msg->flags );
		return true;
	}
	
/*
=================================================
	_MapImageToCpu
=================================================
*/
	bool SWMemory::_MapImageToCpu (const Message< GpuMsg::MapImageToCpu > &)
	{
		TODO( "not supported, you can copy image to buffer and then map buffer" );
		return true;
	}

/*
=================================================
	_FlushMemoryRange
=================================================
*/
	bool SWMemory::_FlushMemoryRange (const Message< GpuMsg::FlushMemoryRange > &msg)
	{
		CHECK_ERR( _binding == EBindingTarget::Buffer );
		CHECK_ERR( _memMapper.FlushMemoryRange( msg->offset, msg->size ) );

		TODO( "glFlushMappedBufferRange" );
		return true;
	}
	
/*
=================================================
	_UnmapMemory
=================================================
*/
	bool SWMemory::_UnmapMemory (const Message< GpuMsg::UnmapMemory > &)
	{
		CHECK_ERR( _binding == EBindingTarget::Buffer );
		CHECK_ERR( _memMapper.Unmap() );

		TODO("");
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool SWMemory::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_FreeMemory();

		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_GpuMemoryRegionChanged
=================================================
*/
	bool SWMemory::_GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &)
	{
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::GpuWrite] );	// this message allowed only for gpu-writable memory

		// request memory barrier
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_GetGpuMemoryDescriptor
=================================================
*/
	bool SWMemory::_GetGpuMemoryDescriptor (const Message< GpuMsg::GetGpuMemoryDescriptor > &msg)
	{
		GpuMemoryDescriptor		descr;

		descr.flags		= _flags;
		descr.size		= BytesUL(_usedMemory.Size());
		descr.access	= _memMapper.MemoryAccess();

		msg->result.Set( descr );
		return true;
	}
/*
=================================================
	_ReadFromGpuMemory
=================================================
*/
	bool SWMemory::_ReadFromGpuMemory (const Message< GpuMsg::ReadFromGpuMemory > &msg) const
	{
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuRead] );
		CHECK_ERR( _binding == EBindingTarget::Buffer );
		CHECK_ERR( msg->writableBuffer.Size() > 0 );
		CHECK_ERR( msg->offset < BytesUL(_usedMemory.Size()) );
		
		const usize		size = Min( usize(_usedMemory.Size()) - usize(msg->offset), usize(msg->writableBuffer.Size()) );
		
		MemCopy( msg->writableBuffer, _usedMemory.SubArray( usize(msg->offset), size ) );

		msg->result.Set( msg->writableBuffer.SubArray( 0, size ) );
		return true;
	}
	
/*
=================================================
	_WriteToGpuMemory
=================================================
*/
	bool SWMemory::_WriteToGpuMemory (const Message< GpuMsg::WriteToGpuMemory > &msg)
	{
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuWrite] );
		CHECK_ERR( _binding == EBindingTarget::Buffer );
		CHECK_ERR( msg->offset < BytesUL(_usedMemory.Size()) );
		
		const usize		size = Min( usize(_usedMemory.Size()) - usize(msg->offset), usize(msg->data.Size()) );

		MemCopy( _usedMemory.SubArray( 0, size ), msg->data );

		msg->wasWritten.Set( BytesUL(size) );
		
		_SendEvent< ModuleMsg::DataRegionChanged >({ EMemoryAccess::CpuWrite, msg->offset, BytesUL(size) });
		return true;
	}
	
/*
=================================================
	_ReadFromImageMemory
=================================================
*/
	bool SWMemory::_ReadFromImageMemory (const Message< GpuMsg::ReadFromImageMemory > &)
	{
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_WriteToImageMemory
=================================================
*/
	bool SWMemory::_WriteToImageMemory (const Message< GpuMsg::WriteToImageMemory > &)
	{
		TODO("");
		return false;
	}
	
/*
=================================================
	_GetSWMemoryData
=================================================
*/
	bool SWMemory::_GetSWMemoryData (const Message< GpuMsg::GetSWMemoryData > &msg)
	{
		msg->result.Set( _usedMemory );
		return true;
	}

}	// PlatformSW
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr SoftRendererObjectsConstructor::CreateSWMemory (GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci)
	{
		return New< PlatformSW::SWMemory >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_SOFT
