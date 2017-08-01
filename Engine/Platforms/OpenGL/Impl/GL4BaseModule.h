// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/OpenGL/Impl/GL4Enums.h"
#include "Engine/Platforms/OpenGL/Impl/GL4Messages.h"
#include "Engine/Platforms/Shared/GPU/Thread.h"

#if defined( GRAPHICS_API_OPENGL )

namespace Engine
{
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
											ModuleMsg::GpuDeviceBeforeDestory
										> >;

		using SupportedEvents_t		= MessageListFrom<
											ModuleMsg::Compose,
											ModuleMsg::Delete
										>;


	// variables
	private:
		const GLSystemsRef		_glSystems;
		

	// methods
	public:
		GL4BaseModule (const GlobalSystemsRef gs,
					   const ModuleConfig &config,
					   const Runtime::VirtualTypeList *msgTypes,
					   const Runtime::VirtualTypeList *eventTypes);

		GL4BaseModule (const GlobalSystemsRef gs,
					   const ModulePtr &gpuThread,
					   const ModuleConfig &config,
					   const Runtime::VirtualTypeList *msgTypes,
					   const Runtime::VirtualTypeList *eventTypes);

		GL4BaseModule (const GlobalSystemsRef gs,
					   const GLSystemsRef glSys,
					   const ModuleConfig &config,
					   const Runtime::VirtualTypeList *msgTypes,
					   const Runtime::VirtualTypeList *eventTypes);

		GLSystemsRef	GLSystems ()	const	{ return _glSystems; }


	protected:
		Ptr< OpenGLThread >		GetGpuThread ()			{ return GLSystems()->Get< OpenGLThread >(); }
		Ptr< GL4Device >		GetDevice ()			{ return GLSystems()->Get< GL4Device >(); }
		
		virtual void _DestroyResources ();


	// message handlers
	protected:
		bool _OnManagerChanged (const Message< ModuleMsg::OnManagerChanged > &);
		bool _GpuDeviceBeforeDestory (const Message< ModuleMsg::GpuDeviceBeforeDestory > &);
	};


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
