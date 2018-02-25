// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/Thread.h"
#include "Engine/Platforms/Soft/Impl/SWMessages.h"

#ifdef GRAPHICS_API_SOFT

namespace Engine
{
namespace GpuMsg
{
	//
	// Get Private Classes
	//
	struct GetSWPrivateClasses
	{
		struct Classes {
		};

		Out< Classes >		result;
	};
}	// GpuMsg

namespace PlatformSW
{

	//
	// Software Renderer Base Module
	//

	class SWBaseModule : public Module
	{
	// types
	protected:
		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Update
										> >
										::Append< MessageListFrom<
											ModuleMsg::OnManagerChanged,
											GpuMsg::GetDeviceInfo,
											GpuMsg::GetSWDeviceInfo,
											GpuMsg::GetSWPrivateClasses
										> >;

		using SupportedEvents_t		= MessageListFrom<
											ModuleMsg::Link,
											ModuleMsg::AfterLink,
											ModuleMsg::Compose,
											ModuleMsg::AfterCompose,
											ModuleMsg::Delete,
											ModuleMsg::OnModuleAttached,
											ModuleMsg::OnModuleDetached
										>;

	// variables
	private:
		//Ptr< SWDevice >		_swDevice;


	// methods
	protected:
		SWBaseModule  (const GlobalSystemsRef gs,
					   const ModuleConfig &config,
					   const TypeIdList *msgTypes,
					   const TypeIdList *eventTypes);

		//Ptr< SWDevice >	GetDevice ()	const	{ return _swDevice; }
		
		ModulePtr _GetGPUThread (const ModulePtr &);


	// message handlers
	protected:
		bool _OnManagerChanged (const Message< ModuleMsg::OnManagerChanged > &);
		bool _GetDeviceInfo (const Message< GpuMsg::GetDeviceInfo > &);
		bool _GetSWDeviceInfo (const Message< GpuMsg::GetSWDeviceInfo > &);
		bool _GetSWPrivateClasses (const Message< GpuMsg::GetSWPrivateClasses > &);
		

	// event handlers
	private:
		bool _DeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &);
		bool _DeviceDeleted (const Message< ModuleMsg::Delete > &);
	};


}	// PlatformSW
}	// Engine

#endif	// GRAPHICS_API_SOFT
