// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Graphics/Public/IDs.h"

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
		using SupportedEvents_t		= MessageListFrom<
											ModuleMsg::Compose,
											ModuleMsg::Delete,
											GpuMsg::DeviceBeforeDestroy
										>;

	// methods
	public:
		GraphicsBaseModule (const GlobalSystemsRef gs,
							const ModuleConfig &config,
							const TypeIdList *eventTypes);

	// message handlers
	protected:
		bool _OnManagerChanged (const ModuleMsg::OnManagerChanged &);
		bool _DeviceBeforeDestroy (const GpuMsg::DeviceBeforeDestroy &);
		
	protected:
		ND_ ModulePtr  _GetGpuThread (const ModulePtr &gpuThread = null) const;
	};


}	// Graphics
}	// Engine
