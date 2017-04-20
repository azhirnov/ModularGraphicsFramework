// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

namespace GX_STL
{
namespace GXTypes
{

	//
	// Debug type with ctors logging
	//
	
	template <usize UID>
	struct TDebugCtorLog
	{
		TDebugCtorLog ()							{ WARNING( (String("ctor - ") << UID).cstr() ); }

		template <usize I>
		TDebugCtorLog (TDebugCtorLog<I> &&)			{ WARNING( (String("move ctor - ") << UID).cstr() ); }

#	ifdef _GX_DBG_VAR_COPY_CTOR
		template <usize I>
		TDebugCtorLog (TDebugCtorLog<I> &)			{ WARNING( (String("copy ctor - ") << UID).cstr() ); }
#	else
		template <usize I>
		TDebugCtorLog (const TDebugCtorLog<I> &)	{ WARNING( (String("copy ctor - ") << UID).cstr() ); }
#	endif

		~TDebugCtorLog ()							{ WARNING( (String("dtor - ") << UID).cstr() ); }
	};
	
	//#define GX_DEBUGINSTCOUNTER_NO_MOVECTOR


	//
	// Debug type with instance counting
	//

	template <usize UID, typename T = int>
	struct TDebugInstCounter
	{
		typedef TDebugInstCounter<UID,T>	Self;

		static ilong		counter;
		static ilong		emptyCtorCnt;
		static ilong		copyCtorCnt;
		static ilong		moveCtorCnt;
		static ilong		dtorCnt;

		ilong	value = 0;
		T		secondVal;

		TDebugInstCounter () : value( counter + (1ll << 62)), secondVal()
		{
			++counter;
			++emptyCtorCnt;
		}

		explicit
		TDebugInstCounter (const T &val) : value( counter + (1ll << 62)), secondVal(val)
		{
			++counter;
			++emptyCtorCnt;
		}

		TDebugInstCounter (const Self &other) : value( other.value ), secondVal( other.secondVal )
		{
			++counter;
			++copyCtorCnt;
		}

	#ifdef GX_DEBUGINSTCOUNTER_NO_MOVECTOR
		TDebugInstCounter (Self &&other) = delete;
	#else
		TDebugInstCounter (Self &&other) : value( other.value ), secondVal( RVREF( other.secondVal ) )
		{
			++counter;
			++moveCtorCnt;
			//other.value = 0;
		}
	#endif

		~TDebugInstCounter ()
		{
			ASSERT( value != 0 );	// assert on second dtor call
			--counter;
			++dtorCnt;
			value = 0;
		}

		Self& operator = (const Self &right)
		{
			value = right.value;
			secondVal = right.secondVal;
			return *this;
		}

		Self& operator = (Self &&right)
		{
			value = right.value;
			secondVal = RVREF( right.secondVal );
			return *this;
		}
		
		bool operator == (const Self &right) const
		{
			return value == right.value;
		}

		bool operator < (const Self &right) const
		{
			return value < right.value;
		}

		bool operator > (const Self &right) const
		{
			return value > right.value;
		}

		static bool CheckStatistic ()
		{
			String	str;

			str << "\nObjectID:    " << UID
				<< "\nCounter:     " << counter
				<< "\nEmpty ctors: " << emptyCtorCnt
				<< "\nCopy ctors:  " << copyCtorCnt
				<< "\nMove ctors:  " << moveCtorCnt
				<< "\nDtors:       " << dtorCnt;

			DEBUG_CONSOLE( str.cstr() );
			return counter == 0;
		}

		static void ClearStatistic ()
		{
			counter			= 0;
			emptyCtorCnt	= 0;
			copyCtorCnt		= 0;
			moveCtorCnt		= 0;
			dtorCnt			= 0;
		}
	};

	template <usize UID, typename T>	ilong	TDebugInstCounter<UID, T>::counter		= 0;
	template <usize UID, typename T>	ilong	TDebugInstCounter<UID, T>::emptyCtorCnt	= 0;
	template <usize UID, typename T>	ilong	TDebugInstCounter<UID, T>::copyCtorCnt	= 0;
	template <usize UID, typename T>	ilong	TDebugInstCounter<UID, T>::moveCtorCnt	= 0;
	template <usize UID, typename T>	ilong	TDebugInstCounter<UID, T>::dtorCnt		= 0;


	
	template <usize UID, typename T>
	struct Hash< TDebugInstCounter<UID, T> > : private Hash<T>
	{
		typedef TDebugInstCounter<UID, T>	key_t;
		typedef Hash<T>						base_t;
		typedef typename base_t::result_t	result_t;

		result_t operator () (const key_t &x) const
		{
			return base_t::operator ()( x.secondVal );
		}
	};


	struct ValueToString
	{
		template <typename T>
		String operator () (const Array<T> &arr, StringCRef divisor) const
		{
			String	str;

			FOR( i, arr )
			{
				if ( i > 0 )
					str << divisor;

				str << (*this)( arr[i] );
			}
			return str;
		}

		template <typename T>
		String operator () (const T& value) const
		{
			return String( value );
		}
	};
	

}	// GXTypes
}	// GX_STL
