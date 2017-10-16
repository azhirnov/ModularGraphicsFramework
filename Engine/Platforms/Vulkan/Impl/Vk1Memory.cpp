// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Memory.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/VulkanContext.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{

	//
	// Vulkan Memory
	//

	class Vk1Memory final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::GetStreamDescriptor,
											ModuleMsg::ReadFromStream,
											ModuleMsg::WriteToStream,
											GpuMsg::MapMemoryToCpu,
											GpuMsg::FlushMemoryRange,
											GpuMsg::UnmapMemory,
											GpuMsg::CopyMemory
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t::Append< MessageListFrom<
											ModuleMsg::DataRegionChanged,
											GpuMsg::OnMemoryBindingChanged
										> >;
		
		using ImageMessages_t		= MessageListFrom< GpuMsg::GetVkImageID >;
		using BufferMessages_t		= MessageListFrom< GpuMsg::GetVkBufferID >;

		using EBindingState			= GpuMsg::OnMemoryBindingChanged::EBindingState;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		vk::VkDeviceMemory		_mem;
		ubyte *					_mappedPtr;

		BytesUL					_size;
		BytesUL					_mappedOffset;
		BytesUL					_mappedSize;
		
		EGpuMemory::bits		_flags;
		EMemoryAccess::bits		_access;
		uint					_heapIndex;
		EBindingState			_binding;
		bool					_isMapped;


	// methods
	public:
		Vk1Memory (const GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci);
		~Vk1Memory ();


	// message handlers
	private:
		bool _GetStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &);
		bool _ReadFromStream (const Message< ModuleMsg::ReadFromStream > &);
		bool _WriteToStream (const Message< ModuleMsg::WriteToStream > &);
		bool _MapMemoryToCpu (const Message< GpuMsg::MapMemoryToCpu > &);
		bool _FlushMemoryRange (const Message< GpuMsg::FlushMemoryRange > &);
		bool _UnmapMemory (const Message< GpuMsg::UnmapMemory > &);
		bool _CopyMemory (const Message< GpuMsg::CopyMemory > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);

	private:
		bool _IsCreated () const;
		bool _AllocForImage ();
		bool _AllocForBuffer ();
		void _FreeMemory ();

		static EMemoryAccess::bits _GpuMemoryToMemoryAccess (EGpuMemory::bits flags);
	};
//-----------------------------------------------------------------------------



	const Runtime::VirtualTypeList	Vk1Memory::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	Vk1Memory::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1Memory::Vk1Memory (const GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci) :
		Vk1BaseModule( gs, ci.gpuThread, ModuleConfig{ VkMemoryModuleID, 1 }, &_msgTypes, &_eventTypes ),
		_mem( VK_NULL_HANDLE ),
		_mappedPtr( null ),
		// TODO: ci.maxSize ignored
		_flags( ci.memFlags ),
		_access( ci.access & _GpuMemoryToMemoryAccess( ci.memFlags ) ),
		_heapIndex( ~0u ),
		_binding( EBindingState::Unbinded ),
		_isMapped( false )
	{
		SetDebugName( "Vk1Memory" );

		_SubscribeOnMsg( this, &Vk1Memory::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1Memory::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1Memory::_AttachModule_Empty );
		_SubscribeOnMsg( this, &Vk1Memory::_DetachModule_Empty );
		_SubscribeOnMsg( this, &Vk1Memory::_FindModule_Empty );
		_SubscribeOnMsg( this, &Vk1Memory::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &Vk1Memory::_Link_Impl );
		_SubscribeOnMsg( this, &Vk1Memory::_Compose );
		_SubscribeOnMsg( this, &Vk1Memory::_Delete );
		_SubscribeOnMsg( this, &Vk1Memory::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1Memory::_DeviceBeforeDestroy );
		_SubscribeOnMsg( this, &Vk1Memory::_GetStreamDescriptor );
		_SubscribeOnMsg( this, &Vk1Memory::_ReadFromStream );
		_SubscribeOnMsg( this, &Vk1Memory::_WriteToStream );
		_SubscribeOnMsg( this, &Vk1Memory::_MapMemoryToCpu );
		_SubscribeOnMsg( this, &Vk1Memory::_FlushMemoryRange );
		_SubscribeOnMsg( this, &Vk1Memory::_UnmapMemory );
		_SubscribeOnMsg( this, &Vk1Memory::_CopyMemory );
		_SubscribeOnMsg( this, &Vk1Memory::_GetVkLogicDevice );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, Platforms::VkThreadModuleID, true );
	}

