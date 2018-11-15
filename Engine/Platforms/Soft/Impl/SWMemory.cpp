// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_SOFT

#include "Engine/Platforms/Public/GPU/Memory.h"
#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/Tools/MemoryMapperHelper.h"
#include "Engine/Platforms/Soft/Impl/SWBaseModule.h"
#include "Engine/Platforms/Soft/SoftRendererObjectsConstructor.h"
#include "Engine/Platforms/Public/GPU/Enums.ToString.h"

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
											DSMsg::GetDataSourceDescription,
											DSMsg::ReadMemRange,
											DSMsg::WriteMemRange,
											GpuMsg::ReadFromGpuMemory,
											GpuMsg::WriteToGpuMemory,
											GpuMsg::ReadFromImageMemory,
											GpuMsg::WriteToImageMemory,
											GpuMsg::MapMemoryToCpu,
											GpuMsg::MapImageToCpu,
											GpuMsg::FlushMemoryRange,
											GpuMsg::UnmapMemory,
											GpuMsg::GetGpuMemoryDescription,
											GpuMsg::GetSWMemoryData
										> >;

		using SupportedEvents_t		= SWBaseModule::SupportedEvents_t::Append< MessageListFrom<
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
		SWMemory (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci);
		~SWMemory ();


	// message handlers
	private:
		bool _GetGpuMemoryDescription (const GpuMsg::GetGpuMemoryDescription &);
		bool _GetDataSourceDescription (const DSMsg::GetDataSourceDescription &);
		bool _ReadFromImageMemory (const GpuMsg::ReadFromImageMemory &);
		bool _WriteToImageMemory (const GpuMsg::WriteToImageMemory &);
		bool _ReadFromGpuMemory (const GpuMsg::ReadFromGpuMemory &) const;
		bool _WriteToGpuMemory (const GpuMsg::WriteToGpuMemory &);
		bool _ReadMemRange (const DSMsg::ReadMemRange &);
		bool _WriteMemRange (const DSMsg::WriteMemRange &);
		bool _MapMemoryToCpu (const GpuMsg::MapMemoryToCpu &);
		bool _MapImageToCpu (const GpuMsg::MapImageToCpu &);
		bool _FlushMemoryRange (const GpuMsg::FlushMemoryRange &);
		bool _UnmapMemory (const GpuMsg::UnmapMemory &);
		bool _GetSWMemoryData (const GpuMsg::GetSWMemoryData &);

		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);

	private:
		bool _IsCreated () const;
		bool _IsMapped () const;

		bool _AllocForImage ();
		bool _AllocForBuffer ();
		void _FreeMemory ();

		static usize _GetAlignedOffset (ubyte *ptr, BytesU align);
	};
