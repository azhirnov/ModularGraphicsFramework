// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "Engine/Platforms/Public/GPU/Memory.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/Tools/MemoryMapperHelper.h"
#include "Engine/Platforms/OpenCL/Impl/CL2BaseModule.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"

namespace Engine
{
namespace PlatformCL
{
	using namespace cl;


	//
	// OpenCL Memory
	//

	class CL2Memory final : public CL2BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= CL2BaseModule::SupportedMessages_t::Append< MessageListFrom<
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
											GpuMsg::GpuMemoryRegionChanged
										> >;

		using SupportedEvents_t		= CL2BaseModule::SupportedEvents_t::Append< MessageListFrom<
											ModuleMsg::DataRegionChanged,
											GpuMsg::OnMemoryBindingChanged
										> >;
		
		using ImageMsg_t			= MessageListFrom< GpuMsg::GetCLImageID, GpuMsg::GetImageDescriptor >;
		using BufferMsg_t			= MessageListFrom< GpuMsg::GetCLBufferID, GpuMsg::GetBufferDescriptor >;

		using EBindingTarget		= GpuMsg::OnMemoryBindingChanged::EBindingTarget;
		using MemMapper_t			= PlatformTools::MemoryMapperHelper;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		cl_mem					_mem;
		MemMapper_t				_memMapper;
		BytesUL					_size;
		EGpuMemory::bits		_flags;
		EBindingTarget			_binding;


	// methods
	public:
		CL2Memory (GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci);
		~CL2Memory ();


	// message handlers
	private:
		bool _GetGpuMemoryDescriptor (const Message< GpuMsg::GetGpuMemoryDescriptor > &);
		bool _GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &);
		bool _GetStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &);
		bool _ReadFromImageMemory (const Message< GpuMsg::ReadFromImageMemory > &);
		bool _WriteToImageMemory (const Message< GpuMsg::WriteToImageMemory > &);
		bool _ReadFromGpuMemory (const Message< GpuMsg::ReadFromGpuMemory > &);
		bool _WriteToGpuMemory (const Message< GpuMsg::WriteToGpuMemory > &);
		bool _ReadFromStream (const Message< ModuleMsg::ReadFromStream > &);
		bool _WriteToStream (const Message< ModuleMsg::WriteToStream > &);
		bool _MapMemoryToCpu (const Message< GpuMsg::MapMemoryToCpu > &);
		bool _MapImageToCpu (const Message< GpuMsg::MapImageToCpu > &);
		bool _FlushMemoryRange (const Message< GpuMsg::FlushMemoryRange > &);
		bool _UnmapMemory (const Message< GpuMsg::UnmapMemory > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);

	private:
		bool _IsCreated () const;

		bool _AllocForImage ();
		bool _AllocForBuffer ();
		void _FreeMemory ();
	};
