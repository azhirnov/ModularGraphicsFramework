// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CMakeBuilder.h"

namespace CMake
{
	
/*
=================================================
	constructor
=================================================
*/
	CMakeBuilder::CMakeBuilder ()
	{}

	CMakeBuilder::CMakeBuilder (StringCRef baseFolder, StringCRef solutionName)
	{
		SetSolution( baseFolder, solutionName );
	}
	
/*
=================================================
	destructor
=================================================
*/
	CMakeBuilder::~CMakeBuilder ()
	{}
	
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
			<< "cmake_minimum_required (VERSION 3.6.0)\n\n";

		if ( not _isSecondary ) {
			src	<< "message( STATUS \"==========================================================================\\n\" )\n"
				<< "message( STATUS \"project '" << _solutionName << "' generation started\" )\n";
		}
		_SetupProject( OUT src );
		
		if ( not _isSecondary ) {
			src	<< "\n"
				<< "project( \"" << _solutionName << "\" LANGUAGES CXX )\n"
				<< "set_property( GLOBAL PROPERTY USE_FOLDERS ON )\n";
		}

		if ( IsNotZero( _version ) ) {
			src	<< "set( " << _solutionName << "_VERSION_MAJOR " << _version.x << " )\n"
				<< "set( " << _solutionName << "_VERSION_MINOR " << _version.y << " )\n\n";
		}

		_OptionsToString( OUT src );

		if ( not _compilers.Empty() )
		{
			_compilerOptions.Clear();
			CHECK_ERR( _CompilersToString( OUT src, OUT _compilerOptions ) );
		}

		if ( not _isSecondary ) {
			src	<< "message( STATUS \"Compiler: ${CMAKE_CXX_COMPILER_ID} (${CMAKE_CXX_COMPILER_VERSION})\" )\n"
				<< "message( STATUS \"Compiler name: ${DETECTED_COMPILER}\" )\n"
				<< "message( STATUS \"target system: ${CMAKE_SYSTEM_NAME} (${CMAKE_SYSTEM_VERSION})\" )\n"
				<< "message( STATUS \"host system: ${CMAKE_HOST_SYSTEM_NAME} (${CMAKE_HOST_SYSTEM_VERSION})\" )\n\n";
		}

		CHECK_ERR( _ProjectsToString( OUT src, _compilerOptions ) );

		if ( not _isSecondary ) {
			src << "message( STATUS \"project '" << _solutionName << "' generation ended\" )\n"
				<< "message( STATUS \"\\n==========================================================================\" )\n\n";
		}

