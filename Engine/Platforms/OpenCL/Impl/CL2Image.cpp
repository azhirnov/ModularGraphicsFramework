// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Image.h"
#include "Engine/Platforms/Shared/GPU/Memory.h"
#include "Engine/Platforms/OpenCL/Impl/CL2BaseModule.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"

#ifdef COMPUTE_API_OPENCL

namespace Engine
{
namespace PlatformCL
{
	
	//
	// OpenCL Image
	//

	class CL2Image final : public CL2BaseModule
	{
	// types
	private:
		using ForwardToMem_t		= MessageListFrom< 
											ModuleMsg::GetStreamDescriptor,
											ModuleMsg::ReadFromStream,
											ModuleMsg::WriteToStream,
											GpuMsg::MapMemoryToCpu,
											GpuMsg::MapImageToCpu,
											GpuMsg::FlushMemoryRange,
											GpuMsg::UnmapMemory,
											GpuMsg::ReadFromGpuMemory,
											GpuMsg::WriteToGpuMemory,
											GpuMsg::ReadFromImageMemory,
											GpuMsg::WriteToImageMemory
										>;

		using SupportedMessages_t	= CL2BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetImageDescriptor,
											GpuMsg::SetImageDescriptor,
											GpuMsg::GetCLImageID,
											GpuMsg::GpuMemoryRegionChanged,
											GpuMsg::SetImageLayout,
											GpuMsg::GetImageLayout
										> >::Append< ForwardToMem_t >;

		using SupportedEvents_t		= CL2BaseModule::SupportedEvents_t;
		
		using MemoryEvents_t		= MessageListFrom< GpuMsg::OnMemoryBindingChanged >;

		using Utils					= Platforms::ImageUtils;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ImageDescriptor			_descr;
		ModulePtr				_memObj;
		cl::cl_mem				_imageId;
		EImageLayout::type		_layout;
		
		EGpuMemory::bits		_memFlags;		// -|-- this flags is requirements for memory obj, don't use it anywhere
		EMemoryAccess::bits		_memAccess;		// -|
		bool					_useMemMngr;	// -|

		bool					_isBindedToMemory;


	// methods
	public:
		CL2Image (GlobalSystemsRef gs, const CreateInfo::GpuImage &ci);
		~CL2Image ();


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _AttachModule (const Message< ModuleMsg::AttachModule > &);
		bool _DetachModule (const Message< ModuleMsg::DetachModule > &);
		bool _GetCLImageID (const Message< GpuMsg::GetCLImageID > &);
		bool _GetImageDescriptor (const Message< GpuMsg::GetImageDescriptor > &);
		bool _SetImageDescriptor (const Message< GpuMsg::SetImageDescriptor > &);
		bool _GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &);
		bool _SetImageLayout (const Message< GpuMsg::SetImageLayout > &);
		bool _GetImageLayout (const Message< GpuMsg::GetImageLayout > &);

	// event handlers
		bool _OnMemoryBindingChanged (const Message< GpuMsg::OnMemoryBindingChanged > &);

	private:
		bool _IsCreated () const;
		bool _CreateImage ();
		void _DestroyAll ();
		void _OnMemoryUnbinded ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	CL2Image::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	CL2Image::_eventTypes{ UninitializedT< SupportedEvents_t >() };

