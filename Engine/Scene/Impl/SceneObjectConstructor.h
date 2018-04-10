// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Scene/Public/Common.h"
#include "Engine/Profilers/Engine.Profilers.h"

namespace Engine
{
namespace CreateInfo
{
	struct Camera;
	struct RenderSurface;
	struct SceneManager;
	struct SceneMain;
	struct SceneRenderer;
	struct ScenePhysics;
	struct SceneLogic;

}	// CreateInfo

namespace Scene
{

	//
	// Scene Object Constructor
	//

	class SceneObjectConstructor final : public Noninstancable
	{
	// methods
	public:
		static ModulePtr  CreateFreeCamera (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::Camera &);
		static ModulePtr  CreateFreeVRCamera (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::Camera &);

		static ModulePtr  CreateWindowSurface (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::RenderSurface &);
		static ModulePtr  CreateVRSurface (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::RenderSurface &);

		static ModulePtr  CreateSceneManager (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::SceneManager &);
		static ModulePtr  CreateSceneMainThread (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::SceneMain &);
		static ModulePtr  CreateSceneRenderer (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::SceneRenderer &);
		static ModulePtr  CreateScenePhysics (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::ScenePhysics &);
		static ModulePtr  CreateSceneLogic (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::SceneLogic &);
	};


}	// Scene
}	// Engine
