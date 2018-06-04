// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "Engine/Platforms/OpenCL/120/CL1BaseModule.h"
#include "Engine/Platforms/OpenCL/120/CL1SamplerCache.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"
#include "Engine/Platforms/Public/Tools/SamplerUtils.h"

namespace Engine
{
namespace PlatformCL
{
	
	//
	// OpencL Sampler
	//

	class CL1Sampler final : public CL1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= CL1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetSamplerDescriptor,
											GpuMsg::GetCLSamplerID
										> >;

		using SupportedEvents_t		= CL1BaseModule::SupportedEvents_t;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		SamplerDescriptor	_descr;

		cl::cl_sampler		_samplerId;


	// methods
	public:
		CL1Sampler (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci);
		~CL1Sampler ();

		SamplerDescriptor const&	GetDescriptor ()	const	{ return _descr; }


	// message handlers
	private:
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetCLSamplerID (const Message< GpuMsg::GetCLSamplerID > &);
		bool _GetSamplerDescriptor (const Message< GpuMsg::GetSamplerDescriptor > &);

	private:
		bool _IsCreated () const;
		bool _CreateSampler ();
		void _DestroySampler ();
	};
//-----------------------------------------------------------------------------



	const TypeIdList	CL1Sampler::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	CL1Sampler::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	CL1Sampler::CL1Sampler (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci) :
		CL1BaseModule( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),		_samplerId( null )
	{
		SetDebugName( "CL1Sampler" );

		_SubscribeOnMsg( this, &CL1Sampler::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &CL1Sampler::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &CL1Sampler::_AttachModule_Impl );
		_SubscribeOnMsg( this, &CL1Sampler::_DetachModule_Impl );
		_SubscribeOnMsg( this, &CL1Sampler::_FindModule_Impl );
		_SubscribeOnMsg( this, &CL1Sampler::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &CL1Sampler::_Link_Impl );
		_SubscribeOnMsg( this, &CL1Sampler::_Compose );
		_SubscribeOnMsg( this, &CL1Sampler::_Delete );
		_SubscribeOnMsg( this, &CL1Sampler::_OnManagerChanged );
		_SubscribeOnMsg( this, &CL1Sampler::_GetCLSamplerID );
		_SubscribeOnMsg( this, &CL1Sampler::_GetSamplerDescriptor );
		_SubscribeOnMsg( this, &CL1Sampler::_GetDeviceInfo );
		_SubscribeOnMsg( this, &CL1Sampler::_GetCLDeviceInfo );
		_SubscribeOnMsg( this, &CL1Sampler::_GetCLPrivateClasses );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	CL1Sampler::~CL1Sampler ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool CL1Sampler::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreateSampler() );

		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );

		CHECK( _SetState( EState::ComposedImmutable ) );
		
		_SendUncheckedEvent< ModuleMsg::AfterCompose >({});
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool CL1Sampler::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroySampler();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetCLSamplerID
=================================================
*/
	bool CL1Sampler::_GetCLSamplerID (const Message< GpuMsg::GetCLSamplerID > &msg)
	{
		ASSERT( _IsCreated() );

		msg->result.Set( _samplerId );
		return true;
	}

/*
=================================================
	_GetSamplerDescriptor
=================================================
*/
	bool CL1Sampler::_GetSamplerDescriptor (const Message< GpuMsg::GetSamplerDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_IsCreated
=================================================
*/
	bool CL1Sampler::_IsCreated () const
	{
		return _samplerId != 0;
	}
	
/*
=================================================
	_CreateSampler
=================================================
*/
	bool CL1Sampler::_CreateSampler ()
	{
		using namespace cl;

		CHECK_ERR( not _IsCreated() );

		CHECK_ERR( _descr.AddressMode().x == _descr.AddressMode().y and _descr.AddressMode().y == _descr.AddressMode().z );
		CHECK_ERR( _descr.CompareOp() == ECompareFunc::None );
		CHECK_ERR( EFilter::GetAnisotropic( _descr.Filter() ) <= 1);

		cl_bool				norm		= CL_TRUE;
		cl_addressing_mode	addressing	= CL1Enum( _descr.AddressMode().x );
		cl_filter_mode		filter		= CL1Enum( _descr.Filter() );

		cl_int	cl_err;
		CL_CHECK(( (_samplerId = clCreateSampler( GetContext(), norm, addressing, filter, &cl_err )), cl_err ));

		return true;
	}
	
/*
=================================================
	_DestroySampler
=================================================
*/
	void CL1Sampler::_DestroySampler ()
	{
		using namespace cl;
		
		if ( _samplerId != 0 ) {
			CL_CALL( clReleaseSampler( _samplerId ) );
		}

		_samplerId	= null;
		_descr		= Uninitialized;
	}
//-----------------------------------------------------------------------------
	

	
/*
=================================================
	SearchableSampler
=================================================
*/
	inline bool CL1SamplerCache::SearchableSampler::operator == (const SearchableSampler &right) const	{ return samp->GetDescriptor() == right.samp->GetDescriptor(); }
	inline bool CL1SamplerCache::SearchableSampler::operator >  (const SearchableSampler &right) const	{ return samp->GetDescriptor() >  right.samp->GetDescriptor(); }
		
//-----------------------------------------------------------------------------


/*
=================================================
	SamplerSearch
=================================================
*/	
	inline bool CL1SamplerCache::SamplerSearch::operator == (const SearchableSampler &right) const	{ return descr == right.samp->GetDescriptor(); }
	inline bool CL1SamplerCache::SamplerSearch::operator >  (const SearchableSampler &right) const	{ return descr >  right.samp->GetDescriptor(); }
	
//-----------------------------------------------------------------------------


/*
=================================================
	constructor
=================================================
*/
	CL1SamplerCache::CL1SamplerCache ()
	{
		_samplers.Reserve( 16 );
	}

/*
=================================================
	Create
=================================================
*/
	CL1SamplerCache::CL1SamplerPtr  CL1SamplerCache::Create (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci)
	{
		SamplerDescriptor	descr = ci.descr;
		PlatformTools::SamplerUtils::ValidateDescriptor( INOUT descr, 0 );

		// find cached sampler
		Samplers_t::const_iterator	iter;

		if ( _samplers.CustomSearch().Find( SamplerSearch( descr ), OUT iter ) and
			 iter->samp->GetState() == Module::EState::ComposedImmutable )
		{
			return iter->samp;
		}

		// create new sampler
		CreateInfo::GpuSampler	create_info;
		create_info.gpuThread	= ci.gpuThread;
		create_info.descr		= descr;

		auto result = New< CL1Sampler >( id, gs, create_info );

		ModuleUtils::Initialize({ result });

		CHECK_ERR( result->GetState() == Module::EState::ComposedImmutable );

		_samplers.Add( SearchableSampler( result ) );
		return result;
	}

}	// PlatformCL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenCLObjectsConstructor::CreateCL1Sampler (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci)
	{
		ModulePtr	mod;
		CHECK_ERR( mod = gs->parallelThread->GetModuleByMsg< CompileTime::TypeListFrom<Message<GpuMsg::GetCLPrivateClasses>> >() );

		Message< GpuMsg::GetCLPrivateClasses >	req_classes;
		mod->Send( req_classes );
		CHECK_ERR( req_classes->result.IsDefined() and req_classes->result->samplerCache );

		return req_classes->result->samplerCache->Create( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL