// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Graphics/Impl/GraphicsObjectsConstructor.h"
#include "Engine/Graphics/Public/BatchRenderer.h"
#include "Engine/Graphics/Public/Font.h"
#include "Engine/Graphics/Public/Commands.h"

namespace Engine
{
namespace Graphics
{

/*
=================================================
	RegisterGraphics
=================================================
*/
	void RegisterGraphics ()
	{
		GraphicsObjectsConstructor::Register();
	}
	
/*
=================================================
	UnregisterGraphics
=================================================
*/
	void UnregisterGraphics ()
	{
		GraphicsObjectsConstructor::Unregister();
	}
	
/*
=================================================
	Register
=================================================
*/
	void GraphicsObjectsConstructor::Register ()
	{
		auto	mf = GetMainSystemInstance()->GlobalSystems()->modulesFactory;

		CHECK( mf->Register( FontModuleID, &CreateFont ) );
		CHECK( mf->Register( FontManagerModuleID, &CreateFontManager ) );
		CHECK( mf->Register( BatchRendererModuleID, &CreateBatchRenderer ) );
		CHECK( mf->Register( AsyncCommandBufferModuleID, &CreateAsyncCommandBuffer ) );
		CHECK( mf->Register( CommandBufferManagerModuleID, &CreateCommandBufferManager ) );
	}
	
/*
=================================================
	Unregister
=================================================
*/
	void GraphicsObjectsConstructor::Unregister ()
	{
		auto	mf = GetMainSystemInstance()->GlobalSystems()->modulesFactory;

		mf->UnregisterAll( FontModuleID );
		mf->UnregisterAll( FontManagerModuleID );
		mf->UnregisterAll( BatchRendererModuleID );
		mf->UnregisterAll( AsyncCommandBufferModuleID );
		mf->UnregisterAll( CommandBufferManagerModuleID );
	}

}	// Graphics
}	// Engine
