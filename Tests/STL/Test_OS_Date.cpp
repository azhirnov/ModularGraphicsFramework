// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Tests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


extern void Test_OS_Date ()
{
	STATIC_ASSERT( sizeof(OS::Date) == sizeof(ulong) );

	OS::Date	d;	d.Now();

	OS::Date	d0 = d;
	TEST( d0 == d );

	OS::Thread::Sleep( TimeL::FromMilliSeconds(1000) );
	d0.Now();

	TEST( d0 != d  );
	TEST( d0 >  d  );
	TEST( d  <  d0 );

	d0.SetMillisecondsSince1970( d.ToMillisecondsSince1970() );
	TEST( d0 == d );

	TimeL t0 = OS::PerformanceTimer().Get<TimeL>();
	d0.SetMillisecondsSinceEpoch( t0.MilliSeconds() );

	TimeL t1;
	t1.SetMilliSeconds( d0.ToMillisecondsSinceEpoch() );
	TEST( t0.MilliSeconds() == t1.MilliSeconds() );


	const char format1[] = "yyyy/mm.dm - hh:mi:ss ms";
	const char format2[] = "yy//mm/dm (dee), dww|mmm  dwww -> mmmm";

	DEBUG_CONSOLE( (String("Date with format \"") << format1 << "\":\n" << d.ToString( format1 )).cstr() );
	DEBUG_CONSOLE( (String("Date with format \"") << format2 << "\":\n" << d.ToString( format2 )).cstr() );
}
