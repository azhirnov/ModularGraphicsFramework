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
		ArrayRef<T>			_view;
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

		ND_ T const* operator -> () const;
		ND_ T *		 operator -> ();

		ND_ T const& operator [] (usize i) const;
		ND_ T &		 operator [] (usize i);
	};



	template <typename T>
	inline SharedMemory<T>::SharedMemory (BinaryArray &data, usize count) :
		_data{ &data }, _view{ ArrayRef<T>::From( data ) }, _count{ count }
	{
		ASSERT( _data->Size() == (SizeOf<T> * count) );
	}

	template <typename T>
	inline T const*  SharedMemory<T>::operator -> () const
	{
		ASSERT( _count == 1 );
		return _view.ptr();
	}
	
	template <typename T>
	inline T *  SharedMemory<T>::operator -> ()
	{
		ASSERT( _count == 1 );
		return _view.ptr();
	}

	template <typename T>
	inline T const&  SharedMemory<T>::operator [] (usize i) const
	{
		return _view[i];
	}
	
	template <typename T>
	inline T&  SharedMemory<T>::operator [] (usize i)
	{
		return _view[i];
	}
//-----------------------------------------------------------------------------


}	// Impl
}	// SWShaderLang

#endif	// GRAPHICS_API_SOFT
