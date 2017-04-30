// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/OpenGL/Impl/GL4BaseModule.h"
#include "Engine/Platforms/Shared/GPU/Texture.h"

#if defined( GRAPHICS_API_OPENGL )

namespace Engine
{
namespace PlatformGL
{

	//
	// OpenGL Texture
	//

	class GL4Texture final : public GL4BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t;
		using SupportedEvents_t		= Module::SupportedEvents_t;

		
	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		TextureDescriptor		_descr;
		gl::GLuint				_texID;
		

	// methods
	public:
		GL4Texture (const GlobalSystemsRef gs, const CreateInfo::GpuTexture &ci);
		~GL4Texture ();

		static OModID::type		GetStaticID ()			{ return "gl4.texture"_OModID; }
	};


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
