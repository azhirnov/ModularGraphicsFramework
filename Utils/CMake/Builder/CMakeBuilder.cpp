// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CMakeBuilder.h"

namespace CMake
{
	
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
		CHECK_ERR( not _solutionName.Empty() );

		// create directory
		if ( not OS::FileSystem::IsDirectoryExist( _baseFolder ) )
		{
			CHECK_ERR( OS::FileSystem::CreateDirectories( _baseFolder ) );
		}

		_ClearSCUBuildDir();

		String	src;
		src << "# auto generated file\n"
			<< "cmake_minimum_required (VERSION 3.6.0)\n\n"
			<< "message( STATUS \"==========================================================================\\n\" )\n"
			<< "message( STATUS \"project '" << _solutionName << "' generation started\" )\n";

		_SetupProject( OUT src );
		
		src	<< "\n"
			<< "project( \"" << _solutionName << "\" LANGUAGES CXX )\n"
			<< "set_property( GLOBAL PROPERTY USE_FOLDERS ON )\n"
			<< "set( " << _solutionName << "_VERSION_MAJOR " << _version.x << " )\n"
			<< "set( " << _solutionName << "_VERSION_MINOR " << _version.y << " )\n\n";

		_OptionsToString( OUT src );

		String	compiler_opt;
		CHECK_ERR( _CompilersToString( OUT src, OUT compiler_opt ) );

		// for debuging
		src	<< "message( STATUS \"Compiler: ${CMAKE_CXX_COMPILER_ID} (${CMAKE_CXX_COMPILER_VERSION})\" )\n"
			<< "message( STATUS \"Compiler name: ${DETECTED_COMPILER}\" )\n"
			<< "message( STATUS \"target system: ${CMAKE_SYSTEM_NAME} (${CMAKE_SYSTEM_VERSION})\" )\n"
			<< "message( STATUS \"host system: ${CMAKE_HOST_SYSTEM_NAME} (${CMAKE_HOST_SYSTEM_VERSION})\" )\n\n";

		CHECK_ERR( _ExternalProjectsToString( OUT src ) );

		CHECK_ERR( _ProjectsToString( OUT src, compiler_opt ) );

		src << "message( STATUS \"project '" << _solutionName << "' generation ended\" )\n"
			<< "message( STATUS \"\\n==========================================================================\" )\n\n";

		// save project
		auto	file = File::HddWFile::New( FileAddress::BuildPath( _baseFolder, filename ) );
		CHECK_ERR( file );

		file->Write( StringCRef(src) );

		return true;
	}
	
/*
=================================================
	_ClearSCUBuildDir
=================================================
*/
	void CMakeBuilder::_ClearSCUBuildDir () const
	{
		// clear fast build files
		String	path = FileAddress::BuildPath( _baseFolder, CMakeBuilder::GetTempFolderForFastCpp() );

		if ( OS::FileSystem::IsDirectoryExist( path ) )
		{
			CHECK( OS::FileSystem::DeleteDirectory( path ) );
		}
	}
	
/*
=================================================
	_SetupProject
=================================================
*/
	void CMakeBuilder::_SetupProject (OUT String &src) const
	{
		// system version and other options
		FOR( i, _sourceBeforeProject )
		{
			auto&	opt = _sourceBeforeProject[i];

			src << (opt.second.Empty() ? "" : "if ( "_str << opt.second << " )\n\t")
				<< opt.first << "\n"
				<< (opt.second.Empty() ? "" : "endif()\n");
		}
	}

/*
=================================================
	_OptionsToString
=================================================
*/
	void CMakeBuilder::_OptionsToString (OUT String &src) const
	{
		// serialize options
		FOR( i, _userOptions ) {
			src << "option( " << _userOptions[i] << " \"test\")\n";
		}
	}

/*
=================================================
	_CompilersToString
=================================================
*/
	bool CMakeBuilder::_CompilersToString (OUT String &src, OUT String &compilerOpts) const
	{
		// check compilers
		{
			HashSet<String>	unique_names;
			FOR( i, _compilers )
			{
				String&	name = _compilers[i].ToPtr<CMakeCompiler>()->_name;

				if ( unique_names.IsExist( name ) )
				{
					// make unique name
					String	new_name;

					for (uint j = 1; j < 100; ++j)
					{
						new_name.Clear();
						new_name << name << '_' << j;
						
						if ( not unique_names.IsExist( new_name ) )
						{
							name = RVREF(new_name);
							break;
						}
					}
				}

				unique_names.Add( name );
			}
		}

		// serialize compilers
		HashMap<String, uint>	defines;

		FOR( i, _compilers )
		{
			auto comp = _compilers[i].ToPtr<CMakeCompiler>();

			CHECK( comp->ToString( OUT src ) );
			CHECK( comp->ToString2_Opt_Pass1( OUT defines ) );
		}

		FOR( i, defines )
		{
			const auto&	item = defines[i];

			if ( item.second == 1 )
				continue;

			compilerOpts << item.first << '\n';
		}

		FOR( i, _compilers )
		{
			CHECK( _compilers[i].ToPtr<CMakeCompiler>()->ToString2_Opt_Pass2( defines, OUT compilerOpts ) );
		}

		src << "if ( NOT DEFINED DETECTED_COMPILER )\n"
			<< "	message( FATAL_ERROR \"current compiler: '${CMAKE_CXX_COMPILER_ID}' is not configured for this project!\" )\n"
			<< "endif()\n\n";

		return true;
	}
	
