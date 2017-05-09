// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/Impl/Vk1RenderPass.h"
#include "Engine/Platforms/Vulkan/VulkanThread.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	
/*
=================================================
	constructor
=================================================
*/
	Vk1RenderPassCache::Vk1RenderPassCache (VkSystemsRef vkSys) :
		_vkSystems( vkSys )
	{
		_renderPasses.Reserve( 64 );
	}
	
/*
=================================================
	Create
=================================================
*/
	Vk1RenderPassCache::Vk1RenderPassPtr  Vk1RenderPassCache::Create (const GlobalSystemsRef gs, const CreateInfo::GpuRenderPass &ci)
	{
		// TODO: validate
		
		// find cached render pass
		RenderPasses_t::const_iterator	iter;

		if ( _renderPasses.CustomSearch().Find( RenderPassSearch( ci.descr ), OUT iter ) and
			 iter->rp->GetState() == Module::EState::ComposedImmutable )
		{
			return iter->rp;
		}
		
		// create new render pass
		auto	result = GXTypes::New< Vk1RenderPass >( gs, ci );
		
		result->Send( Message< ModuleMsg::Link >() );
		result->Send( Message< ModuleMsg::Compose >() );
		
		CHECK_ERR( result->GetState() == Module::EState::ComposedImmutable );

		_renderPasses.Add( SearchableRenderPass( result ) );
		return result;
	}
	
