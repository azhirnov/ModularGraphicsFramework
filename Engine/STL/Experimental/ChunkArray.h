// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Containers/Array.h"

namespace GX_STL
{
namespace GXTypes
{
	
	//
	// Chunk Array
	//
	
	template <	typename T,
				typename S = typename AutoDetectCopyStrategy<T>::type,
				typename MC = MemoryContainer<T>
			 >
	struct ChunkArray : public CompileTime::CopyQualifiers< CompileTime::FastCopyable, MC >
	{
	// types
	public:
		typedef ChunkArray<T,S,MC>		Self;
		typedef S						Strategy;
		typedef MC						MemoryContainer_t;
		typedef T						value_t;

	private:
		struct _Chunk
		{
			MemoryContainer_t	_memory;

		};

	};


}	// GX_STL
}	// GXTypes
