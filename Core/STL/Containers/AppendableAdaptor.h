// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "ArrayRef.h"
#include "Core/STL/Defines/MemberDetector.h"

namespace GX_STL
{
namespace GXTypes
{
namespace ContainerAdaptors
{

	namespace _adaptors_hidden_
	{
		GX_CREATE_MEMBER_DETECTOR( PushBack );
		GX_CREATE_MEMBER_DETECTOR( Reserve );
		GX_CREATE_MEMBER_DETECTOR( Count );
		GX_CREATE_MEMBER_DETECTOR( Add );
		GX_CREATE_MEMBER_DETECTOR( insert );
		GX_CREATE_MEMBER_DETECTOR( push_back );
		GX_CREATE_MEMBER_DETECTOR( reserve );
		GX_CREATE_MEMBER_DETECTOR( size );
		
		template <typename Class>
		static constexpr bool	Has_PushBack	=     Detect_PushBack<Class>::value	and
												  not Detect_Reserve<Class>::value	and
												  not Detect_Add<Class>::value		and
												  not Detect_insert<Class>::value	and
												  not Detect_push_back<Class>::value;
		template <typename Class>
		static constexpr bool	Has_Add			= not Detect_PushBack<Class>::value	and
												  not Detect_Reserve<Class>::value	and
												      Detect_Add<Class>::value		and
												  not Detect_insert<Class>::value	and
												  not Detect_push_back<Class>::value;
		template <typename Class>
		static constexpr bool	Has_pushback	= not Detect_PushBack<Class>::value	and
												  not Detect_Add<Class>::value		and
												  not Detect_reserve<Class>::value	and
												      Detect_push_back<Class>::value;
		template <typename Class>
		static constexpr bool	Has_insert		= not Detect_PushBack<Class>::value	and
												  not Detect_Add<Class>::value		and
												  not Detect_reserve<Class>::value	and
												      Detect_insert<Class>::value	and
												  not Detect_push_back<Class>::value;

		template <typename Class>
		static constexpr bool	Has_PushBack_Reserve	=     Detect_PushBack<Class>::value	and
														      Detect_Reserve<Class>::value	and
														      Detect_Count<Class>::value	and
														  not Detect_Add<Class>::value		and
														  not Detect_insert<Class>::value	and
														  not Detect_push_back<Class>::value;
		template <typename Class>
		static constexpr bool	Has_Add_Reserve			= not Detect_PushBack<Class>::value	and
														      Detect_Count<Class>::value	and
														      Detect_Reserve<Class>::value	and
															  Detect_Add<Class>::value		and
														  not Detect_insert<Class>::value	and
														  not Detect_push_back<Class>::value;
		template <typename Class>
		static constexpr bool	Has_pushback_reserve	= not Detect_PushBack<Class>::value	and
														  not Detect_Add<Class>::value		and
															  Detect_reserve<Class>::value	and
															  Detect_size<Class>::value		and
															  Detect_push_back<Class>::value;
		template <typename Class>
		static constexpr bool	Has_insert_reserve		= not Detect_PushBack<Class>::value	and
														  not Detect_Add<Class>::value		and
															  Detect_reserve<Class>::value	and
															  Detect_size<Class>::value		and
															  Detect_insert<Class>::value	and
														  not Detect_push_back<Class>::value;

	}	// _adaptors_hidden_


	//
	// Appendable Container
	//

	template <typename T>
	struct Appendable
	{
	// types
	private:
		using AppendFunc_t			= void (*) (void *container, ArrayCRef<T> elements);
		using AppendMovableFunc_t	= void (*) (void *container, ArrayRef<T> element);


	// variables
	private:
		void *					_ref			= null;
		AppendFunc_t			_append			= null;
		AppendMovableFunc_t		_appendMovable	= null;

		DEBUG_ONLY( TypeId		_dbgType; )


	// methods
	public:
		Appendable () = delete;

		Appendable (Appendable &&) = default;
		Appendable (const Appendable &) = default;

		Appendable& operator = (Appendable &&) = delete;
		Appendable& operator = (const Appendable &) = delete;

		template <typename Container>
		Appendable (Container &other)
			DEBUG_ONLY( : _dbgType(TypeIdOf<Container>()) )
		{
			_Create( other );
		}

		ND_ bool IsValid () const
		{
			return _ref and _append and _appendMovable;
		}

		ND_ explicit operator bool () const			{ return IsValid(); }

		void PushBack (const T &value)						{ Append( ArrayCRef<T>( &value, 1 ) ); }
		void PushBack (T &&value)							{ AppendMovable( ArrayRef<T>( &value, 1 ) ); }

		void Append (ArrayCRef<T> values)					{ ASSERT( IsValid() ); _append( _ref, values ); }
		void AppendMovable (ArrayRef<T> values)	noexcept	{ ASSERT( IsValid() ); _appendMovable( _ref, values ); }


	private:
		template <typename C>
		CompileTime::EnableIf<_adaptors_hidden_::Has_PushBack<C>>  _Create (C &other)
		{
			_ref			= PointerCast<void>( &other );
			_append			= LAMBDA() (void *ptr, ArrayCRef<T> elements)
								{
									auto&	arr = *PointerCast<C>( ptr );
									FOR( i, elements ) { arr.PushBack( elements[i] ); }
								};
			_appendMovable	= LAMBDA() (void *ptr, ArrayRef<T> elements)
								{
									auto&	arr = *PointerCast<C>( ptr );
									FOR( i, elements ) { arr.PushBack( RVREF(elements[i]) ); }
								};
		}

