// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"

namespace Engine
{
namespace Platforms
{
	
	struct EBufferUsage
	{
		enum type : uint
		{
			TransferSrc = 0,	// in shader: read-only				// CopyReadBuffer in GL
			TransferDst,		// in shader: write-only			// CopyWriteBuffer in GL
			UniformTexel,		// in shader: read-only				// TextureBuffer in GL
			StorageTexel,		// in shader: read-write
			Uniform,			// in shader: read-only, fast		// UniformBuffer in GL
			Storage,			// in shader: read-write			// ShaderStorageBuffer in GL
			Index,				// in shader: read-only				// ElementArrayBuffer in GL
			Vertex,				// in shader: read-only				// ArrayBuffer in GL
			Indirect,			// in shader: read-only				// DrawIndirect or DispatchIndirect in GL

			_Count,
			Unknown		= uint(-1),
		};

		using bits	= EnumBitfield< EBufferUsage >;
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
			Unknown		= uint(-1),
		};

		using bits	= EnumBitfield< ESparseMemory >;
	};


}	// Platforms
}	// Engine
