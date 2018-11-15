// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Memory/MemoryContainer.h"
#include "Core/STL/Containers/CopyStrategy.h"
#include "Core/STL/Containers/ArrayRef.h"
#include "Core/STL/Containers/StringRef.h"
#include "Core/STL/Math/Mathematics.h"
#include "Core/STL/Algorithms/Swap.h"

namespace GX_STL
{
namespace GXTypes
{

	namespace _types_hidden_
	{
		template <typename T, typename B>
		inline usize IntToStr (B val, T * buf, usize size, int radix);
	}



	//
	// String Float Format
	//

	struct StringFormatF
	{
	private:
		ubyte	_int;
		ubyte	_fract;
		bool	_cutZeros : 1;
		bool	_exponent : 1;

	public:
		StringFormatF () : _int(1), _fract(6), _cutZeros(true), _exponent(false)
		{}

		StringFormatF& Fmt (ubyte intPart, ubyte fractPart = 6)
		{
			_int	= intPart;
			_fract	= fractPart;
			return *this;
		}

		template <typename T>
		StringFormatF& Fmt ()
		{
			_int	= 0;
			_fract	= sizeof(T) == sizeof(float) ? 6 : (sizeof(T) == sizeof(double) ? 8 : 4);
			return *this;
		}

		StringFormatF& CutZeros (bool value = true)
		{
			_cutZeros	= value;
			return *this;
		}

		StringFormatF& Exp (bool value = true)
		{
			_exponent	= value;
			return *this;
		}

		template <usize I>
		void ToCString (char (&cstr)[I]) const
		{
			STATIC_ASSERT( I > 7 );
			cstr[0] = '%';
			cstr[1] = char('0' + _int / 10);
			cstr[2] = char('0' + _int % 10);
			cstr[3] = '.';
			cstr[4] = char('0' + _fract / 10);
			cstr[5] = char('0' + _fract % 10);
			cstr[6] = _exponent ? 'e' : 'f';
			cstr[7] = 0;
		}

		bool IsCutZeros () const
		{
			return _cutZeros;
		}

		bool IsExponent () const
		{
			return _exponent;
		}
	};

	
	#define RET_ERROR( _ret_ )	{ WARNING("error in string");  return _ret_; }
	#define RET_FALSE			RET_ERROR( false )
	#define RET_VOID			RET_ERROR( ; )



	//
	// String
	//
	
	template <	typename T,
				typename S = typename AutoDetectCopyStrategy<T>::type,
				typename MC = MemoryContainer<T>
			 >
	struct TString : public CompileTime::CopyQualifiers< CompileTime::FastCopyable, MC >
	{
	// types
	public:
		typedef TString<T,S,MC>		Self;
		typedef S					Strategy_t;
		typedef MC					MemoryContainer_t;
		typedef T					Value_t;


	// variables
	private:
		MemoryContainer_t	_memory;
		usize				_length;
		usize				_size;

		//  _____________________________
		// |  string  |  0  |  reserved  |
		// 0      _length            _size


	// methods
	private:
		usize _Count ()	const	{ return _length + 1; }

		void  _Reallocate (usize newSize, bool allowReserve = true);

		void  _Move (Self &&other);

		static bool  _CheckIntersection (const void *leftBegin, const void *leftEnd,
										 const void *rightBegin, const void *rightEnd);
		
		static usize _GetLength (const void *begin, const void *end);

	public:
		TString (GX_DEFCTOR);
		TString (Self &&other);
		TString (const Self &other);
		TString (TStringRef<const T> other);

		TString (const T *str, usize length = 0);

		explicit TString (const bool bValue);
		explicit TString (const int value);
		explicit TString (const uint uValue);
		explicit TString (const ilong value);
		explicit TString (const ulong uValue);
		explicit TString (const float fValue);
		explicit TString (const double dValue);

		~TString ();

		void Append (const void *begin, const void *end);
		void Append (TStringRef<const T> str);
		void Append (const T value)										{ return Append( TStringRef<const T>( AddressOf(value), usize(value != 0) ) ); }

		//template <typename B>
		//void Append (const B &value);

		template <typename B>
		void Copy (TStringRef<const B> str);
		void Copy (TStringRef<const T> str);

		void Free ();
		void Clear ();

		void Insert (const void *begin, const void *end, usize pos);
		void Insert (TStringRef<const T> str, usize pos);
		void Insert (const T value, usize pos)							{ return Insert( TStringRef<const T>( AddressOf(value), usize(value != 0) ), pos ); }

		void Erase (usize pos, usize count = 1);
		void Erase (const void *begin, const void *end);

		void EraseFromBack (usize count);

		bool Find (TStringRef<const T> value, OUT usize &pos, usize start = 0)	const	{ return TStringRef<const T>(*this).Find( value, OUT pos, start ); }
		bool Find (const T value, OUT usize &pos, usize start = 0)				const	{ return TStringRef<const T>(*this).Find( value, OUT pos, start ); }
		
