// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/VulkanContext.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Pipeline.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Sampler.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1RenderPass.h"
#include "Engine/Platforms/Vulkan/VulkanThread.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace Platforms
{
	
	const Runtime::VirtualTypeList	VulkanContext::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	VulkanContext::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	VulkanContext::VulkanContext (const GlobalSystemsRef gs, const CreateInfo::GpuContext &ci) :
		Module( gs, ModuleConfig{ VkContextModuleID, 1 }, &_msgTypes, &_eventTypes ),
		_createInfo( ci )
	{
		SetDebugName( "VulkanContext" );

		_SubscribeOnMsg( this, &VulkanContext::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &VulkanContext::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &VulkanContext::_AttachModule_Impl );
		_SubscribeOnMsg( this, &VulkanContext::_DetachModule_Impl );
		_SubscribeOnMsg( this, &VulkanContext::_FindModule_Empty );
		_SubscribeOnMsg( this, &VulkanContext::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &VulkanContext::_Update_Empty );
		_SubscribeOnMsg( this, &VulkanContext::_Link_Impl );
		_SubscribeOnMsg( this, &VulkanContext::_Compose_Impl );
		_SubscribeOnMsg( this, &VulkanContext::_Delete_Impl );
		_SubscribeOnMsg( this, &VulkanContext::_AddToManager );
		_SubscribeOnMsg( this, &VulkanContext::_RemoveFromManager );
		
		CHECK( _ValidateMsgSubscriptions() );
	}
	
/*
=================================================
	destructor
=================================================
*/
	VulkanContext::~VulkanContext ()
	{
		LOG( "VulkanContext finalized", ELog::Debug );

		ASSERT( _threads.Empty() );
	}
	
/*
=================================================
	_AddToManager
=================================================
*/
	bool VulkanContext::_AddToManager (const Message< ModuleMsg::AddToManager > &msg)
	{
		CHECK_ERR( msg->module );
		CHECK_ERR( msg->module->GetModuleID() == Platforms::VkThreadModuleID );
		ASSERT( not _threads.IsExist( msg->module ) );

		_threads.Add( msg->module );
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool VulkanContext::_RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &msg)
	{
		CHECK_ERR( msg->module );
		CHECK_ERR( msg->module->GetModuleID() == Platforms::VkThreadModuleID );
		ASSERT( _threads.IsExist( msg->module ) );

		_threads.Erase( msg->module );
		return true;
	}
		
/*
=================================================
	Register
=================================================
*/
	void VulkanContext::Register (const GlobalSystemsRef gs)
	{
		auto	mf = gs->Get< ModulesFactory >();

		CHECK( mf->Register( VkThreadModuleID, &_CreateVulkanThread ) );
		CHECK( mf->Register( VkContextModuleID, &_CreateVulkanContext ) );

		CHECK( mf->Register( VkImageModuleID, &_CreateVk1Image ) );
		CHECK( mf->Register( VkMemoryModuleID, &_CreateVk1Memory ) );
		CHECK( mf->Register( VkBufferModuleID, &_CreateVk1Buffer ) );
		CHECK( mf->Register( VkSamplerModuleID, &_CreateVk1Sampler ) );
		CHECK( mf->Register( VkRenderPassModuleID, &_CreateVk1RenderPass ) );
		CHECK( mf->Register( VkFramebufferModuleID, &_CreateVk1Framebuffer ) );
		CHECK( mf->Register( VkCommandBufferModuleID, &_CreateVk1CommandBuffer ) );
		CHECK( mf->Register( VkCommandBuilderModuleID, &_CreateVk1CommandBuilder ) );
		CHECK( mf->Register( VkGraphicsPipelineModuleID, &_CreateVk1GraphicsPipeline ) );
		CHECK( mf->Register( VkPipelineResourceTableModuleID, &_CreateVk1PipelineResourceTable ) );

		if ( not mf->IsRegistered< CreateInfo::PipelineTemplate >( PipelineTemplateModuleID ) )
			CHECK( mf->Register( PipelineTemplateModuleID, &_CreatePipelineTemplate ) );
	}
	
/*
=================================================
	Unregister
=================================================
*/
	void VulkanContext::Unregister (const GlobalSystemsRef gs)
	{
		auto	mf = gs->Get< ModulesFactory >();

		mf->UnregisterAll( VkThreadModuleID );
		mf->UnregisterAll( VkContextModuleID );

		mf->UnregisterAll( VkImageModuleID );
		mf->UnregisterAll( VkMemoryModuleID );
		mf->UnregisterAll( VkBufferModuleID );
		mf->UnregisterAll( VkSamplerModuleID );
		mf->UnregisterAll( VkRenderPassModuleID );
		mf->UnregisterAll( VkFramebufferModuleID );
		mf->UnregisterAll( VkCommandBufferModuleID );
		mf->UnregisterAll( VkCommandBuilderModuleID );
		mf->UnregisterAll( VkGraphicsPipelineModuleID );
		mf->UnregisterAll( VkPipelineResourceTableModuleID );

		//mf->UnregisterAll< Platforms::PipelineTemplate >();	// TODO
	}

/*
=================================================
	_Create***
=================================================
*/
	inline Ptr< VulkanThread >  _GetVulkanThread (const GlobalSystemsRef gs, const ModulePtr &gpuThread)
	{
		return (gpuThread ? gpuThread : gs->Get< ParallelThread >()->GetModuleByID( Platforms::VkThreadModuleID )).ToPtr< VulkanThread >();
	}

	ModulePtr VulkanContext::_CreateVulkanThread (const GlobalSystemsRef gs, const CreateInfo::GpuThread &ci)
	{
		return New< VulkanThread >( gs, ci );
	}
	
	ModulePtr VulkanContext:: _CreateVulkanContext (const GlobalSystemsRef gs, const CreateInfo::GpuContext &ci)
	{
		return New< VulkanContext >( gs, ci );
	}

	ModulePtr VulkanContext::_CreateVk1Sampler (const GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci)
	{
		auto thread = _GetVulkanThread( gs, ci.gpuThread );
		CHECK_ERR( thread );
		return thread->GetSamplerCache()->Create( gs, ci );
	}

	ModulePtr VulkanContext::_CreateVk1RenderPass (const GlobalSystemsRef gs, const CreateInfo::GpuRenderPass &ci)
	{
		auto thread = _GetVulkanThread( gs, ci.gpuThread );
		CHECK_ERR( thread );
		return thread->GetRenderPassCache()->Create( gs, ci );
	}

	ModulePtr VulkanContext::_CreateVk1GraphicsPipeline (const GlobalSystemsRef gs, const CreateInfo::GraphicsPipeline &ci)
	{
		auto thread = _GetVulkanThread( gs, ci.gpuThread );
		CHECK_ERR( thread );
		return thread->GetPipelineCache()->Create( gs, ci );
	}
		
	ModulePtr VulkanContext::_CreateVk1ComputePipeline (const GlobalSystemsRef gs, const CreateInfo::ComputePipeline &ci)
	{
		auto thread = _GetVulkanThread( gs, ci.gpuThread );
		CHECK_ERR( thread );
		return thread->GetPipelineCache()->Create( gs, ci );
	}


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
