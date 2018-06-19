// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/OS/OSLowLevel.h"
#include "Core/STL/Types/Ptr.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Singleton for multi threading
	//

# ifdef GX_THREADSAFE_STATIC_SUPPORTED
	
	struct SingletonMultiThread
	{
	public:
		template <typename T>
		ND_ static Ptr<T> Instance () noexcept
		{
			static T inst;
			return &inst;
		}
	};

# else

	struct SingletonMultiThread
	{
	protected:
		static OS::Mutex	_mutex;

		template <typename T>
		static T & _Instance () noexcept
		{
			static T inst;
			return inst;
		}


	public:
		template <typename T>
		ND_ static Ptr<T> Instance () noexcept
		{
			static T * volatile	inst;

			if ( inst == null )
			{
				T *	temp = null;
				{
					SCOPELOCK( _mutex );
					temp = & _Instance<T>();
				}
				inst = temp;
			}
			return inst;
		}
	};

# endif	// GX_THREADSAFE_STATIC_SUPPORTED



	//
	// Singleton for single thread (one instance per thread)
	//

# ifdef GX_THREADSAFE_STATIC_SUPPORTED
	
	struct SingletonSingleThread
	{
	public:
		template <typename T>
		ND_ static Ptr<T> Instance () noexcept
		{
			static thread_local T  inst;
			return &inst;
		}
	};

# else

	struct SingletonSingleThread
	{
	protected:
		template <typename T>
		static T *& _Instance() noexcept
		{
			static thread_local T *	inst = null;
			return inst;
		}


	public:
		template <typename T>
		ND_ forceinline static Ptr<T> Instance() noexcept
		{
			T *&	ptr = _Instance<T>();
			return ( ptr != null ? ptr : (ptr = new T()) );
		}
	};

# endif	// GX_THREADSAFE_STATIC_SUPPORTED


}	// GXTypes
}	// GX_STL
