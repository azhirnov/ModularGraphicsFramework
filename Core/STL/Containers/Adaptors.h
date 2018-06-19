// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Containers/AppendableAdaptor.h"
#include "Core/STL/Containers/ErasableAdaptor.h"
// TODO: indexable, searchable, ...

#include "Core/STL/Containers/Array.h"
#include "Core/STL/Containers/Queue.h"
#include "Core/STL/Containers/CircularQueue.h"
#include "Core/STL/Containers/StaticArray.h"
#include "Core/STL/Containers/Stack.h"
#include "Core/STL/Containers/IndexedArray.h"
#include "Core/STL/Containers/Map.h"
#include "Core/STL/Containers/Set.h"
#include "Core/STL/Containers/HashMap.h"
#include "Core/STL/Containers/HashSet.h"
#include "Core/STL/Containers/UniBuffer.h"

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
