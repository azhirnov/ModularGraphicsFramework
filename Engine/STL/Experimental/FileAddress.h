// Copyright © 2014-2016  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Containers/String.h"
#include "Engine/STL/Containers/Buffer.h"
#include "Engine/STL/Containers/Array.h"
#include "Engine/STL/Containers/StaticArray.h"
#include "Engine/STL/Math/Mathematics.h"
#include "Engine/STL/Math/Vec.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// File Address Utils
	//

	// TODO: rename to FileAddress
	struct FileAddressUtils
	{
	// methods
	public:
		static StringCRef	GetPath (StringCRef filename);
		static StringCRef	GetName (StringCRef filename);
		static StringCRef	GetExtension (StringCRef filename);
		static StringCRef	GetNameAndExt (StringCRef filename);

		static StringCRef	ToShortPath (StringCRef filename, uint maxFolders = 1);

		static void		DividePath (StringCRef filename, OUT Array<StringCRef> &path);

		static void		FormatPath (INOUT String &filename);
		//static bool		IsNeedToFormatPath (StringCRef filename);	// TODO

		static void		RemoveName (INOUT String &filename);
		static void		RemoveExtension (INOUT String &filename);
		static bool		PathMoveBack (INOUT String &path);

		static bool		RemoveDirectoriesFromLeft (INOUT String &path, uint numDirectories);
		static bool		RemoveDirectoriesFromRight (INOUT String &path, uint numDirectories);

		static void		AddDirectoryToPath (INOUT String &path, StringCRef dir);
		static void		AddBaseDirectoryToPath (INOUT String &path, StringCRef dir);
		static void		AddNameToPath (INOUT String &path, StringCRef name);
		static void		AddExtensionToName (INOUT String &filename, StringCRef ext);

		static String &	BuildPath (OUT String &result, StringCRef path, StringCRef nameWithExt);
		static String &	BuildPath (OUT String &result, StringCRef path, StringCRef name, StringCRef ext);

	private:
		static usize	_GetExtensionPos (StringCRef filename);
		static usize	_GetNamePos (StringCRef filename);
		static usize	_SafeInc (usize value, StringCRef str);
	};



	//
	// File Address Builder (TODO: do not use this class! there is many strings with dynamic allocations)
	//

	// TODO: remove
	struct FileAddress : public CompileTime::CopyQualifiers< String >
	{
	// types
	public:
		typedef FileAddress		Self;


	// variables
	private:
		String			_prefix;
		String			_name;
		mutable String	_path;
		mutable String	_full;
		mutable bool	_changed;

		
	// methods
	public:
		FileAddress (GX_DEFCTOR) : _changed(true)
		{}

		FileAddress (StringCRef filename) : _changed(true)
		{
			_path = FileAddressUtils::GetPath( filename );
			_name = FileAddressUtils::GetNameAndExt( filename );
		}

		FileAddress (Self &&other) :
			_prefix( RVREF(other._prefix) ),
			_name( RVREF(other._name) ),
			_path( RVREF(other._path) ),
			_full( RVREF(other._full) ),
			_changed( other._changed )
		{
		}

		Self & SetPrefix (StringCRef prefix)
		{
			_changed = true;
			_prefix = prefix;
			return *this;
		}

		Self & SetPath (StringCRef path)
		{
			_changed = true;
			_path = path;
			return *this;
		}
		
		Self & AddPath (StringCRef dir)
		{
			_changed = true;
			FileAddressUtils::AddDirectoryToPath( _path, dir ); 
			return *this;
		}

		Self & AddBasePath (StringCRef dir)
		{
			_changed = true;
			FileAddressUtils::AddBaseDirectoryToPath( _path, dir );
			return *this;
		}

		bool MoveBackDir ()
		{
			_changed = true;
			return FileAddressUtils::PathMoveBack( _path );
		}

		Self & SetName (StringCRef nameWithExt)
		{
			_changed = true;
			_name = nameWithExt;
			return *this;
		}

		Self & AddExtension (StringCRef ext)
		{
			_changed = true;

			if ( not _name.Empty() and _name.Back() != '.' )
				_name << '.';

			_name << ext;
			return *this;
		}

		Self & ChangeExtension (StringCRef ext)
		{
			_changed = true;
			FileAddressUtils::RemoveExtension( _name );
			_name << '.' << ext;
			return *this;
		}

		Self & operator / (StringCRef dir)
		{
			return AddPath( dir );
		}

		bool operator == (const Self &right) const
		{
			return Full() == right.Full();
		}

		bool operator != (const Self &right) const
		{
			return not( (*this) == right );
		}

		bool operator > (const Self &right) const
		{
			return Full() < right.Full();
		}

		bool operator >= (const Self &right) const
		{
			return Full() >= right.Full();
		}

		bool operator < (const Self &right) const
		{
			return right > (*this);
		}
	
		bool operator <= (const Self &right) const
		{
			return not ( (*this) > right );
		}

		StringCRef	Full () const
		{
			if ( _changed )
			{
				_changed = false;

				FileAddressUtils::FormatPath( _path );

				_full.Clear();
				_full << _prefix << _path;
				FileAddressUtils::AddNameToPath( _full, _name );
			}
			return _full;
		}

		StringCRef	Path ()	const			{ return _path; }
		StringCRef	Name () const			{ return FileAddressUtils::GetName( _name ); }
		StringCRef	Ext () const			{ return FileAddressUtils::GetExtension( _name ); }
		StringCRef	NameWithExt () const	{ return _name; }
	};


}	// GXTypes
}	// GX_STL
