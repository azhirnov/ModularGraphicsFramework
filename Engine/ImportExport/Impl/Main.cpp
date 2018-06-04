// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/ImportExport/Impl/ImportExportObjectsConstructor.h"
#include "Engine/ImportExport/Public/Image.h"

namespace Engine
{
namespace ImportExport
{

/*
=================================================
	RegisterImportExport
=================================================
*/
	void RegisterImportExport ()
	{
		using IEOC = ImportExportObjectsConstructor;

		auto	mf = GetMainSystemInstance()->GlobalSystems()->modulesFactory;

		CHECK( mf->Register( GXImageLoaderModuleID, &IEOC::CreateGXImageLoader ) );
		//CHECK( mf->Register( GXImageSaverModuleID, &IEOC::CreateGXImageSaver ) );
	}
	
/*
=================================================
	UnregisterImportExport
=================================================
*/
	void UnregisterImportExport ()
	{
		auto	mf = GetMainSystemInstance()->GlobalSystems()->modulesFactory;
		
		mf->UnregisterAll( GXImageLoaderModuleID );
		mf->UnregisterAll( GXImageSaverModuleID );
	}

}	// ImportExport
}	// Engine
