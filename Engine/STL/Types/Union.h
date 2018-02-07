// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/CompileTime/TypeList.h"
#include "Engine/STL/CompileTime/NewTypeInfo.h"
#include "Engine/STL/Algorithms/Comparators.h"
#include "Engine/STL/Types/VariantRef.h"
#include "Engine/STL/Types/VariantCRef.h"

#ifdef GX_UNRESTRICTED_UNIONS_SUPPORTED

namespace GX_STL
{
namespace GXTypes
{

	//
	// Union Type
	//
	
	template <typename ...Types>
	struct Union : public CompileTime::CopyQualifiers< CompileTime::PODStruct, Types... >
	{
	// types
	public:
		using TypeList_t	= typename CompileTime::TypeListFrom< Types... >;
		using Self			= Union< Types... >;

		static const uint		INVALID_INDEX = UMax;
		
	private:
		struct _TypeList_Destroy;
		struct _TypeList_GetTypeId;
		struct _TypeList_GetSizeOf;
		struct _TypeList_Copy;
		struct _TypeList_Move;

		template <usize CmpType>
		struct _TypeList_Compare;
		
		template <typename Func, typename Data>
		struct _TypeList_Apply;

		static const usize	_DataSize	= TypeList_t::MaxSizeOf();
		static const usize	_DataAlign	= TypeList_t::MaxAlignOf();

		using _AlignedUnion_t	= typename std::aligned_union< 0, Types... >;

		//STATIC_ASSERT( _AlignedUnion_t::_Max_len == _DataSize );
		STATIC_ASSERT( _AlignedUnion_t::alignment_value == _DataAlign );


	// variables
	private:
		alignas(_DataAlign)	char	_data[ _DataSize ];
		uint						_currentIndex;
		DEBUG_ONLY( TypeId			_dbgType );


	// methods
	public:
		Union (GX_DEFCTOR);
		Union (const Self &other);
		Union (Self &&other);
		
		template <typename T>
		explicit Union (const T &value);

		~Union ();

		bool IsDefined () const;
		bool IsSame (const Self &other) const;
		void Destroy () noexcept;

		TypeId			GetCurrentTypeId ()		const;
		usize			GetCurrentIndex ()		const		{ return _currentIndex; }
		BytesU			GetSizeOf ()			const;
		BinArrayCRef	GetData ()				const		{ return BinArrayCRef::From( _data ); }	// TODO: use GetSizeOf ?
		VariantRef		GetVariantRef ()					{ return VariantRef::FromVoid( _data, GetCurrentTypeId() ); }
		VariantCRef		GetVariantRef ()		const		{ return VariantCRef::FromVoid( _data, GetCurrentTypeId() ); }


		bool operator == (const Self &right) const;
		bool operator >  (const Self &right) const;
		bool operator <  (const Self &right) const;
		bool operator != (const Self &right) const			{ return not (*this == right); }
		bool operator >= (const Self &right) const			{ return not (*this <  right); }
		bool operator <= (const Self &right) const			{ return not (*this >  right); }

		Self& operator = (const Self &right);
		Self& operator = (Self &&right);
		
		explicit operator bool () const						{ return IsDefined(); }


		// Type Access //
		template <typename T>		Self &		Create (const T &value) noexcept;
		template <typename T>		Self &		Create2 (T &&value) noexcept;
		template <typename T>		Self &		Set (const T &value) noexcept;

		template <typename Func>	void		Apply (Func& func) noexcept;
		template <typename Func>	void		Apply (const Func& func) const noexcept;

		template <typename T>		bool		Is () const;

		template <typename T>		T &			Get ();
		template <typename T>		T const &	Get () const;
		template <typename T>		T			Get (const T &defaultValue) const;

		template <typename T>		T &			Cast ();
		template <typename T>		T const &	Cast () const;


	private:
		void _Copy (const Self &other) noexcept;
		void _Move (Self &&other) noexcept;

		template <typename T>
		static void _HasType ();
	};


	
/*
=================================================
	_TypeList_Destroy
=================================================
*/
	template <typename ...Types>
	struct Union<Types...>::_TypeList_Destroy
	{
		const usize	index;
		void *		ptr;

		_TypeList_Destroy (void *ptr, usize index) : index(index), ptr(ptr)
		{}

		template <typename T, usize Index>
		forceinline void Process ()
		{
			if ( index == Index )
			{
				((T *) ptr)->~T();
			}
		}
	};
	
/*
=================================================
	_TypeList_GetTypeId
=================================================
*/
	template <typename ...Types>
	struct Union<Types...>::_TypeList_GetTypeId
	{
		TypeId		id;
		const usize	index;

