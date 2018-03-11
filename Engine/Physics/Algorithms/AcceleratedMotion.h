// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Physics/Dimensions/PhysicsValueMath.h"
#include "Engine/Physics/CompileTime/PhysTypeInfo.h"
#include "Engine/Physics/CompileTime/GenType.h"

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
		
		using T = PXGenType< Pos, Vel, Accel, Time >;

		pos += vel * dt * T(0.5);
		vel += accel * dt;
		pos += vel * dt * T(0.5);
	}

}	// GXPhysics