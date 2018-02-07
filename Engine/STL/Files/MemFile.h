// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "BaseFile.h"

namespace GX_STL
{
namespace File
{
	
	class BaseMemRFile;
	class BaseMemWFile;
	class MemRFile;
	class MemWFile;

	
	//
	// Base Memory Read only File
	//

	class BaseMemRFile : public RFile
	{
	// variables
	protected:
		BinaryArray		_memBuffer;
		BinArrayRef		_mem;
		BytesU			_pos;
		bool			_opened;


	// methods
	public:
		BaseMemRFile () : _pos(0), _opened(false)
		{}

		virtual ~BaseMemRFile ()
		{
			_Close();
		}


		// RFile //
		virtual BytesU ReadBuf (void * buf, BytesU size) noexcept override
		{
			if ( not _opened )
				return UMax;

			if ( usize(_pos + size) > _mem.Count() )
				size = BytesU( _mem.Count() - _pos );

			UnsafeMem::MemCopy( buf, _mem.ptr() + (usize)_pos, size );
			_pos += size;

			return size;
		}


		// BaseFile //
		virtual void Close () noexcept override
		{
			return _Close();
		}

		virtual bool IsOpened () const noexcept override
		{
			return _opened;
		}
		
		virtual bool SeekSet (BytesU offset) noexcept override
		{
			ASSERT( offset <= _mem.Size() );
			return _SetPos( offset );
		}

		virtual bool SeekCur (BytesI offset) noexcept override
		{
			usize new_pos = usize(_pos) + isize(offset);

			ASSERT( new_pos >= 0 and new_pos < _mem.Size() );
			return _SetPos( BytesU( new_pos ) );
		}

		virtual bool SeekEnd (BytesU offset) noexcept override
		{
			ASSERT( offset <= _mem.Size() );
			return _SetPos( BytesU(_mem.Count()) - offset );
		}

		virtual BytesU RemainingSize () const noexcept override
		{
			return BytesU(_mem.Count()) - _pos;
		}

		virtual BytesU Size () const noexcept override
		{
			return _mem.Size();
		}

		virtual BytesU Pos () const noexcept override
		{
			return _pos;
		}
		
		virtual bool IsEOF () const noexcept override
		{
			return Pos() >= Size();
		}

		virtual StringCRef	Name () const override
		{
			return StringCRef();
		}

		virtual EFile::type		GetType () const override
		{
			return EFile::Memory;
		}


	protected:
		void _Close ()
		{
			_memBuffer.Clear();
			_mem	= _memBuffer;
			_pos	= BytesU();
			_opened = false;
		}
		
	private:
		bool _SetPos (BytesU newPos)
		{
			if ( newPos >= 0 and
				 newPos < _mem.Count() )
			{
				_pos = newPos;
				return true;
			}
			else
			{
				_pos = BytesU();
				return false;
			}
		}
	};

	

	//
	// Memory Read only File
	//

	class MemRFile : public BaseMemRFile
	{
	// type
	public:
		SHARED_POINTER( MemRFile );

		enum EFlag
		{
			None = 0,
			COPY = 1,
			MOVE = 2,
		};


	// methods
	public:
		MemRFile () {}

		
		CHECKRES static MemRFilePtr New ()
		{
			return new MemRFile();
		}


		CHECKRES static MemRFilePtr New (const RFilePtr &file)
		{
			MemRFilePtr	mfile = new MemRFile();

			if ( mfile->Create( file ) )
				return mfile;

			return null;
		}

		template <typename T>
		CHECKRES static MemRFilePtr New (ArrayCRef<T> arr, EFlag flag)
		{
			MemRFilePtr file = new MemRFile();

			if ( file->Create( arr, flag ) )
				return file;

			return null;
		}


		bool Create (const RFilePtr &file)
		{
			CHECK_ERR( this != file.ptr() );

			usize	size = (usize)file->RemainingSize();
			
			_pos	= BytesU();
			_opened = true;

			_memBuffer.Resize( size, false );
			_mem	= _memBuffer;

			CHECK_ERR( file->Read( _mem.ptr(), _mem.Size() ) );
			return true;
		}

		
		template <typename T>
		bool Create (ArrayCRef<T> arr, EFlag flag)
		{
			CHECK_ERR( not arr.Empty() );

			switch ( flag )
			{
				case COPY :
					_memBuffer.Copy( (ubyte *)arr.ptr(), arr.Size() );
					_mem	= _memBuffer;
					break;

				case None :
					_memBuffer.Free();
					_mem	= arr;
					break;

				case MOVE :
					RETURN_ERR( "move operation not supported for this array type" );

				default :
					RETURN_ERR( "invalid flag" );
			}

			_pos	= BytesU();
			_opened	= true;

			return true;
		}

		
		bool CreateFromArray (INOUT BinaryArray &arr, EFlag flag)
		{
			CHECK_ERR( not arr.Empty() );
			
			_memBuffer.Clear();

			switch ( flag )
			{
				case COPY :
					_memBuffer	= arr;
					_mem		= _memBuffer;
					break;

				case None :
					_mem	= arr;
					break;

				case MOVE :
					RETURN_ERR( "move operation not supported for this array type" );

				default :
					RETURN_ERR( "invalid flag" );
			}

			_pos	= BytesU();
			_opened	= true;

			return true;
		}


