// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/Impl/Vk1Pipeline.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1PipelineCache.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1PipelineLayout.h"
#include "Engine/Platforms/Vulkan/VulkanContext.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	
	const TypeIdList	Vk1GraphicsPipeline::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	Vk1GraphicsPipeline::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1GraphicsPipeline::Vk1GraphicsPipeline (GlobalSystemsRef gs, const CreateInfo::GraphicsPipeline &ci) :
		Vk1BaseModule( gs, ModuleConfig{ VkGraphicsPipelineModuleID, ~0u }, &_msgTypes, &_eventTypes ),
		_pipelineId( VK_NULL_HANDLE ),
		_descr( ci.descr ),
		_shaders( ci.shaders ),
		_renderPass( ci.renderPass )
	{
		SetDebugName( "Vk1GraphicsPipeline" );

		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_AttachModule_Impl );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_DetachModule_Impl );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_FindModule_Impl );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_Link_Impl );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_Compose );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_Delete );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_DeviceBeforeDestroy );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_GetVkGraphicsPipelineID );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_GetGraphicsPipelineDescriptor );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_GetDeviceInfo );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_GetVkDeviceInfo );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_GetVkPrivateClasses );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_GetPipelineLayoutDescriptor );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_GetVkDescriptorLayouts );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_GetVkPipelineLayoutID );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, VkThreadModuleID, true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Vk1GraphicsPipeline::~Vk1GraphicsPipeline ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool Vk1GraphicsPipeline::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreatePipeline() );

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
	bool Vk1GraphicsPipeline::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyPipeline();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetVkGraphicsPipelineID
=================================================
*/
	bool Vk1GraphicsPipeline::_GetVkGraphicsPipelineID (const Message< GpuMsg::GetVkGraphicsPipelineID > &msg)
	{
		msg->result.Set( _pipelineId );
		return true;
	}
	
