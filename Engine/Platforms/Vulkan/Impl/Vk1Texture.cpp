// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/Impl/Vk1Texture.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Memory.h"
#include "Engine/Platforms/Vulkan/VulkanThread.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	
	const Runtime::VirtualTypeList	Vk1Texture::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	Vk1Texture::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1Texture::Vk1Texture (const GlobalSystemsRef gs, const CreateInfo::GpuTexture &ci) :
		Vk1BaseModule( gs, ci.gpuThread, ModuleConfig{ GetStaticID(), ~0u }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),
		_imageId( VK_NULL_HANDLE ),
		_imageView( VK_NULL_HANDLE ),
		_isBindedToMemory( false )
	{
		SetDebugName( "Vk1Texture" );

		_SubscribeOnMsg( this, &Vk1Texture::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1Texture::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1Texture::_AttachModule_Impl );
		_SubscribeOnMsg( this, &Vk1Texture::_DetachModule_Impl );
		_SubscribeOnMsg( this, &Vk1Texture::_FindModule_Impl );
		_SubscribeOnMsg( this, &Vk1Texture::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Vk1Texture::_Link );
		_SubscribeOnMsg( this, &Vk1Texture::_Compose );
		_SubscribeOnMsg( this, &Vk1Texture::_Delete );
		_SubscribeOnMsg( this, &Vk1Texture::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1Texture::_GpuDeviceBeforeDestory );
		_SubscribeOnMsg( this, &Vk1Texture::_GetGpuTextureID );
		_SubscribeOnMsg( this, &Vk1Texture::_GetGpuTextureView );
		_SubscribeOnMsg( this, &Vk1Texture::_GetGpuTextureDescriptor );
		_SubscribeOnMsg( this, &Vk1Texture::_OnGpuMemoryBindingChanged );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, VulkanThread::GetStaticID(), true );

		_ValidateDescriptor( INOUT _descr );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Vk1Texture::~Vk1Texture ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool Vk1Texture::_Link (const Message< ModuleMsg::Link > &msg)
	{
		CHECK_ERR( _IsMutableState( GetState() ) );

		using RequestedEvents_t = MessageListFrom< ModuleMsg::OnGpuMemoryBindingChanged >;

		ModulePtr	mem;
		CHECK( _FindModuleWithEvents< RequestedEvents_t >( _GetAttachments(), OUT mem ) );

		if ( not mem )
		{
			CHECK( _SetState( EState::LinkageFailed ) );
			return false;
		}

		mem->Subscribe( this, &Vk1Texture::_OnGpuMemoryBindingChanged );

		CHECK_ERR( Module::_Link_Impl( msg ) );
		return true;
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool Vk1Texture::_Compose (const  Message< ModuleMsg::Compose > &msg)
	{
		CHECK_ERR( GetState() == EState::Linked );

		CHECK_ERR( _CreateTexture() );

		_SendForEachAttachments( Message< ModuleMsg::Compose >{ this } );
		
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
	bool Vk1Texture::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyAll();

		return Module::_Delete_Impl( msg );
	}
		
/*
=================================================
	_IsCreated
=================================================
*/
	bool Vk1Texture::_IsCreated () const
	{
		return _imageId != VK_NULL_HANDLE;
	}

/*
=================================================
	_CreateTexture
=================================================
*/
	bool Vk1Texture::_CreateTexture ()
	{
		using namespace vk;
		
		CHECK_ERR( _imageId == VK_NULL_HANDLE );

		// create image
		VkImageCreateInfo	info = {};
		info.sType			= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		info.pNext			= null;
		info.flags			= 0;
		info.imageType		= _GetImageType( _descr.target );
		info.format			= Vk1Enum( _descr.format );
		info.extent.width	= _descr.dimension.x;
		info.extent.height	= _descr.dimension.y;
		info.extent.depth	= _descr.dimension.z;
		info.mipLevels		= Clamp( _descr.maxLevel.Get(), 1u, Utils::NumberOfMipmaps( _descr.target, _descr.dimension ) );
		info.arrayLayers	= _descr.dimension.w;
		info.samples		= Vk1Enum( _descr.samples );
		info.tiling			= VK_IMAGE_TILING_OPTIMAL;	// use copy from buffer to write data to texture
		info.usage			= Vk1Enum( _descr.usage );
		info.initialLayout	= Vk1Enum( EImageLayout::Undefined );
		info.sharingMode	= VK_SHARING_MODE_EXCLUSIVE;

		if ( _descr.target == ETexture::TexCube or _descr.target == ETexture::TexCubeArray )
			info.flags |= VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

		VK_CHECK( vkCreateImage( GetLogicalDevice(), &info, null, OUT &_imageId ) );
		return true;
	}
	
/*
=================================================
	_CreateView
=================================================
*/
	bool Vk1Texture::_CreateView ()
	{
		using namespace vk;
		
		CHECK_ERR( _imageId != VK_NULL_HANDLE );
		CHECK_ERR( _imageView == VK_NULL_HANDLE );
		
		VkImageViewCreateInfo	view_info = {};

		view_info.sType			= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		view_info.pNext			= null;
		view_info.viewType		= Vk1Enum( _descr.target );
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
		
		VK_CHECK( vkCreateImageView( GetLogicalDevice(), &view_info, null, OUT &_imageView ) );
		return true;
	}

/*
=================================================
	_DestroyAll
=================================================
*/
	void Vk1Texture::_DestroyAll ()
	{
		using namespace vk;

		auto	dev = GetLogicalDevice();

		if ( dev != VK_NULL_HANDLE and _imageView != VK_NULL_HANDLE )
		{
			vkDestroyImageView( dev, _imageView, null );
		}

		if ( dev != VK_NULL_HANDLE and _imageId != VK_NULL_HANDLE )
		{
			vkDestroyImage( dev, _imageId, null );
		}

		_imageView	= VK_NULL_HANDLE;
		_imageId	= VK_NULL_HANDLE;
		_descr		= Uninitialized;
	}

/*
=================================================
	_GetGpuTextureID
=================================================
*/
	bool Vk1Texture::_GetGpuTextureID (const Message< ModuleMsg::GetGpuTextureID > &msg)
	{
		msg->result.Set( _imageId );
		msg->defaultView.Set( _imageView );
		return true;
	}
	
/*
=================================================
	_GetGpuTextureView
=================================================
*/
	bool Vk1Texture::_GetGpuTextureView (const Message< ModuleMsg::GetGpuTextureView > &msg)
	{
		msg->result.Set( _imageView );
		return true;
	}

/*
=================================================
	_GetGpuTextureDescriptor
=================================================
*/
	bool Vk1Texture::_GetGpuTextureDescriptor (const Message< ModuleMsg::GetGpuTextureDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_OnGpuMemoryBindingChanged
=================================================
*/
	bool Vk1Texture::_OnGpuMemoryBindingChanged (const Message< ModuleMsg::OnGpuMemoryBindingChanged > &msg)
	{
		CHECK_ERR( GetState() == EState::Linked or _IsComposedState( GetState() ) );

		using EBindingState = ModuleMsg::OnGpuMemoryBindingChanged::EBindingState;

		if (  msg->targetObject == this )
		{
			_isBindedToMemory = ( msg->newState == EBindingState::BindedToImage );

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
	_GetImageType
=================================================
*/
	vk::VkImageType  Vk1Texture::_GetImageType (ETexture::type type)
	{
		switch ( type )
		{
			case ETexture::Tex1D :
			case ETexture::Tex1DArray :
				return vk::VK_IMAGE_TYPE_1D;

			case ETexture::Tex2D :
			case ETexture::Tex2DArray :
			case ETexture::Tex2DMS :
			case ETexture::Tex2DMSArray :
			case ETexture::TexCube :
			case ETexture::TexCubeArray :
				return vk::VK_IMAGE_TYPE_2D;

			case ETexture::Tex3D :
				return vk::VK_IMAGE_TYPE_3D;
		}
		return vk::VK_IMAGE_TYPE_MAX_ENUM;
	}
	
/*
=================================================
	_ValidateDescriptor
=================================================
*/
	void Vk1Texture::_ValidateDescriptor (INOUT TextureDescriptor &descr)
	{
		descr.dimension = Max( descr.dimension, uint4(1) );
	}


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
