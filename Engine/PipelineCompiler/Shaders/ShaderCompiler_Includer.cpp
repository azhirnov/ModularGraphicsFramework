// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Shaders/ShaderCompiler_Includer.h"
#include "Engine/PipelineCompiler/glsl/glsl_source_vfs.h"

namespace PipelineCompiler
{

/*
=================================================
	constructor
=================================================
*/
    ShaderCompiler::ShaderIncluder::IncludeResultImpl::IncludeResultImpl (String &&data, const std::string& headerName, void* userData) :
		IncludeResult{ headerName, data.cstr(), data.Length(), userData },
		_data{ RVREF(data) }
	{}
//-----------------------------------------------------------------------------


/*
=================================================
	constructor
=================================================
*/
	ShaderCompiler::ShaderIncluder::ShaderIncluder (StringCRef baseFolder)
	{
		if ( not FileAddress::GetNameAndExt( baseFolder ).Empty() )
			baseFolder = FileAddress::GetPath( baseFolder );

		if ( not baseFolder.Empty() )
		{
			ASSERT( OS::FileSystem::IsAbsolutePath( baseFolder ) );
		
			_baseFolder = baseFolder;
			FileAddress::FormatPath( INOUT _baseFolder );

			_directories.PushBack( _baseFolder );
		}
	}
	
/*
=================================================
	destructor
=================================================
*/
	ShaderCompiler::ShaderIncluder::~ShaderIncluder ()
	{
	}
	
/*
=================================================
	AddDirectory
=================================================
*/
	void ShaderCompiler::ShaderIncluder::AddDirectory (StringCRef path)
	{
		String	fpath;
		
		if ( OS::FileSystem::IsAbsolutePath( path ) ) {
			fpath = path;
		} else {
			fpath = FileAddress::BuildPath( _baseFolder, path );
		}
		
		FileAddress::FormatPath( INOUT fpath );

		CHECK_ERR( OS::FileSystem::IsDirectoryExist( fpath ), void() );

		for (const auto& dir : _directories)
		{
			if ( dir == fpath )
				return;	// already exists
		}

		_directories.PushBack( fpath );
	}
	
/*
=================================================
	GetHeaderSource
=================================================
*/
	bool ShaderCompiler::ShaderIncluder::GetHeaderSource (StringCRef header, OUT StringCRef &source) const
	{
		IncludedFiles_t::const_iterator	iter;
		
		if ( _includedFiles.Find( header, OUT iter ) )
		{
			source = iter->second->GetSource();
			return true;
		}

		const String	sysfile_name = ("<"_str << header << '>');
		
		if ( _includedFiles.Find( sysfile_name, OUT iter ) )
		{
			source = iter->second->GetSource();
			return true;
		}

		return false;
	}

/*
=================================================
	includeSystem
=================================================
*/
    ShaderCompiler::ShaderIncluder::IncludeResult*
		ShaderCompiler::ShaderIncluder::includeSystem (const char* headerName, const char *, size_t)
	{
		String	fname = headerName;
		FileAddress::FormatPath( fname );

		const String	sysfile_name = ("<"_str << headerName << '>');

		if ( _includedFiles.IsExist( sysfile_name ) )
		{
			auto	iter = _results.Add( IncludeResultPtr_t{new IncludeResultImpl( String(" "), headerName )} );
			return iter->ptr();
		}


		// search in built-in virtual file system
		String	fdata;

		if ( glsl_vfs::LoadFile( fname, OUT fdata ) )
		{
			auto	iter = _results.Add( IncludeResultPtr_t{new IncludeResultImpl( RVREF(fdata), headerName )} );
			
			_includedFiles.Add( sysfile_name, iter->ptr());
			return iter->ptr();
		}

		return null;
	}
	
/*
=================================================
	includeLocal
=================================================
*/
    ShaderCompiler::ShaderIncluder::IncludeResult*
		ShaderCompiler::ShaderIncluder::includeLocal (const char* headerName, const char *, size_t)
	{
		String	fname = headerName;
		FileAddress::FormatPath( fname );

		for (const auto& folder : _directories)
		{
			String	filename = FileAddress::BuildPath( folder, fname );

			if ( OS::FileSystem::IsFileExist( filename ) )
			{
				if ( _includedFiles.IsExist( filename ) )
				{
					auto	iter = _results.Add( IncludeResultPtr_t{new IncludeResultImpl( String(" "), headerName )} );
					return iter->ptr();
				}

				GXFile::RFilePtr	file = GXFile::HddRFile::New( filename );
				CHECK_ERR( file, null );

				const usize	len		= usize(file->RemainingSize());
				String		data;	data.Resize( len );
				
				CHECK_ERR( file->Read( data.ptr(), data.LengthInBytes() ), null );

				auto	iter = _results.Add( IncludeResultPtr_t{new IncludeResultImpl( RVREF(data), headerName )} );
				
				_includedFiles.Add( headerName, iter->ptr());
				return iter->ptr();
			}
		}

		return null;
	}
	
/*
=================================================
	releaseInclude
=================================================
*/
    void ShaderCompiler::ShaderIncluder::releaseInclude (IncludeResult *)
	{}

}	// PipelineCompiler