//-----------------------------------------------------------------------------



	const TypeIdList	SWMemory::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SWMemory::SWMemory (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci) :
		SWBaseModule( gs, ModuleConfig{ id, 1 }, &_eventTypes ),
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
		_SubscribeOnMsg( this, &SWMemory::_GetDataSourceDescription );
		_SubscribeOnMsg( this, &SWMemory::_ReadFromImageMemory );
		_SubscribeOnMsg( this, &SWMemory::_WriteToImageMemory );
		_SubscribeOnMsg( this, &SWMemory::_ReadFromGpuMemory );
		_SubscribeOnMsg( this, &SWMemory::_WriteToGpuMemory );
		_SubscribeOnMsg( this, &SWMemory::_ReadMemRange );
		_SubscribeOnMsg( this, &SWMemory::_WriteMemRange );
		_SubscribeOnMsg( this, &SWMemory::_MapMemoryToCpu );
		_SubscribeOnMsg( this, &SWMemory::_MapImageToCpu );
		_SubscribeOnMsg( this, &SWMemory::_FlushMemoryRange );
		_SubscribeOnMsg( this, &SWMemory::_UnmapMemory );
		_SubscribeOnMsg( this, &SWMemory::_GetDeviceInfo );
		_SubscribeOnMsg( this, &SWMemory::_GetSWDeviceInfo );
		_SubscribeOnMsg( this, &SWMemory::_GetSWPrivateClasses );
		_SubscribeOnMsg( this, &SWMemory::_GetGpuMemoryDescription );
		_SubscribeOnMsg( this, &SWMemory::_GetSWMemoryData );

		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

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

		GpuMsg::GetSWImageMemoryRequirements	req_mem;
		_GetParents().Front()->Send( req_mem );
		
		if ( SWDeviceProperties.vulkanCompatibility )
		{
			GpuMsg::GetImageDescription			req_descr;
			_GetParents().Front()->Send( req_descr );

			if ( req_descr.result->usage != (req_descr.result->usage & (EImageUsage::TransferDst | EImageUsage::TransferSrc)) and
				 not _flags[ EGpuMemory::LocalInGPU ] )
			{
				SW_DEBUG_REPORT( false, "image memory must be located in GPU or image must be used only for transfer!", EDbgReport::Error );
			}
		}

		_align = req_mem.result->align;
		CHECK_ERR( IsPowerOfTwo( _align ) );

		_memory.Resize( usize(AlignToLarge( req_mem.result->size + _align, _align )), false );
		_usedMemory = _memory.SubArray( _GetAlignedOffset( _memory.ptr(), _align ), usize(req_mem.result->size) );

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
		
		GpuMsg::GetSWBufferMemoryRequirements	req_mem;
		_GetParents().Front()->Send( req_mem );

		_align = req_mem.result->align;
		CHECK_ERR( IsPowerOfTwo( _align ) );

		_memory.Resize( usize(AlignToLarge( req_mem.result->size + _align, _align )), false );
		_usedMemory = _memory.SubArray( _GetAlignedOffset( _memory.ptr(), _align ), usize(req_mem.result->size) );

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
	bool SWMemory::_Compose (const ModuleMsg::Compose &)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_ERR( not _IsCreated() );
		CHECK_ERR( _GetParents().Count() >= 1 );

		ModulePtr const&	parent = _GetParents().Front();

		if ( parent->SupportsAllMessages< ImageMessages_t >() )
		{
			CHECK_COMPOSING( _AllocForImage() );
		}
		else
		if ( parent->SupportsAllMessages< BufferMessages_t >() )
		{
			CHECK_COMPOSING( _AllocForBuffer() );
		}
		else {
			CHECK_COMPOSING( false );
		}

		CHECK( _DefCompose( false ) );
		CHECK( _SendEvent( GpuMsg::OnMemoryBindingChanged{ parent, _binding }) );

		return true;
	}

/*
=================================================
	_GetDataSourceDescription
=================================================
*/
	bool SWMemory::_GetDataSourceDescription (const DSMsg::GetDataSourceDescription &msg)
	{
		DataSourceDescription	descr;

		descr.memoryFlags	= _memMapper.MappingAccess();
		descr.available		= _memMapper.MappedSize();
		descr.totalSize		= _usedMemory.Size();

		msg.result.Set( descr );
		return true;
	}
	
/*
=================================================
	_ReadMemRange
=================================================
*/
	bool SWMemory::_ReadMemRange (const DSMsg::ReadMemRange &msg)
	{
		BinArrayCRef	data;
		CHECK_ERR( _memMapper.Read( msg.position, msg.writableBuffer->Size(), OUT data ) );

		MemCopy( OUT *msg.writableBuffer, data );

		msg.result.Set( msg.writableBuffer->SubArray( 0, data.Count() ) );
		return true;
	}
	
/*
=================================================
	_WriteMemRange
=================================================
*/
	bool SWMemory::_WriteMemRange (const DSMsg::WriteMemRange &msg)
	{
		BytesU	written;
		CHECK_ERR( _memMapper.Write( msg.data, msg.position, OUT written ) );

		msg.wasWritten.Set( written );
		return true;
	}
	
