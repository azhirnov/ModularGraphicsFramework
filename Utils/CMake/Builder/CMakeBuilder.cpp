// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CMakeBuilder.h"

namespace CMake
{

static char g_targetPlaceholder[] = "#target#";

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
		CHECK_ERR( file_count > 0 );

		outSrc	<< "#==================================================================================================\n"
				<< "# project: " << _name << "\n"
				<< "#==================================================================================================\n";

		if ( not _enableIf.Empty() )
			outSrc << "if (" << _enableIf << ")\n";

		String	src;
		switch ( _projType )
		{
			case EProjectType::Executable :
				src << "add_executable( \"" << _name << "\" ";
				break;
				
			case EProjectType::Library :
				src << "add_library( \"" << _name << "\" STATIC ";
				break;
				
			case EProjectType::SharedLibrary :
				src << "add_library( \"" << _name << "\" SHARED ";
				break;

			default :
				TODO("");
				break;
		}
		
		FOR( i, _groups )
		{
			FOR( j, _groups[i].second ) {
				src << "\n\t\t\"" << _groups[i].second[j] << "\"";
			}
		}
		src << " )\n\n";

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

			CHECK( _ValidateInclude( _includeDirs[i].first, has_option ) );

			src << (has_option ? "if ("_str << _includeDirs[i].second << ")\n\t" : "")
				<< "target_include_directories( \"" << _name << "\" PUBLIC \"" << _includeDirs[i].first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}
		
		FOR( i, _includeDirsPrivate )
		{
			const bool	has_option = not _includeDirsPrivate[i].second.Empty() and _includeDirsPrivate[i].second != _enableIf; 

			CHECK( _ValidateInclude( _includeDirsPrivate[i].first, has_option ) );

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
				<< "target_add_definitions( \"" << _name << "\" \"" << _defines[i].first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}

		FOR( i, _dependencies )
		{
			const bool	has_option = not _dependencies[i].second.Empty() and _dependencies[i].second != _enableIf;

			src << (has_option ? "if ("_str << _dependencies[i].second << ")\n\t" : "")
				<< "add_dependencies( \"" << _name << "\" \"" << _dependencies[i].first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}

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
		String	gr_name = groupName;	gr_name.ReplaceStrings( "/", "\\\\" );

		usize	idx;
		if ( not _groups.FindIndex( gr_name, OUT idx ) )
		{
			idx = _groups.Add( gr_name, {} );
		}

		FOR( i, files )
		{
			String	fname = FileAddress::BuildPath( _baseFolder, files[i] );
			CHECK_ERR( OS::FileSystem::IsFileExist( fname ) );
			
			String	fname2;
			CHECK_ERR( FileAddress::AbsoluteToRelativePath( fname, _builder->_baseFolder, OUT fname2 ) );

			_groups[idx].second << fname2;
		}
		return this;
	}
	
/*
=================================================
	AddFolder
=================================================
*/
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::AddFolder (StringCRef path)
	{
		String	dir = FileAddress::BuildPath( _baseFolder, path );
		CHECK_ERR( OS::FileSystem::IsDirectoryExist( dir ) );

		Array< String >	names;
		CHECK_ERR( OS::FileSystem::GetAllFilesInPath( dir, OUT names ) );
		
		String	gr_name = path;		gr_name.ReplaceStrings( "/", "\\\\" );

		usize	idx;
		if ( not _groups.FindIndex( gr_name, OUT idx ) )
		{
			idx = _groups.Add( gr_name, {} );
		}

		FOR( i, names )
		{
			String	fname = FileAddress::BuildPath( dir, names[i] );
			CHECK_ERR( OS::FileSystem::IsFileExist( fname ) );
			
			String	fname2;
			CHECK_ERR( FileAddress::AbsoluteToRelativePath( fname, _builder->_baseFolder, OUT fname2 ) );

			_groups[idx].second << fname2;
		}
		return this;
	}
	
/*
=================================================
	AddFoldersRecursive
=================================================
*/
	CMakeBuilder::CMakeProject* CMakeBuilder::CMakeProject::AddFoldersRecursive (StringCRef path)
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
				dir2.ReplaceStrings( "/", "\\\\" );

				usize	idx;
				if ( not _groups.FindIndex( dir2, OUT idx ) )
				{
					idx = _groups.Add( dir2, {} );
				}

				FOR( i, names )
				{
					String	fname = FileAddress::BuildPath( folders.Front(), names[i] );
					CHECK_ERR( OS::FileSystem::IsFileExist( fname ) );
					
					String	fname2;
					CHECK_ERR( FileAddress::AbsoluteToRelativePath( fname, _builder->_baseFolder, OUT fname2 ) );

					_groups[idx].second << fname2;
				}
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
		//StringCRef	path = FileAddress::GetPath( lib );
		//lib = FileAddress::GetNameAndExt( lib );

		//if ( not path.Empty() )
		//	LinkDirectory( path, enableIf );
		/*
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
			lib = lib.SubString( 0, lib.Length() - 3 );*/

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
	SetCompilerOptions
=================================================
*/
	void CMakeBuilder::CMakeProject::SetCompilerOptions (StringCRef str)
	{
		_compilerOpt = str;

		for (usize pos = 0; _compilerOpt.FindAndChange( g_targetPlaceholder, "\"" + _name + "\"", OUT pos );) {}
	}
