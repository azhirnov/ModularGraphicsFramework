// This is generated file, don't change anything!

#pragma once

#include "Engine/Shaders/Shader/Helpers.h"

namespace ShaderEditor
{
namespace ShaderNodes
{


	// ImageLoad, ImageStore
	template <typename IT, typename AT, typename P>
	inline P ImageLoad (const Image<IT,AT,P> &image, const typename IT::IntCoord_t &point) {
		STATIC_ASSERT( AT::readAccess );
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< P >( "ImageLoad", image, point );
	}

	template <typename IT, typename AT, typename P>
	inline P ImageLoad (const Image<IT,AT,P> &image, const typename IT::IntCoord_t &point, const Sample &sample) {
		STATIC_ASSERT( AT::readAccess );
		STATIC_ASSERT( IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		return Helpers::BuildNode< P >( "ImageLoad", image, point, sample.value );
	}

	template <typename IT, typename AT, typename P>
	inline void ImageStore (const Image<IT,AT,P> &image, const typename IT::IntCoord_t &point, const P &data) {
		STATIC_ASSERT( AT::writeAccess );
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		Helpers::NodeGetter::Get( image )->Root()->AddField( Helpers::BuildNode< _ShaderNodesHidden_::ShaderOutputNode >( "ImageStore", image, point, data ) );
	}

	template <typename IT, typename AT, typename P>
	inline void ImageStore (const Image<IT,AT,P> &image, const typename IT::IntCoord_t &point, const Sample &sample, const P &data) {
		STATIC_ASSERT( AT::writeAccess );
		STATIC_ASSERT( IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> or Helpers::IsFloat<P> );
		Helpers::NodeGetter::Get( image )->Root()->AddField( Helpers::BuildNode< _ShaderNodesHidden_::ShaderOutputNode >( "ImageStore", image, point, sample.value, data ) );
	}


	// ImageGetSize, ImageGetSamples
	template <typename IT, typename AT, typename P>
	inline typename IT::IntCoord_t ImageGetSize (const Image<IT,AT,P> &image) {
		return Helpers::BuildNode< P >( "ImageGetSize", image );
	}

	template <typename IT, typename AT, typename P>
	inline Int ImageGetSamples (const Image<IT,AT,P> &image) {
		STATIC_ASSERT( IT::Multisample );
		return Helpers::BuildNode< P >( "ImageGetSamples", image );
	}


	// ImageAtomicAdd, ImageAtomicExchange, ImageAtomicMax, ImageAtomicMin, ImageAtomicAnd, ImageAtomicOr, ImageAtomicXor
	template <typename IT, typename AT, typename P>
	inline NVec<typename P::Value_t,1> ImageAtomicAdd (const Image<IT,AT,P> &image, const typename IT::IntCoord_t &point, const NVec<typename P::Value_t,1> &data) {
		STATIC_ASSERT( AT::writeAccess );
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> );
		return Helpers::BuildNode< NVec<typename P::Value_t,1> >( "ImageAtomicAdd", image, point, data );
	}

	template <typename IT, typename AT, typename P>
	inline NVec<typename P::Value_t,1> ImageAtomicAdd (const Image<IT,AT,P> &image, const typename IT::IntCoord_t &point, const Sample &sample, const NVec<typename P::Value_t,1> &data) {
		STATIC_ASSERT( AT::writeAccess );
		STATIC_ASSERT( IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> );
		return Helpers::BuildNode< NVec<typename P::Value_t,1> >( "ImageAtomicAdd", image, point, sample.value, data );
	}

	template <typename IT, typename AT, typename P>
	inline NVec<typename P::Value_t,1> ImageAtomicExchange (const Image<IT,AT,P> &image, const typename IT::IntCoord_t &point, const NVec<typename P::Value_t,1> &data) {
		STATIC_ASSERT( AT::writeAccess );
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> );
		return Helpers::BuildNode< NVec<typename P::Value_t,1> >( "ImageAtomicExchange", image, point, data );
	}

	template <typename IT, typename AT, typename P>
	inline NVec<typename P::Value_t,1> ImageAtomicExchange (const Image<IT,AT,P> &image, const typename IT::IntCoord_t &point, const Sample &sample, const NVec<typename P::Value_t,1> &data) {
		STATIC_ASSERT( AT::writeAccess );
		STATIC_ASSERT( IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> );
		return Helpers::BuildNode< NVec<typename P::Value_t,1> >( "ImageAtomicExchange", image, point, sample.value, data );
	}

