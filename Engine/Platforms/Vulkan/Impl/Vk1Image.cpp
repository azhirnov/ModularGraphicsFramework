// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Image.h"
#include "Engine/Platforms/Shared/GPU/Memory.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/VulkanContext.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	
	//
	// Vulkan Image
	//

	class Vk1Image final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetImageDescriptor,
											GpuMsg::GetVkImageID,
											GpuMsg::GetVkImageView,
											GpuMsg::GpuMemoryRegionChanged,
											GpuMsg::SetImageLayout,
											GpuMsg::GetImageLayout,
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
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;
		
		using MemoryEvents_t		= MessageListFrom< GpuMsg::OnMemoryBindingChanged >;
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
											GpuMsg::WriteToImageMemory >;

		using Utils					= Platforms::ImageUtils;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ImageDescriptor			_descr;
		ModulePtr				_memObj;
		vk::VkImage				_imageId;
		vk::VkImageView			_imageView;
		EImageLayout::type		_layout;
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
		bool _GetVkImageView (const Message< GpuMsg::GetVkImageView > &);
		bool _GetImageDescriptor (const Message< GpuMsg::GetImageDescriptor > &);
		bool _OnMemoryBindingChanged (const Message< GpuMsg::OnMemoryBindingChanged > &);
		bool _GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &);
		bool _SetImageLayout (const Message< GpuMsg::SetImageLayout > &);
		bool _GetImageLayout (const Message< GpuMsg::GetImageLayout > &);

	private:
		bool _IsImageCreated () const	{ return _imageId != VK_NULL_HANDLE; }
		bool _IsViewCreated () const	{ return _imageView != VK_NULL_HANDLE; }

		bool _CreateImage ();
		bool _CreateView ();

		void _DestroyAll ();
		void _DestroyView ();

		static vk::VkImageType  _GetImageType (EImage::type type);
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
		Vk1BaseModule( gs, ModuleConfig{ VkImageModuleID, ~0u }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),				_imageId( VK_NULL_HANDLE ),
		_imageView( VK_NULL_HANDLE ),	_layout( EImageLayout::Unknown ),
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
		_SubscribeOnMsg( this, &Vk1Image::_DeviceBeforeDestroy );
		_SubscribeOnMsg( this, &Vk1Image::_GetVkImageID );
		_SubscribeOnMsg( this, &Vk1Image::_GetVkImageView );
		_SubscribeOnMsg( this, &Vk1Image::_GetImageDescriptor );
		_SubscribeOnMsg( this, &Vk1Image::_SetImageLayout );
		_SubscribeOnMsg( this, &Vk1Image::_GetImageLayout );
		_SubscribeOnMsg( this, &Vk1Image::_GetDeviceInfo );
		_SubscribeOnMsg( this, &Vk1Image::_GetVkDeviceInfo );
		_SubscribeOnMsg( this, &Vk1Image::_GetVkPrivateClasses );
		_SubscribeOnMsg( this, &Vk1Image::_GpuMemoryRegionChanged );

		_AttachSelfToManager( ci.gpuThread, Platforms::VkThreadModuleID, true );

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
		ASSERT( not _IsViewCreated() );
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

		CHECK_ATTACHMENT( _memObj = GetModuleByEvent< MemoryEvents_t >() );

		_memObj->Subscribe( this, &Vk1Image::_OnMemoryBindingChanged );

		CHECK_LINKING( _CopySubscriptions< ForwardToMem_t >( _memObj ) );

		CHECK_ERR( Module::_Link_Impl( msg ) );
		return true;
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
		CHECK( _Attach( msg->name, msg->newModule, true ) );

		if ( msg->newModule->GetSupportedEvents().HasAllTypes< MemoryEvents_t >() )
		{
			CHECK( _SetState( EState::Initial ) );
			_DestroyView();
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
			_DestroyView();
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
		using namespace vk;
		
		CHECK_ERR( not _IsImageCreated() );
		CHECK_ERR( not _IsViewCreated() );

		// create image
		VkImageCreateInfo	info = {};
		info.sType			= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		info.pNext			= null;
		info.flags			= 0;
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
		
		GetDevice()->SetObjectName( _imageId, GetDebugName(), EGpuObject::Image );

		_layout = EImageLayout::Undefined;
		return true;
	}
	
