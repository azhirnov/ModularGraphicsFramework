// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Containers/MapUtils.h"
#include "Core/STL/Containers/IndexedArray.h"
#include "Core/STL/Containers/UniBuffer.h"
#include "Core/STL/Containers/IndexedIterator.h"
#include "Core/STL/CompileTime/FunctionInfo.h"
#include "Core/STL/CompileTime/FunctionInfo.h"

namespace GX_STL
{
namespace GXTypes
{
namespace _types_hidden_
{


	//
	// Base Hash Map (HashMap or MultiHashMap)
	//
	
	template <	template <typename T1, typename S1, typename MC1> class Container,
				typename K,
				typename T,
				bool IsUnique,
				typename H,
				typename S,
				typename MC
			 >
	struct BaseHashMap : public CompileTime::CopyQualifiers< CompileTime::FastCopyable, 
									Container< Pair< CompileTime::ResultOf<decltype(&H::operator())>, Pair< K, T > >, S, MC > >
	{
	// types
	public:
		using Self				= BaseHashMap< Container, K, T, IsUnique, H, S, MC >;

		using Key_t				= K;
		using Value_t			= T;
		using Hash_t			= H;

		using Pair_t			= Pair< K, T >;
		using CPair_t			= Pair< const K, T>;

		using iterator			= Ptr< CPair_t >;
		using const_iterator	= Ptr< const CPair_t >;	// TODO: rename
		
		using idx_iterator		= IndexedIterator< CPair_t >;
		using idx_const_iterator= IndexedIterator< const CPair_t >;


	private:
		using KeyHash_t			= CompileTime::ResultOf<decltype(&H::operator())>;
		using KeyPair_t			= Pair< KeyHash_t, Key_t const& >;
		using Triple_t			= Pair< KeyHash_t, Pair_t >;
		using CTriple_t			= Pair< KeyHash_t, CPair_t >;

		template <typename Key>
		struct _KeySearchCmp
		{
		// variables
			KeyHash_t const		_keyHash;
			Key_t const &		_key;
			
		// methods
			_KeySearchCmp (const Key_t *k) : _keyHash( Hash_t()(*k) ), _key( *k ) {}
			_KeySearchCmp (const Triple_t *t) : _keyHash( t->first ), _key( t->second.first ) {}
			_KeySearchCmp (const KeyPair_t *p) : _keyHash( p->first ), _key( p->second ) {}

			bool operator == (const Triple_t &r) const	{ return _keyHash == r.first and _key == r.second.first; }
			bool operator != (const Triple_t &r) const	{ return not ( *this == r ); }
			bool operator <  (const Triple_t &r) const	{ return _keyHash != r.first ? _keyHash < r.first : r.second.first > _key; }
			bool operator >  (const Triple_t &r) const	{ return _keyHash != r.first ? _keyHash > r.first : _key > r.second.first; }
		};

		using _MapUtils_t	= _types_hidden_::MapUtils< Container< Triple_t, S, MC >, KeyPair_t, _KeySearchCmp, IsUnique >;


	// variables
	private:
		_MapUtils_t		_memory;
		Hash_t			_hasher;


	// methods
	public:
		BaseHashMap (GX_DEFCTOR)
		{}

		BaseHashMap (const Self &other) : _memory( other._memory )
		{}
		
		BaseHashMap (Self &&other) : _memory( RVREF( other._memory ) )
		{}
		
		BaseHashMap (InitializerList<Pair_t> list)
		{
			AddArray( ArrayCRef<Pair_t>( list ) );
		}
		
		BaseHashMap (ArrayCRef<Pair_t> list)
		{
			AddArray( list );
		}


		ND_ CPair_t &		operator [] (usize i)
		{
			return ReferenceCast< CPair_t >( _memory[i].second );
		}

		ND_ CPair_t const &	operator [] (usize i) const
		{
			return ReferenceCast< CPair_t >( _memory[i].second );
		}
		

		ND_ Value_t &		operator () (const Key_t &key)
		{
			usize	idx = 0;
			FindIndex( key, OUT idx );
			return (*this)[ idx ].second.second;
		}
		
		ND_ Value_t const&	operator () (const Key_t &key) const
		{
			usize	idx = 0;
			FindIndex( key, OUT idx );
			return (*this)[ idx ].second.second;
		}


		Self &		operator << (Pair_t &&value)
		{
			Add( RVREF( value ) );
			return *this;
		}

		Self &		operator << (const Pair_t &value)
		{
			Add( value );
			return *this;
		}


		ND_ bool	operator == (const Self &right) const
		{
			if ( Count() != right.Count() )
				return false;

			for (usize i = 0; i < Count(); ++i) {
				if ( (*this)[i] != right[i] )
					return false;
			}
			return true;
		}

		ND_ bool	operator != (const Self &right) const
		{
			return not (*this == right);
		}
		

		ND_ operator UniBuffer<CPair_t> ()
		{
			return UniBuffer<CPair_t>{ reinterpret_cast<CTriple_t *>(ArrayRef<Triple_t>{_memory}.RawPtr()) + OffsetOf( &Triple_t::second ), Count(), SizeOf<Triple_t> };
		}
		
		ND_ operator UniBuffer<const CPair_t> () const
		{
			return UniBuffer<const CPair_t>{ reinterpret_cast<CTriple_t const*>(ArrayCRef<Triple_t>{_memory}.RawPtr()) + OffsetOf( &Triple_t::second ), Count(), SizeOf<Triple_t> };
		}


		Self &		operator =  (Self &&right)		= default;
		Self &		operator =  (const Self &right)	= default;


		ND_ CPair_t &			Front ()				{ return (*this)[0]; }
		ND_ CPair_t const &		Front ()		const	{ return (*this)[0]; }
		ND_ CPair_t &			Back ()					{ return (*this)[ LastIndex() ]; }
		ND_ CPair_t const &		Back ()			const	{ return (*this)[ LastIndex() ]; }

		ND_ bool				Empty ()		const	{ return _memory.Empty(); }
		ND_ usize				Count ()		const	{ return _memory.Count(); }
		ND_ usize				LastIndex ()	const	{ return _memory.LastIndex(); }
		ND_ BytesU				Size ()			const	{ return _memory.Size(); }

		
		ND_ auto				begin ()				{ return idx_iterator{ *this, 0 }; }
		ND_ auto				begin ()		const	{ return idx_const_iterator{ *this, 0 }; }
		ND_ auto				end ()					{ return idx_iterator{ *this, Count() }; }
		ND_ auto				end ()			const	{ return idx_const_iterator{ *this, Count() }; }


		// if IsUnique == true
		// if Map contains same value, then the old value will be replaced
		iterator Add (const Key_t &key, const Value_t &value)
		{
			KeyHash_t		hash = _hasher( key );
			const usize		idx  = _memory.AddOrReplace( Triple_t{ RVREF(hash), Pair_t{ key, value }} );
			return &(*this)[ idx ];
		}

		iterator Add (Key_t &&key, Value_t &&value)
		{
			KeyHash_t		hash = _hasher( key );
			const usize		idx  = _memory.AddOrReplace( Triple_t{ RVREF(hash), Pair_t{ RVREF(key), RVREF(value) }} );
			return &(*this)[ idx ];
		}

		iterator Add (const Pair_t &value)
		{
			KeyHash_t		hash = _hasher( value.first );
			const usize		idx  = _memory.AddOrReplace( Triple_t{ RVREF(hash), Pair_t{ value.first, value.second }} );
			return &(*this)[ idx ];
		}

		iterator Add (Pair_t &&value)
		{
			KeyHash_t		hash = _hasher( value.first );
			const usize		idx  = _memory.AddOrReplace( Triple_t{ RVREF(hash), RVREF(value) } );
			return &(*this)[ idx ];
		}
		

		// if IsUnique == true
		// if Map contains same value, then the old value will remains
		iterator AddOrSkip (const Key_t &key, const Value_t &value)
		{
			KeyHash_t		hash = _hasher( key );
			const usize		idx  = _memory.AddOrSkip( Triple_t{ RVREF(hash), Pair_t{ key, value }} );
			return &(*this)[ idx ];
		}

		iterator AddOrSkip (Key_t &&key, Value_t &&value)
		{
			KeyHash_t		hash = _hasher( key );
			const usize		idx  = _memory.AddOrSkip( Triple_t{ RVREF(hash), Pair_t{ RVREF(key), RVREF(value) }} );
			return &(*this)[ idx ];
		}

		iterator AddOrSkip (const Pair_t &value)
		{
			KeyHash_t		hash = _hasher( value.first );
			const usize		idx  = _memory.AddOrSkip( Triple_t{ RVREF(hash), Pair_t{ value.first, value.second }} );
			return &(*this)[ idx ];
		}

		iterator AddOrSkip (Pair_t &&value)
		{
			KeyHash_t		hash = _hasher( value.first );
			const usize		idx  = _memory.AddOrSkip( Triple_t{ RVREF(hash), RVREF(value) } );
			return &(*this)[ idx ];
		}


		void AddArray (ArrayCRef<Pair_t> value)
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


		bool FindIndex (const Key_t &key, OUT usize &idx) const
		{
			return FindFirstIndex( key, OUT idx );
		}

		bool FindFirstIndex (const Key_t &key, OUT usize &idx) const
		{
			return _memory.FindFirstIndex( KeyPair_t( _hasher( key ), key ), OUT idx );
		}

		void FindLastIndex (usize first, OUT usize &idx) const
		{
			return _memory.FindLastIndex( first, OUT idx );
		}


		ND_ bool IsExist (const Key_t &key) const
		{
			usize idx = 0;
			return FindIndex( key, OUT idx );
		}

		bool Find (const Key_t &key, OUT iterator & result)
		{
			usize	idx = UMax;

			if ( not FindIndex( key, OUT idx ) )
				return false;

			result = &(*this)[ idx ];
			return true;
		}

		bool Find (const Key_t &key, OUT const_iterator & result) const
		{
			usize	idx = UMax;

			if ( not FindIndex( key, OUT idx ) )
				return false;

			result = &(*this)[ idx ];
			return true;
		}


		bool Erase (const Key_t &key)					{ return _memory.Erase( KeyPair_t( _hasher( key ), key ) ); }
		void EraseByIndex (usize index)					{ _memory.EraseByIndex( index ); }
		void EraseByIter (iterator it)					{ _memory.EraseByIndex( _memory.GetIndex( Cast<Triple_t const*>(it.RawPtr()) ) ); }
		void EraseByIter (const_iterator it)			{ _memory.EraseByIndex( _memory.GetIndex( Cast<Triple_t const*>(it.RawPtr()) ) ); }

		void Free ()									{ _memory.Free(); }
		void Clear ()									{ _memory.Clear(); }
		void Resize (usize size)						{ _memory.Resize( size, false ); }
		void Reserve (usize size)						{ _memory.Reserve( size ); }
		

		static constexpr bool	IsLinearMemory ()			{ return _MapUtils_t::IsLinearMemory(); }
		constexpr bool			IsStaticMemory ()	const	{ return _memory.IsStaticMemory(); }
		

		friend void SwapValues (INOUT Self &left, INOUT Self &right)
		{
			SwapValues( left._memory, right._memory );
		}
	};

}	// _types_hidden_


