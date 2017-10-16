// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Containers/String.h"

#include "Engine/STL/Dimensions/ByteAndBit.h"
#include "Engine/STL/Dimensions/RadiansAndDegrees.h"
#include "Engine/STL/Dimensions/Percentage.h"
#include "Engine/STL/Dimensions/PhysicsValue.h"
#include "Engine/STL/Math/Vec.h"
#include "Engine/STL/Math/Matrix.h"
#include "Engine/STL/Math/Color/Color.h"
#include "Engine/STL/Math/Quaternion.h"
#include "Engine/STL/Math/2D/Rectangle.h"
#include "Engine/STL/Math/3D/AxisAlignedBox.h"
#include "Engine/STL/Math/3D/Plane.h"
#include "Engine/STL/Time/Time.h"
#include "Engine/STL/OS/OSLowLevel.h"
#include "Engine/STL/Types/Optional.h"
#include "Engine/STL/Types/ReferenceCounter.h"

namespace GX_STL
{
namespace GXTypes
{
	
/*
=================================================
	ToString
=================================================
*/
	template <typename T>
	inline String  ToString (const T &value)
	{
		return String( value );	// if possible
	}
	
/*
=================================================
	VecToStringShort
	VecToStringShortLong
=================================================
*/
	namespace _types_hidden_
	{
		template <typename T, usize I>
		inline String  VecToStringShort (const GXMath::Vec<T,I> &value)
		{
			String	str;

			FOR( i, value ) {
				str << (i ? ", " : "") << value[i];
			}
			return str;
		}

		template <typename T, usize I>
		inline String  VecToStringShortLong (const GXMath::Vec<T,I> &value, uint nameType = 0)
		{
			static const char	s_aNames[] = {	'x', 'y', 'z', 'w',
												'r', 'g', 'b', 'a' };
			nameType = (nameType & 1) << 2;

			String	str;

			FOR( i, value ) {
				str << (i ? ", " : "") << s_aNames[i + nameType] << ": " << value[i];
			}
			return str;
		}

	}	// _types_hidden_
	
/*
=================================================
	ToString (Vec)
=================================================
*/
	template <typename T, usize I>
	inline String  ToString (const GXMath::Vec<T,I> &value, bool shortly = true)
	{
		if ( shortly )
			return _types_hidden_::VecToStringShort( value );
		else
			return _types_hidden_::VecToStringShortLong( value );
	}
	
/*
=================================================
	ToString (Matrix)
=================================================
*/
	template <typename T, usize C, usize R>
	inline String  ToString (const GXMath::Matrix<T,C,R> &value)
	{
		String	str;

		for (usize c = 0; c < C; ++c)
		{
			str << (c ? ", " : "") << "(";

			for (usize r = 0; r < R; ++r) {
				str << (r ? ", " : "") << ToString( value(c,r) );
			}
			str << ")";
		}
		return str;
	}

/*
=================================================
	ToString (Quaternion)
=================================================
*/
	template <typename T>
	inline String  ToString (const GXMath::Quaternion<T> &value, bool shortly = true)
	{
		return ToString( value.xyzw(), shortly );
	}

/*
=================================================
	ToString (Color)
=================================================
*/
	inline String  ToString (const GXMath::color4u &value, bool shortly = true)
	{
		return ToString( GXMath::ubyte4( value ), shortly );
	}

	inline String  ToString (const GXMath::color4f &value, bool shortly = true)
	{
		return ToString( GXMath::float4( value ), shortly );
	}

/*
=================================================
	ToString (Time)
=================================================
*/
	template <typename T>
	inline String  ToString (const Time<T> &time)
	{
		return ToString( TimeD::FromTime( time ) );
	}
	
