// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Modules/MessageHelpers.h"
#include "Engine/Base/Common/ThreadID.h"
#include "Engine/Base/Common/EModuleGroup.h"

namespace Engine
{
namespace _BaseMessages_
{
	using GModID		= Base::GModID;
	using TModID		= Base::TModID;
	using OModID		= Base::OModID;

	//using ThreadID	= Base::ThreadID;

	using ModulePtr		= Base::ModulePtr;
	using ModuleWPtr	= Base::ModuleWPtr;

	using UntypedID_t	= CompileTime::NearUInt::FromType< GModID::type >;
	using ModuleName_t	= StaticString<64>;
	
	template <typename ...Types>
	using MessageListFrom	= CompileTime::TypeListFrom< Types... >;
	

	using RectI			= GX_STL::GXMath::RectI;
	using RectU			= GX_STL::GXMath::RectU;
	using RectF			= GX_STL::GXMath::RectF;

	using float2		= GX_STL::GXMath::float2;
	using float3		= GX_STL::GXMath::float3;
	using float4		= GX_STL::GXMath::float4;
	using float2x2		= GX_STL::GXMath::float2x2;
	using float3x3		= GX_STL::GXMath::float3x3;
	using float4x4		= GX_STL::GXMath::float4x4;
	
	using int2			= GX_STL::GXMath::int2;
	using int3			= GX_STL::GXMath::int3;
	using int4			= GX_STL::GXMath::int4;
	
	using uint2			= GX_STL::GXMath::uint2;
	using uint3			= GX_STL::GXMath::uint3;
	using uint4			= GX_STL::GXMath::uint4;
	
	using ilong2		= GX_STL::GXMath::ilong2;
	using ilong3		= GX_STL::GXMath::ilong3;
	using ilong4		= GX_STL::GXMath::ilong4;
	
	using ulong2		= GX_STL::GXMath::ulong2;
	using ulong3		= GX_STL::GXMath::ulong3;
	using ulong4		= GX_STL::GXMath::ulong4;
	
	using double2		= GX_STL::GXMath::double2;
	using double3		= GX_STL::GXMath::double3;
	using double4		= GX_STL::GXMath::double4;

	using color4u		= GX_STL::GXMath::color4u;
	using color4f		= GX_STL::GXMath::color4f;
	

	struct _MsgBase_ : CompileTime::FastCopyable
	{
		using __is_message	= bool;
	};

}	// _BaseMessages_


namespace ModuleMsg
{
	using namespace _BaseMessages_;


	//
	// Attach / Detach Module from Parent
	//
	struct AttachModule : _MsgBase_
	{
	// variables
		ModuleName_t	name;		// can be empty
		ModulePtr		newModule;
		
	// methods
		explicit AttachModule (const ModulePtr &unit) : AttachModule( StringCRef(), unit ) {}
		AttachModule (StringCRef name, const ModulePtr &unit) : name( name ), newModule( unit ) {}
	};
	
	struct DetachModule : _MsgBase_
	{
	// variables
		ModulePtr	oldModule;
		
	// methods
		explicit DetachModule (const ModulePtr &unit) : oldModule{unit} {}
	};

	struct ReplaceModule : _MsgBase_
	{
	// variables
		ModuleName_t	name;		// can be empty
		ModulePtr		oldModule;	// can be null
		ModulePtr		newModule;
		
	// methods
		ReplaceModule (StringCRef name, const ModulePtr &newModule) : name{name}, newModule{newModule} {}
		ReplaceModule (const ModulePtr &oldModule, const ModulePtr &newModule) : oldModule{oldModule}, newModule{newModule} {}
	};
	

	//
	// On Module Attached / Detached from Parent
	//
	struct OnModuleAttached : _MsgBase_
	{
	// variables
		ModulePtr		parent;
		ModuleName_t	name;
		ModulePtr		attachedModule;
		
	// methods
		OnModuleAttached (const ModulePtr &	parent,
						  StringCRef		name,
						  const ModulePtr &	attachedModule) :
			parent{parent}, name{name},
			attachedModule{attachedModule}
		{}
	};

