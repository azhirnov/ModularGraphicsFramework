// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/Common.h"

namespace Engine
{
namespace Platforms
{

	struct EQuery
	{
		enum type : uint
		{
			Unknown	= 0,
			Occlusion,
			PipelineStatistic,
			Timestamp,
		};
	};


	struct EPipelineStatistic
	{
		enum type : uint
		{
			InputAssemblyVertices,
			TessellationControllShaderPatches,

			// primitives
			InputAssembluPrimitives,
			GeometryShaderPrimitives,
			ClippingPrimitives,
			
			// invocations
			VertexShaderInvocations,
			GeometryShaderInvocations,
			ClippingInvocations,
			FragmentShaderInvocations,
			TessellationEvaluationShaderInvocations,
			ComputeShaderInvocations,

			_Count
		};

		GX_ENUM_BITFIELD( EPipelineStatistic );
	};

}	// Platforms
}	// Engine