//-----------------------------------------------------------------------------


	
	const Runtime::VirtualTypeList	Vk1RenderPass::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	Vk1RenderPass::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1RenderPass::Vk1RenderPass (const GlobalSystemsRef gs, const CreateInfo::GpuRenderPass &ci) :
		Vk1BaseModule( gs, ci.gpuThread, ModuleConfig{ GetStaticID(), ~0u }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),
		_renderPassId( VK_NULL_HANDLE )
	{
		SetDebugName( "Vk1RenderPass" );

		_SubscribeOnMsg( this, &Vk1RenderPass::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1RenderPass::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1RenderPass::_AttachModule_Impl );
		_SubscribeOnMsg( this, &Vk1RenderPass::_DetachModule_Impl );
		_SubscribeOnMsg( this, &Vk1RenderPass::_FindModule_Impl );
		_SubscribeOnMsg( this, &Vk1RenderPass::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Vk1RenderPass::_Link_Impl );
		_SubscribeOnMsg( this, &Vk1RenderPass::_Compose );
		_SubscribeOnMsg( this, &Vk1RenderPass::_Delete );
		_SubscribeOnMsg( this, &Vk1RenderPass::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1RenderPass::_GpuDeviceBeforeDestory );
		_SubscribeOnMsg( this, &Vk1RenderPass::_GetGpuRenderPassID );
		_SubscribeOnMsg( this, &Vk1RenderPass::_GetGpuRenderPassDescriptor );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, VulkanThread::GetStaticID(), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Vk1RenderPass::~Vk1RenderPass ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool Vk1RenderPass::_Compose (const  Message< ModuleMsg::Compose > &msg)
	{
		CHECK_ERR( GetState() == EState::Linked );

		CHECK_ERR( _CreateRenderPass() );

		_SendForEachAttachments( Message< ModuleMsg::Compose >{ this } );
		
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
	bool Vk1RenderPass::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyRenderPass();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetGpuRenderPassID
=================================================
*/
	bool Vk1RenderPass::_GetGpuRenderPassID (const Message< ModuleMsg::GetGpuRenderPassID > &msg)
	{
		msg->result.Set( _renderPassId );
		return true;
	}

/*
=================================================
	_GetGpuRenderPassDescriptor
=================================================
*/
	bool Vk1RenderPass::_GetGpuRenderPassDescriptor (const Message< ModuleMsg::GetGpuRenderPassDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_IsCreated
=================================================
*/
	bool Vk1RenderPass::_IsCreated () const
	{
		return _renderPassId != VK_NULL_HANDLE;
	}
	
/*
=================================================
	_CreateRenderPass
=================================================
*/
	bool Vk1RenderPass::_CreateRenderPass ()
	{
		using namespace vk;
		
		CHECK_ERR( not _IsCreated() );

		using Attachments		= FixedSizeArray< VkAttachmentDescription, RenderPassDescriptor::MAX_COLOR_ATTACHMENTS+1 >;
		using AttachmentsRef	= FixedSizeArray< VkAttachmentReference, RenderPassDescriptor::MAX_COLOR_ATTACHMENTS * RenderPassDescriptor::MAX_SUBPASSES >;
		using AttachmentsRef2	= FixedSizeArray< VkAttachmentReference, RenderPassDescriptor::MAX_SUBPASSES >;
		using Subpasses			= FixedSizeArray< VkSubpassDescription, RenderPassDescriptor::MAX_SUBPASSES >;
		using Dependencies		= FixedSizeArray< VkSubpassDependency, RenderPassDescriptor::MAX_DEPENDENCIES >;

		Attachments				attachments;
		AttachmentsRef			color_attach_ref;
		AttachmentsRef			input_attach_ref;
		AttachmentsRef2			depth_stencil_attach_ref;
		AttachmentsRef2			resolve_attach_ref;
		Subpasses				subpasses;
		Dependencies			dependencies;

		if ( _descr.DepthStencilAttachment().IsEnabled() )
		{
			auto const&	attach = _descr.DepthStencilAttachment();

			VkAttachmentDescription	descr = {};
			descr.format			= Vk1Enum( attach.format );
			descr.samples			= Vk1Enum( attach.samples );
			descr.loadOp			= Vk1Enum( attach.loadOp );
			descr.storeOp			= Vk1Enum( attach.storeOp );
			descr.stencilLoadOp		= Vk1Enum( attach.stencilLoadOp );
			descr.stencilStoreOp	= Vk1Enum( attach.stencilStoreOp );
			descr.initialLayout		= Vk1Enum( attach.initialLayout );
			descr.finalLayout		= Vk1Enum( attach.finalLayout );

			attachments.PushBack( descr );
		}

		FOR( i, _descr.ColorAttachments() )
		{
			auto const&	attach = _descr.ColorAttachments()[i];

			VkAttachmentDescription	descr = {};
			descr.format			= Vk1Enum( attach.format );
			descr.samples			= Vk1Enum( attach.samples );
			descr.loadOp			= Vk1Enum( attach.loadOp );
			descr.storeOp			= Vk1Enum( attach.storeOp );
			descr.stencilLoadOp		= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			descr.stencilStoreOp	= VK_ATTACHMENT_STORE_OP_DONT_CARE;
			descr.initialLayout		= Vk1Enum( attach.initialLayout );
			descr.finalLayout		= Vk1Enum( attach.finalLayout );

			attachments.PushBack( descr );
		}

		FOR( i, _descr.Subpasses() )
		{
			auto const&		subpass = _descr.Subpasses()[i];

			VkSubpassDescription	descr = {};
			descr.flags						= 0;	// TODO
			descr.pipelineBindPoint			= VK_PIPELINE_BIND_POINT_GRAPHICS;

			descr.colorAttachmentCount		= subpass.colors.Count();
			descr.pColorAttachments			= subpass.colors.Empty() ? null : color_attach_ref.ptr() + color_attach_ref.Count();

			descr.inputAttachmentCount		= subpass.inputs.Count();
			descr.pInputAttachments			= subpass.inputs.Empty() ? null : input_attach_ref.ptr() + input_attach_ref.Count();

			descr.preserveAttachmentCount	= subpass.preserves.Count();
			descr.pPreserveAttachments		= subpass.preserves.Empty() ? null : PointerSafeCast<vk::uint32_t>( subpass.preserves.ptr() );

			descr.pResolveAttachments		= subpass.resolve.IsEnabled() ?
												resolve_attach_ref.ptr() + resolve_attach_ref.Count() :
												null;
			descr.pDepthStencilAttachment	= subpass.depthStencil.IsEnabled() ?
												depth_stencil_attach_ref.ptr() + depth_stencil_attach_ref.Count() :
												null;

			FOR( j, subpass.colors )
			{
				VkAttachmentReference	att_desc = {};
				att_desc.attachment		= subpass.colors[j].index;
				att_desc.layout			= Vk1Enum( subpass.colors[j].layout );
				color_attach_ref.PushBack( att_desc );
			}
			
			FOR( j, subpass.inputs )
			{
				VkAttachmentReference	att_desc = {};
				att_desc.attachment		= subpass.inputs[j].index;
				att_desc.layout			= Vk1Enum( subpass.inputs[j].layout );
				input_attach_ref.PushBack( att_desc );
			}
			
			if ( subpass.depthStencil.IsEnabled() )
			{
				VkAttachmentReference	att_desc = {};
				att_desc.attachment		= subpass.depthStencil.index;
				att_desc.layout			= Vk1Enum( subpass.depthStencil.layout );
				depth_stencil_attach_ref.PushBack( att_desc );
			}
			
			if ( subpass.resolve.IsEnabled() )
			{
				VkAttachmentReference	att_desc = {};
				att_desc.attachment		= subpass.resolve.index;
				att_desc.layout			= Vk1Enum( subpass.resolve.layout );
				resolve_attach_ref.PushBack( att_desc );
			}

			subpasses.PushBack( descr );
		}

		FOR( i, _descr.Dependencies() )
		{
			auto const&		dep = _descr.Dependencies()[i];

			VkSubpassDependency	descr = {};
			descr.dependencyFlags	= Vk1Enum( dep.dependency );
			descr.srcSubpass		= dep.srcPass == RenderPassDescriptor::SubpassIndexExternal ? VK_SUBPASS_EXTERNAL : uint(dep.srcPass);
			descr.srcStageMask		= Vk1Enum( dep.srcStage );
			descr.srcAccessMask		= Vk1Enum( dep.srcAccess );
			descr.dstSubpass		= dep.dstPass == RenderPassDescriptor::SubpassIndexExternal ? VK_SUBPASS_EXTERNAL : uint(dep.dstPass);
			descr.dstStageMask		= Vk1Enum( dep.dstStage );
			descr.dstAccessMask		= Vk1Enum( dep.dstAccess );

			dependencies.PushBack( descr );
		}

		VkRenderPassCreateInfo	info = {};
		info.sType				= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		info.attachmentCount	= attachments.Count();
		info.pAttachments		= attachments.RawPtr();
		info.subpassCount		= subpasses.Count();
		info.pSubpasses			= subpasses.RawPtr();
		info.dependencyCount	= dependencies.Count();
		info.pDependencies		= dependencies.RawPtr();
		
		VK_CHECK( vkCreateRenderPass( GetLogicalDevice(), &info, null, OUT &_renderPassId ) );
		return true;
	}
	
/*
=================================================
	_DestroyRenderPass
=================================================
*/
	void Vk1RenderPass::_DestroyRenderPass ()
	{
		using namespace vk;

		auto	dev = GetLogicalDevice();

		if ( dev != VK_NULL_HANDLE and _renderPassId != VK_NULL_HANDLE )
		{
			vkDestroyRenderPass( dev, _renderPassId, null );
		}

		_renderPassId	= VK_NULL_HANDLE;
		_descr			= Uninitialized;
	}


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
