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
	// Base Set (Set or MultiSet)
	//

	template <	template <typename T1, typename S1, typename MC1> class Container,
				typename Value,
				bool IsUnique,
				typename S,
				typename MC
			 >
	struct BaseSet : public CompileTime::CopyQualifiers< CompileTime::FastCopyable, Container< Value, S, MC > >
	{
	// types
	public:
		using Self					= BaseSet< Container, Value, IsUnique, S, MC >;

		using Key_t					= Value;
		using Value_t				= Value;

		using const_iterator		= Ptr< const Value >;

		using values_range_t		= ArrayRef< Value >;
		using const_values_range_t	= ArrayCRef< Value >;


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

		using _MapUtils_t	= _types_hidden_::MapUtils< Container< Value, S, MC >, Value, _ValueSearchCmp, IsUnique >;


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

		BaseSet (InitializerList<Key_t> list)
		{
			AddArray( ArrayCRef<Key_t>( list ) );
		}


		operator		const_values_range_t () const
		{
			return _memory;
		}


		Value const &	operator [] (usize i) const
		{
			return ReferenceCast< const Value >( _memory[i] );
		}
		
		Value const &	operator () (const Key_t &key) const
		{
			usize	idx = 0;
			FindIndex( key, OUT idx );
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
			return FindIndex( key, OUT idx );
		}

		
		bool Find (const Key_t &key, OUT const_iterator &result) const
		{
			usize	idx;

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

			result = values_range_t( ptr() + first, last - first + 1 );
			return true;
		}

		bool FindAll (const Key_t &key, OUT const_values_range_t &result) const
		{
			usize	first;
			usize	last;

			if ( not FindFirstIndex( key, OUT first ) )
				return false;
			
			FindLastIndex( first, OUT last );

			result = const_values_range_t( ptr() + first, last - first + 1 );
			return true;
		}


		values_range_t  GetRange (usize first, usize last)
		{
			return _memory.SubArray( first, last - first + 1 );
		}

		const_values_range_t  GetRange (usize first, usize last) const
		{
			return _memory.SubArray( first, last - first + 1 );
		}


		bool Erase (const Key_t &key)		{ return _memory.Erase( key ); }
		void EraseByIndex (usize index)		{ _memory.EraseByIndex( index ); }
		void Free ()						{ _memory.Free(); }
		void Clear ()						{ _memory.Clear(); }
		void Resize (usize uSize)			{ _memory.Resize( uSize ); }
		void Reserve (usize uSize)			{ _memory.Reserve( uSize ); }
		
		
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
		bool _Find2 (const KeyT &key, OUT const_iterator &result) const
		{
			usize	idx;
			
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

			if ( not FindFirstIndex2( key, OUT first ) )
				return false;
			
			FindLastIndex2( key, first, OUT last );

			result = values_range_t( ptr() + first, last - first + 1 );
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
			bool Find (const KeyT &key, OUT const_iterator &result) const	{ return _ref._Find2( key, OUT result ); }

			template <typename KeyT>
			bool FindAll (const KeyT &key, OUT values_range_t &result)		{ return _ref._FindAll2( key, OUT result ); }
		};


	public:
		_CustomSearch CustomSearch ()	{ return _CustomSearch( *this ); }
	};

}	// _types_hidden_


	template <	typename Value,
				typename S = typename AutoDetectCopyStrategy< Value >::type,
				typename MC = MemoryContainer< Value >
			 >
	using Set = _types_hidden_::BaseSet< Array, Value, true, S, MC >;
	

	template <	typename Value,
				typename S = typename AutoDetectCopyStrategy< Value >::type,
				typename MC = MemoryContainer< Value >
			 >
	using MultiSet = _types_hidden_::BaseSet< Array, Value, false, S, MC >;
	

	template <typename Value, usize Size>
	using FixedSizeSet = _types_hidden_::BaseSet< Array, Value, true,
								typename AutoDetectCopyStrategy< Value >::type,
								StaticMemoryContainer< Value, Size > >;
	

	template <typename Value, usize Size>
	using FixedSizeMultiSet = _types_hidden_::BaseSet< Array, Value, false,
									typename AutoDetectCopyStrategy< Value >::type,
									StaticMemoryContainer< Value, Size > >;

	//template <typename EnumType>
	//using EnumSet = FixedSizeSet< typename EnumType::type, EnumType::_Count >;

	
	template <	template <typename, typename, typename> class Container,
				typename Value,
				bool IsUnique,
				typename S,
				typename MC
			 >
	struct Hash< _types_hidden_::BaseSet< Container, Value, IsUnique, S, MC > > :
		private Hash< ArrayCRef<Value> >
	{
		typedef _types_hidden_::BaseSet< Container, Value, IsUnique, S, MC >	Key_t;
		typedef Hash< ArrayCRef<Value> >										Base_t;
		typedef typename Base_t::Result_t										Result_t;

		Result_t operator () (const Key_t &x) const noexcept
		{
			return Base_t::operator ()( x );
		}
	};

}	// GXTypes
}	// GX_STL
