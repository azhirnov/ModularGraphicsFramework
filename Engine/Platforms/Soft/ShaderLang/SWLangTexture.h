// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	TODO:
		- Note that offset does not apply to the layer coordinate for texture arrays. Also note that offsets are not supported for cube maps.
*/

#pragma once

#include "Engine/Platforms/Soft/ShaderLang/SWLangCommon.h"

#ifdef GRAPHICS_API_SOFT

namespace SWShaderLang
{
namespace Impl
{

	//
	// Base Texture
	//
	struct BaseTexture
	{
	// types
	protected:
		using EPixelFormat		= Engine::Platforms::EPixelFormat;
		using MemLayout_t		= Engine::GpuMsg::GetSWImageMemoryLayout::ImgLayers3D;
		using Sampler_t			= Engine::Platforms::SamplerDescription;

		struct ESamplerMode
		{
			enum type
			{
				Unknown							= 0,

				// border color
				_BORDER_OFF						= 0,
				_BORDER_MASK					= (1 << (5 + _BORDER_OFF)) - 1,
				Border_Float					= 1 << (0 + _BORDER_OFF),
				Border_Int						= 1 << (1 + _BORDER_OFF),
				Border_Transparent				= 1 << (2 + _BORDER_OFF),
				Border_Black					= 1 << (3 + _BORDER_OFF),
				Border_White					= 1 << (4 + _BORDER_OFF),

				// filter
				_FILTER_OFF						= GX_STL::CompileTime::IntLog2< uint, _BORDER_MASK > + 1,
				_FILTER_MASK					= 0xF << _FILTER_OFF,
				MinMagMipNearest				= 1 << _FILTER_OFF,
				MinMagNearest_MipLinear			= 2 << _FILTER_OFF,
				MinNearest_MagLinear_MipNearest	= 3 << _FILTER_OFF,
				MinNearest_MagMipLinear			= 4 << _FILTER_OFF,
				MinLinear_MagMipNearest			= 5 << _FILTER_OFF,
				MinLinear_MagNearest_MipLinear	= 6 << _FILTER_OFF,
				MinMagLinear_MipNearest			= 7 << _FILTER_OFF,
				MinMagMipLinear					= 8 << _FILTER_OFF,
				Anisotropic_2					= 9 << _FILTER_OFF,
				Anisotropic_4					= 10 << _FILTER_OFF,
				Anisotropic_8					= 11 << _FILTER_OFF,
				Anisotropic_12					= 12 << _FILTER_OFF,
				Anisotropic_16					= 13 << _FILTER_OFF,
				
				// address
				_U_ADDRESS_OFF					= GX_STL::CompileTime::IntLog2< uint, _FILTER_MASK > + 1,
				_U_ADDRESS_MASK					= 0xF << _U_ADDRESS_OFF,
				U_ClampToEdge					= 1 << _U_ADDRESS_OFF,
				U_ClampToBorder					= 2 << _U_ADDRESS_OFF,
				U_Repeat						= 3 << _U_ADDRESS_OFF,
				U_MirroredRepeat				= 4 << _U_ADDRESS_OFF,

				_V_ADDRESS_OFF					= GX_STL::CompileTime::IntLog2< uint, _U_ADDRESS_MASK > + 1,
				_V_ADDRESS_MASK					= 0xF << _V_ADDRESS_OFF,
				V_ClampToEdge					= 1 << _V_ADDRESS_OFF,
				V_ClampToBorder					= 2 << _V_ADDRESS_OFF,
				V_Repeat						= 3 << _V_ADDRESS_OFF,
				V_MirroredRepeat				= 4 << _V_ADDRESS_OFF,

				_W_ADDRESS_OFF					= GX_STL::CompileTime::IntLog2< uint, _V_ADDRESS_MASK > + 1,
				_W_ADDRESS_MASK					= 0xF << _W_ADDRESS_OFF,
				W_ClampToEdge					= 1 << _W_ADDRESS_OFF,
				W_ClampToBorder					= 2 << _W_ADDRESS_OFF,
				W_Repeat						= 3 << _W_ADDRESS_OFF,
				W_MirroredRepeat				= 4 << _W_ADDRESS_OFF,

				// compare mode
				_COMPAREOP_OFF					= GX_STL::CompileTime::IntLog2< uint, _W_ADDRESS_MASK > + 1,
				_COMPAREOP_MASK					= 0xF << _COMPAREOP_OFF,
				CompareOp_Never					= 1 << _COMPAREOP_OFF,
				CompareOp_Less					= 2 << _COMPAREOP_OFF,
				CompareOp_Equal					= 3 << _COMPAREOP_OFF,
				CompareOp_LEqual				= 4 << _COMPAREOP_OFF,
				CompareOp_Greater				= 5 << _COMPAREOP_OFF,
				CompareOp_NotEqual				= 6 << _COMPAREOP_OFF,
				CompareOp_GEqual				= 7 << _COMPAREOP_OFF,
				CompareOp_Always				= 8 << _COMPAREOP_OFF,
			};

			GX_ENUM_BIT_OPERATIONS( type );
		};

		enum class EOutputFormat
		{
			Float4,
			Int4,
			UInt4,
		};

		struct Sampler
		{
		// variables
			ESamplerMode::type	mode		= ESamplerMode::Unknown;
			float				mipLodBias	= 0.0f;
			float2				lodRange;

		// methods
			Sampler () {}

			Sampler (ESamplerMode::type mode, float mipLodBias, float2 lodRange) :
				mode{mode}, mipLodBias{mipLodBias}, lodRange{lodRange} {}
		};

