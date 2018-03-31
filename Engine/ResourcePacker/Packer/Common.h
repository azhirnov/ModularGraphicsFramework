// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Engine.STL.h"
#include "Engine/Script/Engine.Script.h"

namespace ResPack
{

	using namespace GX_STL;
	using namespace GX_STL::GXTypes;


	//
	// Script Helper
	//
	struct ScriptHelper final : Noninstancable
	{
		static bool RunScript (StringCRef filename);
		
		static StringCRef CurrentFileName ();

		static GXScript::ScriptEnginePtr GetScriptEngine ();
	};

}	// ResPack
