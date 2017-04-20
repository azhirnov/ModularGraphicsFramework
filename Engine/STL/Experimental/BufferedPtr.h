// Copyright © 2014-2016  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Memory/MemFunc.h"
#include "Noncopyable.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Buffered Pointer
	//

	template <typename T, usize BS = sizeof(T)>
	struct BufferedPtr : public Noncopyable
	{
	// types
	public:
		typedef BufferedPtr< T, BS >		Self;
		typedef T							value_t;
		typedef TDefaultAllocator<void>		Allocator;

		static const usize					BUFFER_SIZE	= CompileTime::Max< usize, BS, sizeof(void *)*2 >;
		

	// variables
	private:
		T *			_ptr;
		union {
			alignas(void*) ubyte	_buf[ BUFFER_SIZE ];	// TODO: align
			void *					_pointers[2];
		};


	// methods
	public:
		BufferedPtr () : _ptr(null)
		{
			ZeroMem( _buf );
		}
		

		~BufferedPtr ()
		{
			_Delete();
		}

		
		operator bool () const
		{
			return IsNotNull();
		}


		T * operator -> () const
		{
			ASSUME( _ptr != null );
			return _ptr;
		}


		T & operator * () const
		{
			ASSUME( _ptr != null );
			return *_ptr;
		}


		T * ptr ()
		{
			ASSUME( _ptr != null );
			return _ptr;
		}


		const T * const ptr () const
		{
			ASSUME( _ptr != null );
			return _ptr;
		}


		bool IsNull () const
		{
			return ( _ptr == null );
		}


		bool IsNotNull () const
		{
			return not IsNull();
		}


		template <typename B>
		Self & Copy (const B &value)
		{
			_SetPtr( PlacementNew<B>( _GetBuffer< sizeof(value) >(), value ) );
			return *this;
		}
		
		template <typename B>
		Self & Move (B &&value)
		{
			_SetPtr( PlacementNew<B>( _GetBuffer< sizeof(value) >(), RVREF( value ) ) );
			return *this;
		}

		template <typename B>
		Self & Set (const B &value)
		{
			return Copy( value );
		}

		template <typename B>
		Self & Set (B &&value)
		{
			return Move( RVREF( value ) );
		}
		
	private:
		template <usize Size>
		void * _GetBuffer ()
		{
			_Delete();

			if ( Size <= BUFFER_SIZE )
				return _buf;
			
			void *	p = null;
			usize	s = Size;

			CHECK( Allocator::Allocate( p, s ) );

			_pointers[0] = this;
			_pointers[1] = p;

			WARNING( "static buffer is too small" );
			return p;
		}


		void _SetPtr (T *ptr)
		{
			ASSERT( _CmpPtr( ptr ) );
			_ptr = ptr;
		}


	private:
		void _Delete ()
		{
			if ( _ptr == null )
				return;

			_ptr->~T();
			_ptr = null;

			if ( _IsUsedDynamicBuffer() )
				Allocator::Deallocate( _pointers[1] );

			ZeroMem( _buf );
		}


		bool _IsUsedDynamicBuffer () const
		{
			return _pointers[0] == this;
		}


		bool _CmpPtr (T *ptr) const
		{
			if ( _IsUsedDynamicBuffer() )
				return ((const void *) _pointers[1]) == ((const void *) ptr);
			else
				return ((const void *) _buf) == ((const void *) ptr);
		}
	};


}	// GXTypes
}	// GX_STL
