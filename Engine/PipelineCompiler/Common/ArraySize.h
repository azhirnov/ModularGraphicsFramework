// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/PipelineCompiler/Common/Common.h"

namespace PipelineCompiler
{

	//
	// Array Size
	//

	struct ArraySize final
	{
	// variables
	private:
		uint	_size	= 0;		// 0 - not array, > 0 - static array, ~0 - dynamic


	// methods
	public:
		ArraySize () {}

		ArraySize& MakeDynamic ()						{ _size = UMax;  return *this; }
		ArraySize& MakeStatic (uint size)				{ _size = Max( size, 1u );  return *this; }
		ArraySize& MakeNonArray ()						{ _size = 0;  return *this; }

		bool IsNotArray ()		const					{ return _size == 0; }
		bool IsArray ()			const					{ return not IsNotArray(); }
		bool IsDynamicArray ()	const					{ return _size == UMax; }
		bool IsStaticArray ()	const					{ return not (IsNotArray() or IsDynamicArray()); }

		uint Size ()			const					{ return IsNotArray() ? 1 : (IsDynamicArray() ? 0 : _size); }

		bool operator == (const ArraySize &right) const	{ return _size == right._size; }
		bool operator != (const ArraySize &right) const	{ return not (*this == right); }
		bool operator >  (const ArraySize &right) const	{ return _size > right._size; }
	};

}	// PipelineCompiler


namespace GX_STL::GXTypes
{
	template <>
	struct Hash< PipelineCompiler::ArraySize >
	{
		ND_ HashResult  operator () (const PipelineCompiler::ArraySize &x) const noexcept
		{
			return HashOf( x.IsNotArray() ? 0 : x.IsDynamicArray() ? UMax : x.Size() );
		}
	};

}	// GX_STL::GXTypes
