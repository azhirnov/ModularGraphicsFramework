// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/Image.h"

namespace Engine
{
namespace Platforms
{

	//
	// GPU Framebuffer Description
	//

	struct FramebufferDescription : CompileTime::PODStruct
	{
	// types
		using Name_t	= StaticString<64>;
		using Self		= FramebufferDescription;

		struct AttachmentInfo : CompileTime::PODStruct
		{
		// variables
			Name_t			name;
			EImage::type	imageType;
			bool			isRenderbuffer;		// for OpenGL, DirectX

		// methods
			AttachmentInfo () : imageType(Uninitialized), isRenderbuffer(false)
			{}

			AttachmentInfo (StringCRef name, EImage::type imageType, bool isRB = false) :
				name(name), imageType(imageType), isRenderbuffer(isRB)
			{}

			bool IsEnabled () const		{ return imageType != EImage::Unknown; }
		};

		using Attachments_t = FixedSizeArray< AttachmentInfo, GlobalConst::GAPI_MaxColorBuffers >;


	// variables
		Attachments_t	colorAttachments;
		AttachmentInfo	depthStencilAttachment;

		uint2			size;
		uint			layers;
		MultiSamples	samples;


	// methods
		FramebufferDescription (GX_DEFCTOR) : size(), layers(0)
		{}

		explicit FramebufferDescription (uint2 size, uint layers = 1, MultiSamples samp = MultiSamples()) : size(size), layers(layers), samples(samp)
		{}

		Self&	ColorImage (StringCRef name, EImage::type imageType = EImage::Tex2D);
		Self&	ColorRenderbuffer (StringCRef name, EImage::type imageType = EImage::Tex2D);

		Self&	DepthStencilImage (StringCRef name, EImage::type imageType = EImage::Tex2D);
		Self&	DepthStencilRenderbuffer (StringCRef name, EImage::type imageType = EImage::Tex2D);
	};
	

	inline FramebufferDescription&  FramebufferDescription::ColorImage (StringCRef name, EImage::type imageType) {
		colorAttachments.PushBack({ name, imageType, false });
		return *this;
	}

	inline FramebufferDescription&  FramebufferDescription::ColorRenderbuffer (StringCRef name, EImage::type imageType) {
		colorAttachments.PushBack({ name, imageType, true });
		return *this;
	}

	inline FramebufferDescription&  FramebufferDescription::DepthStencilImage (StringCRef name, EImage::type imageType) {
		depthStencilAttachment = { name, imageType, false };
		return *this;
	}

	inline FramebufferDescription&  FramebufferDescription::DepthStencilRenderbuffer (StringCRef name, EImage::type imageType) {
		depthStencilAttachment = { name, imageType, true };
		return *this;
	}

}	// Platforms


namespace CreateInfo
{
	//
	// GPU Framebuffer Create Info
	//

	struct GpuFramebuffer
	{
	// variables
		ModulePtr			gpuThread;			// can be null
		GXMath::uint2		size;
		uint				layers;

	// methods
		GpuFramebuffer () {}
		explicit GpuFramebuffer (const GXMath::uint2 &size, uint layers = 1) : size{size}, layers{layers} {}
	};

}	// CreateInfo


namespace GpuMsg
{
	
	//
	// Get Framebuffer Description
	//
	struct GetFramebufferDescription : _MessageBase_
	{
		Out< Platforms::FramebufferDescription >		result;
	};


	//
	// Attach Image toframebuffer (use it instead of AttachModule)
	//
	struct FramebufferAttachImage : _MessageBase_
	{
	// types
		using ImageLayer		= Platforms::ImageLayer;
		using MipmapLevel		= Platforms::MipmapLevel;
		using EImage			= Platforms::EImage;
		using EPixelFormat		= Platforms::EPixelFormat;
		using ImageViewDescr	= Platforms::ImageViewDescription;

	// variables
		ModuleName_t		name;
		ModulePtr			image;
		ImageViewDescr		viewDescr;

	// methods
		FramebufferAttachImage (GX_DEFCTOR) {}

		FramebufferAttachImage (StringCRef name, const ModulePtr &mod, const ImageViewDescr &descr) : name(name), image(mod), viewDescr(descr) {}

		FramebufferAttachImage (StringCRef name, const ModulePtr &mod, MipmapLevel level = Uninitialized, ImageLayer layer = Uninitialized, uint layerCount = 1,
								EImage::type viewType = Uninitialized, EPixelFormat::type format = Uninitialized) :
			name{name}, image{mod}, viewDescr{ viewType, format, level, 0, layer, layerCount }
		{}
	};


}	// GpuMsg
}	// Engine
