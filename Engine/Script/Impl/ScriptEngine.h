// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
//
// see http://www.angelcode.com/angelscript/
//
// online reference http://www.angelcode.com/angelscript/sdk/docs/manual/index.html
//

#pragma once

#include "Engine/STL/Containers/String.h"
#include "Engine/STL/CompileTime/TypeList.h"
#include "Engine/STL/Types/Ptr.h"
#include "Engine/STL/Files/HDDFile.h"
#include "Engine/STL/Log/ELog.h"
#include "Engine/STL/Types/StaticRefCountedObject.h"

// AngelScript + Addons //
#define AS_USE_NAMESPACE
#include "AngelScript/sdk/angelscript/include/angelscript.h"

namespace GX_STL
{
namespace GXScript
{
	using namespace GX_STL::GXTypes;

	class ScriptModule;
	SHARED_POINTER( ScriptModule );
	
	class ScriptEngine;
	SHARED_POINTER( ScriptEngine );



	//
	// Script Engine
	//

	class ScriptEngine final : public StaticRefCountedObject
	{
	// types
	public:
		class ScriptSharedObj : public StaticRefCountedObject
		{};

		SHARED_POINTER( ScriptSharedObj );


	// variables
	private:
		Ptr< AngelScript::asIScriptEngine >		_engine;

		ScriptModulePtr							_defModule;

		Set< ScriptSharedObjPtr >				_objects;


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

		template <typename T>
		void AddFunction (T func, StringCRef name);

		//template <typename T>
		//void AddFunctionTemplate (T func, StringCRef name);

		template <typename T>
		void AddProperty (INOUT T &var, StringCRef name);
		
		template <typename T>
		void AddConstProperty (const T &var, StringCRef name);

		void SetNamespace (StringCRef name);
		void SetDefaultNamespace ();

		void AddSharedObject (const ScriptSharedObjPtr &obj);
		
		template <typename Ret, typename ...Args>
		bool Run (StringCRef script, StringCRef entry, OUT Ret &result, Args ...args);
		bool Run (StringCRef script, StringCRef entry);
		
		template <typename Ret, typename ...Args>
		bool RunFromFile (StringCRef filename, StringCRef entry, OUT Ret &result, Args ...args);
		bool RunFromFile (StringCRef filename, StringCRef entry);

		static bool _CheckError (int err, StringCRef asFunc, StringCRef func, StringCRef file, int line);


	private:
		static void _MessageCallback (const AngelScript::asSMessageInfo *msg, void *param);
	};
	

#	define AS_CALL( ... ) \
	{ \
		int __as_result = ( __VA_ARGS__ ); \
		::GX_STL::GXScript::ScriptEngine::_CheckError( __as_result, TOSTRING( __VA_ARGS__ ), GX_FUNCTION_NAME, __FILE__, __LINE__ ); \
	}
	
#	define AS_CALL_R( ... ) \
	{ \
		int __as_result = ( __VA_ARGS__ ); \
		if ( not ::GX_STL::GXScript::ScriptEngine::_CheckError( __as_result, TOSTRING( __VA_ARGS__ ), GX_FUNCTION_NAME, __FILE__, __LINE__ ) ) \
			return false; \
	}


}	// GXScript
}	// GX_STL
