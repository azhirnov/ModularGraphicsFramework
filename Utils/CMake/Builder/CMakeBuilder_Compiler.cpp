// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CMakeBuilder.h"

namespace CMake
{

/*
=================================================
	constructor
=================================================
*/
	CMakeBuilder::CMakeCompiler::CMakeCompiler (CMakeBuilder *builder, ECompilerClass compiler, StringCRef name) :
		_builder( builder ), _name( name ), _compName( compiler )
	{
		if ( _name.Empty() )
		{
			switch ( _compName )
			{
				case ECompilerClass::MSVisualStudio :	_name = "MSVC";		break;
				case ECompilerClass::Clang :			_name = "CLANG";	break;
				case ECompilerClass::GCC :				_name = "GCC";		break;
				default :								_name = "UNKNOWN";
			}
		}
		"COMPILER_" >> _name;

		// TODO: validate name
	}
	
/*
=================================================
	ToString
=================================================
*/
	bool CMakeBuilder::CMakeCompiler::ToString (OUT String &outSrc)
	{
		CHECK_ERR( not _configurations.Empty() );
		
		String	src;
		switch ( _compName )
		{
			case ECompilerClass::MSVisualStudio :
				outSrc
					<< "#==================================================================================================\n"
					<< "# Visual Studio Compilation settings\n"
					<< "#==================================================================================================\n"
					<< "set( " << _name << " OFF )\n"
					<< "string( FIND \"${CMAKE_CXX_COMPILER_ID}\" \"MSVC\" outPos )\n"
					<< "if ( (outPos GREATER -1)";
					if ( !_enableIf.Empty() ) outSrc << " AND (" << _enableIf << ")";
				outSrc
					<< " )\n"
					<< "	set( " << _name << " ON )\n"
					<< "endif()\n"
					<< "if ( " << _name << " )\n";

				src << "set( DETECTED_COMPILER \"" << _name << "\" )\n"
					<< "string( REPLACE \"/EHa\" \" \" CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS}\" )\n"
					<< "string( REPLACE \"/EHsc\" \" \" CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS}\" )\n"
					<< "string( REPLACE \"/EHs\" \" \" CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS}\" )\n"
					<< "string( REPLACE \"//EHs-c-\" \" \" CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS}\" )\n";
				break;

			case ECompilerClass::Clang :
				outSrc
					<< "#==================================================================================================\n"
					<< "# Clang Compilation settings\n"
					<< "#==================================================================================================\n"
					<< "set( " << _name << " OFF )\n"
					<< "string( FIND \"${CMAKE_CXX_COMPILER_ID}\" \"Clang\" outPos )\n"
					<< "if ( (outPos GREATER -1)";
					if ( !_enableIf.Empty() ) outSrc << " AND (" << _enableIf << ")";
				outSrc
					<< " )\n"
					<< "	set( " << _name << " ON )\n"
					<< "endif()\n"
					<< "if ( " << _name << " )\n";
				
				src << "set( DETECTED_COMPILER \"" << _name << "\" )\n";
				break;

			case ECompilerClass::GCC :
				outSrc
					<< "#==================================================================================================\n"
					<< "# GCC Compilation settings\n"
					<< "#==================================================================================================\n"
					<< "set( " << _name << " OFF )\n"
					<< "string( FIND \"${CMAKE_CXX_COMPILER_ID}\" \"GNU\" outPos )\n"
					<< "if ( (outPos GREATER -1)";
					if ( !_enableIf.Empty() ) outSrc << " AND (" << _enableIf << ")";
				outSrc
					<< " )\n"
					<< "	set( " << _name << " ON )\n"
					<< "endif()\n"
					<< "if ( " << _name << " )\n";
				
				src << "set( DETECTED_COMPILER \"" << _name << "\" )\n";
				break;

			default :
				RETURN_ERR( "unknown compiler!" );
		}
		
		src << "message( STATUS \"CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}\" )\n"
			<< "message( STATUS \"CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}\" )\n"
			<< "message( STATUS \"CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}\" )\n"
			<< "message( STATUS \"CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}\" )\n";

		// declare configurations
		src << "set( CMAKE_CONFIGURATION_TYPES ";

		FOR( i, _configurations ) {
			src << _configurations[i].first << " ";
		}
		src << ")\n"
			<< "set( CMAKE_CONFIGURATION_TYPES \"${CMAKE_CONFIGURATION_TYPES}\" CACHE STRING \"Configurations\" FORCE )\n\n";

		// include directories
		FOR( i, _includeDirs )
		{
			const bool	has_option = not _includeDirs[i].second.Empty(); 

			src << (has_option ? "if ("_str << _includeDirs[i].second << ")\n\t" : "")
				<< "include_directories( \"" << _includeDirs[i].first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
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

		// set default configuration
		if ( _defaultCfg.Empty() )
		{
			if ( _configurations.IsExist( "Debug" ) )
				_defaultCfg = "Debug";
			else
				_defaultCfg = _configurations.Front().first;
		}
			
		CHECK_ERR( _configurations.IsExist( _defaultCfg ) );
		src << "set( CMAKE_BUILD_TYPE \"" << _defaultCfg << "\")\n";

		// add user-defined source
		if ( not _source.Empty() )
		{
			src << "#--------------------------------------------\n";
			src << _source << "\n";
			src << "#--------------------------------------------\n";
		}
		
		StringParser::IncreaceIndent( INOUT src );

		outSrc << src << "endif()\n\n\n";
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

		src << "if ( " << _name << " )\n";

		// serialize configurations
		FOR( i, _configurations ) {
			_configurations[i].second->ToString2( _configurations[i].first, OUT src );
		}
		
		src << "endif()\n\n\n";
		return true;
	}
	
/*
=================================================
	_CopyFrom
=================================================
*/
	void CMakeBuilder::CMakeCompiler::_CopyFrom (CMakeCompiler *other)
	{
		if ( not other )
			return;

		// check compatibility
		switch ( other->_compName )
		{
			case ECompilerClass::MSVisualStudio :
				ASSERT( _compName == ECompilerClass::MSVisualStudio );
				break;

			case ECompilerClass::Clang :
			case ECompilerClass::GCC :
				ASSERT( _compName == ECompilerClass::Clang or _compName == ECompilerClass::GCC );
				break;

			default :
				WARNING( "unknown compiler!" );
		}

		_configurations = other->_configurations;
		_defaultCfg		= other->_defaultCfg;
		_includeDirs	= other->_includeDirs;
		_linkDirs		= other->_linkDirs;
		_linkLibs		= other->_linkLibs;
		_source			= other->_source;
		_enableIf		= other->_enableIf;
	}
	
/*
=================================================
	EnableIf
=================================================
*/
	CMakeBuilder::CMakeCompiler*  CMakeBuilder::CMakeCompiler::EnableIf (StringCRef condition)
	{
		_enableIf = condition;
		return this;
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
	CMakeBuilder::CMakeCompiler*  CMakeBuilder::CMakeCompiler::IncludeDirectory (StringCRef folder, StringCRef enableIf)
	{
		_includeDirs.Add({ folder, enableIf });
		return this;
	}
	
	CMakeBuilder::CMakeCompiler*  CMakeBuilder::CMakeCompiler::IncludeDirectory (ArrayCRef<String> folders, StringCRef enableIf)
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
	CMakeBuilder::CMakeCompiler*  CMakeBuilder::CMakeCompiler::LinkDirectory (StringCRef folder, StringCRef enableIf)
	{
		_linkDirs.Add({ folder, enableIf });
		return this;
	}
	
	CMakeBuilder::CMakeCompiler*  CMakeBuilder::CMakeCompiler::LinkDirectory (ArrayCRef<String> folders, StringCRef enableIf)
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
	CMakeBuilder::CMakeCompiler*  CMakeBuilder::CMakeCompiler::LinkLibrary (StringCRef lib, StringCRef enableIf)
	{
		_linkLibs.Add({ lib, enableIf });
		return this;
	}
	
	CMakeBuilder::CMakeCompiler*  CMakeBuilder::CMakeCompiler::LinkLibrary (ArrayCRef<String> libs, StringCRef enableIf)
	{
		FOR( i, libs ) {
			_linkLibs.Add({ libs[i], enableIf });
		}
		return this;
	}
	
/*
=================================================
	AddSource
=================================================
*/
	CMakeBuilder::CMakeCompiler*  CMakeBuilder::CMakeCompiler::AddSource (StringCRef cmakeSrc)
	{
		_source = cmakeSrc;
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
		
		src << "# " << name << "\n"
			<< "set_property( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:" << name << ">:";
		FOR( i, _defines ) { src << ' ' << _defines[i]; }
		src << " > )\n";
		
		src << "set( CMAKE_C_FLAGS_" << uc_name << " \"${CMAKE_C_FLAGS}";
		FOR( i, _cFlags ) { src << ' ' << _cFlags[i]; }
		src << " \" CACHE STRING \"\" FORCE )\n";

		src << "set( CMAKE_CXX_FLAGS_" << uc_name << " \"${CMAKE_CXX_FLAGS}";
		FOR( i, _cxxFlags ) { src << ' ' << _cxxFlags[i]; }
		src << " \" CACHE STRING \"\" FORCE )\n";

		src << "set( CMAKE_EXE_LINKER_FLAGS_" << uc_name << " \"${CMAKE_EXE_LINKER_FLAGS}";
		FOR( i, _linkerFlags ) { src << ' ' << _linkerFlags[i]; }
		src << " \" CACHE STRING \"\" FORCE )\n";

		src << "set( CMAKE_SHARED_LINKER_FLAGS_" << uc_name << " \"${CMAKE_EXE_LINKER_FLAGS}";
		FOR( i, _linkerFlags ) { src << ' ' << _linkerFlags[i]; }
		src << " \" CACHE STRING \"\" FORCE )\n";
		

		if ( not _targetCxxFlags.Empty() )
		{
			src << "set( PROJECTS_SHARED_CXX_FLAGS_" << uc_name << " ";
			FOR( i, _targetCxxFlags ) { src << ' ' << _targetCxxFlags[i]; }
			src << " CACHE INTERNAL \"\" FORCE )\n";
		}
		
		if ( not _targetDefines.Empty() )
		{
			src << "set( PROJECTS_SHARED_DEFINES_" << uc_name << " ";
			FOR( i, _targetDefines ) { src << ' ' << _targetDefines[i]; }
			src << " CACHE INTERNAL \"\" FORCE )\n";
		}
		
		if ( not _targetLinkerFlags.Empty() )
		{
			src << "set( PROJECTS_SHARED_LINKED_FLAGS_" << uc_name << " \"";
			FOR( i, _targetLinkerFlags ) { src << ' ' << _targetLinkerFlags[i]; }
			src << "\" CACHE INTERNAL \"\" FORCE )\n";
		}
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
		
		if ( not _targetCxxFlags.Empty()	or
			 not _targetDefines.Empty()		or
			 not _targetLinkerFlags.Empty() )
		{
			src << "	# " << name << "\n";
		}

		if ( not _targetCxxFlags.Empty() )
		{
			src << "	target_compile_options( " << CMakeBuilder::GetTargetPlaceholder() << " PRIVATE $<$<CONFIG:" << name << ">: ${PROJECTS_SHARED_CXX_FLAGS_" << uc_name << "}> )\n";
		}

		if ( not _targetDefines.Empty() )
		{
			src << "	target_compile_definitions( " << CMakeBuilder::GetTargetPlaceholder() << " PRIVATE $<$<CONFIG:" << name << ">: ${PROJECTS_SHARED_DEFINES_" << uc_name << "}> )\n";
		}

		if ( not _targetLinkerFlags.Empty() )
		{
			src << "	set_target_properties( " << CMakeBuilder::GetTargetPlaceholder() << " PROPERTIES LINK_FLAGS_" << uc_name << " ${PROJECTS_SHARED_LINKED_FLAGS_" << uc_name << "} )\n";
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

}	// CMake
