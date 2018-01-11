// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/RenderPass.h"
#include "Engine/Platforms/OpenGL/Impl/GL4BaseModule.h"
#include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"

#if defined( GRAPHICS_API_OPENGL )

namespace Engine
{
namespace PlatformGL
{
	
	//
	// OpenGL Render Pass
	//

	class GL4RenderPass final : public GL4BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= GL4BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetRenderPassDescriptor,
											GpuMsg::GetGLRenderPassID
										> >;

		using SupportedEvents_t		= GL4BaseModule::SupportedEvents_t;

		using RenderPassID_t		= GpuMsg::GetGLRenderPassID::RenderPassID;
		using EClearValue			= GpuMsg::GetGLRenderPassID::EClearValue;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		RenderPassDescriptor	_descr;
		RenderPassID_t			_id;


	// methods
	public:
		GL4RenderPass (GlobalSystemsRef gs, const CreateInfo::GpuRenderPass &ci);
		~GL4RenderPass ();


	// message handlers
	private:
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetGLRenderPassID (const Message< GpuMsg::GetGLRenderPassID > &);
		bool _GetRenderPassDescriptor (const Message< GpuMsg::GetRenderPassDescriptor > &);

	private:
		bool _CreateRenderPass ();
		bool _CreateRenderToSurfacePass ();
		bool _CreateRenderToImagePass ();

		static EClearValue _FormatToClearValue (EPixelFormat::type fmt);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	GL4RenderPass::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	GL4RenderPass::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4RenderPass::GL4RenderPass (GlobalSystemsRef gs, const CreateInfo::GpuRenderPass &ci) :
		GL4BaseModule( gs, ModuleConfig{ GLRenderPassModuleID, UMax }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr )
	{
		SetDebugName( "GL4RenderPass" );

		_SubscribeOnMsg( this, &GL4RenderPass::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &GL4RenderPass::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &GL4RenderPass::_AttachModule_Impl );
		_SubscribeOnMsg( this, &GL4RenderPass::_DetachModule_Impl );
		_SubscribeOnMsg( this, &GL4RenderPass::_FindModule_Impl );
		_SubscribeOnMsg( this, &GL4RenderPass::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &GL4RenderPass::_Link_Impl );
		_SubscribeOnMsg( this, &GL4RenderPass::_Compose );
		_SubscribeOnMsg( this, &GL4RenderPass::_Delete );
		_SubscribeOnMsg( this, &GL4RenderPass::_OnManagerChanged );
		_SubscribeOnMsg( this, &GL4RenderPass::_GetGLRenderPassID );
		_SubscribeOnMsg( this, &GL4RenderPass::_GetRenderPassDescriptor );
		_SubscribeOnMsg( this, &GL4RenderPass::_GetDeviceInfo );
		_SubscribeOnMsg( this, &GL4RenderPass::_GetGLDeviceInfo );
		_SubscribeOnMsg( this, &GL4RenderPass::_GetGLPrivateClasses );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, GLThreadModuleID, true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	GL4RenderPass::~GL4RenderPass ()
	{
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool GL4RenderPass::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );
		
		CHECK_ERR( _CreateRenderPass() );

		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );

