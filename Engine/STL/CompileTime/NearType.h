// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "NewTypeInfo.h"
#include "Engine/STL/Types/Noninstancable.h"

namespace GX_STL
{
namespace CompileTime
{


	//
	// IntegerMinLimit, IntegerMaxLimit
	//

	namespace _ctime_hidden_
	{
		template <typename T>
		struct Int_IsSigned
		{
			STATIC_ASSERT( IsInteger<T> and IsArithmetic<T> );
			static const bool	value = ( T(1) > T(-1) );
		};

	}	// _ctime_hidden_

	template <typename T>
	static constexpr T IntegerMinLimit = _ctime_hidden_::Int_IsSigned<T>::value ? T( T(1) << (SizeOf<T>::bits-1) ) : T(0);
	
	template <typename T>
	static constexpr T IntegerMaxLimit = _ctime_hidden_::Int_IsSigned<T>::value ? T( T(-1) & T(~IntegerMinLimit<T>) ) : T(-1);


	
	//
	// Near Integer Type
	//
	
	struct NearInt : Noninstancable
	{
	private:
		template <usize SizeValue>
		struct _FromSize {
			typedef typename CompileTime::SwitchType< (SizeValue <= sizeof(byte)), byte,
						typename CompileTime::SwitchType< (SizeValue <= sizeof(short)), short,
							typename CompileTime::SwitchType< (SizeValue <= sizeof(int)), int,
								ilong > > >		type;
		};
		
		template <isize Value>
		struct _FromValue {
			typedef typename CompileTime::SwitchType< (Value >= IntegerMinLimit<byte> and Value <= IntegerMaxLimit<byte>), byte,
						typename CompileTime::SwitchType< (Value >= IntegerMinLimit<short> and Value <= IntegerMaxLimit<short>), short,
							typename CompileTime::SwitchType< (Value >= IntegerMinLimit<int> and Value <= IntegerMaxLimit<int>), int, ilong >
						> >		type;
		};
		
		template <typename T>
		struct _FromScalar {
			STATIC_ASSERT( IsScalarOrEnum<T> );
			typedef typename _FromSize< SizeOf<T>::bytes >::type	type;
		};

		template <typename T>
		struct _FromVector {
			STATIC_ASSERT( IsVector<T> );
			typedef typename TypeInfo<T>::template CreateWith<
				typename _FromScalar< typename TypeInfo<T>::inner_type >::type >	type;
		};

		template <typename T>
		struct _FromType {
			//typedef typename _FromScalar<T>::type	type;
			typedef typename SwitchType< IsVector<T>, _FromVector<T>, _FromScalar<T> >::type	type;
		};

	public:
		template <usize Size>
		using FromSize = typename _FromSize< Size >::type;

		template <usize Bits>
		using FromBits = typename _FromSize< ((Bits+7) >> 3) >::type;

		template <typename T>
		using FromType = typename _FromType<T>::type;
	};

	struct NearUInt : Noninstancable
	{
	private:
		template <usize SizeValue>
		struct _FromSize {
			typedef typename CompileTime::SwitchType< (SizeValue <= sizeof(ubyte)), ubyte,
						typename CompileTime::SwitchType< (SizeValue <= sizeof(ushort)), ushort,
							typename CompileTime::SwitchType< (SizeValue <= sizeof(uint)), uint,
								ulong > > >		type;
		};

		template <usize Value>
		struct _FromValue {
			typedef typename CompileTime::SwitchType< (Value <= IntegerMaxLimit<ubyte>), ubyte,
						typename CompileTime::SwitchType< (Value <= IntegerMaxLimit<ushort>), ushort,
							typename CompileTime::SwitchType< (Value <= IntegerMaxLimit<uint>), uint, ulong >
						> >						type;
		};
		
		template <typename T>
		struct _FromScalar {
			STATIC_ASSERT( IsScalarOrEnum<T> );
			typedef typename _FromSize< SizeOf<T>::bytes >::type	type;
		};

		template <typename T>
		struct _FromVector {
			STATIC_ASSERT( IsVector<T> );
			typedef typename TypeInfo<T>::template CreateWith<
				typename _FromScalar< typename TypeInfo<T>::inner_type >::type >	type;
		};
		
		template <typename T>
		struct _FromType {
			typedef typename SwitchType< IsVector<T>, _FromVector<T>, _FromScalar<T> >::type	type;
		};

	public:
		template <usize Size>
		using FromSize = typename _FromSize< Size >::type;

		template <usize Bits>
		using FromBits = typename _FromSize< ((Bits+7) >> 3) >::type;

		template <typename T>
		using FromType = typename _FromType<T>::type;
	};



	//
	// Larger Integer Type
	//
	
	struct LargerInt : Noninstancable
	{
	private:
		template <usize Size>
		struct _FromSize {
			typedef typename NearInt::FromSize< Clamp< uint, (Size << 1), sizeof(byte), sizeof(ilong) > >	type;
		};
		
		template <typename T>
		struct _FromScalar {
			STATIC_ASSERT( IsScalarOrEnum<T> );
			typedef typename _FromSize< SizeOf<T>::bytes >::type	type;
		};

