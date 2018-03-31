// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/CompileTime/TypeListHelpers.h"
#include "Engine/STL/Common/Cast.h"

namespace GX_STL
{
namespace GXTypes
{
	
	template <typename T> struct Bytes;		// literals:	b, Kb, Mb, Gb 
	template <typename T> struct Bits;		// literals:	bit, Kbit, Mbit, Gbit

	typedef Bytes<usize>	BytesU;
	typedef Bytes<isize>	BytesI;
	typedef Bits<usize>		BitsU;
	typedef Bits<isize>		BitsI;

	typedef Bytes<ulong>	BytesUL;
	typedef Bytes<ilong>	BytesL;
	typedef Bits<ulong>		BitsUL;
	typedef Bits<ilong>		BitsL;



	//
	// Bytes
	//

	template <typename T>
	struct Bytes :	public CompileTime::CopyQualifiers< T >
	{
		STATIC_ASSERT( CompileTime::IsInteger<T> and
					   CompileTime::IsScalar<T> );

	// types
	public:
		typedef Bytes<T>							Self;
		typedef T									Value_t;
		typedef CompileTime::NearInt::FromType<T>	Int_t;
		typedef CompileTime::NearUInt::FromType<T>	UInt_t;


	// variables
	private:
		T	_value;


	// methods
	public:
		constexpr Bytes (GX_DEFCTOR) : _value(0) {}

		explicit constexpr Bytes (T value) : _value(value) {}
		
		template <typename B>
		explicit constexpr Bytes (Bits<B> bits);

		template <typename B>
		explicit constexpr Bytes (Bytes<B> bytes) : _value( (T)(B)bytes ) {}

		constexpr Bits<T> ToBits () const;
		
		template <typename B>
		constexpr Bits<B> ToBits () const;

		template <typename B>
		constexpr Bytes<B> To () const;

		explicit constexpr operator byte ()		const	{ return Cast<byte>( _value ); }
		explicit constexpr operator short ()	const	{ return Cast<short>( _value ); }
		explicit constexpr operator int ()		const	{ return Cast<int>( _value ); }
		explicit constexpr operator ilong ()	const	{ return Cast<ilong>( _value ); }

		explicit constexpr operator ubyte ()	const	{ return Cast<ubyte>( _value ); }
		explicit constexpr operator ushort ()	const	{ return Cast<ushort>( _value ); }
		explicit constexpr operator uint ()		const	{ return Cast<uint>( _value ); }
		explicit constexpr operator ulong ()	const	{ return Cast<ulong>( _value ); }

						void *	ToVoidPtr ()			{ return ReferenceCast<void *>( usize( *this ) ); }
		constexpr const void *	ToVoidPtr ()	const	{ return ReferenceCast<const void *>( usize( *this ) ); }

		// move any pointer
		template <typename B>
		CHECKRES friend B const*	operator + (B const *left, const Bytes<T> &right)
		{
			return PointerCast<B>( PointerCast<byte>(left) + right._value );
		}
		
		template <typename B>
		CHECKRES friend B *			operator + (B *left, const Bytes<T> &right)
		{
			return PointerCast<B>( PointerCast<byte>(left) + right._value );
		}

		template <typename B>
		CHECKRES friend B const*	operator - (B const *left, const Bytes<T> &right)
		{
			return PointerCast<B>( PointerCast<byte>(left) - right._value );
		}
		
		template <typename B>
		CHECKRES friend B *			operator - (B *left, const Bytes<T> &right)
		{
			return PointerCast<B>( PointerCast<byte>(left) - right._value );
		}


		CHECKRES constexpr T		Kb ()	const			{ return _value >> 10; }
		CHECKRES constexpr T		Mb ()	const			{ return _value >> 20; }
		CHECKRES constexpr T		Gb ()	const			{ return _value >> 30; }
		
		template <typename B>
		CHECKRES constexpr B		Kb ()	const			{ return B(_value) / (B(1) << 10); }

		template <typename B>
		CHECKRES constexpr B		Mb ()	const			{ return B(_value) / (B(1) << 20); }

		template <typename B>
		CHECKRES constexpr B		Gb ()	const			{ return B(_value) / (B(1) << 30); }

		Self &	SetBytes (T value)							{ return (*this = FromBytes( value )); }
		Self &	SetKb (T value)								{ return (*this = FromBytes( value )); }
		Self &	SetMb (T value)								{ return (*this = FromBytes( value )); }
		Self &	SetGb (T value)								{ return (*this = FromBytes( value )); }

