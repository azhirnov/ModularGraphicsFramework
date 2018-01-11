// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Engine.Platforms.h"

namespace Engine
{
namespace Scene
{
	using namespace Platforms;
	
	
	void RegisterScene ();
	void UnregisterScene ();

}	// Scene


namespace SceneMsg
{
	using namespace ModuleMsg;

}	// SceneMsg
}	// Engine