/*
=================================================
	_ProjectsToString
=================================================
*/
	bool CMakeBuilder::_ProjectsToString (OUT String &src, StringCRef compilerOpts) const
	{
		StringSet_t		existing_proj;

		// external VS projects
		{
			String	tmp;

			FOR( i, _externalVSProjects )
			{
				CHECK( _externalVSProjects[i]->ToString( OUT tmp ) );
				
				existing_proj.Add( _externalVSProjects[i].ToPtr< CMakeExternalVSProject >()->GetName() );
			}

			if ( not tmp.Empty() )
			{
				StringParser::IncreaceIndent( tmp );
				src << "# External VS projects\n"
					<< "if (MSVC)\n"
					<< tmp
					<< "endif()\n\n";
			}
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
			proj->SetCompilerOptions( compilerOpts );

			// validate dependencies
			/*FOR( j, proj->_linkLibs ) {
				CHECK_ERR( existing_proj.IsExist( proj->_linkLibs[j].first ) );
			}*/

			FOR( j, proj->_dependencies ) {
				CHECK( existing_proj.IsExist( proj->_dependencies[j].first ) );
			}

			CHECK( proj->ToString( OUT src ) );
		}
		return true;
	}
	
/*
=================================================
	_ExternalProjectsToString
=================================================
*/
	bool CMakeBuilder::_ExternalProjectsToString (OUT String &src) const
	{
		if ( not _externalProjects.Empty() )
		{
			src << "message( STATUS \"adding external projects\" )\n\n\n";

			FOR( i, _externalProjects ) {
				CHECK( _externalProjects[i]->ToString( OUT src ) );
			}
		}
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
	CMakeBuilder::CMakeCompiler*  CMakeBuilder::AddMSVisualStudioCompiler (StringCRef name, CMakeCompiler *copyFrom)
	{
		auto	comp = new CMakeCompiler( this, CMakeCompiler::ECompilerClass::MSVisualStudio, name );
		comp->_CopyFrom( copyFrom );
		_compilers.PushBack( comp );
		return comp;
	}
	
/*
=================================================
	AddGCCompiler
=================================================
*/
	CMakeBuilder::CMakeCompiler*  CMakeBuilder::AddGCCompiler (StringCRef name, CMakeCompiler *copyFrom)
	{
		auto	comp = new CMakeCompiler( this, CMakeCompiler::ECompilerClass::GCC, name );
		comp->_CopyFrom( copyFrom );
		_compilers.PushBack( comp );
		return comp;
	}
	
/*
=================================================
	AddClangCompiler
=================================================
*/
	CMakeBuilder::CMakeCompiler*  CMakeBuilder::AddClangCompiler (StringCRef name, CMakeCompiler *copyFrom)
	{
		auto	comp = new CMakeCompiler( this, CMakeCompiler::ECompilerClass::Clang, name );
		comp->_CopyFrom( copyFrom );
		_compilers.PushBack( comp );
		return comp;
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
	
/*
=================================================
	GetCPPFilters
=================================================
*/
	HashSet<String> const&  CMakeBuilder::GetCPPFilters ()
	{
		static const HashSet<String>	set = {
			"C", "CC", "CPP", "CXX", "H", "HH", "HPP", "HXX"
		};
		return set;
	}
	
/*
=================================================
	SetSystemVersion
=================================================
*/
	CMakeBuilder* CMakeBuilder::SetSystemVersion (StringCRef value, StringCRef enableIf)
	{
		String	sys_ver = "set( CMAKE_SYSTEM_VERSION \""_str << value << "\" CACHE TYPE INTERNAL FORCE )";

		_sourceBeforeProject.Add({ sys_ver, enableIf });
		return this;
	}
	
/*
=================================================
	GetTargetPlaceholder
=================================================
*/
	StringCRef CMakeBuilder::GetTargetPlaceholder ()
	{
		return "#target#";
	}
	
/*
=================================================
	GetTempFolderForFastCpp
=================================================
*/
	StringCRef CMakeBuilder::GetTempFolderForFastCpp ()
	{
		return "__gxtemp__";
	}
//-----------------------------------------------------------------------------

}	// CMake
