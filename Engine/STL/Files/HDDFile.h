// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "BaseFile.h"

namespace GX_STL
{
namespace File
{

	//
	// HDD Read only File
	//

	class HddRFile : public RFile
	{
	// types
	public:
		SHARED_POINTER( HddRFile );


	// variables
	protected:
		Ptr< FILE >		_file;
		String			_name;


	// methods
	public:
		HddRFile ()		{}
		~HddRFile ()	{ _Close(); }


		static HddRFilePtr New ()
		{
			return new HddRFile();
		}

		static HddRFilePtr New (StringCRef address)
		{
			HddRFilePtr	file = new HddRFile();

			if ( file->Open( address ) )
				return file;

			return null;
		}


		bool Open (StringCRef address)
		{
			Close();
			_name = address;
			_file = fopen( address.cstr(), "rb" );
			return IsOpened();
		}


		// RFile //
		virtual BytesU ReadBuf (void * buf, BytesU size) override
		{
			return BytesU( fread( buf, 1, usize(size), _file.ptr() ) );
		}


		// BaseFile //
		virtual void Close () override
		{
			return _Close();
		}

		virtual bool IsOpened () const override
		{
			return _file.IsNotNull();
		}

		virtual bool SeekSet (BytesU offset) override
		{
			return fseek( _file, int(offset), SEEK_SET ) == 0;
		}

		virtual bool SeekCur (BytesI offset) override
		{
			return fseek( _file, int(offset), SEEK_CUR ) == 0;
		}

		virtual bool SeekEnd (BytesU offset) override
		{
			return fseek( _file, int(offset), SEEK_END ) == 0;
		}
		
		virtual BytesU RemainingSize () const override
		{
			usize	size = 0;
			int		pos  = ftell( _file );

			fseek( _file, 0, SEEK_END );
			size = ftell( _file );
			fseek( _file, pos, SEEK_SET );

			return BytesU( size - pos );
		}

		virtual BytesU Size () const override
		{
			usize	size = 0;
			int		pos  = ftell( _file );

			fseek( _file, 0, SEEK_END );
			size = ftell( _file );
			fseek( _file, pos, SEEK_SET );

			return BytesU( size );
		}

		virtual BytesU Pos () const override
		{
			return BytesU( ftell( _file ) );
		}
		
		virtual bool IsEOF () const override
		{
			return Pos() >= Size();
		}

		virtual StringCRef  Name () const override
		{
			return _name;
		}

		virtual EFile::type  GetType () const override
		{
			return EFile::HDD;
		}


	private:
		void _Close ()
		{
			if ( IsOpened() )
			{
				fclose( _file );
				_file = null;
			}
			_name.Clear();
		}
	};



	//
	// HDD Write only File
	//

	class HddWFile : public WFile
	{
	// types
	public:
		struct EOpenFlags
		{
			enum type
			{
				None	= 0,		// create empty file
				Append	= 1 << 0,	// write to end, seek ignored, create if not exist
				Update	= 1 << 1,	// create empty file for input and output

				AppendUpdate = Append | Update,	// write to end
			};

			GX_ENUM_BIT_OPERATIONS( type );
		};

		SHARED_POINTER( HddWFile );


	// variables
	protected:
		Ptr< FILE >		_file;
		String			_name;


	// methods
	public:
		HddWFile ()		{}
		~HddWFile ()	{ _Close(); }
		

		static HddWFilePtr New ()
		{
			return new HddWFile();
		}

		static HddWFilePtr New (StringCRef address, EOpenFlags::type flags = EOpenFlags::None)
		{
			HddWFilePtr	file = new HddWFile();

			if ( file->Open( address, flags ) )
				return file;

			return null;
		}


		bool Open (StringCRef address, EOpenFlags::type flags = EOpenFlags::None)
		{
			char	mode[8]	= {0};
			int		pos		= 0;

			if ( EnumEq( flags, EOpenFlags::Append ) )
				mode[ pos++ ] = 'a';
			else
				mode[ pos++ ] = 'w';

			mode[ pos++ ] = 'b';

			if ( EnumEq( flags, EOpenFlags::Update ) )
				mode[ pos++ ] = '+';

			mode[ pos++ ] = 0;


			Close();
			_name = address;
			_file = fopen( address.cstr(), mode );
			return IsOpened();
		}


		// WFile //
		virtual BytesU WriteBuf (const void * buf, BytesU size) override
		{
			return BytesU( fwrite( buf, 1, usize(size), _file ) );
		}
		
		virtual void Flush () override
		{
			fflush( _file );
		}


		// BaseFile //
		virtual void Close () override
		{
			return _Close();
		}

		virtual bool IsOpened () const override
		{
			return _file.IsNotNull();
		}

		virtual bool SeekSet (BytesU offset) override
		{
			return fseek( _file, int(offset), SEEK_SET ) == 0;
		}

		virtual bool SeekCur (BytesI offset) override
		{
			return fseek( _file, int(offset), SEEK_CUR ) == 0;
		}

		virtual bool SeekEnd (BytesU offset) override
		{
			return fseek( _file, int(offset), SEEK_END ) == 0;
		}
		
		virtual BytesU RemainingSize () const override
		{
			usize	size = 0;
			int		pos  = ftell( _file );

			fseek( _file, 0, SEEK_END );
			size = ftell( _file );
			fseek( _file, pos, SEEK_SET );

			return BytesU( size - pos );
		}

		virtual BytesU Size () const override
		{
			usize	size = 0;
			int		pos  = ftell( _file );

			fseek( _file, 0, SEEK_END );
			size = ftell( _file );
			fseek( _file, pos, SEEK_SET );

			return BytesU( size );
		}

		virtual BytesU Pos () const override
		{
			return BytesU( ftell( _file ) );
		}
		
		virtual bool IsEOF () const override
		{
			return Pos() >= Size();
		}

		virtual StringCRef  Name () const override
		{
			return _name;
		}

		virtual EFile::type  GetType () const override
		{
			return EFile::HDD;
		}


	private:
		void _Close ()
		{
			if ( IsOpened() )
			{
				fclose( _file );
				_file = null;
			}
			_name.Clear();
		}
	};
	

	SHARED_POINTER( HddRFile );
	SHARED_POINTER( HddWFile );

}	// File
}	// GX_STL