		bool FindIC (TStringRef<const T> value, OUT usize &pos, usize start = 0)const	{ return TStringRef<const T>(*this).FindIC( value, OUT pos, start ); }
		bool FindIC (const T value, OUT usize &pos, usize start = 0)			const	{ return TStringRef<const T>(*this).FindIC( value, OUT pos, start ); }

		bool FindAndDelete (TStringRef<const T> value, OUT usize &pos, usize start = 0);

		bool FindAndChange (TStringRef<const T> searchStr, TStringRef<const T> newStr, OUT usize &pos, usize start = 0);

		void ReplaceChars (const T searchChar, const T newChar);
		void ReplaceStrings (TStringRef<const T> pattern, TStringRef<const T> newString, usize start = 0);

		void CutChars (const T value);

		void SetLength (usize length);

		void CalculateLength ();

		void PopBack ();

		void Resize (usize length, T value = T(' '), bool allowReserve = true);

		void Reserve (usize length);

		void FreeReserve ();

		ND_ TStringRef<T>		SubString (usize pos, usize count = UMax)			{ return TStringRef<T>(*this).SubString( pos, count ); }
		ND_ TStringRef<const T>	SubString (usize pos, usize count = UMax)	const	{ return TStringRef<const T>(*this).SubString( pos, count ); }
		
		template <typename B>
		Self &	FormatI (const B& value, int radix = 10);

		// 10000 -> 10 000
		template <typename B>
		Self &	FormatI (const B& value, usize stepSize, T spaceChar, int radix = 10);

		// 100 -> 000100
		template <typename B>
		Self &	FormatAlignedI (const B& value, uint align, T alignChar = T('0'), int radix = 10);

		Self &	FormatF (float value, const StringFormatF &fmt = StringFormatF());
		Self &	FormatF (double value, const StringFormatF &fmt = StringFormatF());

		ND_ bool StartsWith (TStringRef<const T> right) const				{ return TStringRef<const T>(*this).StartsWith( right ); }
		ND_ bool StartsWithIC (TStringRef<const T> right) const				{ return TStringRef<const T>(*this).StartsWithIC( right ); }

		ND_ bool EndsWith (TStringRef<const T> right) const					{ return TStringRef<const T>(*this).EndsWith( right ); }
		ND_ bool EndsWithIC (TStringRef<const T> right) const				{ return TStringRef<const T>(*this).EndsWithIC( right ); }
		
		ND_ bool HasChar (T right) const									{ return TStringRef<const T>(*this).HasChar( right ); }
		ND_ bool HasCharIC (T right) const									{ return TStringRef<const T>(*this).HasCharIC( right ); }

		ND_ bool HasSubString (TStringRef<const T> right) const				{ return TStringRef<const T>(*this).HasSubString( right ); }
		ND_ bool HasSubStringIC (TStringRef<const T> right) const			{ return TStringRef<const T>(*this).HasSubStringIC( right ); }

		ND_ bool EqualsIC (TStringRef<const T> right) const					{ return TStringRef<const T>(*this).EqualsIC( right ); }
		
		ND_ bool LessThan (TStringRef<const T> right)	const				{ return TStringRef<const T>(*this).LessThan( right ); }
		ND_ bool LessThanIC (TStringRef<const T> right)	const				{ return TStringRef<const T>(*this).LessThanIC( right ); }
		
		ND_ bool GreaterThan (TStringRef<const T> right)	const			{ return TStringRef<const T>(*this).GreaterThan( right ); }
		ND_ bool GreaterThanIC (TStringRef<const T> right)	const			{ return TStringRef<const T>(*this).GreaterThanIC( right ); }

		ND_ bool EqualsInRange (TStringRef<const T> right, usize begin, usize end) const	{ return TStringRef<const T>(*this).EqualsInRange( right, begin, end ); }
		ND_ bool EqualsInRangeIC (TStringRef<const T> right, usize begin, usize end) const	{ return TStringRef<const T>(*this).EqualsInRangeIC( right, begin, end ); }

		ND_ usize GetIndex (const T &value) const							{ return TStringRef<const T>(*this).GetIndex( value ); }

		ND_ T		*		ptr ();
		ND_ T const	*		ptr ()							const;
		ND_ T const	*		cstr ()							const		{ return ptr(); }
		ND_ usize			Length ()						const		{ return _length; }
		ND_ usize			Count ()						const		{ return Length(); }
		ND_ BytesU			LengthInBytes ()				const		{ return SizeOf<T> * Length(); }
		ND_ BytesU			Size ()							const		{ return SizeOf<T> * _Count(); }
		ND_ usize			Capacity ()						const		{ return _size; }
		ND_ constexpr usize MaxCapacity ()					const		{ return _memory.MaxSize(); }	// max available for allocation count of elements
		ND_ BytesU			FullSize ()						const		{ return _size * SizeOf<T>; }
		ND_ bool			Empty ()						const		{ return Length() == 0; }

