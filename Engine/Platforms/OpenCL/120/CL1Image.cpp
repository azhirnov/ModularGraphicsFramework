// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Memory.h"
#include "Engine/Platforms/OpenCL/120/CL1BaseModule.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"
#include "Engine/Platforms/Public/Tools/MemoryMapperHelper.h"
#include "Engine/Platforms/Public/Tools/ImageUtils.h"

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
	// OpenCL Image
	//

	class CL1Image final : public CL1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= CL1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											DSMsg::GetDataSourceDescription,
											//DSMsg::ReadRegion,
											//DSMsg::WriteRegion,
											GpuMsg::GetGpuMemoryDescription,
											//GpuMsg::MapImageToCpu,
											//GpuMsg::FlushMemoryRange,
											GpuMsg::UnmapMemory,
											GpuMsg::ReadFromImageMemory,
											GpuMsg::WriteToImageMemory,
											GpuMsg::GetImageDescription,
											GpuMsg::SetImageDescription,
											GpuMsg::GetCLImageID,
											GpuMsg::GetImageMemoryLayout,
											GpuMsg::GpuMemoryRegionChanged
										> >;

		using SupportedEvents_t		= CL1BaseModule::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::SetImageDescription
										> >;
		
		using MemoryEvents_t		= MessageListFrom< GpuMsg::OnMemoryBindingChanged >;
		
		using MemMapper_t			= PlatformTools::MemoryMapperHelper;
		using Utils					= PlatformTools::ImageUtils;

		using ESharing				= GpuMsg::GetCLImageID::ESharing;
		
		#ifdef GRAPHICS_API_OPENGL
		using GLImageMsgList_t		= MessageListFrom< GpuMsg::GetGLImageID >;
		#endif
		#ifdef GRAPHICS_API_DIRECTX
		using DXImageMsgList_t		= MessageListFrom< GpuMsg::GetDXImageID >;
		#endif


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ImageDescription			_descr;
		cl_mem					_imageId;
		EImageLayout::type		_layout;
		
		MemMapper_t				_memMapper;
		EGpuMemory::bits		_memFlags;

		ModulePtr				_sharedObj;
		ESharing				_sharing;


	// methods
	public:
		CL1Image (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuImage &ci);
		~CL1Image ();


	// message handlers
	private:
		bool _Link (const ModuleMsg::Link &);
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);
		bool _AttachModule (const ModuleMsg::AttachModule &);
		bool _DetachModule (const ModuleMsg::DetachModule &);
		bool _GetCLImageID (const GpuMsg::GetCLImageID &);
		bool _GetImageMemoryLayout (const GpuMsg::GetImageMemoryLayout &);
		bool _GetImageDescription (const GpuMsg::GetImageDescription &);
		bool _SetImageDescription (const GpuMsg::SetImageDescription &);
		bool _GpuMemoryRegionChanged (const GpuMsg::GpuMemoryRegionChanged &);
		bool _GetGpuMemoryDescription (const GpuMsg::GetGpuMemoryDescription &);
		bool _GetDataSourceDescription (const DSMsg::GetDataSourceDescription &);
		bool _ReadFromImageMemory (const GpuMsg::ReadFromImageMemory &);
		bool _WriteToImageMemory (const GpuMsg::WriteToImageMemory &);
		//bool _FlushMemoryRange (const GpuMsg::FlushMemoryRange &);
		bool _UnmapMemory (const GpuMsg::UnmapMemory &);

	// event handlers
		bool _OnSharedObjectComposed (const ModuleMsg::AfterCompose &);
		bool _OnSharedObjectDeleted (const ModuleMsg::Delete &);

	private:
		bool _IsCreated () const;
		bool _CreateImage ();
		bool _CreateSharedImage ();
		bool _FindSharedObjects ();
		void _DestroyAll ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	CL1Image::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	CL1Image::_eventTypes{ UninitializedT< SupportedEvents_t >() };

	static const EImageUsage::bits	supportedImageUsage	= EImageUsage::TransferDst | EImageUsage::TransferSrc |
														  EImageUsage::Storage | EImageUsage::Sampled;

