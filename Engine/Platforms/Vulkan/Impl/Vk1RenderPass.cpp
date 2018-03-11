// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Vulkan/Impl/Vk1RenderPass.h"
#include "Engine/Platforms/Vulkan/VulkanObjectsConstructor.h"

namespace Engine
{
namespace PlatformVK
{

	
	//
	// Vulkan Render Pass
	//

	class Vk1RenderPass final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetRenderPassDescriptor,
											GpuMsg::GetVkRenderPassID
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		RenderPassDescriptor	_descr;

		vk::VkRenderPass		_renderPassId;


	// methods
	public:
		Vk1RenderPass (GlobalSystemsRef gs, const CreateInfo::GpuRenderPass &ci);
		~Vk1RenderPass ();

		RenderPassDescriptor const&	GetDescriptor ()	const	{ return _descr; }


	// message handlers
	private:
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetVkRenderPassID (const Message< GpuMsg::GetVkRenderPassID > &);
		bool _GetRenderPassDescriptor (const Message< GpuMsg::GetRenderPassDescriptor > &);

	private:
		bool _IsCreated () const;
		bool _CreateRenderPass ();
		void _DestroyRenderPass ();
	};
//-----------------------------------------------------------------------------



	const TypeIdList	Vk1RenderPass::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	Vk1RenderPass::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1RenderPass::Vk1RenderPass (GlobalSystemsRef gs, const CreateInfo::GpuRenderPass &ci) :
		Vk1BaseModule( gs, ModuleConfig{ VkRenderPassModuleID, UMax }, &_msgTypes, &_eventTypes ),
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
		_SubscribeOnMsg( this, &Vk1RenderPass::_GetVkRenderPassID );
		_SubscribeOnMsg( this, &Vk1RenderPass::_GetRenderPassDescriptor );
		_SubscribeOnMsg( this, &Vk1RenderPass::_GetDeviceInfo );
		_SubscribeOnMsg( this, &Vk1RenderPass::_GetVkDeviceInfo );
		_SubscribeOnMsg( this, &Vk1RenderPass::_GetVkPrivateClasses );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
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
	bool Vk1RenderPass::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreateRenderPass() );

		_SendForEachAttachments( msg );
		
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
	_GetVkRenderPassID
=================================================
*/
	bool Vk1RenderPass::_GetVkRenderPassID (const Message< GpuMsg::GetVkRenderPassID > &msg)
	{
		msg->result.Set( _renderPassId );
		return true;
	}

