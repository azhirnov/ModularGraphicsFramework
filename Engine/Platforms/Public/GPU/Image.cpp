// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Enums.ToString.h"

namespace Engine
{
namespace Platforms
{
	
/*
=================================================
	constructor
=================================================
*/
	ImageDescription::ImageDescription (EImage::type		imageType,
										const uint4	&		dimension,
										EPixelFormat::type	format,
										EImageUsage::bits	usage,
										MipmapLevel			maxLevel,
										MultiSamples		samples) :
		imageType(imageType),	dimension(dimension),
		format(format),			usage(usage),
		maxLevel(maxLevel),		samples(samples)
	{}
	
/*
=================================================
	ToString
=================================================
*/
	DEBUG_ONLY(
	String  ImageDescription::ToString () const
	{
		String	str;
		str << "ImageDescription {"
			<< "\n	imageType:   " << EImage::ToString( imageType )
			<< "\n	dimension:   " << dimension.x << ", " << dimension.y << ", " << dimension.z
			<< "\n	layers:      " << dimension.w
			<< "\n	format:      " << EPixelFormat::ToString( format )
			<< "\n	usage:       " << EImageUsage::ToString( usage )
			<< "\n	maxLevel:    " << maxLevel.Get()
			<< "\n	samples:     " << samples.Get()
			<< "\n}\n";
		return str;
	})
//-----------------------------------------------------------------------------


	
/*
=================================================
	constructor
=================================================
*/
	ImageViewDescription::ImageViewDescription (EImage::type		viewType,
												EPixelFormat::type	format,
												MipmapLevel			baseLevel,
												uint				levelCount,
												ImageLayer			baseLayer,
												uint				layerCount,
												Swizzle::type		swizzle) :
		viewType{viewType},		format{format},
		baseLevel{baseLevel},	levelCount{levelCount},
		baseLayer{baseLayer},	layerCount{layerCount},
		swizzle{swizzle}
	{}
	
	ImageViewDescription::ImageViewDescription (const ImageDescription &descr) :
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
	bool ImageViewDescription::operator == (const ImageViewDescription &right) const
	{
		return	this->viewType		== right.viewType	and
				this->format		== right.format		and
				this->baseLevel		== right.baseLevel	and
				this->levelCount	== right.levelCount	and
				this->baseLayer		== right.baseLayer	and
				this->layerCount	== right.layerCount	and
				this->swizzle		== right.swizzle;
	}
	
	bool ImageViewDescription::operator >  (const ImageViewDescription &right) const
	{
		return	viewType	!= right.viewType	?	viewType	> right.viewType	:
				format		!= right.format		?	format		> right.format		:
				baseLevel	!= right.baseLevel	?	baseLevel	> right.baseLevel	:
				levelCount	!= right.levelCount	?	levelCount	> right.levelCount	:
				baseLayer	!= right.baseLayer	?	baseLayer	> right.baseLayer	:
				layerCount	!= right.layerCount	?	layerCount	> right.layerCount	:
													swizzle		> right.swizzle;
	}
	
/*
=================================================
	ToString
=================================================
*/
	DEBUG_ONLY(
	String  ImageViewDescription::ToString () const
	{
		String	str;
		str << "ImageViewDescription {"
			<< "\n	viewType:  " << EImage::ToString( viewType )
			<< "\n	format:    " << EPixelFormat::ToString( format )
			<< "\n	levels:    " << baseLevel.Get() << " - " << (baseLevel.Get() + levelCount)
			<< "\n	layers:    " << baseLayer.Get() << " - " << (baseLayer.Get() + layerCount)
			<< "\n	swizzle:   " << swizzle.ToString()
			<< "\n}\n";
		return str;
	})

}	// Platforms
}	// Engine
