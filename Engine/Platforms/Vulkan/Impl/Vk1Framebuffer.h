// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Shared/GPU/Framebuffer.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	
	//
	// Vulkan Framebuffer
	//

	class Vk1Framebuffer final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::GetGpuFramebufferDescriptor,
											ModuleMsg::GetVkFramebufferID
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;
		
		struct AttachmentInfo;

	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		FramebufferDescriptor		_descr;

		vk::VkFramebuffer			_framebufferId;


	// methods
	public:
		Vk1Framebuffer (const GlobalSystemsRef gs, const CreateInfo::GpuFramebuffer &ci);
		~Vk1Framebuffer ();

		static OModID::type				GetStaticID ()					{ return "vk1.fbuffer"_OModID; }


	//protected:
		FramebufferDescriptor const&	GetDescriptor ()		const	{ return _descr; }
		vk::VkFramebuffer				GetFramebufferID ()		const	{ return _framebufferId; }


	// message handlers
	private:
		bool _Compose (const  Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetVkFramebufferID (const Message< ModuleMsg::GetVkFramebufferID > &);
		bool _GetGpuFramebufferDescriptor (const Message< ModuleMsg::GetGpuFramebufferDescriptor > &);

	private:
		bool _IsCreated () const;
		bool _CreateFramebuffer ();
		void _DestroyFramebuffer ();

		bool _CreateRenderPassByAttachment (ArrayCRef<AttachmentInfo> colorAttach, const AttachmentInfo &depthStencilAttach,
											OUT vk::VkRenderPass &renderPass);
		bool _ValidateAttachment (const RenderPassDescriptor &rpDescr, ArrayCRef<AttachmentInfo> colorAttach,
								  const AttachmentInfo &depthStencilAttach);

		static void _ValidateDescriptor (INOUT FramebufferDescriptor &descr);
	};


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
