// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Shared/GPU/Pipeline.h"
#include "Engine/Platforms/Shared/GPU/RenderPass.h"
#include "Engine/Platforms/Shared/GPU/VertexInputState.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	class Vk1PipelineLayout;
	SHARED_POINTER( Vk1PipelineLayout );


	//
	// Vulkan Graphics Pipeline
	//

	class Vk1GraphicsPipeline final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetGraphicsPipelineDescriptor,
											GpuMsg::GetVkGraphicsPipelineID,
											GpuMsg::GetPipelineLayoutDescriptor,
											GpuMsg::GetVkDescriptorLayouts,
											GpuMsg::GetVkPipelineLayoutID
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;

		using Descriptor			= GraphicsPipelineDescriptor;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		vk::VkPipeline			_pipelineId;
		Descriptor				_descr;
		Vk1PipelineLayoutPtr	_layout;
		ModulePtr				_shaders;		// use as attachment
		ModulePtr				_renderPass;	// use as attachment


	// methods
	public:
		Vk1GraphicsPipeline (const GlobalSystemsRef gs, const CreateInfo::GraphicsPipeline &ci);
		~Vk1GraphicsPipeline ();

		Descriptor const&		GetDescriptor ()	const	{ return _descr; }


	// message handlers
	private:
		bool _Compose (const  Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetVkGraphicsPipelineID (const Message< GpuMsg::GetVkGraphicsPipelineID > &);
		bool _GetGraphicsPipelineDescriptor (const Message< GpuMsg::GetGraphicsPipelineDescriptor > &);
		bool _GetPipelineLayoutDescriptor (const Message< GpuMsg::GetPipelineLayoutDescriptor > &);
		bool _GetVkDescriptorLayouts (const Message< GpuMsg::GetVkDescriptorLayouts > &);
		bool _GetVkPipelineLayoutID (const Message< GpuMsg::GetVkPipelineLayoutID > &);

	private:
		bool _IsCreated () const;
		bool _CreatePipeline ();
		void _DestroyPipeline ();
	};
	


	//
	// Vulkan Compute Pipeline
	//
	
	class Vk1ComputePipeline final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetComputePipelineDescriptor,
											GpuMsg::GetVkComputePipelineID,
											GpuMsg::GetPipelineLayoutDescriptor,
											GpuMsg::GetVkDescriptorLayouts,
											GpuMsg::GetVkPipelineLayoutID
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;
		
		using Descriptor			= ComputePipelineDescriptor;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		vk::VkPipeline			_pipelineId;
		Descriptor				_descr;
		Vk1PipelineLayoutPtr	_layout;
		ModulePtr				_shaders;		// use as attachment


	// methods
	public:
		Vk1ComputePipeline (const GlobalSystemsRef gs, const CreateInfo::ComputePipeline &ci);
		~Vk1ComputePipeline ();


	// message handlers
	private:
		bool _Compose (const  Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetVkComputePipelineID (const Message< GpuMsg::GetVkComputePipelineID > &);
		bool _GetComputePipelineDescriptor (const Message< GpuMsg::GetComputePipelineDescriptor > &);
		bool _GetPipelineLayoutDescriptor (const Message< GpuMsg::GetPipelineLayoutDescriptor > &);
		bool _GetVkDescriptorLayouts (const Message< GpuMsg::GetVkDescriptorLayouts > &);
		bool _GetVkPipelineLayoutID (const Message< GpuMsg::GetVkPipelineLayoutID > &);

	private:
		bool _IsCreated () const;
		bool _CreatePipeline ();
		void _DestroyPipeline ();
	};
	

}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
