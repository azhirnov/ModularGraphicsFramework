// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_OPENGL

#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Memory.h"
#include "Engine/Platforms/OpenGL/450/GL4BaseModule.h"
#include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"

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

		using SupportedMessages_t	= GL4BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetBufferDescription,
											GpuMsg::SetBufferDescription,
											GpuMsg::GetGLBufferID
										> >;

		using SupportedEvents_t		= GL4BaseModule::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::SetBufferDescription
										> >;
		
		using MemoryEvents_t		= MessageListFrom< GpuMsg::OnMemoryBindingChanged >;


	// constants
	private:
		static const TypeIdList		_eventTypes;
		
		static const gl::GLenum		_defaultTarget = gl::GL_COPY_READ_BUFFER;


	// variables
	private:
		BufferDescription		_descr;
		ModulePtr				_memObj;
		gl::GLuint				_bufferId;
		
		EGpuMemory::bits		_memFlags;		// -|-- this flags is requirements for memory obj, don't use it anywhere
		EMemoryAccess::bits		_memAccess;		// -|
		bool					_useMemMngr;	// -|

		bool					_isBindedToMemory;


	// methods
	public:
		GL4Buffer (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci);
		~GL4Buffer ();


	// message handlers
	private:
		bool _Link (const ModuleMsg::Link &);
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);
		bool _AttachModule (const ModuleMsg::AttachModule &);
		bool _DetachModule (const ModuleMsg::DetachModule &);
		bool _GetGLBufferID (const GpuMsg::GetGLBufferID &);
		bool _GetBufferDescription (const GpuMsg::GetBufferDescription &);
		bool _SetBufferDescription (const GpuMsg::SetBufferDescription &);
		
	// event handlers
		bool _OnMemoryBindingChanged (const GpuMsg::OnMemoryBindingChanged &);


	private:
		bool _IsCreated () const;
		bool _CreateBuffer ();

		void _DestroyAll ();
		void _OnMemoryUnbinded ();

		static void _ValidateMemFlags (INOUT EGpuMemory::bits &flags);
		static void _ValidateDescription (INOUT BufferDescription &descr);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	GL4Buffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4Buffer::GL4Buffer (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci) :
		GL4BaseModule( gs, ModuleConfig{ id, UMax }, &_eventTypes ),
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
		_SubscribeOnMsg( this, &GL4Buffer::_GetBufferDescription );
		_SubscribeOnMsg( this, &GL4Buffer::_SetBufferDescription );
		_SubscribeOnMsg( this, &GL4Buffer::_GetDeviceInfo );
		_SubscribeOnMsg( this, &GL4Buffer::_GetGLDeviceInfo );
		_SubscribeOnMsg( this, &GL4Buffer::_GetGLPrivateClasses );
		
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
	GL4Buffer::~GL4Buffer ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool GL4Buffer::_Link (const ModuleMsg::Link &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( _IsInitialState( GetState() ) );
		
		_memObj = GetModuleByEvent< MemoryEvents_t >();

		if ( not _memObj and _useMemMngr )
		{
			ModulePtr	mem_module;
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
								GLMemoryModuleID,
								GlobalSystems(),
								CreateInfo::GpuMemory{ _memFlags, _memAccess },
								OUT mem_module ) );

			CHECK_ERR( _Attach( "mem", mem_module ) );
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
	bool GL4Buffer::_Compose (const ModuleMsg::Compose &msg)
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
	bool GL4Buffer::_Delete (const ModuleMsg::Delete &msg)
	{
		_DestroyAll();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_AttachModule
=================================================
*/
	bool GL4Buffer::_AttachModule (const ModuleMsg::AttachModule &msg)
	{
		const bool	is_mem	= msg.newModule->GetSupportedEvents().HasAllTypes< MemoryEvents_t >();

		CHECK( _Attach( msg.name, msg.newModule ) );

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
	bool GL4Buffer::_DetachModule (const ModuleMsg::DetachModule &msg)
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
	bool GL4Buffer::_GetBufferDescription (const GpuMsg::GetBufferDescription &msg)
	{
		msg.result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_SetBufferDescription
=================================================
*/
	bool GL4Buffer::_SetBufferDescription (const GpuMsg::SetBufferDescription &msg)
	{
		CHECK_ERR( GetState() == EState::Initial );

		_descr = msg.descr;
		return true;
	}

/*
=================================================
	_GetGLBufferID
=================================================
*/
	bool GL4Buffer::_GetGLBufferID (const GpuMsg::GetGLBufferID &msg)
	{
		ASSERT( _IsCreated() );

		msg.result.Set( _bufferId );
		return true;
	}
	
/*
=================================================
	_OnMemoryBindingChanged
=================================================
*/
	bool GL4Buffer::_OnMemoryBindingChanged (const GpuMsg::OnMemoryBindingChanged &msg)
	{
		CHECK_ERR( _IsComposedOrLinkedState( GetState() ) );

		using EBindingTarget = GpuMsg::OnMemoryBindingChanged::EBindingTarget;

		if (  msg.targetObject == this )
		{
			_isBindedToMemory = ( msg.newState == EBindingTarget::Buffer );

			if ( _isBindedToMemory )
			{
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
		GL_CALL( glGenBuffers( 1, OUT &_bufferId ) );
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
	_ValidateMemFlags
=================================================
*/
	void GL4Buffer::_ValidateMemFlags (INOUT EGpuMemory::bits &flags)
	{
		ASSERT( not flags[EGpuMemory::SupportAliasing] );	// not supported

		flags[EGpuMemory::SupportAliasing]	= false;
		flags[EGpuMemory::Dedicated]		= true;
	}
	
/*
=================================================
	_ValidateDescription
=================================================
*/
	void GL4Buffer::_ValidateDescription (INOUT BufferDescription &)
	{
	}

}	// PlatformGL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenGLObjectsConstructor::CreateGL4Buffer (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci)
	{
		return New< PlatformGL::GL4Buffer >( id, gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
