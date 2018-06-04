// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/OpenCL/120/CL1BaseModule.h"
#include "Engine/Platforms/OpenCL/120/CL1ResourceCache.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"

#ifdef COMPUTE_API_OPENCL

namespace Engine
{
namespace PlatformCL
{
	
/*
=================================================
	constructor
=================================================
*/
	CL1BaseModule::CL1BaseModule (const GlobalSystemsRef gs,
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
	bool CL1BaseModule::_OnManagerChanged (const Message< ModuleMsg::OnManagerChanged > &msg)
	{
		if ( msg->oldManager )
		{
			msg->oldManager->UnsubscribeAll( this );
			
			if ( _clResourceCache )
				_clResourceCache->Erase( this );
		}
		
		_clDevice			= null;
		_clResourceCache	= null;

		if ( msg->newManager )
		{
			msg->newManager->Subscribe( this, &CL1BaseModule::_DeviceBeforeDestroy );

			Message< GpuMsg::GetCLPrivateClasses >	req_classes;
			msg->newManager->Send( req_classes );

			_clDevice			= req_classes->result->device;
			_clResourceCache	= req_classes->result->resourceCache;
		}

		return true;
	}

/*
=================================================
	_DeviceBeforeDestroy
=================================================
*/
	bool CL1BaseModule::_DeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &)
	{
		_SendMsg< ModuleMsg::Delete >({});
		
		if ( _clResourceCache )
			_clResourceCache->Erase( this );

		_clDevice			= null;
		_clResourceCache	= null;
		return true;
	}
	
/*
=================================================
	_DeviceDeleted
=================================================
*/
	bool CL1BaseModule::_DeviceDeleted (const Message< ModuleMsg::Delete > &msg)
	{
		Send( msg );
		
		if ( _clResourceCache )
			_clResourceCache->Erase( this );

		_clDevice			= null;
		_clResourceCache	= null;
		return true;
	}

/*
=================================================
	_GetDeviceInfo
=================================================
*/
	bool CL1BaseModule::_GetDeviceInfo (const Message< GpuMsg::GetDeviceInfo > &msg)
	{
		return _GetManager() ? _GetManager()->Send( msg ) : false;
	}
	
/*
=================================================
	_GetCLDeviceInfo
=================================================
*/
	bool CL1BaseModule::_GetCLDeviceInfo (const Message< GpuMsg::GetCLDeviceInfo > &msg)
	{
		return _GetManager() ? _GetManager()->Send( msg ) : false;
	}
	
/*
=================================================
	_GetCLPrivateClasses
=================================================
*/
	bool CL1BaseModule::_GetCLPrivateClasses (const Message< GpuMsg::GetCLPrivateClasses > &msg)
	{
		return _GetManager() ? _GetManager()->Send( msg ) : false;
	}
	
/*
=================================================
	_GetGPUThread
=================================================
*/
	ModulePtr CL1BaseModule::_GetGPUThread (const ModulePtr &thread)
	{
		using CThreadMsgList_t		= MessageListFrom< GpuMsg::SubmitComputeQueueCommands, GpuMsg::GetCLDeviceInfo, GpuMsg::GetCLPrivateClasses >;
		using CThreadEventMsgList_t	= MessageListFrom< GpuMsg::DeviceBeforeDestroy, ModuleMsg::Delete >;

		ModulePtr	result = thread;

		if ( not result )
			result = GlobalSystems()->parallelThread->GetModuleByMsgEvent< CThreadMsgList_t, CThreadEventMsgList_t >();

		return result;
	}

}	// PlatformCL
}	// Engine

#endif	// COMPUTE_API_OPENCL
