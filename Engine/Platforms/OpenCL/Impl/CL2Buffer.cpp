// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Memory.h"
#include "Engine/Platforms/OpenCL/Impl/CL2BaseModule.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"

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

		using SupportedMessages_t	= CL2BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetBufferDescriptor,
											GpuMsg::SetBufferDescriptor,
											GpuMsg::GetCLBufferID,
											GpuMsg::CreateCLSubBuffer,
											GpuMsg::GpuMemoryRegionChanged
										> >::Append< ForwardToMem_t >;

		using SupportedEvents_t		= CL2BaseModule::SupportedEvents_t;
		
		using MemoryMsg_t			= MessageListFrom< GpuMsg::GetGpuMemoryDescriptor >;
		using MemoryEvents_t		= MessageListFrom< GpuMsg::OnMemoryBindingChanged >;

		using SubBufferMap_t		= HashMap< Pair<BytesUL, BytesUL>, cl::cl_mem >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		BufferDescriptor		_descr;
		ModulePtr				_memObj;
		cl::cl_mem				_bufferId;
		SubBufferMap_t			_subBuffers;
		
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
		bool _CreateCLSubBuffer (const Message< GpuMsg::CreateCLSubBuffer > &);
		bool _SetBufferDescriptor (const Message< GpuMsg::SetBufferDescriptor > &);
		bool _GetBufferDescriptor (const Message< GpuMsg::GetBufferDescriptor > &);
		bool _GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &);
		
	// event handlers
		bool _OnMemoryBindingChanged (const Message< GpuMsg::OnMemoryBindingChanged > &);


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
		_SubscribeOnMsg( this, &CL2Buffer::_CreateCLSubBuffer );
		_SubscribeOnMsg( this, &CL2Buffer::_SetBufferDescriptor );
		_SubscribeOnMsg( this, &CL2Buffer::_GetBufferDescriptor );
		_SubscribeOnMsg( this, &CL2Buffer::_GetDeviceInfo );
		_SubscribeOnMsg( this, &CL2Buffer::_GetCLDeviceInfo );
		_SubscribeOnMsg( this, &CL2Buffer::_GetCLPrivateClasses );
		_SubscribeOnMsg( this, &CL2Buffer::_GpuMemoryRegionChanged );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
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
		
		CHECK_LINKING( _CopySubscriptions< ForwardToMem_t >( _memObj ) );
		
		return Module::_Link_Impl( msg );
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
		const bool	is_mem	= msg->newModule->GetSupportedEvents().HasAllTypes< MemoryEvents_t >();

		CHECK( _Attach( msg->name, msg->newModule, is_mem ) );

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
	_SetBufferDescriptor
=================================================
*/
	bool CL2Buffer::_SetBufferDescriptor (const Message< GpuMsg::SetBufferDescriptor > &msg)
	{
		CHECK_ERR( GetState() == EState::Initial );

		_descr = msg->descr;
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
	_GetCLBufferID
=================================================
*/
	bool CL2Buffer::_CreateCLSubBuffer (const Message< GpuMsg::CreateCLSubBuffer > &msg)
	{
		using namespace cl;

		CHECK_ERR( _IsCreated() and _isBindedToMemory );
		CHECK_ERR( msg->offset < _descr.size and msg->offset + msg->size <= _descr.size );

		if ( msg->offset == BytesUL(0) and msg->size == _descr.size )
		{
			msg->result.Set( _bufferId );
			return true;
		}

		SubBufferMap_t::iterator		iter;
		const SubBufferMap_t::Key_t		key {msg->offset, msg->size};

		if ( not _subBuffers.Find( key, OUT iter ) )
		{
			struct cl_buffer_region {
				size_t origin;
				size_t size;
			};

			cl_buffer_region	info	= { size_t(msg->offset), size_t(msg->size) };
			cl_int				cl_err	= 0;
			cl_mem				sub_buf = null;
			CL_CALL( (sub_buf = clCreateSubBuffer( _bufferId, CL2Enum( _memAccess ), CL_BUFFER_CREATE_TYPE_REGION, &info, OUT &cl_err )), cl_err );

			iter = _subBuffers.Add( key, sub_buf );
		}

		msg->result.Set( iter->second );
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
		CL_CHECK(( (_bufferId = clCreateBuffer(
							GetContext(),
							CL2Enum( _memFlags ),
							(size_t) _descr.size,
							null,
							OUT &cl_err )), cl_err ));
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
		
		FOR( i, _subBuffers ) {
			CL_CALL( clReleaseMemObject( _subBuffers[i].second ) );
		}
		_subBuffers.Clear();

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
