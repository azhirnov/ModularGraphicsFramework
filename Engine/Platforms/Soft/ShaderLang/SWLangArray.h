// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Soft/ShaderLang/SWLangCommon.h"

#ifdef GRAPHICS_API_SOFT

# ifdef COMPILER_MSVC
#	pragma warning (push)
#	pragma warning (disable: 4200)
# endif

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
	// variables
	private:
		T	_elements[I];


	// methods
	public:
		SWArray () {}

		SWArray (SWArray &&) = default;
		SWArray (const SWArray &) = default;

		SWArray& operator = (SWArray &&) = default;
		SWArray& operator = (const SWArray &) = default;

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



	//
	// Dynamic Array
	//

	template <typename T>
	struct SWArray< T, 0 > final
	{
	// variables
	private:
		T	_elements[];


	// methods
	public:
		SWArray () {}

		SWArray (SWArray &&) = default;
		SWArray (const SWArray &) = default;

		SWArray& operator = (SWArray &&) = default;
		SWArray& operator = (const SWArray &) = default;

		T &  operator [] (const usize index)
		{
			return _elements[index];
		}

		T const &  operator [] (const usize index) const
		{
			return _elements[index];
		}
	};


}	// Impl
}	// SWShaderLang

# ifdef COMPILER_MSVC
#	pragma warning (pop)
# endif

#endif	// GRAPHICS_API_SOFT