	template <typename IT, typename AT, typename P>
	inline NVec<typename P::Value_t,1> ImageAtomicMax (const Image<IT,AT,P> &image, const typename IT::IntCoord_t &point, const NVec<typename P::Value_t,1> &data) {
		STATIC_ASSERT( AT::writeAccess );
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> );
		return Helpers::BuildNode< NVec<typename P::Value_t,1> >( "ImageAtomicMax", image, point, data );
	}

	template <typename IT, typename AT, typename P>
	inline NVec<typename P::Value_t,1> ImageAtomicMax (const Image<IT,AT,P> &image, const typename IT::IntCoord_t &point, const Sample &sample, const NVec<typename P::Value_t,1> &data) {
		STATIC_ASSERT( AT::writeAccess );
		STATIC_ASSERT( IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> );
		return Helpers::BuildNode< NVec<typename P::Value_t,1> >( "ImageAtomicMax", image, point, sample.value, data );
	}

	template <typename IT, typename AT, typename P>
	inline NVec<typename P::Value_t,1> ImageAtomicMin (const Image<IT,AT,P> &image, const typename IT::IntCoord_t &point, const NVec<typename P::Value_t,1> &data) {
		STATIC_ASSERT( AT::writeAccess );
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> );
		return Helpers::BuildNode< NVec<typename P::Value_t,1> >( "ImageAtomicMin", image, point, data );
	}

	template <typename IT, typename AT, typename P>
	inline NVec<typename P::Value_t,1> ImageAtomicMin (const Image<IT,AT,P> &image, const typename IT::IntCoord_t &point, const Sample &sample, const NVec<typename P::Value_t,1> &data) {
		STATIC_ASSERT( AT::writeAccess );
		STATIC_ASSERT( IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> );
		return Helpers::BuildNode< NVec<typename P::Value_t,1> >( "ImageAtomicMin", image, point, sample.value, data );
	}

	template <typename IT, typename AT, typename P>
	inline NVec<typename P::Value_t,1> ImageAtomicAnd (const Image<IT,AT,P> &image, const typename IT::IntCoord_t &point, const NVec<typename P::Value_t,1> &data) {
		STATIC_ASSERT( AT::writeAccess );
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> );
		return Helpers::BuildNode< NVec<typename P::Value_t,1> >( "ImageAtomicAnd", image, point, data );
	}

	template <typename IT, typename AT, typename P>
	inline NVec<typename P::Value_t,1> ImageAtomicAnd (const Image<IT,AT,P> &image, const typename IT::IntCoord_t &point, const Sample &sample, const NVec<typename P::Value_t,1> &data) {
		STATIC_ASSERT( AT::writeAccess );
		STATIC_ASSERT( IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> );
		return Helpers::BuildNode< NVec<typename P::Value_t,1> >( "ImageAtomicAnd", image, point, sample.value, data );
	}

	template <typename IT, typename AT, typename P>
	inline NVec<typename P::Value_t,1> ImageAtomicOr (const Image<IT,AT,P> &image, const typename IT::IntCoord_t &point, const NVec<typename P::Value_t,1> &data) {
		STATIC_ASSERT( AT::writeAccess );
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> );
		return Helpers::BuildNode< NVec<typename P::Value_t,1> >( "ImageAtomicOr", image, point, data );
	}

	template <typename IT, typename AT, typename P>
	inline NVec<typename P::Value_t,1> ImageAtomicOr (const Image<IT,AT,P> &image, const typename IT::IntCoord_t &point, const Sample &sample, const NVec<typename P::Value_t,1> &data) {
		STATIC_ASSERT( AT::writeAccess );
		STATIC_ASSERT( IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> );
		return Helpers::BuildNode< NVec<typename P::Value_t,1> >( "ImageAtomicOr", image, point, sample.value, data );
	}

	template <typename IT, typename AT, typename P>
	inline NVec<typename P::Value_t,1> ImageAtomicXor (const Image<IT,AT,P> &image, const typename IT::IntCoord_t &point, const NVec<typename P::Value_t,1> &data) {
		STATIC_ASSERT( AT::writeAccess );
		STATIC_ASSERT( not IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> );
		return Helpers::BuildNode< NVec<typename P::Value_t,1> >( "ImageAtomicXor", image, point, data );
	}

	template <typename IT, typename AT, typename P>
	inline NVec<typename P::Value_t,1> ImageAtomicXor (const Image<IT,AT,P> &image, const typename IT::IntCoord_t &point, const Sample &sample, const NVec<typename P::Value_t,1> &data) {
		STATIC_ASSERT( AT::writeAccess );
		STATIC_ASSERT( IT::Multisample );
		STATIC_ASSERT( Helpers::IsInt<P> or Helpers::IsUInt<P> );
		return Helpers::BuildNode< NVec<typename P::Value_t,1> >( "ImageAtomicXor", image, point, sample.value, data );
	}



}	// ShaderNodes
}	// ShaderEditor
