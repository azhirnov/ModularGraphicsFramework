// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_OPENGL

#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/OpenGL/450/GL4BaseModule.h"
#include "Engine/Platforms/OpenGL/450/GL4ResourceCache.h"
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
											GpuMsg::GetPipelineLayoutDescription,
											GpuMsg::GetGLPipelineLayoutPushConstants
										>;

		using SupportedMessages_t	= GL4BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GLPipelineResourceTableApply,
											GpuMsg::PipelineAttachBuffer,
											GpuMsg::PipelineAttachImage,
											GpuMsg::PipelineAttachTexture
										> >;

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
			BytesU		offset;
			BytesU		size;
		};

		struct ImageAttachment
		{
			ImageViewDescription	descr;
			EImageLayout::type		layout	= Uninitialized;
		};

		using AttachmentInfo_t		= Union< BufferAttachment, ImageAttachment >;
		using AttachmentInfoMap_t	= Map< UntypedKey<ModulePtr>, AttachmentInfo_t >;

		struct _CreateResourceDescription_Func;
		struct _ApplyDescriptions_Func;


	// constants
	private:
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
		bool _Link (const ModuleMsg::Link &);
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);
		bool _AttachModule (const ModuleMsg::AttachModule &);
		bool _DetachModule (const ModuleMsg::DetachModule &);
		bool _PipelineAttachImage (const GpuMsg::PipelineAttachImage &);
		bool _PipelineAttachBuffer (const GpuMsg::PipelineAttachBuffer &);
		bool _PipelineAttachTexture (const GpuMsg::PipelineAttachTexture &);
		bool _GLPipelineResourceTableApply (const GpuMsg::GLPipelineResourceTableApply &);

	private:
		bool _CreateResourceTable ();
		void _DestroyResourceTable ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	GL4PipelineResourceTable::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4PipelineResourceTable::GL4PipelineResourceTable (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci) :
		GL4BaseModule( gs, ModuleConfig{ id, UMax, true }, &_eventTypes )
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
		
		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

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
	bool GL4PipelineResourceTable::_Link (const ModuleMsg::Link &)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( _IsInitialState( GetState() ) );

		CHECK_ATTACHMENT( _layout = GetModuleByMsg< LayoutMsgList_t >() );
		CHECK_ERR( _CopySubscriptions< LayoutMsgList_t >( _layout ) );
		
		CHECK( _SetState( EState::Linked ) );
		
		_SendUncheckedEvent( ModuleMsg::AfterLink{} );
		return true;
	}

