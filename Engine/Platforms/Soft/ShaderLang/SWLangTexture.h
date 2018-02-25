// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Soft/ShaderLang/SWLangCommon.h"

#ifdef GRAPHICS_API_SOFT

namespace SWShaderLang
{
namespace Impl
{

	//
	// Texture 2D
	//
	template <typename T>
	struct Texture2D
	{
		friend class SWShaderHelper;
		
	// types
	private:
		//using RawType_t		= T;
		//using Result_t		= typename T::Result_t;
		//using Mipmap_t		= Image2D< RawType_t, _swlang_hidden_::EStorageAccess::ReadOnly >;
		//using MipLevels_t	= GX_STL::GXTypes::FixedSizeArray< Mipmap_t, 16 >;

		using Result_t	= T;


	// variables
	private:
		//MipLevels_t		_levels;


	// methods
	public:
		Texture2D () {}
		Texture2D (Texture2D &&) = default;
		Texture2D (const Texture2D &) = default;

		Result_t Sample (const float2 &point) const;
		Result_t Sample (const float2 &point, float bias) const;
		
		Result_t SampleOffset (const float2 &point, const int2 &offset) const;
		Result_t SampleOffset (const float2 &point, const int2 &offset, float bias) const;

		Result_t Fetch (const int2 &point) const;
		Result_t FetchOffset (const int2 &point, const int2 &offset) const;

		Result_t Gather (const float2 &point) const;
		Result_t Gather (const float2 &point, int comp) const;

		Result_t GatherOffset (const float2 &point, const int2 &offset) const;
		Result_t GatherOffset (const float2 &point, const int2 &offset, int comp) const;

		Result_t GatherOffsets (const float2 &point, const int2 &offset0, const int2 &offset1, const int2 &offset2, const int2 &offset3) const;
		Result_t GatherOffsets (const float2 &point, const int2 &offset0, const int2 &offset1, const int2 &offset2, const int2 &offset3, int comp) const;

		Result_t SampleGrad (const float2 &point, const float2 &dPdx, const float2 &dPdy) const;
		Result_t SampleGradOffset (const float2 &point, const float2 &dPdx, const float2 &dPdy, const int2 &offset) const;

		Result_t SampleLod (const float2 &point, float lod) const;
		Result_t SampleLodOffset (const float2 &point, float lod, const int2 &offset) const;

		Result_t Proj (const float3 &point) const;
		Result_t Proj (const float4 &point) const;
		Result_t Proj (const float3 &point, float bias) const;
		Result_t Proj (const float4 &point, float bias) const;

		Result_t ProjGrad (const float3 &point, const float2 &pDx, const float2 &pDy) const;
		Result_t ProjGrad (const float4 &point, const float2 &pDx, const float2 &pDy) const;

		Result_t ProjGradOffset (const float3 &point, const float2 &pDx, const float2 &pDy, const int2 &offset) const;
		Result_t ProjGradOffset (const float4 &point, const float2 &pDx, const float2 &pDy, const int2 &offset) const;

		Result_t ProjLod (const float3 &point, float lod) const;
		Result_t ProjLod (const float4 &point, float lod) const;

		Result_t ProjLodOffset (const float3 &point, float lod, const int2 &offset) const;
		Result_t ProjLodOffset (const float4 &point, float lod, const int2 &offset) const;

		Result_t ProjOffset (const float3 &point, const int2 &offset) const;
		Result_t ProjOffset (const float4 &point, const int2 &offset) const;

		int		QueryLevels () const;
		float2	QueryLod (const float2 &point) const;

		int2	Size (int lod) const;
	};


}	// Impl
}	// SWShaderLang

#endif	// GRAPHICS_API_SOFT
