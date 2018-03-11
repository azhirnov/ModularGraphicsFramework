// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/RenderPass.h"
#include "Engine/Platforms/Public/GPU/Framebuffer.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/VulkanObjectsConstructor.h"

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
											GpuMsg::GetVkFramebufferID,
											GpuMsg::FramebufferAttachImage
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;

		using RenderPassMsgList_t	= MessageListFrom< GpuMsg::GetRenderPassDescriptor, GpuMsg::GetVkRenderPassID >;
		using ImageMsgList_t		= MessageListFrom< GpuMsg::GetImageDescriptor, GpuMsg::CreateVkImageView >;

		struct AttachmentInfo : CompileTime::PODStruct
		{
		// variables
			ModuleName_t			name;
			vk::VkImageView			view	= VK_NULL_HANDLE;
			ImageViewDescriptor		descr;
			MultiSamples			samples;

		// methods
			AttachmentInfo () {}
		};
	
		using Attachments_t		= FixedSizeArray< AttachmentInfo, GlobalConst::Graphics_MaxColorBuffers >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		vk::VkFramebuffer			_framebufferId;

		FramebufferDescriptor		_descr;

		Attachments_t				_attachments;


	// methods
	public:
		Vk1Framebuffer (GlobalSystemsRef gs, const CreateInfo::GpuFramebuffer &ci);
		~Vk1Framebuffer ();


	// message handlers
	private:
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _AttachModule (const Message< ModuleMsg::AttachModule > &);
		bool _DetachModule (const Message< ModuleMsg::DetachModule > &);
		//bool _ReplaceModule (const Message< ModuleMsg::ReplaceModule > &);
		bool _GetVkFramebufferID (const Message< GpuMsg::GetVkFramebufferID > &);
		bool _GetFramebufferDescriptor (const Message< GpuMsg::GetFramebufferDescriptor > &);
		bool _FramebufferAttachImage (const Message< GpuMsg::FramebufferAttachImage > &);

	private:
		bool _IsCreated () const;
		bool _CreateFramebuffer ();
		void _DestroyFramebuffer ();

		bool _CreateRenderPassByAttachment (OUT RenderPassDescriptor &rpDescr, OUT vk::VkRenderPass &renderPass);
		bool _ValidateAttachment (const RenderPassDescriptor &rpDescr) const;

		static void _ValidateDescriptor (INOUT FramebufferDescriptor &descr);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	Vk1Framebuffer::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	Vk1Framebuffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1Framebuffer::Vk1Framebuffer (GlobalSystemsRef gs, const CreateInfo::GpuFramebuffer &ci) :
		Vk1BaseModule( gs, ModuleConfig{ VkFramebufferModuleID, UMax }, &_msgTypes, &_eventTypes ),
		_framebufferId( VK_NULL_HANDLE ),
		_descr( ci.size, ci.layers )
	{
		SetDebugName( "Vk1Framebuffer" );

		_SubscribeOnMsg( this, &Vk1Framebuffer::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_AttachModule );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_DetachModule );
		//_SubscribeOnMsg( this, &Vk1Framebuffer::_ReplaceModule );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_FindModule_Impl );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_Link_Impl );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_Compose );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_Delete );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_GetVkFramebufferID );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_GetFramebufferDescriptor );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_GetDeviceInfo );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_GetVkDeviceInfo );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_GetVkPrivateClasses );
		_SubscribeOnMsg( this, &Vk1Framebuffer::_FramebufferAttachImage );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );

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
	bool Vk1Framebuffer::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		_SendForEachAttachments( msg );

		CHECK_COMPOSING( _CreateFramebuffer() );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );

		CHECK( _SetState( EState::ComposedMutable ) );
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

		_descr = Uninitialized;
		_attachments.Clear();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_AttachModule
