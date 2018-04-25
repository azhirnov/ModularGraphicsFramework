// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_OPENGL

#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/OpenGL/450/GL4BaseModule.h"
#include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"

namespace Engine
{
namespace PlatformGL
{
	using namespace gl;


	//
	// OpenGL Pipeline Resource Table (uniforms)
	//

	class GL4PipelineResourceTable final : public GL4BaseModule
	{
	// types
	private:
		using LayoutMsgList_t		= MessageListFrom<
											GpuMsg::GetPipelineLayoutDescriptor,
											GpuMsg::GetGLPipelineLayoutPushConstants
										>;

		using SupportedMessages_t	= GL4BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GLPipelineResourceTableApply,
											GpuMsg::PipelineAttachBuffer,
											GpuMsg::PipelineAttachImage,
											GpuMsg::PipelineAttachTexture
										> >::Append< LayoutMsgList_t >;

		using SupportedEvents_t		= GL4BaseModule::SupportedEvents_t;

		struct BaseDescr
		{
			uint			binding		= UMax;	// uniform location, texture unit, image unit, buffer binding
			EShader::bits	stageFlags;
		};

		struct TextureDescr final : BaseDescr
		{
			GLenum		target		= 0;
			GLuint		texID		= 0;
			GLuint		sampID		= 0;
		};

		struct ImageDescr final : BaseDescr
		{
			GLuint		imgID		= 0;
			uint		level		= 0;
			uint		layer		= UMax;
			GLenum		access		= 0;
			GLenum		format		= 0;
		};

		struct BufferDescr final : BaseDescr
		{
			GLenum		target		= 0;
			GLuint		bufferID	= 0;
			GLintptr	offset		= 0;
			GLsizeiptr	size		= 0;
		};

		using ResourceDescr_t		= Union< TextureDescr, ImageDescr, BufferDescr >;
		using ResourceDescrArray_t	= Array< ResourceDescr_t >;
		
		struct BufferAttachment
		{
			BytesUL		offset;
			BytesUL		size;
		};

		struct ImageAttachment
		{
			ImageViewDescriptor		descr;
			EImageLayout::type		layout	= Uninitialized;
		};

		using AttachmentInfo_t		= Union< BufferAttachment, ImageAttachment >;
		using AttachmentInfoMap_t	= Map< const void*, AttachmentInfo_t >;

		struct _CreateResourceDescriptor_Func;
		struct _ApplyDescriptors_Func;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		AttachmentInfoMap_t		_attachmentInfo;
		ResourceDescrArray_t	_resources;
		ModulePtr				_layout;


	// methods
	public:
		GL4PipelineResourceTable (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci);
		~GL4PipelineResourceTable ();


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _AttachModule (const Message< ModuleMsg::AttachModule > &);
		bool _DetachModule (const Message< ModuleMsg::DetachModule > &);
		bool _PipelineAttachImage (const Message< GpuMsg::PipelineAttachImage > &);
		bool _PipelineAttachBuffer (const Message< GpuMsg::PipelineAttachBuffer > &);
		bool _PipelineAttachTexture (const Message< GpuMsg::PipelineAttachTexture > &);
		bool _GLPipelineResourceTableApply (const Message< GpuMsg::GLPipelineResourceTableApply > &);