		template <typename C>
		CompileTime::EnableIf<_adaptors_hidden_::Has_PushBack_Reserve<C>>  _Create (C &other)
		{
			_ref			= PointerCast<void>( &other );
			_append			= LAMBDA() (void *ptr, ArrayCRef<T> elements)
								{
									auto&	arr = *PointerCast<C>( ptr );
									arr.Reserve( arr.Count() + elements.Count() );
									FOR( i, elements ) { arr.PushBack( elements[i] ); }
								};
			_appendMovable	= LAMBDA() (void *ptr, ArrayRef<T> elements)
								{
									auto&	arr = *PointerCast<C>( ptr );
									arr.Reserve( arr.Count() + elements.Count() );
									FOR( i, elements ) { arr.PushBack( RVREF(elements[i]) ); }
								};
		}

		template <typename C>
		CompileTime::EnableIf<_adaptors_hidden_::Has_Add<C>>  _Create (C &other)
		{
			_ref			= PointerCast<void>( &other );
			_append			= LAMBDA() (void *ptr, ArrayCRef<T> elements)
								{
									auto&	arr = *PointerCast<C>( ptr );
									FOR( i, elements ) { arr.Add( elements[i] ); }
								};
			_appendMovable	= LAMBDA() (void *ptr, ArrayRef<T> elements)
								{
									auto&	arr = *PointerCast<C>( ptr );
									FOR( i, elements ) { arr.Add( RVREF(elements[i]) ); }
								};
		}

		template <typename C>
		CompileTime::EnableIf<_adaptors_hidden_::Has_Add_Reserve<C>>  _Create (C &other)
		{
			_ref			= PointerCast<void>( &other );
			_append			= LAMBDA() (void *ptr, ArrayCRef<T> elements)
								{
									auto&	arr = *PointerCast<C>( ptr );
									arr.Reserve( arr.Count() + elements.Count() );
									FOR( i, elements ) { arr.Add( elements[i] ); }
								};
			_appendMovable	= LAMBDA() (void *ptr, ArrayRef<T> elements)
								{
									auto&	arr = *PointerCast<C>( ptr );
									arr.Reserve( arr.Count() + elements.Count() );
									FOR( i, elements ) { arr.Add( RVREF(elements[i]) ); }
								};
		}

		template <typename C>
		CompileTime::EnableIf<_adaptors_hidden_::Has_pushback<C>>  _Create (C &other)
		{
			_ref			= PointerCast<void>( &other );
			_append			= LAMBDA() (void *ptr, ArrayCRef<T> elements)
								{
									auto&	arr = *PointerCast<C>( ptr );
									FOR( i, elements ) { arr.push_back( elements[i] ); }
								};
			_appendMovable	= LAMBDA() (void *ptr, ArrayRef<T> elements)
								{
									auto&	arr = *PointerCast<C>( ptr );
									FOR( i, elements ) { arr.push_back( RVREF(elements[i]) ); }
								};
		}

		template <typename C>
		CompileTime::EnableIf<_adaptors_hidden_::Has_pushback_reserve<C>>  _Create (C &other)
		{
			_ref			= PointerCast<void>( &other );
			_append			= LAMBDA() (void *ptr, ArrayCRef<T> elements)
								{
									auto&	arr = *PointerCast<C>( ptr );
									arr.reserve( arr.size() + elements.Count() );
									FOR( i, elements ) { arr.push_back( elements[i] ); }
								};
			_appendMovable	= LAMBDA() (void *ptr, ArrayRef<T> elements)
								{
									auto&	arr = *PointerCast<C>( ptr );
									arr.reserve( arr.size() + elements.Count() );
									FOR( i, elements ) { arr.push_back( RVREF(elements[i]) ); }
								};
		}

		template <typename C>
		CompileTime::EnableIf<_adaptors_hidden_::Has_insert<C>>  _Create (C &other)
		{
			_ref			= PointerCast<void>( &other );
			_append			= LAMBDA() (void *ptr, ArrayCRef<T> elements)
								{
									auto&	arr = *PointerCast<C>( ptr );
									FOR( i, elements ) { arr.insert( elements[i] ); }
								};
			_appendMovable	= LAMBDA() (void *ptr, ArrayRef<T> elements)
								{
									auto&	arr = *PointerCast<C>( ptr );
									FOR( i, elements ) { arr.insert( RVREF(elements[i]) ); }
								};
		}

		template <typename C>
		CompileTime::EnableIf<_adaptors_hidden_::Has_insert_reserve<C>>  _Create (C &other)
		{
			_ref			= PointerCast<void>( &other );
			_append			= LAMBDA() (void *ptr, ArrayCRef<T> elements)
								{
									auto&	arr = *PointerCast<C>( ptr );
									arr.reserve( arr.size() + elements.Count() );
									FOR( i, elements ) { arr.insert( elements[i] ); }
								};
			_appendMovable	= LAMBDA() (void *ptr, ArrayRef<T> elements)
								{
									auto&	arr = *PointerCast<C>( ptr );
									arr.reserve( arr.size() + elements.Count() );
									FOR( i, elements ) { arr.insert( RVREF(elements[i]) ); }
								};
		}
	};


}	// ContainerAdaptors
}	// GXTypes
}	// GX_STL
