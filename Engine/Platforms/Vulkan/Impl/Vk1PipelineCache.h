// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1BaseObject.h"
#include "Engine/Platforms/Shared/GPU/Pipeline.h"
#include "Engine/Platforms/Shared/GPU/RenderPass.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{

	//
	// Vulkan Pipeline Cache
	//

	class Vk1PipelineCache final : public Vk1BaseObject
	{
	// types
	private:
		using EDynamicStates	= EPipelineDynamicState::bits;
		using Vk1DynamicStates	= StaticArray< vk::VkDynamicState, EPipelineDynamicState::_Count >;
		using Attachment_t		= StaticArray< vk::VkPipelineColorBlendAttachmentState, RenderState::ColorBuffersState::ColorBuffers_t::STATIC_COUNT >;
		using ShaderModule		= GpuMsg::GetVkShaderModuleIDs::ShaderModule;
		using Viewports_t		= FixedSizeArray< vk::VkViewport, 16 >;
		using Scissors_t		= FixedSizeArray< vk::VkRect2D, 16 >;


	// variables
	private:
		Set< ModulePtr >								_graphicsPipelines;
		Set< ModulePtr >								_computePipelines;

		vk::VkPipelineCache								_pipelinesCache;

		// temporary arrays
		Array< vk::VkPipelineShaderStageCreateInfo >	_tempStages;
		Array< vk::VkSpecializationInfo >				_tempSpecialization;
		Array< vk::VkVertexInputBindingDescription >	_tempVertexBinding;
		Array< vk::VkVertexInputAttributeDescription >	_tempVertexAttribs;
		Array< vk::VkDescriptorSetLayoutBinding >		_tempDescriptorBindings;
		Viewports_t										_tempViewports;
		Scissors_t										_tempScissors;
		Vk1DynamicStates								_tempDynamicStates;
		Attachment_t									_tempAttachments;


	// methods
	public:
		explicit Vk1PipelineCache (Ptr<Vk1Device> dev);
		~Vk1PipelineCache ();

		ModulePtr	Create (GlobalSystemsRef gs, const CreateInfo::GraphicsPipeline &ci);
		ModulePtr	Create (GlobalSystemsRef gs, const CreateInfo::ComputePipeline &ci);

		bool CreatePipeline (OUT vk::VkPipeline &pipelineId,
							 ArrayCRef< ShaderModule > shaders,
							 vk::VkPipelineLayout layout,
							 const VertexInputState &attribs,
							 const RenderState &renderState,
							 const EDynamicStates &dynamicStates,
							 uint patchControlPoints,
							 const RenderPassDescriptor &rpDescr,
							 vk::VkRenderPass renderPass,
							 uint subpass);

		bool CreatePipeline (OUT vk::VkPipeline &pipelineId,
							 const ShaderModule &shader,
							 vk::VkPipelineLayout layout);

		void Destroy ();


	private:
		bool _CreatePipelineCache ();
		void _DestroyPipelineCache ();

		void _DestroyPipelines ();

		void _ClearTemp ();

		void _SetColorBlendState (OUT vk::VkPipelineColorBlendStateCreateInfo &outState,
								  OUT Attachment_t &attachments,
								  const RenderState::ColorBuffersState &inState,
								  const RenderPassDescriptor::Subpass_t &subpass) const;

		void _SetColorBlendAttachmentState (OUT vk::VkPipelineColorBlendAttachmentState &outState,
											const RenderState::ColorBuffer &inState,
											bool logicOpEnabled) const;

		void _SetDepthStencilState (OUT vk::VkPipelineDepthStencilStateCreateInfo &outState,
									const RenderState::DepthBufferState &depth,
									const RenderState::StencilBufferState &stencil) const;

		void _SetStencilOpState (OUT vk::VkStencilOpState &outState,
								 const RenderState::StencilFaceState &inState) const;

		void _SetupPipelineInputAssemblyState (OUT vk::VkPipelineInputAssemblyStateCreateInfo &outState,
											   const RenderState::InputAssemblyState &inState) const;

		void _SetRasterizationState (OUT vk::VkPipelineRasterizationStateCreateInfo &outState,
									 const RenderState::RasterizationState &inState) const;

		void _SetMultisampleState (OUT vk::VkPipelineMultisampleStateCreateInfo &outState,
								   const RenderState::MultisampleState &inState) const;

		void _SetTessellationState (OUT vk::VkPipelineTessellationStateCreateInfo &outState, uint patchSize) const;

		void _SetDynamicState (OUT vk::VkPipelineDynamicStateCreateInfo &outState,
								OUT Vk1DynamicStates &states,
								const EDynamicStates &inState) const;

		void _SetShaderStages (OUT Array<vk::VkPipelineShaderStageCreateInfo> &stages,
							   OUT Array<vk::VkSpecializationInfo> &specialization,
							   ArrayCRef<ShaderModule> shaders) const;

		void _SetVertexInputState (OUT vk::VkPipelineVertexInputStateCreateInfo &outState,
								   OUT Array<vk::VkVertexInputBindingDescription> &bindingDescr,
								   OUT Array<vk::VkVertexInputAttributeDescription> &attribDescr,
								   const VertexInputState &inState) const;

		void _SetViewportState (OUT vk::VkPipelineViewportStateCreateInfo &outState,
								OUT Viewports_t &tmpViewports,
								OUT Scissors_t &tmpScissors,
								const RenderPassDescriptor::Subpass_t &subpass) const;
	};


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
