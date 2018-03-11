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
		static ModulePtr  CreateFreeCamera (GlobalSystemsRef, const CreateInfo::Camera &);
		static ModulePtr  CreateFreeVRCamera (GlobalSystemsRef, const CreateInfo::Camera &);

		static ModulePtr  CreateWindowSurface (GlobalSystemsRef, const CreateInfo::RenderSurface &);
		static ModulePtr  CreateVRSurface (GlobalSystemsRef, const CreateInfo::RenderSurface &);

		static ModulePtr  CreateSceneManager (GlobalSystemsRef, const CreateInfo::SceneManager &);
		static ModulePtr  CreateSceneMainThread (GlobalSystemsRef, const CreateInfo::SceneMain &);
		static ModulePtr  CreateSceneRenderer (GlobalSystemsRef, const CreateInfo::SceneRenderer &);
		static ModulePtr  CreateScenePhysics (GlobalSystemsRef, const CreateInfo::ScenePhysics &);
		static ModulePtr  CreateSceneLogic (GlobalSystemsRef, const CreateInfo::SceneLogic &);
	};


}	// Scene
}	// Engine
