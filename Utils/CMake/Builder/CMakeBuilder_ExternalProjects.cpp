// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CMakeBuilder.h"

namespace CMake
{

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

		//CHECK_ERR( _ReqursiveGetTargets( EKeyType::SubDir, dir_stack, OUT targets ) );

		outSrc	<< "#==================================================================================================\n"
				<< "# External: " << _path << "\n"
				<< "#==================================================================================================\n";

		if ( not _enableIf.Empty() )
			outSrc << "if (" << _enableIf << ")\n";

		src	<< "message( STATUS \"-----------------------------------------------------\" )\n"
			<< "message( STATUS \"external project '" << _path << "' generation started\" )\n";

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
		
		src	<< "\n"
			<< "message( STATUS \"external project '" << _path << "' generation ended\" )\n"
			<< "message( STATUS \"-----------------------------------------------------\" )\n";

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

				// TODO: check builtin includes
				if ( line.EqualsIC( "CMakePackageConfigHelpers" ) or
					 line.EqualsIC( "ExternalProject" ) )
					return true;

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


}	// CMake
