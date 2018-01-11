// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Modules/ModuleMessages.h"
#include "Engine/Base/Modules/Message.h"

namespace Engine
{
namespace ProfilingMsg
{
	using namespace ModuleMsg;


	//
	// On Send Message
	//
	struct OnSendMsg
	{
	// variables
		ModulePtr							target;
		WeakPointerType< Base::BaseObject >	sender;
		VariantCRef							msg;

	// methods
		template <typename T>
		OnSendMsg (const ModulePtr &target, const Base::Message<T> &msg) :
			target{target}, sender{msg.Sender()}, msg{ VariantCRef::FromConst(msg) }
		{}
	};

}	// ProfilingMsg
}	// Engine