/*
=================================================
	destructor
=================================================
*/
	Vk1Memory::~Vk1Memory ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_IsCreated
=================================================
*/
	bool Vk1Memory::_IsCreated () const
	{
		return _mem != VK_NULL_HANDLE;
	}
	
/*
=================================================
	_AllocForImage
=================================================
*/
	bool Vk1Memory::_AllocForImage ()
	{
		using namespace vk;
		
		CHECK_ERR( not _IsCreated() );

		// request image ID
		Message< GpuMsg::GetVkImageID >	msg;
		SendTo( _GetParents().Front(), msg );

		VkImage	img_id	= msg->result.Get( VkImage(VK_NULL_HANDLE) );
		CHECK_ERR( img_id != VK_NULL_HANDLE );


		// allocate memory
		VkMemoryRequirements	mem_reqs = {};
		vkGetImageMemoryRequirements( GetLogicalDevice(), img_id, &mem_reqs );
		
		VkMemoryAllocateInfo	info = {};
		info.sType				= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		info.pNext				= null;
		info.allocationSize		= (VkDeviceSize) mem_reqs.size;
		info.memoryTypeIndex	= 0;

		CHECK_ERR( GetDevice()->GetMemoryTypeIndex( mem_reqs.memoryTypeBits, Vk1Enum( _flags ), OUT info.memoryTypeIndex ) );
		
		VK_CHECK( vkAllocateMemory( GetLogicalDevice(), &info, null, OUT &_mem ) );
		
		GetDevice()->SetObjectName( _mem, GetDebugName(), EGpuObject::DeviceMemory );

		// bind memory to image
		VK_CHECK( vkBindImageMemory( GetLogicalDevice(), img_id, _mem, 0 ) );
		
		_binding = EBindingState::BindedToImage;
		return true;
	}
	
/*
=================================================
	_AllocForBuffer
=================================================
*/
	bool Vk1Memory::_AllocForBuffer ()
	{
		using namespace vk;
		
		CHECK_ERR( not _IsCreated() );

		// request image ID
		Message< GpuMsg::GetVkBufferID >	msg;
		SendTo( _GetParents().Front(), msg );

		VkBuffer	buf_id	= msg->result.Get( VkBuffer(VK_NULL_HANDLE) );
		CHECK_ERR( buf_id != VK_NULL_HANDLE );


		// allocate memory
		VkMemoryRequirements	mem_reqs = {};
		vkGetBufferMemoryRequirements( GetLogicalDevice(), buf_id, &mem_reqs );
		
		VkMemoryAllocateInfo	info = {};
		info.sType				= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		info.pNext				= null;
		info.allocationSize		= (VkDeviceSize) mem_reqs.size;
		info.memoryTypeIndex	= 0;

		CHECK_ERR( GetDevice()->GetMemoryTypeIndex( mem_reqs.memoryTypeBits, Vk1Enum( _flags ), OUT info.memoryTypeIndex ) );
		
		VK_CHECK( vkAllocateMemory( GetLogicalDevice(), &info, null, OUT &_mem ) );
		
		GetDevice()->SetObjectName( _mem, GetDebugName(), EGpuObject::DeviceMemory );

		// bind memory to image
		VK_CHECK( vkBindBufferMemory( GetLogicalDevice(), buf_id, _mem, 0 ) );

		_binding = EBindingState::BindedToBuffer;
		return true;
	}

/*
=================================================
	_FreeMemory
=================================================
*/
	void Vk1Memory::_FreeMemory ()
	{
		using namespace vk;

		auto	dev = GetLogicalDevice();

		if ( dev != VK_NULL_HANDLE and _mem != VK_NULL_HANDLE )
		{
			vkFreeMemory( dev, _mem, null );
		}

		_mem			= VK_NULL_HANDLE;
		_mappedPtr		= null;
		_size			= Uninitialized;
		_mappedOffset	= Uninitialized;
		_mappedSize		= Uninitialized;
		_flags			= Uninitialized;
		_access			= Uninitialized;
		_heapIndex		= -1;
		_binding		= EBindingState::Unbinded;
		_isMapped		= false;
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool Vk1Memory::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_ERR( not _IsCreated() );
		CHECK_ERR( _GetParents().Count() == 1 );

		ModulePtr const&	parent = _GetParents().Front();

		if ( parent->GetSupportedMessages().HasAllTypes< ImageMessages_t >() )
		{
			CHECK_COMPOSING( _AllocForImage() );
		}
		else
		if ( parent->GetSupportedMessages().HasAllTypes< BufferMessages_t >() )
		{
			CHECK_COMPOSING( _AllocForBuffer() );
		}

		CHECK( _DefCompose( false ) );
		CHECK( _SendEvent( Message< GpuMsg::OnMemoryBindingChanged >{ parent, _binding } ) );

		return true;
	}

