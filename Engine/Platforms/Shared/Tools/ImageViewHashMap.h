// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/ImageLayer.h"
#include "Engine/Platforms/Shared/GPU/ImageEnums.h"
#include "Engine/Platforms/Shared/GPU/MipmapLevel.h"

namespace Engine
{
namespace PlatformTools
{
	using namespace Platforms;


	//
	// Image View Hash Map
	//

	template <typename ViewType>
	struct ImageViewHashMap
	{
	// types
	public:
		using Key_t = ImageViewDescriptor;
		
	private:
		struct ImageViewHash
		{
			using Result_t	= HashResult;

			Result_t operator () (const Key_t &v) const;
		};

		using ImageViewMap_t	= HashMap< Key_t, ViewType, ImageViewHash >;
		using Self				= ImageViewHashMap< ViewType >;


	// variables
	private:
		ImageViewMap_t		_map;


	// methods
	public:
		ImageViewHashMap () {}

		ViewType Find (const Key_t &key) const;

		void Add (const Key_t &key, ViewType view);

		static void Validate (INOUT Key_t &key, const ImageDescriptor &descr);

		void			Clear ()						{ _map.Clear(); }

		usize			Count ()				const	{ return _map.Count(); }
		bool			Empty ()				const	{ return _map.Empty(); }

		decltype(auto)	operator [] (usize i)	const	{ return _map[i]; }
		decltype(auto)	operator [] (usize i)			{ return _map[i]; }
	};
	
	
/*
=================================================
	hash
=================================================
*/
	template <typename ViewType>
	inline HashResult ImageViewHashMap<ViewType>::ImageViewHash::operator () (const Key_t &v) const
	{
		HashResult	result;
		result += HashOf( v.viewType );
		result += HashOf( v.format );
		result += HashOf( v.baseLevel );
		result += HashOf( v.levelCount );
		result += HashOf( v.baseLayer );
		result += HashOf( v.layerCount );
		result += HashOf( v.swizzle );
		return result;
	}
//-----------------------------------------------------------------------------


/*
=================================================
	Find
=================================================
*/
	template <typename ViewType>
	inline ViewType ImageViewHashMap<ViewType>::Find (const Key_t &key) const
	{
		typename ImageViewMap_t::const_iterator	iter;

		if ( _map.Find( key, OUT iter ) )
			return iter->second;

		return ViewType(0);
	}
	
/*
=================================================
	Add
=================================================
*/
	template <typename ViewType>
	inline void ImageViewHashMap<ViewType>::Add (const Key_t &key, ViewType view)
	{
		_map.Add( key, view );
	}
	
/*
=================================================
	Validate
=================================================
*/
	template <typename ViewType>
	inline void ImageViewHashMap<ViewType>::Validate (INOUT Key_t &key, const ImageDescriptor &descr)
	{
		if ( key.viewType == EImage::Unknown )
			key.viewType = descr.imageType;

		if ( key.format == EPixelFormat::Unknown )
			key.format = descr.format;

		uint	max_layers	= descr.imageType == EImage::Tex3D ? 1 : descr.dimension.z * descr.dimension.w;

		key.baseLayer	= ImageLayer(Clamp( key.baseLayer.Get(), 0u, max_layers-1 ));
		key.layerCount	= Clamp( key.layerCount, 1u, max_layers - key.baseLayer.Get() );

		key.baseLevel	= MipmapLevel(Clamp( key.baseLevel.Get(), 0u, descr.maxLevel.Get()-1 ));
		key.levelCount	= Clamp( key.levelCount, 1u, descr.maxLevel.Get() - key.baseLevel.Get() );
	}


}	// PlatformTools
}	// Engine
