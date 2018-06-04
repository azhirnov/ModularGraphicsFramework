// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Config/Engine.Config.h"

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
	struct GetGLPrivateClasses
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
		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Update
										> >
										::Append< MessageListFrom<
											ModuleMsg::OnManagerChanged,
											GpuMsg::GetDeviceInfo,
											GpuMsg::GetGLDeviceInfo,
											GpuMsg::GetGLPrivateClasses
										> >;

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
					   const TypeIdList *msgTypes,
					   const TypeIdList *eventTypes);
		
		ND_ ModulePtr _GetGPUThread (const ModulePtr &);

		ND_ Ptr< GL4Device >		GetDevice ()		const	{ return _glDevice; }
		ND_ Ptr< GL4ResourceCache >	GetResourceCache ()	const	{ return _glResourceCache; }


	// message handlers
	protected:
		bool _OnManagerChanged (const Message< ModuleMsg::OnManagerChanged > &);
		bool _GetDeviceInfo (const Message< GpuMsg::GetDeviceInfo > &);
		bool _GetGLDeviceInfo (const Message< GpuMsg::GetGLDeviceInfo > &);
		bool _GetGLPrivateClasses (const Message< GpuMsg::GetGLPrivateClasses > &);
		

	// event handlers
	private:
		bool _DeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &);
		bool _DeviceDeleted (const Message< ModuleMsg::Delete > &);
	};


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
