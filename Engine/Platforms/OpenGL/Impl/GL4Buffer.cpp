// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Buffer.h"
#include "Engine/Platforms/Shared/GPU/Memory.h"
#include "Engine/Platforms/OpenGL/Impl/GL4BaseModule.h"
#include "Engine/Platforms/OpenGL/OpenGLContext.h"

#if defined( GRAPHICS_API_OPENGL )

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
		using SupportedMessages_t	= GL4BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetBufferDescriptor,
											GpuMsg::GetGLBufferID,
											GpuMsg::GpuMemoryRegionChanged,
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
										> >;

		using SupportedEvents_t		= GL4BaseModule::SupportedEvents_t;
		
		using MemoryEvents_t		= MessageListFrom< GpuMsg::OnMemoryBindingChanged >;
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
											GpuMsg::WriteToImageMemory >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		BufferDescriptor		_descr;
		ModulePtr				_memObj;
		gl::GLuint				_bufferId;
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
		bool _OnMemoryBindingChanged (const Message< GpuMsg::OnMemoryBindingChanged > &);
		bool _GpuMemoryRegionChanged (const Message< GpuMsg::GpuMemoryRegionChanged > &);

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
		GL4BaseModule( gs, ModuleConfig{ GLBufferModuleID, ~0u }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),
		_bufferId( 0 ),
		_isBindedToMemory( false )
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
		_SubscribeOnMsg( this, &GL4Buffer::_DeviceBeforeDestroy );
		_SubscribeOnMsg( this, &GL4Buffer::_GetGLBufferID );
		_SubscribeOnMsg( this, &GL4Buffer::_GetBufferDescriptor );
		_SubscribeOnMsg( this, &GL4Buffer::_GetDeviceInfo );
		_SubscribeOnMsg( this, &GL4Buffer::_GetGLDeviceInfo );
		_SubscribeOnMsg( this, &GL4Buffer::_GetGLPrivateClasses );
		_SubscribeOnMsg( this, &GL4Buffer::_GpuMemoryRegionChanged );

		_AttachSelfToManager( ci.gpuThread, Platforms::GLThreadModuleID, true );
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
		
		CHECK_ATTACHMENT( _memObj = GetModuleByEvent< MemoryEvents_t >() );

		_memObj->Subscribe( this, &GL4Buffer::_OnMemoryBindingChanged );
		
		CHECK_LINKING( _CopySubscriptions< ForwardToMem_t >( _memObj ) );

		CHECK_ERR( Module::_Link_Impl( msg ) );
		return true;
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

		GL_CALL( glBindBuffer( GL_COPY_READ_BUFFER, _bufferId ) );
		GL_CALL( glBufferData( GL_COPY_READ_BUFFER, (GLsizeiptr) _descr.size, null, GL4Enum( _descr.usage ) ) );
		GL_CALL( glBindBuffer( GL_COPY_READ_BUFFER, 0 ) );

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
	ModulePtr OpenGLContext::_CreateGL4Buffer (GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci)
	{
		return New< PlatformGL::GL4Buffer >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
