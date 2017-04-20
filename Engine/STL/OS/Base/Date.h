// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Common.h"

namespace GX_STL
{
namespace OS
{

	//
	// Date
	//

	struct Date : public CompileTime::PODType
	{
	// types
	public:
		struct EWeekDay
		{
			enum type
			{
				Mon = 0,
				Tue,
				Wed,
				Thu,
				Fri,
				Sat,
				Sun,
				_Count
			};
		};

		struct EMonth
		{
			enum type
			{
				Jan = 0,
				Feb,
				Mar,
				Apr,
				May,
				Jun,
				Jul,
				Aug,
				Sep,
				Oct,
				Nov,
				Dec,
				_Count
			};
		};


	// variables
	private:
		uint	_year		: 16,	// ****	(0..65536)
				_month		: 4,	// 12	(0..16)
				_dayOfWeek	: 3,	// 7	(0..8)
				_dayOfYear	: 9,	// 366	(0..512)
				_dayOfMonth	: 5,	// 31	(0..32)
				_hour		: 5,	// 24	(0..32)
				_minute		: 6,	// 60	(0..64)
				_second		: 6,	// 60	(0..64)
				_millis		: 10;	// 1000	(0..1024)	// TODO: change to micro or nano?


	// methods
	public:
		Date():
			_year(0), _month(0), _dayOfWeek(0), _dayOfMonth(0), _dayOfYear(0),
			_hour(0), _minute(0), _second(0), _millis(0)
		{}
		
		// Implement for current platform
		Date & Now ();

		uint			Year ()				const	{ return _year; }
		EMonth::type	Month ()			const	{ return EMonth::type( _month ); }
		uint			WeekOfYear ()		const	{ return (_dayOfYear / 7) + 1; }
		uint			DayOfYear ()		const	{ return _dayOfYear + 1; }
		uint			DayOfMonth ()		const	{ return _dayOfMonth + 1; }
		EWeekDay::type	DayOfWeek ()		const	{ return EWeekDay::type( _dayOfWeek ); }
		uint			Hour ()				const	{ return _hour; }
		uint			Minute ()			const	{ return _minute; }
		uint			Second ()			const	{ return _second; }
		uint			Milliseconds ()		const	{ return _millis; }

		StringCRef		MonthName ()		const;
		StringCRef		DayOfWeekName ()	const;

		static ulong	MillisInDay ()				{ return 24 * MillisInHour(); }
		static ulong	MillisInHour ()				{ return 60 * MillisInMinute(); }
		static ulong	MillisInMinute ()			{ return 60 * MillisInSecond(); }
		static ulong	MillisInSecond ()			{ return 1000; }
		

		bool			IsLeapYear ()		const	{ return _IsLeapYear( _year ); }
		uint			DaysInYear ()		const	{ return _MaxDaysOfYear( _year ); }
		uint			DaysInMonth ()		const	{ return _DaysInMonth( _year, _month ); }


		ulong	ToMillisecondsSince1970 () const		{ return _ToMilliseconds( 1970 ); }
		Date &	SetMillisecondsSince1970 (ulong ms)		{ return _SetMilliseconds( 1970, ms ); }

		ulong	ToMillisecondsSinceEpoch () const		{ return _ToMilliseconds( 0 ); }
		Date &	SetMillisecondsSinceEpoch (ulong ms)	{ return _SetMilliseconds( 0, ms ); }


		Date &  SetYear (uint value)		{ _year = value; return *this; }
		Date &	SetMonth (uint value)		{ _month = value; return *this; }


		String ToString (StringCRef fmt) const;
		
		bool operator == (const Date &other) const;
		bool operator >  (const Date &other) const;
		bool operator <  (const Date &other) const;
		bool operator != (const Date &other) const	{ return not (*this == other); }
		bool operator >= (const Date &other) const	{ return not (*this < other); }
		bool operator <= (const Date &other) const	{ return not (*this > other); }


	private:
		static bool _IsLeapYear (uint year);
		static uint _MaxDaysOfYear (uint year);
		static uint _DaysInMonth (uint year, uint month);
		static uint _CalcDayOfYear (uint year, uint month, uint dayOfMonth);
		static uint _CalcDayOfWeek (uint year, uint month, uint dayOfMonth);
		static void _CalcMonthAndDayOfMonth (uint year, uint dayOfYear, OUT uint &month, OUT uint &dayOfMonth);

		ulong _ToMilliseconds (uint startYear) const;
		Date& _SetMilliseconds (uint startYear, ulong ms);
	};


}	// OS
}	// GX_STL
