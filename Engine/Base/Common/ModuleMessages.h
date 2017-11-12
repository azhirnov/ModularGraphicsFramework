// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Common/MessageHelpers.h"
#include "Engine/Base/Common/ThreadID.h"
#include "Engine/Base/Common/EModuleGroup.h"
#include "Engine/Base/Common/TypeIdList.h"

namespace Engine
{
namespace ModuleMsg
{
	using GModID		= Base::GModID;
	using TModID		= Base::TModID;
	using OModID		= Base::OModID;

	//using ThreadID	= Base::ThreadID;

	using ModulePtr		= Base::ModulePtr;

	using UntypedID_t	= CompileTime::NearUInt::FromType< GModID::type >;
	using ModuleName_t	= StaticString<64>;
	

	//
	// Attach / Detach Module from Parent
	//
	struct AttachModule
	{
	// variables
		ModuleName_t	name;
		ModulePtr		newModule;
		// TODO: deletable flag?

	// methods
		explicit AttachModule (const ModulePtr &unit) : AttachModule( StringCRef(), unit ) {}
		AttachModule (StringCRef name, const ModulePtr &unit) : name( /*name.Empty() ? ModuleName_t("id: ") << ToString(GModID::type( unit->GetModuleID() )) :*/ name ), newModule( unit ) {}
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
		ModulePtr		parent;
		ModuleName_t	name;
		ModulePtr		attachedModule;
	};

	struct OnModuleDetached
	{
		ModulePtr		parent;
		ModuleName_t	name;
		ModulePtr		detachedModule;
	};
	

	//
	// Link All Modules Message
	//
	struct Link
	{
	};


	//
	// Compose 
	//
	struct Compose
	{
		// this is the last chance to initialize module.

		//bool		makeImmutable	= false;	// now this flag ignored by all modules
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
		StringCRef				name;
		Out< ModulePtr >		result;
	};


	//
	// Find Module with Messages and Events
	//
	struct FindModuleByTypes
	{
		Base::TypeIdList		messages;
		Base::TypeIdList		events;
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
	// Search Modules with Messages and Events
	//
	struct ModulesDeepSearchByTypes
	{
		Base::TypeIdList			messages;
		Base::TypeIdList			events;
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
		ModulePtr	oldManager;		// unsubscribe from old manager events
		ModulePtr	newManager;		// subscribe to new manager events
	};


}	// ModuleMsg
}	// Engine
