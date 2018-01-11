// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
											ModuleMsg::OnManagerChanged
										> >;

		using SupportedEvents_t		= MessageListFrom<
											ModuleMsg::Compose,
											ModuleMsg::Delete,
											GpuMsg::DeviceBeforeDestroy
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
		
	protected:
		ModulePtr _GetGpuThread (const ModulePtr &gpuThread = null) const;
	};


}	// Graphics
}	// Engine
