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

		for (auto& cfg : _configurations) {
			src << cfg.first << " ";
		}
		src << ")\n"
			<< "set( CMAKE_CONFIGURATION_TYPES \"${CMAKE_CONFIGURATION_TYPES}\" CACHE STRING \"Configurations\" FORCE )\n\n";

		// include directories
		for (auto& dir : _includeDirs)
		{
			const bool	has_option = not dir.second.Empty(); 

			src << (has_option ? "if ("_str << dir.second << ")\n\t" : "")
				<< "include_directories( \"" << dir.first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}
		
		// link directories
		for (auto& dir : _linkDirs)
		{
			const bool	has_option = not dir.second.Empty();
			
			src << (has_option ? "if ("_str << dir.second << ")\n\t" : "")
				<< "link_directories( \"" << dir.first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}
		
		// link libraries
		for (auto& lib : _linkLibs)
		{
			const bool	has_option = not lib.second.Empty();

			src << (has_option ? "if ("_str << lib.second << ")\n\t" : "")
				<< "link_libraries( \"" << lib.first << "\" )\n"
				<< (has_option ? "endif()\n" : "");
		}

		// serialize configurations
		for (auto& cfg : _configurations) {
			cfg.second->ToString( cfg.first, OUT src );
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
	ToString2_Opt_Pass1
=================================================
*/
	bool CMakeBuilder::CMakeCompiler::ToString2_Opt_Pass1 (OUT HashMap<String, uint> &defined) const
	{
		CHECK_ERR( not _configurations.Empty() );

		for (auto& cfg : _configurations) {
			cfg.second->ToString2_Opt_Pass1( cfg.first, OUT defined );
		}
		return true;
	}
	
/*
=================================================
	ToString2_Opt_Pass2
=================================================
*/
	bool CMakeBuilder::CMakeCompiler::ToString2_Opt_Pass2 (const HashMap<String, uint> &defined, OUT String &src) const
	{
		CHECK_ERR( not _configurations.Empty() );

		String	temp;
		
		for (auto& cfg : _configurations) {
			cfg.second->ToString2_Opt_Pass2( cfg.first, defined, OUT temp );
		}
		
		if ( not temp.Empty() )
		{
			src << "if ( " << _name << " )\n" << temp << "endif()\n\n\n";
		}
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
		for (auto& folder : folders) {
			_includeDirs.Add({ folder, enableIf });
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
	CMakeBuilder::CMakeCompiler*  CMakeBuilder::CMakeCompiler::LinkLibrary (StringCRef lib, StringCRef enableIf)
	{
		_linkLibs.Add({ lib, enableIf });
		return this;
	}
	
	CMakeBuilder::CMakeCompiler*  CMakeBuilder::CMakeCompiler::LinkLibrary (ArrayCRef<String> libs, StringCRef enableIf)
	{
		for (auto& lib : libs) {
			_linkLibs.Add({ lib, enableIf });
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

		for (auto& c : name) {
			uc_name << StringUtils::ToUpper( c );
		}
		
		src << "# " << name << "\n"
			<< "set_property( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:" << name << ">:";
		for (auto& def : _defines) { src << ' ' << def; }
		src << " > )\n";
		
		src << "set( CMAKE_C_FLAGS_" << uc_name << " \"${CMAKE_C_FLAGS}";
		for (auto& flag : _cFlags) { src << ' ' << flag; }
		src << " \" CACHE STRING \"\" FORCE )\n";

		src << "set( CMAKE_CXX_FLAGS_" << uc_name << " \"${CMAKE_CXX_FLAGS}";
		for (auto& flag : _cxxFlags) { src << ' ' << flag; }
		src << " \" CACHE STRING \"\" FORCE )\n";

		src << "set( CMAKE_EXE_LINKER_FLAGS_" << uc_name << " \"${CMAKE_EXE_LINKER_FLAGS}";
		for (auto& flag : _linkerFlags) { src << ' ' << flag; }
		src << " \" CACHE STRING \"\" FORCE )\n";

		src << "set( CMAKE_SHARED_LINKER_FLAGS_" << uc_name << " \"${CMAKE_SHARED_LINKER_FLAGS}";
		for (auto& flag : _linkerFlags) { src << ' ' << flag; }
		src << " \" CACHE STRING \"\" FORCE )\n";
		
		src << "set( PROJECTS_SHARED_CXX_FLAGS_" << uc_name << " ";
		for (auto& flag : _targetCxxFlags) { src << ' ' << flag; }
		src << " CACHE INTERNAL \"\" FORCE )\n";
		
		src << "set( PROJECTS_SHARED_DEFINES_" << uc_name << " ";
		for (auto& def : _targetDefines) { src << ' ' << def; }
		src << " CACHE INTERNAL \"\" FORCE )\n";
		
		src << "set( PROJECTS_SHARED_LINKER_FLAGS_" << uc_name << " \"";
		for (auto& flag : _targetLinkerFlags) { src << ' ' << flag; }
		src << "\" CACHE INTERNAL \"\" FORCE )\n";

		return true;
	}
	
/*
=================================================
	Configuration_CxxFlags
=================================================
*/
	inline String  Configuration_CxxFlags (StringCRef name, StringCRef upperCaseName)
	{
		return "target_compile_options( "_str << CMakeBuilder::GetTargetPlaceholder() << " PRIVATE $<$<CONFIG:"
					<< name << ">: ${PROJECTS_SHARED_CXX_FLAGS_" << upperCaseName << "}> )";
	}
	
/*
=================================================
	Configuration_Defines
=================================================
*/
	inline String  Configuration_Defines (StringCRef name, StringCRef upperCaseName)
	{
		return "target_compile_definitions( "_str << CMakeBuilder::GetTargetPlaceholder() << " PRIVATE $<$<CONFIG:"
					<< name << ">: ${PROJECTS_SHARED_DEFINES_" << upperCaseName << "}> )";
	}
	
/*
=================================================
	Configuration_LinkerFlags
=================================================
*/
	inline String  Configuration_LinkerFlags (StringCRef, StringCRef upperCaseName)
	{
		return "set_target_properties( "_str << CMakeBuilder::GetTargetPlaceholder() << " PROPERTIES LINK_FLAGS_"
					<< upperCaseName << " ${PROJECTS_SHARED_LINKER_FLAGS_" << upperCaseName << "} )";
	}

/*
=================================================
	ToString2_Opt_Pass1
=================================================
*/
	bool CMakeBuilder::CMakeCompiler::Configuration::ToString2_Opt_Pass1 (StringCRef name, OUT HashMap<String, uint> &defined) const
	{
		String	uc_name;
		for (auto& c : name) {
			uc_name << StringUtils::ToUpper( c );
		}

		auto	iter1 = defined.AddOrSkip( Configuration_CxxFlags( name, uc_name ), 0 );	iter1->second++;

		auto	iter2 = defined.AddOrSkip( Configuration_Defines( name, uc_name ), 0 );		iter2->second++;

		auto	iter3 = defined.AddOrSkip( Configuration_LinkerFlags( name, uc_name ), 0 );	iter3->second++;

		return true;
	}
	
/*
=================================================
	ToString2_Opt_Pass2
=================================================
*/
	bool CMakeBuilder::CMakeCompiler::Configuration::ToString2_Opt_Pass2 (StringCRef name, const HashMap<String, uint> &defined, OUT String &src) const
	{
		String	uc_name;
		for (auto& c : name) {
			uc_name << StringUtils::ToUpper( c );
		}

		String	temp;

		if ( not _targetCxxFlags.Empty() and not defined.IsExist( Configuration_CxxFlags( name, uc_name ) ) )
		{
			temp << '\t' << Configuration_CxxFlags( name, uc_name ) << '\n';
		}

		if ( not _targetDefines.Empty() and not defined.IsExist( Configuration_Defines( name, uc_name ) ) )
		{
			temp << '\t' << Configuration_Defines( name, uc_name ) << '\n';
		}

		if ( not _targetLinkerFlags.Empty() and not defined.IsExist( Configuration_LinkerFlags( name, uc_name ) ) )
		{
			temp << '\t' << Configuration_LinkerFlags( name, uc_name ) << '\n';
		}

		if ( not temp.Empty() )
		{
			src << "	# " << name << "\n" << temp;
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
