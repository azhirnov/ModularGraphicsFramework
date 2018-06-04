// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Soft/ShaderLang/SWLangCommon.h"

#ifdef GRAPHICS_API_SOFT

namespace SWShaderLang
{
namespace Impl
{

	//
	// Uniform Buffer
	//
	template <typename T>
	struct UniformBuffer final
	{
		friend class SWShaderHelper;
		
	// types
	private:
		using BufferData_t	= Engine::GpuMsg::GetSWBufferMemoryLayout::Data;


	// variables
	private:
		BufferData_t	_data;
		T *				_ptr	= null;


	// methods
	private:
		explicit UniformBuffer (const BufferData_t &data);
		
		UniformBuffer& operator = (const UniformBuffer &) = default;
		UniformBuffer& operator = (UniformBuffer &&) = default;

	public:
		UniformBuffer () {}
		UniformBuffer (UniformBuffer &&) = default;
		UniformBuffer (const UniformBuffer &) = default;

		ND_ T const* operator -> () const;
	};

	
	template <typename T>
	UniformBuffer<T>::UniformBuffer (const BufferData_t &data) :
		_data{ data },
		_ptr{ reinterpret_cast<T *>( _data.memory.RawPtr() ) }
	{
		const BytesU	t_size = BytesU::SizeOf<T>();

		ASSERT( _data.memory.Size() >= t_size );
	}

	template <typename T>
	inline T const*  UniformBuffer<T>::operator -> () const
	{
		ASSERT( _data.memAccess[ EMemoryAccess::GpuRead ] );
		return _ptr;
	}
//-----------------------------------------------------------------------------



	//
	// Storage Buffer
	//
	template <	typename T,
				EStorageAccess::type Access
			 >
	struct StorageBuffer
	{
		friend class SWShaderHelper;
		
	// types
	private:
		using BufferData_t	= Engine::GpuMsg::GetSWBufferMemoryLayout::Data;


	// variables
	private:
		BufferData_t	_data;
		T *				_ptr	= null;


	// methods
	private:
		explicit StorageBuffer (const BufferData_t &data);

		StorageBuffer& operator = (const StorageBuffer &) = default;
		StorageBuffer& operator = (StorageBuffer &&) = default;

		T const *	_Read ();
		T *			_Write ();

	public:
		StorageBuffer () {}
		StorageBuffer (StorageBuffer &&) = default;
		StorageBuffer (const StorageBuffer &) = default;

		ND_ decltype(auto)	operator -> ();
	};

	
	
	template <typename T, EStorageAccess::type A>
	StorageBuffer<T,A>::StorageBuffer (const BufferData_t &data) :
		_data{ data },
		_ptr{ reinterpret_cast<T *>( _data.memory.RawPtr() ) }
	{
		const BytesU	t_size = BytesU::SizeOf<T>();

		ASSERT( _data.memory.Size() >= t_size );
	}

	template <typename T, EStorageAccess::type A>
	inline decltype(auto)  StorageBuffer<T,A>::operator -> ()
	{
		if_constexpr( EStorageAccess::HasWriteAccess(A) )
			return _Write();
		else
			return _Read();
	}
	
	template <typename T, EStorageAccess::type A>
	inline T const *  StorageBuffer<T,A>::_Read ()
	{
		STATIC_ASSERT( EStorageAccess::HasReadAccess(A) );
		ASSERT( _data.memAccess[ EMemoryAccess::GpuRead ] );
		return _ptr;
	}
	
	template <typename T, EStorageAccess::type A>
	inline T *  StorageBuffer<T,A>::_Write ()
	{
		STATIC_ASSERT( EStorageAccess::HasWriteAccess(A) );
		ASSERT( _data.memAccess[ EMemoryAccess::GpuWrite ] );
		return _ptr;
	}
//-----------------------------------------------------------------------------


}	// Impl
}	// SWShaderLang

#endif	// GRAPHICS_API_SOFT
