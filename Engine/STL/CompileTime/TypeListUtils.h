// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "TemplateUtils.h"
#include "TemplateMath.h"
#include "NewTypeInfo.h"
#include "Engine/STL/Types/Noninstancable.h"

namespace GX_STL
{
namespace CompileTime
{

	//
	// Type List
	//

	namespace _ctime_hidden_
	{
		struct TypeListBase : public Noninstancable
		{};
	}


	template <typename Left, typename Right>
	struct TypeList;

	struct TypeListEnd final : Noninstancable {};

	template <typename T>
	static constexpr bool IsTypeList = IsBaseOf< _ctime_hidden_::TypeListBase, typename TypeTraits::GetBaseType< T > >;;



	namespace _ctime_hidden_
	{
		//
		// Type List Push Back
		//
		
		template <typename Typelist, typename Type>
		struct TTypeList_PushBack;
 
		template <typename Left, typename Right, typename Type>
		struct TTypeList_PushBack < TypeList< Left, Right >, Type >
		{
			typedef TypeList< Left, typename TTypeList_PushBack< Right, Type >::type >	type;
		};

		template <typename Right, typename Type>
		struct TTypeList_PushBack < TypeList< TypeListEnd, Right >, Type >
		{
			typedef TypeList< Type, TypeListEnd >	type;
		};

		template <typename Type>
		struct TTypeList_PushBack< TypeListEnd, Type >
		{
			typedef TypeList< Type, TypeListEnd >	type;
		};



		//
		// Type List Push Front
		//
		
		template <typename Typelist, typename Type>
		struct TTypeList_PushFront
		{
			typedef TypeList< Type, Typelist >	type;
		};



		//
		// Type List Length
		//

		template <typename Typelist>
		struct TTypeList_Length;
 
		template <typename Left, typename Right>
		struct TTypeList_Length < TypeList< Left, Right > >
		{
			static const usize	value = 1 + TTypeList_Length< Right >::value;
		};

		template <typename Right>
		struct TTypeList_Length < TypeList< TypeListEnd, Right > >
		{
			static const usize	value = 0;
		};

		template <>
		struct TTypeList_Length< TypeListEnd >
		{
			static const usize	value = 0;
		};



		//
		// Type List Get Element
		//

		template <typename Typelist, usize Index>
		struct TTypeList_GetElem;

		template <typename Typelist>
		struct TTypeList_GetElem < Typelist, 0 >
		{
			typedef Typelist	type;
		};

		template <typename Left, typename Right>
		struct TTypeList_GetElem < TypeList< Left, Right >, 0 >
		{
			typedef Left		type;
		};

		template <typename Left, typename Right, usize Index>
		struct TTypeList_GetElem < TypeList< Left, Right >, Index >
		{
			typedef typename TTypeList_GetElem< Right, Index-1 >::type	type;
		};

		template <typename Right, usize Index>
		struct TTypeList_GetElem < TypeList< TypeListEnd, Right >, Index >
		{
			typedef TypeListEnd		type;
		};

		template <usize Index>
		struct TTypeList_GetElem < TypeListEnd, Index >
		{
			typedef TypeListEnd		type;
		};


		
		//
		// Compile Time For Each
		//

		template <	typename Typelist,
					typename FullTypelist,
					usize Index, 
					template <usize Index, typename Type, typename PrevFuncResult, typename Typelist> class Func,
					typename PrevFuncResult
				 >
		struct TTypeList_CompileTimeForEach;

		template <	typename Left, typename Right,
					typename FullTypelist,
					usize Index,
					template <usize Index, typename Type, typename PrevFuncResult, typename Typelist> class Func,
					typename PrevFuncResult
				 >
		struct TTypeList_CompileTimeForEach< CompileTime::TypeList< Left, Right >, FullTypelist, Index, Func, PrevFuncResult >
		{
			STATIC_ASSERT( Index < Index+1, "overflow" );
			typedef typename Func< Index, Left, PrevFuncResult, FullTypelist >::result							prev;
			typedef typename TTypeList_CompileTimeForEach< Right, FullTypelist, Index+1, Func, prev >::type		type;
		};
			
