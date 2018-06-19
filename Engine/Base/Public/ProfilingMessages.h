// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Public/ModuleMessages.h"
#include "Engine/Base/Common/BaseObject.h"

namespace Engine
{
namespace ProfilingMsg
{
	using namespace Engine::_BaseMessages_;


	//
	// On Send Message
	//
	struct OnSendMsg : _MessageBase_
	{
	// variables
		ModulePtr							target;
		WeakPointerType< Base::BaseObject >	sender;
		VariantCRef							msg;

	// methods
		template <typename T>
		OnSendMsg (const ModulePtr &target, const T &msg) :
			target{target}, sender{msg.Sender()}, msg{ VariantCRef::FromConst(msg) }
		{}
	};

}	// ProfilingMsg
}	// Engine