=================================================
*/
	bool Vk1Framebuffer::_AttachModule (const Message< ModuleMsg::AttachModule > &msg)
	{
		CHECK_ERR( msg->newModule );
		
		if ( msg->newModule->GetSupportedMessages().HasAllTypes< ImageMsgList_t >() )
		{
			return _FramebufferAttachImage({ msg->name, msg->newModule });
		}

		// render pass must be unique
		bool	is_render_pass	= msg->newModule->GetSupportedMessages().HasAllTypes< RenderPassMsgList_t >();

		if ( _Attach( msg->name, msg->newModule, is_render_pass ) and is_render_pass )
		{
			CHECK( _SetState( EState::Initial ) );
			_DestroyFramebuffer();
		}
		return true;
	}
	
/*
=================================================
	_ReplaceModule
=================================================
*
	bool Vk1Framebuffer::_ReplaceModule (const Message< ModuleMsg::ReplaceModule > &msg)
	{
		CHECK_ERR( msg->newModule );
		
		ModulePtr	old_mod = msg->oldModule;

		if ( not msg->name.Empty() )
		{
			old_mod = GetModuleByName( msg->name );
			CHECK_ERR( old_mod.IsNull() or msg->oldModule.IsNull() or old_mod == msg->oldModule );
			old_mod = old_mod ? old_mod : msg->oldModule;	// module by name has more priority
		}
		CHECK_ERR( old_mod );

		bool	is_render_pass	= msg->newModule->GetSupportedMessages().HasAllTypes< RenderPassMsgList_t >();
		bool	was_render_pass	= old_mod->GetSupportedMessages().HasAllTypes< RenderPassMsgList_t >();
		bool	is_image		= msg->newModule->GetSupportedMessages().HasAllTypes< ImageMsgList_t >();
		bool	was_image		= old_mod->GetSupportedMessages().HasAllTypes< ImageMsgList_t >();

		CHECK_ERR( is_render_pass == was_render_pass and is_image == was_image );

		if ( is_image )
		{
			Message< GpuMsg::GetImageDescriptor >	req_descr1;
			Message< GpuMsg::GetImageDescriptor >	req_descr2;

			old_mod->Send( req_descr1 );
			msg->newModule->Send( req_descr2 );
		}

		CHECK( _Detach( old_mod ) );
		CHECK( _Attach( msg->name, msg->newModule, is_render_pass ) );
		
		CHECK( _SetState( EState::Initial ) );
		_DestroyFramebuffer();
		return true;
	}

/*
=================================================
	_FramebufferAttachImage
=================================================
*/
	bool Vk1Framebuffer::_FramebufferAttachImage (const Message< GpuMsg::FramebufferAttachImage > &msg)
	{
		ModulePtr	mod = GetModuleByName( msg->name );
		if ( mod ) {
			CHECK( _Detach( mod ) );
		}

		bool			found = false;
		AttachmentInfo	new_att;
		
		new_att.name			= msg->name;
		new_att.descr			= msg->viewDescr;
		new_att.descr.swizzle	= ImageSwizzle();

		CHECK( new_att.descr.swizzle == msg->viewDescr.swizzle );

		FOR( i, _attachments )
		{
			auto&	att = _attachments[i];

			// replace
			if ( att.name == msg->name )
			{
				att		= new_att;
				found	= true;
				break;
			}
		}

		// add new attachment
		if ( not found ) {
			_attachments.PushBack( new_att );
		}

		if ( _Attach( msg->name, msg->image, false ) )
		{
			CHECK( _SetState( EState::Initial ) );
			_DestroyFramebuffer();
		}
		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool Vk1Framebuffer::_DetachModule (const Message< ModuleMsg::DetachModule > &msg)
	{
		CHECK_ERR( msg->oldModule );

		bool	is_render_pass	= msg->oldModule->GetSupportedMessages().HasAllTypes< RenderPassMsgList_t >();
		bool	is_image		= msg->oldModule->GetSupportedMessages().HasAllTypes< ImageMsgList_t >();

		if ( _Detach( msg->oldModule ) and (is_image or is_render_pass) )
		{
			CHECK( _SetState( EState::Initial ) );
			_DestroyFramebuffer();
		}
		return true;
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
	_CreateFramebuffer
=================================================
*/
	bool Vk1Framebuffer::_CreateFramebuffer ()
	{
		using namespace vk;

		CHECK_ERR( not _IsCreated() );
		CHECK_ERR( not _attachments.Empty() );

		using ImageViewMessages_t		= MessageListFrom< GpuMsg::GetImageDescriptor, GpuMsg::CreateVkImageView >;
		using ImageViews_t				= FixedSizeArray< VkImageView, GlobalConst::Graphics_MaxColorBuffers + 1 >;
		using ColorAttachmentInfos_t	= FixedSizeArray< AttachmentInfo, GlobalConst::Graphics_MaxColorBuffers >;

		RenderPassDescriptor	render_pass_descr;
		VkRenderPass			render_pass		= VK_NULL_HANDLE;
		ImageViews_t			img_views;

		// get render pass
		{
			ModulePtr	mod = GetModuleByMsg< RenderPassMsgList_t >();

			if ( mod )
			{
				Message< GpuMsg::GetVkRenderPassID >		req_id;
				Message< GpuMsg::GetRenderPassDescriptor >	req_descr;

				SendTo( mod, req_id );
				SendTo( mod, req_descr );

				CHECK_ERR( req_descr->result.IsDefined() and req_id->result.Get( VK_NULL_HANDLE ) != VK_NULL_HANDLE );

				render_pass			= *req_id->result;
				render_pass_descr	= RVREF(*req_descr->result);
			}
		}

		// get attachments by name
		FOR( i, _attachments )
		{
			auto&		att = _attachments[i];
			ModulePtr	mod;
			CHECK_ERR( mod = GetModuleByName( att.name ) );
			CHECK_ERR( _IsComposedState( mod->GetState() ) );
			
			Message< GpuMsg::GetImageDescriptor >	req_descr;
			Message< GpuMsg::CreateVkImageView >	req_view;

			req_view->viewDescr = att.descr;

			SendTo( mod, req_descr );
			SendTo( mod, req_view );

			CHECK_ERR( req_descr->result.IsDefined() and req_view->result.IsDefined() );
			
			att.descr.format	= att.descr.format == EPixelFormat::Unknown ? req_descr->result->format : att.descr.format;
			att.descr.viewType	= att.descr.viewType == EImage::Unknown ? req_descr->result->imageType : att.descr.viewType;
			att.samples			= req_descr->result->samples;
			att.view			= *req_view->result;
			
			const uint4	dim	= Max( ImageUtils::LevelDimension( att.descr.viewType, req_descr->result->dimension, att.descr.baseLevel.Get() ), 1u );

			// validate
			CHECK_ERR( All( dim.xy() >= _descr.size ) );

			if ( _descr.layers > 1 )
			{
				CHECK_ERR( EImage::IsArray( att.descr.viewType ) );
				CHECK_ERR( dim.w <= att.descr.baseLayer.Get() + _descr.layers );
			}
			else
				att.descr.baseLayer = Uninitialized;
		}

		
		// check attachments
		if ( render_pass == VK_NULL_HANDLE )
		{
			CHECK_ERR( _CreateRenderPassByAttachment( OUT render_pass_descr, OUT render_pass ) );
		}
		else
			CHECK_ERR( _ValidateAttachment( render_pass_descr ) );
		
		// prepare
		auto	FindAttachmentIndex =	LAMBDA( &render_pass_descr ) (StringCRef name) -> usize
										{{
											if ( name == render_pass_descr.DepthStencilAttachment().name )
												return render_pass_descr.ColorAttachments().Count();

											FOR( i, render_pass_descr.ColorAttachments() ) {
												if ( name == render_pass_descr.ColorAttachments()[i].name )
													return i;
											}
											RETURN_ERR( "Attachment '" << name << "' not found", UMax );
										}};

		img_views.Resize( render_pass_descr.ColorAttachments().Count() + uint(render_pass_descr.DepthStencilAttachment().IsEnabled()) );
		_descr.colorAttachments.Resize( render_pass_descr.ColorAttachments().Count() );
		_descr.samples = _attachments.Front().samples;	// TODO

		FOR( i, _attachments )
		{
			const auto&	att			= _attachments[i];
			usize		idx			= FindAttachmentIndex( att.name );
			bool		is_depth	= att.name == render_pass_descr.DepthStencilAttachment().name;

			img_views[idx] = att.view;

			if ( is_depth )
				_descr.depthStencilAttachment = FramebufferDescriptor::AttachmentInfo{ att.name, att.descr.viewType };
			else
				_descr.colorAttachments[idx] = FramebufferDescriptor::AttachmentInfo{ att.name, att.descr.viewType };
		}

		// create framebuffer
		VkFramebufferCreateInfo		fb_info	= {};
		
		fb_info.sType			= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fb_info.renderPass		= render_pass;
		fb_info.attachmentCount	= (uint32_t) img_views.Count();
		fb_info.pAttachments	= img_views.RawPtr();
		fb_info.width			= _descr.size.x;
		fb_info.height			= _descr.size.y;
		fb_info.layers			= _descr.layers;
		
		VK_CHECK( vkCreateFramebuffer( GetVkDevice(), &fb_info, null, OUT &_framebufferId ) );

		GetDevice()->SetObjectName( ReferenceCast<uint64_t>(_framebufferId), GetDebugName(), EGpuObject::Framebuffer );
		return true;
	}
	
/*
=================================================
	_CreateRenderPassByAttachment
=================================================
*/
	bool Vk1Framebuffer::_CreateRenderPassByAttachment (OUT RenderPassDescriptor &rpDescr, OUT vk::VkRenderPass &renderPass)
	{
		auto	builder = RenderPassDescrBuilder::CreateForFramebuffer();

		FOR( i, _attachments )
		{
			const auto&	att = _attachments[i];

			ASSERT( att.view != VK_NULL_HANDLE );
			builder.Add( att.name, att.descr.format, att.samples );
		}

		ModulePtr	render_pass;
		CHECK_ERR( GlobalSystems()->modulesFactory->Create(
							VkRenderPassModuleID,
							GlobalSystems(),
							CreateInfo::GpuRenderPass{ null, builder.Finish() },
							OUT render_pass
						));

		ModuleUtils::Initialize({ render_pass });

		CHECK_ERR( _Attach( "renderpass", render_pass, true ) );

		Message< GpuMsg::GetVkRenderPassID >		req_id;
		Message< GpuMsg::GetRenderPassDescriptor >	req_descr;

		SendTo( render_pass, req_id );
		SendTo( render_pass, req_descr );

		renderPass	= *req_id->result;
		rpDescr		= RVREF(*req_descr->result);

		CHECK_ERR( renderPass != VK_NULL_HANDLE );
		return true;
	}

/*
=================================================
	_ValidateAttachment
=================================================
*/
	bool Vk1Framebuffer::_ValidateAttachment (const RenderPassDescriptor &rpDescr) const
	{
		CHECK_ERR( _attachments.Count() == rpDescr.ColorAttachments().Count() + uint(rpDescr.DepthStencilAttachment().IsEnabled()) );

		FOR( i, _attachments )
		{
			const auto&	att = _attachments[i];

			// check in depth stencil attachment
			if ( att.name == rpDescr.DepthStencilAttachment().name )
			{
				CHECK_ERR( att.descr.format == rpDescr.DepthStencilAttachment().format );
				CHECK_ERR( att.samples == rpDescr.DepthStencilAttachment().samples );
				continue;
			}

			// check in color attachments
			bool	found = false;

			FOR( j, rpDescr.ColorAttachments() )
			{
				const auto&	col = rpDescr.ColorAttachments()[j];

				if ( col.name == att.name )
				{
					CHECK_ERR( col.format == att.descr.format );
					CHECK_ERR( col.samples == att.samples );
					found = true;
					break;
				}
			}
			if ( found )
				continue;

			RETURN_ERR( "Attachment '" << att.name << "' not presented in render pass" );
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

		auto	dev = GetVkDevice();

		if ( dev != VK_NULL_HANDLE and _framebufferId != VK_NULL_HANDLE )
		{
			vkDestroyFramebuffer( dev, _framebufferId, null );
		}
		
		_framebufferId	= VK_NULL_HANDLE;
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
	ModulePtr VulkanObjectsConstructor::CreateVk1Framebuffer (GlobalSystemsRef gs, const CreateInfo::GpuFramebuffer &ci)
	{
		return New< PlatformVK::Vk1Framebuffer >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
