// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Types.h"

#ifdef GX_SQLITE_DATABASE_ENABLED

#include "Utf8StringUtils.h"
#include "sqLite/sqlite3.h"

namespace GX_STL
{
namespace GXDataBase
{
	/*
		struct DBIterator
		{
			void BeginRow (uint numColumns) {}
			void EndRow () {}

			void Column (uint index, StringCRef name, StringCRef value) {}
		};
	*/



	//
	// Simple DataBase
	//

	struct SimpleDB
	{
	// variables
	private:	
		Ptr< sqlite3 >	_db;


	// methods
	public:
		SimpleDB ();
		SimpleDB (StringCRef filename);
		~SimpleDB ();

		bool Open (StringCRef filename);
		void Close ();

		bool IsOpened () const		{ return _db; }


		template <typename T>
		bool Exec (T &cmd, StringCRef sql, bool showError = false);


	private:
		template <typename T>
		static int _Callback (void *NotUsed, int argc, char **argv, char **azColName);

	};

	

/*
=================================================
	constructor
=================================================
*/
	inline SimpleDB::SimpleDB ()
	{
	}
	
/*
=================================================
	constructor
=================================================
*/
	inline SimpleDB::SimpleDB (StringCRef filename)
	{
		Open( filename );
	}

/*
=================================================
	desctructor
=================================================
*/
	inline SimpleDB::~SimpleDB ()
	{
		Close();
	}
	
/*
=================================================
	Open
=================================================
*/
	inline bool SimpleDB::Open (StringCRef filename)
	{
		Close();

		int rc = sqlite3_open( filename.ptr(), &_db.ref() );

		if ( rc != SQLITE_OK ) {
			sqlite3_close( _db );
			return false;
		}
		return true;
	}
	
/*
=================================================
	Close
=================================================
*/
	inline void SimpleDB::Close ()
	{
		if ( _db )
		{
			sqlite3_close( _db );
			_db = null;
		}
	}

/*
=================================================
	Exec
=================================================
*/
	template <typename T>
	inline bool SimpleDB::Exec (T &cmd, StringCRef sql, bool showError)
	{
		int	rc = 0;

		if ( showError )
		{
			char *zErrMsg = null;

			rc = sqlite3_exec( _db, sql.cstr(), _Callback<T>, &cmd, &zErrMsg );
			
			if ( zErrMsg != null )
			{
				WARNING( ("SQL error: "_str << zErrMsg).cstr() );
				sqlite3_free( zErrMsg );
			}
		}
		else
		{
			rc = sqlite3_exec( _db, sql.cstr(), _Callback<T>, &cmd, null );
		}
		return rc == SQLITE_OK;
	}
		
/*
=================================================
	_Callback
=================================================
*/
	template <typename T>
	inline int SimpleDB::_Callback (void *param, int argc, char **argv, char **azColName)
	{
		T &	cmd = *(T*) param; 

		cmd.BeginRow( argc );

		for (int i = 0; i < argc; ++i)
		{
			cmd.Column( i, azColName[i], argv[i] );
		}

		cmd.EndRow();
		return 0;
	}


}	// GXDataBase
}	// GX_STL

#endif	// GX_SQLITE_DATABASE_ENABLED
