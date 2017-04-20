// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/OS/OSLowLevel.h"
#include "Engine/STL/Types/Ptr.h"

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
		static Ptr<T> Instance ()
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
		static T & _Instance ()
		{
			static T inst;
			return inst;
		}


	public:
		template <typename T>
		static Ptr<T> Instance ()
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
		static Ptr<T> Instance ()
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
		static T *& _Instance()
		{
			static thread_local T *	inst = null;
			return inst;
		}


	public:
		template <typename T>
		forceinline static Ptr<T> Instance()
		{
			T *&	ptr = _Instance<T>();
			return ( ptr != null ? ptr : (ptr = new T()) );
		}
	};

# endif	// GX_THREADSAFE_STATIC_SUPPORTED


}	// GXTypes
}	// GX_STL
