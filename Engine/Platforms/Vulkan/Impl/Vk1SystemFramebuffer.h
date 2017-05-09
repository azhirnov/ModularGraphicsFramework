// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1Device.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1RenderPass.h"
#include "Engine/Platforms/Shared/GPU/Framebuffer.h"
#include "Engine/Platforms/Vulkan/VulkanThread.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	
	//
	// Vulkan System Framebuffer
	//

	class Vk1Device::Vk1SystemFramebuffer final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Link,
											ModuleMsg::Compose
										> >
										::Append< MessageListFrom<
											ModuleMsg::GetGpuFramebufferDescriptor,
											ModuleMsg::GetGpuFramebufferID
										> >;

		using SupportedEvents_t		= MessageListFrom< ModuleMsg::Delete >;
		

	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		FramebufferDescriptor		_descr;
		vk::VkFramebuffer			_framebufferId;
		uint						_index;


	// methods
	public:
		Vk1SystemFramebuffer (const GlobalSystemsRef gs, const VkSystemsRef vkSys);
		~Vk1SystemFramebuffer ();

		bool CreateFramebuffer (const uint2 &surfaceSize, uint index,
								vk::VkRenderPass renderPass,
								vk::VkImageView colorView, EPixelFormat::type colorFormat,
								vk::VkImageView depthStencilView, EPixelFormat::type depthStencilFormat,
								ETexture::type imageType);

		static ModulePtr CreateModule (GlobalSystemsRef, const CreateInfo::GpuFramebuffer &)	{ return null; }

		static OModID::type				GetStaticID ()					{ return "vk1.sys-fb"_OModID; }
		
		FramebufferDescriptor const&	GetDescriptor ()		const	{ return _descr; }
		vk::VkFramebuffer				GetFramebufferID ()		const	{ return _framebufferId; }
		uint							GetImageIndex ()		const	{ return _index; }


	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetGpuFramebufferID (const Message< ModuleMsg::GetGpuFramebufferID > &);
		bool _GetGpuFramebufferDescriptor (const Message< ModuleMsg::GetGpuFramebufferDescriptor > &);

	private:
		bool _IsCreated () const;
		void _DestroyFramebuffer ();
	};

	
	const Runtime::VirtualTypeList	Vk1Device::Vk1SystemFramebuffer::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	Vk1Device::Vk1SystemFramebuffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1Device::Vk1SystemFramebuffer::Vk1SystemFramebuffer (const GlobalSystemsRef gs, const VkSystemsRef vkSys) :
		Vk1BaseModule( gs, vkSys, ModuleConfig{ GetStaticID(), 1 }, &_msgTypes, &_eventTypes ),
		_framebufferId( VK_NULL_HANDLE ),
		_index( -1 )
	{
		SetDebugName( "Vk1SystemFramebuffer" );

		_SubscribeOnMsg( this, &Vk1SystemFramebuffer::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1SystemFramebuffer::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1SystemFramebuffer::_AttachModule_Impl );
		_SubscribeOnMsg( this, &Vk1SystemFramebuffer::_DetachModule_Impl );
		_SubscribeOnMsg( this, &Vk1SystemFramebuffer::_FindModule_Impl );
		_SubscribeOnMsg( this, &Vk1SystemFramebuffer::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Vk1SystemFramebuffer::_Delete );
		_SubscribeOnMsg( this, &Vk1SystemFramebuffer::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1SystemFramebuffer::_GpuDeviceBeforeDestory );
		_SubscribeOnMsg( this, &Vk1SystemFramebuffer::_GetGpuFramebufferID );
		_SubscribeOnMsg( this, &Vk1SystemFramebuffer::_GetGpuFramebufferDescriptor );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( null, VulkanThread::GetStaticID(), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Vk1Device::Vk1SystemFramebuffer::~Vk1SystemFramebuffer ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	CreateFramebuffer
=================================================
*/
	bool Vk1Device::Vk1SystemFramebuffer::CreateFramebuffer (const uint2 &surfaceSize, uint index,
															 vk::VkRenderPass renderPass,
															 vk::VkImageView colorView, EPixelFormat::type colorFormat,
															 vk::VkImageView depthStencilView, EPixelFormat::type depthStencilFormat,
															 ETexture::type imageType)
	{
		using namespace vk;

		CHECK_ERR( not _IsCreated() );

		const bool							has_depth		= depthStencilView != VK_NULL_HANDLE and depthStencilFormat != EPixelFormat::Unknown;
		FixedSizeArray< VkImageView, 2 >	attachments;

		_index			= index;
		_descr.size		= surfaceSize;
		_descr.layers	= 1;

		if ( has_depth )
		{
			attachments.PushBack( depthStencilView );
			_descr.depthStencilAttachment = { imageType, ERenderTarget::FromPixelFormat( depthStencilFormat ) };
		}

		attachments.PushBack( colorView );
		_descr.colorAttachments.PushBack({ imageType, ERenderTarget::FromPixelFormat( colorFormat, 0 ) });


		VkFramebufferCreateInfo	fb_info = {};
		fb_info.sType			= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fb_info.renderPass		= renderPass;
		fb_info.attachmentCount	= attachments.Count();
		fb_info.pAttachments	= attachments.ptr();
		fb_info.width			= surfaceSize.x;
		fb_info.height			= surfaceSize.y;
		fb_info.layers			= 1;

		VK_CHECK( vkCreateFramebuffer( GetLogicalDevice(), &fb_info, null, OUT &_framebufferId ) );

		CHECK( _SetState( EState::ComposedImmutable ) );
		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool Vk1Device::Vk1SystemFramebuffer::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyFramebuffer();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetGpuFramebufferID
=================================================
*/
	bool Vk1Device::Vk1SystemFramebuffer::_GetGpuFramebufferID (const Message< ModuleMsg::GetGpuFramebufferID > &msg)
	{
		msg->result.Set( _framebufferId );
		return true;
	}

/*
=================================================
	_GetGpuFramebufferDescriptor
=================================================
*/
	bool Vk1Device::Vk1SystemFramebuffer::_GetGpuFramebufferDescriptor (const Message< ModuleMsg::GetGpuFramebufferDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_IsCreated
=================================================
*/
	bool Vk1Device::Vk1SystemFramebuffer::_IsCreated () const
	{
		return _framebufferId != VK_NULL_HANDLE;
	}

/*
=================================================
	_DestroyFramebuffer
=================================================
*/
	void Vk1Device::Vk1SystemFramebuffer::_DestroyFramebuffer ()
	{
		using namespace vk;

		auto	dev = GetLogicalDevice();

		if ( dev != VK_NULL_HANDLE and _framebufferId != VK_NULL_HANDLE )
		{
			vkDestroyFramebuffer( dev, _framebufferId, null );
		}

		_framebufferId	= VK_NULL_HANDLE;
		_descr			= Uninitialized;
		_index			= -1;
	}


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