		CHECKRES static constexpr Self	FromBytes (T value)	{ return Self( value ); }
		CHECKRES static constexpr Self	FromKb (T value)	{ return Self( value << 10 ); }
		CHECKRES static constexpr Self	FromMb (T value)	{ return Self( value << 20 ); }
		CHECKRES static constexpr Self	FromGb (T value)	{ return Self( value << 30 ); }

		template <typename B>	CHECKRES static constexpr Self	SizeOf ()			{ return Self( sizeof(B) ); }
		template <typename B>	CHECKRES static constexpr Self	SizeOf (const B &)	{ return Self( sizeof(B) ); }
		template <typename B>	CHECKRES static constexpr Self	AlignOf ()			{ return Self( alignof(B) ); }
		template <typename B>	CHECKRES static constexpr Self	AlignOf (const B &)	{ return Self( alignof(B) ); }


		_GX_DIM_ALL_INTEGER_OPERATORS_SELF( _value );
		_GX_DIM_ALL_INTEGER_OPERATORS_TYPE( _value, const Value_t&, );
	};



	//
	// Bits
	//

	template <typename T>
	struct Bits : public CompileTime::CopyQualifiers< T >
	{
		STATIC_ASSERT( CompileTime::IsInteger<T> );

	// types
	public:
		typedef Bits<T>								Self;
		typedef T									Value_t;
		typedef CompileTime::NearInt::FromType<T>	Int_t;
		typedef CompileTime::NearUInt::FromType<T>	UInt_t;


	// variables
	private:
		T	_value;
		

	// methods
	public:
		constexpr Bits (GX_DEFCTOR) : _value(0) {}

		explicit constexpr Bits (T value) : _value(value) {}
		
		template <typename B>
		explicit constexpr Bits (Bytes<B> bytes);
		
		template <typename B>
		explicit constexpr Bits (Bits<B> bits) : _value( (T)(B)bits ) {}

		constexpr Bytes<T> ToBytes () const;
		
		template <typename B>
		constexpr Bytes<B> ToBytes () const;
		
		template <typename B>
		constexpr Bits<B> To () const;
		
		CHECKRES explicit constexpr operator byte ()	const	{ return Cast<byte>( _value ); }
		CHECKRES explicit constexpr operator short ()	const	{ return Cast<short>( _value ); }
		CHECKRES explicit constexpr operator int ()		const	{ return Cast<int>( _value ); }
		CHECKRES explicit constexpr operator ilong ()	const	{ return Cast<ilong>( _value ); }

		CHECKRES explicit constexpr operator ubyte ()	const	{ return Cast<ubyte>( _value ); }
		CHECKRES explicit constexpr operator ushort ()	const	{ return Cast<ushort>( _value ); }
		CHECKRES explicit constexpr operator uint ()	const	{ return Cast<uint>( _value ); }
		CHECKRES explicit constexpr operator ulong ()	const	{ return Cast<ulong>( _value ); }
		

		template <typename B>	CHECKRES static constexpr Self SizeOf ()			{ return Bytes<T>::template SizeOf<B>().ToBits(); }
		template <typename B>	CHECKRES static constexpr Self SizeOf (const B &)	{ return Bytes<T>::template SizeOf<B>().ToBits(); }
		template <typename B>	CHECKRES static constexpr Self AlignOf ()			{ return Bytes<T>::template AlignOf<B>().ToBits(); }
		template <typename B>	CHECKRES static constexpr Self AlignOf (const B &)	{ return Bytes<T>::template AlignOf<B>().ToBits(); }


		_GX_DIM_ALL_INTEGER_OPERATORS_SELF( _value );
		_GX_DIM_ALL_INTEGER_OPERATORS_TYPE( _value, const Value_t&, );
	};




	template <typename T>
	static constexpr BytesU SizeOf = BytesU::SizeOf<T>();
	
	template <typename T>
	static constexpr BytesU	AlignOf = BytesU::AlignOf<T>();

	template <typename A, typename B>
	CHECKRES constexpr forceinline BytesU OffsetOf (A (B::*member))
	{
		const union U {
			B		b;
			int		tmp;
			U () : tmp(0) {}
			~U () {}
		} u;
		return BytesU( ReferenceCast<usize>(&(u.b.*member)) - ReferenceCast<usize>(&u.b) );
	}



//----------------------------------- Bytes --------------------------------//
	
