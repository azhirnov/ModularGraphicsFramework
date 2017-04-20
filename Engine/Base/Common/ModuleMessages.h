// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/Base/Common/MessageHelpers.h"
#include "Engine/Base/Common/ThreadID.h"
#include "Engine/Base/Common/EModuleGroup.h"

namespace Engine
{
namespace ModuleMsg
{
	using UntypedID_t	= CompileTime::NearUInt::FromType< GModID::type >;
	

	//
	// Attach / Detach Module Message
	//
	struct AttachModule
	{
		ModulePtr	newModule;
	};
	
	struct DetachModule
	{
		ModulePtr	oldModule;
	};
	

	//
	// On Module Attached / Detached Message
	//
	struct OnModuleAttached
	{
		ModulePtr	parent;
		ModulePtr	attachedModule;
	};

	struct OnModuleDetached
	{
		ModulePtr	parent;
		ModulePtr	detachedModule;
	};
	

	//
	// Link All Modules Message
	//
	struct Link
	{
		// find modules and subscribe to events
		ArrayCRef< Pair<UntypedID_t, ModulePtr> >		modules;
	};


	//
	// Compose 
	//
	struct Compose
	{
		// this is the last chance to initialize module.
	};

	
	//
	// Update Module Message
	//
	struct Update
	{
		TimeD	timeDelta;
		// TODO: add thread priority
	};


	//
	// Find Module
	//
	struct FindModule
	{
		UntypedID_t				id;
		Out< ModulePtr >		result;
	};
	

	//
	// On System Registered / Unregistered in Modules Factory
	//
	struct OnRegistered
	{
		Ptr< ModulesFactory >	factory;
	};

	struct OnUnregistered
	{
		Ptr< ModulesFactory >	factory;
	};
	

	//
	// Delete Module Message
	//
	struct Delete
	{
		// TODO: add flags
	};

	
	//
	// Find Thread Request
	//
	/*struct FindThread
	{
		ThreadID			id;
		Out< ModulePtr >	result;
	};*/

	
	//
	// Push Async Message
	//
	struct PushAsyncMessage;


	//
	// Add / Remove Module from Manager
	//
	struct AddToManager
	{
		ModulePtr	module;
	};

	struct RemoveFromManager
	{
		ModulePtr	module;
	};


}	// ModuleMsg
}	// Engine
