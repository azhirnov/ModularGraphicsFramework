// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/CompileTime/TypeListHelpers.h"
#include "Core/Physics/Dimensions/PhysicsValue.h"

namespace GXPhysics
{

	namespace _phys_hidden_
	{
		template <usize, typename Type, typename PrevFuncResult, typename>
		struct _TypeList_GetInnerTypeFromPhysicsValue
		{
			using result = CompileTime::TypeList< typename Type::Value_t, PrevFuncResult >;
		};
		
		template <typename Typelist>
		using InnerTypes_t = typename Typelist::template ForEach< _TypeList_GetInnerTypeFromPhysicsValue, CompileTime::TypeListEnd >;
		
		template <typename Typelist>
		using GenTypeFromInner_t = typename InnerTypes_t< Typelist >::template ForEach< CompileTime::_ctime_hidden_::GenType_Choose<false>::Func, float >;

	}	// _phys_hidden_


	template <typename ...Types>
	using PXGenType = typename _phys_hidden_::GenTypeFromInner_t< CompileTime::TypeListFrom< Types... > >;


}	// GXPhysics
