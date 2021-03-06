// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Types/Function.h"
#include "Core/STL/Types/Delegate.h"
#include "Core/STL/Types/VariantRef.h"
#include "Core/STL/Types/VariantCRef.h"
#include "Core/STL/CompileTime/Sequence.h"
#include "Core/STL/CompileTime/TypeTraits.h"
#include "Core/STL/CompileTime/CompileTimeDebug.h"
#include "Core/STL/CompileTime/FunctionInfo.h"
#include "Core/STL/Algorithms/ArrayUtils.h"

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
			STATIC_ASSERT( Args::template Equal< ArgsTypeList > );
			
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
			return _CallImpl( func, varArray1.template Get<I1>()... );
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
			return _CallImpl( func, varArray1.template Get<I1>()..., varArray2.template Get<I2>()... );
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
			return _CallImpl( func, varArray1.template Get<I1>()..., varArray2.template Get<I2>()..., varArray3.template Get<I3>()... );
		}
	};

	
/*
=================================================
	InvokeWithVariant
=================================================
*/
	template <typename FuncType, typename ...VariantArrays>
	forceinline typename CompileTime::FunctionInfo< FuncType >::result
		InvokeWithVariant (const FuncType &func, VariantArrays&& ...varArrays)
	{
		using Impl = InvokeWithVariantImpl< 
						typename CompileTime::FunctionInfo< FuncType >::result,
						typename CompileTime::FunctionInfo< FuncType >::args >;

		return Impl::Call( func, FW<VariantArrays>(varArrays)... );
	}

}	// GXTypes
}	// GX_STL
