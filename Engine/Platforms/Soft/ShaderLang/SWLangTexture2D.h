// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Soft/ShaderLang/SWLangTexture.h"

#ifdef GRAPHICS_API_SOFT

namespace SWShaderLang
{
namespace Impl
{

	//
	// Texture 2D
	//

	template <typename T>
	struct Texture2D final : public BaseTexture
	{
		friend class SWShaderHelper;

	// methods
	protected :
		Texture2D (MemLayout_t &&memLayout, const Sampler_t &sampler) : BaseTexture{ RVREF(memLayout), sampler, UninitializedT<T>() } {}
		
		Texture2D& operator = (const Texture2D &) = default;
		Texture2D& operator = (Texture2D &&) = default;

	public:
		Texture2D () {}

		ND_ T Sample (const float2 &point) const;
		ND_ T Sample (const float2 &point, float bias) const;
		
		ND_ T SampleOffset (const float2 &point, const int2 &offset) const;
		ND_ T SampleOffset (const float2 &point, const int2 &offset, float bias) const;

		ND_ T Fetch (const int2 &point, int lod) const;
		ND_ T FetchOffset (const int2 &point, int lod, const int2 &offset) const;

		ND_ T Gather (const float2 &point) const;
		ND_ T Gather (const float2 &point, int comp) const;

		ND_ T GatherOffset (const float2 &point, const int2 &offset) const;
		ND_ T GatherOffset (const float2 &point, const int2 &offset, int comp) const;

		ND_ T GatherOffsets (const float2 &point, const int2 &offset0, const int2 &offset1, const int2 &offset2, const int2 &offset3) const;
		ND_ T GatherOffsets (const float2 &point, const int2 &offset0, const int2 &offset1, const int2 &offset2, const int2 &offset3, int comp) const;

		ND_ T SampleGrad (const float2 &point, const float2 &dPdx, const float2 &dPdy) const;
		ND_ T SampleGradOffset (const float2 &point, const float2 &dPdx, const float2 &dPdy, const int2 &offset) const;

		ND_ T SampleLod (const float2 &point, float lod) const;
		ND_ T SampleLodOffset (const float2 &point, float lod, const int2 &offset) const;

		ND_ T Proj (const float3 &point) const;
		ND_ T Proj (const float4 &point) const;
		ND_ T Proj (const float3 &point, float bias) const;
		ND_ T Proj (const float4 &point, float bias) const;

		ND_ T ProjGrad (const float3 &point, const float2 &pDx, const float2 &pDy) const;
		ND_ T ProjGrad (const float4 &point, const float2 &pDx, const float2 &pDy) const;

		ND_ T ProjGradOffset (const float3 &point, const float2 &pDx, const float2 &pDy, const int2 &offset) const;
		ND_ T ProjGradOffset (const float4 &point, const float2 &pDx, const float2 &pDy, const int2 &offset) const;

		ND_ T ProjLod (const float3 &point, float lod) const;
		ND_ T ProjLod (const float4 &point, float lod) const;

		ND_ T ProjLodOffset (const float3 &point, float lod, const int2 &offset) const;
		ND_ T ProjLodOffset (const float4 &point, float lod, const int2 &offset) const;

		ND_ T ProjOffset (const float3 &point, const int2 &offset) const;
		ND_ T ProjOffset (const float4 &point, const int2 &offset) const;

		ND_ float2	QueryLod (const float2 &point) const;
		
		ND_ int		Samples () const	{ return 0; }
	};
	
	
/*
=================================================
	Fetch
=================================================
*/
	template <typename T>
	inline T  Texture2D<T>::Fetch (const int2 &point, int lod) const
	{
		T	texel;
		_Fetch( point.xyo(), int3(), lod, OUT &texel );
		return texel;
	}
	
	template <typename T>
	inline T  Texture2D<T>::FetchOffset (const int2 &point, int lod, const int2 &offset) const
	{
		T	texel;
		_Fetch( point.xyo(), offset.xyo(), lod, OUT &texel );
		return texel;
	}
	
/*
=================================================
	Sample
=================================================
*/
	template <typename T>
	inline T  Texture2D<T>::Sample (const float2 &point) const
	{
		T	texel;
		_Sample( point.xyo(), int3(), 0.0f, OUT &texel );
		return texel;
	}
	
	template <typename T>
	inline T  Texture2D<T>::Sample (const float2 &point, float bias) const
	{
		T	texel;
		_Sample( point.xyo(), int3(), bias, OUT &texel );
		return texel;
	}
	
/*
=================================================
	SampleOffset
=================================================
*/
	template <typename T>
	inline T  Texture2D<T>::SampleOffset (const float2 &point, const int2 &offset) const
	{
		T	texel;
		_Sample( point.xyo(), offset.xyo(), 0.0f, OUT &texel );
		return texel;
	}
	
	template <typename T>
	inline T  Texture2D<T>::SampleOffset (const float2 &point, const int2 &offset, float bias) const
	{
		T	texel;
		_Sample( point.xyo(), offset.xyo(), bias, OUT &texel );
		return texel;
	}
	
/*
=================================================
	SampleLod
=================================================
*/
	template <typename T>
	inline T  Texture2D<T>::SampleLod (const float2 &point, float lod) const
	{
		T	texel;
		_SampleLod( point.xyo(), int3(), lod, OUT &texel );
		return texel;
	}

	template <typename T>
	inline T  Texture2D<T>::SampleLodOffset (const float2 &point, float lod, const int2 &offset) const
	{
		T	texel;
		_SampleLod( point.xyo(), offset.xyo(), lod, OUT &texel );
		return texel;
	}


}	// Impl
}	// SWShaderLang

#endif	// GRAPHICS_API_SOFT
