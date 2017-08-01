// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Shaders/Shader/AllNodeFuncs.h"

namespace ShaderEditor
{
namespace ShaderNodes
{
	
/*
=================================================
	VecCtor
=================================================
*/
	template <typename T, typename ...Args>
	inline T  VecCtor (const Args& ...args)
	{
		return _VecConstructors<T>::Ctor( args... );
	}


}	// ShaderNodes
}	// ShaderEditor
