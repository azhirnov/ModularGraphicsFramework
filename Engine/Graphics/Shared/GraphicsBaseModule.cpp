// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Graphics/Shared/GraphicsBaseModule.h"

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
	bool GraphicsBaseModule::_DeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &msg)
	{
		_SendMsg< ModuleMsg::Delete >({});
		return true;
	}

}	// Graphics
}	// Engine
