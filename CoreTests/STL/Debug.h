// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
	struct TDebugInstCounter : CompileTime::FastCopyable
	{
		typedef TDebugInstCounter<UID,T>	Self;

		static ilong		_counter;
		static ilong		_emptyCtorCnt;
		static ilong		_copyCtorCnt;
		static ilong		_moveCtorCnt;
		static ilong		_dtorCnt;

		ilong	value = 0;
		T		secondVal;

		TDebugInstCounter () : value( _counter + (1ll << 62)), secondVal()
		{
			++_counter;
			++_emptyCtorCnt;
		}

		explicit
		TDebugInstCounter (const T &val) : value( _counter + (1ll << 62)), secondVal(val)
		{
			++_counter;
			++_emptyCtorCnt;
		}

		TDebugInstCounter (const Self &other) : value( other.value ), secondVal( other.secondVal )
		{
			++_counter;
			++_copyCtorCnt;
		}

	#ifdef GX_DEBUGINSTCOUNTER_NO_MOVECTOR
		TDebugInstCounter (Self &&other) = delete;
	#else
		TDebugInstCounter (Self &&other) : value( other.value ), secondVal( RVREF( other.secondVal ) )
		{
			++_counter;
			++_moveCtorCnt;
			//other.value = 0;
		}
	#endif

		~TDebugInstCounter ()
		{
			TEST( value != 0 );	// assert on second dtor call
			--_counter;
			++_dtorCnt;
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
				<< "\nCounter:     " << _counter
				<< "\nEmpty ctors: " << _emptyCtorCnt
				<< "\nCopy ctors:  " << _copyCtorCnt
				<< "\nMove ctors:  " << _moveCtorCnt
				<< "\nDtors:       " << _dtorCnt;

			DEBUG_CONSOLE( str.cstr() );
			return _counter == 0;
		}

		static void ClearStatistic ()
		{
			_counter		= 0;
			_emptyCtorCnt	= 0;
			_copyCtorCnt	= 0;
			_moveCtorCnt	= 0;
			_dtorCnt		= 0;
		}
	};

	template <usize UID, typename T>	ilong	TDebugInstCounter<UID, T>::_counter			= 0;
	template <usize UID, typename T>	ilong	TDebugInstCounter<UID, T>::_emptyCtorCnt	= 0;
	template <usize UID, typename T>	ilong	TDebugInstCounter<UID, T>::_copyCtorCnt		= 0;
	template <usize UID, typename T>	ilong	TDebugInstCounter<UID, T>::_moveCtorCnt		= 0;
	template <usize UID, typename T>	ilong	TDebugInstCounter<UID, T>::_dtorCnt			= 0;


	
	template <usize UID, typename T>
	struct Hash< TDebugInstCounter<UID, T> >
	{
		HashResult  operator () (const TDebugInstCounter<UID, T> &x) const noexcept
		{
			return HashOf( x.secondVal );
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


namespace std
{
	template <GX_STL::GXTypes::usize UID, typename T>
	struct hash< GX_STL::GXTypes::TDebugInstCounter<UID, T> >
	{
		inline std::size_t operator() (const GX_STL::GXTypes::TDebugInstCounter<UID, T>& k) const
		{
			return GX_STL::GXTypes::HashOf( k ).Get();
		}
	};

}	// std
