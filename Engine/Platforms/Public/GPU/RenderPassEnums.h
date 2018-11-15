// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/Common.h"
#include "Engine/Platforms/Public/GPU/ShaderEnums.h"

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

		static StringCRef ToString (type value);
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
		
		static StringCRef ToString (type value);
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

		static constexpr bits	AllShaders	=	bits() | VertexShader | TessControlShader | TessEvaluationShader |
												GeometryShader | FragmentShader | ComputeShader;

		static constexpr bits	AllCommands	=	bits().SetAll();

		static constexpr bool	IsGraphics (type value);

		static bits				From (EShader::bits shaderStages);

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
		
		static constexpr bits	ColorAttachment			= bits() | ColorAttachmentRead | ColorAttachmentWrite;
		static constexpr bits	DepthStencilAttachment	= bits() | DepthStencilAttachmentRead | DepthStencilAttachmentWrite;
		static constexpr bits	Transfer				= bits() | TransferRead | TransferWrite;
		
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

		static StringCRef ToString (type value);
		static String     ToString (bits values);
	};
	

	
//-----------------------------------------------------------------------------//
// EPipelineStage
	
	ND_ inline constexpr bool  EPipelineStage::IsGraphics (type value)
	{
		return value >= TopOfPipe and value <= BottomOfPipe;
	}
	
		
	ND_ inline EPipelineStage::bits  EPipelineStage::From (EShader::bits shaderStages)
	{
		bits	result;
		if ( shaderStages[EShader::Vertex] )			result |= VertexShader;
		if ( shaderStages[EShader::TessControl] )		result |= TessControlShader;
		if ( shaderStages[EShader::TessEvaluation] )	result |= TessEvaluationShader;
		if ( shaderStages[EShader::Geometry] )			result |= GeometryShader;
		if ( shaderStages[EShader::Fragment] )			result |= FragmentShader;
		return result;
	}


}	// Platforms
}	// Engine
