// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Graphics/Engine.Graphics.h"

using namespace Engine;
using namespace Engine::Base;
using namespace Engine::Platforms;
using namespace Engine::Graphics;


class GApp final : public StaticRefCountedObject
{
// types
private:
	using RenderPassMsgList_t = ModuleMsg::MessageListFrom< GpuMsg::GetRenderPassDescription >;


// variables
private:
	Ptr< Module >		ms;
	bool				looping		= true;
	GraphicsModuleIDs	ids;

	ModulePtr			batchRenderer;

	ModulePtr			gpipeline;
	ModulePtr			pipelineTemplate;
	ModulePtr			resourceTable;
	
	ModulePtr			texture;
	ModulePtr			sampler;

	ModulePtr			cmdBuilder;
	ModulePtr			asyncCmdBuilder;

	
// methods
public:
	GApp ();

	bool Initialize (GAPI::type api);
	void Quit ();
	bool Update ();
	
	bool _GraphicsInit (const GpuMsg::DeviceCreated &);
	bool _GraphicsDelete (const GpuMsg::DeviceBeforeDestroy &);
	bool _Draw (const ModuleMsg::Update &);
	bool _OnWindowClosed (const OSMsg::WindowAfterDestroy &);

	bool _CreatePipeline ();
};