	static const EImageUsage::bits	supportedImageUsage	= EImageUsage::TransferDst | EImageUsage::TransferSrc |
														  EImageUsage::Storage | EImageUsage::Sampled;

/*
=================================================
	constructor
=================================================
*/
	CL2Image::CL2Image (GlobalSystemsRef gs, const CreateInfo::GpuImage &ci) :
		CL2BaseModule( gs, ModuleConfig{ CLImageModuleID, UMax }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),				_imageId( null ),
		_layout( EImageLayout::Unknown ),
		_memFlags( ci.memFlags ),		_memAccess( ci.access ),
		_useMemMngr( ci.allocMem ),		_isBindedToMemory( false )
	{
		SetDebugName( "CL2Image" );

		ASSERT( (_descr.usage & supportedImageUsage).IsNotZero() and "none of supported flags is used" );
		ASSERT( (_descr.usage & ~supportedImageUsage).IsZero() and "used unsupported flags" );

		_SubscribeOnMsg( this, &CL2Image::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &CL2Image::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &CL2Image::_AttachModule );
		_SubscribeOnMsg( this, &CL2Image::_DetachModule );
		_SubscribeOnMsg( this, &CL2Image::_FindModule_Impl );
		_SubscribeOnMsg( this, &CL2Image::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &CL2Image::_Link );
		_SubscribeOnMsg( this, &CL2Image::_Compose );
		_SubscribeOnMsg( this, &CL2Image::_Delete );
		_SubscribeOnMsg( this, &CL2Image::_OnManagerChanged );
		_SubscribeOnMsg( this, &CL2Image::_GetCLImageID );
		_SubscribeOnMsg( this, &CL2Image::_SetImageDescriptor );
		_SubscribeOnMsg( this, &CL2Image::_GetImageDescriptor );
		_SubscribeOnMsg( this, &CL2Image::_SetImageLayout );
		_SubscribeOnMsg( this, &CL2Image::_GetImageLayout );
		_SubscribeOnMsg( this, &CL2Image::_GetDeviceInfo );
		_SubscribeOnMsg( this, &CL2Image::_GetCLDeviceInfo );
		_SubscribeOnMsg( this, &CL2Image::_GetCLPrivateClasses );
		_SubscribeOnMsg( this, &CL2Image::_GpuMemoryRegionChanged );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );

		Utils::ValidateDescriptor( INOUT _descr );
	}
	
/*
=================================================
	destructor
=================================================
*/
	CL2Image::~CL2Image ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool CL2Image::_Link (const Message< ModuleMsg::Link > &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( GetState() == EState::Initial or GetState() == EState::LinkingFailed );
		
		_memObj = GetModuleByEvent< MemoryEvents_t >();

		if ( not _memObj and _useMemMngr )
		{
			ModulePtr	mem_module;
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
								CLMemoryModuleID,
								GlobalSystems(),
								CreateInfo::GpuMemory{ null, _memFlags, _memAccess },
								OUT mem_module ) );

			CHECK_ERR( _Attach( "mem", mem_module, true ) );
			_memObj = mem_module;
		}
		CHECK_ATTACHMENT( _memObj );
		
		// TODO: check shared objects

		Message< GpuMsg::GetGpuMemoryDescriptor >	req_descr;
		_memObj->Send( req_descr );
		_memFlags = req_descr->result->flags;

		_memObj->Subscribe( this, &CL2Image::_OnMemoryBindingChanged );
		
		CHECK_LINKING( _CopySubscriptions< ForwardToMem_t >( _memObj ) );
		
		return Module::_Link_Impl( msg );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool CL2Image::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreateImage() );

		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );

		// composed state will be changed when memory binded to image
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool CL2Image::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyAll();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_AttachModule
=================================================
*/
	bool CL2Image::_AttachModule (const Message< ModuleMsg::AttachModule > &msg)
	{
		const bool	is_mem	= msg->newModule->GetSupportedEvents().HasAllTypes< MemoryEvents_t >();

		CHECK( _Attach( msg->name, msg->newModule, is_mem ) );

		if ( is_mem )
		{
			CHECK( _SetState( EState::Initial ) );
			_OnMemoryUnbinded();
		}
		return true;
	}
	
/*
=================================================
	_DetachModule
=================================================
*/
	bool CL2Image::_DetachModule (const Message< ModuleMsg::DetachModule > &msg)
	{
		CHECK( _Detach( msg->oldModule ) );

		if ( msg->oldModule->GetSupportedEvents().HasAllTypes< MemoryEvents_t >() )
		{
			CHECK( _SetState( EState::Initial ) );
			_OnMemoryUnbinded();
		}
		return true;
	}

/*
=================================================
	_IsCreated
=================================================
*/
	bool CL2Image::_IsCreated () const
	{
		return _imageId != null;
	}

