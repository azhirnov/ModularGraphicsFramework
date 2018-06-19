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
	GetRelativePath
=================================================
*/
	String  CMakeBuilder::CMakeProject::GetRelativePath () const
	{
		String	res;
		FileAddress::AbsoluteToRelativePath( _baseFolder, _builder->_baseFolder, OUT res );
		return res;
	}
	
/*
=================================================
	GetAbsolutePath
=================================================
*
	String  CMakeBuilder::CMakeProject::GetAbsolutePath () const
	{
		return _baseFolder;
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
		for (auto& group : _groups) {
			file_count += group.second.Count();
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
		for (auto& group : _groups)
		{
			for (auto& file : group.second) {
				src << "\n\t\"" << file << "\"";
			}
		}
		src << " )\n";

		CHECK_ERR( _ProjectToString( src ) );

		for (auto& group : _groups)
		{
			if ( group.second.Empty() )
				continue;

			src << "source_group( \"" << group.first << "\" FILES ";
			
			for (auto& file : group.second) {
				src << "\"" << file << "\" ";
			}
			src << ")\n";
		}

		if ( not _folder.Empty() ) {
			src << "set_property( TARGET \"" << _name << "\" PROPERTY FOLDER \"" << _folder << "\" )\n";
		}

		if ( not _outputDir.Empty() ) {
			src << "set_property( TARGET \"" << _name << "\" PROPERTY RUNTIME_OUTPUT_DIRECTORY \"" << _outputDir << "\" )\n";
		}
		
		/*{
			String	include;
			FileAddress::AbsoluteToRelativePath( _baseFolder, _builder->_baseFolder, OUT include );

			if ( not include.Empty() ) {
				src << "target_include_directories( \"" << _name << "\" PRIVATE \"" << include << "\" )\n";
			}
		}*/

		for (auto& dir : _includeDirs)
		{
			const bool	has_option = not dir.second.Empty() and dir.second != _enableIf; 

			//CHECK( _ValidateInclude( dir.first, has_option ) );

			src << (has_option ? "if ("_str << dir.second << ")\n\t" : "")
				<< "target_include_directories( \"" << _name << "\" PUBLIC \"" << dir.first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}
		
		for (auto& dir : _includeDirsPrivate)
		{
			const bool	has_option = not dir.second.Empty() and dir.second != _enableIf; 

			//CHECK( _ValidateInclude( dir.first, has_option ) );

			src << (has_option ? "if ("_str << dir.second << ")\n\t" : "")
				<< "target_include_directories( \"" << _name << "\" PRIVATE \"" << dir.first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}
		
		for (auto& dir : _linkDirs)
		{
			const bool	has_option = not dir.second.Empty() and dir.second != _enableIf; 
			
			CHECK( _ValidateLinkDir( dir.first, has_option ) );

			src << (has_option ? "if ("_str << dir.second << ")\n\t" : "")
				<< "target_link_directory( \"" << _name << "\" \"" << dir.first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}
		
		for (auto& lib : _linkLibs)
		{
			const bool	has_option = not lib.second.Empty() and lib.second != _enableIf; 

			src << (has_option ? "if ("_str << lib.second << ")\n\t" : "")
				<< "target_link_libraries( \"" << _name << "\" \"" << lib.first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}
		
		for (auto& def : _defines)
		{
			const bool	has_option = not def.second.Empty() and def.second != _enableIf; 

			src << (has_option ? "if ("_str << def.second << ")\n\t" : "")
				<< "target_compile_definitions( \"" << _name << "\" PUBLIC \"" << def.first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}

		for (auto& deps : _dependencies)
		{
			const bool	has_option = not deps.second.Empty() and deps.second != _enableIf;

			src << (has_option ? "if ("_str << deps.second << ")\n\t" : "")
				<< "add_dependencies( \"" << _name << "\" \"" << deps.first << "\" )\n"
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
		for (auto& path : filePaths)
		{
			src << "\n\t\t\"" << path << "\"";
		}
		src << " )\n";

		CHECK_ERR( _ProjectToString( src ) );

		if ( not _folder.Empty() ) {
			src << "set_property( TARGET \"" << _name << "\" PROPERTY FOLDER \"" << _folder << "\" )\n";
		}

		if ( not _outputDir.Empty() ) {
			src << "set_property( TARGET \"" << _name << "\" PROPERTY RUNTIME_OUTPUT_DIRECTORY \"" << _outputDir << "\" )\n";
		}

		for (auto& dir : includeDirs)
		{
			const bool	has_option = not dir.second.Empty() and dir.second != _enableIf; 

			CHECK( _ValidateInclude( dir.first, has_option ) );

			src << (has_option ? "if ("_str << dir.second << ")\n\t" : "")
				<< "target_include_directories( \"" << _name << "\" PRIVATE \"" << dir.first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}
		
		for (auto& dir : linkDirs)
		{
			const bool	has_option = not dir.second.Empty() and dir.second != _enableIf; 
			
			CHECK( _ValidateLinkDir( dir.first, has_option ) );

			src << (has_option ? "if ("_str << dir.second << ")\n\t" : "")
				<< "target_link_directory( \"" << _name << "\" \"" << dir.first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}
		
		for (auto& lib : linkLibs)
		{
			const bool	has_option = not lib.second.Empty() and lib.second != _enableIf; 

			src << (has_option ? "if ("_str << lib.second << ")\n\t" : "")
				<< "target_link_libraries( \"" << _name << "\" \"" << lib.first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}
		
		for (auto& def : defines)
		{
			const bool	has_option = not def.second.Empty() and def.second != _enableIf; 

			src << (has_option ? "if ("_str << def.second << ")\n\t" : "")
				<< "target_compile_definitions( \"" << _name << "\" PUBLIC \"" << def.first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}

		for (auto& deps : dependencies)
		{
			const bool	has_option = not deps.second.Empty() and deps.second != _enableIf;

			src << (has_option ? "if ("_str << deps.second << ")\n\t" : "")
				<< "add_dependencies( \"" << _name << "\" \"" << deps.first << "\" )\n"
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

									for (auto& lib : proj->_linkLibs) {
										dependencies.Add( lib.first );
										linkLibs.Add( lib );
									}
									for (auto& dep : proj->_dependencies) {
										deps.Add( dep );
										dependencies.Add( dep.first );
									}
									for (auto& dir : proj->_includeDirs) {
										includeDirs.Add( dir );
									}
									for (auto& dir : proj->_includeDirsPrivate) {
										includeDirs.Add( dir );
									}
									for (auto& def : proj->_defines) {
										defines.Add( def );
									}
								}};

		auto AddProjectFiles =	LAMBDA ( &cppFiles ) (const CMakeProject *proj) -> bool
								{{
									for (auto& group : proj->_groups)
									{
										for (auto& file : group.second)
										{
											StringCRef	fname	= file;
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

			for (auto& obj : _builder->_projects)
			{
				auto	proj = DynCast< CMakeProject const *>( obj );

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

		for (auto& file : cppFiles)
		{
			StringCRef					ext = FileAddress::GetExtension( file );
			FileInfoMap_t::iterator		iter;

			if ( not info.Find( ext, OUT iter ) )
			{
				iter = info.Add( ext, {} );
			}

			iter->second.size += OS::FileSystem::GetFileSize( file );
			iter->second.files << file;
		}

		BytesUL		total;

		for (auto& inf : info) {
			total += inf.second.size;
		}

		BytesUL		size_per_thread = BytesUL( (ulong)GXMath::Ceil( double(ulong(total)) / double(_mergeCppForThreads) ) );
		uint		file_counter	= 0;
		
		for (auto& inf : info)
		{
			for (; not inf.second.files.Empty(); )
			{
				BytesUL	fsize;
				String	fname = FileAddress::BuildPath( path, "File"_str << (file_counter++), inf.first );
				FileAddress::FormatPath( INOUT fname );

				CHECK_ERR( _GenNewFile( fname, size_per_thread, INOUT inf.second.files, OUT fsize ) );

				filePaths << fname;
				inf.second.size -= fsize;
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
		WFilePtr	file = GXFile::HddWFile::New( filename );
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
		for (auto& file : files)
		{
			String	fname = FileAddress::BuildPath( _baseFolder, file );
			CHECK_ERR( OS::FileSystem::IsFileExist( fname ) );
			
			String	fname2;
			CHECK_ERR( FileAddress::AbsoluteToRelativePath( fname, _builder->_baseFolder, OUT fname2 ) );

			FileAddress::FormatPath( INOUT fname2 );

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

		for (auto& c : ext) {
			c = StringUtils::ToUpper( c );
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
		for (auto& name : names)
		{
			if ( not CheckExtension( name, filter ) )
				continue;

			String	fname = FileAddress::BuildPath( dir, name );
			CHECK_ERR( OS::FileSystem::IsFileExist( fname ) );
			
			String	fname2;
			CHECK_ERR( FileAddress::AbsoluteToRelativePath( fname, _builder->_baseFolder, OUT fname2 ) );
			
			FileAddress::FormatPath( INOUT fname2 );

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

			for (auto& name : names)
			{
				String	dir2 = FileAddress::BuildPath( folders.Front(), name );
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
				for (auto& name : names)
				{
					if ( not CheckExtension( name, filter ) )
						continue;

					String	fname = FileAddress::BuildPath( folders.Front(), name );
					CHECK_ERR( OS::FileSystem::IsFileExist( fname ) );
					
					String	fname2;
					CHECK_ERR( FileAddress::AbsoluteToRelativePath( fname, _builder->_baseFolder, OUT fname2 ) );
					
					FileAddress::FormatPath( INOUT fname2 );

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
		for (auto& def : defs) {
			_defines.Add({ def, enableIf });
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
		const bool	has_macro	= Utils::ContainsMacro( folder );
		String		path;

		// make absolute path
		if ( OS::FileSystem::IsAbsolutePath( folder ) or has_macro ) {
			path = folder;
		} else {
			path = FileAddress::BuildPath( _baseFolder, folder );
			FileAddress::FormatPath( INOUT path );
		}

		// make relative to builder path
		String	dir;
		if ( not has_macro )
			FileAddress::AbsoluteToRelativePath( path, _builder->_baseFolder, OUT dir );
		else
			dir = RVREF(path);


		if ( isPublic )
			_includeDirs.Add({ dir, enableIf });
		else
			_includeDirsPrivate.Add({ dir, enableIf });

		return this;
	}
	
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::IncludeDirectory (ArrayCRef<String> folders, bool isPublic, StringCRef enableIf)
	{
		for (auto& folder : folders) {
			IncludeDirectory( folder, isPublic, enableIf );
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
		for (auto& folder : folders) {
			_linkDirs.Add({ folder, enableIf });
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
		for (auto& lib : libs) {
			_linkLibs.Add({ lib, enableIf });
		}
		return this;
	}

/*
=================================================
	LinkLibrary
=================================================
*/
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::LinkLibrary (const CMakeProject *lib)
	{
		//CHECK( lib->_projType == EProjectType::Library or lib->_projType == EProjectType::SharedLibrary );
		CHECK_ERR( lib, this );

		return LinkLibrary( lib->_name, lib->_enableIf );
	}
	
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::LinkLibrary (ArrayCRef<CMakeProject*> libs)
	{
		for (auto& lib : libs ) {
			LinkLibrary( lib );
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
		CHECK_ERR( proj, this );

		return LinkLibrary( proj->_name, proj->_enableIf );
	}
	
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::LinkLibrary (ArrayCRef<CMakeExternalVSProject*> projs)
	{
		for (auto& proj : projs) {
			LinkLibrary( proj );
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

		for (auto& file : files)
		{
			LinkLibrary( "${CMAKE_SOURCE_DIR}/"_str << file, enableIf );
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
		for (auto& dep : deps) {
			AddDependency( dep, enableIf );
		}
		return this;
	}
	
/*
=================================================
	AddDependency
=================================================
*/
	CMakeBuilder::CMakeProject*  CMakeBuilder::CMakeProject::AddDependency (const CMakeProject *lib)
	{
		CHECK_ERR( lib, this );

		return AddDependency( lib->_name, lib->_enableIf );
	}

	CMakeBuilder::CMakeProject*  CMakeBuilder::CMakeProject::AddDependency (ArrayCRef<CMakeProject*> libs)
	{
		for (auto& lib : libs) {
			AddDependency( lib );
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
