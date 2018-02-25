// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Physics/Dimensions/PhysicsValueMath.h"

namespace GXPhysics
{

	// Is***Units //
	template <typename T>
	static constexpr bool	IsMassUnits = T::Dimensions_t::template Equal< typename DefaultPhysicsDimensions::Kilogram >::value;

	template <typename T>
	static constexpr bool	IsDistanceUnits = T::Dimensions_t::template Equal< typename DefaultPhysicsDimensions::Meter >::value;

	template <typename T>
	static constexpr bool	IsDensityUnits = T::Dimensions_t::template Equal< DefaultPhysicsDimensions::KilogramPerCubicMeter >::value;

	template <typename T>
	static constexpr bool	IsVelocityUnits = T::Dimensions_t::template Equal< DefaultPhysicsDimensions::MeterPerSecond >::value;
	
	template <typename T>
	static constexpr bool	IsAccelerationUnits = T::Dimensions_t::template Equal< DefaultPhysicsDimensions::MeterPerSquareSecond >::value;
	
	template <typename T>
	static constexpr bool	IsTimeUnits = T::Dimensions_t::template Equal< DefaultPhysicsDimensions::Second >::value;

	template <typename T1, typename T2>
	static constexpr bool	IsSameDimensions = T1::Dimensions_t::template Equal< T2::Dimensions_t >::value;


}	// GXPhysics
