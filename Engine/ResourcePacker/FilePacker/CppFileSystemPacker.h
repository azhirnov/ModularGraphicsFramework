// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Store files as builtin virtual file system.
*/

#pragma once

#include "Engine/ResourcePacker/FilePacker/IFileSystemPacker.h"

namespace ResPack
{

	//
	// CPP File System Packer
	//

	class CppFileSystemPacker final : public IFileSystemPacker
	{
	// types
	private:
		class OutputTextStream final : public IOutputTextStream
		{
		// variables
		private:
			GXFile::WFilePtr	_file;
			usize				_length;
			BytesU				_pos;

		// methods
		public:
			OutputTextStream (StringCRef funcName, StringCRef nameSpace, const GXFile::WFilePtr &file);
			~OutputTextStream () override;

			bool   Append (StringCRef data) override;
			BytesU GetPosition () const override			{ return _pos; }
		};
		

		class OutputBinStream final : public IOutputBinStream
		{
		// variables
		private:
			GXFile::WFilePtr	_file;
			String				_buf;
			BinaryArray			_data;

			/*usize				_length;
			usize				_lastLineBreak;
			BytesU				_pos;*/

		// methods
		public:
			OutputBinStream (StringCRef funcName, StringCRef nameSpace, const GXFile::WFilePtr &file);
			~OutputBinStream () override;

			bool   Append (BinArrayCRef data) override;
			BytesU GetPosition () const override			{ return _data.Size(); }
		};


	// variables
	private:
		String						_folder;
		String						_vfsName;
		HashMap<String, String>		_functions;		// { function name, file name }


	// methods
	public:
		explicit CppFileSystemPacker (StringCRef folder);

		~CppFileSystemPacker () override;
		
		UniquePtr<IOutputTextStream>  SaveText (StringCRef filename) override;
		UniquePtr<IOutputBinStream>   SaveBinary (StringCRef filename) override;

		Date  GetFileLastModificationTime (StringCRef filename) const override;

	private:
		void _Close ();
	};


}	// ResPack
