// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Platforms.h"
#include "Core/Config/STL.Config.h"

#if defined(COMPILER_MSVC)
#	pragma warning (push, 0)
#endif

#define ulong linux_uint

#include <stdio.h>
#include <cmath>
#include <string.h>
#include <stdlib.h>
#include <new>
#include <chrono>
#include <initializer_list>
#include <algorithm>
#include <functional>
#include <typeindex>
#include <vector>
#include <string>
#include <cstddef>
#include <type_traits>
#include <atomic>

#undef ulong

#if defined(COMPILER_MSVC)
#   include <xstddef>
#	pragma warning (pop)
#endif

#include "Core/STL/Defines/AuxiliaryDefines.h"
#include "Core/STL/Defines/Defines.h"
#include "Core/STL/Defines/CtorHelpers.h"
#include "Core/STL/Defines/EnumHelpers.h"
#include "Core/STL/Defines/Errors.h"
#include "Core/STL/Defines/OperatorHelpers.h"
#include "Core/STL/Defines/MemberDetector.h"
#include "Core/STL/Log/ELog.h"


namespace GX_STL
{
	namespace _defines_hidden_ {}

	namespace GXTypes
	{
		namespace _types_hidden_ {}
	}
	
	namespace TypeTraits
	{
		using namespace GX_STL::GXTypes;

		namespace _ttraits_hidden_ {}
	}

	namespace CompileTime
	{
		//using namespace GX_STL::GXTypes;

		namespace _ctime_hidden_ {}
	}

	namespace GXMath
	{
		using namespace GX_STL::GXTypes;

		namespace _math_hidden_ {}
		namespace _color_hidden_ {}

		void InitializeSTLMath ();
	}
	
	namespace Java
	{
		using namespace GX_STL::GXTypes;

		namespace _java_hidden_ {}
	}

	namespace OS
	{
		using namespace GX_STL::GXTypes;

		namespace _os_hidden_ {}
	}

	namespace GXFilters
	{
		using namespace GX_STL::GXTypes;
		using namespace GX_STL::GXMath;
	}

	namespace GXFile
	{
		using namespace GX_STL::GXTypes;
	}
	
# ifdef GX_SQLITE_DATABASE_ENABLED
	namespace GXDataBase
	{
		using namespace GX_STL::GXTypes;
	}
# endif

}	// GX_STL
