// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/CompileTime/PlainOldDataType.h"
#include "Engine/STL/CompileTime/TypeList.h"
#include "Engine/STL/CompileTime/NearType.h"

namespace GX_STL
{
namespace CompileTime
{
	
	//
	// Type List From types
	//
	
	namespace _ctime_hidden_
	{
		template <typename T, typename RightTL>
		struct _TypeListFrom_Append
		{
			using type	= TypeList< T, RightTL >;
		};

		template <typename Left, typename Right, typename RightTL>
		struct _TypeListFrom_Append< TypeList<Left, Right>, RightTL >
		{
			using type	= typename TTypeList_Append< TypeList<Left, Right>, RightTL >::type;
		};

		template <typename T, typename ...Types>
		struct _TypeListFrom
		{
			using _next = typename _TypeListFrom< Types... >::type;
			using type	= typename _TypeListFrom_Append< T, _next >::type;
		};

		template <typename Left, typename Right>
		struct _TypeListFrom< TypeList<Left, Right> >
		{
			using type	= TypeList<Left, Right>;
		};

		template <typename T>
		struct _TypeListFrom<T>
		{
			using type	= TypeList< T >;
		};
	}

	template <typename ...Types>
	using TypeListFrom = typename _ctime_hidden_::_TypeListFrom< Types... >::type;



	//
	// Type List From Type Lists
	//
	/*
	namespace _ctime_hidden_
	{
		template <typename T, typename ...Types>
		struct _TypeListFrom
		{
			typedef TypeList< T, typename _TypeListFrom< Types... >::type >	type;
		};

		template <typename T>
		struct _TypeListFrom<T>
		{
			typedef TypeList< T >	type;
		};
	}

	template <typename ...Types>
	using TypeListFrom = typename _ctime_hidden_::_TypeListFrom< Types... >::type;
	*/


	//
	// Value List From
	//
	
	namespace _ctime_hidden_
	{
		template <typename T, T Value0, T ...Values>
		struct _ValueListFrom
		{
			typedef TypeList< ValueToType< T, Value0 >, typename _ValueListFrom< T, Values... >::type >	type;
		};
	
		template <typename T, T Value0>
		struct _ValueListFrom< T, Value0 >
		{
			typedef TypeList< ValueToType< T, Value0 > >	type;
		};
	}

	template <typename T, T ...Values>
	using ValueListFrom = typename _ctime_hidden_::_ValueListFrom< T, Values... >::type;



	//
	// Copy Qualifiers From TypeList
	//
	
	namespace _ctime_hidden_
	{
		template <typename Typelist>
		struct CopyQualifiersFromTypeList
		{
			STATIC_ASSERT( IsTypeList<Typelist> );

			template <usize, typename Type, typename PrevFuncResult, typename>
			struct _TypeList_GetMainQualifier
			{
				typedef ValueToType< ETypeQualifier::type,
					(_SwitchStrongerQualifier< _GetTypeQualifier<Type>::value, PrevFuncResult::value >::value) >	result;
			};

			static const ETypeQualifier::type	qual = Typelist::template ForEach< _TypeList_GetMainQualifier,
														ValueToType<ETypeQualifier::type, ETypeQualifier::Unknown> >::value;

			typedef TypeQualifier< qual >	type;
		};

	}	// _ctime_hidden_
	

	//
	// Copy Qualifier Type
	//

	template <typename Typelist>
	using CopyQualifiersTL =  typename _ctime_hidden_::CopyQualifiersFromTypeList< Typelist >::type;

	template <typename ...Types>
	using CopyQualifiers =  CopyQualifiersTL< TypeListFrom< Types... > >;



	//
	// Choose Type
	//
	
	namespace _ctime_hidden_
	{
		template <uint Index, typename ...Types>
		struct _ChooseType
		{
			typedef TypeListFrom< Types... >		TypeList_t;

			STATIC_ASSERT( Index < TypeList_t::Count );

			typedef typename TypeList_t::template Get< Index >	type;
		};
	}

	template <uint Index, typename ...Types>
	using ChooseType = typename _ctime_hidden_::_ChooseType< Index, Types... >::type;



	//
	// Min / Max From
	//
	
	namespace _ctime_hidden_
	{
		template <usize, typename Type, typename PrevFuncResult, typename>
		struct MinFrom_Func
		{
			typedef typename Type::type		type;

			typedef ValueToType< type, Min< type, Type::value, PrevFuncResult::value > >		result;
		};
		
		template <usize, typename Type, typename PrevFuncResult, typename>
		struct MaxFrom_Func
		{
			typedef typename Type::type		type;

			typedef ValueToType< type, Max< type, Type::value, PrevFuncResult::value > >		result;
		};

	}	// _ctime_hidden_


	template <typename T, T ...Args>
	constexpr T MinFrom ()
	{
		typedef ValueListFrom< T, Args... >		ValueList_t;

		return ValueList_t::template ForEach< _ctime_hidden_::MinFrom_Func, typename ValueList_t::Front >::value;
	}

