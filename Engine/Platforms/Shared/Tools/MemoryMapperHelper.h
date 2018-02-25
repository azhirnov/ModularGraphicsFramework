// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/Memory.h"

namespace Engine
{
namespace PlatformTools
{
	using namespace Platforms;


	//
	// Memory Mapper
	//

	class MemoryMapperHelper final : public Noncopyable
	{
	// types
	private:
		using EBindingTarget	= GpuMsg::OnMemoryBindingChanged::EBindingTarget;
		using EMappingFlags		= GpuMsg::MapMemoryToCpu::EMappingFlags;


	// variables
	private:
		ubyte *					_ptr;
		BytesUL					_offset;
		BytesUL					_size;

		EMemoryAccess::bits		_access;
		EMemoryAccess::bits		_mappingAccess;

		bool					_isCachedInCPU		: 1;
		bool					_isCoherentWithCPU	: 1;
		bool					_changed			: 1;


	// methods
	public:
		MemoryMapperHelper (EGpuMemory::bits flags, EMemoryAccess::bits access);
		~MemoryMapperHelper ();
		
		void Clear ();

		bool				IsMapped ()					const	{ return _ptr != null; }
		void *				Pointer ()							{ return _ptr; }
		bool				IsMappedMemoryChanged ()	const	{ return _changed; }
		BytesUL				MappedOffset ()				const	{ return _offset; }
		BytesUL				MappedSize ()				const	{ return _size; }
		EMemoryAccess::bits	MemoryAccess ()				const	{ return _access; }
		EMemoryAccess::bits	MappingAccess ()			const	{ return _mappingAccess; }

		bool IsMappingAllowed (EMappingFlags mapFlags);
		void OnMapped (void *ptr, BytesUL offset, BytesUL size, EMappingFlags mapFlags);
		bool FlushMemoryRange (BytesUL offset, BytesUL size);
		bool Unmap ();

		bool Read (BytesUL offset, BytesUL size, OUT BinArrayCRef &result) const;
		bool Write (BinArrayCRef data, BytesUL offset, OUT BytesUL &result);

		bool ReadImage (OUT BytesUL &readn, OUT BinArrayRef result, const uint4 &dstDimension, BytesUL dstRowPitch, BytesUL dstSlicePitch,
						const uint4 &srcOffset, const uint4 &srcDimension, BytesUL srcMemOffset, BytesUL srcMemSize, BytesUL srcRowPitch, BytesUL srcSlicePitch) const;

		bool WriteImage (OUT BytesUL &written, BinArrayCRef data, const uint4 &srcDimension, BytesUL srcRowPitch, BytesUL srcSlicePitch,
						 const uint4 &dstOffset, const uint4 &dstDimension, BytesUL dstMemOffset, BytesUL dstMemSize, BytesUL dstRowPitch, BytesUL dstSlicePitch);

	private:
		static EMemoryAccess::bits _GpuMemoryToMemoryAccess (EGpuMemory::bits flags);
	};


}	// PlatformTools
}	// Engine
