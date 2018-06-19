// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/CompileTime/TemplateUtils.h"

namespace GX_STL
{
namespace CompileTime
{

	//
	// Deferred Template Type
	//

	/*
		example:

			template <typename T> struct Templ {};

			// switch type without template instantiation
			using D = SwitchType< condition, DeferredTemplate< Templ, T1 >, DeferredTemplate< Templ, T2 > >;

			// instantiate only 1 template
			using T = D::type;
	*/

	template <template <typename ...> class Templ, typename ...Types>
	struct DeferredTemplate
	{
		using type	= Templ< Types... >;
	};


	template <typename Left, typename Right>
	struct DeferredAnd
	{
		static const bool	value = Left::type::value and Right::type::value;
	};


}	// CompileTime
}	// GX_STL