	template <typename T, T ...Args>
	constexpr T MaxFrom ()
	{
		typedef ValueListFrom< T, Args... >		ValueList_t;

		return ValueList_t::template ForEach< _ctime_hidden_::MaxFrom_Func, typename ValueList_t::Front >::value;
	}



	//
	// General Type
	//
	
	namespace _ctime_hidden_
	{
		template <typename T, typename Other>
		struct _DeferredCreateWith
		{
			typedef typename TypeInfo<T>::template CreateWith<Other>	type;
		};

		template <typename T0, typename T1>
		struct _ToSameCapacity
		{
			static const uint	c0 = TypeDescriptor::GetCapacity<T0>;
			static const uint	c1 = TypeDescriptor::GetCapacity<T1>;
			
			typedef typename TypeInfo<T0>::inner_type				t0;
			typedef typename TypeInfo<T1>::inner_type				t1;

			typedef SwitchType< (c0 >= c1 or IsVoid<T0>), T0, typename _DeferredCreateWith<T1,t0>::type >		left;
			typedef SwitchType< (c1 >= c0 or IsVoid<T1>), T1, typename _DeferredCreateWith<T0,t1>::type >		right;
		};

		template <uint Index, typename T0, typename T1, typename T2 = int>
		struct _ToSameWrapper
		{
			typedef typename TypeInfo<T0>::inner_type	t0;
			typedef typename TypeInfo<T1>::inner_type	t1;

			typedef SwitchType< not IsWrapper<T0> and IsWrapper<T1>,
							typename _DeferredCreateWith<T1,t0>::type, T0 >		type0;
			
			typedef SwitchType< not IsWrapper<T1> and IsWrapper<T0>,
							typename _DeferredCreateWith<T0,t1>::type, T1 >		type1;
			/*
			typedef typename SwitchType< IsWrapper<T0>,
						typename SwitchType< IsWrapper<T1>,
							typename SwitchType< (SizeOf<T0>::bytes >= SizeOf<T1>::bytes),	// T0 and T1 are wrappers
								typename _DeferredCreateWith<T0,T2>::type,					// from T0
								typename _DeferredCreateWith<T1,T2>::type >,				// from T1
							typename _DeferredCreateWith<T0,T2>::type >,				// only T0 is wrapper
						typename SwitchType< IsWrapper<T1>,
							typename _DeferredCreateWith<T1,T2>::type,					// only T1 is wrapper
							T2 > >			type2;										// none of T0 and T1 is wrappers
							*/
			typedef ChooseType< Index, type0, type1, T2 >	type;
		};

		
		template <bool UseCppStandard>
		struct GenType_Choose
		{
			template <usize Index, typename Type, typename PrevFuncResult, typename Typelist>
			struct Func
			{
				// change software types to hardware
				static const uint	change_to =
					not IsSoftware< Type > or IsVoid< Type > ? 0 :
					IsFloat< Type > or IsFixed< Type > ? 1 :
					IsSigned< Type > ? 2 : 3;

				typedef ChooseType< change_to,
									Type,
									typename NearFloat::FromType< Type >,
									typename NearInt::FromType< Type >,
									typename NearUInt::FromType< Type > >	hw_left;

				// set same capacity
				typedef _ToSameCapacity< hw_left, PrevFuncResult >	_same_capacity;

				typedef typename _same_capacity::left	left;
				typedef typename _same_capacity::right	right;

				// calc type index
				static const usize	left_size	= SizeOf<left>::bytes;
				static const usize	right_size	= SizeOf<right>::bytes;
				
				static const uint	left_count = TypeDescriptor::GetCapacity< left >;
				static const uint	right_count = TypeDescriptor::GetCapacity< right >;

				static const uint	type_index =
					( not UseCppStandard and IsSameTypes< left, right > ) ? 0 :
					// biggest capacity
					left_count != right_count ? (left_count > right_count ? 0 : 1) :
					// float types > int types
					IsFloat< left > and IsFloat< right > ? ( left_size >= right_size ? 0 : 1 ) :
					IsFloat< left > ? 0 :
					IsFloat< right > ? 1 :
					// small int types converts to 'int'
					( UseCppStandard and Max< usize, left_size, right_size > < sizeof(int) ) ? 2 :
					// biggest int type
					left_size == right_size and (IsUnsigned< left > or IsUnsigned< right >) ? 
					// unsigned int types > signed types
					(IsUnsigned< left > ? 0 : 1) :
					left_size >= right_size ? 0 : 1;

				// choose type with (new) wrapper
				typedef typename _ToSameWrapper< type_index, left, right, int >::type	result;
			};
		};
	}	// _ctime_hidden_
	
	
	template <typename ...Types>
	using GenType = typename TypeListFrom< Types... >::template
						ForEach< _ctime_hidden_::GenType_Choose<false>::Func, void >;

	template <typename ...Types>
	using GenTypeStd = typename TypeListFrom< Types... >::template
							ForEach< _ctime_hidden_::GenType_Choose<true>::Func, void >;
	


}	// CompileTime
}	// GX_STL
