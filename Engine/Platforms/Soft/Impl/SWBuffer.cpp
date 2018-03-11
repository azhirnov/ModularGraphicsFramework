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
			BytesU					size;
			EPipelineAccess::bits	access;
			EPipelineStage::type	stage	= EPipelineStage::Unknown;	// last synchronization stage

		// methods
			Block () {}
			Block (BytesU offset, BytesU size, EPipelineAccess::bits access, EPipelineStage::type stage) :
				offset{offset}, size{size}, access{access}, stage{stage} {}
		};

		using Blocks_t				= Array< Block >; //FixedSizeArray< Block, 16 >;


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
		SWBuffer (GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci);
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

		void _MergeBlocks (Block newBlock);
		//void _MergeBlocks (const Block &newBlock, EPipelineAccess::bits lastAccess, EPipelineStage::bits lastStage);
		
		void _CheckAccess (INOUT Block &block, EPipelineAccess::bits access, EPipelineStage::type stage) const;
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	SWBuffer::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	SWBuffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SWBuffer::SWBuffer (GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci) :
		SWBaseModule( gs, ModuleConfig{ SWBufferModuleID, UMax }, &_msgTypes, &_eventTypes ),
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
		_blocks.PushBack(Block{ 0_b, _memory.Size(), EPipelineAccess::bits(), EPipelineStage::Unknown });

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
	_GetSWBufferMemoryLayout
=================================================
*/
	bool SWBuffer::_GetSWBufferMemoryLayout (const Message< GpuMsg::GetSWBufferMemoryLayout > &msg)
	{
		CHECK_ERR( _isBindedToMemory );

		// check access
		/*for (auto& curr : Range(_blocks))
		{
			if ( curr.offset + curr.size > msg->offset or
				 curr.offset < msg->offset + msg->size )
			{
				_CheckAccess( curr, msg->access, msg->stage );
			}
		}

		_MergeBlocks(Block{ msg->offset, msg->size, msg->access, msg->stage });*/

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

			_MergeBlocks(Block{ BytesU(br.offset), BytesU(br.size), br.dstAccessMask, EPipelineStage::BottomOfPipe });

			//_MergeBlocks( Block{ BytesU(br.offset), BytesU(br.size), br.dstAccessMask, msg->dstStageMask }, br.srcAccessMask, msg->srcStageMask );
		}
		return true;
	}

/*
=================================================
	_MergeBlocks
=================================================
*/
	void SWBuffer::_MergeBlocks (Block newBlock)
	{
		// insert new block
		/*bool	inserted = false;

		FOR( i, _blocks )
		{
			auto&	curr = _blocks[i];

			if ( newBlock.offset == curr.offset )
			{
				if ( newBlock.size == curr.size )
				{
					curr		= newBlock;
					inserted	= true;
					break;
				}
				else
				if ( newBlock.size < curr.size )
				{
					inserted	= true;
					curr.size	= 
					curr.offset = newBlock.offset + newBlock.size;
					_blocks.Insert( newBlock, i );
					break;
				}
				else // newBlock.size > curr.size
				{
					_blocks.Erase( i );
					--i;
					continue;
				}
			}
			else
			if ( newBlock.offset + newBlock.size > curr.offset )
			{
				inserted	= true;
				curr.offset = newBlock.offset + newBlock.size;
				
				if ( i > 0 )
				{
					auto&	prev = _blocks[i-1];
					prev.size = newBlock.offset - prev.offset;

					_blocks.Insert( newBlock, i-1 );
				}
				else
					_blocks.Insert( newBlock, 0 );

				break;
			}
		}

		if ( not inserted and newBlock.offset < _memory.Size() )
		{
			newBlock.size = Min( _memory.Size() - newBlock.offset, newBlock.size );
			_blocks.PushBack( newBlock );
			inserted = true;
		}

		if ( not inserted ) {
			WARNING( "invalid buffer block range!" );
			return;
		}

		// optimize
		FOR( i, _blocks )
		{
			auto&	curr		= _blocks[i];
			BytesU	next_off	= i+1 < _blocks.Count() ? _blocks[i+1].offset : _memory.Size();

			ASSERT( curr.size > 0_b );
			ASSERT( i > 0 or curr.offset == 0_b );
			ASSERT( curr.offset + curr.size == next_off );

			if ( i+1 < _blocks.Count() )
			{
				auto&	next = _blocks[i+1];

				if ( curr.access == next.access and curr.stage == next.stage )
				{
					curr.size = (next.offset + next.size) - curr.offset;

					_blocks.Erase( i );
					--i;
					continue;
				}
			}
		}*/
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
	void SWBuffer::_CheckAccess (INOUT Block &block, EPipelineAccess::bits accessMask, EPipelineStage::type stage) const
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

		block.access = accessMask;
		block.stage  = stage;
	}

}	// PlatformSW
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr SoftRendererObjectsConstructor::CreateSWBuffer (GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci)
	{
		return New< PlatformSW::SWBuffer >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_SOFT
