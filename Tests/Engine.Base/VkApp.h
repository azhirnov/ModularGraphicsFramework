// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Common.h"
#include "Engine/Platforms/Vulkan/Impl/vulkan1.h"
#include "Engine/Platforms/Vulkan/VulkanThread.h"

class VkApp : public StaticRefCountedObject
{
// variables
public:
	Ptr< Module >			ms;

private:
	bool					looping				= true;

	vk::VkShaderModule		fshader				= VK_NULL_HANDLE;
	vk::VkShaderModule		vshader				= VK_NULL_HANDLE;
	vk::VkPipelineLayout	pipelineLayout		= VK_NULL_HANDLE;
	vk::VkPipeline			graphicsPipeline	= VK_NULL_HANDLE;


// methods
public:
	VkApp ();
	void Initialize ();
	void Quit ();
	bool Update ();


private:
	bool _OnWindowClosed (const Message<ModuleMsg::WindowAfterDestroy> &);
	bool _OnKey (const Message< ModuleMsg::InputKey > &);
	bool _OnMotion (const Message< ModuleMsg::InputMotion > &);
	bool _Draw (const Message< ModuleMsg::Update > &);
	bool _VkInit (const Message< ModuleMsg::GpuDeviceCreated > &);
	bool _VkDelete (const Message< ModuleMsg::GpuDeviceBeforeDestory > &);

	bool _CompileShaders (Ptr< Vk1Device > device);
	bool _CreatePipeline (Ptr< Vk1Device > device);
	bool _InitCommandBuffers (Ptr< Vk1Device > device);
};