		template <	typename Right,
					typename FullTypelist,
					usize Index,
					template <usize Index, typename Type, typename PrevFuncResult, typename Typelist> class Func,
					typename PrevFuncResult
				 >
		struct TTypeList_CompileTimeForEach< CompileTime::TypeList< TypeListEnd, Right >, FullTypelist, Index, Func, PrevFuncResult >
		{
			typedef PrevFuncResult	type;
		};

		template <	typename FullTypelist,
					usize Index,
					template <usize Index, typename Type, typename PrevFuncResult, typename Typelist> class Func,
					typename PrevFuncResult
				 >
		struct TTypeList_CompileTimeForEach< TypeListEnd, FullTypelist, Index, Func, PrevFuncResult >
		{
			typedef PrevFuncResult	type;
		};



		//
		// Compile Time Reverse For Each
		//

		template <	typename Typelist,
					typename FullTypelist,
					usize Index, 
					template <usize Index, typename Type, typename NextFuncResult, typename Typelist> class Func,
					typename NextFuncResult
				 >
		struct TTypeList_CompileTimeReverseForEach;

		template <	typename Left, typename Right,
					typename FullTypelist,
					usize Index,
					template <usize Index, typename Type, typename NextFuncResult, typename Typelist> class Func,
					typename NextFuncResult
				 >
		struct TTypeList_CompileTimeReverseForEach< CompileTime::TypeList< Left, Right >, FullTypelist, Index, Func, NextFuncResult >
		{
			STATIC_ASSERT( Index < Index+1 );
			typedef typename TTypeList_CompileTimeReverseForEach< Right, FullTypelist, Index+1, Func, NextFuncResult >::type	next;
			typedef typename Func< Index, Left, next, FullTypelist >::result													type;
		};
			
		template <	typename Right,
					typename FullTypelist,
					usize Index,
					template <usize Index, typename Type, typename NextFuncResult, typename Typelist> class Func,
					typename NextFuncResult
				 >
		struct TTypeList_CompileTimeReverseForEach< CompileTime::TypeList< TypeListEnd, Right >, FullTypelist, Index, Func, NextFuncResult >
		{
			typedef NextFuncResult	type;
		};

		template <	typename FullTypelist,
					usize Index,
					template <usize Index, typename Type, typename NextFuncResult, typename Typelist> class Func,
					typename NextFuncResult
				 >
		struct TTypeList_CompileTimeReverseForEach< TypeListEnd, FullTypelist, Index, Func, NextFuncResult >
		{
			typedef NextFuncResult	type;
		};



		//
		// Compile Time For Each Wrappers
		//

		template <template <typename Type, typename PrevFuncResult> class WrappedFunc>
		struct TTypeList_FuncWrapper2
		{
			template <usize Index, typename Type, typename PrevFuncResult, typename Typelist>
			struct Func
			{
				typedef typename WrappedFunc< Type, PrevFuncResult >::result	result;
			};
		};

		template <template <usize Index, typename Type, typename PrevFuncResult> class WrappedFunc>
		struct TTypeList_FuncWrapper3
		{
			template <usize Index, typename Type, typename PrevFuncResult, typename Typelist>
			struct Func
			{
				typedef typename WrappedFunc< Index, Type, PrevFuncResult >::result	result;
			};
		};



		//
		// Type List Runtime For Each
		//
		/*
			example:
			struct Func
			{
				usize result;

				template <typename T, usize Index>
				void Process ()	{ result += sizeof(T); }
			};
		*/

		template <typename Typelist, typename Func, usize Index>
		struct TTypeList_ForEach;
		
		template <typename Left, typename Right, typename Func, usize Index>
		struct TTypeList_ForEach< TypeList< Left, Right >, Func, Index >
		{
			static void Iterate (Func &func)
			{
				func.template Process< Left, Index >();
				TTypeList_ForEach< Right, Func, Index+1 >::Iterate( func );
			}
		};

