// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_SOFT

#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Memory.h"
#include "Engine/Platforms/Public/GPU/Enums.ToString.h"
#include "Engine/Platforms/Soft/Impl/SWBaseModule.h"
#include "Engine/Platforms/Soft/SoftRendererObjectsConstructor.h"

namespace Engine
{
namespace PlatformSW
{
	
	//
	// Software Renderer Buffer
	//

	class SWBuffer final : public SWBaseModule
	{
	// types
	private:
		using ForwardToMem_t		= MessageListFrom< 
											ModuleMsg::GetStreamDescriptor,
											ModuleMsg::ReadFromStream,
											ModuleMsg::WriteToStream,
											GpuMsg::MapMemoryToCpu,
											GpuMsg::MapImageToCpu,
											GpuMsg::FlushMemoryRange,
											GpuMsg::UnmapMemory,
											GpuMsg::ReadFromGpuMemory,
											GpuMsg::WriteToGpuMemory,
											GpuMsg::ReadFromImageMemory,
											GpuMsg::WriteToImageMemory
										>;

		using SupportedMessages_t	= SWBaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetBufferDescriptor,
											GpuMsg::SetBufferDescriptor,
											GpuMsg::GpuMemoryRegionChanged,
											GpuMsg::GetSWBufferMemoryLayout,
											GpuMsg::GetSWBufferMemoryRequirements,
											GpuMsg::SWBufferBarrier
										> >::Append< ForwardToMem_t >;

		using SupportedEvents_t		= SWBaseModule::SupportedEvents_t;
		
		using MemoryEvents_t		= MessageListFrom< GpuMsg::OnMemoryBindingChanged >;
		

		struct Block : CompileTime::FastCopyable
		{
		// variables
			BytesU					offset;
			EPipelineAccess::bits	access;
			EPipelineStage::type	stage	= EPipelineStage::Unknown;	// last synchronization stage

		// methods
			Block () {}
			Block (BytesU offset, EPipelineAccess::bits access, EPipelineStage::type stage) :
				offset{offset}, access{access}, stage{stage} {}
		};

		using Blocks_t				= Array< Block >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;

		static constexpr BytesU		_align	= 4_b;


	// variables
	private:
		BufferDescriptor		_descr;
		ModulePtr				_memObj;
		BinArrayRef				_memory;
		Blocks_t				_blocks;
		
		EGpuMemory::bits		_memFlags;		// -|-- this flags is requirements for memory obj, don't use it anywhere
		EMemoryAccess::bits		_memAccess;		// -|
		bool					_useMemMngr;	// -|

		bool					_isCreated;
		bool					_isBindedToMemory;


	// methods
	public:
		SWBuffer (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci);
		~SWBuffer ();


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _AttachModule (const Message< ModuleMsg::AttachModule > &);
		bool _DetachModule (const Message< ModuleMsg::DetachModule > &);
		bool _GetBufferDescriptor (const Message< GpuMsg::GetBufferDescriptor > &);
		bool _SetBufferDescriptor (const Message< GpuMsg::SetBufferDescriptor > &);
		bool _GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &);
		bool _GetSWBufferMemoryLayout (const Message< GpuMsg::GetSWBufferMemoryLayout > &);
		bool _GetSWBufferMemoryRequirements (const Message< GpuMsg::GetSWBufferMemoryRequirements > &);
		bool _SWBufferBarrier (const Message< GpuMsg::SWBufferBarrier > &);
		
	// event handlers
		bool _OnMemoryBindingChanged (const Message< GpuMsg::OnMemoryBindingChanged > &);


	private:
		bool _IsCreated () const;
		bool _CreateBuffer ();

		void _DestroyAll ();
		void _OnMemoryBinded ();
		void _OnMemoryUnbinded ();

		bool _MergeBlocks (Block newBlock, BytesU size);
		//void _MergeBlocks (const Block &newBlock, EPipelineAccess::bits lastAccess, EPipelineStage::bits lastStage);
		
		void _CheckAccess (const Block &block, EPipelineAccess::bits access, EPipelineStage::type stage) const;

