// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/Impl/Vk1Pipeline.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1PipelineCache.h"
#include "Engine/Platforms/Vulkan/VulkanThread.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	
	const Runtime::VirtualTypeList	Vk1GraphicsPipeline::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	Vk1GraphicsPipeline::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1GraphicsPipeline::Vk1GraphicsPipeline (const GlobalSystemsRef gs, const CreateInfo::GraphicsPipeline &ci) :
		Vk1BaseModule( gs, ci.gpuThread, ModuleConfig{ VkGraphicsPipelineModuleID, ~0u }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),
		_renderPass( ci.renderPass ),
		_shaders( ci.shaders ),
		_pipelineId( VK_NULL_HANDLE )
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
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_GetVkLogicDevice );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_GetPipelineLayoutDescriptor );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_GetVkDescriptorLayouts );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_GetVkPipelineLayoutID );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, Platforms::VkThreadModuleID, true );
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
	bool Vk1GraphicsPipeline::_Compose (const  Message< ModuleMsg::Compose > &msg)
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

		if ( not _renderPass )
		{
			_renderPass		= GetDevice()->GetDefaultRenderPass();
			_descr.subpass	= 0;

			LOG( "used default render pass", ELog::Debug );
		}

		
		// get render pass id
		Message< GpuMsg::GetVkRenderPassID >		req_pass_id;
		Message< GpuMsg::GetRenderPassDescriptor >	req_pass_descr;

		SendTo( _renderPass, req_pass_id );
		SendTo( _renderPass, req_pass_descr );

		RenderPassDescriptor	rp_descr;	rp_descr << req_pass_descr->result;
		VkRenderPass			rp			= req_pass_id->result.Get( VK_NULL_HANDLE );
		CHECK_ERR( rp != VK_NULL_HANDLE );
		

		// get shader modules
		Message< GpuMsg::GetVkShaderModuleIDs >				req_shader_ids;
		Array< GpuMsg::GetVkShaderModuleIDs::ShaderModule >	modules;

		SendTo( _shaders, req_shader_ids );

		req_shader_ids->result.MoveTo( OUT modules );
		CHECK_ERR( not modules.Empty() );

		auto	vkthread	= GetGpuThread();

		// create layout
		CHECK_ERR(( _layout = vkthread->GetLayoutCache()->Create( _descr.layout ) ));

		// create graphics pipeline
		CHECK_ERR( vkthread->GetPipelineCache()->
						CreatePipeline( OUT _pipelineId,
										modules,
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

		auto	dev = GetLogicalDevice();

		if ( dev != VK_NULL_HANDLE and _pipelineId != VK_NULL_HANDLE )
		{
			vkDestroyPipeline( dev, _pipelineId, null );
		}

		_pipelineId	= VK_NULL_HANDLE;
		_descr		= Uninitialized;
	}
//-----------------------------------------------------------------------------


	
	const Runtime::VirtualTypeList	Vk1ComputePipeline::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	Vk1ComputePipeline::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1ComputePipeline::Vk1ComputePipeline (const GlobalSystemsRef gs, const CreateInfo::ComputePipeline &ci) :
		Vk1BaseModule( gs, ci.gpuThread, ModuleConfig{ VkComputePipelineModuleID, ~0u }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),
		_shaders( ci.shaders ),
		_pipelineId( VK_NULL_HANDLE )
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
		_SubscribeOnMsg( this, &Vk1ComputePipeline::_GetVkLogicDevice );
		_SubscribeOnMsg( this, &Vk1ComputePipeline::_GetPipelineLayoutDescriptor );
		_SubscribeOnMsg( this, &Vk1ComputePipeline::_GetVkDescriptorLayouts );
		_SubscribeOnMsg( this, &Vk1ComputePipeline::_GetVkPipelineLayoutID );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, Platforms::VkThreadModuleID, true );
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
	bool Vk1ComputePipeline::_Compose (const  Message< ModuleMsg::Compose > &msg)
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
		Message< GpuMsg::GetVkShaderModuleIDs >				req_shader_ids;
		Array< GpuMsg::GetVkShaderModuleIDs::ShaderModule >	modules;

		SendTo( _shaders, req_shader_ids );

		req_shader_ids->result.MoveTo( OUT modules );
		CHECK_ERR( not modules.Empty() );

		usize	cs_index = -1;

		FOR( i, modules ) {
			if ( modules[i].type == EShader::Compute ) {
				cs_index = i;
				break;
			}
		}
		CHECK_ERR( cs_index < modules.Count() );	// compute shader not found

		auto	vkthread	= GetGpuThread();

		// create layout
		CHECK_ERR(( _layout = vkthread->GetLayoutCache()->Create( _descr.layout ) ));

		// create compute pipeline
		CHECK_ERR( vkthread->GetPipelineCache()->
						CreatePipeline( OUT _pipelineId,
										modules[cs_index],
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

		auto	dev = GetLogicalDevice();

		if ( dev != VK_NULL_HANDLE and _pipelineId != VK_NULL_HANDLE )
		{
			vkDestroyPipeline( dev, _pipelineId, null );
		}

		_pipelineId	= VK_NULL_HANDLE;
		_descr		= Uninitialized;
	}
//-----------------------------------------------------------------------------

}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
