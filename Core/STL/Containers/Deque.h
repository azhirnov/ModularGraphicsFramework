// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Containers/Array.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Deque (Chunk Array)
	//

	template <	typename T,
				typename S = typename AutoDetectCopyStrategy<T>::type,
				typename MC = MemoryContainer<T>,
				template <typename, typename, typename> class Container = Array,
				usize DefaultChunkSize = 4096
			 >
	struct Deque final
	{
	// types
	public:
		using Self				= Deque< T, S, MC, Container, DefaultChunkSize >;
		using Strategy_t		= S;
		using MemoryContainer_t	= MC;
		
		friend struct Hash< Self >;

	private:
		//
		// Chunk (Ring buffer)
		//
		struct Chunk final : public CompileTime::CopyQualifiers< CompileTime::FastCopyable, MC >
		{
		// variables
		public:
			MemoryContainer_t	memory;
			usize				first	= 0;
			usize				last	= UMax;
			usize				size	= 0;

		// methods
		public:
			Chunk () {}
			Chunk (Chunk &&) = default;
			Chunk (const Chunk &) = delete;
			~Chunk ()											{ Free(); }

				void		Free ();
			ND_ usize		Count ()					const;
			ND_ usize		Distance (usize i, usize j)	const;
			ND_ usize		Available ()				const	{ return size - Count(); }
			ND_ usize		Wrap (isize i)				const;
			ND_ T &			Get (usize i)						{ return memory.Pointer()[ Wrap( first + i ) ]; }
			ND_ T const&	Get (usize i)				const	{ return memory.Pointer()[ Wrap( first + i ) ]; }
		};

		using Chunks_t	= Container< Chunk, typename AutoDetectCopyStrategy<Chunk>::type, MemoryContainer<Chunk> >;


		//
		// Iterator
		//
		template <bool IsConst>
		struct Iterator final
		{
			friend struct Deque;

		// types
		private:
			using Self			= Iterator< IsConst >;
			using ChunkIter_t	= CompileTime::SwitchType< IsConst, typename Chunks_t::const_iterator, typename Chunks_t::iterator >;
			using Val			= CompileTime::SwitchType< IsConst, const T, T >;

		// variables
		private:
			ChunkIter_t		_chunk;
			usize			_index = 0;

		// methods
		private:
			explicit Iterator (ChunkIter_t chunk, usize index = 0) : _chunk{ chunk }, _index{ index } {}

		public:
			Iterator (const Self &) = default;
			Iterator (Self &&) = default;
			
			Self &	operator = (const Self &) = default;
			Self &	operator = (Self &&) = default;

			Self &	operator ++ ();
			Self &	operator -- ();

			ND_ bool		operator == (const Self &right) const	{ return _chunk == right._chunk and _index == right._index; }
			ND_ bool		operator != (const Self &right) const	{ return not (*this == right); }

			ND_ Val &		operator * ()							{ return _chunk->Get( _index ); }
			ND_ Val const&	operator * ()	const					{ return _chunk->Get( _index ); }
			
			ND_ Val *		operator -> ()							{ return &operator *(); }
			ND_ Val const*	operator -> ()	const					{ return &operator *(); }
		};
		
	public:
		using iterator			= Iterator< false >;
		using const_iterator	= Iterator< true >;


	// variables
	private:
		Chunks_t	_chunks;
		//usize		_totalCount		= 0;
		usize		_chunkSize		= 0;

		//    chunk_0                        chunk_1                       chunk_2
		//  ___________      ______________________________________      ___________
		// |  .......  | -> | used memory | reserved | used memory | -> |  .......  | -> ...
		//                  0           last+1      first        size


	// methods
	private:
		//template <typename B>	void _Insert (B *data, usize dataSize, usize pos);

		Chunk&  _AllockChunk (usize insertTo);

	public:
		Deque (GX_DEFCTOR)										{ SetChunkSize( DefaultChunkSize ); }
		Deque (Self &&other);
		Deque (const Self &other)								{ SetChunkSize( other._chunkSize );  Copy( other ); }
		Deque (ArrayCRef<T> other)								{ SetChunkSize( DefaultChunkSize );  Copy( other ); }
		Deque (InitializerList<T> list)							{ SetChunkSize( DefaultChunkSize );  Copy( ArrayCRef<T>(list) ) }

		~Deque ()												{ Free(); }

		void Free ();
		void Clear ();


	// may invalidate element references - begin
		void Erase (usize pos, usize count = 1);
		//void Rearrange ();
		
		//void Insert (const T& value, usize pos)					{ _Insert<const T>( AddressOf(value), 1, pos ); }
		//void Insert (T&& value, usize pos)						{ _Insert<T>( AddressOf(value), 1, pos ); }
		//void Insert (ArrayCRef<T> other, usize pos)				{ _Insert<const T>( other.RawPtr(), other.Count(), pos ); }
		//void Insert (const Self &other, usize pos);
		//void Insert (Self &&other, usize pos);
	// may invalidate element references - end


		void PushBack (const T& value)							{ PushBack( T(value) ); }
		void PushBack (T&& value);

		void PushFront (const T& value)							{ PushFront( T(value) ); }
		void PushFront (T&& value);
		
		void PopBack ()											{ EraseFromBack( 1 ); }
		void PopFront ()										{ Erase( 0 ); }

		void EraseFromFront (usize count);
		void EraseFromBack (usize count);
		
		void AppendBack (InitializerList<T> other)				{ AppendBack( ArrayCRef<T>{other} ); }
		void AppendBack (ArrayCRef<T> other);
		void AppendBack (const Self &other);
		void AppendBack (Self &&other);
		
		void AppendFront (InitializerList<T> other)				{ AppendFront( ArrayCRef<T>{other} ); }
		void AppendFront (ArrayCRef<T> other);
		void AppendFront (const Self &other);
		void AppendFront (Self &&other);
		
		void Copy (ArrayCRef<T> other)							{ Clear();  AppendBack( other ); }
		void Copy (const Self &other)							{ Clear();  AppendBack( other ); }
		void Copy (Self &&other)								{ Clear();  AppendBack( RVREF(other) ); }

		void Resize (usize newSize);

		void SetChunkSize (usize newSize)						{ _chunkSize = GXMath::Min( newSize, MemoryContainer_t::MaxSize() ); }

		ND_ iterator		begin ()							{ return iterator{ _chunks.begin(), 0 }; }
		ND_ const_iterator	begin () const						{ return const_iterator{ _chunks.begin(), 0 }; }
		ND_ iterator		end ()								{ return iterator{ _chunks.end(), 0 }; }
		ND_ const_iterator	end () const						{ return const_iterator{ _chunks.end(), 0 }; }

		ND_ usize		GetChunkSize ()	const					{ return _chunkSize; }
		ND_ usize		Count ()		const;
		ND_ usize		Capacity ()		const;
		ND_ bool		Empty ()		const					{ return _chunks.Empty(); }
		ND_ BytesU		FullSize ()		const;
		
			Self &		operator =  (ArrayCRef<T> right)		{ Copy( right );		return *this; }
			Self &		operator =  (const Self &right)			{ Copy( right );		return *this; }
			Self &		operator =  (Self &&right)				{ Copy( RVREF(right) );	return *this; }

			Self &		operator << (T &&value)					{ PushBack( RVREF(value) );		return *this; }
			Self &		operator << (const T &value)			{ PushBack( value );			return *this; }

			Self &		operator >> (T &&value)					{ PushFront( RVREF(value) );	return *this; }
			Self &		operator >> (const T &value)			{ PushFront( value );			return *this; }

		ND_ T &			operator [] (usize index);
		ND_ T const&	operator [] (usize index)		const;
			
		ND_ bool		operator == (const Self &right) const;
		ND_ bool		operator != (const Self &right) const	{ return not ( (*this) == right ); }
		
		ND_ bool		operator == (ArrayCRef<T> right) const;
		ND_ bool		operator != (ArrayCRef<T> right) const	{ return not ( (*this) == right ); }

		static constexpr bool	IsLinearMemory ()				{ return false; }
		constexpr bool			IsStaticMemory ()	const		{ return false; }
		
		friend void SwapValues (INOUT Self &left, INOUT Self &right)
		{
			return GXTypes::SwapValues( left._chunks, right._chunks );
		}
	};
