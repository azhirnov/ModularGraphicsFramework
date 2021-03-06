// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/Common.h"

namespace Engine
{
namespace Platforms
{
	
	struct EBufferUsage
	{
		enum type : uint
		{						//	cpu read  |  cpu write  |  shader read  |  shader write  |
			TransferSrc = 0,	//            |             |      no       |      no        |
			TransferDst,		//            |             |      no       |      no        |  
			UniformTexel,		//            |             |      yes      |      no        |
			StorageTexel,		//            |             |      yes      |      no        |
			Uniform,			//            |             |      fast     |      no        |
			Storage,			//            |             |      yes      |      yes       |
			Index,				//            |             |      no       |      no        |
			Vertex,				//            |             |      no       |      no        |
			Indirect,			//            |             |      no       |      no        |

			_Count,
			Unknown		= ~0u,
		};

		GX_ENUM_BITFIELD( EBufferUsage );

		static constexpr bits	All			= bits().SetAll();
		static constexpr bits	Transfer	= bits() | TransferDst | TransferSrc;

		static String  ToString (bits values);
	};


	struct ESparseMemory
	{
		enum type : uint
		{
			None = 0,
			Binding,
			Residency,
			Aliased,

			_Count,
			Unknown		= ~0u,
		};

		GX_ENUM_BITFIELD( ESparseMemory );
	};


}	// Platforms
}	// Engine
