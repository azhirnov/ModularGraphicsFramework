// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/TypeId.h"
#include "Engine/STL/CompileTime/TypeTraits.h"
#include "Ptr.h"

namespace GX_STL
{
namespace GXTypes
{
	struct VariantRef;


	//
	// Variant Constant Reference
	//

	struct VariantCRef : CompileTime::FastCopyable
	{
	// types
	private:
		template <typename T>
		using NonVariantRef = CompileTime::DisableIf< CompileTime::IsSameTypes< T, VariantRef > or
													  CompileTime::IsSameTypes< T, VariantCRef >, int >;


	// variables
	private:
		const void *	_reference;
		TypeId			_typeId;


	// methods
	public:
		VariantCRef (GX_DEFCTOR) : _reference(null)
		{}

		template <typename T>
		forceinline VariantCRef (T &ref, NonVariantRef<T> = 0) :
			_reference( static_cast<void const *>( &ref ) ),
			_typeId( TypeIdOf<T>() )
		{
			STATIC_ASSERT(( not CompileTime::IsSameTypes< T, VariantRef > ));
			STATIC_ASSERT(( not CompileTime::IsSameTypes< T, VariantCRef > ));
		}


		VariantCRef (VariantCRef &&) = default;
		VariantCRef (const VariantCRef &) = default;

		VariantCRef& operator = (VariantCRef &&) = default;
		VariantCRef& operator = (const VariantCRef &) = default;


		template <typename T>
		forceinline static VariantCRef  From (T &ref)
		{
			return VariantCRef( ref );
		}
		
		template <typename T>
		forceinline static VariantCRef  From (const T *ptr)
		{
			return VariantCRef( *const_cast<T *>( ptr ) );
		}

		forceinline static VariantCRef  FromVoid (const void *ptr, TypeId typeId)
		{
			VariantCRef	ref;
			ref._reference	= ptr;
			ref._typeId		= typeId;
			return ref;
		}


		// Warning: this is not safe!
		// const reference may be a reference to a temporary object.
		template <typename T>
		forceinline static VariantCRef  FromConst (const T& ref)
		{
			return VariantCRef( const_cast<T &>( ref ) );
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
		forceinline T const &	Get () const
		{
			return *GetPtr<T>();
		}

		template <typename T>
		forceinline T const *	GetPtr () const
		{
			CHECK( IsType<T>() );
			return static_cast<T const *>( _reference );
		}


		template <typename T>
		forceinline void		CopyTo (OUT T &value) const
		{
			value = Get<T>();
		}


		template <typename T>
		friend forceinline void  operator << (T &left, const VariantCRef &right)
		{
			right.CopyTo( left );
		}
	};


	
	//
	// Hash
	//

	template <>
	struct Hash< VariantCRef > : private Hash< void* >
	{
		typedef VariantCRef			Key_t;
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
