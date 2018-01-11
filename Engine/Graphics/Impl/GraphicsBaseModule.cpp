// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Graphics/Impl/GraphicsBaseModule.h"

namespace Engine
{
namespace Graphics
{

/*
=================================================
	constructor
=================================================
*/
	GraphicsBaseModule::GraphicsBaseModule (const GlobalSystemsRef gs,
											const ModuleConfig &config,
											const TypeIdList *msgTypes,
											const TypeIdList *eventTypes) :
		Module( gs, config, msgTypes, eventTypes )
	{}
	
/*
=================================================
	_OnManagerChanged
=================================================
*/
	bool GraphicsBaseModule::_OnManagerChanged (const Message< ModuleMsg::OnManagerChanged > &msg)
	{
		if ( msg->newManager )
			msg->newManager->Subscribe( this, &GraphicsBaseModule::_DeviceBeforeDestroy );
		
		if ( msg->oldManager )
			msg->oldManager->UnsubscribeAll( this );

		return true;
	}
	
/*
=================================================
	_DeviceBeforeDestroy
=================================================
*/
	bool GraphicsBaseModule::_DeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &)
	{
		_SendMsg< ModuleMsg::Delete >({});
		return true;
	}
	
/*
=================================================
	_GetGpuThread
=================================================
*/
	ModulePtr GraphicsBaseModule::_GetGpuThread (const ModulePtr &gpuThread) const
	{
		using GThreadMsgList_t		= CompileTime::TypeListFrom< Message<GpuMsg::ThreadBeginFrame>, Message<GpuMsg::ThreadEndFrame>, Message<GpuMsg::GetDeviceInfo> >;
		using GThreadEventList_t	= CompileTime::TypeListFrom< Message<GpuMsg::DeviceCreated>, Message<GpuMsg::DeviceBeforeDestroy> >;

		return gpuThread ? gpuThread : GlobalSystems()->parallelThread->GetModuleByMsgEvent< GThreadMsgList_t, GThreadEventList_t >();
	}

}	// Graphics
}	// Engine
