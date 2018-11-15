// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Public/Tools/MemoryMapperHelper.h"

namespace Engine
{
namespace PlatformTools
{

/*
=================================================
	constructor
=================================================
*/
	MemoryMapperHelper::MemoryMapperHelper (EGpuMemory::bits flags, EMemoryAccess::bits access) :
		_ptr{ null },		_changed{ false }
	{
		ResetFlags( flags, access );
	}
	
/*
=================================================
	destructor
=================================================
*/
	MemoryMapperHelper::~MemoryMapperHelper ()
	{
	}
	
/*
=================================================
	Clear
=================================================
*/
	void MemoryMapperHelper::Clear ()
	{
		_ptr				= null;
		_offset				= Uninitialized;
		_size				= Uninitialized;
		_access				= Uninitialized;
		_mappingAccess		= Uninitialized;
		_isCachedInCPU		= false;
		_isCoherentWithCPU	= false;
		_changed			= false;
	}
	
/*
=================================================
	ResetFlags
=================================================
*/
	void MemoryMapperHelper::ResetFlags (EGpuMemory::bits flags, EMemoryAccess::bits access)
	{
		CHECK_ERR( not _changed, void() );
		
		_access				= (access & _GpuMemoryToMemoryAccess( flags ));
		_isCachedInCPU		= flags[EGpuMemory::CachedInCPU];
		_isCoherentWithCPU	= flags[EGpuMemory::CoherentWithCPU];
	}

/*
=================================================
	IsMappingAllowed
=================================================
*/
	bool MemoryMapperHelper::IsMappingAllowed (EMappingFlags mapFlags)
	{
		switch ( mapFlags )
		{
			case EMappingFlags::Read :			return not IsMapped() and _access[EMemoryAccess::CpuRead];
			case EMappingFlags::Write :
			case EMappingFlags::WriteDiscard :	return not IsMapped() and _access[EMemoryAccess::CpuWrite];
			case EMappingFlags::ReadWrite :		return not IsMapped() and _access[EMemoryAccess::CpuRead] and _access[EMemoryAccess::CpuWrite];
		}
		return false;
	}
	
/*
=================================================
	OnMapped
=================================================
*/
	void MemoryMapperHelper::OnMapped (void *ptr, BytesU offset, BytesU size, EMappingFlags mapFlags)
	{
		CHECK( ptr != null );

		_mappingAccess = Uninitialized;

		switch ( mapFlags )
		{
			case EMappingFlags::Read :			_mappingAccess |= EMemoryAccess::CpuRead;		break;
			case EMappingFlags::Write :
			case EMappingFlags::WriteDiscard :	_mappingAccess |= EMemoryAccess::CpuWrite;		break;
			case EMappingFlags::ReadWrite :		_mappingAccess |= EMemoryAccess::CpuReadWrite;	break;
			default :							RETURN_ERR( "unknown mapping flags", void() );
		}

		_ptr		= Cast<ubyte *>(ptr);
		_offset		= offset;
		_size		= size;
		_changed	= false;
	}
	
/*
=================================================
	Unmap
=================================================
*/
	bool MemoryMapperHelper::Unmap ()
	{
		CHECK_ERR( IsMapped() );
		CHECK_ERR( _isCoherentWithCPU or not IsMappedMemoryChanged() );	// changes must be flushed

		_ptr			= null;
		_offset			= 0_b;
		_size			= 0_b;
		_changed		= false;
		_mappingAccess	= Uninitialized;
		return true;
	}
	
/*
=================================================
	FlushMemoryRange
=================================================
*/
	bool MemoryMapperHelper::FlushMemoryRange (BytesU offset, BytesU size)
	{
		CHECK_ERR( IsMapped() );
		CHECK( not _isCoherentWithCPU );	// it is not needed becouse of coherency flag

		ASSERT( offset == 0_b and size == _size );	// not a full mapped region will be flushed

		_changed = false;	// it is simple check, without regard to regions
		return true;
	}
	
/*
=================================================
	Read
=================================================
*/
	bool MemoryMapperHelper::Read (BytesU offset, BytesU size, OUT BinArrayCRef &result) const
	{
		CHECK_ERR( IsMapped() );
		CHECK_ERR( _mappingAccess[EMemoryAccess::CpuRead] );
		CHECK_ERR( offset < MappedSize() );

		const usize		c_offset	= usize(offset);
		const usize		c_size		= usize(Min( MappedSize() - offset, size ));

		result = BinArrayCRef( _ptr + c_offset, c_size );
		return true;
	}
	
/*
=================================================
	Write
=================================================
*/
	bool MemoryMapperHelper::Write (BinArrayCRef data, BytesU offset, OUT BytesU &result)
	{
		CHECK_ERR( IsMapped() );
		CHECK_ERR( _mappingAccess[EMemoryAccess::CpuWrite] );
		CHECK_ERR( offset < MappedSize() );
			
		const usize		c_offset	= usize(offset);
		const usize		c_size		= Min( usize(MappedSize()), usize(data.Size()) );

		MemCopy( OUT BinArrayRef( _ptr + c_offset, c_size ), data );
		result		= BytesU(c_size);
		_changed	= true;
		return true;
	}
	
/*
=================================================
	ReadImage
=================================================
*/
	bool MemoryMapperHelper::ReadImage (OUT BytesU &readn, OUT BinArrayRef result, BitsU bitPerPixel,
										const uint3 &dstDimension, BytesU dstRowPitch, BytesU dstSlicePitch,
										const uint3 &srcOffset, const uint3 &srcDimension, BytesU srcMemOffset, BytesU srcMemSize, BytesU srcRowPitch, BytesU srcSlicePitch) const
	{
		CHECK_ERR( IsMapped() );
		CHECK_ERR( _mappingAccess[EMemoryAccess::CpuRead] );
		CHECK_ERR( All( dstDimension.xyz() > uint3(0) ) );
		CHECK_ERR( All ( srcDimension + srcOffset <= dstDimension ) );
		CHECK_ERR( srcMemOffset + srcMemSize <= _size );

		srcRowPitch		= Max( srcRowPitch, BytesU(bitPerPixel * srcDimension.x) );
		srcSlicePitch	= Max( srcSlicePitch, srcRowPitch * srcDimension.y );
		dstRowPitch		= Max( dstRowPitch, BytesU(bitPerPixel * dstDimension.x) );
		dstSlicePitch	= Max( dstSlicePitch, dstRowPitch * dstDimension.y );
		readn			= 0_b;

		CHECK_ERR( dstSlicePitch * dstDimension.z <= result.Size() );
		CHECK_ERR( srcSlicePitch * srcDimension.z <= srcMemSize );

		for (uint z = 0; z < dstDimension.z; ++z)
		{
			readn = AlignToLarge( readn, dstSlicePitch );

			for (uint y = 0; y < dstDimension.y; ++y)
			{
				BytesU			src_off	 = srcMemOffset + srcSlicePitch * (srcOffset.z + z) + srcRowPitch * (srcOffset.y + y) + BytesU(bitPerPixel * srcOffset.x);
				const ubyte *	src		 = _ptr + src_off;
				BytesU			dst_size = dstSlicePitch * z + dstRowPitch * y;
				ubyte *			dst		 = result.ptr() + dst_size;

				UnsafeMem::MemCopy( OUT dst, src, dstRowPitch );

				readn += dstRowPitch;
			}
		}
		return true;
	}
	
/*
=================================================
	WriteImage
=================================================
*/
	bool MemoryMapperHelper::WriteImage (OUT BytesU &written, BinArrayCRef data, BitsU bitPerPixel,
										 const uint3 &srcDimension, BytesU srcRowPitch, BytesU srcSlicePitch,
										 const uint3 &dstOffset, const uint3 &dstDimension, BytesU dstMemOffset, BytesU dstMemSize, BytesU dstRowPitch, BytesU dstSlicePitch)
	{
		CHECK_ERR( IsMapped() );
		CHECK_ERR( _mappingAccess[EMemoryAccess::CpuWrite] );
		CHECK_ERR( All( srcDimension.xyz() > uint3(0) ) );
		CHECK_ERR( All( srcDimension + dstOffset <= dstDimension ) );
		CHECK_ERR( dstMemOffset + dstMemSize <= _size );
		
		srcRowPitch		= Max( srcRowPitch, BytesU(bitPerPixel * srcDimension.x) );
		srcSlicePitch	= Max( srcSlicePitch, srcRowPitch * srcDimension.y );
		dstRowPitch		= Max( dstRowPitch, BytesU(bitPerPixel * dstDimension.x) );
		dstSlicePitch	= Max( dstSlicePitch, dstRowPitch * dstDimension.y );
		written			= 0_b;

		CHECK_ERR( dstSlicePitch * dstDimension.z <= data.Size() );
		CHECK_ERR( dstSlicePitch * dstDimension.z <= dstMemSize );

		for (uint z = 0; z < srcDimension.z; ++z)
		{
			written = AlignToLarge( written, srcSlicePitch );

			for (uint y = 0; y < srcDimension.y; ++y)
			{
				BytesU			src_size = z * srcSlicePitch + y * srcRowPitch;
				const ubyte *	src		 = data.ptr() + src_size;
				BytesU			dst_off	 = dstMemOffset + dstSlicePitch * (dstOffset.z + z) + dstRowPitch * (dstOffset.y + y) + BytesU(bitPerPixel * dstOffset.x);
				ubyte *			dst		 = _ptr + dst_off;

				UnsafeMem::MemCopy( OUT dst, src, srcRowPitch );

				written += srcRowPitch;
			}
		}
		return true;
	}
	
/*
=================================================
	_GpuMemoryToMemoryAccess
=================================================
*/
	EMemoryAccess::bits  MemoryMapperHelper::_GpuMemoryToMemoryAccess (EGpuMemory::bits flags)
	{
		EMemoryAccess::bits		result;

		result |= EMemoryAccess::GpuRead;
		result |= EMemoryAccess::GpuWrite;

		if ( flags[EGpuMemory::CoherentWithCPU] or flags[EGpuMemory::CachedInCPU] )
		{
			result |= EMemoryAccess::CpuRead;
			result |= EMemoryAccess::CpuWrite;
		}

		return result;
	}


}	// PlatformTools
}	// Engine
