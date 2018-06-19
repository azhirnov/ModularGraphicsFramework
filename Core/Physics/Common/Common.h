// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#define GX_PHYSICS_DIMENSIONS_ENABLED

#include "Core/STL/CompileTime/StaticFractional.h"
#include "Core/STL/CompileTime/StaticFloat.h"
#include "Core/STL/CompileTime/StaticInteger.h"
#include "Core/STL/CompileTime/TypeList.h"
#include "Core/STL/Math/Mathematics.h"
#include "Core/STL/Math/Vec.h"
#include "Core/STL/Math/BinaryMath.h"
#include "Core/STL/Math/Interpolations.h"

namespace GXPhysics
{
	using namespace GX_STL;
	using namespace GX_STL::GXTypes;
	using namespace GX_STL::GXMath;


#	define PX_AUTO( _type_ )	auto

}	// GXPhysics
