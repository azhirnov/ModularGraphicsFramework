// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace CompileTime;


template <typename T>
using Templ1 = CompileTime::NearInt::FromType<T>;

template <typename T>
struct Templ2_True
{
	typedef CompileTime::NearInt::FromType<T>	type;
};

template <typename T>
struct Templ2_False
{
	STATIC_ASSERT(( IsSameTypes< T, float > ));
	typedef CompileTime::NearInt::FromType<T>	type;
};


extern void Test_CompileTime_TemplateMath ()
{
	STATIC_ASSERT(( IntLog2< uint, (1<<12) > == 12 ));
	STATIC_ASSERT(( IntLog2< uint, 0 > == 0 ));
	STATIC_ASSERT(( IntLog2< uint, (1<<31) > == 31 ));
	STATIC_ASSERT(( IntLog2< ulong, (ulong(1) << 60) > == 60 ));

	STATIC_ASSERT(( FloorPowerOfTwo< uint, (1<<10) > == (1<<10) ));
	STATIC_ASSERT(( FloorPowerOfTwo< uint, (1<<10) + 20 > == (1<<10) ));
	STATIC_ASSERT(( FloorPowerOfTwo< uint, (1<<10) - 20 > == (1<<9) ));
	
	STATIC_ASSERT(( CeilPowerOfTwo< uint, (1<<15) > == (1<<15) ));
	STATIC_ASSERT(( CeilPowerOfTwo< uint, (1<<15) + 20 > == (1<<16) ));
	STATIC_ASSERT(( CeilPowerOfTwo< uint, (1<<15) - 20 > == (1<<15) ));

	STATIC_ASSERT(( GreatestCommonDivisor< uint, 8, 16 > == 8 ));


	/*
	typedef CompileTime::SwitchType< true, Templ2_True< float >::type,
											Templ2_False< double >::type >	TemplType;
	/*/
	typedef CompileTime::SwitchType< true, CompileTime::DeferredTemplate< Templ2_True, float >::type,
											CompileTime::DeferredTemplate< Templ2_False, double >::type >::type	TemplType;
	//*/
	STATIC_ASSERT(( CompileTime::IsSameTypes< TemplType, int > ));
}
