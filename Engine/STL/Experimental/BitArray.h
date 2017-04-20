// Copyright © 2014-2016  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Array.h"
#include "Engine/STL/BaseTypes/Bitfield.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Bit Array
	//
	/*
	template <	typename T = Bitfield<8>,
				typename S = typename AutoDetectCopyStrategy<T>::type,
				typename MC = MemoryContainer<T>
			 >
	struct TBitArray : public CompileTime::CopyQualifiers< CompileTime::FastCopyable, MC >
	{
	// types
	public:
		typedef TBitArray<T,S,MC>		Self;
		typedef MC						MemoryContainer_t;
		typedef T						value_t;
		typedef Array<T, S, MC >		array_t;


	// variables
	private:
		array_t		_memory;


	// methods
	public:
		TBitArray () {}

		TBitArray (const Self &other) : _memory(other._memory) {}
		
		TBitArray (Self &&other) : _memory(RVREF(other._memory)) {}

		~TBitArray () {}
	};



	template <usize Size>
	struct FixedSizeBitArray
	{
		typedef TBitArray< Bitfield<8>, typename AutoDetectCopyStrategy<T>::type, StaticMemoryContainer<T, Size> >	type;
	};
	*/


}	// GX_STL
}	// GXTypes