		static bool _IsIntersects (const Block &curr, const Block &next, BytesU offset, BytesU size);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	SWBuffer::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	SWBuffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SWBuffer::SWBuffer (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci) :
		SWBaseModule( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),			_memFlags( ci.memFlags ),
		_memAccess( ci.access ),	_useMemMngr( ci.allocMem ),
		_isCreated( false ),		_isBindedToMemory( false )
	{
		SetDebugName( "SWBuffer" );

		_SubscribeOnMsg( this, &SWBuffer::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &SWBuffer::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &SWBuffer::_AttachModule );
		_SubscribeOnMsg( this, &SWBuffer::_DetachModule );
		_SubscribeOnMsg( this, &SWBuffer::_FindModule_Impl );
		_SubscribeOnMsg( this, &SWBuffer::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &SWBuffer::_Link );
		_SubscribeOnMsg( this, &SWBuffer::_Compose );
		_SubscribeOnMsg( this, &SWBuffer::_Delete );
		_SubscribeOnMsg( this, &SWBuffer::_OnManagerChanged );
		_SubscribeOnMsg( this, &SWBuffer::_GetBufferDescriptor );
		_SubscribeOnMsg( this, &SWBuffer::_SetBufferDescriptor );
		_SubscribeOnMsg( this, &SWBuffer::_GetDeviceInfo );
		_SubscribeOnMsg( this, &SWBuffer::_GetSWDeviceInfo );
		_SubscribeOnMsg( this, &SWBuffer::_GetSWPrivateClasses );
		_SubscribeOnMsg( this, &SWBuffer::_GpuMemoryRegionChanged );
		_SubscribeOnMsg( this, &SWBuffer::_GetSWBufferMemoryLayout );
		_SubscribeOnMsg( this, &SWBuffer::_GetSWBufferMemoryRequirements );
		_SubscribeOnMsg( this, &SWBuffer::_SWBufferBarrier );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	SWBuffer::~SWBuffer ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool SWBuffer::_Link (const Message< ModuleMsg::Link > &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( GetState() == EState::Initial or GetState() == EState::LinkingFailed );
		
		_memObj = GetModuleByEvent< MemoryEvents_t >();

		if ( not _memObj and _useMemMngr )
		{
			ModulePtr	mem_module;
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
								SWMemoryModuleID,
								GlobalSystems(),
								CreateInfo::GpuMemory{ null, _memFlags, _memAccess },
								OUT mem_module ) );

			CHECK_ERR( _Attach( "mem", mem_module, true ) );
			_memObj = mem_module;
		}
		CHECK_ATTACHMENT( _memObj );

		_memObj->Subscribe( this, &SWBuffer::_OnMemoryBindingChanged );
		
		CHECK_LINKING( _CopySubscriptions< ForwardToMem_t >( _memObj ) );

		return Module::_Link_Impl( msg );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool SWBuffer::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) or _IsCreated() )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreateBuffer() );

		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );
		
		// composed state will be changed when memory binded to image
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool SWBuffer::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyAll();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_AttachModule
=================================================
*/
	bool SWBuffer::_AttachModule (const Message< ModuleMsg::AttachModule > &msg)
	{
		const bool	is_mem	= msg->newModule->GetSupportedEvents().HasAllTypes< MemoryEvents_t >();

		CHECK( _Attach( msg->name, msg->newModule, is_mem ) );

		if (is_mem )
		{
			CHECK( _SetState( EState::Initial ) );
			_OnMemoryUnbinded();
		}
		return true;
	}
	
/*
=================================================
	_DetachModule
=================================================
*/
	bool SWBuffer::_DetachModule (const Message< ModuleMsg::DetachModule > &msg)
	{
		CHECK( _Detach( msg->oldModule ) );

		if ( msg->oldModule->GetSupportedEvents().HasAllTypes< MemoryEvents_t >() )
		{
			CHECK( _SetState( EState::Initial ) );
			_OnMemoryUnbinded();
		}
		return true;
	}

