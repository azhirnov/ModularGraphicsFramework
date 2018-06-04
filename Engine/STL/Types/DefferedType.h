// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
	struct DeferredType : CompileTime::FastCopyable
	{
	// types
	private:
		using Self	= DeferredType< Size, Align, false >;


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
			UnsafeMem::MemCopy( _data, other._data, BytesU::SizeOf(_data) );
		}

		DeferredType (Self &&other) :
			_typeid( other._typeid ),
			_isDefined( other._isDefined )
		{
			UnsafeMem::MemCopy( _data, other._data, BytesU::SizeOf(_data) );
			other._Destroy();
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

		forceinline Self& operator = (NullPtr_t)
		{
			_Destroy();
			return *this;
		}

		forceinline Self& operator = (const Self &other)
		{
			ASSERT( _typeid == other._typeid );

			UnsafeMem::MemCopy( _data, other._data, BytesU::SizeOf(_data) );
			_isDefined = other._isDefined;
			return *this;
		}

		forceinline Self& operator = (Self &&other)
		{
			ASSERT( _typeid == other._typeid );

			UnsafeMem::MemCopy( _data, other._data, BytesU::SizeOf(_data) );
			_isDefined = other._isDefined;

			other._Destroy();
			return *this;
		}


		template <typename T>
		ND_ forceinline T&  Get ()
		{
			ASSERT( IsDefined() and TypeIdOf<T>() == _typeid );
			return ReferenceCast<T>( _data );
		}

		template <typename T>
		ND_ forceinline T const&  Get () const
		{
			ASSERT( IsDefined() and TypeIdOf<T>() == _typeid );
			return ReferenceCast<T>( _data );
		}


		ND_ forceinline bool IsDefined () const
		{
			return _isDefined;
		}
		
		template <typename T>
		ND_ forceinline bool IsNull () const
		{
			return not IsNotNull<T>();
		}

		template <typename T>
		ND_ forceinline bool IsNotNull () const
		{
			ASSERT( TypeIdOf<T>() == _typeid );

			return	TypeIdOf<T>() == _typeid	and
					IsDefined()					and
					ReferenceCast<T>( _data ) != T(0);
		}


	protected:
		explicit DeferredType (TypeId typeId) : _typeid(typeId), _isDefined(false)
		{}

		template <typename T>
		forceinline void _Create () noexcept
		{
			PlacementNew<T>( BinArrayRef( _data ) );
			_isDefined = true;
		}

		template <typename T>
		forceinline void _Create (const T &value) noexcept
		{
			PlacementNew<T>( BinArrayRef( _data ), value );
			_isDefined = true;
		}

		forceinline void _Destroy () noexcept
		{
			if ( IsDefined() )
			{
				DEBUG_ONLY( ZeroMem( _data ) );

				_isDefined = false;
			}
		}
	};
	


	//
	// Deferred Type with destructor
	//
	
	template <usize Size, usize Align>
	struct DeferredType< Size, Align, true > : DeferredType< Size, Align, false >
	{
	// types
	private:
		using Self			= DeferredType< Size, Align, true >;
		using Base_t		= DeferredType< Size, Align, false >;
		using Destructor_t	= void (*) (void *);


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

		DeferredType (Self &&other) = delete;	// TODO

		~DeferredType ()
		{
			if ( this->IsDefined() and _destructor != null )
				_destructor( this->_data );
		}

		
		template <typename T>
		forceinline Self& operator = (const T &value)
		{
			ASSERT( TypeIdOf<T>() == this->_typeid );

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
		forceinline Self& operator = (Self &&other) = delete;


	private:
		template <typename T>
		forceinline void _Create () noexcept
		{
			Base_t::template _Create<T>();

			_destructor	= &_CallDestructor<T>;
		}


		template <typename T>
		forceinline void _Create (const T &value) noexcept
		{
			Base_t::_Create( value );

			_destructor	= &_CallDestructor<T>;
		}

		forceinline void _Destroy () noexcept
		{
			if ( this->IsDefined() and _destructor != null )
				_destructor( this->_data );

			Base_t::_Destroy();

			_destructor = null;
		}

		template <typename T>
		void _CallDestructor (void *ptr)
		{
			reinterpret_cast<T*>(ptr)->~T();
		}
	};



	//
	// Deferred Type from same other type
	//

	template <typename T, bool WithDestructor = false>
	using DeferredTypeFrom = DeferredType< sizeof(T), alignof(T), WithDestructor >;

	
}	// GXTypes
}	// GX_STL