		ND_ T		*	begin ()										{ return ptr(); }
		ND_ T const	*	begin ()							const		{ return ptr(); }
		ND_ T		*	end ()											{ return ptr() + Length(); }
		ND_ T const	*	end ()								const		{ return ptr() + Length(); }

		ND_ T		&	Front ()										{ return ptr()[0]; }
		ND_ T const	&	Front ()							const		{ return ptr()[0]; }
		ND_ T		&	Back ();
		ND_ T const	&	Back ()								const;
		
		ND_ bool		operator !  ()						const		{ return not Empty(); }
		
		ND_ operator	ArrayRef<T> ()									{ return ArrayRef<T>( _memory.Pointer(), Length() ); }
		ND_ operator	ArrayCRef<T> () const							{ return ArrayCRef<T>( _memory.Pointer(), Length() ); }
		
		ND_ operator	TStringRef<T> ()								{ return TStringRef<T>( _memory.Pointer(), Length() ); }
		ND_ operator	TStringRef<const T> () const					{ return TStringRef<const T>( _memory.Pointer(), Length() ); }

		ND_ T		&	operator [] (usize i);
		ND_ T const	&	operator [] (usize i) const;

		Self &			operator =  (const char *right)					{ Copy( TStringRef<const T>( right ) );	return *this; }
		Self &			operator =  (TStringRef<const T> right)			{ Copy( right );					return *this; }
		Self &			operator =  (const Self &right)					{ Copy( right );					return *this; }
		Self &			operator =  (Self &&right)						{ Free();  _Move( RVREF(right) );	return *this; }

		template <typename B>
		Self &			operator =  (const B &right)					{ Copy( Self( right ) );	return *this; }

		ND_ bool	operator == (TStringRef<const T> right)	const		{ return TStringRef<const T>(*this) == right; }
		ND_ bool	operator != (TStringRef<const T> right)	const		{ return not ( *this == right ); }

		template <typename B>
		ND_ Self	operator +  (const B& right)		const			{ return Self(*this) += right; }
		ND_ Self	operator +  (const Self &right)		const			{ return Self(*this) += right; }

		template <typename B> friend
		ND_ Self	operator +  (const B& left, const Self &right)		{ return Self(left) += right; }

		template <usize I>
		Self &	operator += (const T right[I])							{ return ( *this += TStringRef<const T>(right) ); }
		Self &	operator += (const T *right)							{ return ( *this += TStringRef<const T>(right) ); }
		Self &	operator += (const Self &right)							{ return ( *this += TStringRef<const T>(right) ); }
		Self &	operator += (TStringRef<const T> right)					{ Append( right );					return *this; }
		Self &	operator += (const T right)								{ Append( right );					return *this; }
		
		template <typename B>
		Self &	operator += (const B& right)							{ Append( Self( right ) );			return *this; }

		template <typename B>
		Self &	operator << (const B& right)							{ return (*this += right); }

		template <typename B>
		friend Self &	operator >> (const B& left, Self &right)			{ right.Insert( Self( left ), 0 );	return right; }
		friend Self &	operator >> (TStringRef<const T> left, Self &right)	{ right.Insert( left, 0 );			return right; }
		friend Self &	operator >> (const T left, Self &right)				{ right.Insert( left, 0 );			return right; }

		ND_ bool	operator < (TStringRef<const T> right)	const		{ return TStringRef<const T>(*this) < right; }
		ND_ bool	operator > (TStringRef<const T> right)	const		{ return TStringRef<const T>(*this) > right; }
		ND_ bool	operator <= (TStringRef<const T> right)	const		{ return TStringRef<const T>(*this) <= right; }
		ND_ bool	operator >= (TStringRef<const T> right)	const		{ return TStringRef<const T>(*this) >= right; }

		ND_ bool	NumStrLess (TStringRef<const T> right)	const		{ return TStringRef<const T>(*this).NumStrLess( right ); }

		template <typename T2, typename S2, typename A2>
		void Convert (OUT TString<T2,S2,A2> &str) const;

		
		static constexpr bool	IsLinearMemory ()						{ return true; }
		constexpr bool			IsStaticMemory ()	const				{ return _memory.IsStatic(); }


		ND_ inline friend bool operator == (const T * const left, const Self &right)
		{
			return right == left;
		}
		
		ND_ inline friend bool operator != (const T * const left, const Self &right)
		{
			return right != left;
		}
		
		friend void SwapValues (INOUT Self &left, INOUT Self &right)
		{
			left._memory.SwapMemory( right._memory );

			GXTypes::SwapValues( left._length,	right._length );
			GXTypes::SwapValues( left._size,	right._size );
		}
	};
	

	
	using String	= TString< char >;
	using WString	= TString< wchar >;


	template <typename T>
	ND_ inline String  ToString (const T &value);



