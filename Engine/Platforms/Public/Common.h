// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Engine.Base.h"

#include "Core/STL/Math/Mathematics.h"
#include "Core/STL/Math/Vec.h"
#include "Core/STL/Math/Matrix.h"
#include "Core/STL/Math/2D/Rectangle.h"
#include "Core/STL/Math/Color/Color.h"
#include "Core/STL/Math/MathTypeCast.h"

namespace Engine
{
namespace Platforms
{
	using namespace Engine::Base;
	using namespace GX_STL::GXMath;
	
	void RegisterPlatforms ();
	void UnregisterPlatforms ();

}	// Platforms

namespace GpuMsg {
	using namespace Engine::_BaseMessages_;
}

namespace OSMsg {
	using namespace Engine::_BaseMessages_;
}

namespace CreateInfo
{}

}	// Engine
