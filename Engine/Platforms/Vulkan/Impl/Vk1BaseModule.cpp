// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{

/*
=================================================
	constructor
=================================================
*/
	Vk1BaseModule::Vk1BaseModule (const GlobalSystemsRef gs,
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
	bool Vk1BaseModule::_OnManagerChanged (const Message< ModuleMsg::OnManagerChanged > &msg)
	{
		_vkDevice = null;

		if ( msg->newManager )
		{
			msg->newManager->Subscribe( this, &Vk1BaseModule::_DeviceBeforeDestroy );
			msg->newManager->Subscribe( this, &Vk1BaseModule::_DeviceDeleted );

			Message< GpuMsg::GetVkPrivateClasses >	req_dev;
			CHECK( msg->newManager->Send( req_dev ) );

			_vkDevice = req_dev->result->device;
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
	bool Vk1BaseModule::_DeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &)
	{
		_SendMsg< ModuleMsg::Delete >({});

		_vkDevice = null;
		return true;
	}
	
/*
=================================================
	_DeviceDeleted
=================================================
*/
	bool Vk1BaseModule::_DeviceDeleted (const Message< ModuleMsg::Delete > &msg)
	{
		Send( msg );

		_vkDevice = null;
		return true;
	}

/*
=================================================
	_GetDeviceInfo
=================================================
*/
	bool Vk1BaseModule::_GetDeviceInfo (const Message< GpuMsg::GetDeviceInfo > &msg)
	{
		return _GetManager() ? _GetManager()->Send( msg ) : false;
	}
	
/*
=================================================
	_GetVkDeviceInfo
=================================================
*/
	bool Vk1BaseModule::_GetVkDeviceInfo (const Message< GpuMsg::GetVkDeviceInfo > &msg)
	{
		return _GetManager() ? _GetManager()->Send( msg ) : false;
	}
	
/*
=================================================
	_GetVkPrivateClasses
=================================================
*/
	bool Vk1BaseModule::_GetVkPrivateClasses (const Message< GpuMsg::GetVkPrivateClasses > &msg)
	{
		return _GetManager() ? _GetManager()->Send( msg ) : false;
	}
	
/*
=================================================
	_GetGPUThread
=================================================
*/
	ModulePtr Vk1BaseModule::_GetGPUThread (const ModulePtr &thread)
	{
		using GThreadMsgList_t		= MessageListFrom< GpuMsg::ThreadBeginFrame, GpuMsg::ThreadEndFrame, GpuMsg::GetVkPrivateClasses >;
		using GThreadEventMsgList_t	= MessageListFrom< GpuMsg::DeviceBeforeDestroy, ModuleMsg::Delete >;

		ModulePtr	result = thread;
		
		if ( not result )
			result = GlobalSystems()->parallelThread->GetModuleByID( VkThreadModuleID );

		if ( not result )
			result = GlobalSystems()->parallelThread->GetModuleByMsgEvent< GThreadMsgList_t, GThreadEventMsgList_t >();

		return result;
	}

}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
