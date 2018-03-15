// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CMakeBuilder.h"

namespace CMake
{

/*
=================================================
	constructor
=================================================
*/
	CMakeBuilder::CMakeProject::CMakeProject (CMakeBuilder *builder, EProjectType type, StringCRef name, StringCRef folder) :
		_builder( builder ), _name( name ), _baseFolder( folder ), _projType( type )
	{
		FileAddress::AbsoluteToRelativePath( _baseFolder, _builder->_baseFolder, OUT _folder );
		FileAddress::RemoveDirectoriesFromRight( INOUT _folder, 1 );

		_groups.Add( "", {} );
	}
	
/*
=================================================
	ToString
=================================================
*/
	bool CMakeBuilder::CMakeProject::ToString (OUT String &outSrc)
	{
		CHECK_ERR( not _name.Empty() );

		usize	file_count = 0;
		FOR( i, _groups ) {
			file_count += _groups[i].second.Count();
		}
		CHECK_ERR( file_count > 0 or _mergeCppForThreads > 0 );

		outSrc	<< "#==================================================================================================\n"
				<< "# project: " << _name << "\n"
				<< "#==================================================================================================\n";

		if ( not _enableIf.Empty() )
			outSrc << "if (" << _enableIf << ")\n";

		String	src;
		
		if ( _mergeCppForThreads > 0 and (_projType == EProjectType::Executable or _projType == EProjectType::SharedLibrary) )
			CHECK_ERR( _ToStringOptimized( OUT src ) )
		else
			CHECK_ERR( _ToStringNormal( OUT src ) );

		if ( not _compilerOpt.Empty() )
		{
			src << "# compiler\n"
				<< _compilerOpt << "\n";
		}

		if ( not _source.Empty() )
		{
			src << "#-----------------------------------\n"
				<< _source
				<< "#-----------------------------------\n";
		}
		
		if ( not _enableIf.Empty() )
		{
			StringParser::IncreaceIndent( INOUT src );

			src << "endif()\n";
		}

		outSrc << src << "\n\n";
		return true;
	}
	
/*
=================================================
	_ProjectToString
=================================================
*/
	bool CMakeBuilder::CMakeProject::_ProjectToString (OUT String &src) const
	{
		switch ( _projType )
		{
			case EProjectType::Executable :
				src << "if (DEFINED ANDROID)\n"
					<< "	add_library( \"" << _name << "\" SHARED ${SOURCES} )\n"
					<< "else()\n"
					<< "	add_executable( \"" << _name << "\" ${SOURCES} )\n"
					<< "endif()\n";
				break;
				
			case EProjectType::Library :
				src << "add_library( \"" << _name << "\" STATIC ${SOURCES} )\n";
				break;
				
			case EProjectType::SharedLibrary :
				src << "add_library( \"" << _name << "\" SHARED ${SOURCES} )\n";
				break;

			default :
				TODO("");
				break;
		}
		return true;
	}

/*
=================================================
	_ToStringNormal
=================================================
*/
	bool CMakeBuilder::CMakeProject::_ToStringNormal (OUT String &src) const
	{
		src << "set( SOURCES ";
		FOR( i, _groups )
		{
			FOR( j, _groups[i].second ) {
				src << "\n\t\t\"" << _groups[i].second[j] << "\"";
			}
		}
		src << " )\n";

		CHECK_ERR( _ProjectToString( src ) );

		FOR( i, _groups )
		{
			if ( _groups[i].second.Empty() )
				continue;

			src << "source_group( \"" << _groups[i].first << "\" FILES ";
			
			FOR( j, _groups[i].second ) {
				src << "\"" << _groups[i].second[j] << "\" ";
			}
			src << ")\n";
		}

		if ( not _folder.Empty() ) {
			src << "set_property( TARGET \"" << _name << "\" PROPERTY FOLDER \"" << _folder << "\" )\n";
		}

		if ( not _outputDir.Empty() ) {
			src << "set_property( TARGET \"" << _name << "\" PROPERTY RUNTIME_OUTPUT_DIRECTORY \"" << _outputDir << "\" )\n";
		}
		
		{
			String	include;
			FileAddress::AbsoluteToRelativePath( _baseFolder, _builder->_baseFolder, OUT include );

			if ( not include.Empty() ) {
				src << "target_include_directories( \"" << _name << "\" PRIVATE \"" << include << "\" )\n";
			}
		}

		FOR( i, _includeDirs )
		{
			const bool	has_option = not _includeDirs[i].second.Empty() and _includeDirs[i].second != _enableIf; 

			//CHECK( _ValidateInclude( _includeDirs[i].first, has_option ) );

			src << (has_option ? "if ("_str << _includeDirs[i].second << ")\n\t" : "")
				<< "target_include_directories( \"" << _name << "\" PUBLIC \"" << _includeDirs[i].first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}
		
		FOR( i, _includeDirsPrivate )
		{
			const bool	has_option = not _includeDirsPrivate[i].second.Empty() and _includeDirsPrivate[i].second != _enableIf; 

			//CHECK( _ValidateInclude( _includeDirsPrivate[i].first, has_option ) );

			src << (has_option ? "if ("_str << _includeDirsPrivate[i].second << ")\n\t" : "")
				<< "target_include_directories( \"" << _name << "\" PRIVATE \"" << _includeDirsPrivate[i].first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}
		
		FOR( i, _linkDirs )
		{
			const bool	has_option = not _linkDirs[i].second.Empty() and _linkDirs[i].second != _enableIf; 
			
			CHECK( _ValidateLinkDir( _linkDirs[i].first, has_option ) );

			src << (has_option ? "if ("_str << _linkDirs[i].second << ")\n\t" : "")
				<< "target_link_directory( \"" << _name << "\" \"" << _linkDirs[i].first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}
		
		FOR( i, _linkLibs )
		{
			const bool	has_option = not _linkLibs[i].second.Empty() and _linkLibs[i].second != _enableIf; 

			src << (has_option ? "if ("_str << _linkLibs[i].second << ")\n\t" : "")
				<< "target_link_libraries( \"" << _name << "\" \"" << _linkLibs[i].first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}
		
		FOR( i, _defines )
		{
			const bool	has_option = not _defines[i].second.Empty() and _defines[i].second != _enableIf; 

			src << (has_option ? "if ("_str << _defines[i].second << ")\n\t" : "")
				<< "target_compile_definitions( \"" << _name << "\" PUBLIC \"" << _defines[i].first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}

		FOR( i, _dependencies )
		{
			const bool	has_option = not _dependencies[i].second.Empty() and _dependencies[i].second != _enableIf;

			src << (has_option ? "if ("_str << _dependencies[i].second << ")\n\t" : "")
				<< "add_dependencies( \"" << _name << "\" \"" << _dependencies[i].first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}
		return true;
	}
	
/*
=================================================
	_ToStringOptimized
=================================================
*/
	bool CMakeBuilder::CMakeProject::_ToStringOptimized (OUT String &src) const
	{
		String	path		= FileAddress::BuildPath( _builder->_baseFolder, CMakeBuilder::GetTempFolderForFastCpp() );
		String	temp_folder = _name;

		temp_folder.ReplaceStrings( ".", "_" );
		temp_folder.ReplaceStrings( "/", "_" );
		temp_folder.ReplaceStrings( "\\", "_" );

		path = FileAddress::BuildPath( path, temp_folder );
		
		if ( not OS::FileSystem::IsDirectoryExist( path ) )
		{
			CHECK_ERR( OS::FileSystem::CreateDirectories( path ) );
		}

		// find dependencies
		Set<String>		cpp_files;
		StringMap_t		include_dirs;
		StringMap_t		link_dirs;
		StringMap_t		link_libs;
		StringMap_t		defines;
		StringMap_t		deps;

		CHECK_ERR( _MergeDependencies( OUT cpp_files, OUT include_dirs, OUT link_dirs, OUT link_libs, OUT defines, OUT deps ) );

		Array<String>	files;
		CHECK_ERR( _BuildNewCppFiles( path, cpp_files, OUT files ) );

		CHECK_ERR( _ToStringOptimizedImpl( files, include_dirs, link_dirs, link_libs, defines, deps, OUT src ) );
		return true;
	}
	
/*
=================================================
	_ToStringOptimizedImpl
=================================================
*/
	bool CMakeBuilder::CMakeProject::_ToStringOptimizedImpl (ArrayCRef<String> filePaths, const StringMap_t &includeDirs, const StringMap_t &linkDirs,
															 const StringMap_t &linkLibs, const StringMap_t &defines, const StringMap_t &dependencies,
															 OUT String &src) const
	{
		src << "set( SOURCES ";
		FOR( i, filePaths )
		{
			src << "\n\t\t\"" << filePaths[i] << "\"";
		}
		src << " )\n";

		CHECK_ERR( _ProjectToString( src ) );

		if ( not _folder.Empty() ) {
			src << "set_property( TARGET \"" << _name << "\" PROPERTY FOLDER \"" << _folder << "\" )\n";
		}

		if ( not _outputDir.Empty() ) {
			src << "set_property( TARGET \"" << _name << "\" PROPERTY RUNTIME_OUTPUT_DIRECTORY \"" << _outputDir << "\" )\n";
		}

		FOR( i, includeDirs )
		{
			const bool	has_option = not includeDirs[i].second.Empty() and includeDirs[i].second != _enableIf; 

			CHECK( _ValidateInclude( includeDirs[i].first, has_option ) );

			src << (has_option ? "if ("_str << includeDirs[i].second << ")\n\t" : "")
				<< "target_include_directories( \"" << _name << "\" PRIVATE \"" << includeDirs[i].first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}
		
		FOR( i, linkDirs )
		{
			const bool	has_option = not linkDirs[i].second.Empty() and linkDirs[i].second != _enableIf; 
			
			CHECK( _ValidateLinkDir( linkDirs[i].first, has_option ) );

			src << (has_option ? "if ("_str << linkDirs[i].second << ")\n\t" : "")
				<< "target_link_directory( \"" << _name << "\" \"" << linkDirs[i].first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}
		
		FOR( i, linkLibs )
		{
			const bool	has_option = not linkLibs[i].second.Empty() and linkLibs[i].second != _enableIf; 

			src << (has_option ? "if ("_str << linkLibs[i].second << ")\n\t" : "")
				<< "target_link_libraries( \"" << _name << "\" \"" << linkLibs[i].first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}
		
		FOR( i, defines )
		{
			const bool	has_option = not defines[i].second.Empty() and defines[i].second != _enableIf; 

			src << (has_option ? "if ("_str << defines[i].second << ")\n\t" : "")
				<< "target_compile_definitions( \"" << _name << "\" PUBLIC \"" << defines[i].first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}

		FOR( i, dependencies )
		{
			const bool	has_option = not dependencies[i].second.Empty() and dependencies[i].second != _enableIf;

			src << (has_option ? "if ("_str << dependencies[i].second << ")\n\t" : "")
				<< "add_dependencies( \"" << _name << "\" \"" << dependencies[i].first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}
		return true;
	}
	
/*
=================================================
	_MergeDependencies
=================================================
*/
	bool CMakeBuilder::CMakeProject::_MergeDependencies (OUT Set<String> &cppFiles, OUT StringMap_t &includeDirs, OUT StringMap_t &linkDirs,
														 OUT StringMap_t &linkLibs, OUT StringMap_t &defines, OUT StringMap_t &deps) const
	{
		Set<String>		dependencies;
		Set<String>		processed;

		auto AddProjectDeps =	LAMBDA( &includeDirs, &linkDirs, &linkLibs, &defines, &deps, &dependencies ) (const CMakeProject *proj)
								{{
									String	include;
									FileAddress::AbsoluteToRelativePath( proj->_baseFolder, proj->_builder->_baseFolder, OUT include );
									includeDirs.Add({ include, "" });

									FOR( i, proj->_linkLibs ) {
										dependencies.Add( proj->_linkLibs[i].first );
										linkLibs.Add( proj->_linkLibs[i] );
									}
									FOR( i, proj->_dependencies ) {
										deps.Add( proj->_dependencies[i] );
										dependencies.Add( proj->_dependencies[i].first );
									}
									FOR( i, proj->_includeDirs ) {
										includeDirs.Add( proj->_includeDirs[i] );
									}
									FOR( i, proj->_includeDirsPrivate ) {
										includeDirs.Add( proj->_includeDirsPrivate[i] );
									}
									FOR( i, proj->_defines ) {
										defines.Add( proj->_defines[i] );
									}
								}};

		auto AddProjectFiles =	LAMBDA ( &cppFiles ) (const CMakeProject *proj) -> bool
								{{
									FOR( j, proj->_groups )
									{
										FOR( k, proj->_groups[j].second )
										{
											StringCRef	fname	= proj->_groups[j].second[k];
											StringCRef	ext		= FileAddress::GetExtension( fname );

											if ( ext.EqualsIC( "cpp" ) or ext.EqualsIC( "c" ) or ext.EqualsIC( "cxx" ) )
											{
												String fpath = FileAddress::BuildPath( proj->_builder->_baseFolder, fname );
												CHECK_ERR( OS::FileSystem::IsFileExist( fpath ) );

												cppFiles.Add( fname );
											}
										}
									}
									return true;
								}};

		AddProjectDeps( this );
		AddProjectFiles( this );

		for (bool completed = false; not completed; )
		{
			completed = true;

			FOR( i, _builder->_projects )
			{
				CMakeProject const* proj = dynamic_cast<CMakeProject*>( _builder->_projects[i].RawPtr() );

				if ( proj and
					 dependencies.IsExist( proj->_name ) and
					 not processed.IsExist( proj->_name ) )
				{
					processed.Add( proj->_name );
					AddProjectDeps( proj );
					AddProjectFiles( proj );

					// restart searching
					completed = false;
					break;
				}
			}
		}

		// remove processed projects from libs
		FOR( i, linkLibs )
		{
			if ( processed.IsExist( linkLibs[i].first ) )
			{
				linkLibs.EraseByIndex( i );
				--i;
			}
		}
		
		// remove processed projects from dependencies
		FOR( i, deps )
		{
			if ( processed.IsExist( deps[i].first ) )
			{
				deps.EraseByIndex( i );
				--i;
			}
		}
		return true;
	}
	
/*
=================================================
	_BuildNewCppFiles
=================================================
*/
	bool CMakeBuilder::CMakeProject::_BuildNewCppFiles (StringCRef path, const Set<String> &cppFiles, OUT Array<String> &filePaths) const
	{
		struct FileInfo
		{
			BytesUL				size;
			Array<StringCRef>	files;

			FileInfo () {}
		};
		using FileInfoMap_t	= HashMap< String, FileInfo >;

		FileInfoMap_t	info;

		FOR( i, cppFiles )
		{
			StringCRef					ext = FileAddress::GetExtension( cppFiles[i] );
			FileInfoMap_t::iterator		iter;

			if ( not info.Find( ext, OUT iter ) )
			{
				iter = info.Add( ext, {} );
			}

			iter->second.size += OS::FileSystem::GetFileSize( cppFiles[i] );
			iter->second.files << cppFiles[i];
		}

		BytesUL		total;

		FOR( i, info ) {
			total += info[i].second.size;
		}

		BytesUL		size_per_thread = BytesUL( (ulong)GXMath::Ceil( double(ulong(total)) / double(_mergeCppForThreads) ) );
		uint		file_counter	= 0;
		
		FOR( i, info )
		{
			for (; not info[i].second.files.Empty(); )
			{
				BytesUL	fsize;
				String	fname = FileAddress::BuildPath( path, "File"_str << (file_counter++), info[i].first );
				FileAddress::FormatPath( INOUT fname );

				CHECK_ERR( _GenNewFile( fname, size_per_thread, INOUT info[i].second.files, OUT fsize ) );

				filePaths << fname;
				info[i].second.size -= fsize;
			}
		}
		return true;
	}
	
/*
=================================================
	_GenNewFile
=================================================
*/
	bool CMakeBuilder::CMakeProject::_GenNewFile (StringCRef filename, BytesUL maxSize, INOUT Array<StringCRef> &files, OUT BytesUL &totalSize) const
	{
		File::WFilePtr	file = File::HddWFile::New( filename );
		CHECK_ERR( file );

		String	data;
		String	fname;

		data << "// This is generated file to make compilation faster\n\n";

		totalSize = BytesUL();

		usize	smallest_file_idx	= UMax;
		BytesUL	min_size			= BytesUL(ulong(UMax));

		FOR( i, files )
		{
			BytesUL	size = OS::FileSystem::GetFileSize( files[i] );

			if ( totalSize + size < maxSize )
			{
				fname = files[i];
				FileAddress::FormatPath( INOUT fname );

				data << "#include \"" << fname << "\"\n";

				totalSize += size;
				files.Erase( i );
				--i;
			}
			else
			if ( size < min_size )
			{
				smallest_file_idx	= i;
				min_size			= size;
			}
		}

		if ( smallest_file_idx < files.Count() )
		{
			fname = files[smallest_file_idx];
			FileAddress::FormatPath( INOUT fname );

			data << "#include \"" << fname << "\"\n";

			totalSize += min_size;
			files.Erase( smallest_file_idx );
		}

		CHECK_ERR( file->Write( StringCRef(data) ) );
		return true;
	}

/*
=================================================
	_ValidateInclude
=================================================
*/
	bool CMakeBuilder::CMakeProject::_ValidateInclude (StringCRef path, bool withOption) const
	{
		return	withOption or
				OS::FileSystem::IsDirectoryExist( path ) or
				OS::FileSystem::IsDirectoryExist( FileAddress::BuildPath( _baseFolder, path ) ) or
				OS::FileSystem::IsDirectoryExist( FileAddress::BuildPath( _builder->_baseFolder, path ) );
	}
	
/*
=================================================
	_ValidateLinkDir
=================================================
*/
	bool CMakeBuilder::CMakeProject::_ValidateLinkDir (StringCRef path, bool withOption) const
	{
		return	withOption or
				OS::FileSystem::IsDirectoryExist( path ) or
				OS::FileSystem::IsDirectoryExist( FileAddress::BuildPath( _baseFolder, path ) ) or
				OS::FileSystem::IsDirectoryExist( FileAddress::BuildPath( _builder->_baseFolder, path ) );
	}

/*
=================================================
	AddFile
=================================================
*/
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::AddFile (StringCRef filename)
	{
		String	fname = FileAddress::BuildPath( _baseFolder, filename );
		CHECK_ERR( OS::FileSystem::IsFileExist( fname ) );
		
		String	fname2;
		CHECK_ERR( FileAddress::AbsoluteToRelativePath( fname, _builder->_baseFolder, OUT fname2 ) );

		Groups_t::iterator	iter;
		CHECK_ERR( _groups.Find( "", OUT iter ) );

		iter->second << fname2;
		return this;
	}
	
/*
=================================================
	AddGroup
=================================================
*/
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::AddGroup (StringCRef groupName, ArrayCRef<StringCRef> files)
	{
		String				gr_name = groupName;
		Groups_t::iterator	iter;
		
		// format name
		for (usize pos = 0; gr_name.FindAndDelete( "../", OUT pos );) {}

		gr_name.ReplaceStrings( "/", "\\\\" );
		
		// create or reuse group
		if ( not _groups.Find( gr_name, OUT iter ) )
		{
			iter = _groups.Add( gr_name, {} );
		}
		
		// add files to group
		FOR( i, files )
		{
			String	fname = FileAddress::BuildPath( _baseFolder, files[i] );
			CHECK_ERR( OS::FileSystem::IsFileExist( fname ) );
			
			String	fname2;
			CHECK_ERR( FileAddress::AbsoluteToRelativePath( fname, _builder->_baseFolder, OUT fname2 ) );

			iter->second << fname2;
		}
		return this;
	}
	
/*
=================================================
	CheckExtension
=================================================
*/
	static bool CheckExtension (StringCRef filename, const HashSet<String> &filter)
	{
		if ( filter.Empty() )
			return true;

		String	ext = FileAddress::GetExtension( filename );

		if ( ext.Empty() )
			return false;

		FOR( i, ext ) {
			ext[i] = StringUtils::ToUpper( ext[i] );
		}

		if ( not filter.IsExist( ext ) )
			return false;

		return true;
	}
	
/*
=================================================
	AddFolder
=================================================
*/
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::AddFolder (StringCRef path, const StringSet_t &filter)
	{
		String	dir = FileAddress::BuildPath( _baseFolder, path );
		CHECK_ERR( OS::FileSystem::IsDirectoryExist( dir ) );

		Array< String >		names;
		CHECK_ERR( OS::FileSystem::GetAllFilesInPath( dir, OUT names ) );
		
		String				gr_name = path;
		Groups_t::iterator	iter;
		
		// format name
		for (usize pos = 0; gr_name.FindAndDelete( "../", OUT pos );) {}

		gr_name.ReplaceStrings( "/", "\\\\" );
		
		// create or reuse group
		if ( not _groups.Find( gr_name, OUT iter ) )
		{
			iter = _groups.Add( gr_name, {} );
		}

		// add files to group
		FOR( i, names )
		{
			if ( not CheckExtension( names[i], filter ) )
				continue;

			String	fname = FileAddress::BuildPath( dir, names[i] );
			CHECK_ERR( OS::FileSystem::IsFileExist( fname ) );
			
			String	fname2;
			CHECK_ERR( FileAddress::AbsoluteToRelativePath( fname, _builder->_baseFolder, OUT fname2 ) );

			iter->second << fname2;
		}

		// remove if empty
		if ( iter->second.Empty() )
			_groups.EraseByIter( iter );

		return this;
	}
	
/*
=================================================
	AddFoldersRecursive
=================================================
*/
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::AddFoldersRecursive (StringCRef path, const StringSet_t &filter)
	{
		String	dir = FileAddress::BuildPath( _baseFolder, path );
		CHECK_ERR( OS::FileSystem::IsDirectoryExist( dir ) );
		
		Array< String >		names;
		Queue< String >		folders;	folders.PushBack( dir );

		for (; not folders.Empty();)
		{
			names.Clear();
			CHECK_ERR( OS::FileSystem::GetAllDirsInPath( folders.Front(), OUT names ) );

			FOR( i, names )
			{
				String	dir2 = FileAddress::BuildPath( folders.Front(), names[i] );
				CHECK_ERR( OS::FileSystem::IsDirectoryExist( dir2 ) );

				folders.PushBack( dir2 );
			}

			names.Clear();
			CHECK_ERR( OS::FileSystem::GetAllFilesInPath( folders.Front(), OUT names ) );
			
			if ( not names.Empty() )
			{
				String	dir2;
				CHECK_ERR( FileAddress::AbsoluteToRelativePath( folders.Front(), _baseFolder, OUT dir2 ) );
				
				// format name
				for (usize pos = 0; dir2.FindAndDelete( "../", OUT pos );) {}

				dir2.ReplaceStrings( "/", "\\\\" );

				// create or reuse group
				Groups_t::iterator	iter;

				if ( not _groups.Find( dir2, OUT iter ) )
				{
					iter = _groups.Add( dir2, {} );
				}

				// add files to group
				FOR( i, names )
				{
					if ( not CheckExtension( names[i], filter ) )
						continue;

					String	fname = FileAddress::BuildPath( folders.Front(), names[i] );
					CHECK_ERR( OS::FileSystem::IsFileExist( fname ) );
					
					String	fname2;
					CHECK_ERR( FileAddress::AbsoluteToRelativePath( fname, _builder->_baseFolder, OUT fname2 ) );

					iter->second << fname2;
				}

				// remove if empty
				if ( iter->second.Empty() )
					_groups.EraseByIter( iter );
			}
			folders.PopFront();
		}
		return this;
	}
	
/*
=================================================
	EnableIf
=================================================
*/
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::EnableIf (StringCRef condition)
	{
		_enableIf = condition;
		return this;
	}

/*
=================================================
	ProjFolder
=================================================
*/
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::ProjFolder (StringCRef folder)
	{
		_folder = folder;
		return this;
	}
	
/*
=================================================
	OutputDirectory
=================================================
*/
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::OutputDirectory (StringCRef folder)
	{
		_outputDir = folder;
		return this;
	}
	
/*
=================================================
	AddDefinition
=================================================
*/
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::AddDefinition (StringCRef def, StringCRef enableIf)
	{
		_defines.Add({ def, enableIf });
		return this;
	}
	
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::AddDefinitions (ArrayCRef<String> defs, StringCRef enableIf)
	{
		FOR( i, defs ) {
			_defines.Add({ defs[i], enableIf });
		}
		return this;
	}

/*
=================================================
	IncludeDirectory
=================================================
*/
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::IncludeDirectory (StringCRef folder, bool isPublic, StringCRef enableIf)
	{
		if ( isPublic )
			_includeDirs.Add({ folder, enableIf });
		else
			_includeDirsPrivate.Add({ folder, enableIf });

		return this;
	}
	
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::IncludeDirectory (ArrayCRef<String> folders, bool isPublic, StringCRef enableIf)
	{
		FOR( i, folders ) {
			IncludeDirectory( folders[i], isPublic, enableIf );
		}
		return this;
	}

/*
=================================================
	LinkDirectory
=================================================
*/
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::LinkDirectory (StringCRef folder, StringCRef enableIf)
	{
		_linkDirs.Add({ folder, enableIf });
		return this;
	}
	
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::LinkDirectory (ArrayCRef<String> folders, StringCRef enableIf)
	{
		FOR( i, folders ) {
			_linkDirs.Add({ folders[i], enableIf });
		}
		return this;
	}

/*
=================================================
	LinkLibrary
=================================================
*/
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::LinkLibrary (StringCRef lib, StringCRef enableIf)
	{
		#if 0
		StringCRef	path = FileAddress::GetPath( lib );
		lib = FileAddress::GetNameAndExt( lib );

		if ( not path.Empty() )
			LinkDirectory( path, enableIf );
		
		if ( lib.EndsWithIC( ".lib" ) )
			lib = lib.SubString( 0, lib.Length() - 4 );
		else
		if ( lib.EndsWithIC( ".dll" ) )
			lib = lib.SubString( 0, lib.Length() - 4 );
		else
		if ( lib.EndsWithIC( ".a" ) )
			lib = lib.SubString( 0, lib.Length() - 2 );
		else
		if ( lib.EndsWithIC( ".so" ) )
			lib = lib.SubString( 0, lib.Length() - 3 );
		#endif

		_linkLibs.Add({ lib, enableIf });
		return this;
	}
	
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::LinkLibrary (ArrayCRef<String> libs, StringCRef enableIf)
	{
		FOR( i, libs ) {
			_linkLibs.Add({ libs[i], enableIf });
		}
		return this;
	}

/*
=================================================
	LinkLibrary
=================================================
*/
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::LinkLibrary (CMakeProject *lib)
	{
		//CHECK( lib->_projType == EProjectType::Library or lib->_projType == EProjectType::SharedLibrary );

		return LinkLibrary( lib->_name, lib->_enableIf );
	}
	
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::LinkLibrary (ArrayCRef<CMakeProject*> libs)
	{
		FOR( i, libs ) {
			LinkLibrary( libs[i] );
		}
		return this;
	}
	
/*
=================================================
	LinkLibrary
=================================================
*/
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::LinkLibrary (CMakeExternalVSProject *proj)
	{
		return LinkLibrary( proj->_name, proj->_enableIf );
	}
	
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::LinkLibrary (ArrayCRef<CMakeExternalVSProject*> projs)
	{
		FOR( i, projs ) {
			LinkLibrary( projs[i] );
		}
		return this;
	}
	
/*
=================================================
	SearchLibraries
=================================================
*/
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::SearchLibraries (StringCRef path, ArrayCRef<String> libs, StringCRef enableIf)
	{
		CHECK_ERR( OS::FileSystem::IsDirectoryExist( path ), this );
		
		Array<String>	files;
		Array<String>	req_libs = libs;

		auto			Compare =	LAMBDA() (StringCRef fname, StringCRef lib) -> bool
									{
										StringCRef	ext1 = FileAddress::GetExtension( fname );
										StringCRef	ext2 = FileAddress::GetExtension( lib );

										if ( not ext2.Empty() and ext1.EqualsIC( ext2 ) )
											return false;
										
										if ( ext2.Empty() and not ext1.EqualsIC( "a" ) and not ext1.EqualsIC( "lib" ) )
											return false;

										auto	name1 = FileAddress::GetName( fname );
										auto	name2 = FileAddress::GetName( lib );

										if ( name1.EqualsIC( name2 ) )
											return true;

										// ends with 'd'
										if ( name1.StartsWithIC( name2 ) and name1.Length() == name2.Length()+1 and name1.Back() == 'd' )
											return true;

										return false;
									};

		OS::FileSystem::RecursiveFindFiles( path,
											LAMBDA( &req_libs, Compare ) (StringCRef fname)
											{
												FOR( i, req_libs ) {
													if ( Compare( fname, req_libs[i] ) ) {
														req_libs.Erase( i );
														return true;
													}
												}
												return false;
											},
											20, OUT files );

		CHECK_ERR( req_libs.Empty() );
		CHECK_ERR( files.Count() == libs.Count(), this );

		FOR( i, files )
		{
			LinkLibrary( "${CMAKE_SOURCE_DIR}/"_str << files[i], enableIf );
		}
		return this;
	}
	
/*
=================================================
	AddDependency
=================================================
*/
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::AddDependency (StringCRef dep, StringCRef enableIf)
	{
		_dependencies.Add({ dep, enableIf });
		return this;
	}

	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::AddDependency (Array<String> deps, StringCRef enableIf)
	{
		FOR( i, deps ) {
			AddDependency( deps[i], enableIf );
		}
		return this;
	}

/*
=================================================
	AddSource
=================================================
*/
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::AddSource (StringCRef src)
	{
		_source << src << '\n';
		return this;
	}
	
/*
=================================================
	MergeCPP
=================================================
*/
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::MergeCPP (uint numThreads)
	{
		CHECK( _projType == EProjectType::Executable or _projType == EProjectType::SharedLibrary );

		_mergeCppForThreads = numThreads;
		return this;
	}

/*
=================================================
	SetCompilerOptions
=================================================
*/
	void CMakeBuilder::CMakeProject::SetCompilerOptions (StringCRef str)
	{
		_compilerOpt = str;

		for (usize pos = 0; _compilerOpt.FindAndChange( CMakeBuilder::GetTargetPlaceholder(), "\"" + _name + "\"", OUT pos );) {}
	}
//-----------------------------------------------------------------------------
	
}	// CMake
