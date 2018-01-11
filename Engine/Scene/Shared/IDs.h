// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Scene/Shared/Common.h"

namespace Engine
{
namespace Scene
{


// Scene //

	static constexpr GModID::type  SceneManagerModuleID			= "scene.mngr"_GModID;
	static constexpr TModID::type  SceneModuleID				= "scene.main"_TModID;
	static constexpr TModID::type  SceneRendererModuleID		= "s.renderer"_TModID;
	static constexpr TModID::type  ScenePhysicsModuleID			= "s.physics"_TModID;
	static constexpr TModID::type  SceneLogicModuleID			= "s.logic"_TModID;


// Renderer Objects //

	// Surface
	static constexpr OModID::type  WindowSurfaceModuleID		= "s.wndsurf"_OModID;
	static constexpr OModID::type  VRSurfaceModuleID			= "s.vrsurf"_OModID;

	// Technique
	static constexpr OModID::type  ForwardRendererTechModuleID	= "s.fr-tech"_OModID;
	static constexpr OModID::type  DeferredRendererTechModuleID	= "s.dr-tech"_OModID;
	//static constexpr OModID::type  RayTracingTechModuleID		= "s.ray-tech"_OModID;

	// Model

	// Terrain
	static constexpr OModID::type  TerrainModuleID				= "s.terrain"_OModID;		// clipmaps terrain
	static constexpr OModID::type  VoxelTerrainModuleID			= "s.vox-terr"_OModID;		// complex terrain

	// Particles
	static constexpr OModID::type  ParticleEmitterModuleID		= "s.pt-emit"_OModID;
	static constexpr OModID::type  ParticleManagerModuleID		= "s.pt-mngr"_OModID;



// Scene Objects //

	// Camera
	static constexpr OModID::type  FreeCameraModuleID			= "s.free-cam"_OModID;
	static constexpr OModID::type  FreeVRCameraModuleID			= "s.free-vrcam"_OModID;
	
	static constexpr OModID::type  FirstPersonCameraModuleID	= "s.fp-cam"_OModID;
	static constexpr OModID::type  FirstPersonVRCameraModuleID	= "s.fp-vrcam"_OModID;
	
	static constexpr OModID::type  ThirdPersonCameraModuleID	= "s.3p-cam"_OModID;
	static constexpr OModID::type  ThirdPersonVRCameraModuleID	= "s.3p-vrcam"_OModID;

	// World Manager
	static constexpr OModID::type  WorldManagerModuleID			= "s.wrld.mngr"_OModID;		// terrain + enviroment


}	// Scene
}	// Engine
