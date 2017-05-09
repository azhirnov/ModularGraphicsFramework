// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/VulkanThread.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	
/*
=================================================
	GetVkSubSystems
=================================================
*/
	forceinline VkSystemsRef GetVkSubSystems (const GlobalSystemsRef gs, const ModulePtr &gpuThread)
	{
		if ( gpuThread )
			return gpuThread.ToPtr< VulkanThread >()->GetDevice()->VkSystems();

		ModulePtr const&	gpu_thread = gs->Get< ParallelThread >()->GetModule( VulkanThread::GetStaticID() );
		
		if ( gpu_thread )
			return gpu_thread.ToPtr< VulkanThread >()->GetDevice()->VkSystems();

		return VkSystemsRef(null);
	}
	
/*
=================================================
	constructor
=================================================
*/
	Vk1BaseModule::Vk1BaseModule (const GlobalSystemsRef gs,
								  const ModuleConfig &config,
								  const Runtime::VirtualTypeList *msgTypes,
								  const Runtime::VirtualTypeList *eventTypes) :
		Module( gs, config, msgTypes, eventTypes ),
		_vkSystems( GetVkSubSystems( gs, null ) )
	{
	}
		
/*
=================================================
	constructor
=================================================
*/
	Vk1BaseModule::Vk1BaseModule (const GlobalSystemsRef gs,
								  const ModulePtr &gpuThread,
								  const ModuleConfig &config,
								  const Runtime::VirtualTypeList *msgTypes,
								  const Runtime::VirtualTypeList *eventTypes) :
		Module( gs, config, msgTypes, eventTypes ),
		_vkSystems( GetVkSubSystems( gs, null ) )
	{
	}
	
/*
=================================================
	constructor
=================================================
*/
	Vk1BaseModule::Vk1BaseModule (const GlobalSystemsRef gs,
								  const VkSystemsRef vkSys,
								  const ModuleConfig &config,
								  const Runtime::VirtualTypeList *msgTypes,
								  const Runtime::VirtualTypeList *eventTypes) :
		Module( gs, config, msgTypes, eventTypes ),
		_vkSystems( vkSys )
	{
	}
	
/*
=================================================
	_OnManagerChanged
=================================================
*/
	bool Vk1BaseModule::_OnManagerChanged (const Message< ModuleMsg::OnManagerChanged > &msg)
	{
		if ( msg->newManager )
			msg->newManager->Subscribe( this, &Vk1BaseModule::_GpuDeviceBeforeDestory );
		return true;
	}
	
/*
=================================================
	_DestroyResources
=================================================
*/
	void Vk1BaseModule::_DestroyResources ()
	{
		_SendMsg( Message< ModuleMsg::Delete >() );
	}

/*
=================================================
	_GpuDeviceBeforeDestory
=================================================
*/
	bool Vk1BaseModule::_GpuDeviceBeforeDestory (const Message< ModuleMsg::GpuDeviceBeforeDestory > &msg)
	{
		_DestroyResources();
		return true;
	}


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