	template <typename T, usize Size>
	using TStaticString = TString< T, typename AutoDetectCopyStrategy<T>::type, StaticMemoryContainer< T, Size > >;

	template <usize Size>
	using StaticString = TStaticString< char, Size >;
	
	template <usize Size>
	using StaticWString = TStaticString< wchar, Size >;


	template <typename T, typename S, typename MC>
	struct AutoDetectCopyStrategy< TString<T,S,MC> >
	{
		typedef CopyStrategy::MemCopyWithCtor< TString<T,S,MC> >			type;
	};
	
	template <typename T, usize Size>
	struct AutoDetectCopyStrategy< TStaticString<T,Size> >
	{
		typedef CopyStrategy::CopyAndMoveCtor< TStaticString<T,Size> >		type;
	};

		
/*
=================================================
	operator "" _str
=================================================
*/
	ND_ inline TString<char>		operator "" _str (const char *str, size_t len)		{ return TString<char>( str, len ); }
	ND_ inline TString<wchar_t>		operator "" _str (const wchar_t *str, size_t len)	{ return TString<wchar_t>( str, len ); }
	//inline TString<char16_t>	operator "" _str (const char16_t *str, size_t len)	{ return TString<char16_t>( str, len ); }
	//inline TString<char32_t>	operator "" _str (const char32_t *str, size_t len)	{ return TString<char32_t>( str, len ); }

/*
=================================================
	constructor
=================================================
*/
	template <typename T, typename S, typename MC>
	inline TString<T,S,MC>::TString (UninitializedType) : _length(0), _size(0)
	{}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, typename S, typename MC>
	inline TString<T,S,MC>::TString (const T *str, const usize length) : _length(0), _size(0)
	{
		Copy( TStringRef<const T>( str, length == 0 and str != null ? _types_hidden_::StrLength( str ) : length ) );
	}
		
/*
=================================================
	constructor
=================================================
*/
	template <typename T, typename S, typename MC>
	inline TString<T,S,MC>::TString (const Self &other) : _length(0), _size(0)
	{
		Copy( TStringRef<const T>( other ) );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, typename S, typename MC>
	inline TString<T,S,MC>::TString (Self &&other)
	{
		_Move( RVREF( other ) );
	}
	
/*
=================================================
	_Move
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void TString<T,S,MC>::_Move (Self &&other)
	{
		_memory	= RVREF( other._memory );
		_length	= other._length;
		_size	= other._size;

		other._length	= other._size = 0;
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, typename S, typename MC>
	inline TString<T,S,MC>::TString (TStringRef<const T> str): _length(0), _size(0)
	{
		Copy( str );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, typename S, typename MC>
	inline TString<T,S,MC>::TString (const bool bValue): _length(0), _size(0)
	{
		Copy( bValue ? "true" : "false" );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, typename S, typename MC>
	inline TString<T,S,MC>::TString (const int value): _length(0), _size(0)
	{
		FormatI( value, GlobalConst::STL_StringDefaultRadix );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, typename S, typename MC>
	inline TString<T,S,MC>::TString (const uint uValue): _length(0), _size(0)
	{
		FormatI( uValue, GlobalConst::STL_StringDefaultRadix );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, typename S, typename MC>
	inline TString<T,S,MC>::TString (const ilong value): _length(0), _size(0)
	{
		FormatI( value, GlobalConst::STL_StringDefaultRadix );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, typename S, typename MC>
	inline TString<T,S,MC>::TString (const ulong value): _length(0), _size(0)
	{
		FormatI( value, GlobalConst::STL_StringDefaultRadix );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, typename S, typename MC>
	inline TString<T,S,MC>::TString (const float fValue): _length(0), _size(0)
	{
		FormatF( fValue, StringFormatF().Fmt<float>().CutZeros() );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, typename S, typename MC>
	inline TString<T,S,MC>::TString (const double dValue): _length(0), _size(0)
	{
		FormatF( dValue, StringFormatF().Fmt<double>().CutZeros() );
	}
	
/*
=================================================
	destructor
=================================================
*/
	template <typename T, typename S, typename MC>
	inline TString<T,S,MC>::~TString ()
	{
		Free();
	}
	
/*
=================================================
	_Reallocate
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void TString<T,S,MC>::_Reallocate (const usize newSize, const bool allowReserve)
	{
		ASSERT( newSize > 0 );

		_size				= newSize;
		usize	old_count	= _Count() < newSize ? _Count() : newSize;
		
		MemoryContainer_t	old_memcont;	old_memcont.MoveFrom( INOUT _memory );

		CHECK( _memory.Allocate( INOUT _size, allowReserve ) );
		
		old_count = GXMath::Min( old_count, _size );

		if ( old_memcont.Pointer() == null or old_count <= 1 ) {
			_length = 0;
			return;
		}

		Strategy_t::Replace( _memory.Pointer(), old_memcont.Pointer(), old_count );

		_length = old_count - 1;
	}
	
/*
=================================================
	_CheckIntersection
=================================================
*/
	template <typename T, typename S, typename MC>
	inline bool TString<T,S,MC>::_CheckIntersection (const void *leftBegin, const void *leftEnd,
													 const void *rightBegin, const void *rightEnd)
	{
		return CheckPointersAliasing( leftBegin, leftEnd, rightBegin, rightEnd );
	}
	
/*
=================================================
	_GetLength
=================================================
*/
	template <typename T, typename S, typename MC>
	inline usize TString<T,S,MC>::_GetLength (const void *beginPtr, const void *endPtr)
	{
		ASSERT( beginPtr <= endPtr );
		return ( ReferenceCast<usize>(endPtr) - ReferenceCast<usize>(beginPtr) ) / sizeof(T);
	}
		
/*
=================================================
	Back
=================================================
*/
	template <typename T, typename S, typename MC>
	inline T &  TString<T,S,MC>::Back ()
	{
		ASSUME( _length >= 1 );
		return _memory.Pointer()[_length-1];
	}
	
	template <typename T, typename S, typename MC>
	inline const T & TString<T,S,MC>::Back () const
	{
		ASSUME( _length >= 1 );
		return _memory.Pointer()[_length-1];
	}
	
/*
=================================================
	operator []
=================================================
*/
	template <typename T, typename S, typename MC>
	inline T & TString<T,S,MC>::operator [] (const usize uNumb)
	{
		ASSUME( uNumb < _length );
		return _memory.Pointer()[uNumb];
	}
	
	template <typename T, typename S, typename MC>
	inline const T & TString<T,S,MC>::operator [] (const usize uNumb) const
	{
		ASSUME( uNumb < _length );
		return _memory.Pointer()[uNumb];
	}
	
/*
=================================================
	Append
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void TString<T,S,MC>::Append (TStringRef<const T> str)
	{
		if ( str.Empty() ) 
			return;

		if ( not Empty() and _CheckIntersection( begin(), end(), str.begin(), str.end() ) )
			RET_VOID;

		const usize	u_old = Length();

		if ( _length + str.Length() + 1 > _size )
			_Reallocate( _length + str.Length() + 1 );

		_length += str.Length();

		Strategy_t::Copy( _memory.Pointer() + u_old, str.ptr(), str.Length() );
		_memory.Pointer()[_length] = 0;
	}
	
/*
=================================================
	Append
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void TString<T,S,MC>::Append (const void *beginPtr, const void *endPtr)
	{
		Append( Cast<const T *>(beginPtr), _GetLength( beginPtr, endPtr ) );
	}

/*
=================================================
	Copy
=================================================
*/
	template <typename T, typename S, typename MC>
	template <typename B>
	inline void TString<T,S,MC>::Copy (TStringRef<const B> str)
	{
		if ( str.Empty() ) {
			Clear();
			return;
		}

		if ( not Empty() and _CheckIntersection( begin(), end(), str.begin(), str.end() ) )
			RET_VOID;

		if ( str.Length() >= _size )
		{
			_size = str.Length()+1;
			_memory.Deallocate();
			
			CHECK( _memory.Allocate( INOUT _size ) );
		}
		
		_length = Min( _size-1, str.Length() );

		for (usize i = 0; i < _length; ++i) {
			_memory.Pointer()[i] = T( str[i] );
		}
		_memory.Pointer()[_length] = 0;
	}
	
/*
=================================================
	Copy
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void TString<T,S,MC>::Copy (TStringRef<const T> str)
	{
		if ( str.Empty() ) {
			Clear();
			return;
		}

		if ( not Empty() and _CheckIntersection( begin(), end(), str.begin(), str.end() ) )
			RET_VOID;

		if ( str.Length() >= _size )
		{
			_size = str.Length()+1;
			_memory.Deallocate();
			
			CHECK( _memory.Allocate( INOUT _size ) );
		}

		_length = GXMath::Min( _size-1, str.Length() );

		Strategy_t::Copy( _memory.Pointer(), str.ptr(), _length );

		_memory.Pointer()[_length] = 0;
	}
	
/*
=================================================
	Free
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void TString<T,S,MC>::Free ()
	{
		_memory.Deallocate();
		_length		= 0;
		_size		= 0;
	}
	
/*
=================================================
	Clear
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void TString<T,S,MC>::Clear ()
	{
		if ( _memory.Pointer() == null or _length == 0 )
			return;

		_memory.Pointer()[0] = 0;
		_length = 0;
	}
	
/*
=================================================
	ptr
=================================================
*/
	template <typename T, typename S, typename MC>
	inline T *  TString<T,S,MC>::ptr ()
	{
		ASSERT( _size > 0 and _memory.Pointer() != null );
		return _memory.Pointer();
	}
	
	template <typename T, typename S, typename MC>
	inline T const *  TString<T,S,MC>::ptr () const
	{
		ASSERT( _size > 0 and _memory.Pointer() != null );
		return _memory.Pointer();
	}
	
/*
=================================================
	Insert
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void TString<T,S,MC>::Insert (TStringRef<const T> str, const usize pos)
	{
		if ( str.Empty() )
			return;

		if ( pos > _length or (not Empty() and _CheckIntersection( begin(), end(), str.begin(), str.end() )) )
			RET_VOID;

		_length += str.Length();

		if ( _Count() > _size )
		{
			MemoryContainer_t	old_memcont;	old_memcont.MoveFrom( INOUT _memory );

			_size = _Count();
			CHECK( _memory.Allocate( INOUT _size ) );

			_length = GXMath::Min( _size-1, _length );

			Strategy_t::Replace( _memory.Pointer(), old_memcont.Pointer(), pos );
			Strategy_t::Replace( _memory.Pointer() + pos + str.Length(), old_memcont.Pointer() + pos, _length - str.Length() - pos );
		}
		else
		{
			// TODI: optimize
			isize	i_temp = pos + str.Length();
			for (isize i = _length; i >= i_temp; --i) {
				Strategy_t::Replace( _memory.Pointer() + i, _memory.Pointer() + i - str.Length(), 1, true );
			}
		}

		Strategy_t::Copy( _memory.Pointer() + pos, str.ptr(), str.Length() );
		_memory.Pointer()[_length] = 0;
	}
	
/*
=================================================
	Insert
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void TString<T,S,MC>::Insert (const void *beginPtr, const void *endPtr, const usize pos)
	{
		Insert( TStringRef<const T>( Cast<const T*>(beginPtr), _GetLength( beginPtr, endPtr ) ), pos );
	}
	
/*
=================================================
	Erase
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void TString<T,S,MC>::Erase (const usize pos, usize count)
	{
		if ( pos >= Length() or count == 0 )
			RET_VOID;
		
		if ( count > _length or pos + count > _length )
			count = _length - pos;

		_length -= count;

		for (usize i = 0; i < _length - pos; ++i) {
			Strategy_t::Replace( _memory.Pointer() + pos + i, _memory.Pointer() + pos + count + i, 1, true );
		}
		_memory.Pointer()[_length] = 0;
	}
	
/*
=================================================
	Erase
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void TString<T,S,MC>::Erase (const void *beginPtr, const void *endPtr)
	{
		ASSERT( beginPtr < endPtr );
		if ( not Empty() and not _CheckIntersection( begin(), end(), beginPtr, endPtr ) )
			RET_VOID;

		Erase( GetIndex( *Cast<T const*>(beginPtr) ), _GetLength( beginPtr, endPtr ) );
	}
	
/*
=================================================
	EraseFromBack
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void TString<T,S,MC>::EraseFromBack (const usize count)
	{
		ASSERT( count <= _length );

		if ( count >= Length() ) {
			Clear();
			return;
		}

		_length -= count;
		_memory.Pointer()[_length] = 0;
	}
	
/*
=================================================
	Reserve
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void TString<T,S,MC>::Reserve (const usize length)
	{
		usize	size = length + 1;

		if ( size > _memory.MaxSize() )
			size = _memory.MaxSize();

		if ( size == 0 or size <= _size )
			return;

		_Reallocate( size, false );
		_memory.Pointer()[_length] = 0;
	}
	
/*
=================================================
	Resize
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void TString<T,S,MC>::Resize (const usize length, const T value, const bool allowReserve)
	{
		if ( length >= _size )
			_Reallocate( length+1, allowReserve );
		
		const isize	i_delta = Cast<isize>( length - Length() );

		for (isize i = i_delta; i > 0; --i)
			_memory.Pointer()[ length - i ] = value;

		_length = length;
		_memory.Pointer()[_length] = 0;
	}
	
/*
=================================================
	FreeReserve
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void TString<T,S,MC>::FreeReserve ()
	{
		if ( _Count() > _size )
			_Reallocate( _Count(), false );
	}
	
/*
=================================================
	FindAndDelete
=================================================
*/
	template <typename T, typename S, typename MC>
	inline bool TString<T,S,MC>::FindAndDelete (TStringRef<const T> value, OUT usize &pos, const usize start)
	{
		if ( not Find( value, pos, start ) )
			return false;

		Erase( pos, value.Length() );
		return true;
	}
	
/*
=================================================
	FindAndChange
=================================================
*/
	template <typename T, typename S, typename MC>
	inline bool TString<T,S,MC>::FindAndChange (TStringRef<const T> searchStr, TStringRef<const T> newStr, OUT usize &pos, const usize start)
	{
		usize	u_len = GXMath::Min( searchStr.Length(), newStr.Length() ),
				i = 0;

		if ( not Find( searchStr, pos, start ) )
			return false;

		for (i = 0; i < u_len; ++i) {
			_memory.Pointer()[i+pos] = newStr[i];
		}

		if ( searchStr.Length() > u_len )
			Erase( i+pos, searchStr.Length() - u_len );

		if ( newStr.Length() > u_len )
			Insert( TStringRef<const T>( newStr.cstr() + i, newStr.Length()-i ), pos+i );
		
		return true;
	}
	
/*
=================================================
	ReplaceChars
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void TString<T,S,MC>::ReplaceChars (const T searchChar, const T newChar)
	{
		if ( searchChar == 0 or newChar == 0 )
			RET_VOID;

		for (usize i = 0; i < _length; ++i) {
			if ( _memory.Pointer()[i] == searchChar )
				_memory.Pointer()[i] = newChar;
		}
	}
	
/*
=================================================
	ReplaceStrings
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void TString<T,S,MC>::ReplaceStrings (TStringRef<const T> pattern, TStringRef<const T> newString, const usize start)
	{
		usize	pos = start;
		for (; FindAndChange( pattern, newString, pos, pos ); pos += newString.Length()) {}
	}
	
/*
=================================================
	CutChars
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void TString<T,S,MC>::CutChars (const T value)
	{
		usize	u_step	= 0;
		usize	i		= 0;

		for (; i < Length(); ++i)
		{
			if ( _memory.Pointer()[i] == value )
			{
				u_step = 1;
				++i;
				break;
			}
		}

		for (; i < Length(); ++i)
		{
			if ( _memory.Pointer()[i] == value ) {
				++u_step;
				continue;
			}

			_memory.Pointer()[i-u_step] = _memory.Pointer()[i];
		}
	}
	
/*
=================================================
	SetLength
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void TString<T,S,MC>::SetLength (const usize length)
	{
		if ( length+1 > _size )
			RET_VOID;

		_length = length;
		_memory.Pointer()[_length] = 0;
	}
	
/*
=================================================
	CalculateLength
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void TString<T,S,MC>::CalculateLength ()
	{
		SetLength( _types_hidden_::StrLength( cstr() ) );
	}
	
/*
=================================================
	PopBack
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void TString<T,S,MC>::PopBack ()
	{
		if ( Length() == 0 )
			RET_VOID;
		
		--_length;
		_memory.Pointer()[_length] = 0;
	}
	
/*
=================================================
	FormatI
=================================================
*/
	template <typename T, typename S, typename MC>
	template <typename B>
	inline TString<T,S,MC> & TString<T,S,MC>::FormatI (const B& value, const int radix)
	{
		Reserve( CompileTime::SizeOf<B>::bits + 2 );
		_length = _types_hidden_::IntToStr( value, _memory.Pointer(), _size, radix );
		return *this;
	}
	
/*
=================================================
	FormatAlignedI
=================================================
*/
	template <typename T, typename S, typename MC>
	template <typename B>
	inline TString<T,S,MC> & TString<T,S,MC>::FormatAlignedI (const B& value, uint align, const T alignChar, const int radix)
	{
		Reserve( GXMath::Max<uint>( CompileTime::SizeOf<B>::bits + 2, align ) );
		_length = _types_hidden_::IntToStr( value, _memory.Pointer(), _size, radix );

		align += 1;

		if ( _Count() < align )
		{
			Strategy_t::Replace( _memory.Pointer() + align - _Count(), _memory.Pointer(), _Count(), true );
		
			for (uint i = 0; i < align - _Count(); ++i)
			{
				_memory.Pointer()[i] = alignChar;
			}
			_length = align-1;
		}
		_memory.Pointer()[_length] = 0;
		return *this;
	}
	
/*
=================================================
	FormatI
=================================================
*/
	template <typename T, typename S, typename MC>
	template <typename B>
	inline TString<T,S,MC> & TString<T,S,MC>::FormatI (const B& value, const usize stepSize, const T spaceChar, const int radix)
	{
		if ( stepSize == 0 ) {
			WARNING( "invalid argument" );
			return *this;
		}

		char	a_buf[ CompileTime::SizeOf<B>::bits + 2 ] = {};

		isize	i	= _types_hidden_::IntToStr( value, a_buf, CountOf(a_buf), radix ),
				j	= 0,
				k	= i + i / stepSize - (i % stepSize == 0 ? 1 : 0);

		Reserve( k+2 );
		SetLength( k );

		for (--i; (i >= 0 and k != 0); --i)
		{
			_memory.Pointer()[--k] = T(a_buf[i]);

			if ( ++j >= isize(stepSize) and k != 0 ) {
				_memory.Pointer()[--k] = spaceChar;
				j = 0;
			}
		}

		return *this;
	}
	
/*
=================================================
	FormatF
=================================================
*/
	template <typename T, typename S, typename MC>
	inline TString<T,S,MC> & TString<T,S,MC>::FormatF (const float fValue, const StringFormatF &fmt)
	{
		const float		value	= GXMath::Abs(fValue);
		StringFormatF	format	= fmt;

		if ( GXMath::Abs(value) > 1.0e8f or (fValue != 0.0f and GXMath::Abs(value) < 1.0e-7f) )
			format.Exp();

		char	a_fmt[8] = {};
		format.ToCString( a_fmt );

		Reserve( (sizeof(fValue)<<4) / 10 + 10 );
		
		_length = usize(std::snprintf( _memory.Pointer(), _size, a_fmt, double(fValue) ));

		if ( isize(_length) <= 0 ) {
			WARNING( "can't convert value to string" );
			_length = 0;
			return *this;
		}
		
		_length = GXMath::Min( _length, _size-1 );

		if ( format.IsCutZeros() and not format.IsExponent() )
		{
			for (usize i = _length-1; i > 0 and i < _length; --i)
			{
				if ( _memory.Pointer()[i] == T('0') ) {
					_length = i;
				} else {
					_length = i+1 + (_memory.Pointer()[i] == T('.'));
					break;
				}
			}
			_length = GXMath::Min( _length, _size-1 );
			_memory.Pointer()[_length] = T(0);
		}

		return *this;
	}
	
/*
=================================================
	FormatF
=================================================
*/
	template <typename T, typename S, typename MC>
	inline TString<T,S,MC> & TString<T,S,MC>::FormatF (const double fValue, const StringFormatF &fmt)
	{
		const double	value	= GXMath::Abs(fValue);
		StringFormatF	format	= fmt;
		
		if ( GXMath::Abs(value) > 1.0e13 or (fValue != 0.0 and GXMath::Abs(value) < 1.0e-9) )
			format.Exp();

		char	a_fmt[8] = {};
		format.ToCString( a_fmt );

		Reserve( (sizeof(fValue)<<4) / 10 + 12 );
		
		_length = usize(std::snprintf( _memory.Pointer(), _size, a_fmt, fValue ));

		if ( isize(_length) <= 0 ) {
			WARNING( "can't convert value to string" );
			_length = 0;
			return *this;
		}

		_length = GXMath::Min( _length, _size-1 );
		
		if ( format.IsCutZeros() and not format.IsExponent() )
		{
			for (usize i = _length-1; i > 0 and i < _length; --i)
			{
				if ( _memory.Pointer()[i] == T('0') ) {
					_length = i;
				} else {
					_length = i+1 + (_memory.Pointer()[i] == T('.'));
					break;
				}
			}
			_length = GXMath::Min( _length, _size-1 );
			_memory.Pointer()[_length] = T(0);
		}

		return *this;
	}
	
/*
=================================================
	Convert
=================================================
*/
	template <typename T, typename S, typename MC>
	template <typename T2, typename S2, typename A2>
	inline void TString<T,S,MC>::Convert (OUT TString<T2,S2,A2> &str) const
	{
		str.Reserve( _length + 4 );

		for (usize i = 0; i < _length; ++i) {
			str[i] = T2(_memory.Pointer()[i]);
		}

		str.SetLength( _length );
	}

	
	#undef  RET_ERROR
	#undef	RET_FALSE
	#undef  RET_VOID
	
/*
=================================================
	IntToStr
=================================================
*/
	namespace _types_hidden_
	{
		template <typename T, typename B>
		inline usize IntToStr (B val, T * buf, const usize size, int radix)
		{
			GX_UNUSED( size );

			if ( radix < 1 or radix > 16 )
			{
				WARNING( "incorrect radix" );
				radix = 10;
			}

			const usize		buf_size			= CompileTime::SizeOf<B>::bits + 2;		// TODO: Ln( sizeof(B), radix )
			T				tmp_buf[ buf_size ] = {};
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

			usize	j = buf_size - i - 1;
			UnsafeMem::MemCopy( buf, tmp_buf + i + 1, BytesU(GXMath::Min( j, size-1 )) );

			ASSERT( size >= j );
			return j-1;
		}

	}	// _types_hidden_
	
	
/*
=================================================
	ArrayRef::From
=================================================
*/
	template <typename T>
	template <typename B, typename S, typename MC>
	inline ArrayRef<T>  ArrayRef<T>::From (const TString<B,S,MC> &str)
	{
		return From( TStringRef<const B>( str ) );
	}

	template <typename T>
	template <typename B, typename S, typename MC>
	inline ArrayRef<T>  ArrayRef<T>::From (TString<B,S,MC> &str)
	{
		return From( TStringRef<B>( str ) );
	}

	
/*
=================================================
	Hash
=================================================
*/
	template <typename T, typename S, typename MC>
	struct Hash< TString<T,S,MC> >
	{
		ND_ HashResult  operator () (const TString<T,S,MC> &x) const noexcept
		{
			return HashOf( ArrayCRef<T>( x ) );
		}
	};

}	// GXTypes
}	// GX_STL
