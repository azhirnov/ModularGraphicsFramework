// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Types/RefCountedObject.h"

namespace GX_STL
{
namespace GXFile
{
	
	struct EFile
	{
		enum type
		{
			Unknown		= 0,
			HDD,
			Memory,
			Resource,	// platform depended file type
			SubFile,
			Crypted,
			Multithreaded,

			_Archive	= 0x1000,
			ZIP,
			LZMA,
		};

		forceinline static bool IsArchive (type value)
		{
			return value > _Archive;
		}
	};


	//
	// Base File interface
	//

	class BaseFile : protected RefCountedObject<>
	{
	// interface
	public:
		virtual ~BaseFile () {}

		virtual void Close () noexcept = 0;

		virtual bool SeekSet (BytesU offset) noexcept = 0;
		virtual bool SeekCur (BytesI offset) noexcept = 0;
		virtual bool SeekEnd (BytesU offset) noexcept = 0;

		ND_ virtual BytesU			RemainingSize ()	const noexcept = 0;
		ND_ virtual BytesU			Size ()				const noexcept = 0;
		ND_ virtual BytesU			Pos ()				const noexcept = 0;
		ND_ virtual bool			IsEOF ()			const noexcept = 0;
		ND_ virtual bool			IsOpened ()			const noexcept = 0;

		ND_ virtual EFile::type		GetType ()			const = 0;
		ND_ virtual StringCRef		Name ()				const = 0;
	};



	//
	// Read only File interface
	//

	class RFile : public BaseFile
	{
	// types
	public:
		SHARED_POINTER( RFile );


	// interface
	public:
		virtual BytesU ReadBuf (void * buf, BytesU size) noexcept = 0;
		
		virtual BytesU ReadBufFrom (void * buf, BytesU size, BytesU offset) noexcept
		{
			const BytesU	pos = Pos();
			SeekSet( offset );

			const BytesU	bufsize = ReadBuf( buf, size );
			SeekSet( pos );
			
			return bufsize;
		}


	// methods
	public:
		bool Read (void * buf, BytesU size)
		{
			return ReadBuf( buf, size ) == size;
		}

		template <typename T>
		bool Read (OUT T & value)
		{
			STATIC_ASSERT( not TypeTraits::IsConstOrVolatile<T> );
			STATIC_ASSERT( CompileTime::IsMemCopyFromFileAvailable<T> );
			return ReadBuf( &value, BytesU::SizeOf( value ) ) == BytesU::SizeOf(value);
		}

		template <typename T>
		bool Read (INOUT ArrayRef<T> data)
		{
			STATIC_ASSERT( not TypeTraits::IsConstOrVolatile<T> );
			STATIC_ASSERT( CompileTime::IsMemCopyFromFileAvailable<T> );
			return Read( data.ptr(), data.Size() );
		}

		template <typename T>
		ND_ T ReadValue ()
		{
			T value;
			Read( value );
			return value;
		}
	};



	//
	// Write only File interface
	//

	class WFile : public BaseFile
	{
	// types
	public:
		SHARED_POINTER( WFile );


	// interface
	public:
		virtual void Flush () noexcept = 0;

		virtual BytesU WriteBuf (const void * buf, BytesU size) noexcept = 0;


	// methods
	public:
		bool Write (const void * buf, BytesU size)
		{
			return WriteBuf( buf, size ) == size;
		}

		template <typename T>
		bool Write (const T &value)
		{
			STATIC_ASSERT( CompileTime::IsMemCopyFromFileAvailable<T> );
			return WriteBuf( &value, BytesU::SizeOf( value ) ) == BytesU::SizeOf(value);
		}

		template <typename T>
		bool Write (const ArrayRef<T> data)
		{
			STATIC_ASSERT( CompileTime::IsMemCopyFromFileAvailable<T> );
			return Write( data.RawPtr(), data.Size() );
		}

		template <typename T>
		bool Write (const TStringRef<T> data)
		{
			STATIC_ASSERT( CompileTime::IsMemCopyFromFileAvailable<T> );
			return Write( data.RawPtr(), data.LengthInBytes() );
		}
	};
	
		
	SHARED_POINTER( RFile );
	SHARED_POINTER( WFile );

}	// GXFile
}	// GX_STL
