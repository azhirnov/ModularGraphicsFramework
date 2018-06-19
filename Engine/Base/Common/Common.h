// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/CompileTime/Runtime/TypeIdList.h"
#include "Core/STL/CompileTime/StringToID.h"

#include "Core/STL/Types/SharedPointer.h"
#include "Core/STL/Types/WeakPointer.h"
#include "Core/STL/Types/StaticRefCountedObject.h"
#include "Core/STL/Types/UniquePtr.h"

#include "Core/STL/Types/Union.h"
#include "Core/STL/Types/Optional.h"
#include "Core/STL/Types/Delegate.h"
#include "Core/STL/Types/VariantRef.h"
#include "Core/STL/Types/VariantCRef.h"
#include "Core/STL/Types/VariantInterface.h"
#include "Core/STL/Types/ScopeSetter.h"
#include "Core/STL/Types/UntypedKey.h"

#include "Core/STL/Types/Bitfield.h"
#include "Core/STL/Types/StaticLimit.h"

#include "Core/STL/Containers/String.h"
#include "Core/STL/Containers/Array.h"
#include "Core/STL/Containers/Queue.h"
#include "Core/STL/Containers/Map.h"
#include "Core/STL/Containers/Set.h"
#include "Core/STL/Containers/HashMap.h"
#include "Core/STL/Containers/HashSet.h"

#include "Core/STL/Files/BaseFile.h"
#include "Core/STL/Time/TimeProfiler.h"

#include "Core/STL/Log/Logger.h"
#include "Core/STL/Log/ToString.h"

#include "Core/STL/Math/Interpolations.h"

#include "Core/Config/Engine.Config.h"
#include "Core/Config/Engine.Version.h"
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
	
	namespace Graphics	// deprecated
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
