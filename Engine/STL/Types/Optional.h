// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Memory/MemFunc.h"

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
		
		explicit
		Optional (const Self *value) : _isDefined(false)
		{
			_ClearMem();

			if ( value != null )
				_Create( *value );
		}


		Optional (const T &value) : _isDefined(false)
		{
			_ClearMem();
			_Create( value );
		}


		Optional (T &&value) : _isDefined(false)
		{
			_ClearMem();
			_Create( FW<T>( value ) );
		}

		
		Optional (const Self &other) : _isDefined(false)
		{
			_ClearMem();

			if ( other.IsDefined() )
				_Create( other.Get() );
		}


		~Optional ()
		{
			_Destroy();
		}


		Self &	operator =  (const T& value)
		{
			_Create( value );
			return *this;
		}


		Self &	operator =  (T&& value)
		{
			_Create( RVREF( value ) );
			return *this;
		}


		CHECKRES bool	operator == (const Self &other) const
		{
			return IsDefined() and other.IsDefined() and GXMath::All( Get() == other.Get() );
		}


		CHECKRES bool	operator != (const Self &other) const
		{
			return not (*this == other);
		}


		explicit operator bool	()	const				{ return IsDefined(); }

		T const &	operator * ()	const				{ return Get(); }
		T &			operator * ()						{ return Get(); }

		T *			operator -> ()						{ return GetPtr(); }
		T const *	operator -> ()	const				{ return GetPtr(); }

		bool		IsDefined ()	const				{ return _isDefined; }
		void		Undefine ()							{ _Destroy(); }

		T const &	Get ()			const				{ ASSERT( IsDefined() );  return _value; }
		T &			Get ()								{ ASSERT( IsDefined() );  return _value; }

		T const		Get (const T defaultValue)	const	{ return IsDefined() ? _value : defaultValue; }
		T const&	Get (T& defaultValue) const			{ return IsDefined() ? _value : defaultValue; }

		T *			GetPtrOrNull ()						{ return IsDefined() ? GetPtr() : null; }
		T const *	GetPtrOrNUll ()	const				{ return IsDefined() ? GetPtr() : null; }

		T const *	GetPtr ()		const
		{
			ASSERT( IsDefined() );
			return &_value;
		}

		T *			GetPtr ()
		{
			ASSERT( IsDefined() );
			return &_value;
		}

		T &			CreateDefault ()
		{
			_Create( RVREF( T() ) );
			return Get();
		}

		T &			GetOrCreate ()
		{
			if ( not IsDefined() )	CreateDefault();
			return Get();
		}

		T &			GetOrCreate (const T& defaultValue)
		{
			if ( not IsDefined() )	_Create( defaultValue );
			return Get();
		}

		T &			GetOrCreate (T&& defaultValue)
		{
			if ( not IsDefined() )	_Create( RVREF(defaultValue) );
			return Get();
		}


	private:
		void _Create (const T &value) noexcept
		{
			_Destroy();
			UnsafeMem::PlacementNew<T>( &_value, value );
			_isDefined = true;
		}

		void _Create (T &&value) noexcept
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
	inline Optional<T> OptionalFrom (const T &value, bool isDefined = true)
	{
		return isDefined ? Optional<T>( value ) : Optional<T>();
	}

	template <typename T>
	inline Optional<T> OptionalFrom (T &&value, bool isDefined = true)
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
		HashResult  operator () (const Optional<T> &x) const noexcept
		{
			return x ? HashOf( x.Get() ) : HashResult();
		}
	};

}	// GXTypes
}	// GX_STL
