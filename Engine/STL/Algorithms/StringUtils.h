// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Containers/String.h"
#include "Engine/STL/Containers/ArrayRef.h"

namespace GX_STL
{
namespace GXTypes
{
namespace StringUtils
{

	//
	// IsUpper, IsLower
	//

	template <typename T>
	constexpr forceinline bool IsUpper (const T &c)
	{
		return ( c >= T('A') and c <= T('Z') );
	}


	template <typename T>
	constexpr forceinline bool IsLower (const T &c)
	{
		return ( c >= T('a') and c <= T('z') );
	}


	//
	// ToUpper, ToLower
	//

	template <typename T>
	constexpr forceinline T ToUpper (const T &c)
	{
		return ( IsLower( c ) ? c + T('A' - 'a') : c );
	}


	template <typename T>
	constexpr forceinline T ToLower (const T &c)
	{
		return ( IsUpper( c ) ? c + T('a' - 'A') : c );
	}

	/*
	template <typename T>
	inline TStringRef<T> &  ToUpper (TStringRef<T> &str)
	{
		for(usize i = 0; i < str.Length(); ++i) {
			str[i] = ToUpper( str[i] );
		}
		return str;
	}

	/*
	template <typename T, typename S, typename A>
	inline TString<T,S,A> &  ToUpper (TString<T,S,A> &str)
	{
		TStringRef<T>	ustr( str );
		ToUpper( ustr );
		return str;
	}
	*/
	/*
	template <typename T>
	inline TStringRef<T> &  ToLower (TStringRef<T> &str)
	{
		for(usize i = 0; i < str.Length(); ++i) {
			str[i] = ToLower( str[i] );
		}
		return str;
	}

	/*
	template <typename T, typename S, typename A>
	inline TString<T,S,A> &  ToLower (TString<T,S,A> &str)
	{
		TStringRef<T>	ustr( str );
		ToLower( ustr );
		return str;
	}
	*/
	/*
	template <typename T>
	inline bool GetNames (TString<T> &sFirst, TString<T> &sSecond, const T *pName)
	{
		const T	*	p = pName;

		while ( p != 0 )
		{
			if ( *p == '.' )
			{
				sFirst.Copy( pName, p );
				sSecond.Copy( p+1 );
				return true;
			}

			++p;
		}
		return false;
	}
	*/
	/*
	template <typename T, typename B>
	inline usize IntToStr (B val, T * buf, usize size, int radix)
	{
		GX_UNUSED( size );

		if ( radix < 1 or radix > 16 )
		{
			WARNING( "incorrect radix" );
			radix = 10;
		}

		const usize		buf_size			= CompileTime::SizeOf<B>::bits + 2;		// TODO: Ln( sizeof(B), radix )
		T				tmp_buf[ buf_size ] = {0};
		usize			i					= buf_size - 2;
		bool			sign				= ( radix == 10 and val < B(0) );

		if ( val == 0 )
		{
			tmp_buf[i--] = T('0');
		}
		else
		{
			for(; val and i; --i, val = B(val / radix))
			{
				tmp_buf[i] = T( "0123456789ABCDEF"[ GXMath::Abs( val % radix ) ] );
			}
		}

		if ( sign )
			tmp_buf[i--] = '-';

		UnsafeMem::MemCopy( buf, tmp_buf + i + 1, BytesU( buf_size - i - 1 ) );
		i = buf_size - i - 2;

		ASSERT( size > i );
		return i;
	}
	*/
	
	
	inline int		ToInt32 (StringCRef sStr)			{ return atoi( sStr.cstr() ); }
	//inline int		ToInt32 (const char *pStr)			{ return atoi( pStr ); }

#ifdef COMPILER_MSVC
	inline ilong	ToInt64 (StringCRef sStr)			{ return _atoi64( sStr.cstr() ); }
	//inline ilong	ToInt64 (const char *pStr)			{ return _atoi64( pStr ); }
#endif

#ifdef COMPILER_GCC
	inline ilong	ToInt64 (StringCRef sStr)			{ return atoll( sStr.cstr() ); }
	//inline ilong	ToInt64 (const char *pStr)			{ return atoll( pStr ); }
#endif
	
	inline float	ToFloat (StringCRef sStr)			{ return float( atof( sStr.cstr() ) ); }
	//inline float	ToFloat (const char *pStr)			{ return float( atof( pStr ) ); }
	inline double	ToDouble (StringCRef sStr)			{ return atof( sStr.cstr() ); }
	//inline double	ToDouble (const char *pStr)			{ return atof( pStr ); }

