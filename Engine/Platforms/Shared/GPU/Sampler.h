// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/SamplerEnums.h"
#include "Engine/Platforms/Shared/GPU/RenderStateEnums.h"
#include "Engine/Platforms/Shared/GPU/IDs.h"

namespace Engine
{
namespace Platforms
{

	//
	// Sampler Descriptor
	//

	struct SamplerDescriptor final : CompileTime::PODStruct
	{
	// types
	public:
		using AddressMode_t	= Vec< EAddressMode::type, 3 >;
		using Self			= SamplerDescriptor;

		struct Builder;


	// variables
	private:
		AddressMode_t				_addressMode;
		EFilter::type				_filter;
		ESamplerBorderColor::bits	_borderColor;
		ECompareFunc::type			_compareOp;		// TODO: check: Some implementations will default to shader state if this member does not match
		//float						_mipLodBias;
		//float2					_lodRange;
		HashResult					_hash;


	// methods
	public:
		SamplerDescriptor (GX_DEFCTOR);

		SamplerDescriptor (EAddressMode::type addressMode,
						   EFilter::type filter,
						   ESamplerBorderColor::bits borderColor = Uninitialized,
						   //float mipLodBias = 0.0f,
						   //const float2 &lodRange = float2(),
						   ECompareFunc::type compareOp = ECompareFunc::None);

		bool operator == (const Self &right) const;
		bool operator >  (const Self &right) const;
		bool operator <  (const Self &right) const;

		AddressMode_t const&		AddressMode ()	const	{ return _addressMode; }
		EFilter::type				Filter ()		const	{ return _filter; }
		ESamplerBorderColor::bits	BorderColor ()	const	{ return _borderColor; }
		ECompareFunc::type			CompareOp ()	const	{ return _compareOp; }

		DEBUG_ONLY( String ToString () const );
	};
	


	//
	// Sampler Descriptor Builder
	//

	struct SamplerDescriptor::Builder final
	{
	// variables
	private:
		SamplerDescriptor	_state;
		bool				_changed	= true;


	// methods
	public:
		Builder () {}

		explicit Builder (const SamplerDescriptor &state) : _state(state) {}

		Builder& SetAddressMode (EAddressMode::type u, EAddressMode::type v, EAddressMode::type w);
		Builder& SetAddressMode (EAddressMode::type uvw);
		Builder& SetAddressMode (uint index, EAddressMode::type value);
		Builder& SetAddressMode (const AddressMode_t &uvw);

		Builder& SetFilter (EFilter::type value);

		Builder& SetBorderColor (ESamplerBorderColor::bits value);

		Builder& SetCompareOp (ECompareFunc::type value);
		
		AddressMode_t const&		AddressMode ()	const	{ return _state._addressMode; }
		EFilter::type				Filter ()		const	{ return _state._filter; }
		ESamplerBorderColor::bits	BorderColor ()	const	{ return _state._borderColor; }
		ECompareFunc::type			CompareOp ()	const	{ return _state._compareOp; }

		// TODO
		//void ValidateAnisotropy (uint maxAnisotropy);
		//void ValidateCoords (bool unnormSupported);
		//void ValidateBorder ();
		//void ValidateLod ();
		//void ValidateAddressMode ();
		//void ValidateFilter ();

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
		ModulePtr						gpuThread;			// can be null
		Platforms::SamplerDescriptor	descr;
	};

}	// CreateInfo


namespace GpuMsg
{
	//
	// Get Sampler Descriptor
	//
	struct GetSamplerDescriptor
	{
		Out< Platforms::SamplerDescriptor >		result;
	};


}	// GpuMsg
}	// Engine
