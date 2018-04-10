// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Script/Impl/ScriptEngine.h"
#include "Engine/Script/Impl/ScriptTypes.h"

namespace GX_STL
{
namespace GXScript
{

	//
	// Script Module
	//

	class ScriptModule final : public StaticRefCountedObject
	{
	// variables
	private:
		ScriptEnginePtr							_engine;
		Ptr< AngelScript::asIScriptModule >		_module;


	// methods
	public:
		explicit ScriptModule (const ScriptEnginePtr &eng);
		~ScriptModule ();

		bool Create (StringCRef name);
		void Destroy ();
		
		template <typename Ret, typename ...Args>
		bool Run (StringCRef script, StringCRef entry, OUT Ret &result, Args ...args);
		bool Run (StringCRef script, StringCRef entry);
		
		template <typename Ret, typename ...Args>
		bool RunFromFile (StringCRef filename, StringCRef entry, OUT Ret &result, Args ...args);
		bool RunFromFile (StringCRef filename, StringCRef entry);

	private:
		template <typename T> using CtxResult	= _script_hidden_::ContextResult<T>;

		template <typename Ret, typename ...Args>
		bool _Run (StringCRef script, StringCRef entry, CtxResult<Ret> result, Args ...args);
	};
	

/*
=================================================
	Run
=================================================
*/
	template <typename Ret, typename ...Args>
	inline bool ScriptModule::Run (StringCRef script, StringCRef entry, OUT Ret &result, Args ...args)
	{
		return _Run< Ret, Args... >( script, entry, CtxResult<Ret>{result}, args... );
	}
	
/*
=================================================
	_Run
=================================================
*/
	template <typename Ret, typename ...Args>
	inline bool ScriptModule::_Run (StringCRef script, StringCRef entry, CtxResult<Ret> result, Args ...args)
	{
		using namespace AngelScript;
		
		CHECK_ERR( _module );
		CHECK_ERR( not script.Empty() );
		CHECK_ERR( not entry.Empty() );

		// prepare script
		String	signature;
		GlobalFunction< Ret (*) (Args...) >::GetDescriptor( OUT signature, entry.cstr() );
		
		_module->AddScriptSection( "def_script", script.cstr() );
		
		AS_CALL_R( _module->Build() );
		
		asIScriptContext *	ctx		= _engine->Get()->CreateContext();
		asIScriptFunction * func	= _module->GetFunctionByDecl( signature.cstr() );
		CHECK_ERR( func != null );

		AS_CALL_R( ctx->Prepare( func ) );
		

		// execute
		_script_hidden_::SetContextArgs< Args... >::Set( ctx, 0, args... );

		const int exec_res = ctx->Execute();


		// check result
		if ( exec_res == asEXECUTION_FINISHED )
		{
			if_constexpr( not CompileTime::IsVoid< Ret > )
			{
				result.value = _script_hidden_::ContextSetterGetter<Ret>::Get( ctx );
			}
			else
				GX_UNUSED( result );
		}
		else
		if ( exec_res == asEXECUTION_EXCEPTION )
		{
			String&	err = signature;

			err.Clear();
			err	<< "Exception in function: "
				<< ctx->GetExceptionFunction()->GetName();

			const char *section = 0;
			int column = 0;
			int line = ctx->GetExceptionLineNumber( OUT &column, OUT &section );

			err << "(" << line << ", " << column << "):\n";
			err << section << "\n";
			err << ctx->GetExceptionString();

			RETURN_ERR( err );
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
	template <typename Ret, typename ...Args>
	inline bool ScriptModule::RunFromFile (StringCRef filename, StringCRef entry, OUT Ret &result, Args ...args)
	{
		File::HddRFile	file;
		CHECK_ERR( file.Open( filename ) );

		const usize	len		= usize(file.Size());
		String		data;	data.Reserve( len+1 );

		file.Read( data.ptr(), data.LengthInBytes() );
		file.Close();

		return Run( data, entry, OUT result, args... );
	}
//-----------------------------------------------------------------------------
	
	
/*
=================================================
	Run
=================================================
*/
	template <typename Ret, typename ...Args>
	inline bool ScriptEngine::Run (StringCRef script, StringCRef entry, OUT Ret &result, Args ...args)
	{
		return _defModule->Run( script, entry, OUT result, args... );
	}
	
	inline bool ScriptEngine::Run (StringCRef script, StringCRef entry)
	{
		return _defModule->Run( script, entry );
	}
	
/*
=================================================
	RunFromFile
=================================================
*/
	template <typename Ret, typename ...Args>
	inline bool ScriptEngine::RunFromFile (StringCRef filename, StringCRef entry, OUT Ret &result, Args ...args)
	{
		return _defModule->RunFromFile( filename, entry, OUT result, args... );
	}
	
	inline bool ScriptEngine::RunFromFile (StringCRef filename, StringCRef entry)
	{
		return _defModule->RunFromFile( filename, entry );
	}

}	// GXScript
}	// GX_STL
