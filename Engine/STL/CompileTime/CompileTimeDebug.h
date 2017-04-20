// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.
/*
	Helper types for debuging compile-time code (templates, constexpr, ...)
*/

#pragma once

#include "TemplateUtils.h"
//#include "TypeList.h"

namespace GX_STL
{
namespace CompileTime
{
namespace Debug
{

	//
	// Log types to IDE build console
	// To Log values use CompileTime::ValueToType
	//

	template <	bool Trigger,
				typename T0,		typename T1 = void, typename T2 = void, typename T3 = void, typename T4 = void,
				typename T5 = void, typename T6 = void, typename T7 = void, typename T8 = void, typename T9 = void
			 >
	struct LogTypes
	{
		static const bool	value = not Trigger;
		static_assert( value, "LogTypes" );
	};


#	define GX_COMPILE_TIME_LOG_TYPES( ... ) \
		STATIC_ASSERT( (::GX_STL::CompileTime::Debug::LogTypes< __VA_ARGS__ >::value) )



	//
	// Type List To String
	//

	template <typename Typelist>
	struct TypeListToString
	{
	private:
		struct Func
		{
			const StringCRef	divider;
			String				result;

			explicit Func (StringCRef divider) : divider(divider)
			{}

			template <typename T, usize Index>
			void Process ()
			{
				if ( Index > 0 )
					result << divider;

				result << '[' << Index << "] " << typeid(T).name();
			}
		};

	public:
		static String ToString (StringCRef divider = "\n")
		{
			Func func( divider );
			Typelist::RuntimeForEach( func );
			return func.result;
		}

		static void ToLog ()
		{
			LOG( ToString().cstr(), ELog::Debug );
		}

		static void Show ()
		{
			WARNING( ToString().cstr() );
		}
	};


}	// Debug
}	// CompileTime
}	// GX_STL