/*
=================================================
	_Compose
=================================================
*/
	bool GL4PipelineResourceTable::_Compose (const ModuleMsg::Compose &msg)
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
		
		_SendUncheckedEvent( ModuleMsg::AfterCompose{} );
		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool GL4PipelineResourceTable::_Delete (const ModuleMsg::Delete &msg)
	{
		_SendForEachAttachments( msg );

		_DestroyResourceTable();

		_attachmentInfo.Clear();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_ApplyDescriptions_Func
=================================================
*/
	struct GL4PipelineResourceTable::_ApplyDescriptions_Func
	{
	// types
		using Programs_t		= GpuMsg::GLPipelineResourceTableApply::Programs_t;
		using PushConstants_t	= GpuMsg::GLPipelineResourceTableApply::GLPushConstants;

	// variables
		Programs_t const&		programs;
		PushConstants_t const&	pushConstants;

	// methods
		_ApplyDescriptions_Func (const Programs_t &progs, const PushConstants_t &pc) :
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
	bool GL4PipelineResourceTable::_GLPipelineResourceTableApply (const GpuMsg::GLPipelineResourceTableApply &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );

		_ApplyDescriptions_Func	func( msg.programs, msg.pushConstants );

		FOR( i, _resources ) {
			_resources[i].Accept( func );
		}
		return true;
	}

/*
=================================================
	_AttachModule
=================================================
*/
	bool GL4PipelineResourceTable::_AttachModule (const ModuleMsg::AttachModule &msg)
	{
		CHECK_ERR( msg.newModule );

		// pipeline layout must be unique
		bool	is_layout = msg.newModule->SupportsAllMessages< LayoutMsgList_t >();

		CHECK( _Attach( msg.name, msg.newModule ) );
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
	bool GL4PipelineResourceTable::_PipelineAttachImage (const GpuMsg::PipelineAttachImage &msg)
	{
		CHECK_ERR( msg.newModule );

		ImageAttachment	img;
		img.descr	= msg.descr;
		img.layout	= msg.layout;

		_attachmentInfo.Add( msg.newModule.RawPtr(), AttachmentInfo_t{img} );
		
		CHECK( _Attach( msg.name, msg.newModule ) );
		CHECK( _SetState( EState::Initial ) );

		return true;
	}
	
/*
=================================================
	_PipelineAttachTexture
=================================================
*/
	bool GL4PipelineResourceTable::_PipelineAttachTexture (const GpuMsg::PipelineAttachTexture &msg)
	{
		CHECK_ERR( msg.newModule and msg.sampler );

		ImageAttachment	img;
		img.layout	= msg.layout;
		
		if ( not msg.descr.IsDefined() )
		{
			img.descr = ImageViewDescription{ msg.newModule->Request(GpuMsg::GetImageDescription{}) };
		}
		else
			img.descr = *msg.descr;

		_attachmentInfo.Add( msg.newModule.RawPtr(), AttachmentInfo_t{img} );
		
		CHECK( _Attach( msg.name, msg.newModule ) );
		CHECK( _Attach( msg.name + ".sampler", msg.sampler ) );
		CHECK( _SetState( EState::Initial ) );

		return true;
	}

/*
=================================================
	_PipelineAttachBuffer
=================================================
*/
	bool GL4PipelineResourceTable::_PipelineAttachBuffer (const GpuMsg::PipelineAttachBuffer &msg)
	{
		CHECK_ERR( msg.newModule );

		BufferAttachment	buf;
		buf.offset	= msg.offset;
		buf.size	= msg.size;

		_attachmentInfo.Add( msg.newModule.RawPtr(), AttachmentInfo_t{buf} );
		
		CHECK( _Attach( msg.name, msg.newModule ) );
		CHECK( _SetState( EState::Initial ) );

		return true;
	}

/*
=================================================
	_DetachModule
=================================================
*/
	bool GL4PipelineResourceTable::_DetachModule (const ModuleMsg::DetachModule &msg)
	{
		if ( _Detach( msg.oldModule ) )
		{
			CHECK( _SetState( EState::Initial ) );

			if ( msg.oldModule->SupportsAllMessages< LayoutMsgList_t >() )
				_DestroyResourceTable();
		}
		return true;
	}

/*
=================================================
	_CreateResourceDescription_Func
=================================================
*/
	struct GL4PipelineResourceTable::_CreateResourceDescription_Func
	{
	// types
		using TextureUniform	= PipelineLayoutDescription::TextureUniform;
		using SamplerUniform	= PipelineLayoutDescription::SamplerUniform;
		using ImageUniform		= PipelineLayoutDescription::ImageUniform;
		using UniformBuffer		= PipelineLayoutDescription::UniformBuffer;
		using StorageBuffer		= PipelineLayoutDescription::StorageBuffer;
		using PushConstant		= PipelineLayoutDescription::PushConstant;
		using SubpassInput		= PipelineLayoutDescription::SubpassInput;
		using PushConstBuffer	= PipelineLayoutDescription::PushConstantsBuffer;
		using ImageMsgList		= MessageListFrom< GpuMsg::GetGLImageID >;
		using SamplerMsgList	= MessageListFrom< GpuMsg::GetGLSamplerID >;
		using BufferMsgList		= MessageListFrom< GpuMsg::GetGLBufferID >;
		using ResourceMsgList	= CompileTime::TypeListFrom< ImageMsgList, SamplerMsgList, BufferMsgList >;
		using ModuleMap			= HashMap< ModuleName_t, ModulePtr >;


	// variables
		GL4PipelineResourceTable&	self;
		ResourceDescrArray_t&		resources;
		mutable ModuleMap			moduleMap;


	// methods
		_CreateResourceDescription_Func (OUT ResourceDescrArray_t &resources, GL4PipelineResourceTable& self) :
			self( self ), resources( resources )
		{
			moduleMap.Reserve( self._GetAttachments().Count() );

			for (const auto& mod : self._GetAttachments())
			{
				if ( mod.second->SupportsAnyMessage< ResourceMsgList >() )
					moduleMap.Add( mod.first, mod.second );
			}
		}

		~_CreateResourceDescription_Func ()
		{
			DEBUG_ONLY(
				FOR( i, moduleMap ) {
					LOG( "Unused module in resource table: "_str << moduleMap[i].first, ELog::Warning );
				}
			)
		}
		
		void operator () (const SubpassInput &) const
		{
			WARNING( "not supported" );
		}

		bool operator () (const SamplerUniform &) const
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
			ModuleMap::const_iterator	iter;
			
			if ( moduleMap.Find( name, OUT iter ) )
			{
				CHECK_ERR( iter->second->SupportsAllMessages< MsgList >() );
				result = iter->second;
				
				DEBUG_ONLY( moduleMap.EraseByIter( iter ) );
				return true;
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
			
			GpuMsg::GetGLSamplerID	req_sampler;
			samp_mod->Send( req_sampler );

			const auto&	img_res = self.GetResourceCache()->GetImageID( tex_mod );	// warning: reference may be invalid after any changes

			CHECK( img_res.Get<1>().imageType == tex.textureType );
			CHECK( img_res.Get<1>().usage[ EImageUsage::Sampled ] );
			CHECK( EPixelFormatClass::StrongComparison( tex.format, EPixelFormatClass::From( img_res.Get<1>().format ) ) );
			
			// find attachment info
			GLuint							img_view = img_res.Get<0>();
			AttachmentInfoMap_t::iterator	info;

			if ( self._attachmentInfo.Find( tex_mod.RawPtr(), OUT info ) )
			{
				img_view = tex_mod->Request(GpuMsg::CreateGLImageView{ info->second.Get<ImageAttachment>().descr });
			}
			
			// create descriptor
			TextureDescr	descr;
			descr.target		= GL4Enum( tex.textureType );
			descr.sampID		= req_sampler.result.Get(0);
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
			
			const auto&	img_res = self.GetResourceCache()->GetImageID( img_mod );	// warning: reference may be invalid after any changes

			CHECK( img_res.Get<1>().usage[ EImageUsage::Storage ] );
			
			// find attachment info
			GLuint							img_view = img_res.Get<0>();
			AttachmentInfoMap_t::iterator	info;

			if ( self._attachmentInfo.Find( img_mod.RawPtr(), OUT info ) )
			{
				img_view = img_mod->Request(GpuMsg::CreateGLImageView{ info->second.Get<ImageAttachment>().descr });
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
			
			const auto&	buf_res = self.GetResourceCache()->GetBufferID( buf_mod );	// warning: reference may be invalid after any changes

			CHECK( buf_res.Get<1>().usage[ EBufferUsage::Uniform ] );
			
			// find attachment info
			AttachmentInfoMap_t::iterator	info;
			BytesU							offset;

			if ( self._attachmentInfo.Find( buf_mod.RawPtr(), OUT info ) )
			{
				auto const&		buf_info = info->second.Get<BufferAttachment>();

				offset = buf_info.offset;

				CHECK_ERR( buf_info.size == buf.size );
			}
			else
			{
				CHECK_ERR( buf_res.Get<1>().size >= buf.size );
			}
			
			// create descriptor
			BufferDescr		descr;
			descr.bufferID		= buf_res.Get<0>();
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
			
			const auto&	buf_res = self.GetResourceCache()->GetBufferID( buf_mod );	// warning: reference may be invalid after any changes

			CHECK( buf_res.Get<1>().usage[ EBufferUsage::Storage ] );
			
			// find attachment info
			AttachmentInfoMap_t::iterator	info;
			BytesU							offset;
			BytesU							size	= buf_res.Get<1>().size;

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
				CHECK_ERR(	(buf_res.Get<1>().size >= buf.staticSize) and
							(buf.arrayStride == 0 or (buf_res.Get<1>().size - buf.staticSize) % buf.arrayStride == 0) );
			}
			
			// create descriptor
			BufferDescr		descr;
			descr.bufferID		= buf_res.Get<0>();
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
		void operator () (const PushConstant &) const
		{
			TODO("");
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
		GpuMsg::GetPipelineLayoutDescription	req_descr;
		_layout->Send( req_descr );

		const auto							uniforms = req_descr.result->GetUniforms();
		_CreateResourceDescription_Func		func( OUT _resources, *this );

		// initialize table
		FOR( i, uniforms ) {
			uniforms[i].Accept( func );
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
