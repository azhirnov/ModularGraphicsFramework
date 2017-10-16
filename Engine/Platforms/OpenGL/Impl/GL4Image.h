// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/OpenGL/Impl/GL4BaseModule.h"
#include "Engine/Platforms/Shared/GPU/Image.h"

#if defined( GRAPHICS_API_OPENGL )

namespace Engine
{
namespace PlatformGL
{

	//
	// OpenGL Texture
	//

	class GL4Image final : public GL4BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= GL4BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetImageDescriptor,
											GpuMsg::GetGLImageID
										> >;

		using SupportedEvents_t		= GL4BaseModule::SupportedEvents_t;

		using Utils					= Platforms::ImageUtils;

		
	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		ImageDescriptor		_descr;
		gl::GLuint			_imageId;
		

	// methods
	public:
		GL4Image (const GlobalSystemsRef gs, const CreateInfo::GpuImage &ci);
		~GL4Image ();


	//protected:
		ImageDescriptor const&	GetDescriptor ()	const	{ return _descr; }
		gl::GLuint				GetImageID ()		const	{ return _imageId; }


	// message handlers
	private:
		bool _Compose (const  Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetGLImageID (const Message< GpuMsg::GetGLImageID > &);
		bool _GetImageDescriptor (const Message< GpuMsg::GetImageDescriptor > &);

	private:
		bool _IsCreated () const;

		bool _CreateImage ();
		void _DestroyImage ();
	};


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
