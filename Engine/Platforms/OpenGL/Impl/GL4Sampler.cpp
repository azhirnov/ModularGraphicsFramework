// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/OpenGL/Impl/GL4Sampler.h"
#include "Engine/Platforms/OpenGL/OpenGLThread.h"

#if defined( GRAPHICS_API_OPENGL )

namespace Engine
{
namespace PlatformGL
{
	
/*
=================================================
	constructor
=================================================
*/
	GL4SamplerCache::GL4SamplerCache (GLSystemsRef glSys) :
		_glSystems( glSys ),
		_maxAnisotropy( 0 ),
		_initialized( false )
	{
		_samplers.Reserve( 16 );
	}
	
/*
=================================================
	_Initialize
=================================================
*/
	void GL4SamplerCache::_Initialize ()
	{
		using namespace gl;

		_initialized = true;

		GL_CALL( glGetIntegerv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, (GLint*) &_maxAnisotropy ) );
	}

/*
=================================================
	Create
=================================================
*/
	GL4SamplerCache::GL4SamplerPtr  GL4SamplerCache::Create (const GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci)
	{
		if ( not _initialized )
		{
			_Initialize();
		}

		SamplerDescrBuilder	builder( ci.descr );

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
										EFilter::_MIN_NEAREST | EFilter::_MIN_NEAREST) );
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

					builder.SetAddressMode( i, EAddressMode::ClampUnnorm );
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

		auto result = New< GL4Sampler >( gs, create_info );

		result->Send( Message< ModuleMsg::Link >() );
		result->Send( Message< ModuleMsg::Compose >() );

		CHECK_ERR( result->GetState() == Module::EState::ComposedImmutable );

		_samplers.Add( SearchableSampler( result ) );
		return result;
	}
	
//-----------------------------------------------------------------------------


	
	const Runtime::VirtualTypeList	GL4Sampler::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	GL4Sampler::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4Sampler::GL4Sampler (const GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci) :
		GL4BaseModule( gs, ci.gpuThread, ModuleConfig{ GetStaticID(), ~0u }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),
		_samplerId( 0 )
	{
		SetDebugName( "GL4Sampler" );

		_SubscribeOnMsg( this, &GL4Sampler::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &GL4Sampler::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &GL4Sampler::_AttachModule_Impl );
		_SubscribeOnMsg( this, &GL4Sampler::_DetachModule_Impl );
		_SubscribeOnMsg( this, &GL4Sampler::_FindModule_Impl );
		_SubscribeOnMsg( this, &GL4Sampler::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &GL4Sampler::_Link_Impl );
		_SubscribeOnMsg( this, &GL4Sampler::_Compose );
		_SubscribeOnMsg( this, &GL4Sampler::_OnManagerChanged );
		_SubscribeOnMsg( this, &GL4Sampler::_GpuDeviceBeforeDestory );
		_SubscribeOnMsg( this, &GL4Sampler::_GetGLSamplerID );
		_SubscribeOnMsg( this, &GL4Sampler::_GetGpuSamplerDescriptor );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, OpenGLThread::GetStaticID(), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	GL4Sampler::~GL4Sampler ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool GL4Sampler::_Compose (const  Message< ModuleMsg::Compose > &msg)
	{
		CHECK_ERR( GetState() == EState::Linked );

		CHECK_ERR( _CreateSampler() );

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
	bool GL4Sampler::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroySampler();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetGLSamplerID
=================================================
*/
	bool GL4Sampler::_GetGLSamplerID (const Message< ModuleMsg::GetGLSamplerID > &msg)
	{
		msg->result.Set( _samplerId );
		return true;
	}

/*
=================================================
	_GetGpuSamplerDescriptor
=================================================
*/
	bool GL4Sampler::_GetGpuSamplerDescriptor (const Message< ModuleMsg::GetGpuSamplerDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_IsCreated
=================================================
*/
	bool GL4Sampler::_IsCreated () const
	{
		return _samplerId != 0;
	}
	
/*
=================================================
	_CreateSampler
=================================================
*/
	bool GL4Sampler::_CreateSampler ()
	{
		using namespace gl;

		CHECK_ERR( not _IsCreated() );
		/*
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

			info.anisotropyEnable	= EnumEq( _descr.Filter(), EFilter::_ANISOTROPIC );
			info.maxAnisotropy		= 1.0;	// TODO
		}

		info.unnormalizedCoordinates	= unnorm_coords;

		VK_CHECK( vkCreateSampler( GetLogicalDevice(), &info, null, OUT &_samplerId ) );*/
		return true;
	}
	
/*
=================================================
	_DestroySampler
=================================================
*/
	void GL4Sampler::_DestroySampler ()
	{
		using namespace gl;



		_samplerId	= 0;
		_descr		= Uninitialized;
	}
	
/*
=================================================
	_DestroyResources
=================================================
*/
	void GL4Sampler::_DestroyResources ()
	{
		_DestroySampler();
	}


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
