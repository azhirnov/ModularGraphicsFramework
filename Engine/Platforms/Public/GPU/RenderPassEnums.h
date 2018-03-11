// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/Common.h"

namespace Engine
{
namespace Platforms
{
	
	struct EAttachmentLoadOp
	{
		enum type : uint
		{
			Invalidate = 0,
			Load,
			Clear,

			_Count,
			Unknown	= ~0u,
		};
	};


	struct EAttachmentStoreOp
	{
		enum type : uint
		{
			Invalidate = 0,
			Store,

			_Count,
			Unknown	= ~0u,
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
			Unknown = ~0u,
		};

		GX_ENUM_BITFIELD( EPipelineStage );

		static constexpr bits	AllGraphics =	bits() | TopOfPipe | DrawIndirect | VertexInput | VertexShader | TessControlShader |
												TessEvaluationShader | GeometryShader | FragmentShader | EarlyFragmentTests |
												LateFragmentTests | ColorAttachmentOutput | BottomOfPipe;

		static constexpr bits	AllCommands	=	bits().SetAll();

		static StringCRef	ToString (type value);
		static String		ToString (bits values);
	};


	struct EPipelineAccess
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
			Unknown = ~0u,
		};

		GX_ENUM_BITFIELD( EPipelineAccess );

		static constexpr bits	ReadMask	= bits() | IndirectCommandRead | IndexRead | VertexAttributeRead | UniformRead | 
											  InputAttachmentRead | ShaderRead | ColorAttachmentRead | DepthStencilAttachmentRead |
											  TransferRead | HostRead | MemoryRead;

		static constexpr bits	WriteMask	= bits() | ShaderWrite | ColorAttachmentWrite | DepthStencilAttachmentWrite |
											  TransferWrite | HostWrite | MemoryWrite;
		
		static StringCRef	ToString (type value);
		static String		ToString (bits values);
	};


	struct ESubpassDependency
	{
		enum type : uint
		{
			ByRegion	= 0,
			ViewLocal,
			DeviceGroup,

			_Count,
			Unknown = ~0u,
		};

		GX_ENUM_BITFIELD( ESubpassDependency );
	};
	

}	// Platforms
}	// Engine
