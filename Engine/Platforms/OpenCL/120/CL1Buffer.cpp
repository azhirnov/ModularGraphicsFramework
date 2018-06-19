// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Memory.h"
#include "Engine/Platforms/OpenCL/120/CL1BaseModule.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"
#include "Engine/Platforms/Public/Tools/MemoryMapperHelper.h"

#ifdef GRAPHICS_API_OPENGL
#	include "Engine/Platforms/OpenGL/450/GL4Messages.h"
#endif

#ifdef GRAPHICS_API_DIRECTX
#	include "Engine/Platforms/DirectX/Impl/DX11Messages.h"
#endif

namespace Engine
{
namespace PlatformCL
{
	using namespace cl;

	
	//
	// OpenCL Buffer
	//

	class CL1Buffer final : public CL1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= CL1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											DSMsg::GetDataSourceDescription,
											DSMsg::ReadRegion,
											DSMsg::WriteRegion,
											GpuMsg::GetGpuMemoryDescription,
											GpuMsg::MapMemoryToCpu,
											GpuMsg::FlushMemoryRange,
											GpuMsg::UnmapMemory,
											GpuMsg::ReadFromGpuMemory,
											GpuMsg::WriteToGpuMemory,
											GpuMsg::GetBufferDescription,
											GpuMsg::SetBufferDescription,
											GpuMsg::GetCLBufferID,
											GpuMsg::CreateCLSubBuffer,
											GpuMsg::GpuMemoryRegionChanged
										> >;

		using SupportedEvents_t		= CL1BaseModule::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::SetBufferDescription
										> >;
		
		using MemoryMsg_t			= MessageListFrom< GpuMsg::GetGpuMemoryDescription >;
		using MemoryEvents_t		= MessageListFrom< GpuMsg::OnMemoryBindingChanged >;

		using SubBufferMap_t		= HashMap< Pair<BytesUL, BytesUL>, cl_mem >;
		using ESharing				= GpuMsg::GetCLImageID::ESharing;
		
		using MemMapper_t			= PlatformTools::MemoryMapperHelper;
		
		#ifdef GRAPHICS_API_OPENGL
		using GLBufferMsgList_t		= MessageListFrom< GpuMsg::GetGLBufferID >;
		#endif
		#ifdef GRAPHICS_API_DIRECTX
		using DXBufferMsgList_t		= MessageListFrom< GpuMsg::GetDXBufferID >;
		#endif


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		BufferDescription		_descr;
		cl_mem					_bufferId;
		SubBufferMap_t			_subBuffers;
		
		MemMapper_t				_memMapper;
		EGpuMemory::bits		_memFlags;
		
		ModulePtr				_sharedObj;
		ESharing				_sharing;


	// methods
	public:
		CL1Buffer (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci);
		~CL1Buffer ();


	// message handlers
	private:
		bool _Link (const ModuleMsg::Link &);
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);
		bool _AttachModule (const ModuleMsg::AttachModule &);
		bool _DetachModule (const ModuleMsg::DetachModule &);

		bool _GetCLBufferID (const GpuMsg::GetCLBufferID &);
		bool _CreateCLSubBuffer (const GpuMsg::CreateCLSubBuffer &);
		bool _SetBufferDescription (const GpuMsg::SetBufferDescription &);
		bool _GetBufferDescription (const GpuMsg::GetBufferDescription &);
		bool _GpuMemoryRegionChanged (const GpuMsg::GpuMemoryRegionChanged &);
		bool _GetGpuMemoryDescription (const GpuMsg::GetGpuMemoryDescription &);
		bool _GetDataSourceDescription (const DSMsg::GetDataSourceDescription &);
		bool _ReadFromGpuMemory (const GpuMsg::ReadFromGpuMemory &);
		bool _WriteToGpuMemory (const GpuMsg::WriteToGpuMemory &);
		bool _ReadRegion (const DSMsg::ReadRegion &);
		bool _WriteRegion (const DSMsg::WriteRegion &);
		bool _MapMemoryToCpu (const GpuMsg::MapMemoryToCpu &);
		bool _FlushMemoryRange (const GpuMsg::FlushMemoryRange &);
		bool _UnmapMemory (const GpuMsg::UnmapMemory &);

