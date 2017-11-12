// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/Context.h"
#include "Engine/Platforms/Shared/GPU/Thread.h"
#include "Engine/Platforms/Shared/GPU/Pipeline.h"
#include "Engine/Platforms/OpenCL/Impl/CL2Messages.h"

#if defined( COMPUTE_API_OPENCL )

namespace Engine
{
namespace Platforms
{

	//
	// OpenCL Context
	//
	
	class OpenCLContext final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t;

		using CLThreads_t			= Set< ModulePtr >;
		
		using CLThreadMsgList_t		= MessageListFrom< GpuMsg::SubmitComputeQueueCommands, GpuMsg::GetCLDeviceInfo >;
		using CLThreadEventList_t	= MessageListFrom< GpuMsg::DeviceCreated, GpuMsg::DeviceBeforeDestroy >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;

		
	// variables
	private:
		CreateInfo::GpuContext	_createInfo;

		CLThreads_t				_threads;
		

	// methods
	public:
		OpenCLContext (GlobalSystemsRef gs, const CreateInfo::GpuContext &ci);
		~OpenCLContext ();
		
		static _ENGINE_PLATFORMS_EXPORT_ void Register (GlobalSystemsRef);
		static _ENGINE_PLATFORMS_EXPORT_ void Unregister (GlobalSystemsRef);

		
	// message handlers
	private:
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);
		
	private:
		static ModulePtr _CreateOpenCLThread (GlobalSystemsRef, const CreateInfo::GpuThread &);
		static ModulePtr _CreateOpenCLContext (GlobalSystemsRef, const CreateInfo::GpuContext &);
		static ModulePtr _CreateCL2Image (GlobalSystemsRef, const CreateInfo::GpuImage &);
		static ModulePtr _CreateCL2Memory (GlobalSystemsRef, const CreateInfo::GpuMemory &);
		static ModulePtr _CreateCL2Buffer (GlobalSystemsRef, const CreateInfo::GpuBuffer &);
		static ModulePtr _CreateCL2Sampler (GlobalSystemsRef, const CreateInfo::GpuSampler &);
		static ModulePtr _CreatePipelineTemplate (GlobalSystemsRef, const CreateInfo::PipelineTemplate &);
		static ModulePtr _CreateCL2CommandBuffer (GlobalSystemsRef, const CreateInfo::GpuCommandBuffer &);
		static ModulePtr _CreateCL2CommandBuilder (GlobalSystemsRef, const CreateInfo::GpuCommandBuilder &);
		static ModulePtr _CreateCL2ComputePipeline (GlobalSystemsRef, const CreateInfo::ComputePipeline &);
		static ModulePtr _CreateCL2PipelineResourceTable (GlobalSystemsRef, const CreateInfo::PipelineResourceTable &);
	};


}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