	struct OnModuleDetached : _MsgBase_
	{
	// variables
		ModulePtr		parent;
		ModuleName_t	name;
		ModulePtr		detachedModule;
		bool			isLast;				// if parent module supports attachments with same objects and different names,
											// then this flag indicates that 'detachedModule' is last module and 'detachedModule' must delete 'parent' from parents.
		
	// methods
		OnModuleDetached (const ModulePtr &	parent,
						  StringCRef		name,
						  const ModulePtr &	detachedModule,
						  bool				isLast = true) :
			parent{parent},					name{name},
			detachedModule{detachedModule},	isLast{isLast}
		{}
	};
	

	//
	// Link All Modules Message
	//
	struct Link : _MsgBase_
	{
	};

	// event
	struct AfterLink : _MsgBase_
	{};


	//
	// Compose 
	//
	struct Compose : _MsgBase_
	{
		// this is the last chance to initialize module.

		//bool		makeImmutable	= false;	// at now this flag ignored by all modules
	};

	// event
	struct AfterCompose : _MsgBase_
	{};

	
	//
	// Update Module Message
	//
	struct Update : _MsgBase_
	{
	// variables
		TimeL		timeDelta;
		
	// methods
		Update () {}
		explicit Update (TimeL dt) : timeDelta{dt} {}
	};


	//
	// Find Module
	//
	struct FindModule : _MsgBase_
	{
	// variables
		StringCRef				name;
		Out< ModulePtr >		result;
		
	// methods
		explicit FindModule (StringCRef name) : name{name} {}
	};


	//
	// Find Module with Messages and Events
	//
	struct FindModuleByTypes : _MsgBase_
	{
	// variables
		Runtime::TypeIdList		messages;
		Runtime::TypeIdList		events;
		Out< ModulePtr >		result;
		
	// methods
		FindModuleByTypes (Runtime::TypeIdList	&&messages,
						   Runtime::TypeIdList	&&events) :
			messages{RVREF(messages)}, events{RVREF(events)}
		{}
	};

	
	//
	// Search Modules
	//
	struct ModulesDeepSearch : _MsgBase_
	{
	// variables
		ArrayCRef<UntypedID_t>		ids;
		uint						upDeep;
		uint						downDeep;
		mutable Array< ModulePtr >*	result;
		
	// methods
		ModulesDeepSearch (ArrayCRef<UntypedID_t> ids,
							uint				  upDeep,
							uint				  downDeep) :
			ids{ids}, upDeep{upDeep}, downDeep{downDeep}
		{}
	};

	
	//
	// Search Modules with Messages and Events
	//
	struct ModulesDeepSearchByTypes : _MsgBase_
	{
	// variables
		Runtime::TypeIdList			messages;
		Runtime::TypeIdList			events;
		uint						upDeep;
		uint						downDeep;
		mutable Array< ModulePtr >*	result;
		
	// methods
		ModulesDeepSearchByTypes (Runtime::TypeIdList	&&messages,
								  Runtime::TypeIdList	&&events,
								  uint					upDeep,
								  uint					downDeep) :
			messages{RVREF(messages)}, events{RVREF(events)},
			upDeep{upDeep}, downDeep{downDeep}
		{}
	};
	

	//
	// Delete Module Message
	//
	struct Delete : _MsgBase_
	{
	};

	
	//
	// Push Async Message
	//
	struct PushAsyncMessage;


	//
	// Add / Remove Module from Manager
	//
	struct AddToManager : _MsgBase_
	{
	// variables
		ModulePtr	module;
		
	// methods
		explicit AddToManager (const ModulePtr &module) : module{module} {}
	};

	struct RemoveFromManager : _MsgBase_
	{
	// variables
		ModuleWPtr	module;
		
	// methods
		explicit RemoveFromManager (const ModuleWPtr &module) : module{module} {}
	};


	//
	// On Module Manager Changed
	//
	struct OnManagerChanged : _MsgBase_
	{
	// variables
		ModulePtr	oldManager;		// unsubscribe from old manager events
		ModulePtr	newManager;		// subscribe to new manager events
		
	// methods
		OnManagerChanged (const ModulePtr &oldManager, const ModulePtr &newManager) :
			oldManager{oldManager}, newManager{newManager}
		{}
	};


}	// ModuleMsg
}	// Engine
