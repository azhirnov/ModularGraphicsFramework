// Copyright �  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
		friend B const*	operator + (B const *left, const Bytes<T> &right)
		{
			return PointerCast<B>( PointerCast<byte>(left) + right._value );
		}
		
		template <typename B>
		friend B *		operator + (B *left, const Bytes<T> &right)
		{
			return PointerCast<B>( PointerCast<byte>(left) + right._value );
		}

		template <typename B>
		friend B const*	operator - (B const *left, const Bytes<T> &right)
		{
			return PointerCast<B>( PointerCast<byte>(left) - right._value );
		}
		
		template <typename B>
		friend B *		operator - (B *left, const Bytes<T> &right)
		{
			return PointerCast<B>( PointerCast<byte>(left) - right._value );
		}


		constexpr T		Kb ()	const				{ return _value >> 10; }
		constexpr T		Mb ()	const				{ return _value >> 20; }
		constexpr T		Gb ()	const				{ return _value >> 30; }
		
		template <typename B>
		constexpr B		Kb ()	const				{ return B(_value) / (B(1) << 10); }

		template <typename B>
		constexpr B		Mb ()	const				{ return B(_value) / (B(1) << 20); }

		template <typename B>
		constexpr B		Gb ()	const				{ return B(_value) / (B(1) << 30); }

		Self &	SetBytes (T value)					{ return (*this = FromBytes( value )); }
		Self &	SetKb (T value)						{ return (*this = FromBytes( value )); }
		Self &	SetMb (T value)						{ return (*this = FromBytes( value )); }
		Self &	SetGb (T value)						{ return (*this = FromBytes( value )); }

		static constexpr Self	FromBytes (T value)	{ return Self( value ); }
		static constexpr Self	FromKb (T value)	{ return Self( value << 10 ); }
		static constexpr Self	FromMb (T value)	{ return Self( value << 20 ); }
		static constexpr Self	FromGb (T value)	{ return Self( value << 30 ); }

		template <typename B>
		static constexpr Self	SizeOf ()			{ return Self( sizeof(B) ); }
		
		template <typename B>
		static constexpr Self	SizeOf (B value)	{ return Self( sizeof(B) ); }


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
		
		explicit constexpr operator byte ()		const	{ return Cast<byte>( _value ); }
		explicit constexpr operator short ()	const	{ return Cast<short>( _value ); }
		explicit constexpr operator int ()		const	{ return Cast<int>( _value ); }
		explicit constexpr operator ilong ()	const	{ return Cast<ilong>( _value ); }

		explicit constexpr operator ubyte ()	const	{ return Cast<ubyte>( _value ); }
		explicit constexpr operator ushort ()	const	{ return Cast<ushort>( _value ); }
		explicit constexpr operator uint ()		const	{ return Cast<uint>( _value ); }
		explicit constexpr operator ulong ()	const	{ return Cast<ulong>( _value ); }
		

		template <typename B>
		static constexpr Self SizeOf ()				{ return Bytes<T>::template SizeOf<B>().ToBits(); }
		
		template <typename B>
		static constexpr Self SizeOf (B value)		{ return Bytes<T>::template SizeOf<B>().ToBits(); }


		_GX_DIM_ALL_INTEGER_OPERATORS_SELF( _value );
		_GX_DIM_ALL_INTEGER_OPERATORS_TYPE( _value, const Value_t&, );
	};



	/*template <typename T>
	constexpr Bytes<T> ToBytes (T value)
	{
		return Bytes<T>( value );
	}
	
	template <typename T>
	constexpr Bits<T> ToBits (T value)
	{
		return Bits<T>( value );
	}*/
	

	template <typename T>
	constexpr forceinline BytesU SizeOf ()
	{
		return BytesU::SizeOf<T>();
	}
	
	template <typename T>
	constexpr forceinline BytesU SizeOf (const T&)
	{
		return BytesU::SizeOf<T>();
	}
	

	template <typename A, typename B>
	constexpr forceinline BytesU OffsetOf (A (B::*member))
	{
		const union U {
			B		b;
			int		tmp;
			U () : tmp(0) {}
		} u;
		return BytesU( usize(&(u.b.*member)) - usize(&u.b) );

		//return BytesU( offsetof( B, *member ) );
		//return BytesU( (usize) &reinterpret_cast< char const volatile &>( ((B*)null)->*member ) );
	}



//----------------------------------- Bytes --------------------------------//
	
	template <typename T>
	template <typename B>
	constexpr Bytes<T>::Bytes (Bits<B> bits) : _value( T((B)bits) >> 3 )
	{}


	template <typename T>
	constexpr Bits<T>  Bytes<T>::ToBits () const
	{
		return Bits<T>( *this );
	}
	

	template <typename T>
	template <typename B>
	constexpr Bits<B>  Bytes<T>::ToBits () const
	{
		return Bits<B>( *this );
	}
	

	template <typename T>
	template <typename B>
	constexpr Bytes<B>  Bytes<T>::To () const
	{
		return Bytes<B>( (B)_value );
	}
	

//----------------------------------- Bits ---------------------------------//

	template <typename T>
	template <typename B>
	constexpr Bits<T>::Bits (Bytes<B> bytes) : _value( T((B)bytes) << 3 )
	{}
	

	template <typename T>
	constexpr Bytes<T>  Bits<T>::ToBytes () const
	{
		return Bytes<T>( *this );
	}
	

	template <typename T>
	template <typename B>
	constexpr Bytes<B>  Bits<T>::ToBytes () const
	{
		return Bytes<B>( *this );
	}
	

	template <typename T>
	template <typename B>
	constexpr Bits<B>  Bits<T>::To () const
	{
		return Bits<B>( (B) _value );
	}

	

	constexpr BytesU operator "" _b (unsigned long long value)		{ return BytesU::FromBytes( usize(value) ); }
	constexpr BytesU operator "" _Kb (unsigned long long value)		{ return BytesU::FromKb( usize(value) ); }
	constexpr BytesU operator "" _Mb (unsigned long long value)		{ return BytesU::FromMb( usize(value) ); }
	constexpr BytesU operator "" _Gb (unsigned long long value)		{ return BytesU::FromGb( usize(value) ); }

	constexpr BitsU  operator "" _bit (unsigned long long value)	{ return BitsU(usize(value)); }

	
/*
=================================================
	Hash
=================================================
*/
	template <typename T>
	struct Hash< Bytes<T> > : private Hash<T>
	{
		typedef Bytes<T>					Key_t;
		typedef Hash<T>						Base_t;
		typedef typename Base_t::Result_t	Result_t;

		Result_t operator () (const Key_t &x) const
		{
			return Base_t::operator ()( (T)x );
		}
	};


	template <typename T>
	struct Hash< Bits<T> > : private Hash<T>
	{
		typedef Bits<T>						Key_t;
		typedef Hash<T>						Base_t;
		typedef typename Base_t::Result_t	Result_t;

		Result_t operator () (const Key_t &x) const
		{
			return Base_t::operator ()( (T)x );
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
