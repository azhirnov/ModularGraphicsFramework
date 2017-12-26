// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "VariantRef.h"
#include "VariantCRef.h"
#include "Engine/STL/CompileTime/NewTypeInfo.h"
#include "Engine/STL/CompileTime//FunctionInfo.h"

namespace GX_STL
{
namespace GXTypes
{
	

	//
	// Variant Reference Indexed Container Wrapper
	//

	template <typename Container, typename Typelist>
	struct VariantRefIndexedContainer
	{
		STATIC_ASSERT(( CompileTime::IsSameTypesWithoutQualifiers< typename Container::Value_t, VariantRef> or
						CompileTime::IsSameTypesWithoutQualifiers< typename Container::Value_t, VariantCRef> ));

	// types
	public:
		using Self			= VariantRefIndexedContainer< Container, Typelist >;
		using Container_t	= Container;
		using Typelist_t	= Typelist;
		
		template <usize I>	using Value_t		= TypeTraits::RemoveReference< typename Typelist::template Get<I> >;
		template <usize I>	using ValueRef_t	= CompileTime::ResultOf< decltype(&Container::Value_t::template Get< Value_t<I> >) >;


	// variables
	private:
		Container_t const&		_containerRef;


	// methods
	public:
		explicit VariantRefIndexedContainer (const Container_t &container) : _containerRef(container)
		{
			ASSERT( _containerRef.Count() == Typelist::Count );
		}

		
		template <usize I>
		ValueRef_t<I> 	Get () const
		{
			return _containerRef[I].template Get< Value_t<I> >();
		}
	};
	

	
/*
=================================================
	VariantRefIndexedContainerFrom
=================================================
*/
	template <typename Typelist, typename Container>
	inline VariantRefIndexedContainer<Container, Typelist>  VariantRefIndexedContainerFrom (const Container &value)
	{
		return VariantRefIndexedContainer<Container, Typelist>( value );
	}

}	// GXTypes
}	// GX_STL
