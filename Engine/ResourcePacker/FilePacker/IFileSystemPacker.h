// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/ResourcePacker/Packer/Common.h"

namespace ResPack
{

	//
	// File System Packer interface
	//

	class IFileSystemPacker : public RefCountedObject<>
	{
	// types
	public:
		class IOutputTextStream
		{
		// interface
		public:
			virtual ~IOutputTextStream () {}

			virtual bool Append (StringCRef data) = 0;
			virtual BytesU GetPosition () const = 0;
		};
		
		class IOutputBinStream
		{
		// interface
		public:
			virtual ~IOutputBinStream () {}

			virtual bool Append (BinArrayCRef data) = 0;
			virtual BytesU GetPosition () const = 0;
		};


	// interface
	public:
		virtual ~IFileSystemPacker () {}

		virtual UniquePtr<IOutputTextStream>  SaveText (StringCRef filename) = 0;
		virtual UniquePtr<IOutputBinStream>   SaveBinary (StringCRef filename) = 0;
		
		virtual Date  GetFileLastModificationTime (StringCRef filename) const = 0;
	};

	SHARED_POINTER( IFileSystemPacker );

}	// ResPack
