// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "MemFile.h"

#ifdef GX_LZMA_ENABLED

#include "External/LZMA/lzma.h"

namespace GX_STL
{
namespace File
{

	//
	// LZMA Read File
	//

	class LzmaRFile : public BaseMemRFile
	{
	// types
	public:
		SHARED_POINTER( LzmaRFile );
		
		typedef BaseMemRFile	parent_t;

		struct LzmaFileHeader : PODType
		{
			enum {
				VERSION	= 1
			};

			uint	version;
			uint	uncompressedSize;
			uint	propsSize;
			ubyte	props[LZMA_PROPS_SIZE];
			///
			LzmaFileHeader ()
			{
				ZeroMem( *this );
				version = VERSION;
			}
		};

		
	// variables
	private:
		String	_name;
		

	// methods
	public:
		LzmaRFile ()		{}
		~LzmaRFile ()		{ _Close(); }

		
		static LzmaRFilePtr New ()
		{
			return new LzmaRFile();
		}


		static LzmaRFilePtr New (const RFilePtr &file)
		{
			LzmaRFilePtr	lfile = new LzmaRFile();

			if ( lfile->Create( file ) )
				return lfile;

			return null;
		}


		bool Create (const RFilePtr &file)
		{
			_Close();

			CHECK_ERR( file and file->IsOpened() );
			
			LzmaFileHeader	header;
			file->Read( header );

			CHECK_ERR( header.version == LzmaFileHeader::VERSION );

			usize			src_size = file->RemainingSize();
			usize			dst_size = header.uncompressedSize;
			BinaryArray	src;

			src.Resize( src_size, false );
			CHECK_ERR( file->Read( src.ptr(), src_size ) );

			_mem.Resize( dst_size );
			
			int res = LzmaUncompress( _mem.ptr(), &dst_size, src.ptr(), &src_size, header.props, header.propsSize );

			_mem.Resize( dst_size );
			_name = file->Name();

			switch ( res )
			{
				case SZ_OK :
					_pos	= 0;
					_opened = true;
					return true;

				case SZ_ERROR_DATA			:	RETURN_ERR( "Error in data" );
				case SZ_ERROR_MEM			:	RETURN_ERR( "Memory allocation arror" );
				case SZ_ERROR_UNSUPPORTED	:	RETURN_ERR( "Unsupported properties" );
				case SZ_ERROR_INPUT_EOF		:	RETURN_ERR( "Input buffer is too low" );
				default						:	RETURN_ERR( "Unknown error: " << res );
			}
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
			return EFile::LZMA;
		}


	private:
		void _Close ()
		{
			parent_t::_Close();
			_name.Clear();
		}
	};



	//
	// LZMA Write File
	//

	class LzmaWFile : public BaseMemWFile
	{
	// types
	public:
		SHARED_POINTER( LzmaWFile );

		struct ECompression
		{
			enum type
			{
				VeryFast = 0,
				Fast,
				Normal,
				Maximal,
				Ultra,

				Default = Normal,
			};
		};

	private:
		typedef BaseMemWFile	parent_t;

		typedef LzmaRFile::LzmaFileHeader	LzmaFileHeader;

		static const uint	_ADDITION_SIZE	= 10;

		struct _Properties
		{
			int			level;			// 0 <= level <= 9, default = 5
			uint		dictSize;		// default = (1 << 24)
			int			lc;				// 0 <= lc <= 8, default = 3
			int			lp;				// 0 <= lp <= 4, default = 0
			int			pb;				// 0 <= pb <= 4, default = 2
			int			fb;				// 5 <= fb <= 273, default = 32
			const int	numThreads;		// 1 or 2, default = 2
			///
			_Properties () : numThreads(1)
			{
				SetDefault();
			}
			
