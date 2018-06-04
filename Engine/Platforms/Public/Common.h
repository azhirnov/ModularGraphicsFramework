// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Engine.Base.h"

#include "Engine/STL/Math/Mathematics.h"
#include "Engine/STL/Math/Vec.h"
#include "Engine/STL/Math/Matrix.h"
#include "Engine/STL/Math/2D/Rectangle.h"
#include "Engine/STL/Math/Color/Color.h"
#include "Engine/STL/Math/MathTypeCast.h"

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
