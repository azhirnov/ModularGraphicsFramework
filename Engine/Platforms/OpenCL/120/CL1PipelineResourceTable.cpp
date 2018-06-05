// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/OpenCL/120/CL1BaseModule.h"
#include "Engine/Platforms/OpenCL/120/CL1ResourceCache.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"

namespace Engine
{
namespace PlatformCL
{
	using namespace cl;


	//
	// OpenCL Pipeline Resource Table (program kernel arguments)
	//

	class CL1PipelineResourceTable final : public CL1BaseModule
	{
	// types
	private:
		using LayoutMsgList_t		= MessageListFrom<
											GpuMsg::GetPipelineLayoutDescriptor
										>;

		using SupportedMessages_t	= CL1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetPipelineLayoutDescriptor,
											GpuMsg::CLBindPipelineResourceTable,
											GpuMsg::PipelineAttachBuffer,
											GpuMsg::PipelineAttachImage,
											GpuMsg::PipelineAttachTexture
										> >::Append< LayoutMsgList_t >;

		using SupportedEvents_t		= CL1BaseModule::SupportedEvents_t;
		
		struct BaseArg {
			uint		index	= UMax;
		};

		struct MemObjectArg final : BaseArg {
			cl_mem			id		= null;
			ESharing::type	sharing	= ESharing::None;
		};
		
		struct SamplerArg final : BaseArg {
			cl_sampler		id		= null;
		};

		struct PushConstantArg final : BaseArg {
			// TODO
		};

		using ResourceDescr_t		= Union< MemObjectArg, SamplerArg, PushConstantArg >;
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
		CL1PipelineResourceTable (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci);
		~CL1PipelineResourceTable ();


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
		bool _CLBindPipelineResourceTable (const Message< GpuMsg::CLBindPipelineResourceTable > &);

