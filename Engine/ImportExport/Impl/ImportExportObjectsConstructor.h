// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/ImportExport/Public/IDs.h"

namespace Engine
{
namespace CreateInfo
{

	struct ImageLoader;
	struct ImageSaver;

}	// CreateInfo


namespace ImportExport
{
	
	//
	// Import-Export Objects Constructor
	//

	class ImportExportObjectsConstructor final : public Noninstancable
	{
	public:
		static ModulePtr CreateGXImageLoader (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::ImageLoader &);
		static ModulePtr CreateGXImageSaver (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::ImageSaver &);
	};


}	// ImportExport
}	// Engine
