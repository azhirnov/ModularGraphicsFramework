// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Containers/MapUtils.h"
#include "Engine/STL/Containers/IndexedArray.h"
#include "Engine/STL/Containers/IndexedIterator.h"

namespace GX_STL
{
namespace GXTypes
{
namespace _types_hidden_
{

	//
	// Base Map (Map or MultiMap)
	//

	template <	template <typename T1, typename S1, typename MC1> class Container,
				typename K,
				typename T,
				bool IsUnique,
				typename S,
				typename MC
			 >
	struct BaseMap : public CompileTime::CopyQualifiers< CompileTime::FastCopyable, Container<Pair<K,T>, S, MC> >
	{
	// types
	public:
		using Self					= BaseMap< Container, K, T, IsUnique, S, MC >;

		using Key_t					= K;
		using Value_t				= T;

		using Pair_t				= Pair< Key_t, Value_t >;
		using CPair_t				= Pair< const Key_t, Value_t >;
		
		using iterator				= Ptr< CPair_t >;
		using const_iterator		= Ptr< const CPair_t >;
		
		using values_range_t		= ArrayRef< CPair_t >;
		using const_values_range_t	= ArrayCRef< CPair_t >;
		
		using idx_iterator			= IndexedIterator< CPair_t >;
		using idx_const_iterator	= IndexedIterator< const CPair_t >;

	private:
		template <typename Key>
		struct _KeySearchCmp
		{
		// variables
			const Key &	_key;
			
		// methods
			_KeySearchCmp (const Key *k): _key(*k) {}
			_KeySearchCmp (const Pair_t *p): _key(p->first) {}
			
			bool operator == (const Pair_t &pair) const		{ return _key == pair.first; }
			bool operator != (const Pair_t &pair) const		{ return not ( *this == pair.first ); }
			bool operator <  (const Pair_t &pair) const		{ return pair.first > _key; }
			bool operator >  (const Pair_t &pair) const		{ return _key > pair.first; }
		};

		using _MapUtils_t	= _types_hidden_::MapUtils< Container< Pair_t, S, MC >, Key_t, _KeySearchCmp, IsUnique >;


	// variables
	private:
		_MapUtils_t		_memory;


	// methods
	public:
		BaseMap (GX_DEFCTOR)
		{}

		BaseMap (const Self &other) : _memory( other._memory )
		{}
		
		BaseMap (Self &&other) : _memory( RVREF( other._memory ) )
		{}
		
		BaseMap (InitializerList<Pair_t> list)
		{
			AddArray( ArrayCRef<Pair_t>( list ) );
		}
		
		BaseMap (ArrayCRef<Pair_t> list)
		{
			AddArray( list );
		}

		ND_ operator ArrayCRef<CPair_t> () const
		{
			return ArrayCRef<CPair_t>::From( ArrayCRef<Pair_t>( _memory ) );
		}
		
		ND_ operator UniBuffer<CPair_t> ()
		{
			return UniBuffer<CPair_t>{ ArrayRef<CPair_t>::From( ArrayRef<Pair_t>{_memory} ) };
		}
		
		ND_ operator UniBuffer<const CPair_t> () const
		{
			return UniBuffer<const CPair_t>{ ArrayCRef<CPair_t>::From( ArrayCRef<Pair_t>{_memory} ) };
		}

		ND_ CPair_t &		operator [] (usize i)
		{
			return ReferenceCast< CPair_t >( _memory[i] );
		}

		ND_ CPair_t const&	operator [] (usize i) const
		{
			return ReferenceCast< const CPair_t >( _memory[i] );
		}
		

		ND_ Value_t &		operator () (const Key_t &key)
		{
			usize	i = 0;
			FindIndex( key, OUT i );
			return (*this)[i].second;
		}
		
		ND_ Value_t const&	operator () (const Key_t &key) const
		{
			usize	i = 0;
			FindIndex( key, OUT i );
			return (*this)[i].second;
		}


