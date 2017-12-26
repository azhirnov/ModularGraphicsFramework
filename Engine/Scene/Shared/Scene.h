// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Scene/Shared/IDs.h"

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
	// variables
		Platforms::GraphicsSettings		settings;	// this is default settings for all threads

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
