// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/ResourcePacker/Packer/Common.h"

namespace ResPack
{

	//
	// Script Helper
	//
	struct ScriptHelperImpl final
	{
	// variables
	private:
		GXScript::ScriptEngine	_scriptEngine;

		Stack<String>			_scriptFiles;


	// methods
	public:
		ScriptHelperImpl ();
		
		bool RunScript (StringCRef fname);
		
		StringCRef					CurrentFileName ()	const	{ return _scriptFiles.Get(); }

		GXScript::ScriptEnginePtr	GetScriptEngine ()			{ return &_scriptEngine; }

		static ScriptHelperImpl& Instance ();
	};
	
	
	
/*
=================================================
	RunScript
=================================================
*/
	bool ScriptHelper::RunScript (StringCRef filename)
	{
		return ScriptHelperImpl::Instance().RunScript( filename );
	}
	
/*
=================================================
	CurrentFileName
=================================================
*/
	StringCRef ScriptHelper::CurrentFileName ()
	{
		return ScriptHelperImpl::Instance().CurrentFileName();
	}
	
/*
=================================================
	GetScriptEngine
=================================================
*/
	GXScript::ScriptEnginePtr ScriptHelper::GetScriptEngine ()
	{
		return ScriptHelperImpl::Instance().GetScriptEngine();
	}

/*
=================================================
	Instance
=================================================
*/
	ScriptHelperImpl&  ScriptHelperImpl::Instance ()
	{
		static ScriptHelperImpl	inst;
		return inst;
	}
	
/*
=================================================
	constructor
=================================================
*/
	ScriptHelperImpl::ScriptHelperImpl ()
	{
		_scriptFiles.SetDefault( "" );
	}

/*
=================================================
	RunScript
=================================================
*/
	bool ScriptHelperImpl::RunScript (StringCRef fname)
	{
		using namespace GXScript;

		LOG( "RunScript: '"_str << fname << "'", ELog::Debug );

		_scriptFiles.Push( fname );

		// open & read
		File::RFilePtr	file = File::HddRFile::New( fname );
		CHECK_ERR( file );

		const usize	len		= usize(file->RemainingSize());
		String		buf;	buf.Resize( len );

		CHECK_ERR( file->Read( buf.ptr(), buf.LengthInBytes() ) );
		file = null;


		// find macro
		const StringCRef	angelscript_macro{ "ANGELSCRIPT" };
		usize				pos = 0;
		String				src = buf;

		if ( buf.Find( angelscript_macro, OUT pos ) )
		{
			usize	begin = pos;
			StringParser::ToNextLine( buf, INOUT begin );

			StringCRef	line;
			StringParser::ReadCurrLine( buf, INOUT pos, OUT line );

			Array<StringCRef>	tokens;
			StringParser::DivideString_CPP( line, OUT tokens );

			CHECK_ERR( tokens.Count() >= 2 and tokens[0] == "#" and tokens[1] == "ifdef" and tokens[2] == angelscript_macro );

			usize	end = UMax;
			CHECK_ERR( buf.Find( "endif", OUT end, begin ) );
			
			StringParser::ToBeginOfLine( buf, INOUT end );

			src = buf.SubString( begin, end - begin );
		}


		// run script
		ScriptModule	mod { &_scriptEngine };

		CHECK_ERR( mod.Create( fname ) );
		CHECK_ERR( mod.Run( src, "main" ) );
		
		_scriptFiles.Pop();
		return true;
	}

}	// ResPack