/*
=================================================
	constructor
=================================================
*/
	CL1Image::CL1Image (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuImage &ci) :
		CL1BaseModule{ gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes },
		_descr{ ci.descr },					_imageId{ null },
		_layout{ EImageLayout::Unknown },	_memMapper( ci.memFlags, ci.access ),
		_memFlags{ ci.memFlags },			_sharing{ ESharing::None }
	{
		SetDebugName( "CL1Image" );

		_SubscribeOnMsg( this, &CL1Image::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &CL1Image::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &CL1Image::_AttachModule );
		_SubscribeOnMsg( this, &CL1Image::_DetachModule );
		_SubscribeOnMsg( this, &CL1Image::_FindModule_Impl );
		_SubscribeOnMsg( this, &CL1Image::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &CL1Image::_Link );
		_SubscribeOnMsg( this, &CL1Image::_Compose );
		_SubscribeOnMsg( this, &CL1Image::_Delete );
		_SubscribeOnMsg( this, &CL1Image::_OnManagerChanged );
		_SubscribeOnMsg( this, &CL1Image::_GetCLImageID );
		_SubscribeOnMsg( this, &CL1Image::_GetImageMemoryLayout );
		_SubscribeOnMsg( this, &CL1Image::_SetImageDescription );
		_SubscribeOnMsg( this, &CL1Image::_GetImageDescription );
		_SubscribeOnMsg( this, &CL1Image::_GpuMemoryRegionChanged );
		_SubscribeOnMsg( this, &CL1Image::_GetDataSourceDescription );
		_SubscribeOnMsg( this, &CL1Image::_GetGpuMemoryDescription );
		_SubscribeOnMsg( this, &CL1Image::_ReadFromImageMemory );
		_SubscribeOnMsg( this, &CL1Image::_WriteToImageMemory );
		//_SubscribeOnMsg( this, &CL1Image::_FlushMemoryRange );
		_SubscribeOnMsg( this, &CL1Image::_UnmapMemory );
		_SubscribeOnMsg( this, &CL1Image::_GetDeviceInfo );
		_SubscribeOnMsg( this, &CL1Image::_GetCLDeviceInfo );
		_SubscribeOnMsg( this, &CL1Image::_GetCLPrivateClasses );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );

		// descriptor may be invalid for sharing or for delayed initialization
		if ( Utils::IsValidDescription( _descr ) )
		{
			ASSERT( (_descr.usage & supportedImageUsage).IsNotZero() and "none of supported flags is used" );
			ASSERT( (_descr.usage & ~supportedImageUsage).IsZero() and "used unsupported flags" );

			Utils::ValidateDescription( INOUT _descr );
		}
	}

/*
=================================================
	destructor
=================================================
*/
	CL1Image::~CL1Image ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool CL1Image::_Link (const ModuleMsg::Link &msg)
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
	bool CL1Image::_FindSharedObjects ()
	{
		CHECK_ERR( not _sharedObj );
		_sharing = ESharing::None;


		#ifdef GRAPHICS_API_OPENGL
		if ( not _sharedObj )
		{
			_sharedObj = GetModuleByMsg< GLImageMsgList_t >();

			if ( _sharedObj )
			{
				_sharing = ESharing::OpenGL;
			}
		}		
		#endif	// GRAPHICS_API_OPENGL
			

		#ifdef GRAPHICS_API_DIRECTX
		if ( not _sharedObj )
		{
			_sharedObj = GetModuleByMsg< GLImageMsgList_t >();

			if ( _sharedObj )
			{
				_sharing = ESharing::DirectX;
			}
		}
		#endif	// GRAPHICS_API_DIRECTX


		// no shared objects found
		if ( not _sharedObj )
			return true;
		
		_sharedObj->Subscribe( this, &CL1Image::_OnSharedObjectComposed );
		_sharedObj->Subscribe( this, &CL1Image::_OnSharedObjectDeleted );
		_sharedObj->Subscribe( this, &CL1Image::_SetImageDescription );
		
		// copy descriptor
		GpuMsg::GetImageDescription	req_descr;
		_sharedObj->Send( req_descr );
		_descr = *req_descr.result;
		
		return true;
	}

