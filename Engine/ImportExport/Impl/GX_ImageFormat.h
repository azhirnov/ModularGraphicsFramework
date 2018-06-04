// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/PixelFormatEnums.h"

namespace Engine
{
namespace ImportExport
{
	using namespace Engine::Platforms;


	//
	// GXImage Format
	//
	struct GXImageFormat final : Noninstancable
	{
	// types
		struct EFlags
		{
			enum type : uint {
				None		= 0,
				CubeMap		= 1 << 0,

				Unknown		= None,
			};
			GX_ENUM_BIT_OPERATIONS( type );
		};

		static constexpr ulong	VERSION		= "gximage-1"_StringToID;


		struct Header : CompileTime::PODType
		{
			ulong				version		= 0;
			uint				width		= 0;
			uint				height		= 0;
			uint				depth		= 0;
			uint				layers		= 0;
			uint				maxLevel	= 0;
			uint				samples		= 0;
			EFlags::type		flags		= Uninitialized;
			EPixelFormat::type	pixelFormat;
			Bytes<uint>			blockSize;
			Bytes<uint>			levelsSize;	// size of array of Level
		};
		STATIC_ASSERT( sizeof(Header) == 48 );
		

		struct Level : CompileTime::PODType
		{
			uint			memOffset	= 0;
			uint3			dimension;
			uint			layer		= 0;
			uint			level		= 0;
			Bytes<uint>		rowPitch;		// for 2D, 3D
			Bytes<uint>		slicePitch;		// for 3D
		};
		STATIC_ASSERT( sizeof(Level) == 32 );
	};

}	// ImportExport
}	// Engine
