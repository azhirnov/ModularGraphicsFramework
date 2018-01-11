// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Scene/Shared/IDs.h"
#include "Engine/Scene/Shared/Surface.h"

namespace Engine
{
namespace Scene
{

	//
	// Camera Settings
	//
	struct CameraSettings
	{
		float2		mouseSens		= float2{ 1.0f, 1.0f };	// for mouse or touch
		float		keyRotationSens	= 1.0f;					// 

		float		velocity		= 1.0f;					// m/s

		float2		zoomLimits		= float2{ 1.0f, 2.0f };
		float		zoomStep		= 1.0f;

		Rad			fovY;
		float		aspect			= 1.0f;					// width / height
	};


	DECL_STRING_TO_ID( ActionID, 0 );


}	// Scene


namespace CreateInfo
{

	//
	// Camera Create Info
	//
	struct Camera
	{
		ModulePtr				scene;

		Scene::CameraSettings	settings;

		// TODO: key bindings
	};

}	// CreateInfo


namespace SceneMsg
{

	//
	// Bind Keys to Actions
	//
	struct CameraBindKeys
	{
	};


	//
	// Set / Get settings
	//
	struct CameraUpdateSettings
	{
		Scene::CameraSettings			settings;
	};

	struct CameraGetSettings
	{
		Out< Scene::CameraSettings >	result;
	};


	//
	// Get Current State
	//
	struct CameraGetState
	{
	// types
		using Frustum_t		= GXMath::Frustum<float>;
		using Transform_t	= GXMath::Transformation<float>;
		using Mat4x4_t		= GXMath::float4x4;

		struct State : CompileTime::FastCopyable
		{
		// variables
			Frustum_t		frustum;
			Transform_t		transform;
			Mat4x4_t		viewMat;
			Mat4x4_t		projMat;

		// methods
			State () {}

			State (const Frustum_t &frustum, const Transform_t &transform, const Mat4x4_t &view, const Mat4x4_t &proj) :
				frustum{frustum}, transform{transform}, viewMat{view}, projMat{proj}
			{}
		};

		using States_t = FixedSizeArray< State, 2 >;


	// variables
		Out< States_t >	result;
	};


	//
	// Camera Request Update
	//
	struct CameraRequestUpdate
	{
	// types
		using Cameras_t			= CameraGetState::States_t;
		using Framebuffers_t	= SurfaceRequestUpdate::Framebuffers_t const &;

		STATIC_ASSERT( Cameras_t::MemoryContainer_t::SIZE == SurfaceRequestUpdate::Framebuffers_t::MemoryContainer_t::SIZE );


	// variables
		Cameras_t		cameras;
		Framebuffers_t	framebuffers;
		ModulePtr		cmdBuilder;
	};


}	// SceneMsg
}	// Engine
