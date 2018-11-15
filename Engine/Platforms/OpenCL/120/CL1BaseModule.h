// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "Engine/Platforms/OpenCL/120/CL1Device.h"
#include "Engine/Platforms/OpenCL/120/CL1Messages.h"
#include "Engine/Platforms/Public/GPU/Thread.h"

namespace Engine
{
namespace PlatformCL
{
	class CL1SamplerCache;
	class CL1ResourceCache;

}	// PlatformCL

namespace GpuMsg
{
	//
	// Get Private Classes
	//
	struct GetCLPrivateClasses : _MsgBase_
	{
		struct Classes {
			PlatformCL::CL1Device *			device			= null;
			PlatformCL::CL1SamplerCache *	samplerCache	= null;
			PlatformCL::CL1ResourceCache*	resourceCache	= null;

			Classes (PlatformCL::CL1Device *dev, PlatformCL::CL1SamplerCache *sampCache, PlatformCL::CL1ResourceCache* resCache) :
				device{dev}, samplerCache{sampCache}, resourceCache{resCache}
			{}
		};

		Out< Classes >		result;
	};
}	// GpuMsg

namespace PlatformCL
{

	//
	// OpenCL Base Module
	//

	class CL1BaseModule : public Module
	{
	// types
	protected:
		using SupportedMessages_t	= MessageListFrom<
											ModuleMsg::OnManagerChanged,
											GpuMsg::GetDeviceInfo,
											GpuMsg::GetCLDeviceInfo,
											GpuMsg::GetCLPrivateClasses
										>;

		using SupportedEvents_t		= MessageListFrom<
											ModuleMsg::Compose,
											ModuleMsg::Delete,
											ModuleMsg::OnModuleAttached,
											ModuleMsg::OnModuleDetached
										>;

	// variables
	private:
		Ptr< CL1Device >			_clDevice;
		Ptr< CL1ResourceCache >		_clResourceCache;


	// methods
	protected:
		CL1BaseModule (const GlobalSystemsRef gs,
					   const ModuleConfig &config,
					   const TypeIdList *eventTypes);

		ND_ Ptr< CL1Device >		GetDevice ()		const	{ return _clDevice; }
		ND_ cl::cl_device_id		GetCLDevice ()		const	{ return _clDevice ? _clDevice->GetDevice() : null; }
		ND_ cl::cl_context			GetContext ()		const	{ return _clDevice ? _clDevice->GetContext() : null; }
		ND_ cl::cl_command_queue	GetCommandQueue ()	const	{ return _clDevice ? _clDevice->GetCommandQueue() : null; }
		ND_ Ptr< CL1ResourceCache >	GetResourceCache ()	const	{ return _clResourceCache; }
		
		ND_ ModulePtr _GetGPUThread (const ModulePtr &);


	// message handlers
	protected:
		bool _OnManagerChanged (const ModuleMsg::OnManagerChanged &);
		bool _GetDeviceInfo (const GpuMsg::GetDeviceInfo &);
		bool _GetCLDeviceInfo (const GpuMsg::GetCLDeviceInfo &);
		bool _GetCLPrivateClasses (const GpuMsg::GetCLPrivateClasses &);
		

	// event handlers
	private:
		bool _DeviceBeforeDestroy (const GpuMsg::DeviceBeforeDestroy &);
		bool _DeviceDeleted (const ModuleMsg::Delete &);
	};


}	// PlatformCL
}	// Engine

#endif	// COMPUTE_API_OPENCL
