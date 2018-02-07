// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Containers/MapUtils.h"
#include "Engine/STL/Containers/IndexedArray.h"
#include "Engine/STL/CompileTime/FunctionInfo.h"

namespace GX_STL
{
namespace GXTypes
{
namespace _types_hidden_
{

	//
	// Base Hash Set (HashSet or MultiHashSet)
	//
	
	template <	template <typename T1, typename S1, typename MC1> class Container,
				typename Value,
				bool IsUnique,
				typename H,
				typename S,
				typename MC
			 >
	struct BaseHashSet : public CompileTime::CopyQualifiers< CompileTime::FastCopyable,
									Container< Pair<CompileTime::ResultOf<decltype(&H::operator())>, Value>, S, MC > >
	{
	// types
	public:
		using Self				= BaseHashSet< Container, Value, IsUnique, H, S, MC >;

		using Value_t			= Value;
		using Hash_t			= H;
		using const_iterator	= Ptr< const Value >;


	private:
		using KeyHash_t			= CompileTime::ResultOf<decltype(&H::operator())>;
		using Key_t				= Value;
		using hpair_t			= Pair< KeyHash_t, Key_t >;
		using KeyPair_t			= Pair< KeyHash_t, Key_t const& >;
		
		template <typename Key>
		struct _KeySearchCmp
		{
		// variables
			KeyHash_t		_keyHash;
			Key_t const&	_key;
			
		// methods
			_KeySearchCmp (const Key_t *v) : _keyHash( Hash_t()(*v) ), _key( *v ) {}
			_KeySearchCmp (const hpair_t *p) : _keyHash( p->first ), _key( p->second ) {}
			_KeySearchCmp (const KeyPair_t *p) : _keyHash( p->first ), _key( p->second ) {}

			bool operator == (const hpair_t &r) const	{ return _keyHash == r.first and _key == r.second; }
			bool operator != (const hpair_t &r) const	{ return not ( *this == r ); }
			bool operator <  (const hpair_t &r) const	{ return _keyHash == r.first ? _key < r.second : _keyHash < r.first; }
			bool operator >  (const hpair_t &r) const	{ return _keyHash == r.first ? _key > r.second : _keyHash > r.first; }
		};

		using _MapUtils_t	= _types_hidden_::MapUtils< Container< hpair_t, S, MC >, KeyPair_t, _KeySearchCmp, IsUnique >;


	// variables
	private:
		_MapUtils_t		_memory;
		Hash_t			_hasher;


	// methods
	public:
		BaseHashSet (GX_DEFCTOR)
		{}

		BaseHashSet (const Self &other) : _memory( other._memory )
		{}
		
		BaseHashSet (Self &&other) : _memory( RVREF( other._memory ) )
		{}
		
		BaseHashSet (InitializerList<Key_t> list)
		{
			AddArray( ArrayCRef<Key_t>( list ) );
		}
		
		BaseHashSet (ArrayCRef<Key_t> list)
		{
			AddArray( list );
		}


		Value const &	operator [] (usize i) const
		{
			return _memory[i].second;
		}
		
		Value const &	operator () (const Value &value) const
		{
			usize	idx = 0;
			FindIndex( value, OUT idx );
			return (*this)[ idx ].second;
		}


		Self &			operator << (Value &&value)
		{
			Add( RVREF( value ) );
			return *this;
		}

		template <typename V>
		Self &			operator << (const V &value)
		{
			Add( Value(value) );
			return *this;
		}


		bool			operator == (const Self &right) const
		{
			return _memory == right._memory;
		}

		bool			operator != (const Self &right) const
		{
			return not (*this == right);
		}
		

		Self &			operator =  (Self &&right)		= default;
		Self &			operator =  (const Self &right)	= default;


		Value const &	Front ()		const	{ return (*this)[0]; }
		Value const &	Back ()			const	{ return (*this)[ LastIndex() ]; }

		bool			Empty ()		const	{ return _memory.Empty(); }
		usize			Count ()		const	{ return _memory.Count(); }
		usize			LastIndex ()	const	{ return _memory.LastIndex(); }
		BytesU			Size ()			const	{ return _memory.Size(); }

		
		// if IsUnique == true
		// if Map contains same value, then the old value will be replaced
		const_iterator Add (const Value &value)
		{
			const KeyHash_t	hash = _hasher(value);
			const usize		idx  = _memory.AddOrReplace( RVREF(hpair_t( hash, value )) );
			return &(*this)[ idx ];
		}

		const_iterator Add (Value &&value)
		{
			const KeyHash_t	hash = _hasher(value);
			const usize		idx  = _memory.AddOrReplace( RVREF(hpair_t( hash, RVREF(value) )) );
			return &(*this)[ idx ];
		}
		

		// if IsUnique == true
		// if Map contains same value, then the old value will remains
		const_iterator AddOrSkip (const Value &value)
		{
			const KeyHash_t	hash = _hasher(value);
			const usize		idx  = _memory.AddOrSkip( RVREF(hpair_t( hash, value )) );
			return &(*this)[ idx ];
		}

		const_iterator AddOrSkip (Value &&value)
		{
			const KeyHash_t	hash = _hasher(value);
			const usize		idx  = _memory.AddOrSkip( RVREF(hpair_t( hash, RVREF(value) )) );
			return &(*this)[ idx ];
		}


		void AddArray (ArrayCRef<Value> value)
		{
			FOR( i, value ) {
				Add( value[i] );
			}
		}

		void AddArray (const Self &value)
		{
			FOR( i, value ) {
				Add( value[i] );
			}
		}

		void AddArray (Self &&value)
		{
			FOR( i, value ) {
				Add( RVREF( value[i] ) );
			}
		}


		bool FindIndex (const Value &key, OUT usize &idx) const
		{
			return FindFirstIndex( key, OUT idx );
		}

		bool FindFirstIndex (const Value &key, OUT usize &idx) const
		{
			return _memory.FindFirstIndex( KeyPair_t( _hasher( key ), key ), OUT idx );
		}

		void FindLastIndex (usize first, OUT usize &idx) const
		{
			return _memory.FindLastIndex( first, OUT idx );
		}


		bool IsExist (const Value &key) const
		{
			usize idx = 0;
			return FindIndex( key, OUT idx );
		}

		bool Find (const Value &key, OUT const_iterator & result) const
		{
			usize	idx = UMax;

			if ( not FindIndex( key, OUT idx ) )
				return false;

			result = &(*this)[ idx ];
			return true;
		}


		bool Erase (const Value &key)				{ return _memory.Erase( KeyPair_t( _hasher( key ), key ) ); }
		void EraseByIndex (usize index)				{ _memory.EraseByIndex( index ); }
		void EraseByIter (const_iterator it)		{ _memory.EraseByIndex( _memory.GetIndex( it.RawPtr() ) ); }

		void Free ()								{ _memory.Free(); }
		void Clear ()								{ _memory.Clear(); }
		void Resize (usize size)					{ _memory.Resize( size, false ); }
		void Reserve (usize size)					{ _memory.Reserve( size ); }
		

		static constexpr bool	IsLinearMemory ()			{ return _MapUtils_t::IsLinearMemory(); }
		constexpr bool			IsStaticMemory ()	const	{ return _memory.IsStaticMemory(); }
		

		friend void SwapValues (INOUT Self &left, INOUT Self &right)
		{
			SwapValues( left._memory, right._memory );
		}
	};

}	// _types_hidden_


