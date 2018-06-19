// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/TypeId.h"
#include "Core/STL/CompileTime/TypeTraits.h"
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
		
		GX_DEFCOPYCTOR_ASSIGN( VariantRef );


		template <typename T>
		ND_ forceinline static VariantRef  From (T &ref)
		{
			STATIC_ASSERT( not TypeTraits::IsConst<T> );
			return VariantRef( ref );
		}
		
		template <typename T>
		ND_ forceinline static VariantRef  From (const T *ptr)
		{
			return VariantRef( *const_cast<T *>( ptr ) );
		}

		ND_ forceinline static VariantRef  FromVoid (void *ptr, TypeId typeId)
		{
			VariantRef	ref;
			ref._reference	= ptr;
			ref._typeId		= typeId;
			return ref;
		}


		template <typename T>
		ND_ forceinline bool		IsType () const
		{
			return TypeIdOf<T>() == _typeId;
		}

		template <typename T>
		ND_ forceinline bool		IsTypeAs (const T&) const
		{
			return IsType<T>();
		}


		ND_ forceinline TypeId		GetValueTypeId () const
		{
			return _typeId;
		}


		template <typename T>
		ND_ forceinline T &		Get () const
		{
			return *GetPtr<T>();
		}

		template <typename T>
		ND_ forceinline T *		GetPtr () const
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
		HashResult operator () (const VariantRef &) const noexcept
		{
			// must be compilation error!

			//STATIC_WARNING("hash not supported for variant reference type!");
			//return Result_t();
			//return Base_t::operator ()( x.RawPtr() );
			TODO("");
			return HashResult(0);
		}
	};

}	// GXTypes
}	// GX_STL
