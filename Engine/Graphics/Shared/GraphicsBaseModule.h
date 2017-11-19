// Copyright �  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Graphics/Shared/IDs.h"

namespace Engine
{
namespace Graphics
{

	//
	// Graphics Base Module
	//

	class GraphicsBaseModule : public Module
	{
	// types
	protected:
		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Update
										> >
										::Append< MessageListFrom<
											ModuleMsg::OnManagerChanged,
											GpuMsg::DeviceBeforeDestroy
										> >;

		using SupportedEvents_t		= MessageListFrom<
											ModuleMsg::Compose,
											ModuleMsg::Delete
										>;

	// methods
	public:
		GraphicsBaseModule (const GlobalSystemsRef gs,
							const ModuleConfig &config,
							const TypeIdList *msgTypes,
							const TypeIdList *eventTypes);

	// message handlers
	protected:
		bool _OnManagerChanged (const Message< ModuleMsg::OnManagerChanged > &);
		bool _DeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &);
	};


}	// Graphics
}	// Engine
