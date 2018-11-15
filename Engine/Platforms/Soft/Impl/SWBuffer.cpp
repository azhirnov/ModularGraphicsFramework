// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

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
											DSMsg::GetDataSourceDescription,
											DSMsg::ReadMemRange,
											DSMsg::WriteMemRange,
											GpuMsg::GetGpuMemoryDescription,
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
											GpuMsg::GetBufferDescription,
											GpuMsg::SetBufferDescription,
											GpuMsg::GetSWBufferMemoryLayout,
											GpuMsg::GetSWBufferMemoryRequirements
										> >;

		using SupportedEvents_t		= SWBaseModule::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::SetBufferDescription
										> >;
		
		using MemoryEvents_t		= MessageListFrom< GpuMsg::OnMemoryBindingChanged >;


	// constants
	private:
		static const TypeIdList		_eventTypes;

		static constexpr BytesU		_align	= 4_b;


	// variables
	private:
		BufferDescription		_descr;
		ModulePtr				_memObj;
		BinArrayRef				_memory;
		
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
		bool _Link (const ModuleMsg::Link &);
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);
		bool _AttachModule (const ModuleMsg::AttachModule &);
		bool _DetachModule (const ModuleMsg::DetachModule &);
		bool _GetBufferDescription (const GpuMsg::GetBufferDescription &);
		bool _SetBufferDescription (const GpuMsg::SetBufferDescription &);
		bool _GetSWBufferMemoryLayout (const GpuMsg::GetSWBufferMemoryLayout &);
		bool _GetSWBufferMemoryRequirements (const GpuMsg::GetSWBufferMemoryRequirements &);
		
	// event handlers
		bool _OnMemoryBindingChanged (const GpuMsg::OnMemoryBindingChanged &);


	private:
		bool _IsCreated () const;
		bool _CreateBuffer ();

		void _DestroyAll ();
		void _OnMemoryBinded ();
		void _OnMemoryUnbinded ();
		
		static void _ValidateMemFlags (INOUT EGpuMemory::bits &flags);
		static void _ValidateDescription (INOUT BufferDescription &descr);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	SWBuffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SWBuffer::SWBuffer (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci) :
		SWBaseModule( gs, ModuleConfig{ id, UMax }, &_eventTypes ),
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
		_SubscribeOnMsg( this, &SWBuffer::_GetBufferDescription );
		_SubscribeOnMsg( this, &SWBuffer::_SetBufferDescription );
		_SubscribeOnMsg( this, &SWBuffer::_GetDeviceInfo );
		_SubscribeOnMsg( this, &SWBuffer::_GetSWDeviceInfo );
		_SubscribeOnMsg( this, &SWBuffer::_GetSWPrivateClasses );
		_SubscribeOnMsg( this, &SWBuffer::_GetSWBufferMemoryLayout );
		_SubscribeOnMsg( this, &SWBuffer::_GetSWBufferMemoryRequirements );
		
		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
		
		_ValidateMemFlags( INOUT _memFlags );
		_ValidateDescription( INOUT _descr );
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
	bool SWBuffer::_Link (const ModuleMsg::Link &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( _IsInitialState( GetState() ) );
		
		_memObj = GetModuleByEvent< MemoryEvents_t >();

		if ( not _memObj and _useMemMngr )
		{
			ModulePtr	mem_module;
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
								SWMemoryModuleID,
								GlobalSystems(),
								CreateInfo::GpuMemory{ _memFlags, _memAccess },
								OUT mem_module ) );

			CHECK_ERR( _Attach( "mem", mem_module ) );
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
	bool SWBuffer::_Compose (const ModuleMsg::Compose &msg)
	{
		if ( _IsComposedState( GetState() ) or _IsCreated() )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreateBuffer() );

		_SendForEachAttachments( msg );
		
		CHECK( _ValidateAllSubscriptions() );
		
		// composed state will be changed when memory binded to image
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool SWBuffer::_Delete (const ModuleMsg::Delete &msg)
	{
		_DestroyAll();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_AttachModule
=================================================
*/
	bool SWBuffer::_AttachModule (const ModuleMsg::AttachModule &msg)
	{
		const bool	is_mem	= msg.newModule->GetSupportedEvents().HasAllTypes< MemoryEvents_t >();

		CHECK( _Attach( msg.name, msg.newModule ) );

		if ( is_mem )
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
	bool SWBuffer::_DetachModule (const ModuleMsg::DetachModule &msg)
	{
		CHECK( _Detach( msg.oldModule ) );

		if ( msg.oldModule == _memObj )
		{
			CHECK( _SetState( EState::Initial ) );
			_OnMemoryUnbinded();
		}
		return true;
	}

/*
=================================================
	_GetBufferDescription
=================================================
*/
	bool SWBuffer::_GetBufferDescription (const GpuMsg::GetBufferDescription &msg)
	{
		msg.result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_SetBufferDescription
=================================================
*/
	bool SWBuffer::_SetBufferDescription (const GpuMsg::SetBufferDescription &msg)
	{
		CHECK_ERR( GetState() == EState::Initial );

		_descr = msg.descr;
		return true;
	}
	
/*
=================================================
	_OnMemoryBindingChanged
=================================================
*/
	bool SWBuffer::_OnMemoryBindingChanged (const GpuMsg::OnMemoryBindingChanged &msg)
	{
		CHECK_ERR( _IsComposedOrLinkedState( GetState() ) );

		using EBindingTarget = GpuMsg::OnMemoryBindingChanged::EBindingTarget;

		if (  msg.targetObject == this )
		{
			_isBindedToMemory = ( msg.newState == EBindingTarget::Buffer );

			if ( _isBindedToMemory )
			{
				_OnMemoryBinded();
				CHECK( _SetState( EState::ComposedMutable ) );
				
				_SendUncheckedEvent( ModuleMsg::AfterCompose{} );
			}
			else
			{
				Send( ModuleMsg::Delete{} );
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
		_memory		= _memObj->Request( GpuMsg::GetSWMemoryData{} );
		_memAccess	= _memObj->Request( GpuMsg::GetGpuMemoryDescription{} ).access;

		CHECK( _memory.Size() == _descr.size );
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
	_GetSWBufferMemoryLayout
=================================================
*/
	bool SWBuffer::_GetSWBufferMemoryLayout (const GpuMsg::GetSWBufferMemoryLayout &msg)
	{
		CHECK_ERR( _isBindedToMemory );

		msg.result.Set({ _memory.SubArray( usize(msg.offset), usize(msg.size) ), _align, _memAccess });
		return true;
	}

/*
=================================================
	_GetSWBufferMemoryRequirements
=================================================
*/
	bool SWBuffer::_GetSWBufferMemoryRequirements (const GpuMsg::GetSWBufferMemoryRequirements &msg)
	{
		msg.result.Set({ _descr.size, _align });
		return true;
	}
	
/*
=================================================
	_ValidateMemFlags
=================================================
*/
	void SWBuffer::_ValidateMemFlags (INOUT EGpuMemory::bits &flags)
	{
		if ( flags[EGpuMemory::Dedicated] and flags[EGpuMemory::SupportAliasing] )
		{
			WARNING( "not supported" );

			flags[EGpuMemory::Dedicated] = false;
		}
	}
	
/*
=================================================
	_ValidateDescription
=================================================
*/
	void SWBuffer::_ValidateDescription (INOUT BufferDescription &)
	{
	}

}	// PlatformSW
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr SoftRendererObjectsConstructor::CreateSWBuffer (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci)
	{
		return New< PlatformSW::SWBuffer >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_SOFT
