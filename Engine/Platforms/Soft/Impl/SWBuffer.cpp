// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Buffer.h"
#include "Engine/Platforms/Shared/GPU/Memory.h"
#include "Engine/Platforms/Soft/Impl/SWBaseModule.h"
#include "Engine/Platforms/Soft/SoftRendererObjectsConstructor.h"

#ifdef GRAPHICS_API_SOFT

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
											GpuMsg::GetSWBufferMemoryRequirements
										> >::Append< ForwardToMem_t >;

		using SupportedEvents_t		= SWBaseModule::SupportedEvents_t;
		
		using MemoryEvents_t		= MessageListFrom< GpuMsg::OnMemoryBindingChanged >;


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
		
	// event handlers
		bool _OnMemoryBindingChanged (const Message< GpuMsg::OnMemoryBindingChanged > &);


	private:
		bool _IsCreated () const;
		bool _CreateBuffer ();

		void _DestroyAll ();
		void _OnMemoryBinded ();
		void _OnMemoryUnbinded ();
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

		msg->result.Set({ _memory, _memAccess, _align });
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