//-----------------------------------------------------------------------------
	

	
/*
=================================================
	constructor
=================================================
*/
	CMakeBuilder::CMakeCompiler::CMakeCompiler (CMakeBuilder *builder, ECompilerName name) :
		_builder( builder ), _compName( name )
	{
	}
	
/*
=================================================
	ToString
=================================================
*/
	bool CMakeBuilder::CMakeCompiler::ToString (OUT String &src)
	{
		CHECK_ERR( not _configurations.Empty() );

		switch ( _compName )
		{
			case ECompilerName::MSVisualStudio :
				src << "#==================================================================================================\n"
					<< "# Visual Studio Compilation settings\n"
					<< "#==================================================================================================\n"
					<< "if (\"${CMAKE_CXX_COMPILER_ID}\" STREQUAL \"MSVC\")\n"
					<< "	string( REPLACE \"/EHa\" \" \" CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS}\" )\n"
					<< "	string( REPLACE \"/EHsc\" \" \" CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS}\" )\n"
					<< "	string( REPLACE \"/EHs\" \" \" CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS}\" )\n"
					<< "	string( REPLACE \"//EHs-c-\" \" \" CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS}\" )\n";
				break;

			case ECompilerName::Clang :
				src << "#==================================================================================================\n"
					<< "# Clang Compilation settings\n"
					<< "#==================================================================================================\n"
					<< "if (\"${CMAKE_CXX_COMPILER_ID}\" STREQUAL \"Clang\")\n";
				break;

			case ECompilerName::GCC :
				src << "#==================================================================================================\n"
					<< "# GCC Compilation settings\n"
					<< "#==================================================================================================\n"
					<< "if (\"${CMAKE_CXX_COMPILER_ID}\" STREQUAL \"GNU\")\n";
				break;

			default :
				RETURN_ERR( "unknown compiler!" );
		}
		
		src << "	message( STATUS \"CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}\" )\n"
			<< "	message( STATUS \"CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}\" )\n"
			<< "	message( STATUS \"CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}\" )\n"
			<< "	message( STATUS \"CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}\" )\n";

		// declare configurations
		src << "	if (CMAKE_CONFIGURATION_TYPES)\n"
			<< "		set( CMAKE_CONFIGURATION_TYPES ";

		FOR( i, _configurations ) {
			src << _configurations[i].first << " ";
		}
		src << ")\n"
			<< "		set( CMAKE_CONFIGURATION_TYPES \"${CMAKE_CONFIGURATION_TYPES}\" CACHE STRING \"Configurations\" FORCE )\n"
			<< "	endif()\n\n";

		// include directories
		FOR( i, _includeDirs )
		{
			const bool	has_option = not _includeDirs[i].second.Empty(); 

			src << (has_option ? "	if ("_str << _includeDirs[i].second << ")\n\t" : "")
				<< "	include_directories( \"" << _includeDirs[i].first << "\" )\n"
				<< (has_option ? "	endif()\n" : "");
		}
		
		// link directories
		FOR( i, _linkDirs )
		{
			const bool	has_option = not _linkDirs[i].second.Empty();
			
			src << (has_option ? "if ("_str << _linkDirs[i].second << ")\n\t" : "")
				<< "link_directories( \"" << _linkDirs[i].first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}
		
		// link libraries
		FOR( i, _linkLibs )
		{
			const bool	has_option = not _linkLibs[i].second.Empty();

			src << (has_option ? "if ("_str << _linkLibs[i].second << ")\n\t" : "")
				<< "link_libraries( \"" << _linkLibs[i].first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}

		// serialize configurations
		FOR( i, _configurations ) {
			_configurations[i].second->ToString( _configurations[i].first, OUT src );
		}

		if ( _defaultCfg.Empty() )
		{
			if ( _configurations.IsExist( "Debug" ) )
				_defaultCfg = "Debug";
			else
				_defaultCfg = _configurations.Front().first;
		}
			
		CHECK_ERR( _configurations.IsExist( _defaultCfg ) );
		src << "	set( CMAKE_BUILD_TYPE \"" << _defaultCfg << "\")\n";

		src << "\nendif()\n\n\n";
		return true;
	}
	
/*
=================================================
	ToString2
=================================================
*/
	bool CMakeBuilder::CMakeCompiler::ToString2 (OUT String &src)
	{
		CHECK_ERR( not _configurations.Empty() );

		switch ( _compName )
		{
			case ECompilerName::MSVisualStudio :
				src << "if (\"${CMAKE_CXX_COMPILER_ID}\" STREQUAL \"MSVC\")\n";
				break;

			case ECompilerName::Clang :
				src << "if (\"${CMAKE_CXX_COMPILER_ID}\" STREQUAL \"Clang\")\n";
				break;

			case ECompilerName::GCC :
				src << "if (\"${CMAKE_CXX_COMPILER_ID}\" STREQUAL \"GNU\")\n";
				break;

			default :
				RETURN_ERR( "unknown compiler!" );
		}

		// serialize configurations
		FOR( i, _configurations ) {
			_configurations[i].second->ToString2( _configurations[i].first, OUT src );
		}
		
		src << "endif()\n\n\n";
		return true;
	}

