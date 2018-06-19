// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Scene/Impl/BaseSceneModule.h"

namespace Engine
{
namespace Scene
{

/*
=================================================
	constructor
=================================================
*/
	BaseSceneModule::BaseSceneModule (const GlobalSystemsRef gs,
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
	bool BaseSceneModule::_OnManagerChanged (const ModuleMsg::OnManagerChanged &msg)
	{
		//_vkDevice = null;

		if ( msg.newManager )
		{
			//msg.newManager->Subscribe( this, &BaseSceneModule::_DeviceBeforeDestroy );

			SceneMsg::GetScenePrivateClasses	req_dev;
			msg.newManager->Send( req_dev );

			//_vkDevice = req_dev->result->device;
		}
		
		if ( msg.oldManager )
			msg.oldManager->UnsubscribeAll( this );

		return true;
	}
	
/*
=================================================
	_DeviceBeforeDestroy
=================================================
*
	bool BaseSceneModule::_DeviceBeforeDestroy (const GpuMsg::DeviceBeforeDestroy &)
	{
		_SendMsg( ModuleMsg::Delete{} );

		//_vkDevice = null;
		return true;
	}
	
/*
=================================================
	_GetScenePrivateClasses
=================================================
*/
	bool BaseSceneModule::_GetScenePrivateClasses (const SceneMsg::GetScenePrivateClasses &msg)
	{
		return _GetManager() ? _GetManager()->Send( msg ) : false;
	}


}	// Scene
}	// Engine
