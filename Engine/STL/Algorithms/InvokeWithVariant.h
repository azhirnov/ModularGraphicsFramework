// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Types/Function.h"
#include "Engine/STL/Types/Delegate.h"
#include "Engine/STL/Types/VariantRef.h"
#include "Engine/STL/Types/VariantCRef.h"
#include "Engine/STL/CompileTime/Sequence.h"
#include "Engine/STL/CompileTime/TypeTraits.h"
#include "Engine/STL/CompileTime/CompileTimeDebug.h"
#include "Engine/STL/Experimental/FunctionInfo.h"
#include "Engine/STL/Algorithms/ArrayUtils.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Invoke With Variant
	//

	template <typename Result, typename ArgsTypeList>
	struct InvokeWithVariantImpl
	{
	// methods
	public:
		template <typename FuncType, typename ...VariantArrays>
		forceinline static Result Call (const FuncType &func, const VariantArrays &...varArrays)
		{
			_CheckArgs< typename VariantArrays::Typelist_t... >();

			return _Call( func, varArrays..., CompileTime::IndexSequenceFromSize< VariantArrays::Typelist_t::Count >()... );
		}


	private:
		template <typename ...TypeLists>
		static void _CheckArgs ()
		{
			using Args = CompileTime::TypeListFrom< TypeLists... >;
			STATIC_ASSERT( Args::Equal< ArgsTypeList > );
			
			//CompileTime::Debug::TypeListToString< Args >::Show();
			//CompileTime::Debug::TypeListToString< ArgsTypeList >::Show();
		}


		template <typename FuncType, typename ...Args>
		forceinline static Result _CallImpl (const FuncType &func, Args&& ...args)
		{
			STATIC_ASSERT( CountOf<Args...>() == ArgsTypeList::Count );

			return func( FW<Args>( args )... );
		}


		template <	typename FuncType,
					typename VariantArray1,
					usize ...I1
				 >
		forceinline static
			Result _Call (const FuncType &func, 
						  const VariantArray1 &varArray1,
						  CompileTime::IndexSequence<I1...>)
		{
			return _CallImpl( func, varArray1.Get<I1>()... );
		}


		template <	typename FuncType,
					typename VariantArray1, typename VariantArray2,
					usize ...I1, usize ...I2
				 >
		forceinline static
			Result _Call (const FuncType &func, 
						  const VariantArray1 &varArray1, const VariantArray2 &varArray2,
						  CompileTime::IndexSequence<I1...>, CompileTime::IndexSequence<I2...>)
		{
			return _CallImpl( func, varArray1.Get<I1>()..., varArray2.Get<I2>()... );
		}


		template <	typename FuncType,
					typename VariantArray1, typename VariantArray2, typename VariantArray3,
					usize ...I1, usize ...I2, usize ...I3
				 >
		forceinline static
			Result _Call (const FuncType &func, 
						  const VariantArray1 &varArray1, const VariantArray2 &varArray2, const VariantArray3 &varArray3,
						  CompileTime::IndexSequence<I1...>, CompileTime::IndexSequence<I2...>, CompileTime::IndexSequence<I3...>)
		{
			return _CallImpl( func, varArray1.Get<I1>()..., varArray2.Get<I2>()..., varArray3.Get<I3>()... );
		}
	};

	
/*
=================================================
	InvokeWithVariant
=================================================
*/
	template <typename FuncType, typename ...VariantArrays>
	forceinline typename CompileTime::FunctionInfo< FuncType >::result
		InvokeWithVariant (const FuncType &func, const VariantArrays& ...varArrays)
	{
		using Impl = InvokeWithVariantImpl< 
						typename CompileTime::FunctionInfo< FuncType >::result,
						typename CompileTime::FunctionInfo< FuncType >::args >;

		return Impl::Call( func, varArrays... );
	}

}	// GXTypes
}	// GX_STL
