// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Physics/Dimensions/PhysicsValueMath.h"
#include "Engine/Physics/CompileTime/PhysTypeInfo.h"

namespace GXPhysics
{

/*
=================================================
	UniformlyAcceleratedMotion
=================================================
*/
	template <typename Pos, typename Vel, typename Accel, typename Time>
	inline void UniformlyAcceleratedMotion (INOUT Pos &pos, INOUT Vel &vel, const Accel accel, const Time dt)
	{
		STATIC_ASSERT( IsTimeUnits<Time> and IsAccelerationUnits<Accel> and IsVelocityUnits<Vel> and IsDistanceUnits<Pos> );

		pos += vel * dt * 0.5r;
		vel += accel * dt;
		pos += vel * dt * 0.5r;
	}

}	// GXPhysics