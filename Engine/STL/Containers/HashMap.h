// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Containers/MapUtils.h"
#include "Engine/STL/Containers/IndexedArray.h"

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
									Container< Pair< typename H::Result_t, Pair< K, T > >, S, MC > >
	{
	// types
	public:
		using Self				= BaseHashMap< Container, K, T, IsUnique, H, S, MC >;

		using Key_t				= K;
		using Value_t			= T;
		using Hash_t			= H;

		using pair_t			= Pair< K, T >;
		using const_pair_t		= Pair< const K, T>;

		using iterator			= Ptr< const_pair_t >;
		using const_iterator	= Ptr< const const_pair_t >;


	private:
		using KeyHash_t			= typename H::Result_t;
		using KeyPair_t			= Pair< KeyHash_t, Key_t const& >;
		using triple_t			= Pair< KeyHash_t, pair_t >;

		template <typename Key>
		struct _KeySearchCmp
		{
		// variables
			KeyHash_t const	_keyHash;
			Key_t const &		_key;
			
		// methods
			_KeySearchCmp (const Key_t *k) : _keyHash( Hash_t()(*k) ), _key( *k ) {}
			_KeySearchCmp (const triple_t *t) : _keyHash( t->first ), _key( t->second.first ) {}
			_KeySearchCmp (const KeyPair_t *p) : _keyHash( p->first ), _key( p->second ) {}

			bool operator == (const triple_t &r) const	{ return _keyHash == r.first and _key == r.second.first; }
			bool operator != (const triple_t &r) const	{ return not ( *this == r ); }
			bool operator <  (const triple_t &r) const	{ return _keyHash == r.first ? _key < r.second.first : _keyHash < r.first; }
			bool operator >  (const triple_t &r) const	{ return _keyHash == r.first ? _key > r.second.first : _keyHash > r.first; }
		};

		using _MapUtils_t	= _types_hidden_::MapUtils< Container< triple_t, S, MC >, KeyPair_t, _KeySearchCmp, IsUnique >;


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
		
		BaseHashMap (InitializerList<pair_t> list)
		{
			AddArray( ArrayCRef<pair_t>( list ) );
		}
		
		BaseHashMap (ArrayCRef<pair_t> list)
		{
			AddArray( list );
		}


		const_pair_t &			operator [] (usize i)
		{
			return ReferenceCast< const_pair_t >( _memory[i].second );
		}

		const_pair_t const &	operator [] (usize i) const
		{
			return ReferenceCast< const_pair_t >( _memory[i].second );
		}
		

		Value_t &		operator () (const Key_t &key)
		{
			usize	idx = 0;
			FindIndex( key, OUT idx );
			return (*this)[ idx ].second.second;
		}
		
		Value_t const&	operator () (const Key_t &key) const
		{
			usize	idx = 0;
			FindIndex( key, OUT idx );
			return (*this)[ idx ].second.second;
		}


		Self &			operator << (pair_t &&value)
		{
			Add( RVREF( value ) );
			return *this;
		}

		Self &			operator << (const pair_t &value)
		{
			Add( value );
			return *this;
		}


		bool			operator == (const Self &right) const
		{
			if ( Count() != right.Count() )
				return false;

			for (usize i = 0; i < Count(); ++i) {
				if ( (*this)[i] != right[i] )
					return false;
			}
			return true;
		}

		bool			operator != (const Self &right) const
		{
			return not (*this == right);
		}
		

		Self &			operator =  (Self &&right)		= default;
		Self &			operator =  (const Self &right)	= default;


		const_pair_t &			Front ()				{ return (*this)[0]; }
		const_pair_t const &	Front ()		const	{ return (*this)[0]; }
		const_pair_t &			Back ()					{ return (*this)[ LastIndex() ]; }
		const_pair_t const &	Back ()			const	{ return (*this)[ LastIndex() ]; }

		bool					Empty ()		const	{ return _memory.Empty(); }
		usize					Count ()		const	{ return _memory.Count(); }
		usize					LastIndex ()	const	{ return _memory.LastIndex(); }
		BytesU					Size ()			const	{ return _memory.Size(); }

		
		// if IsUnique == true
		// if Map contains same value, then the old value will be replaced
		usize Add (const Key_t &key, const Value_t &value)
		{
			const KeyHash_t	hash = _hasher( key );
			return _memory.AddOrReplace( RVREF(triple_t( hash, pair_t( key, value ))) );
		}

		usize Add (Key_t &&key, Value_t &&value)
		{
			const KeyHash_t	hash = _hasher( key );
			return _memory.AddOrReplace( RVREF(triple_t( hash, RVREF(pair_t( RVREF(key), RVREF(value) )) )) );
		}

		usize Add (const pair_t &value)
		{
			const KeyHash_t	hash = _hasher( value.first );
			return _memory.AddOrReplace( RVREF(triple_t( hash, RVREF(pair_t( value.first, value.second )) )) );
		}

		usize Add (pair_t &&value)
		{
			const KeyHash_t	hash = _hasher( value.first );
			return _memory.AddOrReplace( RVREF(triple_t( hash, RVREF(value) )) );
		}
		

		// if IsUnique == true
		// if Map contains same value, then the old value will remains
		usize AddOrSkip (const Key_t &key, const Value_t &value)
		{
			const KeyHash_t	hash = _hasher( key );
			return _memory.AddOrSkip( RVREF(triple_t( hash, key, value )) );
		}

		usize AddOrSkip (Key_t &&key, Value_t &&value)
		{
			const KeyHash_t	hash = _hasher( key );
			return _memory.AddOrSkip( RVREF(triple_t( hash, RVREF(pair_t( RVREF(key), RVREF(value) )) )) );
		}

		usize AddOrSkip (const pair_t &value)
		{
			const KeyHash_t	hash = _hasher( value.first );
			return _memory.AddOrSkip( RVREF(triple_t( hash, RVREF(pair_t( value.first, value.second )) )) );
		}

		usize AddOrSkip (pair_t &&value)
		{
			const KeyHash_t	hash = _hasher( value.first );
			return _memory.AddOrSkip( RVREF(triple_t( hash, RVREF(value) )) );
		}


		void AddArray (ArrayCRef<pair_t> value)
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


		bool IsExist (const Key_t &key) const
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
		void EraseByIndex (usize index)					{ return _memory.EraseByIndex( index ); }
		void Free ()									{ _memory.Free(); }
		void Clear ()									{ _memory.Clear(); }
		void Resize (usize uSize)						{ _memory.Resize( uSize, false ); }
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
				typename S = typename AutoDetectCopyStrategy< Pair< typename H::Result_t, Pair<K, T> > >::type,
				typename MC = MemoryContainer< Pair< typename H::Result_t, Pair<K, T> > >
			 >
	using HashMap = _types_hidden_::BaseHashMap< Array, K, T, true, H, S, MC >;
	

	template <	typename K,
				typename T,
				typename H = Hash< K >,
				typename S = typename AutoDetectCopyStrategy< Pair< typename H::Result_t, Pair<K, T> > >::type,
				typename MC = MemoryContainer< Pair< typename H::Result_t, Pair<K, T> > >
			 >
	using MultiHashMap = _types_hidden_::BaseHashMap< Array, K, T, false, H, S, MC >;
	

	template <	typename K,
				typename T,
				usize Size,
				typename H = Hash< K >
			 >
	using FixedSizeHashMap = _types_hidden_::BaseHashMap< Array, K, T, true, H,
								typename AutoDetectCopyStrategy< Pair< typename H::Result_t, Pair<K, T> > >::type,
								StaticMemoryContainer< Pair< typename H::Result_t, Pair<K, T> >, Size > >;
	
	
	template <	typename K,
				typename T,
				usize Size,
				typename H = Hash< K >
			 >
	using FixedSizeMultiHashMap = _types_hidden_::BaseHashMap< Array, K, T, false, H,
									typename AutoDetectCopyStrategy< Pair< typename H::Result_t, Pair<K, T> > >::type,
									StaticMemoryContainer< Pair< typename H::Result_t, Pair<K, T> >, Size > >;

	
	template <	template <typename T1, typename S1, typename MC1> class Container,
				typename K,
				typename T,
				bool IsUnique,
				typename H,
				typename S,
				typename MC
			 >
	struct Hash< _types_hidden_::BaseHashMap< Container, K, T, IsUnique, H, S, MC > > :
		private Hash< ArrayCRef<Pair< typename H::Result_t, Pair<K, T> > > >
	{
		typedef _types_hidden_::BaseHashMap< Container, K, T, IsUnique, H, S, MC >	Key_t;
		typedef Hash< ArrayCRef<Pair< typename H::Result_t, Pair<K, T> > > >		Base_t;
		typedef typename Base_t::Result_t											Result_t;

		Result_t operator () (const Key_t &x) const noexcept
		{
			return Base_t::operator ()( x );
		}
	};

}	// GXTypes
}	// GX_STL