	// event handlers
		bool _OnSharedObjectComposed (const ModuleMsg::AfterCompose &);
		bool _OnSharedObjectDeleted (const ModuleMsg::Delete &);


	private:
		bool _IsCreated () const;
		bool _CreateBuffer ();
		bool _CreateSharedBuffer ();
		bool _FindSharedObjects ();
		void _DestroyAll ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	CL1Buffer::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	CL1Buffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

	static const EBufferUsage::bits	supportedBufferUsage	= EBufferUsage::Storage | EBufferUsage::Uniform |
															  EBufferUsage::TransferDst | EBufferUsage::TransferSrc;

/*
=================================================
	constructor
=================================================
*/
	CL1Buffer::CL1Buffer (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci) :
		CL1BaseModule( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),						_bufferId( null ),
		_memMapper( ci.memFlags, ci.access ),	_memFlags{ ci.memFlags },
		_sharing{ ESharing::None }
	{
		SetDebugName( "CL1Buffer" );
		
		_SubscribeOnMsg( this, &CL1Buffer::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &CL1Buffer::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &CL1Buffer::_AttachModule );
		_SubscribeOnMsg( this, &CL1Buffer::_DetachModule );
		_SubscribeOnMsg( this, &CL1Buffer::_FindModule_Impl );
		_SubscribeOnMsg( this, &CL1Buffer::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &CL1Buffer::_Link );
		_SubscribeOnMsg( this, &CL1Buffer::_Compose );
		_SubscribeOnMsg( this, &CL1Buffer::_Delete );
		_SubscribeOnMsg( this, &CL1Buffer::_OnManagerChanged );
		_SubscribeOnMsg( this, &CL1Buffer::_GetCLBufferID );
		_SubscribeOnMsg( this, &CL1Buffer::_CreateCLSubBuffer );
		_SubscribeOnMsg( this, &CL1Buffer::_SetBufferDescription );
		_SubscribeOnMsg( this, &CL1Buffer::_GetBufferDescription );
		_SubscribeOnMsg( this, &CL1Buffer::_GpuMemoryRegionChanged );
		_SubscribeOnMsg( this, &CL1Buffer::_GetDataSourceDescription );
		_SubscribeOnMsg( this, &CL1Buffer::_GetGpuMemoryDescription );
		_SubscribeOnMsg( this, &CL1Buffer::_ReadFromGpuMemory );
		_SubscribeOnMsg( this, &CL1Buffer::_WriteToGpuMemory );
		_SubscribeOnMsg( this, &CL1Buffer::_ReadRegion );
		_SubscribeOnMsg( this, &CL1Buffer::_WriteRegion );
		_SubscribeOnMsg( this, &CL1Buffer::_MapMemoryToCpu );
		_SubscribeOnMsg( this, &CL1Buffer::_FlushMemoryRange );
		_SubscribeOnMsg( this, &CL1Buffer::_UnmapMemory );
		_SubscribeOnMsg( this, &CL1Buffer::_GetDeviceInfo );
		_SubscribeOnMsg( this, &CL1Buffer::_GetCLDeviceInfo );
		_SubscribeOnMsg( this, &CL1Buffer::_GetCLPrivateClasses );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );

		if ( _descr.size > 0 and _descr.usage.IsNotZero() )
		{
			ASSERT( (_descr.usage & supportedBufferUsage).IsNotZero() and "none of supported flags is used" );
			ASSERT( (_descr.usage & ~supportedBufferUsage).IsZero() and "used unsupported flags" );
		}
	}
	
/*
=================================================
	destructor
=================================================
*/
	CL1Buffer::~CL1Buffer ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool CL1Buffer::_Link (const ModuleMsg::Link &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( _IsInitialState( GetState() ) );
		
		CHECK_LINKING( _FindSharedObjects() );

		Module::_Link_Impl( msg );
		
		if ( _sharedObj and _IsComposedState( _sharedObj->GetState() ) )
			_OnSharedObjectComposed({});

		return true;
	}
	
