// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"

namespace Engine
{
namespace Platforms
{
	
	struct ETexture
	{
		enum type : uint
		{
			Tex1D		= 0,
			Tex1DArray,
			Tex2D,
			Tex2DArray,
			Tex2DMS,
			Tex2DMSArray,
			TexCube,
			TexCubeArray,
			Tex3D,
			Buffer,

			_Count,
			Unknown		= uint(-1),
		};

		static bool IsMultisampled (type value);
	};


	struct EImageUsage
	{
		enum type : uint
		{
			TransferSrc = 0,			// for all copy operations
			TransferDst,				// for all copy operations
			Sampled,					// access in shader as texture
			Storage,					// access in shader as image
			ColorAttachment,
			DepthStencilAttachment,
			TransientAttachment,
			InputAttachment,

			_Count,
			Unknown = uint(-1),
		};

		using bits = EnumBitfield< EImageUsage >;
	};
	


//-----------------------------------------------------------------------------//
// ETexture
	
	inline bool ETexture::IsMultisampled (type value)
	{
		switch ( value )
		{
			case ETexture::Tex2DMS :
			case ETexture::Tex2DMSArray :
				return true;
		}
		return false;
	}


}	// Platforms
}	// Engine