		_TypeList_GetTypeId (usize index) : index(index)
		{}
			
		template <typename T, usize Index>
		forceinline void Process ()
		{
			if ( index == Index )
			{
				id = TypeIdOf<T>();
			}
		}
	};
	
/*
=================================================
	_TypeList_GetSizeOf
=================================================
*/
	template <typename ...Types>
	struct Union<Types...>::_TypeList_GetSizeOf
	{
		usize		sizeOf;
		const usize	index;

		_TypeList_GetSizeOf (usize index) : sizeOf(0), index(index)
		{}
			
		template <typename T, usize Index>
		forceinline void Process ()
		{
			if ( index == Index )
			{
				sizeOf = sizeof( T );
			}
		}
	};
	
/*
=================================================
	_TypeList_Apply
=================================================
*/
	template <typename ...Types>
	template <typename Func, typename Data>
	struct Union<Types...>::_TypeList_Apply
	{
		Data		data;
		Func		func;
		const usize	index;

		_TypeList_Apply (Func func, Data data, usize index) :
			data(data), func(func), index(index)
		{}

		template <typename T, usize Index>
		forceinline void Process ()
		{
			using Result_t	= TypeTraits::CopyConstToPointer< Data, T *>;

			if ( index == Index )
			{
				func( *reinterpret_cast< Result_t >( data ) );
			}
		}
	};
	
/*
=================================================
	_TypeList_Copy
=================================================
*/
	template <typename ...Types>
	struct Union<Types...>::_TypeList_Copy
	{
		Self const& src;
		Self &		dst;

		_TypeList_Copy (Self const &src, Self &dst) :
			src(src), dst(dst)
		{}
			
		template <typename T, usize Index>
		forceinline void Process ()
		{
			if ( src.GetCurrentIndex() == Index )
			{
				dst.Create( src.Get<T>() );
			}
		}
	};
	
/*
=================================================
	_TypeList_Move
=================================================
*/
	template <typename ...Types>
	struct Union<Types...>::_TypeList_Move
	{
		Self &		src;
		Self &		dst;

		_TypeList_Move (Self &src, Self &dst) :
			src(src), dst(dst)
		{}
			
		template <typename T, usize Index>
		forceinline void Process ()
		{
			if ( src.GetCurrentIndex() == Index )
			{
				dst.Create2( RVREF( src.Get<T>() ) );
			}
		}
	};
	
/*
=================================================
	_TypeList_Compare
=================================================
*/
	template <typename ...Types>
	template <usize CmpType>
	struct Union<Types...>::_TypeList_Compare
	{
		Self const&	left;
		Self const&	right;
		bool		result;

		_TypeList_Compare (Self const& left, Self const& right) :
			left(left), right(right), result(false)
		{}

