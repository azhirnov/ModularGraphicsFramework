// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
#include "Engine/STL/Types/SharedPointer.h"
#include "Engine/STL/Types/WeakPointer.h"

namespace GX_STL
{
namespace GXTypes
{
	
/*
=================================================
	ToStringImpl
=================================================
*/
	inline String  ToStringImpl (bool value)	{ return String( value ); }
	inline String  ToStringImpl (char value)	{ return String( &value, 1 ); }
	inline String  ToStringImpl (byte value)	{ return String( int(value) ); }
	inline String  ToStringImpl (ubyte value)	{ return String( uint(value) ); }
	inline String  ToStringImpl (short value)	{ return String( int(value) ); }
	inline String  ToStringImpl (ushort value)	{ return String( uint(value) ); }
	inline String  ToStringImpl (int value)		{ return String( value ); }
	inline String  ToStringImpl (uint value)	{ return String( value ); }
	inline String  ToStringImpl (ilong value)	{ return String( value ); }
	inline String  ToStringImpl (ulong value)	{ return String( value ); }
	inline String  ToStringImpl (float value)	{ return String( value ); }
	inline String  ToStringImpl (double value)	{ return String( value ); }

/*
=================================================
	VecToStringShort
	VecToStringShortLong
=================================================
*/
	namespace _types_hidden_
	{
		template <typename T, usize I, ulong U>
		inline String  VecToStringShort (const GXMath::Vec<T,I,U> &value)
		{
			String	str;

			FOR( i, value ) {
				str << (i ? ", " : "") << ToString( value[i] );
			}
			return str;
		}

		template <typename T, usize I, ulong U>
		inline String  VecToStringShortLong (const GXMath::Vec<T,I,U> &value, uint nameType = 0)
		{
			static const char	s_aNames[] = {	'x', 'y', 'z', 'w',
												'r', 'g', 'b', 'a' };
			nameType = (nameType & 1) << 2;

			String	str;

			FOR( i, value ) {
				str << (i ? ", " : "") << s_aNames[i + nameType] << ": " << ToString( value[i] );
			}
			return str;
		}

	}	// _types_hidden_
	
/*
=================================================
	ToStringImpl (Vec)
=================================================
*/
	template <typename T, usize I, ulong U>
	inline String  ToStringImpl (const GXMath::Vec<T,I,U> &value, bool shortly = true)
	{
		if ( shortly )
			return _types_hidden_::VecToStringShort( value );
		else
			return _types_hidden_::VecToStringShortLong( value );
	}
	
/*
=================================================
	ToStringImpl (Matrix)
=================================================
*/
	template <typename T, usize C, usize R, ulong U>
	inline String  ToStringImpl (const GXMath::Matrix<T,C,R,U> &value)
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
	ToStringImpl (Quaternion)
=================================================
*/
	template <typename T>
	inline String  ToStringImpl (const GXMath::Quaternion<T> &value, bool shortly = true)
	{
		return ToStringImpl( value.xyzw(), shortly );
	}

/*
=================================================
	ToStringImpl (Color)
=================================================
*/
	inline String  ToStringImpl (const GXMath::color4u &value, bool shortly = true)
	{
		return ToStringImpl( GXMath::ubyte4( value ), shortly );
	}

	inline String  ToStringImpl (const GXMath::color4f &value, bool shortly = true)
	{
		return ToStringImpl( GXMath::float4( value ), shortly );
	}

/*
=================================================
	ToStringImpl (Time)
=================================================
*/
	template <typename T>
	inline String  ToStringImpl (const Time<T> &time)
	{
		return ToString( TimeD::FromTime( time ) );
	}
	