	private:
		bool _CreateResourceTable ();
		void _DestroyResourceTable ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	GL4PipelineResourceTable::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	GL4PipelineResourceTable::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4PipelineResourceTable::GL4PipelineResourceTable (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci) :
		GL4BaseModule( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( "GL4PipelineResourceTable" );

		_SubscribeOnMsg( this, &GL4PipelineResourceTable::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &GL4PipelineResourceTable::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &GL4PipelineResourceTable::_AttachModule );
		_SubscribeOnMsg( this, &GL4PipelineResourceTable::_DetachModule );
		_SubscribeOnMsg( this, &GL4PipelineResourceTable::_FindModule_Impl );
		_SubscribeOnMsg( this, &GL4PipelineResourceTable::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &GL4PipelineResourceTable::_Link );
		_SubscribeOnMsg( this, &GL4PipelineResourceTable::_Compose );
		_SubscribeOnMsg( this, &GL4PipelineResourceTable::_Delete );
		_SubscribeOnMsg( this, &GL4PipelineResourceTable::_OnManagerChanged );
		_SubscribeOnMsg( this, &GL4PipelineResourceTable::_GetDeviceInfo );
		_SubscribeOnMsg( this, &GL4PipelineResourceTable::_GetGLDeviceInfo );
		_SubscribeOnMsg( this, &GL4PipelineResourceTable::_GetGLPrivateClasses );
		_SubscribeOnMsg( this, &GL4PipelineResourceTable::_PipelineAttachImage );
		_SubscribeOnMsg( this, &GL4PipelineResourceTable::_PipelineAttachBuffer );
		_SubscribeOnMsg( this, &GL4PipelineResourceTable::_PipelineAttachTexture );
		_SubscribeOnMsg( this, &GL4PipelineResourceTable::_GLPipelineResourceTableApply );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	GL4PipelineResourceTable::~GL4PipelineResourceTable ()
	{
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool GL4PipelineResourceTable::_Link (const Message< ModuleMsg::Link > &)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( GetState() == EState::Initial or GetState() == EState::LinkingFailed );

		CHECK_ATTACHMENT( _layout = GetModuleByMsg< LayoutMsgList_t >() );
		CHECK_ERR( _CopySubscriptions< LayoutMsgList_t >( _layout ) );
		
		CHECK( _SetState( EState::Linked ) );
		
		_SendUncheckedEvent< ModuleMsg::AfterLink >({});
		return true;
	}

/*
=================================================
	_Compose
=================================================
*/
	bool GL4PipelineResourceTable::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _layout );
		CHECK_COMPOSING( _CreateResourceTable() );

		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );

		CHECK( _SetState( EState::ComposedMutable ) );
		
		_SendUncheckedEvent< ModuleMsg::AfterCompose >({});
		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool GL4PipelineResourceTable::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_SendForEachAttachments( msg );

		_DestroyResourceTable();

		_attachmentInfo.Clear();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_ApplyDescriptors_Func
=================================================
*/
	struct GL4PipelineResourceTable::_ApplyDescriptors_Func
	{
	// types
		using Programs_t		= GpuMsg::GLPipelineResourceTableApply::Programs_t;
		using PushConstants_t	= GpuMsg::GLPipelineResourceTableApply::GLPushConstants;

	// variables
		Programs_t const&		programs;
		PushConstants_t const&	pushConstants;

	// methods
		_ApplyDescriptors_Func (const Programs_t &progs, const PushConstants_t &pc) :
			programs{progs}, pushConstants{pc}
		{}

		void operator () (const TextureDescr &tex) const
		{
			GL_CALL( glBindMultiTexture( tex.binding, tex.target, tex.texID ) );
			GL_CALL( glBindSampler( tex.binding, tex.sampID ) );
		}

		void operator () (const ImageDescr &img) const
		{
			bool layered = (img.layer != UMax);
			GL_CALL( glBindImageTexture( img.binding, img.imgID, img.level, layered, layered ? img.layer : 0, img.access, img.format ) );
		}

		void operator () (const BufferDescr &buf) const
		{
			if ( buf.bufferID != UMax ) {
				GL_CALL( glBindBufferRange( buf.target, buf.binding, buf.bufferID, buf.offset, buf.size ) );
			} else {
				CHECK( pushConstants.bufferID != 0 );
				GL_CALL( glBindBufferRange( buf.target, buf.binding, pushConstants.bufferID, pushConstants.offset, pushConstants.size ) );
			}
		}
	};
	
/*
=================================================
	_GLPipelineResourceTableApply
=================================================
*/
	bool GL4PipelineResourceTable::_GLPipelineResourceTableApply (const Message< GpuMsg::GLPipelineResourceTableApply > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );

		_ApplyDescriptors_Func	func( msg->programs, msg->pushConstants );

