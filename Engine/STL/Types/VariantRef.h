// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/TypeId.h"
#include "Engine/STL/CompileTime/TypeTraits.h"
#include "Ptr.h"

namespace GX_STL
{
namespace GXTypes
{
	struct VariantCRef;


	//
	// Variant Reference
	//

	struct VariantRef : CompileTime::FastCopyable
	{
	// types
	private:
		template <typename T>
		using NonVariantRef = CompileTime::DisableIf< CompileTime::IsSameTypes< T, VariantRef > or
													  CompileTime::IsSameTypes< T, VariantCRef >, int >;


	// variables
	private:
		void *		_reference;
		TypeId		_typeId;


	// methods
	public:
		VariantRef (GX_DEFCTOR) : _reference(null)
		{}
		
		template <typename T>
		forceinline VariantRef (T &ref, NonVariantRef<T> = 0) :
			_reference( static_cast<void *>( &ref ) ),
			_typeId( TypeIdOf<T>() )
		{
			STATIC_ASSERT(( not CompileTime::IsSameTypes< T, VariantRef > ));
			STATIC_ASSERT(( not CompileTime::IsSameTypes< T, VariantCRef > ));
		}

		
		VariantRef (VariantRef &&) = default;
		VariantRef (const VariantRef &) = default;

		VariantRef& operator = (VariantRef &&) = default;
		VariantRef& operator = (const VariantRef &) = default;


		template <typename T>
		forceinline static VariantRef  From (T &ref)
		{
			return VariantRef( ref );
		}
		
		template <typename T>
		forceinline static VariantRef  From (const T *ptr)
		{
			return VariantRef( *const_cast<T *>( ptr ) );
		}

		forceinline static VariantRef  FromVoid (void *ptr, TypeId typeId)
		{
			VariantRef	ref;
			ref._reference	= ptr;
			ref._typeId		= typeId;
			return ref;
		}


		// Warning: this is not safe!
		// const reference may be a reference to a temporary object.
		template <typename T>
		forceinline static VariantRef  FromConst (const T& ref)
		{
			return VariantRef( const_cast<T &>( ref ) );
		}


		template <typename T>
		forceinline bool		IsType () const
		{
			return TypeIdOf<T>() == _typeId;
		}

		template <typename T>
		forceinline bool		IsTypeAs (const T&) const
		{
			return IsType<T>();
		}


		forceinline TypeId		GetValueTypeId () const
		{
			return _typeId;
		}


		template <typename T>
		forceinline T &			Get () const
		{
			return *GetPtr<T>();
		}

		template <typename T>
		forceinline T *			GetPtr () const
		{
			CHECK( IsType<T>() );
			return const_cast< T* >( static_cast< T const *>( _reference ) );
		}


		template <typename T>
		forceinline void		CopyTo (OUT T &value) const
		{
			value = Get<T>();
		}


		template <typename T>
		friend forceinline void  operator << (T &left, const VariantRef &right)
		{
			right.CopyTo( left );
		}
	};



	//
	// Variant Array Ref		// TODO
	//
	/*
	struct VariantArrayRef
	{
	// variables
	private:
		void *		_reference;
		TypeId		_typeId;
		

	// methods
	public:
		VariantArrayRef (GX_DEFCTOR) : _reference(null)
		{}

		template <typename T>
		explicit VariantArrayRef (ArrayRef<T> &ref) :
			_reference( static_cast<void *>( &ref ) ),
			_typeId( TypeIdOf<T>() )
		{}

		forceinline TypeId		GetValueTypeId () const
		{
			return _typeId;
		}


		template <typename T>
		forceinline ArrayRef<T> &	Get () const
		{
			CHECK( IsType<T>() );
			return *GetPtr<T>();
		}

	};
	*/


	//
	// Hash
	//
	
	template <>
	struct Hash< VariantRef > : private Hash< void* >
	{
		typedef VariantRef			Key_t;
		typedef Hash< void* >		Base_t;
		typedef Base_t::Result_t	Result_t;

		Result_t operator () (const Key_t &) const noexcept
		{
			// must be compilation error!

			//STATIC_WARNING("hash not supported for variant reference type!");
			//return Result_t();
			//return Base_t::operator ()( x.RawPtr() );
			TODO("");
			return Result_t(0);
		}
	};

}	// GXTypes
}	// GX_STL