	template <>
	inline String  ToStringImpl (const TimeD &time)
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
	ToStringImpl (Bytes)
=================================================
*/
	template <typename T>
	inline String  ToStringImpl (const Bytes<T> &value)
	{
		const T	kb	= T(1) << 14;
		const T mb	= T(1) << 24;
		const T	gb	= T(1) << GXMath::Min( T(34), T(Bits<T>::template SizeOf<T>())-1 );
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
	ToStringImpl (Bits)
=================================================
*/
	template <typename T>
	inline String  ToStringImpl (const Bits<T> &value)
	{
		const T	kb	= T(1) << 14;
		const T mb	= T(1) << 24;
		const T	gb	= T(1) << GXMath::Min( T(34), T(Bits<T>::template SizeOf<T>()-1) );
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
	ToStringImpl (Radians)
=================================================
*/
	template <typename T>
	inline String  ToStringImpl (const GXMath::Radians<T> &value)
	{
		String	str;
		str.FormatF( T(value), StringFormatF().Fmt(0,2) ) << " rad";
		return str;
	}
	
/*
=================================================
	ToStringImpl (Degrees)
=================================================
*/
	template <typename T>
	inline String  ToStringImpl (const GXMath::Degrees<T> &value)
	{
		String	str;
		str.FormatF( T(value), StringFormatF().Fmt(0,2) ) << " deg";
		return str;
	}
	
/*
=================================================
	ToStringImpl (Percentage)
=================================================
*/
	template <typename T>
	inline String  ToStringImpl (const GXMath::Percentage<T> &value)
	{
		String	str;
		str.FormatF( value.GetPercents(), StringFormatF().Fmt(0,2) ) << "%";
		return str;
	}

#ifdef GX_PHYSICS_DIMENSIONS_ENABLED
/*
=================================================
	ToStringImpl (PhysicsValue)
=================================================
*/
	template <typename T, typename D, typename S>
	inline String  ToStringImpl (const GXMath::PhysicsValue<T,D,S> &value)
	{
		return value.ToString();
	}
#endif
	
/*
=================================================
	ToStringImpl (Rectangle)
=================================================
*/
	template <typename T>
	inline String ToStringImpl (const GXMath::Rectangle<T> &value)
	{
		return	"left: "_str << ToString( value.left ) << ", bottom: " << ToString( value.bottom ) <<
				", right: " << ToString( value.right ) << ", top: " << ToString( value.top );
	}
	
/*
=================================================
	ToStringImpl (AABBox)
=================================================
*/
	template <typename T>
	inline String  ToStringImpl (const GXMath::AABBox<T> &value)
	{
		return "min( "_str << ToString( value.Min() ) << " ), max( " << ToString( value.Max() ) << " )";
	}

/*
=================================================
	ToStringImpl (Plane)
=================================================
*/
	template <typename T>
	inline String  ToStringImpl (const GXMath::Plane<T> &value)
	{
		return "norm: ("_str << ToString( value.Normal() ) << "), dist: " << ToString( value.Distance() );
	}

/*
=================================================
	ToStringImpl (Date)
=================================================
*/
	inline String  ToStringImpl (const OS::Date &value)
	{
		return value.ToString( "yyyy/mm/dm - hh:mi:ss" );
	}
	
/*
=================================================
	ToStringImpl (StaticFractional)
=================================================
*
	template <template <typename T, T N, T D> class SF, typename T, T Numerator, T Denominator = 1>
	inline String ToStringImpl ()
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
	ToStringImpl (TypeId)
=================================================
*/
	inline String  ToStringImpl (const TypeId &value)
	{
		return value.Name();
	}

/*
=================================================
	ToStringImpl (Optional)
=================================================
*/
	template <typename T>
	inline String  ToStringImpl (const Optional<T> &value)
	{
		return value ? ToString( value.Get() ) : "(none)";
	}
	
/*
=================================================
	ToStringImpl (Raw Pointer)
=================================================
*/
	template <typename T>
	inline String  ToStringImpl (T const * value)
	{
		return String().FormatI( ReferenceCast<usize>( value ), 16 );
	}
	
/*
=================================================
	ToStringImpl (Ptr)
=================================================
*/
	template <typename T>
	inline String  ToStringImpl (const Ptr<T> &value)
	{
		return ToString( value.RawPtr() );
	}
	
/*
=================================================
	ToStringImpl (SharedPointer)
=================================================
*/
	template <typename T, typename B, typename S>
	inline String  ToStringImpl (const SharedPointer<T,B,S> &value)
	{
		return ToString( value.RawPtr() );
	}
	
/*
=================================================
	ToStringImpl (WeakPointer)
=================================================
*/
	template <typename T, typename B, typename WS, typename SPS>
	inline String  ToStringImpl (const WeakPointer<T,B,WS,SPS> &value)
	{
		return ToString( value.RawPtr() );
	}
	
/*
=================================================
	ToStringImpl (ArrayCRef)
=================================================
*/
	template <typename T>
	inline String  ToStringImpl (ArrayCRef<T> arr)
	{
		String	str;	str.Reserve( arr.Count() * 4 );
		FOR( i, arr ) {
			str << (i ? ", " : "") << '(' << ToString( arr[i] ) << ')';
		}
		return str;
	}
	
/*
=================================================
	ToString
=================================================
*/
	template <typename T>
	inline String  ToString (const T &value)
	{
		return ToStringImpl( value );
	}


}	// GXTypes
}	// GX_STL
