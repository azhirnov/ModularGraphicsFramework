// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

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
											GpuMsg::GetPipelineLayoutDescription
										>;

		using SupportedMessages_t	= CL1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::CLBindPipelineResourceTable,
											GpuMsg::PipelineAttachBuffer,
											GpuMsg::PipelineAttachImage,
											GpuMsg::PipelineAttachTexture
										> >;

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
		struct _WriteDescription_Func;


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
		CL1PipelineResourceTable (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci);
		~CL1PipelineResourceTable ();


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
		bool _CLBindPipelineResourceTable (const GpuMsg::CLBindPipelineResourceTable &);

	private:
		bool _CreateResourceTable ();
		void _DestroyResourceTable ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	CL1PipelineResourceTable::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	CL1PipelineResourceTable::CL1PipelineResourceTable (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci) :
		CL1BaseModule( gs, ModuleConfig{ id, UMax, true }, &_eventTypes )
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
		
		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

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
	bool CL1PipelineResourceTable::_Link (const ModuleMsg::Link &)
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
	bool CL1PipelineResourceTable::_Compose (const ModuleMsg::Compose &msg)
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
	bool CL1PipelineResourceTable::_Delete (const ModuleMsg::Delete &msg)
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
	bool CL1PipelineResourceTable::_AttachModule (const ModuleMsg::AttachModule &msg)
	{
		CHECK_ERR( msg.newModule );

		CHECK( _Attach( msg.name, msg.newModule ) );
		CHECK( _SetState( EState::Initial ) );

		_DestroyResourceTable();
		return true;
	}
	
/*
=================================================
	_PipelineAttachImage
=================================================
*/
	bool CL1PipelineResourceTable::_PipelineAttachImage (const GpuMsg::PipelineAttachImage &msg)
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
	_PipelineAttachBuffer
=================================================
*/
	bool CL1PipelineResourceTable::_PipelineAttachBuffer (const GpuMsg::PipelineAttachBuffer &msg)
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
	_PipelineAttachTexture
=================================================
*/
	bool CL1PipelineResourceTable::_PipelineAttachTexture (const GpuMsg::PipelineAttachTexture &msg)
	{
		CHECK_ERR( msg.newModule and msg.sampler );

		if ( msg.descr.IsDefined() )
		{
			RETURN_ERR( "image view is not supported in OpenCL 1.2" );
		}
		
		CHECK( _Attach( msg.name, msg.newModule ) );
		CHECK( _Attach( msg.name + ".sampler", msg.sampler ) );
		CHECK( _SetState( EState::Initial ) );

		return true;
	}

/*
=================================================
	_DetachModule
=================================================
*/
	bool CL1PipelineResourceTable::_DetachModule (const ModuleMsg::DetachModule &msg)
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
	struct CL1PipelineResourceTable::_CreateResourceDescription_Func
	{
	// types
		using TextureUniform	= PipelineLayoutDescription::TextureUniform;
		using SamplerUniform	= PipelineLayoutDescription::SamplerUniform;
		using ImageUniform		= PipelineLayoutDescription::ImageUniform;
		using UniformBuffer		= PipelineLayoutDescription::UniformBuffer;
		using StorageBuffer		= PipelineLayoutDescription::StorageBuffer;
		using PushConstant		= PipelineLayoutDescription::PushConstant;
		using SubpassInput		= PipelineLayoutDescription::SubpassInput;
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
		_CreateResourceDescription_Func (OUT ResourceDescrArray_t &resources, CL1PipelineResourceTable& self, usize maxIndex) :
			self{ self }, resources{ resources }, maxIndex{ maxIndex }
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
			
			// create description
			MemObjectArg		img_descr;
			img_descr.id		= img_res.Get<0>().id;
			img_descr.sharing	= img_res.Get<0>().sharing;
			img_descr.index		= tex.uniqueIndex;
			resources.PushBack(ResourceDescr_t( img_descr ));

			SamplerArg			samp_descr;
			samp_descr.id		= samp_mod->Request( GpuMsg::GetCLSamplerID{} );
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

			CHECK( buf_res.Get<1>().size == buf.size );
			CHECK( buf_res.Get<1>().usage[ EBufferUsage::Uniform ] );
			
			// find attachment info
			AttachmentInfoMap_t::iterator	info;
			GpuMsg::GetCLBufferID::Data		buf_id;

			if ( self._attachmentInfo.Find( buf_mod.RawPtr(), OUT info ) )
			{
				auto const&		buf_info = info->second.Get<BufferAttachment>();
				CHECK_ERR( buf_info.size == buf.size );

				buf_id = buf_mod->Request( GpuMsg::CreateCLSubBuffer{ buf_info.offset, buf_info.size } );
			}
			else
			{
				CHECK_ERR( buf_res.Get<1>().size >= buf.size );

				buf_id = buf_res.Get<0>();
			}
			
			// create description
			MemObjectArg	descr;
			descr.index		= buf.uniqueIndex;
			descr.id		= buf_id.id;
			descr.sharing	= buf_id.sharing;
			
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
			GpuMsg::GetCLBufferID::Data		buf_id;

			if ( self._attachmentInfo.Find( buf_mod.RawPtr(), OUT info ) )
			{
				auto const&		buf_info = info->second.Get<BufferAttachment>();
				
				CHECK_ERR(	(buf_info.size >= buf.staticSize) and
							(buf.arrayStride == 0 or (buf_info.size - buf.staticSize) % buf.arrayStride == 0) );
				
				buf_id = buf_mod->Request( GpuMsg::CreateCLSubBuffer{ buf_info.offset, buf_info.size } );
			}
			else
			{
				CHECK_ERR(	(buf_res.Get<1>().size >= buf.staticSize) and
							(buf.arrayStride == 0 or (buf_res.Get<1>().size - buf.staticSize) % buf.arrayStride == 0) );

				buf_id = buf_res.Get<0>();
			}
			
			// create description
			MemObjectArg	descr;
			descr.index		= buf.uniqueIndex;
			descr.id		= buf_id.id;
			descr.sharing	= buf_id.sharing;

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
		GpuMsg::GetPipelineLayoutDescription		req_descr;
		_layout->Send( req_descr );
		
		const auto						uniforms	= req_descr.result->GetUniforms();
		_CreateResourceDescription_Func	func( OUT _resources, *this, uniforms.Count() );
		
		// initialize table
		FOR( i, uniforms ) {
			uniforms[i].Accept( func );
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
	_WriteDescription_Func
=================================================
*/
	struct CL1PipelineResourceTable::_WriteDescription_Func
	{
	// variables
		Ptr<CL1Device>		_device;
		const ModulePtr &	_owner;
		const cl_kernel		_kernel;

	// methods
		explicit _WriteDescription_Func (Ptr<CL1Device> dev, const ModulePtr &owner, cl_kernel id) :
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
	bool CL1PipelineResourceTable::_CLBindPipelineResourceTable (const GpuMsg::CLBindPipelineResourceTable &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );

		_WriteDescription_Func	func( GetDevice(), this, msg.kernelId );

		FOR( i, _resources ) {
			_resources[i].Accept( func );
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
