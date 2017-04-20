// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "MapUtils.h"

namespace GX_STL
{
namespace GXTypes
{
namespace _types_hidden_
{

	//
	// Base Set (Set or MultiSet)
	//

	template <	typename Value,
				bool IsUnique,
				typename S,
				typename MC
			 >
	struct BaseSet : public CompileTime::CopyQualifiers< CompileTime::FastCopyable, MC >
	{
	// types
	public:
		typedef BaseSet< Value, IsUnique, S, MC >	Self;

		typedef Value								key_t;
		typedef Value								value_t;

		typedef Ptr< const Value >					const_iterator;

		typedef ArrayRef< Value >						values_range_t;
		typedef ArrayCRef< Value >				const_values_range_t;


	private:
		template <typename Key>
		struct _ValueSearchCmp
		{
		// variables
			const Key &	_key;

		// methods
			_ValueSearchCmp (const Key *k): _key(*k) {}

			bool operator == (const Value &value) const		{ return GXMath::All( _key == value ); }
			bool operator != (const Value &value) const		{ return not ( *this == value ); }
			bool operator <  (const Value &value) const		{ return GXMath::All( _key < value ); }
			bool operator >  (const Value &value) const		{ return GXMath::All( _key > value ); }
		};

		typedef _types_hidden_::MapUtils< Array< Value, S, MC >, Value, _ValueSearchCmp, IsUnique >	_MapUtils_t;


	// variables
	private:
		_MapUtils_t		_memory;


	// methods
	public:
		BaseSet (GX_DEFCTOR)
		{}

		BaseSet (const Self &other) : _memory( other._memory )
		{}
		
		BaseSet (Self &&other) : _memory( RVREF( other._memory ) )
		{}


		operator		const_values_range_t () const
		{
			return _memory;
		}


		Value const &	operator [] (usize i) const
		{
			return ReferenceCast< const Value >( _memory[i] );
		}
		
		Value const &	operator () (const key_t &key) const
		{
			usize	idx;
			FindIndex( key, idx );
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


		bool			operator == (const_values_range_t right) const
		{
			return _memory == right._memory;
		}

		bool			operator != (const_values_range_t right) const
		{
			return not (*this == right);
		}
		

		Self &		operator =  (Self &&right)		= default;
		Self &		operator =  (const Self &right)	= default;


		Value const &	Front ()		const	{ return (*this)[0]; }
		Value const &	Back ()			const	{ return (*this)[ LastIndex() ]; }

		bool			Empty ()		const	{ return _memory.Empty(); }
		usize			Count ()		const	{ return _memory.Count(); }
		usize			LastIndex ()	const	{ return _memory.LastIndex(); }
		BytesU			Size ()			const	{ return _memory.Size(); }

		
		// if IsUnique == true
		// if Map contains same value, then the old value will be replaced
		usize Add (const Value &value)			{ return _memory.AddOrReplace( value ); }
		usize Add (Value &&value)				{ return _memory.AddOrReplace( RVREF( value ) ); }
		

		// if IsUnique == true
		// if Map contains same value, then the old value will remains
		usize AddOrSkip (const Value &value)	{ return _memory.AddOrSkip( value ); }
		usize AddOrSkip (Value &&value)			{ return _memory.AddOrSkip( RVREF( value ) ); }


		void AddArray (const_values_range_t value)
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
			return _memory.FindFirstIndex( key, OUT idx );
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

		
		bool Find (const key_t &key, OUT const_iterator &result) const
		{
			usize	idx;

			if ( not FindIndex( key, OUT idx ) )
				return false;
			
			result = &(*this)[ idx ];
			return true;
		}


		bool FindAll (const key_t &key, OUT values_range_t &result)
		{
			usize	first;
			usize	last;

			if ( not FindFirstIndex( key, OUT first ) )
				return false;
			
			FindLastIndex( first, OUT last );

			result = values_range_t( ptr() + first, last - first + 1 );
			return true;
		}

		bool FindAll (const key_t &key, OUT const_values_range_t &result) const
		{
			usize	first;
			usize	last;

			if ( not FindFirstIndex( key, OUT first ) )
				return false;
			
			FindLastIndex( first, OUT last );

			result = const_values_range_t( ptr() + first, last - first + 1 );
			return true;
		}


		/*template <typename Key>
		bool LinearSearch (const Key &key) const
		{
			const_iterator	iter;
			return LinearSearch( key, iter );
		}

		template <typename Key>
		bool LinearSearch (const Key &key, OUT const_iterator &result) const
		{
			FOR( i, _memory ) {
				if ( key == _memory[i] ) {
					result = &_memory[i];
					return true;
				}
			}
			return false;
		}*/


		values_range_t  GetRange (usize first, usize last)
		{
			return _memory.SubArray( first, last - first + 1 );
		}

		const_values_range_t  GetRange (usize first, usize last) const
		{
			return _memory.SubArray( first, last - first + 1 );
		}


		bool Erase (const key_t &key)		{ return _memory.Erase( key ); }
		void EraseFromIndex (usize index)	{ _memory.EraseFromIndex( index ); }
		void Free ()						{ _memory.Free(); }
		void Clear ()						{ _memory.Clear(); }
		void Resize (usize uSize)			{ _memory.Resize( uSize ); }
		void Reserve (usize uSize)			{ _memory.Reserve( uSize ); }
		
		
		friend void SwapValues (INOUT Self &left, INOUT Self &right)
		{
			SwapValues( left._memory, right._memory );
		}
	};

}	// _types_hidden_


	template <	typename Value,
				typename S = typename AutoDetectCopyStrategy< Value >::type,
				typename MC = MemoryContainer< Value >
			 >
	using Set = _types_hidden_::BaseSet< Value, true, S, MC >;
	

	template <	typename Value,
				typename S = typename AutoDetectCopyStrategy< Value >::type,
				typename MC = MemoryContainer< Value >
			 >
	using MultiSet = _types_hidden_::BaseSet< Value, false, S, MC >;
	

	template <typename Value, usize Size>
	using FixedSizeSet = _types_hidden_::BaseSet< Value, true,
								typename AutoDetectCopyStrategy< Value >::type,
								StaticMemoryContainer< Value, Size > >;
	

	template <typename Value, usize Size>
	using FixedSizeMultiSet = _types_hidden_::BaseSet< Value, false,
									typename AutoDetectCopyStrategy< Value >::type,
									StaticMemoryContainer< Value, Size > >;

	//template <typename EnumType>
	//using EnumSet = FixedSizeSet< typename EnumType::type, EnumType::_Count >;

	
	template <typename Value, bool IsUnique, typename S, typename MC>
	struct Hash< _types_hidden_::BaseSet< Value, IsUnique, S, MC > > :
		private Hash< ArrayCRef<Value> >
	{
		typedef _types_hidden_::BaseSet< Value, IsUnique, S, MC >	key_t;
		typedef Hash< ArrayCRef<Value> >							base_t;
		typedef typename base_t::result_t							result_t;

		result_t operator () (const key_t &x) const
		{
			return base_t::operator ()( x );
		}
	};

}	// GXTypes
}	// GX_STL