		template <typename Right, typename Func, usize Index>
		struct TTypeList_ForEach< TypeList< TypeListEnd, Right >, Func, Index > {
			static void Iterate (Func &)
			{}
		};

		template <typename Func, usize Index>
		struct TTypeList_ForEach< TypeList< TypeListEnd, TypeListEnd >, Func, Index > {
			static void Iterate (Func &)
			{}
		};

		template <typename Func, usize Index>
		struct TTypeList_ForEach< TypeListEnd, Func, Index > {
			static void Iterate (Func &)
			{}
		};

	
		//
		// Helpers
		//
		
		template <typename RefType>
		struct TTypeList_GetFirstIndexOfType
		{
			template <usize Index, typename Type, typename PrevFuncResult, typename Typelist>
			struct Func
			{
				static const usize	val0 = IsSameTypes< Type, RefType > ? Index : usize(-1);
				static const usize	val1 = PrevFuncResult::value;
				static const usize	val2 = Min< usize, val0, val1 >;

				typedef ValueToType< usize, val2 >	result;
			};
		};
		
		template <typename RefType>
		struct TTypeList_GetLastIndexOfType
		{
			template <usize Index, typename Type, typename PrevFuncResult, typename Typelist>
			struct Func
			{
				static const usize	val0 = IsSameTypes< Type, RefType > ? Index : usize(-1);
				static const usize	val1 = PrevFuncResult::value;
				static const usize	val2 = val0 != usize(-1) and val1 != usize(-1) ?
											Max< usize, val0, val1 > :
											Min< usize, val0, val1 >;
				
				typedef ValueToType< usize, val2 >	result;
			};
		};
		
		template <typename RefType>
		struct TTypeList_IsAllSameAs
		{
			template <usize Index, typename Type, typename PrevFuncResult, typename Typelist>
			struct Func
			{
				static const bool	val0 = IsSameTypes< RefType, Type >;
				static const bool	val1 = val0 and PrevFuncResult::value;

				typedef ValueToType< bool, val1 >		result;
			};
		};
		
		template <typename RefTypeList>
		struct TTypeList_Equal
		{
			template <usize Index, typename Type, typename PrevFuncResult, typename Typelist>
			struct Func
			{
				static const bool	val = IsSameTypes< Type, typename RefTypeList::Get< Index > >;

				typedef ValueToType< bool, (val and PrevFuncResult::value) >	result;
			};
		};
		
		template <usize Index, typename Type, typename PrevFuncResult, typename Typelist>
		struct TTypeList_MaxSize
		{
			typedef ValueToType< usize, (Max< usize, SizeOf<Type>::bytes, PrevFuncResult::value >) >	result;
		};
		
		template <usize Index, typename Type, typename PrevFuncResult, typename Typelist>
		struct TTypeList_MaxAlign
		{
			typedef ValueToType< usize, (Max< usize, AlignOf<Type>::bytes, PrevFuncResult::value >) >	result;
		};
		
		template <usize First, usize Last>
		struct TTypeList_SubListFunc
		{
			template <usize Index, typename Type, typename PrevFuncResult, typename Typelist>
			struct Func
			{
				typedef typename SwitchType< (Index >= First and Index < Last), Type, TypeListEnd >		L;
				typedef PrevFuncResult																	R;

				typedef typename SwitchType< IsSameTypes< TypeListEnd, L >, R, TypeList< L, R > >	result;
			};
		};


		//
		// Append
		//
		
		template <typename LeftTypelist, typename RightTypelist, usize Index, bool IsEnd = false>
		struct TTypeList_Append2
		{
			static const usize	llen	= LeftTypelist::Count;
			static const usize	rlen	= RightTypelist::Count;
			static const usize	nextIdx = Index + 1;

