// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Engine.Platforms.h"

namespace Engine
{
namespace Graphics
{
	using namespace Engine::Base;
	using namespace GX_STL::GXMath;
	using EPrimitive		= Engine::Platforms::EPrimitive;
	using VertexInputState	= Engine::Platforms::VertexInputState;
	

	void RegisterGraphics ();
	void UnregisterGraphics ();

}	// Graphics

namespace GraphicsMsg
{
	using namespace Engine::_BaseMessages_;

}	// GraphicsMsg
}	// Engine
