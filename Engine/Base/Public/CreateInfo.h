// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Modules/MessageHelpers.h"
#include "Engine/Base/Public/ModuleMessages.h"
#include "Engine/Base/Common/Enums.h"
#include "Engine/Base/Common/EngineSubSystems.h"

namespace Engine
{
namespace CreateInfo
{
	template <typename T>	using ReadOnce	= ModuleMsg::ReadOnce<T>;

	using ModulePtr		= Engine::Base::ModulePtr;
	using UntypedID_t	= Engine::ModuleMsg::UntypedID_t;


}	// CreateInfo
}	// Engine
