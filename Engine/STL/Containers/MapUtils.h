// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Array.h"
#include "Pair.h"
#include "Engine/STL/Types/Ptr.h"

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
		typedef MapUtils< Container, KeyType, Search, IsUnique >	Self;

		typedef typename Container::value_t							value_t;
		typedef KeyType												key_t;
		

	private:
		struct _ValueRVRef
		{
			typedef value_t			type;
			static value_t &&		To (value_t *val)			{ return RVREF( *val ); }
		};
		
		struct _ValueConstRef
		{
			typedef value_t const	type;
			static value_t const&	To (value_t const *val)		{ return *val; }
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
		
		value_t const &	operator [] (usize i)	const	{ return _memory[i]; }
		value_t &		operator [] (usize i)			{ return _memory[i]; }

		operator ArrayCRef<value_t> () const			{ return _memory; }
		operator ArrayRef<value_t> ()					{ return _memory; }

		usize AddOrReplace (const value_t &value);
		usize AddOrReplace (value_t &&value);
		
		usize AddOrSkip (const value_t &value);
		usize AddOrSkip (value_t &&value);

		bool FindFirstIndex (const key_t &key, OUT usize &idx) const;
		void FindLastIndex (usize first, OUT usize &idx) const;

		bool Erase (const key_t &key);
		void EraseFromIndex (usize index);
		
		usize GetIndex (Ptr<const value_t> iter) const;
		//usize GetIndex (Ptr<value_t> iter) const;

		void Free ()									{ _memory.Free(); }
		void Clear ()									{ _memory.Clear(); }

		void Resize (usize uSize)						{ _memory.Resize( uSize, false ); }
		void Reserve (usize uSize)						{ _memory.Reserve( uSize ); }


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
	inline usize MapUtils<C,K,Search,IsUnique>::AddOrReplace (const value_t &value)
	{
		typedef Search< key_t >	cmp_t;
		return IsUnique ?	_AddOrChange< cmp_t, _ValueConstRef, /*KeepSource*/false >( &value ) :
							_Add< cmp_t, _ValueConstRef >( &value );
	}
	
/*
=================================================
	AddOrReplace
=================================================
*/
	template <typename C, typename K, template <typename T> class Search, bool IsUnique>
	inline usize MapUtils<C,K,Search,IsUnique>::AddOrReplace (value_t &&value)
	{
		typedef Search< key_t >	cmp_t;
		return IsUnique ?	_AddOrChange< cmp_t, _ValueRVRef, /*KeepSource*/false >( &value ) :
							_Add< cmp_t, _ValueRVRef >( &value );
	}

/*
=================================================
	AddOrSkip
=================================================
*/
	template <typename C, typename K, template <typename T> class Search, bool IsUnique>
	inline usize MapUtils<C,K,Search,IsUnique>::AddOrSkip (const value_t &value)
	{
		typedef Search< key_t >	cmp_t;
		return IsUnique ?	_AddOrChange< cmp_t, _ValueConstRef, /*KeepSource*/true >( &value ) :
							_Add< cmp_t, _ValueConstRef >( &value );
	}
	
/*
=================================================
	AddOrSkip
=================================================
*/
	template <typename C, typename K, template <typename T> class Search, bool IsUnique>
	inline usize MapUtils<C,K,Search,IsUnique>::AddOrSkip (value_t &&value)
	{
		typedef Search< key_t >	cmp_t;
		return IsUnique ?	_AddOrChange< cmp_t, _ValueRVRef, /*KeepSource*/true >( &value ) :
							_Add< cmp_t, _ValueRVRef >( &value );
	}
	
/*
=================================================
	FindFirstIndex
=================================================
*/
	template <typename C, typename K, template <typename T> class Search, bool U>
	inline bool MapUtils<C,K,Search,U>::FindFirstIndex (const key_t &key, OUT usize &idx) const
	{
		typedef Search< key_t >		cmp_t;
		return _FindIndex< cmp_t >( key, OUT idx );
	}
	
/*
=================================================
	FindLastIndex
=================================================
*/
	template <typename C, typename K, template <typename T> class S, bool U>
	inline void MapUtils<C,K,S,U>::FindLastIndex (usize first, OUT usize &idx) const
	{
		key_t const &	key = _memory[first].first;

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
	inline bool MapUtils<C,K,S,U>::Erase (const key_t &key)
	{
		usize	idx = -1;

		if ( not FindFirstIndex( key, OUT idx ) )
			return false;

		_memory.Erase( idx );
		return true;
	}
	
/*
=================================================
	EraseFromIndex
=================================================
*/
	template <typename C, typename K, template <typename T> class S, bool U>
	inline void MapUtils<C,K,S,U>::EraseFromIndex (usize index)
	{
		_memory.Erase( index );
	}
		
/*
=================================================
	GetIndex
=================================================
*/
	template <typename C, typename K, template <typename T> class S, bool U>
	inline usize MapUtils<C,K,S,U>::GetIndex (Ptr<const value_t> iter) const
	{
		return _memory.GetIndex( *(const value_t *) iter.ptr() );
	}
	/*
	template <typename C, typename K, template <typename T> class S, bool U>
	inline usize MapUtils<C,K,S,U>::GetIndex (Ptr<value_t> iter) const
	{
		return _memory.GetIndex( *(const value_t *) iter.ptr() );
	}
	*/
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
		return usize(-1);
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
		return usize(-1);
	}
	
/*
=================================================
	_FindIndex
=================================================
*/
	template <typename C, typename K, template <typename T> class S, bool U>
	template <typename SearchCmp, typename Key>
	inline bool MapUtils<C,K,S,U>::_FindIndex (const Key &key, usize &idx) const
	{
		idx = usize(-1);

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
