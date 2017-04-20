// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "MemFile.h"

#ifdef GX_ZLIB_ENABLED

#include "External/Zip/zip.h"

namespace GX_STL
{
namespace File
{

	//
	// Zip Archive
	//

	struct ZipArchive : public Noncopyable
	{
	// variables
	private:
		unzFile				_file;
		zlib_filefunc_def	_opaque;
		RFilePtr			_rfile;


	// methods
	public:
		ZipArchive () : _file(null)
		{}

		~ZipArchive ()
		{
			Close();
		}

		unzFile	Get () const
		{
			return _file;
		}

		bool Open (StringCRef filename)
		{
			Close();
			_file = unzOpen( filename.cstr() );
			return IsOpened();
		}

		bool Open (const RFilePtr &file)
		{
			Close();

			CHECK_ERR( file and file->IsOpened() );

			_opaque.zopen_file	= &_FileWrapper::open_file_func;
			_opaque.zread_file	= &_FileWrapper::read_file_func;
			_opaque.zwrite_file	= &_FileWrapper::write_file_func;
			_opaque.ztell_file	= &_FileWrapper::tell_file_func;
			_opaque.zseek_file	= &_FileWrapper::seek_file_func;
			_opaque.zclose_file	= &_FileWrapper::close_file_func;
			_opaque.zerror_file	= &_FileWrapper::testerror_file_func;
			_opaque.opaque		= file.ptr();

			_rfile = file;
			_file  = unzOpen2( file->Name().cstr(), &_opaque );
			return IsOpened();
		}

		bool IsOpened () const
		{
			return _file != null;
		}

		void Close ()
		{
			if ( _file != null )
			{
				CHECK( unzCloseCurrentFile( _file ) == UNZ_OK );

				CHECK( unzClose( _file ) == UNZ_OK );

				_file = null;
			}

			ZeroMem( _opaque );
			_rfile = null;
		}

		bool IsFileExist (StringCRef filename) const
		{
			return unzLocateFile( _file, filename.cstr(), 0 ) == UNZ_OK;
		}


		template <typename Iterator>
		bool EnumFiles (Iterator &iter)
		{
			String	fname;
			fname.Resize( 256 );

			int err = UNZ_OK;

			while ( err == UNZ_OK )
			{
				fname.Clear();
				unzGetCurrentFileInfo( _file, null, fname.ptr(), fname.Capacity(), null, 0, null, 0 );
				fname.CalculateLength();

				bool	is_dir	 = fname.Back() == '\\' or fname.Back() == '/';

				if ( not is_dir )
				{
					iter( fname );
				}

				err = unzGoToNextFile( _file );

				if ( err == UNZ_END_OF_LIST_OF_FILE )
					return true;
			}
			return false;
		}

		
		template <typename Iterator>
		bool EnumDirectories (Iterator &iter)
		{
			String	fname;
			fname.Resize( 256 );

			int err = UNZ_OK;

			while ( err == UNZ_OK )
			{
				fname.Clear();
				unzGetCurrentFileInfo( _file, null, fname.ptr(), fname.Capacity(), null, 0, null, 0 );
				fname.CalculateLength();

				bool	is_dir	 = fname.Back() == '\\' or fname.Back() == '/';

				if ( is_dir )
				{
					iter( fname );
				}

				err = unzGoToNextFile( _file );

				if ( err == UNZ_END_OF_LIST_OF_FILE )
					return true;
			}
			return false;
		}

	// types
	private:
		struct _FileWrapper
		{
			static voidpf	ZCALLBACK open_file_func	(voidpf opaque, const char* filename, int mode)
			{
				Ptr< RFile >	file = (RFile *) opaque;
				return ReferenceCast<void *>( usize(1) );
			}

			static uLong	ZCALLBACK read_file_func	(voidpf opaque, voidpf stream, void* buf, uLong size)
			{
				Ptr< RFile >	file = (RFile *) opaque;
				return file->ReadBuf( buf, size );
			}

			static uLong	ZCALLBACK write_file_func	(voidpf opaque, voidpf stream, const void* buf, uLong size)
			{
				return 0;
			}

			static int		ZCALLBACK close_file_func	(voidpf opaque, voidpf stream)
			{
				Ptr< RFile >	file = (RFile *) opaque;
				file->Close();
				return 0;
			}

			static int		ZCALLBACK testerror_file_func	(voidpf opaque, voidpf stream)
			{
				return 0;
			}

			static long		ZCALLBACK tell_file_func	(voidpf opaque, voidpf stream)
			{
				Ptr< RFile >	file = (RFile *) opaque;
				return file->Pos();
			}

