// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/OpenGL/450/GL4BaseModule.h"
#include "Engine/Platforms/OpenGL/450/GL4ResourceCache.h"
#include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"

#ifdef GRAPHICS_API_OPENGL

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
	bool GL4BaseModule::_OnManagerChanged (const ModuleMsg::OnManagerChanged &msg)
	{
		if ( msg.oldManager )
		{
			msg.oldManager->UnsubscribeAll( this );
			
			if ( _glResourceCache )
				_glResourceCache->Erase( this );
		}

		_glDevice			= null;
		_glResourceCache	= null;

		if ( msg.newManager )
		{
			msg.newManager->Subscribe( this, &GL4BaseModule::_DeviceBeforeDestroy );
			msg.newManager->Subscribe( this, &GL4BaseModule::_DeviceDeleted );

			GpuMsg::GetGLPrivateClasses		req_classes;
			CHECK( msg.newManager->Send( req_classes ) );

			_glDevice			= req_classes.result->device;
			_glResourceCache	= req_classes.result->resourceCache;
		}

		return true;
	}

/*
=================================================
	_DeviceBeforeDestroy
=================================================
*/
	bool GL4BaseModule::_DeviceBeforeDestroy (const GpuMsg::DeviceBeforeDestroy &)
	{
		_SendMsg( ModuleMsg::Delete{} );
		
		if ( _glResourceCache )
			_glResourceCache->Erase( this );

		_glDevice			= null;
		_glResourceCache	= null;
		return true;
	}
	
/*
=================================================
	_DeviceDeleted
=================================================
*/
	bool GL4BaseModule::_DeviceDeleted (const ModuleMsg::Delete &msg)
	{
		Send( msg );
		
		if ( _glResourceCache )
			_glResourceCache->Erase( this );

		_glDevice			= null;
		_glResourceCache	= null;
		return true;
	}

/*
=================================================
	_GetDeviceInfo
=================================================
*/
	bool GL4BaseModule::_GetDeviceInfo (const GpuMsg::GetDeviceInfo &msg)
	{
		return _GetManager() ? _GetManager()->Send( msg ) : false;
	}
	
/*
=================================================
	_GetGLDeviceInfo
=================================================
*/
	bool GL4BaseModule::_GetGLDeviceInfo (const GpuMsg::GetGLDeviceInfo &msg)
	{
		return _GetManager() ? _GetManager()->Send( msg ) : false;
	}
	
/*
=================================================
	_GetGLPrivateClasses
=================================================
*/
	bool GL4BaseModule::_GetGLPrivateClasses (const GpuMsg::GetGLPrivateClasses &msg)
	{
		return _GetManager() ? _GetManager()->Send( msg ) : false;
	}
	
/*
=================================================
	_GetGPUThread
=================================================
*/
	ModulePtr GL4BaseModule::_GetGPUThread (const ModulePtr &thread)
	{
		using GThreadMsgList_t		= MessageListFrom< GpuMsg::ThreadBeginFrame, GpuMsg::ThreadEndFrame, GpuMsg::GetGLDeviceInfo, GpuMsg::GetGLPrivateClasses >;
		using GThreadEventMsgList_t	= MessageListFrom< GpuMsg::DeviceBeforeDestroy, ModuleMsg::Delete >;

		ModulePtr	result = thread;

		if ( not result )
			result = GlobalSystems()->parallelThread->GetModuleByMsgEvent< GThreadMsgList_t, GThreadEventMsgList_t >();

		return result;
	}

}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