//-----------------------------------------------------------------------------



	const TypeIdList	CL2Memory::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	CL2Memory::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	CL2Memory::CL2Memory (GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci) :
		CL2BaseModule( gs, ModuleConfig{ CLMemoryModuleID, 1 }, &_msgTypes, &_eventTypes ),
		_mem( null ),				_memMapper( ci.memFlags, ci.access ),
		_flags( ci.memFlags ),		_binding( EBindingTarget::Unbinded )
	{
		SetDebugName( "CL2Memory" );

		_SubscribeOnMsg( this, &CL2Memory::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &CL2Memory::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &CL2Memory::_AttachModule_Empty );
		_SubscribeOnMsg( this, &CL2Memory::_DetachModule_Empty );
		_SubscribeOnMsg( this, &CL2Memory::_FindModule_Empty );
		_SubscribeOnMsg( this, &CL2Memory::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &CL2Memory::_Link_Impl );
		_SubscribeOnMsg( this, &CL2Memory::_Compose );
		_SubscribeOnMsg( this, &CL2Memory::_Delete );
		_SubscribeOnMsg( this, &CL2Memory::_OnManagerChanged );
		_SubscribeOnMsg( this, &CL2Memory::_GetStreamDescriptor );
		_SubscribeOnMsg( this, &CL2Memory::_GetGpuMemoryDescriptor );
		_SubscribeOnMsg( this, &CL2Memory::_ReadFromImageMemory );
		_SubscribeOnMsg( this, &CL2Memory::_WriteToImageMemory );
		_SubscribeOnMsg( this, &CL2Memory::_ReadFromGpuMemory );
		_SubscribeOnMsg( this, &CL2Memory::_WriteToGpuMemory );
		_SubscribeOnMsg( this, &CL2Memory::_ReadFromStream );
		_SubscribeOnMsg( this, &CL2Memory::_WriteToStream );
		_SubscribeOnMsg( this, &CL2Memory::_MapMemoryToCpu );
		_SubscribeOnMsg( this, &CL2Memory::_MapImageToCpu );
		_SubscribeOnMsg( this, &CL2Memory::_FlushMemoryRange );
		_SubscribeOnMsg( this, &CL2Memory::_UnmapMemory );
		_SubscribeOnMsg( this, &CL2Memory::_GetDeviceInfo );
		_SubscribeOnMsg( this, &CL2Memory::_GetCLDeviceInfo );
		_SubscribeOnMsg( this, &CL2Memory::_GetCLPrivateClasses );
		_SubscribeOnMsg( this, &CL2Memory::_GpuMemoryRegionChanged );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}

/*
=================================================
	destructor
=================================================
*/
	CL2Memory::~CL2Memory ()
	{
		ASSERT( not _IsCreated() );
		ASSERT( not _memMapper.IsMapped() );
	}
	
/*
=================================================
	_IsCreated
=================================================
*/
	bool CL2Memory::_IsCreated () const
	{
		return _mem != null;
	}
	
/*
=================================================
	_AllocForImage
=================================================
*/
	bool CL2Memory::_AllocForImage ()
	{
		CHECK_ERR( not _IsCreated() );

		Message< GpuMsg::GetCLImageID >			req_id;
		Message< GpuMsg::GetImageDescriptor >	req_descr;

		SendTo( _GetParents().Front(), req_id );
		SendTo( _GetParents().Front(), req_descr );

		_mem = req_id->result.Get(null);
		CHECK_ERR( _mem != null );

		size_t	mem_size = 0;
		CL_CALL( clGetMemObjectInfo( _mem, CL_MEM_SIZE, sizeof(mem_size), OUT &mem_size, null ) );

		_size	 = BytesUL(mem_size);
		_binding = EBindingTarget::Image;
		return true;
	}
	
/*
=================================================
	_AllocForBuffer
=================================================
*/
	bool CL2Memory::_AllocForBuffer ()
	{
		CHECK_ERR( not _IsCreated() );

		Message< GpuMsg::GetCLBufferID >		req_id;
		Message< GpuMsg::GetBufferDescriptor >	req_descr;

		SendTo( _GetParents().Front(), req_id );
		SendTo( _GetParents().Front(), req_descr );

		_mem = req_id->result.Get(null);
		CHECK_ERR( _mem != null );
		
		size_t	mem_size = 0;
		CL_CALL( clGetMemObjectInfo( _mem, CL_MEM_SIZE, sizeof(mem_size), OUT &mem_size, null ) );
		
		_size	 = BytesUL(mem_size);
		_binding = EBindingTarget::Buffer;
		return true;
	}