/*
=================================================
	_GetRenderPassDescriptor
=================================================
*/
	bool Vk1RenderPass::_GetRenderPassDescriptor (const Message< GpuMsg::GetRenderPassDescriptor > &msg)
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
		using Preserves			= FixedSizeArray< uint32_t, RenderPassDescriptor::MAX_COLOR_ATTACHMENTS * RenderPassDescriptor::MAX_SUBPASSES >;

		Attachments				attachments;
		AttachmentsRef			color_attach_ref;
		AttachmentsRef			input_attach_ref;
		AttachmentsRef2			depth_stencil_attach_ref;
		AttachmentsRef2			resolve_attach_ref;
		Subpasses				subpasses;
		Dependencies			dependencies;
		Preserves				preserves;

		static const char		external_pass[] = "external";

		const auto	FindAttachmentIndex =LAMBDA( this ) (StringCRef name) -> uint32_t
										{{
											if ( _descr.DepthStencilAttachment().name == name )
												return uint32_t(_descr.ColorAttachments().Count());

											FOR( i, _descr.ColorAttachments() ) {
												if ( _descr.ColorAttachments()[i].name == name )
													return uint32_t(i);
											}
											RETURN_ERR( "Attachement '" << name << "' not found", UMax );	// return any invalid value
										}};

		const auto	FindSubpassIndex =	LAMBDA( this ) (StringCRef name) -> uint32_t
										{{
											if ( name.EqualsIC( external_pass ) )
												return VK_SUBPASS_EXTERNAL;

											FOR( i, _descr.Subpasses() ) {
												if ( _descr.Subpasses()[i].name == name )
													return uint32_t(i);
											}
											RETURN_ERR( "Subpass '" << name << "' not found", VK_SUBPASS_EXTERNAL-1 ); // return any invalid value
										}};

		attachments.Resize( _descr.ColorAttachments().Count() + usize(_descr.DepthStencilAttachment().IsEnabled()) );

		FOR( i, _descr.ColorAttachments() )
		{
			const auto&		attach = _descr.ColorAttachments()[i];

			VkAttachmentDescription&	descr = attachments[i];
			descr.format			= Vk1Enum( attach.format );
			descr.samples			= Vk1Enum( attach.samples );
			descr.loadOp			= Vk1Enum( attach.loadOp );
			descr.storeOp			= Vk1Enum( attach.storeOp );
			descr.stencilLoadOp		= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			descr.stencilStoreOp	= VK_ATTACHMENT_STORE_OP_DONT_CARE;
			descr.initialLayout		= Vk1Enum( attach.initialLayout );
			descr.finalLayout		= Vk1Enum( attach.finalLayout );
		}
		
		if ( _descr.DepthStencilAttachment().IsEnabled() )
		{
			const auto&		attach = _descr.DepthStencilAttachment();

			VkAttachmentDescription&	descr = attachments.Back();
			descr.format			= Vk1Enum( attach.format );
			descr.samples			= Vk1Enum( attach.samples );
			descr.loadOp			= Vk1Enum( attach.loadOp );
			descr.storeOp			= Vk1Enum( attach.storeOp );
			descr.stencilLoadOp		= Vk1Enum( attach.stencilLoadOp );
			descr.stencilStoreOp	= Vk1Enum( attach.stencilStoreOp );
			descr.initialLayout		= Vk1Enum( attach.initialLayout );
			descr.finalLayout		= Vk1Enum( attach.finalLayout );
		}

		FOR( i, _descr.Subpasses() )
		{
			const auto&		subpass = _descr.Subpasses()[i];

			VkSubpassDescription	descr = {};
			descr.flags						= 0;	// no flags, only vendor specific flags exists
			descr.pipelineBindPoint			= VK_PIPELINE_BIND_POINT_GRAPHICS;	// only graphics subpasses are supported (vulkan spec 1.0)

			descr.colorAttachmentCount		= (uint32_t) subpass.colors.Count();
			descr.pColorAttachments			= subpass.colors.Empty() ? null : color_attach_ref.RawPtr() + color_attach_ref.Count();

			descr.inputAttachmentCount		= (uint32_t) subpass.inputs.Count();
			descr.pInputAttachments			= subpass.inputs.Empty() ? null : input_attach_ref.RawPtr() + input_attach_ref.Count();

			descr.preserveAttachmentCount	= (uint32_t) subpass.preserves.Count();
			descr.pPreserveAttachments		= preserves.Empty() ? null : preserves.RawPtr() + preserves.Count();

			descr.pResolveAttachments		= subpass.resolve.IsEnabled() ?
												resolve_attach_ref.RawPtr() + resolve_attach_ref.Count() :
												null;
			descr.pDepthStencilAttachment	= subpass.depthStencil.IsEnabled() ?
												depth_stencil_attach_ref.RawPtr() + depth_stencil_attach_ref.Count() :
												null;

			FOR( j, subpass.colors )
			{
				VkAttachmentReference	att_desc = {};
				att_desc.attachment		= FindAttachmentIndex( subpass.colors[j].name );
				att_desc.layout			= Vk1Enum( subpass.colors[j].layout );
				color_attach_ref.PushBack( att_desc );
			}
			
			FOR( j, subpass.inputs )
			{
				VkAttachmentReference	att_desc = {};
				att_desc.attachment		= FindAttachmentIndex( subpass.inputs[j].name );
				att_desc.layout			= Vk1Enum( subpass.inputs[j].layout );
				input_attach_ref.PushBack( att_desc );
			}

			FOR( j, subpass.preserves )
			{
				preserves << FindAttachmentIndex( subpass.preserves[j] );
			}
			
			if ( subpass.depthStencil.IsEnabled() )
			{
				VkAttachmentReference	att_desc = {};
				att_desc.attachment		= FindAttachmentIndex( subpass.depthStencil.name );
				att_desc.layout			= Vk1Enum( subpass.depthStencil.layout );
				depth_stencil_attach_ref.PushBack( att_desc );
			}
			
			if ( subpass.resolve.IsEnabled() )
			{
				VkAttachmentReference	att_desc = {};
				att_desc.attachment		= FindAttachmentIndex( subpass.resolve.name );
				att_desc.layout			= Vk1Enum( subpass.resolve.layout );
				resolve_attach_ref.PushBack( att_desc );
			}

			subpasses.PushBack( descr );
		}

		FOR( i, _descr.Dependencies() )
		{
			const auto&		dep = _descr.Dependencies()[i];

			VkSubpassDependency	descr = {};
			descr.dependencyFlags	= Vk1Enum( dep.dependency );
			descr.srcSubpass		= FindSubpassIndex( dep.srcPass );
			descr.srcStageMask		= Vk1Enum( dep.srcStage );
			descr.srcAccessMask		= Vk1Enum( dep.srcAccess );
			descr.dstSubpass		= FindSubpassIndex( dep.dstPass );
			descr.dstStageMask		= Vk1Enum( dep.dstStage );
			descr.dstAccessMask		= Vk1Enum( dep.dstAccess );

			dependencies.PushBack( descr );
		}

		VkRenderPassCreateInfo	info = {};
		info.sType				= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		info.attachmentCount	= (uint32_t) attachments.Count();
		info.pAttachments		= attachments.RawPtr();
		info.subpassCount		= (uint32_t) subpasses.Count();
		info.pSubpasses			= subpasses.RawPtr();
		info.dependencyCount	= (uint32_t) dependencies.Count();
		info.pDependencies		= dependencies.RawPtr();
		
		VK_CHECK( vkCreateRenderPass( GetVkDevice(), &info, null, OUT &_renderPassId ) );

		GetDevice()->SetObjectName( ReferenceCast<uint64_t>(_renderPassId), GetDebugName(), EGpuObject::RenderPass );
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

		auto	dev = GetVkDevice();

		if ( dev != VK_NULL_HANDLE and _renderPassId != VK_NULL_HANDLE )
		{
			vkDestroyRenderPass( dev, _renderPassId, null );
		}

		_renderPassId	= VK_NULL_HANDLE;
		_descr			= Uninitialized;
	}