	template <	typename K,
				typename T,
				typename H = Hash< K >,
				typename S = typename AutoDetectCopyStrategy< Pair< CompileTime::ResultOf<decltype(&H::operator())>, Pair<K, T> > >::type,
				typename MC = MemoryContainer< Pair< CompileTime::ResultOf<decltype(&H::operator())>, Pair<K, T> > >
			 >
	using HashMap = _types_hidden_::BaseHashMap< Array, K, T, true, H, S, MC >;
	

	template <	typename K,
				typename T,
				typename H = Hash< K >,
				typename S = typename AutoDetectCopyStrategy< Pair< CompileTime::ResultOf<decltype(&H::operator())>, Pair<K, T> > >::type,
				typename MC = MemoryContainer< Pair< CompileTime::ResultOf<decltype(&H::operator())>, Pair<K, T> > >
			 >
	using MultiHashMap = _types_hidden_::BaseHashMap< Array, K, T, false, H, S, MC >;
	

	template <	typename K,
				typename T,
				usize Size,
				typename H = Hash< K >
			 >
	using FixedSizeHashMap = _types_hidden_::BaseHashMap< Array, K, T, true, H,
								typename AutoDetectCopyStrategy< Pair< typename CompileTime::ResultOf<decltype(&H::operator())>, Pair<K, T> > >::type,
								StaticMemoryContainer< Pair< typename CompileTime::ResultOf<decltype(&H::operator())>, Pair<K, T> >, Size > >;
	
	
	template <	typename K,
				typename T,
				usize Size,
				typename H = Hash< K >
			 >
	using FixedSizeMultiHashMap = _types_hidden_::BaseHashMap< Array, K, T, false, H,
									typename AutoDetectCopyStrategy< Pair< CompileTime::ResultOf<decltype(&H::operator())>, Pair<K, T> > >::type,
									StaticMemoryContainer< Pair< CompileTime::ResultOf<decltype(&H::operator())>, Pair<K, T> >, Size > >;

	
	template <	template <typename T1, typename S1, typename MC1> class Container,
				typename K,
				typename T,
				bool IsUnique,
				typename H,
				typename S,
				typename MC
			 >
	struct Hash< _types_hidden_::BaseHashMap< Container, K, T, IsUnique, H, S, MC > >
	{
		HashResult  operator () (const _types_hidden_::BaseHashMap< Container, K, T, IsUnique, H, S, MC > &x) const noexcept
		{
			return HashOf( ArrayCRef<Pair< CompileTime::ResultOf<decltype(&H::operator())>, Pair<K, T> > >( x ) );
		}
	};

}	// GXTypes
}	// GX_STL
