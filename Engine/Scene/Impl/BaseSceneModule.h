// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Scene/Public/IDs.h"

namespace Engine
{
namespace SceneMsg
{

	struct GetScenePrivateClasses : _MessageBase_
	{
		struct Classes {
		};

		Out< Classes >		result;
	};

}	// SceneMsg

namespace Scene
{

	//
	// Base Scene Module
	//

	class BaseSceneModule : public Module
	{
	// types
	protected:
		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Update
										> >
										::Append< MessageListFrom<
											ModuleMsg::OnManagerChanged,
											//GpuMsg::DeviceBeforeDestroy,
											SceneMsg::GetScenePrivateClasses
										> >;

		using SupportedEvents_t		= MessageListFrom<
											ModuleMsg::Compose,
											ModuleMsg::Delete,
											ModuleMsg::OnModuleAttached,
											ModuleMsg::OnModuleDetached
										>;
	

	// variables
	private:
		// TODO


	// methods
	public:
		BaseSceneModule (const GlobalSystemsRef gs,
						 const ModuleConfig &config,
						 const TypeIdList *msgTypes,
						 const TypeIdList *eventTypes);
		

	// message handlers
	protected:
		bool _OnManagerChanged (const ModuleMsg::OnManagerChanged &);
		//bool _OnManagerDestroyed (const ModuleMsg::Delete &);
		bool _GetScenePrivateClasses (const SceneMsg::GetScenePrivateClasses &);
	};


}	// Scene
}	// Engine
