// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "ArrayRef.h"
#include "Engine/STL/Defines/MemberDetector.h"

namespace GX_STL
{
namespace GXTypes
{
namespace ContainerAdaptors
{

	namespace _adaptors_hidden_
	{
		GX_CREATE_MEMBER_DETECTOR( Erase );
		GX_CREATE_MEMBER_DETECTOR( EraseByIndex );
		GX_CREATE_MEMBER_DETECTOR( erase );
		
		template <typename Class>
		static constexpr bool	Has_Erase		=     Detect_Erase<Class>::value		and
												  not Detect_EraseByIndex<Class>::value	and
												  not Detect_erase<Class>::value;

		template <typename Class>
		static constexpr bool	Has_EraseByIndex =    Detect_Erase<Class>::value		and
												      Detect_EraseByIndex<Class>::value	and
												  not Detect_erase<Class>::value;
		
		template <typename Class>
		static constexpr bool	Has_erase		= not Detect_Erase<Class>::value		and
												  not Detect_EraseByIndex<Class>::value	and
												      Detect_erase<Class>::value;

	}	// _adaptors_hidden_


	//
	// Erasable Adaptor
	//

	template <typename T>
	struct Erasable
	{
	// types
	private:
		using EraseByIndexFunc_t	= void (*) (void *container, usize index, OUT ArrayRef<T> &arr);
		

	// variables
	private:
		ArrayRef<T>			_arr;			// TODO: use UnitBuffer
		void *				_ref			= null;
		EraseByIndexFunc_t	_eraseByIdx		= null;
		
		DEBUG_ONLY( TypeId	_dbgType; )


	// methods
	public:
		Erasable () = delete;

		Erasable (Erasable &&) = default;
		Erasable (const Erasable &) = default;

		Erasable& operator = (Erasable &&) = delete;
		Erasable& operator = (const Erasable &) = delete;

		template <typename Container>
		Erasable (Container &other)
			DEBUG_ONLY( : _dbgType(TypeIdOf<Container>()) )
		{
			_Create( other );
		}

		bool IsValid () const
		{
			return _ref and _eraseByIdx;
		}

		void Erase (usize index)
		{
			ASSERT( IsValid() );
			ASSERT( index < _arr.Count() );

			_eraseByIdx( _ref, index, OUT _arr );
		}
		
		explicit operator bool ()				const		{ return IsValid(); }

		T&			operator [] (usize index)				{ return _arr[index]; }
		T const&	operator [] (usize index)	const		{ return _arr[ index ]; }

		usize		Count ()					const		{ return _arr.Count(); }
		bool		Empty ()					const		{ return _arr.Empty(); }
		

	private:
		template <typename C>
		void _Create (C &other, CompileTime::EnableIf< _adaptors_hidden_::Has_Erase<C>, int > = 0)
		{
			_arr			= other;
			_ref			= PointerCast<void>( &other );
			_eraseByIdx		= LAMBDA() (void *ptr, usize index, OUT ArrayRef<T> &result)
								{
									auto&	arr = *PointerCast<C>( ptr );
									arr.Erase( index );
									result = arr;
								};
		}

		template <typename C>
		void _Create (C &other, CompileTime::EnableIf< _adaptors_hidden_::Has_EraseByIndex<C>, int > = 0)
		{
			_arr			= other;
			_ref			= PointerCast<void>( &other );
			_eraseByIdx		= LAMBDA() (void *ptr, usize index, OUT ArrayRef<T> &result)
								{
									auto&	arr = *PointerCast<C>( ptr );
									arr.EraseByIndex( index );
									result = arr;
								};
		}

		template <typename C>
		void _Create (C &other, CompileTime::EnableIf< _adaptors_hidden_::Has_erase<C>, int > = 0)
		{
			_arr			= ArrayRef<T>::FromStd( other );
			_ref			= PointerCast<void>( &other );
			_eraseByIdx		= LAMBDA() (void *ptr, usize index, OUT ArrayRef<T> &result)
								{
									auto&	arr = *PointerCast<C>( ptr );
									arr.erase( arr.begin() + index );
									result = ArrayRef<T>::FromStd( arr );
								};
		}
	};


}	// ContainerAdaptors
}	// GXTypes
}	// GX_STL
