// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/VulkanContext.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Sampler.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1RenderPass.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1PipelineCache.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace Platforms
{
	
	const TypeIdList	VulkanContext::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	VulkanContext::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	VulkanContext::VulkanContext (GlobalSystemsRef gs, const CreateInfo::GpuContext &ci) :
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
		CHECK_ERR( msg->module->GetSupportedMessages().HasAllTypes< VkThreadMsgList_t >() );
		CHECK_ERR( msg->module->GetSupportedEvents().HasAllTypes< VkThreadEventList_t >() );
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
		//CHECK_ERR( msg->module->GetSupportedMessages().HasAllTypes< VkThreadMsgList_t >() );
		//CHECK_ERR( msg->module->GetSupportedEvents().HasAllTypes< VkThreadEventList_t >() );
		ASSERT( _threads.IsExist( msg->module ) );

		_threads.Erase( msg->module );
		return true;
	}
		
/*
=================================================
	Register
=================================================
*/
	void VulkanContext::Register (GlobalSystemsRef gs)
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
	void VulkanContext::Unregister (GlobalSystemsRef gs)
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
	_CreateVulkanContext
=================================================
*/	
	ModulePtr VulkanContext:: _CreateVulkanContext (GlobalSystemsRef gs, const CreateInfo::GpuContext &ci)
	{
		return New< VulkanContext >( gs, ci );
	}


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
