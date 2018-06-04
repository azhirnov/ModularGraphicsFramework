/*
	This is generated file, don't change anything!
*/

namespace SWShaderLang
{

	// imageLoad
	template <typename GenType, Impl::EStorageAccess::type Acs> GenType imageLoad (gimage2D<GenType,Acs> &image, ivec2 p)  { return image.Load( _RCast<Impl::int2>(p) ); }

	// imageStore
	template <typename GenType, Impl::EStorageAccess::type Acs> void imageStore (gimage2D<GenType,Acs> &image, ivec2 p, const GenType &data)  { return image.Store( _RCast<Impl::int2>(p), data ); }

}	// SWShaderLang
