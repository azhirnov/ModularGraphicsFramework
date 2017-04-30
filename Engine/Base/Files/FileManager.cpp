// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "FileManager.h"

namespace Engine
{
namespace Base
{
	
/*
=================================================
	constructor
=================================================
*/
	FileManager::FileManager (const GlobalSystemsRef gs) :
		BaseObject( gs )
	{
		GlobalSystems()->GetSetter< FileManager >().Set( this );
	}
	
/*
=================================================
	destructor
=================================================
*/
	FileManager::~FileManager ()
	{
		GlobalSystems()->GetSetter< FileManager >().Set( null );
	}

/*
=================================================
	OpenForRead
=================================================
*/
	bool FileManager::OpenForRead (StringCRef filename, RFilePtr &file) const
	{
		File::HddRFilePtr	rfile = File::HddRFile::New();

		if ( IsFileExist( filename ) )
		{
			CHECK_ERR( rfile->Open( filename ) );
		}
		else
		{
			RETURN_ERR( "can't find file \"" << filename << '"' );
		}

		file = rfile;
		return true;
	}

/*
=================================================
	OpenForWrite
=================================================
*/
	bool FileManager::OpenForWrite (StringCRef filename, WFilePtr &file) const
	{
		File::HddWFilePtr	wfile = File::HddWFile::New();
		
		if ( IsFileExist( filename ) )
		{
			CHECK_ERR( wfile->Open( filename ) );
		}
		else
		{
			RETURN_ERR( "can't find file \"" << filename << '"' );
		}

		file = wfile;
		return true;
	}
	
/*
=================================================
	OpenForAppend
=================================================
*/
	bool FileManager::OpenForAppend (StringCRef filename, WFilePtr &file) const
	{
		File::HddWFilePtr	wfile = File::HddWFile::New();
		
		if ( IsFileExist( filename ) )
		{
			CHECK_ERR( wfile->Open( filename, File::HddWFile::EOpenFlags::Append ) );
		}
		else
		{
			RETURN_ERR( "can't find file \"" << filename << '"' );
		}

		file = wfile;
		return true;
	}

/*
=================================================
	CreateFile
=================================================
*/
	bool FileManager::CreateFile (StringCRef filename, WFilePtr &file) const
	{
		File::HddWFilePtr	wfile = File::HddWFile::New();
		
		if ( not wfile->Open( filename ) )
		{
			RETURN_ERR( "can't create file \"" << filename << '"' );
		}

		file = wfile;
		return true;
	}
	
/*
=================================================
	OpenForDecrypt
=================================================
*
	bool FileManager::OpenForDecrypt (StringCRef filename, RFilePtr &file, StringCRef password) const
	{
		RFilePtr	rfile;

		if ( IsFileExist( filename ) )
		{
			CHECK_ERR( OpenForRead( filename, rfile ) );
		}
		else
		if ( IsResourceExist( filename ) )
		{
			CHECK_ERR( OpenResource( filename, rfile ) );
		}
		else
		{
			RETURN_ERR( "can't find file: " << filename );
		}
		
		using namespace File;
		SHARED_POINTER( SimpleRCryptFile );

		SimpleRCryptFilePtr	cfile = new SimpleRCryptFile( rfile, SimpleRCryptFile::CryptAlgorithm( password ) );
		file = cfile;
		return true;
	}
	
/*
=================================================
	CreateEncryptFile
=================================================
*
	bool FileManager::CreateEncryptFile (StringCRef filename, WFilePtr &file, StringCRef password) const
	{
		WFilePtr	wfile;

		CHECK_ERR( CreateFile( filename, wfile ) );
		
		using namespace File;
		SHARED_POINTER( SimpleWCryptFile );

		SimpleWCryptFilePtr	cfile = new SimpleWCryptFile( wfile, SimpleWCryptFile::CryptAlgorithm( password ) );
		file = cfile;
		return true;
	}
	
/*
=================================================
	CreateMemFile
=================================================
*/
	bool FileManager::CreateMemFile (WFilePtr &file, BytesU reserve) const
	{
		File::MemWFilePtr	wfile = File::MemWFile::New();

		CHECK_ERR( wfile->Create( reserve ) );

		file = wfile;
		return true;
	}
	
/*
=================================================
	SaveMemFile
=================================================
*/
	bool FileManager::SaveMemFile (StringCRef filename, const WFilePtr &file) const
	{
		CHECK_ERR( file->GetType() == File::EFile::Memory );

		WFilePtr	wfile;
		CHECK_ERR( CreateFile( filename, wfile ) );

		CHECK_ERR( file.To< File::MemWFilePtr >()->Save( wfile ) );
		return true;
	}
	
/*
=================================================
	ReadToMem
=================================================
*/
	bool FileManager::ReadToMem (StringCRef filename, RFilePtr &file) const
	{
		RFilePtr	rfile;
		CHECK_ERR( OpenForRead( filename, rfile ) );
		
		File::MemRFilePtr	mfile;
		CHECK_ERR( mfile = File::MemRFile::New( rfile ) );

		file = mfile;
		return true;
	}
	
/*
=================================================
	ReadToMem
=================================================
*/
	bool FileManager::ReadToMem (const RFilePtr &fromFile, RFilePtr &toFile) const
	{
		CHECK_ERR( fromFile and fromFile->IsOpened() );

		File::MemRFilePtr	mfile;
		CHECK_ERR( mfile = File::MemRFile::New( fromFile ) );

		toFile = mfile;
		return true;
	}
	
/*
=================================================
	ReadToMemIfSmall
=================================================
*/
	bool FileManager::ReadToMemIfSmall (const RFilePtr &fromFile, RFilePtr &toFile) const
	{
		CHECK_ERR( fromFile and fromFile->IsOpened() );

		const usize	max_size = 1 << 20;	// 1Mb

		usize	size = (usize)fromFile->Size();

		if ( size > max_size )
		{
			toFile = fromFile;
			return false;
		}

		File::MemRFilePtr	mfile;
		CHECK_ERR( mfile = File::MemRFile::New( fromFile ) );

		toFile = mfile;
		return true;
	}
	
/*
=================================================
	CopyFile
=================================================
*/
	bool FileManager::CopyFile (const RFilePtr &fromFile, const WFilePtr &toFile) const
	{
		CHECK_ERR( fromFile and toFile );

		const usize		rsize			= (usize)fromFile->RemainingSize();
		const usize		bufsize			= rsize < (128u << 20) ? (1u << 20) : (16u << 20);
		usize			size			= 0;
		Array< ubyte >	buf;			buf.Resize( bufsize );
		
		while ( size < rsize )
		{
			const BytesU	readn  = fromFile->ReadBuf( buf.ptr(), buf.Size() );

			CHECK_ERR( readn > 0 );
			CHECK_ERR( toFile->Write( buf.ptr(), readn ) );

			size += (usize)readn;
		}

		CHECK_ERR( size == rsize );
		
		toFile->Flush();
		return true;
	}

/*
=================================================
	CopyFile
=================================================
*/
	bool FileManager::CopyFile (StringCRef fromFilename, StringCRef toFilename) const
	{
		RFilePtr	rfile;
		WFilePtr	wfile;

		CHECK_ERR( OpenForRead( fromFilename, rfile ) );
		CHECK_ERR( OpenForWrite( toFilename, wfile ) );

		return CopyFile( rfile, wfile );
	}

/*
=================================================
	CreateDirectory
=================================================
*/
	bool FileManager::CreateDirectories (StringCRef dir) const
	{
		return OS::FileSystem::CreateDirectories( dir );
	}
	
/*
=================================================
	FindAndSetCurrentDir
=================================================
*/
	bool FileManager::FindAndSetCurrentDir (StringCRef dirname, uint searchDepth) const
	{
		return OS::FileSystem::FindAndSetCurrentDir( dirname, searchDepth );
	}

/*
=================================================
	IsFileExist
=================================================
*/
	bool FileManager::IsFileExist (StringCRef filename) const
	{
		return OS::FileSystem::IsFileExist( filename );
	}

/*
=================================================
	IsDirectoryExist
=================================================
*/
	bool FileManager::IsDirectoryExist (StringCRef dirname) const
	{
		return OS::FileSystem::IsDirectoryExist( dirname );
	}
	
/*
=================================================
	NewDirectory
=================================================
*/
	bool FileManager::NewDirectory (StringCRef dir) const
	{
		return OS::FileSystem::NewDirectory( dir );
	}
	
/*
=================================================
	DeleteEmptyDirectory
=================================================
*/
	bool FileManager::DeleteEmptyDirectory (StringCRef dir) const
	{
		return OS::FileSystem::DeleteEmptyDirectory( dir );
	}
	
/*
=================================================
	DeleteDirectory
=================================================
*/
	bool FileManager::DeleteDirectory (StringCRef dir) const
	{
		return OS::FileSystem::DeleteDirectory( dir );
	}
	
/*
=================================================
	DeleteFile
=================================================
*/
	bool FileManager::DeleteFile (StringCRef filename)
	{
		return OS::FileSystem::DeleteFile( filename );
	}
	
/*
=================================================
	SetCurrentDirectory
=================================================
*/
	bool FileManager::SetCurrentDirectory (StringCRef dir) const
	{
		return OS::FileSystem::SetCurrentDirectory( dir );
	}
	
/*
=================================================
	GetCurrentDirectory
=================================================
*/
	bool FileManager::GetCurrentDirectory (OUT String &dir) const
	{
		return OS::FileSystem::GetCurrentDirectory( dir );
	}
	
/*
=================================================
	SearchFile
=================================================
*/
	bool FileManager::SearchFile (StringCRef file, uint depth, OUT String &result) const
	{
		return OS::FileSystem::SearchFile( file, depth, result );
	}
	
/*
=================================================
	SearchFileForward
=================================================
*/
	bool FileManager::SearchFileForward (StringCRef file, uint depth, OUT String &result) const
	{
		return OS::FileSystem::SearchFileForward( file, depth, result );
	}
	
/*
=================================================
	SearchFileBackward
=================================================
*/
	bool FileManager::SearchFileBackward (StringCRef file, uint depth, OUT String &result) const
	{
		return OS::FileSystem::SearchFileBackward( file, depth, result );
	}
	
/*
=================================================
	SearchDir
=================================================
*/
	bool FileManager::SearchDir (StringCRef dir, uint depth, OUT String &result) const
	{
		return OS::FileSystem::SearchDir( dir, depth, result );
	}
	
/*
=================================================
	SearchDirForward
=================================================
*/
	bool FileManager::SearchDirForward (StringCRef dir, uint depth, OUT String &result) const
	{
		return OS::FileSystem::SearchDirForward( dir, depth, result );
	}
	
/*
=================================================
	SearchDirBackward
=================================================
*/
	bool FileManager::SearchDirBackward (StringCRef dir, uint depth, OUT String &result) const
	{
		return OS::FileSystem::SearchDirBackward( dir, depth, result );
	}
	
/*
=================================================
	MakeUniqueName
=================================================
*/
	bool FileManager::MakeUniqueName (StringCRef filename, OUT String &result) const
	{
		if ( not IsFileExist( filename ) )
		{
			result = filename;
			return true;
		}

		StringCRef	ext = FileAddress::GetExtension( filename );
		StringCRef	name = FileAddress::GetName( filename );
		StringCRef	path = FileAddress::GetPath( filename );

		result.Clear();
		result.Reserve( filename.Length() + 10 );

		result << path << '/' << name;

		const usize		len = result.Length();

		uint	left  = 0;
		uint	right = 1 << 20;

		// binary search
		while ( right - left > 1 )
		{
			uint	mid = (right + left) >> 1;
			
			result.SetLength( len );
			result << '-' << mid << '.' << ext;

			if ( IsFileExist( result ) )
				left = mid;
			else
				right = mid;
		}

		for (uint i = left; i <= right; ++i)
		{
			result.SetLength( len );
			result << '-' << i << '.' << ext;

			if ( not IsFileExist( result ) )
				return true;
		}
		
		result = filename;
		RETURN_ERR( "unknown error!" );
	}
	

}	// Base
}	// Engine