	template <>
	inline String  ToString (const TimeD &time)
	{
		String	str;

		if ( time > 59.0 * 60.0 )
			str.FormatF( time.Hours(), StringFormatF().Fmt(0,2) ) << " h";
		else
		if ( time > 59.0 )
			str.FormatF( time.Minutes(), StringFormatF().Fmt(0,2) ) << " m";
		else
		if ( time > 1.0e-1 )
			str.FormatF( time.Seconds(), StringFormatF().Fmt(0,2) ) << " s";
		else
		if ( time > 1.0e-4 )
			str.FormatF( time.MilliSeconds(), StringFormatF().Fmt(0,2) ) << " ms";
		else
		if ( time > 1.0e-7 )
			str.FormatF( time.MicroSeconds(), StringFormatF().Fmt(0,2) ) << " us";
		else
			str.FormatF( time.NanoSeconds(), StringFormatF().Fmt(0,2) ) << " ns";

		return str;
	}

/*
=================================================
	ToString (Bytes)
=================================================
*/
	template <typename T>
	inline String  ToString (const Bytes<T> &value)
	{
		const T	kb	= T(1) << 14;
		const T mb	= T(1) << 24;
		const T	gb	= T(1) << GXMath::Min( T(34), T(Bits<T>::SizeOf<T>())-1 );
		const T	val	= (T) value;

		String	str;

		if ( val < kb )	str.FormatI( val,       3, ' ' ) << " b";	else
		if ( val < mb )	str.FormatI( val >> 10, 3, ' ' ) << " Kb";	else
		if ( val < gb )	str.FormatI( val >> 20, 3, ' ' ) << " Mb";	else
						str.FormatI( val >> 30, 3, ' ' ) << " Gb";
		return str;
	}

/*
=================================================
	ToString (Bits)
=================================================
*/
	template <typename T>
	inline String  ToString (const Bits<T> &value)
	{
		const T	kb	= T(1) << 14;
		const T mb	= T(1) << 24;
		const T	gb	= T(1) << GXMath::Min( T(34), T(Bits<T>::SizeOf<T>()-1) );
		const T	val	= (T) value;

		String	str;

		if ( val < kb )	str.FormatI( val,       3, ' ' ) << " bit";		else
		if ( val < mb )	str.FormatI( val >> 10, 3, ' ' ) << " Kbit";	else
		if ( val < gb )	str.FormatI( val >> 20, 3, ' ' ) << " Mbit";	else
						str.FormatI( val >> 30, 3, ' ' ) << " Gbit";
		return str;
	}
	
/*
=================================================
	ToString (Radians)
=================================================
*/
	template <typename T>
	inline String  ToString (const GXMath::Radians<T> &value)
	{
		String	str;
		str.FormatF( (T)value, StringFormatF().Fmt(0,2) ) << " rad";
		return str;
	}
	
/*
=================================================
	ToString (Degrees)
=================================================
*/
	template <typename T>
	inline String  ToString (const GXMath::Degrees<T> &value)
	{
		String	str;
		str.FormatF( (T)value, StringFormatF().Fmt(0,2) ) << " deg";
		return str;
	}
	
/*
=================================================
	ToString (Percentage)
=================================================
*/
	template <typename T>
	inline String  ToString (const GXMath::Percentage<T> &value)
	{
		String	str;
		str.FormatF( value.GetPercents(), StringFormatF().Fmt(0,2) ) << "%";
		return str;
	}
	
/*
=================================================
	ToString (PhysicsValue)
=================================================
*/
	template <typename T, typename D, typename S>
	inline String  ToString (const GXMath::PhysicsValue<T,D,S> &value)
	{
		return value.ToString();
	}
	
/*
=================================================
	ToString (Rectangle)
=================================================
*/
	template <typename T>
	inline String ToString (const GXMath::Rectangle<T> &value)
	{
		return "left: "_str << value.left << ", bottom: " << value.bottom
							<< ", right: " << value.right << ", top: " << value.top;
	}
	
/*
=================================================
	ToString (AABBox)
=================================================
*/
	template <typename T>
	inline String  ToString (const GXMath::AABBox<T> &value)
	{
		return "min( "_str << ToString( value.Min() ) << " ), max( " << ToString( value.Max() ) << " )";
	}

/*
=================================================
	ToString (Plane)
=================================================
*/
	template <typename T>
	inline String  ToString (const GXMath::Plane<T> &value)
	{
		return "norm: ("_str << ToString( value.Normal() ) << "), dist: " << ToString( value.Distance() );
	}

/*
=================================================
	ToString (Date)
=================================================
*/
	inline String  ToString (const OS::Date &value)
	{
		return value.ToString( "yyyy/mm/dm - hh:mi:ss" );
	}
	
/*
=================================================
	ToString (StaticFractional)
=================================================
*
	template <template <typename T, T N, T D> class SF, typename T, T Numerator, T Denominator = 1>
	inline String ToString ()
	{
		typedef SF

		String	s;
		s << String().FormatI( N, 10 );

		if ( N != 0 and D > 1 )
			s << '/' << String().FormatI( D, 10 );
			
		return s;
	}

/*
=================================================
	ToString (TypeId)
=================================================
*/
	inline String  ToString (const TypeId &value)
	{
		return value.Name();
	}

/*
=================================================
	ToString (Optional)
=================================================
*/
	template <typename T>
	inline String  ToString (const Optional<T> &value)
	{
		return value ? ToString( value.Get() ) : "(none)";
	}
	
/*
=================================================
	ToString (ReferenceCounter)
=================================================
*/
	template <typename T, typename B, typename S>
	inline String  ToString (const ReferenceCounter<T,B,S> &value)
	{
		return String().FormatI( ReferenceCast<usize>( value.RawPtr() ), 16 );
	}
	
/*
=================================================
	ToString (ArrayCRef)
=================================================
*/
	template <typename T>
	inline String  ToString (ArrayCRef<T> arr)
	{
		String	str;	str.Reserve( arr.Count() * 4 );
		FOR( i, arr ) {
			str << (i ? ", " : "") << '(' << ToString( arr[i] ) << ')';
		}
		return str;
	}


}	// GXTypes
}	// GX_STL
