// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/OpenGL/Impl/GL4Sampler.h"
#include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"

#if defined( GRAPHICS_API_OPENGL )

namespace Engine
{
namespace PlatformGL
{
	
	//
	// OpenGL Sampler
	//

	class GL4Sampler final : public GL4BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= GL4BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetSamplerDescriptor,
											GpuMsg::GetGLSamplerID
										> >;

		using SupportedEvents_t		= GL4BaseModule::SupportedEvents_t;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		SamplerDescriptor	_descr;

		gl::GLuint			_samplerId;


	// methods
	public:
		GL4Sampler (GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci);
		~GL4Sampler ();

		SamplerDescriptor const&	GetDescriptor ()	const	{ return _descr; }


	// message handlers
	private:
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetGLSamplerID (const Message< GpuMsg::GetGLSamplerID > &);
		bool _GetSamplerDescriptor (const Message< GpuMsg::GetSamplerDescriptor > &);

	private:
		bool _IsCreated () const;
		bool _CreateSampler ();
		void _DestroySampler ();
	};
//-----------------------------------------------------------------------------



	const TypeIdList	GL4Sampler::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	GL4Sampler::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4Sampler::GL4Sampler (GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci) :
		GL4BaseModule( gs, ModuleConfig{ GLSamplerModuleID, UMax }, &_msgTypes, &_eventTypes ),
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
		_SubscribeOnMsg( this, &GL4Sampler::_Delete );
		_SubscribeOnMsg( this, &GL4Sampler::_OnManagerChanged );
		_SubscribeOnMsg( this, &GL4Sampler::_GetGLSamplerID );
		_SubscribeOnMsg( this, &GL4Sampler::_GetSamplerDescriptor );
		_SubscribeOnMsg( this, &GL4Sampler::_GetDeviceInfo );
		_SubscribeOnMsg( this, &GL4Sampler::_GetGLDeviceInfo );
		_SubscribeOnMsg( this, &GL4Sampler::_GetGLPrivateClasses );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, GLThreadModuleID, true );
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
	bool GL4Sampler::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already linked

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
	bool GL4Sampler::_GetGLSamplerID (const Message< GpuMsg::GetGLSamplerID > &msg)
	{
		msg->result.Set( _samplerId );
		return true;
	}

/*
=================================================
	_GetSamplerDescriptor
=================================================
*/
	bool GL4Sampler::_GetSamplerDescriptor (const Message< GpuMsg::GetSamplerDescriptor > &msg)
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

		// create sampler
		GL_CALL( glGenSamplers( 1, &_samplerId ) );
		CHECK_ERR( _samplerId != 0 );

		// wrap
		GL_CALL( glSamplerParameteri( _samplerId, GL_TEXTURE_WRAP_S, GL4Enum( _descr.AddressMode().x ) ) );
		GL_CALL( glSamplerParameteri( _samplerId, GL_TEXTURE_WRAP_T, GL4Enum( _descr.AddressMode().y ) ) );
		GL_CALL( glSamplerParameteri( _samplerId, GL_TEXTURE_WRAP_R, GL4Enum( _descr.AddressMode().z ) ) );
		
		// border color
		if ( _descr.BorderColor()[ ESamplerBorderColor::Int ] )
		{
			uint4	color;
			CHECK_ERR( GL4Enum( _descr.BorderColor(), OUT color ) );
			GL_CALL( glSamplerParameteriv( _samplerId, GL_TEXTURE_BORDER_COLOR, (GLint const*) color.ptr() ) );
		}
		else
		{
			float4	color;
			CHECK_ERR( GL4Enum( _descr.BorderColor(), OUT color ) );
			GL_CALL( glSamplerParameterfv( _samplerId, GL_TEXTURE_BORDER_COLOR, color.ptr() ) );
		}

		// filtering
		GL4MinFilter	min_filter;
		GL4MagFilter	mag_filter;

		CHECK_ERR( GL4Enum( _descr.Filter(), OUT min_filter, OUT mag_filter ) );

		GL_CALL( glSamplerParameteri( _samplerId, GL_TEXTURE_MIN_FILTER, min_filter ) );
		GL_CALL( glSamplerParameteri( _samplerId, GL_TEXTURE_MAG_FILTER, mag_filter ) );

		// anisotropy
		const uint	aniso = EFilter::GetAnisotropic( _descr.Filter() );

		if ( aniso > 0 )
		{
			GL_CALL( glSamplerParameteri( _samplerId, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso ) );
		}

		// compare func
		if ( _descr.CompareOp() != ECompareFunc::None )
		{
			GL_CALL( glSamplerParameteri( _samplerId, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE ) );
			GL_CALL( glSamplerParameteri( _samplerId, GL_TEXTURE_COMPARE_FUNC, GL4Enum( _descr.CompareOp() ) ) );
		}
		else
		{
			GL_CALL( glSamplerParameteri( _samplerId, GL_TEXTURE_COMPARE_MODE, GL_NONE ) );
		}
		
		GetDevice()->SetObjectName( _samplerId, GetDebugName(), EGpuObject::Sampler );
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
		
		if ( _samplerId != 0 ) {
			GL_CALL( glDeleteSamplers( 1, &_samplerId ) );
		}

		_samplerId	= 0;
		_descr		= Uninitialized;
	}
//-----------------------------------------------------------------------------
	

	
/*
=================================================
	SearchableSampler
=================================================
*/
	inline bool GL4SamplerCache::SearchableSampler::operator == (const SearchableSampler &right) const	{ return samp->GetDescriptor() == right.samp->GetDescriptor(); }
	inline bool GL4SamplerCache::SearchableSampler::operator >  (const SearchableSampler &right) const	{ return samp->GetDescriptor() >  right.samp->GetDescriptor(); }
	inline bool GL4SamplerCache::SearchableSampler::operator <  (const SearchableSampler &right) const	{ return samp->GetDescriptor() <  right.samp->GetDescriptor(); }
		
//-----------------------------------------------------------------------------


/*
=================================================
	SamplerSearch
=================================================
*/	
	inline bool GL4SamplerCache::SamplerSearch::operator == (const SearchableSampler &right) const	{ return descr == right.samp->GetDescriptor(); }
	inline bool GL4SamplerCache::SamplerSearch::operator >  (const SearchableSampler &right) const	{ return descr >  right.samp->GetDescriptor(); }
	inline bool GL4SamplerCache::SamplerSearch::operator <  (const SearchableSampler &right) const	{ return descr <  right.samp->GetDescriptor(); }
	
//-----------------------------------------------------------------------------


/*
=================================================
	constructor
=================================================
*/
	GL4SamplerCache::GL4SamplerCache () :
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
	GL4SamplerCache::GL4SamplerPtr  GL4SamplerCache::Create (GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci)
	{
		if ( not _initialized )
		{
			_Initialize();
		}

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

		auto result = New< GL4Sampler >( gs, create_info );

		ModuleUtils::Initialize({ result });

		CHECK_ERR( result->GetState() == Module::EState::ComposedImmutable );

		_samplers.Add( SearchableSampler( result ) );
		return result;
	}

}	// PlatformGL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenGLObjectsConstructor::CreateGL4Sampler (GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci)
	{
		return New< PlatformGL::GL4Sampler >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
