// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Allocators.h"

namespace GX_STL
{
namespace GXTypes
{
	

	//
	// Memory Container
	//

	template <typename T, typename A = TDefaultAllocator<T> >
	struct MemoryContainer : public CompileTime::FastCopyable
	{
	// types
	public:
		typedef MemoryContainer< T, A >		Self;
		typedef A							Allocator_t;
		typedef T							value_t;


	// variables
	private:
		union {
			T *					_memory;
			TMemoryViewer<T>	_memView;
		};


	// methods
	protected:
		explicit MemoryContainer (const Self &) {}
		
	public:
		MemoryContainer () : _memory( null )
		{}


		MemoryContainer (Self &&other) : _memory( null )
		{
			MoveFrom( other );
		}


		~MemoryContainer ()
		{
			Deallocate();
		}


		Self & operator = (Self &&other)
		{
			MoveFrom( other );
			return *this;
		}


		T *			Pointer ()			{ return _memory; }

		T const *	Pointer ()	const	{ return _memory; }


		bool Allocate (INOUT usize &size, bool allowReserve = true) noexcept
		{
			const usize	required_size	= size;
			const usize	min_size		= GlobalConst::STL_MemContainerResizingMinSize;

			if ( allowReserve )
			{
				const usize	nom	= GlobalConst::STL_MemContainerResizingNominator;
				const usize	den	= GlobalConst::STL_MemContainerResizingDenominator;
				
				size += (size * nom + den - 1) / den + min_size;
			}

			return Allocator_t::Allocate( _memory, size ) and size >= required_size;
		}


		void Deallocate () noexcept
		{
			Allocator_t::Deallocate( _memory );
		}


		usize MaxSize () const
		{
			return usize(-1);
		}
		

		void MoveFrom (INOUT Self &other)
		{
			this->_memory	= other._memory;
			other._memory	= null;
		}


		void SwapMemory (Self &other)
		{
			SwapValues( _memory, other._memory );
		}
	};



	//
	// Static Memory Container
	//

	template <typename T, usize ArraySize>
	struct StaticMemoryContainer : public CompileTime::CopyQualifiers< CompileTime::FastCopyable, T >
	{
		STATIC_ASSERT( CompileTime::IsMemCopyAvailable<T> );
		STATIC_ASSERT( ArraySize * sizeof(T) <= GlobalConst::STL_MemContainerMaxStaticSize );

	// types
	public:
		typedef StaticMemoryContainer< T, ArraySize >	Self;
		typedef T										value_t;

		static const usize	SIZE = ArraySize;


	// variables
	private:
		union {
			ubyte	_buf[ sizeof(T) * SIZE ];
			T		_values[ SIZE ];
		};

		union {
			T *						_memory;
			TMemoryViewer<T, SIZE>	_memView;
		};


	// methods
	private:
		StaticMemoryContainer (const Self &) = delete;
		void operator = (const Self &) = delete;

	public:
		StaticMemoryContainer () :
			_memory((T*) _buf)
		{}
		

		StaticMemoryContainer (Self &&other) :
			_memory((T*) _buf)
		{
			MoveFrom( other );
		}


		~StaticMemoryContainer ()
		{
			Deallocate();
		}


		Self & operator = (Self &&other)
		{
			MoveFrom( other );
			return *this;
		}


		T *			Pointer ()			{ return _memory; }

		T const *	Pointer ()	const	{ return _memory; }


		bool Allocate (INOUT usize &size, bool = false) noexcept
		{
			CHECK( size < ArraySize );

			size = ArraySize;
			return true;
		}


		void Deallocate () noexcept
		{
			DEBUG_ONLY( ZeroMem( _buf ) );
		}


		usize MaxSize () const
		{
			return ArraySize;
		}
		

		void MoveFrom (INOUT Self &other)
		{
			Deallocate();

			UnsafeMem::MemCopy( Pointer(), other.Pointer(), SizeOf(_buf) );
			DEBUG_ONLY( ZeroMem( other._buf ) );
		}


