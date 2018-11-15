// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/OS/OSEnums.h"
#include "Engine/Platforms/Public/OS/IDs.h"

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
	struct InputKey : _MsgBase_
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

		ND_ bool IsDown ()	const	{ return pressure > 0.0f; }
		ND_ bool IsUp ()	const	{ return not IsDown(); }
	};


	//
	// Input Motion Event
	//
	struct InputMotion : _MsgBase_
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
	struct InputKeyBind : _MsgBase_
	{
	// types
		using KeyID			= Platforms::KeyID;
		using EKeyState		= Platforms::EKeyState;
		using Callback_t	= Delegate< void (const InputKey &) >;

	// variables
		Callback_t			callback;
		KeyID::type			key			= KeyID::Unknown;
		EKeyState			state		= EKeyState::OnKeyDown;

	// methods
		InputKeyBind () {}

		template <typename Class>
		InputKeyBind (Class *mod, void (Class::*cb) (const InputKey &), KeyID::type key, EKeyState state) :
			callback(DelegateBuilder( WeakPointerType<Class>(mod), cb )), key(key), state(state)
		{}
	};

	struct InputKeyUnbindAll : _MsgBase_
	{
	// variables
		ModuleWPtr		object;

	// methods
		explicit InputKeyUnbindAll (const ModuleWPtr &obj) : object{obj} {}
	};


	//
	// Bind On Motion Event
	//
	struct InputMotionBind : _MsgBase_
	{
	// types
		using MotionID		= Platforms::MotionID;
		using Callback_t	= Delegate< void (const InputMotion &) >;

	// variables
		Callback_t			callback;
		MotionID::type		motion	= MotionID::Unknown;

	// methods
		InputMotionBind () {}

		template <typename Class>
		InputMotionBind (Class *mod, void (Class::*cb) (const InputMotion &), MotionID::type motion) :
			callback(DelegateBuilder( WeakPointerType<Class>(mod), cb )), motion(motion)
		{}
	};


	struct InputMotionUnbindAll : _MsgBase_
	{
	// variables
		ModuleWPtr		object;

	// methods
		explicit InputMotionUnbindAll (const ModuleWPtr &obj) : object{obj} {}
	};


}	// ModuleMsg
}	// Engine
