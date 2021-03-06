// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Memory/Allocators.h"
#include "Core/STL/Algorithms/Swap.h"

namespace GX_STL
{
namespace GXTypes
{
	

	//
	// Memory Container
	//

	template <typename T>
	struct MemoryContainer : public CompileTime::FastCopyable
	{
	// types
	public:
		using Self			= MemoryContainer< T >;
		using Allocator_t	= TDefaultAllocator<void>;
		using Value_t		= T;


	// variables
	private:
		union {
			void *				_memory;
			T *					_memPtr;
			TMemoryViewer<T>	_memView;
		};


	// methods
	public:
		MemoryContainer () : _memory( null )
		{}


		MemoryContainer (Self &&other) : _memory( null )
		{
			MoveFrom( other );
		}


		MemoryContainer (const Self &) = delete;


		~MemoryContainer ()
		{
			Deallocate();
		}


		Self & operator = (Self &&other)
		{
			MoveFrom( other );
			return *this;
		}


		T *				Pointer ()			{ return ReferenceCast<T *>(_Aligned()); }
		T const *		Pointer ()	const	{ return ReferenceCast<T const*>(_Aligned()); }

		constexpr bool	IsStatic ()	const	{ return false; }


		usize _Aligned () const
		{
			STATIC_ASSERT(( CompileTime::IsPowerOfTwo< uint, alignof(T) > ));

			if_constexpr( alignof(T) < sizeof(void*) )
				return ReferenceCast<usize>(_memory);
			else
				return (ReferenceCast<usize>(_memory) + alignof(T)-1) & ~(alignof(T)-1);
		}


		bool Allocate (INOUT usize &size, bool allowReserve = true) noexcept
		{
			const usize	required_size	= size * sizeof(T);
			const usize	min_size		= GlobalConst::STL_MemContainerResizingMinSize;

			if ( allowReserve )
			{
				const usize	nom	= GlobalConst::STL_MemContainerResizingNominator;
				const usize	den	= GlobalConst::STL_MemContainerResizingDenominator;
				
				size += (size * nom + den - 1) / den + min_size;
			}

			usize size2 = size * sizeof(T) + alignof(T);

			if ( not (Allocator_t::Allocate( INOUT _memory, INOUT size2 ) and size2 >= required_size) )
				return false;

			DEBUG_ONLY( UnsafeMem::ZeroMem( _memory, BytesU(size2) ) );

			size2 -= (_Aligned() - ReferenceCast<usize>(_memory));
			size = size2 / sizeof(T);
			return true;
		}


		void Deallocate () noexcept
		{
			Allocator_t::Deallocate( _memory );
		}


		static constexpr usize MaxSize ()
		{
			return UMax;
		}
		

		void MoveFrom (INOUT Self &other) noexcept
		{
			Deallocate();

			this->_memory	= other._memory;
			other._memory	= null;
		}


		void SwapMemory (Self &other) noexcept
		{
			SwapValues( _memory, other._memory );
		}
	};
	


	//
	// Preallocated Memory Container
	//

	template <typename T>
	struct PreallocatedMemoryContainer : public CompileTime::FastCopyable
	{
	// types
	public:
		using Self			= PreallocatedMemoryContainer< T >;
		using Allocator_t	= TDefaultAllocator<void>;
		using Value_t		= T;


	// variables
	private:
		union {
			void *				_memory;
			T *					_memPtr;
			TMemoryViewer<T>	_memView;
		};


	// methods
	public:
		PreallocatedMemoryContainer () : _memory( null )
		{}


		PreallocatedMemoryContainer (Self &&other) : _memory( null )
		{
			MoveFrom( other );
		}


		PreallocatedMemoryContainer (const Self &) = delete;


		~PreallocatedMemoryContainer ()
		{
			Deallocate();
		}


		Self & operator = (Self &&other)
		{
			MoveFrom( other );
			return *this;
		}


		T *				Pointer ()			{ return ReferenceCast<T *>(_Aligned()); }
		T const *		Pointer ()	const	{ return ReferenceCast<T const*>(_Aligned()); }

		constexpr bool	IsStatic ()	const	{ return false; }


