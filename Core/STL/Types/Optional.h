// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Memory/MemFunc.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Optional
	//

	template <typename T>
	struct alignas(T) Optional : CompileTime::CopyQualifiers< CompileTime::PODStruct, T >
	{
		STATIC_ASSERT( TypeTraits::IsCopyConstructible<T> );

	// types
	public:
		using Self		= Optional< T >;
		using Value_t	= T;


	// variables
	private:
		union {
			T		_value;				// for align
			char	_buf[ sizeof(T) ];
		};
		bool		_isDefined;


	// methods
	public:
		Optional (GX_DEFCTOR) : _isDefined(false)
		{
			_ClearMem();
		}


		Optional (const T &value) : _isDefined(false)
		{
			_ClearMem();
			_Create<T>( value );
		}


		Optional (T &&value) : _isDefined(false)
		{
			_ClearMem();
			_Create<T>( FW<T>( value ) );
		}

		
		Optional (const Self &other) : _isDefined(false)
		{
			_ClearMem();

			if ( other.IsDefined() )
				_Create<T>( other._value );
		}

		
		Optional (Self &&other) : _isDefined(false)
		{
			_ClearMem();

			if ( other.IsDefined() )
				_Create<T>( FW<T>( other._value ) );

			other.Undefine();
		}


		~Optional ()
		{
			_Destroy();
		}


		Self &	operator =  (const T& right)
		{
			_Create<T>( right );
			return *this;
		}


		Self &	operator =  (T&& right)
		{
			_Create<T>( FW<T>( right ) );
			return *this;
		}


		Self &	operator =  (const Self &right)
		{
			if ( right.IsDefined() )
				_Create<T>( right._value );
			else
				_Destroy();

			return *this;
		}


		Self &	operator =  (Self &&right)
		{
			if ( right.IsDefined() ) {
				_Create<T>( FW<T>( right._value ) );
				right.Undefine();
			} else
				_Destroy();

			return *this;
		}


		ND_ bool	operator == (const Self &right) const
		{
			return IsDefined() and right.IsDefined() and GXMath::All( Get() == right.Get() );
		}


		ND_ bool	operator != (const Self &right) const
		{
			return not (*this == right);
		}


		ND_ explicit operator bool	()	const				{ return IsDefined(); }

		ND_ T const &	operator * ()	const				{ return Get(); }
		ND_ T &			operator * ()						{ return Get(); }

		ND_ T *			operator -> ()						{ return GetPtr(); }
		ND_ T const *	operator -> ()	const				{ return GetPtr(); }

		ND_ bool		IsDefined ()	const				{ return _isDefined; }
			void		Undefine ()							{ _Destroy(); }

		ND_ T const &	Get ()			const				{ ASSERT( IsDefined() );  return _value; }
		ND_ T &			Get ()								{ ASSERT( IsDefined() );  return _value; }

		ND_ T const		Get (const T defaultValue)	const	{ return IsDefined() ? _value : defaultValue; }
		ND_ T const&	Get (T& defaultValue) const			{ return IsDefined() ? _value : defaultValue; }

		ND_ T *			GetPtrOrNull ()						{ return IsDefined() ? GetPtr() : null; }
		ND_ T const *	GetPtrOrNUll ()	const				{ return IsDefined() ? GetPtr() : null; }

		ND_ T const *	GetPtr ()		const
		{
			ASSERT( IsDefined() );
			return &_value;
		}

		ND_ T *		GetPtr ()
		{
			ASSERT( IsDefined() );
			return &_value;
		}

		T &			CreateDefault ()
		{
			_Create<T>( RVREF( T() ) );
			return Get();
		}

		ND_ T &		GetOrCreate ()
		{
			if ( not IsDefined() )
				return CreateDefault();
			return Get();
		}

		ND_ T &		GetOrCreate (const T& defaultValue)
		{
			if ( not IsDefined() )
				return _Create<T>( defaultValue );
			return Get();
		}

		ND_ T &		GetOrCreate (T&& defaultValue)
		{
			if ( not IsDefined() )
				return _Create<T>( FW<T>( defaultValue ) );
			return Get();
		}


	private:
		template <typename B>
		CompileTime::EnableIf<TypeTraits::IsCopyConstructible<B>>  _Create (const B &value) noexcept
		{
			_Destroy();
			UnsafeMem::PlacementNew<T>( &_value, value );
			_isDefined = true;
		}

		template <typename B>
		CompileTime::EnableIf<TypeTraits::IsMoveConstructible<B>>  _Create (B &&value) noexcept
		{
			_Destroy();
			UnsafeMem::PlacementNew<T>( &_value, FW<T>( value ) );
			_isDefined = true;
		}

		void _Destroy () noexcept
		{
			if ( _isDefined )
			{
				PlacementDelete( Get() );
				_isDefined = false;
			}
		}

		void _ClearMem ()
		{
			DEBUG_ONLY( ZeroMem( _buf ) );
		}
	};

	
/*
=================================================
	OptionalFrom
=================================================
*/
	template <typename T>
	ND_ inline Optional<T> OptionalFrom (const T &value, bool isDefined = true)
	{
		return isDefined ? Optional<T>( value ) : Optional<T>();
	}

	template <typename T>
	ND_ inline Optional<T> OptionalFrom (T &&value, bool isDefined = true)
	{
		return isDefined ? Optional<T>( RVREF(value) ) : Optional<T>();
	}
	
/*
=================================================
	Hash
=================================================
*/
	template <typename T>
	struct Hash< Optional<T> >
	{
		ND_ HashResult  operator () (const Optional<T> &x) const noexcept
		{
			return x ? HashOf( x.Get() ) : HashResult();
		}
	};

}	// GXTypes
}	// GX_STL
