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
	// OpenGL Graphics Pipeline
	//

	class GL4GraphicsPipeline final : public GL4BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetGraphicsPipelineDescriptor
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;

		using Descriptor			= GraphicsPipelineDescriptor;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		Descriptor		_descr;

		//ModulePtr		_shaders;
		//ModulePtr		_renderPass;

		gl::GLuint		_programPipelineID;


	// methods
	public:
		GL4GraphicsPipeline (const GlobalSystemsRef gs, const CreateInfo::GraphicsPipeline &ci);
		~GL4GraphicsPipeline ();


	//protected:
		Descriptor const&		GetDescriptor ()	const	{ return _descr; }
		//gl::GLuint				GetPipelineID ()	const	{ return _programPipelineID; }


	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		//bool _GetGLGraphicsPipelineID (const Message< GpuMsg::GetGLGraphicsPipelineID > &);
		bool _GetGraphicsPipelineDescriptor (const Message< GpuMsg::GetGraphicsPipelineDescriptor > &);

	private:
		bool _IsCreated () const;
		bool _CreatePipeline ();
		void _DestroyPipeline ();
	};
	

}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
