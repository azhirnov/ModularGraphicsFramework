// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

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
											GpuMsg::GetSamplerDescription
										> >;

		using SupportedEvents_t		= SWBaseModule::SupportedEvents_t;


	// constants
	private:
		static const TypeIdList		_eventTypes;


	// variables
	private:
		SamplerDescription	_descr;


	// methods
	public:
		SWSampler (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci);
		~SWSampler ();

		SamplerDescription const&	GetDescription ()	const	{ return _descr; }


	// message handlers
	private:
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);
		bool _GetSamplerDescription (const GpuMsg::GetSamplerDescription &);
	};
//-----------------------------------------------------------------------------



	const TypeIdList	SWSampler::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SWSampler::SWSampler (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci) :
		SWBaseModule( gs, ModuleConfig{ id, UMax }, &_eventTypes ),
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
		_SubscribeOnMsg( this, &SWSampler::_GetSamplerDescription );
		_SubscribeOnMsg( this, &SWSampler::_GetDeviceInfo );
		_SubscribeOnMsg( this, &SWSampler::_GetSWDeviceInfo );
		_SubscribeOnMsg( this, &SWSampler::_GetSWPrivateClasses );

		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

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
	bool SWSampler::_Compose (const ModuleMsg::Compose &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( GetState() == EState::Linked );

		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );

		CHECK( _SetState( EState::ComposedImmutable ) );
		
		_SendUncheckedEvent( ModuleMsg::AfterCompose{} );
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool SWSampler::_Delete (const ModuleMsg::Delete &msg)
	{
		_descr = Uninitialized;

		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_GetSamplerDescription
=================================================
*/
	bool SWSampler::_GetSamplerDescription (const GpuMsg::GetSamplerDescription &msg)
	{
		msg.result.Set( _descr );
		return true;
	}
//-----------------------------------------------------------------------------
	

	
/*
=================================================
	SearchableSampler
=================================================
*/
	inline bool SWSamplerCache::SearchableSampler::operator == (const SearchableSampler &right) const	{ return samp->GetDescription() == right.samp->GetDescription(); }
	inline bool SWSamplerCache::SearchableSampler::operator >  (const SearchableSampler &right) const	{ return samp->GetDescription() >  right.samp->GetDescription(); }
		
//-----------------------------------------------------------------------------


/*
=================================================
	SamplerSearch
=================================================
*/	
	inline bool SWSamplerCache::SamplerSearch::operator == (const SearchableSampler &right) const	{ return descr == right.samp->GetDescription(); }
	inline bool SWSamplerCache::SamplerSearch::operator >  (const SearchableSampler &right) const	{ return descr >  right.samp->GetDescription(); }
	
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
		SamplerDescription	descr = ci.descr;
		PlatformTools::SamplerUtils::ValidateDescription( INOUT descr, 0 );

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
		CHECK_ERR( mod = gs->parallelThread->GetModuleByMsg< ModuleMsg::MessageListFrom< GpuMsg::GetSWPrivateClasses >>() );

		GpuMsg::GetSWPrivateClasses		req_classes;
		mod->Send( req_classes );
		CHECK_ERR( req_classes.result and req_classes.result->samplerCache );

		return req_classes.result->samplerCache->Create( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_SOFT
