// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Image.h"
#include "Engine/Platforms/Shared/GPU/Buffer.h"
#include "Engine/Platforms/Shared/GPU/Pipeline.h"
#include "Engine/Platforms/OpenGL/Impl/GL4BaseModule.h"
#include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"

#if defined( GRAPHICS_API_OPENGL )

namespace Engine
{
namespace PlatformGL
{

	//
	// OpenGL Pipeline Resource Table (uniforms)
	//

	class GL4PipelineResourceTable final : public GL4BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= GL4BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetPipelineLayoutDescriptor,
											GpuMsg::GLPipelineResourceTableApply
										> >;

		using SupportedEvents_t		= GL4BaseModule::SupportedEvents_t;

		struct BaseDescr
		{
			uint			binding		= UMax;	// uniform location, texture unit, image unit, buffer binding
			EShader::bits	stageFlags;
		};

		struct TextureDescr : BaseDescr
		{
			gl::GLenum		target		= 0;
			gl::GLuint		texID		= 0;
			gl::GLuint		sampID		= 0;
		};

		struct ImageDescr : BaseDescr
		{
			gl::GLuint		imgID		= 0;
			uint			level		= 0;
			uint			layer		= UMax;
			gl::GLenum		access		= 0;
			gl::GLenum		format		= 0;
		};

		struct BufferDescr : BaseDescr
		{
			gl::GLenum		target		= 0;
			gl::GLuint		bufferID	= 0;
		};

		using LayoutMsgList_t		= MessageListFrom< GpuMsg::GetPipelineLayoutDescriptor >;

		using ResourceDescr_t		= Union< TextureDescr, ImageDescr, BufferDescr >;
		using ResourceDescrArray_t	= Array< ResourceDescr_t >;
		
		struct _CreateResourceDescriptor_Func;
		struct _ApplyDescriptors_Func;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ResourceDescrArray_t	_resources;
		ModulePtr				_layout;


	// methods
	public:
		GL4PipelineResourceTable (GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci);
		~GL4PipelineResourceTable ();


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetPipelineLayoutDescriptor (const Message< GpuMsg::GetPipelineLayoutDescriptor > &);
		bool _GLPipelineResourceTableApply (const Message< GpuMsg::GLPipelineResourceTableApply > &);
		bool _AttachModule (const Message< ModuleMsg::AttachModule > &);
		bool _DetachModule (const Message< ModuleMsg::DetachModule > &);

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
	GL4PipelineResourceTable::GL4PipelineResourceTable (GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci) :
		GL4BaseModule( gs, ModuleConfig{ GLPipelineResourceTableModuleID, UMax }, &_msgTypes, &_eventTypes )
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
		_SubscribeOnMsg( this, &GL4PipelineResourceTable::_GetPipelineLayoutDescriptor );
		_SubscribeOnMsg( this, &GL4PipelineResourceTable::_GLPipelineResourceTableApply );

		CHECK( _ValidateMsgSubscriptions() );

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
		
		CHECK( _SetState( EState::Linked ) );
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

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetPipelineLayoutDescriptor
=================================================
*/
	bool GL4PipelineResourceTable::_GetPipelineLayoutDescriptor (const Message< GpuMsg::GetPipelineLayoutDescriptor > &msg)
	{
		return _layout ? SendTo( _layout, msg ) : false;
	}
	
/*
=================================================
	_ApplyDescriptors_Func
=================================================
*/
	struct GL4PipelineResourceTable::_ApplyDescriptors_Func
	{
		using Programs_t	= GpuMsg::GLPipelineResourceTableApply::Programs_t;

		Programs_t const&	programs;


		_ApplyDescriptors_Func (const Programs_t &progs) :
			programs(progs)
		{}

		void operator () (const TextureDescr &tex) const
		{
			using namespace gl;
			GL_CALL( glBindMultiTexture( tex.binding, tex.target, tex.texID ) );
			GL_CALL( glBindSampler( tex.binding, tex.sampID ) );
		}

		void operator () (const ImageDescr &img) const
		{
			using namespace gl;
			bool layered = (img.layer != UMax);
			GL_CALL( glBindImageTexture( img.binding, img.imgID, img.level, layered, layered ? img.layer : 0, img.access, img.format ) );
		}

		void operator () (const BufferDescr &buf) const
		{
			using namespace gl;
			GL_CALL( glBindBufferBase( buf.target, buf.binding, buf.bufferID ) );
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

		_ApplyDescriptors_Func	func( msg->programs );

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
		using Uniform			= PipelineLayoutDescriptor::Uniform;
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
		
		void operator () (const PushConstant &pc) const
		{
			WARNING( "not supported" );
		}
		
		void operator () (const SubpassInput &sp) const
		{
			WARNING( "not supported" );
		}
		
		void operator () (const Uniform &un) const
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

			Message< GpuMsg::GetGLImageID >			req_image;
			Message< GpuMsg::GetGLSamplerID >		req_sampler;
			Message< GpuMsg::GetImageDescriptor >	req_img_descr;

			// TODO: check result
			self.SendTo( tex_mod, req_image );
			self.SendTo( tex_mod, req_img_descr );
			self.SendTo( samp_mod, req_sampler );

			ImageDescriptor	tex_descr;	tex_descr << req_img_descr->result;
			CHECK( tex_descr.imageType == tex.textureType );
			CHECK( EPixelFormatClass::StrongComparison( tex.format, EPixelFormatClass::From( tex_descr.format ) ) );

			TextureDescr	descr;
			descr.target		= GL4Enum( tex.textureType );
			descr.sampID		= req_sampler->result.Get(0);
			descr.texID			= req_image->result.Get(0);
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

			Message< GpuMsg::GetGLImageID >		req_image;
			self.SendTo( img_mod, req_image );	// TODO: check result

			ImageDescr		descr;
			descr.format		= GL4Enum( img.format );
			descr.level			= 0;	// TODO
			descr.layer			= UMax;
			descr.imgID			= req_image->result.Get(0);
			descr.binding		= img.binding;
			descr.stageFlags	= img.stageFlags;
			descr.access		= img.readAccess and img.writeAccess ? gl::GL_READ_WRITE :
								  img.writeAccess ? gl::GL_WRITE_ONLY :
								  gl::GL_READ_ONLY;

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

			CHECK( req_descr->result.Get().size == buf.size );
			
			BufferDescr		descr;
			descr.bufferID		= req_buffer->result.Get(0);
			descr.target		= gl::GL_UNIFORM_BUFFER;
			descr.binding		= buf.binding;
			descr.stageFlags	= buf.stageFlags;

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

			BufferDescriptor	buf_descr;	buf_descr << req_descr->result;

			CHECK( (buf_descr.size >= buf.staticSize) and
					(buf.arrayStride == 0 or
					(buf_descr.size - buf.staticSize) % buf.arrayStride == 0) );

			BufferDescr		descr;
			descr.bufferID		= req_buffer->result.Get(0);
			descr.target		= gl::GL_SHADER_STORAGE_BUFFER;
			descr.binding		= buf.binding;
			descr.stageFlags	= buf.stageFlags;

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

		PipelineLayoutDescriptor			layout_descr;	layout_descr << req_descr->result;
		_CreateResourceDescriptor_Func		func( OUT _resources, *this );

		// initialize table
		FOR( i, layout_descr.GetUniforms() ) {
			layout_descr.GetUniforms()[i].Apply( func );
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
	ModulePtr OpenGLObjectsConstructor::CreateGL4PipelineResourceTable (GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci)
	{
		return New< PlatformGL::GL4PipelineResourceTable >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
