// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Scene/Public/IDs.h"
#include "Engine/Platforms/Public/GPU/VR.h"

namespace Engine
{
namespace Scene
{


}	// Scene


namespace CreateInfo
{
	
	//
	// Surface Create Info
	//
	struct RenderSurface
	{
		ModulePtr	scene;
	};

}	// CreateInfo


namespace SceneMsg
{
	
	//
	// Surface Resized Event
	//
	struct SurfaceOnResize
	{
		GXMath::uint2	newSize;
	};


	//
	// Get Surface Descriptor
	//
	struct SurfaceGetDescriptor
	{
	// types
		using EPixelFormat = Platforms::EPixelFormat;

		struct Descr
		{
			GXMath::uint2		size;
			EPixelFormat::type	colorFmt;
			EPixelFormat::type	depthFmt;
		};

	// variables
		Out< Descr >	result;
	};


	//
	// Surface Request Update
	//
	struct SurfaceRequestUpdate
	{
	// types
		using PerEye			= GpuMsg::ThreadBeginVRFrame::PerEye;
		using Framebuffers_t	= FixedSizeArray< PerEye, 2 >;		// 1 for screen/texture, 2 for VR (left, right)


	// variables
		Framebuffers_t		framebuffers;

		ModulePtr			cmdBuilder;
	};


}	// SceneMsg
}	// Engine