// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CoreTests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


inline bool CmpDate (const Date &lhs, const Date &rhs)
{
	return	lhs.Year()			== rhs.Year()		and
			lhs.Month()			== rhs.Month()		and
			lhs.DayOfYear()		== rhs.DayOfYear()	and
			lhs.DayOfMonth()	== rhs.DayOfMonth()	and
			lhs.DayOfWeek()		== rhs.DayOfWeek()	and
			lhs.Hour()			== rhs.Hour()		and
			lhs.Minute()		== rhs.Minute()		and
			lhs.Second()		== rhs.Second()		and
			lhs.Milliseconds()	== rhs.Milliseconds();
}


static void DateTest1 ()
{
	STATIC_ASSERT( sizeof(Date) == sizeof(ulong) );

	Date	d;	d.Now();

	Date	d0 = d;
	TEST( CmpDate( d0, d ) );

	OS::Thread::Sleep( TimeL::FromMilliSeconds(1000) );
	d0.Now();

	TEST( not CmpDate( d0, d ) );
	TEST( d0 >  d  );
	TEST( d  <  d0 );

	d0.SetMillisecondsSince1970( d.ToMillisecondsSince1970() );
	TEST( CmpDate( d0, d ) );

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


static void DateTest2 ()
{
	Date	d0;	d0.Now();
	DEBUG_CONSOLE( ToString( d0 ).cstr() );

	TimeL	t0 = d0.ToTime();

	Date	d1;	d1.SetTime( t0 );
	DEBUG_CONSOLE( ToString( d1 ).cstr() );

	TEST( CmpDate( d0, d1 ) );
}


extern void Test_OS_Date ()
{
	DateTest1();
	DateTest2();
}
