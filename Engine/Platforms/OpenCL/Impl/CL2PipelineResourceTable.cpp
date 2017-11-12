// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Image.h"
#include "Engine/Platforms/Shared/GPU/Buffer.h"
#include "Engine/Platforms/Shared/GPU/Pipeline.h"
#include "Engine/Platforms/Shared/GPU/Uniforms.h"
#include "Engine/Platforms/OpenCL/Impl/CL2BaseModule.h"
#include "Engine/Platforms/OpenCL/OpenCLContext.h"

#if defined( COMPUTE_API_OPENCL )

namespace Engine
{
namespace PlatformCL
{

	//
	// OpenCL Pipeline Resource Table (program kernel arguments)
	//

	class CL2PipelineResourceTable final : public CL2BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= CL2BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetPipelineLayoutDescriptor,
											GpuMsg::CLPipelineResourceTableApply
										> >;

		using SupportedEvents_t		= CL2BaseModule::SupportedEvents_t;

		using LayoutMsgList_t		= MessageListFrom< GpuMsg::GetPipelineLayoutDescriptor >;
		
		struct BaseArg {
			uint		index	= ~0u;
			ModulePtr	module;
		};

		struct MemObjectArg : BaseArg {
			cl::cl_mem	id		= null;
		};

		struct UniformArg : BaseArg {
			UniformsDescriptor::Value_t	value;
			BytesU						sizeOf;
		};

		using ResourceDescr_t		= Union< MemObjectArg, UniformArg >;
		using ResourceDescrArray_t	= Array< ResourceDescr_t >;

		struct _CreateResourceDescriptor_Func;
		struct _WriteDescriptor_Func;


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
		CL2PipelineResourceTable (GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci);
		~CL2PipelineResourceTable ();


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetPipelineLayoutDescriptor (const Message< GpuMsg::GetPipelineLayoutDescriptor > &);
		bool _CLPipelineResourceTableApply (const Message< GpuMsg::CLPipelineResourceTableApply > &);
		bool _AttachModule (const Message< ModuleMsg::AttachModule > &);
		bool _DetachModule (const Message< ModuleMsg::DetachModule > &);

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
	CL2PipelineResourceTable::CL2PipelineResourceTable (GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci) :
		CL2BaseModule( gs, ModuleConfig{ CLPipelineResourceTableModuleID, ~0u }, &_msgTypes, &_eventTypes )
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
		_SubscribeOnMsg( this, &CL2PipelineResourceTable::_DeviceBeforeDestroy );
		_SubscribeOnMsg( this, &CL2PipelineResourceTable::_GetDeviceInfo );
		_SubscribeOnMsg( this, &CL2PipelineResourceTable::_GetCLDeviceInfo );
		_SubscribeOnMsg( this, &CL2PipelineResourceTable::_GetCLPrivateClasses );
		_SubscribeOnMsg( this, &CL2PipelineResourceTable::_GetPipelineLayoutDescriptor );
		_SubscribeOnMsg( this, &CL2PipelineResourceTable::_CLPipelineResourceTableApply );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, Platforms::CLThreadModuleID, true );
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
		
		CHECK( _SetState( EState::Linked ) );
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

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetPipelineLayoutDescriptor
=================================================
*/
	bool CL2PipelineResourceTable::_GetPipelineLayoutDescriptor (const Message< GpuMsg::GetPipelineLayoutDescriptor > &msg)
	{
		return _layout ? SendTo( _layout, msg ) : false;
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
	_DetachModule
=================================================
*/
	bool CL2PipelineResourceTable::_DetachModule (const Message< ModuleMsg::DetachModule > &msg)
	{
		if ( _Detach( msg->oldModule ) )
		{
			CHECK( _SetState( EState::Initial ) );
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
		using Uniform			= PipelineLayoutDescriptor::Uniform;
		using ImageMsgList		= MessageListFrom< GpuMsg::GetCLImageID >;
		using BufferMsgList		= MessageListFrom< GpuMsg::GetCLBufferID >;
		using UniformsMsgList	= MessageListFrom< GpuMsg::GetUniformsDescriptor >;


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

			Message< GpuMsg::GetCLImageID >	req_image;
			self.SendTo( img_mod, req_image );	// TODO: check result

			// TODO: check format

			MemObjectArg	descr;
			descr.index		= img.binding;
			descr.id		<< req_image->result;
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

			Message< GpuMsg::GetCLBufferID >		req_buffer;
			Message< GpuMsg::GetBufferDescriptor >	req_descr;

			// TODO: check result
			self.SendTo( buf_mod, req_buffer );
			self.SendTo( buf_mod, req_descr );

			CHECK( req_descr->result.Get().size == buf.size );
			
			MemObjectArg	descr;
			descr.index		= buf.binding;
			descr.id		<< req_buffer->result;
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

			Message< GpuMsg::GetCLBufferID >		req_buffer;
			Message< GpuMsg::GetBufferDescriptor >	req_descr;

			// TODO: check result
			self.SendTo( buf_mod, req_buffer );
			self.SendTo( buf_mod, req_descr );

			BufferDescriptor	buf_descr;	buf_descr << req_descr->result;

			CHECK( (buf_descr.size >= buf.staticSize) and
					(buf.arrayStride == 0 or
					(buf_descr.size - buf.staticSize) % buf.arrayStride == 0) );
			
			MemObjectArg	descr;
			descr.index		= buf.binding;
			descr.id		<< req_buffer->result;
			descr.module	= buf_mod;

			resources.PushBack(ResourceDescr_t( descr ));
			return true;
		}
		
/*
=================================================
	operator (Uniform)
=================================================
*/
		bool operator () (const Uniform &un) const
		{
			FOR( i, self._GetAttachments() )
			{
				const auto&	pair = self._GetAttachments()[i];

				if ( pair.second->GetSupportedMessages().HasAllTypes< UniformsMsgList >() )
				{
					Message< GpuMsg::GetUniformsDescriptor >	req_descr;
					pair.second->Send( req_descr );

					UniformsDescriptor::ValueMap_t::const_iterator	iter;

					if ( not req_descr->result->values.Find( StringCRef(un.name), OUT iter ) )
						continue;

					UniformArg		descr;
					descr.index		= un.binding;
					descr.module	= pair.second;
					descr.value		= iter->second;
					descr.sizeOf	= iter->second.GetSizeOf();

					resources.PushBack(ResourceDescr_t( descr ));
					return true;
				}
			}

			RETURN_ERR( "uniform not found!" );
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
		const cl::cl_kernel		_kernel;

	// methods
		explicit _WriteDescriptor_Func (cl::cl_kernel id) : _kernel(id)
		{}

		void operator () (const MemObjectArg &arg) const
		{
			using namespace cl;
			CL_CALL( clSetKernelArg( _kernel, arg.index, sizeof(arg.id), &arg.id ) );
		}

		void operator () (const UniformArg &arg) const
		{
			using namespace cl;
			CL_CALL( clSetKernelArg( _kernel, arg.index, (size_t) arg.sizeOf, arg.value.GetData().ptr() ) );
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
	ModulePtr OpenCLContext::_CreateCL2PipelineResourceTable (GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci)
	{
		return New< PlatformCL::CL2PipelineResourceTable >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