	/*
	/*template <typename T>
	inline int ToInt32 (TStringRef<const T> sStr)
	{
		String	s_str;
		s_str.Copy( sStr );
		return ToInt32( s_str );
	}


	template <typename T>
	inline ilong ToInt64 (TStringRef<const T> sStr)
	{
		String	s_str;
		s_str.Copy( sStr );
		return ToInt64( s_str );
	}


	template <typename T>
	inline float ToFloat (TStringRef<const T> sStr)
	{
		String	s_str;
		s_str.Copy( sStr );
		return ToFloat( s_str );
	}


	template <typename T>
	inline double ToDouble (TStringRef<const T> sStr)
	{
		String	s_str;
		s_str.Copy( sStr );
		return ToDouble( s_str );
	}*/

	
	/*template <typename T, typename S, typename A>
	inline TString<T,S,A> & NanoTimeToStr (INOUT TString<T,S,A> &sToStr, float fTimeNano)
	{
		if ( fTimeNano > 1.0e+9f)
			return sToStr << TString<T,S,A>().FormatF( fTimeNano * 1.0e-9f, StringFormatF().Fmt(0,2) ) << " s";
		else
		if ( fTimeNano > 1.0e+6f )
			return sToStr << TString<T,S,A>().FormatF( fTimeNano * 1.0e-6f, StringFormatF().Fmt(0,2) ) << " ms";
		else
		if ( fTimeNano > 1.0e+3f )
			return sToStr << TString<T,S,A>().FormatF( fTimeNano * 1.0e-3f, StringFormatF().Fmt(0,2) ) << " us";
		else
			return sToStr << TString<T,S,A>().FormatF( fTimeNano, StringFormatF().Fmt(0,2) ) << " ns";
	}*/

	/*
	template <typename T, typename S, typename A>
	inline TString<T,S,A> & TimeToStr (INOUT TString<T,S,A> &sToStr, double timeInSeconds)
	{
		if ( timeInSeconds > 1.0e-1 )
			return sToStr << TString<T,S,A>().FormatF( timeInSeconds, StringFormatF().Fmt(0,2) ) << " s";
		else
		if ( timeInSeconds > 1.0e-4 )
			return sToStr << TString<T,S,A>().FormatF( timeInSeconds * 1.0e+3, StringFormatF().Fmt(0,2) ) << " ms";
		else
		if ( timeInSeconds > 1.0e-7 )
			return sToStr << TString<T,S,A>().FormatF( timeInSeconds * 1.0e+6, StringFormatF().Fmt(0,2) ) << " us";
		else
			return sToStr << TString<T,S,A>().FormatF( timeInSeconds * 1.0e+9, StringFormatF().Fmt(0,2) ) << " ns";
	}

	/*
	template <typename T>
	inline ulong FromStr16ToInt (TStringRef<const T> s)
	{
		ulong	val  = 0;

		for (usize i = 0; i < s.Length(); ++i)
		{
			const T	c = s[i];

			val = (val << 4) + ( c >= T('0') and c <= T('9') ? c - T('0') : (
								 c >= T('a') and c <= T('f') ? c - T('a') + 10 : (
								 c >= T('A') and c <= T('F') ? c - T('A') + 10 : 0 ) ) );
		}
		return val;
	}

	/*
	template <typename T>
	inline ilong FromStr10ToInt (TStringRef<const T> s)
	{
		ilong	val  = 0;
		bool	sign = (s[0] == '-');

		for (usize i = (s[0] == '-' or s[0] == '+'); i < s.Length(); ++i)
		{
			const T	c = s[i];

			val = (val * 10) + ( c >= T('0') and c <= T('9') ? c - T('0') : 0 );
		}
		return sign ? -val : val;
	}*/