/*
=================================================
	_GetBufferDescriptor
=================================================
*/
	bool SWBuffer::_GetBufferDescriptor (const Message< GpuMsg::GetBufferDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_SetBufferDescriptor
=================================================
*/
	bool SWBuffer::_SetBufferDescriptor (const Message< GpuMsg::SetBufferDescriptor > &msg)
	{
		CHECK_ERR( GetState() == EState::Initial );

		_descr = msg->descr;
		return true;
	}
	
/*
=================================================
	_OnMemoryBindingChanged
=================================================
*/
	bool SWBuffer::_OnMemoryBindingChanged (const Message< GpuMsg::OnMemoryBindingChanged > &msg)
	{
		CHECK_ERR( _IsComposedOrLinkedState( GetState() ) );

		using EBindingTarget = GpuMsg::OnMemoryBindingChanged::EBindingTarget;

		if (  msg->targetObject == this )
		{
			_isBindedToMemory = ( msg->newState == EBindingTarget::Buffer );

			if ( _isBindedToMemory )
			{
				_OnMemoryBinded();
				CHECK( _SetState( EState::ComposedMutable ) );
				
				_SendUncheckedEvent< ModuleMsg::AfterCompose >({});
			}
			else
			{
				CHECK( _SetState( EState::Linked ) );
			}
		}
		return true;
	}

/*
=================================================
	_IsCreated
=================================================
*/
	bool SWBuffer::_IsCreated () const
	{
		return _isCreated;
	}
	
/*
=================================================
	_CreateBuffer
=================================================
*/
	bool SWBuffer::_CreateBuffer ()
	{
		CHECK_ERR( not _IsCreated() );
		CHECK_ERR( not _isBindedToMemory );

		_isCreated = true;
		return true;
	}
	
/*
=================================================
	_DestroyAll
=================================================
*/
	void SWBuffer::_DestroyAll ()
	{
		_OnMemoryUnbinded();

		_isCreated	= false;
		_descr		= Uninitialized;
	}
	
/*
=================================================
	_OnMemoryBinded
=================================================
*/
	void SWBuffer::_OnMemoryBinded ()
	{
		Message< GpuMsg::GetSWMemoryData >			req_data;
		Message< GpuMsg::GetGpuMemoryDescriptor >	req_descr;

		SendTo( _memObj, req_data );
		SendTo( _memObj, req_descr );

		_memory		= *req_data->result;
		_memAccess	= req_descr->result->access;

		_blocks.Clear();
		_blocks.PushBack(Block{ 0_b, EPipelineAccess::bits(), EPipelineStage::Unknown });
		_blocks.PushBack(Block{ _memory.Size(), EPipelineAccess::bits(), EPipelineStage::Unknown });

		CHECK( BytesUL(_memory.Size()) == _descr.size );
	}

/*
=================================================
	_OnMemoryUnbinded
=================================================
*/
	void SWBuffer::_OnMemoryUnbinded ()
	{
		if ( _memObj )
		{
			this->UnsubscribeAll( _memObj );
			_memObj->UnsubscribeAll( this );
			_memObj = null;
		}

		_isBindedToMemory	= false;
		_memory				= Uninitialized;
		_blocks.Clear();
	}

/*
=================================================
	_GpuMemoryRegionChanged
=================================================
*/
	bool SWBuffer::_GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &)
	{
		// request buffer memory barrier
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_IsIntersects
=================================================
*/
	bool SWBuffer::_IsIntersects (const Block &curr, const Block &next, BytesU offset, BytesU size)
	{
		return !!(	((curr.offset < offset + size) & (next.offset > offset)) |
					((offset + size < curr.offset) & (offset > next.offset)) );
	}

/*
=================================================
	_GetSWBufferMemoryLayout
=================================================
*/
	bool SWBuffer::_GetSWBufferMemoryLayout (const Message< GpuMsg::GetSWBufferMemoryLayout > &msg)
	{
		CHECK_ERR( _isBindedToMemory );

		// check access
		for (usize i = 1; i < _blocks.Count(); ++i)
		{
			auto const&	curr = _blocks[i-1];
			auto const&	next = _blocks[i];

			if ( _IsIntersects( curr, next, msg->offset, msg->size ) )
			{
				_CheckAccess( curr, msg->access, msg->stage );
			}
		}

		_MergeBlocks( Block{ msg->offset, msg->access, msg->stage }, msg->size );

		msg->result.Set({ _memory.SubArray( usize(msg->offset), usize(msg->size) ), _align, _memAccess });
		return true;
	}

/*
=================================================
	_GetSWBufferMemoryRequirements
=================================================
*/
	bool SWBuffer::_GetSWBufferMemoryRequirements (const Message< GpuMsg::GetSWBufferMemoryRequirements > &msg)
	{
		msg->result.Set({ BytesU(_descr.size), _align });
		return true;
	}
	
/*
=================================================
	_SWBufferBarrier
=================================================
*/
	bool SWBuffer::_SWBufferBarrier (const Message< GpuMsg::SWBufferBarrier > &msg)
	{
		for (auto& br : Range(msg->barriers))
		{
			ASSERT( br.buffer == this );

			_MergeBlocks( Block{ BytesU(br.offset), br.dstAccessMask, EPipelineStage::BottomOfPipe }, BytesU(br.size ));

			//_MergeBlocks( Block{ BytesU(br.offset), BytesU(br.size), br.dstAccessMask, msg->dstStageMask }, br.srcAccessMask, msg->srcStageMask );
		}
		return true;
	}

/*
=================================================
	_MergeBlocks
=================================================
*/
	bool SWBuffer::_MergeBlocks (Block newBlock, BytesU blockSize)
	{
		// insert new blocks
		Block	curr_block	= _blocks.Front();

		for (usize i = 1; i < _blocks.Count(); ++i)
		{
			auto&			curr	= _blocks[i-1];
			auto const&		next	= _blocks[i];
			const BytesU	size	= next.offset - curr.offset;
			
			if ( not _IsIntersects( curr, next, newBlock.offset, blockSize ) )
				continue;

			if ( curr.offset < newBlock.offset and
				 newBlock.offset < next.offset )
			{
				curr_block = curr;
				curr_block.offset = newBlock.offset;

				_blocks.Insert( curr_block, i );
				continue;
			}

			if ( curr.offset == newBlock.offset and
				 blockSize > 0 )
			{
				curr_block	= curr;
				curr		= newBlock;

				if ( blockSize < size )
				{
					curr_block.offset += blockSize;

					_blocks.Insert( curr_block, i );
					break;
				}
				else
				{
					blockSize		-= size;
					newBlock.offset += size;
				}
			}
				 
		}

		// optimize
		for (usize i = 1; i+1 < _blocks.Count(); ++i)
		{
			auto const&		curr	= _blocks[i-1];
			auto const&		next	= _blocks[i];

			if ( curr.access == next.access and
				 curr.stage == next.stage )
			{
				_blocks.Erase( i );
				--i;
			}
		}
		return true;
	}
	
	/*void SWBuffer::_MergeBlocks (const Block &newBlock, EPipelineAccess::bits lastAccess, EPipelineStage::bits lastStage)
	{
		// TODO
		_MergeBlocks( newBlock );
	}*/
	
/*
=================================================
	_CheckAccess
=================================================
*/
	void SWBuffer::_CheckAccess (const Block &block, EPipelineAccess::bits accessMask, EPipelineStage::type stage) const
	{
		const bool	was_written		= !!(block.access & EPipelineAccess::WriteMask);
		const bool	will_be_read	= !!(accessMask & EPipelineAccess::ReadMask);
		const bool	will_be_written	= !!(accessMask & EPipelineAccess::WriteMask);
		const bool	need_sync		= was_written and will_be_read;
		
		// read,  read  -> no sync
		// write, read  -> sync
		// write, write -> no sync ?

		SW_DEBUG_REPORT( not need_sync,
						 "missed synchronization between read and write access: from stage '"_str <<
						 EPipelineStage::ToString( block.stage ) << "', with access '" << EPipelineAccess::ToString( block.access ) <<
						 "' to stage '" << EPipelineStage::ToString( stage ) << "', with access '" << EPipelineAccess::ToString( accessMask ) << "'",
						 EDbgReport::Error );
	}

}	// PlatformSW
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr SoftRendererObjectsConstructor::CreateSWBuffer (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci)
	{
		return New< PlatformSW::SWBuffer >( id, gs, ci );
	}
	
	ModulePtr SoftRendererObjectsConstructor::CreateSWSharedBuffer (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuSharedBuffer &ci)
	{
		return null;	// TODO
		//return New< PlatformSW::SWBuffer >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_SOFT
