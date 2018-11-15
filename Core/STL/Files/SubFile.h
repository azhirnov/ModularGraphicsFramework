// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "BaseFile.h"

namespace GX_STL
{
namespace GXFile
{

	//
	// Read only Sub File
	//

	class SubRFile : public RFile
	{
	// types
	public:
		SHARED_POINTER( SubRFile );

		typedef SubRFile	Self;


	// variables
	protected:
		RFilePtr	_file;
		BytesU		_offset,
					_size,
					_pos;


	// methods
	public:
		SubRFile (const RFilePtr &file, BytesU offset, BytesU size):
			_file(file), _offset(offset), _size(size), _pos(0)
		{
			ASSERT( _file );
			ASSERT( offset + size <= _file->Size() );
		}


		ND_ static SubRFilePtr New (const RFilePtr &file, BytesU offset, BytesU size)
		{
			return new Self( file, offset, size );
		}
		

		// RFile //
		virtual BytesU ReadBuf (void * buf, BytesU size) noexcept override
		{
			BytesU	result = _file->ReadBufFrom( buf, size, _offset + _pos );
			_pos += result;
			return result;
		}
		

		// BaseFile //
		virtual void Close () noexcept override
		{
			_file	= null;
			_offset	= 0_b;
			_size	= 0_b;
			_pos	= 0_b;
		}
		
		virtual bool IsOpened () const noexcept override
		{
			return _file.IsNotNull();
		}

		virtual bool SeekSet (BytesU offset) noexcept override
		{
			ASSERT( offset <= _size );
			return _SetPos( _offset + offset );
		}

		virtual bool SeekCur (BytesI offset) noexcept override
		{
			usize new_pos = usize(_pos) + isize(offset);

			ASSERT( new_pos >= 0 and new_pos < _size );
			return _SetPos( BytesU( new_pos ) );
		}

		virtual bool SeekEnd (BytesU offset) noexcept override
		{
			ASSERT( offset <= _size );
			return _SetPos( _offset + _size - offset );
		}
		
		virtual BytesU RemainingSize () const noexcept override
		{
			return _size - _pos;
		}

		virtual BytesU Size () const noexcept override
		{
			return _size;
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
			return _file->Name();
		}

		virtual EFile::type  GetType () const override
		{
			return EFile::SubFile;
		}


	private:
		bool _SetPos (BytesU newPos)
		{
			if ( newPos >= _offset and
				 newPos < _offset + _size )
			{
				_pos = newPos;
				return true;
			}
			else
			{
				_pos = 0_b;
				return false;
			}
		}
	};

	

	//
	// Write only Sub File
	//

	class SubWFile : public WFile
	{
	// types
	public:
		SHARED_POINTER( SubWFile );

		typedef SubWFile	Self;


	// variables
	protected:
		WFilePtr	_file;
		BytesU		_offset,
					_size,
					_pos;


	// methods
	public:
		SubWFile (const WFilePtr &file, BytesU offset, BytesU size):
			_file(file), _offset(offset), _size(size), _pos(0)
		{
			ASSERT( _file );
			ASSERT( offset + size <= _file->Size() );
		}


		ND_ static SubWFilePtr New (const WFilePtr &file, BytesU offset, BytesU size)
		{
			return new Self( file, offset, size );
		}
		

		// WFile //
		virtual BytesU WriteBuf (const void * buf, BytesU size) noexcept override
		{
			const BytesU	pos		= _file->Pos();
			const BytesU	new_pos = BytesU( _offset + _pos );

			if ( pos != new_pos )
				_file->SeekSet( new_pos );

			BytesU	result = _file->WriteBuf( buf, size );
			_pos += result;

			_file->SeekSet( pos );
			return result;
		}
		
		virtual void Flush () noexcept override
		{
			_file->Flush();
		}


		// BaseFile //
		virtual void Close () noexcept override
		{
			_file	= null;
			_offset	= 0_b;
			_size	= 0_b;
			_pos	= 0_b;
		}
		
		virtual bool IsOpened () const noexcept override
		{
			return _file.IsNotNull();
		}
		
		virtual bool SeekSet (BytesU offset) noexcept override
		{
			ASSERT( offset <= _size );
			return _SetPos( _offset + offset );
		}

		virtual bool SeekCur (BytesI offset) noexcept override
		{
			usize new_pos = usize(_pos) + isize(offset);
			
			ASSERT( new_pos >= 0 and new_pos < _size );
			return _SetPos( BytesU( new_pos ) );
		}

		virtual bool SeekEnd (BytesU offset) noexcept override
		{
			ASSERT( offset <= _size );
			return _SetPos( _offset + _size - offset );
		}
		
		virtual BytesU RemainingSize () const noexcept override
		{
			return BytesU( _size - _pos );
		}

		virtual BytesU Size () const noexcept override
		{
			return BytesU( _size );
		}

		virtual BytesU Pos () const noexcept override
		{
			return BytesU( _pos );
		}
		
		virtual bool IsEOF () const noexcept override
		{
			return Pos() >= Size();
		}

		virtual StringCRef  Name () const override
		{
			return _file->Name();
		}

		virtual EFile::type  GetType () const override
		{
			return EFile::SubFile;
		}


	private:
		bool _SetPos (BytesU newPos)
		{
			if ( newPos >= _offset and
				 newPos < _offset + _size )
			{
				_pos = newPos;
				return true;
			}
			else
			{
				_pos = 0_b;
				return false;
			}
		}
	};
	

}	// GXFile
}	// GX_STL
