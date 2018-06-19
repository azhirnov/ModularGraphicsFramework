// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "../Common.h"


class GApp final : public StaticRefCountedObject
{
// types
private:
	using RenderPassMsgList_t = CompileTime::TypeListFrom< GpuMsg::GetRenderPassDescription >;


// variables
private:
	Ptr< Module >		ms;
	bool				looping		= true;
	GraphicsModuleIDs	gpuIDs;
	OSModuleIDs			osIDs;

	ModulePtr			syncManager;

	// triangle
	ModulePtr			gpipeline1;
	ModulePtr			pipelineTemplate1;
	ModulePtr			resourceTable1;

	// quad
	ModulePtr			gpipeline2;
	ModulePtr			pipelineTemplate2;
	ModulePtr			resourceTable2;

	ModulePtr			framebuffer;
	ModulePtr			fbColorImage;

	ModulePtr			texture;
	ModulePtr			sampler;

	ModulePtr			vbuffer;
	ModulePtr			ibuffer;
	ModulePtr			ubuffer;

	uint				cmdBufIndex	= 0;
	Array<ModulePtr>	cmdBuffers;
	Array<GpuFenceId>	cmdFence;
	ModulePtr			cmdBuilder;
	bool				firstCall	= true;


// methods
public:
	GApp ();
	bool Initialize (GAPI::type api);
	void Quit ();
	bool Update ();


private:
	bool _OnWindowClosed (const OSMsg::WindowAfterDestroy &);
	bool _OnKey (const ModuleMsg::InputKey &);
	bool _OnMotion (const ModuleMsg::InputMotion &);
	bool _Draw (const ModuleMsg::Update &);
	bool _GInit (const GpuMsg::DeviceCreated &);
	bool _GDelete (const GpuMsg::DeviceBeforeDestroy &);
	
	bool _CreatePipeline1 ();
	bool _CreatePipeline2 ();
	bool _CreateCmdBuffers ();
};
