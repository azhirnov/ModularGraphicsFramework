// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "AppendableAdaptor.h"
#include "ErasableAdaptor.h"
// TODO: indexable, searchable, ...

#include "Array.h"
#include "Queue.h"
#include "CircularQueue.h"
#include "StaticArray.h"
#include "Stack.h"
#include "IndexedArray.h"
#include "Map.h"
#include "Set.h"
#include "HashMap.h"
#include "HashSet.h"
#include "UniBuffer.h"

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
