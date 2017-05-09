// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/TextureEnums.h"

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
			ETexture::type			imageType;
			ERenderTarget::type		target;

		// methods
			AttachmentInfo () : imageType(Uninitialized), target(Uninitialized)
			{}

			AttachmentInfo (ETexture::type imageType, ERenderTarget::type target) : imageType(imageType), target(target)
			{}

			bool IsEnabled () const		{ return imageType != ETexture::Unknown and target != ERenderTarget::Unknown; }
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


namespace ModuleMsg
{
	
	//
	// Get GPU Framebuffer Descriptor
	//
	struct GetGpuFramebufferDescriptor
	{
		Out< Platforms::FramebufferDescriptor >		result;
	};

	
	// platform-dependent
	struct GetGpuFramebufferID;


	//
	// Is GPU Framebuffer Compatible With Render Pass
	//
	/*struct IsFramebufferCompatibleWithRenderPass
	{
		ModulePtr		renderPass;
	};*/


}	// ModuleMsg
}	// Engine
