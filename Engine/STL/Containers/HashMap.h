// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "MapUtils.h"

namespace GX_STL
{
namespace GXTypes
{
namespace _types_hidden_
{


	//
	// Base Hash Map (HashMap or MultiHashMap)
	//
	
	template <	typename K,
				typename T,
				bool IsUnique,
				typename H,
				typename S,
				typename MC
			 >
	struct BaseHashMap : public CompileTime::CopyQualifiers< CompileTime::FastCopyable, MC >
	{
	// types
	public:
		typedef BaseHashMap< K, T, IsUnique, H, S, MC >		Self;

		typedef K									key_t;
		typedef T									value_t;
		typedef H									hash_t;

		typedef Pair< K, T >						pair_t;
		typedef Pair< const K, T>					const_pair_t;

		typedef Ptr< const_pair_t >					iterator;
		typedef Ptr< const const_pair_t >			const_iterator;


	private:
		typedef typename H::result_t				key_hash_t;
		typedef Pair< key_hash_t, key_t const& >	key_pair_t;
		typedef Pair< key_hash_t, pair_t >			triple_t;

		template <typename Key>
		struct _KeySearchCmp
		{
		// variables
			key_hash_t const	_keyHash;
			key_t const &		_key;
			
		// methods
			_KeySearchCmp (const key_t *k) : _keyHash( hash_t()(*k) ), _key( *k ) {}
			_KeySearchCmp (const triple_t *t) : _keyHash( t->first ), _key( t->second.first ) {}
			_KeySearchCmp (const key_pair_t *p) : _keyHash( p->first ), _key( p->second ) {}

			bool operator == (const triple_t &r) const	{ return _keyHash == r.first and _key == r.second.first; }
			bool operator != (const triple_t &r) const	{ return not ( *this == r ); }
			bool operator <  (const triple_t &r) const	{ return _keyHash == r.first ? _key < r.second.first : _keyHash < r.first; }
			bool operator >  (const triple_t &r) const	{ return _keyHash == r.first ? _key > r.second.first : _keyHash > r.first; }
		};

		typedef _types_hidden_::MapUtils< Array< triple_t, S, MC >, key_pair_t, _KeySearchCmp, IsUnique >	_MapUtils_t;


	// variables
	private:
		_MapUtils_t		_memory;
		hash_t			_hash;


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


		const_pair_t &			operator [] (usize i)
		{
			return ReferenceCast< const_pair_t >( _memory[i].second );
		}

		const_pair_t const &	operator [] (usize i) const
		{
			return ReferenceCast< const_pair_t >( _memory[i].second );
		}
		

		const_pair_t &			operator () (const key_t &key)
		{
			usize	idx;
			FindIndex( key, idx );
			return (*this)[ idx ];
		}
		