		template <typename T>
		struct _FromVector {
			STATIC_ASSERT( IsVector<T> );
			typedef typename TypeInfo<T>::template CreateWith<
				typename _FromScalar< typename TypeInfo<T>::inner_type >::type >	type;
		};
		
		template <typename T>
		struct _FromType {
			typedef typename SwitchType< IsVector<T>, _FromVector<T>, _FromScalar<T> >::type	type;
		};

	public:
		template <usize Size>
		using FromSize = typename _FromSize< Size >::type;

		template <typename T>
		using FromType = typename _FromType< T >::type;
	};

	
	struct LargerUInt : Noninstancable
	{
	private:
		template <usize Size>
		struct _FromSize {
			typedef typename NearUInt::FromSize< Clamp< uint, (Size << 1), sizeof(ubyte), sizeof(ulong) > >	type;
		};

		template <typename T>
		struct _FromScalar {
			STATIC_ASSERT( IsScalarOrEnum<T> );
			typedef typename _FromSize< SizeOf<T>::bytes >::type	type;
		};

		template <typename T>
		struct _FromVector {
			STATIC_ASSERT( IsVector<T> );
			typedef typename TypeInfo<T>::template CreateWith<
				typename _FromScalar< typename TypeInfo<T>::inner_type >::type >	type;
		};
		
		template <typename T>
		struct _FromType {
			typedef typename SwitchType< IsVector<T>, _FromVector<T>, _FromScalar<T> >::type	type;
		};

	public:
		template <usize Size>
		using FromSize = typename _FromSize< Size >::type;

		template <typename T>
		using FromType = typename _FromType< T >::type;
	};



	//
	// Lesser Integer Type
	//
	
	struct LesserInt : Noninstancable
	{
	private:
		template <usize Size>
		struct _FromSize {
			typedef typename NearInt::FromSize< Clamp< uint, (Size >> 1), sizeof(byte), sizeof(ilong) > >	type;
		};
		
		template <typename T>
		struct _FromScalar {
			STATIC_ASSERT( IsScalarOrEnum<T> );
			typedef typename _FromSize< SizeOf<T>::bytes >::type	type;
		};

		template <typename T>
		struct _FromVector {
			STATIC_ASSERT( IsVector<T> );
			typedef typename TypeInfo<T>::template CreateWith<
				typename _FromScalar< typename TypeInfo<T>::inner_type >::type >	type;
		};
		
		template <typename T>
		struct _FromType {
			typedef typename SwitchType< IsVector<T>, _FromVector<T>, _FromScalar<T> >::type	type;
		};

	public:
		template <usize Size>
		using FromSize = typename _FromSize< Size >::type;

		template <typename T>
		using FromType = typename _FromType< T >::type;
	};

	
	struct LesserUInt : Noninstancable
	{
	private:
		template <usize Size>
		struct _FromSize {
			typedef typename NearUInt::FromSize< Clamp< uint, (Size >> 1), sizeof(ubyte), sizeof(ulong) > >	type;
		};
		
		template <typename T>
		struct _FromScalar {
			STATIC_ASSERT( IsScalarOrEnum<T> );
			typedef typename _FromSize< SizeOf<T>::bytes >::type	type;
		};

		template <typename T>
		struct _FromVector {
			STATIC_ASSERT( IsVector<T> );
			typedef typename TypeInfo<T>::template CreateWith<
				typename _FromScalar< typename TypeInfo<T>::inner_type >::type >	type;
		};
		
		template <typename T>
		struct _FromType {
			typedef typename SwitchType< IsVector<T>, _FromVector<T>, _FromScalar<T> >::type	type;
		};

	public:
		template <usize Size>
		using FromSize = typename _FromSize< Size >::type;

		template <typename T>
		using FromType = typename _FromType< T >::type;
	};



	//
	// Near Float Type
	//

	struct NearFloat : Noninstancable
	{
	private:
		template <usize Size>
		struct _FromSize {
			typedef typename CompileTime::SwitchType< (Size <= sizeof(float)), float,
							typename CompileTime::SwitchType< (Size <= sizeof(double)), double,
								long double > >		type;
		};

		template <typename T>
		struct _FromScalar {
			STATIC_ASSERT( IsScalarOrEnum<T> );
			typedef typename _FromSize< SizeOf<T>::bytes >::type	type;
		};

		template <typename T>
		struct _FromVector {
			STATIC_ASSERT( IsVector<T> );
			typedef typename TypeInfo<T>::template CreateWith<
				typename _FromScalar< typename TypeInfo<T>::inner_type >::type >	type;
		};
		
		template <typename T>
		struct _FromType {
			typedef typename SwitchType< IsVector<T>, _FromVector<T>, _FromScalar<T> >::type	type;
		};

	public:
		template <usize Size>
		using FromSize = typename _FromSize< Size >::type;

		template <typename T>
		using FromType = typename _FromType<T>::type;
	};


}	// CompileTime
}	// GX_STL
