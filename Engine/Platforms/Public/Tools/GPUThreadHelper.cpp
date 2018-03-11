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
		using MsgList_t		= CompileTime::TypeListFrom<
									Message<GpuMsg::ThreadBeginFrame>,
									Message<GpuMsg::ThreadEndFrame>,
									Message<GpuMsg::SubmitGraphicsQueueCommands>,
									Message<GpuMsg::GetDeviceInfo>,
									Message<GpuMsg::GetGraphicsModules>,
									Message<GpuMsg::GetGraphicsSettings>
								>;
		using EventList_t	= CompileTime::TypeListFrom<
									Message<GpuMsg::DeviceCreated>,
									Message<GpuMsg::DeviceBeforeDestroy>
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
		using MsgList_t		= CompileTime::TypeListFrom<
									Message<GpuMsg::SubmitComputeQueueCommands>,
									Message<GpuMsg::GetDeviceInfo>,
									Message<GpuMsg::GetGraphicsModules>,
									Message<GpuMsg::GetComputeSettings>
								>;
		using EventList_t	= CompileTime::TypeListFrom<
									Message<GpuMsg::DeviceCreated>,
									Message<GpuMsg::DeviceBeforeDestroy>
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
		using MsgList_t		= CompileTime::TypeListFrom<
									Message<GpuMsg::ThreadBeginVRFrame>,
									Message<GpuMsg::ThreadEndVRFrame>,
									Message<GpuMsg::SubmitGraphicsQueueCommands>,
									Message<GpuMsg::GetVRDeviceInfo>,
									Message<GpuMsg::GetGraphicsModules>,
									Message<GpuMsg::GetGraphicsSettings>
								>;
		using EventList_t	= CompileTime::TypeListFrom<
									Message<GpuMsg::DeviceCreated>,
									Message<GpuMsg::DeviceBeforeDestroy>
								>;

		return gs->parallelThread->GetModuleByMsgEvent< MsgList_t, EventList_t >();
	}


}	// PlatformTools
}	// Engine
