// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Containers/Array.h"
#include "Core/STL/Containers/Pair.h"
#include "Core/STL/Types/Ptr.h"

namespace GX_STL
{
namespace GXTypes
{
namespace _types_hidden_
{

	//
	// Map Utils
	//

	template <	typename Container,
				typename KeyType,
				template <typename T> class Search,
				bool IsUnique
			 >
	struct MapUtils final : public CompileTime::CopyQualifiers< CompileTime::FastCopyable, Container >
	{
		STATIC_ASSERT( Container::IsLinearMemory() );

	// types
	public:
		using Self		= MapUtils< Container, KeyType, Search, IsUnique >;
		using Value_t	= typename Container::Value_t;
		using Key_t		= KeyType;
		

	private:
		struct _ValueRVRef
		{
			typedef Value_t			type;
			static Value_t &&		To (Value_t *val)			{ return RVREF( *val ); }
		};
		
		struct _ValueConstRef
		{
			typedef Value_t const	type;
			static Value_t const&	To (Value_t const *val)		{ return *val; }
		};


	// variables
	private:
		Container	_memory;


	// methods
	public:
		MapUtils ()
		{}

		MapUtils (const Self &other) : _memory( other._memory )
		{}

		MapUtils (Self &&other) : _memory( RVREF( other._memory ) )
		{}
		

		Self &			operator =  (Self &&right)		{ _memory = RVREF( right._memory );	return *this; }
		Self &			operator =  (const Self &right)	{ _memory = right._memory;			return *this; }

		bool			Empty ()				const	{ return _memory.Empty(); }
		usize			Count ()				const	{ return _memory.Count(); }
		usize			LastIndex ()			const	{ return _memory.LastIndex(); }
		BytesU			Size ()					const	{ return _memory.Size(); }
		
		Value_t const &	operator [] (usize i)	const	{ return _memory[i]; }
		Value_t &		operator [] (usize i)			{ return _memory[i]; }

		operator ArrayCRef<Value_t> () const			{ return _memory; }
		operator ArrayRef<Value_t> ()					{ return _memory; }

		usize AddOrReplace (const Value_t &value);
		usize AddOrReplace (Value_t &&value);
		
		usize AddOrSkip (const Value_t &value);
		usize AddOrSkip (Value_t &&value);

		bool FindFirstIndex (const Key_t &key, OUT usize &idx) const;
		void FindLastIndex (usize first, OUT usize &idx) const;
		
		template <typename TKey2>
		bool FindFirstIndex2 (const TKey2 &key, OUT usize &idx) const;

		template <typename TKey2>
		void FindLastIndex2 (const TKey2 &key, usize firstIdx, OUT usize &idx) const;

		bool Erase (const Key_t &key);
		void EraseByIndex (usize index);
		
		usize GetIndex (const Value_t* iter) const;

		void Free ()									{ _memory.Free(); }
		void Clear ()									{ _memory.Clear(); }

		void Resize (usize size)						{ _memory.Resize( size, false ); }
		void Reserve (usize size)						{ _memory.Reserve( size ); }

		
		static constexpr bool	IsLinearMemory ()			{ return Container::IsLinearMemory(); }
		constexpr bool			IsStaticMemory ()	const	{ return _memory.IsStaticMemory(); }


		friend void SwapValues (INOUT Self &left, INOUT Self &right)
		{
			SwapValues( left._memory, right._memory );
		}


	private:
		template <typename SearchCmp>
		void _GetIndexBounds (const SearchCmp &cmp, OUT usize &left, OUT usize &right) const;
		
		template <typename SearchCmp, typename ValueRef>
		usize _Add (typename ValueRef::type *value);
		
		template <typename SearchCmp, typename ValueRef, bool KeepSource>
		usize _AddOrChange (typename ValueRef::type *value);

