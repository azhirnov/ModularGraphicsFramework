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
		_ptr{ null },
		_isCachedInCPU{ flags[EGpuMemory::CachedInCPU] },
		_isCoherentWithCPU{ flags[EGpuMemory::CoherentWithCPU] },
		_access{ access & _GpuMemoryToMemoryAccess( flags ) },
		_changed{ false }
	{
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

		_ptr		= (ubyte*) ptr;
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

		const usize		c_offset	= (usize) offset;
		const usize		c_size		= (usize) Min( MappedSize() - offset, size );

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
			
		const usize		c_offset	= (usize) offset;
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
	bool MemoryMapperHelper::ReadImage (OUT BytesUL &readn, OUT BinArrayRef result, const uint3 &dstDimension, BytesUL dstRowPitch, BytesUL dstSlicePitch,
										const uint3 &srcOffset, const uint3 &srcDimension, BytesUL srcMemOffset, BytesUL srcMemSize, BytesUL srcRowPitch, BytesUL srcSlicePitch) const
	{
		CHECK_ERR( IsMapped() );
		CHECK_ERR( _mappingAccess[EMemoryAccess::CpuRead] );
		CHECK_ERR( All( dstDimension.xyz() > uint3(0) ) );
		//CHECK_ERR( All ( srcDimension + srcOffset <= srcDimension ) );	// TODO
		CHECK_ERR( srcMemOffset + srcMemSize <= _size );

		uint3			coord;
		const ubyte *	src_end	= _ptr + (srcMemOffset + srcMemSize);
		BitsUL			bpp		= BitsUL(srcRowPitch) / srcDimension.x;

		readn = BytesUL(0);

		for (; coord.z < dstDimension.z; ++coord.z)
		{
			readn = AlignToLarge( readn, dstSlicePitch );

			for (; coord.y < dstDimension.y; ++coord.y)
			{
				const ubyte *	src		 = _ptr + (srcMemOffset + srcSlicePitch * (srcOffset.z + coord.z) + srcRowPitch * (srcOffset.y + coord.y) + BytesUL(bpp * srcOffset.x));
				BytesUL			dst_size = (dstSlicePitch * coord.z + dstRowPitch * coord.y);
				ubyte *			dst		 = result.ptr() + dst_size;

				CHECK_ERR( src < src_end and dst < result.End() );
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
	bool MemoryMapperHelper::WriteImage (OUT BytesUL &written, BinArrayCRef data, const uint3 &srcDimension, BytesUL srcRowPitch, BytesUL srcSlicePitch,
										 const uint3 &dstOffset, const uint3 &dstDimension, BytesUL dstMemOffset, BytesUL dstMemSize, BytesUL dstRowPitch, BytesUL dstSlicePitch)
	{
		CHECK_ERR( IsMapped() );
		CHECK_ERR( _mappingAccess[EMemoryAccess::CpuWrite] );
		CHECK_ERR( All( srcDimension.xyz() > uint3(0) ) );
		CHECK_ERR( All ( srcDimension + dstOffset <= dstDimension ) );
		CHECK_ERR( dstMemOffset + dstMemSize <= _size );

		uint3			coord;
		const ubyte*	dst_end	= _ptr + (dstMemOffset + dstMemSize);
		const BitsUL	bpp		= BitsUL(dstRowPitch) / dstDimension.x;

		written = BytesUL(0);

		for (; coord.z < srcDimension.z; ++coord.z)
		{
			written = AlignToLarge( written, srcSlicePitch );

			for (; coord.y < srcDimension.y; ++coord.y)
			{
				BytesUL			src_size = (coord.z * srcSlicePitch + coord.y * srcRowPitch + coord.x);
				const ubyte *	src		 = data.ptr() + src_size;
				ubyte *			dst		 = _ptr + (dstMemOffset + dstSlicePitch * (dstOffset.z + coord.z) + dstRowPitch * (dstOffset.y + coord.y) + BytesUL(bpp * dstOffset.x));

				CHECK_ERR( src < data.End() and dst < dst_end );
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
