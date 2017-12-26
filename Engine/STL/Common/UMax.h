// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Types.h"

namespace GX_STL
{
namespace GXTypes
{

	namespace _types_hidden_
	{
		struct _UMax
		{
			template <typename T>
			constexpr operator T () const
			{
				STATIC_ASSERT( ~T(0) > T(0) );
				return T(~T(0));
			}
			
			template <typename T>
			friend constexpr bool operator == (const T& left, const _UMax &right)
			{
				return T(right) == left;
			}
			
			template <typename T>
			friend constexpr bool operator != (const T& left, const _UMax &right)
			{
				return T(right) != left;
			}
		};
	}

	static constexpr _types_hidden_::_UMax	UMax{};

}	// GXTypes
}	// GX_STL
