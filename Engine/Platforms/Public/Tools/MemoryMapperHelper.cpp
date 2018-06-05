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
	void MemoryMapperHelper::OnMapped (void *ptr, BytesUL offset, BytesUL size, EMappingFlags mapFlags)
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
		_offset			= BytesUL();
		_size			= BytesUL();
		_changed		= false;
		_mappingAccess	= Uninitialized;
		return true;
	}
	
/*
=================================================
	FlushMemoryRange
=================================================
*/
	bool MemoryMapperHelper::FlushMemoryRange (BytesUL offset, BytesUL size)
	{
		CHECK_ERR( IsMapped() );
		CHECK( not _isCoherentWithCPU );	// it is not needed becouse of coherency flag

		ASSERT( offset == BytesUL(0) and size == _size );	// not a full mapped region will be flushed

		_changed = false;	// it is simple check, without regard to regions
		return true;
	}
	
/*
=================================================
	Read
=================================================
*/
	bool MemoryMapperHelper::Read (BytesUL offset, BytesUL size, OUT BinArrayCRef &result) const
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
	bool MemoryMapperHelper::Write (BinArrayCRef data, BytesUL offset, OUT BytesUL &result)
	{
		CHECK_ERR( IsMapped() );
		CHECK_ERR( _mappingAccess[EMemoryAccess::CpuWrite] );
		CHECK_ERR( offset < MappedSize() );
			
		const usize		c_offset	= usize(offset);
		const usize		c_size		= Min( usize(MappedSize()), usize(data.Size()) );

		MemCopy( BinArrayRef( _ptr + c_offset, c_size ), data );
		result		= BytesUL(c_size);
		_changed	= true;
		return true;
	}
	
/*
=================================================
	ReadImage
=================================================
*/
	bool MemoryMapperHelper::ReadImage (OUT BytesUL &readn, OUT BinArrayRef result, BitsU bitPerPixel,
										const uint3 &dstDimension, BytesUL dstRowPitch, BytesUL dstSlicePitch,
										const uint3 &srcOffset, const uint3 &srcDimension, BytesUL srcMemOffset, BytesUL srcMemSize, BytesUL srcRowPitch, BytesUL srcSlicePitch) const
	{
		CHECK_ERR( IsMapped() );
		CHECK_ERR( _mappingAccess[EMemoryAccess::CpuRead] );
		CHECK_ERR( All( dstDimension.xyz() > uint3(0) ) );
		CHECK_ERR( All ( srcDimension + srcOffset <= dstDimension ) );
		CHECK_ERR( srcMemOffset + srcMemSize <= _size );

		srcRowPitch		= Max( srcRowPitch, BytesUL(bitPerPixel * srcDimension.x) );
		srcSlicePitch	= Max( srcSlicePitch, srcRowPitch * srcDimension.y );
		dstRowPitch		= Max( dstRowPitch, BytesUL(bitPerPixel * dstDimension.x) );
		dstSlicePitch	= Max( dstSlicePitch, dstRowPitch * dstDimension.y );
		readn			= BytesUL(0);

		CHECK_ERR( dstSlicePitch * dstDimension.z <= BytesUL(result.Size()) );
		CHECK_ERR( srcSlicePitch * srcDimension.z <= srcMemSize );

		for (uint z = 0; z < dstDimension.z; ++z)
		{
			readn = AlignToLarge( readn, dstSlicePitch );

			for (uint y = 0; y < dstDimension.y; ++y)
			{
				BytesUL			src_off	 = srcMemOffset + srcSlicePitch * (srcOffset.z + z) + srcRowPitch * (srcOffset.y + y) + BytesUL(bitPerPixel * srcOffset.x);
				const ubyte *	src		 = _ptr + src_off;
				BytesUL			dst_size = dstSlicePitch * z + dstRowPitch * y;
				ubyte *			dst		 = result.ptr() + dst_size;

				UnsafeMem::MemCopy( dst, src, BytesU(dstRowPitch) );

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
	bool MemoryMapperHelper::WriteImage (OUT BytesUL &written, BinArrayCRef data, BitsU bitPerPixel,
										 const uint3 &srcDimension, BytesUL srcRowPitch, BytesUL srcSlicePitch,
										 const uint3 &dstOffset, const uint3 &dstDimension, BytesUL dstMemOffset, BytesUL dstMemSize, BytesUL dstRowPitch, BytesUL dstSlicePitch)
	{
		CHECK_ERR( IsMapped() );
		CHECK_ERR( _mappingAccess[EMemoryAccess::CpuWrite] );
		CHECK_ERR( All( srcDimension.xyz() > uint3(0) ) );
		CHECK_ERR( All( srcDimension + dstOffset <= dstDimension ) );
		CHECK_ERR( dstMemOffset + dstMemSize <= _size );
		
		srcRowPitch		= Max( srcRowPitch, BytesUL(bitPerPixel * srcDimension.x) );
		srcSlicePitch	= Max( srcSlicePitch, srcRowPitch * srcDimension.y );
		dstRowPitch		= Max( dstRowPitch, BytesUL(bitPerPixel * dstDimension.x) );
		dstSlicePitch	= Max( dstSlicePitch, dstRowPitch * dstDimension.y );
		written			= BytesUL(0);

		CHECK_ERR( dstSlicePitch * dstDimension.z <= BytesUL(data.Size()) );
		CHECK_ERR( dstSlicePitch * dstDimension.z <= dstMemSize );

		for (uint z = 0; z < srcDimension.z; ++z)
		{
			written = AlignToLarge( written, srcSlicePitch );

			for (uint y = 0; y < srcDimension.y; ++y)
			{
				BytesUL			src_size = z * srcSlicePitch + y * srcRowPitch;
				const ubyte *	src		 = data.ptr() + src_size;
				BytesUL			dst_off	 = dstMemOffset + dstSlicePitch * (dstOffset.z + z) + dstRowPitch * (dstOffset.y + y) + BytesUL(bitPerPixel * dstOffset.x);
				ubyte *			dst		 = _ptr + dst_off;

				UnsafeMem::MemCopy( dst, src, BytesU(srcRowPitch) );

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
