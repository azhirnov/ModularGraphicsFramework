// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.
/*
	Runtime reflection of TypeList
*/

#pragma once

#include "Engine/STL/CompileTime/TypeList.h"
#include "Engine/STL/Containers/ArrayRef.h"
#include "Engine/STL/Containers/String.h"

namespace GX_STL
{
namespace Runtime
{
	using namespace GXTypes;



	//
	// Type List
	//

	struct VirtualTypeList final
	{
	// types
	private:

		//
		// Type list
		//
		class ITypelist
		{
		// interface
		public:
			virtual usize	Count ()			const = 0;
			virtual BytesU	MaxSizeOf ()		const = 0;
			virtual TypeId	Get (usize index)	const = 0;
			virtual usize	IndexOf (TypeId id)	const = 0;
			virtual String	ToString ()			const = 0;
		};


		template <typename T>	class TypelistImpl;

		struct _TypelistImpl_Get;
		struct _TypelistImpl_IndexOf;
		struct _TypelistImpl_ToString;

		using DymmyTypeList = CompileTime::TypeList< CompileTime::TypeListEnd >;

		
		union _Storage_t {
			ulong	maxAlign;
			ubyte	buf[ sizeof(void*) ];
		};


	// variables
	private:
		_Storage_t		_storage;


	// methods
	public:
		VirtualTypeList (GX_DEFCTOR);

		template <typename T>
		VirtualTypeList (UninitializedT<T>);
		

		usize	Count ()			const		{ return _Internal()->Count(); }
		bool	Empty ()			const		{ return Count() == 0; }

		BytesU	MaxSizeOf ()		const		{ return _Internal()->MaxSizeOf(); }

		TypeId	Get (usize index)	const		{ return _Internal()->Get( index ); }
		usize	IndexOf (TypeId id)	const		{ return _Internal()->IndexOf( id ); }
		bool	HasType (TypeId id)	const		{ return IndexOf( id ) != -1; }

		String	ToString ()			const		{ return _Internal()->ToString(); }


	private:
		forceinline ITypelist const *	_Internal ()	const	{ return (ITypelist const *) _storage.buf; }
		forceinline BinArrayRef			_Data ()				{ return BinArrayRef( _storage.buf ); }
	};



	//
	// Type list implementation
	//
	template <typename T>
	class VirtualTypeList::TypelistImpl final : public ITypelist
	{
		STATIC_ASSERT( CompileTime::IsTypeList<T> );

	// methods
	public:
		usize	Count ()			const override		{ return T::Length; }
		BytesU	MaxSizeOf ()		const override		{ return BytesU( T::MaxSizeOf() ); }
		TypeId	Get (usize index)	const override;
		usize	IndexOf (TypeId id)	const override;
		String	ToString ()			const override;
	};
		

	//
	// Get function for Typelist
	//
	struct VirtualTypeList::_TypelistImpl_Get
	{
		TypeId		id;
		const usize	idx;

		explicit _TypelistImpl_Get (usize i) : id(), idx(i) {}
			
		template <typename T, usize Index>
		void Process ()
		{
			if ( Index == idx )
				id = TypeIdOf<T>();
		}
	};


	//
	// IndexOf function for Typelist
	//
	struct VirtualTypeList::_TypelistImpl_IndexOf
	{
		const TypeId	id;
		usize			idx;

		explicit _TypelistImpl_IndexOf (TypeId id) : id(id), idx(-1) {}
			
		template <typename T, usize Index>
		void Process ()
		{
			if ( idx == -1 and TypeIdOf<T>() == id )
				idx = Index;
		}
	};
	

	//
	// ToString function for Typelist
	//
	struct VirtualTypeList::_TypelistImpl_ToString
	{
		String	str;

		_TypelistImpl_ToString ()
		{}

		template <typename T, usize Index>
		void Process ()
		{
			str << (Index == 0 ? "" : " || ") << typeid(T).name();
		}
	};


/*
=================================================
	constructor
=================================================
*/
	inline VirtualTypeList::VirtualTypeList (UninitializedType) :
		VirtualTypeList( UninitializedT<DymmyTypeList>() )
	{
		ASSERT( Empty() );
	}

	template <typename T>
	inline VirtualTypeList::VirtualTypeList (UninitializedT<T>)
	{
		PlacementNew< TypelistImpl< T::Minimize::type > >( _Data() );
	}
	
/*
=================================================
	Get
=================================================
*/
	template <typename T>
	inline TypeId  VirtualTypeList::TypelistImpl<T>::Get (usize index) const
	{
		_TypelistImpl_Get	func( index );

		T::RuntimeForEach( func );
		return func.id;
	}
	
/*
=================================================
	IndexOf
=================================================
*/
	template <typename T>
	inline usize  VirtualTypeList::TypelistImpl<T>::IndexOf (TypeId id) const
	{
		_TypelistImpl_IndexOf	func( id );

		T::RuntimeForEach( func );
		return func.idx;
	}
	
/*
=================================================
	ToString
=================================================
*/
	template <typename T>
	inline String  VirtualTypeList::TypelistImpl<T>::ToString () const
	{
		_TypelistImpl_ToString	func;

		T::RuntimeForEach( func );
		return func.str;
	}


}	// Runtime
}	// GX_STL
