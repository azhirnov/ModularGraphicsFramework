// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Soft/ShaderLang/SWLangCommon.h"

#ifdef GRAPHICS_API_SOFT

namespace SWShaderLang
{
namespace Impl
{

	//
	// Shared Memory
	//
	template <typename T>
	struct SharedMemory final
	{
		friend class SWShaderHelper;

	// variables
	private:
		Ptr<BinaryArray>	_data;
		usize				_count	= 0;


	// methods
	private:
		explicit SharedMemory (BinaryArray &data, usize count);
		
		SharedMemory& operator = (const SharedMemory &) = default;
		SharedMemory& operator = (SharedMemory &&) = default;

	public:
		SharedMemory () {}
		SharedMemory (SharedMemory &&) = default;
		SharedMemory (const SharedMemory &) = default;

		T const* operator -> () const;
		T *		 operator -> ();

		T const& operator [] (usize i) const;
		T &		 operator [] (usize i);
	};



	template <typename T>
	inline SharedMemory<T>::SharedMemory (BinaryArray &data, usize count) : _data(&data), _count(count)
	{
		ASSERT( _data->Size() == (SizeOf<T> * count) );
	}

	template <typename T>
	inline T const*  SharedMemory<T>::operator -> () const
	{
		ASSERT( _count == 1 );
		return (T const *) _data->ptr();
	}
	
	template <typename T>
	inline T *  SharedMemory<T>::operator -> ()
	{
		ASSERT( _count == 1 );
		return (T *) _data->ptr();
	}

	template <typename T>
	inline T const&  SharedMemory<T>::operator [] (usize i) const
	{
		ASSERT( i < _count );
		return *(T const*)(_data->ptr() + (SizeOf<T> * i));
	}
	
	template <typename T>
	inline T&  SharedMemory<T>::operator [] (usize i)
	{
		ASSERT( i < _count );
		return *(T *)(_data->ptr() + (SizeOf<T> * i));
	}
//-----------------------------------------------------------------------------


}	// Impl
}	// SWShaderLang

#endif	// GRAPHICS_API_SOFT
