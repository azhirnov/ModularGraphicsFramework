// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Vulkan/110/Vk1BaseModule.h"
#include "Engine/Platforms/Public/Tools/ImageViewHashMap.h"
#include "Engine/Platforms/Public/Tools/ImageUtils.h"

namespace Engine
{
namespace GpuMsg
{
	
	struct SetVkSwapchainImage
	{
	// variables
		vk::VkImage				image	= VK_NULL_HANDLE;
		Out< vk::VkImageView >	result;

	// methods
		explicit SetVkSwapchainImage (vk::VkImage image) : image{image} {}
	};

}	// GpuMsg


namespace PlatformVK
{
	using namespace vk;


	//
	// Vulkan Swapchain Image
	//

	class Vk1SwapchainImage final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetImageDescriptor,
											GpuMsg::SetImageDescriptor,
											GpuMsg::GetVkImageID,
											GpuMsg::CreateVkImageView,
											GpuMsg::SetVkSwapchainImage
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;

		using Utils					= PlatformTools::ImageUtils;

		using ImageViewMap_t		= PlatformTools::ImageViewHashMap< VkImageView >;
		using ImageView_t			= ImageViewMap_t::Key_t;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ImageDescriptor			_descr;
		ImageViewMap_t			_viewMap;
		VkImage					_imageId;
		VkImageView				_imageView;		// default image view, has all mipmaps and all layers
		EImageLayout::type		_layout;


	// methods
	public:
		Vk1SwapchainImage (GlobalSystemsRef gs, const CreateInfo::GpuImage &ci);
		~Vk1SwapchainImage ();


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetVkImageID (const Message< GpuMsg::GetVkImageID > &);
		bool _CreateVkImageView (const Message< GpuMsg::CreateVkImageView > &);
		bool _GetImageDescriptor (const Message< GpuMsg::GetImageDescriptor > &);
		bool _SetImageDescriptor (const Message< GpuMsg::SetImageDescriptor > &);
		bool _SetVkSwapchainImage (const Message< GpuMsg::SetVkSwapchainImage > &);

	private:
		bool _IsImageCreated () const	{ return _imageId != VK_NULL_HANDLE; }

		bool _CreateDefaultView ();
		void _DestroyViews ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	Vk1SwapchainImage::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	Vk1SwapchainImage::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1SwapchainImage::Vk1SwapchainImage (GlobalSystemsRef gs, const CreateInfo::GpuImage &ci) :
		Vk1BaseModule( gs, ModuleConfig{ VkImageModuleID, UMax }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),				_imageId( VK_NULL_HANDLE ),	
		_imageView( VK_NULL_HANDLE ),	_layout( EImageLayout::Unknown )
	{
		SetDebugName( "Vk1SwapchainImage" );

		_SubscribeOnMsg( this, &Vk1SwapchainImage::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1SwapchainImage::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1SwapchainImage::_AttachModule_Impl );
		_SubscribeOnMsg( this, &Vk1SwapchainImage::_DetachModule_Impl );
		_SubscribeOnMsg( this, &Vk1SwapchainImage::_FindModule_Impl );
		_SubscribeOnMsg( this, &Vk1SwapchainImage::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Vk1SwapchainImage::_Link_Impl );
		_SubscribeOnMsg( this, &Vk1SwapchainImage::_Compose_Impl );
		_SubscribeOnMsg( this, &Vk1SwapchainImage::_Delete );
		_SubscribeOnMsg( this, &Vk1SwapchainImage::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1SwapchainImage::_GetVkImageID );
		_SubscribeOnMsg( this, &Vk1SwapchainImage::_CreateVkImageView );
		_SubscribeOnMsg( this, &Vk1SwapchainImage::_GetImageDescriptor );
		_SubscribeOnMsg( this, &Vk1SwapchainImage::_SetImageDescriptor );
		_SubscribeOnMsg( this, &Vk1SwapchainImage::_GetDeviceInfo );
		_SubscribeOnMsg( this, &Vk1SwapchainImage::_GetVkDeviceInfo );
		_SubscribeOnMsg( this, &Vk1SwapchainImage::_GetVkPrivateClasses );
		_SubscribeOnMsg( this, &Vk1SwapchainImage::_SetVkSwapchainImage );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );

		Utils::ValidateDescriptor( INOUT _descr );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Vk1SwapchainImage::~Vk1SwapchainImage ()
	{
		ASSERT( not _IsImageCreated() );
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool Vk1SwapchainImage::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyViews();
		
		_imageId	= VK_NULL_HANDLE;
		_descr		= Uninitialized;

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_CreateDefaultView
=================================================
*/
	bool Vk1SwapchainImage::_CreateDefaultView ()
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
	_DestroyViews
=================================================
*/
	void Vk1SwapchainImage::_DestroyViews ()
	{
		auto	dev = GetVkDevice();

		if ( dev != VK_NULL_HANDLE and not _viewMap.Empty() )
		{
			for (auto& view : _viewMap) {
				vkDestroyImageView( dev, view.second, null );
			}
		}
		_viewMap.Clear();

		_imageView = VK_NULL_HANDLE;
	}

/*
=================================================
	_GetVkImageID
=================================================
*/
	bool Vk1SwapchainImage::_GetVkImageID (const Message< GpuMsg::GetVkImageID > &msg)
	{
		msg->result.Set({ _imageId, _imageView });
		return true;
	}
	
/*
=================================================
	_CreateVkImageView
=================================================
*/
	bool Vk1SwapchainImage::_CreateVkImageView (const Message< GpuMsg::CreateVkImageView > &msg)
	{
		CHECK_ERR( _IsImageCreated() );

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
	_SetVkSwapchainImage
=================================================
*/
	bool Vk1SwapchainImage::_SetVkSwapchainImage (const Message< GpuMsg::SetVkSwapchainImage > &msg)
	{
		_DestroyViews();

		_imageId = msg->image;

		CHECK_ERR( _CreateDefaultView() );

		msg->result.Set( _imageView );
		return true;
	}

/*
=================================================
	_GetImageDescriptor
=================================================
*/
	bool Vk1SwapchainImage::_GetImageDescriptor (const Message< GpuMsg::GetImageDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_SetImageDescriptor
=================================================
*/
	bool Vk1SwapchainImage::_SetImageDescriptor (const Message< GpuMsg::SetImageDescriptor > &msg)
	{
		CHECK_ERR( GetState() == EState::Initial );

		_descr = msg->descr;

		Utils::ValidateDescriptor( INOUT _descr );
		return true;
	}

}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
