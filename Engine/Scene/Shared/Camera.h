// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
		Scene::CameraSettings		settings;
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
		struct State : CompileTime::FastCopyable
		{
		// variables
			GXMath::Frustum<float>			frustum;
			GXMath::Transformation<float>	transform;
			GXMath::float4x4				view;
			GXMath::float4x4				projection;

		// methods
			State () {}

			State (const GXMath::Frustum<float> &frustum, const GXMath::Transformation<float> &transform, const GXMath::float4x4 &view, const GXMath::float4x4 &projection) :
				frustum{frustum}, transform{transform}, view{view}, projection{projection}
			{}
		};

		Out< State >	result;
	};

	struct StereoCameraGetState
	{
		struct State : CompileTime::FastCopyable
		{
			GXMath::Frustum<float>			leftFrustum;
			GXMath::Frustum<float>			rightFrustum;
			GXMath::Transformation<float>	transform;
			GXMath::float4x4				leftView;
			GXMath::float4x4				rightView;
			GXMath::float4x4				projection;
		};

		Out< State >	result;
	};


	//
	// Camera Request Update
	//
	struct CameraRequestUpdate
	{
	// types
		using Frustum_t			= FixedSizeArray< CameraGetState::State, SurfaceRequestUpdate::Framebuffers_t::MemoryContainer_t::SIZE >;
		using Framebuffers_t	= SurfaceRequestUpdate::Framebuffers_t const &;
		using CmdBuffers_t		= SurfaceRequestUpdate::CmdBuffers_t &;

	// variables
		Frustum_t		frustums;		// in
		Framebuffers_t	framebuffers;	// in

		CmdBuffers_t	cmdBuffers;		// out
	};


}	// SceneMsg
}	// Engine
