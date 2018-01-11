// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Memory.h"
#include "Engine/Platforms/Shared/GPU/Buffer.h"
#include "Engine/Platforms/Shared/GPU/Image.h"
#include "Engine/Platforms/OpenCL/Impl/CL2BaseModule.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"

#if defined( COMPUTE_API_OPENCL )

namespace Engine
{
namespace PlatformCL
{

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


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		cl::cl_mem				_mem;
		ubyte *					_mappedPtr;
		BytesUL					_size;
		
		BytesUL					_mappedOffset;
		BytesUL					_mappedSize;

		EGpuMemory::bits		_flags;
		EMemoryAccess::bits		_access;
		EBindingTarget			_binding;
		
		bool					_isMappedMemChanged;


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
		bool _IsMapped () const;

		bool _AllocForImage ();
		bool _AllocForBuffer ();
		void _FreeMemory ();

		static EMemoryAccess::bits _GpuMemoryToMemoryAccess (EGpuMemory::bits flags);
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
		_mem( null ),							_mappedPtr( null ),
		_flags( ci.memFlags ),					_access( ci.access & _GpuMemoryToMemoryAccess( ci.memFlags ) ),
		_binding( EBindingTarget::Unbinded ),	_isMappedMemChanged( false )
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

		_AttachSelfToManager( ci.gpuThread, CLThreadModuleID, true );
	}

/*
=================================================
	destructor
=================================================
*/
	CL2Memory::~CL2Memory ()
	{
		ASSERT( not _IsCreated() );
		ASSERT( not _IsMapped() );
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
	_IsMapped
=================================================
*/
	bool CL2Memory::_IsMapped () const
	{
		return _mappedPtr != null;
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

		// TODO: calc size
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
		
		_size	 = req_descr->result->size;
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
		_mem				= null;
		_mappedPtr			= null;
		_size				= Uninitialized;
		_mappedOffset		= Uninitialized;
		_mappedSize			= Uninitialized;
		_flags				= Uninitialized;
		_access				= Uninitialized;
		_binding			= EBindingTarget::Unbinded;
		_isMappedMemChanged	= false;
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
		//CHECK_ERR( _GetParents().IsExist( msg.Sender() ) );

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

		descr.memoryFlags	= _access;
		descr.available		= _mappedSize;
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
		CHECK_ERR( _IsMapped() );
		CHECK_ERR( _access[EMemoryAccess::CpuRead] );
		CHECK_ERR( msg->offset < _mappedSize );

		const usize		offset	= (usize) msg->offset;
		const usize		size	= (usize) Min( _mappedSize - msg->offset, msg->size.Get( UMax ) );

		msg->result.Set( BinArrayCRef( _mappedPtr + offset, size ) );

		_SendEvent< ModuleMsg::DataRegionChanged >({ EMemoryAccess::CpuRead, BytesU(offset), BytesU(size) });
		return true;
	}
	
/*
=================================================
	_WriteToStream
=================================================
*/
	bool CL2Memory::_WriteToStream (const Message< ModuleMsg::WriteToStream > &msg)
	{
		CHECK_ERR( _IsMapped() );
		CHECK_ERR( _access[EMemoryAccess::CpuWrite] );
		CHECK_ERR( msg->offset < _mappedSize );
		
		const usize		offset		= (usize) msg->offset;
		const usize		size		= Min( usize(_mappedSize), usize(msg->data.Size()) );

		MemCopy( BinArrayRef( _mappedPtr + offset, size ), msg->data );

		msg->wasWritten.Set( BytesUL(size) );

		_isMappedMemChanged = true;

		_SendEvent< ModuleMsg::DataRegionChanged >({ EMemoryAccess::CpuWrite, BytesU(offset), BytesU(size) });
		return true;
	}
	
/*
=================================================
	_MapMemoryToCpu
=================================================
*/
	bool CL2Memory::_MapMemoryToCpu (const Message< GpuMsg::MapMemoryToCpu > &msg)
	{
		using namespace cl;
		
		CHECK_ERR( _IsCreated() and not _IsMapped() );
		CHECK_ERR( _access[EMemoryAccess::CpuRead] or _access[EMemoryAccess::CpuWrite] );
		CHECK_ERR( msg->offset < _size );
		
		const BytesUL	size = Min( _size, msg->size );

		if ( _binding == EBindingTarget::Buffer )
		{
			cl_int	cl_err = 0;
			CL_CHECK((( _mappedPtr = (ubyte*) clEnqueueMapBuffer(
										GetCommandQueue(),
										_mem,
										CL_TRUE, // blocking
										CL2Enum( _access, msg->flags ),
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

			const usize3	origin	= usize3(0);
			const usize3	region	= usize3(Platforms::ImageUtils::ConvertSize( req_descr->result->imageType, req_descr->result->dimension ));

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

		_mappedSize			= size;
		_mappedOffset		= msg->offset;
		_isMappedMemChanged	= false;
		return true;
	}
	
/*
=================================================
	_MapImageToCpu
=================================================
*/
	bool CL2Memory::_MapImageToCpu (const Message< GpuMsg::MapImageToCpu > &msg)
	{
		/*using namespace cl;
		
		CHECK_ERR( _IsCreated() and not _IsMapped() );
		CHECK_ERR( _binding == EBindingTarget::Image );
		CHECK_ERR( _access[EMemoryAccess::CpuRead] or _access[EMemoryAccess::CpuWrite] );
		
		// request image info
		Message< GpuMsg::GetImageDescriptor >	req_descr;
		SendTo( _GetParents().Front(), req_descr );
	
		// validate and map memory
		CHECK_ERR( All( msg->offset + msg->dimension <= req_descr->result->dimension ) );
		
		const usize3	origin	= usize3(0);
		const usize3	region	= usize3(Platforms::ImageUtils::ConvertSize( req_descr->result->imageType, req_descr->result->dimension ));

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
		using namespace cl;

		CHECK_ERR( _IsMapped() );
		CHECK( _flags[EGpuMemory::CoherentWithCPU] or not _isMappedMemChanged );	// changes must be flushed
		
		CL_CHECK( clEnqueueUnmapMemObject( GetCommandQueue(),
										   _mem,
										   _mappedPtr,
										   0, null,
										   null ) );

		_mappedSize			= BytesUL(0);
		_mappedPtr			= null;
		_isMappedMemChanged	= false;

		return true;
	}
	
/*
=================================================
	_ReadFromGpuMemory
=================================================
*/
	bool CL2Memory::_ReadFromGpuMemory (const Message< GpuMsg::ReadFromGpuMemory > &msg)
	{
		using namespace cl;

		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _access[EMemoryAccess::CpuRead] );
		CHECK_ERR( _binding == EBindingTarget::Buffer );
		CHECK_ERR( not msg->size or not msg->writableBuffer or (*msg->size == BytesUL(msg->writableBuffer->Size())) );

		// just read from mapped memory
		if ( _IsMapped() )
		{
			CHECK_ERR( msg->offset >= _mappedOffset );

			Message< ModuleMsg::ReadFromStream >	read_stream;
			read_stream->offset	= msg->offset - _mappedOffset;
			read_stream->size	= msg->size;
			CHECK( _SendMsg( read_stream ) );

			// copy to buffer
			if ( msg->writableBuffer )
			{
				CHECK( msg->writableBuffer->Size() >= read_stream->result->Size() );	// never gonna happen, but...

				const usize	size = (usize) Min( msg->writableBuffer->Size(), read_stream->result->Size() );

				MemCopy( *msg->writableBuffer, *read_stream->result );

				msg->result.Set( msg->writableBuffer->SubArray( 0, size ) );
			}
			else
				msg->result.Set( read_stream->result.Get() );

			return true;
		}
			
		// read without mapping
		CHECK_ERR( msg->offset < _size );
		CHECK_ERR( msg->writableBuffer and not msg->writableBuffer->Empty() );

		const usize	size = Min( usize(_size), usize(msg->writableBuffer->Size()) );

		CL_CALL( clEnqueueReadBuffer( GetCommandQueue(),
									  _mem,
									  CL_TRUE,	// blocking
									  (usize) msg->offset,
									  (usize) size,
									  (void *) msg->writableBuffer->ptr(),
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
		using namespace cl;

		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _access[EMemoryAccess::CpuWrite] );
		CHECK_ERR( _binding == EBindingTarget::Buffer );

		// just write to mapped memory
		if ( _IsMapped() )
		{
			CHECK_ERR( msg->offset >= _mappedOffset );

			Message< ModuleMsg::WriteToStream >		write_stream;
			write_stream->offset	= msg->offset - _mappedOffset;
			write_stream->data		= msg->data;

			CHECK( _SendMsg( write_stream ) );

			TODO("");
			return false;
		}
		
		// write without mapping
		CHECK_ERR( msg->offset < _size );

		const usize	size = Min( usize(_size), usize(msg->data.Size()) );

		CL_CALL( clEnqueueWriteBuffer( GetCommandQueue(),
									   _mem,
									   CL_TRUE, // blocking
									   (usize) msg->offset,
									   (usize) size,
									   msg->data.ptr(),
									   0, null,
									   null ) );

		msg->wasWritten.Set( BytesUL(size) );
		// TODO: event - region changed
		return true;
	}
	
/*
=================================================
	_ReadFromImageMemory
=================================================
*/
	bool CL2Memory::_ReadFromImageMemory (const Message< GpuMsg::ReadFromImageMemory > &msg)
	{
		// use clEnqueueReadBufferRect 

		TODO( "" );
		return true;
	}
	
/*
=================================================
	_WriteToImageMemory
=================================================
*/
	bool CL2Memory::_WriteToImageMemory (const Message< GpuMsg::WriteToImageMemory > &msg)
	{
		// use clEnqueueWriteBufferRect 

		TODO( "" );
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
	_GpuMemoryToMemoryAccess
=================================================
*/
	EMemoryAccess::bits CL2Memory::_GpuMemoryToMemoryAccess (EGpuMemory::bits flags)
	{
		EMemoryAccess::bits		result;

		result |= EMemoryAccess::GpuRead;
		result |= EMemoryAccess::GpuWrite;

		if ( flags[EGpuMemory::CoherentWithCPU] or flags[EGpuMemory::CachedInCPU] )
		{
			result |= EMemoryAccess::CpuRead;
			result |= EMemoryAccess::CpuWrite;
		}

		return result;
	}

/*
=================================================
	_GpuMemoryRegionChanged
=================================================
*/
	bool CL2Memory::_GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &msg)
	{
		CHECK_ERR( _access[EMemoryAccess::GpuWrite] );	// this message allowed only for gpu-writable memory

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
		descr.access	= _access;

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
