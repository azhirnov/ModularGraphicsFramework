// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_OPENGL

#include "Engine/Platforms/Public/GPU/Thread.h"
#include "Engine/Platforms/OpenGL/Impl/GL4Device.h"
#include "Engine/Platforms/OpenGL/Impl/GL4Messages.h"

namespace Engine
{
namespace GpuMsg
{
	//
	// Get Private Classes
	//
	struct GetGLPrivateClasses
	{
		struct Classes {
			PlatformGL::GL4Device *		device	= null;
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
		Ptr< GL4Device >		_glDevice;


	// methods
	protected:
		GL4BaseModule (const GlobalSystemsRef gs,
					   const ModuleConfig &config,
					   const TypeIdList *msgTypes,
					   const TypeIdList *eventTypes);
		
		ModulePtr _GetGPUThread (const ModulePtr &);

		Ptr< GL4Device >	GetDevice ()	const	{ return _glDevice; }


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
