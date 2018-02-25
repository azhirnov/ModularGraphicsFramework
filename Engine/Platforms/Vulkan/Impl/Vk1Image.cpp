// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Image.h"
#include "Engine/Platforms/Shared/GPU/Memory.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/VulkanObjectsConstructor.h"
#include "Engine/Platforms/Shared/Tools/ImageViewHashMap.h"

#ifdef GRAPHICS_API_VULKAN

namespace Engine
{
namespace PlatformVK
{
	using namespace vk;


	//
	// Vulkan Image
	//

	class Vk1Image final : public Vk1BaseModule
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

		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetImageDescriptor,
											GpuMsg::SetImageDescriptor,
											GpuMsg::GetVkImageID,
											GpuMsg::CreateVkImageView,
											GpuMsg::GpuMemoryRegionChanged,
											GpuMsg::SetImageLayout,
											GpuMsg::GetImageLayout
										> >::Append< ForwardToMem_t >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;
		
		using MemoryEvents_t		= MessageListFrom< GpuMsg::OnMemoryBindingChanged >;

		using Utils					= Platforms::ImageUtils;

		using ImageViewMap_t	= PlatformTools::ImageViewHashMap< VkImageView >;
		using ImageView_t		= ImageViewMap_t::Key_t;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ImageDescriptor			_descr;
		ModulePtr				_memObj;
		ModulePtr				_memManager;	// optional
		ImageViewMap_t			_viewMap;
		VkImage					_imageId;
		VkImageView				_imageView;		// default image view, has all mipmaps and all layers
		EImageLayout::type		_layout;
		
		EGpuMemory::bits		_memFlags;		// -|-- this flags is requirements for memory obj, don't use it anywhere
		EMemoryAccess::bits		_memAccess;		// -|
		bool					_useMemMngr;	// -|

		bool					_isBindedToMemory;


	// methods
	public:
		Vk1Image (GlobalSystemsRef gs, const CreateInfo::GpuImage &ci);
		~Vk1Image ();


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _AttachModule (const Message< ModuleMsg::AttachModule > &);
		bool _DetachModule (const Message< ModuleMsg::DetachModule > &);
		bool _GetVkImageID (const Message< GpuMsg::GetVkImageID > &);
		bool _CreateVkImageView (const Message< GpuMsg::CreateVkImageView > &);
		bool _GetImageDescriptor (const Message< GpuMsg::GetImageDescriptor > &);
		bool _SetImageDescriptor (const Message< GpuMsg::SetImageDescriptor > &);
		bool _GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &);
		bool _SetImageLayout (const Message< GpuMsg::SetImageLayout > &);
		bool _GetImageLayout (const Message< GpuMsg::GetImageLayout > &);
		
	// event handlers
		bool _OnMemoryBindingChanged (const Message< GpuMsg::OnMemoryBindingChanged > &);


	private:
		bool _IsImageCreated () const	{ return _imageId != VK_NULL_HANDLE; }

		bool _CreateImage ();
		bool _CreateDefaultView ();

		void _DestroyAll ();
		void _DestroyViews ();

