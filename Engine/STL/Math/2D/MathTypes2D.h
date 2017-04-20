// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Math/MathTypes.h"
#include "Engine/STL/Math/Vec.h"
#include "Engine/STL/math/Mathematics.h"
#include "Engine/STL/Dimensions/RadiansAndDegrees.h"

namespace GX_STL
{
namespace GXMath
{
	
	// Rectangle
	template <typename T> struct Rectangle;

	typedef Rectangle< float   >	RectF;
	typedef Rectangle< double  >	RectD;
	typedef Rectangle< int     >	RectI;
	typedef Rectangle< uint    >	RectU;
	typedef Rectangle< ilong   >	RectL;
	typedef Rectangle< real    >	Rect;


	// Line 2D
	template <typename T> struct Line2;

	//typedef Line2< half   >		Line2H;
	typedef Line2< float  >			Line2F;
	typedef Line2< double >			Line2D;
	typedef Line2< real   >			Line2R;

	
	// Circle
	template <typename T> struct Circle;

	//typedef Circle< half   >		CircleH;
	typedef Circle< float  >		CircleF;
	typedef Circle< double >		CircleD;
	typedef Circle< real   >		CircleR;


}	// GXMath
}	// GX_STL
