// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/OS/OSEnums.h"
#include "Engine/Platforms/Shared/OS/IDs.h"

namespace Engine
{
namespace CreateInfo
{

	//
	// Input Manager Create Info
	//

	struct InputManager
	{
	};


	//
	// Input Thread Create Info
	//

	struct InputThread
	{
	};


	//
	// Raw Input Handler Create Info
	//

	struct RawInputHandler
	{
	};


}	// CreateInfo


namespace Platforms
{
	DECL_STRING_TO_ID( KeyID, 8 );
	DECL_STRING_TO_ID( MotionID, 9 );
	
	enum class EKeyState
	{
		OnKeyDown,		// single event when key down
		OnKeyUp,		// signle event when key up
		OnKeyPressed,	// continiously event until key is pressed
	};

}	// Platforms


namespace ModuleMsg
{

	//
	// Input Key / Button Event
	//
	struct InputKey
	{
	// types
		using KeyID = Platforms::KeyID;

	// variables
		KeyID::type	key			= KeyID::Unknown;
		float		pressure	= 0.0f;

	// methods
		InputKey (KeyID::type key, float pressure) :
			key(key), pressure(pressure)
		{}

		bool IsDown ()	const	{ return pressure > 0.0f; }
		bool IsUp ()	const	{ return not IsDown(); }
	};


	//
	// Input Motion Event
	//
	struct InputMotion
	{
	// types
		using MotionID = Platforms::MotionID;

	// variables
		MotionID::type	motion		= MotionID::Unknown;
		float			relative	= 0.0f;
		float			absolute	= 0.0f;

	// methods
		InputMotion (MotionID::type motion, float relative, float absolute) :
			motion(motion), relative(relative), absolute(absolute)
		{}
	};


	//
	// Bind On Key Event
	//
	struct InputKeyBind
	{
	// types
		using KeyID			= Platforms::KeyID;
		using EKeyState		= Platforms::EKeyState;
		using Callback_t	= Delegate< void (const InputKey &) >;

	// variables
		SingleRead< Callback_t >	cb;
		KeyID::type					key		= KeyID::Unknown;
		EKeyState					state	= EKeyState::OnKeyDown;

	// methods
		InputKeyBind (Callback_t &&cb, KeyID::type key, EKeyState state) :
			cb( RVREF(cb) ), key(key), state(state)
		{}
	};


	//
	// Bind On Motion Event
	//
	struct InputMotionBind
	{
	// types
		using MotionID		= Platforms::MotionID;
		using Callback_t	= Delegate< void (const InputMotion &) >;

	// variables
		SingleRead< Callback_t >	cb;
		MotionID::type				motion	= MotionID::Unknown;

	// methods
		InputMotionBind (Callback_t &&cb, MotionID::type motion) :
			cb( RVREF(cb) ), motion(motion)
		{}
	};


}	// ModuleMsg
}	// Engine