		template <typename SearchCmp, typename Key>
		bool _FindIndex (const Key &key, usize &idx) const;
	};

	

/*
=================================================
	AddOrReplace
=================================================
*/
	template <typename C, typename K, template <typename T> class Search, bool IsUnique>
	inline usize MapUtils<C,K,Search,IsUnique>::AddOrReplace (const Value_t &value)
	{
		typedef Search< Key_t >	cmp_t;
		return IsUnique ?	_AddOrChange< cmp_t, _ValueConstRef, /*KeepSource*/false >( &value ) :
							_Add< cmp_t, _ValueConstRef >( &value );
	}
	
/*
=================================================
	AddOrReplace
=================================================
*/
	template <typename C, typename K, template <typename T> class Search, bool IsUnique>
	inline usize MapUtils<C,K,Search,IsUnique>::AddOrReplace (Value_t &&value)
	{
		typedef Search< Key_t >	cmp_t;
		return IsUnique ?	_AddOrChange< cmp_t, _ValueRVRef, /*KeepSource*/false >( &value ) :
							_Add< cmp_t, _ValueRVRef >( &value );
	}

/*
=================================================
	AddOrSkip
=================================================
*/
	template <typename C, typename K, template <typename T> class Search, bool IsUnique>
	inline usize MapUtils<C,K,Search,IsUnique>::AddOrSkip (const Value_t &value)
	{
		typedef Search< Key_t >	cmp_t;
		return IsUnique ?	_AddOrChange< cmp_t, _ValueConstRef, /*KeepSource*/true >( &value ) :
							_Add< cmp_t, _ValueConstRef >( &value );
	}
	
/*
=================================================
	AddOrSkip
=================================================
*/
	template <typename C, typename K, template <typename T> class Search, bool IsUnique>
	inline usize MapUtils<C,K,Search,IsUnique>::AddOrSkip (Value_t &&value)
	{
		typedef Search< Key_t >	cmp_t;
		return IsUnique ?	_AddOrChange< cmp_t, _ValueRVRef, /*KeepSource*/true >( &value ) :
							_Add< cmp_t, _ValueRVRef >( &value );
	}
	
/*
=================================================
	FindFirstIndex
=================================================
*/
	template <typename C, typename K, template <typename T> class Search, bool U>
	inline bool MapUtils<C,K,Search,U>::FindFirstIndex (const Key_t &key, OUT usize &idx) const
	{
		typedef Search< Key_t >		cmp_t;
		return _FindIndex< cmp_t >( key, OUT idx );
	}
	
/*
=================================================
	FindFirstIndex2
=================================================
*/
	template <typename C, typename K, template <typename T> class Search, bool U>
	template <typename TKey2>
	inline bool MapUtils<C,K,Search,U>::FindFirstIndex2 (const TKey2 &key, OUT usize &idx) const
	{
		typedef Search< TKey2 >		cmp_t;
		return _FindIndex< cmp_t >( key, OUT idx );
	}
	
/*
=================================================
	FindLastIndex2
=================================================
*/
	template <typename C, typename K, template <typename T> class Search, bool U>
	template <typename TKey2>
	inline void MapUtils<C,K,Search,U>::FindLastIndex2 (const TKey2 &key, const usize firstIdx, OUT usize &idx) const
	{
		ASSERT( key == _memory[firstIdx].first );

		idx = firstIdx;

		for (usize i = firstIdx+1; i < this->LastIndex() and key == _memory[i].first; ++i) {
			idx = i;
		}
	}

/*
=================================================
	FindLastIndex
=================================================
*/
	template <typename C, typename K, template <typename T> class S, bool U>
	inline void MapUtils<C,K,S,U>::FindLastIndex (const usize first, OUT usize &idx) const
	{
		Key_t const &	key = _memory[first].first;

		idx = first;

		for (usize i = first+1; i < this->LastIndex() and key == _memory[i].first; ++i) {
			idx = i;
		}
	}
	
/*
=================================================
	Erase
=================================================
*/
	template <typename C, typename K, template <typename T> class S, bool U>
	inline bool MapUtils<C,K,S,U>::Erase (const Key_t &key)
	{
		usize	idx = UMax;

		if ( not FindFirstIndex( key, OUT idx ) )
			return false;

		_memory.Erase( idx );
		return true;
	}
	
/*
=================================================
	EraseByIndex
=================================================
*/
	template <typename C, typename K, template <typename T> class S, bool U>
	inline void MapUtils<C,K,S,U>::EraseByIndex (const usize index)
	{
		_memory.Erase( index );
	}
		
/*
=================================================
	GetIndex
=================================================
*/
	template <typename C, typename K, template <typename T> class S, bool U>
	inline usize MapUtils<C,K,S,U>::GetIndex (const Value_t* iter) const
	{
		return _memory.GetIndex( *iter );
	}
/*
=================================================
	_GetIndexBounds
=================================================
*/
	template <typename C, typename K, template <typename T> class S, bool U>
	template <typename SearchCmp>
	inline void MapUtils<C,K,S,U>::_GetIndexBounds (const SearchCmp &cmp, OUT usize &left, OUT usize &right) const
	{
		ASSERT( not _memory.Empty() );
	
		// binary search
		left  = 0;
		right = _memory.Count() - 1;
	
		while ( right - left > 1 )
		{
			usize	mid = (right + left) >> 1;

			if ( cmp > _memory[mid] )
				left = mid;
			else
				right = mid;
		}
	}
	
/*
=================================================
	_Add
=================================================
*/
	template <typename C, typename K, template <typename T> class S, bool U>
	template <typename SearchCmp, typename ValueRef>
	inline usize MapUtils<C,K,S,U>::_Add (typename ValueRef::type *value)
	{
		if ( _memory.Empty() )
		{
			_memory.PushBack( ValueRef::To( value ) );
			return _memory.LastIndex();
		}

		usize		left,
					right;
		SearchCmp	cmp( value );

		_GetIndexBounds( cmp, left, right );

		for (usize i = left; i < right; ++i)
		{
			if ( not (cmp < _memory[i]) and cmp < _memory[i+1] ) {
				_memory.Insert( ValueRef::To( value ), i+1 );
				return i+1;
			}
		}

		if ( cmp == _memory[right] )
		{
			for (; right < _memory.Count() and cmp == _memory[right]; ++right) {}

			_memory.Insert( ValueRef::To( value ), right );
			return right;
		}

		if ( cmp > _memory[right] )
		{
			_memory.Insert( ValueRef::To( value ), right+1 );
			return right+1;
		}
			
		if ( cmp < _memory[left] )
		{
			_memory.Insert( ValueRef::To( value ), left );
			return left;
		}

		WARNING( "" );
		return UMax;
	}
		
/*
=================================================
	_AddOrChange
=================================================
*/
	template <typename C, typename K, template <typename T> class S, bool U>
	template <typename SearchCmp, typename ValueRef, bool KeepSource>
	inline usize MapUtils<C,K,S,U>::_AddOrChange (typename ValueRef::type *value)
	{
		if ( _memory.Empty() )
		{
			_memory.PushBack( ValueRef::To( value ) );
			return _memory.LastIndex();
		}
			
		usize		left,
					right;
		SearchCmp	cmp( value );

		_GetIndexBounds( cmp, left, right );
			
		for (usize i = left; i < right; ++i)
		{
			if ( cmp == _memory[i] )
			{
				if (KeepSource)
					return i;

				_memory[i] = ValueRef::To( value );
				return i;
			}

			if ( cmp > _memory[i] and cmp < _memory[i+1] ) {
				_memory.Insert( ValueRef::To( value ), i+1 );
				return i+1;
			}
		}

		if ( cmp == _memory[right] )
		{
			if (KeepSource)
				return right;

			_memory[right] = ValueRef::To( value );
			return right;
		}

		if ( cmp > _memory[right] )
		{
			_memory.Insert( ValueRef::To( value ), right+1 );
			return right+1;
		}
			
		if ( cmp < _memory[left] )
		{
			_memory.Insert( ValueRef::To( value ), left );
			return left;
		}
			
		WARNING( "" );
		return UMax;
	}
	
/*
=================================================
	_FindIndex
=================================================
*/
	template <typename C, typename K, template <typename T> class S, bool U>
	template <typename SearchCmp, typename Key>
	inline bool MapUtils<C,K,S,U>::_FindIndex (const Key &key, OUT usize &idx) const
	{
		idx = UMax;

		if ( _memory.Empty() )
			return false;

		// binary search
		usize			left  = 0,
						right = _memory.LastIndex();
		const SearchCmp	cmp( &key );
			
		while ( right - left > 1 )
		{
			usize	mid = (left + right) >> 1;

			if ( cmp > _memory[mid] )
				left = mid;
			else
				right = mid;
		}

		for (usize i = 0; i < 2 and left+i <= right; ++i)
		{
			if ( cmp == _memory[left+i] )
			{
				idx = left+i;
				return true;
			}
		}

		return false;
	}


}	// _types_hidden_
}	// GXTypes
}	// GX_STL
