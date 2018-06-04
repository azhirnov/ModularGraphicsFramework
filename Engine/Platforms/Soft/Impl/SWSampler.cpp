// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_SOFT

#include "Engine/Platforms/Soft/Impl/SWBaseModule.h"
#include "Engine/Platforms/Soft/Impl/SWSamplerCache.h"
#include "Engine/Platforms/Soft/SoftRendererObjectsConstructor.h"
#include "Engine/Platforms/Public/Tools/SamplerUtils.h"

namespace Engine
{
namespace PlatformSW
{
	
	//
	// Software Sampler
	//

	class SWSampler final : public SWBaseModule
	{
	// types
	private:
		using SupportedMessages_t	= SWBaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetSamplerDescriptor
										> >;

		using SupportedEvents_t		= SWBaseModule::SupportedEvents_t;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		SamplerDescriptor	_descr;


	// methods
	public:
		SWSampler (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci);
		~SWSampler ();

		SamplerDescriptor const&	GetDescriptor ()	const	{ return _descr; }


	// message handlers
	private:
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetSamplerDescriptor (const Message< GpuMsg::GetSamplerDescriptor > &);
	};
//-----------------------------------------------------------------------------



	const TypeIdList	SWSampler::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	SWSampler::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SWSampler::SWSampler (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci) :
		SWBaseModule( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr )
	{
		SetDebugName( "SWSampler" );

		_SubscribeOnMsg( this, &SWSampler::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &SWSampler::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &SWSampler::_AttachModule_Impl );
		_SubscribeOnMsg( this, &SWSampler::_DetachModule_Impl );
		_SubscribeOnMsg( this, &SWSampler::_FindModule_Impl );
		_SubscribeOnMsg( this, &SWSampler::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &SWSampler::_Link_Impl );
		_SubscribeOnMsg( this, &SWSampler::_Compose );
		_SubscribeOnMsg( this, &SWSampler::_Delete );
		_SubscribeOnMsg( this, &SWSampler::_OnManagerChanged );
		_SubscribeOnMsg( this, &SWSampler::_GetSamplerDescriptor );
		_SubscribeOnMsg( this, &SWSampler::_GetDeviceInfo );
		_SubscribeOnMsg( this, &SWSampler::_GetSWDeviceInfo );
		_SubscribeOnMsg( this, &SWSampler::_GetSWPrivateClasses );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	SWSampler::~SWSampler ()
	{
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool SWSampler::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( GetState() == EState::Linked );

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
	bool SWSampler::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_descr = Uninitialized;

		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_GetSamplerDescriptor
=================================================
*/
	bool SWSampler::_GetSamplerDescriptor (const Message< GpuMsg::GetSamplerDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
//-----------------------------------------------------------------------------
	

	
/*
=================================================
	SearchableSampler
=================================================
*/
	inline bool SWSamplerCache::SearchableSampler::operator == (const SearchableSampler &right) const	{ return samp->GetDescriptor() == right.samp->GetDescriptor(); }
	inline bool SWSamplerCache::SearchableSampler::operator >  (const SearchableSampler &right) const	{ return samp->GetDescriptor() >  right.samp->GetDescriptor(); }
		
//-----------------------------------------------------------------------------


/*
=================================================
	SamplerSearch
=================================================
*/	
	inline bool SWSamplerCache::SamplerSearch::operator == (const SearchableSampler &right) const	{ return descr == right.samp->GetDescriptor(); }
	inline bool SWSamplerCache::SamplerSearch::operator >  (const SearchableSampler &right) const	{ return descr >  right.samp->GetDescriptor(); }
	
//-----------------------------------------------------------------------------


/*
=================================================
	constructor
=================================================
*/
	SWSamplerCache::SWSamplerCache ()
	{
		_samplers.Reserve( 16 );
	}

/*
=================================================
	Create
=================================================
*/
	SWSamplerCache::SWSamplerPtr  SWSamplerCache::Create (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci)
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

		auto result = New< SWSampler >( id, gs, create_info );

		ModuleUtils::Initialize({ result });

		CHECK_ERR( result->GetState() == Module::EState::ComposedImmutable );

		_samplers.Add( SearchableSampler( result ) );
		return result;
	}

}	// PlatformSW
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr SoftRendererObjectsConstructor::CreateSWSampler (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci)
	{
		ModulePtr	mod;
		CHECK_ERR( mod = gs->parallelThread->GetModuleByMsg< CompileTime::TypeListFrom<Message<GpuMsg::GetSWPrivateClasses>> >() );

		Message< GpuMsg::GetSWPrivateClasses >	req_classes;
		mod->Send( req_classes );
		CHECK_ERR( req_classes->result.IsDefined() and req_classes->result->samplerCache );

		return req_classes->result->samplerCache->Create( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_SOFT
