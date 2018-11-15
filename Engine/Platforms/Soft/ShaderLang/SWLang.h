// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Soft/ShaderLang/SWShaderHelper.h"

#ifdef GRAPHICS_API_SOFT

namespace SWShaderLang
{
	using namespace ::glm;
	
	template <typename GenType, Impl::EStorageAccess::type Acs>	using gimageBuffer		= Impl::Image2D< GenType, Acs >;
	template <typename GenType, Impl::EStorageAccess::type Acs>	using gimage1D			= Impl::Image2D< GenType, Acs >;
	template <typename GenType, Impl::EStorageAccess::type Acs>	using gimage1DArray		= Impl::Image2D< GenType, Acs >;
	template <typename GenType, Impl::EStorageAccess::type Acs>	using gimage2D			= Impl::Image2D< GenType, Acs >;
	template <typename GenType, Impl::EStorageAccess::type Acs>	using gimage2DArray		= Impl::Image2D< GenType, Acs >;
	template <typename GenType, Impl::EStorageAccess::type Acs>	using gimage2DMS		= Impl::Image2D< GenType, Acs >;
	template <typename GenType, Impl::EStorageAccess::type Acs>	using gimage2DMSArray	= Impl::Image2D< GenType, Acs >;
	template <typename GenType, Impl::EStorageAccess::type Acs>	using gimage3D			= Impl::Image2D< GenType, Acs >;
	template <typename GenType, Impl::EStorageAccess::type Acs>	using gimageCube		= Impl::Image2D< GenType, Acs >;
	template <typename GenType, Impl::EStorageAccess::type Acs>	using gimageCubeArray	= Impl::Image2D< GenType, Acs >;

	template <typename GenType>	using gsamplerBuffer	= Impl::Texture2D< GenType >;
	template <typename GenType>	using gsampler1D		= Impl::Texture2D< GenType >;
	template <typename GenType>	using gsampler1DArray	= Impl::Texture2D< GenType >;
	template <typename GenType>	using gsampler2D		= Impl::Texture2D< GenType >;
	template <typename GenType>	using gsampler2DArray	= Impl::Texture2D< GenType >;
	template <typename GenType>	using gsampler2DMS		= Impl::Texture2D< GenType >;
	template <typename GenType>	using gsampler2DMSArray	= Impl::Texture2D< GenType >;
	template <typename GenType>	using gsampler3D		= Impl::Texture2D< GenType >;
	template <typename GenType>	using gsamplerCube		= Impl::Texture2D< GenType >;
	template <typename GenType>	using gsamplerCubeArray	= Impl::Texture2D< GenType >;

	template <typename GenType, size_t Count>	using SArr = Impl::SWArray< GenType, Count >;

}	// SWShaderLang

#define _RCast		GX_STL::GXTypes::ReferenceCast

#include "Engine/Platforms/Soft/ShaderLang/gen_Image.h"
#include "Engine/Platforms/Soft/ShaderLang/gen_Texture.h"

#undef _RCast

#endif	// GRAPHICS_API_SOFT
