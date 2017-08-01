// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"

namespace Engine
{
namespace Platforms
{
	
	struct EShader
	{
		enum type : uint
		{
			Vertex		= 0,
			TessControl,
			TessEvaluation,
			Geometry,
			Fragment,
			Compute,

			_Count,
			Unknown		= uint(-1),
		};

		using bits = EnumBitfield< EShader >;

		static StringCRef ToString (type value);
	};



//-----------------------------------------------------------------------------//
// EShader
	
	inline StringCRef EShader::ToString (type value)
	{
		switch ( value )
		{
			case EShader::Vertex			:	return "Vertex";
			case EShader::TessControl		:	return "TessellationControl";
			case EShader::TessEvaluation	:	return "TessellationEvaluation";
			case EShader::Geometry			:	return "Geometry";
			case EShader::Fragment			:	return "Fragment";
			case EShader::Compute			:	return "Compute";
		}
		RETURN_ERR( "invalid shader type", "unknown" );
	}


}	// Platforms
}	// Engine
