// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"

namespace Engine
{
namespace Platforms
{
	
	struct EAttachmentLoadOp
	{
		enum type : uint
		{
			None = 0,
			Load,
			Clear,

			_Count,
			Unknown	= uint(-1),
		};
	};


	struct EAttachmentStoreOp
	{
		enum type : uint
		{
			None = 0,
			Store,

			_Count,
			Unknown	= uint(-1),
		};
	};

	
	struct EPipelineStage
	{
		enum type : uint
		{
			TopOfPipe = 0,
			DrawIndirect,
			VertexInput,
			VertexShader,
			TessControlShader,
			TessEvaluationShader,
			GeometryShader,
			FragmentShader,
			EarlyFragmentTests,
			LateFragmentTests,
			ColorAttachmentOutput,
			ComputeShader,
			Transfer,
			BottomOfPipe,
			Host,

			_Count,
			Unknown = uint(-1),
		};

		using bits = EnumBitfield< EPipelineStage >;
	};


	struct ESubpassAccess
	{
		enum type : uint
		{
			IndirectCommandRead = 0,
			IndexRead,
			VertexAttributeRead,
			UniformRead,
			InputAttachmentRead,
			ShaderRead,
			ShaderWrite,
			ColorAttachmentRead,
			ColorAttachmentWrite,
			DepthStencilAttachmentRead,
			DepthStencilAttachmentWrite,
			TransferRead,
			TransferWrite,
			HostRead,
			HostWrite,
			MemoryRead,
			MemoryWrite,
			//CommandProcessRead,
			//CommandProcessWrite,

			_Count,
			Unknown = uint(-1),
		};

		using bits = EnumBitfield< ESubpassAccess >;
	};


	struct ESubpassDependency
	{
		enum type : uint
		{
			ByRegion	= 0,
			ViewLocal,
			DeviceGroup,

			_Count,
			Unknown = uint(-1),
		};

		using bits = EnumBitfield< ESubpassDependency >;
	};

}	// Platforms
}	// Engine
