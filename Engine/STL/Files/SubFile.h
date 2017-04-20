// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "BaseFile.h"

namespace GX_STL
{
namespace File
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
		SubRFile (const File::RFilePtr &file, BytesU offset, BytesU size):
			_file(file), _offset(offset), _size(size), _pos(0)
		{
			ASSERT( _file );
			ASSERT( offset + size <= _file->Size() );
		}


		static SubRFilePtr New (const File::RFilePtr &file, BytesU offset, BytesU size)
		{
			return new Self( file, offset, size );
		}
		

		// RFile //
		virtual BytesU ReadBuf (void * buf, BytesU size) override
		{
			BytesU	result = _file->ReadBufFrom( buf, size, _offset + _pos );
			_pos += result;
			return result;
		}
		

		// BaseFile //
		virtual void Close () override
		{
			_file	= null;
			_offset	= BytesU();
			_size	= BytesU();
			_pos	= BytesU();
		}
		
		virtual bool IsOpened () const override
		{
			return _file.IsNotNull();
		}

		virtual bool SeekSet (BytesU offset) override
		{
			ASSERT( offset <= _size );
			return _SetPos( _offset + offset );
		}

		virtual bool SeekCur (BytesI offset) override
		{
			usize new_pos = usize(_pos) + isize(offset);

			ASSERT( new_pos >= 0 and new_pos < _size );
			return _SetPos( BytesU( new_pos ) );
		}

		virtual bool SeekEnd (BytesU offset) override
		{
			ASSERT( offset <= _size );
			return _SetPos( _offset + _size - offset );
		}
		
		virtual BytesU RemainingSize () const override
		{
			return _size - _pos;
		}

		virtual BytesU Size () const override
		{
			return _size;
		}

		virtual BytesU Pos () const override
		{
			return _pos;
		}
		
		virtual bool IsEOF () const override
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
				_pos = BytesU();
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
		SubWFile (const File::WFilePtr &file, BytesU offset, BytesU size):
			_file(file), _offset(offset), _size(size), _pos(0)
		{
			ASSERT( _file );
			ASSERT( offset + size <= _file->Size() );
		}


		static SubWFilePtr New (const File::WFilePtr &file, BytesU offset, BytesU size)
		{
			return new Self( file, offset, size );
		}
		

		// WFile //
		virtual BytesU WriteBuf (const void * buf, BytesU size) override
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
		
		virtual void Flush () override
		{
			_file->Flush();
		}


		// BaseFile //
		virtual void Close () override
		{
			_file	= null;
			_offset	= BytesU();
			_size	= BytesU();
			_pos	= BytesU();
		}
		
		virtual bool IsOpened () const override
		{
			return _file.IsNotNull();
		}
		
		virtual bool SeekSet (BytesU offset) override
		{
			ASSERT( offset <= _size );
			return _SetPos( _offset + offset );
		}

		virtual bool SeekCur (BytesI offset) override
		{
			usize new_pos = usize(_pos) + isize(offset);
			
			ASSERT( new_pos >= 0 and new_pos < _size );
			return _SetPos( BytesU( new_pos ) );
		}

		virtual bool SeekEnd (BytesU offset) override
		{
			ASSERT( offset <= _size );
			return _SetPos( _offset + _size - offset );
		}
		
		virtual BytesU RemainingSize () const override
		{
			return BytesU( _size - _pos );
		}

		virtual BytesU Size () const override
		{
			return BytesU( _size );
		}

		virtual BytesU Pos () const override
		{
			return BytesU( _pos );
		}
		
		virtual bool IsEOF () const override
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
				_pos = BytesU();
				return false;
			}
		}
	};
	

}	// File
}	// GX_STL