		// save project
		auto	file = GXFile::HddWFile::New( FileAddress::BuildPath( _baseFolder, filename ) );
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
		for (auto& opt : _sourceBeforeProject)
		{
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
		for (auto& opt : _userOptions) {
			src << opt;
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

			for (const auto& comp : _compilers)
			{
				String&	name = comp.ToPtr<CMakeCompiler>()->_name;

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
		
		for (const auto& obj : _compilers)
		{
			auto comp = obj.ToPtr<CMakeCompiler>();

			CHECK( comp->ToString( OUT src ) );
			CHECK( comp->ToString2_Opt_Pass1( OUT defines ) );
		}

		for (const auto& item : defines)
		{
			if ( item.second == 1 )
				continue;

			compilerOpts << item.first << '\n';
		}

		for (const auto& comp : _compilers)
		{
			CHECK( comp.ToPtr<CMakeCompiler>()->ToString2_Opt_Pass2( defines, OUT compilerOpts ) );
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
		// serialize projects
		for (auto& obj : _projects)
		{
			if ( auto proj = DynCast< CMakeProject *>( obj ) )
			{
				if ( not _projectOutputDir.Empty() and proj->_outputDir.Empty() )
					proj->_outputDir = _projectOutputDir;

				proj->_includeDirs.AddArray( _projectIncludeDirs );
				proj->_linkDirs.AddArray( _projectLinkDirs );
				proj->_linkLibs.AddArray( _projectLinkLibs );
				proj->_defines.AddArray( _projectDefines );
				proj->SetCompilerOptions( compilerOpts );
			}

			CHECK( obj->ToString( OUT src ) );
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
	GetProject
=================================================
*/
	CMakeBuilder::CMakeProject const*  CMakeBuilder::GetProject (StringCRef name)
	{
		for (const auto& proj : _projects)
		{
			if ( auto* p = DynCast< CMakeProject const *>(proj) )
			{
				if ( p->GetName() == name )
					return p;
			}
		}
		return null;
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
		String	path;
		if ( OS::FileSystem::IsAbsolutePath( folder ) or Utils::ContainsMacro( folder ) ) {
			path = folder;
		} else {
			path = FileAddress::BuildPath( _baseFolder, folder );
			FileAddress::FormatPath( INOUT path );
		}

		_projectIncludeDirs.Add({ path, enableIf });
		return this;
	}
	
/*
=================================================
	Projects_LinkDirectory
=================================================
*/
	CMakeBuilder* CMakeBuilder::Projects_LinkDirectory (StringCRef folder, StringCRef enableIf)
	{
		String	path;
		if ( OS::FileSystem::IsAbsolutePath( folder ) or Utils::ContainsMacro( folder ) ) {
			path = folder;
		} else {
			path = FileAddress::BuildPath( _baseFolder, folder );
			FileAddress::FormatPath( INOUT path );
		}

		_projectLinkDirs.Add({ path, enableIf});
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
	AddOption
=================================================
*/
	CMakeBuilder*  CMakeBuilder::AddOption (StringCRef name, StringCRef info, bool value, StringCRef enableIf)
	{
		String	str;
		
		if ( not enableIf.Empty() )
			str << "if (" << enableIf << ")\n\t";

		str << "set( " << name << " " << (value ? "ON" : "OFF") << " CACHE BOOL \"" << info << "\" " << " )\n";
		
		if ( not enableIf.Empty() )
			str << "endif()\n";

		_userOptions.Add( RVREF(str) );
		return this;
	}
	
/*
=================================================
	AddOption
=================================================
*/
	CMakeBuilder*  CMakeBuilder::AddOption (StringCRef name, StringCRef info, int value, StringCRef enableIf)
	{
		String	str;
		
		if ( not enableIf.Empty() )
			str << "if (" << enableIf << ")\n\t";

		str << "set( " << name << " \"" << value << "\" CACHE STRING \"" << info << "\" " << " )\n";
		
		if ( not enableIf.Empty() )
			str << "endif()\n";

		_userOptions.Add( RVREF(str) );
		return this;
	}
	
/*
=================================================
	AddOption
=================================================
*/
	CMakeBuilder*  CMakeBuilder::AddOption (StringCRef name, StringCRef info, StringCRef value, StringCRef enableIf)
	{
		String	str;

		if ( not enableIf.Empty() )
			str << "if (" << enableIf << ")\n\t";

		str << "set( " << name << " \"" << value << "\" CACHE STRING \"" << info << "\" " << " )\n";

		if ( not enableIf.Empty() )
			str << "endif()\n";

		_userOptions.Add( RVREF(str) );
		return this;
	}
	
/*
=================================================
	SetSolution
=================================================
*/
	CMakeBuilder*  CMakeBuilder::SetSolution (StringCRef baseFolder, StringCRef solutionName)
	{
		_solutionName = solutionName;
	
		String	dir;
		OS::FileSystem::GetCurrentDirectory( OUT dir );

		_baseFolder = FileAddress::BuildPath( dir, baseFolder );

		if ( _solutionName.Empty() )
			_solutionName = FileAddress::GetName( _baseFolder );

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
	SetSecondary
=================================================
*/
	CMakeBuilder*  CMakeBuilder::SetSecondary (bool value)
	{
		_isSecondary = value;
		return this;
	}

/*
=================================================
	InheritCompilers
=================================================
*/
	CMakeBuilder*  CMakeBuilder::InheritCompilers (const CMakeBuilder* other)
	{
		CHECK_ERR( other and (not other->_compilerOptions.Empty() or not other->_compilers.Empty()) );

		if ( other->_compilerOptions.Empty() and not other->_compilers.Empty() )
		{
			String	src;
			_compilerOptions.Clear();
			
			CHECK_ERR( other->_CompilersToString( OUT src, OUT _compilerOptions ) );
		}
		else
		{
			_compilerOptions = other->_compilerOptions;
		}
		return this;
	}
	
/*
=================================================
	InheritIncludeDirectories
=================================================
*/
	CMakeBuilder*  CMakeBuilder::InheritIncludeDirectories (const CMakeBuilder* other)
	{
		_projectIncludeDirs.AddArray( other->_projectIncludeDirs );
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
		_projects.PushBack( proj );
		return proj;
	}
	
/*
=================================================
	AddExternal
=================================================
*/
	CMakeBuilder::CMakeExternalProjects*  CMakeBuilder::AddExternal (const CMakeBuilder* builder, StringCRef enableIf)
	{
		String	path;
		CHECK_ERR( FileAddress::AbsoluteToRelativePath( builder->_baseFolder, _baseFolder, OUT path ) );

		return AddExternal( path, enableIf );
	}
	
/*
=================================================
	IncludeExternal
=================================================
*/
	CMakeBuilder::CMakeExternalInclude*  CMakeBuilder::IncludeExternal (StringCRef filename, StringCRef enableIf)
	{
		auto	proj = new CMakeExternalInclude( this, filename, enableIf );
		_projects.PushBack( proj );
		return proj;
	}
	
/*
=================================================
	AddSource
=================================================
*/
	CMakeBuilder::CMakeSource*  CMakeBuilder::AddSource (StringCRef source, StringCRef enableIf)
	{
		auto	proj = new CMakeSource( source, enableIf );
		_projects.PushBack( proj );
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
		_projects.PushBack( proj );
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

		for (auto& file : files)
		{
			auto proj = AddVSProject( file, enableIf );
			
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
	

/*
=================================================
	ContainsMacro
=================================================
*/
	bool CMakeBuilder::Utils::ContainsMacro (StringCRef str)
	{
		usize	pos = 0;

		if ( str.Find( "${", OUT pos ) )
		{
			if ( str.Find( '}', OUT pos, pos ) )
				return true;
		}
		return false;
	}
//-----------------------------------------------------------------------------

}	// CMake
