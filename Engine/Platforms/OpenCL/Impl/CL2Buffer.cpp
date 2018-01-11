// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Buffer.h"
#include "Engine/Platforms/Shared/GPU/Memory.h"
#include "Engine/Platforms/OpenCL/Impl/CL2BaseModule.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"

#if defined( COMPUTE_API_OPENCL )

namespace Engine
{
namespace PlatformCL
{
	
	//
	// OpenCL Buffer
	//

	class CL2Buffer final : public CL2BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= CL2BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetBufferDescriptor,
											GpuMsg::GetCLBufferID,
											GpuMsg::GpuMemoryRegionChanged,
											ModuleMsg::GetStreamDescriptor,
											ModuleMsg::ReadFromStream,
											ModuleMsg::WriteToStream,
											GpuMsg::MapMemoryToCpu,
											GpuMsg::FlushMemoryRange,
											GpuMsg::UnmapMemory
										> >;

		using SupportedEvents_t		= CL2BaseModule::SupportedEvents_t;
		
		using MemoryMsg_t			= MessageListFrom< GpuMsg::GetGpuMemoryDescriptor >;
		using MemoryEvents_t		= MessageListFrom< GpuMsg::OnMemoryBindingChanged >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		BufferDescriptor		_descr;
		ModulePtr				_memObj;
		cl::cl_mem				_bufferId;
		
		EGpuMemory::bits		_memFlags;		// -|-- this flags is requirements for memory obj, don't use it anywhere
		EMemoryAccess::bits		_memAccess;		// -|
		bool					_useMemMngr;	// -|

		bool					_isBindedToMemory;


	// methods
	public:
		CL2Buffer (GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci);
		~CL2Buffer ();


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _AttachModule (const Message< ModuleMsg::AttachModule > &);
		bool _DetachModule (const Message< ModuleMsg::DetachModule > &);
		bool _GetCLBufferID (const Message< GpuMsg::GetCLBufferID > &);
		bool _GetBufferDescriptor (const Message< GpuMsg::GetBufferDescriptor > &);
		bool _OnMemoryBindingChanged (const Message< GpuMsg::OnMemoryBindingChanged > &);
		bool _GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &);
		bool _GetStreamDescriptor (const Message< GpuMsg::GetStreamDescriptor > &);
		bool _ReadFromStream (const Message< GpuMsg::ReadFromStream > &);
		bool _WriteToStream (const Message< GpuMsg::WriteToStream > &);
		bool _MapMemoryToCpu (const Message< GpuMsg::MapMemoryToCpu > &);
		bool _FlushMemoryRange (const Message< GpuMsg::FlushMemoryRange > &);
		bool _UnmapMemory (const Message< GpuMsg::UnmapMemory > &);

	private:
		bool _IsCreated () const;
		bool _CreateBuffer ();

		void _DestroyAll ();
		void _OnMemoryUnbinded ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	CL2Buffer::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	CL2Buffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

	static const EBufferUsage::bits	supportedBufferUsage	= EBufferUsage::Storage | EBufferUsage::Uniform |
															  EBufferUsage::TransferDst | EBufferUsage::TransferSrc;

/*
=================================================
	constructor
=================================================
*/
	CL2Buffer::CL2Buffer (GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci) :
		CL2BaseModule( gs, ModuleConfig{ CLBufferModuleID, UMax }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),				_bufferId( null ),
		_memFlags( ci.memFlags ),		_memAccess( ci.access ),
		_useMemMngr( ci.allocMem ),		_isBindedToMemory( false )
	{
		SetDebugName( "CL2Buffer" );
		
		ASSERT( (_descr.usage & supportedBufferUsage).IsNotZero() and "none of supported flags is used" );
		ASSERT( (_descr.usage & ~supportedBufferUsage).IsZero() and "used unsupported flags" );

		_SubscribeOnMsg( this, &CL2Buffer::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &CL2Buffer::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &CL2Buffer::_AttachModule );
		_SubscribeOnMsg( this, &CL2Buffer::_DetachModule );
		_SubscribeOnMsg( this, &CL2Buffer::_FindModule_Impl );
		_SubscribeOnMsg( this, &CL2Buffer::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &CL2Buffer::_Link );
		_SubscribeOnMsg( this, &CL2Buffer::_Compose );
		_SubscribeOnMsg( this, &CL2Buffer::_Delete );
		_SubscribeOnMsg( this, &CL2Buffer::_OnManagerChanged );
		_SubscribeOnMsg( this, &CL2Buffer::_GetCLBufferID );
		_SubscribeOnMsg( this, &CL2Buffer::_GetBufferDescriptor );
		_SubscribeOnMsg( this, &CL2Buffer::_GetDeviceInfo );
		_SubscribeOnMsg( this, &CL2Buffer::_GetCLDeviceInfo );
		_SubscribeOnMsg( this, &CL2Buffer::_GetCLPrivateClasses );
		_SubscribeOnMsg( this, &CL2Buffer::_GpuMemoryRegionChanged );
		_SubscribeOnMsg( this, &CL2Buffer::_GetStreamDescriptor );
		_SubscribeOnMsg( this, &CL2Buffer::_ReadFromStream );
		_SubscribeOnMsg( this, &CL2Buffer::_WriteToStream );
		_SubscribeOnMsg( this, &CL2Buffer::_MapMemoryToCpu );
		_SubscribeOnMsg( this, &CL2Buffer::_FlushMemoryRange );
		_SubscribeOnMsg( this, &CL2Buffer::_UnmapMemory );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, CLThreadModuleID, true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	CL2Buffer::~CL2Buffer ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool CL2Buffer::_Link (const Message< ModuleMsg::Link > &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( GetState() == EState::Initial or GetState() == EState::LinkingFailed );

		_memObj = GetModuleByMsgEvent< MemoryMsg_t, MemoryEvents_t >();

		if ( not _memObj and _useMemMngr )
		{
			ModulePtr	mem_module;
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
								CLMemoryModuleID,
								GlobalSystems(),
								CreateInfo::GpuMemory{ null, _memFlags, _memAccess },
								OUT mem_module ) );

			CHECK_ERR( _Attach( "mem", mem_module, true ) );
			_memObj = mem_module;
		}
		CHECK_ATTACHMENT( _memObj );

