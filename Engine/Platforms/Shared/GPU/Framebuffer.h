// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/ImageEnums.h"
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
		struct AttachmentInfo : CompileTime::PODStruct
		{
		// variables
			StaticString<64>		name;
			EImage::type			imageType;
			ERenderTarget::type		target;

		// methods
			AttachmentInfo () : imageType(Uninitialized), target(Uninitialized)
			{}

			AttachmentInfo (StringCRef name, EImage::type imageType, ERenderTarget::type target) :
				name(name), imageType(imageType), target(target)
			{}

			bool IsEnabled () const		{ return imageType != EImage::Unknown and target != ERenderTarget::Unknown; }
		};

		using Attachments_t = FixedSizeArray< AttachmentInfo, GlobalConst::Graphics_MaxColorBuffers >;


	// variables
		Attachments_t	colorAttachments;
		AttachmentInfo	depthStencilAttachment;

		uint2			size;
		uint			layers;


	// methods
		FramebufferDescriptor (GX_DEFCTOR) : size(), layers(0)
		{}

		explicit FramebufferDescriptor (uint2 size, uint layers = 1) : size(size), layers(layers)
		{}
	};

}	// Platforms


namespace CreateInfo
{
	//
	// GPU Framebuffer Create Info
	//

	struct GpuFramebuffer
	{
		ModulePtr							gpuThread;
		Platforms::FramebufferDescriptor	descr;
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

	
	// platform-dependent
	struct GetVkFramebufferID;
	struct GetGLFramebufferID;


	//
	// Is GPU Framebuffer Compatible With Render Pass
	//
	/*struct IsFramebufferCompatibleWithRenderPass
	{
		ModulePtr		renderPass;
	};*/


}	// GpuMsg
}	// Engine
