// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/Context.h"
#include "Engine/Platforms/OpenGL/OpenGLThread.h"

#if defined( GRAPHICS_API_OPENGL )

namespace Engine
{
namespace Platforms
{

	//
	// OpenGL Context
	//
	
	class OpenGLContext final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t;

		using GLThreads_t			= Set< ModulePtr >;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;

		
	// variables
	private:
		CreateInfo::GpuContext	_createInfo;

		GLThreads_t				_threads;
		

	// methods
	public:
		OpenGLContext (const GlobalSystemsRef gs, const CreateInfo::GpuContext &ci);
		~OpenGLContext ();
		
		static GModID::type		GetStaticID ()			{ return "gl4.ctx"_GModID; }
		
		static void Register (GlobalSystemsRef);
		static void Unregister (GlobalSystemsRef);

		
	// message handlers
	private:
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);
		
	private:
		static ModulePtr _CreateOpenGLThread (GlobalSystemsRef, const CreateInfo::GpuThread &);
		static ModulePtr _CreateOpenGLContext (GlobalSystemsRef, const CreateInfo::GpuContext &);
	};


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
