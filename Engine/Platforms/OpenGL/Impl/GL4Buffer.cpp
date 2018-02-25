// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Buffer.h"
#include "Engine/Platforms/Shared/GPU/Memory.h"
#include "Engine/Platforms/OpenGL/Impl/GL4BaseModule.h"
#include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"

#ifdef GRAPHICS_API_OPENGL

namespace Engine
{
namespace PlatformGL
{
	
	//
	// OpenGL Buffer
	//

	class GL4Buffer final : public GL4BaseModule
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

		using SupportedMessages_t	= GL4BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetBufferDescriptor,
											GpuMsg::SetBufferDescriptor,
											GpuMsg::GetGLBufferID,
											GpuMsg::GpuMemoryRegionChanged
										> >::Append< ForwardToMem_t >;

		using SupportedEvents_t		= GL4BaseModule::SupportedEvents_t;
		
		using MemoryEvents_t		= MessageListFrom< GpuMsg::OnMemoryBindingChanged >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;
		
		static const gl::GLenum		_defaultTarget = gl::GL_COPY_READ_BUFFER;


	// variables
	private:
		BufferDescriptor		_descr;
		ModulePtr				_memObj;
		gl::GLuint				_bufferId;
		
		EGpuMemory::bits		_memFlags;		// -|-- this flags is requirements for memory obj, don't use it anywhere
		EMemoryAccess::bits		_memAccess;		// -|
		bool					_useMemMngr;	// -|

		bool					_isBindedToMemory;


	// methods
	public:
		GL4Buffer (GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci);
		~GL4Buffer ();


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _AttachModule (const Message< ModuleMsg::AttachModule > &);
		bool _DetachModule (const Message< ModuleMsg::DetachModule > &);
		bool _GetGLBufferID (const Message< GpuMsg::GetGLBufferID > &);
		bool _GetBufferDescriptor (const Message< GpuMsg::GetBufferDescriptor > &);
		bool _SetBufferDescriptor (const Message< GpuMsg::SetBufferDescriptor > &);
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


	
	const TypeIdList	GL4Buffer::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	GL4Buffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4Buffer::GL4Buffer (GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci) :
		GL4BaseModule( gs, ModuleConfig{ GLBufferModuleID, UMax }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),				_bufferId( 0 ),
		_memFlags( ci.memFlags ),		_memAccess( ci.access ),
		_useMemMngr( ci.allocMem ),		_isBindedToMemory( false )
	{
		SetDebugName( "GL4Buffer" );

		_SubscribeOnMsg( this, &GL4Buffer::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &GL4Buffer::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &GL4Buffer::_AttachModule );
		_SubscribeOnMsg( this, &GL4Buffer::_DetachModule );
		_SubscribeOnMsg( this, &GL4Buffer::_FindModule_Impl );
		_SubscribeOnMsg( this, &GL4Buffer::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &GL4Buffer::_Link );
		_SubscribeOnMsg( this, &GL4Buffer::_Compose );
		_SubscribeOnMsg( this, &GL4Buffer::_Delete );
		_SubscribeOnMsg( this, &GL4Buffer::_OnManagerChanged );
		_SubscribeOnMsg( this, &GL4Buffer::_GetGLBufferID );
		_SubscribeOnMsg( this, &GL4Buffer::_GetBufferDescriptor );
		_SubscribeOnMsg( this, &GL4Buffer::_SetBufferDescriptor );
		_SubscribeOnMsg( this, &GL4Buffer::_GetDeviceInfo );
		_SubscribeOnMsg( this, &GL4Buffer::_GetGLDeviceInfo );
		_SubscribeOnMsg( this, &GL4Buffer::_GetGLPrivateClasses );
		_SubscribeOnMsg( this, &GL4Buffer::_GpuMemoryRegionChanged );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	GL4Buffer::~GL4Buffer ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool GL4Buffer::_Link (const Message< ModuleMsg::Link > &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( GetState() == EState::Initial or GetState() == EState::LinkingFailed );
		
		_memObj = GetModuleByEvent< MemoryEvents_t >();

		if ( not _memObj and _useMemMngr )
		{
			ModulePtr	mem_module;
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
								GLMemoryModuleID,
								GlobalSystems(),
								CreateInfo::GpuMemory{ null, _memFlags, _memAccess },
								OUT mem_module ) );

			CHECK_ERR( _Attach( "mem", mem_module, true ) );
			_memObj = mem_module;
		}
		CHECK_ATTACHMENT( _memObj );

