// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

//#include "Engine/STL/Engine.STL.h"

#include "Engine/STL/CompileTime/Runtime/TypeIdList.h"
#include "Engine/STL/CompileTime/StringToID.h"

#include "Engine/STL/Types/SharedPointer.h"
#include "Engine/STL/Types/WeakPointer.h"
#include "Engine/STL/Types/StaticRefCountedObject.h"
#include "Engine/STL/Types/UniquePtr.h"

#include "Engine/STL/Types/Union.h"
#include "Engine/STL/Types/Optional.h"
#include "Engine/STL/Types/Delegate.h"
#include "Engine/STL/Types/VariantRef.h"
#include "Engine/STL/Types/VariantCRef.h"
#include "Engine/STL/Types/VariantInterface.h"
#include "Engine/STL/Types/ScopeSetter.h"

#include "Engine/STL/Types/Bitfield.h"
#include "Engine/STL/Types/StaticLimit.h"

#include "Engine/STL/Containers/String.h"
#include "Engine/STL/Containers/Array.h"
#include "Engine/STL/Containers/Queue.h"
#include "Engine/STL/Containers/Map.h"
#include "Engine/STL/Containers/Set.h"
#include "Engine/STL/Containers/HashMap.h"
#include "Engine/STL/Containers/HashSet.h"

#include "Engine/STL/Files/BaseFile.h"
#include "Engine/STL/Time/TimeProfiler.h"

#include "Engine/STL/Log/Logger.h"
#include "Engine/STL/Log/ToString.h"

#include "Engine/STL/Math/Interpolations.h"

#include "Engine/Config/Engine.Config.h"
#include "Engine/Config/Engine.Version.h"
#include "Engine/Base/Common/Defines.h"

namespace Engine
{
	using namespace GX_STL;
	using namespace GX_STL::GXTypes;

	namespace Base
	{
		class BaseObject;
		class Module;
		class ModulesFactory;

		// TODO: Serializer

		WEAK_POINTER( Module );
		SHARED_POINTER( Module );
		SHARED_POINTER( ModulesFactory );

		Ptr<Module>  GetMainSystemInstance ();

		template <typename T> struct Message;
		
		using TypeIdList = Runtime::TypeIdList;

		namespace _engbase_hidden_ {}
	}
	
	namespace ModuleMsg
	{
		// never use 'using namespace ModuleMsg' !!!
	}

	namespace ProfilingMsg
	{
		// never use 'using namespace ProfilingMsg' !!!
	}

	namespace CreateInfo
	{
		// never use 'using namespace CreateInfo' !!!
	}
	
	namespace Platforms
	{
	}
	
	namespace Profilers
	{
	}
	
	namespace Validators
	{
	}
	
	namespace Graphics
	{
	}
	
	namespace Scene
	{
	}

	namespace Audio
	{
	}
	
	namespace UI
	{
	}

}	// Engine
