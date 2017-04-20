// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.
//
// see http://www.angelcode.com/angelscript/
//
// online reference http://www.angelcode.com/angelscript/sdk/docs/manual/index.html
//

#pragma once

#include "ScriptTypes.h"

#ifdef GX_ANGELSCRIPT_ENABLED

namespace GX_STL
{
namespace GXScript
{
	
	//
	// Script Engine
	//

	class ScriptEngine
	{
	// variables
	private:
		Ptr< AngelScript::asIScriptEngine >		_engine;


	// methods
	private:
		ScriptEngine (const ScriptEngine &) {}
		void operator = (const ScriptEngine &) {}

	public:
		ScriptEngine ();
		explicit ScriptEngine (AngelScript::asIScriptEngine *se);
		~ScriptEngine ();

		AngelScript::asIScriptEngine *			Get ()					{ return _engine; }
		AngelScript::asIScriptEngine const *	Get ()	const			{ return _engine; }

		AngelScript::asIScriptEngine *			operator -> ()			{ return _engine; }
		AngelScript::asIScriptEngine const *	operator -> () const	{ return _engine; }

		template <typename T, typename R>
		bool Run (StringCRef script, StringCRef entry, T arg, OUT R &result);
		bool Run (StringCRef script, StringCRef entry);
		
		template <typename T, typename R>
		bool RunFromFile (StringCRef filename, StringCRef entry, T arg, OUT R &result);
		bool RunFromFile (StringCRef filename, StringCRef entry);

		template <typename T>
		void AddFunction (T func, StringCRef name);

		//template <typename T>
		//void AddFunctionTemplate (T func, StringCRef name);

		template <typename T>
		void AddProperty (INOUT T &var, StringCRef name);
		
		template <typename T>
		void AddConstProperty (const T &var, StringCRef name);

		void AddEnumValue (StringCRef name, StringCRef valueName, int value);

		void SetNamespace (StringCRef name);
		void SetDefaultNamespace ();


		static bool _CheckError (int err, StringCRef asFunc, StringCRef func, StringCRef file, int line);

	private:
		static void _MessageCallback (const AngelScript::asSMessageInfo *msg, void *param);
	};

	
/*
=================================================
	AddFunction
=================================================
*/
	template <typename T>
	inline void ScriptEngine::AddFunction (T func, StringCRef name)
	{
		using namespace AngelScript;

		String	s;
		GlobalFunction<T>::GetDescriptor( s, name );

		AS_CALL( _engine->RegisterGlobalFunction( s.cstr(), asFUNCTION( *func ), asCALL_CDECL ) );
	}
	
/*
=================================================
	AddProperty
=================================================
*/
	template <typename T>
	inline void ScriptEngine::AddProperty (INOUT T &var, StringCRef name)
	{
		String	s;
		ScriptTypeInfo<T>::Name( s );
		s << ' ' << name;

		AS_CALL( _engine->RegisterGlobalProperty( s.cstr(), (void *) &var ) );
	}
	
/*
=================================================
	AddConstProperty
=================================================
*/
	template <typename T>
	inline void ScriptEngine::AddConstProperty (const T &var, StringCRef name)
	{
		String	s("const ");
		ScriptTypeInfo<T>::Name( s );
		s << ' ' << name;

		AS_CALL( _engine->RegisterGlobalProperty( s.cstr(), (void *) &var ) );
	}
	
/*
=================================================
	AddEnumValue
=================================================
*/
	inline void ScriptEngine::AddEnumValue (StringCRef name, StringCRef valueName, int value)
	{
		using namespace AngelScript;

		int	res = _engine->RegisterEnum( name.cstr() );

		if ( res < 0 and res != asALREADY_REGISTERED ) {
			AS_CALL( res );
			return;
		}

		AS_CALL( _engine->RegisterEnumValue( name.cstr(), valueName.cstr(), value ) );
	}
	
/*
=================================================
	SetNamespace
=================================================
*/
	inline void ScriptEngine::SetNamespace (StringCRef name)
	{
		AS_CALL( _engine->SetDefaultNamespace( name.cstr() ) );
	}
	
/*
=================================================
	SetDefaultNamespace
=================================================
*/
	inline void ScriptEngine::SetDefaultNamespace ()
	{
		SetNamespace( "" );
	}

/*
=================================================
	Run
=================================================
*/
	template <typename T, typename R>
	inline bool ScriptEngine::Run (StringCRef script, StringCRef entry, T arg, OUT R &result)
	{
		using namespace AngelScript;

		String	s;
		GlobalFunction< R (*) (T) >::GetDescriptor( s, entry.cstr() );
		
		asIScriptModule *	mod = _engine->GetModule( "def", asGM_CREATE_IF_NOT_EXISTS );
		mod->AddScriptSection( "def_script", script.cstr() );
		
		AS_CALL_R( mod->Build() );
		
		asIScriptContext *	ctx = _engine->CreateContext();
		
		asIScriptFunction * func = mod->GetFunctionByDecl( s.cstr() );
		CHECK_ERR( func != null );

		AS_CALL_R( ctx->Prepare( func ) );

		AS_CALL( ContextSetterGetter<T>::Set( ctx, 0, arg ) );

		const int exec_res = ctx->Execute();

		if ( exec_res == asEXECUTION_FINISHED )
		{
			result = ContextSetterGetter<R>::Get( ctx );
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
	template <typename T, typename R>
	inline bool ScriptEngine::RunFromFile (StringCRef filename, StringCRef entry, T arg, OUT R &result)
	{
		File::HddRFile	file;
		CHECK_ERR( file.Open( filename ) );

		String		data;
		const usize	len = file.Size();

		data.Reserve( len+1 );
		file.Read( data.ptr(), len );
		data.SetLength( len );

		file.Close();

		return Run( data, entry, arg, result );
	}

}	// GXScript
}	// GX_STL

#endif	// GX_ANGELSCRIPT_ENABLED