			static long		ZCALLBACK seek_file_func	(voidpf opaque, voidpf stream, uLong offset, int origin)
			{
				Ptr< RFile >	file = (RFile *) opaque;

				switch ( origin )
				{
					case SEEK_SET :		file->SeekSet( offset );	break;
					case SEEK_CUR :		file->SeekCur( offset );	break;
					case SEEK_END :		file->SeekEnd( offset );	break;
					default :			RETURN_ERR( "invalid seek falg" );
				}

				return 0;
			}
		};
	};



	//
	// Zip Read File
	//

	class ZipRFile : public BaseMemRFile
	{
	// types
	public:
		SHARED_POINTER( ZipRFile );

		typedef BaseMemRFile	parent_t;


	// variables
	private:
		String	_name;
		

	// methods
	public:
		ZipRFile ()		{}
		~ZipRFile ()	{ _Close(); }

		
		static ZipRFilePtr New ()
		{
			return new ZipRFile();
		}


		static ZipRFilePtr New (StringCRef archiveName, StringCRef fileInArchive,
								StringCRef password = StringCRef())
		{
			ZipRFilePtr	zfile = new ZipRFile();

			if ( zfile->Create( archiveName, fileInArchive, password ) )
				return zfile;

			return null;
		}


		static ZipRFilePtr New (const RFilePtr &file, StringCRef fileInArchive,
								StringCRef password = StringCRef())
		{
			ZipRFilePtr	zfile = new ZipRFile();

			if ( zfile->Create( file, fileInArchive, password ) )
				return zfile;

			return null;
		}


		bool Create (StringCRef archiveName, StringCRef fileInArchive, StringCRef password)
		{
			_Close();

			ZipArchive		pak;
			CHECK_ERR( pak.Open( archiveName ) );

			_name << archiveName << '|' << fileInArchive;

			return _Create( pak, fileInArchive, password );
		}


		bool Create (const RFilePtr &file, StringCRef fileInArchive, StringCRef password)
		{
			CHECK_ERR( this != file.ptr() );
			CHECK_ERR( file and file->IsOpened() );

			_Close();

			ZipArchive		pak;
			CHECK_ERR( pak.Open( file ) );
			
			_name << file->Name() << '|' << fileInArchive;

			return _Create( pak, fileInArchive, password );
		}


		// BaseFile //
		virtual void Close () override
		{
			return _Close();
		}

		virtual StringCRef	Name () const override
		{
			return _name;
		}

		virtual EFile::type		GetType () const override
		{
			return EFile::ZIP;
		}


	private:
		bool _Create (ZipArchive &pak, StringCRef fileInArchive, StringCRef password)
		{
			unz_file_info	info;
			
			CHECK_ERR( unzLocateFile( pak.Get(), fileInArchive.cstr(), 0 ) == UNZ_OK );
			CHECK_ERR( unzGetCurrentFileInfo( pak.Get(), &info, null, 0, null, 0, null, 0 ) == UNZ_OK );
			
			if ( password.Empty() ) {
				CHECK_ERR( unzOpenCurrentFile( pak.Get() ) == UNZ_OK );
			}
			else {
				CHECK_ERR( unzOpenCurrentFilePassword( pak.Get(), password.cstr() ) == UNZ_OK );
			}

			usize	size = info.uncompressed_size;
			
			_pos	= 0;
			_opened = true;

			_mem.Resize( size, false );

			int		readn	= unzReadCurrentFile( pak.Get(), _mem.ptr(), _mem.Size() );
			uint	offset	= 0;

			while ( readn > 0 and offset < _mem.Size() )
			{
				offset += readn;
				readn	= unzReadCurrentFile( pak.Get(), _mem.ptr() + offset, _mem.Size() - offset );
			}
			
			CHECK_ERR( offset == _mem.Size() );

			return true;
		}


		void _Close ()
		{
			parent_t::_Close();
			_name.Clear();
		}
	};



	//
	// Zip Write File
	//

	class ZipWFile : public BaseMemWFile
	{
	// types
	public:
		SHARED_POINTER( ZipWFile );

		typedef BaseMemWFile	parent_t;


	// methods
	public:
		ZipWFile ()		{}
		~ZipWFile ()	{}

		
		// BaseFile //
		virtual EFile::type		GetType () const override
		{
			return EFile::ZIP;
		}
	};
	
		
	SHARED_POINTER( ZipRFile );
	SHARED_POINTER( ZipWFile );

}	// File
}	// GX_STL

#endif	// GX_ZLIB_ENABLED
