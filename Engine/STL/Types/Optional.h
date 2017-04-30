// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
	struct Optional : CompileTime::CopyQualifiers< CompileTime::PODStruct, T >
	{
	// types
	public:
		using Self		= Optional< T >;
		using value_t	= T;


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


		bool	operator == (const Self &other) const
		{
			return IsDefined() and other.IsDefined() and Get() == other.Get();
		}


		bool	operator != (const Self &other) const
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
		T const		Get (const T& defaultValue)	const	{ return IsDefined() ? Get() : defaultValue; }

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


	private:
		void _Create (const T &value)
		{
			_Destroy();
			UnsafeMem::PlacementNew<T>( &_value, value );
			_isDefined = true;
		}

		void _Create (T &&value)
		{
			_Destroy();
			UnsafeMem::PlacementNew<T>( &_value, FW<T>( value ) );
			_isDefined = true;
		}

		void _Destroy ()
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
	

	template <typename T>
	struct Hash< Optional<T> > : private Hash<T>
	{
		typedef Optional<T>					key_t;
		typedef Hash<T>						base_t;
		typedef typename base_t::result_t	result_t;

		result_t operator () (const key_t &x) const
		{
			return x ? base_t::operator ()( x.Get() ) : result_t();
		}
	};

}	// GXTypes
}	// GX_STL
