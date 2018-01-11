// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/OpenGL/Impl/GL4BaseModule.h"

#if defined( GRAPHICS_API_OPENGL )

namespace Engine
{
namespace PlatformGL
{

/*
=================================================
	constructor
=================================================
*/
	GL4BaseModule::GL4BaseModule (const GlobalSystemsRef gs,
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
	bool GL4BaseModule::_OnManagerChanged (const Message< ModuleMsg::OnManagerChanged > &msg)
	{
		_glDevice = null;

		if ( msg->newManager )
		{
			msg->newManager->Subscribe( this, &GL4BaseModule::_DeviceBeforeDestroy );

			Message< GpuMsg::GetGLPrivateClasses >	req_dev;
			msg->newManager->Send( req_dev );

			_glDevice = req_dev->result->device;
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
	bool GL4BaseModule::_DeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &)
	{
		_SendMsg< ModuleMsg::Delete >({});

		_glDevice = null;
		return true;
	}
	
/*
=================================================
	_GetDeviceInfo
=================================================
*/
	bool GL4BaseModule::_GetDeviceInfo (const Message< GpuMsg::GetDeviceInfo > &msg)
	{
		return _GetManager() ? _GetManager()->Send( msg ) : false;
	}
	
/*
=================================================
	_GetGLDeviceInfo
=================================================
*/
	bool GL4BaseModule::_GetGLDeviceInfo (const Message< GpuMsg::GetGLDeviceInfo > &msg)
	{
		return _GetManager() ? _GetManager()->Send( msg ) : false;
	}
	
/*
=================================================
	_GetGLPrivateClasses
=================================================
*/
	bool GL4BaseModule::_GetGLPrivateClasses (const Message< GpuMsg::GetGLPrivateClasses > &msg)
	{
		return _GetManager() ? _GetManager()->Send( msg ) : false;
	}

}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
