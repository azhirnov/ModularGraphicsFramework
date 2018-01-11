// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "TypeListHelpers.h"

namespace GX_STL
{
namespace CompileTime
{

	//
	// Type Key, Any Value Map
	//

	template <typename KeyList, typename ValueList>
	struct Map
	{
		STATIC_ASSERT( IsTypeList< KeyList > );
		STATIC_ASSERT( IsTypeList< ValueList > );
		STATIC_ASSERT( KeyList::Count == ValueList::Count );

	// types
	public:
		using Self			= Map< KeyList, ValueList >;
		using KeyList_t		= KeyList;
		using ValueList_t	= ValueList;
		
	// static values
	private:
		template <typename Key, typename Value>
		struct _Add {
			static constexpr bool	has = KeyList::template HasType< Key >;
			STATIC_ASSERT( not has );

			using keylist	= typename KeyList::template PushBack< Key >;
			using vallist	= typename ValueList::template PushBack< Value >;
			using result	= Map< keylist, vallist >;
		};
		
	public:	// becouse of bug in VS 15.x
		template <typename Key>
		struct _GetValue {
			using valtype	= typename ValueList::template Get< KeyList::template IndexOf<Key> >;
			using type		= typename valtype::type;	// it is valid for 'ValueToType' type
		};

	public:
		// Count
		static constexpr usize	Count		= KeyList::Count;

		// Empty
		static constexpr bool	Empty		= Count == 0;
		
		// IsExist
		template <typename Key>
		static constexpr bool	IsExist		= KeyList::template HasType< Key >;

		// Add
		template <typename Key, typename Value>
		using Add			= typename _Add< Key, Value >::result;
		
		#ifdef GX_AUTO_IN_TEMPLATE_SUPPORTED
		// AddValue
		template <typename Key, auto Value>
		using AddValue		= typename _Add< Key, ValueToType<decltype(Value), Value> >::result;
		#endif

		// Get
		template <typename Key>
		using Get			= typename ValueList::template Get< KeyList::template IndexOf<Key> >;
		
		// GetValue
		template <typename Key>
		static const typename _GetValue<Key>::type	GetValue = _GetValue<Key>::valtype::value;
	};


	template <typename Key, typename Value>
	using CreateMap	= Map< TypeListFrom< Key >, TypeListFrom< Value > >;

}	// CompileTime
}	// GX_STL
