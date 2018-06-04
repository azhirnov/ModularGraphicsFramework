// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Public/GPU/Image.h"

namespace Engine
{
namespace Platforms
{
	
/*
=================================================
	constructor
=================================================
*/
	ImageDescriptor::ImageDescriptor (EImage::type			imageType,
									  const uint4	&		dimension,
									  EPixelFormat::type	format,
									  EImageUsage::bits		usage,
									  MipmapLevel			maxLevel,
									  MultiSamples			samples) :
		imageType(imageType),	dimension(dimension),
		format(format),			usage(usage),
		maxLevel(maxLevel),		samples(samples)
	{}
//-----------------------------------------------------------------------------


	
/*
=================================================
	constructor
=================================================
*/
	ImageViewDescriptor::ImageViewDescriptor (EImage::type			viewType,
											  EPixelFormat::type	format,
											  MipmapLevel			baseLevel,
											  uint					levelCount,
											  ImageLayer			baseLayer,
											  uint					layerCount,
											  Swizzle::type			swizzle) :
		viewType{viewType},		format{format},
		baseLevel{baseLevel},	levelCount{levelCount},
		baseLayer{baseLayer},	layerCount{layerCount},
		swizzle{swizzle}
	{}
	
	ImageViewDescriptor::ImageViewDescriptor (const ImageDescriptor &descr) :
		viewType{ descr.imageType },	format{ descr.format },
		baseLevel{},					levelCount{ descr.maxLevel.Get() },
		baseLayer{},					layerCount{ descr.dimension.w },
		swizzle{ "RGBA"_Swizzle }
	{}

/*
=================================================
	compare operators
=================================================
*/
	bool ImageViewDescriptor::operator == (const ImageViewDescriptor &right) const
	{
		return	this->viewType		== right.viewType	and
				this->format		== right.format		and
				this->baseLevel		== right.baseLevel	and
				this->levelCount	== right.levelCount	and
				this->baseLayer		== right.baseLayer	and
				this->layerCount	== right.layerCount	and
				this->swizzle		== right.swizzle;
	}
	
	bool ImageViewDescriptor::operator >  (const ImageViewDescriptor &right) const
	{
		return	viewType	!= right.viewType	?	viewType	> right.viewType	:
				format		!= right.format		?	format		> right.format		:
				baseLevel	!= right.baseLevel	?	baseLevel	> right.baseLevel	:
				levelCount	!= right.levelCount	?	levelCount	> right.levelCount	:
				baseLayer	!= right.baseLayer	?	baseLayer	> right.baseLayer	:
				layerCount	!= right.layerCount	?	layerCount	> right.layerCount	:
													swizzle		> right.swizzle;
	}

}	// Platforms
}	// Engine
