// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	StaticRefCountedObject class may be created as static or
	dynamic object and cast to shared pointer always available and
	safe in most cases.
*/

#pragma once

#include "RefCountedObject.h"

namespace GX_STL
{
namespace GXTypes
{

	template <typename T, typename ...Args>
	forceinline SharedPointerType<T> New (Args ...args) noexcept;


	
	//
	// Static Reference Counted Object
	//

	class StaticRefCountedObject : public RefCountedObject<>
	{
	// types
	private:
		using Self				= StaticRefCountedObject;
		using SelfPtr_t			= SharedPointerType< Self >;
		using Base_t			= RefCountedObject<>;
		using Strategy_t		= SharedPointerStrategy< Base_t >;
		using WeakStrategy_t	= WeakPointerStrategy< Base_t >;


	// variables
	private:
		bool	_isDynamicObj;


	// methods
	public:
		StaticRefCountedObject () : _isDynamicObj(true)
		{
			_SetDynamicObj( false );
		}
		

		explicit StaticRefCountedObject (const Self &other) :
			Base_t(other),
			_isDynamicObj(true)
		{
			_SetDynamicObj( false );
		}


		explicit StaticRefCountedObject (Self &&other) :
			Base_t( other ),
			_isDynamicObj(true)
		{
			_SetDynamicObj( false );
		}


		~StaticRefCountedObject () override
		{
			if ( not _isDynamicObj )
			{
				ASSERT( Strategy_t::Count( this ) == 1 );

				// free weak pointer
				WeakStrategy_t::Counter_t cnt{ WeakStrategy_t::Create( this ) };

				_GetCounter().Dec();
				WeakStrategy_t::DecRef( cnt );
			}
			else
			{
				ASSERT( Strategy_t::Count( this ) == 0 );
			}
		}

		
		// use New<T>( T(args) ) for private/protected constructors
		template <typename T, typename ...Args>
		ND_ friend forceinline SharedPointerType<T>  New (Args ...args) noexcept
		{
			SharedPointerType<T> tmp = new T( FW<Args>(args)... );
			tmp->_SetDynamicObj( true );
			return tmp;
		}

		// can't use delete for shared pointer (but GCC can't compile this as private...)
		forceinline void operator delete (void* p) noexcept
		{
			return ::operator delete( p );
		}


	private:
		// use 'New' function
		forceinline void * operator new (size_t size) noexcept
		{
			return ::operator new( size );
		}

		forceinline void _SetDynamicObj (bool isDynamic)
		{
			if ( _isDynamicObj == isDynamic )
				return;

			_isDynamicObj = isDynamic;

			if ( not _isDynamicObj )
			{
				Strategy_t::IncRef( this );
			}
			else
			{
				Strategy_t::DecRef( this );
			}
		}


	// RefCountedObject //
	protected:
		void _Release (RefCounter_t &) override
		{
			ASSERT( _isDynamicObj );

			if ( _isDynamicObj )
			{
				delete this;
			}
		}
	};


}	// GXTypes
}	// GX_STL
