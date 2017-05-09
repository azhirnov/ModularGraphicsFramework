// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Shared/GPU/Pipeline.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	
	//
	// Vulkan Graphics Pipeline
	//

	class Vk1GraphicsPipeline final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::GetGraphicsPipelineDescriptor,
											ModuleMsg::GetGraphicsPipelineID
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;

		using Descriptor			= GraphicsPipelineDescriptor;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		Descriptor		_descr;

		ModulePtr		_shaders;
		ModulePtr		_renderPass;

		vk::VkPipeline	_pipelineId;


	// methods
	public:
		Vk1GraphicsPipeline (const GlobalSystemsRef gs, const CreateInfo::GraphicsPipeline &ci);
		~Vk1GraphicsPipeline ();

		static OModID::type		GetStaticID ()				{ return "vk1.g-ppln"_OModID; }


	//protected:
		Descriptor const&		GetDescriptor ()	const	{ return _descr; }
		vk::VkPipeline			GetPipelineID ()	const	{ return _pipelineId; }


	// message handlers
	private:
		bool _Compose (const  Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetGraphicsPipelineID (const Message< ModuleMsg::GetGraphicsPipelineID > &);
		bool _GetGraphicsPipelineDescriptor (const Message< ModuleMsg::GetGraphicsPipelineDescriptor > &);

	private:
		bool _IsCreated () const;
		bool _CreatePipeline ();
		void _DestroyPipeline ();
	};
	


	//
	// Vulkan Compute Pipeline
	//
	/*
	class Vk1ComputePipeline final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::GetComputePipelineDescriptor
											ModuleMsg::GetComputePipelineID
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		PipelineDescriptor		_descr;

		ModulePtr				_shaders;
		ModulePtr				_renderPass;
		uint					_subpass;

		vk::VkPipeline			_pipelineId;
		const bool				_isGraphics;


	// methods
	public:
		Vk1ComputePipeline (const GlobalSystemsRef gs, const CreateInfo::ComputePipeline &ci);
		~Vk1ComputePipeline ();

		static OModID::type			GetStaticID ()				{ return "vk1.c-ppln"_OModID; }


	//protected:
		PipelineDescriptor const&	GetDescriptor ()	const	{ return _descr; }
		vk::VkPipeline				GetPipelineID ()	const	{ return _pipelineId; }


	// message handlers
	private:
		bool _Compose (const  Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetComputePipelineID (const Message< ModuleMsg::GetGraphicsPipelineID > &);
		bool _GetGpuPipelineDescriptor (const Message< ModuleMsg::GetGpuRenderPassDescriptor > &);

	private:
		bool _IsCreated () const;
		bool _CreateGraphicsPipeline ();
		void _DestroyPipeline ();
	};
	*/


	//
	// Vulkan Pipeline Cache
	//

	class Vk1PipelineCache final
	{
	// types
	private:
		class InScreenRenderPass;
		SHARED_POINTER( InScreenRenderPass );

		SHARED_POINTER( Vk1GraphicsPipeline );
		
		using DynamicStates		= EPipelineDynamicState::bits;
		using Vk1DynamicStates	= StaticArray< vk::VkDynamicState, EPipelineDynamicState::_Count >;
		using Attachment_t		= StaticArray< vk::VkPipelineColorBlendAttachmentState, RenderState::ColorBuffersState::ColorBuffers_t::STATIC_COUNT >;
		using ShaderModule		= ModuleMsg::GetGpuShaderModuleIDs::ShaderModule;
		using Viewports_t		= FixedSizeArray< vk::VkViewport, 16 >;
		using Scissors_t		= FixedSizeArray< vk::VkRect2D, 16 >;


	// variables
	private:
		Set< Vk1GraphicsPipelinePtr >					_graphicsPipelines;
		vk::VkPipelineCache								_graphicsPipelinesCache;

		vk::VkPipelineLayout							_emptyLayout;
		InScreenRenderPassPtr							_inScreenRenderPass;

		// temporary arrays
		Array< vk::VkPipelineShaderStageCreateInfo >	_stages;
		Array< vk::VkSpecializationInfo >				_specialization;
		Array< vk::VkVertexInputBindingDescription >	_bindingDescr;
		Array< vk::VkVertexInputAttributeDescription >	_attribDescr;
		Viewports_t										_viewports;
		Scissors_t										_scissors;
		Vk1DynamicStates								_dynamicStates;
		Attachment_t									_attachments;

		const VkSystemsRef								_vkSystems;


	// methods
	public:
		explicit Vk1PipelineCache (VkSystemsRef vkSys);
		~Vk1PipelineCache ();

		Vk1GraphicsPipelinePtr	Create (const GlobalSystemsRef gs, const CreateInfo::GraphicsPipeline &ci);

		VkSystemsRef	VkSystems ()	const	{ return _vkSystems; }

		bool CreatePipeline (OUT vk::VkPipeline &pipelineId,
							 ArrayRef< ShaderModule > shaders,
							 vk::VkPipelineLayout layout,
							 const VertexAttribs &attribs,
							 const RenderState &renderState,
							 const DynamicStates &dynamicStates,
							 uint patchControlPoints,
							 const RenderPassDescriptor &rpDescr,
							 vk::VkRenderPass renderPass,
							 uint subpass);

		//bool CreatePipeline (OUT vk::VkPipeline &pipelineId,
		//					 vk::VkShaderModule shader);

		void DestroyCache ();


	private:
		bool _CreateCache (vk::VkDevice dev);

		bool _CreateLayout (vk::VkDevice dev);
		void _DestroyLayout (vk::VkDevice dev);

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
								const DynamicStates &inState) const;

		void _SetShaderStages (OUT Array<vk::VkPipelineShaderStageCreateInfo> &stages,
							   OUT Array<vk::VkSpecializationInfo> &specialization,
							   ArrayRef<ShaderModule> shaders) const;

		void _SetVertexInputState (OUT vk::VkPipelineVertexInputStateCreateInfo &outState,
								   OUT Array<vk::VkVertexInputBindingDescription> &bindingDescr,
								   OUT Array<vk::VkVertexInputAttributeDescription> &attribDescr,
								   const VertexAttribs &inState) const;

		void _SetViewportState (OUT vk::VkPipelineViewportStateCreateInfo &outState,
								OUT Viewports_t &tmpViewports,
								OUT Scissors_t &tmpScissors,
								const RenderPassDescriptor::Subpass_t &subpass);
	};


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