		_memObj->Subscribe( this, &GL4Buffer::_OnMemoryBindingChanged );
		
		CHECK_LINKING( _CopySubscriptions< ForwardToMem_t >( _memObj ) );
		
		return Module::_Link_Impl( msg );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool GL4Buffer::_Compose (const Message< ModuleMsg::Compose > &msg)
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
	bool GL4Buffer::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyAll();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_AttachModule
=================================================
*/
	bool GL4Buffer::_AttachModule (const Message< ModuleMsg::AttachModule > &msg)
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
	bool GL4Buffer::_DetachModule (const Message< ModuleMsg::DetachModule > &msg)
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
	bool GL4Buffer::_GetBufferDescriptor (const Message< GpuMsg::GetBufferDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_SetBufferDescriptor
=================================================
*/
	bool GL4Buffer::_SetBufferDescriptor (const Message< GpuMsg::SetBufferDescriptor > &msg)
	{
		CHECK_ERR( GetState() == EState::Initial );

		_descr = msg->descr;
		return true;
	}

/*
=================================================
	_GetGLBufferID
=================================================
*/
	bool GL4Buffer::_GetGLBufferID (const Message< GpuMsg::GetGLBufferID > &msg)
	{
		msg->result.Set( _bufferId );
		return true;
	}
	
/*
=================================================
	_OnMemoryBindingChanged
=================================================
*/
	bool GL4Buffer::_OnMemoryBindingChanged (const Message< GpuMsg::OnMemoryBindingChanged > &msg)
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
	bool GL4Buffer::_IsCreated () const
	{
		return _bufferId != 0;
	}
	
/*
=================================================
	_CreateBuffer
=================================================
*/
	bool GL4Buffer::_CreateBuffer ()
	{
		using namespace gl;

		CHECK_ERR( not _IsCreated() );
		CHECK_ERR( not _isBindedToMemory );

		// create buffer
		GL_CALL( glGenBuffers( 1, &_bufferId ) );
		CHECK_ERR( _bufferId != 0 );

		GL_CALL( glBindBuffer( _defaultTarget, _bufferId ) );
		//GL_CALL( glBufferData( _defaultTarget, (GLsizeiptr) _descr.size, null, GL4Enum( _descr.usage ) ) );
		GL_CALL( glBufferStorage( _defaultTarget, GLsizeiptr(_descr.size), null, GL4Enum( _memFlags, _memAccess ) ) );
		GL_CALL( glBindBuffer( _defaultTarget, 0 ) );

		GetDevice()->SetObjectName( _bufferId, GetDebugName(), EGpuObject::Buffer );
		return true;
	}
	
/*
=================================================
	_DestroyAll
=================================================
*/
	void GL4Buffer::_DestroyAll ()
	{
		using namespace gl;
		
		if ( _bufferId != 0 )
		{
			GL_CALL( glDeleteBuffers( 1, &_bufferId ) );
		}

		_OnMemoryUnbinded();

		_bufferId	= 0;
		_descr		= Uninitialized;
	}
	
/*
=================================================
	_OnMemoryUnbinded
=================================================
*/
	void GL4Buffer::_OnMemoryUnbinded ()
	{
		if ( _memObj )
		{
			this->UnsubscribeAll( _memObj );
			_memObj->UnsubscribeAll( this );
			_memObj = null;
		}
		_isBindedToMemory = false;
	}

/*
=================================================
	_GpuMemoryRegionChanged
=================================================
*/
	bool GL4Buffer::_GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &)
	{
		// request buffer memory barrier
		TODO( "" );
		return false;
	}

}	// PlatformGL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenGLObjectsConstructor::CreateGL4Buffer (GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci)
	{
		return New< PlatformGL::GL4Buffer >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
