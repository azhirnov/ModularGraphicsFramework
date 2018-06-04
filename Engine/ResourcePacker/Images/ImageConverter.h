// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/ResourcePacker/Packer/Common.h"
#include "Engine/ResourcePacker/FilePacker/IFileSystemPacker.h"
#include "Engine/ImportExport/Impl/GX_ImageFormat.h"

namespace ResPack
{

	//
	// Image Converter
	//

	class ImageConverter final
	{
	// types
	public:
		using ScriptEnginePtr	= GXScript::ScriptEnginePtr;
		using EPixelFormat		= Engine::Platforms::EPixelFormat;
		using StreamPtr			= UniquePtr< IFileSystemPacker::IOutputBinStream >;

		struct ImageData;


	// variables
	private:
		IFileSystemPackerPtr	_fileSystem;


	// methods
	public:
		ImageConverter ();
		~ImageConverter ();
		
		bool PackTexture (const String &filename, const String &outputName);
		bool PackTexture2DArray (ArrayCRef<String> filenames, const String &outputName);
		bool PackCubeMap (const String &px, const String &py, const String &pz,
						  const String &nx, const String &ny, const String &nz,
						  const String &outputName);

		bool PackTexturesInFolder (const String &folder);

		bool SetFileSystem (const IFileSystemPackerPtr &fs);
		bool SetCPPFileSystem (const String &folder);

		static void BindAll (ScriptEnginePtr se);

	private:
		static void _BindSelf (ScriptEnginePtr se);

		static bool _DevIL_Load (StringCRef filename, OUT ImageData &image);

		static bool _GX_Save (StreamPtr &&stream, const ImageData &image);
		static bool _ValidateImage (INOUT ImageData &image);
	};


	
	//
	// Image
	//

	struct ImageConverter::ImageData final
	{
	// types
		struct MipLevel : Engine::ImportExport::GXImageFormat::Level
		{
			EPixelFormat::type	pixelFormat		= Uninitialized;
			void *				dataPtr			= null;
		};

		using Header_t		= Engine::ImportExport::GXImageFormat::Header;
		using Layers_t		= Array< MipLevel >;
		using Mipmaps_t		= Array< Layers_t >;
		using Chunks_t		= Array< BinaryArray >;

	// variables
		Chunks_t		data;
		Mipmaps_t		mipmaps;
		Header_t		header;


	// methods
		ImageData ()
		{
			Clear();
		}

		void Clear ()
		{
			data.Clear();
			mipmaps.Clear();
			ZeroMem( header );
		}
	};

}	// ResPack
