// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Engine.Platforms.h"
#include "Engine/Graphics/Engine.Graphics.h"

namespace Engine
{
namespace Scene
{
	using namespace Engine::Base;
	using namespace GX_STL::GXMath;

	//using namespace Engine::Platforms;
	//using namespace Engine::Graphics;
	
	
	void RegisterScene ();
	void UnregisterScene ();

}	// Scene


namespace SceneMsg
{
	using namespace ModuleMsg;

}	// SceneMsg
}	// Engine
