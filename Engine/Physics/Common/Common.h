// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#define GX_PHYSICS_DIMENSIONS_ENABLED

#include "Engine/STL/CompileTime/StaticFractional.h"
#include "Engine/STL/CompileTime/StaticFloat.h"
#include "Engine/STL/CompileTime/StaticInteger.h"
#include "Engine/STL/CompileTime/TypeList.h"
#include "Engine/STL/Math/Mathematics.h"
#include "Engine/STL/Math/Vec.h"
#include "Engine/STL/Math/BinaryMath.h"
#include "Engine/STL/Math/Interpolations.h"

namespace GXPhysics
{
	using namespace GX_STL;
	using namespace GX_STL::GXTypes;
	using namespace GX_STL::GXMath;


#	define PX_AUTO( _type_ )	auto

}	// GXPhysics
