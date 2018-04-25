// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Public/GPU/Context.h"
#include "Engine/Platforms/Public/GPU/Thread.h"
#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/CommandBuffer.h"
#include "Engine/Platforms/Public/GPU/Framebuffer.h"
#include "Engine/Platforms/Public/GPU/Memory.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Messages.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Sampler.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1RenderPass.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1PipelineCache.h"
#include "Engine/Platforms/Vulkan/VulkanObjectsConstructor.h"

namespace Engine
{
namespace Platforms
{

	//
	// Vulkan Context
	//
	
	class VulkanContext final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager,
											GpuMsg::GetGraphicsModules
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t;

		using VkThreads_t			= Set< ModulePtr >;

		using VkThreadMsgList_t		= MessageListFrom< GpuMsg::ThreadBeginFrame, GpuMsg::ThreadEndFrame, GpuMsg::GetVkDeviceInfo >;
		using VkThreadEventList_t	= MessageListFrom< GpuMsg::DeviceCreated, GpuMsg::DeviceBeforeDestroy >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;

		
	// variables
	private:
		CreateInfo::GpuContext	_createInfo;

		VkThreads_t				_threads;
		

	// methods
	public:
		VulkanContext (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuContext &ci);
		~VulkanContext ();

		
	// message handlers
	private:
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);
		bool _GetGraphicsModules (const Message< GpuMsg::GetGraphicsModules > &);
	};
//-----------------------------------------------------------------------------



	const TypeIdList	VulkanContext::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	VulkanContext::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	VulkanContext::VulkanContext (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuContext &ci) :
		Module( gs, ModuleConfig{ id, 1 }, &_msgTypes, &_eventTypes ),
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
		_SubscribeOnMsg( this, &VulkanContext::_GetGraphicsModules );
		
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

		ModulePtr	module = msg->module.Lock();

		if ( not module )
			return false;

		ASSERT( _threads.IsExist( module ) );

		_threads.Erase( module );
		return true;
	}
	
/*
=================================================
	_GetGraphicsModules
=================================================
*/	
	bool VulkanContext::_GetGraphicsModules (const Message< GpuMsg::GetGraphicsModules > &msg)
	{
		msg->compute.Set( VulkanObjectsConstructor::GetComputeModules() );
		msg->graphics.Set( VulkanObjectsConstructor::GetGraphicsModules() );
		return true;
	}
//-----------------------------------------------------------------------------


	
/*
=================================================
	GetGraphicsModules
=================================================
*/
	GraphicsModuleIDs VulkanObjectsConstructor::GetGraphicsModules ()
	{
		GraphicsModuleIDs	graphics;
		graphics.pipeline		= VkGraphicsPipelineModuleID;
		graphics.framebuffer	= VkFramebufferModuleID;
		graphics.renderPass		= VkRenderPassModuleID;
		graphics.context		= VkContextModuleID;
		graphics.thread			= VkThreadModuleID;
		graphics.commandBuffer	= VkCommandBufferModuleID;
		graphics.commandBuilder	= VkCommandBuilderModuleID;
		graphics.buffer			= VkBufferModuleID;
		graphics.image			= VkImageModuleID;
		graphics.sampler		= VkSamplerModuleID;
		graphics.memory			= VkManagedMemoryModuleID;
		graphics.memoryManager	= VkMemoryManagerModuleID;
		graphics.resourceTable	= VkPipelineResourceTableModuleID;
		return graphics;
	}
	
/*
=================================================
	GetComputeModules
=================================================
*/
	ComputeModuleIDs VulkanObjectsConstructor::GetComputeModules ()
	{
		ComputeModuleIDs	compute;
		compute.pipeline		= VkComputePipelineModuleID;
		compute.context			= VkContextModuleID;
		compute.thread			= VkThreadModuleID;
		compute.commandBuffer	= VkCommandBufferModuleID;
		compute.commandBuilder	= VkCommandBuilderModuleID;
		compute.buffer			= VkBufferModuleID;
		compute.image			= VkImageModuleID;
		compute.memory			= VkManagedMemoryModuleID;
		compute.memoryManager	= VkMemoryManagerModuleID;
		compute.resourceTable	= VkPipelineResourceTableModuleID;
		return compute;
	}

