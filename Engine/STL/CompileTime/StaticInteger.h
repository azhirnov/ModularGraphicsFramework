// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "TemplateMath.h"
#include "Engine/STL/Types/Noninstancable.h"

#ifdef GX_PHYSICS_DIMENSIONS_ENABLED

namespace GX_STL
{
namespace CompileTime
{

	namespace _ctime_hidden_
	{
#		pragma warning (push)
#		pragma warning (disable: 4307)	// 'operator' : integral constant overflow
		
		template <bool Condition, typename T, T Left, T Right, T Default = T(0)>
		struct MulIf {
			static const T	value = Left * Right;
		};
	
		template <typename T, T Left, T Right, T Default>
		struct MulIf< false, T, Left, Right, Default > {
			static const T	value = Default;
		};


		template <typename T, T Left, T Right>
		struct StaticInteger_CanAdd {
			static const bool	value = ( Right <= (IntegerMaxLimit<T> - Left) );
		};

		template <typename T, T Left, T Right>
		struct StaticInteger_CanSub {
			static const bool	value = ( Right >= (IntegerMinLimit<T> + Left) );
		};
		
		template <typename T, T Left, T Right>
		struct StaticInteger_CanMul {
			static const bool	value = ( ( Right <= (IntegerMaxLimit<T> / Left) ) and
										  ( Right >= (IntegerMinLimit<T> / Left) ) );
		};
		
		template <typename T, T Left, T Right>
		struct StaticInteger_CanDiv {
			static const bool	value = ( ( Left <= (IntegerMaxLimit<T> * Right) ) and 
										  ( Left >= (IntegerMinLimit<T> * Right) ) );
		};

		template <typename T, T Left, T Right>
		struct StaticInteger_CanAddWithSign {
			static const bool	value = typename SwitchType< (Left >= 0 and Right >= 0) or (Left < 0 and Right < 0),
											StaticInteger_CanAdd< T, (Abs< T, Left >), (Abs< T, Right >) >,	// ++/--
											typename SwitchType< (Left > 0),
												StaticInteger_CanSub< T, Left, T(0)-Right >,		// +-
												StaticInteger_CanSub< T, Right, T(0)-Left > >		// -+
										>::value;
		};

		template <typename T, T Left, T Right>
		struct StaticInteger_CanMulWithSign {
			static const bool	value = typename SwitchType< (Left == 0 or Right == 0),
											StaticInteger_CanMul< T, 1, 1 >,			// 00
											typename SwitchType< ((Left >= 0 and Right >= 0) or (Left < 0 and Right < 0)),
												StaticInteger_CanMul< T, (Abs< T, Left >), (Abs< T, Right >) >,	// ++/--
												typename SwitchType< (Left > 0),
													StaticInteger_CanMul< T, Left, Right >,			// +-
													StaticInteger_CanMul< T, Right, Left > >		// -+
											>
										>::value;
		};
		
		template <typename T, T Value, T Base, uint Power>
		struct StaticInteger_CanPowRecursive {
			static const bool	_b	  = (IntegerMaxLimit<T> / Base) >= Value;
			static const T		_v	  = MulIf< _b, T, Value, Base >::value;
			static const bool	value = _b and StaticInteger_CanPowRecursive< T, _v, Base, Power-1 >::value;
		};

		template <typename T, T Value, T Base>
		struct StaticInteger_CanPowRecursive< T, Value, Base, 0 > {
			static const bool	value = true;
		};
			   
		template <typename T, T Value, int Power>
		struct StaticInteger_CanPow {
			static const T		_val  = Abs< T, Value >;
			static const bool	value = StaticInteger_CanPowRecursive< T, _val, _val, (Max< int, Power-1, 0 >) >::value;
		};
		
#		pragma warning (pop)

	}	// _ctime_hidden_



	//
	// Static Integer
	//

	template <typename T, T Value>
	struct StaticInteger : public Noninstancable
	{
	// types
	public:
		typedef StaticInteger< T, Value >		Self;
		typedef T								value_t;
		typedef typename NearInt::FromType<T>	signed_t;
		typedef typename NearUInt::FromType<T>	unsigned_t;
		
		static const T		value	= Value;

		
	// types (methods)
	private:
		template <T RightValue>
		struct _AddInt {
			STATIC_ASSERT( (_ctime_hidden_::StaticInteger_CanAddWithSign< T, value, RightValue >::value), "overflow!" );

			typedef StaticInteger< T, value + RightValue >		type;
		};
		
		template <T RightValue>
		struct _MulInt {
			STATIC_ASSERT( (_ctime_hidden_::StaticInteger_CanMulWithSign< T, value, RightValue >::value), "overflow!" );

			typedef StaticInteger< T, value * RightValue >		type;
		};

		template <int ToPower>
		struct _PowInt {
			STATIC_ASSERT( (_ctime_hidden_::StaticInteger_CanPow< T, value, ToPower >::value), "overflow!" );

			typedef StaticInteger< T, CompileTime::Pow< T, value, ToPower > >	type;
		};

	public:
		struct IsSigned {
			static const bool	value = CompileTime::IsSigned<T>;
		};
		
		template <T RightValue>
		using AddInt = typename _AddInt<RightValue>::type;
		
		template <typename RightType>
		using Add = typename AddInt< RightType::value >;
		
		template <T RightValue>
		using SubInt = typename AddInt< T(0)-RightValue >;

		template <typename RightType>
		using Sub = typename SubInt< RightType::value >;
		
		template <T RightValue>
		using MulInt = typename _MulInt< RightValue >::type;

		template <typename RightType>
		using Mul = typename _MulInt< RightType::value >::type;

		template <T RightValue>
		using DivInt = StaticInteger< T, value / RightValue >;

		template <typename RightType>
		using Div = typename DivInt< RightType::value >;
		
		template <int ToPower>
		using PowInt = typename _PowInt< ToPower >::type;

		template <typename ToPower>
		using Pow = typename PowInt< ToPower::value >;


	// methods
	public:
		operator T () const		{ return value; }
	};



	template <byte Value>	using StaticByte	= StaticInteger<byte,  Value>;
	template <short Value>	using StaticShort	= StaticInteger<short, Value>;
	template <int Value>	using StaticInt		= StaticInteger<int,   Value>;
	template <ilong Value>	using StaticILong	= StaticInteger<ilong, Value>;
	
	template <ubyte Value>	using StaticUByte	= StaticInteger<ubyte,  Value>;
	template <ushort Value>	using StaticUShort	= StaticInteger<ushort, Value>;
	template <uint Value>	using StaticUInt	= StaticInteger<uint,   Value>;
	template <ulong Value>	using StaticULong	= StaticInteger<ulong,  Value>;


}	// CompileTime
}	// GXTypes

#endif	// GX_PHYSICS_DIMENSIONS_ENABLED