		const_pair_t const &	operator () (const key_t &key) const
		{
			usize	idx;
			FindIndex( key, idx );
			return (*this)[ idx ];
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
			return _memory == right._memory;
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
		usize Add (const key_t &key, const value_t &value)
		{
			const key_hash_t	hash = _hash( key );
			return _memory.AddOrReplace( RVREF(triple_t( hash, pair_t( key, value ))) );
		}

		usize Add (key_t &&key, value_t &&value)
		{
			const key_hash_t	hash = _hash( key );
			return _memory.AddOrReplace( RVREF(triple_t( hash, RVREF(pair_t( RVREF(key), RVREF(value) )) )) );
		}

		usize Add (const pair_t &value)
		{
			const key_hash_t	hash = _hash( value.first );
			return _memory.AddOrReplace( RVREF(triple_t( hash, RVREF(pair_t( value.first, value.second )) )) );
		}

		usize Add (pair_t &&value)
		{
			const key_hash_t	hash = _hash( value.first );
			return _memory.AddOrReplace( RVREF(triple_t( hash, RVREF(value) )) );
		}
		

		// if IsUnique == true
		// if Map contains same value, then the old value will remains
		usize AddOrSkip (const key_t &key, const value_t &value)
		{
			const key_hash_t	hash = _hash( key );
			return _memory.AddOrSkip( RVREF(triple_t( hash, key, value )) );
		}

		usize AddOrSkip (key_t &&key, value_t &&value)
		{
			const key_hash_t	hash = _hash( key );
			return _memory.AddOrSkip( RVREF(triple_t( hash, RVREF(pair_t( RVREF(key), RVREF(value) )) )) );
		}

		usize AddOrSkip (const pair_t &value)
		{
			const key_hash_t	hash = _hash( value.first );
			return _memory.AddOrSkip( RVREF(triple_t( hash, RVREF(pair_t( value.first, value.second )) )) );
		}

		usize AddOrSkip (pair_t &&value)
		{
			const key_hash_t	hash = _hash( value.first );
			return _memory.AddOrSkip( RVREF(triple_t( hash, RVREF(value) )) );
		}


		void AddArray (ArrayCRef<pair_t> value)
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


		bool FindIndex (const key_t &key, OUT usize &idx) const
		{
			return FindFirstIndex( key, OUT idx );
		}

		bool FindFirstIndex (const key_t &key, OUT usize &idx) const
		{
			return _memory.FindFirstIndex( key_pair_t( _hash( key ), key ), OUT idx );
		}

		void FindLastIndex (usize first, OUT usize &idx) const
		{
			return _memory.FindLastIndex( first, OUT idx );
		}


		bool IsExist (const key_t &key) const
		{
			usize idx = 0;
			return FindIndex( key, OUT idx );
		}

		bool Find (const key_t &key, OUT iterator & result)
		{
			usize	idx = -1;

			if ( not FindIndex( key, OUT idx ) )
				return false;

			result = &(*this)[ idx ];
			return true;
		}

		bool Find (const key_t &key, OUT const_iterator & result) const
		{
			usize	idx = -1;

			if ( not FindIndex( key, OUT idx ) )
				return false;

			result = &(*this)[ idx ];
			return true;
		}


		bool Erase (const key_t &key)					{ return _memory.Erase( key_pair_t( _hash( key ), key ) ); }
		void EraseFromIndex (usize index)				{ return _memory.EraseFromIndex( index ); }
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
				typename S = typename AutoDetectCopyStrategy< Pair< typename H::result_t, Pair<K, T> > >::type,
				typename MC = MemoryContainer< Pair< typename H::result_t, Pair<K, T> > >
			 >
	using HashMap = _types_hidden_::BaseHashMap< K, T, true, H, S, MC >;
	

	template <	typename K,
				typename T,
				typename H = Hash< K >,
				typename S = typename AutoDetectCopyStrategy< Pair< typename H::result_t, Pair<K, T> > >::type,
				typename MC = MemoryContainer< Pair< typename H::result_t, Pair<K, T> > >
			 >
	using MultiHashMap = _types_hidden_::BaseHashMap< K, T, false, H, S, MC >;
	

	template <	typename K,
				typename T,
				usize Size,
				typename H = Hash< K >
			 >
	using FixedSizeHashMap = _types_hidden_::BaseHashMap< K, T, true, H,
								typename AutoDetectCopyStrategy< Pair< typename H::result_t, Pair<K, T> > >::type,
								StaticMemoryContainer< Pair< typename H::result_t, Pair<K, T> >, Size > >;
	
	
	template <	typename K,
				typename T,
				usize Size,
				typename H = Hash< K >
			 >
	using FixedSizeMultiHashMap = _types_hidden_::BaseHashMap< K, T, false, H,
									typename AutoDetectCopyStrategy< Pair< typename H::result_t, Pair<K, T> > >::type,
									StaticMemoryContainer< Pair< typename H::result_t, Pair<K, T> >, Size > >;

	
	template <typename K, typename T, bool IsUnique, typename H, typename S, typename MC>
	struct Hash< _types_hidden_::BaseHashMap< K, T, IsUnique, H, S, MC > > :
		private Hash< ArrayCRef<Pair< typename H::result_t, Pair<K, T> > > >
	{
		typedef _types_hidden_::BaseHashMap< K, T, IsUnique, H, S, MC >				key_t;
		typedef Hash< ArrayCRef<Pair< typename H::result_t, Pair<K, T> > > >		base_t;
		typedef typename base_t::result_t											result_t;

		result_t operator () (const key_t &x) const
		{
			return base_t::operator ()( x );
		}
	};

}	// GXTypes
}	// GX_STL
