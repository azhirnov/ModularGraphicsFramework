// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Public/Tools/GPUThreadHelper.h"

namespace Engine
{
namespace PlatformTools
{

/*
=================================================
	FindGraphicsThread
=================================================
*/
	ModulePtr  GPUThreadHelper::FindGraphicsThread (GlobalSystemsRef gs)
	{
		using MsgList_t		= ModuleMsg::MessageListFrom<
									GpuMsg::ThreadBeginFrame,
									GpuMsg::ThreadEndFrame,
									GpuMsg::SubmitCommands,
									GpuMsg::GetDeviceInfo,
									GpuMsg::GetGraphicsModules,
									GpuMsg::GetGraphicsSettings
								>;
		using EventList_t	= ModuleMsg::MessageListFrom<
									GpuMsg::DeviceCreated,
									GpuMsg::DeviceBeforeDestroy
								>;

		return gs->parallelThread->GetModuleByMsgEvent< MsgList_t, EventList_t >();
	}
	
/*
=================================================
	FindComputeThread
=================================================
*/
	ModulePtr  GPUThreadHelper::FindComputeThread (GlobalSystemsRef gs)
	{
		using MsgList_t		= ModuleMsg::MessageListFrom<
									GpuMsg::SubmitCommands,
									GpuMsg::GetDeviceInfo,
									GpuMsg::GetGraphicsModules,
									GpuMsg::GetComputeSettings
								>;
		using EventList_t	= ModuleMsg::MessageListFrom<
									GpuMsg::DeviceCreated,
									GpuMsg::DeviceBeforeDestroy
								>;

		return gs->parallelThread->GetModuleByMsgEvent< MsgList_t, EventList_t >();
	}
	
/*
=================================================
	FindVRThread
=================================================
*/
	ModulePtr  GPUThreadHelper::FindVRThread (GlobalSystemsRef gs)
	{
		using MsgList_t		= ModuleMsg::MessageListFrom<
									GpuMsg::ThreadBeginVRFrame,
									GpuMsg::ThreadEndVRFrame,
									GpuMsg::SubmitCommands,
									GpuMsg::GetVRDeviceInfo,
									GpuMsg::GetGraphicsModules,
									GpuMsg::GetGraphicsSettings
								>;
		using EventList_t	= ModuleMsg::MessageListFrom<
									GpuMsg::DeviceCreated,
									GpuMsg::DeviceBeforeDestroy
								>;

		return gs->parallelThread->GetModuleByMsgEvent< MsgList_t, EventList_t >();
	}


}	// PlatformTools
}	// Engine
