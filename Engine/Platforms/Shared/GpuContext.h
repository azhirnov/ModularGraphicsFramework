// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/Platforms/Common/GraphicsEnums.h"

namespace Engine
{
namespace CreateInfo
{
	DECL_STRING_TO_ID( GAPI, 10 );


	//
	// GPU Context Create Info
	//

	struct GpuContext
	{
	// types
		struct EFlags
		{
			enum type : uint
			{
				DebugContext,
				NoErrorContext,
				Stereo,
				VSync,
				_Count
			};

			using bits = EnumBitfield< EFlags >;
		};

		using EPixelFormat	= Platforms::EPixelFormat;


	// variables
		String				device;
		GAPI::type			version;
		EFlags::bits		flags;

		EPixelFormat::type	colorFmt;
		EPixelFormat::type	depthStencilFmt;

		//Multisamples		samples;


	// methods
		GpuContext (GX_DEFCTOR) :
			version( "GL 4.5"_GAPI ),
			colorFmt( EPixelFormat::RGBA8_UNorm ),
			depthStencilFmt( EPixelFormat::Depth24 )
		{}
	};

}	// CreateInfo


namespace ModuleMsg
{


}	// ModuleMsg
}	// Engine
