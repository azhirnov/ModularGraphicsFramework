// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/Memory.h"

namespace Engine
{
namespace PlatformTools
{
	using namespace Engine::Platforms;


	//
	// Memory Mapper
	//

	class MemoryMapperHelper final : public Noncopyable
	{
	// types
	private:
		using EBindingTarget	= GpuMsg::OnMemoryBindingChanged::EBindingTarget;
		using EMappingFlags		= GpuMsg::EMappingFlags;


	// variables
	private:
		ubyte *					_ptr;
		BytesU					_offset;
		BytesU					_size;

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
		void ResetFlags (EGpuMemory::bits flags, EMemoryAccess::bits access);

		ND_ bool				IsMapped ()					const	{ return _ptr != null; }
		ND_ void *				Pointer ()							{ return _ptr; }
		ND_ bool				IsMappedMemoryChanged ()	const	{ return _changed; }
		ND_ BytesU				MappedOffset ()				const	{ return _offset; }
		ND_ BytesU				MappedSize ()				const	{ return _size; }
		ND_ EMemoryAccess::bits	MemoryAccess ()				const	{ return _access; }
		ND_ EMemoryAccess::bits	MappingAccess ()			const	{ return _mappingAccess; }

		ND_ bool IsMappingAllowed (EMappingFlags mapFlags);

		void OnMapped (void *ptr, BytesU offset, BytesU size, EMappingFlags mapFlags);
		bool FlushMemoryRange (BytesU offset, BytesU size);
		bool Unmap ();

		bool Read (BytesU offset, BytesU size, OUT BinArrayCRef &result) const;
		bool Write (BinArrayCRef data, BytesU offset, OUT BytesU &result);

		bool ReadImage (OUT BytesU &readn, OUT BinArrayRef result, BitsU bitPerPixel,
						const uint3 &dstDimension, BytesU dstRowPitch, BytesU dstSlicePitch,
						const uint3 &srcOffset, const uint3 &srcDimension, BytesU srcMemOffset, BytesU srcMemSize, BytesU srcRowPitch, BytesU srcSlicePitch) const;

		bool WriteImage (OUT BytesU &written, BinArrayCRef data, BitsU bitPerPixel,
						 const uint3 &srcDimension, BytesU srcRowPitch, BytesU srcSlicePitch,
						 const uint3 &dstOffset, const uint3 &dstDimension, BytesU dstMemOffset, BytesU dstMemSize, BytesU dstRowPitch, BytesU dstSlicePitch);

	private:
		static EMemoryAccess::bits _GpuMemoryToMemoryAccess (EGpuMemory::bits flags);
	};


}	// PlatformTools
}	// Engine
