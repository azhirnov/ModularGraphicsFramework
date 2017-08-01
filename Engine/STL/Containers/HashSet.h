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
	// Base Hash Set (HashSet or MultiHashSet)
	//
	
	template <	typename Value,
				bool IsUnique,
				typename H,
				typename S,
				typename MC
			 >
	struct BaseHashSet : public CompileTime::CopyQualifiers< CompileTime::FastCopyable, MC >
	{
	// types
	public:
		typedef BaseHashSet< Value, IsUnique, H, S, MC >	Self;

		typedef Value								value_t;
		typedef H									hash_t;
		typedef Ptr< const Value >					const_iterator;


	private:
		typedef typename H::result_t				key_hash_t;
		typedef Value								key_t;
		typedef Pair< key_hash_t, key_t >			hpair_t;
		typedef Pair< key_hash_t, key_t const& >	key_pair_t;
		
		template <typename Key>
		struct _KeySearchCmp
		{
		// variables
			key_hash_t		_keyHash;
			key_t const&	_key;
			
		// methods
			_KeySearchCmp (const key_t *v) : _keyHash( hash_t()(*v) ), _key( *v ) {}
			_KeySearchCmp (const hpair_t *p) : _keyHash( p->first ), _key( p->second ) {}
			_KeySearchCmp (const key_pair_t *p) : _keyHash( p->first ), _key( p->second ) {}

			bool operator == (const hpair_t &r) const	{ return _keyHash == r.first and _key == r.second; }
			bool operator != (const hpair_t &r) const	{ return not ( *this == r ); }
			bool operator <  (const hpair_t &r) const	{ return _keyHash == r.first ? _key < r.second : _keyHash < r.first; }
			bool operator >  (const hpair_t &r) const	{ return _keyHash == r.first ? _key > r.second : _keyHash > r.first; }
		};

		typedef _types_hidden_::MapUtils< Array< hpair_t, S, MC >, key_pair_t, _KeySearchCmp, IsUnique >	_MapUtils_t;


	// variables
	private:
		_MapUtils_t		_memory;
		hash_t			_hash;


	// methods
	public:
		BaseHashSet (GX_DEFCTOR)
		{}

		BaseHashSet (const Self &other) : _memory( other._memory )
		{}
		
		BaseHashSet (Self &&other) : _memory( RVREF( other._memory ) )
		{}
		
		BaseHashSet (InitializerList<key_t> list)
		{
			AddArray( ArrayCRef<key_t>( list ) );
		}


		Value const &	operator [] (usize i) const
		{
			return _memory[i].second;
		}
		
		Value const &	operator () (const Value &value) const
		{
			usize	idx;
			FindIndex( value, idx );
			return (*this)[ idx ];
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
		usize Add (const Value &value)
		{
			const key_hash_t	hash = _hash(value);
			return _memory.AddOrReplace( RVREF(hpair_t( hash, value )) );
		}

		usize Add (Value &&value)
		{
			const key_hash_t	hash = _hash(value);
			return _memory.AddOrReplace( RVREF(hpair_t( hash, RVREF(value) )) );
		}
		

		// if IsUnique == true
		// if Map contains same value, then the old value will remains
		usize AddOrSkip (const Value &value)
		{
			const key_hash_t	hash = _hash(value);
			return _memory.AddOrSkip( RVREF(hpair_t( hash, value )) );
		}

		usize AddOrSkip (Value &&value)
		{
			const key_hash_t	hash = _hash(value);
			return _memory.AddOrSkip( RVREF(hpair_t( hash, RVREF(value) )) );
		}


		void AddArray (ArrayCRef<Value> value)
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
			return _memory.FindFirstIndex( key_pair_t( _hash( key ), key ), OUT idx );
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
			usize	idx = -1;

			if ( not FindIndex( key, OUT idx ) )
				return false;

			result = &(*this)[ idx ];
			return true;
		}


		bool Erase (const Value &key)				{ return _memory.Erase( key_pair_t( _hash( key ), key ) ); }
		void EraseFromIndex (usize index)			{ return _memory.EraseFromIndex( index ); }
		void Free ()								{ _memory.Free(); }
		void Clear ()								{ _memory.Clear(); }
		void Resize (usize uSize)					{ _memory.Resize( uSize, false ); }
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
				typename S = typename AutoDetectCopyStrategy< Pair< typename H::result_t, Value > >::type,
				typename MC = MemoryContainer< Pair< typename H::result_t, Value > >
			 >
	using HashSet = _types_hidden_::BaseHashSet< Value, true, H, S, MC >;
	

	template <	typename Value,
				typename H = Hash< Value >,
				typename S = typename AutoDetectCopyStrategy< Pair< typename H::result_t, Value > >::type,
				typename MC = MemoryContainer< Pair< typename H::result_t, Value > >
			 >
	using MultiHashSet = _types_hidden_::BaseHashSet< Value, false, H, S, MC >;
	

	template <	typename Value,
				usize Size,
				typename H = Hash< Value >
			 >
	using FixedSizeHashSet = _types_hidden_::BaseHashSet< Value, true, H,
								typename AutoDetectCopyStrategy< Pair< typename H::result_t, Value > >::type,
								StaticMemoryContainer< Pair< typename H::result_t, Value >, Size > >;
	
	
	template <	typename Value,
				usize Size,
				typename H = Hash< Value >
			 >
	using FixedSizeMultiHashSet = _types_hidden_::BaseHashSet< Value, false, H,
									typename AutoDetectCopyStrategy< Pair< typename H::result_t, Value > >::type,
									StaticMemoryContainer< Pair< typename H::result_t, Value >, Size > >;

	
	template <typename Value, bool IsUnique, typename H, typename S, typename MC>
	struct Hash< _types_hidden_::BaseHashSet< Value, IsUnique, H, S, MC > > :
		private Hash< ArrayCRef<Pair< typename H::result_t, Value > > >
	{
		typedef _types_hidden_::BaseHashSet< Value, IsUnique, H, S, MC >	key_t;
		typedef Hash< ArrayCRef<Pair< typename H::result_t, Value > > >	base_t;
		typedef typename base_t::result_t									result_t;

		result_t operator () (const key_t &x) const
		{
			return base_t::operator ()( x );
		}
	};

}	// GXTypes
}	// GX_STL