/*
=================================================
	AddConfiguration
=================================================
*/
	CMakeBuilder::CMakeCompiler::Configuration*  CMakeBuilder::CMakeCompiler::AddConfiguration (StringCRef name, Configuration* copyFrom)
	{
		auto	cfg = new Configuration();

		if ( copyFrom )
			cfg->_CopyFrom( *copyFrom );

		_configurations.Add( name, cfg );
		return cfg;
	}
	
/*
=================================================
	DefaultConfiguration
=================================================
*/
	CMakeBuilder::CMakeCompiler*  CMakeBuilder::CMakeCompiler::DefaultConfiguration (StringCRef name)
	{
		_defaultCfg = name;
		return this;
	}

/*
=================================================
	IncludeDirectory
=================================================
*/
	CMakeBuilder::CMakeCompiler* CMakeBuilder::CMakeCompiler::IncludeDirectory (StringCRef folder, StringCRef enableIf)
	{
		_includeDirs.Add({ folder, enableIf });
		return this;
	}
	
	CMakeBuilder::CMakeCompiler* CMakeBuilder::CMakeCompiler::IncludeDirectory (ArrayCRef<String> folders, StringCRef enableIf)
	{
		FOR( i, folders ) {
			_includeDirs.Add({ folders[i], enableIf });
		}
		return this;
	}

/*
=================================================
	LinkDirectory
=================================================
*/
	CMakeBuilder::CMakeCompiler* CMakeBuilder::CMakeCompiler::LinkDirectory (StringCRef folder, StringCRef enableIf)
	{
		_linkDirs.Add({ folder, enableIf });
		return this;
	}
	
	CMakeBuilder::CMakeCompiler* CMakeBuilder::CMakeCompiler::LinkDirectory (ArrayCRef<String> folders, StringCRef enableIf)
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
	CMakeBuilder::CMakeCompiler* CMakeBuilder::CMakeCompiler::LinkLibrary (StringCRef lib, StringCRef enableIf)
	{
		_linkLibs.Add({ lib, enableIf });
		return this;
	}
	
	CMakeBuilder::CMakeCompiler* CMakeBuilder::CMakeCompiler::LinkLibrary (ArrayCRef<String> libs, StringCRef enableIf)
	{
		FOR( i, libs ) {
			_linkLibs.Add({ libs[i], enableIf });
		}
		return this;
	}
//-----------------------------------------------------------------------------


	
/*
=================================================
	constructor
=================================================
*/
	CMakeBuilder::CMakeCompiler::Configuration::Configuration ()
	{
	}

/*
=================================================
	ToString
=================================================
*/
	bool CMakeBuilder::CMakeCompiler::Configuration::ToString (StringCRef name, OUT String &src)
	{
		String	uc_name;

		FOR( i, name ) {
			uc_name << StringUtils::ToUpper( name[i] );
		}
		
		src << "	# " << name << "\n"
			<< "	set_property (DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:" << name << ">:";
		FOR( i, _defines ) { src << ' ' << _defines[i]; }
		src << " >)\n";
		
		src << "	set (CMAKE_C_FLAGS_" << uc_name << " \"${CMAKE_C_FLAGS}";
		FOR( i, _cFlags ) { src << ' ' << _cFlags[i]; }
		src << " \")\n";

		src << "	set (CMAKE_CXX_FLAGS_" << uc_name << " \"${CMAKE_CXX_FLAGS}";
		FOR( i, _cxxFlags ) { src << ' ' << _cxxFlags[i]; }
		src << " \")\n";

		src << "	set (CMAKE_EXE_LINKER_FLAGS_" << uc_name << " \"${CMAKE_EXE_LINKER_FLAGS}";
		FOR( i, _linkerFlags ) { src << ' ' << _linkerFlags[i]; }
		src << " \")\n";

		src << "	set (CMAKE_SHARED_LINKER_FLAGS_" << uc_name << " \"${CMAKE_EXE_LINKER_FLAGS}";
		FOR( i, _linkerFlags ) { src << ' ' << _linkerFlags[i]; }
		src << " \")\n";
		
		return true;
	}

/*
=================================================
	ToString2
=================================================
*/
	bool CMakeBuilder::CMakeCompiler::Configuration::ToString2 (StringCRef name, OUT String &src)
	{
		String	uc_name;

		FOR( i, name ) {
			uc_name << StringUtils::ToUpper( name[i] );
		}
		
		if ( not _targetCxxFlags.Empty() )
		{
			src << "	# " << name << "\n"
				<< "	target_compile_options ( " << g_targetPlaceholder << " PRIVATE $<$<CONFIG:" << name << ">:";
			FOR( i, _targetCxxFlags ) { src << ' ' << _targetCxxFlags[i]; }
			src << " >)\n";
		}

		if ( not _targetDefines.Empty() )
		{
			src << "	target_compile_definitions( " << g_targetPlaceholder << " PRIVATE $<$<CONFIG:" << name << ">:";
			FOR( i, _targetDefines ) { src << ' ' << _targetDefines[i]; }
			src << " >)\n";
		}

		if ( not _targetLinkerFlags.Empty() )
		{
			src << "	set_target_properties( " << g_targetPlaceholder << " PROPERTIES LINK_FLAGS_" << uc_name << " \"";
			FOR( i, _targetLinkerFlags ) { src << ' ' << _targetLinkerFlags[i]; }
			src << "\" )\n";
		}
		return true;
	}
	
