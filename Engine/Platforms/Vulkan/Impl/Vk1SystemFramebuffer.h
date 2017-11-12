// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1Device.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Shared/GPU/Framebuffer.h"

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
											GpuMsg::GetFramebufferDescriptor,
											GpuMsg::GetVkFramebufferID
										> >;

		using SupportedEvents_t		= MessageListFrom< ModuleMsg::Delete >;
		

	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		FramebufferDescriptor		_descr;
		vk::VkFramebuffer			_framebufferId;
		uint						_index;


	// methods
	public:
		explicit Vk1SystemFramebuffer (GlobalSystemsRef gs);
		~Vk1SystemFramebuffer ();

		bool CreateFramebuffer (const uint2 &surfaceSize, uint index,
								vk::VkRenderPass renderPass,
								vk::VkImageView colorView, EPixelFormat::type colorFormat,
								vk::VkImageView depthStencilView, EPixelFormat::type depthStencilFormat,
								EImage::type imageType);

		static ModulePtr CreateModule (GlobalSystemsRef, const CreateInfo::GpuFramebuffer &)	{ return null; }


	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetVkFramebufferID (const Message< GpuMsg::GetVkFramebufferID > &);
		bool _GetFramebufferDescriptor (const Message< GpuMsg::GetFramebufferDescriptor > &);

	private:
		bool _IsCreated () const;
		void _DestroyFramebuffer ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	Vk1Device::Vk1SystemFramebuffer::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	Vk1Device::Vk1SystemFramebuffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1Device::Vk1SystemFramebuffer::Vk1SystemFramebuffer (GlobalSystemsRef gs) :
		Vk1BaseModule( gs, ModuleConfig{ VkSystemFramebufferModuleID, 1 }, &_msgTypes, &_eventTypes ),
		_framebufferId( VK_NULL_HANDLE ),
		_index( ~0u )
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
		_SubscribeOnMsg( this, &Vk1SystemFramebuffer::_DeviceBeforeDestroy );
		_SubscribeOnMsg( this, &Vk1SystemFramebuffer::_GetVkFramebufferID );
		_SubscribeOnMsg( this, &Vk1SystemFramebuffer::_GetFramebufferDescriptor );
		_SubscribeOnMsg( this, &Vk1SystemFramebuffer::_GetDeviceInfo );
		_SubscribeOnMsg( this, &Vk1SystemFramebuffer::_GetVkDeviceInfo );
		_SubscribeOnMsg( this, &Vk1SystemFramebuffer::_GetVkPrivateClasses );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( null, Platforms::VkThreadModuleID, true );
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
															 EImage::type imageType)
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
			_descr.depthStencilAttachment = { "depth", imageType/*, ERenderTarget::FromPixelFormat( depthStencilFormat )*/ };
		}

		attachments.PushBack( colorView );
		_descr.colorAttachments.PushBack({ "color", imageType/*, ERenderTarget::FromPixelFormat( colorFormat, 0 )*/ });


		VkFramebufferCreateInfo	fb_info = {};
		fb_info.sType			= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fb_info.renderPass		= renderPass;
		fb_info.attachmentCount	= (uint32_t) attachments.Count();
		fb_info.pAttachments	= attachments.ptr();
		fb_info.width			= surfaceSize.x;
		fb_info.height			= surfaceSize.y;
		fb_info.layers			= 1;

		VK_CHECK( vkCreateFramebuffer( GetVkDevice(), &fb_info, null, OUT &_framebufferId ) );
		
		GetDevice()->SetObjectName( _framebufferId, GetDebugName(), EGpuObject::Framebuffer );

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
	_GetVkFramebufferID
=================================================
*/
	bool Vk1Device::Vk1SystemFramebuffer::_GetVkFramebufferID (const Message< GpuMsg::GetVkFramebufferID > &msg)
	{
		msg->result.Set( _framebufferId );
		return true;
	}

/*
=================================================
	_GetFramebufferDescriptor
=================================================
*/
	bool Vk1Device::Vk1SystemFramebuffer::_GetFramebufferDescriptor (const Message< GpuMsg::GetFramebufferDescriptor > &msg)
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

		auto	dev = GetVkDevice();

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
