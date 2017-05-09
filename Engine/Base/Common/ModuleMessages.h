// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
	// Attach / Detach Module from Parent
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
	// On Module Attached / Detached from Parent
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
		//ArrayCRef< Pair<UntypedID_t, ModulePtr> >		modules;	// TODO: don't use it, use parent->GetModule
	};


	//
	// Compose 
	//
	struct Compose
	{
		// this is the last chance to initialize module.

		bool		makeImmutable	= false;
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
	// Search Modules
	//
	struct ModulesDeepSearch
	{
		ArrayCRef<UntypedID_t>		ids;
		uint						upDeep;
		uint						downDeep;
		mutable Array< ModulePtr >*	result;
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


	//
	// On Module Manager Changed
	//
	struct OnManagerChanged
	{
		ModulePtr	newManager;
	};


}	// ModuleMsg
}	// Engine