	/*
	template <typename R>
	inline R  Pow10 (const int p)
	{
		R	ret = R(1);

		for (int i = 0; i < Abs(p); ++i)
			ret *= R(10);

		return ( p < 0  ?  R(1) / ret  :  ret );
	}

	template <typename T>
	inline double FromStrToFloat (TStringRef<const T> s)
	{
		double	val		= 0;
		double	div		= 0;
		int	dcount	= 0;
		int	pow		= 0;
		bool	sign	= (s[0] == '-');
		bool	spow	= false;
		usize	i		= sign or s[0] == '+';

		for (; i < s.Length(); ++i)
		{
			const T	c = s[i];

			if ( c == '.' ) break;
			val = (val * 10.0) + ( c >= T('0') and c <= T('9') ? c - T('0') : 0 );
		}

		if ( s[i] == '.' )
		{
			++i;

			for (; i < s.Length(); ++i)
			{
				const T c = s[i];

				if ( c == 'e' or c == 'E' ) break;

				div = (div * 10.0) + ( c >= T('0') and c <= T('9') ? c - T('0') : 0 );
				++dcount;
			}
		}

		if ( s[i] == 'e' or s[i] == 'E' )
		{
			++i;
			spow = (s[i] == '-');

			i += ( spow or s[i] == '+' );

			for (; i < s.Length(); ++i)
			{
				const T	c = s[i];

				pow = (pow * 10) + ( c >= T('0') and c <= T('9') ? c - T('0') : 0 );
			}
		}

		return	( GXMath::SetSign( val, sign ) + div * GXMath::_math_hidden_::Pow10<double>( - dcount ) ) *
				  GXMath::_math_hidden_::Pow10<double>( GXMath::SetSign( pow, spow ) );
	}

	/*
	template <typename T>
	inline usize FromStrToNumber (TStringRef<const T> str, OUT double_t &sNumber, OUT bool &bIsInt)
	{
		// return code
		const usize	SUCCESS	= usize(-1);

		if ( not (str[0] >= '0' and str[0] <= '9') and str[0] != '-' and str[0] != '+' )
			return 0;

		
		bool	sign	= (str[0] == '-');
		usize	i		= (sign or str[0] == '+');
		double	dval	= 0;


		if ( str.Length() > i+2 and str[i] == '0' and
			( str[i+1] == 'x' or str[i+1] == 'X' ) )
		{
		// Int 16bit
			ulong	ival	= 0;

			i   += 2;

			for (; i < str.Length(); ++i)
			{
				if ( str[i] >= '0' and str[i] <= '9' )		(ival <<= 4) += str[i] - '0';		else
				if ( str[i] >= 'a' and str[i] <= 'f' )		(ival <<= 4) += str[i] - 'a'+10;	else
				if ( str[i] >= 'A' and str[i] <= 'F' )		(ival <<= 4) += str[i] - 'A'+10;
				else										return i;
			}

			bIsInt = true;
			sNumber.SetI( GXMath::SetSign( (ilong)ival, sign ) );
			return SUCCESS;
		}
		else
		// Int 10bit
		{
			ulong	ival	= 0;

			for (; i < str.Length(); ++i)
			{
				if ( str[i] >= '0' and str[i] <= '9' )		(ival *= 10) += str[i] - '0';
				else										break;
			}

			if ( i >= str.Length() ) {
				bIsInt = true;
				sNumber.SetI( GXMath::SetSign( (ilong)ival, sign ) );
				return SUCCESS;
			}

			dval = (double) ival;
		}

		if ( str[i] == '.' )
		{
			double	div		= 0;
			int		dcount	= 0;
			int		ipow	= 0;
			bool	signpow	= false;

			++i;

			// Float
			for (;  i < str.Length(); ++i)
			{
				if ( str[i] >= '0' and str[i] <= '9' )
				{
					(div *= 10.0) += str[i] - '0';
					++dcount;
				}
				else
				if ( str[i] == 'e' or str[i] == 'E' )	{ ++i;	break; }
				else									return i;
			}

			if ( str[i] == '-' )	{ ++i;	signpow = true; }	else
			if ( str[i] == '+' )	{ ++i;	signpow = false; }

			for (; i < str.Length(); ++i)
			{
				if ( str[i] >= '0' and str[i] <= '9' )		(ipow *= 10) += str[i] - '0';	
				else										return i;
			}

			bIsInt = false;
			sNumber.SetF( GXMath::SetSign( dval + div * GXMath::Pow10<double>( -dcount ), sign ) *
						  GXMath::Pow10<double>( GXMath::SetSign( ipow, signpow ) ) );
			return SUCCESS;
		}

		return i;
	}

	/*
	template <typename T>
	inline TStringRef<const T> GetFieldName (TStringRef<const T> str)
	{
		FOR_rev( i, str ) {
			if ( str[i] == T('.') or str[i] == T('>') )	// find '.' or '->'
				return TStringRef<const T>( ( str.cstr() + i+1 ), str.Length() - i-1 );
		}
		return str;
	}

	/*
	template <typename T>
	inline String  BytesToString (const Bytes<T> value)
	{
		const T	kb	= T(1) << 14;
		const T mb	= T(1) << 24;
		const T	gb	= T(1) << GXMath::Min( T(34), T(Bits<T>::SizeOf<T>())-1 );
		const T	val	= (T) value;

		if ( val < kb )	return String().FormatI( val,       3, ' ' ) << " b";
		if ( val < mb )	return String().FormatI( val >> 10, 3, ' ' ) << " Kb";
		if ( val < gb )	return String().FormatI( val >> 20, 3, ' ' ) << " Mb";
						return String().FormatI( val >> 30, 3, ' ' ) << " Gb";
	}


	template <typename T>
	inline String  BitsToString (const Bits<T> value)
	{
		const T	kb	= T(1) << 14;
		const T mb	= T(1) << 24;
		const T	gb	= T(1) << GXMath::Min( T(34), T(Bits<T>::SizeOf<T>()-1) );
		const T	val	= (T) value;

		if ( val < kb )	return String().FormatI( val,       3, ' ' ) << " bit";
		if ( val < mb )	return String().FormatI( val >> 10, 3, ' ' ) << " Kbit";
		if ( val < gb )	return String().FormatI( val >> 20, 3, ' ' ) << " Mbit";
						return String().FormatI( val >> 30, 3, ' ' ) << " Gbit";
	}*/


}	// StringUtils
}	// GXTypes
}	// GX_STL