/*
=================================================
	_CreateView
=================================================
*/
	bool Vk1Image::_CreateView ()
	{
		using namespace vk;
		
		CHECK_ERR( _IsImageCreated() );
		CHECK_ERR( not _IsViewCreated() );
		
		VkImageViewCreateInfo	view_info = {};

		view_info.sType			= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		view_info.pNext			= null;
		view_info.viewType		= Vk1Enum( _descr.imageType );
		view_info.flags			= 0;
		view_info.image			= _imageId;
		view_info.format		= Vk1Enum( _descr.format );
		view_info.components	= { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };

		view_info.subresourceRange.aspectMask		= EPixelFormat::IsColor( _descr.format ) ? VK_IMAGE_ASPECT_COLOR_BIT :
													  ((EPixelFormat::HasDepth( _descr.format ) ? VK_IMAGE_ASPECT_DEPTH_BIT : 0) |
													   (EPixelFormat::HasStencil( _descr.format ) ? VK_IMAGE_ASPECT_STENCIL_BIT : 0));
		view_info.subresourceRange.baseMipLevel		= 0;
		view_info.subresourceRange.levelCount		= 1;
		view_info.subresourceRange.baseArrayLayer	= 0;
		view_info.subresourceRange.layerCount		= 1;	// TODO
		
		VK_CHECK( vkCreateImageView( GetVkDevice(), &view_info, null, OUT &_imageView ) );

		GetDevice()->SetObjectName( _imageView, GetDebugName(), EGpuObject::ImageView );
		return true;
	}

/*
=================================================
	_DestroyAll
=================================================
*/
	void Vk1Image::_DestroyAll ()
	{
		using namespace vk;

		auto	dev = GetVkDevice();

		if ( dev != VK_NULL_HANDLE and _imageId != VK_NULL_HANDLE )
		{
			vkDestroyImage( dev, _imageId, null );
		}

		_DestroyView();

		_imageId	= VK_NULL_HANDLE;
		_descr		= Uninitialized;
	}
	
/*
=================================================
	_DestroyView
=================================================
*/
	void Vk1Image::_DestroyView ()
	{
		using namespace vk;

		auto	dev = GetVkDevice();

		if ( dev != VK_NULL_HANDLE and _imageView != VK_NULL_HANDLE )
		{
			vkDestroyImageView( dev, _imageView, null );
		}
		
		if ( _memObj )
		{
			_memObj->UnsubscribeAll( this );
			_memObj = null;
		}

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
	_GetVkImageView
=================================================
*/
	bool Vk1Image::_GetVkImageView (const Message< GpuMsg::GetVkImageView > &msg)
	{
		msg->result.Set( _imageView );
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
				CHECK( _CreateView() );
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
	vk::VkImageType  Vk1Image::_GetImageType (EImage::type type)
	{
		switch ( type )
		{
			case EImage::Tex1D :
			case EImage::Tex1DArray :
				return vk::VK_IMAGE_TYPE_1D;

			case EImage::Tex2D :
			case EImage::Tex2DArray :
			case EImage::Tex2DMS :
			case EImage::Tex2DMSArray :
			case EImage::TexCube :
			case EImage::TexCubeArray :
				return vk::VK_IMAGE_TYPE_2D;

			case EImage::Tex3D :
				return vk::VK_IMAGE_TYPE_3D;
		}
		return vk::VK_IMAGE_TYPE_MAX_ENUM;
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
	ModulePtr VulkanContext::_CreateVk1Image (GlobalSystemsRef gs, const CreateInfo::GpuImage &ci)
	{
		return New< PlatformVK::Vk1Image >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