		CHECK( _SetState( EState::ComposedImmutable ) );
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool GL4RenderPass::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_descr	= Uninitialized;
		_id		= Uninitialized;

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetGLRenderPassID
=================================================
*/
	bool GL4RenderPass::_GetGLRenderPassID (const Message< GpuMsg::GetGLRenderPassID > &msg)
	{
		msg->result.Set( _id );
		return true;
	}

/*
=================================================
	_GetRenderPassDescriptor
=================================================
*/
	bool GL4RenderPass::_GetRenderPassDescriptor (const Message< GpuMsg::GetRenderPassDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_CreateRenderPass
=================================================
*/
	bool GL4RenderPass::_CreateRenderPass ()
	{
		_id = Uninitialized;
		
		if ( not _descr.ColorAttachments().Empty() and
			 EImageLayout::IsPresent( _descr.ColorAttachments().Front().finalLayout ) )
		{
			return _CreateRenderToSurfacePass();
		}

		return _CreateRenderToImagePass();
	}
	
/*
=================================================
	_CreateRenderToSurfacePass
=================================================
*/
	bool GL4RenderPass::_CreateRenderToSurfacePass ()
	{
		switch ( _descr.ColorAttachments().Count() )
		{
			case 1 : {
				_id.draw.colorBuffers.PushBack( gl::GL_BACK_LEFT );
				break;
			}
			case 2 : {
				TODO("test it");
				_id.draw.colorBuffers.PushBack( gl::GL_BACK_LEFT );
				_id.draw.colorBuffers.PushBack( gl::GL_BACK_RIGHT );
				break;
			}
			default :
				RETURN_ERR( "unknown color attachment config for render to surface pass" );
		}
		
		FOR( i, _descr.ColorAttachments() )
		{
			const auto&	col = _descr.ColorAttachments()[i];
			ASSERT( EImageLayout::IsPresent( col.finalLayout ) );

			switch ( col.loadOp ) {
				case EAttachmentLoadOp::Clear :			_id.clear.PushBack({ _id.draw.colorBuffers[i], _FormatToClearValue( col.format ) });	break;
				case EAttachmentLoadOp::Invalidate :	_id.invalidateBefore.PushBack( _id.draw.colorBuffers[i] );								break;
			}
			if ( col.storeOp == EAttachmentStoreOp::Invalidate )
				_id.invalidateAfter.PushBack( _id.draw.colorBuffers[i] );
		}

		if ( _descr.DepthStencilAttachment().IsEnabled() )
		{
			// write to buffer
			_id.draw.depthBuffer	= (_descr.DepthStencilAttachment().storeOp == EAttachmentStoreOp::Store);
			_id.draw.stencilBuffer	= (_descr.DepthStencilAttachment().stencilStoreOp == EAttachmentStoreOp::Store);

			// clear
			bool	clear_depth		= (_descr.DepthStencilAttachment().loadOp == EAttachmentLoadOp::Clear);
			bool	clear_stencil	= (_descr.DepthStencilAttachment().stencilLoadOp == EAttachmentLoadOp::Clear);

			if ( clear_depth and clear_stencil )
				_id.clear.PushBack({ gl::GL_DEPTH_STENCIL, EClearValue::FloatInt });
			else
			if ( clear_depth )
				_id.clear.PushBack({ gl::GL_DEPTH, EClearValue::Float });
			else
			if ( clear_stencil )
				_id.clear.PushBack({ gl::GL_STENCIL, EClearValue::Int });

			// before rendering
			if ( _descr.DepthStencilAttachment().loadOp == EAttachmentLoadOp::Invalidate )
				_id.invalidateBefore.PushBack( gl::GL_DEPTH );
			
			if ( _descr.DepthStencilAttachment().stencilLoadOp == EAttachmentLoadOp::Invalidate )
				_id.invalidateBefore.PushBack( gl::GL_STENCIL );
			
			// after rendering
			if ( _descr.DepthStencilAttachment().storeOp == EAttachmentStoreOp::Invalidate )
				_id.invalidateAfter.PushBack( gl::GL_DEPTH );
			
			if ( _descr.DepthStencilAttachment().stencilStoreOp == EAttachmentStoreOp::Invalidate )
				_id.invalidateAfter.PushBack( gl::GL_STENCIL );
		}
		return true;
	}
	
/*
=================================================
	_CreateRenderToImagePass
=================================================
*/
	bool GL4RenderPass::_CreateRenderToImagePass ()
	{
		FOR( i, _descr.ColorAttachments() )
		{
			const auto&	col = _descr.ColorAttachments()[i];
			ASSERT( not EImageLayout::IsPresent( col.finalLayout ) );

			_id.draw.colorBuffers.PushBack( gl::GL_COLOR_ATTACHMENT0 + uint(i) );

			switch ( col.loadOp ) {
				case EAttachmentLoadOp::Clear :			_id.clear.PushBack({ gl::GL_COLOR_ATTACHMENT0 + uint(i), _FormatToClearValue( col.format ) });	break;
				case EAttachmentLoadOp::Invalidate :	_id.invalidateBefore.PushBack( gl::GL_COLOR_ATTACHMENT0 + uint(i) );							break;
			}
			if ( col.storeOp == EAttachmentStoreOp::Invalidate )
				_id.invalidateAfter.PushBack( gl::GL_COLOR_ATTACHMENT0 + uint(i) );
		}

		if ( _descr.DepthStencilAttachment().IsEnabled() )
		{
			_id.draw.depthBuffer		= (_descr.DepthStencilAttachment().storeOp == EAttachmentStoreOp::Store);
			_id.draw.stencilBuffer		= (_descr.DepthStencilAttachment().stencilStoreOp == EAttachmentStoreOp::Store);
			
			bool	clear_depth			= (_descr.DepthStencilAttachment().loadOp == EAttachmentLoadOp::Clear);
			bool	clear_stencil		= (_descr.DepthStencilAttachment().stencilLoadOp == EAttachmentLoadOp::Clear);

			bool	depth_inv_before	= (_descr.DepthStencilAttachment().loadOp == EAttachmentLoadOp::Invalidate);
			bool	depth_inv_after		= (_descr.DepthStencilAttachment().storeOp == EAttachmentStoreOp::Invalidate);
			
			bool	stencil_inv_before	= (_descr.DepthStencilAttachment().stencilLoadOp == EAttachmentLoadOp::Invalidate);
			bool	stencil_inv_after	= (_descr.DepthStencilAttachment().stencilStoreOp == EAttachmentStoreOp::Invalidate);

			// clear
			if ( clear_depth and clear_stencil )
				_id.clear.PushBack({ gl::GL_DEPTH_STENCIL, EClearValue::FloatInt });
			else
			if ( clear_depth )
				_id.clear.PushBack({ gl::GL_DEPTH, EClearValue::Float });
			else
			if ( clear_stencil )
				_id.clear.PushBack({ gl::GL_STENCIL, EClearValue::Int });

			// before rendering
			if ( depth_inv_before and stencil_inv_before )
				_id.invalidateBefore.PushBack( gl::GL_DEPTH_STENCIL_ATTACHMENT );
			else
			if ( depth_inv_before )
				_id.invalidateBefore.PushBack( gl::GL_DEPTH_ATTACHMENT );
			else
			if ( stencil_inv_before )
				_id.invalidateBefore.PushBack( gl::GL_STENCIL_ATTACHMENT );
			
			// after remdering
			if ( depth_inv_after and stencil_inv_after )
				_id.invalidateAfter.PushBack( gl::GL_DEPTH_STENCIL_ATTACHMENT );
			else
			if ( depth_inv_after )
				_id.invalidateAfter.PushBack( gl::GL_DEPTH_ATTACHMENT );
			else
			if ( stencil_inv_after )
				_id.invalidateAfter.PushBack( gl::GL_STENCIL_ATTACHMENT );
		}
		return true;
	}
	
/*
=================================================
	_FormatToClearValue
=================================================
*/
	GL4RenderPass::EClearValue GL4RenderPass::_FormatToClearValue (EPixelFormat::type fmt)
	{
		if ( fmt == EPixelFormat::Depth16_Stencil8 or
			 fmt == EPixelFormat::Depth24_Stencil8 or
			 fmt == EPixelFormat::Depth32F_Stencil8 )
		{
			return EClearValue::FloatInt;
		}

		if ( EPixelFormat::IsFloat( fmt ) or EPixelFormat::IsIntNorm( fmt ) )
			return EClearValue::Float;

		if ( EPixelFormat::IsInt( fmt ) )
			return EClearValue::Int;

		if ( EPixelFormat::IsUInt( fmt ) )
			return EClearValue::UInt;

		RETURN_ERR( "unknown format", EClearValue(-1) );
	}

}	// PlatformGL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenGLObjectsConstructor::CreateGL4RenderPass (GlobalSystemsRef gs, const CreateInfo::GpuRenderPass &ci)
	{
		return New< PlatformGL::GL4RenderPass >( gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
