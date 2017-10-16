// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Image.h"
#include "Engine/Platforms/Shared/GPU/RenderPass.h"
#include "Engine/Platforms/Shared/GPU/Framebuffer.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/VulkanContext.h"

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
											GpuMsg::GetFramebufferDescriptor,
											GpuMsg::GetVkFramebufferID
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


	// message handlers
	private:
		bool _Compose (const  Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetVkFramebufferID (const Message< GpuMsg::GetVkFramebufferID > &);
		bool _GetFramebufferDescriptor (const Message< GpuMsg::GetFramebufferDescriptor > &);

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
//-----------------------------------------------------------------------------


	
	const Runtime::VirtualTypeList	Vk1Framebuffer::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	Vk1Framebuffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1Framebuffer::Vk1Framebuffer (const GlobalSystemsRef gs, const CreateInfo::GpuFramebuffer &ci) :
		Vk1BaseModule( gs, ci.gpuThread, ModuleConfig{ VkFramebufferModuleID, ~0u }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),
		_framebufferId( VK_NULL_HANDLE )
	{
		SetDebugName( "Vk1Framebuffer" );

		_SubscribeOnMsg( this, &Vk1Framebuffer::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_AttachModule_Impl );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_DetachModule_Impl );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_FindModule_Impl );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_Link_Impl );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_Compose );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_Delete );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_DeviceBeforeDestroy );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_GetVkFramebufferID );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_GetFramebufferDescriptor );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_GetVkLogicDevice );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, Platforms::VkThreadModuleID, true );

		_ValidateDescriptor( INOUT _descr );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Vk1Framebuffer::~Vk1Framebuffer ()
	{
		ASSERT( not _IsCreated() );
	}

/*
=================================================
	_Compose
=================================================
*/
	bool Vk1Framebuffer::_Compose (const  Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		_SendForEachAttachments( msg );

		CHECK_COMPOSING( _CreateFramebuffer() );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );

		CHECK( _SetState( EState::ComposedImmutable ) );
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool Vk1Framebuffer::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyFramebuffer();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetVkFramebufferID
=================================================
*/
	bool Vk1Framebuffer::_GetVkFramebufferID (const Message< GpuMsg::GetVkFramebufferID > &msg)
	{
		msg->result.Set( _framebufferId );
		return true;
	}