/*
=================================================
	_CreateImage
=================================================
*/
	bool CL2Image::_CreateImage ()
	{
		using namespace cl;
		
		CHECK_ERR( _imageId == null );
		CHECK_ERR( not _isBindedToMemory );
		
		CL2ChannelOrder		order = {};
		CL2ChannelDataType	data_type = {};
		BytesU				bpp;

		uint4 const			size = Utils::ValidateDimension( _descr.imageType, _descr.dimension );

		CHECK_ERR( CL2Enum( _descr.format, OUT order, OUT data_type, OUT bpp ) );

		cl_image_format		img_format	= {};
		cl_image_desc		img_desc	= {};

		img_format.image_channel_order		= (cl_channel_order) order;
		img_format.image_channel_data_type	= (cl_channel_type) data_type;

		img_desc.image_type			= CL2Enum( _descr.imageType );
		img_desc.image_width		= size.x;
		img_desc.image_height		= size.y;
		img_desc.image_depth		= size.z;
		img_desc.image_array_size	= size.w;

		cl_int	cl_err = 0;
		CL_CHECK(( (_imageId = clCreateImage(
							GetContext(),
							CL2Enum( _memFlags ),
							&img_format,
							&img_desc,
							null,
							OUT &cl_err )), cl_err ));

		//GetDevice()->SetObjectName( _imageId, GetDebugName(), EGpuObject::Image );

		_layout = EImageLayout::Undefined;
		return true;
	}

/*
=================================================
	_DestroyAll
=================================================
*/
	void CL2Image::_DestroyAll ()
	{
		using namespace cl;
		
		if ( _imageId != null ) {
			CL_CALL( clReleaseMemObject( _imageId ) );
		}

		_OnMemoryUnbinded();

		_imageId	= null;
		_descr		= Uninitialized;
	}
	
/*
=================================================
	_OnMemoryUnbinded
=================================================
*/
	void CL2Image::_OnMemoryUnbinded ()
	{
		if ( _memObj )
		{
			_memObj->UnsubscribeAll( this );
			_memObj = null;
		}

		_memFlags			= Uninitialized;
		_isBindedToMemory	= false;
	}

/*
=================================================
	_GetCLImageID
=================================================
*/
	bool CL2Image::_GetCLImageID (const Message< GpuMsg::GetCLImageID > &msg)
	{
		msg->result.Set( _imageId );
		return true;
	}
	
/*
=================================================
	_SetImageDescriptor
=================================================
*/
	bool CL2Image::_SetImageDescriptor (const Message< GpuMsg::SetImageDescriptor > &msg)
	{
		CHECK_ERR( GetState() == EState::Initial );

		_descr = msg->descr;

		Utils::ValidateDescriptor( INOUT _descr );
		return true;
	}

/*
=================================================
	_GetImageDescriptor
=================================================
*/
	bool CL2Image::_GetImageDescriptor (const Message< GpuMsg::GetImageDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_OnMemoryBindingChanged
=================================================
*/
	bool CL2Image::_OnMemoryBindingChanged (const Message< GpuMsg::OnMemoryBindingChanged > &msg)
	{
		CHECK_ERR( GetState() == EState::Linked or _IsComposedState( GetState() ) );

		using EBindingTarget = GpuMsg::OnMemoryBindingChanged::EBindingTarget;

		if (  msg->targetObject == this )
		{
			_isBindedToMemory = ( msg->newState == EBindingTarget::Image );

			if ( _isBindedToMemory )
			{
				CHECK( _SetState( EState::ComposedMutable ) );
			}
			else
			{
				CHECK( _SetState( EState::Linked ) );
			}
		}
		return true;
	}
	
/*
=================================================
	_SetImageLayout
=================================================
*/
	bool CL2Image::_SetImageLayout (const Message< GpuMsg::SetImageLayout > &msg)
	{
		CHECK_ERR( _IsCreated() );

		_layout = msg->newLayout;
		return true;
	}

/*
=================================================
	_GetImageLayout
=================================================
*/
	bool CL2Image::_GetImageLayout (const Message< GpuMsg::GetImageLayout > &msg)
	{
		msg->result.Set( _layout );
		return true;
	}

/*
=================================================
	_GpuMemoryRegionChanged
=================================================
*/
	bool CL2Image::_GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &msg)
	{
		// request image memory barrier
		TODO( "" );
		return false;
	}

}	// PlatformCL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenCLObjectsConstructor::CreateCL2Image (GlobalSystemsRef gs, const CreateInfo::GpuImage &ci)
	{
		return New< PlatformCL::CL2Image >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
