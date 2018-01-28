// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/Common.h"

namespace Engine
{
namespace ScreenCoords
{
	using namespace GX_STL::GXMath;


	// Pixels
	using pixelsi1	= Vec< int, 1, "pixels"_StringToID >;
	using pixelsi2	= Vec< int, 2, "pixels"_StringToID >;
	using pixelsi3	= Vec< int, 3, "pixels"_StringToID >;
	using pixelsi4	= Vec< int, 4, "pixels"_StringToID >;

	using pixelsf1	= Vec< float, 1, "pixels"_StringToID >;
	using pixelsf2	= Vec< float, 2, "pixels"_StringToID >;
	using pixelsf3	= Vec< float, 3, "pixels"_StringToID >;
	using pixelsf4	= Vec< float, 4, "pixels"_StringToID >;

	using pixelsri	= Rectangle< int, "pixels"_StringToID >;
	using pixelsrf	= Rectangle< float, "pixels"_StringToID >;

	using pixelsf1	= Vec< float, 1, "pixels"_StringToID >;
	using pixelsf2	= Vec< float, 2, "pixels"_StringToID >;
	using pixelsf3	= Vec< float, 3, "pixels"_StringToID >;
	using pixelsf4	= Vec< float, 4, "pixels"_StringToID >;


	// Dips
	using dipsf1	= Vec< float, 1, "dips"_StringToID >;
	using dipsf2	= Vec< float, 2, "dips"_StringToID >;
	using dipsf3	= Vec< float, 3, "dips"_StringToID >;
	using dipsf4	= Vec< float, 4, "dips"_StringToID >;
	
	using dipsri	= Rectangle< int, "dips"_StringToID >;
	using dipsrf	= Rectangle< float, "dips"_StringToID >;


	// Physical
	using metersf1	= Vec< float, 1, "meters"_StringToID >;
	using metersf2	= Vec< float, 2, "meters"_StringToID >;
	using metersf3	= Vec< float, 3, "meters"_StringToID >;
	using metersf4	= Vec< float, 4, "meters"_StringToID >;


}	// ScreenCoords
}	// Engine