		void SwapMemory (INOUT Self &other)
		{
			TODO( "SwapMemory" );

			ubyte tmp;

			// TODO
			for (usize i = 0; i < sizeof(_buf); ++i)
			{
				SwapValuesWithTemp( this->_buf[i], other._buf[i], tmp );
			}
		}
	};



	//
	// Mixed Memory Container
	//

	template <typename T, usize FixedArraySize, typename A = TDefaultAllocator<T>>
	struct MixedMemoryContainer : public CompileTime::CopyQualifiers< CompileTime::FastCopyable, T >
	{
		STATIC_ASSERT( CompileTime::IsMemCopyAvailable<T> );
		STATIC_ASSERT( FixedArraySize * sizeof(T) <= GlobalConst::STL_MemContainerMaxStaticSize );

	// types
	public:
		typedef MixedMemoryContainer<T,FixedArraySize,A>	Self;
		typedef A											Allocator_t;
		typedef T											value_t;

		static const usize	FIXED_SIZE = FixedArraySize;


	// variables
	private:
		union {
			ubyte	_buf[ sizeof(T) * FIXED_SIZE ];
			T		_values[ FIXED_SIZE ];
		};

		union {
			T *					_memory;
			TMemoryViewer<T>	_memView;
		};


	// methods
	protected:
		MixedMemoryContainer (const Self &) = delete;
		void operator = (const Self &) = delete;

		bool _IsInStaticMemory () const
		{
			return _memory == (T*) _buf;
		}


	public:
		MixedMemoryContainer () : _memory( null )
		{}


		MixedMemoryContainer (Self &&other) : _memory(null)
		{
			MoveFrom( other );
		}


		~MixedMemoryContainer ()
		{
			Deallocate();
		}


		Self & operator = (Self &&other)
		{
			MoveFrom( other );
			return *this;
		}


		T *			Pointer ()			{ return _memory; }

		T const *	Pointer ()	const	{ return _memory; }


		bool Allocate (INOUT usize &size, bool allowReserve = true) noexcept
		{
			const usize	required_size	= size;
			const usize	min_size		= GlobalConst::STL_MemContainerResizingMinSize;

			if ( size > FIXED_SIZE and allowReserve )
			{
				const usize	nom	= GlobalConst::STL_MemContainerResizingNominator;
				const usize	den	= GlobalConst::STL_MemContainerResizingDenominator;
				
				size += (size * nom + den - 1) / den + min_size;
			}

			if ( size <= FIXED_SIZE )
			{
				_memory = _buf;

				return size >= required_size;
			}

			return Allocator_t::Allocate( _memory, size ) and size >= required_size;
		}


		void Deallocate () noexcept
		{
			if ( _IsInStaticMemory() )
			{
				DEBUG_ONLY( ZeroMem( _buf ) );
				_memory = null;
				return;
			}

			Allocator_t::Deallocate( _memory );
		}


		usize MaxSize () const
		{
			return usize(-1);
		}
		

		void MoveFrom (INOUT Self &other)
		{
			Deallocate();

			if ( other._IsInStaticMemory() )
			{
				this->_memory = _buf;

				UnsafeMem::MemCopy( this->_buf, other._buf, sizeof(_buf) );
				DEBUG_ONLY( ZeroMem( other._buf ) );
			}
			else
			{
				this->_memory = other._memory;	
			}

			other._memory = null;
		}


		void SwapMemory (INOUT Self &other)
		{
			const bool	this_in_static_mem	= this->_IsInStaticMemory();
			const bool	other_in_static_mem	= other._IsInStaticMemory();

			if ( not this_in_static_mem and not other_in_static_mem )
			{
				SwapValues( this->_memory, other._memory );
				DEBUG_ONLY( ZeroMem( this->_buf ) );
				DEBUG_ONLY( ZeroMem( other._buf ) );
				return;
			}

			ubyte tmp;

			for (usize i = 0; i < sizeof(_buf); ++i)
			{
				SwapValuesWithTemp( this->_buf[i], other._buf[i], tmp );
			}
			
			if ( this_in_static_mem )
			{
				other._memory = other._buf;
			}

			if ( other_in_static_mem )
			{
				this->_memory = this->_buf;
			}
		}
	};


}	// GXTypes
}	// GX_STL