		// TODO: check shared objects

		Message< GpuMsg::GetGpuMemoryDescriptor >	req_descr;
		_memObj->Send( req_descr );
		_memFlags = req_descr->result->flags;

		_memObj->Subscribe( this, &CL2Buffer::_OnMemoryBindingChanged );

		CHECK_ERR( Module::_Link_Impl( msg ) );
		return true;
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool CL2Buffer::_Compose (const Message< ModuleMsg::Compose > &msg)
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
	bool CL2Buffer::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyAll();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_AttachModule
=================================================
*/
	bool CL2Buffer::_AttachModule (const Message< ModuleMsg::AttachModule > &msg)
	{
		CHECK( _Attach( msg->name, msg->newModule, true ) );

		if ( msg->newModule->GetSupportedEvents().HasAllTypes< MemoryEvents_t >() )
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
	bool CL2Buffer::_DetachModule (const Message< ModuleMsg::DetachModule > &msg)
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
	bool CL2Buffer::_GetBufferDescriptor (const Message< GpuMsg::GetBufferDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_GetCLBufferID
=================================================
*/
	bool CL2Buffer::_GetCLBufferID (const Message< GpuMsg::GetCLBufferID > &msg)
	{
		msg->result.Set( _bufferId );
		return true;
	}
	
/*
=================================================
	_OnMemoryBindingChanged
=================================================
*/
	bool CL2Buffer::_OnMemoryBindingChanged (const Message< GpuMsg::OnMemoryBindingChanged > &msg)
	{
		CHECK_ERR( _IsComposedOrLinkedState( GetState() ) );

		using EBindingTarget = GpuMsg::OnMemoryBindingChanged::EBindingTarget;

		if (  msg->targetObject == this )
		{
			_isBindedToMemory = ( msg->newState == EBindingTarget::Buffer );

			if ( _isBindedToMemory )
			{
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
	bool CL2Buffer::_IsCreated () const
	{
		return _bufferId != null;
	}
	
/*
=================================================
	_CreateBuffer
=================================================
*/
	bool CL2Buffer::_CreateBuffer ()
	{
		using namespace cl;

		CHECK_ERR( not _IsCreated() );
		CHECK_ERR( not _isBindedToMemory );
		
		cl_int	cl_err = 0;
		CL_CHECK( ((_bufferId = clCreateBuffer(
							GetContext(),
							CL2Enum( _memFlags ),
							(size_t) _descr.size,
							null,
							&cl_err )), cl_err) );
		return true;
	}
	
/*
=================================================
	_DestroyAll
=================================================
*/
	void CL2Buffer::_DestroyAll ()
	{
		using namespace cl;
		
		if ( _bufferId != null ) {
			CL_CALL( clReleaseMemObject( _bufferId ) );
		}

		_OnMemoryUnbinded();

		_bufferId	= null;
		_descr		= Uninitialized;
	}
	
/*
=================================================
	_OnMemoryUnbinded
=================================================
*/
	void CL2Buffer::_OnMemoryUnbinded ()
	{
		if ( _memObj )
		{
			_memObj->UnsubscribeAll( this );
			_memObj = null;
		}
		
		_memFlags			= Uninitialized;
		_isBindedToMemory	= false;
	}

/*
=================================================
	_GpuMemoryRegionChanged
=================================================
*/
	bool CL2Buffer::_GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &)
	{
		// request buffer memory barrier
		TODO( "" );
		return false;
	}
	
/*
=================================================
	_GetStreamDescriptor
=================================================
*/
	bool CL2Buffer::_GetStreamDescriptor (const Message< GpuMsg::GetStreamDescriptor > &msg)
	{
		return _memObj ? _memObj->Send( msg ) : false;
	}
	
/*
=================================================
	_ReadFromStream
=================================================
*/
	bool CL2Buffer::_ReadFromStream (const Message< GpuMsg::ReadFromStream > &msg)
	{
		return _memObj ? _memObj->Send( msg ) : false;
	}
	
/*
=================================================
	_WriteToStream
=================================================
*/
	bool CL2Buffer::_WriteToStream (const Message< GpuMsg::WriteToStream > &msg)
	{
		return _memObj ? _memObj->Send( msg ) : false;
	}
	
/*
=================================================
	_MapMemoryToCpu
=================================================
*/
	bool CL2Buffer::_MapMemoryToCpu (const Message< GpuMsg::MapMemoryToCpu > &msg)
	{
		return _memObj ? _memObj->Send( msg ) : false;
	}
	
/*
=================================================
	_FlushMemoryRange
=================================================
*/
	bool CL2Buffer::_FlushMemoryRange (const Message< GpuMsg::FlushMemoryRange > &msg)
	{
		return _memObj ? _memObj->Send( msg ) : false;
	}
	
/*
=================================================
	_UnmapMemory
=================================================
*/
	bool CL2Buffer::_UnmapMemory (const Message< GpuMsg::UnmapMemory > &msg)
	{
		return _memObj ? _memObj->Send( msg ) : false;
	}
	
}	// PlatformCL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenCLObjectsConstructor::CreateCL2Buffer (GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci)
	{
		return New< PlatformCL::CL2Buffer >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
