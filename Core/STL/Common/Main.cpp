// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/STL/Common/Init.h"
#include "Core/STL/Types/RefCountedObject.h"

#include "Core/STL/Defines/PublicMacro.h"
#include "Core/STL/CompileTime/CompileTimeChecks.h"
#include "Core/STL/Math/Rand/Random.h"

namespace GX_STL
{
namespace GXMath
{

/*
=================================================
	InitializeAllRandomGenerators_Op
=================================================
*/
	struct InitializeAllRandomGenerators_Op
	{
		usize result;

		InitializeAllRandomGenerators_Op () : result(0)
		{}

		template <typename T, usize Index>
		void Process ()
		{
			result += uint(GXMath::TRandomGenerator<T>::Initialize());
		}
	};

/*
=================================================
	InitializeAllRandomGenerators
=================================================
*/
	static void InitializeAllRandomGenerators ()
	{
		using engine_list	= CompileTime::TypeListFrom< GXMath::DefaultRandomEngine, GXMath::PlatformRandomEngine >;

		InitializeAllRandomGenerators_Op	op;
		engine_list::RuntimeForEach( op );

		return void(op.result);
	}
	
/*
=================================================
	InitializeSTLMath
=================================================
*/
	void InitializeSTLMath ()
	{
		InitializeAllRandomGenerators();
	}

}	// GXMath
	
	
namespace GXTypes
{

#if not defined( GX_THREADSAFE_STATIC_SUPPORTED )
	Mutex		SingletonMultiThread::_mutex;
#endif

}	// GXTypes
}	// GX_STL


using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;
using namespace GX_STL::CompileTime;
using namespace GX_STL::TypeTraits;
using namespace GX_STL::Java;
using namespace GX_STL::OS;
using namespace GX_STL::GXFilters;
using namespace GX_STL::GXFile;


// check hidden namespaces intersection
using namespace _types_hidden_;
using namespace _ctime_hidden_;
using namespace _math_hidden_;
using namespace _color_hidden_;
using namespace _ttraits_hidden_;
using namespace _java_hidden_;
using namespace _os_hidden_;