/*
=================================================
	_FreeMemory
=================================================
*/
	void CL2Memory::_FreeMemory ()
	{
		_mem		= null;
		_size		= Uninitialized;
		_flags		= Uninitialized;
		_binding	= EBindingTarget::Unbinded;
		_memMapper.Clear();
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool CL2Memory::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_ERR( not _IsCreated() );
		CHECK_ERR( _GetParents().Count() >= 1 );

		ModulePtr const&	parent = _GetParents().Front();

		if ( parent->GetSupportedMessages().HasAllTypes< ImageMsg_t >() )
		{
			CHECK_COMPOSING( _AllocForImage() );
		}
		else
		if ( parent->GetSupportedMessages().HasAllTypes< BufferMsg_t >() )
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
	bool CL2Memory::_GetStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &msg)
	{
		StreamDescriptor	descr;
		
		descr.memoryFlags	= _memMapper.MappingAccess();
		descr.available		= _memMapper.MappedSize();
		descr.totalSize		= _size;

		msg->result.Set( descr );
		return true;
	}
	
/*
=================================================
	_ReadFromStream
=================================================
*/
	bool CL2Memory::_ReadFromStream (const Message< ModuleMsg::ReadFromStream > &msg)
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
	bool CL2Memory::_WriteToStream (const Message< ModuleMsg::WriteToStream > &msg)
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
	bool CL2Memory::_MapMemoryToCpu (const Message< GpuMsg::MapMemoryToCpu > &msg)
	{
		using EMappingFlags = GpuMsg::MapMemoryToCpu::EMappingFlags;

		CHECK_ERR( _IsCreated() and _memMapper.IsMappingAllowed( msg->flags ) );
		CHECK_ERR( _binding == EBindingTarget::Buffer );
		CHECK_ERR( msg->offset < _size );
		
		const BytesUL	size	= Min( _size, msg->size );
		void *			ptr		= null;

		if ( _binding == EBindingTarget::Buffer )
		{
			cl_int	cl_err = 0;
			CL_CHECK((( ptr = clEnqueueMapBuffer(
										GetCommandQueue(),
										_mem,
										CL_TRUE, // blocking
										CL2Enum( _memMapper.MemoryAccess(), msg->flags ),
										(usize) msg->offset,
										(usize) size,
										0, null,
										null,
										&cl_err )), cl_err ));
		}
		else
		if ( _binding == EBindingTarget::Image )
		{
			RETURN_ERR( "not supported" );

			/*Message< GpuMsg::GetImageDescriptor >	req_descr;
			SendTo( _GetParents().Front(), req_descr );

			usize3	origin	= usize3(0);
			usize3	region	= Max( ImageUtils::ConvertSize( req_descr->result->imageType, req_descr->result->dimension ), 1u );

			size_t	row_pitch	= 0;
			size_t	slice_pitch	= 0;
			
			cl_int	cl_err = 0;
			CL_CHECK( _mappedPtr = (ubyte*) clEnqueueMapImage(
										GetCommandQueue(),
										&origin, &region,
										&row_pitch, &slice_pitch,
										0, null,
										null,
										&cl_err ), cl_err );*/
		}
		
		_memMapper.OnMapped( ptr, msg->offset, size, msg->flags );
		return true;
	}
	
/*
=================================================
	_MapImageToCpu
=================================================
*/
	bool CL2Memory::_MapImageToCpu (const Message< GpuMsg::MapImageToCpu > &msg)
	{
		/*
		CHECK_ERR( _IsCreated() and not _IsMapped() );
		CHECK_ERR( _binding == EBindingTarget::Image );
		CHECK_ERR( _access[EMemoryAccess::CpuRead] or _access[EMemoryAccess::CpuWrite] );
		
		// request image info
		Message< GpuMsg::GetImageDescriptor >	req_descr;
		SendTo( _GetParents().Front(), req_descr );
	
		// validate and map memory
		CHECK_ERR( All( msg->offset + msg->dimension <= req_descr->result->dimension ) );
		
		const usize3	origin	= usize3(0);
		const usize3	region	= Max( ImageUtils::ConvertSize( req_descr->result->imageType, req_descr->result->dimension ), 1u );

		size_t	row_pitch	= 0;
		size_t	slice_pitch	= 0;

		cl_int	cl_err = 0;
		CL_CHECK(( _mappedPtr = (ubyte*) clEnqueueMapImage(
									GetCommandQueue(),
									_mem,
									CL_TRUE,	// blocking
									CL2Enum( _access, msg->flags ),
									origin.ptr(), region.ptr(),
									&row_pitch, &slice_pitch,
									0, null,
									null,
									&cl_err ), cl_err ));

		// write output
		msg->pixelAlign.Set({ BytesUL(row_pitch), BytesUL(slice_pitch) });
		msg->range.Set({ BytesUL(), _size });

		_mappedSize			= _size;
		_mappedOffset		= BytesUL();
		_isMappedMemChanged	= false;*/
		return true;
	}
	