/*
=================================================
	_MapMemoryToCpu
=================================================
*/
	bool SWMemory::_MapMemoryToCpu (const GpuMsg::MapMemoryToCpu &msg)
	{
		CHECK_ERR( _IsCreated() and _memMapper.IsMappingAllowed( msg.flags ) );
		CHECK_ERR( _binding == EBindingTarget::Buffer );
		CHECK_ERR( msg.position < _usedMemory.Size() );
		
		const BytesU	size = Min( _usedMemory.Size(), msg.size );
		
		_memMapper.OnMapped( _usedMemory.ptr() + msg.position, msg.position, size, msg.flags );
		
		msg.result.Set( BinArrayRef::FromVoid( _usedMemory.ptr() + msg.position, size ) );
		return true;
	}
	
/*
=================================================
	_MapImageToCpu
=================================================
*/
	bool SWMemory::_MapImageToCpu (const GpuMsg::MapImageToCpu &)
	{
		TODO( "not supported, you can copy image to buffer and then map buffer" );
		return true;
	}

/*
=================================================
	_FlushMemoryRange
=================================================
*/
	bool SWMemory::_FlushMemoryRange (const GpuMsg::FlushMemoryRange &msg)
	{
		CHECK_ERR( _binding == EBindingTarget::Buffer );
		CHECK_ERR( _memMapper.FlushMemoryRange( msg.offset, msg.size ) );

		TODO( "glFlushMappedBufferRange" );
		return true;
	}
	
/*
=================================================
	_UnmapMemory
=================================================
*/
	bool SWMemory::_UnmapMemory (const GpuMsg::UnmapMemory &)
	{
		CHECK_ERR( _binding == EBindingTarget::Buffer );
		CHECK_ERR( _memMapper.Unmap() );

		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool SWMemory::_Delete (const ModuleMsg::Delete &msg)
	{
		_FreeMemory();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetGpuMemoryDescription
=================================================
*/
	bool SWMemory::_GetGpuMemoryDescription (const GpuMsg::GetGpuMemoryDescription &msg)
	{
		GpuMemoryDescription		descr;

		descr.flags		= _flags;
		descr.size		= _usedMemory.Size();
		descr.access	= _memMapper.MemoryAccess();

		msg.result.Set( descr );
		return true;
	}
/*
=================================================
	_ReadFromGpuMemory
=================================================
*/
	bool SWMemory::_ReadFromGpuMemory (const GpuMsg::ReadFromGpuMemory &msg) const
	{
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuRead] );
		CHECK_ERR( _binding == EBindingTarget::Buffer );
		CHECK_ERR( msg.writableBuffer->Size() > 0 );
		CHECK_ERR( msg.position < _usedMemory.Size() );
		
		GpuMsg::GetSWBufferMemoryLayout		req_mem{ msg.position, msg.writableBuffer->Size(), EPipelineAccess::HostRead, EPipelineStage::Host };
		_GetParents().Front()->Send( req_mem );
		
		const BytesU	size = Min( req_mem.result->memory.Size(), msg.writableBuffer->Size() );

		UnsafeMem::MemCopy( OUT msg.writableBuffer->RawPtr(), req_mem.result->memory.RawPtr(), size );

		msg.result.Set( msg.writableBuffer->SubArray( 0, usize(size) ) );
		return true;
	}
	
/*
=================================================
	_WriteToGpuMemory
=================================================
*/
	bool SWMemory::_WriteToGpuMemory (const GpuMsg::WriteToGpuMemory &msg)
	{
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuWrite] );
		CHECK_ERR( _binding == EBindingTarget::Buffer );
		CHECK_ERR( msg.position < _usedMemory.Size() );
		
		GpuMsg::GetSWBufferMemoryLayout		req_mem{ msg.position, msg.data.Size(), EPipelineAccess::HostWrite, EPipelineStage::Host };
		_GetParents().Front()->Send( req_mem );
		
		const BytesU	size = Min( req_mem.result->memory.Size(), msg.data.Size() );
		
		UnsafeMem::MemCopy( OUT req_mem.result->memory.RawPtr(), msg.data.RawPtr(), size );

		msg.wasWritten.Set( size );
		return true;
	}
	