			_Properties (ECompression::type mode, uint fileSize) : numThreads(1)
			{
				switch (mode)
				{
					case ECompression::VeryFast :
						level		= 0;
						dictSize	= 1<<14;
						lc			= 3;
						lp			= 0;
						pb			= 2;
						fb			= 8;
						break;

					case ECompression::Fast :
						level		= 2;
						dictSize	= 1<<18;
						lc			= 3;
						lp			= 0;
						pb			= 2;
						fb			= 16;
						break;

					case ECompression::Normal :
						level		= 5;
						dictSize	= 1<<24;
						lc			= 3;
						lp			= 0;
						pb			= 2;
						fb			= 32;
						break;

					case ECompression::Maximal :
						level		= 7;
						dictSize	= 1<<26;
						lc			= 3;
						lp			= 0;
						pb			= 2;
						fb			= 64;
						break;

					case ECompression::Ultra :
						level		= 9;
						dictSize	= 1<<26;
						lc			= 3;
						lp			= 0;
						pb			= 2;
						fb			= 64;
						break;

					default:
						SetDefault();
						return;
				}

				Validate();
			}

			void SetDefault ()
			{
				level		= 5;
				dictSize	= 1<<24;
				lc			= 3;
				lp			= 0;
				pb			= 2;
				fb			= 32;
			}

			void Validate ()
			{
				using namespace GXMath;

				level		= Clamp( level,		0,		9 );
				lc			= Clamp( lc,		0,		8 );
				lp			= Clamp( lp,		0,		4 );
				pb			= Clamp( pb,		0,		4 );
				fb			= Clamp( fb,		5,		273 );
				dictSize	= Clamp( dictSize,	1u<<12,	1u<<27 );

				if ( not IsPowerOfTwo( dictSize ) )		dictSize = 1u << IntLog2( dictSize );
			}
		};


	// methods
	public:
		LzmaWFile ()		{}
		~LzmaWFile ()		{ _Close(); }
		

		static LzmaWFilePtr New (BytesU reserve = 0)
		{
			LzmaWFilePtr	file = new LzmaWFile();

			if ( file->Create( reserve ) )
				return file;

			return null;
		}


		bool Create (BytesU reserve)
		{
			_mem.Clear();
			_mem.Reserve( reserve );

			_pos	= 0;
			_opened	= true;

			return true;
		}


		bool Save (const WFilePtr &file, ECompression::type compression = ECompression::Default) const
		{
			CHECK_ERR( file and file->IsOpened() );
			CHECK_ERR( not _mem.Empty() );

			BinaryArray	dst;
			dst.Resize( _mem.Count() + _ADDITION_SIZE, false );
			
			LzmaFileHeader	header;
			header.propsSize		= sizeof(header.props);
			header.uncompressedSize	= _mem.Count();

			usize	dst_size	= dst.Count();

			const _Properties	properties( compression, _mem.Count() );

			int res = LzmaCompress( dst.ptr(), &dst_size, _mem.ptr(), _mem.Count(),
									header.props, &header.propsSize,
									properties.level, properties.dictSize,
									properties.lc, properties.lp,
									properties.pb, properties.fb,
									properties.numThreads );

			dst.Resize( dst_size );
			
			switch ( res )
			{
				case SZ_OK					:	break;
				case SZ_ERROR_MEM			:	RETURN_ERR( "Memory allocation error" );
				case SZ_ERROR_PARAM			:	RETURN_ERR( "Incorrect paramater" );
				case SZ_ERROR_OUTPUT_EOF	:	RETURN_ERR( "Output buffer overflow" );
				case SZ_ERROR_THREAD		:	RETURN_ERR( "Errors in multithreading functions" );
				default						:	RETURN_ERR( "Unknown error: " << res );
			}

			bool writen = true;

			writen &= file->Write( header );
			writen &= file->Write( dst.ptr(), dst.Size() );

			CHECK_ERR( writen );
			return true;
		}
		

		// BaseFile //
		virtual StringCRef	Name () const override
		{
			return StringCRef();
		}

		virtual EFile::type		GetType () const override
		{
			return EFile::LZMA;
		}
	};

	
	SHARED_POINTER( LzmaRFile );
	SHARED_POINTER( LzmaWFile );


}	// File
}	// GX_STL

#endif	// GX_LZMA_ENABLED