	template <typename T>
	template <typename B>
	constexpr Bytes<T>::Bytes (Bits<B> bits) : _value( T((B)bits) >> 3 )
	{}


	template <typename T>
	CHECKRES constexpr Bits<T>  Bytes<T>::ToBits () const
	{
		return Bits<T>( *this );
	}
	

	template <typename T>
	template <typename B>
	CHECKRES constexpr Bits<B>  Bytes<T>::ToBits () const
	{
		return Bits<B>( *this );
	}
	

	template <typename T>
	template <typename B>
	CHECKRES constexpr Bytes<B>  Bytes<T>::To () const
	{
		return Bytes<B>( (B)_value );
	}
	

//----------------------------------- Bits ---------------------------------//

	template <typename T>
	template <typename B>
	constexpr Bits<T>::Bits (Bytes<B> bytes) : _value( T((B)bytes) << 3 )
	{}
	

	template <typename T>
	CHECKRES constexpr Bytes<T>  Bits<T>::ToBytes () const
	{
		return Bytes<T>( *this );
	}
	

	template <typename T>
	template <typename B>
	CHECKRES constexpr Bytes<B>  Bits<T>::ToBytes () const
	{
		return Bytes<B>( *this );
	}
	

	template <typename T>
	template <typename B>
	CHECKRES constexpr Bits<B>  Bits<T>::To () const
	{
		return Bits<B>( (B) _value );
	}

	

	CHECKRES constexpr BytesU operator "" _b (unsigned long long value)		{ return BytesU::FromBytes( usize(value) ); }
	CHECKRES constexpr BytesU operator "" _Kb (unsigned long long value)	{ return BytesU::FromKb( usize(value) ); }
	CHECKRES constexpr BytesU operator "" _Mb (unsigned long long value)	{ return BytesU::FromMb( usize(value) ); }
	CHECKRES constexpr BytesU operator "" _Gb (unsigned long long value)	{ return BytesU::FromGb( usize(value) ); }

	CHECKRES constexpr BitsU  operator "" _bit (unsigned long long value)	{ return BitsU(usize(value)); }

	
/*
=================================================
	Hash
=================================================
*/
	template <typename T>
	struct Hash< Bytes<T> >
	{
		CHECKRES HashResult  operator () (const Bytes<T> &x) const
		{
			return HashOf( T(x) );
		}
	};


	template <typename T>
	struct Hash< Bits<T> >
	{
		CHECKRES HashResult  operator () (const Bits<T> &x) const
		{
			return HashOf( T(x) );
		}
	};

}	// GXTypes

namespace CompileTime
{
/*
=================================================
	TypeInfo
=================================================
*/
	template <typename T>
	struct TypeInfo< GXTypes::Bytes<T> >
	{
		typedef GXTypes::Bytes<T>	type;
		typedef T					inner_type;
		
		template <typename OtherType>
		using CreateWith = GXTypes::Bytes< OtherType >;

		enum {
			FLAGS	= (int)TypeInfo< inner_type >::FLAGS | int(_ctime_hidden_::WRAPPER),
		};
		
		static constexpr type	Max()		{ return type( TypeInfo< inner_type >::Max() ); }
		static constexpr type	Min()		{ return type( TypeInfo< inner_type >::Min() ); }
		
		static constexpr type	Epsilon()	{ return type( TypeInfo< inner_type >::Epsilon() ); }
		static constexpr uint	SignBit()	{ return TypeInfo< inner_type >::SignBit(); }
		static constexpr uint	Count()		{ return TypeInfo< inner_type >::Count(); }
	};


	template <typename T>
	struct TypeInfo< GXTypes::Bits<T> >
	{
		typedef GXTypes::Bits<T>	type;
		typedef T					inner_type;
		
		template <typename OtherType>
		using CreateWith = GXTypes::Bits< OtherType >;

		enum {
			FLAGS	= (int)TypeInfo< inner_type >::FLAGS | int(_ctime_hidden_::WRAPPER),
		};
		
		static constexpr type	Max()		{ return type( TypeInfo< inner_type >::Max() ); }
		static constexpr type	Min()		{ return type( TypeInfo< inner_type >::Min() ); }
		
		static constexpr type	Epsilon()	{ return type( TypeInfo< inner_type >::Epsilon() ); }
		static constexpr uint	SignBit()	{ return TypeInfo< inner_type >::SignBit(); }
		static constexpr uint	Count()		{ return TypeInfo< inner_type >::Count(); }
	};

}	// CompileTime
}	// GX_STL
