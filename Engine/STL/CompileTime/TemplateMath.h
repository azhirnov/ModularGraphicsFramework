// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "TemplateUtils.h"

namespace GX_STL
{
namespace CompileTime
{

	//
	// Compile-time Abs
	//
	
	namespace _ctime_hidden_
	{
		template <typename T, T X>
		struct _Abs
		{
			static const T		value = (X < T(0)) ? -X : X;
		};

		template <ubyte X>
		struct _Abs< ubyte, X >
		{
			static const ubyte	value = X;
		};

		template <ushort X>
		struct _Abs< ushort, X >
		{
			static const ushort	value = X;
		};

		template <uint X>
		struct _Abs< uint, X >
		{
			static const uint	value = X;
		};

		template <ulong X>
		struct _Abs< ulong, X >
		{
			static const ulong	value = X;
		};

	}	// _ctime_hidden_
	
	template <typename T, T X>
	static constexpr T Abs	= _ctime_hidden_::_Abs<T, X>::value;



	//
	// Sign, SignOrZero
	//
	
	// 1 or -1
	template <typename T, T X>
	static constexpr int Sign		= ( X >= T(0) ? 1 : -1 );

	// 1 or 0 or -1
	template <typename T, T X>
	static constexpr int SignOrZero	= ( X > T(0) ) - ( X < T(0) );

	

	//
	// Compile-time Pow
	//
	
	namespace _ctime_hidden_
	{
		template <typename T, T X, uint Power>
		struct _Pow
		{
			STATIC_ASSERT( Power < 0xFF );
			static const T	value = T( X * _Pow< T, X, Power-1 >::value );
		};
	
		template <typename T, T X>
		struct _Pow <T, X, 0>
		{
			static const T	value = 1;
		};

	}	// _ctime_hidden_

	template <typename T, T X, uint Power>
	static constexpr T Pow	= _ctime_hidden_::_Pow< T, X, Power >::value;
	


	//
	// Power Of Two (compile time)
	//

	namespace _ctime_hidden_
	{
		template <typename T, T X>
		struct _IsPOT {
			static const bool	value	= ( X != 0 and ( (X & (X - T(1))) == T(0) ) );
		};
		
		template <typename T, T X, uint Bit>
		struct _CmpBits {
			static const uint	value	= (X >> Bit) != 0;
		};

		template <typename T, T X, uint Bit>
		struct __GetPOT {
			static const uint	value	= _CmpBits<T, X, Bit>::value + __GetPOT<T, X, Bit-1>::value;
		};
		
		template <typename T, T X>
		struct __GetPOT <T, X, 0> {
			static const uint	value	= 0;
		};

		template <typename T, T X>
		struct _GetPOT {
			static const uint	value	= __GetPOT<T, X, SizeOf<T>::bits-1 >::value;
		};

		template <typename T, T X>
		struct _NearPowerOfTwo
		{
			static const T		_x	 = Abs<T,X>;
			static const uint	_pot = IntLog2<T,X>;
			static const T		_a	 = T(1) << (_pot + 1);
			static const T		_b	 = T(1) << _pot;
			static const T		_c	 = ((_a - _b) >> 1) + _b;
			static const T		value = ( _x >= _c ? _a : _b ) * Sign<T,X>;
		};

	}	// _ctime_hidden_

	template <typename T, T X>
	static constexpr bool IsPowerOfTwo	= _ctime_hidden_::_IsPOT< T, Abs<T,X> >::value;

	template <typename T, T X>
	struct MustBePowerOfTwo
	{
		STATIC_ASSERT( (IsPowerOfTwo<T, X>), "value must be power of two" );
	};

	template <typename T, T X>
	static constexpr uint IntLog2		= _ctime_hidden_::_GetPOT< T, Abs< T, X > >::value;

	template <typename T, T X>
	static constexpr T CeilPowerOfTwo	= ( T(1) << ( IntLog2<T,X> + not IsPowerOfTwo<T,X> ) ) * Sign<T,X>;

	template <typename T, T X>
	static constexpr T FloorPowerOfTwo	= ( T(1) << IntLog2<T,X> ) * Sign<T,X>;

	template <typename T, T X>
	static constexpr T NearPowerOfTwo	= _ctime_hidden_::_NearPowerOfTwo<T, X>::value;



	//
	// Greatest common divisor
	//
	
	namespace _ctime_hidden_
	{
		template <typename T, T Value0, T Value1>
		struct _GreatestCommonDivisor
		{
			static const T	value = SwitchType< Value1 != T(0),
										_GreatestCommonDivisor< T, Value1, Value0 % Value1 >,
										ValueToType< T, Value0 > >::value;
		};

	}	// _ctime_hidden_