/*
=================================================
	_FlushMemoryRange
=================================================
*/
	bool CL2Memory::_FlushMemoryRange (const Message< GpuMsg::FlushMemoryRange > &msg)
	{
		CHECK_ERR( _binding == EBindingTarget::Buffer );
		CHECK_ERR( _memMapper.FlushMemoryRange( msg->offset, msg->size ) );

		TODO("");
		return true;
	}
	
/*
=================================================
	_UnmapMemory
=================================================
*/
	bool CL2Memory::_UnmapMemory (const Message< GpuMsg::UnmapMemory > &)
	{
		CHECK_ERR( _binding == EBindingTarget::Buffer );
		
		CL_CHECK( clEnqueueUnmapMemObject( GetCommandQueue(),
										   _mem,
										   _memMapper.Pointer(),
										   0, null,
										   null ) );

		CHECK_ERR( _memMapper.Unmap() );
		return true;
	}
	
/*
=================================================
	_ReadFromGpuMemory
=================================================
*/
	bool CL2Memory::_ReadFromGpuMemory (const Message< GpuMsg::ReadFromGpuMemory > &msg)
	{
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuRead] );
		CHECK_ERR( _binding == EBindingTarget::Buffer );
		CHECK_ERR( msg->writableBuffer->Size() > 0 );
		CHECK_ERR( msg->offset < _size );
		
		const BytesUL	req_size = BytesUL(msg->writableBuffer->Size());

		// read from mapped memory
		if ( _memMapper.IsMapped() )
		{
			CHECK_ERR( msg->offset >= _memMapper.MappedOffset() and
					   msg->offset + req_size <= _memMapper.MappedOffset() + _memMapper.MappedSize() );

			// read
			Message< ModuleMsg::ReadFromStream >	read_stream;
			read_stream->offset	= msg->offset - _memMapper.MappedOffset();
			read_stream->size	= req_size;

			CHECK( _ReadFromStream( read_stream ) );
			
			// copy to writable buffer
			CHECK( msg->writableBuffer->Size() >= read_stream->result->Size() );

			MemCopy( *msg->writableBuffer, *read_stream->result );
			msg->result.Set( msg->writableBuffer->SubArray( 0, usize(read_stream->result->Size()) ) );
			return true;
		}

		// read without mapping
		const usize		size = Min( usize(_size - msg->offset), usize(req_size) );

		CL_CALL( clEnqueueReadBuffer( GetCommandQueue(),
									  _mem,
									  CL_TRUE,	// blocking
									  (usize) msg->offset,
									  (usize) size,
									  OUT msg->writableBuffer->ptr(),
									  0, null,
									  null ) );

		msg->result.Set( msg->writableBuffer->SubArray( 0, size ) );
		return true;
	}
		