/*
=================================================
	_FindSharedObjects
=================================================
*/
	bool CL1Buffer::_FindSharedObjects ()
	{
		CHECK_ERR( not _sharedObj );
		_sharing = ESharing::None;


		#ifdef GRAPHICS_API_OPENGL
		if ( not _sharedObj )
		{
			_sharedObj = GetModuleByMsg< GLBufferMsgList_t >();

			if ( _sharedObj )
			{
				_sharing = ESharing::OpenGL;
			}
		}		
		#endif	// GRAPHICS_API_OPENGL
			

		#ifdef GRAPHICS_API_DIRECTX
		if ( not _sharedObj )
		{
			_sharedObj = GetModuleByMsg< GLBufferMsgList_t >();

			if ( _sharedObj )
			{
				_sharing = ESharing::DirectX;
			}
		}
		#endif	// GRAPHICS_API_DIRECTX


		// no shared objects found
		if ( not _sharedObj )
			return true;
		
		_sharedObj->Subscribe( this, &CL1Buffer::_OnSharedObjectComposed );
		_sharedObj->Subscribe( this, &CL1Buffer::_OnSharedObjectDeleted );
		_sharedObj->Subscribe( this, &CL1Buffer::_SetBufferDescription );
		
		// copy descriptor
		GpuMsg::GetBufferDescription		req_descr;
		_sharedObj->Send( req_descr );
		_descr = *req_descr.result;
		
		return true;
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool CL1Buffer::_Compose (const ModuleMsg::Compose &)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed
		
		if ( _sharedObj )
			return true;	// wait shared object composing

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreateBuffer() );
		
		return _DefCompose( false );
	}
	
/*
=================================================
	_OnSharedObjectComposed
=================================================
*/
	bool CL1Buffer::_OnSharedObjectComposed (const ModuleMsg::AfterCompose &)
	{
		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreateSharedBuffer() );
		
		return _DefCompose( false );
	}
	
