// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/ResourcePacker/Images/ImageConverter.h"
#include "Engine/ResourcePacker/FilePacker/CppFileSystemPacker.h"
#include "Engine/ResourcePacker/Packer/ResourcePacker.h"

namespace ResPack
{
	using namespace GX_STL::GXMath;

/*
=================================================
	constructor
=================================================
*/
	ImageConverter::ImageConverter ()
	{
	}
	
/*
=================================================
	destructor
=================================================
*/
	ImageConverter::~ImageConverter ()
	{
	}
	
/*
=================================================
	SetFileSystem
=================================================
*/
	bool ImageConverter::SetFileSystem (const IFileSystemPackerPtr &fs)
	{
		_fileSystem = fs;
		return true;
	}
	
/*
=================================================
	SetCPPFileSystem
=================================================
*/
	bool ImageConverter::SetCPPFileSystem (const String &folder)
	{
		_fileSystem = new CppFileSystemPacker( folder );
		return true;
	}

/*
=================================================
	PackTexture
=================================================
*/
	bool ImageConverter::PackTexture (const String &filename, const String &outputName)
	{
		CHECK_ERR( _fileSystem );

		// check modification time
		{
			TimeL	src_time	= OS::FileSystem::GetFileLastModificationTime( filename ).ToTime();
			TimeL	dst_time	= _fileSystem->GetFileLastModificationTime( outputName ).ToTime();
			TimeL	exe_time	= OS::FileSystem::GetFileLastModificationTime( ResourcePacker::Instance()->GetExecutableName() ).ToTime();

			if ( Max( src_time, exe_time ) <= dst_time )
				return true;
		}

		ImageData	image;
		CHECK_ERR( _DevIL_Load( filename, OUT image ) );

		auto stream = _fileSystem->SaveBinary( outputName );

		CHECK_ERR( _ValidateImage( INOUT image ) );
		CHECK_ERR( _GX_Save( RVREF(stream), image ) );
		return true;
	}
	
/*
=================================================
	PackTexture2DArray
=================================================
*/
	bool ImageConverter::PackTexture2DArray (ArrayCRef<String> filenames, const String &outputName)
	{
		TODO( "" );
		return true;
	}
	
/*
=================================================
	PackCubeMap
=================================================
*/
	bool ImageConverter::PackCubeMap (const String &px, const String &py, const String &pz,
									const String &nx, const String &ny, const String &nz,
									const String &outputName)
	{
		TODO( "" );
		return true;
	}
	
/*
=================================================
	PackTexturesInFolder
=================================================
*/
	bool ImageConverter::PackTexturesInFolder (const String &folder)
	{
		CHECK_ERR( OS::FileSystem::IsDirectoryExist( folder ) );

		using Extension_t	= StaticString<8>;

		static const HashSet<Extension_t>	image_extensions = {
			"bmp", "jpg", "jpeg", "tga", "png", "dds", "ktx"
		};

		Array<String>	files;
		OS::FileSystem::GetAllFilesInPath( folder, OUT files );

		for (auto& fname : files)
		{
			Extension_t	ext = FileAddress::GetExtension( fname );

			StringUtils::StringToLower( INOUT ext );

			if ( not image_extensions.IsExist( ext  ) )
				continue;

			const String	path = FileAddress::BuildPath( folder, fname );

			CHECK_ERR( PackTexture( path, fname ) );
		}
		return true;
	}

/*
=================================================
	_GX_Save
=================================================
*/
	bool ImageConverter::_GX_Save (StreamPtr &&stream, const ImageData &image)
	{
		using Level_t	= Engine::ImportExport::GXImageFormat::Level;

		// store header
		stream->Append( BinArrayCRef::FromValue( image.header ) );

		// store level info
		BytesU	real_offset;
		BytesU	expected_offset	= SizeOf<Level_t> * image.header.layers * image.header.maxLevel;

		for (auto& level : image.mipmaps)
		{
			for (auto& layer : level)
			{
				Level_t	img_level = layer;

				img_level.memOffset	+= uint(expected_offset);

				stream->Append( BinArrayCRef::FromValue( img_level ) );

				real_offset += sizeof(img_level);
			}
		}

		CHECK_ERR( real_offset == expected_offset );

		// store pixels
		for (auto& buf : image.data)
		{
			stream->Append( buf );
		}
		return true;
	}
	
/*
=================================================
	_ValidateImage
=================================================
*/
	bool ImageConverter::_ValidateImage (INOUT ImageData &image)
	{
		CHECK_ERR( not image.data.Empty() and not image.mipmaps.Empty() );

		using EFlags		= Engine::ImportExport::GXImageFormat::EFlags;
		using OffsetsMap_t	= Map< void*, usize >;

		OffsetsMap_t	map_offsets;		// map { pointer to buffer data, data offset in file }
		BytesU			offset;

		for (auto& data : image.data)
		{
			map_offsets.Add( data.RawPtr(), usize(offset) );

			offset += data.Size();
		}


		uint3				dimension;
		usize				num_layers	= 0;
		EPixelFormat::type	format		= EPixelFormat::Unknown;

		FOR( i, image.mipmaps )
		{
			auto&	level = image.mipmaps[i];

			if ( i == 0 ) {
				num_layers = level.Count();
				CHECK_ERR( num_layers > 0 );
			}
			else {
				CHECK_ERR( num_layers == level.Count() );
			}

			uint3	layer_dim;

			FOR( j, level )
			{
				auto&	layer = level[i];

				if ( i == 0 ) {
					dimension	= layer.dimension;
					format		= layer.pixelFormat;
				}

				if ( j == 0 ) {
					layer_dim = layer.dimension;
					CHECK_ERR( All( layer_dim > 0 ) );
					CHECK_ERR( All( layer_dim == Max( dimension >> uint(j), 1u ) ) );
				}
				else {
					CHECK_ERR( All( layer_dim == layer.dimension ) );
				}

				CHECK_ERR( layer.layer == j );
				CHECK_ERR( layer.level == i );
				CHECK_ERR( format == layer.pixelFormat );

				OffsetsMap_t::iterator	iter;
				CHECK_ERR( map_offsets.Find( layer.dataPtr, OUT iter ) );
				layer.memOffset = uint(iter->second);
			}
		}

		image.header.version	= Engine::ImportExport::GXImageFormat::VERSION;
		image.header.width		= dimension.x;
		image.header.height		= dimension.y;
		image.header.depth		= dimension.z;
		image.header.layers		= uint(num_layers);
		image.header.maxLevel	= uint(image.mipmaps.Count());
		image.header.samples	= 1;
		image.header.flags		= EFlags::None;
		image.header.pixelFormat= format;
		image.header.blockSize	= Bytes<uint>(0);	// TODO

		return true;
	}

/*
=================================================
	BindAll
=================================================
*/
	void ImageConverter::BindAll (ScriptEnginePtr se)
	{
		_BindSelf( se );
	}
		
/*
=================================================
	_BindSelf
=================================================
*/
	void ImageConverter::_BindSelf (ScriptEnginePtr se)
	{
		using namespace GX_STL::GXScript;
		
		using Self = ImageConverter;

		ClassBinder<Self>		binder{ se, "ImagePacker" };

		binder.CreateClassValue();
		binder.AddMethod( &Self::SetCPPFileSystem,		"SetCPPVFS" );
		binder.AddMethod( &Self::PackTexture,			"PackTexture" );
		//binder.AddMethod( &Self::PackTexture2DArray,	"PackTexture2DArray" );
		binder.AddMethod( &Self::PackCubeMap,			"PackCubeMap" );
		binder.AddMethod( &Self::PackTexturesInFolder,	"PackTexturesInFolder" );
	}

}	// ResPack