	template <	typename Value,
				typename H = Hash< Value >,
				typename S = typename AutoDetectCopyStrategy< Pair< CompileTime::ResultOf<decltype(&H::operator())>, Value > >::type,
				typename MC = MemoryContainer< Pair< CompileTime::ResultOf<decltype(&H::operator())>, Value > >
			 >
	using HashSet = _types_hidden_::BaseHashSet< Array, Value, true, H, S, MC >;
	

	template <	typename Value,
				typename H = Hash< Value >,
				typename S = typename AutoDetectCopyStrategy< Pair< CompileTime::ResultOf<decltype(&H::operator())>, Value > >::type,
				typename MC = MemoryContainer< Pair< CompileTime::ResultOf<decltype(&H::operator())>, Value > >
			 >
	using MultiHashSet = _types_hidden_::BaseHashSet< Array, Value, false, H, S, MC >;
	

	template <	typename Value,
				usize Size,
				typename H = Hash< Value >
			 >
	using FixedSizeHashSet = _types_hidden_::BaseHashSet< Array, Value, true, H,
								typename AutoDetectCopyStrategy< Pair< CompileTime::ResultOf<decltype(&H::operator())>, Value > >::type,
								StaticMemoryContainer< Pair< CompileTime::ResultOf<decltype(&H::operator())>, Value >, Size > >;
	
	
	template <	typename Value,
				usize Size,
				typename H = Hash< Value >
			 >
	using FixedSizeMultiHashSet = _types_hidden_::BaseHashSet< Array, Value, false, H,
									typename AutoDetectCopyStrategy< Pair< CompileTime::ResultOf<decltype(&H::operator())>, Value > >::type,
									StaticMemoryContainer< Pair< CompileTime::ResultOf<decltype(&H::operator())>, Value >, Size > >;

	
	template <	template <typename, typename, typename> class Container,
				typename Value,
				bool IsUnique,
				typename H,
				typename S,
				typename MC
			 >
	struct Hash< _types_hidden_::BaseHashSet< Container, Value, IsUnique, H, S, MC > >
	{
		HashResult  operator () (const _types_hidden_::BaseHashSet< Container, Value, IsUnique, H, S, MC > &x) const noexcept
		{
			return HashOf( ArrayCRef<Pair< CompileTime::ResultOf<decltype(&H::operator())>, Value > >( x ) );
		}
	};

}	// GXTypes
}	// GX_STL
