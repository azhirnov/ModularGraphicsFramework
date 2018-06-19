// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Types/Ptr.h"
#include "Core/STL/Types/SharedPointer.h"
#include "Core/STL/Types/WeakPointer.h"

namespace GX_STL
{
namespace GXTypes
{

	namespace _types_hidden_
	{

		template <typename T>
		struct _UntypedKey;

		template <typename T>		struct _UntypedKey< T* >			{ using type = void const *; };
		template <typename T>		struct _UntypedKey< T const * >	{ using type = void const *; };

		template <typename T>		struct _UntypedKey< Ptr<T> >		{ using type = void const *; };
		
		template <typename T, typename B, typename S>					struct _UntypedKey< SharedPointer<T,B,S> >		{ using type = void const *; };
		
		template <typename T, typename B, typename WS, typename SPS>	struct _UntypedKey< WeakPointer<T,B,WS,SPS> >	{ using type = void const *; };

	}	// _types_hidden_


	template <typename T>
	using UntypedKey  = typename _types_hidden_::_UntypedKey< T >::type;


}	// GXTypes
}	// GX_STL