		Self &		operator << (const Pair_t &value)
		{
			Add( value );
			return *this;
		}

		Self &		operator << (Pair_t &&value)
		{
			Add( RVREF( value ) );
			return *this;
		}


		ND_ bool	operator == (const_values_range_t right) const
		{
			return _memory == right._memory;
		}

		ND_ bool	operator != (const_values_range_t right) const
		{
			return not (*this == right);
		}
		

		Self &		operator =  (Self &&right)		= default;
		Self &		operator =  (const Self &right)	= default;


		ND_ CPair_t &		Front ()				{ return (*this)[0]; }
		ND_ CPair_t const&	Front ()		const	{ return (*this)[0]; }
		
		ND_ CPair_t &		Back ()					{ return (*this)[ LastIndex() ]; }
		ND_ CPair_t const&	Back ()			const	{ return (*this)[ LastIndex() ]; }
		

		ND_ bool			Empty ()		const	{ return _memory.Empty(); }
		ND_ usize			Count ()		const	{ return _memory.Count(); }
		ND_ usize			LastIndex ()	const	{ return _memory.LastIndex(); }
		ND_ BytesU			Size ()			const	{ return _memory.Size(); }
		
		
		ND_ auto			begin ()				{ return idx_iterator{ *this, 0 }; }
		ND_ auto			begin ()		const	{ return idx_const_iterator{ *this, 0 }; }
		ND_ auto			end ()					{ return idx_iterator{ *this, Count() }; }
		ND_ auto			end ()			const	{ return idx_const_iterator{ *this, Count() }; }
		

		// if IsUnique == true
		// if Map contains same value, then the old value will be replaced
		iterator Add (const Key_t &key, const Value_t &value)
		{
			const usize	idx = _memory.AddOrReplace( RVREF( Pair_t( key, value ) ) );
			return &(*this)[ idx ];
		}
		
		iterator Add (Key_t &&key, Value_t &&value)
		{
			const usize	idx = _memory.AddOrReplace( RVREF( Pair_t( RVREF(key), RVREF(value) ) ) );
			return &(*this)[ idx ];
		}

		iterator Add (const Pair_t &value)
		{
			const usize	idx = _memory.AddOrReplace( value );
			return &(*this)[ idx ];
		}

		iterator Add (Pair_t &&value)
		{
			const usize	idx = _memory.AddOrReplace( RVREF( value ) );
			return &(*this)[ idx ];
		}
		

		// if IsUnique == true
		// if Map contains same value, then the old value will remains
		iterator AddOrSkip (const Key_t &key, const Value_t &value)
		{
			const usize	idx = _memory.AddOrSkip( RVREF( Pair_t( key, value ) ) );
			return &(*this)[ idx ];
		}
		
		iterator AddOrSkip (Key_t &&key, Value_t &&value)
		{
			const usize	idx = _memory.AddOrSkip( RVREF( Pair_t( RVREF(key), RVREF(value) ) ) );
			return &(*this)[ idx ];
		}

		iterator AddOrSkip (const Pair_t &value)
		{
			const usize	idx = _memory.AddOrSkip( value );
			return &(*this)[ idx ];
		}

		iterator AddOrSkip (Pair_t &&value)
		{
			const usize	idx = _memory.AddOrSkip( RVREF( value ) );
			return &(*this)[ idx ];
		}


		void AddArray (ArrayCRef<Pair_t> value)
		{
			FOR( i, value ) {
				Add( value[i] );
			}
		}

		void AddArray (const_values_range_t value)
		{
			FOR( i, value ) {
				Add( value[i] );
			}
		}

		void AddArray (const Self &value)
		{
			AddArray( const_values_range_t(value) );
		}

		void AddArray (Self &&value)
		{
			FOR( i, value ) {
				Add(RVREF( value[i] ));
			}
		}


		bool FindIndex (const Key_t &key, OUT usize &idx) const
		{
			return FindFirstIndex( key, OUT idx );
		}

