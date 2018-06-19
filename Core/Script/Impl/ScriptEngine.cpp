// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Script/Impl/ScriptEngine.h"
#include "Core/Script/Impl/ScriptModule.h"
#include "Core/STL/Math/BinaryMath.h"
#include "Core/STL/Math/Interpolations.h"
#include "Core/STL/Log/ToString.h"

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

		_defModule = New<ScriptModule>( this );
		_defModule->Create( "def" );
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
		
		_defModule = New<ScriptModule>( this );
		_defModule->Create( "def" );
	}
	
/*
=================================================
	destructor
=================================================
*/
	ScriptEngine::~ScriptEngine ()
	{
		_defModule = null;

		_objects.Clear();

		_engine->ShutDownAndRelease();
	}
	
/*
=================================================
	SetNamespace
=================================================
*/
	void ScriptEngine::SetNamespace (StringCRef name)
	{
		AS_CALL( _engine->SetDefaultNamespace( name.cstr() ) );
	}
	
/*
=================================================
	SetDefaultNamespace
=================================================
*/
	void ScriptEngine::SetDefaultNamespace ()
	{
		SetNamespace( "" );
	}
	
/*
=================================================
	AddSharedObject
=================================================
*/
	void ScriptEngine::AddSharedObject (const ScriptSharedObjPtr &obj)
	{
		_objects.Add( obj );
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
			case asMSGTYPE_INFORMATION :	str << "INFO  ";	type = ELog::Info;		break;
			case asMSGTYPE_ERROR :			str << "ERROR ";	type = ELog::Error;		break;
		}
			
		str << msg->message;

		StringCRef	msg_str		= msg->message;
		StringCRef	code_mark	= "(Code: ";
		usize		pos			= 0;

		if ( msg_str.FindIC( code_mark, OUT pos ) )
		{
			const usize		begin	= pos + code_mark.Length();
			StringCRef		code	= msg_str.SubString( begin );
			
			//if ( code.Find( ')', OUT pos ) )
			//	code = code.SubString( 0, pos );

			const int	int_code = StringUtils::ToInt32( code );

			str << ", code name: ";

			switch ( int_code )
			{
				case asERetCodes::asERROR :									str << "asERROR";						break;
				case asERetCodes::asCONTEXT_ACTIVE :						str << "asCONTEXT_ACTIVE";				break;
				case asERetCodes::asCONTEXT_NOT_FINISHED :					str << "asCONTEXT_NOT_FINISHED";		break;
				case asERetCodes::asCONTEXT_NOT_PREPARED :					str << "asCONTEXT_NOT_PREPARED";		break;
				case asERetCodes::asINVALID_ARG :							str << "asINVALID_ARG";					break;
				case asERetCodes::asNO_FUNCTION :							str << "asNO_FUNCTION";					break;
				case asERetCodes::asNOT_SUPPORTED :							str << "asNOT_SUPPORTED";				break;
				case asERetCodes::asINVALID_NAME :							str << "asINVALID_NAME";				break;
				case asERetCodes::asNAME_TAKEN :							str << "asNAME_TAKEN";					break;
				case asERetCodes::asINVALID_DECLARATION :					str << "asINVALID_DECLARATION";			break;
				case asERetCodes::asINVALID_OBJECT :						str << "asINVALID_OBJECT";				break;
				case asERetCodes::asINVALID_TYPE :							str << "asINVALID_TYPE";				break;
				case asERetCodes::asALREADY_REGISTERED :					str << "asALREADY_REGISTERED";			break;
				case asERetCodes::asMULTIPLE_FUNCTIONS :					str << "asMULTIPLE_FUNCTIONS";			break;
				case asERetCodes::asNO_MODULE :								str << "asNO_MODULE";					break;
				case asERetCodes::asNO_GLOBAL_VAR :							str << "asNO_GLOBAL_VAR";				break;
				case asERetCodes::asINVALID_CONFIGURATION :					str << "asINVALID_CONFIGURATION";		break;
				case asERetCodes::asINVALID_INTERFACE :						str << "asINVALID_INTERFACE";			break;
				case asERetCodes::asCANT_BIND_ALL_FUNCTIONS :				str << "asCANT_BIND_ALL_FUNCTIONS";		break;
				case asERetCodes::asLOWER_ARRAY_DIMENSION_NOT_REGISTERED :	str << "asLOWER_ARRAY_DIMENSION_NOT_REGISTERED";	break;
				case asERetCodes::asWRONG_CONFIG_GROUP :					str << "asWRONG_CONFIG_GROUP";			break;
				case asERetCodes::asCONFIG_GROUP_IS_IN_USE :				str << "asCONFIG_GROUP_IS_IN_USE";		break;
				case asERetCodes::asILLEGAL_BEHAVIOUR_FOR_TYPE :			str << "asILLEGAL_BEHAVIOUR_FOR_TYPE";	break;
				case asERetCodes::asWRONG_CALLING_CONV :					str << "asWRONG_CALLING_CONV";			break;
				case asERetCodes::asBUILD_IN_PROGRESS :						str << "asBUILD_IN_PROGRESS";			break;
				case asERetCodes::asINIT_GLOBAL_VARS_FAILED :				str << "asINIT_GLOBAL_VARS_FAILED";		break;
				case asERetCodes::asOUT_OF_MEMORY :							str << "asOUT_OF_MEMORY";				break;
				case asERetCodes::asMODULE_IS_IN_USE :						str << "asMODULE_IS_IN_USE";			break;
				default :													str << "unknown";
			}
		}

		LOG( str, type );
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

		LOG( str, ELog::Error, file.cstr(), line );
		return false;
	}

}	// GXScript
