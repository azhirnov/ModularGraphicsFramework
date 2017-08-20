// This is generated file, don't change anything!

#pragma once

#include "Engine/Shaders/Shader/Helpers.h"

namespace ShaderEditor
{
namespace ShaderNodes
{


	// TextureSample, TextureSampleOffset
	template <typename IT, typename P>
	inline P TextureSample (const Image<IT,ReadOnly,P> &image, const Sampler &sampler, const typename IT::FpCoord_t &point) {
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< P >( "TextureSample", image, sampler, point );
	}

	template <typename IT, typename P>
	inline P TextureSampleOffset (const Image<IT,ReadOnly,P> &image, const Sampler &sampler, const typename IT::FpCoord_t &point, const typename IT::IntCoord_t &offset) {
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< P >( "TextureSampleOffset", image, sampler, point, offset );
	}


	// TextureSampleLod, TextureSampleLodOffset
	template <typename IT, typename P>
	inline P TextureSampleLod (const Image<IT,ReadOnly,P> &image, const Sampler &sampler, const typename IT::FpCoord_t &point, const Float &lod) {
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< P >( "TextureSampleLod", image, sampler, point, lod );
	}

	template <typename IT, typename P>
	inline P TextureSampleOffsetLod (const Image<IT,ReadOnly,P> &image, const Sampler &sampler, const typename IT::FpCoord_t &point, const typename IT::IntCoord_t &offset, const Float &lod) {
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< P >( "TextureSampleLodOffset", image, sampler, point, lod, offset );
	}


	// TextureSampleGrad, TextureSampleGradOffset
	template <typename IT, typename P>
	inline P TextureSampleGrad (const Image<IT,ReadOnly,P> &image, const Sampler &sampler, const typename IT::FpCoord_t &point, const typename NVec<float, IT::Dimension> &dPdx, const typename NVec<float, IT::Dimension> &dPdy) {
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< P >( "TextureSampleGrad", image, sampler, point, dPdx, dPdy );
	}

	template <typename IT, typename P>
	inline P TextureSampleGradOffset (const Image<IT,ReadOnly,P> &image, const Sampler &sampler, const typename IT::FpCoord_t &point, const typename NVec<float, IT::Dimension> &dPdx, const typename NVec<float, IT::Dimension> &dPdy, const typename IT::IntCoord_t &offset) {
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< P >( "TextureSampleGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}


	// TextureSampleGrather, TextureSampleGatherOffset
	template <typename IT, typename P>
	inline P TextureSampleGather (const Image<IT,ReadOnly,P> &image, const Sampler &sampler, const typename IT::FpCoord_t &point) {
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< P >( "TextureSampleGather", image, sampler, point );
	}

	template <typename IT, typename P>
	inline P TextureSampleGatherOffset (const Image<IT,ReadOnly,P> &image, const Sampler &sampler, const typename IT::FpCoord_t &point, const typename IT::IntCoord_t &offset) {
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< P >( "TextureSampleGatherOffset", image, sampler, point, offset );
	}


	// TextureSampleProj, TextureSampleProjOffset
	template <typename IT, typename P>
	inline P TextureSampleProj (const Image<IT,ReadOnly,P> &image, const Sampler &sampler, const Float4 &point) {
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< P >( "TextureSampleProj", image, sampler, point );
	}

	template <typename IT, typename P>
	inline P TextureSampleProjOffset (const Image<IT,ReadOnly,P> &image, const Sampler &sampler, const typename IT::FpCoord_t &point, const typename IT::IntCoord_t &offset) {
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< P >( "TextureSampleProjOffset", image, sampler, point, offset );
	}


	// TextureSampleProjLod, TextureSampleProjLodOffset
	template <typename IT, typename P>
	inline P TextureSampleProjLod (const Image<IT,ReadOnly,P> &image, const Sampler &sampler, const Float4 &point, const Float &lod) {
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< P >( "TextureSampleProjLod", image, sampler, point, lod );
	}

	template <typename IT, typename P>
	inline P TextureSampleProjLodOffset (const Image<IT,ReadOnly,P> &image, const Sampler &sampler, const Float4 &point, const Float &lod, const typename IT::IntCoord_t &offset) {
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< P >( "TextureSampleProjLodOffset", image, sampler, point, lod, offset );
	}


	// TextureSampleProjGrad, TextureSampleProjGradOffset
	template <typename IT, typename P>
	inline P TextureSampleProjGrad (const Image<IT,ReadOnly,P> &image, const Sampler &sampler, const Float4 &point, const typename NVec<float, IT::Dimension> &dPdx, const typename NVec<float, IT::Dimension> &dPdy) {
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< P >( "TextureSampleProjGrad", image, sampler, point, dPdx, dPdy );
	}

	template <typename IT, typename P>
	inline P TextureSampleProjGradOffset (const Image<IT,ReadOnly,P> &image, const Sampler &sampler, const typename IT::FpCoord_t &point, const typename NVec<float, IT::Dimension> &dPdx, const typename NVec<float, IT::Dimension> &dPdy, const typename IT::IntCoord_t &offset) {
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< P >( "TextureSampleProjGradOffset", image, sampler, point, dPdx, dPdy, offset );
	}


	// TextureFetch, TextureFetchOffset
	template <typename IT, typename P>
	inline P TextureFetch (const Image<IT,ReadOnly,P> &image, const typename IT::IntCoord_t &point, const Int &lod) {
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< P >( "TextureFetch", image, point, lod );
	}

	template <typename IT, typename P>
	inline P TextureFetch (const Image<IT,ReadOnly,P> &image, const typename IT::IntCoord_t &point, const Sample &sample) {
		STATIC_ASSERT( IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< P >( "TextureFetch", image, point, sample.value );
	}

	template <typename IT, typename P>
	inline P TextureFetchOffset (const Image<IT,ReadOnly,P> &image, const typename IT::IntCoord_t &point, const Int &lod, const typename IT::IntCoord_t &offset) {
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< P >( "TextureFetchOffset", image, point, lod, offset );
	}

	template <typename IT, typename P>
	inline P TextureFetchOffset (const Image<IT,ReadOnly,P> &image, const typename IT::IntCoord_t &point, const Sample &sample, const typename IT::IntCoord_t &offset) {
		STATIC_ASSERT( IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< P >( "TextureFetchOffset", image, point, sample.value, offset );
	}


	// TextureGetSamples, TextureGetSize, TextureQueryLod, TextureQueryLevels
	template <typename IT, typename P>
	inline Int TextureGetSamples (const Image<IT,ReadOnly,P> &image) {
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< Int >( "TextureGetSamples", image );
	}

	template <typename IT, typename P>
	inline typename IT::IntCoord_t TextureGetSize (const Image<IT,ReadOnly,P> &image, const Int &lod) {
		STATIC_ASSERT( IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< typename IT::IntCoord_t >( "TextureGetSize", image, lod );
	}

	template <typename IT, typename P>
	inline typename IT::IntCoord_t TextureGetSize (const Image<IT,ReadOnly,P> &image) {
		STATIC_ASSERT( IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< typename IT::IntCoord_t >( "TextureGetSize", image, point, lod );
	}

	template <typename IT, typename P>
	inline Float2 TextureQueryLod (const Image<IT,ReadOnly,P> &image, const typename IT::FpCoord_t &point) {
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< Float2 >( "TextureQueryLod", image, point );
	}

	template <typename IT, typename P>
	inline Int TextureQueryLevels (const Image<IT,ReadOnly,P> &image) {
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< Int >( "TextureQueryLevels", image );
	}



}	// ShaderNodes
}	// ShaderEditor