/*
=================================================
	_Compose
=================================================
*/
	bool CL1Image::_Compose (const ModuleMsg::Compose &)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		if ( _sharedObj )
			return true;	// wait shared object composing

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreateImage() );
		
		return _DefCompose( false );
	}
	
/*
=================================================
	_OnSharedObjectComposed
=================================================
*/
	bool CL1Image::_OnSharedObjectComposed (const ModuleMsg::AfterCompose &)
	{
		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreateSharedImage() );
		
		return _DefCompose( false );
	}
	
/*
=================================================
	_OnSharedObjectDeleted
=================================================
*/
	bool CL1Image::_OnSharedObjectDeleted (const ModuleMsg::Delete &)
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
	bool CL1Image::_Delete (const ModuleMsg::Delete &msg)
	{
		_DestroyAll();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_AttachModule
=================================================
*/
	bool CL1Image::_AttachModule (const ModuleMsg::AttachModule &msg)
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
	bool CL1Image::_DetachModule (const ModuleMsg::DetachModule &msg)
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
	_IsCreated
=================================================
*/
	bool CL1Image::_IsCreated () const
	{
		return _imageId != null;
	}

/*
=================================================
	_CreateImage
=================================================
*/
	bool CL1Image::_CreateImage ()
	{
		CHECK_ERR( not _IsCreated() );
		
		CL1ChannelOrder		order		= {};
		CL1ChannelDataType	data_type	= {};
		BytesU				bpp;
		CHECK_ERR( CL1Enum( _descr.format, OUT order, OUT data_type, OUT bpp ) );

		uint4 const			size		= Max( 1u, Utils::ValidateDimension( _descr.imageType, _descr.dimension ) );
		cl_image_format		img_format	= {};
		cl_image_desc		img_desc	= {};

		img_format.image_channel_order		= cl_channel_order( order );
		img_format.image_channel_data_type	= cl_channel_type( data_type );

		img_desc.image_type			= CL1Enum( _descr.imageType );
		img_desc.image_width		= size.x;
		img_desc.image_height		= size.y;
		img_desc.image_depth		= size.z;
		img_desc.image_array_size	= size.w;

		cl_int	cl_err = 0;
		CL_CHECK(( (_imageId = clCreateImage(
							GetContext(),
							CL1Enum( _memFlags, _memMapper.MemoryAccess() ),
							&img_format,
							&img_desc,
							null,
							OUT &cl_err )), cl_err ));

		_layout	= EImageLayout::General;
		return true;
	}
	
/*
=================================================
	_CreateSharedImage
=================================================
*/
	bool CL1Image::_CreateSharedImage ()
	{
		CHECK_ERR( not _IsCreated() );
		CHECK_ERR( _sharedObj );
		
		Utils::ValidateDescription( INOUT _descr );
		
		// update memory flags
		GpuMsg::GetGpuMemoryDescription	req_descr;
		_sharedObj->Send( req_descr );
		_memFlags	= req_descr.result->flags;

		_memMapper.ResetFlags( req_descr.result->flags, req_descr.result->access );


		#ifdef GRAPHICS_API_OPENGL
		if ( _sharing == ESharing::OpenGL )
		{
			GpuMsg::GetGLImageID	req_image;
			_sharedObj->Send( req_image );
			CHECK_ERR( req_image.result.Get(0) != 0 );

			cl_int	cl_err = 0;
			CL_CHECK( ((_imageId = clCreateFromGLTexture(
								GetContext(),
								CL1Enum( _memFlags, _memMapper.MemoryAccess() ),
								PlatformGL::GL4Enum( _descr.imageType ),
								0,	// mip level
								*req_image.result,
								OUT &cl_err )), cl_err ) );
		}
		#endif

		#ifdef GRAPHICS_API_DIRECTX
		if ( _sharing == ESharing::DirectX )
		{
			TODO( "DX sharing" );
		}
		#endif

		_layout	= EImageLayout::General;
		return true;
	}

/*
=================================================
	_DestroyAll
=================================================
*/
	void CL1Image::_DestroyAll ()
	{
		if ( _imageId != null ) {
			CL_CALL( clReleaseMemObject( _imageId ) );
		}
		
		_memFlags		= Uninitialized;
		_memMapper.Clear();

		_imageId		= null;
		_layout			= Uninitialized;
		_descr			= Uninitialized;

		if ( _sharedObj )
			_sharedObj->UnsubscribeAll( this );

		_sharedObj		= null;
		_sharing	= ESharing::None;
	}

/*
=================================================
	_GetCLImageID
=================================================
*/
	bool CL1Image::_GetCLImageID (const GpuMsg::GetCLImageID &msg)
	{
		ASSERT( _IsCreated() );

		msg.result.Set({ _imageId, _sharing });
		return true;
	}
	
/*
=================================================
	_GetImageMemoryLayout
=================================================
*/
	bool CL1Image::_GetImageMemoryLayout (const GpuMsg::GetImageMemoryLayout &msg)
	{
		CHECK_ERR( _IsCreated() );
		
		size_t	row_pitch = 0;
		CL_CALL( clGetImageInfo( _imageId, CL_IMAGE_ROW_PITCH, sizeof(row_pitch), OUT &row_pitch, null ) );

		size_t	slice_pitch = 0;
		CL_CALL( clGetImageInfo( _imageId, CL_IMAGE_SLICE_PITCH, sizeof(slice_pitch), OUT &slice_pitch, null ) );

		size_t	img_size = 0;
		CL_CALL( clGetMemObjectInfo( _imageId, CL_MEM_SIZE, sizeof(img_size), OUT &img_size, null ) );

		GpuMsg::GetImageMemoryLayout::MemLayout	result;
		result.offset		= BytesUL(0);
		result.rowPitch		= BytesUL(row_pitch);
		result.slicePitch	= BytesUL(slice_pitch);
		result.dimension	= _descr.dimension.xyz();
		result.size			= BytesUL(img_size);

		if ( result.rowPitch == 0 )
			result.rowPitch = BytesUL(result.dimension.x * EPixelFormat::BitPerPixel( _descr.format ));

		if ( result.slicePitch == 0 )
			result.slicePitch = result.dimension.y * result.rowPitch;
		
		ASSERT( result.slicePitch * result.dimension.z <= result.size );	// on Intel image size may be aligned to 4Kb

		msg.result.Set( result );
		return true;
	}

/*
=================================================
	_SetImageDescription
=================================================
*/
	bool CL1Image::_SetImageDescription (const GpuMsg::SetImageDescription &msg)
	{
		CHECK_ERR( GetState() == EState::Initial );

		_descr = msg.descr;

		Utils::ValidateDescription( INOUT _descr );
		return true;
	}

/*
=================================================
	_GetImageDescription
=================================================
*/
	bool CL1Image::_GetImageDescription (const GpuMsg::GetImageDescription &msg)
	{
		msg.result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_GetDataSourceDescription
=================================================
*/
	bool CL1Image::_GetDataSourceDescription (const DSMsg::GetDataSourceDescription &msg)
	{
		DataSourceDescription	descr;
		
		descr.memoryFlags	= _memMapper.MappingAccess();
		descr.available		= _memMapper.MappedSize();
		//descr.totalSize		= _size;	// TODO

		msg.result.Set( descr );
		return true;
	}
	
/*
=================================================
	_MapImageToCpu
=================================================
*
	bool CL1Image::_MapImageToCpu (const GpuMsg::MapImageToCpu &)
	{
		/*
		CHECK_ERR( _IsCreated() and not _IsMapped() );
		CHECK_ERR( _binding == EBindingTarget::Image );
		CHECK_ERR( _access[EMemoryAccess::CpuRead] or _access[EMemoryAccess::CpuWrite] );
	
		// validate and map memory
		CHECK_ERR( All( msg.offset + msg.dimension <= _descr.dimension ) );
		
		const usize3	origin	= usize3(0);
		const usize3	region	= Max( ImageUtils::ConvertSize( _descr.imageType, _descr.dimension ), 1u );

		size_t	row_pitch	= 0;
		size_t	slice_pitch	= 0;

		cl_int	cl_err = 0;
		CL_CHECK(( _mappedPtr = (ubyte*) clEnqueueMapImage(
									GetCommandQueue(),
									_imageId,
									CL_TRUE,	// blocking
									CL1Enum( _access, msg.flags ),
									origin.ptr(), region.ptr(),
									&row_pitch, &slice_pitch,
									0, null,
									null,
									&cl_err ), cl_err ));

		// write output
		msg.pixelAlign.Set({ BytesUL(row_pitch), BytesUL(slice_pitch) });
		msg.range.Set({ BytesUL(), _size });

		_mappedSize			= _size;
		_mappedOffset		= BytesUL();
		_isMappedMemChanged	= false;* /
		return true;
	}
	
/*
=================================================
	_FlushMemoryRange
=================================================
*
	bool CL1Image::_FlushMemoryRange (const GpuMsg::FlushMemoryRange &msg)
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
	bool CL1Image::_UnmapMemory (const GpuMsg::UnmapMemory &)
	{
		CL_CHECK( clEnqueueUnmapMemObject( GetCommandQueue(),
										   _imageId,
										   _memMapper.Pointer(),
										   0, null,
										   null ) );
		
		CHECK( GetDevice()->ReleaseSharedObj( this ) );

		CHECK_ERR( _memMapper.Unmap() );
		return true;
	}

/*
=================================================
	_ReadFromImageMemory
=================================================
*/
	bool CL1Image::_ReadFromImageMemory (const GpuMsg::ReadFromImageMemory &msg)
	{
		CHECK_ERR( _IsCreated() and not _memMapper.IsMapped() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuRead] );
		CHECK_ERR( msg.memOffset == BytesUL(0) );		// not supported
		CHECK_ERR( msg.mipLevel == MipmapLevel(0) );	// not supported
		CHECK_ERR( IsNotZero( msg.dimension ) );
		
		GpuMsg::GetImageMemoryLayout	req_layout;
		_GetImageMemoryLayout( req_layout );

		CHECK_ERR( All( msg.offset + msg.dimension <= _descr.dimension.xyz() ) );
		CHECK_ERR( msg.layer.Get() < _descr.dimension.w );
		CHECK_ERR( BytesUL(msg.rowPitch) == req_layout.result->rowPitch );
		
		size_t	slice_pitch	= 0;
		usize	data_size	= 0;
		usize3	img_offset	= usize3(msg.offset);

		if ( _descr.imageType == EImage::Tex3D )
		{
			CHECK_ERR( msg.writableBuffer->Size() >= msg.dimension.z * msg.slicePitch );
			CHECK_ERR( BytesUL(msg.slicePitch) == req_layout.result->slicePitch );

			slice_pitch = size_t(msg.slicePitch);
			data_size	= usize(msg.dimension.z * msg.slicePitch);
		}
		else
		{
			CHECK_ERR( msg.dimension.z == 1 );
			CHECK_ERR( msg.writableBuffer->Size() >= msg.dimension.y * msg.rowPitch );

			data_size = usize(msg.dimension.y * msg.rowPitch);

			if ( EImage::IsArray2D( _descr.imageType ) )
			{
				img_offset.z = msg.layer.Get();
			}
		}
		
		CHECK( GetDevice()->AddSharedObj( this, _imageId, _sharing ) );
		CHECK( GetDevice()->AcquireSharedObj( this ) );

		CL_CALL( clEnqueueReadImage(
							GetCommandQueue(),
							_imageId,
							CL_TRUE,	// blocking
							img_offset.ptr(),
							usize3(msg.dimension).ptr(),
							size_t(msg.rowPitch),
							slice_pitch,
							OUT msg.writableBuffer->ptr(),
							0, null,
							null ) );
		
		CHECK( GetDevice()->ReleaseSharedObj( this ) );

		msg.result.Set( msg.writableBuffer->SubArray( 0, data_size ) );
		return true;
	}
	
/*
=================================================
	_WriteToImageMemory
=================================================
*/
	bool CL1Image::_WriteToImageMemory (const GpuMsg::WriteToImageMemory &msg)
	{
		CHECK_ERR( _IsCreated() and not _memMapper.IsMapped() );
		CHECK_ERR( _memMapper.MemoryAccess()[EMemoryAccess::CpuWrite] );
		CHECK_ERR( msg.memOffset == BytesUL(0) );		// not supported
		CHECK_ERR( msg.mipLevel == MipmapLevel(0) );	// not supported
		CHECK_ERR( IsNotZero( msg.dimension ) );
		
		GpuMsg::GetImageMemoryLayout	req_layout;
		_GetImageMemoryLayout( req_layout );
		
		CHECK_ERR( All( msg.offset + msg.dimension <= _descr.dimension.xyz() ) );
		CHECK_ERR( msg.layer.Get() < _descr.dimension.w );
		CHECK_ERR( BytesUL(msg.rowPitch) ==  req_layout.result->rowPitch );
			
		size_t	slice_pitch = 0;
		BytesUL	data_size;
		usize3	img_offset	= usize3(msg.offset);
		
		if ( _descr.imageType == EImage::Tex3D )
		{
			CHECK_ERR( msg.data.Size() == msg.dimension.z * msg.slicePitch );
			CHECK_ERR( BytesUL(msg.slicePitch) == req_layout.result->slicePitch );

			slice_pitch = size_t(msg.slicePitch);
			data_size	= msg.dimension.z * BytesUL(msg.slicePitch);
		}
		else
		{
			CHECK_ERR( msg.dimension.z == 1 );
			CHECK_ERR( msg.data.Size() >= msg.dimension.y * msg.rowPitch );
			
			data_size = msg.dimension.y * BytesUL(msg.rowPitch);
			
			if ( EImage::IsArray2D( _descr.imageType ) )
			{
				img_offset.z = msg.layer.Get();
			}
		}
		
		CHECK( GetDevice()->AddSharedObj( this, _imageId, _sharing ) );
		CHECK( GetDevice()->AcquireSharedObj( this ) );

		CL_CALL( clEnqueueWriteImage(
							GetCommandQueue(),
							_imageId,
							CL_TRUE,	// blocking
							usize3(msg.offset).ptr(),
							usize3(msg.dimension).ptr(),
							size_t(msg.rowPitch),
							slice_pitch,
							msg.data.ptr(),
							0, null,
							null ) );
		
		CHECK( GetDevice()->ReleaseSharedObj( this ) );

		msg.wasWritten.Set( data_size );
		return true;
	}

/*
=================================================
	_GpuMemoryRegionChanged
=================================================
*/
	bool CL1Image::_GpuMemoryRegionChanged (const GpuMsg::GpuMemoryRegionChanged &)
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
	bool CL1Image::_GetGpuMemoryDescription (const GpuMsg::GetGpuMemoryDescription &msg)
	{
		GpuMemoryDescription		descr;

		descr.flags		= _memFlags;
		//descr.size		= _size;	// TODO
		descr.access	= _memMapper.MemoryAccess();

		msg.result.Set( descr );
		return true;
	}

}	// PlatformCL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenCLObjectsConstructor::CreateCL1Image (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuImage &ci)
	{
		return New< PlatformCL::CL1Image >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