/*
=================================================
	_GetGraphicsPipelineDescriptor
=================================================
*/
	bool Vk1GraphicsPipeline::_GetGraphicsPipelineDescriptor (const Message< GpuMsg::GetGraphicsPipelineDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_GetPipelineLayoutDescriptor
=================================================
*/
	bool Vk1GraphicsPipeline::_GetPipelineLayoutDescriptor (const Message< GpuMsg::GetPipelineLayoutDescriptor > &msg)
	{
		if ( _layout )
			msg->result.Set( _layout->GetDescriptor() );
		return true;
	}
	
/*
=================================================
	_GetVkDescriptorLayouts
=================================================
*/
	bool Vk1GraphicsPipeline::_GetVkDescriptorLayouts (const Message< GpuMsg::GetVkDescriptorLayouts > &msg)
	{
		if ( _layout )
			msg->result.Set( _layout->GetDescriptorLayouts() );
		return true;
	}
		
/*
=================================================
	_GetVkPipelineLayoutID
=================================================
*/
	bool Vk1GraphicsPipeline::_GetVkPipelineLayoutID (const Message< GpuMsg::GetVkPipelineLayoutID > &msg)
	{
		if ( _layout )
			msg->result.Set( _layout->GetLayoutID() );
		return true;
	}

/*
=================================================
	_IsCreated
=================================================
*/
	bool Vk1GraphicsPipeline::_IsCreated () const
	{
		return _pipelineId != VK_NULL_HANDLE;
	}
	
/*
=================================================
	_CreatePipeline
=================================================
*/
	bool Vk1GraphicsPipeline::_CreatePipeline ()
	{
		using namespace vk;

		CHECK_ERR( not _IsCreated() );
		CHECK_ERR( _GetManager() );

		// validate render pass
		if ( not _renderPass )
		{
			_renderPass		= GetDevice()->GetDefaultRenderPass();
			_descr.subpass	= 0;

			LOG( "used default render pass", ELog::Debug );
		}
		CHECK_ERR( _ValidateRenderPass() );

		
		// get render pass id
		Message< GpuMsg::GetVkRenderPassID >		req_pass_id;
		Message< GpuMsg::GetRenderPassDescriptor >	req_pass_descr;

		SendTo( _renderPass, req_pass_id );
		SendTo( _renderPass, req_pass_descr );

		RenderPassDescriptor	rp_descr;	rp_descr << req_pass_descr->result;
		VkRenderPass			rp			= req_pass_id->result.Get( VK_NULL_HANDLE );
		CHECK_ERR( rp != VK_NULL_HANDLE );
		

		// get shader modules
		Message< GpuMsg::GetVkShaderModuleIDs >		req_shader_ids;
		SendTo( _shaders, req_shader_ids );
		CHECK_ERR( req_shader_ids->result and not req_shader_ids->result->Empty() );
		
		// create layout
		Message< GpuMsg::GetVkPrivateClasses >	req_cache;
		_SendMsg( req_cache );

		CHECK_ERR(( _layout = req_cache->result->layoutCache->Create( _descr.layout ) ));

		// create graphics pipeline
		CHECK_ERR( req_cache->result->pipelineCache->
						CreatePipeline( OUT _pipelineId,
										*req_shader_ids->result,
										_layout->GetLayoutID(),
										_descr.vertexInput,
										_descr.renderState,
										_descr.dynamicStates,
										_descr.patchControlPoints,
										rp_descr,
										rp,
										_descr.subpass )
		);
		
		return true;
	}
	
/*
=================================================
	_DestroyPipeline
=================================================
*/
	void Vk1GraphicsPipeline::_DestroyPipeline ()
	{
		using namespace vk;

		auto	dev = GetVkDevice();

		if ( dev != VK_NULL_HANDLE and _pipelineId != VK_NULL_HANDLE )
		{
			vkDestroyPipeline( dev, _pipelineId, null );
		}

		_pipelineId	= VK_NULL_HANDLE;
		_descr		= Uninitialized;
		_shaders	= null;
		_renderPass	= null;
	}
	
/*
=================================================
	_ValidateRenderPass
=================================================
*/
	bool Vk1GraphicsPipeline::_ValidateRenderPass () const
	{
		Message< GpuMsg::GetRenderPassDescriptor >	req_descr;
		_renderPass->Send( req_descr );

		const auto&	descr = *req_descr->result;

		CHECK_ERR( _descr.subpass < descr.Subpasses().Count() );
		
		const auto&	subpass	= descr.Subpasses()[ _descr.subpass ];

		CHECK_ERR( _descr.fragOutput.Count() == subpass.colors.Count() );

		// can't make mistake when number of attachment is 1
		if ( _descr.fragOutput.Count() <= 1 )
			return true;

		FOR( i, subpass.colors )
		{
			auto	frag = _descr.fragOutput( subpass.colors[i].name );
			CHECK_ERR( frag and frag->index == i );
		}

		return true;
	}
//-----------------------------------------------------------------------------


	
	const TypeIdList	Vk1ComputePipeline::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	Vk1ComputePipeline::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1ComputePipeline::Vk1ComputePipeline (GlobalSystemsRef gs, const CreateInfo::ComputePipeline &ci) :
		Vk1BaseModule( gs, ModuleConfig{ VkComputePipelineModuleID, ~0u }, &_msgTypes, &_eventTypes ),
		_pipelineId( VK_NULL_HANDLE ),
		_descr( ci.descr ),
		_shaders( ci.shaders )
	{
		SetDebugName( "Vk1ComputePipeline" );

		_SubscribeOnMsg( this, &Vk1ComputePipeline::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1ComputePipeline::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1ComputePipeline::_AttachModule_Impl );
		_SubscribeOnMsg( this, &Vk1ComputePipeline::_DetachModule_Impl );
		_SubscribeOnMsg( this, &Vk1ComputePipeline::_FindModule_Impl );
		_SubscribeOnMsg( this, &Vk1ComputePipeline::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Vk1ComputePipeline::_Link_Impl );
		_SubscribeOnMsg( this, &Vk1ComputePipeline::_Compose );
		_SubscribeOnMsg( this, &Vk1ComputePipeline::_Delete );
		_SubscribeOnMsg( this, &Vk1ComputePipeline::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1ComputePipeline::_DeviceBeforeDestroy );
		_SubscribeOnMsg( this, &Vk1ComputePipeline::_GetVkComputePipelineID );
		_SubscribeOnMsg( this, &Vk1ComputePipeline::_GetComputePipelineDescriptor );
		_SubscribeOnMsg( this, &Vk1ComputePipeline::_GetDeviceInfo );
		_SubscribeOnMsg( this, &Vk1ComputePipeline::_GetVkDeviceInfo );
		_SubscribeOnMsg( this, &Vk1ComputePipeline::_GetVkPrivateClasses );
		_SubscribeOnMsg( this, &Vk1ComputePipeline::_GetPipelineLayoutDescriptor );
		_SubscribeOnMsg( this, &Vk1ComputePipeline::_GetVkDescriptorLayouts );
		_SubscribeOnMsg( this, &Vk1ComputePipeline::_GetVkPipelineLayoutID );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, VkThreadModuleID, true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Vk1ComputePipeline::~Vk1ComputePipeline ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool Vk1ComputePipeline::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreatePipeline() );

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
	bool Vk1ComputePipeline::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyPipeline();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetVkComputePipelineID
=================================================
*/
	bool Vk1ComputePipeline::_GetVkComputePipelineID (const Message< GpuMsg::GetVkComputePipelineID > &msg)
	{
		msg->result.Set( _pipelineId );
		return true;
	}
	
/*
=================================================
	_GetComputePipelineDescriptor
=================================================
*/
	bool Vk1ComputePipeline::_GetComputePipelineDescriptor (const Message< GpuMsg::GetComputePipelineDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_GetPipelineLayoutDescriptor
=================================================
*/
	bool Vk1ComputePipeline::_GetPipelineLayoutDescriptor (const Message< GpuMsg::GetPipelineLayoutDescriptor > &msg)
	{
		if ( _layout )
			msg->result.Set( _layout->GetDescriptor() );
		return true;
	}
	
/*
=================================================
	_GetVkDescriptorLayouts
=================================================
*/
	bool Vk1ComputePipeline::_GetVkDescriptorLayouts (const Message< GpuMsg::GetVkDescriptorLayouts > &msg)
	{
		if ( _layout )
			msg->result.Set( _layout->GetDescriptorLayouts() );
		return true;
	}
		
/*
=================================================
	_GetVkPipelineLayoutID
=================================================
*/
	bool Vk1ComputePipeline::_GetVkPipelineLayoutID (const Message< GpuMsg::GetVkPipelineLayoutID > &msg)
	{
		if ( _layout )
			msg->result.Set( _layout->GetLayoutID() );
		return true;
	}

/*
=================================================
	_IsCreated
=================================================
*/
	bool Vk1ComputePipeline::_IsCreated () const
	{
		return _pipelineId != VK_NULL_HANDLE;
	}
	
/*
=================================================
	_CreatePipeline
=================================================
*/
	bool Vk1ComputePipeline::_CreatePipeline ()
	{
		using namespace vk;

		CHECK_ERR( not _IsCreated() );
		CHECK_ERR( _GetManager() );

		// get shader modules
		Message< GpuMsg::GetVkShaderModuleIDs >		req_shader_ids;
		SendTo( _shaders, req_shader_ids );
		CHECK_ERR( req_shader_ids->result and not req_shader_ids->result->Empty() );

		usize	cs_index = -1;

		FOR( i, *req_shader_ids->result ) {
			if ( (*req_shader_ids->result)[i].type == EShader::Compute ) {
				cs_index = i;
				break;
			}
		}
		CHECK_ERR( cs_index < req_shader_ids->result->Count() );	// compute shader not found

		// create layout
		Message< GpuMsg::GetVkPrivateClasses >	req_cache;
		_SendMsg( req_cache );

		CHECK_ERR(( _layout = req_cache->result->layoutCache->Create( _descr.layout ) ));

		// create compute pipeline
		CHECK_ERR( req_cache->result->pipelineCache->
						CreatePipeline( OUT _pipelineId,
										(*req_shader_ids->result)[cs_index],
										_layout->GetLayoutID() )
		);
		
		return true;
	}
	
/*
=================================================
	_DestroyPipeline
=================================================
*/
	void Vk1ComputePipeline::_DestroyPipeline ()
	{
		using namespace vk;

		auto	dev = GetVkDevice();

		if ( dev != VK_NULL_HANDLE and _pipelineId != VK_NULL_HANDLE )
		{
			vkDestroyPipeline( dev, _pipelineId, null );
		}

		_pipelineId	= VK_NULL_HANDLE;
		_descr		= Uninitialized;
		_shaders	= null;
	}
//-----------------------------------------------------------------------------

}	// PlatformVK

namespace Platforms
{

	ModulePtr VulkanContext::_CreateVk1GraphicsPipeline (GlobalSystemsRef gs, const CreateInfo::GraphicsPipeline &ci)
	{
		ModulePtr	mod;
		CHECK_ERR( mod = gs->Get< ParallelThread >()->GetModuleByMsg< MessageListFrom< GpuMsg::GetVkPrivateClasses > >() );

		Message< GpuMsg::GetVkPrivateClasses >	req_cl;
		mod->Send( req_cl );
		CHECK_ERR( req_cl->result.IsDefined() and req_cl->result->pipelineCache );

		return req_cl->result->pipelineCache->Create( gs, ci );
	}
		
	ModulePtr VulkanContext::_CreateVk1ComputePipeline (GlobalSystemsRef gs, const CreateInfo::ComputePipeline &ci)
	{
		ModulePtr	mod;
		CHECK_ERR( mod = gs->Get< ParallelThread >()->GetModuleByMsg< MessageListFrom< GpuMsg::GetVkPrivateClasses > >() );

		Message< GpuMsg::GetVkPrivateClasses >	req_cl;
		mod->Send( req_cl );
		CHECK_ERR( req_cl->result.IsDefined() and req_cl->result->pipelineCache );

		return req_cl->result->pipelineCache->Create( gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
