// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Types/UniquePtr.h"
#include "Core/STL/Containers/HashMap.h"
#include "Core/STL/Containers/Stack.h"

#include "Core/Script/Core.Script.h"

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