		usize _Aligned () const
		{
			STATIC_ASSERT(( CompileTime::IsPowerOfTwo< uint, alignof(T) > ));

			if_constexpr( alignof(T) < sizeof(void*) )
				return ReferenceCast<usize>(_memory);
			else
				return (ReferenceCast<usize>(_memory) + alignof(T)-1) & ~(alignof(T)-1);
		}


		bool Allocate (INOUT usize &size, bool = true) noexcept
		{
			usize size2 = GlobalConst::STL_MemContainerPreallocSize;

			if ( not Allocator_t::Allocate( INOUT _memory, INOUT size2 ) )
				return false;

			DEBUG_ONLY( UnsafeMem::ZeroMem( _memory, BytesU(size2) ) );

			size2 -= (_Aligned() - ReferenceCast<usize>(_memory));
			size = size2 / sizeof(T);
			return true;
		}


		void Deallocate () noexcept
		{
			Allocator_t::Deallocate( _memory );
		}


		static constexpr usize MaxSize ()
		{
			return GlobalConst::STL_MemContainerPreallocSize / sizeof(T);
		}
		

		void MoveFrom (INOUT Self &other) noexcept
		{
			Deallocate();

			this->_memory	= other._memory;
			other._memory	= null;
		}


		void SwapMemory (Self &other) noexcept
		{
			SwapValues( _memory, other._memory );
		}
	};



	//
	// Static Memory Container
	//

	template <typename T, usize ArraySize>
	struct alignas(T) StaticMemoryContainer : public CompileTime::CopyQualifiers< CompileTime::FastCopyable, T >
	{
		STATIC_ASSERT( ArraySize > 0 );
		STATIC_ASSERT( CompileTime::IsMemCopyAvailable<T> );
		STATIC_ASSERT( ArraySize * sizeof(T) <= GlobalConst::STL_MemContainerMaxStaticSize );

	// types
	public:
		using Self		= StaticMemoryContainer< T, ArraySize >;
		using Value_t	= T;

		static const usize	SIZE = ArraySize;


	// variables
	private:
		union {
			ubyte	_storage[ sizeof(T) * SIZE ];
			T		_values[ SIZE ];
		};


	// methods
	public:
		StaticMemoryContainer ()
		{}
		

		StaticMemoryContainer (Self &&other)
		{
			MoveFrom( other );
		}


		~StaticMemoryContainer ()
		{
			Deallocate();
		}


		StaticMemoryContainer (const Self &) = delete;

		void operator = (const Self &) = delete;


		Self & operator = (Self &&other)
		{
			MoveFrom( other );
			return *this;
		}


		T *				Pointer ()			{ return _values; }
		T const *		Pointer ()	const	{ return _values; }

		constexpr bool	IsStatic ()	const	{ return true; }


		bool Allocate (INOUT usize &size, bool = false) noexcept
		{
			ASSERT( size <= ArraySize );

			size = ArraySize;
			return true;
		}


		void Deallocate () noexcept
		{
			DEBUG_ONLY( ZeroMem( _storage ) );
		}


		static constexpr usize MaxSize ()
		{
			return ArraySize;
		}
		

		void MoveFrom (INOUT Self &other) noexcept
		{
			Deallocate();

			UnsafeMem::MemCopy( Pointer(), other.Pointer(), BytesU::SizeOf(_storage) );
			DEBUG_ONLY( ZeroMem( other._storage ) );
		}


		void SwapMemory (INOUT Self &other) noexcept
		{
			TODO( "SwapMemory" );

			// TODO: optimize
			for (usize i = 0; i < sizeof(_storage); ++i)
			{
				SwapValues( this->_storage[i], other._storage[i] );
			}
		}
	};



	//
	// Mixed Memory Container
	//

	template <typename T, usize FixedArraySize>
	struct alignas(T) MixedMemoryContainer : public CompileTime::CopyQualifiers< CompileTime::FastCopyable, T >
	{
		STATIC_ASSERT( FixedArraySize > 0 );
		STATIC_ASSERT( CompileTime::IsMemCopyAvailable<T> );
		STATIC_ASSERT( FixedArraySize * sizeof(T) <= GlobalConst::STL_MemContainerMaxStaticSize );

