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
		using SupportedMessages_t	= GL4BaseModule::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::GetGpuTextureDescriptor,
											ModuleMsg::GetGLTextureID
										> >;

		using SupportedEvents_t		= GL4BaseModule::SupportedEvents_t;

		using Utils					= TextureUtils;

		
	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		TextureDescriptor		_descr;
		gl::GLuint				_imageId;
		

	// methods
	public:
		GL4Texture (const GlobalSystemsRef gs, const CreateInfo::GpuTexture &ci);
		~GL4Texture ();

		static OModID::type		GetStaticID ()			{ return "gl4.texture"_OModID; }


	//protected:
		TextureDescriptor const&	GetDescriptor ()	const	{ return _descr; }
		gl::GLuint					GetImageID ()		const	{ return _imageId; }


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const  Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetGLTextureID (const Message< ModuleMsg::GetGLTextureID > &);
		bool _GetGpuTextureDescriptor (const Message< ModuleMsg::GetGpuTextureDescriptor > &);

	private:
		bool _IsCreated () const;

		bool _CreateTexture ();
		void _DestroyTexture ();
	};


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