/*
=================================================
	_WriteToGpuMemory
=================================================
*/
	bool CL2Memory::_WriteToGpuMemory (const Message< GpuMsg::WriteToGpuMemory > &msg)
	{
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuWrite] );
		CHECK_ERR( _binding == EBindingTarget::Buffer );
		CHECK_ERR( msg->offset < _size );
		
		// write to mapped memory
		if ( _memMapper.IsMapped() )
		{
			CHECK_ERR( msg->offset >= _memMapper.MappedOffset() and
					   msg->offset + BytesUL(msg->data.Size()) <= _memMapper.MappedOffset() + _memMapper.MappedSize() );

			// write
			Message< ModuleMsg::WriteToStream >		write_stream;
			write_stream->offset	= msg->offset - _memMapper.MappedOffset();
			write_stream->data		= msg->data;

			CHECK( _WriteToStream( write_stream ) );

			// flush
			if ( not _flags[ EGpuMemory::CoherentWithCPU ] )
			{
				Message< GpuMsg::FlushMemoryRange >	flush;
				flush->offset	= write_stream->offset;
				flush->size		= write_stream->wasWritten.Get( UMax );

				CHECK( _FlushMemoryRange( flush ) );
			}

			msg->wasWritten.Set( write_stream->wasWritten.Get() );
			return true;
		}

		// write without mapping
		const usize		size = Min( usize(_size - msg->offset), usize(msg->data.Size()) );

		CL_CALL( clEnqueueWriteBuffer( GetCommandQueue(),
									   _mem,
									   CL_TRUE, // blocking
									   (usize) msg->offset,
									   (usize) size,
									   msg->data.ptr(),
									   0, null,
									   null ) );

		msg->wasWritten.Set( BytesUL(size) );
		
		_SendEvent< ModuleMsg::DataRegionChanged >({ EMemoryAccess::CpuWrite, msg->offset, BytesUL(size) });
		return true;
	}
	
/*
=================================================
	_ReadFromImageMemory
=================================================
*/
	bool CL2Memory::_ReadFromImageMemory (const Message< GpuMsg::ReadFromImageMemory > &msg)
	{
		CHECK_ERR( _IsCreated() and not _memMapper.IsMapped() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuRead] );
		CHECK_ERR( msg->memOffset == BytesUL(0) );	// not supported
		CHECK_ERR( msg->mipLevel == MipmapLevel(0) );
		
		// read without mapping
		Message< GpuMsg::GetImageDescriptor >	req_descr;
		Message< GpuMsg::GetImageMemoryLayout >	req_layout;
		
		// TODO: buffer hasn't image memory layout and image descriptor...
		SendTo( _GetParents().Front(), req_descr );
		SendTo( _GetParents().Front(), req_layout );
		
		const uint3		img_size = Max( ImageUtils::ConvertSize( req_descr->result->imageType, req_descr->result->dimension ), 1u );
		
		CHECK_ERR( All( msg->offset + msg->dimension <= img_size ) );
		CHECK_ERR( msg->writableBuffer->Size() >= msg->dimension.z * msg->slicePitch );
		
		/*if ( _binding == EBindingTarget::Image )
		{
			CHECK_ERR( BytesUL(msg->rowPitch) == req_layout->result->rowPitch );
			CHECK_ERR( BytesUL(msg->slicePitch) == req_layout->result->slicePitch );
			
			const size_t	slice_pitch = (req_descr->result->imageType == EImage::Tex3D ? size_t(msg->slicePitch) : 0);

			CL_CALL( clEnqueueReadImage(
								GetCommandQueue(),
								_mem,
								CL_TRUE,	// blocking
								usize3(msg->offset).ptr(),
								usize3(msg->dimension).ptr(),
								size_t(msg->rowPitch),
								slice_pitch,
								OUT msg->writableBuffer->ptr(),
								0, null,
								null ) );
		}
		else*/
		{
			const usize		bpp		= (usize) BytesU(EPixelFormat::BitPerPixel( req_descr->result->format ));
			const usize3	offset	= usize3(msg->offset.x * bpp, msg->offset.y, msg->offset.z);
			const usize3	region	= Max( usize3(msg->dimension.x * bpp, msg->dimension.y, msg->dimension.z), 1u );

			CL_CALL( clEnqueueReadBufferRect(
								GetCommandQueue(),
								_mem,
								CL_TRUE,	// blocking
								offset.ptr(),
								usize3(0).ptr(),
								region.ptr(),
								size_t(req_layout->result->rowPitch),
								size_t(req_layout->result->slicePitch),
								size_t(msg->rowPitch),
								size_t(msg->slicePitch),
								OUT msg->writableBuffer->ptr(),
								0, null,
								null ) );
		}

		msg->result.Set( msg->writableBuffer->SubArray( 0, usize(msg->dimension.z * msg->slicePitch) ) );
		return true;
	}
	