	private:
		bool _CreateResourceTable ();
		void _DestroyResourceTable ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	CL1PipelineResourceTable::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	CL1PipelineResourceTable::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	CL1PipelineResourceTable::CL1PipelineResourceTable (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci) :
		CL1BaseModule( gs, ModuleConfig{ id, UMax, true }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( "CL1PipelineResourceTable" );

		_SubscribeOnMsg( this, &CL1PipelineResourceTable::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &CL1PipelineResourceTable::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &CL1PipelineResourceTable::_AttachModule );
		_SubscribeOnMsg( this, &CL1PipelineResourceTable::_DetachModule );
		_SubscribeOnMsg( this, &CL1PipelineResourceTable::_FindModule_Impl );
		_SubscribeOnMsg( this, &CL1PipelineResourceTable::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &CL1PipelineResourceTable::_Link );
		_SubscribeOnMsg( this, &CL1PipelineResourceTable::_Compose );
		_SubscribeOnMsg( this, &CL1PipelineResourceTable::_Delete );
		_SubscribeOnMsg( this, &CL1PipelineResourceTable::_OnManagerChanged );
		_SubscribeOnMsg( this, &CL1PipelineResourceTable::_GetDeviceInfo );
		_SubscribeOnMsg( this, &CL1PipelineResourceTable::_GetCLDeviceInfo );
		_SubscribeOnMsg( this, &CL1PipelineResourceTable::_GetCLPrivateClasses );
		_SubscribeOnMsg( this, &CL1PipelineResourceTable::_PipelineAttachImage );
		_SubscribeOnMsg( this, &CL1PipelineResourceTable::_PipelineAttachBuffer );
		_SubscribeOnMsg( this, &CL1PipelineResourceTable::_PipelineAttachTexture );
		_SubscribeOnMsg( this, &CL1PipelineResourceTable::_CLBindPipelineResourceTable );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	CL1PipelineResourceTable::~CL1PipelineResourceTable ()
	{
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool CL1PipelineResourceTable::_Link (const Message< ModuleMsg::Link > &)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( _IsInitialState( GetState() ) );

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
	bool CL1PipelineResourceTable::_Compose (const Message< ModuleMsg::Compose > &msg)
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
	bool CL1PipelineResourceTable::_Delete (const Message< ModuleMsg::Delete > &msg)
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
	bool CL1PipelineResourceTable::_AttachModule (const Message< ModuleMsg::AttachModule > &msg)
	{
		CHECK_ERR( msg->newModule );

		CHECK( _Attach( msg->name, msg->newModule ) );
		CHECK( _SetState( EState::Initial ) );

		_DestroyResourceTable();
		return true;
	}
	
/*
=================================================
	_PipelineAttachImage
=================================================
*/
	bool CL1PipelineResourceTable::_PipelineAttachImage (const Message< GpuMsg::PipelineAttachImage > &msg)
	{
		CHECK_ERR( msg->newModule );

		ImageAttachment	img;
		img.descr	= msg->descr;
		img.layout	= msg->layout;

		_attachmentInfo.Add( msg->newModule.RawPtr(), AttachmentInfo_t{img} );
		
		CHECK( _Attach( msg->name, msg->newModule ) );
		CHECK( _SetState( EState::Initial ) );

		return true;
	}
	
/*
=================================================
	_PipelineAttachBuffer
=================================================
*/
	bool CL1PipelineResourceTable::_PipelineAttachBuffer (const Message< GpuMsg::PipelineAttachBuffer > &msg)
	{
		CHECK_ERR( msg->newModule );

		BufferAttachment	buf;
		buf.offset	= msg->offset;
		buf.size	= msg->size;

		_attachmentInfo.Add( msg->newModule.RawPtr(), AttachmentInfo_t{buf} );
		
		CHECK( _Attach( msg->name, msg->newModule ) );
		CHECK( _SetState( EState::Initial ) );

		return true;
	}
	
/*
=================================================
	_PipelineAttachTexture
=================================================
*/
	bool CL1PipelineResourceTable::_PipelineAttachTexture (const Message< GpuMsg::PipelineAttachTexture > &msg)
	{
		CHECK_ERR( msg->newModule and msg->sampler );

		if ( msg->descr.IsDefined() )
		{
			RETURN_ERR( "image view is not supported in OpenCL 1.2" );
		}
		
		CHECK( _Attach( msg->name, msg->newModule ) );
		CHECK( _Attach( msg->name + ".sampler", msg->sampler ) );
		CHECK( _SetState( EState::Initial ) );

		return true;
	}

/*
=================================================
	_DetachModule
=================================================
*/
	bool CL1PipelineResourceTable::_DetachModule (const Message< ModuleMsg::DetachModule > &msg)
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
	struct CL1PipelineResourceTable::_CreateResourceDescriptor_Func
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
		using SamplerMsgList	= MessageListFrom< GpuMsg::GetCLSamplerID >;
		using BufferMsgList		= MessageListFrom< GpuMsg::GetCLBufferID >;
		using ResourceMsgList	= CompileTime::TypeListFrom< ImageMsgList, SamplerMsgList, BufferMsgList >;
		using ModuleMap			= HashMap< ModuleName_t, ModulePtr >;


	// variables
		CL1PipelineResourceTable&	self;
		ResourceDescrArray_t&		resources;
		mutable ModuleMap			moduleMap;
		mutable usize				maxIndex;		// used for samplers that stored with images (textures)


	// methods
		_CreateResourceDescriptor_Func (OUT ResourceDescrArray_t &resources, CL1PipelineResourceTable& self, usize maxIndex) :
			self{ self }, resources{ resources }, maxIndex{ maxIndex }
		{
			moduleMap.Reserve( self._GetAttachments().Count() );

			for (const auto& mod : self._GetAttachments())
			{
				if ( mod.second->GetSupportedMessages().HasAnyType< ResourceMsgList >() )
					moduleMap.Add( mod.first, mod.second );
			}
		}

		~_CreateResourceDescriptor_Func ()
		{
			DEBUG_ONLY(
				FOR( i, moduleMap ) {
					LOG( "Unused module in resource table: "_str << moduleMap[i].first, ELog::Warning );
				}
			)
		}
		
		void operator () (const PushConstant &) const
		{
			WARNING( "not supported" );
		}
		
		void operator () (const SubpassInput &) const
		{
			WARNING( "not supported" );
		}

		void operator () (const SamplerUniform &) const
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
			ModuleMap::const_iterator	iter;
			
			if ( moduleMap.Find( name, OUT iter ) )
			{
				CHECK_ERR( iter->second->GetSupportedMessages().HasAllTypes< MsgList >() );
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
			
			Message< GpuMsg::GetCLSamplerID >	req_sampler;
			self.SendTo( samp_mod, req_sampler );

			const auto&	img_res = self.GetResourceCache()->GetImageID( tex_mod );	// warning: reference may be invalid after any changes

			CHECK( img_res.Get<1>().imageType == tex.textureType );
			CHECK( img_res.Get<1>().usage[ EImageUsage::Sampled ] );
			CHECK( EPixelFormatClass::StrongComparison( tex.format, EPixelFormatClass::From( img_res.Get<1>().format ) ) );
			
			// find attachment info
			AttachmentInfoMap_t::iterator	info;

			if ( self._attachmentInfo.Find( tex_mod.RawPtr(), OUT info ) )
			{
				RETURN_ERR( "not supported" );
			}
			
			// create descriptor
			MemObjectArg		img_descr;
			img_descr.id		= img_res.Get<0>().id;
			img_descr.sharing	= img_res.Get<0>().sharing;
			img_descr.index		= tex.uniqueIndex;
			resources.PushBack(ResourceDescr_t( img_descr ));

			SamplerArg			samp_descr;
			samp_descr.id		= *req_sampler->result;
			samp_descr.index	= uint(maxIndex++);
			resources.PushBack(ResourceDescr_t( samp_descr ));
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

			const auto&	img_res = self.GetResourceCache()->GetImageID( img_mod );

			// TODO: check format
			CHECK( img_res.Get<1>().usage[ EImageUsage::Storage ] );

			MemObjectArg	descr;
			descr.index		= img.uniqueIndex;
			descr.id		= img_res.Get<0>().id;
			descr.sharing	= img_res.Get<0>().sharing;

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

			CHECK( buf_res.Get<1>().size == BytesUL(buf.size) );
			CHECK( buf_res.Get<1>().usage[ EBufferUsage::Uniform ] );
			
			// find attachment info
			AttachmentInfoMap_t::iterator	info;
			cl::cl_mem						buffer_id		= null;
			ESharing::type					buffer_sharing	= ESharing::None;

			if ( self._attachmentInfo.Find( buf_mod.RawPtr(), OUT info ) )
			{
				auto const&		buf_info = info->second.Get<BufferAttachment>();
				CHECK_ERR( buf_info.size == BytesUL(buf.size) );

				Message< GpuMsg::CreateCLSubBuffer >	req_subbuffer{ buf_info.offset, buf_info.size };
				self.SendTo( buf_mod, req_subbuffer );

				buffer_id		= req_subbuffer->result->id;
				buffer_sharing	= req_subbuffer->result->sharing;
			}
			else
			{
				CHECK_ERR( buf_res.Get<1>().size >= BytesUL(buf.size) );

				buffer_id		= buf_res.Get<0>().id;
				buffer_sharing	= buf_res.Get<0>().sharing;
			}
			
			// create descriptor
			MemObjectArg	descr;
			descr.index		= buf.uniqueIndex;
			descr.id		= buffer_id;
			descr.sharing	= buffer_sharing;
			
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
			cl::cl_mem						buffer_id		= null;
			ESharing::type					buffer_sharing	= ESharing::None;

			if ( self._attachmentInfo.Find( buf_mod.RawPtr(), OUT info ) )
			{
				auto const&		buf_info = info->second.Get<BufferAttachment>();
				
				CHECK_ERR(	(buf_info.size >= buf.staticSize) and
							(buf.arrayStride == 0 or (buf_info.size - buf.staticSize) % buf.arrayStride == 0) );
				
				Message< GpuMsg::CreateCLSubBuffer >	req_subbuffer{ buf_info.offset, buf_info.size };
				self.SendTo( buf_mod, req_subbuffer );

				buffer_id		= req_subbuffer->result->id;
				buffer_sharing	= req_subbuffer->result->sharing;
			}
			else
			{
				CHECK_ERR(	(buf_res.Get<1>().size >= buf.staticSize) and
							(buf.arrayStride == 0 or (buf_res.Get<1>().size - buf.staticSize) % buf.arrayStride == 0) );

				buffer_id		= buf_res.Get<0>().id;
				buffer_sharing	= buf_res.Get<0>().sharing;
			}
			
			// create descriptor
			MemObjectArg	descr;
			descr.index		= buf.uniqueIndex;
			descr.id		= buffer_id;
			descr.sharing	= buffer_sharing;

			resources.PushBack(ResourceDescr_t( descr ));
			return true;
		}
	};

/*
=================================================
	_CreateResourceTable
=================================================
*/
	bool CL1PipelineResourceTable::_CreateResourceTable ()
	{
		Message< GpuMsg::GetPipelineLayoutDescriptor >	req_descr;
		SendTo( _layout, req_descr );
		
		const auto							uniforms	= req_descr->result->GetUniforms();

		_CreateResourceDescriptor_Func		func( OUT _resources, *this, uniforms.Count() );
		
		// initialize table
		FOR( i, uniforms ) {
			uniforms[i].Apply( func );
		}
		return true;
	}

/*
=================================================
	_CreateResourceTable
=================================================
*/
	void CL1PipelineResourceTable::_DestroyResourceTable ()
	{
		_layout = null;
		_resources.Clear();
	}
	
/*
=================================================
	_WriteDescriptor_Func
=================================================
*/
	struct CL1PipelineResourceTable::_WriteDescriptor_Func
	{
	// variables
		Ptr<CL1Device>		_device;
		const ModulePtr &	_owner;
		const cl_kernel		_kernel;

	// methods
		explicit _WriteDescriptor_Func (Ptr<CL1Device> dev, const ModulePtr &owner, cl_kernel id) :
			_device{dev}, _owner{owner}, _kernel(id)
		{}

		void operator () (const MemObjectArg &arg) const
		{
			CHECK( _device->AddSharedObj( _owner, arg.id, arg.sharing ) );

			CL_CALL( clSetKernelArg( _kernel, arg.index, sizeof(arg.id), &arg.id ) );
		}
		
		void operator () (const SamplerArg &arg) const
		{
			CL_CALL( clSetKernelArg( _kernel, arg.index, sizeof(arg.id), &arg.id ) );
		}

		void operator () (const PushConstantArg &) const
		{
			TODO("");
			//CL_CALL( clSetKernelArg( _kernel, arg.index, (size_t) arg.sizeOf, arg.value.GetData().ptr() ) );
		}
	};

/*
=================================================
	_CLBindPipelineResourceTable
=================================================
*/
	bool CL1PipelineResourceTable::_CLBindPipelineResourceTable (const Message< GpuMsg::CLBindPipelineResourceTable > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );

		_WriteDescriptor_Func	func( GetDevice(), this, msg->kernelId );

		FOR( i, _resources ) {
			_resources[i].Apply( func );
		}
		return true;
	}

}	// PlatformCL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenCLObjectsConstructor::CreateCL1PipelineResourceTable (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci)
	{
		return New< PlatformCL::CL1PipelineResourceTable >( id, gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