		FOR( i, _resources ) {
			_resources[i].Apply( func );
		}
		return true;
	}

/*
=================================================
	_AttachModule
=================================================
*/
	bool GL4PipelineResourceTable::_AttachModule (const Message< ModuleMsg::AttachModule > &msg)
	{
		CHECK_ERR( msg->newModule );

		// pipeline layout must be unique
		bool	is_layout = msg->newModule->GetSupportedMessages().HasAllTypes< LayoutMsgList_t >();

		CHECK( _Attach( msg->name, msg->newModule, is_layout ) );
		CHECK( _SetState( EState::Initial ) );

		if ( is_layout )
			_DestroyResourceTable();

		return true;
	}
	
/*
=================================================
	_PipelineAttachImage
=================================================
*/
	bool GL4PipelineResourceTable::_PipelineAttachImage (const Message< GpuMsg::PipelineAttachImage > &msg)
	{
		CHECK_ERR( msg->newModule );

		ImageAttachment	img;
		img.descr	= msg->descr;
		img.layout	= msg->layout;

		_attachmentInfo.Add( msg->newModule.RawPtr(), AttachmentInfo_t{img} );
		
		CHECK( _Attach( msg->name, msg->newModule, false ) );
		CHECK( _SetState( EState::Initial ) );

		return true;
	}
	
/*
=================================================
	_PipelineAttachTexture
=================================================
*/
	bool GL4PipelineResourceTable::_PipelineAttachTexture (const Message< GpuMsg::PipelineAttachTexture > &msg)
	{
		CHECK_ERR( msg->newModule and msg->sampler );

		ImageAttachment	img;
		img.layout	= msg->layout;
		
		if ( not msg->descr.IsDefined() )
		{
			Message< GpuMsg::GetImageDescriptor >	req_descr;
			CHECK( msg->newModule->Send( req_descr ) );

			img.descr = ImageViewDescriptor{ *req_descr->result };
		}
		else
			img.descr = *msg->descr;

		_attachmentInfo.Add( msg->newModule.RawPtr(), AttachmentInfo_t{img} );
		
		CHECK( _Attach( msg->name, msg->newModule, false ) );
		CHECK( _Attach( msg->name + ".sampler", msg->sampler, false ) );
		CHECK( _SetState( EState::Initial ) );

		return true;
	}

/*
=================================================
	_PipelineAttachBuffer
=================================================
*/
	bool GL4PipelineResourceTable::_PipelineAttachBuffer (const Message< GpuMsg::PipelineAttachBuffer > &msg)
	{
		CHECK_ERR( msg->newModule );

		BufferAttachment	buf;
		buf.offset	= msg->offset;
		buf.size	= msg->size;

		_attachmentInfo.Add( msg->newModule.RawPtr(), AttachmentInfo_t{buf} );
		
		CHECK( _Attach( msg->name, msg->newModule, false ) );
		CHECK( _SetState( EState::Initial ) );

		return true;
	}

/*
=================================================
	_DetachModule
=================================================
*/
	bool GL4PipelineResourceTable::_DetachModule (const Message< ModuleMsg::DetachModule > &msg)
	{
		if ( _Detach( msg->oldModule ) )
		{
			CHECK( _SetState( EState::Initial ) );

			if ( msg->oldModule->GetSupportedMessages().HasAllTypes< LayoutMsgList_t >() )
				_DestroyResourceTable();
		}
		return true;
	}

/*
=================================================
	_CreateResourceDescriptor_Func
=================================================
*/
	struct GL4PipelineResourceTable::_CreateResourceDescriptor_Func
	{
	// types
		using TextureUniform	= PipelineLayoutDescriptor::TextureUniform;
		using SamplerUniform	= PipelineLayoutDescriptor::SamplerUniform;
		using ImageUniform		= PipelineLayoutDescriptor::ImageUniform;
		using UniformBuffer		= PipelineLayoutDescriptor::UniformBuffer;
		using StorageBuffer		= PipelineLayoutDescriptor::StorageBuffer;
		using PushConstant		= PipelineLayoutDescriptor::PushConstant;
		using SubpassInput		= PipelineLayoutDescriptor::SubpassInput;
		using PushConstBuffer	= PipelineLayoutDescriptor::PushConstantsBuffer;

		using ImageMsgList		= MessageListFrom< GpuMsg::GetGLImageID >;
		using SamplerMsgList	= MessageListFrom< GpuMsg::GetGLSamplerID >;
		using BufferMsgList		= MessageListFrom< GpuMsg::GetGLBufferID >;


	// variables
		GL4PipelineResourceTable&	self;
		ResourceDescrArray_t&		resources;


	// methods
		_CreateResourceDescriptor_Func (OUT ResourceDescrArray_t &resources, GL4PipelineResourceTable& self) :
			self( self ), resources( resources )
		{}

		~_CreateResourceDescriptor_Func ()
		{
		}
		
		void operator () (const SubpassInput &sp) const
		{
			WARNING( "not supported" );
		}

