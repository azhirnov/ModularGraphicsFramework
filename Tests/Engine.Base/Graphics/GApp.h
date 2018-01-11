// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "../Common.h"


class GApp final : public StaticRefCountedObject
{
// types
private:
	using RenderPassMsgList_t = CompileTime::TypeListFrom< Message<GpuMsg::GetRenderPassDescriptor> >;


// variables
public:
	Ptr< Module >		ms;

private:
	bool				looping		= true;
	uint				cmdBufIndex	= 0;
	GraphicsModuleIDs	ids;

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

	Array<ModulePtr>	cmdBuffers;
	Array<GpuFenceId>	cmdFence;
	ModulePtr			cmdBuilder;


// methods
public:
	GApp ();
	bool Initialize (GAPI::type api);
	void Quit ();
	bool Update ();


private:
	bool _OnWindowClosed (const Message<OSMsg::WindowAfterDestroy> &);
	bool _OnKey (const Message< ModuleMsg::InputKey > &);
	bool _OnMotion (const Message< ModuleMsg::InputMotion > &);
	bool _Draw (const Message< ModuleMsg::Update > &);
	bool _GInit (const Message< GpuMsg::DeviceCreated > &);
	bool _GDelete (const Message< GpuMsg::DeviceBeforeDestroy > &);
	
	bool _CreatePipeline1 ();
	bool _CreatePipeline2 ();
	bool _CreateCmdBuffers ();
};