//-----------------------------------------------------------------------------

	
/*
=================================================
	constructor
=================================================
*/
	inline bool Vk1RenderPassCache::SearchableRenderPass::operator == (const SearchableRenderPass &right) const	{ return rp->GetDescriptor() == right.rp->GetDescriptor(); }
	inline bool Vk1RenderPassCache::SearchableRenderPass::operator >  (const SearchableRenderPass &right) const	{ return rp->GetDescriptor() >  right.rp->GetDescriptor(); }
	inline bool Vk1RenderPassCache::SearchableRenderPass::operator <  (const SearchableRenderPass &right) const	{ return rp->GetDescriptor() <  right.rp->GetDescriptor(); }
//-----------------------------------------------------------------------------
	

/*
=================================================
	constructor
=================================================
*/		
	inline bool Vk1RenderPassCache::RenderPassSearch::operator == (const SearchableRenderPass &right) const	{ return descr == right.rp->GetDescriptor(); }
	inline bool Vk1RenderPassCache::RenderPassSearch::operator >  (const SearchableRenderPass &right) const	{ return descr >  right.rp->GetDescriptor(); }
	inline bool Vk1RenderPassCache::RenderPassSearch::operator <  (const SearchableRenderPass &right) const	{ return descr <  right.rp->GetDescriptor(); }
//-----------------------------------------------------------------------------

	
/*
=================================================
	constructor
=================================================
*/
	Vk1RenderPassCache::Vk1RenderPassCache (Ptr<Vk1Device> dev) :
		Vk1BaseObject( dev )
	{
		_renderPasses.Reserve( 64 );
	}
	
/*
=================================================
	Create
=================================================
*/
	Vk1RenderPassCache::Vk1RenderPassPtr  Vk1RenderPassCache::Create (GlobalSystemsRef gs, const CreateInfo::GpuRenderPass &ci)
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
		auto	result = New< Vk1RenderPass >( gs, ci );

		ModuleUtils::Initialize( {result}, null );

		CHECK_ERR( result->GetState() == Module::EState::ComposedImmutable );

		_renderPasses.Add( SearchableRenderPass( result ) );
		return result;
	}
	
/*
=================================================
	Destroy
=================================================
*/
	void Vk1RenderPassCache::Destroy ()
	{
		Message< ModuleMsg::Delete >	del_msg;

		FOR( i, _renderPasses ) {
			_renderPasses[i].rp->Send( del_msg );
		}

		_renderPasses.Clear();
	}

}	// PlatformVK
//-----------------------------------------------------------------------------

namespace Platforms
{

	ModulePtr VulkanObjectsConstructor::CreateVk1RenderPass (GlobalSystemsRef gs, const CreateInfo::GpuRenderPass &ci)
	{
		ModulePtr	mod;
		CHECK_ERR( mod = gs->parallelThread->GetModuleByMsg< CompileTime::TypeListFrom<Message<GpuMsg::GetVkPrivateClasses>> >() );

		Message< GpuMsg::GetVkPrivateClasses >	req_cl;
		mod->Send( req_cl );
		CHECK_ERR( req_cl->result.IsDefined() and req_cl->result->renderPassCache );

		return req_cl->result->renderPassCache->Create( gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
