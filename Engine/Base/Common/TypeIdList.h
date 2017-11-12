// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Common/Common.h"

namespace Engine
{
namespace Base
{

	//
	// TypeId List
	//

	struct TypeIdList
	{
	// types
	private:
		using TypeSet_t		= Set< TypeId >;
		using TypesArray_t	= ArrayCRef< TypeId >;

		struct _TypeToTypeId_Func;
		struct _HasTypes_Func;


	// variables
	private:
		TypeSet_t		_types;


	// methods
	public:
		TypeIdList (GX_DEFCTOR)
		{}

		template <typename T>
		explicit TypeIdList (UninitializedT<T>);

		explicit TypeIdList (InitializerList<TypeId> list) : _types(list) {}

		template <typename T>
		bool	HasAllTypes () const;

		template <typename T>
		bool	HasAnyType () const;

		bool	HasAllTypes (const TypeIdList &other) const		{ return HasAllTypes( TypesArray_t(other) ); }
		bool	HasAnyType (const TypeIdList &other) const		{ return HasAnyType( TypesArray_t(other) ); }
		
		bool	HasAllTypes (const TypesArray_t &other) const;
		bool	HasAnyType (const TypesArray_t &other) const;

		String	ToString () const;

		template <typename T>
		bool	HasType ()				const	{ return _types.IsExist( TypeIdOf<T>() ); }

		bool	HasType (TypeId id)		const	{ return _types.IsExist( id ); }

		usize	Count ()				const	{ return _types.Count(); }
		bool	Empty ()				const	{ return _types.Empty(); }

		TypeId	Get (usize index)		const	{ return _types[ index ]; }

		operator TypesArray_t ()		 const	{ return _types; }
	};
	

	
/*
=================================================
	_TypeToTypeId_Func
=================================================
*/
	struct TypeIdList::_TypeToTypeId_Func
	{
		TypeSet_t	&_types;

		explicit _TypeToTypeId_Func (TypeSet_t &types) : _types(types)
		{}

		template <typename T, usize Index>
		void Process ()
		{
			_types << TypeIdOf<T>();
		}
	};
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	inline TypeIdList::TypeIdList (UninitializedT<T>)
	{
		_TypeToTypeId_Func	func{ _types };
		T::RuntimeForEach( func );
	}
	
/*
=================================================
	HasAllTypes
=================================================
*/
	inline bool TypeIdList::HasAllTypes (const TypesArray_t &other) const
	{
		FOR( i, other )
		{
			if ( not HasType( other[i] ) )
				return false;
		}
		return true;
	}
	
/*
=================================================
	HasAnyType
=================================================
*/
	inline bool TypeIdList::HasAnyType (const TypesArray_t &other) const
	{
		FOR( i, other )
		{
			if ( HasType( other[i] ) )
				return true;
		}
		return false;
	}
	
/*
=================================================
	_HasTypes_Func
=================================================
*/
	struct TypeIdList::_HasTypes_Func
	{
		TypeSet_t const	&	_types;
		bool				and_result	= true;
		bool				or_result	= false;

		explicit _HasTypes_Func (TypeSet_t const& types) : _types(types)
		{}
		
		template <typename T, usize Index>
		void Process ()
		{
			bool	exist = _types.IsExist( TypeIdOf<T>() );

			and_result	&= exist;
			or_result	|= exist;
		}
	};
	
/*
=================================================
	HasAllTypes
=================================================
*/
	template <typename T>
	inline bool TypeIdList::HasAllTypes () const
	{
		_HasTypes_Func	func{ _types };

		T::RuntimeForEach( func );
		return func.and_result;
	}
	
/*
=================================================
	HasAnyType
=================================================
*/
	template <typename T>
	inline bool TypeIdList::HasAnyType () const
	{
		_HasTypes_Func	func{ _types };

		T::RuntimeForEach( func );
		return func.or_result;
	}

/*
=================================================
	ToString
=================================================
*/
	inline String TypeIdList::ToString () const
	{
		String	str;

		FOR( i, _types ) {
			str << (i ? ", " : "") << GXTypes::ToString( _types[i] );
		}
		return str;
	}


}	// Base
}	// Engine