/*
=================================================
	_ReadFromImageMemory
=================================================
*/
	bool SWMemory::_ReadFromImageMemory (const GpuMsg::ReadFromImageMemory &msg)
	{
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuRead] );
		CHECK_ERR( _binding == EBindingTarget::Image );
		CHECK_ERR( IsNotZero( msg.dimension ) );
		CHECK_ERR( msg.memOffset == 0_b );
		CHECK_ERR( msg.layer.Get() == 0 or (msg.offset.z == 0 and msg.dimension.z == 1) );	// 3D texture array is not supported
		
		const uint	offset_z = Max( msg.layer.Get(), msg.offset.z );

		GpuMsg::GetSWImageMemoryLayout	req_mem{ EPipelineAccess::HostRead, EPipelineStage::Host };
		GpuMsg::GetImageDescription		req_descr;
		_GetParents().Front()->Send( req_mem );
		_GetParents().Front()->Send( req_descr );

		// find array layer or z-slice
		CHECK_ERR( req_mem.result );
		//CHECK_ERR( req_mem->result->memAccess[EMemoryAccess::CpuRead] );
		CHECK_ERR( offset_z < req_mem.result->layers.Count() and
					offset_z + msg.dimension.z <= req_mem.result->layers.Count() );

		auto const&	mem_layer = req_mem.result->layers[ offset_z ];

		// find mipmap level
		CHECK_ERR( msg.mipLevel.Get() < mem_layer.mipmaps.Count() );

		auto const&	mem_level		= mem_layer.mipmaps[ msg.mipLevel.Get() ];
		const BitsU		bpp				= EPixelFormat::BitPerPixel( mem_level.format ); // must be in bits becouse compressed image pixel may be less than 1 byte
		const BytesU	src_row_pitch	= GXImageUtils::AlignedRowSize( mem_level.dimension.x, bpp, req_mem.result->align );
		const BytesU	row_size		= BytesU(bpp * msg.dimension.x);

		CHECK_ERR( mem_level.memory != null );
		CHECK_ERR( All( msg.offset.xy() < mem_level.dimension ) );
		CHECK_ERR( All( msg.offset.xy() + msg.dimension.xy() <= mem_level.dimension ) );
		CHECK_ERR( msg.rowPitch >= row_size );
		
		SW_DEBUG_REPORT( mem_level.layout == EImageLayout::Preinitialized or mem_level.layout == EImageLayout::General,
						"Mapping an image with layout '"_str << EImageLayout::ToString( mem_level.layout ) <<
						"' can result in undefined behavior if this memory is used by the device.",
						EDbgReport::Warning );
		
		if ( req_descr.result->imageType == EImage::Tex3D ) {
			CHECK_ERR( msg.writableBuffer->Size() >= msg.slicePitch * msg.dimension.z );
		}else{
			CHECK_ERR( msg.writableBuffer->Size() >= msg.rowPitch * msg.dimension.y );
		}

		usize	readn	= 0;

		// read pixels
		for (uint y = 0; y < msg.dimension.y; ++y)
		{
			BytesU			src_off = BytesU(bpp * msg.offset.x) + ((msg.offset.y + y) * src_row_pitch);

			BinArrayCRef	src		= mem_level.Data().SubArray( usize(src_off), usize(row_size) );
			BinArrayRef		dst		= msg.writableBuffer->SubArray( usize(y * msg.rowPitch), usize(row_size) );

			MemCopy( OUT dst, src );

			readn += usize(msg.rowPitch);
		}

		msg.result.Set( msg.writableBuffer->SubArray( 0, readn ) );
		return true;
	}
	
