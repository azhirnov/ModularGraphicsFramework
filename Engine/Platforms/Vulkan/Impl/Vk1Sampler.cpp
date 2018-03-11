// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Vulkan/Impl/Vk1Sampler.h"
#include "Engine/Platforms/Vulkan/VulkanObjectsConstructor.h"

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
											GpuMsg::GetSamplerDescriptor,
											GpuMsg::GetVkSamplerID
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		SamplerDescriptor	_descr;

		vk::VkSampler		_samplerId;


	// methods
	public:
		Vk1Sampler (GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci);
		~Vk1Sampler ();

		SamplerDescriptor const&	GetDescriptor ()	const	{ return _descr; }


	// message handlers
	private:
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetVkSamplerID (const Message< GpuMsg::GetVkSamplerID > &);
		bool _GetSamplerDescriptor (const Message< GpuMsg::GetSamplerDescriptor > &);

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
	Vk1Sampler::Vk1Sampler (GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci) :
		Vk1BaseModule( gs, ModuleConfig{ VkSamplerModuleID, UMax }, &_msgTypes, &_eventTypes ),
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
		_SubscribeOnMsg( this, &Vk1Sampler::_GetSamplerDescriptor );
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
	bool Vk1Sampler::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreateSampler() );

		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );

		CHECK( _SetState( EState::ComposedImmutable ) );
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool Vk1Sampler::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroySampler();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetVkSamplerID
=================================================
*/
	bool Vk1Sampler::_GetVkSamplerID (const Message< GpuMsg::GetVkSamplerID > &msg)
	{
		msg->result.Set( _samplerId );
		return true;
	}

/*
=================================================
	_GetSamplerDescriptor
=================================================
*/
	bool Vk1Sampler::_GetSamplerDescriptor (const Message< GpuMsg::GetSamplerDescriptor > &msg)
	{
		msg->result.Set( _descr );
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
	inline bool Vk1SamplerCache::SearchableSampler::operator == (const SearchableSampler &right) const	{ return samp->GetDescriptor() == right.samp->GetDescriptor(); }
	inline bool Vk1SamplerCache::SearchableSampler::operator >  (const SearchableSampler &right) const	{ return samp->GetDescriptor() >  right.samp->GetDescriptor(); }
	inline bool Vk1SamplerCache::SearchableSampler::operator <  (const SearchableSampler &right) const	{ return samp->GetDescriptor() <  right.samp->GetDescriptor(); }
//-----------------------------------------------------------------------------
	

/*
=================================================
	SamplerSearch
=================================================
*/		
	inline bool Vk1SamplerCache::SamplerSearch::operator == (const SearchableSampler &right) const	{ return descr == right.samp->GetDescriptor(); }
	inline bool Vk1SamplerCache::SamplerSearch::operator >  (const SearchableSampler &right) const	{ return descr >  right.samp->GetDescriptor(); }
	inline bool Vk1SamplerCache::SamplerSearch::operator <  (const SearchableSampler &right) const	{ return descr <  right.samp->GetDescriptor(); }
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
	Vk1SamplerCache::Vk1SamplerPtr  Vk1SamplerCache::Create (GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci)
	{
		SamplerDescriptor::Builder	builder( ci.descr );

		const bool		unnorm_coords = (builder.AddressMode().x == EAddressMode::ClampUnnorm) or
										(builder.AddressMode().y == EAddressMode::ClampUnnorm);

		// validate filtering mode
		{
			// validate anisotropic filtering
			if ( EnumEq( builder.Filter(), EFilter::_ANISOTROPIC ) )
			{
				builder.SetFilter( EFilter::type( (builder.Filter() ^ EFilter::_A_FACTOR_MASK) |
												Clamp(	uint(builder.Filter() & EFilter::_A_FACTOR_MASK),
														uint(SupportAnisotropyFiltering()) * 2,
														GetMaxAnisotropyLevel() ) ) );
			}

			// validate for unnormalized coords
			if ( unnorm_coords )
			{
				ASSERT( not EFilter::IsMipmapLinear( builder.Filter() ) );
				ASSERT( EFilter::IsMinLinear( builder.Filter() ) == EFilter::IsMagLinear( builder.Filter() ) );
				ASSERT( not EnumEq( builder.Filter(), EFilter::_ANISOTROPIC ) );

				builder.SetFilter( EFilter::_MIP_NEAREST |
									(EFilter::IsMinLinear( builder.Filter() ) or EFilter::IsMagLinear( builder.Filter() ) ?
										EFilter::_MIN_LINEAR | EFilter::_MAG_LINEAR :
										EFilter::_MIN_NEAREST | EFilter::_MAG_NEAREST) );
			}
		}

		// validate addressing mode
		{
			FOR( i, builder.AddressMode() )
			{
				if ( unnorm_coords )
				{
					// unnormalized coords supports only with ClampToEdge addressing mode
					ASSERT( i == 2 or builder.AddressMode()[i] == EAddressMode::ClampUnnorm );

					builder.SetAddressMode( uint(i), EAddressMode::ClampUnnorm );
				}
			}
		}

		// validate compare mode
		{
			if ( unnorm_coords )
			{
				ASSERT( builder.CompareOp() == ECompareFunc::None );
				builder.SetCompareOp( ECompareFunc::None );
			}
		}

		// find cached sampler
		Samplers_t::const_iterator	iter;

		if ( _samplers.CustomSearch().Find( SamplerSearch( builder.Finish() ), OUT iter ) and
			 iter->samp->GetState() == Module::EState::ComposedImmutable )
		{
			return iter->samp;
		}

		// create new sampler
		CreateInfo::GpuSampler	create_info;
		create_info.gpuThread	= ci.gpuThread;
		create_info.descr		= builder.Finish();

		auto result = New< Vk1Sampler >( gs, create_info );

		ModuleUtils::Initialize( {result}, null );

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
		Message< ModuleMsg::Delete >	del_msg;

		FOR( i, _samplers ) {
			_samplers[i].samp->Send( del_msg );
		}

		_samplers.Clear();
	}

}	// PlatformVK
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr VulkanObjectsConstructor::CreateVk1Sampler (GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci)
	{
		ModulePtr	mod;
		CHECK_ERR( mod = gs->parallelThread->GetModuleByMsg< CompileTime::TypeListFrom<Message<GpuMsg::GetVkPrivateClasses>> >() );

		Message< GpuMsg::GetVkPrivateClasses >	req_cl;
		mod->Send( req_cl );
		CHECK_ERR( req_cl->result.IsDefined() and req_cl->result->samplerCache );

		return req_cl->result->samplerCache->Create( gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
