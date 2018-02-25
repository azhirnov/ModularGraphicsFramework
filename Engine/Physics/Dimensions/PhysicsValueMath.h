// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Physics/Dimensions/PhysicsValueVec.h"

namespace GXPhysics
{

	template <typename AValueType, typename ADimensions, typename AValueScale,
			  typename BValueType, typename BDimensions, typename BValueScale
			 >
	CHECKRES forceinline auto  MiddleValue (const PhysicsValue<AValueType, ADimensions, AValueScale>& a,
											const PhysicsValue<BValueType, BDimensions, BValueScale>& b)
	{
		return decltype(a+b)(GXMath::MiddleValue( a.ref(), b.ref() ));
	}

}	// GXPhysics
