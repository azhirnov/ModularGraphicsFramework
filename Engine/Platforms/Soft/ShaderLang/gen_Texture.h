/*
	This is generated file, don't change anything!
*/

namespace SWShaderLang
{

	// texelFetch
	template <typename GenType> GenType texelFetch (gsampler2D<GenType> &sampler, ivec2 p, int lod)  { return sampler.Fetch( _RCast<Impl::int2>(p), lod ); }

	// texelFetchOffset
	template <typename GenType> GenType texelFetchOffset (gsampler2D<GenType> &sampler, ivec2 p, int lod, ivec2 offset)  { return sampler.FetchOffset( _RCast<Impl::int2>(p), lod, _RCast<Impl::int2>(offset) ); }

	// texture
	template <typename GenType> GenType texture (gsampler2D<GenType> &sampler, vec2 p, float bias = 0.0f)  { return sampler.Sample( _RCast<Impl::float2>(p), bias ); }

	// textureLod
	template <typename GenType> GenType textureLod (gsampler2D<GenType> &sampler, vec2 p, float lod)  { return sampler.SampleLod( _RCast<Impl::float2>(p), lod ); }

	// textureLodOffset
	template <typename GenType> GenType textureLodOffset (gsampler2D<GenType> &sampler, vec2 p, float lod, ivec2 offset)  { return sampler.SampleLodOffset( _RCast<Impl::float2>(p), lod, _RCast<Impl::int2>(offset) ); }

	// textureOffset
	template <typename GenType> GenType textureOffset (gsampler2D<GenType> &sampler, vec2 p, ivec2 offset, float bias = 0.0f)  { return sampler.SampleOffset( _RCast<Impl::float2>(p), _RCast<Impl::int2>(offset), bias ); }

	// textureProj
	template <typename GenType> GenType textureProj (gsampler2D<GenType> &sampler, vec3 p, float bias = 0.0f)  { return sampler.Proj( _RCast<Impl::float3>(p), bias ); }

	// textureProjLod
	template <typename GenType> GenType textureProjLod (gsampler2D<GenType> &sampler, vec3 p, float lod)  { return sampler.ProjLod( _RCast<Impl::float3>(p), lod ); }

	// textureProjLodOffset
	template <typename GenType> GenType textureProjLodOffset (gsampler2D<GenType> &sampler, vec3 p, float lod, ivec2 offset, float bias = 0.0f)  { return sampler.ProjLodOffset( _RCast<Impl::float3>(p), lod, _RCast<Impl::int2>(offset), bias ); }

	// textureProjOffset
	template <typename GenType> GenType textureProjOffset (gsampler2D<GenType> &sampler, vec3 p, ivec2 offset, float bias = 0.0f)  { return sampler.ProjOffset( _RCast<Impl::float3>(p), _RCast<Impl::int2>(offset), bias ); }

	// textureQueryLevels
	template <typename GenType> int textureQueryLevels (gsampler2D<GenType> &sampler)  { return sampler.QueryLevels(); }

	// textureQueryLod
	template <typename GenType> vec2 textureQueryLod (gsampler2D<GenType> &sampler, vec2 p)  { return _RCast<vec2>( sampler.QueryLod( _RCast<Impl::float2>(p) ) ); }

	// textureSamples

	// textureSize
	template <typename GenType> ivec2 textureSize (gsampler2D<GenType> &sampler, int lod)  { return _RCast<ivec2>( sampler.Size( lod ) ); }

}	// SWShaderLang