		static VkImageType  _GetImageType (EImage::type type);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	Vk1Image::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	Vk1Image::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1Image::Vk1Image (GlobalSystemsRef gs, const CreateInfo::GpuImage &ci) :
		Vk1BaseModule( gs, ModuleConfig{ VkImageModuleID, UMax }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),					_memManager( ci.memManager ),
		_imageId( VK_NULL_HANDLE ),			_imageView( VK_NULL_HANDLE ),
		_layout( EImageLayout::Unknown ),	_memFlags( ci.memFlags ),
		_memAccess( ci.access ),			_useMemMngr( ci.allocMem or ci.memManager.IsNotNull() ),
		_isBindedToMemory( false )
	{
		SetDebugName( "Vk1Image" );

		_SubscribeOnMsg( this, &Vk1Image::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1Image::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1Image::_AttachModule );
		_SubscribeOnMsg( this, &Vk1Image::_DetachModule );
		_SubscribeOnMsg( this, &Vk1Image::_FindModule_Impl );
		_SubscribeOnMsg( this, &Vk1Image::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Vk1Image::_Link );
		_SubscribeOnMsg( this, &Vk1Image::_Compose );
		_SubscribeOnMsg( this, &Vk1Image::_Delete );
		_SubscribeOnMsg( this, &Vk1Image::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1Image::_GetVkImageID );
		_SubscribeOnMsg( this, &Vk1Image::_CreateVkImageView );
		_SubscribeOnMsg( this, &Vk1Image::_GetImageDescriptor );
		_SubscribeOnMsg( this, &Vk1Image::_SetImageDescriptor );
		_SubscribeOnMsg( this, &Vk1Image::_SetImageLayout );
		_SubscribeOnMsg( this, &Vk1Image::_GetImageLayout );
		_SubscribeOnMsg( this, &Vk1Image::_GetDeviceInfo );
		_SubscribeOnMsg( this, &Vk1Image::_GetVkDeviceInfo );
		_SubscribeOnMsg( this, &Vk1Image::_GetVkPrivateClasses );
		_SubscribeOnMsg( this, &Vk1Image::_GpuMemoryRegionChanged );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );

		Utils::ValidateDescriptor( INOUT _descr );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Vk1Image::~Vk1Image ()
	{
		ASSERT( not _IsImageCreated() );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool Vk1Image::_Link (const Message< ModuleMsg::Link > &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( GetState() == EState::Initial or GetState() == EState::LinkingFailed );
		
		_memObj = GetModuleByEvent< MemoryEvents_t >();

		if ( not _memObj and _useMemMngr )
		{
			ModulePtr	mem_module;
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
								VkManagedMemoryModuleID,
								GlobalSystems(),
								CreateInfo::GpuMemory{ _GetManager(), _memManager, _memFlags, _memAccess },
								OUT mem_module ) );

			CHECK_ERR( _Attach( "mem", mem_module, true ) );
			_memObj = mem_module;
		}
		CHECK_ATTACHMENT( _memObj );

		_memObj->Subscribe( this, &Vk1Image::_OnMemoryBindingChanged );

		CHECK_LINKING( _CopySubscriptions< ForwardToMem_t >( _memObj ) );
		
		return Module::_Link_Impl( msg );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool Vk1Image::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) or _IsImageCreated() )
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
	bool Vk1Image::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyAll();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_AttachModule
=================================================
*/
	bool Vk1Image::_AttachModule (const Message< ModuleMsg::AttachModule > &msg)
	{
		const bool	is_mem	= msg->newModule->GetSupportedEvents().HasAllTypes< MemoryEvents_t >();

		CHECK( _Attach( msg->name, msg->newModule, is_mem ) );

		if ( is_mem )
		{
			CHECK( _SetState( EState::Initial ) );
			_DestroyViews();
		}
		return true;
	}
	
/*
=================================================
	_DetachModule
=================================================
*/
	bool Vk1Image::_DetachModule (const Message< ModuleMsg::DetachModule > &msg)
	{
		CHECK( _Detach( msg->oldModule ) );

		if ( msg->oldModule->GetSupportedEvents().HasAllTypes< MemoryEvents_t >() )
		{
			CHECK( _SetState( EState::Initial ) );
			_DestroyViews();
		}
		return true;
	}

/*
=================================================
	_CreateImage
=================================================
*/
	bool Vk1Image::_CreateImage ()
	{
		CHECK_ERR( not _IsImageCreated() );
		CHECK_ERR( _viewMap.Empty() );

		// create image
		VkImageCreateInfo	info = {};
		info.sType			= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		info.pNext			= null;
		info.flags			= VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT;
		info.imageType		= _GetImageType( _descr.imageType );
		info.format			= Vk1Enum( _descr.format );
		info.extent.width	= _descr.dimension.x;
		info.extent.height	= _descr.dimension.y;
		info.extent.depth	= _descr.dimension.z;
		info.mipLevels		= _descr.maxLevel.Get();
		info.arrayLayers	= _descr.dimension.w;
		info.samples		= Vk1Enum( _descr.samples );
		info.tiling			= VK_IMAGE_TILING_OPTIMAL;	// use copy from buffer to write data to texture
		info.usage			= Vk1Enum( _descr.usage );
		info.initialLayout	= Vk1Enum( EImageLayout::Undefined );	// Undefined or Preinitialized
		info.sharingMode	= VK_SHARING_MODE_EXCLUSIVE;

		if ( _descr.imageType == EImage::TexCube or _descr.imageType == EImage::TexCubeArray )
			info.flags |= VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

		VK_CHECK( vkCreateImage( GetVkDevice(), &info, null, OUT &_imageId ) );
		
		GetDevice()->SetObjectName( ReferenceCast<uint64_t>(_imageId), GetDebugName(), EGpuObject::Image );

		_layout = EImageLayout::Undefined;
		return true;
	}
	