		bool FindFirstIndex (const Key_t &key, OUT usize &idx) const
		{
			return _memory.FindFirstIndex( key, OUT idx );
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

		
		bool Find (const Key_t &key, OUT iterator &result)
		{
			usize	idx = UMax;

			if ( not FindIndex( key, OUT idx ) )
				return false;
			
			result = &(*this)[ idx ];
			return true;
		}

		bool Find (const Key_t &key, OUT const_iterator &result) const
		{
			usize	idx = UMax;

			if ( not FindIndex( key, OUT idx ) )
				return false;
			
			result = &(*this)[ idx ];
			return true;
		}


		bool FindAll (const Key_t &key, OUT values_range_t &result)
		{
			usize	first;
			usize	last;

			if ( not FindFirstIndex( key, OUT first ) )
				return false;
			
			FindLastIndex( first, OUT last );

			result = values_range_t( &(*this)[first], last - first + 1 );
			return true;
		}

		bool FindAll (const Key_t &key, OUT const_values_range_t &result) const
		{
			usize	first;
			usize	last;

			if ( not FindFirstIndex( key, OUT first ) )
				return false;
			
			FindLastIndex( first, OUT last );

			result = const_values_range_t( &(*this)[first], last - first + 1 );
			return true;
		}


		ND_ values_range_t  GetRange (const usize first, const usize last)
		{
			return const_values_range_t(*this).SubArray( first, last - first + 1 );
		}

		ND_ const_values_range_t  GetRange (const usize first, const usize last) const
		{
			return const_values_range_t(*this).SubArray( first, last - first + 1 );
		}


		void EraseByIter (const_iterator iter)		{ EraseByIndex( _memory.GetIndex( Cast<Pair_t const*>(iter.RawPtr()) ) ); }
		void EraseByIter (iterator iter)			{ EraseByIndex( _memory.GetIndex( Cast<Pair_t const*>(iter.RawPtr()) ) ); }
		void EraseByIndex (usize index)				{ _memory.EraseByIndex( index ); }
		bool Erase (const Key_t &key)				{ return _memory.Erase( key ); }

		void Free ()								{ _memory.Free(); }
		void Clear ()								{ _memory.Clear(); }
		void Resize (usize size)					{ _memory.Resize( size ); }
		void Reserve (usize size)					{ _memory.Reserve( size ); }
		

		static constexpr bool	IsLinearMemory ()			{ return _MapUtils_t::IsLinearMemory(); }
		constexpr bool			IsStaticMemory ()	const	{ return _memory.IsStaticMemory(); }


		friend void SwapValues (INOUT Self &left, INOUT Self &right)
		{
			SwapValues( left._memory, right._memory );
		}


	private:
		template <typename KeyT>
		bool _FindFirstIndex2 (const KeyT &key, OUT usize &idx) const
		{
			return _memory.FindFirstIndex2( key, OUT idx );
		}
		
		template <typename KeyT>
		void _FindLastIndex2 (const KeyT &key, usize first, OUT usize &idx) const
		{
			return _memory.FindLastIndex2( key, first, OUT idx );
		}
		
		template <typename KeyT>
		bool _Find2 (const KeyT &key, OUT iterator &result)
		{
			usize	idx = UMax;

			if ( not _FindFirstIndex2( key, OUT idx ) )
				return false;
			
			result = &(*this)[ idx ];
			return true;
		}
		
		template <typename KeyT>
		bool _Find2 (const KeyT &key, OUT const_iterator &result) const
		{
			usize	idx = UMax;

			if ( not _FindFirstIndex2( key, OUT idx ) )
				return false;
			
			result = &(*this)[ idx ];
			return true;
		}
		
		template <typename KeyT>
		bool _FindAll2 (const KeyT &key, OUT values_range_t &result)
		{
			usize	first;
			usize	last;

			if ( not _FindFirstIndex2( key, first ) )
				return false;
			
			_FindLastIndex2( key, first, last );

			result = values_range_t( &(*this)[first], last - first + 1 );
			return true;
		}


		struct _CustomSearch
		{
			Self &	_ref;

			_CustomSearch (Self &ref) : _ref(ref) {}
			
			template <typename KeyT>
			bool FindFirstIndex (const KeyT &key, OUT usize &idx) const		{ return _ref._FindFirstIndex2( key, OUT idx ); }
			
			template <typename KeyT>
			void FindLastIndex (const KeyT &key, usize first, OUT usize &idx) const	{ return _ref._FindLastIndex2( key, first, OUT idx ); }

			template <typename KeyT>
			bool Find (const KeyT &key, OUT iterator &result) const			{ return _ref._Find2( key, OUT result ); }

			template <typename KeyT>
			bool Find (const KeyT &key, OUT const_iterator &result) const	{ return _ref._Find2( key, OUT result ); }

			template <typename KeyT>
			bool FindAll (const KeyT &key, OUT values_range_t &result)		{ return _ref._FindAll2( key, OUT result ); }

			template <typename KeyT>
			bool IsExist (const KeyT &key) const							{ usize idx;  return FindFirstIndex( key, OUT idx ); }
		};


	public:
		_CustomSearch		CustomSearch ()			{ return _CustomSearch( *this ); }
		_CustomSearch const	CustomSearch ()	const	{ return _CustomSearch( *this ); }
	};

	
}	// _types_hidden_