/*
=================================================
	_WriteToImageMemory
=================================================
*/
	bool SWMemory::_WriteToImageMemory (const GpuMsg::WriteToImageMemory &msg)
	{
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuWrite] );
		CHECK_ERR( _binding == EBindingTarget::Image );
		CHECK_ERR( IsNotZero( msg.dimension ) );
		CHECK_ERR( msg.memOffset == 0_b );
		CHECK_ERR( msg.layer.Get() == 0 or (msg.offset.z == 0 and msg.dimension.z == 1) );	// 3D texture array is not supported
			
		const uint	offset_z = Max( msg.layer.Get(), msg.offset.z );
			
		GpuMsg::GetSWImageMemoryLayout	req_mem{ EPipelineAccess::HostWrite, EPipelineStage::Host };
		GpuMsg::GetImageDescription		req_descr;
		_GetParents().Front()->Send( req_mem );
		_GetParents().Front()->Send( req_descr );

		// find array layer or z-slice
		CHECK_ERR( req_mem.result );
		//CHECK_ERR( req_mem->result->memAccess[EMemoryAccess::CpuWrite] );
		CHECK_ERR( offset_z < req_mem.result->layers.Count() and
					offset_z + msg.dimension.z <= req_mem.result->layers.Count() );

		auto&	mem_layer = req_mem.result->layers[ offset_z ];

		// find mipmap level
		CHECK_ERR( msg.mipLevel.Get() < mem_layer.mipmaps.Count() );

		auto&			mem_level		= mem_layer.mipmaps[ msg.mipLevel.Get() ];
		const BitsU		bpp				= EPixelFormat::BitPerPixel( mem_level.format ); // must be in bits becouse compressed image pixel may be less than 1 byte
		const BytesU	dst_row_pitch	= GXImageUtils::AlignedRowSize( mem_level.dimension.x, bpp, req_mem.result->align );
		const BytesU	row_size		= BytesU(bpp * msg.dimension.x);

		CHECK_ERR( mem_level.memory != null );
		CHECK_ERR( All( msg.offset.xy() < mem_level.dimension ) );
		CHECK_ERR( All( msg.offset.xy() + msg.dimension.xy() <= mem_level.dimension ) );
		CHECK_ERR( msg.rowPitch >= row_size );
		
		SW_DEBUG_REPORT( mem_level.layout == EImageLayout::Preinitialized or mem_level.layout == EImageLayout::General,
						"Mapping an image with layout '"_str << EImageLayout::ToString( mem_level.layout ) <<
						"' can result in undefined behavior if this memory is used by the device.",
						EDbgReport::Warning );

		if ( req_descr.result->imageType == EImage::Tex3D ) {
			CHECK_ERR( msg.data.Size() >= msg.slicePitch * msg.dimension.z );
		}else{
			CHECK_ERR( msg.data.Size() >= msg.rowPitch * msg.dimension.y );
		}
		
		// write pixels
		BytesU	written;

		for (uint y = 0; y < mem_level.dimension.y; ++y)
		{
			BytesU			dst_off = BytesU(bpp * msg.offset.x) + ((msg.offset.y + y) * dst_row_pitch);

			BinArrayRef		dst		= mem_level.Data().SubArray( usize(dst_off), usize(row_size) );
			BinArrayCRef	src		= msg.data.SubArray( usize(y * msg.rowPitch), usize(row_size) );

			MemCopy( OUT dst, src );
			written += msg.rowPitch;
		}

		msg.wasWritten.Set( written );
		return true;
	}
	
/*
=================================================
	_GetSWMemoryData
=================================================
*/
	bool SWMemory::_GetSWMemoryData (const GpuMsg::GetSWMemoryData &msg)
	{
		msg.result.Set( _usedMemory );
		return true;
	}

}	// PlatformSW
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr SoftRendererObjectsConstructor::CreateSWMemory (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci)
	{
		return New< PlatformSW::SWMemory >( id, gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_SOFT
