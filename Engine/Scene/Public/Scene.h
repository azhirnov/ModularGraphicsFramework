// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Scene/Public/IDs.h"

namespace Engine
{
namespace Scene
{


}	// Scene


namespace CreateInfo
{
	
	//
	// Scene Manager Create Info
	//
	struct SceneManager
	{
	// types
		struct VRSettings
		{
			GXMath::uint2		eyeTextureDimension;		// dimension of per eye textures
			bool				layered		= false;		// create texture array or use multiview extension, depended of implementation
			bool				enabled		= false;
		};

	// variables
		Platforms::GraphicsSettings		settings;		// this is default settings for all threads
		VRSettings						vrSettings;

	// methods
		SceneManager () {}
	};


	//
	// Scene Main Thread Create Info
	//
	struct SceneMain
	{
	};


	//
	// Scene Renderer Main Thread Create Info
	//
	struct SceneRenderer
	{
	};


	//
	// Scene Physics Main Thread Create Info
	//
	struct ScenePhysics
	{
	};


	//
	// Scene Logic (AI, Gameplay) Main Thread Create Info
	//
	struct SceneLogic
	{
	};

}	// CreateInfo


namespace SceneMsg
{
	

}	// SceneMsg
}	// Engine