//-----------------------------------------------------------------------------

	

/*
=================================================
	Chunk::Free
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline void  Deque<T,S,MC,C,CS>::Chunk::Free ()
	{
		if ( memory.Pointer() != null )
		{
			if ( first > last )
			{
				Strategy_t::Destroy( memory.Pointer() + first, size - first );
				Strategy_t::Destroy( memory.Pointer(), last+1 );
			}
			else
			{
				Strategy_t::Destroy( memory.Pointer() + first, last+1 - first );
			}
			memory.Deallocate();
		}

		first = 0;
		last  = 0;
		size  = 0;
	}
	
/*
=================================================
	Chunk::Count
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	forceinline usize  Deque<T,S,MC,C,CS>::Chunk::Count () const
	{
		return Distance( first, last );
	}
	
/*
=================================================
	Chunk::Distance
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	forceinline usize  Deque<T,S,MC,C,CS>::Chunk::Distance (usize i, usize j) const
	{
		return i <= j ? (j - i + 1) : (size - i + j + 1);
	}

/*
=================================================
	Chunk::Wrap
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	forceinline usize  Deque<T,S,MC,C,CS>::Chunk::Wrap (isize i) const
	{
		const isize s = isize(size);
		return usize( i < 0 ? s + i : (i >= s ? i - s : i) );
	}
//-----------------------------------------------------------------------------
	


/*
=================================================
	Iterator::operator ++
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	template <bool IsConst>
	forceinline typename Deque<T,S,MC,C,CS>::Iterator<IsConst> &  Deque<T,S,MC,C,CS>::Iterator<IsConst>::operator ++ ()
	{
		++_index;

		if ( _index >= _chunk->Count() )
		{
			++_chunk;
			_index = 0;
		}
		return *this;
	}

/*
=================================================
	Iterator::operator --
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	template <bool IsConst>
	forceinline typename Deque<T,S,MC,C,CS>::Iterator<IsConst> &  Deque<T,S,MC,C,CS>::Iterator<IsConst>::operator -- ()
	{
		--_index;

		if ( _index >= _chunk->Count() )
		{
			--_chunk;
			_index = _chunk->Count()-1;
		}
		return *this;
	}
//-----------------------------------------------------------------------------



/*
=================================================
	constructor
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline Deque<T,S,MC,C,CS>::Deque (Self &&other) :
		_chunks{ RVREF(other._chunks) },
		//_totalCount{ other._totalCount },
		_chunkSize{ other._chunkSize }
	{
		//other._totalCount = 0;
		other._chunks.Clear();
	}
	
/*
=================================================
	Free
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline void  Deque<T,S,MC,C,CS>::Free ()
	{
		_chunks.Clear();
		//_totalCount = 0;
	}
	
/*
=================================================
	Clear
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline void  Deque<T,S,MC,C,CS>::Clear ()
	{
		_chunks.Clear();
		//_totalCount = 0;
	}

/*
=================================================
	Erase
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline void  Deque<T,S,MC,C,CS>::Erase (usize pos, usize count)
	{
		for (usize i = 0; count > 0 and i < _chunks.Count(); ++i)
		{
			auto&	ch = _chunks[i];

			if ( pos >= ch.Count() )
			{
				pos -= ch.Count();
				continue;
			}
			
			const usize	cnt			= GXMath::Min( count, ch.Count() - pos );
			const usize	first_del	= ch.Wrap( ch.first + pos );
			const usize	last_del	= ch.Wrap( first_del + cnt - 1 );
			
			count -= cnt;
			
			// delete range [first_del, last_del]
			if ( first_del > last_del )
			{
				Strategy_t::Destroy( ch.memory.Pointer() + first_del, ch.size - first_del );
				Strategy_t::Destroy( ch.memory.Pointer(), last_del + 1 );
			}
			else
			{
				Strategy_t::Destroy( ch.memory.Pointer() + first_del, cnt );
			}

			// remove empty chunk
			if ( ch.Count() == cnt )
			{
				_chunks.Erase( i );
				--i;
				continue;
			}

			const usize	left_size	= ch.Distance( ch.first, first_del ) - 1;
			const usize	right_size	= ch.Distance( last_del, ch.last ) - 1;

			if ( left_size < right_size )
			{
				// move left part
				const usize	new_first = ch.Wrap( last_del+1 - left_size );

				// copy range [first, first_del-1] into [new_first, last_del]
				for (usize k = first_del-1, j = last_del;;)
				{
					Strategy_t::Replace( ch.memory.Pointer() + j, ch.memory.Pointer() + k, 1, true );

					if ( k == ch.first or j == new_first )
						break;

					k = ch.Wrap( k-1 );
					j = ch.Wrap( j-1 );
				}

				ch.first = new_first;
			}
			else
			{
				// move right part
				const usize	new_last = ch.Wrap( first_del-1 + right_size );

				// copy range [last_del+1, last] into [first_del, new_last]
				for (usize k = last_del+1, j = first_del;;)
				{
					Strategy_t::Replace( ch.memory.Pointer() + j, ch.memory.Pointer() + k, 1, true );
					
					if ( k == ch.last or j == new_last )
						break;

					k = ch.Wrap( k+1 );
					j = ch.Wrap( j+1 );
				}

				ch.last = new_last;
			}
		}
		ASSERT( count == 0 );
	}
	
/*
=================================================
	EraseFromFront
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline void  Deque<T,S,MC,C,CS>::EraseFromFront (usize count)
	{
		for (usize i = 0; count > 0 and i < _chunks.Count(); ++i)
		{
			auto&		ch			= _chunks[i];
			const usize	cnt			= GXMath::Min( count, ch.Count() );
			const usize	last_del	= ch.Wrap( ch.first + cnt );
			
			count -= cnt;
			
			// delete range [first, last_del]
			if ( last_del < ch.first )
			{
				Strategy_t::Destroy( ch.memory.Pointer() + ch.first, ch.size - ch.first );
				Strategy_t::Destroy( ch.memory.Pointer(), last_del );
			}
			else
			{
				Strategy_t::Destroy( ch.memory.Pointer() + ch.first, cnt );
			}

			if ( ch.Count() == cnt )
			{
				// remove empty chunk
				_chunks.Erase( i );
			}
			else
				ch.first = last_del;
		}
		ASSERT( count == 0 );
	}
	
/*
=================================================
	EraseFromBack
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline void  Deque<T,S,MC,C,CS>::EraseFromBack (usize count)
	{
		for (usize i = _chunks.LastIndex(); count > 0 and i < _chunks.Count(); --i)
		{
			auto&		ch			= _chunks[i];
			const usize	cnt			= GXMath::Min( count, ch.Count() );
			const usize	first_del	= ch.Wrap( ch.first + ch.Count() - cnt );
			
			count -= cnt;

			// delete range [first_del, last]
			if ( first_del >= ch.last )
			{
				Strategy_t::Destroy( ch.memory.Pointer() + first_del, ch.size - first_del );
				Strategy_t::Destroy( ch.memory.Pointer(), ch.last+1 );
			}
			else
			{
				Strategy_t::Destroy( ch.memory.Pointer() + first_del, cnt );
			}

			if ( ch.Count() == cnt )
			{
				// remove empty chunk
				_chunks.Erase( i );
			}
			else
				ch.last = ch.Wrap( first_del-1 );
		}
		ASSERT( count == 0 );
	}
	
/*
=================================================
	_AllockChunk
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	forceinline typename Deque<T,S,MC,C,CS>::Chunk&  Deque<T,S,MC,C,CS>::_AllockChunk (usize insertTo)
	{
		Chunk	ch;
		
		ch.size  = _chunkSize;
		ch.memory.Allocate( OUT ch.size, false );
		
		ch.first = 0;
		//ch.first = ch.Wrap( ch.size/2 );	// temp
		ch.last  = ch.Wrap( ch.first-1 );

		_chunks.Insert( RVREF(ch), insertTo );

		return _chunks[ insertTo ];
	}
		
/*
=================================================
	PushBack
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline void  Deque<T,S,MC,C,CS>::PushBack (T&& value)
	{
		auto&	ch = _chunks.Back();

		if ( ch.Count() < ch.size )
		{
			ch.last = ch.Wrap( ch.last + 1 );

			Strategy_t::Move( ch.memory.Pointer() + ch.last, AddressOf(value), 1 );
		}
		else
		{
			auto&	ch2 = _AllockChunk( _chunks.Count() );
			
			ch2.last = ch2.Wrap( ch2.last + 1 );

			Strategy_t::Move( ch2.memory.Pointer() + ch2.last, AddressOf(value), 1 );
		}
	}
	
/*
=================================================
	PushFront
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline void  Deque<T,S,MC,C,CS>::PushFront (T&& value)
	{
		auto&	ch = _chunks.Front();

		if ( ch.Count() < ch.size )
		{
			ch.first = ch.Wrap( ch.first - 1 );

			Strategy_t::Move( ch.memory.Pointer() + ch.first, AddressOf(value), 1 );
		}
		else
		{
			auto&	ch2 = _AllockChunk( 0 );
			
			ch2.first = ch2.Wrap( ch2.first - 1 );

			Strategy_t::Move( ch2.memory.Pointer() + ch2.first, AddressOf(value), 1 );
		}
	}

/*
=================================================
	_Insert
=================================================
*
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	template <typename B>
	inline void  Deque<T,S,MC,C,CS>::_Insert (B *data, usize dataSize, usize pos)
	{
		usize	chunk_index = 0;

		for (; chunk_index < _chunks.Count(); ++chunk_index)
		{
			auto&	ch	= _chunks[ chunk_index ];

			if ( pos > ch.Count() )
			{
				pos -= ch.Count();
				continue;
			}
		}

		if ( chunk_index < _chunks.Count() )
		{
			auto&	ch	= _chunks[ chunk_index ];

			// insert in current chunk
			if ( ch.Available() <= dataSize )
			{
				const usize	new_last = ch.Wrap( ch.last + dataSize );

				if ( new_last > ch.first )
				{
					Strategy_t::Replace( ch.memory.Pointer(), ch.memo );
				}
				else
				{
				}
				return;
			}
		}



		if ( chunk_index+1 < _chunks.Count() )
		{
			auto&	next = _chunks[ chunk_index+1 ];
			usize	cnt1 = ch.Count() - pos;
			usize	cnt2 = next.Available();

			// move from current to next

				
			break;
		}
		else
		{




			break;
		}


		ASSERT( dataSize == 0 );
	}
	
/*
=================================================
	Insert
=================================================
*
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline void Deque<T,S,MC,C,CS>::Insert (const Self &other, usize pos)
	{
	}

/*
=================================================
	Insert
=================================================
*
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline void  Deque<T,S,MC,C,CS>::Insert (Self &&other, usize pos)
	{
	}

/*
=================================================
	AppendBack
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline void  Deque<T,S,MC,C,CS>::AppendBack (ArrayCRef<T> other)
	{
		if ( other.Empty() )
			return;

		T const*	data_ptr	= other.RawPtr();
		usize		data_size	= other.Count();
		
		// copy to the last chunk
		if ( not _chunks.Empty() )
		{
			auto&		ch		= _chunks.Back();
			const usize	cnt		= GXMath::Min( data_size, ch.Available() );
			const usize	uninit	= ch.Wrap( ch.first + ch.Count() );		// index of first uninitialized element
			
			ch.last   = ch.Wrap( ch.last + cnt );
			data_size -= cnt;

			// initialize
			if ( uninit >= ch.last )
			{
				Strategy_t::Copy( ch.memory.Pointer() + uninit, data_ptr, ch.size - uninit );
				data_ptr += ch.size - uninit;

				Strategy_t::Copy( ch.memory.Pointer(), data_ptr, ch.last+1 );
				data_ptr += ch.last+1;
			}
			else
			{
				Strategy_t::Copy( ch.memory.Pointer() + uninit, data_ptr, cnt );
				data_ptr += cnt;
			}
		}
		
		// create new chunks
		for (; data_size != 0; )
		{
			auto&	ch = _AllockChunk( _chunks.Count() );

			if ( data_size < ch.size )
			{
				ch.first  = 0;
				ch.last   = data_size-1;
				
				Strategy_t::Copy( ch.memory.Pointer() + ch.first, data_ptr, ch.last+1 - ch.first );

				data_size = 0;
			}
			else
			{
				ch.first   = 0;
				ch.last    = ch.size-1;

				Strategy_t::Copy( ch.memory.Pointer() + ch.first, data_ptr, ch.last+1 - ch.first );

				data_ptr  += ch.size;
				data_size -= ch.size;
			}
		}
	}
	
/*
=================================================
	AppendBack
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline void  Deque<T,S,MC,C,CS>::AppendBack (const Self &other)
	{
		for (auto& src : other._chunks)
		{
			if ( src.first <= src.last )
			{
				AppendBack( ArrayCRef<T>( src.memory.Pointer() + src.first, src.Count() ) );
			}
			else
			{
				AppendBack( ArrayCRef<T>( src.memory.Pointer() + src.first, src.size - src.first ) );
				AppendBack( ArrayCRef<T>( src.memory.Pointer() + src.first, src.last+1 ) );
			}
		}
	}
	
/*
=================================================
	AppendBack
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline void  Deque<T,S,MC,C,CS>::AppendBack (Self &&other)
	{
		_chunks.AppendBack( RVREF(other._chunks) );
	}
	
/*
=================================================
	AppendFront
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline void  Deque<T,S,MC,C,CS>::AppendFront (ArrayCRef<T> other)
	{
		if ( other.Empty() )
			return;

		T const*	data_ptr	= other.RawPtr();
		usize		data_size	= other.Count();
		
		// copy to the first chunk
		if ( not _chunks.Empty() )
		{
			auto&		ch		= _chunks.Front();
			const usize	cnt		= GXMath::Min( data_size, ch.Available() );
			const usize	uninit	= ch.Wrap( ch.first - cnt );	// index of first uninitialized element

			// copy to range [uninit, first]
			if ( uninit > ch.first )
			{
				data_size -= ch.size - uninit;
				Strategy_t::Copy( ch.memory.Pointer() + uninit, data_ptr + data_size, ch.size - uninit );

				data_size -= ch.first;
				Strategy_t::Copy( ch.memory.Pointer(), data_ptr + data_size, ch.first );
			}
			else
			{
				data_size -= cnt;
				Strategy_t::Copy( ch.memory.Pointer() + uninit, data_ptr + data_size, cnt );
			}
			
			ch.first = uninit;
		}
		
		// create new chunks
		for (; data_size != 0; )
		{
			auto&	ch = _AllockChunk( 0 );

			if ( data_size < ch.size )
			{
				ch.first  = 0;
				ch.last   = data_size-1;
				data_size = 0;
				
				Strategy_t::Copy( ch.memory.Pointer() + ch.first, data_ptr, ch.last+1 - ch.first );
			}
			else
			{
				ch.first   = 0;
				ch.last    = ch.size-1;
				data_size -= ch.size;

				Strategy_t::Copy( ch.memory.Pointer() + ch.first, data_ptr + data_size, ch.last+1 - ch.first );
			}
		}
	}
	
/*
=================================================
	AppendFront
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline void  Deque<T,S,MC,C,CS>::AppendFront (const Self &other)
	{
		FOR_rev( i, other._chunks )
		{
			auto&	src = other._chunks[i];

			if ( src.first <= src.last )
			{
				AppendFront( ArrayCRef<T>( src.memory.Pointer() + src.first, src.Count() ) );
			}
			else
			{
				AppendFront( ArrayCRef<T>( src.memory.Pointer() + src.first, src.last+1 ) );
				AppendFront( ArrayCRef<T>( src.memory.Pointer() + src.first, src.size - src.first ) );
			}
		}
	}
	
/*
=================================================
	AppendFront
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline void  Deque<T,S,MC,C,CS>::AppendFront (Self &&other)
	{
		_chunks.AppendFront( RVREF(other._chunks) );
	}

/*
=================================================
	Resize
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline void  Deque<T,S,MC,C,CS>::Resize (usize newSize)
	{
		FOR( i, _chunks )
		{
			auto&	ch = _chunks[i];

			if ( ch.Count() < newSize )
			{
				newSize -= ch.Count();
			}
			else
			// delete elements
			{
				const usize  old_count  = ch.Count();
				const usize  first_del  = ch.Wrap( ch.first + newSize );

				if ( first_del >= ch.last )
				{
					Strategy_t::Destroy( ch.memory.Pointer() + first_del, ch.size - first_del );
					Strategy_t::Destroy( ch.memory.Pointer(), ch.last+1 );
				}
				else
				{
					Strategy_t::Destroy( ch.memory.Pointer() + first_del, old_count - newSize );
				}

				ch.last = ch.Wrap( first_del-1 );

				_chunks.Resize( i+1 );
				return;
			}
		}

		// resize last chunk
		if ( _chunks.Count() > 0 )
		{
			auto&		ch		= _chunks.Back();
			const usize	cnt		= GXMath::Min( newSize, ch.Available() );
			const usize	uninit	= ch.Wrap( ch.first + ch.Count() );			// index of first uninitialized element
			
			ch.last  = ch.Wrap( ch.last + cnt );
			newSize -= cnt;

			// initialize
			if ( uninit >= ch.last )
			{
				Strategy_t::Create( ch.memory.Pointer() + uninit, ch.size - uninit );
				Strategy_t::Create( ch.memory.Pointer(), ch.last+1 );
			}
			else
			{
				Strategy_t::Create( ch.memory.Pointer() + uninit, cnt );
			}
		}

		// create new chunks
		for (; newSize != 0; )
		{
			auto&	ch = _AllockChunk( _chunks.Count() );

			if ( newSize < ch.size )
			{
				ch.first = 0;
				ch.last  = newSize-1;
				newSize  = 0;
			}
			else
			{
				ch.first = 0;
				ch.last  = ch.size-1;
				newSize -= ch.size;
			}
			
			Strategy_t::Create( ch.memory.Pointer() + ch.first, ch.Count() );
		}
	}
	
/*
=================================================
	Rearrange
=================================================
*
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline void  Deque<T,S,MC,C,CS>::Rearrange ()
	{
	}

/*
=================================================
	Count
	TODO: use _totalCount
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline usize  Deque<T,S,MC,C,CS>::Count () const
	{
		usize	count = 0;

		for (auto& ch : _chunks) {
			count += ch.Count();
		}
		return count;
	}
	
/*
=================================================
	Capacity
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline usize  Deque<T,S,MC,C,CS>::Capacity () const
	{
		usize	size = 0;

		for (auto& ch : _chunks) {
			size += ch.size;
		}
		return size;
	}
	
/*
=================================================
	FullSize
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline BytesU  Deque<T,S,MC,C,CS>::FullSize () const
	{
		BytesU	size;

		for (auto& ch : _chunks) {
			size += ch.size * SizeOf<T>;
		}
		return size;
	}

/*
=================================================
	operator []
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline T &  Deque<T,S,MC,C,CS>::operator [] (usize index)
	{
		for (auto& ch : _chunks)
		{
			if ( index < ch.Count() )
			{
				return ch.Get( index );
			}

			index -= ch.Count();
		}

		WARNING( "index out of range" );
		return *Cast<T *>( null );
	}

/*
=================================================
	operator []
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline T const&  Deque<T,S,MC,C,CS>::operator [] (usize index) const
	{
		return const_cast<Self *>(this)->operator[] (index);
	}

/*
=================================================
	operator ==
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline bool  Deque<T,S,MC,C,CS>::operator == (const Self &right) const
	{
		usize	left_chunk	= 0;
		usize	right_chunk	= 0;
		usize	left_index	= 0;
		usize	right_index	= 0;

		for (; left_chunk < _chunks.Count() and right_chunk < right._chunks.Count(); )
		{
			auto&	lch		= _chunks[ left_chunk ];
			auto&	rch		= right._chunks[ right_chunk ];
			bool	skip	= false;

			if ( left_index >= lch.Count() )
			{
				left_index = 0;
				++left_chunk;
				skip = true;
			}

			if ( right_index >= rch.Count() )
			{
				right_index = 0;
				++right_chunk;
				skip = true;
			}

			if ( skip )
				continue;

			if ( not (lch.Get( left_index ) == rch.Get( right_index )) )
				return false;

			++left_index;
			++right_index;
		}

		return left_chunk == _chunks.Count() and right_chunk == right._chunks.Count();
	}

/*
=================================================
	operator ==
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	inline bool  Deque<T,S,MC,C,CS>::operator == (ArrayCRef<T> right) const
	{
		if ( _chunks.Empty() and right.Empty() )
			return true;

		usize	chunk		= 0;
		usize	left_index	= 0;
		usize	right_index	= 0;

		for (; chunk < _chunks.Count() and right_index < right.Count(); )
		{
			auto&	ch = _chunks[ chunk ];

			if ( left_index >= ch.Count() )
			{
				left_index = 0;
				++chunk;
				continue;
			}

			if ( not (ch.Get( left_index ) == right[ right_index ]) )
				return false;

			++left_index;
			++right_index;
		}

		return	right_index == right.Count() and
				(chunk+1 == _chunks.Count() and left_index == _chunks.Back().Count());
	}

/*
=================================================
	Hash
=================================================
*/
	template <typename T, typename S, typename MC, template <typename, typename, typename> class C, usize CS>
	struct Hash< Deque<T,S,MC,C,CS> >
	{
		ND_ HashResult  operator () (const Deque<T,S,MC,C,CS> &x) const noexcept
		{
			HashResult	result;
			for (auto& ch : x._chunks)
			{
				for (usize i = ch.first;;)
				{
					result += HashOf( ch.memory.Pointer()[ i ] );
					
					if ( i == ch.last )
						break;

					i = ch.Wrap( i+1 );
				}
			}
			return result;
		}
	};
	

}	// GXTypes
}	// GX_STL