/*
=================================================
	_CreateDefaultView
=================================================
*/
	bool Vk1Image::_CreateDefaultView ()
	{
		CHECK_ERR( _IsImageCreated() );
		CHECK_ERR( _imageView == VK_NULL_HANDLE );
		
		Message< GpuMsg::CreateVkImageView >	create;
		create->viewDescr.layerCount	= _descr.dimension.w;
		create->viewDescr.levelCount	= _descr.maxLevel.Get();

		CHECK_ERR( _CreateVkImageView( create ) );

		_imageView = *create->result;
		return true;
	}

/*
=================================================
	_DestroyAll
=================================================
*/
	void Vk1Image::_DestroyAll ()
	{
		auto	dev = GetVkDevice();

		if ( dev != VK_NULL_HANDLE and _imageId != VK_NULL_HANDLE )
		{
			vkDestroyImage( dev, _imageId, null );
		}

		_DestroyViews();

		_imageId	= VK_NULL_HANDLE;
		_descr		= Uninitialized;
	}
	
/*
=================================================
	_DestroyViews
=================================================
*/
	void Vk1Image::_DestroyViews ()
	{
		auto	dev = GetVkDevice();

		if ( dev != VK_NULL_HANDLE and not _viewMap.Empty() )
		{
			FOR( i, _viewMap ) {
				vkDestroyImageView( dev, _viewMap[i].second, null );
			}
		}
		_viewMap.Clear();
		
		if ( _memObj )
		{
			this->UnsubscribeAll( _memObj );
			_memObj->UnsubscribeAll( this );
		}

		_memObj				= null;
		_memManager			= null;
		_imageView			= VK_NULL_HANDLE;
		_isBindedToMemory	= false;
	}

/*
=================================================
	_GetVkImageID
=================================================
*/
	bool Vk1Image::_GetVkImageID (const Message< GpuMsg::GetVkImageID > &msg)
	{
		msg->result.Set( _imageId );
		msg->defaultView.Set( _imageView );
		return true;
	}
	
/*
=================================================
	_CreateVkImageView
=================================================
*/
	bool Vk1Image::_CreateVkImageView (const Message< GpuMsg::CreateVkImageView > &msg)
	{
		CHECK_ERR( _IsImageCreated() );
		CHECK_ERR( _isBindedToMemory );

		ImageView_t		descr = msg->viewDescr;

		ImageViewMap_t::Validate( INOUT descr, _descr );

		// search in cache
		VkImageView		img_view = _viewMap.Find( descr );
		
		if ( img_view != VK_NULL_HANDLE )
		{
			msg->result.Set( img_view );
			return true;
		}

		const VkComponentSwizzle	components[] = {
			VK_COMPONENT_SWIZZLE_ZERO,	// unknown
			VK_COMPONENT_SWIZZLE_R,
			VK_COMPONENT_SWIZZLE_G,
			VK_COMPONENT_SWIZZLE_B,
			VK_COMPONENT_SWIZZLE_A,
			VK_COMPONENT_SWIZZLE_ZERO,
			VK_COMPONENT_SWIZZLE_ONE
		};
		const uint4		swizzle = Min( uint4(uint(CountOf(components)-1)), descr.swizzle.ToVec() );

		// create new image view
		VkImageViewCreateInfo	view_info	= {};

		view_info.sType			= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		view_info.pNext			= null;
		view_info.viewType		= Vk1Enum( descr.viewType );
		view_info.flags			= 0;
		view_info.image			= _imageId;
		view_info.format		= Vk1Enum( descr.format );
		view_info.components	= { components[swizzle.x], components[swizzle.y], components[swizzle.z], components[swizzle.w] };

		view_info.subresourceRange.aspectMask		= EPixelFormat::IsColor( descr.format ) ? VK_IMAGE_ASPECT_COLOR_BIT :
													  ((EPixelFormat::HasDepth( descr.format ) ? VK_IMAGE_ASPECT_DEPTH_BIT : 0) |
													   (EPixelFormat::HasStencil( descr.format ) ? VK_IMAGE_ASPECT_STENCIL_BIT : 0));
		view_info.subresourceRange.baseMipLevel		= descr.baseLevel.Get();
		view_info.subresourceRange.levelCount		= descr.levelCount;
		view_info.subresourceRange.baseArrayLayer	= descr.baseLayer.Get();
		view_info.subresourceRange.layerCount		= descr.layerCount;
		
		VK_CHECK( vkCreateImageView( GetVkDevice(), &view_info, null, OUT &img_view ) );
		_viewMap.Add( descr, img_view );

		GetDevice()->SetObjectName( ReferenceCast<uint64_t>(img_view), GetDebugName(), EGpuObject::ImageView );

		msg->result.Set( img_view );
		return true;
	}