			typedef typename SwitchType< (Index < llen),
									typename LeftTypelist::template Get< Clamp< isize, Index, 0, llen-1 > >, 
									typename RightTypelist::template Get< Clamp< isize, Index - llen, 0, rlen-1 > > >	ltype;

			typedef typename TTypeList_Append2< LeftTypelist, RightTypelist, nextIdx, (nextIdx >= llen + rlen) >::type	rtype;

			typedef TypeList< ltype, rtype >	type;
		};

		template <typename LeftTypelist, typename RightTypelist, usize Index>
		struct TTypeList_Append2< LeftTypelist, RightTypelist, Index, true >
		{
			typedef TypeListEnd		type;
		};

		template <typename LeftTypelist, typename RightTypelist>
		struct TTypeList_Append
		{
			STATIC_ASSERT( IsTypeList<LeftTypelist> and IsTypeList<RightTypelist> );

			typedef typename TTypeList_Append2< LeftTypelist, RightTypelist, 0, (LeftTypelist::Count + RightTypelist::Count == 0) >::type	type;
		};
		


		//
		// Sub List
		//

		template <typename Typelist, usize First, usize Count>
		struct TTypeList_SubList2
		{
			static const usize	length	= TTypeList_Length< Typelist >::value;

			STATIC_ASSERT(	First < length and First + Count <= length );

			typedef typename TTypeList_CompileTimeReverseForEach< Typelist, Typelist, 0, 
					TTypeList_SubListFunc< First, First + Count >::Func, TypeListEnd >::type	type;
		};

		template <typename Typelist, usize First>
		struct TTypeList_SubList2< Typelist, First, 0 >
		{
			typedef TypeListEnd		type;
		};
		
		template <typename Typelist, usize First, usize Count>
		struct TTypeList_SubList
		{
			static const usize	first	= Max< isize, 0, First >;
			static const usize	count	= Max< isize, 0, Count >;

			typedef typename TTypeList_SubList2< Typelist, first, count >::type		type;
		};



		//
		// Erase
		//
		
		template <typename ErasedTypes>
		struct TTypeList_EraseFunc
		{
			static const bool	is_typelist = IsTypeList< ErasedTypes >;

			template <typename Index, typename Type, typename PrevFuncResult, typename Typelist>
			struct Build1 {
				static const bool	skip	= not (ErasedTypes::HasType< Type >);
				using result				= SwitchType< skip, TypeList< Type, PrevFuncResult >, PrevFuncResult >;
			};

			template <typename Index, typename Type, typename PrevFuncResult, typename Typelist>
			struct Build2 {
				using result	= SwitchType< (not IsSameTypes< ErasedTypes, Type >), TypeList< Type, PrevFuncResult >, PrevFuncResult >;
			};

			template <usize Index, typename Type, typename PrevFuncResult, typename Typelist>
			using Build = typename SwitchType< is_typelist,
							DeferredTemplate< Build1, ValueToType<usize, Index>, Type, PrevFuncResult, Typelist >,
							DeferredTemplate< Build2, ValueToType<usize, Index>, Type, PrevFuncResult, Typelist > >::type;
		};


		
		//
		// Remove Duplicates
		//

		struct TTypeList_RemoveDuplicatesFunc
		{
			template <bool IsTL, typename Type, typename PrevFuncResult>
			struct Skip
			{
				static const bool	value = PrevFuncResult::HasType< Type >;
			};

			template <typename Type, typename PrevFuncResult>
			struct Skip< false, Type, PrevFuncResult >
			{
				static const bool	value = false;
			};

			template <usize Index, typename Type, typename PrevFuncResult, typename Typelist>
			struct Build
			{
				static const bool	skip = Skip< IsTypeList< PrevFuncResult >, Type, PrevFuncResult >::value;

				using result	= SwitchType< not skip,
										TypeList< Type, PrevFuncResult >,
										PrevFuncResult >;
			};
		};


	}	// _ctime_hidden_


}	// CompileTime
}	// GX_STL
