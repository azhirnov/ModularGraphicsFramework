// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/SamplerEnums.h"
#include "Engine/Platforms/Public/GPU/RenderStateEnums.h"
#include "Engine/Platforms/Public/GPU/IDs.h"

namespace Engine
{
namespace Platforms
{

	//
	// Sampler Description
	//

	struct SamplerDescription final : CompileTime::PODStruct
	{
	// types
	public:
		using AddressMode_t	= Vec< EAddressMode::type, 3 >;
		using Self			= SamplerDescription;

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
		SamplerDescription (GX_DEFCTOR);

		SamplerDescription (EAddressMode::type			addressMode,
							EFilter::type				filter,
							ESamplerBorderColor::bits	borderColor = Uninitialized,
							//float						mipLodBias = 0.0f,
							//const float2 &			lodRange = float2(),
							ECompareFunc::type			compareOp = ECompareFunc::None);

		ND_ bool operator == (const Self &right) const;
		ND_ bool operator >  (const Self &right) const;

		ND_ AddressMode_t const&		AddressMode ()	const	{ return _addressMode; }
		ND_ EFilter::type				Filter ()		const	{ return _filter; }
		ND_ ESamplerBorderColor::bits	BorderColor ()	const	{ return _borderColor; }
		ND_ ECompareFunc::type			CompareOp ()	const	{ return _compareOp; }

		DEBUG_ONLY( ND_ String ToString () const );
	};
	


	//
	// Sampler Description Builder
	//

	struct SamplerDescription::Builder final
	{
	// variables
	private:
		SamplerDescription	_state;
		bool				_changed	= true;


	// methods
	public:
		Builder () {}

		explicit Builder (const SamplerDescription &state) : _state(state) {}

		Builder& SetAddressMode (EAddressMode::type u, EAddressMode::type v, EAddressMode::type w);
		Builder& SetAddressMode (EAddressMode::type uvw);
		Builder& SetAddressMode (uint index, EAddressMode::type value);
		Builder& SetAddressMode (const AddressMode_t &uvw);

		Builder& SetFilter (EFilter::type value);

		Builder& SetBorderColor (ESamplerBorderColor::bits value);

		Builder& SetCompareOp (ECompareFunc::type value);
		
		ND_ AddressMode_t const&		AddressMode ()	const	{ return _state._addressMode; }
		ND_ EFilter::type				Filter ()		const	{ return _state._filter; }
		ND_ ESamplerBorderColor::bits	BorderColor ()	const	{ return _state._borderColor; }
		ND_ ECompareFunc::type			CompareOp ()	const	{ return _state._compareOp; }

		// TODO
		//void ValidateAnisotropy (uint maxAnisotropy);
		//void ValidateCoords (bool unnormSupported);
		//void ValidateBorder ();
		//void ValidateLod ();
		//void ValidateAddressMode ();
		//void ValidateFilter ();

		// validate, calculate hash and return
		ND_ SamplerDescription const& Finish ();
	};

}	// Platforms


namespace CreateInfo
{

	//
	// GPU Sampler Create Info
	//

	struct GpuSampler
	{
	// variables
		ModulePtr						gpuThread;			// can be null
		Platforms::SamplerDescription	descr;

	// methods
		GpuSampler () {}
		explicit GpuSampler (const Platforms::SamplerDescription &descr) : descr{descr} {}
	};

}	// CreateInfo


namespace GpuMsg
{
	//
	// Get Sampler Description
	//
	struct GetSamplerDescription : _MessageBase_
	{
		Out< Platforms::SamplerDescription >		result;
	};


}	// GpuMsg
}	// Engine
