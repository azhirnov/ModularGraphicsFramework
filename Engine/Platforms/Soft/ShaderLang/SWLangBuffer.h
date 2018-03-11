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


	// methods
	private:
		explicit UniformBuffer (const BufferData_t &data);
		
		UniformBuffer& operator = (const UniformBuffer &) = default;
		UniformBuffer& operator = (UniformBuffer &&) = default;

	public:
		UniformBuffer () {}
		UniformBuffer (UniformBuffer &&) = default;
		UniformBuffer (const UniformBuffer &) = default;

		T const* operator -> () const;
	};

	
	template <typename T>
	UniformBuffer<T>::UniformBuffer (const BufferData_t &data) : _data(data)
	{
		ASSERT( _data.memory.Size() == SizeOf<T>() );
	}

	template <typename T>
	inline T const*  UniformBuffer<T>::operator -> () const
	{
		ASSERT( _data.memAccess[ EMemoryAccess::GpuRead ] );
		return (T const *) _data.memory.ptr();
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

		decltype(auto)	operator -> ();
	};

	
	
	template <typename T, EStorageAccess::type A>
	StorageBuffer<T,A>::StorageBuffer (const BufferData_t &data) : _data(data)
	{
		ASSERT( _data.memory.Size() >= SizeOf<T> );
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
		return (T const *) _data.memory.ptr();
	}
	
	template <typename T, EStorageAccess::type A>
	inline T *  StorageBuffer<T,A>::_Write ()
	{
		STATIC_ASSERT( EStorageAccess::HasWriteAccess(A) );
		ASSERT( _data.memAccess[ EMemoryAccess::GpuWrite ] );
		return (T *) _data.memory.ptr();
	}
//-----------------------------------------------------------------------------


}	// Impl
}	// SWShaderLang

#endif	// GRAPHICS_API_SOFT
