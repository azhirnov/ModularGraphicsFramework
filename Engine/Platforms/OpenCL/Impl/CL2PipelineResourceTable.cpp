// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/OpenCL/Impl/CL2BaseModule.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"

namespace Engine
{
namespace PlatformCL
{
	using namespace cl;


	//
	// OpenCL Pipeline Resource Table (program kernel arguments)
	//

	class CL2PipelineResourceTable final : public CL2BaseModule
	{
	// types
	private:
		using LayoutMsgList_t		= MessageListFrom<
											GpuMsg::GetPipelineLayoutDescriptor
										>;

		using SupportedMessages_t	= CL2BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetPipelineLayoutDescriptor,
											GpuMsg::CLPipelineResourceTableApply,
											GpuMsg::PipelineAttachBuffer,
											GpuMsg::PipelineAttachImage,
											GpuMsg::PipelineAttachTexture
										> >::Append< LayoutMsgList_t >;

		using SupportedEvents_t		= CL2BaseModule::SupportedEvents_t;
		
		struct BaseArg {
			uint		index	= UMax;
			ModulePtr	module;
		};

		struct MemObjectArg final : BaseArg {
			cl_mem		id		= null;
		};

		struct PushConstantArg final : BaseArg {
			// TODO
		};

		using ResourceDescr_t		= Union< MemObjectArg, PushConstantArg >;
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
		struct _WriteDescriptor_Func;


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
		CL2PipelineResourceTable (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci);
		~CL2PipelineResourceTable ();


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
		bool _CLPipelineResourceTableApply (const Message< GpuMsg::CLPipelineResourceTableApply > &);

