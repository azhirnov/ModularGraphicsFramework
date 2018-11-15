// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/ResourcePacker/FilePacker/CppFileSystemPacker.h"
#include "Core/STL/Compression/LZ4Compression.h"
#include "Core/STL/Compression/MiniZCompression.h"

namespace ResPack
{
	using namespace GX_STL::GXMath;

/*
=================================================
	constructor
=================================================
*/
	CppFileSystemPacker::OutputTextStream::OutputTextStream (StringCRef funcName, StringCRef nameSpace, const GXFile::WFilePtr &file) :
		_file{ file }, _length{ 0 }
	{
		CHECK( _file and _file->IsOpened() );

		String	header;
		header	<<	"// This is generated file, don't change anything!\n\n"
					"#include \"" << nameSpace << "_main.h\"\n\n"
					"namespace " << nameSpace << "\n{\n"
					"\textern void " << funcName << " (OUT BinaryArray &data)\n\t{\n"
					"\t\tdata << BinArrayCRef::From(R\"#(";

		CHECK( _file->Write( StringCRef(header) ) );
	}

/*
=================================================
	destructor
=================================================
*/
	CppFileSystemPacker::OutputTextStream::~OutputTextStream ()
	{
		if ( _file )
		{
			String	src;
			src <<	")#\"_ref;\n"
					"\t}\n"
					"}\n";

			CHECK( _file->Write( StringCRef(src) ) );

			_file->Close();
		}
	}
	
/*
=================================================
	Append
=================================================
*/
	bool CppFileSystemPacker::OutputTextStream::Append (StringCRef data)
	{
		constexpr usize	max_len = GX_MAX_STRING_LENGTH_BYTES / sizeof(StringCRef::Value_t) - 100;
		
		for (usize s = 0; s < data.Count();)
		{
			const usize	len = Min( data.Count() - s + _length, max_len );

			CHECK( _file->Write( data.SubString( s, len ) ) );
			CHECK( _file->Write( StringCRef(")#\"_ref;\n\t\tdata << R\"#(") ) );

			s		+= len;
			_length	+= len;
		}

		_pos += data.LengthInBytes();
		return true;
	}
//-----------------------------------------------------------------------------
	


/*
=================================================
	constructor
=================================================
*/
	CppFileSystemPacker::OutputBinStream::OutputBinStream (StringCRef funcName, StringCRef nameSpace, const GXFile::WFilePtr &file) :
		_file{ file }
	{
		CHECK( _file and _file->IsOpened() );

		_buf.Reserve( 2u << 20 );
		_buf <<	"// This is generated file, don't change anything!\n\n"
				"#include \"" << nameSpace << "_main.h\"\n"
				"#include \"Core/STL/Compression/LZ4Compression.h\"\n"
				"#include \"Core/STL/Compression/MiniZCompression.h\"\n"
				"#include \"Core/STL/Types/UniquePtr.h\"\n\n"
				"namespace " << nameSpace << "\n{\n"
				"\textern void " << funcName << " (OUT BinaryArray &data)\n\t{\n"
				"\t\tdata << BinArrayCRef{\n";

		CHECK( _file->Write( StringCRef(_buf) ) );
	}

/*
=================================================
	destructor
=================================================
*/
	CppFileSystemPacker::OutputBinStream::~OutputBinStream ()
	{
		if ( _file )
		{
			using CompressorPtr = UniquePtr< GXCompression::ICompressor >;

			constexpr usize		max_len			= (32u << 10) / sizeof(decltype(_buf)::Value_t) - 100;
			constexpr usize		max_line		= 140;
			usize				last_line_break	= 0;
			CompressorPtr		compressor		{ new GXCompression::MiniZCompressor() };
			BinaryArray			comp_buf;		comp_buf.Resize( usize(compressor->GetPrefferedSize( _data.Size() )), false );
			BinArrayRef			compressed		= comp_buf;

			CHECK( compressor->Compress( _data, INOUT compressed ) );

			_buf.Clear();
		
			for (usize i = 0; i < compressed.Count(); ++i)
			{
				const ubyte	val		= compressed[i];
				const char	hex[]	= "0123456789ABCDEF";
				const char	str[]	= { '0', 'x', hex[(val >> 4) & 0xF], hex[val & 0xF], '\0' };

				_buf << str;

				const usize	cnt = _buf.Length();

				if ( cnt > max_len )
				{
					_buf <<	"};\n"
							"\t\tdata << BinArrayCRef{\n";

					CHECK( _file->Write( StringCRef(_buf) ) );

					_buf.Clear();
					last_line_break	= 0;
				}
				else
				{
					_buf << ", ";

					if ( cnt - last_line_break >= max_line ) {
						_buf << '\n';
						last_line_break = cnt;
					}
				}
			}

			if ( not _buf.Empty() )
			{
				CHECK( _file->Write( StringCRef(_buf) ) );
			}

			_buf.Clear();
			_buf <<	"};\n"
					"		const usize uncompressed_size = " << _data.Count() << ";\n"
					"		UniquePtr<GXCompression::IDecompressor> decompressor { new GXCompression::MiniZDecompressor() };\n"
					"		BinaryArray	temp;	temp.Resize( usize(decompressor->GetPrefferedSize( data.Size() )), false );\n"
					"		BinArrayRef	uncompressed = temp;\n"
					"		if ( not decompressor->Decompress( data, uncompressed ) ) {\n"
					"			data.Clear();\n"
					"			return;\n"
					"		}\n"
					"		temp.Resize( uncompressed.Count() );\n"
					"		data = RVREF(temp);\n"
					"	}\n"
					"}\n";

			CHECK( _file->Write( StringCRef(_buf) ) );

			_file->Close();
		}
	}
	
/*
=================================================
	Append
=================================================
*/
	bool CppFileSystemPacker::OutputBinStream::Append (BinArrayCRef data)
	{
		_data << data;
		return true;

		/*constexpr usize		max_len		= (32u << 10) / sizeof(decltype(_buf)::Value_t) - 100;
		constexpr usize		max_line	= 140;
		
		_buf.Clear();
		
		for (usize i = 0; i < data.Count(); ++i)
		{
			const ubyte	val		= data[i];
			const char	hex[]	= "0123456789ABCDEF";
			const char	str[]	= { '0', 'x', hex[(val >> 4) & 0xF], hex[val & 0xF], '\0' };

			_buf << str;

			const usize	cnt = _buf.Length() + _length;

			if ( cnt > max_len )
			{
				_buf <<	"};\n"
						"\t\tdata << BinArrayCRef{\n";

				CHECK( _file->Write( StringCRef(_buf) ) );

				_buf.Clear();
				_length			= 0;
				_lastLineBreak	= 0;
			}
			else
			{
				_buf << ", ";

				if ( cnt - _lastLineBreak >= max_line ) {
					_buf << '\n';
					_lastLineBreak = cnt;
				}
			}
		}

		if ( not _buf.Empty() )
		{
			_length += _buf.Count();

			CHECK( _file->Write( StringCRef(_buf) ) );
			_buf.Clear();
		}

		_pos += data.Size();
		return true;*/
	}
//-----------------------------------------------------------------------------


	
/*
=================================================
	constructor
=================================================
*/
	CppFileSystemPacker::CppFileSystemPacker (StringCRef folder)
	{
		_vfsName = folder;
		_vfsName.ReplaceChars( '\\', '_' );
		_vfsName.ReplaceChars( '/', '_' );
		_vfsName.ReplaceChars( '.', '_' );

		for (usize pos = 0; _vfsName.FindAndChange( "__", "_", OUT pos );) {}

		if ( _vfsName[0] == '_' )
			_vfsName.Erase( 0 );

		if ( _vfsName.Empty() )
			_vfsName = "packed_files";

		String	dir;
		OS::FileSystem::GetCurrentDirectory( OUT dir );

		_folder = FileAddress::BuildPath( dir, folder );

		CHECK( OS::FileSystem::CreateDirectories( _folder ) );
	}

/*
=================================================
	destructor
=================================================
*/
	CppFileSystemPacker::~CppFileSystemPacker ()
	{
		_Close();
	}
		
/*
=================================================
	_Close
=================================================
*/
	void CppFileSystemPacker::_Close ()
	{
		if ( _functions.Empty() )
			return;

		const String	name_space	= "vfs_"_str << _vfsName;
		const String	header_name	= "vfs_"_str << _vfsName << "_main.h";
		const String	source_name	= "vfs_"_str << _vfsName << "_main.cpp";

		// header
		{
			const String		fname	= FileAddress::BuildPath( _folder, header_name );
			GXFile::WFilePtr	file	= GXFile::HddWFile::New( fname );

			if ( file )
			{
				String	str;
				str <<	"// This is generated file, don't change anything!\n\n"
						"#include \"Core/STL/Containers/StringRef.h\"\n"
						"#include \"Core/STL/Containers/Array.h\"\n\n"
						"namespace " << name_space << "\n{\n"
						"\tusing namespace GX_STL;\n"
						"\tusing namespace GX_STL::GXTypes;\n\n"
						"\tvoid (* GetBuiltinFileLoader (StringCRef filename)) (OUT BinaryArray &);\n\n"
						"}\n";
				
				CHECK( file->Write( StringCRef(str) ) );
			}
		}
		
		// source
		{
			const String		fname	= FileAddress::BuildPath( _folder, source_name );
			GXFile::WFilePtr	file	= GXFile::HddWFile::New( fname );

			if ( file )
			{
				String	str;
				str <<	"// This is generated file, don't change anything!\n\n"
						"#include \"" << header_name << "\"\n"
						"#include \"Core/STL/Containers/HashMap.h\"\n\n"
						"namespace " << name_space << "\n{\n\n";

				FOR( i, _functions ) {
					str << "\textern void " << _functions[i].first << " (OUT BinaryArray &data);\n";
				}

				str <<	"\n\tvoid (* GetBuiltinFileLoader (StringCRef filename)) (OUT BinaryArray &)\n\t{\n"
						"		static const HashMap< String, void (*) (OUT BinaryArray &) > vfs = {\n";
				
				FOR( i, _functions ) {
					str << (i ? ",\n" : "") << "\t\t\t{ \"" << _functions[i].second << "\", &" << _functions[i].first << " }";
				}
				
				str <<	"		};\n\n"
						"		usize idx = UMax;\n"
						"		if ( vfs.FindIndex( filename, OUT idx ) ) {\n"
						"			return vfs[idx].second;\n"
						"		}\n"
						"		return null;\n"
						"	}\n\n"
						"}\n";

				CHECK( file->Write( StringCRef(str) ) );
			}
		}
	}
	
/*
=================================================
	SaveText
=================================================
*/
	UniquePtr<CppFileSystemPacker::IOutputTextStream>  CppFileSystemPacker::SaveText (StringCRef filename)
	{
		String				fname		= FileAddress::BuildPath( _folder, FileAddress::GetName( filename ), "cpp" );
		String				func_name	= "VFS_"_str << _vfsName << '_' << FileAddress::GetName( filename );
		String				name_space	= "vfs_"_str << _vfsName;
		GXFile::WFilePtr	file;
		
		func_name.ReplaceChars( '\\', '_' );
		func_name.ReplaceChars( '/', '_' );
		func_name.ReplaceChars( '.', '_' );

		CHECK_ERR( file = GXFile::HddWFile::New( fname ) );
		CHECK_ERR( not _functions.IsExist( func_name ) );

		_functions.Add( func_name, filename );

		return UniquePtr<IOutputTextStream>{ new OutputTextStream{ func_name, name_space, file } };
	}
	
/*
=================================================
	SaveBinary
=================================================
*/
	UniquePtr<CppFileSystemPacker::IOutputBinStream>  CppFileSystemPacker::SaveBinary (StringCRef filename)
	{
		String				fname		= FileAddress::BuildPath( _folder, FileAddress::GetName( filename ), "cpp" );
		String				func_name	= "VFS_"_str << _vfsName << '_' << FileAddress::GetName( filename );
		String				name_space	= "vfs_"_str << _vfsName;
		GXFile::WFilePtr	file;
		
		func_name.ReplaceChars( '\\', '_' );
		func_name.ReplaceChars( '/', '_' );
		func_name.ReplaceChars( '.', '_' );

		CHECK_ERR( file = GXFile::HddWFile::New( fname ) );
		CHECK_ERR( not _functions.IsExist( func_name ) );

		_functions.Add( func_name, filename );

		return UniquePtr<IOutputBinStream>{ new OutputBinStream{ func_name, name_space, file } };
	}
	
/*
=================================================
	GetFileLastModificationTime
=================================================
*/
	Date  CppFileSystemPacker::GetFileLastModificationTime (StringCRef filename) const
	{
		String	path = FileAddress::BuildPath( _folder, FileAddress::GetName( filename ), "cpp" );

		return OS::FileSystem::GetFileLastModificationTime( path );
	}

}	// ResPack