	template <typename T, T Value0, T Value1>
	static constexpr T GreatestCommonDivisor	= _ctime_hidden_::_GreatestCommonDivisor< T, Value0, Value1 >::value;


	
	//
	// Min
	//

	template <typename T, T a, T b>
	static constexpr T Min		= ( a < b ? a : b );

	
	//
	// Max
	//

	template <typename T, T a, T b>
	static constexpr T Max		= ( a > b ? a : b );


	//
	// Clamp
	//

	template <typename T, T x, T minValue, T maxValue>
	static constexpr T Clamp	= Min< T, maxValue, Max< T, x, minValue > >;



	//
	// AlignToLarge
	//
	
	namespace _ctime_hidden_
	{
		template <typename T, T Value, uint AlignTo>
		struct _AlignToLarge
		{
			STATIC_ASSERT( (IsPowerOfTwo< uint, AlignTo >), "align must be power of two!" );

			static const uint	_ALIGN_POT	= IntLog2< uint, AlignTo >;
			static const uint	_ALIGN_MASK	= (1 << _ALIGN_POT) - 1;
			static const uint	value		= ((Value + _ALIGN_MASK) & ~_ALIGN_MASK);
		};

	}	// _ctime_hidden_
	
	template <typename T, T Value, uint AlignTo>
	static constexpr T AlignToLarge	= _ctime_hidden_::_AlignToLarge< T, Value, AlignTo >::value;



	//
	// ToMask
	//

	namespace _ctime_hidden_
	{
		template <typename T, uint BitIndex>
		struct _ToMask
		{
			static const uint	value =	BitIndex < 0 ? T(0) :
											BitIndex < CompileTime::SizeOf<T>::bits ?
												(T(1) << (BitIndex & (CompileTime::SizeOf<T>::bits-1))) - 1 :
												T(-1);
		};

	}	// _ctime_hidden_

	template <typename T, uint BitIndex>
	static constexpr T ToMask	= _ctime_hidden_::_ToMask< T, BitIndex >::value;

	template <typename T, uint FirstBitIndex, uint LastBitIndex>
	static constexpr T ToMask2	= ToMask<T, FirstBitIndex> ^ ToMask<T, LastBitIndex>;



	//
	// BitScanReverse, BitScanForward
	//

	namespace _ctime_hidden_
	{
		template <typename T, T X, uint Bit>
		struct _RecursiveBitScanReverse
		{
			static const uint	value = uint( (X >> Bit) > 0 ) + _RecursiveBitScanReverse< T, X, Bit-1 >::value;
		};
		
		template <typename T, T X>
		struct _RecursiveBitScanReverse< T, X, 0 >
		{
			static const uint	value = uint( X > 0 );
		};
		

		template <typename T, T X, uint Bit>
		struct _RecursiveBitScanForward
		{
			static const uint	value = uint( (X << Bit) > 0 ) + _RecursiveBitScanForward< T, X, Bit-1 >::value;
		};
		
		template <typename T, T X>
		struct _RecursiveBitScanForward< T, X, 0 >
		{
			static const uint	value = uint( X > 0 );
		};
		

		template <typename T, T X>
		struct _BitScanReverse
		{
			static const uint value = _RecursiveBitScanReverse< T, X, CompileTime::SizeOf<T>::bits-1 >::value - 1;
		};
		
		template <typename T, T X>
		struct _BitScanForward
		{
			static const uint bits	= _RecursiveBitScanForward< T, X, CompileTime::SizeOf<T>::bits-1 >::value;
			static const uint value	= bits > 0 ? CompileTime::SizeOf<T>::bits - bits : -1;
		};


		template <typename T, T X>
		struct _BitScanReverseU {};
		
		template <uint X>
		struct _BitScanReverseU< uint, X > { static const uint value = _BitScanReverse< uint, X >::value; };
		
		template <ulong X>
		struct _BitScanReverseU< ulong, X > { static const uint value = _BitScanReverse< ulong, X >::value; };

		
		template <typename T, T X>
		struct _BitScanForwardU {};
		
		template <uint X>
		struct _BitScanForwardU< uint, X > { static const uint value = _BitScanForward< uint, X >::value; };
		
		template <ulong X>
		struct _BitScanForwardU< ulong, X > { static const uint value = _BitScanForward< ulong, X >::value; };

	}	// _ctime_hidden_
	
	template <typename T, T X>
	static constexpr uint	BitScanReverse	= _ctime_hidden_::_BitScanReverseU< T, X >::value;
	
	template <typename T, T X>
	static constexpr uint	BitScanForward	= _ctime_hidden_::_BitScanForwardU< T, X >::value;


}	// CompileTime
}	// GX_STL