		bool operator () (const SamplerUniform &samp) const
		{
			RETURN_ERR( "unsupported!" );
		}
		
/*
=================================================
	FindModule
=================================================
*/
		template <typename MsgList>
		bool FindModule (StringCRef name, OUT ModulePtr &result) const
		{
			FOR( i, self._GetAttachments() )
			{
				const auto&	pair = self._GetAttachments()[i];

				if ( pair.first == name )
				{
					CHECK_ERR( pair.second->GetSupportedMessages().HasAllTypes< MsgList >() );
					result = pair.second;
					return true;
				}
			}
			return false;
		}

/*
=================================================
	operator (TextureUniform)
=================================================
*/
		bool operator () (const TextureUniform &tex) const
		{
			ModulePtr	tex_mod, samp_mod;
			CHECK_ERR( FindModule< ImageMsgList >( tex.name, OUT tex_mod ) );
			CHECK_ERR( FindModule< SamplerMsgList >( String(tex.name) << ".sampler", OUT samp_mod ) );

			Message< GpuMsg::GetGLSamplerID >		req_sampler;
			Message< GpuMsg::GetImageDescriptor >	req_img_descr;

			// TODO: check result
			self.SendTo( tex_mod, req_img_descr );
			self.SendTo( samp_mod, req_sampler );

			CHECK( req_img_descr->result->imageType == tex.textureType );
			CHECK( req_img_descr->result->usage[ EImageUsage::Sampled ] );
			CHECK( EPixelFormatClass::StrongComparison( tex.format, EPixelFormatClass::From( req_img_descr->result->format ) ) );
			
			// find attachment info
			GLuint							img_view = 0;
			AttachmentInfoMap_t::iterator	info;

			if ( self._attachmentInfo.Find( tex_mod.RawPtr(), OUT info ) )
			{
				Message< GpuMsg::CreateGLImageView >	req_imageview{ info->second.Get<ImageAttachment>().descr };
				self.SendTo( tex_mod, req_imageview );

				img_view = req_imageview->result.Get( 0 );
			}
			else
			{
				Message< GpuMsg::GetGLImageID >		req_image;
				self.SendTo( tex_mod, req_image );

				img_view = req_image->result.Get( 0 );
			}
			
			// create descriptor
			TextureDescr	descr;
			descr.target		= GL4Enum( tex.textureType );
			descr.sampID		= req_sampler->result.Get(0);
			descr.texID			= img_view;
			descr.binding		= tex.binding;
			descr.stageFlags	= tex.stageFlags;

			resources.PushBack(ResourceDescr_t( descr ));
			return true;
		}
		
/*
=================================================
	operator (ImageUniform)
=================================================
*/
		bool operator () (const ImageUniform &img) const
		{
			ModulePtr	img_mod;
			CHECK_ERR( FindModule< ImageMsgList >( img.name, OUT img_mod ) );

			Message< GpuMsg::GetImageDescriptor >	req_img_descr;
			self.SendTo( img_mod, req_img_descr );

			CHECK( req_img_descr->result->usage[ EImageUsage::Storage ] );
			
			// find attachment info
			GLuint							img_view = 0;
			AttachmentInfoMap_t::iterator	info;

			if ( self._attachmentInfo.Find( img_mod.RawPtr(), OUT info ) )
			{
				Message< GpuMsg::CreateGLImageView >	req_imageview{ info->second.Get<ImageAttachment>().descr };
				self.SendTo( img_mod, req_imageview );

				img_view = req_imageview->result.Get( 0 );
			}
			else
			{
				Message< GpuMsg::GetGLImageID >		req_image;
				self.SendTo( img_mod, req_image );

				img_view = req_image->result.Get( 0 );
			}

			// create descriptor
			ImageDescr			descr;
			descr.format		= GL4Enum( img.format );
			descr.level			= 0;
			descr.layer			= UMax;
			descr.imgID			= img_view;
			descr.binding		= img.binding;
			descr.stageFlags	= img.stageFlags;
			descr.access		= EShaderMemoryModel::HasReadWriteAccess( img.access ) ? GL_READ_WRITE :
								  EShaderMemoryModel::HasWriteAccess( img.access ) ? GL_WRITE_ONLY :
								  GL_READ_ONLY;

			resources.PushBack(ResourceDescr_t( descr ));
			return true;
		}
		
/*
=================================================
	operator (UniformBuffer)
=================================================
*/
		bool operator () (const UniformBuffer &buf) const
		{
			ModulePtr	buf_mod;
			CHECK_ERR( FindModule< BufferMsgList >( buf.name, OUT buf_mod ) );

			Message< GpuMsg::GetGLBufferID >		req_buffer;
			Message< GpuMsg::GetBufferDescriptor >	req_descr;

			// TODO: check result
			self.SendTo( buf_mod, req_buffer );
			self.SendTo( buf_mod, req_descr );

			CHECK( req_descr->result->usage[ EBufferUsage::Uniform ] );
			
			// find attachment info
			AttachmentInfoMap_t::iterator	info;
			BytesUL							offset;

			if ( self._attachmentInfo.Find( buf_mod.RawPtr(), OUT info ) )
			{
				auto const&		buf_info = info->second.Get<BufferAttachment>();

				offset = buf_info.offset;

				CHECK_ERR( buf_info.size == BytesUL(buf.size) );
			}
			else
			{
				CHECK_ERR( req_descr->result->size >= BytesUL(buf.size) );
			}
			
			// create descriptor
			BufferDescr		descr;
			descr.bufferID		= req_buffer->result.Get(0);
			descr.target		= GL_UNIFORM_BUFFER;
			descr.binding		= buf.binding;
			descr.stageFlags	= buf.stageFlags;
			descr.offset		= GLintptr(offset);
			descr.size			= GLsizeiptr(buf.size);

			resources.PushBack(ResourceDescr_t( descr ));
			return true;
		}
		
/*
=================================================
	operator (StorageBuffer)
=================================================
*/
		bool operator () (const StorageBuffer &buf) const
		{
			ModulePtr	buf_mod;
			CHECK_ERR( FindModule< BufferMsgList >( buf.name, OUT buf_mod ) );

			Message< GpuMsg::GetGLBufferID >		req_buffer;
			Message< GpuMsg::GetBufferDescriptor >	req_descr;

			// TODO: check result
			self.SendTo( buf_mod, req_buffer );
			self.SendTo( buf_mod, req_descr );

			CHECK( req_descr->result->usage[ EBufferUsage::Storage ] );
			
			// find attachment info
			AttachmentInfoMap_t::iterator	info;
			BytesUL							offset;
			BytesUL							size	= req_descr->result->size;

			if ( self._attachmentInfo.Find( buf_mod.RawPtr(), OUT info ) )
			{
				auto const&		buf_info = info->second.Get<BufferAttachment>();

				offset	= buf_info.offset;
				size	= buf_info.size;
				
				CHECK_ERR(	(size >= buf.staticSize) and
							(buf.arrayStride == 0 or (size - buf.staticSize) % buf.arrayStride == 0) );
			}
			else
			{
				CHECK_ERR(	(req_descr->result->size >= buf.staticSize) and
							(buf.arrayStride == 0 or (req_descr->result->size - buf.staticSize) % buf.arrayStride == 0) );
			}
			
			// create descriptor
			BufferDescr		descr;
			descr.bufferID		= req_buffer->result.Get(0);
			descr.target		= GL_SHADER_STORAGE_BUFFER;
			descr.binding		= buf.binding;
			descr.stageFlags	= buf.stageFlags;
			descr.offset		= GLintptr(offset);
			descr.size			= GLsizeiptr(size);

			resources.PushBack(ResourceDescr_t( descr ));
			return true;
		}
		
/*
=================================================
	operator (PushConstant)
=================================================
*/
		void operator () (const PushConstant &pc) const
		{
		}
		