		using FetchFunc_t		= void (*) (const MemLayout_t &memLayout, const int3 &coord, const int3 &offset, int lod, OUT void *texel);
		using SampleFunc_t		= void (*) (const MemLayout_t &memLayout, const float3 &coord, const int3 &offset, float bias, const Sampler &samp, OUT void *texel);
		using SampleLodFunc_t	= void (*) (const MemLayout_t &memLayout, const float3 &coord, const int3 &offset, float lod, const Sampler &samp, OUT void *texel);


	// variables
	private:
		FetchFunc_t			_fetch		= null;
		SampleFunc_t		_sample		= null;
		SampleLodFunc_t		_sampleLod	= null;
		MemLayout_t			_memLayout;
		Sampler				_sampler;


	// methods
	protected:
		template <typename T>
		BaseTexture (MemLayout_t &&memLayout, const Sampler_t &sampler, UninitializedT<T>);
		BaseTexture (MemLayout_t &&memLayout, const Sampler_t &sampler, EOutputFormat outFmt);
		
		BaseTexture () {}
		
		GX_DEFCOPYCTOR_ASSIGN( BaseTexture );

		void _Fetch (const int3 &coord, const int3 &offset, int lod, OUT void *texel) const;
		void _Sample (const float3 &coord, const int3 &offset, float bias, OUT void *texel) const;
		void _SampleLod (const float3 &coord, const int3 &offset, float lod, OUT void *texel) const;

	private:
		bool _Init (EOutputFormat outFmt);
		
		template <typename SrcColor, typename DstColor>
		bool _InitByFormat ();

		static ESamplerMode::type _ChooseSampler (const Sampler_t &samp);

		template <typename SrcColor, typename DstColor>
		static void _FetchImpl (const MemLayout_t &memLayout, const int3 &coord, const int3 &offset, int lod, OUT void *texel);
		
		template <typename SrcColor, typename DstColor>
		static void _SampleTrilinear (const MemLayout_t &memLayout, const float3 &coord, const int3 &offset, float bias, const Sampler &samp, OUT void *texel);
		
		template <typename SrcColor, typename DstColor>
		static void _SampleAnisotropic (const MemLayout_t &memLayout, const float3 &coord, const int3 &offset, float bias, const Sampler &samp, OUT void *texel);
		
		template <typename SrcColor, typename DstColor>
		static void _SampleLodTrilinear (const MemLayout_t &memLayout, const float3 &coord, const int3 &offset, float lod, const Sampler &samp, OUT void *texel);
		
		template <typename SrcColor, typename DstColor>
		static void _SampleLodAnisotropic (const MemLayout_t &memLayout, const float3 &coord, const int3 &offset, float lod, const Sampler &samp, OUT void *texel);

		static void _TransformCoords (OUT float3 &outFrac, OUT float3 &outICoord, OUT bool &isBorder,
									  const MemLayout_t &memLayout, const float3 &coord, const int3 &offset, ESamplerMode::type samp);

		static void _CheckOffset (const MemLayout_t &memLayout, const int3 &offset);
		
		static bool _IsTextureArray1D (const MemLayout_t &memLayout);
		static bool _IsTextureArray2D (const MemLayout_t &memLayout);
		static bool _IsTexture1D (const MemLayout_t &memLayout);
		static bool _IsTexture2D (const MemLayout_t &memLayout);
		static bool _IsTexture3D (const MemLayout_t &memLayout);

		template <typename DstColor>
		static DstColor  _GetBorderColor (ESamplerMode::type borderColor);

		static bool _IsMipmapLinearFilter (ESamplerMode::type sampler);
		static bool _IsMinLinearFilter (ESamplerMode::type sampler);
		static bool _IsMagLinearFilter (ESamplerMode::type sampler);
		static uint _GetAnisotropyLevel (ESamplerMode::type sampler);

		static bool _CheckMipmap (const Engine::GpuMsg::GetSWImageMemoryLayout::ImgLayer &mipmap);

	public:
		ND_ int3	Size (int lod) const;
		ND_ int		QueryLevels () const;
	};


	template <>	inline BaseTexture::BaseTexture (MemLayout_t &&memLayout, const Sampler_t &sampler, UninitializedT<glm::vec4>)  : BaseTexture{ RVREF(memLayout), sampler, EOutputFormat::Float4 } {}
	template <>	inline BaseTexture::BaseTexture (MemLayout_t &&memLayout, const Sampler_t &sampler, UninitializedT<glm::ivec4>) : BaseTexture{ RVREF(memLayout), sampler, EOutputFormat::Int4 }   {}
	template <>	inline BaseTexture::BaseTexture (MemLayout_t &&memLayout, const Sampler_t &sampler, UninitializedT<glm::uvec4>) : BaseTexture{ RVREF(memLayout), sampler, EOutputFormat::UInt4 }  {}
	
	inline void BaseTexture::_Fetch (const int3 &coord, const int3 &offset, int lod, OUT void *texel) const {
		ASSERT( _fetch );
		return _fetch( _memLayout, coord, offset, lod, OUT texel );
	}

	inline void BaseTexture::_Sample (const float3 &coord, const int3 &offset, float bias, OUT void *texel) const {
		ASSERT( _sample );
		return _sample( _memLayout, coord, offset, bias, _sampler, texel );
	}
	
	inline void BaseTexture::_SampleLod (const float3 &coord, const int3 &offset, float lod, OUT void *texel) const {
		ASSERT( _sampleLod );
		return _sampleLod( _memLayout, coord, offset, lod, _sampler, texel );
	}


}	// Impl
}	// SWShaderLang

#endif	// GRAPHICS_API_SOFT
