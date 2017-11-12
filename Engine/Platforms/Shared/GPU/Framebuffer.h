// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/ImageEnums.h"
#include "Engine/Platforms/Shared/GPU/ImgArrLayer.h"
#include "Engine/Platforms/Shared/GPU/MipmapLevel.h"
#include "Engine/Platforms/Shared/GPU/MultiSamples.h"
#include "Engine/Platforms/Shared/GPU/IDs.h"

namespace Engine
{
namespace Platforms
{

	//
	// GPU Framebuffer Descriptor
	//

	struct FramebufferDescriptor : CompileTime::PODStruct
	{
	// types
		using Name_t	= StaticString<64>;
		using Self		= FramebufferDescriptor;

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

		using Attachments_t = FixedSizeArray< AttachmentInfo, GlobalConst::Graphics_MaxColorBuffers >;


	// variables
		Attachments_t	colorAttachments;
		AttachmentInfo	depthStencilAttachment;

		uint2			size;
		uint			layers;
		MultiSamples	samples;


	// methods
		FramebufferDescriptor (GX_DEFCTOR) : size(), layers(0)
		{}

		explicit FramebufferDescriptor (uint2 size, uint layers = 1, MultiSamples samp = MultiSamples()) : size(size), layers(layers), samples(samp)
		{}

		Self&	ColorImage (StringCRef name, EImage::type imageType = EImage::Tex2D);
		Self&	ColorRenderbuffer (StringCRef name, EImage::type imageType = EImage::Tex2D);

		Self&	DepthStencilImage (StringCRef name, EImage::type imageType = EImage::Tex2D);
		Self&	DepthStencilRenderbuffer (StringCRef name, EImage::type imageType = EImage::Tex2D);
	};
	

	inline FramebufferDescriptor&  FramebufferDescriptor::ColorImage (StringCRef name, EImage::type imageType) {
		colorAttachments.PushBack({ name, imageType, false });
		return *this;
	}

	inline FramebufferDescriptor&  FramebufferDescriptor::ColorRenderbuffer (StringCRef name, EImage::type imageType) {
		colorAttachments.PushBack({ name, imageType, true });
		return *this;
	}

	inline FramebufferDescriptor&  FramebufferDescriptor::DepthStencilImage (StringCRef name, EImage::type imageType) {
		depthStencilAttachment = { name, imageType, false };
		return *this;
	}

	inline FramebufferDescriptor&  FramebufferDescriptor::DepthStencilRenderbuffer (StringCRef name, EImage::type imageType) {
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
	// types
		using ImgArrLayer	= Platforms::ImgArrLayer;
		using MipmapLevel	= Platforms::MipmapLevel;

		struct Attachment final : CompileTime::FastCopyable
		{
		// variables
			ModuleMsg::ModuleName_t	name;
			ModulePtr				module;
			MipmapLevel				mipmap;
			ImgArrLayer				layer;		// one layer or first layer for layered framebuffer

		// methods
			Attachment () {}
			Attachment (StringCRef name, const ModulePtr &mod, MipmapLevel lvl = Uninitialized, ImgArrLayer layer = Uninitialized) : name(name), module(mod), mipmap(lvl), layer(layer) {}
		};
		using Attachments_t = FixedSizeArray< Attachment, GlobalConst::Graphics_MaxColorBuffers >;

	// vaariables
		ModulePtr			gpuThread;
		GXMath::uint2		size;
		uint				layers;
		Attachments_t		attachment;
	};

}	// CreateInfo


namespace GpuMsg
{
	
	//
	// Get Framebuffer Descriptor
	//
	struct GetFramebufferDescriptor
	{
		Out< Platforms::FramebufferDescriptor >		result;
	};


}	// GpuMsg
}	// Engine
