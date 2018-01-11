// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Engine.Platforms.h"

namespace Engine
{
namespace Graphics
{
	using namespace Platforms;
	
	void RegisterGraphics ();
	void UnregisterGraphics ();

}	// Graphics

namespace GraphicsMsg
{
	using namespace ModuleMsg;

}	// GraphicsMsg
}	// Engine
