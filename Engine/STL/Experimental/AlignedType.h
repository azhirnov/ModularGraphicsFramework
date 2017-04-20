// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "TemplateUtils.h"

namespace GX_STL
{
namespace CompileTime
{
	
	namespace _ctime_hidden_
	{
		template <usize Align>
		struct alignas(Align) _TypeWithAlign
		{};

	}	// _ctime_hidden_



	//
	// Aligned Type
	//

	template <usize Size, usize Align>
	struct AlignedType
	{
		union {
			_ctime_hidden_::_TypeWithAlign< Align >	_alignedType;
			char									value[ Size ];
		};
	};


}	// CompileTime
}	// GX_STL