/*
=================================================
	_GetImageDescriptor
=================================================
*/
	bool Vk1Image::_GetImageDescriptor (const Message< GpuMsg::GetImageDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_SetImageDescriptor
=================================================
*/
	bool Vk1Image::_SetImageDescriptor (const Message< GpuMsg::SetImageDescriptor > &msg)
	{
		CHECK_ERR( GetState() == EState::Initial );

		_descr = msg->descr;

		Utils::ValidateDescriptor( INOUT _descr );
		return true;
	}

/*
=================================================
	_OnMemoryBindingChanged
=================================================
*/
	bool Vk1Image::_OnMemoryBindingChanged (const Message< GpuMsg::OnMemoryBindingChanged > &msg)
	{
		CHECK_ERR( GetState() == EState::Linked or _IsComposedState( GetState() ) );

		using EBindingTarget = GpuMsg::OnMemoryBindingChanged::EBindingTarget;

		if (  msg->targetObject == this )
		{
			_isBindedToMemory = ( msg->newState == EBindingTarget::Image );

			if ( _isBindedToMemory )
			{
				CHECK( _CreateDefaultView() );
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
	bool Vk1Image::_SetImageLayout (const Message< GpuMsg::SetImageLayout > &msg)
	{
		CHECK_ERR( _IsImageCreated() );

		_layout = msg->newLayout;
		return true;
	}

/*
=================================================
	_GetImageLayout
=================================================
*/
	bool Vk1Image::_GetImageLayout (const Message< GpuMsg::GetImageLayout > &msg)
	{
		msg->result.Set( _layout );
		return true;
	}

/*
=================================================
	_GetImageType
=================================================
*/
	VkImageType  Vk1Image::_GetImageType (EImage::type type)
	{
		switch ( type )
		{
			case EImage::Tex1D :
			case EImage::Tex1DArray :
				return VK_IMAGE_TYPE_1D;

			case EImage::Tex2D :
			case EImage::Tex2DArray :
			case EImage::Tex2DMS :
			case EImage::Tex2DMSArray :
			case EImage::TexCube :
			case EImage::TexCubeArray :
				return VK_IMAGE_TYPE_2D;

			case EImage::Tex3D :
				return VK_IMAGE_TYPE_3D;
		}
		RETURN_ERR( "not supported", VK_IMAGE_TYPE_MAX_ENUM );
	}

/*
=================================================
	_GpuMemoryRegionChanged
=================================================
*/
	bool Vk1Image::_GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &msg)
	{
		// request image memory barrier
		TODO( "" );
		return false;
	}

}	// PlatformVK
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr VulkanObjectsConstructor::CreateVk1Image (GlobalSystemsRef gs, const CreateInfo::GpuImage &ci)
	{
		return New< PlatformVK::Vk1Image >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