/*
=================================================
	_OnSharedObjectDeleted
=================================================
*/
	bool CL1Buffer::_OnSharedObjectDeleted (const ModuleMsg::Delete &)
	{
		_DestroyAll();

		CHECK( _SetState( EState::Initial ) );
		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool CL1Buffer::_Delete (const ModuleMsg::Delete &msg)
	{
		_DestroyAll();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_AttachModule
=================================================
*/
	bool CL1Buffer::_AttachModule (const ModuleMsg::AttachModule &msg)
	{
		// memory objects is not supported in OpenCL 1.2
		CHECK_ERR( not msg.newModule->GetSupportedEvents().HasAnyType< MemoryEvents_t >() );

		CHECK( _Attach( msg.name, msg.newModule ) );

		return true;
	}
	
/*
=================================================
	_DetachModule
=================================================
*/
	bool CL1Buffer::_DetachModule (const ModuleMsg::DetachModule &msg)
	{
		CHECK( _Detach( msg.oldModule ) );

		if ( msg.oldModule == _sharedObj )
		{
			_OnSharedObjectDeleted({});
		}
		return true;
	}
	
/*
=================================================
	_SetBufferDescription
=================================================
*/
	bool CL1Buffer::_SetBufferDescription (const GpuMsg::SetBufferDescription &msg)
	{
		CHECK_ERR( GetState() == EState::Initial );

		_descr = msg.descr;
		return true;
	}

/*
=================================================
	_GetBufferDescription
=================================================
*/
	bool CL1Buffer::_GetBufferDescription (const GpuMsg::GetBufferDescription &msg)
	{
		msg.result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_GetCLBufferID
=================================================
*/
	bool CL1Buffer::_GetCLBufferID (const GpuMsg::GetCLBufferID &msg)
	{
		ASSERT( _IsCreated() );

		msg.result.Set({ _bufferId, _sharing });
		return true;
	}
	
/*
=================================================
	_GetCLBufferID
=================================================
*/
	bool CL1Buffer::_CreateCLSubBuffer (const GpuMsg::CreateCLSubBuffer &msg)
	{
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( msg.offset < _descr.size and msg.offset + msg.size <= _descr.size );

		if ( msg.offset == BytesUL(0) and msg.size == _descr.size )
		{
			msg.result.Set({ _bufferId, _sharing });
			return true;
		}

		SubBufferMap_t::iterator		iter;
		const SubBufferMap_t::Key_t		key {msg.offset, msg.size};

		if ( not _subBuffers.Find( key, OUT iter ) )
		{
			struct cl_buffer_region {
				size_t origin;
				size_t size;
			};

			cl_buffer_region	info	= { size_t(msg.offset), size_t(msg.size) };
			cl_int				cl_err	= 0;
			cl_mem				sub_buf = null;
			CL_CALL( (sub_buf = clCreateSubBuffer(	_bufferId,
													CL1Enum( _memFlags, _memMapper.MemoryAccess() ),
													CL_BUFFER_CREATE_TYPE_REGION,
													&info,
													OUT &cl_err )), cl_err );

			iter = _subBuffers.Add( key, sub_buf );
		}

		msg.result.Set({ iter->second, _sharing });
		return true;
	}

/*
=================================================
	_IsCreated
=================================================
*/
	bool CL1Buffer::_IsCreated () const
	{
		return _bufferId != null;
	}
	
/*
=================================================
	_CreateBuffer
=================================================
*/
	bool CL1Buffer::_CreateBuffer ()
	{
		CHECK_ERR( not _IsCreated() );
		
		cl_int	cl_err = 0;
		CL_CHECK(( (_bufferId = clCreateBuffer(
							GetContext(),
							CL1Enum( _memFlags, _memMapper.MemoryAccess() ),
							size_t(_descr.size),
							null,
							OUT &cl_err )), cl_err ));
		return true;
	}
	
/*
=================================================
	_CreateSharedBuffer
=================================================
*/
	bool CL1Buffer::_CreateSharedBuffer ()
	{
		CHECK_ERR( not _IsCreated() );
		CHECK_ERR( _sharedObj );
		
		// update memory flags
		GpuMsg::GetGpuMemoryDescription	req_descr;
		_sharedObj->Send( req_descr );
		_memFlags	= req_descr.result->flags;

		_memMapper.ResetFlags( req_descr.result->flags, req_descr.result->access );


		#ifdef GRAPHICS_API_OPENGL
		if ( _sharing == ESharing::OpenGL )
		{
			GpuMsg::GetGLBufferID	req_buf;
			_sharedObj->Send( req_buf );
			CHECK_ERR( req_buf.result.Get(0) != 0 );

			cl_int	cl_err = 0;
			CL_CHECK( ((_bufferId = clCreateFromGLBuffer(
								GetContext(),
								CL1Enum( _memFlags, _memMapper.MemoryAccess() ),
								*req_buf.result,
								OUT &cl_err )), cl_err ) );
		}
		#endif

		#ifdef GRAPHICS_API_DIRECTX
		if ( _sharing == ESharing::DirectX )
		{
			TODO( "DX sharing" );
		}
		#endif

		return true;
	}

/*
=================================================
	_DestroyAll
=================================================
*/
	void CL1Buffer::_DestroyAll ()
	{
		FOR( i, _subBuffers ) {
			CL_CALL( clReleaseMemObject( _subBuffers[i].second ) );
		}
		_subBuffers.Clear();

		if ( _bufferId != null ) {
			CL_CALL( clReleaseMemObject( _bufferId ) );
		}
		
		_memFlags		= Uninitialized;
		_memMapper.Clear();

		_bufferId		= null;
		_descr			= Uninitialized;
		
		if ( _sharedObj )
			_sharedObj->UnsubscribeAll( this );

		_sharedObj	= null;
		_sharing	= ESharing::None;
	}
	
/*
=================================================
	_GetDataSourceDescription
=================================================
*/
	bool CL1Buffer::_GetDataSourceDescription (const DSMsg::GetDataSourceDescription &msg)
	{
		DataSourceDescription	descr;
		
		descr.memoryFlags	= _memMapper.MappingAccess();
		descr.available		= _memMapper.MappedSize();
		descr.totalSize		= _descr.size;

		msg.result.Set( descr );
		return true;
	}
	
/*
=================================================
	_ReadRegion
=================================================
*/
	bool CL1Buffer::_ReadRegion (const DSMsg::ReadRegion &msg)
	{
		BinArrayCRef	data;
		CHECK_ERR( _memMapper.Read( msg.position, BytesUL(msg.writableBuffer->Size()), OUT data ) );

		MemCopy( *msg.writableBuffer, data );

		msg.result.Set( msg.writableBuffer->SubArray( 0, data.Count() ) );
		
		//_SendEvent( ModuleMsg::DataRegionChanged{ EMemoryAccess::CpuRead, _memMapper.MappedOffset() + msg.offset, BytesUL(data.Size()) });
		return true;
	}
	
/*
=================================================
	_WriteRegion
=================================================
*/
	bool CL1Buffer::_WriteRegion (const DSMsg::WriteRegion &msg)
	{
		BytesUL	written;
		CHECK_ERR( _memMapper.Write( msg.data, msg.position, OUT written ) );
		msg.wasWritten.Set( written );

		//_SendEvent( ModuleMsg::DataRegionChanged{ EMemoryAccess::CpuWrite, _memMapper.MappedOffset() + msg.offset, written });
		return true;
	}
	
/*
=================================================
	_MapMemoryToCpu
=================================================
*/
	bool CL1Buffer::_MapMemoryToCpu (const GpuMsg::MapMemoryToCpu &msg)
	{
		CHECK_ERR( _IsCreated() and _memMapper.IsMappingAllowed( msg.flags ) );
		CHECK_ERR( msg.position < _descr.size );
		
		const BytesUL	size	= Min( _descr.size, msg.size );
		void *			ptr		= null;

		CHECK( GetDevice()->AddSharedObj( this, _bufferId, _sharing ) );
		CHECK( GetDevice()->AcquireSharedObj( this ) );

		cl_int	cl_err = 0;
		CL_CHECK((( ptr = clEnqueueMapBuffer(
									GetCommandQueue(),
									_bufferId,
									CL_TRUE, // blocking
									CL1Enum( _memMapper.MemoryAccess(), msg.flags ),
									size_t(msg.position),
									size_t(size),
									0, null,
									null,
									&cl_err )), cl_err ));
		
		_memMapper.OnMapped( ptr, msg.position, size, msg.flags );

		msg.result.Set( BinArrayRef::FromVoid( ptr, BytesU(size) ) );
		return true;
	}
	
/*
=================================================
	_FlushMemoryRange
=================================================
*/
	bool CL1Buffer::_FlushMemoryRange (const GpuMsg::FlushMemoryRange &msg)
	{
		CHECK_ERR( _memMapper.FlushMemoryRange( msg.offset, msg.size ) );

		TODO("");
		return true;
	}
	
/*
=================================================
	_UnmapMemory
=================================================
*/
	bool CL1Buffer::_UnmapMemory (const GpuMsg::UnmapMemory &)
	{
		CL_CHECK( clEnqueueUnmapMemObject( GetCommandQueue(),
										   _bufferId,
										   _memMapper.Pointer(),
										   0, null,
										   null ) );
		
		CHECK( GetDevice()->ReleaseSharedObj( this ) );

		CHECK_ERR( _memMapper.Unmap() );
		return true;
	}
	
/*
=================================================
	_ReadFromGpuMemory
=================================================
*/
	bool CL1Buffer::_ReadFromGpuMemory (const GpuMsg::ReadFromGpuMemory &msg)
	{
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuRead] );
		CHECK_ERR( msg.writableBuffer->Size() > 0 );
		CHECK_ERR( msg.position < _descr.size );
		
		const BytesUL	req_size = BytesUL(msg.writableBuffer->Size());

		// read from mapped memory
		if ( _memMapper.IsMapped() )
		{
			CHECK_ERR( msg.position >= _memMapper.MappedOffset() and
					   msg.position + req_size <= _memMapper.MappedOffset() + _memMapper.MappedSize() );

			// read
			DSMsg::ReadRegion	read_stream;
			read_stream.position		= msg.position - _memMapper.MappedOffset();
			read_stream.writableBuffer	= msg.writableBuffer;

			CHECK( _ReadRegion( read_stream ) );
			
			// copy to writable buffer
			CHECK( msg.writableBuffer->Size() >= read_stream.result->Size() );

			MemCopy( *msg.writableBuffer, *read_stream.result );
			msg.result.Set( msg.writableBuffer->SubArray( 0, usize(read_stream.result->Size()) ) );
			return true;
		}

		// read without mapping
		const usize		size = Min( usize(_descr.size - msg.position), usize(req_size) );
		
		CHECK( GetDevice()->AddSharedObj( this, _bufferId, _sharing ) );
		CHECK( GetDevice()->AcquireSharedObj( this ) );

		CL_CALL( clEnqueueReadBuffer( GetCommandQueue(),
									  _bufferId,
									  CL_TRUE,	// blocking
									  size_t(msg.position),
									  size_t(size),
									  OUT msg.writableBuffer->ptr(),
									  0, null,
									  null ) );
		
		CHECK( GetDevice()->ReleaseSharedObj( this ) );

		msg.result.Set( msg.writableBuffer->SubArray( 0, size ) );
		return true;
	}
		
/*
=================================================
	_WriteToGpuMemory
=================================================
*/
	bool CL1Buffer::_WriteToGpuMemory (const GpuMsg::WriteToGpuMemory &msg)
	{
		CHECK_ERR( _IsCreated() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuWrite] );
		CHECK_ERR( msg.position < _descr.size );
		
		// write to mapped memory
		if ( _memMapper.IsMapped() )
		{
			CHECK_ERR( msg.position >= _memMapper.MappedOffset() and
					   msg.position + BytesUL(msg.data.Size()) <= _memMapper.MappedOffset() + _memMapper.MappedSize() );

			// write
			DSMsg::WriteRegion	write_stream;
			write_stream.position	= msg.position - _memMapper.MappedOffset();
			write_stream.data		= msg.data;

			CHECK( _WriteRegion( write_stream ) );

			// flush
			if ( not _memFlags[ EGpuMemory::CoherentWithCPU ] )
			{
				GpuMsg::FlushMemoryRange	flush;
				flush.offset	= write_stream.position;
				flush.size		= write_stream.wasWritten.Get( UMax );

				CHECK( _FlushMemoryRange( flush ) );
			}

			msg.wasWritten.Set( *write_stream.wasWritten );
			return true;
		}

		// write without mapping
		const usize		size = Min( usize(_descr.size - msg.position), usize(msg.data.Size()) );
		
		CHECK( GetDevice()->AddSharedObj( this, _bufferId, _sharing ) );
		CHECK( GetDevice()->AcquireSharedObj( this ) );

		CL_CALL( clEnqueueWriteBuffer( GetCommandQueue(),
									   _bufferId,
									   CL_TRUE, // blocking
									   size_t(msg.position),
									   size_t(size),
									   msg.data.ptr(),
									   0, null,
									   null ) );
		
		CHECK( GetDevice()->ReleaseSharedObj( this ) );

		msg.wasWritten.Set( BytesUL(size) );
		
		//_SendEvent( ModuleMsg::DataRegionChanged{ EMemoryAccess::CpuWrite, msg.offset, BytesUL(size) });
		return true;
	}
	