		bool operator () (const PushConstBuffer &pcb) const
		{
			BufferDescr		descr;
			descr.bufferID		= UMax;		// special flag to indicate push constants buffer
			descr.target		= GL_UNIFORM_BUFFER;
			descr.binding		= pcb.binding;
			descr.stageFlags	= pcb.stageFlags;
			//descr.offset		= 
			//descr.size		=

			resources.PushBack(ResourceDescr_t( descr ));
			return true;
		}
	};

/*
=================================================
	_CreateResourceTable
=================================================
*/
	bool GL4PipelineResourceTable::_CreateResourceTable ()
	{
		Message< GpuMsg::GetPipelineLayoutDescriptor >	req_descr;

		SendTo( _layout, req_descr );

		_CreateResourceDescriptor_Func		func( OUT _resources, *this );

		// initialize table
		FOR( i, req_descr->result->GetUniforms() ) {
			req_descr->result->GetUniforms()[i].Apply( func );
		}

		return true;
	}

/*
=================================================
	_DestroyResourceTable
=================================================
*/
	void GL4PipelineResourceTable::_DestroyResourceTable ()
	{
		_layout = null;
		_resources.Clear();
	}

}	// PlatformGL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenGLObjectsConstructor::CreateGL4PipelineResourceTable (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci)
	{
		return New< PlatformGL::GL4PipelineResourceTable >( id, gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL