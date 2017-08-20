// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "ImageTypes.h"
#include "Generated/AllNodeFuncs.h"

namespace ShaderEditor
{
namespace ShaderNodes
{

/*
=================================================
	operator =
=================================================
*/
	template <typename I, typename A, typename P>
	inline Image<I,A,P> &  Image<I,A,P>::operator = (const ModulePtr &value)
	{
		_SelfNode()->SetConstant( _ShaderNodesHidden_::ISrcNode::ImageUnit( value ) );
		return *this;
	}

/*
=================================================
	Load
=================================================
*/
	template <typename I, typename A, typename P>
	inline P	 Image<I,A,P>::Load (const IntCoord_t &coord) const
	{
		return ImageLoad( *this, coord );
	}

/*
=================================================
	Store
=================================================
*/
	template <typename I, typename A, typename P>
	inline void  Image<I,A,P>::Store (const IntCoord_t &coord, const Pixel_t &value) const
	{
		return ImageStore( *this, coord, value );
	}
	
/*
=================================================
	AtomicAdd
=================================================
*/
	template <typename I, typename A, typename P>
	inline void  Image<I,A,P>::AtomicAdd (const IntCoord_t &coord, const Data_t &data) const
	{
		return ImageAtomicAdd( *this, coord, data );
	}

/*
=================================================
	AtomicExchange
=================================================
*/
	template <typename I, typename A, typename P>
	inline void  Image<I,A,P>::AtomicExchange (const IntCoord_t &coord, const Data_t &data) const
	{
		return ImageAtomicExchange( *this, coord, data );
	}
	
/*
=================================================
	AtomicMax
=================================================
*/
	template <typename I, typename A, typename P>
	inline void  Image<I,A,P>::AtomicMax (const IntCoord_t &coord, const Data_t &data) const
	{
		return ImageAtomicMax( *this, coord, data );
	}

/*
=================================================
	AtomicMin
=================================================
*/
	template <typename I, typename A, typename P>
	inline void  Image<I,A,P>::AtomicMin (const IntCoord_t &coord, const Data_t &data) const
	{
		return ImageAtomicMin( *this, coord, data );
	}
	
/*
=================================================
	AtomicAnd
=================================================
*/
	template <typename I, typename A, typename P>
	inline void  Image<I,A,P>::AtomicAnd (const IntCoord_t &coord, const Data_t &data) const
	{
		return ImageAtomicAnd( *this, coord, data );
	}

/*
=================================================
	AtomicOr
=================================================
*/
	template <typename I, typename A, typename P>
	inline void  Image<I,A,P>::AtomicOr (const IntCoord_t &coord, const Data_t &data) const
	{
		return ImageAtomicOr( *this, coord, data );
	}

/*
=================================================
	AtomicXor
=================================================
*/
	template <typename I, typename A, typename P>
	inline void  Image<I,A,P>::AtomicXor (const IntCoord_t &coord, const Data_t &data) const
	{
		return ImageAtomicXor( *this, coord, data );
	}
	
/*
=================================================
	_Typename
=================================================
*/
	template <typename I, typename A, typename P>
	inline typename Image<I,A,P>::Name_t  Image<I,A,P>::_Typename ()
	{
		return Name_t("Image") << '_' << Image_t::Name() << '_' << Access_t::Name() << '_' << Pixel_t::_Typename();
	}

//=============================================================================
	

	
/*
=================================================
	operator =
=================================================
*/
	inline Sampler &  Sampler::operator = (const ModulePtr &value)
	{
		_SelfNode()->SetConstant( _ShaderNodesHidden_::ISrcNode::SamplerUnit( value ) );
		return *this;
	}

//=============================================================================
	

	
/*
=================================================
	operator =
=================================================
*/
	template <typename A, typename D>
	inline Buffer<A,D> &  Buffer<A,D>::operator = (const ModulePtr &value)
	{
		_SelfNode()->SetConstant( _ShaderNodesHidden_::ISrcNode::BufferUnit( value ) );
		return *this;
	}

//=============================================================================

	
	
	template <typename I, typename P>
	inline P  Texture<I,P>::Sample (const FpCoord_t &point) const
	{
		return TextureSample( image, sampler, point );
	}
	
	template <typename I, typename P>
	inline P  Texture<I,P>::SampleOffset (const FpCoord_t &point, const IntCoord_t &offset) const
	{
		return TextureSampleOffset( image, sampler, point, offset );
	}
	
	template <typename I, typename P>
	inline P  Texture<I,P>::SampleLod (const FpCoord_t &point, const Float &lod) const
	{
		return TextureSampleLod( image, sampler, point, lod );
	}
	
	template <typename I, typename P>
	inline P  Texture<I,P>::SampleLodOffset (const FpCoord_t &point, const Float &lod, const IntCoord_t &offset) const
	{
		return TextureSampleLodOffset( image, sampler, point, lod, offset );
	}
	
	template <typename I, typename P>
	inline P  Texture<I,P>::SampleGrad (const FpCoord_t &point, const FpCoord_t &dPdx, const FpCoord_t &dPdy) const
	{
		return TextureSampleGrad( image, sampler, point, dPdx, dPdy );
	}
	
	template <typename I, typename P>
	inline P  Texture<I,P>::SampleGradOffset (const FpCoord_t &point, const FpCoord_t &dPdx, const FpCoord_t &dPdy, const IntCoord_t &offset) const
	{
		return TextureSampleGradOffset( image, sampler, point, dPdx, dPdy, offset );
	}
	
	template <typename I, typename P>
	inline P  Texture<I,P>::SampleGather (const FpCoord_t &point) const
	{
		return TextureSampleGather( image, sampler, point );
	}
	
	template <typename I, typename P>
	inline P  Texture<I,P>::SampleGatherOffset (const FpCoord_t &point, const IntCoord_t &offset) const
	{
		return TextureSampleGatherOffset( image, sampler, point, offset );
	}
	
	template <typename I, typename P>
	inline P  Texture<I,P>::SampleProj (const Float4 &point) const
	{
		return TextureSampleProj( image, sampler, point );
	}
	
	template <typename I, typename P>
	inline P  Texture<I,P>::SampleProjOffset (const Float4 &point, const IntCoord_t &offset) const
	{
		return TextureSampleProjOffset( image, sampler, point, offset );
	}
	
	template <typename I, typename P>
	inline P  Texture<I,P>::SampleProjLod (const Float4 &point, const Float &lod) const
	{
		return TextureSampleProjLod( image, sampler, point, lod );
	}
	
	template <typename I, typename P>
	inline P  Texture<I,P>::SampleProjLodOffset (const Float4 &point, const Float &lod, const IntCoord_t &offset) const
	{
		return TextureSampleProjLodOffset( image, sampler, point, lod, offset );
	}
	
	template <typename I, typename P>
	inline P  Texture<I,P>::SampleProjGrad (const Float4 &point, const FpCoord_t &dPdx, const FpCoord_t &dPdy) const
	{
		return TextureSampleProjGrad( image, sampler, point, dPdx, dPdy );
	}
	
	template <typename I, typename P>
	inline P  Texture<I,P>::SampleProjGradOffset (const Float4 &point, const FpCoord_t &dPdx, const FpCoord_t &dPdy, const IntCoord_t &offset) const
	{
		return TextureSampleProjGradOffset( image, sampler, point, dPdx, dPdy, offset );
	}
	
//=============================================================================


}	// ShaderNodes
}	// ShaderEditor