/*
=================================================
	_GetFramebufferDescriptor
=================================================
*/
	bool Vk1Framebuffer::_GetFramebufferDescriptor (const Message< GpuMsg::GetFramebufferDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_IsCreated
=================================================
*/
	bool Vk1Framebuffer::_IsCreated () const
	{
		return _framebufferId != VK_NULL_HANDLE;
	}
	
/*
=================================================
	AttachmentInfo
=================================================
*/
	struct Vk1Framebuffer::AttachmentInfo : CompileTime::PODStruct
	{
	// variables
		ModuleName_t		name;
		vk::VkImageView		view	= VK_NULL_HANDLE;
		EPixelFormat::type	format	= EPixelFormat::Unknown;
		MultiSamples		samples;

	// methods
		AttachmentInfo ()
		{}

		AttachmentInfo (const ModuleName_t &name, vk::VkImageView view, EPixelFormat::type format, MultiSamples samples) :
			name(name), view(view), format(format), samples(samples)
		{}

		bool IsEnabled ()	const	{ return view == VK_NULL_HANDLE; }
	};
	
/*
=================================================
	_CreateFramebuffer
=================================================
*/
	bool Vk1Framebuffer::_CreateFramebuffer ()
	{
		using namespace vk;

		CHECK_ERR( not _IsCreated() );

		using RenderPassMessages_t		= MessageListFrom< GpuMsg::GetRenderPassDescriptor, GpuMsg::GetVkRenderPassID >;
		using ImageViewMessages_t		= MessageListFrom< GpuMsg::GetImageDescriptor, GpuMsg::GetVkImageView >;
		using Attachments_t				= FixedSizeArray< VkImageView, GlobalConst::Graphics_MaxColorBuffers + 1 >;
		using ColorAttachmentInfos_t	= FixedSizeArray< AttachmentInfo, GlobalConst::Graphics_MaxColorBuffers >;

		RenderPassDescriptor	render_pass_descr;
		VkRenderPass			render_pass					= VK_NULL_HANDLE;
		AttachmentInfo			depth_stencil_attachment;
		ColorAttachmentInfos_t	color_attachments;
		Attachments_t			attachments;

		// search for render pass and images
		FOR( i, _GetAttachments() )
		{
			ModuleName_t const&		name	= _GetAttachments()[i].first;
			ModulePtr const&		mod		= _GetAttachments()[i].second;

			if ( not _IsComposedState( mod->GetState() ) )
				continue;

			// get render pass
			if ( render_pass == VK_NULL_HANDLE and
				 mod->GetSupportedMessages().HasAllTypes< RenderPassMessages_t >() )
			{
				Message< GpuMsg::GetVkRenderPassID >		req_id;
				Message< GpuMsg::GetRenderPassDescriptor >	req_descr;

				SendTo( mod, req_id );
				SendTo( mod, req_descr );

				if ( req_descr->result.IsDefined() and
					 req_id->result.Get( VK_NULL_HANDLE ) != VK_NULL_HANDLE )
				{
					render_pass			<< req_id->result;
					render_pass_descr	<< req_descr->result;
				}
			}

			// add attachment
			if ( mod->GetSupportedMessages().HasAllTypes< ImageViewMessages_t >() )
			{
				Message< GpuMsg::GetImageDescriptor >	req_descr;
				Message< GpuMsg::GetVkImageView >		req_view;

				SendTo( mod, req_descr );
				SendTo( mod, req_view );

				if ( req_descr->result.IsDefined() and
					 req_view->result.IsDefined() )
				{
					auto const&		descr	= req_descr->result.Get();
					VkImageView		view	= req_view->result.Get();

					// set depth stencil attachment
					if ( EPixelFormat::HasDepth( descr.format ) or
						 EPixelFormat::HasStencil( descr.format ) )
					{
						depth_stencil_attachment = AttachmentInfo{ name, view, descr.format, descr.samples };
					}

					// add color attachment
					if ( EPixelFormat::IsColor( descr.format ) )
					{
						color_attachments.PushBack( AttachmentInfo{ name, view, descr.format, descr.samples } );
					}
				}
			}
		}
		
		// check attachments
		if ( render_pass == VK_NULL_HANDLE )
		{
			CHECK_ERR( _CreateRenderPassByAttachment( color_attachments, depth_stencil_attachment, OUT render_pass ) );
		}
		else
			CHECK_ERR( _ValidateAttachment( render_pass_descr, color_attachments, depth_stencil_attachment ) );
		
		// prepare
		if ( depth_stencil_attachment.IsEnabled() )
			attachments.PushBack( depth_stencil_attachment.view );

		FOR( i, color_attachments ) {
			attachments.PushBack( color_attachments[i].view );
		}

		// create framebuffer
		VkFramebufferCreateInfo		fb_info	= {};
		
		fb_info.sType			= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fb_info.renderPass		= render_pass;
		fb_info.attachmentCount	= (uint32_t) attachments.Count();
		fb_info.pAttachments	= attachments.RawPtr();
		fb_info.width			= _descr.size.x;
		fb_info.height			= _descr.size.y;
		fb_info.layers			= _descr.layers;
		
		VK_CHECK( vkCreateFramebuffer( GetLogicalDevice(), &fb_info, null, OUT &_framebufferId ) );

		GetDevice()->SetObjectName( _framebufferId, GetDebugName(), EGpuObject::Framebuffer );
		return true;
	}
	
/*
=================================================
	_CreateRenderPassByAttachment
=================================================
*/
	bool Vk1Framebuffer::_CreateRenderPassByAttachment (ArrayCRef<AttachmentInfo> colorAttachment,
														const AttachmentInfo &depthStencilAttachment,
														OUT vk::VkRenderPass &renderPass)
	{
		RenderPassDescrBuilder	builder;

		FOR( i, colorAttachment )
		{
			builder.AddColorAttachment( colorAttachment[i].name,
										colorAttachment[i].format,
										colorAttachment[i].samples,
										EAttachmentLoadOp::None,
										EAttachmentStoreOp::None,
										EImageLayout::Undefined,
										EImageLayout::ColorAttachmentOptimal );
		}

		ModulePtr	module;

		CHECK_ERR( GlobalSystems()->Get< ModulesFactory >()->Create(
			Platforms::VkRenderPassModuleID,
			GlobalSystems(),
			CreateInfo::GpuRenderPass{ null, builder.Finish() },
			OUT module
		) );

		Message< GpuMsg::GetVkRenderPassID >	req_id;
		SendTo( module, req_id );

		renderPass << req_id->result;
		CHECK_ERR( renderPass != VK_NULL_HANDLE );

		return true;
	}

/*
=================================================
	_ValidateAttachment
=================================================
*/
	bool Vk1Framebuffer::_ValidateAttachment (const RenderPassDescriptor &rpDescr, ArrayCRef<AttachmentInfo> colorAttachment,
											  const AttachmentInfo &depthStencilAttachment)
	{
		// check attachments
		if ( rpDescr.DepthStencilAttachment().IsEnabled() )
		{
			CHECK_ERR( depthStencilAttachment.IsEnabled() );
			CHECK_ERR( depthStencilAttachment.format == rpDescr.DepthStencilAttachment().format );
			CHECK_ERR( depthStencilAttachment.name == rpDescr.DepthStencilAttachment().name );
		}
		else
		{
			// depth attachment disabled in render pass but presented in framebuffer attachment
			CHECK_ERR( depthStencilAttachment.IsEnabled() );
		}

		CHECK_ERR( colorAttachment.Count() == rpDescr.ColorAttachments().Count() );

		FOR( i, rpDescr.ColorAttachments() )
		{
			CHECK_ERR( colorAttachment[i].IsEnabled() );
			CHECK_ERR( colorAttachment[i].format == rpDescr.ColorAttachments()[i].format );
			CHECK_ERR( colorAttachment[i].name == rpDescr.ColorAttachments()[i].name );
		}

		return true;
	}

/*
=================================================
	_DestroyFramebuffer
=================================================
*/
	void Vk1Framebuffer::_DestroyFramebuffer ()
	{
		using namespace vk;

		auto	dev = GetLogicalDevice();

		if ( dev != VK_NULL_HANDLE and _framebufferId != VK_NULL_HANDLE )
		{
			vkDestroyFramebuffer( dev, _framebufferId, null );
		}

		_framebufferId	= VK_NULL_HANDLE;
		_descr			= Uninitialized;
	}
	
/*
=================================================
	_ValidateDescriptor
=================================================
*/
	void Vk1Framebuffer::_ValidateDescriptor (INOUT FramebufferDescriptor &descr)
	{
		CHECK( All( descr.size != uint2(0) ) );

		descr.layers	= Max( descr.layers, 1u );
	}

}	// PlatformVK
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr VulkanContext::_CreateVk1Framebuffer (const GlobalSystemsRef gs, const CreateInfo::GpuFramebuffer &ci)
	{
		return New< PlatformVK::Vk1Framebuffer >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
