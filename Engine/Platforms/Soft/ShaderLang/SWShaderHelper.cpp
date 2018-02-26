// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Soft/ShaderLang/SWShaderHelper.h"

#ifdef GRAPHICS_API_SOFT

namespace SWShaderLang
{
namespace Impl
{

/*
=================================================
	constructor
=================================================
*/
	SWShaderHelper::SWShaderHelper () :
		_stage{ EShader::Unknown }
	{}

/*
=================================================
	_Reset
=================================================
*/
	void SWShaderHelper::_Reset ()
	{
		_vertexShaderState		= VertexShader();
		_geometryShaderState	= GeometryShader();
		_fragmentShaderState	= FragmentShader();
		_computeShaderState		= ComputeShader();
		_stage					= EShader::Unknown;
	}


}	// Impl
}	// SWShaderLang

#endif	// GRAPHICS_API_SOFT