/*
=================================================
	_WriteToImageMemory
=================================================
*/
	bool CL2Memory::_WriteToImageMemory (const Message< GpuMsg::WriteToImageMemory > &msg)
	{
		CHECK_ERR( _IsCreated() and not _memMapper.IsMapped() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuWrite] );
		CHECK_ERR( msg->memOffset == BytesUL(0) );	// not supported
		CHECK_ERR( msg->mipLevel == MipmapLevel(0) );
		CHECK_ERR( msg->data.Size() >= msg->dimension.z * msg->slicePitch );
		CHECK_ERR( IsNotZero( msg->dimension ) );

		// write without mapping
		Message< GpuMsg::GetImageDescriptor >	req_descr;
		Message< GpuMsg::GetImageMemoryLayout >	req_layout;
		
		// TODO: buffer hasn't image memory layout and image descriptor...
		SendTo( _GetParents().Front(), req_descr );
		SendTo( _GetParents().Front(), req_layout );
		
		const uint3		img_size = Max( ImageUtils::ConvertSize( req_descr->result->imageType, req_descr->result->dimension ), 1u );
		
		CHECK_ERR( All( msg->offset + msg->dimension <= img_size ) );
		CHECK_ERR( msg->data.Size() == msg->dimension.z * msg->slicePitch );

		/*if ( _binding == EBindingTarget::Image )
		{
			CHECK_ERR( BytesUL(msg->rowPitch) == req_layout->result->rowPitch );
			CHECK_ERR( BytesUL(msg->slicePitch) == req_layout->result->slicePitch );

			const size_t	slice_pitch = (req_descr->result->imageType == EImage::Tex3D ? size_t(msg->slicePitch) : 0);

			CL_CALL( clEnqueueWriteImage(
								GetCommandQueue(),
								_mem,
								CL_TRUE,	// blocking
								usize3(msg->offset).ptr(),
								usize3(msg->dimension).ptr(),
								size_t(msg->rowPitch),
								slice_pitch,
								msg->data.ptr(),
								0, null,
								null ) );
		}
		else*/
		{
			const usize		bpp		= (usize) BytesU(EPixelFormat::BitPerPixel( req_descr->result->format ));
			const usize3	offset	= usize3(msg->offset.x * bpp, msg->offset.y, msg->offset.z);
			const usize3	region	= Max( usize3(msg->dimension.x * bpp, msg->dimension.y, msg->dimension.z), 1u );

			CL_CALL( clEnqueueWriteBufferRect(
								GetCommandQueue(),
								_mem,
								CL_TRUE,	// blocking
								offset.ptr(),
								usize3(0).ptr(),
								region.ptr(),
								size_t(req_layout->result->rowPitch),
								size_t(req_layout->result->slicePitch),
								size_t(msg->rowPitch),
								size_t(msg->slicePitch),
								msg->data.ptr(),
								0, null,
								null ) );
		}

		msg->wasWritten.Set( BytesUL(msg->dimension.z * msg->slicePitch) );
		return true;
	}
		
/*
=================================================
	_Delete
=================================================
*/
	bool CL2Memory::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_FreeMemory();

		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_GpuMemoryRegionChanged
=================================================
*/
	bool CL2Memory::_GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &msg)
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
	bool CL2Memory::_GetGpuMemoryDescriptor (const Message< GpuMsg::GetGpuMemoryDescriptor > &msg)
	{
		GpuMemoryDescriptor		descr;

		descr.flags		= _flags;
		descr.size		= _size;
		descr.access	= _memMapper.MemoryAccess();

		msg->result.Set( descr );
		return true;
	}

}	// PlatformCL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenCLObjectsConstructor::CreateCL2Memory (GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci)
	{
		return New< PlatformCL::CL2Memory >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