/*
=================================================
	_CopyFrom
=================================================
*/
	void CMakeBuilder::CMakeCompiler::Configuration::_CopyFrom (const Configuration &other)
	{
		_defines		= other._defines;
		_cFlags			= other._cFlags;
		_cxxFlags		= other._cxxFlags;
		_linkerFlags	= other._linkerFlags;
	}

/*
=================================================
	AddGlobalDefinitions
=================================================
*/
	CMakeBuilder::CMakeCompiler::Configuration*  CMakeBuilder::CMakeCompiler::Configuration::AddGlobalDefinitions (ArrayCRef<String> defs)
	{
		FOR( i, defs ) {
			_defines << defs[i];
		}
		return this;
	}

/*
=================================================
	AddTargetDefinitions
=================================================
*/
	CMakeBuilder::CMakeCompiler::Configuration*  CMakeBuilder::CMakeCompiler::Configuration::AddTargetDefinitions (ArrayCRef<String> defs)
	{
		FOR( i, defs ) {
			_targetDefines << defs[i];
		}
		return this;
	}

/*
=================================================
	AddGlobalCFlags
=================================================
*/
	CMakeBuilder::CMakeCompiler::Configuration*  CMakeBuilder::CMakeCompiler::Configuration::AddGlobalCFlags (ArrayCRef<String> flags)
	{
		FOR( i, flags ) {
			_cFlags << flags[i];
		}
		return this;
	}

/*
=================================================
	AddGlobalCxxFlags
=================================================
*/
	CMakeBuilder::CMakeCompiler::Configuration*  CMakeBuilder::CMakeCompiler::Configuration::AddGlobalCxxFlags (ArrayCRef<String> flags)
	{
		FOR( i, flags ) {
			_cxxFlags << flags[i];
		}
		return this;
	}

/*
=================================================
	AddTargetCxxFlags
=================================================
*/
	CMakeBuilder::CMakeCompiler::Configuration*  CMakeBuilder::CMakeCompiler::Configuration::AddTargetCxxFlags (ArrayCRef<String> flags)
	{
		FOR( i, flags ) {
			_targetCxxFlags << flags[i];
		}
		return this;
	}
	
/*
=================================================
	AddTargetLinkerFlags
=================================================
*/
	CMakeBuilder::CMakeCompiler::Configuration*  CMakeBuilder::CMakeCompiler::Configuration::AddTargetLinkerFlags (ArrayCRef<String> flags)
	{
		FOR( i, flags ) {
			_targetLinkerFlags << flags[i];
		}
		return this;
	}

/*
=================================================
	AddGlobalLinkerFlags
=================================================
*/
	CMakeBuilder::CMakeCompiler::Configuration*  CMakeBuilder::CMakeCompiler::Configuration::AddGlobalLinkerFlags (ArrayCRef<String> flags)
	{
		FOR( i, flags ) {
			_linkerFlags << flags[i];
		}
		return this;
	}
//-----------------------------------------------------------------------------
	

	
/*
=================================================
	constructor
=================================================
*/
	CMakeBuilder::CMakeExternalProjects::CMakeExternalProjects (CMakeBuilder *builder, StringCRef path, StringCRef enableIf) :
		_builder( builder ), _enableIf( enableIf ),
		_baseFolder( FileAddress::BuildPath( builder->_baseFolder, path ) ),
		_path( path ), _folder( path ),
		_overrideSrcFolders( false )
	{
		CHECK( OS::FileSystem::IsDirectoryExist( _baseFolder ) );
	}
		
