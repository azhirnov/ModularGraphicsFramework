// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_OPENGL

#include "Engine/Platforms/Public/GPU/Thread.h"
#include "Engine/Platforms/OpenGL/450/GL4Device.h"
#include "Engine/Platforms/OpenGL/450/GL4Messages.h"

namespace Engine
{
namespace PlatformGL
{
	class GL4SamplerCache;
	class GL4ResourceCache;

}	// PlatformGL

namespace GpuMsg
{
	//
	// Get Private Classes
	//
	struct GetGLPrivateClasses : _MsgBase_
	{
		struct Classes {
			PlatformGL::GL4Device *			device			= null;
			PlatformGL::GL4SamplerCache *	samplerCache	= null;
			PlatformGL::GL4ResourceCache *	resourceCache	= null;

			Classes (PlatformGL::GL4Device *dev, PlatformGL::GL4SamplerCache *sampCache, PlatformGL::GL4ResourceCache *resCache) :
				device{dev}, samplerCache{sampCache}, resourceCache{resCache} {}
		};

		Out< Classes >		result;
	};
}	// GpuMsg

namespace PlatformGL
{

	//
	// OpenGL Base Module
	//

	class GL4BaseModule : public Module
	{
	// types
	protected:
		using SupportedMessages_t	= MessageListFrom<
											ModuleMsg::OnManagerChanged,
											GpuMsg::GetDeviceInfo,
											GpuMsg::GetGLDeviceInfo,
											GpuMsg::GetGLPrivateClasses
										>;

		using SupportedEvents_t		= MessageListFrom<
											ModuleMsg::Link,
											ModuleMsg::AfterLink,
											ModuleMsg::Compose,
											ModuleMsg::AfterCompose,
											ModuleMsg::Delete,
											ModuleMsg::OnModuleAttached,
											ModuleMsg::OnModuleDetached
										>;

	// variables
	private:
		Ptr< GL4Device >			_glDevice;
		Ptr< GL4ResourceCache >		_glResourceCache;


	// methods
	protected:
		GL4BaseModule (const GlobalSystemsRef gs,
					   const ModuleConfig &config,
					   const TypeIdList *eventTypes);
		
		ND_ ModulePtr _GetGPUThread (const ModulePtr &);

		ND_ Ptr< GL4Device >		GetDevice ()		const	{ return _glDevice; }
		ND_ Ptr< GL4ResourceCache >	GetResourceCache ()	const	{ return _glResourceCache; }


	// message handlers
	protected:
		bool _OnManagerChanged (const ModuleMsg::OnManagerChanged &);
		bool _GetDeviceInfo (const GpuMsg::GetDeviceInfo &);
		bool _GetGLDeviceInfo (const GpuMsg::GetGLDeviceInfo &);
		bool _GetGLPrivateClasses (const GpuMsg::GetGLPrivateClasses &);
		

	// event handlers
	private:
		bool _DeviceBeforeDestroy (const GpuMsg::DeviceBeforeDestroy &);
		bool _DeviceDeleted (const ModuleMsg::Delete &);
	};


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
