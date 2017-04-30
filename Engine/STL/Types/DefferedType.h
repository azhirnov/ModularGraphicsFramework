// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "VariantRef.h"

namespace GX_STL
{
namespace GXTypes
{
	
	
	//
	// Deferred Type without destructor
	//
	
	template <usize Size, usize Align, bool WithDestructor = false>
	struct DeferredType : Noncopyable
	{
	// types
	private:
		typedef DeferredType< Size, Align, false >	Self;


	// variables
	protected:
		alignas(Align)	ubyte	_data[ Size ];
		const TypeId			_typeid;
		bool					_isDefined;


	// methods
	public:
		template <typename T>
		explicit
		DeferredType (UninitializedT<T>) : _typeid( TypeIdOf<T>() ), _isDefined(false)
		{
			_Create<T>();
		}

		template <typename T>
		explicit
		DeferredType (const T &value) : _typeid( TypeIdOf<T>() ), _isDefined(false)
		{
			_Create( value );
		}

		DeferredType (const Self &other) :
			_typeid( other._typeid ),
			_isDefined( other._isDefined )
		{
			UnsafeMem::MemCopy( _data, other._data, SizeOf(_data) );
		}

		~DeferredType ()
		{
			_Destroy();
		}

		
		template <typename T>
		forceinline Self& operator = (const T &value)
		{
			ASSERT( TypeIdOf<T>() == _typeid );

			// destructor not needed
			_Create( value );
			return *this;
		}

		forceinline Self& operator = (NullPtr_t value)
		{
			_Destroy();
			return *this;
		}

		forceinline Self& operator = (const Self &other)
		{
			ASSERT( _typeid == other._typeid );

			UnsafeMem::MemCopy( _data, other._data, SizeOf(_data) );
			_isDefined = other._isDefined;
			return *this;
		}


		template <typename T>
		forceinline T&  Get ()
		{
			ASSERT( IsDefined() and TypeIdOf<T>() == _typeid );
			return ReferenceCast<T>( _data );
		}

		template <typename T>
		forceinline T const&  Get () const
		{
			ASSERT( IsDefined() and TypeIdOf<T>() == _typeid );
			return ReferenceCast<T>( _data );
		}


		forceinline bool IsDefined () const
		{
			return _isDefined;
		}
		
		template <typename T>
		forceinline bool IsNull () const
		{
			return not IsNotNull<T>();
		}

		template <typename T>
		forceinline bool IsNotNull () const
		{
			ASSERT( TypeIdOf<T>() == _typeid );

			return	TypeIdOf<T>() == _typeid	and
					IsDefined()					and
					ReferenceCast<T>( _data ) != 0;
		}


	protected:
		explicit DeferredType (TypeId typeId) : _typeid(typeId), _isDefined(false)
		{}

		template <typename T>
		forceinline void _Create ()
		{
			PlacementNew<T>( BinArrayRef( _data ) );
			_isDefined = true;
		}

		template <typename T>
		forceinline void _Create (const T &value)
		{
			PlacementNew<T>( BinArrayRef( _data ), value );
			_isDefined = true;
		}

		forceinline void _Destroy ()
		{
			if ( IsDefined() )
			{
				DEBUG_ONLY( ZeroMem( _data ) );

				_isDefined = false;
			}
		}
	};
	


	//
	// Hidden OS Type with destructor
	//
	
	template <usize Size, usize Align>
	struct DeferredType< Size, Align, true > : DeferredType< Size, Align, false >
	{
	// types
	private:
		typedef DeferredType< Size, Align, true >	Self;
		typedef DeferredType< Size, Align, false >	Base_t;
		
		typedef void (*Destructor_t) (void *);


	// variables
	private:
		Destructor_t	_destructor;


	// methods
	public:
		template <typename T>
		explicit
		DeferredType (UninitializedT<T>) : Base_t( TypeIdOf<T>() ), _destructor(null)
		{
			_Create<T>();
		}

		template <typename T>
		explicit
		DeferredType (const T &value) : Base_t( TypeIdOf<T>() ), _destructor(null)
		{
			_Create( value );
		}

		DeferredType (const Self &other) = delete;

		~DeferredType ()
		{
			if ( IsDefined() and _destructor != null )
				_destructor( _data );
		}

		
		template <typename T>
		forceinline Self& operator = (const T &value)
		{
			ASSERT( TypeIdOf<T>() == _typeid );

			_Destroy();
			_Create( value );

			return *this;
		}

		forceinline Self& operator = (NullPtr_t value)
		{
			_Destroy();
			return *this;
		}
		
		forceinline Self& operator = (const Self &other) = delete;


	private:
		template <typename T>
		forceinline void _Create ()
		{
			Base_t::_Create<T>();

			_destructor	= &_CallDestructor<T>;
		}


		template <typename T>
		forceinline void _Create (const T &value)
		{
			Base_t::_Create( value );

			_destructor	= &_CallDestructor<T>;
		}

		forceinline void _Destroy ()
		{
			if ( IsDefined() and _destructor != null )
				_destructor( _data );

			Base_t::_Destroy();

			_destructor = null;
		}
	};



	//
	// Deferred Type from same other type
	//

	template <typename T, bool WithDestructor = false>
	using DeferredTypeFrom = DeferredType< sizeof(T), alignof(T), WithDestructor >;

	
}	// GXTypes
}	// GX_STL
