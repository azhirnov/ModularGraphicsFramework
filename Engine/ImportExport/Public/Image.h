// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/ImportExport/Public/IDs.h"

namespace Engine
{
namespace CreateInfo
{

	//
	// Image Loader Create Info
	//
	struct ImageLoader
	{
	// types
		using EImageLayout	= Platforms::EImageLayout;

	// variables
		ModulePtr			dataInput;		// random access data input module
		EImageLayout::type	imageLayout;	// image layout after loading

	// methods
		explicit ImageLoader (const ModulePtr &dataInput, EImageLayout::type layout = EImageLayout::ShaderReadOnlyOptimal) : dataInput{dataInput}, imageLayout{layout} {}
	};


	//
	// Image Saver Create Info
	//
	struct ImageSaver
	{
	// types
		using EImageLayout	= Platforms::EImageLayout;

	// variables
		ModulePtr			asyncThread;	// can be null, used to asynchroniusly save image
		ModulePtr			dataOutput;		// random access data output module
		EImageLayout::type	imageLayout;	// image layout after saving

	// methods
		ImageSaver (const ModulePtr &dataOutput, EImageLayout::type layout, const ModulePtr &asyncThread = null) :
			asyncThread{asyncThread}, dataOutput{dataOutput}, imageLayout{layout} {}
	};

}	// CreateInfo
}	// Engine