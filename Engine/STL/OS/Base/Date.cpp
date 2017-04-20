// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Date.h"
#include "Engine/STL/Algorithms/StringParser.h"

namespace GX_STL
{
namespace OS
{

/*
=================================================
	MonthName
=================================================
*/
	StringCRef Date::MonthName () const
	{
		static const char * names[] = {
			"January", "February", "March", "April", "May", "June", "July",
			"August", "September", "October", "November", "December"
		};
		return names[ _month ];
	}
	
/*
=================================================
	DayOfWeekName
=================================================
*/
	StringCRef Date::DayOfWeekName () const
	{
		static const char * names[] = {
			"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
		};
		return names[ _dayOfWeek ];
	}
	
/*
=================================================
	_ToMilliseconds
=================================================
*/
	ulong Date::_ToMilliseconds (uint startYear) const
	{
		ulong	ms = _millis;

		ms += _second * MillisInSecond();
		ms += _minute * MillisInMinute();
		ms += _hour * MillisInHour();
		ms += _dayOfYear * MillisInDay();

		for (uint y = startYear; y < _year; ++y) {
			ms += _MaxDaysOfYear( y ) * MillisInDay();
		}
		return ms;
	}
	
/*
=================================================
	_SetMilliseconds
=================================================
*/
	Date &  Date::_SetMilliseconds (uint startYear, ulong ms)
	{
		_millis		= ms % MillisInSecond();
		_second		= (ms / MillisInSecond()) % 60;
		_minute		= (ms / MillisInMinute()) % 60;
		_hour		= (ms / MillisInHour())   % 24;
			
		ulong days	= ms / MillisInDay();
		uint  year	= startYear;
		uint  max_d	= _MaxDaysOfYear( year );

		for (; days >= max_d;)
		{
			days -= max_d;
			max_d = _MaxDaysOfYear( ++year );
		}

		_dayOfYear	= days;
		_year		= year;

		uint	m, dm;
		_CalcMonthAndDayOfMonth( _year, _dayOfYear, OUT m, OUT dm );

		_month		= m;
		_dayOfMonth	= dm;
		_dayOfWeek	= _CalcDayOfWeek( _year, _month, _dayOfMonth );

		return *this;
	}

/*
=================================================
	ToString
=================================================
*/
	String Date::ToString (StringCRef fmt) const
	{
		// yy - year short
		// yyyy - year full
		// mm - month
		// mmm - month short name
		// mmmm... - month name
		// we - week of year
		// dm - day of month
		// de - day of year
		// dee - day of year aligned
		// dw - day of week
		// dww - day of week short name
		// dwww... - day of week full name
		// hh - hour
		// mi - minute
		// ss - second
		// ms - millisecond
		
		Array< StringCRef > tokens;
		String				str;

		StringParser::DivideString_Words(fmt, tokens);

		FOR( i, tokens )
		{
			if ( tokens[i] == "yy" )
				str << String().FormatAlignedI( Year(), 4, '0', 10 ).SubString( 2 );
			else
			if ( tokens[i] == "yyyy" )
				str << String().FormatI( Year(), 10 );
			else
			if ( tokens[i] == "mm" )
				str << String().FormatAlignedI( uint(Month())+1, 2, '0', 10 );
			else
			if ( tokens[i] == "mmm" )
				str << MonthName().SubString( 0, 3 );
			else
			if ( tokens[i].StartsWith( "mmmm" ) )
				str << MonthName();
			else
			if ( tokens[i] == "we" )
				str << String().FormatAlignedI( WeekOfYear(), 2, '0', 10 );
			else
			if ( tokens[i] == "dm" )
				str << String().FormatAlignedI( DayOfMonth(), 2, '0', 10 );
			else
			if ( tokens[i] == "de" )
				str << String().FormatI( DayOfYear(), 10 );
			else
			if ( tokens[i] == "dee" )
				str << String().FormatAlignedI( DayOfYear(), 3, '0', 10 );
			else
			if ( tokens[i] == "dw" )
				str << String().FormatAlignedI( uint(DayOfWeek())+1, 2, '0', 10 );
			else
			if ( tokens[i] == "dww" )
				str << DayOfWeekName().SubString( 0, 3 );
			else
			if ( tokens[i].StartsWith( "dwww" ) )
				str << DayOfWeekName();
			else
			if ( tokens[i] == "hh" )
				str << String().FormatAlignedI( Hour(), 2, '0', 10 );
			else
			if ( tokens[i] == "mi" )
				str << String().FormatAlignedI( Minute(), 2, '0', 10 );
			else
			if ( tokens[i] == "ss" )
				str << String().FormatAlignedI( Second(), 2, '0', 10 );
			else
			if ( tokens[i] == "ms" )
				str << String().FormatAlignedI( Milliseconds(), 3, '0', 10 );
			else
				// TODO: it may be unsuported token, need some way to detect it
				str << tokens[i];
		}
		return str;
	}
	
/*
=================================================
	operator ==
=================================================
*/
	bool Date::operator == (const Date &other) const
	{
		return	Year()			== other.Year()			and
				DayOfYear()		== other.DayOfYear()	and
				Hour()			== other.Hour()			and
				Minute()		== other.Minute()		and
				Second()		== other.Second()		and
				Milliseconds()	== other.Milliseconds();
	}
	
/*
=================================================
	operator >
=================================================
*/
	bool Date::operator > (const Date &other) const
	{
		return	Year()			!= other.Year()			?	Year()		> other.Year()		:
				DayOfYear()		!= other.DayOfYear()	?	DayOfYear()	> other.DayOfYear()	:
				Hour()			!= other.Hour()			?	Hour()		> other.Hour()		:
				Minute()		!= other.Minute()		?	Minute()	> other.Minute()	:
				Second()		!= other.Second()		?	Second()	> other.Second()	:
				Milliseconds()	<  other.Milliseconds();
	}
	
/*
=================================================
	operator <
=================================================
*/
	bool Date::operator < (const Date &other) const
	{
		return	Year()			!= other.Year()			?	Year()		< other.Year()		:
				DayOfYear()		!= other.DayOfYear()	?	DayOfYear()	< other.DayOfYear()	:
				Hour()			!= other.Hour()			?	Hour()		< other.Hour()		:
				Minute()		!= other.Minute()		?	Minute()	< other.Minute()	:
				Second()		!= other.Second()		?	Second()	< other.Second()	:
				Milliseconds()	<  other.Milliseconds();
	}

/*
=================================================
	_IsLeapYear
=================================================
*/
	bool Date::_IsLeapYear (uint year)
	{
		return ((year & 0x3) == 0 and year % 100 != 0);
	}
	
/*
=================================================
	_MaxDaysOfYear
=================================================
*/
	uint Date::_MaxDaysOfYear (uint year)
	{
		return 365 + uint(_IsLeapYear( year ));
	}
		
/*
=================================================
	_DaysInMonth
=================================================
*/
	uint Date::_DaysInMonth (uint year, uint month)
	{
		const ubyte days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		return days[ month ] + uint( month == EMonth::Feb and _IsLeapYear( year ) );
	}
	
/*
=================================================
	_CalcDayOfYear
=================================================
*/
	uint Date::_CalcDayOfYear (uint year, uint month, uint dayOfMonth)
	{
		uint day = 0;

		for (uint i = 0; i < month; ++i) {
			day += _DaysInMonth( year, i );
		}
		return day + dayOfMonth;
	}
	
/*
=================================================
	_CalcDayOfWeek
=================================================
*/
	uint Date::_CalcDayOfWeek (uint year, uint month, uint dayOfMonth)
	{
		int	m = month;
		int y = year;

		if ( m <= 0 )
		{
			m += 12;
			y--;
		}

		int n = (dayOfMonth + (13 * m - 1) / 5 + (y % 100) + (y / 100) / 4 + (y % 100) / 4 - 2 * (y / 100)) % 7;

		if ( n < 0 )
			n += 7;

		return n == 0 ? 6 : n-1;
	}
	
/*
=================================================
	_CalcMonthAndDayOfMonth
=================================================
*/
	void Date::_CalcMonthAndDayOfMonth (uint year, uint dayOfYear, OUT uint &month, OUT uint &dayOfMonth)
	{
		uint days_in_month = _DaysInMonth( year, 0 );

		for (month = 0; dayOfYear >= days_in_month; ++month)
		{
			dayOfYear		-= days_in_month;
			days_in_month	 = _DaysInMonth( year, month );
		}

		dayOfMonth = dayOfYear;
	}


}	// OS
}	// GX_STL
