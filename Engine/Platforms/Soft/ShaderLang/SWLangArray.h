// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Soft/ShaderLang/SWLangCommon.h"

#ifdef GRAPHICS_API_SOFT

namespace SWShaderLang
{
namespace Impl
{

	//
	// Static Array
	//

	template <typename T, usize I>
	struct SWArray final
	{
		STATIC_ASSERT( I > 0 );

	// variables
	private:
		T	_elements[I];


	// methods
	public:
		SWArray () {}

		GX_DEFCOPYCTOR_ASSIGN( SWArray );

		T &  operator [] (const usize index)
		{
			ASSERT( index < I );
			return _elements[index];
		}

		T const &  operator [] (const usize index) const
		{
			ASSERT( index < I );
			return _elements[index];
		}

		bool operator == (const SWArray<T,I> &right) const
		{
			FOR( i, _elements ) {
				if ( not glm::all( _elements[i] == right._elements[i] ) )
					return false;
			}
			return true;
		}

		bool operator != (const SWArray<T,I> &right) const {
			return not (*this == right);
		}
	};

}	// Impl
}	// SWShaderLang

#endif	// GRAPHICS_API_SOFT
