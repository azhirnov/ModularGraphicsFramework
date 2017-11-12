// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/OpenCL/Impl/CL2BaseModule.h"

#if defined( COMPUTE_API_OPENCL )

namespace Engine
{
namespace PlatformCL
{
	
/*
=================================================
	constructor
=================================================
*/
	CL2BaseModule::CL2BaseModule (const GlobalSystemsRef gs,
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
	bool CL2BaseModule::_OnManagerChanged (const Message< ModuleMsg::OnManagerChanged > &msg)
	{
		_clDevice = null;

		if ( msg->newManager )
		{
			msg->newManager->Subscribe( this, &CL2BaseModule::_DeviceBeforeDestroy );

			Message< GpuMsg::GetCLPrivateClasses >	req_dev;
			msg->newManager->Send( req_dev );

			_clDevice = req_dev->result->device;
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
	bool CL2BaseModule::_DeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &)
	{
		_SendMsg< ModuleMsg::Delete >({});

		_clDevice = null;
		return true;
	}
	
/*
=================================================
	_GetDeviceInfo
=================================================
*/
	bool CL2BaseModule::_GetDeviceInfo (const Message< GpuMsg::GetDeviceInfo > &msg)
	{
		return _GetManager() ? _GetManager()->Send( msg ) : false;
	}
	
/*
=================================================
	_GetCLDeviceInfo
=================================================
*/
	bool CL2BaseModule::_GetCLDeviceInfo (const Message< GpuMsg::GetCLDeviceInfo > &msg)
	{
		return _GetManager() ? _GetManager()->Send( msg ) : false;
	}
	
/*
=================================================
	_GetCLPrivateClasses
=================================================
*/
	bool CL2BaseModule::_GetCLPrivateClasses (const Message< GpuMsg::GetCLPrivateClasses > &msg)
	{
		return _GetManager() ? _GetManager()->Send( msg ) : false;
	}

}	// PlatformCL
}	// Engine

#endif	// COMPUTE_API_OPENCL
