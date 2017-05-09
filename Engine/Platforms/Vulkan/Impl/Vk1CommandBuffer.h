// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Shared/GPU/CommandBuffer.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	
	//
	// Vulkan Command Buffer
	//

	class Vk1CommandBuffer final : public Vk1BaseModule
	{
		friend class Vk1CommandBufferBuilder;

	// types
	private:
		SHARED_POINTER( Vk1BaseModule );
		SHARED_POINTER( Vk1CommandBufferBuilder );

		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::GetGpuCommandBufferDescriptor,
											ModuleMsg::GetGpuCommandBufferID
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;
		
		using UsedResources			= Set< Vk1BaseModulePtr >;
		using BuilderPtr			= Vk1CommandBufferBuilderPtr;

		struct ERecordingState
		{
			enum type : uint
			{
				Initial,
				Recording,
				Executable,
				Pending,
				Deleted,
			};
		};


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		CommandBufferDescriptor		_descr;
		BuilderPtr					_builder;

		vk::VkCommandBuffer			_cmdId;
		
		UsedResources				_resources;
		ERecordingState::type		_recordingState;


	// methods
	public:
		Vk1CommandBuffer (const GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci);
		~Vk1CommandBuffer ();

		static OModID::type				GetStaticID ()				{ return "vk1.cmdbuf"_OModID; }


	//protected:
		CommandBufferDescriptor const&	GetDescriptor ()	const	{ return _descr; }
		vk::VkCommandBuffer				GetCmdBufferID ()	const	{ return _cmdId; }
		BuilderPtr const&				GetBuilder ()		const	{ return _builder; }


	// message handlers
	private:
		bool _Compose (const  Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetGpuCommandBufferID (const Message< ModuleMsg::GetGpuCommandBufferID > &);
		bool _GetGpuCommandBufferDescriptor (const Message< ModuleMsg::GetGpuCommandBufferDescriptor > &);

	private:
		bool _IsCreated () const;
		bool _CreateCmdBuffer ();
		bool _ResetCmdBuffer ();
		void _DestroyCmdBuffer ();
		
		bool _Submit ();
		bool _BeginRecording ();
		bool _EndRecording ();
	};


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
