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
	SWShaderHelper::SWShaderHelper ()
	{}

/*
=================================================
	Reset
=================================================
*/
	void SWShaderHelper::Reset ()
	{
		_resourceTable			= null;

		_vertexShaderState		= VertexShader();
		_geometryShaderState	= GeometryShader();
		_fragmentShaderState	= FragmentShader();
		_computeShaderState		= ComputeShader();
	}


}	// Impl
}	// SWShaderLang

#endif	// GRAPHICS_API_SOFT
