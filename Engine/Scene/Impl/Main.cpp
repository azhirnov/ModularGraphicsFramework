// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Scene/Impl/SceneObjectConstructor.h"
#include "Engine/Scene/Shared/IDs.h"
#include "Engine/Scene/Shared/Scene.h"
#include "Engine/Scene/Shared/Surface.h"
#include "Engine/Scene/Shared/Camera.h"

namespace Engine
{
namespace Scene
{

/*
=================================================
	RegisterScene
=================================================
*/
	void RegisterScene ()
	{
		using SOC = SceneObjectConstructor;

		auto	factory = GetMainSystemInstance()->GlobalSystems()->modulesFactory;

		factory->Register( FreeCameraModuleID, &SOC::CreateFreeCamera );

		factory->Register( WindowSurfaceModuleID, &SOC::CreateWindowSurface );

		factory->Register( SceneManagerModuleID, &SOC::CreateSceneManager );
		factory->Register( SceneModuleID, &SOC::CreateSceneMainThread );
		factory->Register( SceneRendererModuleID, &SOC::CreateSceneRenderer );
		factory->Register( ScenePhysicsModuleID, &SOC::CreateScenePhysics );
		factory->Register( SceneLogicModuleID, &SOC::CreateSceneLogic );
	}
	
/*
=================================================
	UnregisterScene
=================================================
*/
	void UnregisterScene ()
	{
		auto	factory = GetMainSystemInstance()->GlobalSystems()->modulesFactory;

		factory->UnregisterAll( FreeCameraModuleID );
		factory->UnregisterAll( WindowSurfaceModuleID );
		factory->UnregisterAll( SceneManagerModuleID );
		factory->UnregisterAll( SceneModuleID );
		factory->UnregisterAll( SceneRendererModuleID );
		factory->UnregisterAll( ScenePhysicsModuleID );
		factory->UnregisterAll( SceneLogicModuleID );
	}

}	// Scene
}	// Engine
