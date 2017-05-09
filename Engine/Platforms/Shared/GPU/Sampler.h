// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/SamplerEnums.h"
#include "Engine/Platforms/Shared/GPU/RenderStateEnums.h"

namespace Engine
{
namespace Platforms
{

	//
	// Sampler Descriptor
	//

	struct SamplerDescriptor : CompileTime::PODStruct
	{
		friend struct SamplerDescrBuilder;

	// types
	public:
		using AddressMode_t	= Vec< EAddressMode::type, 3 >;
		using Self			= SamplerDescriptor;


	// variables
	private:
		AddressMode_t				_addressMode;
		EFilter::type				_filter;
		ESamplerBorderColor::type	_borderColor;
		ECompareFunc::type			_compareOp;		// TODO: check: Some implementations will default to shader state if this member does not match
		//float						_mipLodBias;
		//float2					_lodRange;
		HashResult					_hash;


	// methods
	public:
		SamplerDescriptor (GX_DEFCTOR);

		SamplerDescriptor (EAddressMode::type addressMode,
						   EFilter::type filter,
						   ESamplerBorderColor::type borderColor = ESamplerBorderColor::Unknown,
						   //float mipLodBias = 0.0f,
						   //const float2 &lodRange = float2(),
						   ECompareFunc::type compareOp = ECompareFunc::None);

		bool operator == (const Self &right) const;
		bool operator >  (const Self &right) const;
		bool operator <  (const Self &right) const;

		AddressMode_t const&		AddressMode ()	const	{ return _addressMode; }
		EFilter::type				Filter ()		const	{ return _filter; }
		ESamplerBorderColor::type	BorderColor ()	const	{ return _borderColor; }
		ECompareFunc::type			CompareOp ()	const	{ return _compareOp; }

		DEBUG_ONLY( String ToString () const );
	};
	


	//
	// Sampler Descriptor Builder
	//

	struct SamplerDescrBuilder
	{
	// types
	public:
		using Self			= SamplerDescrBuilder;
		using AddressMode_t	= SamplerDescriptor::AddressMode_t;

		
	// variables
	private:
		SamplerDescriptor	_state;
		bool			_changed;


	// methods
	public:
		SamplerDescrBuilder () : _state(), _changed(true) {}

		explicit SamplerDescrBuilder (const SamplerDescriptor &state) : _state(state), _changed(true) {}

		Self& SetAddressMode (EAddressMode::type u, EAddressMode::type v, EAddressMode::type w);
		Self& SetAddressMode (EAddressMode::type uvw);
		Self& SetAddressMode (uint index, EAddressMode::type value);
		Self& SetAddressMode (const AddressMode_t &uvw);

		Self& SetFilter (EFilter::type value);

		Self& SetBorderColor (ESamplerBorderColor::type value);

		Self& SetCompareOp (ECompareFunc::type value);
		
		AddressMode_t const&		AddressMode ()	const	{ return _state._addressMode; }
		EFilter::type				Filter ()		const	{ return _state._filter; }
		ESamplerBorderColor::type	BorderColor ()	const	{ return _state._borderColor; }
		ECompareFunc::type			CompareOp ()	const	{ return _state._compareOp; }

		// validate, calculate hash and return
		SamplerDescriptor const& Finish ();
	};


}	// Platforms


namespace CreateInfo
{

	//
	// GPU Sampler Create Info
	//

	struct GpuSampler
	{
		ModulePtr						gpuThread;
		Platforms::SamplerDescriptor	descr;
	};

}	// CreateInfo


namespace ModuleMsg
{
	//
	// Get GPU Sampler Descriptor
	//
	struct GetGpuSamplerDescriptor
	{
		Out< Platforms::SamplerDescriptor >		result;
	};
	

	// platform-dependent
	struct GetGpuSamplerID;


}	// ModuleMsg
}	// Engine
