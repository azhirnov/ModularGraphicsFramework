// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Soft/Impl/SWBaseModule.h"

#ifdef GRAPHICS_API_SOFT

namespace Engine
{
namespace PlatformSW
{

/*
=================================================
	constructor
=================================================
*/
	SWBaseModule::SWBaseModule   (const GlobalSystemsRef gs,
								  const ModuleConfig &config,
								  const TypeIdList *msgTypes,
								  const TypeIdList *eventTypes) :
		Module( gs, config, msgTypes, eventTypes )
	{
	}
	
/*
=================================================
	_OnManagerChanged
=================================================
*/
	bool SWBaseModule::_OnManagerChanged (const Message< ModuleMsg::OnManagerChanged > &msg)
	{
		//_swDevice = null;

		if ( msg->newManager )
		{
			msg->newManager->Subscribe( this, &SWBaseModule::_DeviceBeforeDestroy );
			msg->newManager->Subscribe( this, &SWBaseModule::_DeviceDeleted );

			Message< GpuMsg::GetSWPrivateClasses >	req_dev;
			CHECK( msg->newManager->Send( req_dev ) );

			//_swDevice = req_dev->result->device;
		}

		if ( msg->oldManager )
			msg->oldManager->UnsubscribeAll( this );

		return true;
	}

/*
=================================================
	_DeviceBeforeDestroy
=================================================
*/
	bool SWBaseModule::_DeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &)
	{
		_SendMsg< ModuleMsg::Delete >({});

		//_swDevice = null;
		return true;
	}
	
/*
=================================================
	_DeviceDeleted
=================================================
*/
	bool SWBaseModule::_DeviceDeleted (const Message< ModuleMsg::Delete > &msg)
	{
		Send( msg );

		//_swDevice = null;
		return true;
	}

/*
=================================================
	_GetDeviceInfo
=================================================
*/
	bool SWBaseModule::_GetDeviceInfo (const Message< GpuMsg::GetDeviceInfo > &msg)
	{
		return _GetManager() ? _GetManager()->Send( msg ) : false;
	}
	
/*
=================================================
	_GetSWDeviceInfo
=================================================
*/
	bool SWBaseModule::_GetSWDeviceInfo (const Message< GpuMsg::GetSWDeviceInfo > &msg)
	{
		return _GetManager() ? _GetManager()->Send( msg ) : false;
	}
	
/*
=================================================
	_GetSWPrivateClasses
=================================================
*/
	bool SWBaseModule::_GetSWPrivateClasses (const Message< GpuMsg::GetSWPrivateClasses > &msg)
	{
		return _GetManager() ? _GetManager()->Send( msg ) : false;
	}
	
/*
=================================================
	_GetGPUThread
=================================================
*/
	ModulePtr SWBaseModule::_GetGPUThread (const ModulePtr &thread)
	{
		using GThreadMsgList_t		= MessageListFrom< GpuMsg::ThreadBeginFrame, GpuMsg::ThreadEndFrame, GpuMsg::GetSWPrivateClasses >;
		using GThreadEventMsgList_t	= MessageListFrom< GpuMsg::DeviceBeforeDestroy, ModuleMsg::Delete >;

		ModulePtr	result = thread;
		
		if ( not result )
			result = GlobalSystems()->parallelThread->GetModuleByID( SWThreadModuleID );

		if ( not result )
			result = GlobalSystems()->parallelThread->GetModuleByMsgEvent< GThreadMsgList_t, GThreadEventMsgList_t >();

		return result;
	}

}	// PlatformSW
}	// Engine

#endif	// GRAPHICS_API_SOFT
