// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Script/Impl/ScriptEngine.h"
#include "Engine/Script/Impl/ScriptTypes.h"

namespace GX_STL
{
namespace GXScript
{

/*
=================================================
	AddFunction
=================================================
*/
	template <typename T>
	inline void ScriptEngine::AddFunction (T func, StringCRef name)
	{
		using namespace AngelScript;

		String	signature;
		GlobalFunction<T>::GetDescriptor( OUT signature, name );

		AS_CALL( _engine->RegisterGlobalFunction( signature.cstr(), asFUNCTION( *func ), asCALL_CDECL ) );
	}
	
/*
=================================================
	AddProperty
=================================================
*/
	template <typename T>
	inline void ScriptEngine::AddProperty (INOUT T &var, StringCRef name)
	{
		String	signature;
		ScriptTypeInfo<T>::Name( OUT signature );
		signature << ' ' << name;

		AS_CALL( _engine->RegisterGlobalProperty( signature.cstr(), Cast<void *>(&var) ) );
	}
	
/*
=================================================
	AddConstProperty
=================================================
*/
	template <typename T>
	inline void ScriptEngine::AddConstProperty (const T &var, StringCRef name)
	{
		String	signature( "const " );
		ScriptTypeInfo<T>::Name( OUT signature );
		signature << ' ' << name;

				AS_CALL( _engine->RegisterGlobalProperty( signature.cstr(), Cast<void *>(const_cast<T*>(&var)) ) );
	}

}	// GXScript
}	// GX_STL