		template <typename T, usize Index>
		forceinline void Process ()
		{
			using Comp = GXTypes::Comparator<T>;

			if ( left.GetCurrentIndex() == Index )
			{
				if_constexpr ( CmpType == 0 )
				{
					result = Comp( left.Get<T>() ) == Comp( right.Get<T>() );
				}
				else
				if_constexpr ( CmpType == 1 )
				{
					result = Comp( left.Get<T>() ) < Comp( right.Get<T>() );
				}
				else
				if_constexpr ( CmpType == 2 )
				{
					result = Comp( left.Get<T>() ) > Comp( right.Get<T>() );
				}

				STATIC_ASSERT( CmpType < 3 );
			}
		}
	};
	
/*
=================================================
	constructor
=================================================
*/
	template <typename ...Types>
	inline Union<Types...>::Union (UninitializedType) :
		_currentIndex(INVALID_INDEX)
	{
		DEBUG_ONLY( ZeroMem( _data ) );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename ...Types>
	inline Union<Types...>::Union (const Self &other) :
		_currentIndex(INVALID_INDEX)
	{
		DEBUG_ONLY( ZeroMem( _data ) );

		_Copy( other );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename ...Types>
	inline Union<Types...>::Union (Self &&other) :
		_currentIndex(INVALID_INDEX)
	{
		DEBUG_ONLY( ZeroMem( _data ) );

		_Move( RVREF( other ) );
	}
		
/*
=================================================
	constructor
=================================================
*/
	template <typename ...Types>
	template <typename T>
	inline Union<Types...>::Union (const T &value) :
		_currentIndex(INVALID_INDEX)
	{
		Create( value );
	}
		
/*
=================================================
	destructor
=================================================
*/
	template <typename ...Types>
	inline Union<Types...>::~Union ()
	{
		Destroy();
	}
	
/*
=================================================
	GetCurrentTypeId
=================================================
*/
	template <typename ...Types>
	forceinline TypeId  Union<Types...>::GetCurrentTypeId () const
	{
		// TODO: cache this value
		_TypeList_GetTypeId		func( _currentIndex );
		TypeList_t::RuntimeForEach( func );
		return func.id;
	}
	
/*
=================================================
	IsDefined
=================================================
*/
	template <typename ...Types>
	inline bool Union<Types...>::IsDefined () const
	{
		return _currentIndex != INVALID_INDEX;
	}
	
/*
=================================================
	GetSizeOf
=================================================
*/
	template <typename ...Types>
	forceinline BytesU Union<Types...>::GetSizeOf () const
	{
		_TypeList_GetSizeOf		func( _currentIndex );
		TypeList_t::RuntimeForEach( func );
		return BytesU( func.sizeOf );
	}
	
/*
=================================================
	Create
=================================================
*/
	template <typename ...Types>
	template <typename T>
	forceinline Union<Types...> &  Union<Types...>::Create (const T &value) noexcept
	{
		_HasType<T>();
		Destroy();

		STATIC_ASSERT( sizeof(value) <= sizeof(_data) );

		_currentIndex = TypeList_t::template IndexOf<T>;
		DEBUG_ONLY( _dbgType = TypeIdOf<T>() );
			
		UnsafeMem::PlacementNew<T>( _data, value );
		return *this;
	}
	
/*
=================================================
	Create2
=================================================
*/
	template <typename ...Types>
	template <typename T>
	forceinline Union<Types...> &  Union<Types...>::Create2 (T &&value) noexcept
	{
		_HasType<T>();
		Destroy();

		_currentIndex = TypeList_t::template IndexOf<T>;
		DEBUG_ONLY( _dbgType = TypeIdOf<T>() );
			
		UnsafeMem::PlacementNew<T>( _data, RVREF( value ) );
		return *this;
	}
		
/*
=================================================
	Is
=================================================
*/
	template <typename ...Types>
	template <typename T>
	forceinline bool Union<Types...>::Is () const
	{
		_HasType<T>();
		return _currentIndex == TypeList_t::template IndexOf<T>;
	}
	
/*
=================================================
	IsSame
=================================================
*/
	template <typename ...Types>
	forceinline bool Union<Types...>::IsSame (const Self &other) const
	{
		return this->GetCurrentIndex() == other.GetCurrentIndex();
	}
	
/*
=================================================
	Get
=================================================
*/
	template <typename ...Types>
	template <typename T>
	forceinline T &  Union<Types...>::Get ()
	{
		ASSERT( Is<T>() );
		return *PointerCast< T >( _data );
	}
	
/*
=================================================
	Get
=================================================
*/
	template <typename ...Types>
	template <typename T>
	forceinline T const &  Union<Types...>::Get () const
	{
		ASSERT( Is<T>() );
		return *PointerCast< T >( _data );
	}
	
/*
=================================================
	Get
=================================================
*/
	template <typename ...Types>
	template <typename T>
	forceinline T  Union<Types...>::Get (const T &defaultValue) const
	{
		return Is<T>() ? *PointerCast< T >( _data ) : defaultValue;
	}
	
/*
=================================================
	Cast
=================================================
*/
	template <typename ...Types>
	template <typename T>
	forceinline T &  Union<Types...>::Cast ()
	{
		_HasType<T>();
		ASSERT( IsDefined() );
		return *PointerCast< T >( _data );
	}
	
/*
=================================================
	Cast
=================================================
*/
	template <typename ...Types>
	template <typename T>
	forceinline T const &  Union<Types...>::Cast () const
	{
		_HasType<T>();
		ASSERT( IsDefined() );
		return *PointerCast< T >( _data );
	}
	
/*
=================================================
	Set
=================================================
*/
	template <typename ...Types>
	template <typename T>
	forceinline Union<Types...> &  Union<Types...>::Set (const T &value) noexcept
	{
		Get<T>() = value;
		return *this;
	}
	
/*
=================================================
	Apply
=================================================
*/
	template <typename ...Types>
	template <typename Func>
	forceinline void Union<Types...>::Apply (Func& func) noexcept
	{
		ASSERT( IsDefined() );
		_TypeList_Apply< Func&, void* >	iter( func, _data, _currentIndex );
		TypeList_t::RuntimeForEach( iter );
	}
	
/*
=================================================
	Apply
=================================================
*/
	template <typename ...Types>
	template <typename Func>
	forceinline void Union<Types...>::Apply (const Func& func) const noexcept
	{
		ASSERT( IsDefined() );
		_TypeList_Apply< const Func&, void const * const >	iter( func, _data, _currentIndex );
		TypeList_t::RuntimeForEach( iter );
	}
	
/*
=================================================
	operator ==
=================================================
*/
	template <typename ...Types>
	forceinline bool Union<Types...>::operator == (const Self &right) const
	{
		if ( not IsSame( right ) )
			return false;

		_TypeList_Compare<0>	func( *this, right );
		TypeList_t::RuntimeForEach( func );
		return func.result;
	}
	
/*
=================================================
	operator >
=================================================
*/
	template <typename ...Types>
	forceinline bool Union<Types...>::operator >  (const Self &right) const
	{
		if ( not IsSame( right ) )
			return false;
		
		_TypeList_Compare<1>	func( *this, right );
		TypeList_t::RuntimeForEach( func );
		return func.result;
	}
	
/*
=================================================
	operator <
=================================================
*/
	template <typename ...Types>
	forceinline bool Union<Types...>::operator <  (const Self &right) const
	{
		if ( not IsSame( right ) )
			return false;
		
		_TypeList_Compare<2>	func( *this, right );
		TypeList_t::RuntimeForEach( func );
		return func.result;
	}
	
/*
=================================================
	operator =
=================================================
*/
	template <typename ...Types>
	forceinline Union<Types...>&  Union<Types...>::operator = (const Self &right)
	{
		Destroy();
		_Copy( right );
		return *this;
	}
	
/*
=================================================
	operator =
=================================================
*/
	template <typename ...Types>
	forceinline Union<Types...>&  Union<Types...>::operator = (Self &&right)
	{
		Destroy();
		_Move( RVREF( right ) );
		return *this;
	}
	
/*
=================================================
	Destroy
=================================================
*/
	template <typename ...Types>
	forceinline void Union<Types...>::Destroy () noexcept
	{
		if ( not IsDefined() )
			return;
		
		if_constexpr ( CompileTime::IsDtorAvailable< Self > )
		{
			_TypeList_Destroy	func( _data, _currentIndex );
			TypeList_t::RuntimeForEach( func );
		}
		
		DEBUG_ONLY( _dbgType = TypeId() );
		DEBUG_ONLY( ZeroMem( _data ) );

		_currentIndex = INVALID_INDEX;
	}
	
/*
=================================================
	_HasType
=================================================
*/
	template <typename ...Types>
	template <typename T>
	forceinline void Union<Types...>::_HasType ()
	{
		STATIC_ASSERT( (TypeList_t::template HasType<T>), "type not presented in union" );
	}
	
/*
=================================================
	_Copy
=================================================
*/
	template <typename ...Types>
	forceinline void Union<Types...>::_Copy (const Self &other) noexcept
	{
		if ( not other.IsDefined() )
			return;

		if_constexpr ( not CompileTime::IsCtorAvailable< Self > and CompileTime::IsMemCopyAvailable< Self > )
		{
			UnsafeMem::MemMove( _data, other._data, SizeOf(_data) );
			_currentIndex = other._currentIndex;
		}
		else
		{
			_TypeList_Copy	func( other, *this );
			TypeList_t::RuntimeForEach( func );
		}
	}
	
/*
=================================================
	_Move
=================================================
*/
	template <typename ...Types>
	forceinline void Union<Types...>::_Move (Self &&other) noexcept
	{
		if ( not other.IsDefined() )
			return;
		
		if_constexpr ( CompileTime::IsMemCopyAvailable< Self > )
		{
			UnsafeMem::MemMove( _data, other._data, SizeOf(_data) );
			_currentIndex = other._currentIndex;
			other._currentIndex = INVALID_INDEX;
		}
		else
		{
			_TypeList_Move	func( other, *this );
			TypeList_t::RuntimeForEach( func );
		}
	}


	
/*
=================================================
	Hash
=================================================
*/
	template <typename ...Types>
	struct Hash< Union<Types...> >
	{
		HashResult  operator () (const Union<Types...> &x) const noexcept
		{
			// TODO: it is not same as Hash( union.Get<T>() )
			return HashOf( x.GetData() );
		}
	};

}	// GXTypes
}	// GX_STL

#endif	// GX_UNRESTRICTED_UNIONS_SUPPORTED
