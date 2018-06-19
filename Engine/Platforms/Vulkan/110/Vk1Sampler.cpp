// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Vulkan/110/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/110/Vk1SamplerCache.h"
#include "Engine/Platforms/Vulkan/VulkanObjectsConstructor.h"
#include "Engine/Platforms/Public/Tools/SamplerUtils.h"

namespace Engine
{
namespace PlatformVK
{
	
	//
	// Vulkan Sampler
	//

	class Vk1Sampler final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetSamplerDescription,
											GpuMsg::GetVkSamplerID
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		SamplerDescription	_descr;

		vk::VkSampler		_samplerId;


	// methods
	public:
		Vk1Sampler (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci);
		~Vk1Sampler ();

		SamplerDescription const&	GetDescription ()	const	{ return _descr; }


	// message handlers
	private:
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);
		bool _GetVkSamplerID (const GpuMsg::GetVkSamplerID &);
		bool _GetSamplerDescription (const GpuMsg::GetSamplerDescription &);

	private:
		bool _IsCreated () const;
		bool _CreateSampler ();
		void _DestroySampler ();
	};
//-----------------------------------------------------------------------------



	const TypeIdList	Vk1Sampler::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	Vk1Sampler::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1Sampler::Vk1Sampler (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci) :
		Vk1BaseModule( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),
		_samplerId( VK_NULL_HANDLE )
	{
		SetDebugName( "Vk1Sampler" );

		_SubscribeOnMsg( this, &Vk1Sampler::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1Sampler::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1Sampler::_AttachModule_Impl );
		_SubscribeOnMsg( this, &Vk1Sampler::_DetachModule_Impl );
		_SubscribeOnMsg( this, &Vk1Sampler::_FindModule_Impl );
		_SubscribeOnMsg( this, &Vk1Sampler::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Vk1Sampler::_Link_Impl );
		_SubscribeOnMsg( this, &Vk1Sampler::_Compose );
		_SubscribeOnMsg( this, &Vk1Sampler::_Delete );
		_SubscribeOnMsg( this, &Vk1Sampler::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1Sampler::_GetVkSamplerID );
		_SubscribeOnMsg( this, &Vk1Sampler::_GetSamplerDescription );
		_SubscribeOnMsg( this, &Vk1Sampler::_GetDeviceInfo );
		_SubscribeOnMsg( this, &Vk1Sampler::_GetVkDeviceInfo );
		_SubscribeOnMsg( this, &Vk1Sampler::_GetVkPrivateClasses );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Vk1Sampler::~Vk1Sampler ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool Vk1Sampler::_Compose (const ModuleMsg::Compose &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreateSampler() );

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
	bool Vk1Sampler::_Delete (const ModuleMsg::Delete &msg)
	{
		_DestroySampler();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetVkSamplerID
=================================================
*/
	bool Vk1Sampler::_GetVkSamplerID (const GpuMsg::GetVkSamplerID &msg)
	{
		ASSERT( _IsCreated() );

		msg.result.Set( _samplerId );
		return true;
	}

/*
=================================================
	_GetSamplerDescription
=================================================
*/
	bool Vk1Sampler::_GetSamplerDescription (const GpuMsg::GetSamplerDescription &msg)
	{
		msg.result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_IsCreated
=================================================
*/
	bool Vk1Sampler::_IsCreated () const
	{
		return _samplerId != VK_NULL_HANDLE;
	}
	
/*
=================================================
	_CreateSampler
=================================================
*/
	bool Vk1Sampler::_CreateSampler ()
	{
		using namespace vk;

		CHECK_ERR( not _IsCreated() );

		const bool	unnorm_coords = (_descr.AddressMode().x == EAddressMode::ClampUnnorm) or
									(_descr.AddressMode().y == EAddressMode::ClampUnnorm);

		VkSamplerCreateInfo	info = {};

		info.sType			= VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		info.flags			= 0;
		
		info.magFilter		= EFilter::IsMagLinear( _descr.Filter() ) ? VK_FILTER_LINEAR : VK_FILTER_NEAREST;
		info.minFilter		= EFilter::IsMinLinear( _descr.Filter() ) ? VK_FILTER_LINEAR : VK_FILTER_NEAREST;
		info.mipmapMode		= EFilter::IsMipmapLinear( _descr.Filter() ) ? VK_SAMPLER_MIPMAP_MODE_LINEAR : VK_SAMPLER_MIPMAP_MODE_NEAREST;

		info.addressModeU	= Vk1Enum( _descr.AddressMode().x );
		info.addressModeV	= Vk1Enum( _descr.AddressMode().y );
		info.addressModeW	= Vk1Enum( _descr.AddressMode().z );

		info.borderColor	= Vk1Enum( _descr.BorderColor() );

		if ( not unnorm_coords )
		{
			info.mipLodBias			= 0.0f;	// TODO
			info.minLod				= 0.0f;	// TODO
			info.maxLod				= 1024.0f;	// TODO

			info.compareEnable		= _descr.CompareOp() != ECompareFunc::None;
			info.compareOp			= info.compareEnable ? Vk1Enum( _descr.CompareOp() ) : VK_COMPARE_OP_ALWAYS;
			
			const uint	aniso = EFilter::GetAnisotropic( _descr.Filter() );

			info.anisotropyEnable	= aniso > 0;
			info.maxAnisotropy		= float(aniso);	// TODO
		}

		info.unnormalizedCoordinates	= unnorm_coords;

		VK_CHECK( vkCreateSampler( GetVkDevice(), &info, null, OUT &_samplerId ) );

		GetDevice()->SetObjectName( ReferenceCast<uint64_t>(_samplerId), GetDebugName(), EGpuObject::Sampler );
		return true;
	}
	
/*
=================================================
	_DestroySampler
=================================================
*/
	void Vk1Sampler::_DestroySampler ()
	{
		using namespace vk;

		auto	dev = GetVkDevice();

		if ( dev != VK_NULL_HANDLE and _samplerId != VK_NULL_HANDLE )
		{
			vkDestroySampler( dev, _samplerId, null );
		}

		_samplerId	= VK_NULL_HANDLE;
		_descr		= Uninitialized;
	}
//-----------------------------------------------------------------------------


	
/*
=================================================
	SearchableSampler
=================================================
*/
	inline bool Vk1SamplerCache::SearchableSampler::operator == (const SearchableSampler &right) const	{ return samp->GetDescription() == right.samp->GetDescription(); }
	inline bool Vk1SamplerCache::SearchableSampler::operator >  (const SearchableSampler &right) const	{ return samp->GetDescription() >  right.samp->GetDescription(); }
//-----------------------------------------------------------------------------
	

/*
=================================================
	SamplerSearch
=================================================
*/		
	inline bool Vk1SamplerCache::SamplerSearch::operator == (const SearchableSampler &right) const	{ return descr == right.samp->GetDescription(); }
	inline bool Vk1SamplerCache::SamplerSearch::operator >  (const SearchableSampler &right) const	{ return descr >  right.samp->GetDescription(); }
//-----------------------------------------------------------------------------


/*
=================================================
	constructor
=================================================
*/
	Vk1SamplerCache::Vk1SamplerCache (Ptr<Vk1Device> dev) :
		Vk1BaseObject( dev )
	{
		_samplers.Reserve( 16 );
	}
	
/*
=================================================
	Create
=================================================
*/
	Vk1SamplerCache::Vk1SamplerPtr  Vk1SamplerCache::Create (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci)
	{
		SamplerDescription	descr = ci.descr;
		PlatformTools::SamplerUtils::ValidateDescription( INOUT descr, GetMaxAnisotropyLevel() );

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

		auto result = New< Vk1Sampler >( id, gs, create_info );

		ModuleUtils::Initialize({ result });

		CHECK_ERR( result->GetState() == Module::EState::ComposedImmutable );

		_samplers.Add( SearchableSampler( result ) );
		return result;
	}
	
/*
=================================================
	Destroy
=================================================
*/
	void Vk1SamplerCache::Destroy ()
	{
		for (auto& sampler : _samplers) {
			sampler.samp->Send( ModuleMsg::Delete{} );
		}

		_samplers.Clear();
	}

}	// PlatformVK
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr VulkanObjectsConstructor::CreateVk1Sampler (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci)
	{
		ModulePtr	mod;
		CHECK_ERR( mod = gs->parallelThread->GetModuleByMsg< ModuleMsg::MessageListFrom< GpuMsg::GetVkPrivateClasses >>() );

		GpuMsg::GetVkPrivateClasses		req_classes;
		mod->Send( req_classes );
		CHECK_ERR( req_classes.result and req_classes.result->samplerCache );

		return req_classes.result->samplerCache->Create( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