		bool CreateFromMemWFile (const SharedPointerType< BaseMemWFile > &file, EFlag flag);
		

		BinArrayCRef GetData () const
		{
			return (BinArrayCRef) _mem;
		}
	};



	//
	// Base Memory Write only File
	//
	
	class BaseMemWFile : public WFile
	{
		friend class MemRFile;

	// variables
	protected:
		BinaryArray		_mem;
		BytesU			_pos;
		bool			_opened;


	// methods
	public:
		BaseMemWFile () : _pos(0), _opened(false)	{}
		virtual ~BaseMemWFile ()					{ _Close(); }


		// WFile //
		virtual BytesU WriteBuf (const void * buf, BytesU size) noexcept override
		{
			if ( not _opened )
				return UMax;

			if ( usize(_pos) != _mem.Count() )
			{
				_mem.Resize( (usize)_pos );
			}
			
			_mem.Append( BinArrayCRef::FromVoid( buf, size ) );
			_pos += size;

			return size;
		}
		
		virtual void Flush () noexcept override
		{
		}

		
		// BaseFile //
		virtual void Close () noexcept override
		{
			return _Close();
		}

		virtual bool IsOpened () const noexcept override
		{
			return _opened;
		}
		
		virtual bool SeekSet (BytesU offset) noexcept override
		{
			ASSERT( offset <= _mem.Size() );
			return _SetPos( offset );
		}

		virtual bool SeekCur (BytesI offset) noexcept override
		{
			usize new_pos = usize(_pos) + isize(offset);

			ASSERT( new_pos >= 0 and new_pos < _mem.Size() );
			return _SetPos( BytesU( new_pos ) );
		}

		virtual bool SeekEnd (BytesU offset) noexcept override
		{
			ASSERT( offset <= _mem.Size() );
			return _SetPos( _mem.Count() - offset );
		}

		virtual BytesU RemainingSize () const noexcept override
		{
			return BytesU(_mem.Count()) - _pos;
		}

		virtual BytesU Size () const noexcept override
		{
			return _mem.Size();
		}

		virtual BytesU Pos () const noexcept override
		{
			return _pos;
		}
		
		virtual bool IsEOF () const noexcept override
		{
			return Pos() >= Size();
		}

		virtual StringCRef  Name () const override
		{
			return StringCRef();
		}

		virtual EFile::type  GetType () const override
		{
			return EFile::Memory;
		}


	protected:
		void _Close ()
		{
			_mem.Clear();
			_pos	= BytesU();
			_opened	= false;
		}
		
	private:
		bool _SetPos (BytesU newPos)
		{
			if ( newPos >= 0 and
				 newPos < _mem.Count() )
			{
				_pos = newPos;
				return true;
			}
			else
			{
				_pos = BytesU();
				return false;
			}
		}
	};

	
	inline bool MemRFile::CreateFromMemWFile (const SharedPointerType< BaseMemWFile > &file, EFlag flag)
	{
		CHECK_ERR( file );
		return CreateFromArray( file->_mem, flag );
	}


	
	//
	// Memory Write only File
	//
	
	class MemWFile : public BaseMemWFile
	{
	// types
	public:
		SHARED_POINTER( MemWFile );


	// methods
	public:
		MemWFile () {}


		CHECKRES static MemWFilePtr New (BytesU reserve = BytesU())
		{
			MemWFilePtr	file = new MemWFile();

			if ( file->Create( reserve ) )
				return file;

			return null;
		}


		bool Create (BytesU reserve)
		{
			_mem.Clear();
			_mem.Reserve( (usize)reserve );

			_pos	= BytesU();
			_opened	= true;

			return true;
		}


		bool Save (const WFilePtr &file) const
		{
			CHECK_ERR( this != file.ptr() );

			CHECK_ERR( file and file->IsOpened() );
			CHECK_ERR( not _mem.Empty() );

			CHECK_ERR( file->Write( _mem.ptr(), _mem.Size() ) );
			return true;
		}


		BinArrayCRef GetData () const
		{
			return _mem;
		}
	};
	
		
	SHARED_POINTER( MemRFile );
	SHARED_POINTER( MemWFile );

}	// File
}	// GX_STL
