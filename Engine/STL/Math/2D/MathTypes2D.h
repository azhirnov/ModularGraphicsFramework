// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Math/MathTypes.h"
#include "Engine/STL/Math/Vec.h"
#include "Engine/STL/Math/Mathematics.h"
#include "Engine/STL/Dimensions/RadiansAndDegrees.h"

namespace GX_STL
{
namespace GXMath
{
	
	// Rectangle
	template <typename T, ulong UID = 0> struct Rectangle;

	typedef Rectangle< float   >	RectF;
	typedef Rectangle< double  >	RectD;
	typedef Rectangle< int     >	RectI;
	typedef Rectangle< uint    >	RectU;
	typedef Rectangle< ilong   >	RectL;
	typedef Rectangle< real    >	Rect;


	// Line 2D
	template <typename T, ulong UID = 0> struct Line2;

	//typedef Line2< half   >		Line2H;
	typedef Line2< float  >			Line2F;
	typedef Line2< double >			Line2D;
	typedef Line2< real   >			Line2R;

	
	// Circle
	template <typename T, ulong UID = 0> struct Circle;

	//typedef Circle< half   >		CircleH;
	typedef Circle< float  >		CircleF;
	typedef Circle< double >		CircleD;
	typedef Circle< real   >		CircleR;


}	// GXMath
}	// GX_STL
