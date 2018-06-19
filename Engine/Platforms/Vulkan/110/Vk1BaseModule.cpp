// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/110/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/110/Vk1ResourceCache.h"
#include "Engine/Platforms/Vulkan/VulkanObjectsConstructor.h"

#ifdef GRAPHICS_API_VULKAN

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
	bool Vk1BaseModule::_OnManagerChanged (const ModuleMsg::OnManagerChanged &msg)
	{
		if ( msg.oldManager )
		{
			msg.oldManager->UnsubscribeAll( this );

			if ( _vkResourceCache )
				_vkResourceCache->Erase( this );
		}

		_vkDevice			= null;
		_vkResourceCache	= null;

		if ( msg.newManager )
		{
			msg.newManager->Subscribe( this, &Vk1BaseModule::_DeviceBeforeDestroy );
			msg.newManager->Subscribe( this, &Vk1BaseModule::_DeviceDeleted );

			GpuMsg::GetVkPrivateClasses		req_classes;
			CHECK( msg.newManager->Send( req_classes ) );

			_vkDevice			= req_classes.result->device;
			_vkResourceCache	= req_classes.result->resourceCache;
		}

		return true;
	}

/*
=================================================
	_DeviceBeforeDestroy
=================================================
*/
	bool Vk1BaseModule::_DeviceBeforeDestroy (const GpuMsg::DeviceBeforeDestroy &)
	{
		_SendMsg( ModuleMsg::Delete{} );
		
		if ( _vkResourceCache )
			_vkResourceCache->Erase( this );
		
		_vkDevice			= null;
		_vkResourceCache	= null;
		return true;
	}
	
/*
=================================================
	_DeviceDeleted
=================================================
*/
	bool Vk1BaseModule::_DeviceDeleted (const ModuleMsg::Delete &msg)
	{
		Send( msg );
		
		if ( _vkResourceCache )
			_vkResourceCache->Erase( this );
		
		_vkDevice			= null;
		_vkResourceCache	= null;
		return true;
	}

/*
=================================================
	_GetDeviceInfo
=================================================
*/
	bool Vk1BaseModule::_GetDeviceInfo (const GpuMsg::GetDeviceInfo &msg)
	{
		return _GetManager() ? _GetManager()->Send( msg ) : false;
	}
	
/*
=================================================
	_GetVkDeviceInfo
=================================================
*/
	bool Vk1BaseModule::_GetVkDeviceInfo (const GpuMsg::GetVkDeviceInfo &msg)
	{
		return _GetManager() ? _GetManager()->Send( msg ) : false;
	}
	
/*
=================================================
	_GetVkPrivateClasses
=================================================
*/
	bool Vk1BaseModule::_GetVkPrivateClasses (const GpuMsg::GetVkPrivateClasses &msg)
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
		using GThreadMsgList_t		= MessageListFrom< GpuMsg::ThreadBeginFrame, GpuMsg::ThreadEndFrame, GpuMsg::GetVkDeviceInfo, GpuMsg::GetVkPrivateClasses >;
		using GThreadEventMsgList_t	= MessageListFrom< GpuMsg::DeviceBeforeDestroy, ModuleMsg::Delete >;

		ModulePtr	result = thread;

		if ( not result )
			result = GlobalSystems()->parallelThread->GetModuleByMsgEvent< GThreadMsgList_t, GThreadEventMsgList_t >();

		return result;
	}

}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