/*
=================================================
	_ReadFromImageMemory
=================================================
*
	bool CL1Buffer::_ReadFromImageMemory (const GpuMsg::ReadFromImageMemory &msg)
	{
		CHECK_ERR( _IsCreated() and not _memMapper.IsMapped() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuRead] );
		CHECK_ERR( msg.memOffset == BytesUL(0) );	// not supported
		CHECK_ERR( msg.mipLevel == MipmapLevel(0) );
		
		// read without mapping
		const uint3		img_size = Max( ImageUtils::ConvertSize( _descr.imageType, _descr.dimension ), 1u );
		
		CHECK_ERR( All( msg.offset + msg.dimension <= img_size ) );
		CHECK_ERR( msg.writableBuffer->Size() >= msg.dimension.z * msg.slicePitch );
		
		CHECK( GetDevice()->AddSharedObj( this, _bufferId, _sharing ) );
		CHECK( GetDevice()->AcquireSharedObj( this ) );

		const usize		bpp		= (usize) BytesU(EPixelFormat::BitPerPixel( req_descr->result->format ));
		const usize3	offset	= usize3(msg.offset.x * bpp, msg.offset.y, msg.offset.z);
		const usize3	region	= Max( usize3(msg.dimension.x * bpp, msg.dimension.y, msg.dimension.z), 1u );

		CL_CALL( clEnqueueReadBufferRect(
							GetCommandQueue(),
							_bufferId,
							CL_TRUE,	// blocking
							offset.ptr(),
							usize3(0).ptr(),
							region.ptr(),
							size_t(req_layout->result->rowPitch),
							size_t(req_layout->result->slicePitch),
							size_t(msg.rowPitch),
							size_t(msg.slicePitch),
							OUT msg.writableBuffer->ptr(),
							0, null,
							null ) );
		
		CHECK( GetDevice()->ReleaseSharedObj( this ) );

		msg.result.Set( msg.writableBuffer->SubArray( 0, usize(msg.dimension.z * msg.slicePitch) ) );
		return true;
	}
	
/*
=================================================
	_WriteToImageMemory
=================================================
*
	bool CL1Buffer::_WriteToImageMemory (const GpuMsg::WriteToImageMemory &msg)
	{
		CHECK_ERR( _IsCreated() and not _memMapper.IsMapped() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuWrite] );
		CHECK_ERR( msg.memOffset == BytesUL(0) );	// not supported
		CHECK_ERR( msg.mipLevel == MipmapLevel(0) );
		CHECK_ERR( msg.data.Size() >= msg.dimension.z * msg.slicePitch );
		CHECK_ERR( IsNotZero( msg.dimension ) );

		const uint3		img_size = Max( ImageUtils::ConvertSize( req_descr->result->imageType, req_descr->result->dimension ), 1u );
		
		CHECK_ERR( All( msg.offset + msg.dimension <= img_size ) );
		CHECK_ERR( msg.data.Size() == msg.dimension.z * msg.slicePitch );
		
		CHECK( GetDevice()->AddSharedObj( this, _bufferId, _sharing ) );
		CHECK( GetDevice()->AcquireSharedObj( this ) );

		const usize		bpp		= (usize) BytesU(EPixelFormat::BitPerPixel( req_descr->result->format ));
		const usize3	offset	= usize3(msg.offset.x * bpp, msg.offset.y, msg.offset.z);
		const usize3	region	= Max( usize3(msg.dimension.x * bpp, msg.dimension.y, msg.dimension.z), 1u );

		CL_CALL( clEnqueueWriteBufferRect(
							GetCommandQueue(),
							_bufferId,
							CL_TRUE,	// blocking
							offset.ptr(),
							usize3(0).ptr(),
							region.ptr(),
							size_t(req_layout->result->rowPitch),
							size_t(req_layout->result->slicePitch),
							size_t(msg.rowPitch),
							size_t(msg.slicePitch),
							msg.data.ptr(),
							0, null,
							null ) );
		
		CHECK( GetDevice()->ReleaseSharedObj( this ) );

		msg.wasWritten.Set( BytesUL(msg.dimension.z * msg.slicePitch) );
		return true;
	}

/*
=================================================
	_GpuMemoryRegionChanged
=================================================
*/
	bool CL1Buffer::_GpuMemoryRegionChanged (const GpuMsg::GpuMemoryRegionChanged &)
	{
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::GpuWrite] );	// this message allowed only for gpu-writable memory

		// request memory barrier
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_GetGpuMemoryDescription
=================================================
*/
	bool CL1Buffer::_GetGpuMemoryDescription (const GpuMsg::GetGpuMemoryDescription &msg)
	{
		GpuMemoryDescription		descr;

		descr.flags		= _memFlags;
		descr.size		= _descr.size;
		descr.access	= _memMapper.MemoryAccess();

		msg.result.Set( descr );
		return true;
	}

}	// PlatformCL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenCLObjectsConstructor::CreateCL1Buffer (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci)
	{
		return New< PlatformCL::CL1Buffer >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
