// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Containers/AppendableAdaptor.h"
#include "Engine/STL/Containers/ErasableAdaptor.h"
// TODO: indexable, searchable, ...

#include "Engine/STL/Containers/Array.h"
#include "Engine/STL/Containers/Queue.h"
#include "Engine/STL/Containers/CircularQueue.h"
#include "Engine/STL/Containers/StaticArray.h"
#include "Engine/STL/Containers/Stack.h"
#include "Engine/STL/Containers/IndexedArray.h"
#include "Engine/STL/Containers/Map.h"
#include "Engine/STL/Containers/Set.h"
#include "Engine/STL/Containers/HashMap.h"
#include "Engine/STL/Containers/HashSet.h"
#include "Engine/STL/Containers/UniBuffer.h"

namespace GX_STL
{
namespace GXTypes
{

	namespace ContainerAdaptors
	{
	}	// ContainerAdaptors


	template <typename T>
	using AppendableAdaptor		= ContainerAdaptors::Appendable<T>;

}	// GXTypes
}	// GX_STL