	private:
		bool _CreateResourceTable ();
		void _DestroyResourceTable ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	CL2PipelineResourceTable::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	CL2PipelineResourceTable::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	CL2PipelineResourceTable::CL2PipelineResourceTable (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci) :
		CL2BaseModule( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( "CL2PipelineResourceTable" );

		_SubscribeOnMsg( this, &CL2PipelineResourceTable::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &CL2PipelineResourceTable::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &CL2PipelineResourceTable::_AttachModule );
		_SubscribeOnMsg( this, &CL2PipelineResourceTable::_DetachModule );
		_SubscribeOnMsg( this, &CL2PipelineResourceTable::_FindModule_Impl );
		_SubscribeOnMsg( this, &CL2PipelineResourceTable::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &CL2PipelineResourceTable::_Link );
		_SubscribeOnMsg( this, &CL2PipelineResourceTable::_Compose );
		_SubscribeOnMsg( this, &CL2PipelineResourceTable::_Delete );
		_SubscribeOnMsg( this, &CL2PipelineResourceTable::_OnManagerChanged );
		_SubscribeOnMsg( this, &CL2PipelineResourceTable::_GetDeviceInfo );
		_SubscribeOnMsg( this, &CL2PipelineResourceTable::_GetCLDeviceInfo );
		_SubscribeOnMsg( this, &CL2PipelineResourceTable::_GetCLPrivateClasses );
		_SubscribeOnMsg( this, &CL2PipelineResourceTable::_PipelineAttachImage );
		_SubscribeOnMsg( this, &CL2PipelineResourceTable::_PipelineAttachBuffer );
		_SubscribeOnMsg( this, &CL2PipelineResourceTable::_PipelineAttachTexture );
		_SubscribeOnMsg( this, &CL2PipelineResourceTable::_CLPipelineResourceTableApply );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	CL2PipelineResourceTable::~CL2PipelineResourceTable ()
	{
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool CL2PipelineResourceTable::_Link (const Message< ModuleMsg::Link > &)
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
	bool CL2PipelineResourceTable::_Compose (const Message< ModuleMsg::Compose > &msg)
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
	bool CL2PipelineResourceTable::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_SendForEachAttachments( msg );

		_DestroyResourceTable();
		
		_attachmentInfo.Clear();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_AttachModule
=================================================
*/
	bool CL2PipelineResourceTable::_AttachModule (const Message< ModuleMsg::AttachModule > &msg)
	{
		CHECK_ERR( msg->newModule );

		// pipeline layout must be unique
		bool	is_layout = msg->newModule->GetSupportedMessages().HasAllTypes< LayoutMsgList_t >();

		CHECK( _Attach( msg->name, msg->newModule, is_layout ) );
		CHECK( _SetState( EState::Initial ) );

		_DestroyResourceTable();
		return true;
	}
	
/*
=================================================
	_PipelineAttachImage
=================================================
*/
	bool CL2PipelineResourceTable::_PipelineAttachImage (const Message< GpuMsg::PipelineAttachImage > &msg)
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
	_PipelineAttachBuffer
=================================================
*/
	bool CL2PipelineResourceTable::_PipelineAttachBuffer (const Message< GpuMsg::PipelineAttachBuffer > &msg)
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
	_PipelineAttachTexture
=================================================
*/
	bool CL2PipelineResourceTable::_PipelineAttachTexture (const Message< GpuMsg::PipelineAttachTexture > &)
	{
		RETURN_ERR( "texture is not supported in OpenCL 1.2" );
	}

/*
=================================================
	_DetachModule
=================================================
*/
	bool CL2PipelineResourceTable::_DetachModule (const Message< ModuleMsg::DetachModule > &msg)
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
	struct CL2PipelineResourceTable::_CreateResourceDescriptor_Func
	{
	// types
		using TextureUniform	= PipelineLayoutDescriptor::TextureUniform;
		using SamplerUniform	= PipelineLayoutDescriptor::SamplerUniform;
		using ImageUniform		= PipelineLayoutDescriptor::ImageUniform;
		using UniformBuffer		= PipelineLayoutDescriptor::UniformBuffer;
		using StorageBuffer		= PipelineLayoutDescriptor::StorageBuffer;
		using PushConstant		= PipelineLayoutDescriptor::PushConstant;
		using SubpassInput		= PipelineLayoutDescriptor::SubpassInput;
		using ImageMsgList		= MessageListFrom< GpuMsg::GetCLImageID >;
		using BufferMsgList		= MessageListFrom< GpuMsg::GetCLBufferID >;


	// variables
		CL2PipelineResourceTable&	self;
		ResourceDescrArray_t&		resources;


	// methods
		_CreateResourceDescriptor_Func (OUT ResourceDescrArray_t &resources, CL2PipelineResourceTable& self) :
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

		void operator () (const TextureUniform &tex) const
		{
			WARNING( "not supported" );
		}

		void operator () (const SamplerUniform &samp) const
		{
			WARNING( "not supported" );
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
	operator (ImageUniform)
=================================================
*/
		bool operator () (const ImageUniform &img) const
		{
			ModulePtr	img_mod;
			CHECK_ERR( FindModule< ImageMsgList >( img.name, OUT img_mod ) );

			Message< GpuMsg::GetCLImageID >			req_image;
			Message< GpuMsg::GetImageDescriptor >	req_img_descr;

			self.SendTo( img_mod, req_image );	// TODO: check result
			self.SendTo( img_mod, req_img_descr );

			// TODO: check format
			CHECK( req_img_descr->result->usage[ EImageUsage::Storage ] );

			MemObjectArg	descr;
			descr.index		= img.binding;
			descr.id		= req_image->result.Get( null );
			descr.module	= img_mod;

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

			Message< GpuMsg::GetBufferDescriptor >	req_descr;
			self.SendTo( buf_mod, req_descr );

			CHECK( req_descr->result.Get().size == BytesUL(buf.size) );
			CHECK( req_descr->result->usage[ EBufferUsage::Uniform ] );
			
			// find attachment info
			AttachmentInfoMap_t::iterator	info;
			cl::cl_mem						buffer_id = null;

			if ( self._attachmentInfo.Find( buf_mod.RawPtr(), OUT info ) )
			{
				auto const&		buf_info = info->second.Get<BufferAttachment>();
				CHECK_ERR( buf_info.size == BytesUL(buf.size) );

				Message< GpuMsg::CreateCLSubBuffer >	req_subbuffer{ buf_info.offset, buf_info.size };
				self.SendTo( buf_mod, req_subbuffer );

				buffer_id = req_subbuffer->result.Get( null );
			}
			else
			{
				CHECK_ERR( req_descr->result->size >= BytesUL(buf.size) );

				Message< GpuMsg::GetCLBufferID >	req_buffer;
				self.SendTo( buf_mod, req_buffer );

				buffer_id = req_buffer->result.Get( null );
			}
			
			// create descriptor
			MemObjectArg	descr;
			descr.index		= buf.binding;
			descr.id		= buffer_id;
			descr.module	= buf_mod;
			
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

			Message< GpuMsg::GetBufferDescriptor >	req_descr;
			self.SendTo( buf_mod, req_descr );
			
			CHECK( req_descr->result->usage[ EBufferUsage::Storage ] );
			
			// find attachment info
			AttachmentInfoMap_t::iterator	info;
			cl::cl_mem						buffer_id = null;

			if ( self._attachmentInfo.Find( buf_mod.RawPtr(), OUT info ) )
			{
				auto const&		buf_info = info->second.Get<BufferAttachment>();
				
				CHECK_ERR(	(buf_info.size >= buf.staticSize) and
							(buf.arrayStride == 0 or (buf_info.size - buf.staticSize) % buf.arrayStride == 0) );
				
				Message< GpuMsg::CreateCLSubBuffer >	req_subbuffer{ buf_info.offset, buf_info.size };
				self.SendTo( buf_mod, req_subbuffer );

				buffer_id = req_subbuffer->result.Get( null );
			}
			else
			{
				CHECK_ERR(	(req_descr->result->size >= buf.staticSize) and
							(buf.arrayStride == 0 or (req_descr->result->size - buf.staticSize) % buf.arrayStride == 0) );

				Message< GpuMsg::GetCLBufferID >	req_buffer;
				self.SendTo( buf_mod, req_buffer );

				buffer_id = req_buffer->result.Get( null );
			}
			
			// create descriptor
			MemObjectArg	descr;
			descr.index		= buf.binding;
			descr.id		= buffer_id;
			descr.module	= buf_mod;

			resources.PushBack(ResourceDescr_t( descr ));
			return true;
		}
	};

/*
=================================================
	_CreateResourceTable
=================================================
*/
	bool CL2PipelineResourceTable::_CreateResourceTable ()
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
	_CreateResourceTable
=================================================
*/
	void CL2PipelineResourceTable::_DestroyResourceTable ()
	{
		_layout = null;
		_resources.Clear();
	}
	
/*
=================================================
	_WriteDescriptor_Func
=================================================
*/
	struct CL2PipelineResourceTable::_WriteDescriptor_Func
	{
	// variables
		const cl_kernel		_kernel;

	// methods
		explicit _WriteDescriptor_Func (cl_kernel id) : _kernel(id)
		{}

		void operator () (const MemObjectArg &arg) const
		{
			CL_CALL( clSetKernelArg( _kernel, arg.index, sizeof(arg.id), &arg.id ) );
		}

		void operator () (const PushConstantArg &arg) const
		{
			TODO("");
			//CL_CALL( clSetKernelArg( _kernel, arg.index, (size_t) arg.sizeOf, arg.value.GetData().ptr() ) );
		}
	};

/*
=================================================
	_CLPipelineResourceTableApply
=================================================
*/
	bool CL2PipelineResourceTable::_CLPipelineResourceTableApply (const Message< GpuMsg::CLPipelineResourceTableApply > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );

		_WriteDescriptor_Func	func( msg->kernelId );

		FOR( i, _resources ) {
			_resources[i].Apply( func );
		}
		return true;
	}

}	// PlatformCL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenCLObjectsConstructor::CreateCL2PipelineResourceTable (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci)
	{
		return New< PlatformCL::CL2PipelineResourceTable >( id, gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