/*
=================================================
	ToString
=================================================
*/
	bool CMakeBuilder::CMakeExternalProjects::ToString (OUT String &outSrc)
	{
		CHECK_ERR( OS::FileSystem::IsDirectoryExist( _baseFolder ) );

		String				src;
		Array<TargetInfo>	targets;
		Array<String>		dir_stack;	dir_stack.PushBack( _baseFolder );

		CHECK_ERR( _ReqursiveGetTargets( EKeyType::SubDir, dir_stack, OUT targets ) );

		outSrc	<< "#==================================================================================================\n"
				<< "# External: " << _path << "\n"
				<< "#==================================================================================================\n";

		if ( not _enableIf.Empty() )
			outSrc << "if (" << _enableIf << ")\n";

		FOR( i, _options ) {
			src << "set( " << _options[i].first << " " << _options[i].second << " )\n";
		}

		src << "add_subdirectory( \"" << _path << "\" )\n";

		// change project folders for targets
		FOR( i, targets )
		{
			const String proj_folder = _overrideSrcFolders ? _folder : FileAddress::BuildPath( _folder, targets[i].projFolder );

			src << "if (TARGET \"" << targets[i].name << "\" )\n"
				<< "	set_property( TARGET \"" << targets[i].name << "\" PROPERTY FOLDER \"" << proj_folder << "\" )\n"
				<< "endif()\n";
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
	_ReqursiveGetTargets
=================================================
*/
	bool CMakeBuilder::CMakeExternalProjects::_ReqursiveGetTargets (EKeyType type, Array<String> dirs, OUT Array<TargetInfo> &result)
	{
		CHECK_ERR( OS::FileSystem::IsDirectoryExist( dirs.Back() ) );

		const String	cmake = "CMakeLists.txt";

		switch ( type )
		{
			case EKeyType::SubDir :
			{
				usize			counter = 0;
				Array<String>	all_files;
				CHECK_ERR( OS::FileSystem::GetAllFilesInPath( dirs.Back(), OUT all_files ) );

				FOR( i, all_files )
				{
					if ( all_files[i].EqualsIC( cmake ) )
					{
						CHECK_ERR( _ParseCMake( FileAddress::BuildPath( dirs.Back(), all_files[i] ), dirs, OUT result ) );
						++counter;
					}
				}
				CHECK_ERR( counter > 0 );	// cmake not found
				break;
			}

			case EKeyType::Incl :
			{

				break;
			}

			default :
				RETURN_ERR( "not supported!" );
		}

		return true;
	}
	
/*
=================================================
	IsEmptyChar
=================================================
*/
	inline bool IsEmptyChar (char c)
	{
		return	c == ' ' or c == '\t' or c == '\n' or c == '\r';
	}

/*
=================================================
	_ParseCMake
=================================================
*/
	bool CMakeBuilder::CMakeExternalProjects::_ParseCMake (StringCRef filename, Array<String> dirs, OUT Array<TargetInfo> &result)
	{
		const Array<Pair<EKeyType, String>>	keys = {{ EKeyType::SubDir,	"add_subdirectory"	},
													{ EKeyType::Exe,	"add_executable"	},
													{ EKeyType::Lib,	"add_library"		},
													{ EKeyType::Incl,	"include"			},
													{ EKeyType::Prop,	"set_property"		}};

		File::RFilePtr	file;
		CHECK_ERR( file = File::HddRFile::New( filename ) );

		String	buf;
		const usize	len = usize(file->RemainingSize());

		buf.Reserve( len+1 );
		CHECK_ERR( file->Read( buf.ptr(), BytesU(len) ) );
		buf.SetLength( len );
		file = null;

		StringCRef		line;
		usize			pos = 0;
		Array<String>	tokens;

		FOR( i, keys )
		{
			for (; buf.FindIC( keys[i].second, OUT pos, pos ); pos += keys[i].second.Length())
			{
				const char	prev = pos > 0 ? buf[ pos - 1 ] : ' ';
				const char	next = buf[ pos + keys[i].second.Length() ];

				if ( IsEmptyChar( prev ) and (IsEmptyChar( next ) or next == '(') )
				{
					CHECK( _ParseBlock( keys[i].first, buf, pos, dirs, OUT result ) );
				}
			}
		}
		return true;
	}
	
/*
=================================================
	_ParseBlock
=================================================
*/
	bool CMakeBuilder::CMakeExternalProjects::_ParseBlock (EKeyType blockType, StringCRef str, usize startPos, Array<String> dirs, OUT Array<TargetInfo> &result)
	{
		usize	start_pos = 0;
		str.Find( '(', OUT start_pos, startPos );

		// find last ')'
		usize	pos			= ++start_pos;
		int		br_counter	= 1;

		for (; pos < str.Length(); ++pos)
		{
			const char	c = str[pos];

			if ( c == '(' )
				++br_counter;
			else
			if ( c == ')' and --br_counter == 0 )
				break;
		}

		String	line = str.SubString( start_pos, pos - start_pos );

		switch ( blockType )
		{
			case EKeyType::SubDir :
			{
				// find subdirs
				String	dir_name;
				FOR_rev( i, dirs )
				{
					dir_name = FileAddress::BuildPath( dirs[i], line );

					if ( OS::FileSystem::IsDirectoryExist( dir_name ) )
					{
						Array<String>	new_dirs = dirs;	new_dirs << dir_name;

						CHECK_ERR( _ReqursiveGetTargets( blockType, new_dirs, OUT result ) );
						return true;
					}
				}
				RETURN_ERR( "sub directory '" << line << "' not found!" );
			}
			case EKeyType::Exe : {
				CHECK_ERR( _ParseAddProject( line, OUT result ) );
				break;
			}
			case EKeyType::Lib : {
				CHECK_ERR( _ParseAddProject( line, OUT result ) );
				break;
			}
			case EKeyType::Incl :
			{
				// find file
				String	fname;
				FOR_rev( i, dirs )
				{
					fname = FileAddress::BuildPath( dirs[i], line );

					if ( OS::FileSystem::IsFileExist( fname ) ) {
						//CHECK_ERR( _ReqursiveGetTargets( blockType, fname, OUT result ) );
						return true;
					}
				}
				RETURN_ERR( "file '" << line << "' not found!" );
			}
			case EKeyType::Prop : {
				CHECK_ERR( _ParseSetProperty( line, OUT result ) );
				break;
			}
			default :
				RETURN_ERR( "unknown block!" );
		}
		return true;
	}
	
/*
=================================================
	_ParseAddProject
=================================================
*/
	bool CMakeBuilder::CMakeExternalProjects::_ParseAddProject (StringCRef line, OUT Array<TargetInfo> &result)
	{
		// find start
		usize	start		= 0;
		bool	is_string	= false;

		for (; start < line.Length(); ++start)
		{
			const char	c = line[start];

			if ( not IsEmptyChar( c ) and c != '"' )
			{
				is_string = start > 0 and line[start-1] == '"';
				break;
			}
		}

		usize	end = start+1;
		for (; end < line.Length(); ++end)
		{
			const char	c = line[end];

			if ( is_string )
			{
				if ( c == '"' )
					break;

				continue;
			}

			if ( IsEmptyChar( c ) )
				break;
		}

		String	target = line.SubString( start, end - start );

		FOR( i, result )
		{
			if ( result[i].name == target )
				return true;
		}

		result.PushBack({ target, "", "" });
		return true;
	}

/*
=================================================
	_ParseSetProperty
=================================================
*/
	bool CMakeBuilder::CMakeExternalProjects::_ParseSetProperty (StringCRef line, OUT Array<TargetInfo> &result)
	{
		return true;
	}

/*
=================================================
	ProjFolder
=================================================
*/
	CMakeBuilder::CMakeExternalProjects*  CMakeBuilder::CMakeExternalProjects::ProjFolder (StringCRef folder, bool overrideSrc)
	{
		_folder = folder;
		_overrideSrcFolders	= overrideSrc;
		return this;
	}
	
/*
=================================================
	Rename
=================================================
*/
	CMakeBuilder::CMakeExternalProjects*  CMakeBuilder::CMakeExternalProjects::Rename (StringCRef srcProjectName, StringCRef dstProjectName)
	{
		ASSERT( not _renameProjMap.IsExist( srcProjectName ) );

		_renameProjMap.Add( srcProjectName, dstProjectName );
		return this;
	}
	
/*
=================================================
	AddOption
=================================================
*/
	CMakeBuilder::CMakeExternalProjects*  CMakeBuilder::CMakeExternalProjects::AddBoolOption (StringCRef name, bool value, StringCRef info)
	{
		ASSERT( not _options.IsExist( name ) );

		_options.Add( name, String(value ? "TRUE" : "OFF") << " CACHE BOOL \"" << info << "\" FORCE" );
		return this;
	}
	
	CMakeBuilder::CMakeExternalProjects*  CMakeBuilder::CMakeExternalProjects::AddStringOption (StringCRef name, StringCRef value, StringCRef info)
	{
		ASSERT( not _options.IsExist( name ) );

		_options.Add( name, "\""_str << value << "\" CACHE STRING \"" << info << "\" FORCE" );
		return this;
	}
	
	CMakeBuilder::CMakeExternalProjects*  CMakeBuilder::CMakeExternalProjects::AddPathOption (StringCRef name, StringCRef value, StringCRef info)
	{
		ASSERT( not _options.IsExist( name ) );

		_options.Add( name, "\""_str << value << "\" CACHE PATH \"" << info << "\" FORCE" );
		return this;
	}

/*
=================================================
	AddSource
=================================================
*/
	CMakeBuilder::CMakeExternalProjects*  CMakeBuilder::CMakeExternalProjects::AddSource (StringCRef src)
	{
		_source << src << '\n';
		return this;
	}
//-----------------------------------------------------------------------------


	
/*
=================================================
	constructor
=================================================
*/
	CMakeBuilder::CMakeExternalVSProject::CMakeExternalVSProject (CMakeBuilder *builder, StringCRef filename, StringCRef enableIf) :
		_builder(builder), _enableIf(enableIf),
		_baseFolder( FileAddress::BuildPath( builder->_baseFolder, FileAddress::GetPath( filename ) ) ),
		_name( FileAddress::GetName( filename ) )
	{
	}
	
/*
=================================================
	ProjFolder
=================================================
*/
	CMakeBuilder::CMakeExternalVSProject*  CMakeBuilder::CMakeExternalVSProject::ProjFolder (StringCRef folder)
	{
		_folder = folder;
		return this;
	}
	
/*
=================================================
	Search
=================================================
*/
	CMakeBuilder::CMakeExternalVSProject*  CMakeBuilder::CMakeExternalVSProject::Search ()
	{
		CHECK_ERR( OS::FileSystem::IsDirectoryExist( _baseFolder ), this );
		
		String			proj_name = FileAddress::BuildPath( "", _name, "vcxproj" );
		Array<String>	files;

		OS::FileSystem::RecursiveFindFiles( _baseFolder,
											LAMBDA( proj_name ) (StringCRef fname) {
												return fname.EndsWithIC( proj_name );
											},
											20, OUT files );

		CHECK_ERR( files.Count() == 1, this );

		_baseFolder = FileAddress::GetPath( files.Front() );
		return this;
	}

/*
=================================================
	ToString
=================================================
*/
	bool CMakeBuilder::CMakeExternalVSProject::ToString (OUT String &src)
	{
		CHECK_ERR( OS::FileSystem::IsDirectoryExist( _baseFolder ) );
		CHECK_ERR( OS::FileSystem::IsFileExist( FileAddress::BuildPath( _baseFolder, _name, "vcxproj" ) ) );

		String	proj_folder;
		FileAddress::AbsoluteToRelativePath( _baseFolder, _builder->_baseFolder, OUT proj_folder );

		String	filename = FileAddress::BuildPath( "${CMAKE_SOURCE_DIR}", proj_folder );
		filename = FileAddress::BuildPath( filename, _name, "vcxproj" );

		if ( not _enableIf.Empty() ) {
			src << "if (" << _enableIf << ")\n";
		}

		src << " include_external_msproject( \"" << _name << "\" \"" << filename << "\" )\n";

		if ( not _folder.Empty() ) {
			src << " set_property( TARGET \"" << _name << "\" PROPERTY FOLDER \"" << _folder << "\" )\n";
		}

		if ( not _enableIf.Empty() ) {
			src << "endif()\n";
		}
		return true;
	}
//-----------------------------------------------------------------------------

	
/*
=================================================
	constructor
=================================================
*/
	CMakeBuilder::CMakeBuilder (StringCRef baseFolder, StringCRef solutionName) :
		_solutionName( solutionName )
	{
		String	dir;
		OS::FileSystem::GetCurrentDirectory( OUT dir );

		_baseFolder = FileAddress::BuildPath( dir, baseFolder );

		if ( _solutionName.Empty() )
			_solutionName = FileAddress::GetName( _baseFolder );
	}
	
/*
=================================================
	destructor
=================================================
*/
	CMakeBuilder::~CMakeBuilder ()
	{
	}
	
/*
=================================================
	Save
=================================================
*/
	bool CMakeBuilder::Save (StringCRef filename)
	{
		if ( not OS::FileSystem::IsDirectoryExist( _baseFolder ) )
		{
			Array< StringCRef >	paths;
			String				new_path;

			FileAddress::DividePath( _baseFolder, OUT paths );

			FOR( i, paths )
			{
				FileAddress::AddDirectoryToPath( new_path, paths[i] );

				if ( not OS::FileSystem::IsDirectoryExist( new_path ) ) {
					CHECK_ERR( OS::FileSystem::NewDirectory( new_path ) );
				}
			}

			ASSERT( new_path == _baseFolder );
		}

		String	src;
		String	compiler_opt;

		src << "# auto generated file\n"
			<< "cmake_minimum_required (VERSION 3.6.3)\n\n"
			<< "project( \"" << _solutionName << "\" LANGUAGES CXX )\n"
			<< "set_property( GLOBAL PROPERTY USE_FOLDERS ON )\n"
			<< "set( " << _solutionName << "_VERSION_MAJOR " << _version.x << " )\n"
			<< "set( " << _solutionName << "_VERSION_MINOR " << _version.y << " )\n\n";

		// serialize options
		FOR( i, _userOptions ) {
			src << "option( " << _userOptions[i] << " \"test\")\n";
		}

		// serialize compilers
		FOR( i, _compilers )
		{
			CHECK( _compilers[i]->ToString( OUT src ) );
			CHECK( _compilers[i]->ToString2( OUT compiler_opt ) );
		}

		StringSet_t		existing_proj;

		// external VS projects
		{
			String	tmp;

			FOR( i, _externalVSProjects )
			{
				CHECK( _externalVSProjects[i]->ToString( OUT tmp ) );
				
				existing_proj.Add( _externalVSProjects[i].ToPtr< CMakeExternalVSProject >()->GetName() );
			}

			StringParser::IncreaceIndent( tmp );
			src << "if (MSVC)\n"
				<< tmp
				<< "endif()\n\n";
		}

		// validate projects
		FOR( i, _projects )
		{
			Ptr<CMakeProject>	proj = _projects[i].ToPtr< CMakeProject >();

			CHECK_ERR( not existing_proj.IsExist( proj->_name ) );
			existing_proj.Add( proj->_name );
		}

		// serialize projects
		FOR( i, _projects )
		{
			Ptr<CMakeProject>	proj = _projects[i].ToPtr< CMakeProject >();

			if ( not _projectOutputDir.Empty() and proj->_outputDir.Empty() )
				proj->_outputDir = _projectOutputDir;

			proj->_includeDirs.AddArray( _projectIncludeDirs );
			proj->_linkDirs.AddArray( _projectLinkDirs );
			proj->_linkLibs.AddArray( _projectLinkLibs );
			proj->_defines.AddArray( _projectDefines );
			proj->SetCompilerOptions( compiler_opt );

			// validate dependencies
			/*FOR( j, proj->_linkLibs ) {
				CHECK_ERR( existing_proj.IsExist( proj->_linkLibs[j].first ) );
			}*/

			FOR( j, proj->_dependencies ) {
				CHECK( existing_proj.IsExist( proj->_dependencies[j].first ) );
			}

			CHECK( proj->ToString( OUT src ) );
		}

		// serialize external projects
		FOR( i, _externalProjects ) {
			CHECK( _externalProjects[i]->ToString( OUT src ) );
		}

		// save project
		auto	file = File::HddWFile::New( FileAddress::BuildPath( _baseFolder, filename ) );
		CHECK_ERR( file );

		file->Write( StringCRef(src) );

		return true;
	}
	
/*
=================================================
	Clear
=================================================
*/
	void CMakeBuilder::Clear ()
	{
		_compilers.Clear();
		_projects.Clear();
		_externalProjects.Clear();
		
		_userOptions.Clear();

		_projectOutputDir.Clear();
		_projectIncludeDirs.Clear();
		_projectLinkDirs.Clear();
		_projectLinkLibs.Clear();
		_projectDefines.Clear();
	}
	
/*
=================================================
	AddLibrary
=================================================
*/
	CMakeBuilder::CMakeProject* CMakeBuilder::AddLibrary (StringCRef name, StringCRef folder)
	{
		auto	proj = new CMakeProject( this, CMakeProject::EProjectType::Library, name, FileAddress::BuildPath( _baseFolder, folder ) );
		_projects.PushBack( proj );
		return proj;
	}
	
/*
=================================================
	AddExecutable
=================================================
*/
	CMakeBuilder::CMakeProject* CMakeBuilder::AddExecutable (StringCRef name, StringCRef folder)
	{
		auto	proj = new CMakeProject( this, CMakeProject::EProjectType::Executable, name, FileAddress::BuildPath( _baseFolder, folder ) );
		_projects.PushBack( proj );
		return proj;
	}
	
/*
=================================================
	AddMSVisualStudioCompiler
=================================================
*/
	CMakeBuilder::CMakeCompiler*  CMakeBuilder::AddMSVisualStudioCompiler ()
	{
		auto	comp = new CMakeCompiler( this, CMakeCompiler::ECompilerName::MSVisualStudio );
		_compilers.PushBack( comp );
		return comp;
	}
	
/*
=================================================
	AddGCCompiler
=================================================
*/
	CMakeBuilder::CMakeCompiler*  CMakeBuilder::AddGCCompiler ()
	{
		auto	comp = new CMakeCompiler( this, CMakeCompiler::ECompilerName::GCC );
		_compilers.PushBack( comp );
		return comp;
	}
	
/*
=================================================
	AddClangCompiler
=================================================
*/
	CMakeBuilder::CMakeCompiler*  CMakeBuilder::AddClangCompiler ()
	{
		auto	comp = new CMakeCompiler( this, CMakeCompiler::ECompilerName::Clang );
		_compilers.PushBack( comp );
		return comp;
	}

/*
=================================================
	AddLibraryProjects
=================================================
*
	CMakeBuilder*  CMakeBuilder::AddLibraryProjects (StringCRef folder)
	{
		return this;
	}
	
/*
=================================================
	AddExecutableProjects
=================================================
*
	CMakeBuilder*  CMakeBuilder::AddExecutableProjects (StringCRef folder)
	{
		return this;
	}

/*
=================================================
	Projects_OutputDirectory
=================================================
*/
	CMakeBuilder* CMakeBuilder::Projects_OutputDirectory (StringCRef folder)
	{
		_projectOutputDir = folder;
		return this;
	}
	
/*
=================================================
	Projects_AddDefinition
=================================================
*/
	CMakeBuilder* CMakeBuilder::Projects_AddDefinition (StringCRef def, StringCRef enableIf)
	{
		_projectDefines.Add({ def, enableIf });
		return this;
	}
	
/*
=================================================
	Projects_IncludeDirectory
=================================================
*/
	CMakeBuilder* CMakeBuilder::Projects_IncludeDirectory (StringCRef folder, StringCRef enableIf)
	{
		_projectIncludeDirs.Add({ folder, enableIf });
		return this;
	}
	
/*
=================================================
	Projects_LinkDirectory
=================================================
*/
	CMakeBuilder* CMakeBuilder::Projects_LinkDirectory (StringCRef folder, StringCRef enableIf)
	{
		_projectLinkDirs.Add({ folder, enableIf});
		return this;
	}
	
/*
=================================================
	Projects_LinkLibrary
=================================================
*/
	CMakeBuilder* CMakeBuilder::Projects_LinkLibrary (StringCRef lib, StringCRef enableIf)
	{
		_projectLinkLibs.Add({ lib, enableIf });
		return this;
	}
	
/*
=================================================
	AddCondition
=================================================
*/
	CMakeBuilder*  CMakeBuilder::AddCondition (StringCRef name)
	{
		_userOptions.Add( name );
		return this;
	}
	
/*
=================================================
	SetVersion
=================================================
*/
	CMakeBuilder*  CMakeBuilder::SetVersion (uint major, uint minor)
	{
		_version = GXMath::uint2{ major, minor };
		return this;
	}
	
/*
=================================================
	AddExternal
=================================================
*/
	CMakeBuilder::CMakeExternalProjects*  CMakeBuilder::AddExternal (StringCRef path, StringCRef enableIf)
	{
		auto	proj = new CMakeExternalProjects( this, path, enableIf );
		_externalProjects.PushBack( proj );
		return proj;
	}
	
/*
=================================================
	AddVSProject
=================================================
*/
	CMakeBuilder::CMakeExternalVSProject*  CMakeBuilder::AddVSProject (StringCRef path, StringCRef enableIf)
	{
		auto	proj = new CMakeExternalVSProject( this, path, enableIf );
		_externalVSProjects.PushBack( proj );
		return proj;
	}
	
/*
=================================================
	SearchVSProjects
=================================================
*/
	CMakeBuilder*  CMakeBuilder::SearchVSProjects (StringCRef path, StringCRef projFolder, StringCRef enableIf)
	{
		CHECK_ERR( OS::FileSystem::IsDirectoryExist( path ), this );
		
		Array<String>	files;

		OS::FileSystem::RecursiveFindFiles( path,
											LAMBDA() (StringCRef fname) {
												return	not fname.EqualsIC( "ALL_BUILD.vcxproj" ) and
														not fname.EqualsIC( "ZERO_CHECK.vcxproj" ) and
														not fname.EqualsIC( "INSTALL.vcxproj" ) and
														not fname.EqualsIC( "PACKAGE.vcxproj" ) and
														fname.EndsWithIC( ".vcxproj" );
											},
											20, OUT files );

		FOR( i, files )
		{
			auto proj = AddVSProject( files[i], enableIf );
			
			if ( not projFolder.Empty() )
				proj->ProjFolder( projFolder );
		}
		return this;
	}
//-----------------------------------------------------------------------------


}	// CMake
