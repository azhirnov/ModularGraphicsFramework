// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Files/BaseFile.h"

namespace GX_STL
{
namespace File
{

	//
	// Read only multithread file
	//

	class MtRFile : public RFile
	{
	// variables
	private:
		RFilePtr			_file;
		mutable OS::Mutex	_lock;


	// methods
	public:
		explicit
		MtRFile (const RFilePtr &file) : _file(file)
		{}


		static SHARED_POINTER_TYPE( MtRFile )  New (const RFilePtr &file)
		{
			ASSERT( file->GetType() != EFile::Multithreaded );
			return new MtRFile( file );
		}

		
		// RFile //
		virtual BytesU ReadBuf (void * buf, BytesU size) override
		{
			SCOPELOCK( _lock );
			return _file->ReadBuf( buf, size );
		}
		
		virtual BytesU ReadBufFrom (void * buf, BytesU size, BytesU offset) override
		{
			SCOPELOCK( _lock );

			const BytesU	pos = _file->Pos();
			_file->SeekSet( offset );

			const BytesU	result = _file->ReadBuf( buf, size );
			_file->SeekSet( pos );
			
			return result;
		}


		// BaseFile //
		virtual void Close () override
		{
			SCOPELOCK( _lock );
			_file->Close();
		}
		
		virtual bool IsOpened () const override
		{
			SCOPELOCK( _lock );
			return _file.IsNotNull();
		}

		virtual bool SeekSet (BytesU offset) override
		{
			SCOPELOCK( _lock );
			return _file->SeekSet( offset );
		}

		virtual bool SeekCur (BytesI offset) override
		{
			SCOPELOCK( _lock );
			return _file->SeekCur( offset );
		}

		virtual bool SeekEnd (BytesU offset) override
		{
			SCOPELOCK( _lock );
			return _file->SeekEnd( offset );
		}
		
		virtual BytesU RemainingSize () const override
		{
			SCOPELOCK( _lock );
			return _file->RemainingSize();
		}

		virtual BytesU Size () const override
		{
			SCOPELOCK( _lock );
			return _file->Size();
		}

		virtual BytesU Pos () const override
		{
			SCOPELOCK( _lock );
			return _file->Pos();
		}
		
		virtual bool IsEOF () const override
		{
			SCOPELOCK( _lock );
			return _file->IsEOF();
		}

		virtual StringCRef  Name () const override
		{
			SCOPELOCK( _lock );
			return _file->Name();
		}

		virtual EFile::type  GetType () const override
		{
			return EFile::Multithreaded;
		}
	};

	

	//
	// Write only multithread file
	//

	class MtWFile : public WFile
	{
	// variables
	private:
		WFilePtr			_file;
		mutable OS::Mutex	_lock;


	// methods
	public:
		MtWFile (const WFilePtr &file) : _file(file)
		{}


		static SHARED_POINTER_TYPE( MtWFile )  New (const WFilePtr &file)
		{
			ASSERT( file->GetType() != EFile::Multithreaded );
			return new MtWFile( file );
		}

		
		// WFile //
		virtual BytesU WriteBuf (const void * buf, BytesU size) override
		{
			SCOPELOCK( _lock );
			return _file->WriteBuf( buf, size );
		}

		virtual void Flush () override
		{
			SCOPELOCK( _lock );
			return _file->Flush();
		}
		

		// BaseFile //
		virtual void Close () override
		{
			SCOPELOCK( _lock );
			_file->Close();
		}
		
		virtual bool IsOpened () const override
		{
			SCOPELOCK( _lock );
			return _file.IsNotNull();
		}

		virtual bool SeekSet (BytesU offset) override
		{
			SCOPELOCK( _lock );
			return _file->SeekSet( offset );
		}

		virtual bool SeekCur (BytesI offset) override
		{
			SCOPELOCK( _lock );
			return _file->SeekCur( offset );
		}

		virtual bool SeekEnd (BytesU offset) override
		{
			SCOPELOCK( _lock );
			return _file->SeekEnd( offset );
		}
		
		virtual BytesU RemainingSize () const override
		{
			SCOPELOCK( _lock );
			return _file->RemainingSize();
		}

		virtual BytesU Size () const override
		{
			SCOPELOCK( _lock );
			return _file->Size();
		}

		virtual BytesU Pos () const override
		{
			SCOPELOCK( _lock );
			return _file->Pos();
		}
		
		virtual bool IsEOF () const override
		{
			SCOPELOCK( _lock );
			return _file->IsEOF();
		}

		virtual StringCRef  Name () const override
		{
			SCOPELOCK( _lock );
			return _file->Name();
		}

		virtual EFile::type  GetType () const override
		{
			return EFile::Multithreaded;
		}
	};


}	// File
}	// GX_STL
