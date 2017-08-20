// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Shaders/Shader/VecTypes.h"
#include "Engine/Shaders/Shader/Generated/AllNodeFuncs.h"

namespace ShaderEditor
{
namespace ShaderNodes
{
	
	template <typename T, typename ...Args>
	inline T  NVecCtor (const Args& ...args)
	{
		return _VecConstructors< T >::Ctor( args... );
	}
	

}	// ShaderNodes
}	// ShaderEditor
