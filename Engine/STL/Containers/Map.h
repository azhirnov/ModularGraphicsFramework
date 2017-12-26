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

		using pair_t				= Pair< Key_t, Value_t >;
		using const_pair_t			= Pair< const Key_t, Value_t >;
		
		using iterator				= Ptr< const_pair_t >;
		using const_iterator		= Ptr< const const_pair_t >;
		
		using values_range_t		= ArrayRef< const_pair_t >;
		using const_values_range_t	= ArrayCRef< const_pair_t >;

	private:
		template <typename Key>
		struct _KeySearchCmp
		{
		// variables
			const Key &	_key;
			
		// methods
			_KeySearchCmp (const Key *k): _key(*k) {}
			_KeySearchCmp (const pair_t *p): _key(p->first) {}
			
			bool operator == (const pair_t &pair) const		{ return GXMath::All( _key == pair.first ); }
			bool operator != (const pair_t &pair) const		{ return not ( *this == pair.first ); }
			bool operator <  (const pair_t &pair) const		{ return GXMath::All( _key < pair.first ); }
			bool operator >  (const pair_t &pair) const		{ return GXMath::All( _key > pair.first ); }
		};

		using _MapUtils_t	= _types_hidden_::MapUtils< Container< pair_t, S, MC >, Key_t, _KeySearchCmp, IsUnique >;


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
		
		BaseMap (InitializerList<pair_t> list)
		{
			AddArray( ArrayCRef<pair_t>( list ) );
		}
		
		BaseMap (ArrayCRef<pair_t> list)
		{
			AddArray( list );
		}

		operator	ArrayCRef< pair_t > () const
		{
			return ArrayCRef< typename _MapUtils_t::Value_t >( _memory );
			//return const_values_range_t( _memory.ptr(), _memory.ptr() + _memory.Count() );
		}


		const_pair_t &	operator [] (usize i)
		{
			return ReferenceCast< const_pair_t >( _memory[i] );
		}

		const_pair_t const&	operator [] (usize i) const
		{
			return ReferenceCast< const const_pair_t >( _memory[i] );
		}
		

		Value_t &	operator () (const Key_t &key)
		{
			usize	i = 0;
			FindIndex( key, OUT i );
			return (*this)[i].second;
		}
		
		Value_t const&	operator () (const Key_t &key) const
		{
			usize	i = 0;
			FindIndex( key, OUT i );
			return (*this)[i].second;
		}


		Self &		operator << (const pair_t &value)
		{
			Add( value );
			return *this;
		}

		Self &		operator << (pair_t &&value)
		{
			Add( RVREF( value ) );
			return *this;
		}


		bool		operator == (const_values_range_t right) const
		{
			return _memory == right._memory;
		}

		bool		operator != (const_values_range_t right) const
		{
			return not (*this == right);
		}
		

		Self &		operator =  (Self &&right)		= default;
		Self &		operator =  (const Self &right)	= default;


		const_pair_t &		Front ()			{ return (*this)[0]; }
		const_pair_t const&	Front ()	const	{ return (*this)[0]; }

		const_pair_t &		Back ()				{ return (*this)[ LastIndex() ]; }
		const_pair_t const&	Back ()		const	{ return (*this)[ LastIndex() ]; }


		//const_pair_t *		ptr ()				{ return (const_pair_t *) _memory.ptr(); }
		//const_pair_t const*	ptr ()		const	{ return (const_pair_t const *) _memory.ptr(); }
		

		bool			Empty ()		const	{ return _memory.Empty(); }
		usize			Count ()		const	{ return _memory.Count(); }
		usize			LastIndex ()	const	{ return _memory.LastIndex(); }
		BytesU			Size ()			const	{ return _memory.Size(); }

		
		// if IsUnique == true
		// if Map contains same value, then the old value will be replaced
		usize Add (const Key_t &key, const Value_t &value)
		{
			return _memory.AddOrReplace( RVREF( pair_t( key, value ) ) );
		}
		
		usize Add (Key_t &&key, Value_t &&value)
		{
			return _memory.AddOrReplace( RVREF( pair_t( RVREF(key), RVREF(value) ) ) );
		}

		usize Add (const pair_t &value)
		{
			return _memory.AddOrReplace( value );
		}

		usize Add (pair_t &&value)
		{
			return _memory.AddOrReplace( RVREF( value ) );
		}
		

		// if IsUnique == true
		// if Map contains same value, then the old value will remains
		usize AddOrSkip (const Key_t &key, const Value_t &value)
		{
			return _memory.AddOrSkip( RVREF( pair_t( key, value ) ) );
		}
		
		usize AddOrSkip (Key_t &&key, Value_t &&value)
		{
			return _memory.AddOrSkip( RVREF( pair_t( RVREF(key), RVREF(value) ) ) );
		}

		usize AddOrSkip (const pair_t &value)
		{
			return _memory.AddOrSkip( value );
		}

		usize AddOrSkip (pair_t &&value)
		{
			return _memory.AddOrSkip( RVREF( value ) );
		}


		void AddArray (ArrayCRef<pair_t> value)
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


		/*usize ChangeKey (const Key_t &key, usize idx)
		{
			const const_pair_t	tmp = (*this)[ idx ];

			EraseByIndex( idx );

			return Add( key, tmp.second );
		}

		usize ChangeKey (const Key_t &key, const_iterator iter)
		{
			return ChangeKey( key, GetIndex( iter ) );
		}*/


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


		bool IsExist (const Key_t &key) const
		{
			usize idx = 0;
			return FindIndex( key, idx );
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

			if ( not FindFirstIndex( key, first ) )
				return false;
			
			FindLastIndex( first, last );

			result = values_range_t( &(*this)[first], last - first + 1 );
			return true;
		}

		bool FindAll (const Key_t &key, OUT const_values_range_t &result) const
		{
			usize	first;
			usize	last;

			if ( not FindFirstIndex( key, first ) )
				return false;
			
			FindLastIndex( first, last );

			result = const_values_range_t( &(*this)[first], last - first + 1 );
			return true;
		}


		values_range_t  GetRange (usize first, usize last)
		{
			return const_values_range_t(*this).SubArray( first, last - first + 1 );
		}

		const_values_range_t  GetRange (usize first, usize last) const
		{
			return const_values_range_t(*this).SubArray( first, last - first + 1 );
		}


		void EraseByIter (const_iterator iter)		{ return EraseByIndex( _memory.GetIndex( (pair_t *) iter.ptr() ) ); }
		void EraseByIter (iterator iter)			{ return EraseByIndex( _memory.GetIndex( (pair_t *) iter.ptr() ) ); }

		bool Erase (const Key_t &key)				{ return _memory.Erase( key ); }
		void EraseByIndex (usize index)				{ _memory.EraseByIndex( index ); }
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
			usize	idx = -1;

			if ( not _FindFirstIndex2( key, OUT idx ) )
				return false;
			
			result = &(*this)[ idx ];
			return true;
		}
		
		template <typename KeyT>
		bool _Find2 (const KeyT &key, OUT const_iterator &result) const
		{
			usize	idx = -1;

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
	

	template <typename K, typename T, usize Size>
	using FixedSizeMap = _types_hidden_::BaseMap< Array, K, T, true,
								typename AutoDetectCopyStrategy< Pair<K,T> >::type,
								StaticMemoryContainer< Pair<K,T>, Size > >;
	

	template <typename K, typename T, usize Size>
	using FixedSizeMultiMap = _types_hidden_::BaseMap< Array, K, T, false,
									typename AutoDetectCopyStrategy< Pair<K, T> >::type,
									StaticMemoryContainer< Pair<K, T>, Size > >;

	
	template <	template <typename, typename, typename> class Container,
				typename K,
				typename T,
				bool IsUnique,
				typename S,
				typename MC
			 >
	struct Hash< _types_hidden_::BaseMap< Container, K, T, IsUnique, S, MC > > :
		private Hash< ArrayCRef< Pair<const K, T> > >
	{
		typedef _types_hidden_::BaseMap< Container, K, T, IsUnique, S, MC >		Key_t;
		typedef ArrayCRef< Pair<const K, T> >									Base_t;
		typedef typename Base_t::Result_t										Result_t;

		Result_t operator () (const Key_t &x) const noexcept
		{
			return Base_t::operator ()( x );
		}
	};


}	// GXTypes
}	// GX_STL