	// types
	public:
		using Self			= MixedMemoryContainer<T,FixedArraySize>;
		using Allocator_t	= TDefaultAllocator<void>;
		using Value_t		= T;
		
		static const usize	FIXED_SIZE = FixedArraySize;


	// variables
	private:
		union {
			ubyte	_storage[ sizeof(T) * FIXED_SIZE ];
			T		_values[ FIXED_SIZE ];
		};

		union {
			void *				_memory;
			T *					_memPtr;
			TMemoryViewer<T>	_memView;
		};


	// methods
	protected:
		bool _IsInStaticMemory () const
		{
			return _memory == Cast<T const*>(_storage);
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


		MixedMemoryContainer (const Self &) = delete;

		void operator = (const Self &) = delete;


		Self & operator = (Self &&other)
		{
			MoveFrom( other );
			return *this;
		}

		
		T *				Pointer ()			{ return ReferenceCast<T *>(_Aligned()); }
		T const *		Pointer ()	const	{ return ReferenceCast<T const*>(_Aligned()); }

		constexpr bool	IsStatic ()	const	{ return false; }
		

		usize _Aligned () const
		{
			STATIC_ASSERT(( CompileTime::IsPowerOfTwo< uint, alignof(T) > ));

			if_constexpr( alignof(T) < sizeof(void*) )
				return ReferenceCast<usize>(_memory);
			else
				return (ReferenceCast<usize>(_memory) + alignof(T)-1) & ~(alignof(T)-1);
		}


		bool Allocate (INOUT usize &size, bool allowReserve = true) noexcept
		{
			const usize	required_size	= size * sizeof(T);
			const usize	min_size		= GlobalConst::STL_MemContainerResizingMinSize;

			if ( size <= FIXED_SIZE )
			{
				_memory = _storage;
				size	= FIXED_SIZE;

				return true;
			}

			if ( allowReserve )
			{
				const usize	nom	= GlobalConst::STL_MemContainerResizingNominator;
				const usize	den	= GlobalConst::STL_MemContainerResizingDenominator;
				
				size += (size * nom + den - 1) / den + min_size;
			}

			usize size2 = size * sizeof(T) + alignof(T);

			if ( not (Allocator_t::Allocate( INOUT _memory, INOUT size2 ) and size2 >= required_size) )
				return false;

			DEBUG_ONLY( UnsafeMem::ZeroMem( _memory, BytesU(size2) ) );

			size2 -= (_Aligned() - ReferenceCast<usize>(_memory));
			size = size2 / sizeof(T);
			return true;
		}


		void Deallocate () noexcept
		{
			if ( _IsInStaticMemory() )
			{
				DEBUG_ONLY( ZeroMem( _storage ) );
				_memory = null;
				return;
			}

			Allocator_t::Deallocate( _memory );
		}


		static constexpr usize MaxSize ()
		{
			return UMax;
		}
		

		void MoveFrom (INOUT Self &other) noexcept
		{
			Deallocate();

			if ( other._IsInStaticMemory() )
			{
				this->_memory = _storage;

				UnsafeMem::MemCopy( this->_storage, other._storage, BytesU::SizeOf(_storage) );
				DEBUG_ONLY( ZeroMem( other._storage ) );
			}
			else
			{
				this->_memory = other._memory;	
			}

			other._memory = null;
		}


		void SwapMemory (INOUT Self &other) noexcept
		{
			const bool	this_in_static_mem	= this->_IsInStaticMemory();
			const bool	other_in_static_mem	= other._IsInStaticMemory();

			if ( not this_in_static_mem and not other_in_static_mem )
			{
				SwapValues( this->_memory, other._memory );
				DEBUG_ONLY( ZeroMem( this->_storage ) );
				DEBUG_ONLY( ZeroMem( other._storage ) );
				return;
			}

			// TODO: optimize
			for (usize i = 0; i < sizeof(_storage); ++i)
			{
				SwapValues( this->_storage[i], other._storage[i] );
			}
			
			if ( this_in_static_mem )
			{
				other._memory = other._storage;
			}

			if ( other_in_static_mem )
			{
				this->_memory = this->_storage;
			}
		}
	};


}	// GXTypes
}	// GX_STL