/*
=================================================
	Register
=================================================
*/
	void VulkanObjectsConstructor::Register ()
	{
		auto	mf = GetMainSystemInstance()->GlobalSystems()->modulesFactory;

		CHECK( mf->Register( VkThreadModuleID, &CreateVulkanThread ) );
		CHECK( mf->Register( VkContextModuleID, &CreateVulkanContext ) );

		CHECK( mf->Register( VkImageModuleID, &CreateVk1Image ) );
		CHECK( mf->Register( VkBufferModuleID, &CreateVk1Buffer ) );
		CHECK( mf->Register( VkSamplerModuleID, &CreateVk1Sampler ) );
		CHECK( mf->Register( VkRenderPassModuleID, &CreateVk1RenderPass ) );
		CHECK( mf->Register( VkSyncManagerModuleID, CreateVk1SyncManager ) );
		CHECK( mf->Register( VkFramebufferModuleID, &CreateVk1Framebuffer ) );
		CHECK( mf->Register( VkCommandQueueModuleID, &CreateVk1CommandQueue ) );
		CHECK( mf->Register( VkManagedMemoryModuleID, &CreateVk1ManagedMemory ) );
		CHECK( mf->Register( VkMemoryManagerModuleID, &CreateVk1MemoryManager ) );
		CHECK( mf->Register( VkCommandBufferModuleID, &CreateVk1CommandBuffer ) );
		CHECK( mf->Register( VkCommandBuilderModuleID, &CreateVk1CommandBuilder ) );
		CHECK( mf->Register( VkComputePipelineModuleID, &CreateCachedVk1ComputePipeline ) );
		CHECK( mf->Register( VkGraphicsPipelineModuleID, &CreateCachedVk1GraphicsPipeline ) );
		CHECK( mf->Register( VkPipelineResourceTableModuleID, &CreateVk1PipelineResourceTable ) );

		if ( not mf->IsRegistered< CreateInfo::PipelineTemplate >( PipelineTemplateModuleID ) )
			CHECK( mf->Register( PipelineTemplateModuleID, &CreatePipelineTemplate ) );
	}
	
/*
=================================================
	Unregister
=================================================
*/
	void VulkanObjectsConstructor::Unregister ()
	{
		auto	mf = GetMainSystemInstance()->GlobalSystems()->modulesFactory;

		mf->UnregisterAll( VkThreadModuleID );
		mf->UnregisterAll( VkContextModuleID );
		
		mf->UnregisterAll( VkImageModuleID );
		mf->UnregisterAll( VkBufferModuleID );
		mf->UnregisterAll( VkSamplerModuleID );
		mf->UnregisterAll( VkRenderPassModuleID );
		mf->UnregisterAll( VkFramebufferModuleID );
		mf->UnregisterAll( VkCommandQueueModuleID );
		mf->UnregisterAll( VkManagedMemoryModuleID );
		mf->UnregisterAll( VkMemoryManagerModuleID );
		mf->UnregisterAll( VkCommandBufferModuleID );
		mf->UnregisterAll( VkSyncManagerModuleID );
		mf->UnregisterAll( VkCommandBuilderModuleID );
		mf->UnregisterAll( VkComputePipelineModuleID );
		mf->UnregisterAll( VkGraphicsPipelineModuleID );
		mf->UnregisterAll( VkPipelineResourceTableModuleID );

		//mf->UnregisterAll< Platforms::PipelineTemplate >();	// TODO
	}

/*
=================================================
	CreateVulkanContext
=================================================
*/	
	ModulePtr VulkanObjectsConstructor::CreateVulkanContext (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuContext &ci)
	{
		return New< VulkanContext >( id, gs, ci );
	}


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
