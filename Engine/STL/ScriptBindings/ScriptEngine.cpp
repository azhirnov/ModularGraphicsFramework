// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "ScriptEngine.h"

#ifdef GX_ANGELSCRIPT_ENABLED

namespace GX_STL
{
namespace GXScript
{
	
/*
=================================================
	constructor
=================================================
*/
	ScriptEngine::ScriptEngine ()
	{
		using namespace AngelScript;

		_engine = asCreateScriptEngine( ANGELSCRIPT_VERSION );

		_engine->SetMessageCallback( asFUNCTION( _MessageCallback ), 0, asCALL_CDECL );
	}
	
/*
=================================================
	constructor
=================================================
*/
	ScriptEngine::ScriptEngine (AngelScript::asIScriptEngine *se)
	{
		CHECK( se != null );

		_engine = se;
		_engine->AddRef();
	}
	
/*
=================================================
	destructor
=================================================
*/
	ScriptEngine::~ScriptEngine ()
	{
		_engine->ShutDownAndRelease();
	}
	
/*
=================================================
	Run
=================================================
*/
	bool ScriptEngine::Run (StringCRef script, StringCRef entry)
	{
		using namespace AngelScript;

		String	s;
		GlobalFunction< void (*) () >::GetDescriptor( s, entry.cstr() );
		
		asIScriptModule *	mod = _engine->GetModule( "def", asGM_CREATE_IF_NOT_EXISTS );
		mod->AddScriptSection( "def_script", script.cstr() );
		
		AS_CALL_R( mod->Build() );
		
		asIScriptContext *	ctx = _engine->CreateContext();
		
		asIScriptFunction * func = mod->GetFunctionByDecl( s.cstr() );
		CHECK_ERR( func != null );

		AS_CALL_R( ctx->Prepare( func ) );
		
		const int exec_res = ctx->Execute();

		if ( exec_res == asEXECUTION_FINISHED )
		{
		}
		else
		if ( exec_res == asEXECUTION_EXCEPTION )
		{
			s.Clear();
			s	<< "Exception in function: "
				<< ctx->GetExceptionFunction()->GetName();

			const char *section = 0;
			int column = 0;
			int line = ctx->GetExceptionLineNumber( &column, &section );

			s << "(" << line << ", " << column << "):\n";
			s << section << "\n";
			s << ctx->GetExceptionString();

			RETURN_ERR( s );
		}
		else
		{
			RETURN_ERR( "AngelScript execution failed" );
		}

		AS_CALL( ctx->Release() );
		return true;
	}
	
/*
=================================================
	RunFromFile
=================================================
*/
	bool ScriptEngine::RunFromFile (StringCRef filename, StringCRef entry)
	{
		File::HddRFile	file;
		CHECK_ERR( file.Open( filename ) );

		String			data;
		const BytesU	len = file.Size();

		data.Reserve( len+1 );
		file.Read( data.ptr(), len );
		data.SetLength( len );

		file.Close();

		return Run( data, entry );
	}

/*
=================================================
	_MessageCallback
=================================================
*/
	void ScriptEngine::_MessageCallback (const AngelScript::asSMessageInfo *msg, void *)
	{
		using namespace AngelScript;

		String		str("AngelScript message: ");
		ELog::type	type = ELog::Debug;

		str << msg->section << " (" << msg->row << ", " << msg->col << ") ";

		switch ( msg->type )
		{
			case asMSGTYPE_WARNING :		str << "WARN  ";	type = ELog::Warning;	break;
			case asMSGTYPE_INFORMATION :	str << "INFO  ";	type = ELog::Info;	break;
			case asMSGTYPE_ERROR :			str << "ERROR ";	type = ELog::Error;	break;
		}
			
		str << msg->message;

		LOG( str.cstr(), type );
	}
	
/*
=================================================
	_CheckError
=================================================
*/
	bool ScriptEngine::_CheckError (int err, StringCRef asFunc, StringCRef func, StringCRef file, int line)
	{
		using namespace AngelScript;

		if ( err >= 0 )
			return true;

		String	str("AngelScript error: ");

#		define __AS_CASE_ERR( _val_ ) \
			case _val_ : str << TOSTRING( _val_ ); break;

		switch ( err )
		{
			__AS_CASE_ERR( asERROR );
			__AS_CASE_ERR( asCONTEXT_ACTIVE );
			__AS_CASE_ERR( asCONTEXT_NOT_FINISHED );
			__AS_CASE_ERR( asCONTEXT_NOT_PREPARED );
			__AS_CASE_ERR( asINVALID_ARG );
			__AS_CASE_ERR( asNO_FUNCTION  );
			__AS_CASE_ERR( asNOT_SUPPORTED );
			__AS_CASE_ERR( asINVALID_NAME );
			__AS_CASE_ERR( asNAME_TAKEN );
			__AS_CASE_ERR( asINVALID_DECLARATION );
			__AS_CASE_ERR( asINVALID_OBJECT );
			__AS_CASE_ERR( asINVALID_TYPE );
			__AS_CASE_ERR( asALREADY_REGISTERED );
			__AS_CASE_ERR( asMULTIPLE_FUNCTIONS );
			__AS_CASE_ERR( asNO_MODULE );
			__AS_CASE_ERR( asNO_GLOBAL_VAR );
			__AS_CASE_ERR( asINVALID_CONFIGURATION );
			__AS_CASE_ERR( asINVALID_INTERFACE );
			__AS_CASE_ERR( asCANT_BIND_ALL_FUNCTIONS );
			__AS_CASE_ERR( asLOWER_ARRAY_DIMENSION_NOT_REGISTERED );
			__AS_CASE_ERR( asWRONG_CONFIG_GROUP );
			__AS_CASE_ERR( asCONFIG_GROUP_IS_IN_USE );
			__AS_CASE_ERR( asILLEGAL_BEHAVIOUR_FOR_TYPE );
			__AS_CASE_ERR( asWRONG_CALLING_CONV );
			__AS_CASE_ERR( asBUILD_IN_PROGRESS );
			__AS_CASE_ERR( asINIT_GLOBAL_VARS_FAILED );
			__AS_CASE_ERR( asOUT_OF_MEMORY );

			default :
				str << "code: 0x" << String().FormatI( err, 10 );
				break;
		}

#			undef __AS_CASE_ERR
			
		str << ", in " << asFunc;
		str << ", function: " << func;

		LOG( str.cstr(), ELog::Error, file.cstr(), line );
		return false;
	}

}	// GXScript
}	// GX_STL

#endif	// GX_ANGELSCRIPT_ENABLED