	// dynamic size
	template <	typename K,
				typename T,
				typename S = typename AutoDetectCopyStrategy< Pair<K, T> >::type,
				typename MC = MemoryContainer< Pair<K, T> >
			 >
	using Map = _types_hidden_::BaseMap< Array, K, T, true, S, MC >;
	

	template <	typename K,
				typename T,
				typename S = typename AutoDetectCopyStrategy< Pair<K, T> >::type,
				typename MC = MemoryContainer< Pair<K, T> >
			 >
	using IndexedMap = _types_hidden_::BaseMap< IndexedArray, K, T, true, S, MC >;


	template <	typename K,
				typename T,
				typename S = typename AutoDetectCopyStrategy< Pair<K, T> >::type,
				typename MC = MemoryContainer< Pair<K, T> >
			 >
	using MultiMap = _types_hidden_::BaseMap< Array, K, T, false, S, MC >;
	

	// static size
	template <typename K, typename T, usize Size>
	using FixedSizeMap = _types_hidden_::BaseMap< Array, K, T, true,
								typename AutoDetectCopyStrategy< Pair<K,T> >::type,
								StaticMemoryContainer< Pair<K,T>, Size > >;
	

	template <typename K, typename T, usize Size>
	using FixedSizeMultiMap = _types_hidden_::BaseMap< Array, K, T, false,
									typename AutoDetectCopyStrategy< Pair<K, T> >::type,
									StaticMemoryContainer< Pair<K, T>, Size > >;
	

	// static + dynamic size
	template <typename K, typename T, usize Size>
	using MixedSizeMap = _types_hidden_::BaseMap< Array, K, T, true,
								typename AutoDetectCopyStrategy< Pair<K,T> >::type,
								StaticMemoryContainer< Pair<K,T>, Size > >;
	

	template <typename K, typename T, usize Size>
	using MixedSizeMultiMap = _types_hidden_::BaseMap< Array, K, T, false,
									typename AutoDetectCopyStrategy< Pair<K, T> >::type,
									StaticMemoryContainer< Pair<K, T>, Size > >;

	
	template <	template <typename, typename, typename> class Container,
				typename K,
				typename T,
				bool IsUnique,
				typename S,
				typename MC
			 >
	struct Hash< _types_hidden_::BaseMap< Container, K, T, IsUnique, S, MC > >
	{
		HashResult  operator () (const _types_hidden_::BaseMap< Container, K, T, IsUnique, S, MC > &x) const noexcept
		{
			return HashOf( ArrayCRef< Pair<const K, T> >( x ) );
		}
	};


}	// GXTypes
}	// GX_STL
