// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/VulkanContext.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Memory.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Buffer.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Texture.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Pipeline.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Sampler.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1RenderPass.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Framebuffer.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1CommandBuffer.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1ShaderModules.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1CommandBuilder.h"

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
		Module( gs, ModuleConfig{ GetStaticID(), 1 }, &_msgTypes, &_eventTypes ),
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
		CHECK_ERR( msg->module->GetModuleID() == VulkanThread::GetStaticID() );
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
		CHECK_ERR( msg->module->GetModuleID() == VulkanThread::GetStaticID() );
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

		CHECK( mf->Register( VulkanContext::GetStaticID(), &_CreateVulkanContext ) );
		CHECK( mf->Register( VulkanThread::GetStaticID(), &_CreateVulkanThread ) );
		CHECK( mf->Register( PlatformVK::Vk1Memory::GetStaticID(), &_CreateVk1Memory ) );
		CHECK( mf->Register( PlatformVK::Vk1Buffer::GetStaticID(), &_CreateVk1Buffer ) );
		CHECK( mf->Register( PlatformVK::Vk1Texture::GetStaticID(), &_CreateVk1Texture ) );
		CHECK( mf->Register( PlatformVK::Vk1Sampler::GetStaticID(), &_CreateVk1Sampler ) );
		CHECK( mf->Register( PlatformVK::Vk1RenderPass::GetStaticID(), &_CreateVk1RenderPass ) );
		CHECK( mf->Register( PlatformVK::Vk1Framebuffer::GetStaticID(), &_CreateVk1Framebuffer ) );
		CHECK( mf->Register( PlatformVK::Vk1CommandBuffer::GetStaticID(), &_CreateVk1CommandBuffer ) );
		CHECK( mf->Register( PlatformVK::Vk1ShaderModules::GetStaticID(), &_CreateVk1ShaderModules ) );
		CHECK( mf->Register( PlatformVK::Vk1ShaderModules::GetStaticID(), &_CreateVk1ShaderModules2 ) );
		CHECK( mf->Register( PlatformVK::Vk1GraphicsPipeline::GetStaticID(), &_CreateVk1GraphicsPipeline ) );
		CHECK( mf->Register( PlatformVK::Vk1CommandBuilder::GetStaticID(), &_CreateVk1CommandBuilder ) );
	}
	
/*
=================================================
	Unregister
=================================================
*/
	void VulkanContext::Unregister (const GlobalSystemsRef gs)
	{
		auto	mf = gs->Get< ModulesFactory >();

		mf->UnregisterAll< VulkanContext >();
		mf->UnregisterAll< VulkanThread >();
		mf->UnregisterAll< PlatformVK::Vk1Memory >();
		mf->UnregisterAll< PlatformVK::Vk1Buffer >();
		mf->UnregisterAll< PlatformVK::Vk1Texture >();
		mf->UnregisterAll< PlatformVK::Vk1Sampler >();
		mf->UnregisterAll< PlatformVK::Vk1RenderPass >();
		mf->UnregisterAll< PlatformVK::Vk1Framebuffer >();
		mf->UnregisterAll< PlatformVK::Vk1CommandBuffer >();
		mf->UnregisterAll< PlatformVK::Vk1ShaderModules >();
		mf->UnregisterAll< PlatformVK::Vk1GraphicsPipeline >();
		mf->UnregisterAll< PlatformVK::Vk1CommandBuilder >();
	}

/*
=================================================
	_Create***
=================================================
*/
	inline Ptr< VulkanThread >  _GetVulkanThread (const GlobalSystemsRef gs, const ModulePtr &gpuThread)
	{
		return (gpuThread ? gpuThread : gs->Get< ParallelThread >()->GetModule( VulkanThread::GetStaticID() )).ToPtr< VulkanThread >();
	}

	ModulePtr VulkanContext::_CreateVulkanThread (const GlobalSystemsRef gs, const CreateInfo::GpuThread &ci)
	{
		return New< VulkanThread >( gs, ci );
	}
	
	ModulePtr VulkanContext:: _CreateVulkanContext (const GlobalSystemsRef gs, const CreateInfo::GpuContext &ci)
	{
		return New< VulkanContext >( gs, ci );
	}

	ModulePtr VulkanContext::_CreateVk1Memory (const GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci)
	{
		return New< PlatformVK::Vk1Memory >( gs, ci );
	}
	
	ModulePtr VulkanContext::_CreateVk1Buffer (const GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci)
	{
		return New< PlatformVK::Vk1Buffer >( gs, ci );
	}

	ModulePtr VulkanContext::_CreateVk1Texture (const GlobalSystemsRef gs, const CreateInfo::GpuTexture &ci)
	{
		return New< PlatformVK::Vk1Texture >( gs, ci );
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
	
	ModulePtr VulkanContext::_CreateVk1Framebuffer (const GlobalSystemsRef gs, const CreateInfo::GpuFramebuffer &ci)
	{
		return New< PlatformVK::Vk1Framebuffer >( gs, ci );
	}

	ModulePtr VulkanContext::_CreateVk1CommandBuffer (const GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci)
	{
		return New< PlatformVK::Vk1CommandBuffer >( gs, ci );
	}

	ModulePtr VulkanContext::_CreateVk1ShaderModules (const GlobalSystemsRef gs, const CreateInfo::GpuShaderModulesFromFile &ci)
	{
		return New< PlatformVK::Vk1ShaderModules >( gs, ci );
	}

	ModulePtr VulkanContext::_CreateVk1ShaderModules2 (const GlobalSystemsRef gs, const CreateInfo::GpuShaderModulesFromBuffer &ci)
	{
		return New< PlatformVK::Vk1ShaderModules >( gs, ci );
	}
	
	ModulePtr VulkanContext::_CreateVk1CommandBuilder (const GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci)
	{
		return New< PlatformVK::Vk1CommandBuilder >( gs, ci );
	}


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
