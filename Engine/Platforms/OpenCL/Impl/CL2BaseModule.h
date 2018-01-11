// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/OpenCL/Impl/CL2Device.h"
#include "Engine/Platforms/OpenCL/Impl/CL2Messages.h"
#include "Engine/Platforms/Shared/GPU/Thread.h"

#if defined( COMPUTE_API_OPENCL )

namespace Engine
{
namespace GpuMsg
{
	//
	// Get Private Classes
	//
	struct GetCLPrivateClasses
	{
		struct Classes {
			PlatformCL::CL2Device *		device	= null;
		};

		Out< Classes >		result;
	};
}	// GpuMsg

namespace PlatformCL
{

	//
	// OpenCL Base Module
	//

	class CL2BaseModule : public Module
	{
	// types
	protected:
		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Update
										> >
										::Append< MessageListFrom<
											ModuleMsg::OnManagerChanged,
											GpuMsg::GetDeviceInfo,
											GpuMsg::GetCLDeviceInfo,
											GpuMsg::GetCLPrivateClasses
										> >;

		using SupportedEvents_t		= MessageListFrom<
											ModuleMsg::Compose,
											ModuleMsg::Delete,
											ModuleMsg::OnModuleAttached,
											ModuleMsg::OnModuleDetached,
											GpuMsg::DeviceBeforeDestroy
										>;

	// variables
	private:
		Ptr< CL2Device >	_clDevice;


	// methods
	public:
		CL2BaseModule (const GlobalSystemsRef gs,
					   const ModuleConfig &config,
					   const TypeIdList *msgTypes,
					   const TypeIdList *eventTypes);

	protected:
		Ptr< CL2Device >		GetDevice ()		const	{ return _clDevice; }
		cl::cl_device_id		GetCLDevice ()		const	{ return _clDevice ? _clDevice->GetDevice() : null; }
		cl::cl_context			GetContext ()		const	{ return _clDevice ? _clDevice->GetContext() : null; }
		cl::cl_command_queue	GetCommandQueue ()	const	{ return _clDevice ? _clDevice->GetCommandQueue() : null; }


	// message handlers
	protected:
		bool _OnManagerChanged (const Message< ModuleMsg::OnManagerChanged > &);
		bool _DeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &);
		bool _GetDeviceInfo (const Message< GpuMsg::GetDeviceInfo > &);
		bool _GetCLDeviceInfo (const Message< GpuMsg::GetCLDeviceInfo > &);
		bool _GetCLPrivateClasses (const Message< GpuMsg::GetCLPrivateClasses > &);
	};


}	// PlatformCL
}	// Engine

#endif	// COMPUTE_API_OPENCL