/*
=================================================
	_GetStreamDescriptor
=================================================
*/
	bool Vk1Memory::_GetStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &msg)
	{
		StreamDescriptor	descr;

		descr.memoryFlags	= _access;
		descr.available		= _mappedSize;
		descr.totalSize		= _IsCreated() ? _size : BytesUL();

		msg->result.Set( descr );
		return true;
	}
	
/*
=================================================
	_ReadFromStream
=================================================
*/
	bool Vk1Memory::_ReadFromStream (const Message< ModuleMsg::ReadFromStream > &msg)
	{
		CHECK_ERR( _isMapped );
		CHECK_ERR( msg->offset < _mappedSize );

		const usize		offset	= (usize) msg->offset;
		const usize		size	= (usize) Min( _mappedSize - msg->offset, msg->size.Get( BytesUL(-1) ) );

		msg->result.Set( BinArrayCRef( _mappedPtr + offset, size ) );

		_SendEvent< ModuleMsg::DataRegionChanged >({ EMemoryAccess::CpuRead, BytesU(offset), BytesU(size) });
		return true;
	}
	
/*
=================================================
	_WriteToStream
=================================================
*/
	bool Vk1Memory::_WriteToStream (const Message< ModuleMsg::WriteToStream > &msg)
	{
		CHECK_ERR( _isMapped );
		CHECK_ERR( msg->offset < _mappedSize );
		
		const usize		offset		= (usize) msg->offset;
		const usize		size		= Min( usize(_mappedSize), usize(msg->data.Size()) );

		MemCopy( BinArrayRef( _mappedPtr + offset, size ), msg->data );

		msg->wasWritten.Set( BytesUL(size) );

		_SendEvent< ModuleMsg::DataRegionChanged >({ EMemoryAccess::CpuWrite, BytesU(offset), BytesU(size) });
		return true;
	}
	
/*
=================================================
	_MapMemoryToCpu
=================================================
*/
	bool Vk1Memory::_MapMemoryToCpu (const Message< GpuMsg::MapMemoryToCpu > &msg)
	{
		using namespace vk;
		
		CHECK_ERR( _IsCreated() and not _isMapped );
		CHECK_ERR( msg->offset < _size );
		
		const BytesUL	size = Min( _size, msg->size );

		VK_CHECK( vkMapMemory( GetLogicalDevice(), _mem, (VkDeviceSize) msg->offset, (VkDeviceSize) size, 0, (void **)&_mappedPtr ) );

		_isMapped		= true;
		_mappedSize		= size;
		_mappedOffset	= msg->offset;
		return true;
	}
	
/*
=================================================
	_FlushMemoryRange
=================================================
*/
	bool Vk1Memory::_FlushMemoryRange (const Message< GpuMsg::FlushMemoryRange > &msg)
	{
		TODO( "" );
		return true;
	}
	
/*
=================================================
	_UnmapMemory
=================================================
*/
	bool Vk1Memory::_UnmapMemory (const Message< GpuMsg::UnmapMemory > &msg)
	{
		using namespace vk;

		CHECK_ERR( _isMapped );
		
		vkUnmapMemory( GetLogicalDevice(), _mem );

		_isMapped	= false;
		_mappedSize	= BytesUL(0);
		_mappedPtr	= null;

		return true;
	}
	
/*
=================================================
	_CopyMemory
=================================================
*/
	bool Vk1Memory::_CopyMemory (const Message< GpuMsg::CopyMemory > &msg)
	{
		CHECK_ERR( _IsCreated() );

		// TODO: create buffer for transfer and copy memory in gpu
		TODO( "" );
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool Vk1Memory::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_FreeMemory();

		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_GpuMemoryToMemoryAccess
=================================================
*/
	EMemoryAccess::bits Vk1Memory::_GpuMemoryToMemoryAccess (EGpuMemory::bits flags)
	{
		EMemoryAccess::bits		result;

		result |= EMemoryAccess::GpuRead;
		result |= EMemoryAccess::GpuWrite;

		if ( flags.Get( EGpuMemory::CoherentWithCPU ) or flags.Get( EGpuMemory::CachedInCPU ) )
		{
			result |= EMemoryAccess::CpuRead;
			result |= EMemoryAccess::CpuWrite;
		}

		return result;
	}

}	// PlatformVK
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr VulkanContext::_CreateVk1Memory (const GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci)
	{
		return New< PlatformVK::Vk1Memory >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
